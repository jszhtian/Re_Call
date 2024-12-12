#include "stdafx.h"
#include <cstdio>
#include <Windows.h>
#include <string>

using std::wstring;

#pragma pack(1)
typedef struct 
{
	char magic[3];		/* "GR3" */
	WORD bits_count;
	ULONG width;
	ULONG height;
	ULONG dib_len;
	ULONG flag_bits;
} grp_header_t;

typedef struct {
	char magic[3];	/* "GR2" */
	WORD bits_count;	/* 8, 15, 16, 24 and default */
	DWORD width;
	DWORD height;
} grp_header_t_old;
#pragma pack()


void Decode(char* Buffer, ULONG Length)
{
	for (ULONG i = 0; i < Length; i++)
	{
		Buffer[i] = ~Buffer[i];
	}
}

inline unsigned char getbit_le(unsigned char byte, unsigned int pos)
{
	return !!(byte & (1 << pos));
}

void WriteBmp(const wstring& filename,
	unsigned char*   buff,
	unsigned long    len,
	unsigned long    width,
	unsigned long    height,
	unsigned short   depth_bytes);

DWORD grp3_lzss_decompress(unsigned char *uncompr, unsigned long uncomprlen,
	unsigned char *compr, unsigned int comprlen,
	unsigned char *flag_bitmap, unsigned int flag_bits)
{
	unsigned long act_uncomprLen = 0;
	unsigned int curbyte = 0;

	memset(uncompr, 0, uncomprlen);
	for (unsigned int i = 0; i < flag_bits; i++) {
		BYTE flag = flag_bitmap[i >> 3];

		if (!getbit_le(flag, i & 7)) {
			if (curbyte >= comprlen)
				goto out;
			if (act_uncomprLen >= uncomprlen)
				goto out;
			uncompr[act_uncomprLen++] = compr[curbyte++];
		}
		else {
			unsigned int copy_bytes, win_offset;

			if (curbyte >= comprlen)
				goto out;
			copy_bytes = compr[curbyte++];

			if (curbyte >= comprlen)
				goto out;
			win_offset = compr[curbyte++] << 8;

			win_offset = (copy_bytes | win_offset) >> 3;
			copy_bytes = copy_bytes & 0x7;
			copy_bytes++;

			for (unsigned int k = 0; k < copy_bytes; k++) {
				if (act_uncomprLen + k >= uncomprlen)
					goto out;

				uncompr[act_uncomprLen + k] = uncompr[act_uncomprLen - win_offset - 1 + k];
			}
			act_uncomprLen += copy_bytes;
		}
	}
out:
	return act_uncomprLen;
}

static void *my_malloc(DWORD len)
{
	return malloc(len);
}


