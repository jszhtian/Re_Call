#include	"pch.h"



/*=============================================================================
	パレットテーブル初期化
void init_palette_table(void);
		IN		なし
		OUT		なし
-----------------------------------------------------------------------------*/
void init_palette_table(void){

	// インデックスカラー
	G256_table_hd = 0;
	G256_table_p = 0;
	G256_table_adr_hd = 0;
	G256_table_adr_p = 0;

	// 不透明度
	Gclr_table_hd = 0;
	Gclr_table_p = 0;
	Gclr_table_adr_hd = 0;
	Gclr_table_adr_p = 0;

	// モノクロ
	Gmono_table_r_hd = 0;
	Gmono_table_r_p = 0;
	Gmono_table_g_hd = 0;
	Gmono_table_g_p = 0;
	Gmono_table_b_hd = 0;
	Gmono_table_b_p = 0;

	// ＲＧＢフィルタ
	Grgbfilter_table_adr_hd = 0;
	Grgbfilter_table_adr_p = 0;

	// 彩度
	Gsat_table_r_hd = 0;
	Gsat_table_r_p = 0;
	Gsat_table_g_hd = 0;
	Gsat_table_g_p = 0;
	Gsat_table_b_hd = 0;
	Gsat_table_b_p = 0;
	Gsat_table_s_hd = 0;
	Gsat_table_s_p = 0;
	Gsat_table_s_adr_hd = 0;
	Gsat_table_s_adr_p = 0;
}



/*=============================================================================
	パレットテーブル作成
void make_palette_table(void);
		IN		なし
		OUT		なし
-----------------------------------------------------------------------------*/

void make_palette_table_mem_alloc(HANDLE *hd, void *p, UL size, UC *name){
#ifdef __USE_MEMORY_GLOBAL_ALLOC
	*hd = GlobalAlloc(GHND, size);
	if(*hd){
		*(UI *)p = (UI)GlobalLock(*hd);
	}
#else
	mem_alloc(hd, p, size, name);
#endif
}

