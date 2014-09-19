#include <stdbool.h>

void assert(bool);
void assume(bool);
int __undef_int() __attribute__((pure));
bool __undef_bool() __attribute__((pure));

int main(){
  int  n = __undef_int();
  int m,l,i,j,k;

  i = n;
  while (i>=1) { // Accumulation of right-hand transwhilemations. 
    l = i+1;
    if (i < n) {
      if (__undef_bool()) {
        j = l;
	while (j<=n) { // Double division to avoid possible underflow. 
	  assert(1<=j);assert(j<=n);
	  assert(1<=i);assert(i<=n);
	  j++;
	}
	j = l;
	while (j<=n) {
	  k = l;
	  while (k<=n) { 
	    assert(1<=k);assert(k<=n);
	    assert(1<=j);assert(j<=n);
	    k++;
	  }
	  k = l;
	  while (k<=n) { 
	    assert(1<=k);assert(k<=n);
	    assert(1<=j);assert(j<=n);
	    assert(1<=i);assert(i<=n);
	    k++;
	  }
	  j++;
	}
      }
      j = l;
      while (j<=n) { 
        assert(1<=j);assert(j<=n);
	assert(1<=i);assert(i<=n);
	j++;
      }
    }
    assert(1<=i);assert(i<=n);
    assert(1<=i);assert(i<=n);
    l=i;
    i--;
  }

}
