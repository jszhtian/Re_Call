
#include <string>
#include <stdio.h>
#include <iostream>

#include "zlib.h"
#include "libpng/png.h"

using namespace std;


enum AlphaFlag
{
	HAVE_ALPHA = 0,
	NO_ALPHA
};

typedef struct _PNG_DATA
{
	unsigned int nWidth;
	unsigned int nHeight;
	AlphaFlag eFlag;
	unsigned char *pRgba;
} PNG_DATA;

class PngFile
{
public:

	static bool ReadPngFile(const char* pszFileName, PNG_DATA* pPngData, int* chn);

	static bool ReadPngFile(FILE* pFile, PNG_DATA* pPngData, int* chn);

	static bool WritePngFile(const char* pszFileName, PNG_DATA* pPngData);

	static bool WritePngFile(FILE* pFile, PNG_DATA* pPngData);

private:
	static bool ReadRGBA(png_bytepp pRow, int nWidth, int nHeight, PNG_DATA* pPngData);
	static bool ReadRGB(png_bytepp pRow, int nWidth, int nHeight, PNG_DATA* pPngData);
};

