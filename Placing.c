#include <stdio.h>

int main(int argc, char const *argv[]) {
  int input,count = 0;
  scanf("%d", &input);
  for (int i = 0; i < 3; i++) {
    if (input % 10 == 1) {
      count++;
    }
    input = input/10;
  }
  printf("%d\n",count);
  return 0;
}
