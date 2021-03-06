{-# LANGUAGE ExistentialQuantification,FlexibleContexts #-}
module SMTPool where

import Language.SMTLib2
import Language.SMTLib2.Connection

import Data.Pool
import Control.Exception

data SMTInstance info a = forall b. SMTBackend b IO =>
                          SMTInstance { instanceConn :: SMTConnection b
                                      , instanceVars :: a
                                      , instanceInfo :: info
                                      }

type SMTPool info a = Pool (SMTInstance info a)

createSMTPool :: SMTBackend b IO
                 => IO b
                 -> SMT a
                 -> IO (SMTPool () a)
createSMTPool backend act = createSMTPool' backend () act

createSMTPool' :: SMTBackend b IO
                 => IO b
                 -> info
                 -> SMT a
                 -> IO (SMTPool info a)
createSMTPool' createBackend info act
  = createPool (do
                   b <- createBackend
                   conn <- open b
                   vars <- performSMTExitCleanly conn act
                   return $ SMTInstance conn vars info)
    (\(SMTInstance { instanceConn = conn }) -> close conn)
    1 5 10

withSMTPool :: SMTPool info a -> (a -> SMT b) -> IO b
withSMTPool pool act = do
  Right res <- withSMTPool' pool (\info vars -> do
                                     res <- act vars
                                     return (Right (res,info)))
  return res

withSMTPool' :: SMTPool info a -> (info -> a -> SMT (Either c (b,info))) -> IO (Either c b)
withSMTPool' pool act = do
  (inst@SMTInstance { instanceConn = conn
                    , instanceVars = vars
                    , instanceInfo = info },local) <- takeResource pool
  (do
      res <- performSMTExitCleanly conn
             (act info vars)
      case res of
       Left x -> do
         destroyResource pool local inst
         return (Left x)
       Right (res,info') -> do
         putResource local (SMTInstance { instanceConn = conn
                                        , instanceVars = vars
                                        , instanceInfo = info' })
         return (Right res))
    `onException` (destroyResource pool local inst)
