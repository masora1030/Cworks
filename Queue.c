/*   Queue.c
     17B08965
     Sora Takashima   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int Item; // Itemはintとした

typedef struct node *link;
struct node {
int item;
link next;
};

static link head=NULL;
static link tail=NULL;

void QUEUEerror() {
  fprintf(stderr,"error operation\n");
  exit(2);
}

link NEW(Item item, link next) {
  link x = malloc(sizeof *x);
  if (x == NULL) QUEUEerror(); // メモリ確保失敗したらエラー
  x->item = item;
  x->next = next;
  return x;
}

void QUEUEenqueue(Item item) {
  if (head == NULL){
    head = (tail = NEW(item, head));
  } else {
  tail->next = NEW(item, tail->next);
  tail = tail->next;
  }
}

Item QUEUEdequeue() {
  if (head == NULL) QUEUEerror(); // キューが空だったらエラー
  Item item = head->item;
  link t = head->next;
  free(head);
  head = t;
  return item;
}

int main(void) {
  printf("キューが空の時にdequeueしたらエラーする確認\n");
  QUEUEenqueue(10); // 10をenqueue
  printf("%d\n", QUEUEdequeue()); // 10をdequeue
  QUEUEdequeue(); // キューが空の状態でdequeue
  return 0;
}
