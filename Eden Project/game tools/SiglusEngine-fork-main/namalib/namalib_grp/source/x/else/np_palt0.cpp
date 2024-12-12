#include	"pch.h"



/*=============================================================================
	�p���b�g�e�[�u��������
void init_palette_table(void);
		IN		�Ȃ�
		OUT		�Ȃ�
-----------------------------------------------------------------------------*/
void init_palette_table(void){

	// �C���f�b�N�X�J���[
	G256_table_hd = 0;
	G256_table_p = 0;
	G256_table_adr_hd = 0;
	G256_table_adr_p = 0;

	// �s�����x
	Gclr_table_hd = 0;
	Gclr_table_p = 0;
	Gclr_table_adr_hd = 0;
	Gclr_table_adr_p = 0;

	// ���m�N��
	Gmono_table_r_hd = 0;
	Gmono_table_r_p = 0;
	Gmono_table_g_hd = 0;
	Gmono_table_g_p = 0;
	Gmono_table_b_hd = 0;
	Gmono_table_b_p = 0;

	// �q�f�a�t�B���^
	Grgbfilter_table_adr_hd = 0;
	Grgbfilter_table_adr_p = 0;

	// �ʓx
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
	�p���b�g�e�[�u���쐬
void make_palette_table(void);
		IN		�Ȃ�
		OUT		�Ȃ�
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

	// �C���f�b�N�X�J���[�e�[�u���쐬
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

	// �s�����x�ϊ��e�[�u���쐬
	if(Gclr_table_hd == 0){
		make_palette_table_mem_alloc(&Gclr_table_hd, &Gclr_table_p, 511*256*4, (UC *)"CLR_TABLE");
		make_palette_table_mem_alloc(&Gclr_table_adr_hd, &Gclr_table_adr_p, 256*4, (UC *)"CLR_TABLE_ADR");
		p = Gclr_table_p;
		adr = Gclr_table_adr_p;
		for(i=0; i<256; i++){
			for(j=-255; j<=255; j++){
				*(p++) = (j * i) / 255;
			}
			*(adr++) = Gclr_table_p + (i * 511) + 255;		// +255 = �j���[�g�����l�i����d�v�j
			// �����x�̃e�[�u����
		}
	}

	// ���m�N���ϊ��e�[�u���쐬
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

	// �q�f�a�t�B���^�ϊ��e�[�u���쐬
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

	// �ʓx�ϊ��e�[�u���쐬
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
	�p���b�g�e�[�u���J��
void free_palette_table(void);
		IN		�Ȃ�
		OUT		�Ȃ�
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



