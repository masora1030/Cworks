#include <stdio.h>

int main(int argc, char const *argv[]) {
  int num,input,arr[200],i,count=0;
  scanf("%d", &num);
  for (i = 0; i < num; i++) {
    scanf("%d", &input);
    arr[i] = input;
  }
  while (1) {
    for (i = 0; i < num; i++) {
      if (arr[i] % 2 == 0) {
        arr[i] = arr[i]/2;
      } else {
        break;
      }
    }
    if (i != num) {
      break;
    } else {
      count++;
    }
  }
  printf("%d\n", count);
  return 0;
}
