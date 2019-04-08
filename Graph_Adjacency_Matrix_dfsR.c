/*
Graph_Adjacency_Matrix_dfsR.c
17B08965
Sora Takashima
*/

#include <stdio.h>
#include <stdlib.h>
#include "GRAPH.h"
struct graph{
  int V;
  int E;
  int **adj;
};

Edge EDGE(int v, int w){
  Edge x;
  x.v = v;
  x.w = w;
  return x;
}

int **MATRIXint(int r, int c, int val){
  int i, j;
  int **t = malloc(r * sizeof(int *));
  for (i = 0; i < r; i++) {
    t[i] = malloc(c * sizeof(int));
  }
  for (i = 0; i < r; i++) {
    for (j = 0; j < c; j++) {
      t[i][j] = val;
    }
  }
  return t;
}

Graph GRAPHinit(int V){
  Graph G = malloc(sizeof *G);
  G->V = V;
  G->E = 0;
  G->adj = MATRIXint(V, V ,0);
  return G;
}

void GRAPHinsertE(Graph G, Edge e){
  int v = e.v, w = e.w;
  if (G->adj[v][w] ==  0) {
    G->E++;
  }
  G->adj[v][w] = 1;
  G->adj[w][v] = 1;
}

void GRAPHremoveE(Graph G, Edge e){
  int v = e.v, w = e.w;
  if (G->adj[v][w] ==  1) {
    G->E--;
  }
  G->adj[v][w] = 0;
  G->adj[w][v] = 0;
}

int GRAPHedges(Edge a[], Graph G){
  int v, w, E = 0;
  for (v = 0; v < G->V; v++) {
    for (w = v+1; w < G->V; w++) {
      if (G->adj[v][w] ==  1) {
        a[E++] = EDGE(v, w);
      }
    }
  }
  return E;
}

void GRAPHshow(Graph G){
  int i, j;
  printf("%d verticles, %d edges\n", G->V, G->E);
  for (i = 0; i < G->V; i++) {
    printf("%2d:", i);
    for (j = 0; j < G->E; j++) {
      if (G->adj[i][j] == 1) {
        printf(" %2d", j);
      }
    }
    printf("\n");
  }
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

int flg = 0; // 最初にdfsVが呼び出された時のみ0のフラグ
int *vertex_visited; // visited配列を大域的に定義

void dfsR(Graph G, Edge e) {
  int vertex = e.v;
  if (flg == 0) { // 最初だけvisited配列を0で初期化
    vertex_visited = malloc(sizeof(int) * G->V);
    for (int i = 0; i < G->V; i++) {
      vertex_visited[i] = 0;
    }
  }
  flg = 1;
  if (vertex_visited[vertex] == 0) {
    printf("頂点%dを訪問。\n", vertex); // 訪問した
    vertex_visited[vertex] = 1; // vertexをvisitedに
  }
  for (int j = 0; j < G->V; j++) {
    if (G->adj[vertex][j] == 1 && vertex_visited[j] == 0) { // エッジ[vertex][j]が存在し、かつjが未訪問
      dfsR(G, EDGE(j, 0));
    }
  }
}

int main(int argc, char const *argv[]) {
  int V = atoi(argv[1]), E =  atoi(argv[2]);
  Graph G = GRAPHrand(V, E);
  Edge edges[E];
  GRAPHedges(edges, G);
  if (V < 20) {
    dfsR(G, edges[0]);
  } else {
    printf("%d verticles, %d edges\n", G->V, G->E);
  }
  return 0;
}
