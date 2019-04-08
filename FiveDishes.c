#include <stdio.h>

int main(int argc, char const *argv[]) {
  int Time[5],i,max=10,max_i,total=0;

  for (i = 0; i < 5; i++) {
    scanf("%d", &Time[i]);
  }

  for (i = 0; i < 5; i++) {
    if (max > Time[i] % 10 && Time[i] % 10 != 0) {
      max = Time[i] % 10;
      max_i = i;
    }
  }

  for (i = 0; i < 5; i++) {
    if (i == max_i) {
      total += Time[i];
    } else if (Time[i] % 10 != 0) {
      total += (Time[i] + (10 - (Time[i] % 10)));
    } else if (Time[i] % 10 == 0) {
      total += Time[i];
    }
  }

  printf("%d\n",total);
  return 0;
}
