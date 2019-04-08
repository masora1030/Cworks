#ifndef ITEM_H
#define ITEM_H

typedef char* Key;
struct record{
  Key keyVal;
  int value;
};
typedef struct record *Item;

#define maxN 1000
#define maxKey 100
#define NULLitem NULL
#define key(A) (A)
#define eq(A, B) (strcmp(A,B) == 0)
#define less(A, B) (strcmp(A,B) < 0)


#endif
