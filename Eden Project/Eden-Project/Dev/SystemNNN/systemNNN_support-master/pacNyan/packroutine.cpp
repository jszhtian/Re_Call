//
// packroutine.cpp
//

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <commctrl.h>

#include "packroutine.h"
#include "checkfiledate.h"

#include "..\..\systemNNN\nyanPictureLib\pngLoader.h"

char* CPackRoutine::m_nameData = NULL;
int* CPackRoutine::m_nameTable = NULL;

/*
char nopackHeader[]= "\
IF PACKTYPE==0  \
CUT THIS 64 BYTE\
THEN REMAKE BMP \
PACKTYPE=0      ";
*/


//	headerBuffer[8] = fileSize;
//	headerBuffer[9] = size.cx;
//	headerBuffer[10] = size.cy;

char CPackRoutine::m_headerType0[]="\
BMP             \
                \
                \
PACKTYPE=0      ";


char CPackRoutine::m_headerType2[]="\
BMP+MASK        \
                \
                \
PACKTYPE=2A     ";


char CPackRoutine::m_headerType3[]="\
PACKBMP+MASK    \
                \
                \
PACKTYPE=3A     ";

char CPackRoutine::m_headerType5[]="\
JPEG            \
                \
                \
PACKTYPE=5      ";

char CPackRoutine::m_headerType7[]="\
JPEG+MASK       \
                \
                \
PACKTYPE=7A     ";

char CPackRoutine::m_headerType8[]="\
PNG             \
                \
                \
PACKTYPE=8A     ";


#define JPEG_SOI 0xD8
#define JPEG_EOI 0xD9
#define JPEG_APP0 0xE0
#define JPEG_DQT 0xDB
#define JPEG_SOF0 0xC0
#define JPEG_SOF2 0xC2
#define JPEG_DHT 0xC4
#define JPEG_SOS 0xDA
#define JPEG_DRI 0xDD


CPackRoutine::CPackRoutine()
{
	m_errorSkipFlag = FALSE;

	m_checkFileDate = new CCheckFileDate();
	m_loadBuffer = new char[1024*16384];
	m_loadBuffer2 = new char[1024*8192];
	m_saveBuffer = new char[1024*8192];

	m_pngLoader = new CPngLoader(8);
}



CPackRoutine::~CPackRoutine()
{
	End();
}

void CPackRoutine::End(void)
{
	if (m_pngLoader != NULL)
	{
		delete m_pngLoader;
		m_pngLoader = NULL;
	}

	if (m_checkFileDate != NULL)
	{
		m_checkFileDate->End();
		delete m_checkFileDate;
		m_checkFileDate = NULL;
	}

	if (m_loadBuffer != NULL)
	{
		delete [] m_loadBuffer;
		m_loadBuffer = NULL;
	}

	if (m_loadBuffer2 != NULL)
	{
		delete [] m_loadBuffer2;
		m_loadBuffer2 = NULL;
	}

	if (m_saveBuffer != NULL)
	{
		delete [] m_saveBuffer;
		m_saveBuffer = NULL;
	}
}



BOOL CPackRoutine::CreateDWQ(LPSTR srcDir0,LPSTR srcDir1,LPSTR srcDir2,LPSTR dstDir1,BOOL taFlag,BOOL compFlag)
{
	OutputDebugString(srcDir0);
	OutputDebugString("\\");
	OutputDebugString(srcDir1);
	OutputDebugString("\\");
	OutputDebugString(srcDir2);
	OutputDebugString("\n");
	OutputDebugString(dstDir1);
	OutputDebugString("\n");


	char srcDir[1024];
	if ((*srcDir2) == 0)
	{
		wsprintf(srcDir,"%s\\%s",srcDir0,srcDir1);
	}
	else
	{
		wsprintf(srcDir,"%s\\%s\\%s",srcDir0,srcDir1,srcDir2);
	}

	char dstDir[1024];
	if ((*dstDir1) == 0)
	{
		wsprintf(dstDir,"dwq\\%s",srcDir1);
	}
	else
	{
		wsprintf(dstDir,"dwq\\%s\\%s",srcDir1,dstDir1);
	}


	BOOL createFlag = compFlag;

	WIN32_FIND_DATA findData;
	char startFile[256];
	wsprintf(startFile,"%s\\*.%s",srcDir,srcDir0);


	char mes[256];
	wsprintf(mes,"DWQ作成中 %s",srcDir);
	SetWindowText(m_staticText,mes);
	UpdateWindow(m_staticText);

	int kosuu = 0;

	HANDLE handle = FindFirstFile(startFile,&findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (CreateDWQSub(&findData,srcDir0,srcDir,dstDir,taFlag,compFlag))
			{
				createFlag = TRUE;
			}
		}

		WIN32_FIND_DATA findData2;
		while (FindNextFile(handle,&findData2) != 0)
		{
			if (!(findData2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if ((kosuu % 10) == 0)
				{
					SendMessage(m_progressBar2,PBM_SETPOS,kosuu % 100,0);
				}
				kosuu++;

				if (CreateDWQSub(&findData2,srcDir0,srcDir,dstDir,taFlag,compFlag))
				{
					createFlag = TRUE;
				}
			}
		}
		FindClose(handle);
	}

	return TRUE;
}


BOOL CPackRoutine::CreateDWQSub(WIN32_FIND_DATA* lpFind,LPSTR extName,LPSTR srcDir,LPSTR dstDir,BOOL taFlag,BOOL compFlag)
{
	LPSTR filename = lpFind->cFileName;

	if (_stricmp(filename,".") == 0) return FALSE;
	if (_stricmp(filename,"..") == 0) return FALSE;

	char filenameonly[256];
	int ln = strlen(lpFind->cFileName);
	if (ln>254) ln = 254;
	
	int found = ln - 1;
	for (int i=0;i<ln;i++)
	{
		if ((*(filename+i)) == '.')
		{
			found = i;
			break;
		}
	}

	if (ln > 0)
	{
		memcpy(filenameonly,filename,found);
	}
	filenameonly[found] = 0;

	if (taFlag)
	{
		if (_stricmp(extName,"png") != 0)
		{
			if (CheckFileTailIsM(filename)) return FALSE;
		}
	}

	if (compFlag == FALSE)
	{
		if (m_checkFileDate->CheckNeedUpdate(filenameonly,srcDir,dstDir,extName,taFlag) == FALSE)
		{
			return FALSE;
		}
	}


	int md = 0;
	BOOL jpgFlag = FALSE;
	if (_stricmp(extName,"jpg") == 0)
	{
		md = 5;
		if (taFlag) md = 7;
	}
	else if (_stricmp(extName,"png") == 0)
	{
		md = 8;
	}
	else
	{
		if (taFlag)
		{
			md = 2;
			//bmp256

			char checkfilename[1024];
			wsprintf(checkfilename,"%s\\%s",srcDir,filename);
			if (Check256(checkfilename))
			{
				md = 3;
			}
		}
	}

	if (md == 0) PackRoutine0(srcDir,filenameonly,extName,dstDir);
	if (md == 2) PackRoutine2(srcDir,filenameonly,extName,dstDir);
	if (md == 3) PackRoutine3(srcDir,filenameonly,extName,dstDir);
	if (md == 5) PackRoutine5(srcDir,filenameonly,extName,dstDir);
	if (md == 7) PackRoutine7(srcDir,filenameonly,extName,dstDir);
	if (md == 8) PackRoutine8(srcDir,filenameonly,extName,dstDir);

	return TRUE;
}


