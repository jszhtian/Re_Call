// CUFFS 汎用　メッセージローダー
// 開発期間　2007-01-18------2007-04-28 nota
//
// ■対応確認ゲーム
// さくらむすび
// ワンコとリリー
// アメサラサ
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DEBUG  0 // 0.通常出力 1.デバッグモード

// プロトタイプ宣言
void conv1(void);//一般デコード用
void conv2(void);//主人公名前代入用
void conv3(void);//ゴミ除去用

// グローバルで使うため外部宣言
char version[]="2007/04/28 VER 1.3";	// バージョン、ツール最終改定日
//2007/04/28 VER 1.3 アメサラサに正式対応
//2007/04/01 VER 1.2 こっそりアメサラサにも対応
//2007/02/14 VER 1.1 ワンコとリリーも対応
//2007/01/19 VER 1.0 さくらむすび専用

FILE *fp,*to,*tbl;					// ファイルポインタ

fpos_t fsize = 1;//サイズならびに、一時的なポジションの記憶用
unsigned long i;				// 汎用
unsigned long i_bak;				// 汎用
unsigned long r;			// 汎用
unsigned char iGetFile;		// リードデータ格納用
unsigned char iGetFile2;		// リードデータ格納用

unsigned char table[65536][3];		// UNICODE to SHIFTJIS変換テーブル用

unsigned long rsize;				// ファイルサイズ 
unsigned char name[100];			// ファイル読み込み名前 
unsigned char select_f;			// 選択肢カウンタ  ■
unsigned char t[40];			// 汎用
unsigned char count_a;			// 汎用