int MyBuildBMPFile(BYTE *dib, DWORD dib_length,
	BYTE *palette, DWORD palette_length,
	DWORD width, DWORD height, DWORD bits_count,
	BYTE **ret, DWORD *ret_length, void *(*alloc)(DWORD))
{
	BITMAPFILEHEADER *bmfh;
	BITMAPINFOHEADER *bmiHeader;
	DWORD act_height, aligned_line_length, raw_line_length;
	DWORD act_dib_length, act_palette_length, output_length;
	BYTE *pdib, *pal, *output;
	unsigned int colors, pixel_bytes;

	if (alloc == (void *(*)(DWORD))malloc)
		return -1;

	raw_line_length = width * bits_count / 8;
	aligned_line_length = (width * bits_count / 8 + 3) & ~3;

	if (bits_count <= 8) {
		if (palette_length) {
			if (!palette || palette_length == 1024)
				colors = 256;
			else
				colors = palette_length / 3;
		}
		else
			colors = 1 << bits_count;
	}
	else
		colors = 0;
	act_palette_length = colors * 4;

	pixel_bytes = bits_count / 8;

	if (!(height & 0x80000000))
		act_height = height;
	else
		act_height = 0 - height;
	act_dib_length = aligned_line_length * act_height;

	output_length = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + act_palette_length + act_dib_length;

	output = (BYTE *)alloc(output_length);
	if (!output)
		return -1;

	bmfh = (BITMAPFILEHEADER *)output;
	bmfh->bfType = 0x4D42;
	bmfh->bfSize = output_length;
	bmfh->bfReserved1 = 0;
	bmfh->bfReserved2 = 0;
	bmfh->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + act_palette_length;
	bmiHeader = (BITMAPINFOHEADER *)(bmfh + 1);
	bmiHeader->biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader->biWidth = width;
	bmiHeader->biHeight = act_height;
	bmiHeader->biPlanes = 1;
	bmiHeader->biBitCount = (WORD)bits_count;
	bmiHeader->biCompression = BI_RGB;
	bmiHeader->biSizeImage = act_dib_length;
	bmiHeader->biXPelsPerMeter = 0;
	bmiHeader->biYPelsPerMeter = 0;
	bmiHeader->biClrUsed = bits_count <= 8 ? colors : 0;
	bmiHeader->biClrImportant = 0;

	pal = (BYTE *)(bmiHeader + 1);
	if (bits_count <= 8) {
		unsigned int p;

		if (!palette || !palette_length) {
			for (p = 0; p < colors; p++) {
				pal[p * 4 + 0] = p;
				pal[p * 4 + 1] = p;
				pal[p * 4 + 2] = p;
				pal[p * 4 + 3] = 0;
			}
		}
		else if (palette_length != act_palette_length) {
			for (p = 0; p < colors; p++) {
				pal[p * 4 + 0] = palette[p * 3 + 0];
				pal[p * 4 + 1] = palette[p * 3 + 1];
				pal[p * 4 + 2] = palette[p * 3 + 2];
				pal[p * 4 + 3] = 0;
			}
		}
		else
			memcpy(pal, palette, palette_length);
	}

	pdib = pal + act_palette_length;
	/* 有些系统的bmp结尾会多出2字节 */
	if (dib_length > act_dib_length)
		dib_length = act_dib_length;

	if (dib_length == act_dib_length)
		raw_line_length = aligned_line_length;

	if (act_height == height) {
		for (unsigned int y = 0; y < act_height; ++y) {
			for (unsigned int x = 0; x < width; ++x) {
				for (unsigned int p = 0; p < pixel_bytes; ++p)
					pdib[y * aligned_line_length + x * pixel_bytes + p] = dib[y * raw_line_length + x * pixel_bytes + p];
			}
		}
	}
	else {
		for (unsigned int y = 0; y < act_height; ++y) {
			for (unsigned int x = 0; x < width; ++x) {
				for (unsigned int p = 0; p < pixel_bytes; ++p)
					pdib[y * aligned_line_length + x * pixel_bytes + p] = dib[(act_height - y - 1) * raw_line_length + x * pixel_bytes + p];
			}
		}
	}

#if 0
	if (pixel_bytes == 4) {
		BYTE *rgba = pdib;
		for (unsigned int y = 0; y < act_height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				BYTE alpha = rgba[3];

				rgba[0] = (rgba[0] * alpha + 0xff * ~alpha) / 255;
				rgba[1] = (rgba[1] * alpha + 0xff * ~alpha) / 255;
				rgba[2] = (rgba[2] * alpha + 0xff * ~alpha) / 255;
				rgba += 4;
			}
		}
	}
#endif	
	*ret = output;
	*ret_length = output_length;
	return 0;
}

#define RGB555 1
#define RGB565 2


