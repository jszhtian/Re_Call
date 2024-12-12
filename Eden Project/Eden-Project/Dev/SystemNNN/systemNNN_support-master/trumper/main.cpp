//
// main.cpp
//

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>


#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\namelist.h"



#if !defined _DEBUG
//#pragma comment(lib,"..\\nyanlib\\lib\\nyanLib2005.lib")
//#pragma comment(lib,"..\\nyanlib\\lib\\nnnUtilLib2005.lib")
#else
//#pragma comment(lib,"..\\nyanlib\\lib\\nyanLib2005D.lib")
//#pragma comment(lib,"..\\nyanlib\\lib\\nnnUtilLib2005D.lib")
#endif

//#define __TAIKEN_VERSION__




#include "main.h"

BOOL GetInitGameParam(int* lpVar, LPSTR name);
BOOL GetInitGameString(LPSTR* lpStr, LPSTR name);
BOOL LoadSetupFile(LPSTR filenameonly, int varKosuu = 64);

void LoadOrgPic(void);
void LoadPartsPic(void);
void SaveMakedPic(void);

void PutSmallMark(int st,int n,int putX,int putY,int revFlag);
void PutMiddleMark(int st,int n,int putX,int putY,int revFlag);
void PutLargeMark(int st,int n,int putX,int putY,int revFlag);
void PutNumber(int st,int n,int putX,int putY,int revFlag);

void PutRoutine(int putX,int putY,int srcX,int srcY,int sizeX,int sizeY,int rev);


CNameList* m_setup = NULL;


int m_cardSizeX = 96;
int m_cardSizeY = 144;

int m_largeSizeX = 28;
int m_largeSizeY = 28;

int m_middleSizeX = 21;
int m_middleSizeY = 21;

int m_smallSizeX = 10;
int m_smallSizeY = 11;

int m_fontSizeX = 8;
int m_fontSizeY = 8;

int m_middleSrcY = 28;
int m_smallSrcY = 28+21;
int m_fontSrcY = 28+21+11;


int m_number1PutX = 3;
int m_number1PutY = 12;

int m_mark1PutX = 2;
int m_mark1PutY = 23;



int m_number2PutX = 85;
int m_number2PutY = 124;

int m_mark2PutX = 84;
int m_mark2PutY = 110;



int m_middle1PutX = 61;
int m_middle1PutY = 13;
int m_middle2PutX = 14;
int m_middle2PutY = 109;


int m_putKosuu[] ={1,2,3,4,5,6,7,8,9,10,2,2,2};
int m_normalKosuu[] = {1,1,2,2,3,4,5,5,5,5,1,1,1};
int m_sizeFlag[] = {0,0,0,0,0,0,0,0,0,0,1,1,1};

int m_middlePrintFlag = 1;
int m_worldPrintFlag = 1;
int m_markPrintFlag = 1;
int m_numberPrintFlag = 1;

int m_orgLoadFlag = 0;
int m_partsLoadFlag = 0;

int m_miniFlag = 0;
int m_3d = 0;

LPSTR m_filenameBase = "trumpbase";
LPSTR m_filenameParts = "trumpparts";
LPSTR m_filenameMake = "trumpseat";

BITMAPFILEHEADER m_baseHeader;
BITMAPFILEHEADER m_partsHeader;
BITMAPFILEHEADER m_maskHeader;

BITMAPINFOHEADER m_baseInfo;
BITMAPINFOHEADER m_partsInfo;
BITMAPINFOHEADER m_maskInfo;

char* m_partsPic = NULL;
char* m_basePic = NULL;
char* m_maskPic = NULL;
int m_paletteTable[256];


int m_putXY[]=
{
	34,58,
	34,12, 34,104,
	34,12, 34,58, 34,104,
	12,12, 56,12, 12,104, 56,104,
	12,12, 56,12, 34,58, 12,104, 56,104,
	12,12, 56,12, 12,58, 56,58, 12,104, 56,104,
	12,12, 56,12, 34,35, 12,58, 56,58, 12,104, 56,104,
	12,12, 56,12, 34,35, 12,58, 56,58, 34,82, 12,104, 56,104,
	12,12, 56,12, 12,43, 56,43, 34,58, 12,74, 56,74, 12,105, 56,105,
	12,12, 56,12, 12,43, 56,43, 34,27, 12,74, 56,74, 34,90, 12,105, 56,105,
	61,13, 14,109,
	61,13, 14,109,
	61,13, 14,109,
};


