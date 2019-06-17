#include <stdarg.h> /* va_list */
#include <stdio.h> /* printf, enum */
#include <dlfcn.h> /* dladdr */
#include <stdlib.h> /* atexit, getenv */
#include <string.h>

#define MAX_DEPTH 32
#define MAX_CALLS 1024

int flag = 0;

typedef struct count_func { // 関数の呼び出し回数を記録するための構造体リスト
    char func_name_callee[50];
    char func_name_caller[50];
    int func_count;
    struct count_func *next;
} count_func;

count_func *root = NULL;

__attribute__((no_instrument_function))
count_func *New(const char *func_name_callee, const char *func_name_caller) { // 新しいエッジ生成
  count_func *x;
  x = malloc(sizeof(count_func));
  strcpy(x->func_name_callee, func_name_callee);
  strcpy(x->func_name_caller, func_name_caller);
  x->func_count = 1;
  x->next = NULL;
  return x;
}

__attribute__((no_instrument_function))
int Count(const char *func_name_callee, const char *func_name_caller) { // カウントとinsertを行う
  count_func *tmp;
  if (root == NULL) { //まだリストの最初がない
    root = New(func_name_callee, func_name_caller);
    return 1;
  } else {
    tmp = root;
    while (1) {
      if ((!strcmp(tmp->func_name_callee, func_name_callee)) && (!strcmp(tmp->func_name_caller, func_name_caller))) {
        break;
      } else {
        if (tmp->next == NULL) { //　エッジまだない
          tmp->next = New(func_name_callee, func_name_caller);
          tmp = NULL;
          break;
        }
      }
      tmp = tmp->next;
    }

    if (tmp != NULL) { // すでにエッジ存在していた
      tmp->func_count++;
      return tmp->func_count;
    } else {
      return 1; // 初めてなら1を返す。
    }
  }
}

__attribute__((no_instrument_function))
void free_counter() { // メモリ解放
  count_func *tmp, *tmp2;
  tmp = root;
  while (tmp != NULL) {
    tmp2 = tmp->next;
    free(tmp);
    tmp = tmp2;
  }
}

__attribute__((no_instrument_function))
void exit_draw(void) { // exit()もしくはreturnされたらここに飛ぶ。
  FILE *fp;
  fp = fopen("cg.dot", "a");
  if (flag == 1) { // まだ最終処理されていなかったら書いて終了
    flag = 2;
    fprintf(fp, "}\n");
    fflush(fp);
    free_counter();
  }
  fclose(fp);
}

__attribute__((no_instrument_function))
int log_to_stderr(const char *file, int line, const char *func, const char *format, ...) {
    char message[4096];
    va_list va;
    va_start(va, format);
    vsprintf(message, format, va);
    va_end(va);
    return fprintf(stderr, "%s:%d(%s): %s\n", file, line, func, message);
}
#define LOG(...) log_to_stderr(__FILE__, __LINE__, __func__, __VA_ARGS__)

__attribute__((no_instrument_function))
const char *addr2name(void* address) {
    Dl_info dli;
    if (dladdr(address, &dli) != 0) {
      return dli.dli_sname;
    } else {
      return NULL;
    }
}


__attribute__((no_instrument_function))
void __cyg_profile_func_enter(void *addr, void *call_site) {
    FILE *fp;
    if(flag == 0) { // 初期設定
      atexit(exit_draw);
      fp = fopen("cg.dot", "w");
      fprintf(fp, "strict digraph G {\n");
      fflush(fp);
      flag = 1;
    } else { // エッジ書き込み処理
      fp = fopen("cg.dot", "a");
      if (getenv("SYSPROG_CG_LABEL") != NULL) {
        if (!strcmp(getenv("SYSPROG_CG_LABEL"), "1")) { //有向グラフが・・・有効！
          fprintf(fp, "%s -> %s [label=\"%d\"];\n", addr2name(call_site), addr2name(addr), Count(addr2name(addr), addr2name(call_site)));
        } else {
          fprintf(fp, "%s -> %s;\n", addr2name(call_site), addr2name(addr));
        }
      } else {
        fprintf(fp, "%s -> %s;\n", addr2name(call_site), addr2name(addr));
      }
      fflush(fp);
    }
    fclose(fp);
}

__attribute__((no_instrument_function))
void __cyg_profile_func_exit(void *addr, void *call_site) {
    FILE *fp;
    fp = fopen("cg.dot", "a");
    if(strcmp(addr2name(addr), "main") == 0) { // 最終処理
      flag = 2;
      fprintf(fp, "}\n");
      fflush(fp);
      free_counter();
    }
    fclose(fp);
}