int MyBuildBMP16File(BYTE *dib, DWORD dib_length,
	DWORD width, DWORD height, BYTE **ret, DWORD *ret_length,
	unsigned long flags, DWORD *mask, void *(*alloc)(DWORD))
{
	BITMAPFILEHEADER *bmfh;
	BITMAPINFOHEADER *bmiHeader;
	DWORD act_height, aligned_line_length, raw_line_length;
	DWORD act_dib_length, act_mask_length = 0, output_length;
	BYTE *pdib, *pmask, *output;
	DWORD rgb565_mask[4] = { 0xF800, 0x07E0, 0x001F, 0 };
	//	DWORD rgb565_mask[3] = { 0xF800, 0x07E0, 0x001F };
	WORD biCompression;

	if ((alloc == (void *(*)(DWORD))malloc) || !alloc)
		return -1;

	raw_line_length = width * 2;
	aligned_line_length = (width * 2 + 3) & ~3;

	act_height = height;
	if (act_height & 0x80000000)
		act_height = 0 - height;
	act_dib_length = aligned_line_length * act_height;

	if (flags & RGB555) {
		biCompression = BI_RGB;
		mask = NULL;
	}
	else if (flags & RGB565) {
		biCompression = BI_BITFIELDS;
		mask = rgb565_mask;
		act_mask_length = sizeof(rgb565_mask);
	}
	else {
		if (!mask)
			return -1;

		biCompression = BI_BITFIELDS;
		act_mask_length = sizeof(rgb565_mask);
	}

	output_length = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + act_mask_length + act_dib_length;
	output = (BYTE *)alloc(output_length);
	if (!output)
		return -1;

	bmfh = (BITMAPFILEHEADER *)output;
	bmfh->bfType = 0x4D42;
	bmfh->bfSize = output_length;
	bmfh->bfReserved1 = 0;
	bmfh->bfReserved2 = 0;
	bmfh->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + act_mask_length;
	bmiHeader = (BITMAPINFOHEADER *)(bmfh + 1);
	bmiHeader->biSize = sizeof(BITMAPINFOHEADER) + act_mask_length;
	bmiHeader->biWidth = width;
	bmiHeader->biHeight = height;
	bmiHeader->biPlanes = 1;
	bmiHeader->biBitCount = 16;
	bmiHeader->biCompression = biCompression;
	bmiHeader->biSizeImage = act_dib_length;
	bmiHeader->biXPelsPerMeter = 0;
	bmiHeader->biYPelsPerMeter = 0;
	bmiHeader->biClrUsed = 0;
	bmiHeader->biClrImportant = 0;

	pmask = (BYTE *)(bmiHeader + 1);
	if (mask)
		memcpy(pmask, mask, act_mask_length);

	pdib = pmask + act_mask_length;
	if (act_dib_length != dib_length) {
		for (unsigned int y = 0; y < act_height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				pdib[y * aligned_line_length + x * 2 + 0] = dib[y * raw_line_length + x * 2 + 0];
				pdib[y * aligned_line_length + x * 2 + 1] = dib[y * raw_line_length + x * 2 + 1];
			}
		}
	}
	else
		memcpy(pdib, dib, dib_length);

	*ret = output;
	*ret_length = output_length;
	return 0;
}


static int NeXAS_grp_extract_resource(wstring& filename, FILE *pkg, ULONG grp_len)
{
	grp_header_t *grp_header;
	unsigned int flag_bitmap_len;
	BYTE *flag_bitmap, *compr, *uncompr;
	DWORD comprLen;


	grp_header = (grp_header_t *)malloc(grp_len);
	if (!grp_header)
	{
		-1;
	}
	rewind(pkg);
	fread(grp_header, 1, grp_len, pkg);

	//Decode((char*)(grp_header + sizeof(grp_header_t)), grp_len - sizeof(grp_header_t));

	flag_bitmap_len = (grp_header->flag_bits + 7) >> 3;
	flag_bitmap = (BYTE *)(grp_header + 1);
	comprLen = *(DWORD *)&flag_bitmap[flag_bitmap_len];
	compr = &flag_bitmap[flag_bitmap_len] + 4;

	uncompr = (BYTE *)malloc(grp_header->dib_len);
	if (!uncompr) 
	{
		free(grp_header);
		return -1;
	}

	DWORD act_uncomprlen = grp3_lzss_decompress(uncompr, grp_header->dib_len,
		compr, comprLen, flag_bitmap, grp_header->flag_bits);
	if (act_uncomprlen != grp_header->dib_len) 
	{
		wprintf(_T("grp decompress error occured (%d VS %d)\n"), act_uncomprlen, grp_header->dib_len);
		free(uncompr);
		free(grp_header);
		return 0;
	}

	BYTE *actual_data = nullptr;
	ULONG actual_data_length = 0;

	if (MyBuildBMPFile(uncompr, act_uncomprlen, NULL, 0, grp_header->width,
		0 - grp_header->height, grp_header->bits_count, (BYTE **)&actual_data,
		&actual_data_length, my_malloc)) 
	{
		free(uncompr);
		free(grp_header);
		return -1;
	}
	
	FILE* out = _wfopen(filename.c_str(), L"wb");
	fwrite(actual_data, 1, actual_data_length, out);
	fclose(out);

	free(uncompr);
	free(grp_header);

	return 0;
}


