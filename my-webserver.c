#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>    /* errno */
#include <stdbool.h>  /* bool */
#include <sys/wait.h> /* wait */
#include <unistd.h>   /* fork */
#include <sys/types.h>
#include <signal.h>


char *not_found_body = "<html><body><center><h1>404 Not Found!</h1></center></body></html>\n";

int find_file(char *path) {
    struct stat info;
    if (stat(path, &info) == -1) {
        return -1;
    }
    return (int)info.st_size;        // file size
}

char *get_content_type(char *filename) {
    if (filename[strlen(filename) - 1] == 'g') {        // jpeg or jpg
        return "image/jpeg";

    } else if (filename[strlen(filename) - 1] == 'f') {        // gif
        return "image/gif";

    } else if (filename[strlen(filename) - 1] == 'l' || filename[strlen(filename) - 1] == 'm') {        // html or htm
        return "text/html";

    /* others */
    } else {
        return "text/plain";
    }
}

void send_content_data(FILE *from, FILE *to) {
    char buf[BUFSIZ];
    int counter = 0;
    while (1) {
        int size = fread(buf, 1, BUFSIZ, from);
        if (size > 0) {
            fwrite(buf, size, 1, to);
        } else {
            break;
        }
        counter++;
    }
}

void session(int fd, char *cli_addr, int cli_port) {

    FILE *fin, *fout;
    fin = fdopen(fd, "r"); fout = fdopen(fd, "w");
    int flag_close = 0;
    while (flag_close == 0) {
        /* read request line */
        char request_buf[BUFSIZ];
        if (fgets(request_buf, sizeof(request_buf), fin) == NULL) {
            fflush(fout);
            fclose(fin);
            fclose(fout);
            close(fd);
            return;        // disconnected
        }

        /* parse request line */
        char method[BUFSIZ];
        char uri[BUFSIZ], *path;
        char version[BUFSIZ];
        sscanf(request_buf, "%s %s %s", method, uri, version);
        path = &(uri[1]);

        printf("HTTP Request: %s %s %s %s\n", method, uri, path, version);
        fflush(stdout);

        /* read/parse header lines */
        while (1) {

            /* read header lines */
            char headers_buf[BUFSIZ];
            if (fgets(headers_buf, sizeof(headers_buf), fin) == NULL) {
                fclose(fin);
                fclose(fout);
                close(fd);
                return;            // disconnected from client
            }

            /* check header end */
            if (strcmp(headers_buf, "\r\n") == 0) {
                break;
            }

            /* parse header lines */
            char header[BUFSIZ];
            char value[BUFSIZ];
            sscanf(headers_buf, "%s %s", header, value);

            if (strcmp(value, "close") == 0 && strcmp(header, "Connection:") == 0) {
              flag_close = 1; // この後はwhileから脱出して切断
            }

            printf("Header: %s %s\n", header, value);

        } // while

        /*

          送信部分を完成させなさい．

         */
         char DefaultPath[11] = "index.html";

         if (!strcmp(path,"")) {
           path = DefaultPath;
         }

         if (find_file(path) == -1) {  //見つかってない
           fprintf(fout, "%s 404 Not Found\r\n", version);
           fprintf(fout, "\r\n");
           fprintf(fout, "%s",not_found_body);

         } else {  //見つかった
           FILE *fdata = fopen(path, "r");

           fprintf(fout, "%s 200 OK\r\n", version);
           fprintf(fout, "Content-Type: %s\r\n", get_content_type(path));
           fprintf(fout, "Content-Length: %d\r\n", find_file(path));
           fprintf(fout, "\r\n");
           send_content_data(fdata, fout);
           fflush(fdata);
           fclose(fdata);
         }
         fflush(fout);
    }


    /* close connection */
    fclose(fin);
    fclose(fout);
    close(fd);
    printf("Connection closed.\n");
    fflush(stdout);
}

void handler_NoWait(int sig) {
  int status;
  int waited_pid;

  waited_pid = waitpid(-1, &status, WNOHANG);
  if (waited_pid == -1) {
      if (errno == ECHILD) {
          /* すでに成仏していた（何もしない） */
        } else {

        }
      } else if (waited_pid == 0) {
        // 何もしない
      } else {
        WEXITSTATUS(status);
        WIFEXITED(status);
      }
  }

int main(int argc, char *argv[]) {

    int listfd, connfd, optval = 1, port = 10000;
    unsigned int addrlen;
    struct sockaddr_in saddr, caddr;

    if (argc >= 2) {
        port = atoi(argv[1]);
    }

    listfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listfd, (struct sockaddr *)&saddr, sizeof(saddr));

    listen(listfd, 10);

    while (1) {
        addrlen = sizeof(caddr);
        connfd = accept(listfd, (struct sockaddr *)&caddr, (socklen_t*)&addrlen);

        int pid = fork();
        if (pid == -1) {
            fprintf(stderr,"Can not fork.\n");
        }
        if (pid == 0) {
          /* 子プロセス側 */
          close(listfd);
          session(connfd, inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
          exit(0);
        } else {
          /* 親プロセス側 */
          close(connfd);
          /* 生成した子プロセスを待機 */
          signal(SIGCHLD, handler_NoWait);

        }
    }
    return 0;
}