BOOL CPackRoutine::Check256(LPSTR filename)
{
	FILE* file = NULL;
	fopen_s(&file,filename,"rb");

	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;

	if (file == NULL) return FALSE;

	BOOL flg = FALSE;
	if (fread(&bmf,sizeof(BITMAPFILEHEADER),1,file) == 1)
	{
		if (fread(&bmi,sizeof(BITMAPINFOHEADER),1,file) == 1)
		{
			if (bmi.biBitCount == 8) flg = TRUE;
		}
	}
	fclose(file);
	return flg;
}



BOOL CPackRoutine::ArcDWQ(int bit64,LPSTR dstDir,int version,LPSTR dstDir2)
{
	//


	int nameSize = 0;
	int nameKosuu = 0;

	char mes[256];
	if (dstDir2 == NULL)
	{
		wsprintf(mes,"DWQ固め中 %s",dstDir);
	}
	else
	{
		wsprintf(mes,"DWQ固め中 %s - %s",dstDir,dstDir2);
	}

	SetWindowText(m_staticText,mes);
	UpdateWindow(m_staticText);


	WIN32_FIND_DATA findData;
	WIN32_FIND_DATA findData2;
	char startFile[256];
	if (dstDir2 == NULL)
	{
		wsprintf(startFile,"dwq\\%s\\*.dwq",dstDir);
	}
	else
	{
		wsprintf(startFile,"dwq\\sys\\%s\\*.dwq",dstDir);
	}

	HANDLE handle = FindFirstFile(startFile,&findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			int ln = strlen(findData.cFileName);
			//
			ln -= 4;
			nameSize += (ln + 1);
			nameKosuu++;
		}

		while (FindNextFile(handle,&findData2) != 0)
		{
			if (!(findData2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				int ln = strlen(findData2.cFileName);
				//
				ln -= 4;
				nameSize += (ln + 1);
				nameKosuu++;
			}
		}
		FindClose(handle);
	}


	m_nameData = new char[nameSize];
	int* nameTable = new int[nameKosuu];
	int* offsetTable = new int[nameKosuu];
	LONGLONG* offsetTable2 = new LONGLONG[nameKosuu];


	int n = 0;
	int k = 0;

	handle = FindFirstFile(startFile,&findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			int ln = strlen(findData.cFileName);
			//
			ln -= 4;
			memcpy(m_nameData+n,findData.cFileName,ln);
			m_nameData[n+ln] = 0;
			nameTable[k] = n;
			n += (ln + 1);
			k++;
		}

		while (FindNextFile(handle,&findData2) != 0)
		{
			if (!(findData2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				int ln = strlen(findData2.cFileName);
				//
				ln -= 4;
				memcpy(m_nameData+n,findData2.cFileName,ln);
				m_nameData[n+ln] = 0;
				nameTable[k] = n;
				n += (ln + 1);
				k++;
			}
		}
		FindClose(handle);
	}

	m_nameTable = nameTable;

	//小文字にする
	for (int j=0;j<nameKosuu;j++)
	{
		int p = nameTable[j];
		int ln = strlen(m_nameData+p);

		for (int i=0;i<ln;i++)
		{
			char c = *(m_nameData+p+i);
			if ((c>='A') && (c<='Z'))
			{
				c += 0x20;
				*(m_nameData+p+i) = c;
			}
			else
			{
				if (c & 0x80) i++;
			}
		}
	}

	//sort

	qsort(nameTable,nameKosuu,sizeof(int),MyCompare);


	int offset = 0;
	LONGLONG offset64 = 0;

	for (int j=0;j<nameKosuu;j++)
	{
		int p = nameTable[j];
		char filename0[256];
		if (dstDir2 == NULL)
		{
			wsprintf(filename0,"dwq\\%s\\%s.dwq",dstDir,m_nameData+p);
		}
		else
		{
			wsprintf(filename0,"dwq\\sys\\%s\\%s.dwq",dstDir,m_nameData+p);
		}

		FILE* file = NULL;
		fopen_s(&file,filename0,"rb");
		if (file != NULL)
		{
			fseek(file,0,SEEK_END);
			int sz = ftell(file);
			offsetTable[j] = offset;
			offsetTable2[j] = offset64;

			offset += sz;
			offset64 += sz;

			fclose(file);
		}
		else
		{
			PrintFatalError(filename0,"PacError");
		}
	}



	//実データのパック

	char filename[256];
	wsprintf(filename,"dwq\\%s%d.gtb",dstDir,version);

	FILE* file = NULL;
	fopen_s(&file,filename,"wb");
	if (file != NULL)
	{
		fwrite(&nameKosuu,sizeof(int),1,file);
		
		fwrite(nameTable,sizeof(int),nameKosuu,file);
		fwrite(offsetTable,sizeof(int),nameKosuu,file);
		fwrite(m_nameData,sizeof(char),nameSize,file);

		//pad
		int d = sizeof(int) + nameKosuu * sizeof(int) + nameKosuu * sizeof(int) + nameSize;
		int d2 = (d + 7) & ~7;
		if ((d2-d) > 0)
		{
			char pad[8];
			for (int i=0;i<8;i++)pad[i]=0;
			fwrite(pad,sizeof(char),d2-d,file);
		}

		//exp
		fwrite(offsetTable2,sizeof(LONGLONG),nameKosuu,file);

		char pad0[8];
		for (int i=0;i<8;i++)pad0[i]=0;
		fwrite(pad0,sizeof(char),8,file);
		char cd[]= "over2G!";
		fwrite(cd,sizeof(char),8,file);

		fclose(file);
	}
	else
	{
		PrintFatalError(filename,"PacError(gtb)");
	}


	wsprintf(filename,"dwq\\%s%d.gpk",dstDir,version);
	fopen_s(&file,filename,"wb");
	if (file != NULL)
	{
		for (int j=0;j<nameKosuu;j++)
		{
			char filename3[256];
			int p2 = nameTable[j];

			if ((j % 10) == 0)
			{
				SendMessage(m_progressBar2,PBM_SETPOS,j % 100,0);
			}


			if (dstDir2 == NULL)
			{
				wsprintf(filename3,"dwq\\%s\\%s.dwq",dstDir,&m_nameData[p2]);
			}
			else
			{
				wsprintf(filename3,"dwq\\sys\\%s\\%s.dwq",dstDir,&m_nameData[p2]);
			}
			FILE* file3 = NULL;
			fopen_s(&file3,filename3,"rb");
			if (file3 != NULL)
			{
				int fileSize = fread(m_loadBuffer,sizeof(char),8192*1024,file3);
				fclose(file3);
				fwrite(m_loadBuffer,sizeof(char),fileSize,file);
			}
			else
			{
				PrintFatalError(filename3,"PacError(gpk)");
			}
		}
		fclose(file);
	}
	else
	{
		PrintFatalError(filename,"PacError(gtb)");
	}















	delete [] offsetTable2;
	delete [] offsetTable;
	delete [] nameTable;
	delete [] m_nameData;

	return TRUE;
}


