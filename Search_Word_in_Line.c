/*   2q-final_3_addNOT.c
     takashima.s.ab
     Sora Takashima   */

     #include <stdio.h>
     #include <stdlib.h>
     #include <string.h>

     // ポスティングリストを構成する構造体 //
     typedef struct lnode {
       int ID;  // 文書ID格納 //
       struct lnode *next;
     } lnode;

     // 単語の二分木を構成する構造体 //
     typedef struct tnode {
       char word[21];  // 索引語格納 //
       lnode *pos;  // ポスティングリストへのポインタ //
       struct tnode *lt;  // 左の子 //
       struct tnode *rt;  // 右の子 //
     } tnode;

     // 逆ポーランド記法で与えられた論理演算を処理するスタックの構造体 //
     typedef struct snode {
       lnode *pos;  // ポスティングリストへのポインタ //
       struct snode *next;
     } snode;

     lnode *createLNode(int ID){
       lnode *p;
       p = malloc(sizeof(lnode));
       if (p == NULL) exit(0); // 失敗 //
       p->ID = ID;
       p->next = NULL;
       return p;
     }

     lnode *insertLNode(lnode *p,int ID){
       if (p == NULL) {    // ノード追加 //
         p = createLNode(ID);
       }
       else if (ID > p->ID) {  // 探索続行 //
         p->next = insertLNode(p->next,ID);
       }
       return p;  // このIDはすでにある。 //
     }

     void printList(lnode *p) {
       if (p != NULL) {
         while (p != NULL) {
           printf("%d\n",p->ID);
           p = p->next;
         }
       } else {
         printf("Not found\n");
       }
     }

     void freeList(lnode *p) {
       lnode *tmp;
       if(p != NULL){
         while (p != NULL) {
           tmp = p->next;
           free(p);
           p = tmp;
         }
       }
     }

     tnode *createTNode(char word[21]){
       tnode *p;
       p = malloc(sizeof(tnode));
       if (p == NULL) exit(0); // 失敗 //
       strcpy(p->word,word);
       p->pos = NULL;
       p->lt = NULL;
       p->rt = NULL;
       return p;
     }

     tnode *insertTermAndList(tnode *p,char word[21],int ID){
       if (p == NULL) {    // ノード追加 //
         p = createTNode(word);
       }
       else if (strcmp(word,p->word) < 0) {  // 探索続行(左) //
         p->lt = insertTermAndList(p->lt,word,ID);
       }
       else if (strcmp(word,p->word) > 0) {  // 探索続行(右) //
         p->rt = insertTermAndList(p->rt,word,ID);
       }
       if (strcmp(word,p->word) == 0) {
         p->pos = insertLNode(p->pos,ID);  // ポスティング //
       }
       return p;  // wordはすでにある。 //
     }

     lnode *search(tnode *p,char word[21]){
       if (p == NULL) {    // 見つからず //
         return NULL;
       }
       else if (strcmp(word,p->word) < 0) {  // 探索続行(左) //
         return search(p->lt,word);
       }
       else if (strcmp(word,p->word) > 0) {  // 探索続行(右) //
         return search(p->rt,word);
       }
       return p->pos;  // クエリ発見。ポスティングリストを返す。 //
     }

     // 中間順巡回によるword一覧出力 //
     void printTree(tnode *p){
       if(p != NULL){
         printTree(p->lt);        // 左見て、 //
         printf("%s\n",p->word);  // 自分見て、(今回はwordに改行文字が含まれてないので、 //
                                  // 改行が必要。) //
         printTree(p->rt);        // 右見る。 //
       }
     }

     // 左端からポスティングリストごとfree //
     void freeTree(tnode *p){
       if(p != NULL){
         freeTree(p->lt);  // 左見て、 //
         freeTree(p->rt);  // 右見て、 //
         freeList(p->pos);  // ポスティングリストfree //
         free(p);  // 最後に自身をfree //
       }
     }

     // 逆ポーランド記法で与えられた論理演算を処理するスタックを実装 //
     void push(snode **first_sta,lnode *first_pos){
       snode *new;
       new = malloc(sizeof(snode));
       if (new == NULL) exit(0); // 失敗 //
       new->pos = first_pos;
       new->next = *first_sta;
       *first_sta = new;
     }

     lnode *pop(snode **first){
       lnode *p;
       snode *tmp = *first;
       if (tmp == NULL) exit(0); // 失敗 //
       p = tmp->pos;
       *first = tmp->next;
       free(tmp);
       return p;
     }

     // AND演算処理 //
     lnode *Calculation_AND(lnode *first_pos_1,lnode *first_pos_2){
       lnode *p=NULL;
       // どっちもNULLじゃない限りループ //
       while (first_pos_1 != NULL && first_pos_2 != NULL) {
         // first_pos_1->ID <= first_pos_2->IDとなるまで //
         while (first_pos_1->ID > first_pos_2->ID) {
           first_pos_2 = first_pos_2->next;
           // first_pos_2がNULLになった瞬間break //
           if (first_pos_2 == NULL) break;
         }
         // もしID一致してたら、返すポストリストpにID追加 //
         if (first_pos_2 != NULL) {
           if (first_pos_1->ID == first_pos_2->ID) {
             p = insertLNode(p,first_pos_1->ID);
           }
         }
         // 一個first_pos_1を進める //
         first_pos_1 = first_pos_1->next;
       }
       return p;
     }

     // OR演算処理 //
     lnode *Calculation_OR(lnode *first_pos_1,lnode *first_pos_2){
       lnode *p=NULL;
       // どっちもNULLじゃない限りループ //
       while (first_pos_1 != NULL && first_pos_2 != NULL) {
         // first_pos_1->ID <= first_pos_2->IDとなるまで //
         while (first_pos_1->ID > first_pos_2->ID) {
           p = insertLNode(p,first_pos_2->ID);
           first_pos_2 = first_pos_2->next;
           // first_pos_2がNULLになった瞬間break //
           if (first_pos_2 == NULL) break;
         }
         // first_pos_1->ID <= first_pos_2->IDとなったら、 //
         // 返すポストリストpにfirst_pos_1->IDを追加し、 //
         // first_pos_1を1つ進める //
         p = insertLNode(p,first_pos_1->ID);
         first_pos_1 = first_pos_1->next;
       }
       // まだ残っているfirst_pos_1をpに追加 //
       while (first_pos_1 != NULL) {
         p = insertLNode(p,first_pos_1->ID);
         first_pos_1 = first_pos_1->next;
       }
       // まだ残っているfirst_pos_2をpに追加 //
       while (first_pos_2 != NULL) {
         p = insertLNode(p,first_pos_2->ID);
         first_pos_2 = first_pos_2->next;
       }
       return p;
     }

     // NOT演算処理 //
     lnode *Calculation_NOT(lnode *first_pos_1,lnode *first_pos_2){
       lnode *p=NULL;
       // どっちもNULLじゃない限りループ //
       while (first_pos_1 != NULL && first_pos_2 != NULL) {
         // first_pos_2->ID <= first_pos_1->IDとなるまで //
         while (first_pos_2->ID > first_pos_1->ID) {
           first_pos_1 = first_pos_1->next;
           // first_pos_1がNULLになった瞬間break //
           if (first_pos_1 == NULL) break;
         }
         // first_pos_2->ID <= first_pos_1->IDとなったら、 //
         // IDが一致しているか調べ、一致していなかったらfirst_pos_2->IDを //
         // pに追加 //
         if (first_pos_1 != NULL) {
           if (first_pos_2->ID != first_pos_1->ID) {
             p = insertLNode(p,first_pos_2->ID);
           }
         }
         first_pos_2 = first_pos_2->next;
       }
       // まだ残っているfirst_pos_2をpに追加 //
       while (first_pos_2 != NULL) {
         p = insertLNode(p,first_pos_2->ID);
         first_pos_2 = first_pos_2->next;
       }
       return p;
     }

     int main(int argc, char **argv) {
       // 定義部分 //
       char *fname,*word,array_input[401]={},buf[1001]={};
       FILE *fp;
       int ID,i;
       tnode *root=NULL;
       lnode *first_pos=NULL,*first_pos_1=NULL,*first_pos_2=NULL,*first_pos_result=NULL;
       snode *first_sta=NULL;

       if(argc!=2) {
         fprintf(stderr,"Usage: %s prob_file\n", argv[0]);
       }
       fname = argv[1];

       fp = fopen(fname, "r");
       if (fp == NULL) {
         fprintf(stderr,"error opening file\n");
       }
       // bufに1文書ずつ入力し、辞書の二分木を作りながらワードポスティング //
       while (fscanf(fp,"%d\t",&ID) != EOF) {
         fgets(buf,1001,fp);
         word = strtok(buf," \n"); // 区切り文字は、スペースと改行文字 //
         while (word) {
           root = insertTermAndList(root,word,ID);
           word = strtok(NULL," \n"); // 文書が終わりstrtokがNULLを返すまで、区切り続ける //
         }
         // buf初期化 //
         for (i = 0; buf[i] != '\0'; i++) {
           buf[i] = '\0';
         }
       }
       fclose(fp);

       // 入力部分 //
       scanf("%[a-zA-Z ]",array_input);
       word = strtok(array_input," "); // 区切り文字は、スペース //
       // 演算処理 //
       while (word) {
         // ANDだったら2つpopして積集合をpush //
         if (strcmp(word,"AND") == 0) {
           first_pos_1 = pop(&first_sta);
           first_pos_2 = pop(&first_sta);
           first_pos_result = Calculation_AND(first_pos_1,first_pos_2);
           push(&first_sta,first_pos_result);
           // ORだったら2つpopして和集合をpush //
         } else if (strcmp(word,"OR") == 0) {
           first_pos_1 = pop(&first_sta);
           first_pos_2 = pop(&first_sta);
           first_pos_result = Calculation_OR(first_pos_1,first_pos_2);
           push(&first_sta,first_pos_result);
           // NOTだったら2つpopして和集合をpush //
         } else if (strcmp(word,"NOT") == 0) {
           first_pos_1 = pop(&first_sta);
           first_pos_2 = pop(&first_sta);
           first_pos_result = Calculation_NOT(first_pos_1,first_pos_2);
           push(&first_sta,first_pos_result);
           // 単語はそのIDの集合(=単語に紐づいたポストリスト)をpush //
         } else {
           // 探索部分 //
           first_pos = search(root,word);
           push(&first_sta,first_pos);
         }
         word = strtok(NULL," "); // 文書が終わりstrtokがNULLを返すまで、区切り続ける //
       }
       // 出力部分 //
       // スタックに一つ残った演算結果をpopして出力 //
       printList(pop(&first_sta));
       // メモリ解放 //
       freeTree(root);
       return 0;
     }
