#include <stdio.h>

int main(int argc, char const *argv[]) {
  int A,B,C,X,i,j,count=0;
  scanf("%d\n%d\n%d\n%d",&A,&B,&C,&X);
  X /= 50;
  for (i = A; i >= 0; i--) {
    if (i*10 <= X) {
      for (j = B; j >= 0; j--) {
        if (j*2 <= X - i*10) {
          if (C >= X - i*10 - j*2) {
            count++;
          }
        }
      }
    }
  }
  printf("%d\n", count);
  return 0;
}
