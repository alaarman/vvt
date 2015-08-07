module Main where

import Paths_hctigar

import System.Process
import System.IO
import System.Environment
import System.FilePath
import System.Console.GetOpt

data Action = Verify FilePath
            | Encode FilePath
            | ShowLLVM FilePath

data Options = Options { karrAnalysis :: Bool
                       , showHelp :: Bool
                       , defines :: [String]
                       }

defaultOptions :: Options
defaultOptions = Options { karrAnalysis = False
                         , showHelp = False
                         , defines = [] }

optDescr :: [OptDescr (Options -> Options)]
optDescr = [Option ['h'] ["help"] (NoArg $ \opt -> opt { showHelp = True }) "Show this help"
           ,Option ['k'] ["karr"] (NoArg $ \opt -> opt { karrAnalysis = True }) "Use Karr analysis to get better predicates"
           ,Option ['D'] [] (ReqArg (\arg opt -> opt { defines = arg:defines opt }) "VAR[=VAL]") "Define macros for the C-preprocessor"]

getAction :: IO (Maybe (Action,Options))
getAction = do
  args <- getArgs
  let (xs,extra,errs) = getOpt Permute optDescr args
      opts = foldl (flip id) defaultOptions xs
  if showHelp opts
    then do
    putStrLn $ usageInfo "Usage:\n\n    vvt ACTION [OPTIONS] [FILE..]\n\nAvailable actions:\n  encode - Create a transition relation from a C file.\n  show-llvm - Show the LLVM code that is used for the translation.\n\nAvailable options:" optDescr
    return Nothing
    else do
    act <- case extra of
            [] -> error "Please provide an action."
            "verify":rest -> case rest of
              [] -> error "Please provide a C-file to verify."
              [file] -> return (Encode file)
            "encode":rest -> case rest of
              [] -> error "Please provide a C-file to encode."
              [file] -> return (Encode file)
            "show-llvm":rest -> case rest of
              [] -> error "Please provide a C-file to compile."
              [file] -> return (ShowLLVM file)
    return (Just (act,opts))

performAction :: (Action,Options) -> IO ()
performAction (Encode fn,opts) = do
  outp <- openFile (replaceExtension fn "l") WriteMode
  (inp,_) <- compile fn (defines opts)
  ph <- execPipe inp outp [progOptimize
                          ,progEncode
                          ,progPredicates (karrAnalysis opts)
                          ,progPretty]
  waitForProcess ph
  return ()
performAction (ShowLLVM fn,opts) = do
  (inp,_) <- compile fn (defines opts)
  ph <- execPipe inp stdout [progOptimize,progDisassemble]
  waitForProcess ph
  return ()  

main :: IO ()
main = do
  act <- getAction
  case act of
   Nothing -> return ()
   Just act -> performAction act

execPipe :: Handle -> Handle -> [IO (FilePath,[String])] -> IO ProcessHandle
execPipe inp outp [act] = do
  (prog,args) <- act
  (_,_,_,ph) <- createProcess ((proc prog args) { std_in = UseHandle inp
                                                , std_out = UseHandle outp })
  return ph
execPipe inp outp (act:acts) = do
  (prog,args) <- act
  (_,Just pout,_,ph) <- createProcess ((proc prog args) { std_in = UseHandle inp
                                                        , std_out = CreatePipe })
  execPipe pout outp acts

compile :: FilePath -> [String] -> IO (Handle,ProcessHandle)
compile fp defs = do
  includePath <- getDataFileName "include"
  let clang = (proc "clang" $
                    ["-O0","-emit-llvm","-c","-o","-",fp,"-I"++includePath,"-DHCTIGAR"]++
                    ["-D"++def | def <- defs ]) { std_out = CreatePipe }
  --let clang = (proc "ls" ["-l"]) { std_out = CreatePipe }
  (_,Just pout,_,ph) <- createProcess clang
  return (pout,ph)

progOptimize :: IO (FilePath,[String])
progOptimize = return ("opt",["-mem2reg"
                             ,"-internalize-public-api-list=main"
                             ,"-internalize"
                             ,"-inline"
                             ,"-loops"
                             ,"-loop-simplify"
                             ,"-loop-rotate"
                             ,"-lcssa"
                             ,"-loop-unroll"
                             ,"-instnamer"
                             ,"-","-o","-"])

progDisassemble :: IO (FilePath,[String])
progDisassemble = return ("llvm-dis",["-","-o","-"])

progEncode :: IO (FilePath,[String])
progEncode = do
  bin <- getBinDir
  return (bin </> "vvt-enc",[])

progPredicates :: Bool -> IO (FilePath,[String])
progPredicates useKarr = do
  bin <- getBinDir
  return (bin </> "vvt-predicates",if useKarr then ["--karr=on"] else [])

progVerify :: IO (FilePath,[String])
progVerify = do
  bin <- getBinDir
  return (bin </> "vvt-verify",["-v2"])

progPretty :: IO (FilePath,[String])
progPretty = do
  bin <- getBinDir
  return (bin </> "vvt-pp",[])