unsigned char g_name1[50];			// ゲーム名 
unsigned char f_name1[30];			// ファイル名 
unsigned char h_name1[30];			// 主人公名字 
unsigned char h_name2[30];			// 主人公名前 
unsigned char h_name3[30];			// 主人公あだな 
unsigned char title_f;				// シーンタイトルフラグ 
unsigned char buf[1000];			// バッファ
unsigned char seen_f;				//セーブタイトルフラグ
// メインルーチン
int main(void)
{
	printf("■ CUFFS 汎用 メッセージローダー ■\n%s\n",version);
	printf("------------------------------------\n");



	//ゲーム情報テーブル作成
	if((tbl=fopen("setting.ini","rb"))==NULL){
		printf("●エラー\n");
		printf("設定ファイル [setting.ini] が見つかりません。\n");
		printf("[cuffs_con.exe]と同じフォルダに入れてから再度実行してください。\n");
		printf("（Enterキーを押すと終了します）\n");
		if(!DEBUG){
			getchar();//ダミー
		}
		exit(1);
	}
	printf("●ゲーム情報取得（setting.ini参照中…）\n");
	fseek(tbl,0,SEEK_SET);
	for(i=0;i<=40;i++){//ゲーム名テーブル作成（実は変換には無関係なんですが）
		fread(&g_name1[i],1,1,tbl);
		if(g_name1[i]==0x0d){
			g_name1[i]=0x00;
			printf("　ゲーム名　　:%s\n",g_name1);
			fseek(tbl,1,SEEK_CUR);
			break;
		}
	}
	for(i=0;i<=20;i++){//シナリオファイルテーブル作成
		fread(&f_name1[i],1,1,tbl);
		if(f_name1[i]==0x0d){
			f_name1[i]=0x00;
			printf("　ファイル名　:%s\n",f_name1);
			fseek(tbl,1,SEEK_CUR);
			break;
		}
	}
	for(i=0;i<=20;i++){//主人公名字テーブル作成
		fread(&h_name1[i],1,1,tbl);
		if(h_name1[i]==0x0d){
			h_name1[i]=0x00;
			printf("　名字　　　　:%s\n",h_name1);
			fseek(tbl,1,SEEK_CUR);
			break;
		}
	}
	for(i=0;i<=20;i++){//主人公名前テーブル作成
		fread(&h_name2[i],1,1,tbl);
		if(h_name2[i]==0x0d){
			h_name2[i]=0x00;
			printf("　名前　　　　:%s\n",h_name2);
			fseek(tbl,1,SEEK_CUR);
			break;
		}
	}
	for(i=0;i<=20;i++){//主人公あだなテーブル作成
		fread(&h_name3[i],1,1,tbl);
		if(h_name3[i]==0x0d){
			h_name3[i]=0x00;
			printf("　あだな　　　:%s\n",h_name3);
			fseek(tbl,1,SEEK_CUR);
			break;
		}
	}
	for(i=0;i<=0;i++){//シーンタイトルフラグ（アメアラサはオフ）
		fread(&title_f,1,1,tbl);
		title_f-=0x30;
		printf("　タイトル出力:");
		if(title_f==0){
			printf("オフ\n");
		}
		if(title_f==1){
			printf("オン\n");
		}
	}
	fclose(tbl);


	// シナリオファイルオープン
	if((fp=fopen(f_name1,"rb"))==NULL){
		printf("●エラー\n");
		printf("　シナリオファイル [%s] が見つかりません。\n",f_name1);
		printf("　[cuffs_con.exe]と同じフォルダに入れてから再度実行してください。\n");
		printf("（Enterキーを押すと終了します）\n");
		if(!DEBUG){
			getchar();//ダミー
		}
		exit(1);
	}

	//UnicodeからShitjisへの変換テーブル作成
	if((tbl=fopen("uni2shi.tbl","rb"))==NULL){
		printf("●エラー\n");
		printf("　テーブルファイル [uni2shi.tbl] が見つかりません。\n");
		printf("　[cuffs_con.exe]と同じフォルダに入れてから再度実行してください。\n");
		printf("（Enterキーを押すと終了します）\n");
		if(!DEBUG){
			getchar();//ダミー
		}
		exit(1);
	}
	for(i=0;i<=65535;i++){//Unicodeからshiftjisへの変換テーブル作成
		fread(&iGetFile,1,1,tbl);
		table[i][0]=iGetFile;
		fread(&iGetFile,1,1,tbl);
		table[i][1]=iGetFile;
		table[i][2]=0x00;
	}
	fclose(tbl);


	printf("●シナリオ変換中...\n"); 
	printf("（そのままお待ちください）\n");

	conv1();  // メッセージコンバート

	// ファイルクローズ
	fclose(fp);

	printf("●主人公名前修正中...\n"); 
	printf("（そのままお待ちください）\n");
	fp=fopen("temp.nota","rb");
	conv2();  // メッセージコンバート
	fclose(fp);

	printf("●ゴミ除去中...\n"); 
	printf("（そのままお待ちください）\n");
	fp=fopen("temp2.nota","rt");
	conv3();  // メッセージコンバート
	fclose(fp);

	remove("temp.nota");
	remove("temp2.nota");

	printf("●シナリオ変換完了\n");
	printf("（Enterキーを押すと終了します）\n");
	if(!DEBUG){
		getchar();//ダミー
	}

	return 0;
}