int CPackRoutine::MyCompare( const void* arg1, const void* arg2 )
{
	int* ptr1 = (int*)arg1;
	int* ptr2 = (int*)arg2;

	int n1 = *ptr1;
	int n2 = *ptr2;

//	int offset1 = m_nameTable[n1];
//	int offset2 = m_nameTable[n2];


	char* mes1 = (char*)(m_nameData+n1);
	char* mes2 = (char*)(m_nameData+n2);

	return _stricmp(mes1,mes2);
}


BOOL CPackRoutine::CheckFileTailIsM(LPSTR filename)
{
	if (filename == NULL) return FALSE;
	int ln = strlen(filename);
	
	int found = -1;
	for (int i=0;i<ln;i++)
	{
		if (*(filename+i) == '.')
		{
			found = i;
			break;
		}
	}
	if (found == -1) return FALSE;

	if (found == 0) return FALSE;
	char c = *(filename+found-1);

	if ((c != 'm') && (c != 'M')) return FALSE;

//	if (strcmp(filename,"ta_back武士.bmp") == 0)
//	{
//		int aaa = 0;
//		aaa++;
//	}
//	if (strcmp(filename,"ta_back武士m.bmp") == 0)
//	{
//		int aaa = 0;
//		aaa++;
//	}

	int k = 0;
	while (k<found)
	{
		char cc = *(filename + k);
		int ic = (int)cc;
		ic &= 0xff;
		if ( ((ic>=0x80) && (ic<=0x9f)) || ((ic>=0xe0) && (ic<=0xff)) )
		{
			k += 2;
		}
		else
		{
			k += 1;
		}
		if (k == found-1) return TRUE;
	}

	return FALSE;
}





BOOL CPackRoutine::PackRoutine0(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir)
{
	char srcFileName[256];
	wsprintf(srcFileName,"%s\\%s.%s",srcDir,filename,extName);

	FILE* file = NULL;
	fopen_s(&file,srcFileName,"rb");

	int sx,sy,dy,startY,readX;

	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;
	RGBQUAD rgbquad[256];

	BOOL bError = TRUE;

	if (file != NULL)
	{
		if (fread(&bmf,sizeof(BITMAPFILEHEADER),1,file) == 1)
		{
			if (fread(&bmi,sizeof(BITMAPINFOHEADER),1,file) == 1)
			{
				//load palette
				ZeroMemory(&rgbquad,sizeof(RGBQUAD)*256);

				if ((bmi.biBitCount != 8) && (bmi.biBitCount != 24))
				{
					PrintFatalError(filename,"色数に問題あり");
				}

				if (bmi.biBitCount == 8)
				{
					int p = bmi.biClrUsed;
					if (p == 0) p = 256;
					bmi.biClrUsed = p;

					fread(&rgbquad,sizeof(RGBQUAD),p,file);
				}



				sx = bmi.biWidth;
				sy = bmi.biHeight;

				dy = -1;
				startY = sy - 1;

				if (sy<0)
				{
					sy = -sy;
					dy = -dy;
					startY = 0;
				}

				int dataByte = bmi.biBitCount>>3;

				readX = (sx*dataByte+3) & (~3);

				if (startY == 0)
				{
					fread(m_loadBuffer,sizeof(char),readX*sy,file);
				}
				else
				{
					for (int j=0;j<sy;j++)
					{
						fread(&m_loadBuffer[readX * (startY - j)],sizeof(char),readX,file);
					}
				}

				if (dataByte == 3)
				{
					ChangeRGB(sx,sy);
				}

				
				bError = FALSE;
			}
		}
		fclose(file);
	}



	if (bError)
	{
		PrintFatalError(filename,"LoadError");
		return FALSE;
	}

	int headerBuffer[64/4];
	memcpy(headerBuffer,m_headerType0,64);

	headerBuffer[9] = sx;
	headerBuffer[10] = sy;


	char dstFileName[256];
	wsprintf(dstFileName,"%s\\%s.dwq",dstDir,filename);

	FILE* file2 = NULL;
	fopen_s(&file2,dstFileName,"wb");
	if (file2 == NULL)
	{
		PrintFatalError(filename,"SaveOpenError pack0");
		return FALSE;
	}


//	fwrite(nopackHeader,sizeof(char),64,file2);
	fwrite(headerBuffer,sizeof(char),64,file2);

	fwrite(&bmf,sizeof(bmf),1,file2);
	fwrite(&bmi,sizeof(bmi),1,file2);

	if (bmi.biBitCount == 8)
	{
		int bc = bmi.biClrUsed;
		if (bc == 0) bc = 256;
		fwrite(&rgbquad,sizeof(RGBQUAD),bc,file2);
	}

	fwrite(m_loadBuffer,sizeof(char),readX*sy,file);

	fclose(file2);

	return TRUE;
}



