// FTtest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <Windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H

typedef uint8_t byte;

int main()
{
	FT_Error error;
	FT_Library  library;
	FT_Face face;

	error = FT_Init_FreeType(&library);
	if (error)
	{
		std::cout << "Failed to initialize freetype library." << std::endl;
		return 1;
	}

	error = FT_New_Face(library, "CNFONT.otf", 0, &face);
	if (error)
	{
		std::cout << "Failed to create font face." << std::endl;
		return 1;
	}

	constexpr int FONT_WIDTH = 18;
	constexpr int FONT_HEIGHT = 18;

	error = FT_Set_Pixel_Sizes(face, FONT_WIDTH * 0.8, FONT_HEIGHT * 0.8);
	if (error)
	{
		std::cout << "Failed to set font sizes." << std::endl;
		return 1;
	}

	auto glyph_index = FT_Get_Char_Index(face, (FT_ULong)L'测');

	error = FT_Load_Glyph(face, glyph_index, FT_LOAD_TARGET_NORMAL);
	if (error)
	{
		std::cout << "Failed to load font glyph." << std::endl;
		return 1;
	}

	error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	if (error)
	{
		std::cout << "Failed to render font glyph." << std::endl;
		return 1;
	}

	if (face->glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
	{
		std::cout << "Pixel mode not supported." << std::endl;
		return 1;
	}

	face->glyph->metrics.horiBearingX >> 6;

	auto& bitmap = face->glyph->bitmap;

	std::cout << "width: " << bitmap.width << std::endl;
	std::cout << "height: " << bitmap.rows << std::endl;

	std::vector<byte> output;

	size_t output_width = std::ceil(FONT_WIDTH / 2.0);
	size_t output_height = FONT_HEIGHT;

	output.resize(output_width * output_height);

	// 字形bitmap在output中的偏移量，太大会导致越界
	size_t offset_x = 1;
	size_t offset_y = 1;

	// 从bitmap中指定坐标读取像素
	auto get_pixel = [&bitmap](size_t x, size_t y) -> byte
	{
		// 检查是否越界，调试用
		assert(x * y < bitmap.width* bitmap.rows);

		return bitmap.buffer[y * bitmap.width + x];
	};

	// 设置GBA-4BPP bitmap中指定坐标处的像素值
	auto set_pixel = [&output, output_width](size_t x, size_t y, byte alpha)
	{
		// 算出该像素存储在哪个字节
		size_t pos = y * output_width + (x / 2);
		// 算出像素在该字节内的偏移量
		size_t shift = (2 - 1 - (x % 2)) * 4;

		// 把该像素值清零
		output[pos] &= ~(0xF << shift);
		// 设置新的像素值
		output[pos] |= (alpha & 0xF) << shift;
	};

	for (uint32_t y = 0; y < bitmap.rows; y++)
	{
		for (uint32_t x = 0; x < bitmap.width; x += 2)
		{
			BYTE pixel0 = (x + 0 < bitmap.width) ? get_pixel(x + 0, y) : 0;
			BYTE pixel1 = (x + 1 < bitmap.width) ? get_pixel(x + 1, y) : 0;

			pixel0 = pixel0 * 0xF / 0xFF;
			pixel1 = (pixel1 * 0xF / 0xFF) << 4;

			BYTE val = pixel1 | pixel0;

			size_t pos = offset_y + y * output_width + (offset_x + x / 2);

			output[pos] = val;
		}
	}

	std::ofstream fout("dump.dmp", std::ios::binary);
	if (!fout)
	{
		std::cout << "Failed to create output file." << std::endl;
		return 1;
	}

	fout.write((char*)output.data(), output.size());
	fout.flush();
	fout.close();
	system("pause");
	return 0;

//ERROR:
//	if (face)
//		FT_Done_Face(face);
//
//	if (library)
//		FT_Done_FreeType(library);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