void conv1(void){
	unsigned long ulStart;
	unsigned long ulEnd;

	to=fopen("temp.nota","wt");//結果格納用ファイル

	fseek(fp,-1,SEEK_END); 
	fgetpos(fp,&fsize);//ファイルサイズゲット
	seen_f=0;
	ulStart=0;
	ulEnd  =ulStart+fsize;

	fseek(fp,ulStart,SEEK_SET);
	for(i=ulStart;i<=ulEnd;i++){
		fread(&iGetFile,1,1,fp);

		if(i%2000000==0){//進行状況カウンタ
			printf("%10d/%9d completed...\n",i,ulEnd);
		}

		if(iGetFile==0x41){//選択肢文章
			//AddSelect 41 00 64 00 64 00 53 00 65 00 6C 00 65 00 63 00 74 00
			fseek(fp,-3,SEEK_CUR);
			i-=3;
			fread(&iGetFile,1,1,fp);
			i++;
			fread(&iGetFile2,1,1,fp);
			i++;
			if(iGetFile==0x00&&iGetFile2==0x00){// \0\0Talkチェック
				//OKだったとき
				fseek(fp,1,SEEK_CUR);
				i++;
			}else{//"AddSelect"でなかったとき
				fseek(fp,+2,SEEK_CUR);
				i+=2;
				continue;
			}
			//AddSelect 41 00 64 00 64 00 53 00 65 00
			for(r=0;r<=8;r++){//AddSeちぇっく
				fread(&iGetFile,1,1,fp);
				i++;
				t[r]=iGetFile;
			}
			if(t[0]==0x00&&t[1]==0x64&&t[2]==0x00&&t[3]==0x64&&t[4]==0x00&&t[5]==0x53&&t[6]==0x00&&t[7]==0x65&&t[8]==0x00){

//fprintf(to,"\n%8x/ad/\n",i);//debug
				//AddSeだったとき
				fseek(fp,8,SEEK_CUR);
				i+=8;
				fread(&iGetFile,1,1,fp);
				i++;
				fread(&iGetFile2,1,1,fp);
				i++;
				if(iGetFile==0x01&&iGetFile2==0x02){//OK
					fseek(fp,-0x24,SEEK_CUR);
					i-=0x24;
					while(i<ulEnd){//選択肢文開始部分測定
						fread(&iGetFile,1,1,fp);
						i++;
						if(iGetFile==0x00){
							fseek(fp,-2,SEEK_CUR);
							i-=2;
							fread(&iGetFile,1,1,fp);
							i++;
							if(iGetFile==0x00){//OK
								fseek(fp,1,SEEK_CUR);
								i++;
								break;
							}else{
								fseek(fp,-2,SEEK_CUR);
								i-=2;
								continue;
							}
						}
						fseek(fp,-2,SEEK_CUR);
						i-=2;
					}
					fprintf(to,"■");
					while(i<ulEnd){
						fread(&iGetFile2,1,1,fp);
						i++;
						fread(&iGetFile,1,1,fp);
						i++;
						if(iGetFile==0x00&&iGetFile2==0x02){//文末
							fprintf(to,"\n");
							fseek(fp,0x15,SEEK_CUR);
							i+=0x15;
							break;
						}
						fprintf(to,"%s",table[iGetFile*256+iGetFile2]);
					}
				}else{//AddSe+??の違う制御文のとき
					continue;
				}
			}else{//TalkかMessでなかったとき
				fseek(fp,-9,SEEK_CUR);
				i-=9;
				continue;
			}
		}




/////////////////////////////////発言者書き込み
		if(iGetFile==0x06){//02 00 06 XX 00 00 00 :XX*2＝発言者バイト数
			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile<0x01&&iGetFile>0x0f){//NG名前多すぎ
				fseek(fp,-1,SEEK_CUR);
				i--;
				seen_f=0;
				continue;
			}
			count_a=iGetFile*2;//バイト数メモ

			fseek(fp,-4,SEEK_CUR);
			i-=4;
			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile!=0x02){//00チェックその-2
				fseek(fp,2,SEEK_CUR);
				i+=2;
				continue;
			}

			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile!=0x00){//00チェックその-1
				fseek(fp,1,SEEK_CUR);
				i+=1;
				continue;
			}
			fseek(fp,2,SEEK_CUR);
			i+=2;

			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile!=0x00){//00チェックその１
				fseek(fp,-2,SEEK_CUR);
				i-=2;
				continue;
			}
			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile!=0x00){//00チェックその２
				fseek(fp,-3,SEEK_CUR);
				i-=3;				seen_f=0;
				continue;
			}
			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile!=0x00){//00チェックその３
				fseek(fp,-4,SEEK_CUR);
				i-=4;
				continue;
			}

//fprintf(to,"\n%8x/se/\n",i);//debug

			if(seen_f==0 || title_f==0){
				fprintf(to,"【");
			}else{
				fprintf(to,"＜");//セーブタイトル
			}
			for(r=1;r<=count_a;r+=2){
				fread(&iGetFile2,1,1,fp);
				i++;
				fread(&iGetFile,1,1,fp);
				i++;
				fprintf(to,"%s",table[iGetFile*256+iGetFile2]);
			}
			if(seen_f==0 || title_f==0){
				fprintf(to,"】\n");
			}else{
				fprintf(to,"＞\n");//セーブタイトル
				seen_f=0;
			}
		}
//////////////////////////////////////////