BOOL CPackRoutine::PackRoutine2(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir)
{
	char srcFileName[256];
	wsprintf(srcFileName,"%s\\%s.%s",srcDir,filename,extName);

	FILE* file = NULL;
	fopen_s(&file,srcFileName,"rb");

	int sx,sy,dy,startY,readX;

	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;
	RGBQUAD rgbquad[256];

	BOOL bError = TRUE;

	if (file != NULL)
	{
		if (fread(&bmf,sizeof(BITMAPFILEHEADER),1,file) == 1)
		{
			if (fread(&bmi,sizeof(BITMAPINFOHEADER),1,file) == 1)
			{
				//load palette
				ZeroMemory(&rgbquad,sizeof(RGBQUAD)*256);

				if ((bmi.biBitCount != 8) && (bmi.biBitCount != 24))
				{
					PrintFatalError(filename,"色数に問題あり");
				}

				if (bmi.biBitCount == 8)
				{
					int p = bmi.biClrUsed;
					if (p == 0) p = 256;
					bmi.biClrUsed = p;

					fread(&rgbquad,sizeof(RGBQUAD),p,file);
				}



				sx = bmi.biWidth;
				sy = bmi.biHeight;

				dy = -1;
				startY = sy - 1;

				if (sy<0)
				{
					sy = -sy;
					dy = -dy;
					startY = 0;
				}

				int dataByte = bmi.biBitCount>>3;

				readX = (sx*dataByte+3) & (~3);

				if (startY == 0)
				{
					fread(m_loadBuffer,sizeof(char),readX*sy,file);
				}
				else
				{
					for (int j=0;j<sy;j++)
					{
						fread(&m_loadBuffer[readX * (startY - j)],sizeof(char),readX,file);
					}
				}

				if (dataByte == 3)
				{
					ChangeRGB(sx,sy);
				}
	
				bError = FALSE;
			}
		}
		fclose(file);
	}



	if (bError)
	{
		PrintFatalError(filename,"LoadError");
		return FALSE;
	}

	int headerBuffer[64/4];
	memcpy(headerBuffer,m_headerType2,64);


	if (bmi.biBitCount == 8)
	{
		headerBuffer[8] = sizeof(bmf) + sizeof(bmi) + sizeof(RGBQUAD)*(bmi.biClrUsed) + readX * sy;
	}
	else
	{
		headerBuffer[8] = sizeof(bmf) + sizeof(bmi) + readX * sy;
	}


	headerBuffer[9] = sx;
	headerBuffer[10] = sy;


	RECT cutter = CalcuCutData(srcDir,filename);


	headerBuffer[4] = cutter.top;
	headerBuffer[5] = cutter.left;
	headerBuffer[6] = cutter.right;
	headerBuffer[7] = cutter.bottom;


	char dstFileName[256];
	wsprintf(dstFileName,"%s\\%s.dwq",dstDir,filename);

	FILE* file2 = NULL;
	fopen_s(&file2,dstFileName,"wb");
	if (file2 == NULL)
	{
		PrintFatalError(filename,"SaveOpenError pack2");
		return FALSE;
	}


//	fwrite(nopackHeader,sizeof(char),64,file2);
	fwrite(headerBuffer,sizeof(char),64,file2);

	fwrite(&bmf,sizeof(bmf),1,file2);
	fwrite(&bmi,sizeof(bmi),1,file2);

	if (bmi.biBitCount == 8)
	{
		int bc = bmi.biClrUsed;
		if (bc == 0) bc = 256;
		fwrite(&rgbquad,sizeof(RGBQUAD),bc,file2);
	}

	fwrite(m_loadBuffer,sizeof(char),readX*sy,file);

	MakeAndSaveMask(file2,srcDir,filename);

	fclose(file2);

	return TRUE;

}


BOOL CPackRoutine::PackRoutine3(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir)
{
	int dataSize = 0;

	int i,j;
	char srcFileName[256];
	wsprintf(srcFileName,"%s\\%s.%s",srcDir,filename,extName);

	FILE* file = NULL;
	fopen_s(&file,srcFileName,"rb");

	int sx,sy,dy,startY,readX;

	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;
	RGBQUAD rgbquad[256];

	BOOL bError = TRUE;
	if (file != NULL)
	{
		if (fread(&bmf,sizeof(BITMAPFILEHEADER),1,file) == 1)
		{
			if (fread(&bmi,sizeof(BITMAPINFOHEADER),1,file) == 1)
			{
				//load palette
				ZeroMemory(&rgbquad,sizeof(RGBQUAD)*256);
				if (bmi.biBitCount == 8)
				{
					int p = bmi.biClrUsed;
					if (p == 0) p = 256;
					bmi.biClrUsed = p;

					fread(&rgbquad,sizeof(RGBQUAD),p,file);

					sx = bmi.biWidth;
					sy = bmi.biHeight;
					dy = -1;
					startY = sy - 1;

					if (sy<0)
					{
						sy = -sy;
						dy = -dy;
						startY = 0;
					}


					readX = (sx+3) & (~3);

					for (i=0;i<readX;i++)
					{
						m_loadBuffer[i] = 0;
					}

					int y = startY;

					y++;

					for (j=0;j<sy;j++)
					{
						fread(&m_loadBuffer[y*readX],sizeof(char),readX,file);
						y += dy;
					}
					
					bError = FALSE;
				}
			}
		}
		fclose(file);
	}

	if (bError)
	{
		PrintFatalError(filename,"LoadError");
		return -1;
	}



//	FILE* file2 = fopen(filename2,"wb");
//	if (file2 == NULL)
//	{
//		MessageBox(NULL,"Save Open Error","Error",MB_OK);
//		return -2;
//	}



	
	
	
	

	//pack main
	//xor
	for (j=sy;j>=1;j--)
	{
		for (i=0;i<sx;i++)
		{
			char c = m_loadBuffer[j*readX+i];
			c ^= m_loadBuffer[(j-1)*readX+i];
			m_loadBuffer[j*readX+i] = c;
		}
	}

	int savePtr = 0;
	//set header

	//pack
	for (j=0;j<sy;j++)
	{
		int pt = 0;
		int ln = 0;
		while (pt<sx)
		{
			char c = m_loadBuffer[(j+1)*readX+pt];
			pt++;

			if (c != 0)
			{
				if (ln>0)
				{
					m_saveBuffer[savePtr] = 0;
					savePtr++;
					m_saveBuffer[savePtr] = ln;
					savePtr++;
					ln=0;
				}


				m_saveBuffer[savePtr] = c;
				savePtr++;
				ln = 0;
			}
			else
			{
				ln++;
				if ((ln>=255) || (pt>=sx))
				{
					m_saveBuffer[savePtr] = 0;
					savePtr++;
					m_saveBuffer[savePtr] = ln;
					savePtr++;
					ln=0;
				}
			}
		}
	}


	char dstFileName[256];
	wsprintf(dstFileName,"%s\\%s.dwq",dstDir,filename);

	FILE* file2 = NULL;
	fopen_s(&file2,dstFileName,"wb");
	if (file2 == NULL)
	{
		PrintFatalError(filename,"SaveOpenError pack3");
		return FALSE;
	}

	int headerBuffer[64/4];
	memcpy(headerBuffer,m_headerType3,64);

	int bc = bmi.biClrUsed;
	if (bc == 0) bc = 256;

	headerBuffer[8] = sizeof(bmf) + sizeof(bmi) + sizeof(RGBQUAD)*bc + savePtr;
	headerBuffer[9] = sx;
	headerBuffer[10] = sy;


	RECT cutter = CalcuCutData(srcDir,filename);

	headerBuffer[4] = cutter.top;
	headerBuffer[5] = cutter.left;
	headerBuffer[6] = cutter.right;
	headerBuffer[7] = cutter.bottom;



	fwrite(headerBuffer,sizeof(char),64,file2);

	fwrite(&bmf,sizeof(bmf),1,file2);
	fwrite(&bmi,sizeof(bmi),1,file2);
	fwrite(&rgbquad,sizeof(RGBQUAD),bc,file2);

	fwrite(m_saveBuffer,sizeof(char),savePtr,file2);


	MakeAndSaveMask(file2,srcDir,filename);

	fclose(file2);

	return TRUE;
}


