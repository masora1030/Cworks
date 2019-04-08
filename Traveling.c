#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  int N,t=0,x=0,y=0,t_input,x_input,y_input,distance;
  scanf("%d\n",&N);
  while (N > 0) {
    scanf("%d %d %d\n",&t_input,&x_input,&y_input);
    t_input = t_input - t;
    x_input = x_input - x;
    y_input = y_input - y;
    distance = abs(x_input)+abs(y_input);
    if (t_input >= distance && distance % 2 == t_input % 2) {
      t = t + t_input;
      x = x + x_input;
      y = y + y_input;
      N--;
    } else {
      printf("No\n");
      return 0;
    }
  }
  printf("Yes\n");
  return 0;
}
