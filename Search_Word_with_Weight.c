/*   2q-final_2_addWeight_2.c
     takashima.s.ab
     Sora Takashima   */

     #include <stdio.h>
     #include <stdlib.h>
     #include <string.h>

     // ポスティングリストを構成する構造体 //
     typedef struct lnode {
       int ID;  // 文書ID格納 //
       int ID_wei; // 文書ID重み付け(何回出てきたか) //
       struct lnode *next;
     } lnode;

     // 単語の二分木を構成する構造体 //
     typedef struct tnode {
       char word[21];  // 索引語格納 //
       lnode *pos;  // ポスティングリストへのポインタ //
       struct tnode *lt;  // 左の子 //
       struct tnode *rt;  // 右の子 //
     } tnode;

     lnode *createLNode(int ID){
       lnode *p;
       p = malloc(sizeof(lnode));
       if (p == NULL) exit(2); // 失敗 //
       p->ID = ID;
       p->ID_wei = 0;
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
       if (p->ID == ID) {
         p->ID_wei++;  // 同じ文書内に、同じ単語が出てきたら重ね付け //
       }
       return p;  // このIDはすでにある。 //
     }

     // IDの関連度(ID_wei)降順にポスティングリストをソート(バブルソート) //
     lnode *sortLnode(lnode *p){
       lnode *tmp,*rem;
       int swap;
       if (p != NULL) {
         tmp = p;
         rem = NULL; // remは常に「ID比較終了node」の1つ先を指す //
         while (rem != p->next) {  //「ID比較終了node」と先頭が重なるまで //
           while (tmp->next != rem) { //「ID比較するnode」と「ID比較終了node」が重なるまで //
             if (tmp->ID_wei < tmp->next->ID_wei){ //「ID比較するnode」の関連度の方が1つ先のより小さいなら //
               swap = tmp->next->ID;               //「ID比較するnode」の中身を1つ先と入れ替える //
               tmp->next->ID = tmp->ID;
               tmp->ID = swap;
               swap = tmp->next->ID_wei;
               tmp->next->ID_wei = tmp->ID_wei;
               tmp->ID_wei = swap;
             }
             tmp = tmp->next; // 「ID比較するnode」を1つ先へ //
           }
           rem = tmp; // 比較列最後尾に一番小さいnodeを持ってきたので、「ID比較終了node」を1つ左へ //
           tmp = p; // 「ID比較開始node」をリスト先頭に戻す //
         }
       }
       return p; // ソート済みのリスト返す //
     }

     void printList(lnode *p) {
       if (p != NULL) {
         while (p != NULL) {
           printf("%d \t関連度：%d\n",p->ID,p->ID_wei);
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
       if (p == NULL) exit(2); // 失敗 //
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

     int main(int argc, char **argv) {
       // 定義部分 //
       char *fname,*word,key[21]={},buf[1001]={};
       FILE *fp;
       int ID,i;
       tnode *root=NULL;
       lnode *first_pos=NULL;

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
       scanf("%s",key);
       // 探索部分 //
       first_pos = search(root,key);
       // ソート部分 //
       first_pos = sortLnode(first_pos);
       // 出力部分 //
       printList(first_pos);
       // メモリ解放 //
       freeTree(root);
       return 0;
     }
