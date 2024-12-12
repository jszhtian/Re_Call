// _Util.cpp : 定义静态库的函数。
//

#include "_Util.h"

#include "libpng/png.h"

struct MemStream
{
public:
    MemStream(void* buff1, uint32_t size1)
    {
        start = (uint8_t*)buff1;
        cur = start;
        size = size1;
    }

    uint8_t* start;
    uint8_t* cur;
    uint32_t size;
};

void ErrorToFileW(PVOID buffer, DWORD size)
{
    FILE* fp = nullptr;
    auto err = fopen_s(&fp, "LogErrorW.txt", "ab+");
    if (!fp)
    {
        MessageBoxW(0, L"Create error file error.", L"ErrorToFileW", MB_OK | MB_ICONERROR);
        ExitProcess(0);
    }
    fwrite(buffer, size, 1, fp);
    fwrite(L"\r\n", 4, 1, fp);
    fclose(fp);
}

void ErrorToFileA(PVOID buffer, DWORD size)
{
    FILE* fp = nullptr;
    auto err = fopen_s(&fp, "LogErrorA.txt", "ab+");
    if (!fp)
    {
        MessageBoxA(0, "Create error file error.", "ErrorToFileA", MB_OK | MB_ICONERROR);
        ExitProcess(0);
    }
    fwrite(buffer, size, 1, fp);
    fwrite("\r\n", 2, 1, fp);
    fclose(fp);
}

void ErrorW(const wchar_t* format, ...)
{
    wchar_t buffer[0x1000];
    va_list ap;
    va_start(ap, format);
    auto size = vswprintf_s(buffer, format, ap);
    std::wcout << buffer << std::endl;
    MessageBoxW(0, buffer, L"Error", MB_OK | MB_ICONERROR);
    ErrorToFileW(buffer, size * 2);
    va_end(ap);
}

void ErrorA(const char* format, ...)
{
    char buffer[0x1000];
    va_list ap;
    va_start(ap, format);
    auto size = vsprintf_s(buffer, format, ap);
    std::cout << buffer << std::endl;
    MessageBoxA(0, buffer, "Error", MB_OK | MB_ICONERROR);
    ErrorToFileA(buffer, size);
    va_end(ap);
}


/* pic work */

static inline uint32_t round_4(uint32_t val)
{
    return (val + 3) & ~3;
}

void PngRead(png_struct* png, png_byte* buff, png_size_t len)
{
    auto stream = (MemStream*)png_get_io_ptr(png);
    if (stream->cur - stream->start + len > stream->size)
    {
        png_error(png, "exceed limit in MemStream");
    }

    memcpy(buff, stream->cur, len);
    stream->cur += len;
}

bool ReadPngToBmp(BYTE* src, int size, int* width, int* height, int* bit_count, NakedMemory& dib, int opt)
{
    png_struct* png_ptr;
    png_info* info_ptr;
    int color_type, temp;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

    if (png_ptr == NULL)
    {
        ErrorA("Can't create png struct.");
        return false;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        ErrorA("Can't create png info.");
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        /* Free all of the memory associated with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        /* If we get here, we had a problem reading the file */
        ErrorA("png reading file fail");
        return false;
    }

    MemStream stream(src, size);
    png_set_read_fn(png_ptr, &stream, PngRead);

    png_read_info(png_ptr, info_ptr);

    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)width, (png_uint_32*)height, &bit_depth, &color_type,
        &temp, NULL, NULL);

    if (!(bit_depth == 8 && (color_type == PNG_COLOR_TYPE_RGBA || color_type == PNG_COLOR_TYPE_RGB)))
    {
        ErrorA("png file info error: bit_depth:%d, color_type:%d", bit_depth, color_type);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return false;
    }

    *bit_count = color_type == PNG_COLOR_TYPE_RGB ? 24 : 32;

    png_set_bgr(png_ptr);

    int bmp_row_bytes;
    if (opt & PngOptionRowNonAlign)
    {
        bmp_row_bytes = *width * (*bit_count / 8);
    }
    else
    {
        bmp_row_bytes = round_4(*width * (*bit_count / 8));
    }
    auto rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    if (rowbytes > bmp_row_bytes)
    {
        ErrorA("png fatal error: rowbytes:%d, bmp_rowbytes:%d", rowbytes, bmp_row_bytes);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return false;
    }
    NakedMemory dibdata(*height * bmp_row_bytes);

    auto rowPointers = new uint8_t * [*height];
    uint8_t* p;
    int diff;
    if (opt & PngOptionNonReverse)
    {
        p = (uint8_t*)dibdata.Get();
        diff = bmp_row_bytes;
    }
    else
    {
        p = (uint8_t*)dibdata.Get() + (*height - 1) * bmp_row_bytes;
        diff = -bmp_row_bytes;
    }
    for (int i = 0; i < *height; i++)
    {
        rowPointers[i] = p;
        p -= bmp_row_bytes;
    }

    png_read_image(png_ptr, rowPointers);

    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    delete[] rowPointers;

    dib = std::move(dibdata);
    return true;
}

