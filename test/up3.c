#include <stdbool.h>

void assert(bool);
void assume(bool);
int __nondet_int() __attribute__((pure));
bool __nondet_bool() __attribute__((pure));

int main() {
  int n = __nondet_int();
  int i;
  int k;
  int j;
  i = k = 0;
  while( i < n ) {
	i = i + 2;
	k++;
  }
  j = 0;
  while( j < n ) {
    assert(k > 0);
	j = j + 2;
	k--;
  }
}
