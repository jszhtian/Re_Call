#pragma		once

// ****************************************************************
// �}�N������
// ================================================================

// �A���t�@�x�b�g���������̉��Z�l
#define		SMALL_ALPHA_ADD		(_T('a') - _T('A'))

// �A���t�@�x�b�g�啶�����̉��Z�l
#define		LARGE_ALPHA_ADD		(_T('A') - _T('a'))

// ����
#define		RNDSEED(num)		srand(num)
#define		RND(num)			(rand() % num)

// �z�񐔎擾
#define		ARRAYCOUNT(list)			(sizeof(list) / sizeof(list[0]))

// ****************************************************************
// ���[�h�l
// ================================================================
struct MODEVALUE{
	int		mod;
	int		value;
};

// ****************************************************************
// �G���[�\����
// ================================================================

struct NAMALIB_SIMPLE_ERR_STRUCT{
	int line;
	TSTR msg;
};

typedef			ARRAY<NAMALIB_SIMPLE_ERR_STRUCT>			NAMALIB_SIMPLE_ERR_LIST;