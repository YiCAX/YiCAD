#include "Swap.h"

unsigned short SwapOrder ()
{
  unsigned short usDummy = 1;
  return *((char*) &usDummy) == 1 ? LOW_ENDIAN : HIGH_ENDIAN;
}

void SwapVar (char&)
{
}

void SwapVar (unsigned char&)
{
}

void SwapVar (short& s)
{
  short sTmp = s;
  int i;

  for (i = 0; i < (int)sizeof (short); i++)
    *(((char*) &sTmp) + i) = *(((char*) &s) + sizeof (short) - i - 1);
  s = sTmp;
}

void SwapVar (unsigned short& s)
{
  short sTmp = s;
  int i;

  for (i = 0; i < (int)sizeof (short); i++)
    *(((char*) &sTmp) + i) = *(((char*) &s) + sizeof (short) - i - 1);
  s = sTmp;
}

void SwapVar (long& l)
{
  long lTmp = l;
  int i;

  for (i = 0; i < (int)sizeof (long); i++)
    *(((char*) &lTmp) + i) = *(((char*) &l) + sizeof (long) - i - 1);
  l = lTmp;
}

void SwapVar (unsigned long& l)
{
  long lTmp = l;
  int i;

  for (i = 0; i < (int)sizeof (long); i++)
    *(((char*) &lTmp) + i) = *(((char*) &l) + sizeof (long) - i - 1);
  l = lTmp;
}

void SwapVar (float& f)
{
  float fTmp = f;
  int i;

  for (i = 0; i < (int)sizeof (float); i++)
    *(((char*) &fTmp) + i) = *(((char*) &f) + sizeof (float) - i - 1);
  f = fTmp;
}

void SwapVar (double& d)
{
  double dTmp = d;
  int i;

  for (i = 0; i < (int)sizeof (double); i++)
    *(((char*) &dTmp) + i) = *(((char*) &d) + sizeof (double) - i - 1);
  d = dTmp;
}


