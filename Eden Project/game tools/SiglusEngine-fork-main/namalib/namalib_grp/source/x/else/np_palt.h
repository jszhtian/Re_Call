
#ifdef expand_hensuu_np_pal_data
#	define	NPPALDATA
#else
#	define	NPPALDATA	extern
#endif

NPPALDATA	HANDLE G256_table_hd;
NPPALDATA	HANDLE G256_table_adr_hd;
NPPALDATA	HANDLE Gclr_table_hd;
NPPALDATA	HANDLE Gclr_table_adr_hd;
NPPALDATA	HANDLE Gmono_table_r_hd;
NPPALDATA	HANDLE Gmono_table_g_hd;
NPPALDATA	HANDLE Gmono_table_b_hd;
NPPALDATA	HANDLE Grgbfilter_table_adr_hd;
NPPALDATA	HANDLE Gsat_table_r_hd;
NPPALDATA	HANDLE Gsat_table_g_hd;
NPPALDATA	HANDLE Gsat_table_b_hd;
NPPALDATA	HANDLE Gsat_table_s_hd;
NPPALDATA	HANDLE Gsat_table_s_adr_hd;

NPPALDATA	int *G256_table_p;
NPPALDATA	int **G256_table_adr_p;
NPPALDATA	int *Gclr_table_p;
NPPALDATA	int **Gclr_table_adr_p;
NPPALDATA	int *Gmono_table_r_p;
NPPALDATA	int *Gmono_table_g_p;
NPPALDATA	int *Gmono_table_b_p;
NPPALDATA	int **Grgbfilter_table_adr_p;
NPPALDATA	int *Gsat_table_r_p;
NPPALDATA	int *Gsat_table_g_p;
NPPALDATA	int *Gsat_table_b_p;
NPPALDATA	int *Gsat_table_s_p;
NPPALDATA	int **Gsat_table_s_adr_p;

NPPALDATA	int **Grgbfilter_table_adr_neutral;



void init_palette_table(void);
void make_palette_table(void);
void free_palette_table(void);
