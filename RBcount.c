#include <stdio.h>

int main(int argc, char const *argv[]) {
  int N,i,count=0;
  char s[101];
  scanf("%d",&N);
  scanf("%s",s);
  for (i = 0; i < N; i++) {
    if (s[i] == 'R') {
      count++;
    } else {
      count--;
    }
  }
  if (count > 0) {
    printf("Yes\n");
  } else {
    printf("No\n");
  }
  return 0;
}