/*
*/
		//セーブタイトルフラグ
		if(iGetFile==0x53){//53 00 63 00 65 00 6E 00 61 00 72 00 69 00 6F 00 45 00 6E 00 74 00 65 00 72 :ScenarioEnter
			for(r=0;r<=23;r++){//ScenarioEnter(セーブタイトル表示)
				fread(&iGetFile,1,1,fp);
				i++;
				t[r]=iGetFile;
			}
			if(t[0]==0x00&&t[1]==0x63&&t[2]==0x00&&t[3]==0x65&&t[4]==0x00&&t[5]==0x6e&&t[6]==0x00&&t[7]==0x61&&t[8]==0x00&&t[9]==0x72&&t[10]==0x00&&t[11]==0x69&&t[12]==0x00&&t[13]==0x6f&&t[14]==0x00&&t[15]==0x45&&t[16]==0x00&&t[17]==0x6e&&t[18]==0x00&&t[19]==0x74&&t[20]==0x00&&t[21]==0x65&&t[22]==0x00&&t[23]==0x72){
				seen_f=1;
				continue;
			}else{
				fseek(fp,-24,SEEK_CUR);
				i-=24;
			}
			continue;
		}
















		///文章書き出し系統
		if(iGetFile==0x54||iGetFile==0x4d){
			//TalK 54 00 61 00 6C 00 6B 00
			//Mess 4D 00 65 00 73 00 73 00
			fseek(fp,-3,SEEK_CUR);
			i-=3;
			fread(&iGetFile,1,1,fp);
			i++;
			fread(&iGetFile2,1,1,fp);
			i++;
			if(iGetFile==0x00&&iGetFile2==0x00){// \0\0Talkチェック
				//TalkかMessだったとき
				fseek(fp,1,SEEK_CUR);
				i++;
			}else{//"Talk"か"Mess"でなかったとき
				fseek(fp,+2,SEEK_CUR);
				i+=2;
				continue;
			}
			//TalK 54 00 61 00 6C 00 6B 00
			//Mess 4D 00 65 00 73 00 73 00
			for(r=0;r<=6;r++){//TalK Messちぇっく
				fread(&iGetFile,1,1,fp);
				i++;
				t[r]=iGetFile;
			}

			if((t[0]==0x00&&t[1]==0x61&&t[2]==0x00&&t[3]==0x6c&&t[4]==0x00&&t[5]==0x6b&&t[6]==0x00)||(t[0]==0x00&&t[1]==0x65&&t[2]==0x00&&t[3]==0x73&&t[4]==0x00&&t[5]==0x73&&t[6]==0x00)){
				//Talkか
				//Messだったとき
				fread(&iGetFile,1,1,fp);
				i++;
				fread(&iGetFile2,1,1,fp);
				i++;
				if(iGetFile==0x01&&iGetFile2==0x02){//OK
//fprintf(to,"\n%8x/me/\n",i);//debug
					fseek(fp,6,SEEK_CUR);
					i+=6;
				}else{//talk+??とかの違う制御文のとき
					continue;
				}
			}else{//TalkかMessでなかったとき
				fseek(fp,-7,SEEK_CUR);
				i-=7;
				continue;
			}
			while(i<ulEnd){//文章書き出し
				fread(&iGetFile2,1,1,fp);
				i++;
				fread(&iGetFile,1,1,fp);
				i++;
				if(iGetFile==0x05&&iGetFile2==0x08){//メッセージエンド
					break;
				}
				if(iGetFile==0x00&&iGetFile2==0x02){//メッセージエンド
					fprintf(to,"\n");
					break;
				}
				if(iGetFile2==0x08&&iGetFile==0x05){//文末
					break;
				}
				fprintf(to,"%s",table[iGetFile*256+iGetFile2]);
			}
		}
	}
	fprintf(to,"でこ～どしゅうーりょー\n");//ファイルエンド保険予備
	fclose(to);
}