BOOL CPackRoutine::PackRoutine5(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir)
{
	char srcFileName[256];
	wsprintf(srcFileName,"%s\\%s.jpg",srcDir,filename);

	FILE* file = NULL;
	fopen_s(&file,srcFileName,"rb");

	int fileSize = fread(m_loadBuffer,sizeof(char),1024*8192,file);
	fclose(file);

	//解析
	SIZE size;
	if (GetJpegSize(m_loadBuffer,&size) == FALSE)
	{
		PrintFatalError(filename,"JPEG DATA ERROR");
		return FALSE;
	}


	int headerBuffer[64/4];
	memcpy(headerBuffer,m_headerType5,64);

	headerBuffer[8] = fileSize;
	headerBuffer[9] = size.cx;
	headerBuffer[10] = size.cy;

	int sx = size.cx;
	int sy = size.cy;

	char dstFileName[256];
	wsprintf(dstFileName,"%s\\%s.dwq",dstDir,filename);

	FILE* file2 = NULL;
	fopen_s(&file2,dstFileName,"wb");
	if (file2 == NULL)
	{
		PrintFatalError(dstFileName,"SaveOpenError pack5");
		return FALSE;
	}


//	fwrite(nopackHeader,sizeof(char),64,file2);
	fwrite(headerBuffer,sizeof(char),64,file2);
	fwrite(m_loadBuffer,sizeof(char),fileSize,file2);

	fclose(file2);

	return TRUE;
}




BOOL CPackRoutine::PackRoutine7(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir)
{
	char srcFileName[256];
	wsprintf(srcFileName,"%s\\%s.jpg",srcDir,filename);

	FILE* file = NULL;
	fopen_s(&file,srcFileName,"rb");

	int fileSize = fread(m_loadBuffer,sizeof(char),1024*8192,file);
	fclose(file);

	//解析
	SIZE size;
	if (GetJpegSize(m_loadBuffer,&size) == FALSE)
	{
		MessageBox(NULL,srcFileName,"JPEG DATA ERROR",MB_OK);
		return FALSE;
	}


	int headerBuffer[64/4];
	memcpy(headerBuffer,m_headerType7,64);

	headerBuffer[8] = fileSize;
	headerBuffer[9] = size.cx;
	headerBuffer[10] = size.cy;


	RECT cutter = CalcuCutData(srcDir,filename);

	headerBuffer[4] = cutter.top;
	headerBuffer[5] = cutter.left;
	headerBuffer[6] = cutter.right;
	headerBuffer[7] = cutter.bottom;




	int sx = size.cx;
	int sy = size.cy;



	char dstFileName[256];
	wsprintf(dstFileName,"%s\\%s.dwq",dstDir,filename);

	FILE* file2 = NULL;
	fopen_s(&file2,dstFileName,"wb");
	if (file2 == NULL)
	{
		PrintFatalError(filename,"SaveOpenError pack7");
		return FALSE;
	}


//	fwrite(nopackHeader,sizeof(char),64,file2);
	fwrite(headerBuffer,sizeof(char),64,file2);
	fwrite(m_loadBuffer,sizeof(char),fileSize,file2);


	//mask data
	MakeAndSaveMask(file2,srcDir,filename);


	fclose(file2);

	return TRUE;
}


