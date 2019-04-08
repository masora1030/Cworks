/*
Graph_Adjacency_Lists_bfsR.c
17B08965
Sora Takashima
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GRAPH.h"

typedef int Item; // キューのItemはintとした

typedef struct node *link;

typedef struct nodeQ *linkQ; // キューのリンク
struct nodeQ {
Item item;
linkQ next;
};

struct node{ // グラフのリンク
  int v;
  link next;
};

struct graph{
  int V;
  int E;
  link *adj;
};

static linkQ head=NULL;
static linkQ tail=NULL;

void QUEUEerror() {
  fprintf(stderr,"error operation\n");
  exit(2);
}

linkQ NEWQ(Item item, linkQ next) { // キューのNEW
  linkQ x = malloc(sizeof *x);
  if (x == NULL) QUEUEerror(); // メモリ確保失敗したらエラー
  x->item = item;
  x->next = next;
  return x;
}

void QUEUEenqueue(Item item) { // エンキュー
  if (head == NULL){
    head = (tail = NEWQ(item, head));
  } else {
  tail->next = NEWQ(item, tail->next);
  tail = tail->next;
  }
}

Item QUEUEdequeue() { // デキュー
  if (head == NULL) QUEUEerror(); // キューが空だったらエラー
  Item item = head->item;
  linkQ t = head->next;
  free(head);
  head = t;
  return item;
}

Edge EDGE(int v, int w){
  Edge x;
  x.v = v;
  x.w = w;
  return x;
}

link NEW(int v, link next){
  link x = malloc(sizeof *x);
  x->v = v;
  x->next = next;
  return x;
}

Graph GRAPHinit(int V){
  int v;
  Graph G = malloc(sizeof *G);
  G->V = V;
  G->E = 0;
  G->adj = malloc(sizeof(link)*V);
  for (v = 0; v < V; v++) {
    G->adj[v] = NULL;
  }
  return G;
}

void GRAPHinsertE(Graph G, Edge e){
  int v = e.v, w = e.w;
  link t;
  for (t = G->adj[v]; t != NULL; t = t->next) { // すでに追加済みじゃないかチェック
    if (t->v == w) {
      break;
    }
  }
  if (t == NULL) { // まだなかったら追加
    G->adj[v] = NEW(w, G->adj[v]);
    G->adj[w] = NEW(v, G->adj[w]);
    G->E++;
  }
}

void GRAPHremoveE(Graph G, Edge e){
  int v = e.v, w = e.w;
  link t,rem;
  if (G->adj[v] !=  NULL) {
    if (G->adj[v]->v == w) { // 先頭にあった場合
      G->E--;
      rem = G->adj[v]->next;
      free(G->adj[v]);
      G->adj[v] = rem;
    } else {
      for (t = G->adj[v]; t != NULL; t = t->next) { // 先頭以外から該当のエッジあるか探す
        if (w == t->next->v) { // あったら削除
          G->E--;
          rem = t->next->next;
          free(t->next);
          t->next = rem;
          break;
        }
      }
    }

    if (G->adj[w]->v == v) { // 相方についても同じことを行う。
      rem = G->adj[w]->next; // G->Eの減少はなし
      free(G->adj[w]);
      G->adj[w] = rem;
    } else {
      for (t = G->adj[w]; t != NULL; t = t->next) { // 先頭以外から該当のエッジあるか探す
        if (v == t->next->v) { // あったら削除
          rem = t->next->next;
          free(t->next);
          t->next = rem;
          break;
        }
      }
    }
  }
}

int GRAPHedges(Edge a[], Graph G){
  int v, E = 0;
  link t;
  for (v = 0; v < G->V; v++) {
    for (t = G->adj[v]; t != NULL; t = t->next) {
      if (v < t->v) {
        a[E++] =  EDGE(v, t->v);
      }
    }
  }
  return E;
}

int randV(Graph G){
  return G->V * (rand() / (RAND_MAX + 1.0));
}

Graph GRAPHrand(int V, int E){
  Graph G = GRAPHinit(V);
  while (G->E < E) {
    GRAPHinsertE(G, EDGE(randV(G), randV(G)));
  }
  return G;
}

void GRAPHshow(Graph G){
  int i, j;
  link t;
  printf("%d verticles, %d edges\n", G->V, G->E);
  for (i = 0; i < G->V; i++) {
    printf("%2d:", i);
    for (t = G->adj[i]; t != NULL; t = t->next) {
      printf(" %2d", t->v);
    }
    printf("\n");
  }
}

void bfsR(Graph G, Edge e) {
  int vertex_visited[G->V]; // visited配列を定義
  int vertex = e.v;
  link t;
  for (int i = 0; i < G->V; i++) { // visited配列を0で初期化
    vertex_visited[i] = 0;
  }
  QUEUEenqueue(vertex);
  vertex_visited[vertex] = 1;
  while (head != NULL) { // 幅優先探索
    vertex = QUEUEdequeue();
    printf("頂点%dを訪問。\n", vertex);
    for (t = G->adj[vertex]; t != NULL; t = t->next) {
      if (vertex_visited[t->v] != 1) { // 訪れたものは除外
        QUEUEenqueue(t->v);
        vertex_visited[t->v] = 1;
      }
    }
  }
}

int main(int argc, char const *argv[]) {
  int V = atoi(argv[1]), E =  atoi(argv[2]);
  Graph G = GRAPHrand(V, E);
  Edge edges[E];
  GRAPHedges(edges, G);
  if (V < 20) {
    bfsR(G, edges[0]);
  } else {
    printf("%d verticles, %d edges\n", G->V, G->E);
  }
  return 0;
}
