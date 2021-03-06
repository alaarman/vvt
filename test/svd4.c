#include "benchmarks.h"

int main()
{
  NONDET_INT(n);
  NONDET_INT(m);
  int i,j,k,l;
  
  if (n<=m) return 0;
  if (m<=n) { i = m; } else { i = n; } 

  while (i>=1) { // Accumulation of left-hand transwhilemations. 
    l=i+1;

    assert(1<=i);
    assert(i<=n); 

    j=l;
    while (j<=n) {
      assert(1<=i);
      assert(i<=m);
      assert(1<=j);assert(j<=n);
      j++;
    }

    if (__nondet_bool()) {
      j = l;
      while (j<=n) {
        k=l;
	while (k<=m) {
	  assert(1<=k);assert(k<=m);
	  assert(1<=i);assert(i<=n);
	  assert(1<=j);assert(j<=n);
	  k++;
	}

	assert(1<=i);assert(i<=m);
	assert(1<=i);assert(i<=n);
	k = i;
	while (k<=m) {
	  assert(1<=k);assert(k<=m);
	  assert(1<=j);assert(j<=n);
	  assert(1<=i);assert(i<=n);
	  k++;
	  }
	j++;
      }
      j=i;
      while (j<=m) { 
	assert(1<=j);assert(j<=m); 
	assert(1<=i);assert(i<=n);
	j++;
      }
    } else { j = i; while (j<=m) { 
      assert(1<=j);assert(j<=m); 
      assert(1<=i);assert(i<=n);
      j++;
    }}
    
    assert(1<=i);assert(i<=m); 
    assert(1<=i);assert(i<=n);
    i--;
    }
}
