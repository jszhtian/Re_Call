// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 FVP_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// FVP_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

//FVP headers

#include <string>

using namespace std;


enum CMDMask
{
	UsingUnicode = 0x1UL, //文件名会被转码,需要配合ForceXXX使用
	SaveTempData = 2,
	ForceCHSCP   = 4, //源是gbk
	ForceJPCP    = 8,  //源是sjis
	ExtractSingle= 16, //提取单独的文件
	UsingImageEx = 32 //强制混合32-bit的alpha通道
};


BOOL bSaveTempData = FALSE;//是否保存临时文件 用于分析之类的
BOOL UnicodeCmd = FALSE;//是否保存为Unicode形式
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
