// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
#include <Windows.h>
#include <iostream>
#include <stdint.h>
#include <string>
#include <memory.h>
#include <stdio.h>
#include <stdarg.h>
#pragma comment(lib, "libpng16.lib")


class NakedMemory
{
public:
    NakedMemory() :buff_(nullptr), size_(0) {}
    NakedMemory(uint32_t size);
    NakedMemory(const void* buff, uint32_t size);
    NakedMemory(const NakedMemory& other);
    NakedMemory(NakedMemory&& other) noexcept;
    ~NakedMemory();

    void* Get() const
    {
        return buff_;
    }
    uint32_t GetSize() const
    {
        return size_;
    }

    void SetSize(uint32_t size);

    NakedMemory& operator=(const NakedMemory& other);
    NakedMemory& operator=(NakedMemory&& other) noexcept;

private:
    uint8_t* buff_;
    uint32_t size_;
};

void ErrorW(const wchar_t* format, ...);
void ErrorA(const char* format, ...);

enum PngOption {
    PngOptionRowAlign4 = 0,
    PngOptionReverse = 0,

    PngOptionRowNonAlign = 1,
    PngOptionNonReverse = 1 << 1,
};

bool ReadPngToBmp(BYTE* src, int size, int* width, int* height, int* bit_count, NakedMemory& dib, int opt);
NakedMemory Dib32To24(int width, int height, NakedMemory& old, NakedMemory& alpha);
NakedMemory Dib24To32(int width, int height, NakedMemory& old);
void WriteBmp(const char* filename, NakedMemory& dib_info, long width, long height, short bpp);

static NakedMemory Dib2Mask(int w, int h, NakedMemory& old)
{
    NakedMemory newd(w * h);
    auto nrows = (uint8_t*)newd.Get();
    auto orows = (uint8_t*)old.Get();
    for (int i = 0; i < h; i++)
    {
        auto pn = nrows;
        auto po = orows;
        for (int j = 0; j < w; j++)
        {
            pn[0] = po[3];
            pn += 1;
            po += 4;
        }
        nrows += w;
        orows += (w * 4 + 3) & ~3;
    }
    return std::move(newd);
}