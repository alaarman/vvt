#include "benchmarks.h"

int main()
{
  NONDET_INT(n);
  int k;
  int i;
  int j;
  k = i = 1;
  j = 0;
  while(i<=n-1) {
    j=0;
    while(j<=i-1) {
      k = k+(i-j);
      j++;
    }
    if(j<i)
      return 0;
    i++;
  }
  if(i < n)
    return 0;
  assert(k > n-1);
}
