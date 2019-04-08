#include <stdio.h>

int main(int argc, char const *argv[]) {
  int N,A,B,i,sum = 0,num;

  scanf("%d %d %d",&N,&A,&B);
  for (i = 1; i <= N; i++) {
    num = i/10000%10 + i/1000%10 + i/100%10 + i/10%10 + i%10;
    if (num >= A && num <= B) {
      sum += i;
    }
  }
  printf("%d\n",sum);
  return 0;
}
