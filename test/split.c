#include <stdbool.h>

void assert(bool);
void assume(bool);
int __nondet_int() __attribute__((pure));
bool __nondet_bool() __attribute__((pure));

int main() {
  int k;
  int b = __nondet_int();
  int i;
  int j = __nondet_int();
  int n;
  i = j;
  k = 100;
  n = 0;
  while(n < 2*k) {
    if(b) {
      i++;
    } else {
      j++;
    }
    if (b) b = 0; else b = 1;
    n++;
  }
  assert(i == j);
}
