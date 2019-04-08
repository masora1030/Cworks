/*   1q-final_3_rev.c
     takashima.s.ab
     Sora Takashima   */

     #include <stdio.h>
//エリアの壁込みの大きさ決定//
     #define H 52
     #define W 102

     void Generational_change(char dp_in[H][W],char dp_out[H][W]);

     int main(void) {
//定義部分//
       char dp_in[H][W]={},dp_out[H][W]={};
       int i,j,n;
       FILE *fp;


//シュミレーションしたい世代入力//
       scanf("%d",&n);
//データ読み込み//
       fp = fopen("input-final3.txt","r");
       if (fp == NULL) {
         printf("error opening file\n");
         return 1;
       }

        for (i = 1; i < H-1; i++) {
          for (j = 1; j < W-1; j++) {
            fscanf(fp,"%c\n",&dp_in[i][j]);
          }
        }

        fclose(fp);


//n回世代交代//
        while (n > 0) {
          //ループ壁処理//
                  dp_in[0][0] = dp_in[H-2][W-2];
                  dp_in[0][W-1] = dp_in[H-2][1];
                  dp_in[H-1][0] = dp_in[1][W-2];
                  dp_in[H-1][W-1] = dp_in[1][1];
                  for (i = 1; i < H-1; i++) {
                    dp_in[i][0] = dp_in[i][W-2];
                    dp_in[i][W-1] = dp_in[i][1];
                  }
                  for (i = 1; i < W-1; i++) {
                    dp_in[0][i] = dp_in[H-2][i];
                    dp_in[H-1][i] = dp_in[1][i];
                  }
          Generational_change(dp_in,dp_out);
          n--;
          //dp_inにdp_outをコピーし、次の世代交代の準備//
          for (i = 0; i < H; i++) {
            for (j = 0; j < W; j++) {
              dp_in[i][j] = dp_out[i][j];
            }
          }
        }
//データ出力//
        fp = fopen("output-final3.txt","w");

        for (i = 1; i < H-1; i++) {
          for (j = 1; j < W-1; j++) {
            fprintf(fp,"%c",dp_out[i][j]);
          }
          fprintf(fp,"\n");
        }

        fclose(fp);

        return 0;
      }

//世代交代関数//
      void Generational_change(char dp_in[H][W],char dp_out[H][W]){
        int i,j,k,count_living_cell=0;
        for (i = 1; i < H-1; i++) {
          for (j = 1; j < W-1; j++) {
            for (k = 0; k < 3; k++) {
              //注目するマスに隣接する左縦３列の生セル数え上げ//
              if (dp_in[i-1+k][j-1] == '1') {
                count_living_cell += 1;
              }
              //注目するマスに隣接する右縦３列の生セル数え上げ//
              if (dp_in[i-1+k][j+1] == '1') {
                count_living_cell += 1;
              }
            }
            //注目するマスの上に隣接する生セル数え上げ//
            if (dp_in[i-1][j] == '1') {
              count_living_cell += 1;
            }
            //注目するマスの下に隣接する生セル数え上げ//
            if (dp_in[i+1][j] == '1') {
              count_living_cell += 1;
            }
            //注目するマスが生セルだった時の処理//
            if (dp_in[i][j] == '1') {
              if (count_living_cell == 2 || count_living_cell == 3) {
                dp_out[i][j] = '1';
              } else {
                dp_out[i][j] = '0';
              }
            }
            //注目するマスが死セルだった時の処理//
            if (dp_in[i][j] == '0') {
              if (count_living_cell == 3) {
                dp_out[i][j] = '1';
              } else {
                dp_out[i][j] = '0';
              }
            }
            count_living_cell = 0;
          }
        }
      }
