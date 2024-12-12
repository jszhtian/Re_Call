#include <iostream>
#include <fstream>
using namespace std;

#pragma pack(2)        //两字节对齐，否则bmp_fileheader会占16Byte
struct bmp_fileheader
{
    unsigned short    bfType;        //若不对齐，这个会占4Byte
    unsigned long    bfSize;
    unsigned short    bfReverved1;
    unsigned short    bfReverved2;
    unsigned long    bfOffBits;
};

struct bmp_infoheader
{
    unsigned long    biSize;
    unsigned long    biWidth;
    unsigned long    biHeight;
    unsigned short    biPlanes;
    unsigned short    biBitCount;
    unsigned long    biCompression;
    unsigned long    biSizeImage;
    unsigned long    biXPelsPerMeter;
    unsigned long    biYpelsPerMeter;
    unsigned long    biClrUsed;
    unsigned long    biClrImportant;
};

fstream input_file;
fstream output_file;

struct bmp_fileheader bfh;
struct bmp_infoheader bih;

unsigned char* src_buff;
unsigned char* dst_buff;

void read_bmp_fileheader()
{

    input_file.read((char*)&bfh, sizeof(struct bmp_fileheader));

}

void read_bmp_infoheader()
{

    input_file.read((char*)&bih, sizeof(struct bmp_infoheader));

}

void read_bmp_data()
{
    src_buff = new unsigned char[bih.biHeight * bih.biWidth * 3];

    input_file.read((char*)src_buff, sizeof(unsigned char) * bih.biHeight * bih.biWidth * 3);

}

void bmp_rgb2gray()
{
    dst_buff = new unsigned char[bih.biHeight * bih.biWidth];
    int tmp;
    unsigned long j = 0;
    for (unsigned long i = 0; i < bih.biHeight * bih.biWidth * 3; i += 3)
    {
        tmp = (src_buff[i] + src_buff[i + 1] + src_buff[i + 2]) / 3;
        dst_buff[j++] = (unsigned char)tmp;
    }
}

void write_bmp_fileheader()
{

    bfh.bfSize = bfh.bfOffBits + 256 * 4 + bih.biWidth * bih.biHeight;        //256*4是调色板占的字节数
    bfh.bfOffBits = bfh.bfOffBits + 256 * 4;

    output_file.write((char*)&bfh, sizeof(struct bmp_fileheader));

}

void write_bmp_infoheader()
{
    bih.biBitCount = 8;            //8位灰度图

    output_file.write((char*)&bih, sizeof(struct bmp_infoheader));
}

void write_bmp_paltte()        //灰度图像需要调色板
{
    unsigned char j = 0;
    unsigned char z = 0;
    for (int i = 0; i < 1024; i += 4)
    {
        output_file.write((char*)&j, sizeof(unsigned char));
        output_file.write((char*)&j, sizeof(unsigned char));
        output_file.write((char*)&j, sizeof(unsigned char));

        output_file.write((char*)&z, sizeof(unsigned char));
        j++;
    }
}

void write_bmp_data()
{

    output_file.write((char*)dst_buff, sizeof(unsigned char) * bih.biWidth * bih.biHeight);

}

int main()
{
    input_file.open("c_mata.bmp", ios::binary | ios::in);
    output_file.open("c_mata.bmp.bmp", ios::binary | ios::out);

    read_bmp_fileheader();
    read_bmp_infoheader();
    read_bmp_data();

    bmp_rgb2gray();

    write_bmp_fileheader();
    write_bmp_infoheader();
    write_bmp_paltte();
    write_bmp_data();

    input_file.close();
    output_file.close();

    cout << "good job." << endl;
    cin.get();

    delete[] src_buff;
    delete[] dst_buff;
    return 0;
}