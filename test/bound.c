#include <stdbool.h>

void assert(bool);
void assume(bool);
int __nondet_int();
bool __nondet_bool();

int main() {
  int n = __nondet_int();
  int h,i,j,k,m;
  
  if (n < 0) return 0;
  if (n > 200) return 0; 
  k=0;
  i=n;
  h = i+k;
  while( i > 0 ){
    i--;
    k++;
    h = i+k;
  }

  j = k;
  m = 0;
  h = j+m;
  while( j > 0 ) {
	j--;
	m++;
	h = j+m;
  }
  assert (i >= 0);
}
