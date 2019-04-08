#include <stdio.h>

int main(int argc, char const *argv[]) {
  char S[100001];
  int i=0;

  scanf("%s",S);
  if (S[i] == '\0') {
    printf("YES\n");
  } else {
  for (i = 0; S[i] != '\0'; i++);
  for (i--; i >= 0; i--) {
    if (S[i] == 'e') {
      i--;
      if (S[i] == 's') {
        i--;
        if (S[i] == 'a') {
          i--;
          if (S[i] == 'r') {
            i--;
            if (S[i] == 'e') {
              //何もしない
            } else {
              printf("NO\n");
              return 0;
            }
          } else {
            printf("NO\n");
            return 0;
          }
        } else {
          printf("NO\n");
          return 0;
        }
      } else {
        printf("NO\n");
        return 0;
      }
    } else if (S[i] == 'm') {
      i--;
      if (S[i] == 'a') {
        i--;
        if (S[i] == 'e') {
          i--;
          if (S[i] == 'r') {
            i--;
            if (S[i] == 'd') {
              //何もしない
            } else {
              printf("NO\n");
              return 0;
            }
          } else {
            printf("NO\n");
            return 0;
          }
        } else {
          printf("NO\n");
          return 0;
        }
      } else {
        printf("NO\n");
        return 0;
      }
    } else if (S[i] == 'r') {
      i--;
      if (S[i] == 'e') {
        i--;
        if (S[i] == 's') {
          i--;
          if (S[i] == 'a') {
            i--;
            if (S[i] == 'r') {
              i--;
              if (S[i] == 'e') {
                //何もしない
              } else {
                printf("NO\n");
                return 0;
              }
            } else {
              printf("NO\n");
              return 0;
            }
          } else {
            printf("NO\n");
            return 0;
          }
        } else if (S[i] == 'm') {
          i--;
          if (S[i] == 'a') {
            i--;
            if (S[i] == 'e') {
              i--;
              if (S[i] == 'r') {
                i--;
                if (S[i] == 'd') {
                  //何もしない
                } else {
                  printf("NO\n");
                  return 0;
                }
              } else {
                printf("NO\n");
                return 0;
              }
            } else {
              printf("NO\n");
              return 0;
            }
          } else {
            printf("NO\n");
            return 0;
          }
        } else {
          printf("NO\n");
          return 0;
        }
      } else {
        printf("NO\n");
        return 0;
      }
    } else {
      printf("NO\n");
      return 0;
    }
  }
  printf("YES\n");
  }
  return 0;
}
