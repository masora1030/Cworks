#include <stdio.h>

int main(int argc, char const *argv[]) {
  long long int N,cap[5],min;
  int min_i;

  scanf("%lld",&N);
  for (int i = 0; i < 5; i++) {
    scanf("%lld",&cap[i]);
  }

  min = cap[0];
  min_i = 0;

  for (int i = 1; i < 5; i++) {
    if (min > cap[i]) {
      min = cap[i];
      min_i = i;
    }
  }

  if (N % cap[min_i] == 0) {
    printf("%lld\n", (N / cap[min_i]) + 4);
  } else {
    printf("%lld\n", (N / cap[min_i]) + 5);
  }
  
  return 0;
}