BOOL CPackRoutine::PackRoutine8(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir)
{
	char srcFileName[256];
	wsprintf(srcFileName,"%s\\%s.png",srcDir,filename);

	if (m_pngLoader->LoadFile(srcFileName) == FALSE)
	{
		MessageBox(NULL,"PNG LOAD ERROR!",srcFileName,MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
		//error!
	}

	RECT cutter;
	cutter.left = 0;
	cutter.right = 0;
	cutter.top = 0;
	cutter.bottom = 0;

	SIZE pngPicSize = m_pngLoader->GetPicSize();
	SIZE size;
	size.cx = pngPicSize.cx;
	size.cy = pngPicSize.cy;

	if (m_pngLoader->GetMaskFlag())
	{
		m_pngLoader->GetMaskData(m_loadBuffer);
		cutter = CalcuCutPng(pngPicSize,m_loadBuffer);
	}



	FILE* file = NULL;
	fopen_s(&file,srcFileName,"rb");

	int fileSize = fread(m_loadBuffer,sizeof(char),1024*16384,file);
	fclose(file);

	/*
	//解析
	if (GetJpegSize(m_loadBuffer,&size) == FALSE)
	{
		MessageBox(NULL,srcFileName,"JPEG DATA ERROR",MB_OK);
		return FALSE;
	}
	*/

	int headerBuffer[64/4];
	memcpy(headerBuffer,m_headerType8,64);

	headerBuffer[8] = fileSize;
	headerBuffer[9] = size.cx;
	headerBuffer[10] = size.cy;


//	RECT cutter = CalcuCutData(srcDir,filename);





	headerBuffer[4] = cutter.top;
	headerBuffer[5] = cutter.left;
	headerBuffer[6] = cutter.right;
	headerBuffer[7] = cutter.bottom;




	int sx = size.cx;
	int sy = size.cy;



	char dstFileName[256];
	wsprintf(dstFileName,"%s\\%s.dwq",dstDir,filename);

	FILE* file2 = NULL;
	fopen_s(&file2,dstFileName,"wb");
	if (file2 == NULL)
	{
		PrintFatalError(filename,"SaveOpenError pack8");
		return FALSE;
	}


	fwrite(headerBuffer,sizeof(char),64,file2);
	fwrite(m_loadBuffer,sizeof(char),fileSize,file2);


	//mask data
//	MakeAndSaveMask(file2,srcDir,filename);//@@@


	fclose(file2);

	return TRUE;
}



RECT CPackRoutine::CalcuCutData(LPSTR dirName,LPSTR filename)
{
	RECT rc;
	SetRect(&rc,0,0,0,0);

	int i,j;
	char srcFileName[256];
	wsprintf(srcFileName,"%s\\%sm.bmp",dirName,filename);

	FILE* file = NULL;
	fopen_s(&file,srcFileName,"rb");

	int sx,sy,dy,startY,readX;

	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;
	RGBQUAD rgbquad[256];

	char tmpBuffer[8192*4];

	int picSizeX = 1;
	int picSizeY = 1;
	int dataSizeX = 1;

	BOOL bError = TRUE;
	if (file != NULL)
	{
		if (fread(&bmf,sizeof(BITMAPFILEHEADER),1,file) == 1)
		{
			if (fread(&bmi,sizeof(BITMAPINFOHEADER),1,file) == 1)
			{
				//load palette
				ZeroMemory(&rgbquad,sizeof(RGBQUAD)*256);
				if (bmi.biBitCount == 8)
				{
					int p = bmi.biClrUsed;
					if (p == 0) p = 256;
					bmi.biClrUsed = p;

					fread(&rgbquad,sizeof(RGBQUAD),p,file);

					sx = bmi.biWidth;
					sy = bmi.biHeight;
					dy = -1;
					startY = sy - 1;

					if (sy<0)
					{
						sy = -sy;
						dy = -dy;
						startY = 0;
					}

					readX = (sx+3) & (~3);

					picSizeX = sx;
					picSizeY = sy;
					dataSizeX = readX;

//					for (i=0;i<readX;i++)
//					{
//						m_loadBuffer2[i] = 0;
//					}

					int y = startY;

//					y++;

					for (j=0;j<sy;j++)
					{
						fread(&m_loadBuffer2[y*readX],sizeof(char),readX,file);
						y += dy;
					}
					
					bError = FALSE;
				}
				else if (bmi.biBitCount == 24)
				{
					//make dummy palette
					for (i=0;i<256;i++)
					{
						rgbquad[i].rgbRed = i;
						rgbquad[i].rgbGreen = i;
						rgbquad[i].rgbBlue = i;
						rgbquad[i].rgbReserved =  0;
					}

					sx = bmi.biWidth;
					sy = bmi.biHeight;
					dy = -1;
					startY = sy - 1;

					if (sy<0)
					{
						sy = -sy;
						dy = -dy;
						startY = 0;
					}

					readX = (sx*3+3) & (~3);
					int writeX = (sx+3) & (~3);


//					for (i=0;i<readX;i++)
					for (i=0;i<writeX;i++)
					{
						m_loadBuffer2[i] = 0;
					}

					int y = startY;

					y++;

					for (j=0;j<sy;j++)
					{
						fread(tmpBuffer,sizeof(char),readX,file);

						//color change
						for (int i=0;i<sx;i++)
						{
							int orgB = (int)(tmpBuffer[i*3+0]);
							int orgG = (int)(tmpBuffer[i*3+1]);
							int orgR = (int)(tmpBuffer[i*3+2]);
							orgR &= 0xff;
							orgG &= 0xff;
							orgB &= 0xff;
							int yuvY = (orgR * 299 + orgG * 587 + orgB * 114 + 500) / 1000;
							if (yuvY>255) yuvY=255;
							m_loadBuffer2[y*writeX+i] = (char)yuvY;

						}

					//	fread(&m_loadBuffer[y*readX],sizeof(char),readX,file);
						y += dy;
					}
					


					bmf.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
					bmi.biBitCount = 8;
					bmi.biClrUsed = 0;
					bmi.biClrImportant = 0;

					readX = writeX;

					bError = FALSE;
				}


			}
		}
		fclose(file);
	}

	if (bError)
	{
		PrintFatalError(filename,"LoadPaletteError");
		return rc;
	}

	int bc = bmi.biClrUsed;
	if (bc == 0) bc = 256;

	//cut main
	int upperCut = 0;
	int lowerCut = 0;
	int leftCut = 0;
	int rightCut = 0;

	for (j=0;j<picSizeY-1;j++)
	{
		BOOL found = FALSE;
		char* ptr = m_loadBuffer2;
		ptr += j * dataSizeX;

		for (i=0;i<picSizeX;i++)
		{
			char c = *ptr;
			int d = (int)c;
			d &=0xff;
			if (rgbquad[d].rgbBlue != 0)
			{
				found = TRUE;
				break;
			}
			ptr++;
		}

		if (found)
		{
			break;
		}
		upperCut++;
	}

	for (j=picSizeY-1;j>0;j--)
	{
		BOOL found = FALSE;
		char* ptr = m_loadBuffer2;
		ptr += j * dataSizeX;

		for (i=0;i<picSizeX;i++)
		{
			char c = *ptr;
			int d = (int)c;
			d &=0xff;
			if (rgbquad[d].rgbBlue != 0)
			{
				found = TRUE;
				break;
			}
			ptr++;
		}

		if (found)
		{
			break;
		}
		lowerCut++;
	}



	if ((upperCut+lowerCut) >= picSizeY)
	{
		upperCut = picSizeY - 1;
		lowerCut = 0;
	}


	for (i=0;i<picSizeX-1;i++)
	{
		BOOL found = FALSE;
		char* ptr = m_loadBuffer2;
		ptr += i;

		for (j=0;j<picSizeY;j++)
		{
			char c = *ptr;
			int d = (int)c;
			d &=0xff;
			if (rgbquad[d].rgbBlue != 0)
			{
				found = TRUE;
				break;
			}

			ptr += dataSizeX;
		}

		if (found)
		{
			break;
		}
		leftCut++;
	}

	for (i=picSizeX-1;i>0;i--)
	{
		BOOL found = FALSE;
		char* ptr = m_loadBuffer2;
		ptr += i;

		for (j=0;j<picSizeY;j++)
		{
			char c = *ptr;
			int d = (int)c;
			d &=0xff;
			if (rgbquad[d].rgbBlue != 0)
			{
				found = TRUE;
				break;
			}

			ptr += dataSizeX;
		}

		if (found)
		{
			break;
		}
		rightCut++;
	}

	if ((leftCut+rightCut) >= picSizeX)
	{
		leftCut = picSizeX - 1;
		rightCut = 0;
	}


	rc.top = upperCut;
	rc.left = leftCut;
	rc.right = rightCut;
	rc.bottom = lowerCut;

	return rc;
}


RECT CPackRoutine::CalcuCutPng(SIZE picSize,char* maskData)
{
	RECT rc;

	//cut main
	int upperCut = 0;
	int lowerCut = 0;
	int leftCut = 0;
	int rightCut = 0;

	int i,j;

	int picSizeX = picSize.cx;
	int picSizeY = picSize.cy;


	for (j=0;j<picSizeY-1;j++)
	{
		BOOL found = FALSE;
		char* ptr = maskData;
		ptr += j * picSizeX;

		for (i=0;i<picSizeX;i++)
		{
			char c = *ptr;
			if ( c != 0)
			{
				found = TRUE;
				break;
			}
			ptr++;
		}

		if (found)
		{
			break;
		}
		upperCut++;
	}

	for (j=picSizeY-1;j>0;j--)
	{
		BOOL found = FALSE;
		char* ptr = maskData;
		ptr += j * picSizeX;

		for (i=0;i<picSizeX;i++)
		{
			char c = *ptr;
			if (c != 0)
			{
				found = TRUE;
				break;
			}
			ptr++;
		}

		if (found)
		{
			break;
		}
		lowerCut++;
	}



	if ((upperCut+lowerCut) >= picSizeY)
	{
		upperCut = picSizeY - 1;
		lowerCut = 0;
	}


	for (i=0;i<picSizeX-1;i++)
	{
		BOOL found = FALSE;
		char* ptr = maskData;
		ptr += i;

		for (j=0;j<picSizeY;j++)
		{
			char c = *ptr;
			if (c != 0)
			{
				found = TRUE;
				break;
			}

			ptr += picSizeX;
		}

		if (found)
		{
			break;
		}
		leftCut++;
	}

	for (i=picSizeX-1;i>0;i--)
	{
		BOOL found = FALSE;
		char* ptr = maskData;
		ptr += i;

		for (j=0;j<picSizeY;j++)
		{
			char c = *ptr;
			if (c != 0)
			{
				found = TRUE;
				break;
			}

	
			ptr += picSizeX;
		}

		if (found)
		{
			break;
		}
		rightCut++;
	}

	if ((leftCut+rightCut) >= picSizeX)
	{
		leftCut = picSizeX - 1;
		rightCut = 0;
	}


	rc.top = upperCut;
	rc.left = leftCut;
	rc.right = rightCut;
	rc.bottom = lowerCut;

	return rc;
}

int CPackRoutine::MakeAndSaveMask(FILE* file2, LPSTR dirName,LPSTR filename)
{
	int dataSize = 0;

	int i,j;
	char srcFileName[256];
	wsprintf(srcFileName,"%s\\%sm.bmp",dirName,filename);


	FILE* file = NULL;
	fopen_s(&file,srcFileName,"rb");

	int sx,sy,dy,startY,readX;

	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;
	RGBQUAD rgbquad[256];

	char tmpBuffer[8192*4];

	BOOL bError = TRUE;
	if (file != NULL)
	{
		if (fread(&bmf,sizeof(BITMAPFILEHEADER),1,file) == 1)
		{
			if (fread(&bmi,sizeof(BITMAPINFOHEADER),1,file) == 1)
			{
				//load palette
				ZeroMemory(&rgbquad,sizeof(RGBQUAD)*256);
				if (bmi.biBitCount == 8)
				{
					int p = bmi.biClrUsed;
					if (p == 0) p = 256;
					bmi.biClrUsed = p;

					fread(&rgbquad,sizeof(RGBQUAD),p,file);

					sx = bmi.biWidth;
					sy = bmi.biHeight;
					dy = -1;
					startY = sy - 1;

					if (sy<0)
					{
						sy = -sy;
						dy = -dy;
						startY = 0;
					}


					readX = (sx+3) & (~3);

					for (i=0;i<readX;i++)
					{
						m_loadBuffer[i] = 0;
					}

					int y = startY;

					y++;

					for (j=0;j<sy;j++)
					{
						fread(&m_loadBuffer[y*readX],sizeof(char),readX,file);
						y += dy;
					}
					
					bError = FALSE;
				}
				else if (bmi.biBitCount == 24)
				{
					//make dummy palette
					for (i=0;i<256;i++)
					{
						rgbquad[i].rgbRed = i;
						rgbquad[i].rgbGreen = i;
						rgbquad[i].rgbBlue = i;
						rgbquad[i].rgbReserved =  0;
					}

					sx = bmi.biWidth;
					sy = bmi.biHeight;
					dy = -1;
					startY = sy - 1;

					if (sy<0)
					{
						sy = -sy;
						dy = -dy;
						startY = 0;
					}

					readX = (sx*3+3) & (~3);
					int writeX = (sx+3) & (~3);


//					for (i=0;i<readX;i++)
					for (i=0;i<writeX;i++)
					{
						m_loadBuffer[i] = 0;
					}

					int y = startY;

					y++;

					for (j=0;j<sy;j++)
					{
						fread(tmpBuffer,sizeof(char),readX,file);

						//color change
						for (int i=0;i<sx;i++)
						{
							int orgB = (int)(tmpBuffer[i*3+0]);
							int orgG = (int)(tmpBuffer[i*3+1]);
							int orgR = (int)(tmpBuffer[i*3+2]);
							orgR &= 0xff;
							orgG &= 0xff;
							orgB &= 0xff;
							int yuvY = (orgR * 299 + orgG * 587 + orgB * 114 + 500) / 1000;
							if (yuvY>255) yuvY=255;
							m_loadBuffer[y*writeX+i] = (char)yuvY;

						}

					//	fread(&m_loadBuffer[y*readX],sizeof(char),readX,file);
						y += dy;
					}
					


					bmf.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
					bmi.biBitCount = 8;
					bmi.biClrUsed = 0;
					bmi.biClrImportant = 0;

					readX = writeX;

					bError = FALSE;
				}
			}
		}
		fclose(file);
	}

	if (bError)
	{
		PrintFatalError(filename,"CreateMaskError");
		return -1;
	}



//	FILE* file2 = fopen(filename2,"wb");
//	if (file2 == NULL)
//	{
//		MessageBox(NULL,"Save Open Error","Error",MB_OK);
//		return -2;
//	}



//	fwrite(packHeader1,sizeof(char),64,file2);
	fwrite(&bmf,sizeof(bmf),1,file2);
	fwrite(&bmi,sizeof(bmi),1,file2);
	int bc = bmi.biClrUsed;
	if (bc == 0) bc = 256;
	fwrite(&rgbquad,sizeof(RGBQUAD),bc,file2);

	//pack main
	//xor
	for (j=sy;j>=1;j--)
	{
		for (i=0;i<sx;i++)
		{
			char c = m_loadBuffer[j*readX+i];
			c ^= m_loadBuffer[(j-1)*readX+i];
			m_loadBuffer[j*readX+i] = c;
		}
	}

	int savePtr = 0;
	//set header

	//pack
	for (j=0;j<sy;j++)
	{
		int pt = 0;
		int ln = 0;
		while (pt<sx)
		{
			char c = m_loadBuffer[(j+1)*readX+pt];
			pt++;

			if (c != 0)
			{
				if (ln>0)
				{
					m_saveBuffer[savePtr] = 0;
					savePtr++;
					m_saveBuffer[savePtr] = ln;
					savePtr++;
					ln=0;
				}


				m_saveBuffer[savePtr] = c;
				savePtr++;
				ln = 0;
			}
			else
			{
				ln++;
				if ((ln>=255) || (pt>=sx))
				{
					m_saveBuffer[savePtr] = 0;
					savePtr++;
					m_saveBuffer[savePtr] = ln;
					savePtr++;
					ln=0;
				}
			}
		}
	}


	fwrite(m_saveBuffer,sizeof(char),savePtr,file2);

//	fclose(file2);

//	char mes[256];
//	sprintf(mes,"file size=%d",savePtr + sizeof(bmf) + sizeof(bmi) + sizeof(rgbquad));
//	MessageBox(NULL,mes,"Size",MB_OK);


	return savePtr;	//unknown error


	return dataSize;
}


BOOL CPackRoutine::GetJpegSize(char* jpegData,SIZE* lpSize)
{
	char* ptr = jpegData;

//MessageBox(NULL,"0","0",MB_OK);
	while (TRUE)
	{
//MessageBox(NULL,"1","1",MB_OK);

		ptr = HuffmanSearchCommand(ptr,640*480*4);
		int c = (int)*ptr;
		c &= 0xff;

		ptr++;

		if (c == JPEG_EOI)
		{
MessageBox(NULL,"JPEG DATA ERROR","ERROR",MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
		if (c == JPEG_SOI) continue;

		if (c == JPEG_SOF0)
		{
			lpSize->cx = GetWORDData(ptr+5);
			lpSize->cy = GetWORDData(ptr+3);
			return TRUE;
		}

		if (c == JPEG_SOS)
		{
			break;
		}

		ptr += GetWORDData(ptr);

	}

MessageBox(NULL,"JPEG DATA WARNING","WARNING",MB_OK | MB_ICONINFORMATION );

	lpSize->cx = 640;
	lpSize->cy = 480;

	return FALSE;
}



char* CPackRoutine::HuffmanSearchCommand(char* dataPtr, int maxDataSize)
{
	char* src = dataPtr;
	int loopSize = maxDataSize;
	char* ans;

	__asm
	{
		push eax
		push ecx
		push esi

		mov esi,src
		mov ecx,loopSize

		xor eax,eax
		mov al,0ffh
LOOP0:
		cmp al,[esi]
		jz FOUND1
		inc esi
		dec ecx
		jnz LOOP0
NOT_FOUND:
		mov eax,-1
		jmp LAST
FOUND1:
		inc esi
		dec ecx
		jz NOT_FOUND
		cmp al,[esi]	;FF FF
		jz LOOP0
;
		cmp ah,[esi]
		jz LOOP0

		mov eax,esi
LAST:
		mov ans,eax

		pop esi
		pop ecx
		pop eax
	}

	return ans;
}

int CPackRoutine::GetWORDData(char* jpegPtr)
{
	int high = *jpegPtr;
	int low = *(jpegPtr+1);
	high &= 0xff;
	low &= 0xff;

	return (high<<8) | low;
}


void CPackRoutine::PrintFatalError(LPSTR filename,LPSTR caption)
{
	if (m_errorSkipFlag) return;

	char text[1024];
	wsprintf(text,"エラー:%s\nエラー表示を続けますか?",filename);

	int rt = MessageBox(NULL,text,caption,MB_OKCANCEL | MB_ICONEXCLAMATION);
	if (rt == IDCANCEL) m_errorSkipFlag = TRUE;
}


void CPackRoutine::ChangeRGB(int sx,int sy)
{
	char* ptr0 = m_loadBuffer;
	int nextY = (sx*3+3) & (~3);
	int loopX = sx;
	int loopY = sy;

	__asm
	{
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi

		mov edi,ptr0

		mov edx,loopY
LOOP1:
		push edi

		mov ecx,loopX
LOOP2:
		mov al,[edi+0]
		mov bl,[edi+2]
		mov [edi+0],bl
		mov [edi+2],al
		add edi,3
		dec ecx
		jnz LOOP2

		pop edi
		add edi,nextY
		dec edx
		jnz LOOP1


		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}



void CPackRoutine::SetProgressWindow(HWND staticMessage,HWND bar1,HWND bar2)
{
	m_staticText = staticMessage;
	m_progressBar = bar1;
	m_progressBar2 = bar2;
}


/*_*/

