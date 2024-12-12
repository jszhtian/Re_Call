#pragma		once

// ****************************************************************
// マクロ命令
// ================================================================

// アルファベット小文字化の加算値
#define		SMALL_ALPHA_ADD		(_T('a') - _T('A'))

// アルファベット大文字化の加算値
#define		LARGE_ALPHA_ADD		(_T('A') - _T('a'))

// 乱数
#define		RNDSEED(num)		srand(num)
#define		RND(num)			(rand() % num)

// 配列数取得
#define		ARRAYCOUNT(list)			(sizeof(list) / sizeof(list[0]))

// ****************************************************************
// モード値
// ================================================================
struct MODEVALUE{
	int		mod;
	int		value;
};

// ****************************************************************
// エラー構造体
// ================================================================

struct NAMALIB_SIMPLE_ERR_STRUCT{
	int line;
	TSTR msg;
};

typedef			ARRAY<NAMALIB_SIMPLE_ERR_STRUCT>			NAMALIB_SIMPLE_ERR_LIST;