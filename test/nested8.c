#include <stdbool.h>

void assert(bool);
int __nondet_int() __attribute__((pure));
bool __nondet_bool() __attribute__((pure));

int main() {
  int n = __nondet_int();
  int m = __nondet_int();
  int i,j,k;
  
  if(n>m) return 0;
  i = 0;
  while (i<n) {
    j = 0;
    while (j<n) {
      k = j;
      while (k<n+m) {
	assert(i+j<=n+k+m);
	k++;
      }
      j++;
    }
    i++;
  }
}
