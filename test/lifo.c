#include <stdbool.h>

void assert(bool);
int __nondet_int() __attribute__((pure));
bool __nondet_bool() __attribute__((pure));

//This example is adapted from StinG 
int main()
{
  int i = __nondet_int();
  int sa;
  int ea;
  int ma;
  int sb;
  int eb;
  int mb;

  if (! (i>=1)) return 0;
  sa=0;
  ea=0;
  ma=0;
  sb=0;
  eb=0;
  mb=0;

  while(__nondet_bool())
    {
      if (__nondet_bool())
	{
	  if (! (sb >= 1)) return 0;
	  sb = sb-1;
	  sa = ea+ma+1;
	  ea = 0;
	  ma = 0;
	}
      else
	{
	  if (__nondet_bool())
	    {
	      if (! (i >= 1)) return 0;
	      i = i -1;
	      sa = sa + ea + ma + 1;
	      ea = 0;
	      ma =0;
	    }
	  else
	    {
	      if (__nondet_bool())
		{
		  if (! (i>=1)) return 0;
		  i=i-1;
		  sb=sb+eb+mb+1;
		  eb=0;
		  mb=0;
		}
	      else
		{
		  if (__nondet_bool())
		    {
		      if (! (sa>=1)) return 0;
		      sa=sa-1;
		      sb=sb+eb+mb+1;
		      eb=0;
		      mb=0;
		    }
		  else
		    {
		      if (__nondet_bool())
			{
			  if (! (sa>=1)) return 0;
			  i=i+sa+ea+ma;
			  sa=0;
			  ea=1;
			  ma=0;
			}
		      else
			{
			  if (__nondet_bool())
			    {
			      if (! (sb>=1)) return 0;
			      sb=sb-1;
			      i=i+sa+ea+ma;
			      sa=0;
			      ea=1;
			      ma=0;
			    }
			  else
			    {
			      if (__nondet_bool())
				{
				  if (! (sb>=1)) return 0;
				  i=i+sb+eb+mb;
				  sb=0;
				  eb=1;
				  mb=0;
				}
			      else
				{
				  if (__nondet_bool())
				    {
				      if (! (sa>=1)) return 0;
				      sa=sa-1;
				      i=i+sb+eb+mb;
				      sb=0;
				      eb=1;
				      mb=0;
				    }
				  else
				    {
				      if (__nondet_bool())
					{
					  if (! (ea >=1)) return 0;
					  ea = ea -1;
					  ma = ma +1;
					}
				      else
					{
					  if (! (eb >=1)) return 0;
					  eb = eb -1;
					  mb = mb +1;
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
  
  assert (ea + ma <= 1);
  assert (eb + mb <= 1);
  assert (mb  >= 0);
  assert (eb  >= 0);
  assert (ma  >= 0);
  assert (ea  >= 0);
}