void make_palette_table(void){
	int i, j;
	int *p;
	int **adr;

	// インデックスカラーテーブル作成
	if(G256_table_hd == 0){
		make_palette_table_mem_alloc(&G256_table_hd, &G256_table_p, 256*256*4, (UC *)"256_TABLE");
		make_palette_table_mem_alloc(&G256_table_adr_hd, &G256_table_adr_p, 256*4, (UC *)"256_TABLE_ADR");
		p = G256_table_p;
		adr = G256_table_adr_p;
		for(i=0; i<256; i++){
			*(p++) = 0;
			for(j=1; j<256; j++){
				*(p++) = (i * 255) / j;
			}
			*(adr++) = G256_table_p + (i * 256);
		}
	}

	// 不透明度変換テーブル作成
	if(Gclr_table_hd == 0){
		make_palette_table_mem_alloc(&Gclr_table_hd, &Gclr_table_p, 511*256*4, (UC *)"CLR_TABLE");
		make_palette_table_mem_alloc(&Gclr_table_adr_hd, &Gclr_table_adr_p, 256*4, (UC *)"CLR_TABLE_ADR");
		p = Gclr_table_p;
		adr = Gclr_table_adr_p;
		for(i=0; i<256; i++){
			for(j=-255; j<=255; j++){
				*(p++) = (j * i) / 255;
			}
			*(adr++) = Gclr_table_p + (i * 511) + 255;		// +255 = ニュートラル値（これ重要）
			// 透明度のテーブルは
		}
	}

	// モノクロ変換テーブル作成
	if(Gmono_table_r_hd == 0){
		make_palette_table_mem_alloc(&Gmono_table_r_hd, &Gmono_table_r_p, 256*4, (UC *)"MONO_TABLE_R");
		make_palette_table_mem_alloc(&Gmono_table_g_hd, &Gmono_table_g_p, 256*4, (UC *)"MONO_TABLE_G");
		make_palette_table_mem_alloc(&Gmono_table_b_hd, &Gmono_table_b_p, 256*4, (UC *)"MONO_TABLE_B");
		p = Gmono_table_r_p;
		for(i=0; i<256; i++){
			*(p++) = ((i * 30) << 8) / 100;
		}
		p = Gmono_table_g_p;
		for(i=0; i<256; i++){
			*(p++) = ((i * 59) << 8) / 100;
		}
		p = Gmono_table_b_p;
		for(i=0; i<256; i++){
			*(p++) = ((i * 11) << 8) / 100;
		}
	}

	// ＲＧＢフィルタ変換テーブル作成
	if(Grgbfilter_table_adr_hd == 0){
		make_palette_table_mem_alloc(&Grgbfilter_table_adr_hd, &Grgbfilter_table_adr_p, (255+1+255+255)*4, (UC *)"RGBFILTER_TABLE_ADR");
		adr = Grgbfilter_table_adr_p;
		for(i=-255; i<0; i++){
			*(adr++) = *(Gclr_table_adr_p + (255 + i));
		}
		Grgbfilter_table_adr_neutral = adr;
		for(i=0; i<=510; i++){
			*(adr++) = *(Gclr_table_adr_p + (255 - i));
		}
	}

	// 彩度変換テーブル作成
	if(Gsat_table_r_hd == 0){
		make_palette_table_mem_alloc(&Gsat_table_r_hd, &Gsat_table_r_p, 256*4, (UC *)"SAT_TABLE_R");
		make_palette_table_mem_alloc(&Gsat_table_g_hd, &Gsat_table_g_p, 256*4, (UC *)"SAT_TABLE_G");
		make_palette_table_mem_alloc(&Gsat_table_b_hd, &Gsat_table_b_p, 256*4, (UC *)"SAT_TABLE_B");
		make_palette_table_mem_alloc(&Gsat_table_s_hd, &Gsat_table_s_p, 511*201*4, (UC *)"SAT_TABLE_S");
		make_palette_table_mem_alloc(&Gsat_table_s_adr_hd, &Gsat_table_s_adr_p, 201*4, (UC *)"SAT_TABLE_S_ADR");
		p = Gsat_table_r_p;
		for(i=0; i<256; i++){
			*(p++) = ((i * 30) << 8) / 100;
		}
		p = Gsat_table_g_p;
		for(i=0; i<256; i++){
			*(p++) = ((i * 59) << 8) / 100;
		}
		p = Gsat_table_b_p;
		for(i=0; i<256; i++){
			*(p++) = ((i * 11) << 8) / 100;
		}
		p = Gsat_table_s_p;
		adr = Gsat_table_s_adr_p;
		for(i=0; i<201; i++){
			*(adr++) = p + 255;
			for(j=0; j<510; j++){
				*(p++) = (j - 255) * i / 100;
			}
		}
	}
}



/*=============================================================================
	パレットテーブル開放
void free_palette_table(void);
		IN		なし
		OUT		なし
-----------------------------------------------------------------------------*/

void free_palette_table_mem_free(HANDLE *hd, void *p){
#ifdef __USE_MEMORY_GLOBAL_ALLOC
	if(*hd){
		GlobalUnlock(*hd);
		GlobalFree(*hd);
		*hd = 0;
		*(UI *)p = (UI)0;
	}
#else
	mem_free(hd, p);
#endif
}

void free_palette_table(void){
	free_palette_table_mem_free(&G256_table_hd, &G256_table_p);
	free_palette_table_mem_free(&G256_table_adr_hd, &G256_table_adr_p);
	free_palette_table_mem_free(&Gclr_table_hd, &Gclr_table_p);
	free_palette_table_mem_free(&Gclr_table_adr_hd, &Gclr_table_adr_p);
	free_palette_table_mem_free(&Gmono_table_r_hd, &Gmono_table_r_p);
	free_palette_table_mem_free(&Gmono_table_g_hd, &Gmono_table_g_p);
	free_palette_table_mem_free(&Gmono_table_b_hd, &Gmono_table_b_p);
	free_palette_table_mem_free(&Grgbfilter_table_adr_hd, &Grgbfilter_table_adr_p);
	free_palette_table_mem_free(&Gsat_table_r_hd, &Gsat_table_r_p);
	free_palette_table_mem_free(&Gsat_table_g_hd, &Gsat_table_g_p);
	free_palette_table_mem_free(&Gsat_table_b_hd, &Gsat_table_b_p);
	free_palette_table_mem_free(&Gsat_table_s_hd, &Gsat_table_s_p);
	free_palette_table_mem_free(&Gsat_table_s_adr_hd, &Gsat_table_s_adr_p);
}



