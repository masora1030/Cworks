#include <stdio.h>

int main(int argc, char const *argv[]) {
  int array[5],k;

  for (int i = 0; i < 5; i++) {
    scanf("%d\n", &array[i]);
  }
  scanf("%d",&k);

  for (int i = 1; i < 4; i++) {
    if (array[i+1] - array[0] > k) {
      printf(":(\n");
      return 0;
    }
  }

  printf("Yay!\n");
  return 0;
}
