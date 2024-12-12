// FontStudy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
using namespace std;

FIXED FixedFromDouble(double d)
{
 long l;
 l = (long)(d * 65536L);
 return *(FIXED*)&l;
}

//设置字体图形变换矩阵。
void SetMat(LPMAT2 lpMat)
{
 lpMat->eM11 = FixedFromDouble(2);
 lpMat->eM12 = FixedFromDouble(0);
 lpMat->eM21 = FixedFromDouble(0);
 lpMat->eM22 = FixedFromDouble(2);
 }

int main()
{
   // std::cout << "Hello World!\n";
    HDC hDC = GetDC(NULL);
    TCHAR chText = L'我';
    MAT2 mat2;
    SetMat(&mat2);
    GLYPHMETRICS gm;
    DWORD dwNeedSize = GetGlyphOutline(hDC, chText, GGO_BITMAP, &gm, 0, NULL, &mat2);
    if (dwNeedSize)
    {
        LPBYTE lpBuf = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeedSize);
        GetGlyphOutline(hDC, chText, GGO_BITMAP, &gm, dwNeedSize, lpBuf, &mat2);
        FILE* fp = fopen("dump", "wb");
        fwrite(lpBuf, dwNeedSize, 1, fp);
        fclose(fp);
    }
    system("pause");
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