NakedMemory Dib32To24(int width, int height, NakedMemory& old, NakedMemory& alpha)
{
    if (width * 4 * height != old.GetSize())
    {
        ErrorA("Dib size not correct!");
        return NakedMemory();
    }

    auto nrowbytes = round_4(width * 3);
    auto arowbytes = round_4(width);
    auto orowbytes = width * 4;
    NakedMemory newd(nrowbytes * height);
    NakedMemory newa(arowbytes * height);
    auto nrows = (uint8_t*)newd.Get();
    auto orows = (uint8_t*)old.Get();
    auto arows = (uint8_t*)newa.Get();
    for (int i = 0; i < height; i++)
    {
        auto pn = nrows;
        auto po = orows;
        auto pa = arows;
        for (int j = 0; j < width; j++)
        {
            pn[0] = po[0];
            pn[1] = po[1];
            pn[2] = po[2];
            *pa++ = po[3];
            pn += 3;
            po += 4;
        }
        nrows += nrowbytes;
        orows += orowbytes;
        arows += arowbytes;
    }
    alpha = std::move(newa);
    return std::move(newd);
}

NakedMemory Dib32To24_NoAlpha(int width, int height, NakedMemory& old/*, NakedMemory& alpha*/)
{
    if (width * 4 * height != old.GetSize())
    {
        ErrorA("Dib size not correct!");
        return NakedMemory();
    }

    auto nrowbytes = round_4(width * 3);
    auto arowbytes = round_4(width);
    auto orowbytes = width * 4;
    NakedMemory newd(nrowbytes * height);
    NakedMemory newa(arowbytes * height);
    auto nrows = (uint8_t*)newd.Get();
    auto orows = (uint8_t*)old.Get();
    auto arows = (uint8_t*)newa.Get();
    for (int i = 0; i < height; i++)
    {
        auto pn = nrows;
        auto po = orows;
        auto pa = arows;
        for (int j = 0; j < width; j++)
        {
            pn[0] = po[0];
            pn[1] = po[1];
            pn[2] = po[2];
            *pa++ = po[3];
            pn += 3;
            po += 4;
        }
        nrows += nrowbytes;
        orows += orowbytes;
        arows += arowbytes;
    }
    //alpha = std::move(newa);
    return std::move(newd);
}

NakedMemory Dib24To32(int width, int height, NakedMemory& old)
{
    auto orowbytes = round_4(width * 3);
    if (orowbytes * height != old.GetSize())
    {
        ErrorA("Dib size not correct!");
        return NakedMemory();
    }

    auto nrowbytes = width * 4;
    NakedMemory newd(nrowbytes * height);
    auto nrows = (uint8_t*)newd.Get();
    auto orows = (uint8_t*)old.Get();
    for (int i = 0; i < height; i++)
    {
        auto pn = nrows;
        auto po = orows;
        for (int j = 0; j < width; j++)
        {
            pn[0] = po[0];
            pn[1] = po[1];
            pn[2] = po[2];
            pn[4] = 0;
            pn += 4;
            po += 3;
        }
        nrows += nrowbytes;
        orows += orowbytes;
    }
    return std::move(newd);
}

void WriteBmp(const char* filename, NakedMemory& dib_info, long width, long height, short bpp)
{
    BITMAPFILEHEADER bmf;
    BITMAPINFOHEADER bmi;

    memset(&bmf, 0, sizeof(bmf));
    memset(&bmi, 0, sizeof(bmi));

    bmf.bfType = 0x4D42;
    bmf.bfSize = sizeof(bmf) + sizeof(bmi) + dib_info.GetSize();
    bmf.bfOffBits = sizeof(bmf) + sizeof(bmi);

    bmi.biSize = sizeof(bmi);
    bmi.biWidth = width;
    bmi.biHeight = height;
    bmi.biPlanes = 1;
    bmi.biBitCount = bpp;
    char FileName[MAX_PATH] = { 0 };
    sprintf(FileName, "%s.bmp", filename);
    FILE* fd = nullptr;
    auto err = fopen_s(&fd, FileName, "wb");
    if (!fd)
    {
        std::cout << "Can't create bmp file:" << FileName << std::endl;
        return;
    }
    fwrite(&bmf, 1, sizeof(bmf), fd);
    fwrite(&bmi, 1, sizeof(bmi), fd);
    fwrite(dib_info.Get(), 1, dib_info.GetSize(), fd);
    fclose(fd);
}