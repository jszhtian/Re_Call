#include "BmpWriter.h"
#include <malloc.h>
#include <Windows.h>

int MySaveBMPFile(const char *name, BYTE *buf, DWORD len,
	BYTE *palette, DWORD palette_size,
	DWORD width, DWORD height, DWORD bits_count,
	DWORD need_align)
{
	FILE* file;
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmiHeader;
	DWORD line_len, image_size, tmp_height, raw_line_len;
	CHAR fname[256];
	BYTE *dib_buf = NULL;
	unsigned int use_gray_palette = 0;
	unsigned int colors;

	raw_line_len = width * bits_count / 8;
	line_len = (width * bits_count / 8 + 3) & ~3;
	colors = 1 << bits_count;

	tmp_height = height;
	if (tmp_height & 0x80000000)
		tmp_height = 0 - height;
	image_size = line_len * tmp_height;

	if ((raw_line_len != line_len) && (need_align & WRITE_BMP_ALIGN)) {
		dib_buf = (BYTE *)GlobalAlloc(GMEM_FIXED, image_size);
		if (!dib_buf)
			return -1;

		for (unsigned int y = 0; y < tmp_height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				for (unsigned int p = 0; p < bits_count / 8; p++)
					dib_buf[y * line_len + x * bits_count / 8 + p] = buf[y * raw_line_len + x * bits_count / 8 + p];
			}
		}
	}

	if (bits_count <= 8) {
		if (!palette && !palette_size) {
			palette_size = (1 << bits_count) * 4;
			palette = (BYTE *)GlobalAlloc(GMEM_FIXED, palette_size);
			if (!palette) {
				if (dib_buf)
					GlobalFree(dib_buf);
				return -1;
			}

			for (unsigned int p = 0; p < colors; p++) {
				palette[p * 4 + 0] = p;
				palette[p * 4 + 1] = p;
				palette[p * 4 + 2] = p;
				palette[p * 4 + 3] = 0;
			}

			use_gray_palette = 1;
		}
		else if (need_align & WRITE_BMP_ALIGN) {
			BYTE *tmp;

			tmp = (BYTE *)GlobalAlloc(GMEM_FIXED, (1 << bits_count) * 4);
			if (!tmp) {
				if (dib_buf)
					GlobalFree(dib_buf);
				return -1;
			}

			for (unsigned int p = 0; p < colors; p++) {
				tmp[p * 4 + 0] = palette[p * 3 + 0];
				tmp[p * 4 + 1] = palette[p * 3 + 1];
				tmp[p * 4 + 2] = palette[p * 3 + 2];
				tmp[p * 4 + 3] = 0;
			}
			palette = tmp;
			palette_size = colors * 4;
		}
	}
	else {
		palette_size = 0;
		palette = NULL;
	}

	bmfh.bfType = 0x4D42;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ palette_size + image_size;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ palette_size;

	bmiHeader.biSize = sizeof(bmiHeader);
	bmiHeader.biWidth = width;
	bmiHeader.biHeight = height;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = (WORD)bits_count;
	bmiHeader.biCompression = BI_RGB;
	bmiHeader.biSizeImage = image_size;
	bmiHeader.biXPelsPerMeter = 0;
	bmiHeader.biYPelsPerMeter = 0;
	bmiHeader.biClrUsed = 0;
	bmiHeader.biClrImportant = 0;

	memset(fname, 0, sizeof(fname));
	strcpy(fname, name);

	file = fopen(fname, "wb");
	if (file == 0) {
		if (use_gray_palette)
			GlobalFree(palette);
		if (dib_buf)
			GlobalFree(dib_buf);
		return -1;
	}

	fwrite(&bmfh, 1, sizeof(bmfh), file);
	fwrite(&bmiHeader, 1, sizeof(bmiHeader), file);

	if (palette_size && palette) 
	{
		fwrite(palette, 1, palette_size, file);

		if (use_gray_palette)
			GlobalFree(palette);
	}

	if (dib_buf) {
		fwrite(dib_buf, 1, image_size, file);
		GlobalFree(dib_buf);
	}
	else 
	{
		fwrite(buf, 1, image_size, file);
	}
	fclose(file);
	return 0;
}


void write_bmp(const string& out_filename,
	unsigned char* out_buff,
	unsigned long out_len,
	unsigned long width,
	unsigned long height,
	unsigned long bits,
	unsigned long flag)
{
	MySaveBMPFile(out_filename.c_str(), out_buff, out_len,
		nullptr, 0, width, flag & WRITE_BMP_FLIP ? height : -height, bits * 8,
		flag & WRITE_BMP_ALIGN ? WRITE_BMP_ALIGN : 0);
}