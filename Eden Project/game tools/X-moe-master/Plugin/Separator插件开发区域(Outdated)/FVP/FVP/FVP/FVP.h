// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� FVP_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// FVP_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

//FVP headers

#include <string>

using namespace std;


enum CMDMask
{
	UsingUnicode = 0x1UL, //�ļ����ᱻת��,��Ҫ���ForceXXXʹ��
	SaveTempData = 2,
	ForceCHSCP   = 4, //Դ��gbk
	ForceJPCP    = 8,  //Դ��sjis
	ExtractSingle= 16, //��ȡ�������ļ�
	UsingImageEx = 32 //ǿ�ƻ��32-bit��alphaͨ��
};


BOOL bSaveTempData = FALSE;//�Ƿ񱣴���ʱ�ļ� ���ڷ���֮���
BOOL UnicodeCmd = FALSE;//�Ƿ񱣴�ΪUnicode��ʽ
BOOL bUsingImageEx = FALSE;
HMODULE hDLL = NULL;
struct BINHDR
{
	DWORD entry_count;
	DWORD filenames_length;
};

struct BINENTRY
{
	DWORD filename_offset;
	DWORD offset;
	DWORD length;
};

struct HZC1HDR
{
	unsigned char signature[4]; // "hzc1"
	DWORD original_length;
	DWORD header_length;
};

struct NVSGHDR
{
	unsigned char signature[4]; // "NVSG"
	WORD unknown1;
	WORD type;
	WORD width;
	WORD height;
	WORD offset_x;
	WORD offset_y;
	DWORD unknown2;
	DWORD entry_count;
	DWORD unknown3;
	DWORD unknown4;
};

static const WORD NVSGHDR_TYPE_SINGLE_24BIT = 0;
static const WORD NVSGHDR_TYPE_SINGLE_32BIT = 1;
static const WORD NVSGHDR_TYPE_MULTI_32BIT  = 2;
static const WORD NVSGHDR_TYPE_SINGLE_8BIT  = 3;
static const WORD NVSGHDR_TYPE_SINGLE_1BIT  = 4;

//common functios

static string CALLBACK GuessFileExtensionA(const char* buff, unsigned len);

int CALLBACK WriteBMP32W(const wchar_t*   filename, unsigned char* buff, unsigned long len,
						 unsigned long    width,
               			 unsigned long    height,
               			 unsigned short   depth);

int CALLBACK WriteBMP32A(const string&    filename, unsigned char* buff, unsigned long len,
						 unsigned long    width,
               			 unsigned long    height,
               			 unsigned short   depth);

int CALLBACK WriteFile32W(const wchar_t *filename, const char *buff, const unsigned long len);

int CALLBACK WriteFile32A(const string &filename, const char *buff, const unsigned long len);

extern "C" _declspec (dllexport) void ShowInfo();

extern "C" _declspec (dllexport) BOOL Match(TCHAR *pszStr, TCHAR *pFile);

extern "C" _declspec (dllexport) BOOL Engine(TCHAR *pszStr);

extern "C" _declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output = NULL, const TCHAR *args = NULL);
