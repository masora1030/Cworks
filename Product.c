#include <stdio.h>

int main(int argc, char const *argv[]) {
  int a,b;
  scanf("%d %d",&a,&b);
  if ((a*b) % 2 == 0) {
    printf("Even\n");
  } else {
    printf("Odd\n");
  }
  return 0;
}
