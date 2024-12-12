/*
����libmng����vs���뵽ʹ��һ·�ȿӼ��Լ����޸���Ȼ�޷�����ʹ�ã�
ʱ��ʱ��������������Ʒ��������֪�����Լ������⻹�ǿⱾ������⣬
���ò������Լ���������޷���Ԧlibmng������ϵĿ⣬
�������Σ�ֻ�ܿ�mng��ʽ�ı�׼�ĵ��ٰ����Լ���Ҫ�Ĺ���д�˱�����
��ӭ����ȥ��Ԧlibmng�����ѱ����������������ĵÿɿ���Ŀreadme��

mng�ı�׼�ĵ���http://www.libpng.org/pub/mng/spec/
mng�Ŀ���ָ�ϣ�https://libmng.com/developers.html
*/
#define _CRT_SECURE_NO_WARNINGS
#define _USE_32BIT_TIME_T
#include <iostream>
#include <vector> 
#include <string> 
#include <fstream>
#include <Windows.h>
#include <direct.h>
#include <io.h>

using namespace std;

#pragma pack(1)
#define MNG_SIG 0x8A4D4E47
#define PNG_SIG 0x89504E47
#define POST_SIG 0x0D0A1A0A
#define MHDR_SIG 0x4D484452
#define IHDR_SIG 0x49484452
#define tEXt_SIG 0x74455874
#define IDAT_SIG 0x49444154
#define DEFI_SIG 0x44454649
#define oFFs_SIG 0x6F464673
#define IEND_SIG 0x49454E44
#define MEND_SIG 0x4D454E44

typedef struct mng_mhdr_s//MHDR
{
    DWORD iWidth;
    DWORD iHeight;
    DWORD iTicks;
    DWORD iLayercount;
    DWORD iFramecount;
    DWORD iPlaytime;
    DWORD iSimplicity;
} mng_mhdr_t;
typedef mng_mhdr_t* mng_mhdrp;

typedef struct mng_ihdr_s//IDHR
{
    DWORD iWidth;
    DWORD iHeight;
    BYTE iBitdepth;
    BYTE iColortype;
    BYTE iCompression;
    BYTE iFilter;
    BYTE iInterlace;
} mng_ihdr_t;
typedef mng_ihdr_t* mng_ihdrp;

typedef struct mng_text_s//tEXt
{
    DWORD iKeywordsize;
    char* zKeyword;
    DWORD iTextsize;
    char* zText;
} mng_text_t;
typedef mng_text_t* mng_textp;

typedef struct mng_defi_s//DEFI
{
    USHORT iObjectid;
    BYTE bHasdonotshow;
    BYTE bHasconcrete;
    DWORD iXlocation;
    DWORD iYlocation;
    DWORD iLeftcb;
    DWORD iRightcb;
    DWORD iTopcb;
    DWORD iBottomcb;
} mng_defi_t;
typedef mng_defi_t* mng_defip;
#pragma pack()

class NEKOPACK_mng
{
public:
    DWORD filenum = 0;
    DWORD filesize = 0;
    NEKOPACK_mng(string pathname, string mode);
    ~NEKOPACK_mng();
    bool CheckHeader(string pathname, string mode);
    bool Export();
    bool Import();
    bool GetChunkInfo(string mode);
private:
    ifstream src;
    FILE* info = NULL;
    string dirname;
    bool Check_OK;
    DWORD BE2LE(DWORD bbuff);
    USHORT BE2LE(USHORT bbuff);
};