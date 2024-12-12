#include <WinFile.h>


void BMP_TO_DIB(PBYTE data, int width, int height, int BitCount)
{
	BYTE* TempBuffer;
	int i;
	int widthlen;

	int nAlignWidth = (width * 32 + 31) / 32;
	size_t BufferSize = (BitCount / 8) * nAlignWidth * height;
	TempBuffer = (BYTE*)GlobalAlloc(0, BufferSize);

	widthlen = width * (BitCount / 8);
	for (i = 0; i<height; i++)
	{
		memcpy(&TempBuffer[(((height - i) - 1)*widthlen)], &data[widthlen * i], widthlen);
	}
	memcpy(data, TempBuffer, BufferSize);
	GlobalFree(TempBuffer);
}


VOID GetBMPData(PBYTE BMPImage)
{
	BITMAPFILEHEADER* Header;
	BITMAPINFOHEADER* HeaderInfo;
	BYTE* SourceBits;
	int TargetAlignWidth;
	int SourceAlignWidth;
	int BitCount;
	LONG RealHeight;

	Header = (BITMAPFILEHEADER*)BMPImage;
	HeaderInfo = (BITMAPINFOHEADER*)(BMPImage + sizeof(BITMAPFILEHEADER));

	SourceBits = &BMPImage[Header->bfOffBits];

	BitCount = HeaderInfo->biBitCount;
	SourceAlignWidth = (HeaderInfo->biWidth*BitCount + 31) / 32;

	RealHeight = HeaderInfo->biHeight < 0 ? -HeaderInfo->biHeight : HeaderInfo->biHeight;
	
	BMP_TO_DIB(SourceBits, SourceAlignWidth, RealHeight, BitCount);
	HeaderInfo->biHeight = 0;
	HeaderInfo->biHeight = RealHeight;
}



int wmain(int argc, WCHAR* argv[])
{
	if (argc != 2)
		return 0;

	WinFile File, OutFile;
	ULONG  InSize;
	PBYTE  InBuffer = nullptr;
	WCHAR  lpFileName[MAX_PATH] = { 0 };

	do
	{
		if (FAILED(File.Open(argv[1], WinFile::FileRead)))
			break;

		InSize = File.GetSize32();
		InBuffer = (PBYTE)GlobalAlloc(0, InSize);
		if (!InBuffer)
			break;

		File.Read(InBuffer, InSize);
		wsprintfW(lpFileName, L"%s.bmp", argv[1]);
		GetBMPData(InBuffer);

		if (FAILED(OutFile.Open(lpFileName, WinFile::FileWrite)))
			break;

		OutFile.Write(InBuffer, InSize);
		OutFile.Release();

	} while (0);

	if (InBuffer)
		GlobalFree(InBuffer);

	File.Release();
	return 0;
}