int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	m_setup = new CNameList();
	BOOL setupFlag = m_setup->LoadFile("trumper.txt");

	if (setupFlag)
	{
		GetInitGameParam(&m_miniFlag,"miniFlag");
		GetInitGameParam(&m_3d,"flag3D");

		GetInitGameParam(&m_cardSizeX,"cardSizeX");
		GetInitGameParam(&m_cardSizeY,"cardSizeY");

		GetInitGameParam(&m_largeSizeX,"largeSizeX");
		GetInitGameParam(&m_largeSizeY,"largeSizeY");

		GetInitGameParam(&m_middleSizeX,"middleSizeX");
		GetInitGameParam(&m_middleSizeY,"middleSizeY");

		GetInitGameParam(&m_smallSizeX,"smallSizeX");
		GetInitGameParam(&m_smallSizeY,"smallSizeY");

		GetInitGameParam(&m_fontSizeX,"fontSizeX");
		GetInitGameParam(&m_fontSizeY,"fontSizeY");

		if (m_miniFlag)
		{
			m_cardSizeX /= 2;
			m_cardSizeY /= 2;
			m_largeSizeX /= 2;
			m_largeSizeY /= 2;
			m_middleSizeX /= 2;
			m_middleSizeY /= 2;
			m_smallSizeX /= 2;
			m_smallSizeY /= 2;
			m_fontSizeX = m_fontSizeX / 2 + 1;
			m_fontSizeY = m_fontSizeY / 2 + 1;



		}

		if (m_miniFlag)
		{
			m_middleSrcY = m_largeSizeY;
			m_smallSrcY = m_middleSrcY + m_middleSizeY;
			m_fontSrcY = m_smallSrcY + m_smallSizeY;
		}

		m_middleSrcY = m_largeSizeY;
		m_smallSrcY = m_largeSizeY + m_middleSizeY;
		m_fontSrcY = m_largeSizeY + m_middleSizeY + m_smallSizeY;

		GetInitGameParam(&m_worldPrintFlag,"worldPrintFlag");
		GetInitGameParam(&m_middlePrintFlag,"middlePrintFlag");

		GetInitGameParam(&m_markPrintFlag,"markPrintFlag");
		GetInitGameParam(&m_numberPrintFlag,"numberPrintFlag");

		GetInitGameString(&m_filenameParts,"filenameParts");
		GetInitGameString(&m_filenameBase,"filenameBase");
		GetInitGameString(&m_filenameMake,"filenameMake");

		for (int i=0;i<13;i++)
		{
			char name[256];
			wsprintf(name,"normalNumber%d",i+1);
			int d = m_normalKosuu[i];
			GetInitGameParam(&d,name);
			m_normalKosuu[i] = d;
		}


		GetInitGameParam(&m_number1PutX,"number1PutX");
		GetInitGameParam(&m_number1PutY,"number1PutY");
		GetInitGameParam(&m_number2PutX,"number2PutX");
		GetInitGameParam(&m_number2PutY,"number2PutY");

		GetInitGameParam(&m_mark1PutX,"smallMark1PutX");
		GetInitGameParam(&m_mark1PutY,"smallMark1PutY");
		GetInitGameParam(&m_mark2PutX,"smallMark2PutX");
		GetInitGameParam(&m_mark2PutY,"smallMark2PutY");


		GetInitGameParam(&m_middle1PutX,"middlePutX1");
		GetInitGameParam(&m_middle1PutY,"middlePutY1");
		GetInitGameParam(&m_middle2PutX,"middlePutX2");
		GetInitGameParam(&m_middle2PutY,"middlePutY2");

		if (m_miniFlag)
		{
			m_number1PutX /= 2;
			m_number1PutY /= 2;
			m_number2PutX /= 2;
			m_number2PutY /= 2;

			m_mark1PutX /= 2;
			m_mark1PutY /= 2;
			m_mark2PutX /= 2;
			m_mark2PutY /= 2;

			m_mark1PutY += 1;
			m_mark2PutY += 1;

			m_middle1PutX /= 2;
			m_middle1PutY /= 2;
			m_middle2PutX /= 2;
			m_middle2PutY /= 2;
		}



		for (int i=0;i<3;i++)
		{
			m_putXY[(55+i*2)*2+0*2+0] = m_middle1PutX;
			m_putXY[(55+i*2)*2+0*2+1] = m_middle1PutY;
			m_putXY[(55+i*2)*2+1*2+0] = m_middle2PutX;
			m_putXY[(55+i*2)*2+1*2+1] = m_middle2PutY;
		}

		int start = 0;
		for (int i=0;i<13;i++)
		{
			int kosuu = m_putKosuu[i];

			for (int k=0;k<kosuu;k++)
			{
				char name[256];
				
				wsprintf(name,"mark%dputX%d",i+1,k+1);
				int x = m_putXY[(start+k)*2+0];
				GetInitGameParam(&x,name);
				if (m_miniFlag)
				{
					if (i<10)
					{
						x /= 2;
					}
				}
				m_putXY[(start+k)*2+0] = x;

				wsprintf(name,"mark%dputY%d",i+1,k+1);
				int y = m_putXY[(start+k)*2+1];
				GetInitGameParam(&y,name);
				if (m_miniFlag)
				{
					if (i<10)
					{
						y /= 2;
					}
				}
				m_putXY[(start+k)*2+1] = y;
			}

			start += kosuu;
		}
	}


	LoadOrgPic();
	LoadPartsPic();

	if (m_orgLoadFlag && m_partsLoadFlag)
	{
		for (int st=0;st<4;st++)
		{
			int start2 = 0;

			for (int i=0;i<13;i++)
			{
				int kosuu = m_putKosuu[i];

				//mark
				if (m_markPrintFlag)
				{
					PutSmallMark(st,i,m_mark1PutX,m_mark1PutY,0);
					PutSmallMark(st,i,m_mark2PutX,m_mark2PutY,1);
				}

				//number
				if (m_numberPrintFlag)
				{
					PutNumber(st,i,m_number1PutX,m_number1PutY,0);
					PutNumber(st,i,m_number2PutX,m_number2PutY,1);
				}


				int sizeFlag = m_sizeFlag[i];

				for (int k=0;k<kosuu;k++)
				{
					int putX = m_putXY[(start2+k)*2+0];
					int putY = m_putXY[(start2+k)*2+1];
					int rev = 0;
					if (k>=m_normalKosuu[i])
					{
						rev = 1;
					}

					if (sizeFlag == 0)
					{
						if (((i != 0) || (st != 0)) || (m_worldPrintFlag))
						{
							PutLargeMark(st,i,putX,putY,rev);
						}
					}
					else
					{
						if (m_middlePrintFlag)
						{
							PutMiddleMark(st,i,putX,putY,rev);
						}
					}

				}

				start2 += kosuu;
			}
		}

		SaveMakedPic();
	}



	DELETEARRAY(m_maskPic);
	DELETEARRAY(m_basePic);
	DELETEARRAY(m_partsPic);


	ENDDELETECLASS(m_setup);

	MessageBox(NULL,"おわりましたにゃ","End",MB_OK);

	return 0;
}

