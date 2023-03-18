
#include "AccountManagement.h"

AccountManagement::AccountManagement(int x,int y)
{
  gx = x;
  gy = y;
}

int AccountManagement::getSum()
{
  return gx + gy;
}