// FreeTypeTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <ft2build.h>
#include <cmath>
#include <algorithm>

#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H

#define FONT_WIDTH 30
#define FONT_HEIGHT 30

#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

char Get_Pixel(BYTE* Bitmap, int w, int h, int x, int y)
{
    //return Bitmap[(y * w * 4) + (x * 4)];
    return Bitmap[y*w+x];
}

#if 1
int main()
{
    FT_Library    library;
    FT_Face       face;

    FT_GlyphSlot  slot;
    FT_Glyph          glyph;
    FT_Matrix     matrix;                 /* transformation matrix */
    FT_Vector     pen;                    /* untransformed origin  */
    FT_Error      error;

    int           target_height;
    int           n, num_chars;

    unsigned int moji = L'这';

    error = FT_Init_FreeType(&library);              /*初始化freetype库*/
    cout << "Init FreeType" << endl;
    error = FT_New_Face(library,"CNFONT.otf", 0, &face); /*打开字体文件*/
    cout << "SetFont" << endl;
    //FT_Set_Char_Size(face, 30 << 6, 30 << 6, 72, 90);
    FT_Set_Pixel_Sizes(face, FONT_WIDTH, FONT_HEIGHT);

    FT_Select_Charmap(face, FT_ENCODING_UNICODE);

    FT_UInt glyph_idx = FT_Get_Char_Index(face, moji);


    FT_Load_Glyph(face, glyph_idx, FT_LOAD_DEFAULT);

    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);


    cout << face->glyph->bitmap.pitch << endl;
    cout << face->glyph->bitmap.rows << endl;
    
   /*
    cv::Mat resized_down;
    cv::Mat image(cv::Size(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        CV_8UC1, face->glyph->bitmap.buffer, cv::Mat::AUTO_STEP);
    cv::resize(image, resized_down, cv::Size(30, 30), cv::INTER_LINEAR);
    int w = face->glyph->bitmap.pitch;
    int h = face->glyph->bitmap.rows;
    int t = (60 - h) / 2;
    int b = 60 - t - h;
    int l = (60 - w) / 2;
    int r = 60 - w - l;
    //cv::copyMakeBorder(image, resized_down, t, b, l, r, cv::BORDER_CONSTANT, 0);
    //cv::resize(resized_down, resized_down, cv::Size(30, 30), cv::INTER_AREA);
    std::vector<char> outputBuf;
    BYTE* DstBuf = new BYTE[30 * 30];
    for (int y = 0; y < 30; y++)
    {
        for (int x = 0; x < 30; x += 2)
        {
            auto PixCol1 = resized_down.at<char>(y, x);
            auto PixCol2 = resized_down.at<char>(y, x + 1);
            PixCol1 = PixCol1 * 0xF / 0xFF;
            PixCol2 = (PixCol2 * 0xF / 0xFF) << 4;
            char val = (char)PixCol2 | PixCol1;
            outputBuf.push_back(val);
        }
    }

    */

    ///*
    int w = face->glyph->bitmap.width / 2;
    int h = face->glyph->bitmap.rows / 2;
    int t = (30 - h) / 2;
    int b = 30 - t - h;
    int l = (30 - w) / 2;
    int r = 30 - w - l;

    BYTE* dst = face->glyph->bitmap.buffer;

    std::vector<char> output;

    size_t output_w = std::ceil(FONT_WIDTH / 2.0);
    size_t output_h = FONT_HEIGHT;

    output.resize(output_w * output_h);

    size_t offset_x = 4;
    size_t offset_y = 4;

    auto& bitmap = face->glyph->bitmap;

    auto get_pixel = [&bitmap](size_t x, size_t y) -> byte
    {
        // 检查是否越界，调试用
        assert(x * y < bitmap.width* bitmap.rows);

        return bitmap.buffer[y * bitmap.width + x];
    };

    // 设置GBA-4BPP bitmap中指定坐标处的像素值
    auto set_pixel = [&output, output_w](size_t x, size_t y, byte alpha)
    {
        // 算出该像素存储在哪个字节
        size_t pos = y * output_w + (x / 2);
        // 算出像素在该字节内的偏移量
        size_t shift = (2 - 1 - (x % 2)) * 4;

        // 把该像素值清零
        output[pos] &= ~(0xF << shift);
        // 设置新的像素值
        output[pos] |= (alpha & 0xF) << shift;
    };

    for (size_t y = 0; y < face->glyph->bitmap.rows; y++)
    {
        for (size_t x = 0; x < face->glyph->bitmap.width; x += 2)
        {
            BYTE pix0 = (x + 0 < face->glyph->bitmap.width) ?
                face->glyph->bitmap.buffer[y * face->glyph->bitmap.width + x + 0] : 0;
            BYTE pix1 = (x + 1 < face->glyph->bitmap.width) ?
                face->glyph->bitmap.buffer[y * face->glyph->bitmap.width + x + 1] : 0;

            pix0 = pix0 / 255.0 * 15.0;
            pix1 = pix1 / 255.0 * 15.0;

            BYTE pack = ((pix0 & 0xF) << 4) | (pix1 & 0xF);

            //output.push_back(pack);
            output[(y + offset_y) * output_w + ((x + offset_x) / 2)] = pack;
        }
    }
    //*/
    //memcpy(face->glyph->bitmap.buffer, outputBuf.data(), outputBuf.size());
    FILE* fp = fopen("dump.dmp", "wb");
    fwrite(output.data(), output.size(), 1, fp);
    fclose(fp);

    //Draw_Bitmap(face->glyph->bitmap.buffer, face->glyph->bitmap.pitch, face->glyph->bitmap.rows);
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    system("pause");
    return 0;
}
#endif
#if 0
GLYPHMETRICS Draw()
{
    DWORD chText = '测';
    LOGFONT logfont;
    logfont.lfHeight = 24;
    logfont.lfWidth = 0;
    logfont.lfEscapement = 0;
    logfont.lfOrientation = 0;
    logfont.lfItalic = 0;
    logfont.lfUnderline = 0;
    logfont.lfStrikeOut = 0;
    logfont.lfCharSet = GB2312_CHARSET;
    logfont.lfOutPrecision = 0;
    logfont.lfClipPrecision = 0;
    logfont.lfQuality = 0;
    logfont.lfPitchAndFamily = 0;
    wcscpy(logfont.lfFaceName, L"黑体");
    HFONT hFont = CreateFontIndirect(&logfont);
    HDC hDC = CreateCompatibleDC(NULL);;
    SelectObject(hDC, hFont);
    MAT2 mat2;
    mat2.eM11.value = 1;
    mat2.eM11.fract = 0;
    mat2.eM12.value = 0;
    mat2.eM12.fract = 0;
    mat2.eM21.value = 0;
    mat2.eM21.fract = 0;
    mat2.eM22.value = 1;
    mat2.eM22.fract = 0;
    GLYPHMETRICS gm;
    UINT NeedSize = GetGlyphOutlineA(hDC, chText, GGO_GRAY8_BITMAP, &gm, 0, NULL, &mat2);
    if (NeedSize > 0 && NeedSize < 0xFFFF)
    {
        char* lpBuf = (char*)malloc(NeedSize);
        if (lpBuf)
        {
            GetGlyphOutlineA(hDC, chText, GGO_GRAY8_BITMAP, &gm, NeedSize, lpBuf, &mat2);       
            for (UINT j = 0; j < NeedSize; j++)
                if (lpBuf[j] == 0)
                    lpBuf[j] = 0;
                else if (lpBuf[j] == 0x40)
                    lpBuf[j] = 0xFF;
                else
                    lpBuf[j] <<= 2;
            //WritePng(fp, NeedSize / gm.gmBlackBoxY, gm.gmBlackBoxY, lpBuf);
            write_bmp("dump", lpBuf, NeedSize, NeedSize / gm.gmBlackBoxY, gm.gmBlackBoxY, 24);
            FILE* fp = fopen("dump.dmp", "wb");
            fwrite(lpBuf, 1, NeedSize, fp);
            fclose(fp);
            free(lpBuf);
        }
    }
    else
        wprintf(L"所需大小错误！ size:0x%X\n", NeedSize);
    DeleteObject(hFont);
    DeleteDC(hDC);
    return gm;
}
int main()
{
    Draw();
    system("pause");
    return 0;
}
#endif
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