void LoadOrgPic(void)
{
	char filename[256];
	if (m_3d == 0)
	{
		if (m_miniFlag == 0)
		{
			wsprintf(filename,"%s.bmp",m_filenameBase);
		}
		else
		{
			wsprintf(filename,"%s_mini.bmp",m_filenameBase);
		}
	}
	else
	{
		if (m_miniFlag == 0)
		{
			wsprintf(filename,"%s3d.bmp",m_filenameBase);
		}
		else
		{
			wsprintf(filename,"%s3d_mini.bmp",m_filenameBase);
		}
	}

	FILE* file = NULL;
	fopen_s(&file,filename,"rb");
	if (file == NULL)
	{
		MessageBox(NULL,"ファイルが見つかりません",m_filenameBase,MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	fread(&m_baseHeader,sizeof(BITMAPFILEHEADER),1,file);
	fread(&m_baseInfo,sizeof(BITMAPINFOHEADER),1,file);
	if (m_baseInfo.biBitCount != 24)
	{
		MessageBox(NULL,"フルカラー画像ではありません",m_filenameBase,MB_ICONEXCLAMATION | MB_OK);
		fclose(file);
		return;
	}


	int readX = (m_baseInfo.biWidth*3 + 3) & (~3);
	int readY = m_baseInfo.biHeight;
	if (readY < 0) readY *= -1;

	int sz = readX * readY;
	m_basePic = new char[sz];

	fread(m_basePic,sizeof(char),sz,file);






	m_orgLoadFlag = 1;
	fclose(file);
}

void LoadPartsPic(void)
{
	char filename[256];
	if (m_miniFlag == 0)
	{
		wsprintf(filename,"%s.bmp",m_filenameParts);
	}
	else
	{
		wsprintf(filename,"%s_mini.bmp",m_filenameParts);
	}

	FILE* file = NULL;
	fopen_s(&file,filename,"rb");
	if (file == NULL)
	{
		MessageBox(NULL,"ファイルが見つかりません",m_filenameParts,MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	fread(&m_partsHeader,sizeof(BITMAPFILEHEADER),1,file);
	fread(&m_partsInfo,sizeof(BITMAPINFOHEADER),1,file);
	if (m_partsInfo.biBitCount != 24)
	{
		MessageBox(NULL,"フルカラー画像ではありません",m_filenameParts,MB_ICONEXCLAMATION | MB_OK);
		fclose(file);
		return;
	}
	
	int readX = (m_partsInfo.biWidth*3 + 3) & (~3);
	int readY = m_partsInfo.biHeight;
	if (readY < 0) readY *= -1;

	int sz = readX * readY;
	m_partsPic = new char[sz];

	fread(m_partsPic,sizeof(char),sz,file);
	fclose(file);




	if (m_miniFlag == 0)
	{
		wsprintf(filename,"%sm.bmp",m_filenameParts);
	}
	else
	{
		wsprintf(filename,"%s_minim.bmp",m_filenameParts);
	}
	file = NULL;
	fopen_s(&file,filename,"rb");
	if (file == NULL)
	{
		MessageBox(NULL,"マスクファイルが見つかりません",m_filenameParts,MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	fread(&m_maskHeader,sizeof(BITMAPFILEHEADER),1,file);
	fread(&m_maskInfo,sizeof(BITMAPINFOHEADER),1,file);
	if (m_maskInfo.biBitCount != 8)
	{
		MessageBox(NULL,"マスクが256色画像ではありません",m_filenameParts,MB_ICONEXCLAMATION | MB_OK);
		fclose(file);
		return;
	}
	

	//palette
	for (int i=0;i<256;i++)
	{
		m_paletteTable[i] = i;
	}

	int paletteKosuu = m_maskInfo.biClrUsed;
	if (paletteKosuu == 0) paletteKosuu = 256;

	for (int i=0;i<paletteKosuu;i++)
	{
		char r;
		char g;
		char b;
		char a;

		fread(&b,sizeof(char),1,file);
		fread(&g,sizeof(char),1,file);
		fread(&r,sizeof(char),1,file);
		fread(&a,sizeof(char),1,file);


		int rr = ((int)r) & 0xff;
		int gg = ((int)g) & 0xff;
		int bb = ((int)b) & 0xff;


		int yuvY = (rr * 299 + gg * 587 + bb * 114 + 500) / 1000;
		if (yuvY > 255) yuvY = 255;

		m_paletteTable[i] = yuvY;
	}




	readX = (m_maskInfo.biWidth + 3) & (~3);
	readY = m_maskInfo.biHeight;
	if (readY < 0) readY *= -1;

	sz = readX * readY;
	m_maskPic = new char[sz];

	fread(m_maskPic,sizeof(char),sz,file);



	m_partsLoadFlag = 1;
}

void SaveMakedPic(void)
{
	char filename[256];
	if (m_3d == 0)
	{
		if (m_miniFlag == 0)
		{
			wsprintf(filename,"%s.bmp",m_filenameMake);
		}
		else
		{
			wsprintf(filename,"%s_mini.bmp",m_filenameMake);
		}
	}
	else
	{
		if (m_miniFlag == 0)
		{
			wsprintf(filename,"%s3d.bmp",m_filenameMake);
		}
		else
		{
			wsprintf(filename,"%s3d_mini.bmp",m_filenameMake);
		}
	}

	FILE* file = NULL;
	fopen_s(&file,filename,"wb");
	if (file == NULL)
	{
		MessageBox(NULL,"ファイルが作成できません",m_filenameMake,MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	fwrite(&m_baseHeader,sizeof(BITMAPFILEHEADER),1,file);
	fwrite(&m_baseInfo,sizeof(BITMAPINFOHEADER),1,file);

	int readX = (m_baseInfo.biWidth*3 + 3) & (~3);
	int readY = m_baseInfo.biHeight;
	if (readY < 0) readY *= -1;

	int sz = readX * readY;
	fwrite(m_basePic,sizeof(char),sz,file);
	fclose(file);
}


void PutSmallMark(int st,int n,int putX,int putY,int revFlag)
{
	int sizeX = m_smallSizeX;
	int sizeY = m_smallSizeY;
	int srcX = sizeX * st;
	int srcY = m_smallSrcY;
	if (m_3d == 0)
	{
		putX += m_cardSizeX * n;
		putY += m_cardSizeY * st;
	}
	else
	{
		if (n<10)
		{
			putX += m_cardSizeX * n;
			putY += m_cardSizeY * st;
		}
		else
		{
			int k = (n-10)+st*3;
			putX += (k % 9)*m_cardSizeX;
			putY += (4+(k / 9)) * m_cardSizeY;
		}
	}

	PutRoutine(putX,putY,srcX,srcY,sizeX,sizeY,revFlag);
}

void PutMiddleMark(int st,int n,int putX,int putY,int revFlag)
{
	int sizeX = m_middleSizeX;
	int sizeY = m_middleSizeY;
	int srcX = sizeX * st;
	int srcY = m_middleSrcY;
	if (m_3d == 0)
	{
		putX += m_cardSizeX * n;
		putY += m_cardSizeY * st;
	}
	else
	{
		if (n<10)
		{
			putX += m_cardSizeX * n;
			putY += m_cardSizeY * st;
		}
		else
		{
			int k = (n-10)+st*3;
			putX += (k % 9)*m_cardSizeX;
			putY += (4+(k / 9)) * m_cardSizeY;
		}
	}

//	putX += m_cardSizeX * n;
//	putY += m_cardSizeY * st;

	PutRoutine(putX,putY,srcX,srcY,sizeX,sizeY,revFlag);
}

void PutLargeMark(int st,int n,int putX,int putY,int revFlag)
{
	int sizeX = m_largeSizeX;
	int sizeY = m_largeSizeY;
	int srcX = sizeX * st;
	int srcY = 0;
	if (m_3d == 0)
	{
		putX += m_cardSizeX * n;
		putY += m_cardSizeY * st;
	}
	else
	{
		if (n<10)
		{
			putX += m_cardSizeX * n;
			putY += m_cardSizeY * st;
		}
		else
		{
			int k = (n-10)+st*3;
			putX += (k % 9)*m_cardSizeX;
			putY += (4+(k / 9)) * m_cardSizeY;
		}
	}

//	putX += m_cardSizeX * n;
//	putY += m_cardSizeY * st;

	PutRoutine(putX,putY,srcX,srcY,sizeX,sizeY,revFlag);
}

void PutNumber(int st,int n,int putX,int putY,int revFlag)
{
	int sizeX = m_fontSizeX;
	int sizeY = m_fontSizeY;
	int srcX = sizeX * n;
	int srcY = m_fontSrcY + st*sizeY;
//	putX += m_cardSizeX * n;
//	putY += m_cardSizeY * st;

	if (m_3d == 0)
	{
		putX += m_cardSizeX * n;
		putY += m_cardSizeY * st;
	}
	else
	{
		if (n<10)
		{
			putX += m_cardSizeX * n;
			putY += m_cardSizeY * st;
		}
		else
		{
			int k = (n-10)+st*3;
			putX += (k % 9)*m_cardSizeX;
			putY += (4+(k / 9)) * m_cardSizeY;
		}
	}

	PutRoutine(putX,putY,srcX,srcY,sizeX,sizeY,revFlag);
}

void PutRoutine(int putX,int putY,int srcX,int srcY,int sizeX,int sizeY,int rev)
{
	int putMaxX = m_baseInfo.biWidth;
	int putMaxY = m_baseInfo.biHeight;
	int srcMaxX = m_partsInfo.biWidth;
	int srcMaxY = m_partsInfo.biHeight;
	int maskMaxX = m_maskInfo.biWidth;
	int maskMaxY = m_maskInfo.biHeight;

	if (putMaxY < 0) putMaxY *= -1;
	if (srcMaxY < 0) srcMaxY *= -1;
	if (maskMaxY < 0) maskMaxY *= -1;
	
	int putPitch = (m_baseInfo.biWidth*3 + 3) & (~3);
	int srcPitch = (m_partsInfo.biWidth*3 + 3) & (~3);
	int maskPitch = (m_maskInfo.biWidth + 3) & (~3);
//	int maskPitch = m_maskInfo.biWidth;


	for (int j=0;j<sizeY;j++)
	{
		int y = putY + j;
		if ((y<0) || (y>=putMaxY)) continue;

		int sy = srcY + j;
		if (rev)
		{
			sy = srcY + sizeY - 1 - j;
		}

		if ((sy < 0) || (sy>=srcMaxY)) continue;
		if ((sy < 0) || (sy>=maskMaxY)) continue;

		for (int i=0;i<sizeX;i++)
		{
			int x = putX + i;
			if ((x<0) || (x>=putMaxX)) continue;

			int sx = srcX + i;
			if (rev)
			{
				sx = srcX + sizeX - 1 - i;
			}
	
			if ((sx < 0) || (sx>=srcMaxX)) continue;
			if ((sx < 0) || (sx>=maskMaxX)) continue;


			char* dst = m_basePic;
			dst += x * 3;
			dst += (putMaxY - 1 - y) * putPitch;

			char* src = m_partsPic;
			src += sx * 3;
			src += (srcMaxY - 1 - sy) * srcPitch;

			char* mask = m_maskPic;
			mask += sx;
			mask += (maskMaxY - 1 - sy) * maskPitch;

			char r = *(src+0);
			char g = *(src+1);
			char b = *(src+2);
//r=0;
//g = 127;
//b=0;
			int r2 = ((int)r) & 0xff;
			int g2 = ((int)g) & 0xff;
			int b2 = ((int)b) & 0xff;

			char m = *mask;
			char mm = ((int)m) & 0xff;
			int ml = m_paletteTable[((int)(mm)) & 0xff];

			char r0 = *(dst+0);
			char g0 = *(dst+1);
			char b0 = *(dst+2);

			int r1 = ((int)r0) & 0xff;
			int g1 = ((int)g0) & 0xff;
			int b1 = ((int)b0) & 0xff;


			int rr = (r1 * (255-ml) + (r2 * ml)) / 255;
			int gg = (g1 * (255-ml) + (g2 * ml)) / 255;
			int bb = (b1 * (255-ml) + (b2 * ml)) / 255;

			if (rr > 255) rr = 255;
			if (gg > 255) gg = 255;
			if (bb > 255) bb = 255;


			*(dst+0) = (char)rr;
			*(dst+1) = (char)gg;
			*(dst+2) = (char)bb;
		}
	}


}


BOOL GetInitGameParam(int* lpVar, LPSTR name)
{
	int rNum = m_setup->SearchName2(name);
	if (rNum == -1) return FALSE;

	*lpVar = atoi(m_setup->GetName(rNum + 1));

	return TRUE;
}


BOOL GetInitGameString(LPSTR* lpStr, LPSTR name)
{
	int rNum = m_setup->SearchName2(name);
	if (rNum == -1) return FALSE;

	*lpStr = m_setup->GetName(rNum + 1);

	return TRUE;
}




/*_*/