void baldrx_decompress(BYTE *uncompr, DWORD uncomprlen, BYTE *compr,
	DWORD comprlen, BYTE *flag_bitmap, DWORD flag_bits)
{
	DWORD act_uncomprlen = 0;

	for (DWORD i = 0; i < flag_bits; i++) {
		BYTE flag = flag_bitmap[i / 8];

		if (!getbit_le(flag, i & 7))
			uncompr[act_uncomprlen++] = *compr++;
		else {
			DWORD copy_bytes, win_offset;

			copy_bytes = *compr++;
			win_offset = *compr++ << 8;

			win_offset = (copy_bytes | win_offset) >> 5;
			copy_bytes = (copy_bytes & 0x1f) + 1;
			for (DWORD k = 0; k < copy_bytes; k++) {
				uncompr[act_uncomprlen] = uncompr[act_uncomprlen - win_offset - 1];
				act_uncomprlen++;
			}
		}
	}
}


int BaldrX_grp_extract_resource(wstring& filename, FILE *pkg)
{
	grp_header_t_old grp_header;
	BYTE *dib, *uncompr, *palette;
	DWORD uncomprlen, palette_size, dib_size;

	rewind(pkg);
	fread(&grp_header, 1, sizeof(grp_header), pkg);

	switch (grp_header.bits_count) 
	{
	case 8:
		uncomprlen = grp_header.width * grp_header.height + 0x300;
		break;
	case 15:
	case 16:
		uncomprlen = grp_header.width * grp_header.height * 2;
		break;
	case 24:
		uncomprlen = grp_header.width * grp_header.height * 3;
		break;
	default:
		uncomprlen = 4;
	}

	uncompr = (BYTE *)malloc(uncomprlen);
	if (!uncompr)
		return -1;

	if (grp_header.bits_count == 8) 
	{
		dib = uncompr;
		dib_size = grp_header.width * grp_header.height;
		palette = dib + dib_size;
		palette_size = 0x300;
	}
	else 
	{
		dib = uncompr;
		dib_size = uncomprlen;
		palette_size = 0;
		palette = NULL;
	}

	UCHAR Flag = ~grp_header.magic[2];
	if (Flag == '2') 
	{
		ULONG dib_len, flag_bits, comprlen;
		DWORD flag_bitmap_len;
		BYTE *flag_bitmap, *compr;

		fseek(pkg, sizeof(grp_header), SEEK_SET);
		fread(&dib_len, 1, 4, pkg);
		fread(&flag_bits, 1, 4, pkg);

		flag_bitmap_len = (flag_bits + 7) / 8;
		flag_bitmap = (BYTE *)malloc(flag_bitmap_len);
		if (!flag_bitmap) 
		{
			free(uncompr);
			return -1;
		}

		fseek(pkg, sizeof(grp_header) + 8, SEEK_SET);
		fread(flag_bitmap, 1, flag_bitmap_len, pkg);

		//Decode((char*)flag_bitmap, flag_bitmap_len);

		fseek(pkg, flag_bitmap_len + sizeof(grp_header) + 8, SEEK_SET);
		fread(&comprlen, 1, 4, pkg);

		compr = (BYTE *)malloc(comprlen);
		if (!compr) 
		{
			free(flag_bitmap);
			return -1;
		}

		fseek(pkg, flag_bitmap_len + sizeof(grp_header) + 12, SEEK_SET);
		fread(compr, 1, comprlen, pkg);
		//Decode((char*)compr, comprlen);

		baldrx_decompress(uncompr, uncomprlen, compr, comprlen, flag_bitmap, flag_bits);
		free(compr);
		free(flag_bitmap);
	}
	else 
	{
		fseek(pkg, sizeof(grp_header), SEEK_SET);
		fread(uncompr, 1, uncomprlen, pkg);
	}

	BYTE* actual_data = nullptr;
	ULONG actual_data_length = 0;
	if (grp_header.bits_count == 16) 
	{
		DWORD flag;

		// BALDR BULLET バルドバレット的16位图都是RGB555的
		if (false)
			flag = RGB555;
		else
			flag = RGB565;

		if (MyBuildBMP16File(dib, dib_size, grp_header.width,
			0 - grp_header.height, (BYTE **)&actual_data,
			(DWORD *)&actual_data_length, flag, NULL, my_malloc)) {
			free(uncompr);
			return -1;
		}
	}
	else if (grp_header.bits_count == 15) {
		if (MyBuildBMP16File(dib, dib_size, grp_header.width,
			0 - grp_header.height, (BYTE **)&actual_data,
			(DWORD *)&actual_data_length, RGB555, NULL, my_malloc)) {
			free(uncompr);
			return -1;
		}
	}
	else {
		if (grp_header.bits_count == 8) {
			BYTE *p = palette;
			for (int i = 0; i < 256; i++) {
				BYTE b;
				b = p[2];
				p[2] = p[0];
				p[0] = b;
				p += 3;
			}
		}

		if (MyBuildBMPFile(dib, dib_size, palette, palette_size, grp_header.width,
			0 - grp_header.height, grp_header.bits_count, (BYTE **)&actual_data,
			(DWORD *)&actual_data_length, my_malloc)) 
		{
			free(uncompr);
			return -1;
		}
	}
	free(uncompr);

	FILE* out = _wfopen(filename.c_str(), L"wb");
	fwrite(actual_data, 1, actual_data_length, out);
	fclose(out);

	return 0;
}



