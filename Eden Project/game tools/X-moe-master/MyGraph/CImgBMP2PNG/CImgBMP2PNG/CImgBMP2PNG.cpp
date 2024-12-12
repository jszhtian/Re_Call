#include "stdafx.h"
#include "CImg.h"
#include "png.h"
#include "LodePNG.h"
#include "pngdib.h"

#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "zlib.lib")

using namespace cimg_library;

struct MemStream
{
	BYTE* start;
	BYTE* cur;
	DWORD len;
	ULONG total;

};

#pragma pack(push, 1)

typedef struct
{
	UINT16 wTag;                   // 0x00
	UINT32 dwFileSize;             // 0x02
	UINT32 dwReserve;              // 0x06
	UINT32 dwRawOffset;            // 0x0A
	struct
	{
		UINT32 dwInfoHeaderSize;   // 0x0E
		UINT32 dwWidth;            // 0x12
		UINT32 dwHeight;           // 0x16
		UINT16 wLayer;             // 0x1A
		UINT16 wBit;               // 0x1C
		UINT32 dwCompressed;       // 0x1E
		UINT32 dwSizeImage;        // 0x22
		INT32  iXPelsPerMeter;     // 0x26
		INT32  iYPelsPerMeter;     // 0x2A
		UINT32 dwClrUsed;          // 0x2E
		UINT32 dwClrImportant;     // 0x32
	} Info;
} IMG_BITMAP_HEADER, SBitMapHeader, *PBitMapHeader, *LPBitMapHeader;

typedef struct
{
	UCHAR  ID;
	UCHAR  byPaletteType;
	UCHAR  byImageType;
	UINT16 usPaletteEntry;
	UINT16 usPaletteLength;
	UCHAR  byPaletteBitCount;
	UINT16 usOriginX;
	UINT16 usOriginY;
	UINT16 usWidth;
	UINT16 usHeight;
	UCHAR  byPixelDepth;
	UCHAR  byImageDescription;
} IMG_TGA_HEADER, STGAHeader;

typedef struct
{
	union
	{
		LPVOID lpBuffer;
		PBYTE  pbBuffer;
	};
	INT32  Width;
	INT32  Height;
	INT32  BitsPerPixel;
	INT32  Stride;
	LPVOID lpExtraInfo;
	INT32  ExtraInfoSize;
} UCIInfo, *PUCIInfo, *LPUCIInfo;

#pragma pack(pop)

void PngWrite(png_struct* png, png_byte* buff, png_size_t len)
{
	auto stream = (MemStream*)png_get_io_ptr(png);
	if (stream->cur - stream->start + len > stream->len)
	{
		png_error(png, "err");
	}

	memcpy(stream->cur, buff, len);
	stream->cur += len;
	stream->total += len;
}

void PngFlush(png_struct* png)
{

}

