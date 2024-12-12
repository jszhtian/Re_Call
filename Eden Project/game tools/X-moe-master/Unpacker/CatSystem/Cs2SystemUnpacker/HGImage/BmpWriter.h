#pragma once

#include <string>

using std::string;

#define WRITE_BMP_ALIGN 1
#define WRITE_BMP_FLIP  2

void write_bmp(const string& out_filename,
	unsigned char* out_buff,
	unsigned long out_len,
	unsigned long width,
	unsigned long height,
	unsigned long bits,
	unsigned long flag);

