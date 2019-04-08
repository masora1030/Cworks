#include <stdio.h>

int main(int argc, char const *argv[]) {
  int N,Q,i,arr_1[200000] = {},arr_2[200000] = {},count=0;
  char s[200001],t,d;

  scanf("%d %d",&N,&Q);
  scanf("%s",s);
  for (i = 0; i < N; i++) {
    arr_1[i] = 1;
    arr_2[i] = 1;
  }

  while (Q > 0) {
    scanf("\n%c %c",&t,&d);
    for (i = 0; i < N; i++) {
      if (s[i] == t) {
        arr_2[i] -= arr_1[i];
        if (d == 'L') {
          if (i != 0) {
            arr_2[i-1] += arr_1[i];
          }
        } else if (d == 'R'){
          if (i != N-1) {
            arr_2[i+1] += arr_1[i];
          }
        }
      }
    }
    for (i = 0; i < N; i++) {
      arr_1[i] = arr_2[i];
    }
    Q--;
  }
  for (i = 0; i < N; i++) {
    count += arr_1[i];
  }
  printf("%d\n",count);
  return 0;
}