int EncodeBmpToPng(int width, int height, int bit, void* pallete, void* dib, MemStream* stream)
{
	int colorType;
	if (bit == 8)
		colorType = PNG_COLOR_TYPE_PALETTE;
	else if (bit == 24)
		colorType = PNG_COLOR_TYPE_RGB;
	else if (bit == 32)
		colorType = PNG_COLOR_TYPE_RGBA;
	else
	{
		return -1;
	}

	auto png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!png)
		return -1;
	auto pngInfo = png_create_info_struct(png);
	if (!pngInfo)
	{
		png_destroy_write_struct(&png, 0);
		return -1;
	}

	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_write_struct(&png, &pngInfo);
		return -1;
	}

	png_set_write_fn(png, stream, PngWrite, PngFlush);

	int realHeight = height > 0 ? height : -height;

	png_set_IHDR(png, pngInfo, width, realHeight, 8, colorType,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_color* newPlte = nullptr;
	if (pallete)
	{
		if (bit == 8)
		{
			newPlte = new png_color[256];
			auto p1 = newPlte;
			auto p2 = (BYTE*)pallete;
			for (int i = 0; i < 256; i++)
			{
				p1->red = p2[2];
				p1->green = p2[1];
				p1->blue = p2[0];
				p1++;
				p2 += 4;
			}

			png_set_PLTE(png, pngInfo, newPlte, 256);
		}
	}

	png_write_info(png, pngInfo);

	if (bit != 8)
		png_set_bgr(png);

	auto rowPointers = new png_byte*[realHeight];
	int bytesPerRow = (bit / 8 * width + 3)&~3;

	if (height < 0)
	{
		auto p = (png_byte*)dib;
		for (int i = 0; i < realHeight; i++)
		{
			rowPointers[i] = p;
			p += bytesPerRow;
		}
	}
	else
	{
		auto p = (png_byte*)dib + (realHeight - 1)*bytesPerRow;
		for (int i = 0; i < realHeight; i++)
		{
			rowPointers[i] = p;
			p -= bytesPerRow;
		}
	}

	png_write_image(png, rowPointers);
	png_write_end(png, pngInfo);

	if (bit == 8 && newPlte)
		delete[] newPlte;

	delete[] rowPointers;

	png_destroy_write_struct(&png, &pngInfo);
	
	return 0;
}

