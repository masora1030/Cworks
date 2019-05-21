#include "mysh.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <string.h>

/** Run a node and obtain an exit status. */
int invoke_node(node_t *node) {
    LOG("Invoke: %s", inspect_node(node));
    int status,i;

    switch (node->type) {
    case N_COMMAND:
        for (int i = 0; node->argv[i] != NULL; i++) {
            LOG("node->argv[%d]: \"%s\"", i, node->argv[i]);
        }

        fflush(stdout);
        if (fork() == 0) { // child
          execvp(node->argv[0], node->argv);
          exit(0);
        } else { // parent
           if (wait(&status) == -1) { // child wait
             perror("wait");
             exit(errno);
           }
         }
        return 0;

    case N_PIPE: /* foo | bar */
        LOG("node->lhs: %s", inspect_node(node->lhs));
        LOG("node->rhs: %s", inspect_node(node->rhs));

        //課題4
        int **fd;
        int ProcessNum=0; // ProcessNum : 子プロセス数
        node_t *tmp;


        tmp = node;
        for (ProcessNum = 0; node != NULL; ProcessNum++) { // 子プロセス数取得
          node = node->rhs;
        }
        node = tmp;

        fd = (int**)malloc((ProcessNum-1) * sizeof(int*)); //子プロセス数-1個だけのパイプが確保できるようにfdのメモリ確保
        for (i = 0; i < ProcessNum-1; i++) {
          fd[i] = (int*)malloc(2 * sizeof(int));
        }


        i = 0;
        while (node != NULL) {                             //最後のコマンドをnodeが指し終わるまでループ
          if (node->rhs != NULL) {                         //最後の子プロセスの処理ではパイプを新しく作成する必要はない。
            if (node->type == N_PIPE) {                //出力リダイレクトが来た時もパイプは作らない。
              pipe(fd[i]);
            }
          }
          fflush(stdout);
          if (fork() == 0) {           // child
            if (i == 0) {                                  //最初の子プロセスの処理(出力を最初のパイプに流す。 入力は標準入力から。)
              close(fd[i][0]);
              dup2(fd[i][1],1);
              close(fd[i][1]);
              invoke_node(node->lhs);
              exit(0);
            } else if (node->rhs == NULL){                 //最後の子プロセスの処理(出力を標準出力に流す。 入力は前のパイプから。)
              dup2(fd[i-1][0],0);
              close(fd[i-1][0]);
              invoke_node(node);
              exit(0);
            } else {                                       //途中の子プロセスの処理(出力を次のパイプに流す。 入力は前のパイプから。)
              close(fd[i][0]);
              dup2(fd[i][1],1);
              dup2(fd[i-1][0],0);
              close(fd[i][1]);
              close(fd[i-1][0]);
              invoke_node(node->lhs);
              exit(0);
            }
          } else {                     // parent
            if (i == 0) {                                  //最初の子プロセスの看取り処理
              close(fd[i][1]);
              if (wait(&status) == -1) { /* child wait*/
                perror("wait");
                exit(errno);
              }
            } else if (node->rhs == NULL) {                //最後の子プロセスの看取り処理
              close(fd[i-1][0]);
              if (wait(&status) == -1) { /* child wait*/
                perror("wait");
                exit(errno);
              }
            } else {                                       //途中の子プロセスの看取り処理
              close(fd[i][1]);
              close(fd[i-1][0]);
              if (wait(&status) == -1) { /* child wait*/
                perror("wait");
                exit(errno);
              }
            }
          }
          i++;
          node = node->rhs;
        }

        for (i = 0; i < ProcessNum-1; i++) {               //fdのメモリ解放
          free(fd[i]);
        }
        free(fd);

        return 0;


        //課題3
        /*
        LOG("node->lhs: %s", inspect_node(node->lhs));
        LOG("node->rhs: %s", inspect_node(node->rhs));
        int fd[2];
        pipe(fd);
        fflush(stdout);
        if (fork() == 0) { // child1
          close(fd[0]);
          dup2(fd[1], 1); // 出力をパイプに流す。 入力は標準入力。
          close(fd[1]);
          execvp(node->lhs->argv[0], node->lhs->argv);  // exic_write
          exit(0);
        } else { // parent
          fflush(stdout);
          if (fork() == 0) { // child 2
            close(fd[1]);
            dup2(fd[0], 0); // 入力を前のパイプから受け取る。 出力は標準出力。
            close(fd[0]);
            execvp(node->rhs->argv[0], node->rhs->argv); // exic_read
            exit(0);
          } else { // parent
            close(fd[0]);
            close(fd[1]);
            if (wait(&status) == -1) { // child 2 wait
              perror("wait");
              exit(errno);
            }
          }
          if (wait(&status) == -1) { // child 1 wait
            perror("wait");
            exit(errno);
          }
        }
        return 0;
        */

    case N_REDIRECT_IN:     /* foo < bar */
        LOG("node->filename: %s", node->filename);

        int fd_in=0;

        fflush(stdout);
        if (fork() == 0) {             // child
          fd_in = open(node->filename, O_RDONLY);                    //読み込みモード
          if (fd_in == -1) {
            fprintf(stderr, "can not open file %s\n", node->filename);
            exit(2);
          }
          dup2(fd_in,0);
          close(fd_in);
          invoke_node(node->lhs);
          exit(0);
        } else {                       // parent
          if (wait(&status) == -1) { // child wait
            perror("wait");
            exit(errno);
          }
        }

        return 0;

    case N_REDIRECT_OUT:    /* foo > bar */
        LOG("node->filename: %s", node->filename);

        int fd_out=0;

        fflush(stdout);
        if (fork() == 0) {             // child
          fd_out = open(node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);   //書き込みモード
          if (fd_out == -1) {
            fprintf(stderr, "can not open file %s\n", node->filename);
            exit(2);
          }
          dup2(fd_out,1);
          close(fd_out);
          invoke_node(node->lhs);
          exit(0);
        } else {                       // parent
          if (wait(&status) == -1) { // child wait
            perror("wait");
            exit(errno);
          }
        }

        return 0;

    case N_REDIRECT_APPEND: /* foo >> bar */
        LOG("node->filename: %s", node->filename);

        int fd_addout=0;

        fflush(stdout);
        if (fork() == 0) {             // child
          fd_addout = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);   //追加書き込みモード
          if (fd_addout == -1) {
            fprintf(stderr, "can not open file %s\n", node->filename);
            exit(2);
          }
          dup2(fd_addout,1);
          close(fd_addout);
          invoke_node(node->lhs);
          exit(0);
        } else {                       // parent
          if (wait(&status) == -1) { // child wait
            perror("wait");
            exit(errno);
          }
        }

        return 0;

    case N_SEQUENCE: /* foo ; bar */
        LOG("node->lhs: %s", inspect_node(node->lhs));
        LOG("node->rhs: %s", inspect_node(node->rhs));

        fflush(stdout);
        if (fork() == 0) { // child 1
          execvp(node->lhs->argv[0], node->lhs->argv); //左をしてから
          exit(0);
        } else {   // parent
        if (wait(&status) == -1) {// child 1 wait
          perror("wait");
          exit(errno);
           }
           if (fork() == 0) { // child 2
             invoke_node(node->rhs); //右のnodeを展開
             exit(0);
           } else {   // parent
            if (wait(&status) == -1) {// child 2 wait
             perror("wait");
             exit(errno);
           }
         }
       }

        return 0;

    case N_AND: /* foo && bar */
      LOG("node->lhs: %s", inspect_node(node->lhs));
      LOG("node->rhs: %s", inspect_node(node->rhs));

      fflush(stdout);
      if (fork() == 0) { // child 1
        if (node->lhs->type == N_COMMAND) {
          execvp(node->lhs->argv[0], node->lhs->argv); //左をしてから
        } else {
          invoke_node(node->lhs); //左をしてから
        }
        exit(0);
      } else {   // parent
          if (wait(&status) == -1) {// child 1 wait
            perror("wait");
            exit(errno);
          }
          if (status == 0) { // 真なら以下実行
            fflush(stdout);
            if (fork() == 0) { // child 2
              invoke_node(node->rhs); //右のnodeを展開
              exit(0);
            } else {   // parent
              if (wait(&status) == -1) {// child 2 wait
                perror("wait");
                exit(errno);
              }
            }
          }
        }

        return 0;

    case N_OR:  /* foo || bar */
        LOG("node->lhs: %s", inspect_node(node->lhs));
        LOG("node->rhs: %s", inspect_node(node->rhs));

        fflush(stdout);
        if (fork() == 0) { // child 1
          if (node->lhs->type == N_COMMAND) {
            execvp(node->lhs->argv[0], node->lhs->argv); //左をしてから
          } else {
            invoke_node(node->lhs); //左をしてから
          }
          exit(0);
      } else {   // parent
           if (wait(&status) == -1) {// child 1 wait
             perror("wait");
             exit(errno);
           }
           if (status != 0) { // 偽なら以下実行
             fflush(stdout);
             if (fork() == 0) { // child 2
               invoke_node(node->rhs); //右のnodeを展開
               exit(0);
             } else {   // parent
               if (wait(&status) == -1) {// child 2 wait
                 perror("wait");
                 exit(errno);
               }
             }
           }
         }

        return 0;

    case N_SUBSHELL: /* ( foo... ) */
        LOG("node->lhs: %s", inspect_node(node->lhs));

        fflush(stdout);
        if (fork() == 0) { //child 1
          invoke_node(node->lhs); //シェルの中身を展開
          exit(0);
        } else { // parent
          if (wait(&status) == -1) {// child 2 wait
            perror("wait");
            exit(errno);
          }
        }

        return 0;

    default:
        return 0;
    }
}
