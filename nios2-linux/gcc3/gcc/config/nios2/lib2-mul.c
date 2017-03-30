/* while we are debugging (ie compile outside of gcc build) 
   disable gcc specific headers */
#ifndef DEBUG_MULSI3


/* We include auto-host.h here to get HAVE_GAS_HIDDEN.  This is
   supposedly valid even though this is a "target" file.  */
#include "auto-host.h"


#include "tconfig.h"
#include "tsystem.h"
#include "coretypes.h"
#include "tm.h"


/* Don't use `fancy_abort' here even if config.h says to use it.  */
#ifdef abort
#undef abort
#endif


#ifdef HAVE_GAS_HIDDEN
#define ATTRIBUTE_HIDDEN  __attribute__ ((__visibility__ ("hidden")))
#else
#define ATTRIBUTE_HIDDEN
#endif

#include "libgcc2.h"

#else
#define SItype int
#define USItype unsigned int
#endif


extern SItype __mulsi3 (SItype, SItype);

SItype
__mulsi3 (SItype a, SItype b)
{
  SItype res = 0;
  USItype cnt = a;
  
  while (cnt)
    {
      if (cnt & 1)
        {
	  res += b;	  
	}
      b <<= 1;
      cnt >>= 1;
    }
    
  return res;
}
/*
TODO: Choose best alternative implementation.

SItype
__divsi3 (SItype a, SItype b)
{
  SItype res = 0;
  USItype cnt = 0;
  
  while (cnt < 32)
    {
      if (a & (1L << cnt))
        {
	  res += b;	  
	}
      b <<= 1;
      cnt++;
    }
    
  return res;
}
*/


#ifdef DEBUG_MULSI3

int
main ()
{
  int i, j;
  int error = 0;
  
  for (i = -1000; i < 1000; i++)
    for (j = -1000; j < 1000; j++)
      {
	int expect = i * j;
	int actual = A__divsi3 (i, j);
	if (expect != actual)
	  {
	    printf ("error: %d * %d = %d not %d\n", i, j, expect, actual);
	    error = 1;
	  }
      }

  return error;
}
#endif