unsigned decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, const std::vector<unsigned char>& bmp)
{
	static const unsigned MINHEADER = 54; //minimum BMP header size

	if (bmp.size() < MINHEADER) return -1;
	if (bmp[0] != 'B' || bmp[1] != 'M') return 1; //It's not a BMP file if it doesn't start with marker 'BM'
	unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
	//read width and height from BMP header
	w = bmp[18] + bmp[19] * 256;
	h = bmp[22] + bmp[23] * 256;
	//read number of channels from BMP header
	if (bmp[28] != 24 && bmp[28] != 32) return 2; //only 24-bit and 32-bit BMPs are supported.
	unsigned numChannels = bmp[28] / 8;

	//The amount of scanline bytes is width of image times channels, with extra bytes added if needed
	//to make it a multiple of 4 bytes.
	unsigned scanlineBytes = w * numChannels;
	if (scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

	unsigned dataSize = scanlineBytes * h;
	if (bmp.size() < dataSize + pixeloffset) return 3; //BMP file too small to contain all pixels

	image.resize(w * h * 4);

	/*
	There are 3 differences between BMP and the raw image buffer for LodePNG:
	-it's upside down
	-it's in BGR instead of RGB format (or BRGA instead of RGBA)
	-each scanline has padding bytes to make it a multiple of 4 if needed
	The 2D for loop below does all these 3 conversions at once.
	*/
	for (unsigned y = 0; y < h; y++)
		for (unsigned x = 0; x < w; x++)
		{
			//pixel start byte position in the BMP
			unsigned bmpos = pixeloffset + (h - y - 1) * scanlineBytes + numChannels * x;
			//pixel start byte position in the new raw image
			unsigned newpos = 4 * y * w + 4 * x;
			if (numChannels == 3)
			{
				image[newpos + 0] = bmp[bmpos + 2]; //R
				image[newpos + 1] = bmp[bmpos + 1]; //G
				image[newpos + 2] = bmp[bmpos + 0]; //B
				image[newpos + 3] = 255;            //A
			}
			else
			{
				image[newpos + 0] = bmp[bmpos + 3]; //R
				image[newpos + 1] = bmp[bmpos + 2]; //G
				image[newpos + 2] = bmp[bmpos + 1]; //B
				image[newpos + 3] = bmp[bmpos + 0]; //A
			}
		}
	return 0;
}

#define B2P_ERROR 0
#define B2P_OK    1

// Read a file into a memory block.
static int read_file_to_mem(const TCHAR *fn, unsigned char **pmem, DWORD *pfsize)
{
	HANDLE hfile = INVALID_HANDLE_VALUE;
	DWORD fsize;
	void *fbuf;
	DWORD bytesread;
	int retval = B2P_ERROR;

	hfile = CreateFile(fn, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE) goto done;

	fsize = GetFileSize(hfile, NULL);
	if (fsize == INVALID_FILE_SIZE || fsize <= 0) goto done;

	fbuf = malloc(fsize);
	if (!fbuf) goto done;

	if (!ReadFile(hfile, fbuf, fsize, &bytesread, NULL)) goto done;
	if (bytesread != fsize) goto done;

	*pmem = (unsigned char*)fbuf;
	fbuf = NULL; // Don't free the memory; it's now owned by the caller.
	*pfsize = fsize;
	retval = B2P_OK;

done:
	if (hfile != INVALID_HANDLE_VALUE) CloseHandle(hfile);
	if (fbuf) free(fbuf);
	return retval;
}

static int bmp2png_v3(const TCHAR *bmpfn, const TCHAR *pngfn)
{
	unsigned char *bmpp = NULL;
	DWORD bmpsize;
	LPBITMAPINFOHEADER pdib;
	LPBITMAPFILEHEADER pbmfh;
	void *pbits;
	PNGDIB *pngdib = NULL;
	int ret;
	int retval = B2P_ERROR;

	if (read_file_to_mem(bmpfn, &bmpp, &bmpsize) != B2P_OK) {
		_ftprintf(stderr, _T("Can't read BMP from file\n"));
		goto done;
	}

	pbmfh = (LPBITMAPFILEHEADER)bmpp;
	pbits = &bmpp[pbmfh->bfOffBits];
	pdib = (LPBITMAPINFOHEADER)&bmpp[14];

	pngdib = pngdib_d2p_init();
	if (!pngdib) goto done;
	pngdib_d2p_set_dib(pngdib, pdib, bmpsize - 14, pbits, 0);
	pngdib_d2p_set_png_filename(pngdib, pngfn);
	pngdib_d2p_set_gamma_label(pngdib, 1, PNGDIB_DEFAULT_FILE_GAMMA);
	ret = pngdib_d2p_run(pngdib);
	if (ret != PNGD_E_SUCCESS) {
		_ftprintf(stderr, _T("Error: %s (%d)\n"), pngdib_get_error_msg(pngdib), ret);
		goto done;
	}

done:
	if (bmpp) free(bmpp);
	if (pngdib) pngdib_done(pngdib);
	return retval;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		return 0;
	}

#if 0
	FILE* fin = _wfopen(argv[1], L"rb");
	fseek(fin, 0, SEEK_END);
	ULONG Size = ftell(fin);
	rewind(fin);
	unsigned char* Buffer = new unsigned char[Size];
	fread(Buffer, 1, Size, fin);
	fclose(fin);

	MemStream mem;
	mem.start = new BYTE[Size * 4];
	mem.cur = mem.start;
	mem.len = Size * 4;
	mem.total = 0;
	
	IMG_BITMAP_HEADER* pHeader = (IMG_BITMAP_HEADER*)Buffer;
	/*
	if (!EncodeBmpToPng(pHeader->Info.dwWidth, pHeader->Info.dwHeight, 32, nullptr, Buffer, &mem))
	{
		FILE* fout = _wfopen(L"out.png", L"wb");
		fwrite(mem.start, 1, mem.total, fout);
		fclose(fout);
	}
	*/

	std::vector<unsigned char> bmp;
	lodepng::load_buffer(bmp, Size, Buffer);
	std::vector<unsigned char> image;
	unsigned w, h;
	unsigned error = decodeBMP(image, w, h, bmp);

	std::vector<unsigned char> png;
	error = lodepng::encode(png, image, w, h, LCT_RGBA, 8);

	FILE* fout = _wfopen(L"out.png", L"wb");
	fwrite(&png[0], 1, png.size(), fout);
	fclose(fout);
#endif
	TCHAR *bmpfn, *pngfn;
	int ret;

	bmpfn = argv[1];
	pngfn = L"out.png";

	ret = bmp2png_v3(bmpfn, pngfn);
	return 0;
}

