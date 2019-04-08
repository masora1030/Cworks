#include <stdio.h>

int main(int argc, char const *argv[]) {
  int N,Y,i,j,k,R,I,J,K;

  scanf("%d %d",&N,&Y);
  R = Y;
  I = R / 10000;
  R = R % 10000;
  J = R / 5000;
  R = R % 5000;
  K = R / 1000;
  R = R % 1000;
  k = K;
  if (N >= I+J+K && N <= Y/1000) {
    for (i = I; i >= 0; i--) {
      for (j = J; j >= 0; j--) {
        if (i+j+k == N && i*10000 + j*5000 + k*1000 == Y) {
          printf("%d %d %d\n",i,j,k);
          return 0;
        } else {
          k += 5;
        }
      }
      k = K;
      J += 2;
    }
  }
  printf("-1 -1 -1\n");
  return 0;
}
