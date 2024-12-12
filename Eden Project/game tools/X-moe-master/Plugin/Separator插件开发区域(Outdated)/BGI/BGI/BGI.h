#ifndef __XXX_BGI_H__
#define __XXX_BGI_H__

#pragma pack (1)
typedef struct
{
	char magic[12];		/* "PackFile    " */
	unsigned long entries;
}ArchieveHeader;


static const char NewPckSign[] =
{
	0x42, 0x55, 0x52, 0x49,
	0x4B, 0x4F, 0x20, 0x41,
	0x52, 0x43, 0x32, 0x30
};

typedef struct
{
	char name[6*0x10];
	unsigned long offset;//ע�� ,���offset��һ�����ֵ,��header����0��ʼ���� 
	unsigned long length;
	unsigned long reserved1;
	unsigned long reserved2;
	unsigned long reserved3;
	unsigned long reserved4;
	unsigned long reserved5;
	unsigned long reserved6;
}ArchieveEntryNew;

//0x10 * 2
typedef struct
{
	char name[16];
	unsigned long offset;
	unsigned long length;
	unsigned long reserved1;
	unsigned long reserved2;
}ArchieveEntry;

typedef struct
{
	char magic[16];			/* "DSC FORMAT 1.00" */
	unsigned long key;
	unsigned long uncomprlen;
	unsigned long dec_counts;			/* ���ܵĴ��� */
	unsigned long reserved;
} DSCHeader;

typedef struct
{
	char magic[16];			/* "SDC FORMAT 1.00" */
	unsigned long key;
	unsigned long comprlen;
	unsigned long uncomprlen;
	unsigned short sum_check;			/* �ۼ�У��ֵ */
	unsigned short xor_check;			/* �����У��ֵ */
} GDBHeader;

typedef struct
{
	unsigned long header_length;
	unsigned long unknown0;
	unsigned long length;				/* ʵ�ʵ����ݳ��� */
	unsigned long unknown[13];
} SNDHeader;

typedef struct
{
	unsigned short width;
	unsigned short height;
	unsigned long color_depth;
	unsigned long reserved0[2];
} GPRHeader;

typedef struct
{
	char magic[16];			/* "CompressedBG___" */
	unsigned short width;
	unsigned short height;
	unsigned long color_depth;
	unsigned long reserved0[2];
	unsigned long zero_comprlen;		/* huffman��ѹ���0ֵ�ֽ�ѹ�������ݳ��� */
	unsigned long key;				/* ����Ҷ�ڵ�Ȩֵ����key */
	unsigned long encode_length;		/* �����˵�Ҷ�ڵ�Ȩֵ�εĳ��� */
	unsigned char sum_check;
	unsigned char xor_check;
	unsigned short reserved1;          //since 2013��value = 2 
} BGHeader, bg_header_t;
#pragma pack ()

typedef char s8;
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;


extern "C" _declspec (dllexport) void ShowInfo();

extern "C" _declspec (dllexport) BOOL Match(TCHAR *pszStr, TCHAR *pFile);

extern "C" _declspec (dllexport) BOOL Engine(TCHAR *pszStr);

extern "C" _declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output = NULL, const TCHAR *args = NULL);


#endif