int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		return 0;
	}
	FILE *fin = _wfopen(argv[1], L"rb");
	if (fin == nullptr)
	{
		return 0;
	}
	
	fseek(fin, 0, SEEK_END);
	ULONG FileSize = ftell(fin);
	rewind(fin);

	UCHAR Magic[4] = { 0 };
	fread(Magic, 1, 3, fin);
	rewind(fin);

	Decode((char*)Magic, 3);

	printf("%s\n", Magic);
	wstring filename = wstring(argv[1]) + L".bmp";
	if (!strncmp((char*)Magic, "GR3", 3))
	{
		NeXAS_grp_extract_resource(filename, fin, FileSize);
	}
	else if (!strncmp((char*)Magic, "GR2", 3))
	{
		BaldrX_grp_extract_resource(filename, fin);
	}

	fclose(fin);
	return 0;
	
}



void WriteBmp(const wstring& filename,
	unsigned char*   buff,
	unsigned long    len,
	unsigned long    width,
	unsigned long    height,
	unsigned short   depth_bytes)
{
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;

	memset(&bmf, 0, sizeof(bmf));
	memset(&bmi, 0, sizeof(bmi));

	bmf.bfType = 0x4D42;
	bmf.bfSize = sizeof(bmf) + sizeof(bmi) + len;
	bmf.bfOffBits = sizeof(bmf) + sizeof(bmi);

	bmi.biSize = sizeof(bmi);
	bmi.biWidth = width;
	bmi.biHeight = height;
	bmi.biPlanes = 1;
	bmi.biBitCount = depth_bytes * 8;

	FILE* fd = _wfopen(filename.c_str(), L"wb");
	fwrite(&bmf, 1, sizeof(bmf), fd);
	fwrite(&bmi, 1, sizeof(bmi), fd);
	fwrite(buff, 1, len, fd);
	fclose(fd);
}