void conv2(void){//主人公名前代入用
	unsigned long ulStart;
	unsigned long ulEnd;

	to=fopen("temp2.nota","wb");//結果格納用ファイル

	fseek(fp,-1,SEEK_END); 
	fgetpos(fp,&fsize);//ファイルサイズゲット

	ulStart=0;
	ulEnd  =ulStart+fsize;

	fseek(fp,ulStart,SEEK_SET);
	for(i=ulStart;i<=ulEnd;i++){
		fread(&iGetFile,1,1,fp);
		if(i%400000==0){//進行状況カウンタ
			printf("%10d/%9d completed...\n",i,ulEnd);
		}
		if(iGetFile==0x81){//名前代入
			fread(&iGetFile,1,1,fp);
			i++;

			if(iGetFile==0x97){//主人公名字＠＠＠＠
				fread(&iGetFile,1,1,fp);
				i++;
				if(iGetFile==0x81){//主人公名字＠＠＠＠
					fread(&iGetFile,1,1,fp);
					i++;
					if(iGetFile==0x97){//主人公名字＠＠＠＠
						fseek(fp,4,SEEK_CUR);
						i+=4;
						fprintf(to,"%s",h_name1);
						continue;
					}
					fseek(fp,-3,SEEK_CUR);
					i-=3;
					fprintf(to,"%c",0x81);
					continue;
				}
				fseek(fp,-2,SEEK_CUR);
				i-=2;
				fprintf(to,"%c",0x81);
				continue;
			}

			if(iGetFile==0x96){//主人公名前＊＊＊＊
				fread(&iGetFile,1,1,fp);
				i++;
				if(iGetFile==0x81){//主人公名字＊＊＊＊
					fread(&iGetFile,1,1,fp);
					i++;
					if(iGetFile==0x96){//主人公名前＊＊＊＊
						fseek(fp,4,SEEK_CUR);
						i+=4;
						fprintf(to,"%s",h_name2);
						continue;
					}
					fseek(fp,-3,SEEK_CUR);
					i-=3;
					fprintf(to,"%c",0x81);
					continue;
				}
				fseek(fp,-2,SEEK_CUR);
				i-=2;
				fprintf(to,"%c",0x81);
				continue;
			}

			if(iGetFile==0x90){//主人公あだな＄＄＄＄
				fread(&iGetFile,1,1,fp);
				i++;
				if(iGetFile==0x81){//主人公名字＄＄＄＄
					fread(&iGetFile,1,1,fp);
					i++;
					if(iGetFile==0x90){//主人公あだな＄＄＄＄
						fseek(fp,4,SEEK_CUR);
						i+=4;
						fprintf(to,"%s",h_name3);
						continue;
					}
					fseek(fp,-3,SEEK_CUR);
					i-=3;
					fprintf(to,"%c",0x81);
					continue;
				}
				fseek(fp,-2,SEEK_CUR);
				i-=2;
				fprintf(to,"%c",0x81);
				continue;
			}

			fprintf(to,"%c",0x81);
			fseek(fp,-1,SEEK_CUR);
			i--;
			continue;
		}else{
			fprintf(to,"%c",iGetFile);
			continue;
		}
	}
	fclose(to);
}

void conv3(void){//ゴミ除去用
	unsigned long ulStart;
	unsigned long ulEnd;

	to=fopen("cuffs_dec.txt","wt");//結果格納用ファイル

	fseek(fp,-1,SEEK_END); 
	fgetpos(fp,&fsize);//ファイルサイズゲット

	ulStart=0;
	ulEnd  =ulStart+fsize;

	fseek(fp,ulStart,SEEK_SET);
	while(1){
		fgets(buf, 0x200, fp);

		if(i%400000<=6){//進行状況カウンタ
			printf("%10d/%9d completed...\n",i,ulEnd);
		}
		for(r=0;r<=0x200;r++){
			if(buf[r]==0x0d||buf[r]==0x0a){
				buf[r]=0x00;
				break;
			}
			if(r==0x200){
				buf[r]=0x00;
				break;
			}
		}

		if(buf[0]==0x81&&buf[1]==0xa1){//選択肢重複除去
			fseek(to,-r-4,SEEK_CUR); 
			fprintf(to,"%s\n",buf);
			continue;
		}
		if(strcmp(buf, "【共通】")==0){//誤検出
			continue;
			i=ftell(fp);
		}
		if(strcmp(buf, "【心の声】")==0){//名前なし
			continue;
			i=ftell(fp);
		}
		if(strcmp(buf, "【語り】")==0){//名前なし
			continue;
			i=ftell(fp);
		}
		if(strcmp(buf, "【】")==0){//誤検出
			continue;
			i=ftell(fp);
		}
		if(strcmp(buf, "＜＞")==0){//誤検出
			continue;
			i=ftell(fp);
		}
		if(strcmp(buf, "でこ～どしゅうーりょー")==0){//デコード完了
			break;
		}
		fprintf(to,"%s\n",buf);
		i=ftell(fp);
		continue;
	}
	fclose(to);
}


