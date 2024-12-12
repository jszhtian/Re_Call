//
// main.cpp
//


#include <windows.h>
#include <stdio.h>
#include <process.h>

#include "..\..\systemNNN\nnnUtilLib\getCommandLineParam.h"

HWND globalHWND = NULL;

char m_tmp[768];
int Pack(void);

int m_a[256];


//pack format
//int kosuu
//int filetable offset=16
//int filenamedata offset
//int packdataoffset
//

//
//filetable
//{
//	nameoffset(fromfilenamedata offset)
//  dataoffset(from packdataoffset)
//}
//


//data
// int type	0:nopack 1:pack
// int packedsize
// int makedsize
// int 0
// data...
//


int m_fileKosuu = 0;
char* m_filenameBuffer = NULL;
int m_filenameBufferSize = 0;
int m_filenamePointer = 0;

int* m_filenameTable = NULL;
int m_filenameTableKosuu = 0;

int* m_fileSetTable = NULL;
int m_fileSetTableKosuu = 0;


char* m_readBuffer = NULL;
int m_readBufferSize = 0;


char* m_packedDataBuffer = NULL;
int m_packedDataBufferSize = 0;

int m_dataOffset = 0;

void CreateDummyFile(LPSTR dirName);

void PackRoutine(LPSTR dirName,LPSTR extName1,LPSTR extName2,BOOL packFlag1,BOOL packFlag2 = FALSE);
void NonPackCopy(FILE* dstFile,LPSTR dirName,LPSTR filename);
void PackCopy(FILE* dstFile,LPSTR dirName,LPSTR filename);


int CalcuPackSub(FILE* file,int readFileSize,FILE* dstFile = NULL);

int PackBlock(char* sourcePtr);

void AddFileName(LPSTR filename);

int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	char cur[1024];
	GetCurrentDirectory(1024,cur);

	m_filenameBufferSize = 1024*256;
	m_filenameBuffer = new char[m_filenameBufferSize];

	m_readBufferSize = 1024*1024*9;
	m_readBuffer = new char[m_readBufferSize];

	m_filenameTableKosuu = 1024;
	m_filenameTable = new int[m_filenameTableKosuu ];

	m_fileSetTableKosuu = 1024;
	m_fileSetTable = new int[m_fileSetTableKosuu * 2];

	m_packedDataBufferSize = 1024*64;
	 m_packedDataBuffer = new char[m_packedDataBufferSize];

	CreateDirectory("ダミーフォルダー",NULL);

	CreateDirectory("ダミーフォルダー\\dwq",NULL);
	CreateDirectory("ダミーフォルダー\\cdvaw",NULL);

	CreateDummyFile("dwq");
	CreateDummyFile("cdvaw");

	BOOL packFlag1 = FALSE;
	int rt = MessageBox(NULL,"dwqに簡易圧縮をかけますか","DWQ圧縮",MB_YESNO);
	if (rt == IDYES) packFlag1 = TRUE;

	PackRoutine("dwq","gpk","gtb",packFlag1);
	PackRoutine("cdvaw","vpk","vtb",FALSE);

	if (m_packedDataBuffer != NULL) delete [] m_packedDataBuffer;
	if (m_fileSetTable != NULL) delete [] m_fileSetTable;
	if (m_filenameTable != NULL) delete [] m_filenameTable;
	if (m_readBuffer != NULL) delete [] m_readBuffer;
	if (m_filenameBuffer != NULL) delete [] m_filenameBuffer;

	return 0;


//	CreateDirectory("ダミーフォルダー\\pkdwq",NULL);
//	CreateDirectory("ダミーフォルダー\\pkcdvaw",NULL);

//	SetCurrentDirectory("dwq");

	for (int i=0;i<256;i++)
	{
		m_a[i] = 0;
	}

	FILE* file = NULL;
	fopen_s(&file,"dwq\\ev0.gpk","rb");
	if (file != NULL)
	{
		fseek(file,0,SEEK_END);
		int ln = ftell(file);
		fseek(file,0,SEEK_SET);

		int lp = ln / 768;
		int sz = 0;
		int n = 0;

		fseek(file,0,SEEK_SET);

		while (n<ln)
		{
			fread(m_tmp,sizeof(char),768,file);
			n += 768;
			sz += Pack();
		}

		fclose(file);
		char mes[1024];

		for (int i=0;i<256;i++)
		{
			wsprintf(mes,"[%d:%d]",i,m_a[i]/10000);
			OutputDebugString(mes);
			if ((i % 16) == 0)
			{
				OutputDebugString("\n");
			}

		}

		wsprintf(mes,"%dMB / %dMB",sz/1024/1024,ln/1024/1024);


		MessageBox(NULL,mes,"size",MB_OK);


	}




//	system("compress.exe -R *.gpk ..\\ダミーフォルダー\\pkdwq");
//	SetCurrentDirectory("..\\cdvaw");
//	system("compress.exe -R *.vpk ..\\ダミーフォルダー\\pkcdvaw");
//	SetCurrentDirectory(cur);

//	int er1 = (int)ShellExecute(NULL,"open","compress.exe","-R dwq\\sc0.gpk ダミーフォルダー\\pkdwq\\sc0.gp_",cur,SW_SHOW);
//	int er2 = (int)ShellExecute(NULL,"open","compress.exe","cdvaw\\*.vpk ダミーフォルダー\\pkcdvaw",cur,SW_SHOW);

//	system("copy dwq\\*.gtb ダミーフォルダー\\pkdwq");
//	system("copy cdvaw\\*.vtb ダミーフォルダー\\pkcdvaw");





	//make 1byte file


	return 0;



	char buff1[1024];
	char buff2[1024];
	char buff3[1024];
	buff1[0] = 0;
	buff2[0] = 0;
	buff3[0] = 0;

	char* lplpBuff[3];

	lplpBuff[0] = buff1;
	lplpBuff[1] = buff2;

//	int cmd = CGetCommandLineParam::GetParam(3,1024,lplpBuff,lpCmdLine);
	int cmd = 0;

	BOOL flg = FALSE;
	if (cmd < 2)
	{
		char mes[1024];
		wsprintf(mes,"%s\n個数=%d\n1:%s\n2:%s",lpCmdLine,cmd,buff1,buff2);

		MessageBox(NULL,mes,"Error:パラメータが足りません",MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		if (_stricmp(buff1,"install") != 0)
		{
			MessageBox(NULL,"1つめのパラメーターが/Installになっていません","Error",MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			flg = TRUE;
		}
	}

	if (flg)
	{
		if (strlen(buff2) == 2)
		{
			buff2[2] = '\\';
			buff2[3] = 0;
		}

		char sourceDir[1024];
		wsprintf(sourceDir,"%sDirectX",buff2);

MessageBox(NULL,sourceDir,"SourceDir",MB_OK);

		SetCurrentDirectory(sourceDir);
		ShellExecute(NULL,"open","dxsetup.exe",NULL,NULL,SW_SHOW);
	}


	SetCurrentDirectory(cur);
	return 0;
}

int Pack(void)
{
	char tmp[256];
	int szTotal = 0;

	for (int n=0;n<3;n++)
	{
		int sz = 0;
		tmp[0] = m_tmp[n];
		tmp[1] = m_tmp[3+n] - m_tmp[n];

		for (int i=2;i<256;i++)
		{
//			tmp[i] = m_tmp[i*3+n] - (m_tmp[(i-1)*3+n] * 2 - m_tmp[(i-2)*3+n]); 
			tmp[i] = m_tmp[i*3+n] - m_tmp[(i-1)*3+n];
		}

		for (int i=0;i<256;i++)
		{
			int a = ((int)(tmp[i])) & 0xff;
			m_a[a]++;
		}

		int ln = 0;
		int old = -1;

		for (int i=0;i<256;i++)
		{
			char c = tmp[i];
			if (c == 0)
			{
				sz++;
				continue;
				if (old == 0)
				{
					ln++;
					if (ln>7)
					{
						sz += 7;
						old = -1;
					}
				}
				else
				{
					old = 0;
					ln = 1;
				}
			}
			else if ((c>=-4) && (c<=3))
			{
				sz += 4;
//				if (old == 0)
//				{
//					sz += 7;
//				}
//				sz += 4;
//				old = c;
			}
			else
			{
				sz += 12;
//				if (old == 0)
//				{
//					sz += 7;
//				}
//				sz += 12;
//				old = c;
			}
		}

		if (0)
		{
			szTotal += (0 + (sz+7) / 8);
		}
		else
		{
			if (sz > 256*8)
			{
				szTotal += 257;
			}
			else
			{
				szTotal += (1 + (sz+7) / 8);
			}
		}
	}


	return szTotal;
}


void CreateDummyFile(LPSTR dirName)
{
	WIN32_FIND_DATA find;

	char searchName[1024];
	wsprintf(searchName,"%s\\*.*",dirName);

	HANDLE handle = FindFirstFile(searchName,&find);
	if (handle == INVALID_HANDLE_VALUE) return;

	int n = 0;
	while (1)
	{
		LPSTR cFileName = find.cFileName;
		if ((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			if ((_stricmp(cFileName,".") != 0) && (_stricmp(cFileName,"..") != 0))
			{
				FILE* file = NULL;
				char filename[1024];
				wsprintf(filename,"ダミーフォルダー\\%s\\%s",dirName,cFileName);
				fopen_s(&file,filename,"wb");
				
				if (file != NULL)
				{
					fwrite(&n,sizeof(int),1,file);
					fclose(file);
					n++;
				}
			}
		}


		if (FindNextFile(handle,&find) == 0)
		{
			break;
		}
	}

	FindClose(handle);
}


void PackRoutine(LPSTR dirName,LPSTR extName1,LPSTR extName2,BOOL packFlag1,BOOL packFlag2)
{
//packFlag1 = FALSE;
	//make file list
	m_fileKosuu = 0;
	m_filenamePointer = 0;

	WIN32_FIND_DATA find;

	char searchName[1024];
	wsprintf(searchName,"%s\\*.%s",dirName,extName1);

	HANDLE handle = FindFirstFile(searchName,&find);
	if (handle != INVALID_HANDLE_VALUE)
	{
		while (1)
		{
			if ((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				LPSTR cFileName = find.cFileName;
				if ((_stricmp(cFileName,".") != 0) && (_stricmp(cFileName,"..") != 0))
				{
					AddFileName(cFileName);
				}
			}

			if (FindNextFile(handle,&find) == 0)
			{
				break;
			}

		}

		FindClose(handle);
	}

	int firstKosuu = m_fileKosuu;

	wsprintf(searchName,"%s\\*.%s",dirName,extName2);

	handle = FindFirstFile(searchName,&find);
	if (handle != INVALID_HANDLE_VALUE)
	{
		while (1)
		{
			if ((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				LPSTR cFileName = find.cFileName;
				if ((_stricmp(cFileName,".") != 0) && (_stricmp(cFileName,"..") != 0))
				{
					AddFileName(cFileName);
				}
			}

			if (FindNextFile(handle,&find) == 0)
			{
				break;
			}

		}

		FindClose(handle);
	}


	if (m_fileKosuu >= m_fileSetTableKosuu)
	{
		int* tmp2 = new int[(m_fileSetTableKosuu + 256) * 2];
		memcpy(tmp2,m_fileSetTable,sizeof(int)*m_fileSetTableKosuu*2);

		delete [] m_fileSetTable;
		m_fileSetTable = tmp2;
		m_fileSetTableKosuu += 256;
	}

	//make dummy header (nameoffset only)
	for (int i=0;i<m_fileKosuu;i++)
	{
		*(m_fileSetTable+i*2) = *(m_filenameTable+i);
	}

	FILE* dstFile = NULL;
	char dstFileName[1024];
	wsprintf(dstFileName,"ダミーフォルダー\\%s.nyp",dirName);
	fopen_s(&dstFile,dstFileName,"wb");
	if (dstFile == NULL) return;	//error!

	fwrite(&m_fileKosuu,sizeof(int),1,dstFile);

	int fileSetTableOffset = 16;
	fwrite(&fileSetTableOffset,sizeof(int),1,dstFile);

	int filenameDataOffset = 16 + m_fileKosuu * 2 * sizeof(int);
	fwrite(&filenameDataOffset,sizeof(int),1,dstFile);

	int packDataOffset = filenameDataOffset + m_filenamePointer;
	fwrite(&packDataOffset,sizeof(int),1,dstFile);

	//dummy table
	fwrite(m_fileSetTable,sizeof(int)*2,m_fileKosuu,dstFile);

	//filenamedata
	fwrite(m_filenameBuffer,1,m_filenamePointer,dstFile);




	m_dataOffset = 0;
	//pack or copy main
	for (int i=0;i<m_fileKosuu;i++)
	{
		BOOL packFlag = FALSE;
		if (i<firstKosuu)
		{
			packFlag = packFlag1;
		}
		else
		{
			packFlag = packFlag2;
		}

		LPSTR cFilename = m_filenameBuffer + m_filenameTable[i];


		*(m_fileSetTable+i*2+1) = m_dataOffset;

		if (packFlag == FALSE)
		{
			NonPackCopy(dstFile,dirName,cFilename);
		}
		else
		{
			PackCopy(dstFile,dirName,cFilename);
		}

	}



	//seekしてヘッダーかきこみ
	fseek(dstFile,16,SEEK_SET);
	fwrite(m_fileSetTable,sizeof(int)*2,m_fileKosuu,dstFile);

	fclose(dstFile);
}

void PackCopy(FILE* dstFile,LPSTR dirName,LPSTR filename)
{
	int writeSize = 0;

	int packType = 1;
	fwrite(&packType,sizeof(int),1,dstFile);
	writeSize += sizeof(int);


	FILE* file = NULL;
	char loadfilename[1024];
	wsprintf(loadfilename,"%s\\%s",dirName,filename);

	fopen_s(&file,loadfilename,"rb");
	if (file == NULL)
	{
		MessageBox(NULL,loadfilename,"Load Error",MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	fseek(file,0,SEEK_END);
	int ln = ftell(file);
	fseek(file,0,SEEK_SET);
	
	int packedSize = CalcuPackSub(file,ln);
	fwrite(&packedSize,sizeof(int),1,dstFile);
	writeSize += sizeof(int);

	int makedSize = ln;
	fwrite(&makedSize,sizeof(int),1,dstFile);
	writeSize += sizeof(int);

	int dummy = 0;
	fwrite(&dummy,sizeof(int),1,dstFile);
	writeSize += sizeof(int);

	fseek(file,0,SEEK_SET);

	writeSize += CalcuPackSub(file,ln,dstFile);

	m_dataOffset += writeSize;

	fclose(file);

}

int CalcuPackSub(FILE* file,int readFileSize,FILE* dstFile)
{
	int readNokori = readFileSize;
	int packSize = 0;

	while (readNokori > 0)
	{
		int loadLength = readNokori;
		if (loadLength > m_readBufferSize)
		{
			loadLength = m_readBufferSize;
		}

		fread(m_readBuffer,sizeof(char),loadLength,file);
		int block = loadLength / (256*3);
		int blockAmari = loadLength % (256*3);
		if (blockAmari > 0)
		{
			//fill zero last
			for (int i=blockAmari;i<256*3;i++)
			{
				*(m_readBuffer+block*256*3+i) = 0;
			}

			block++;
		}

		for (int i=0;i<block;i++)
		{
			int packBlockSize = PackBlock(m_readBuffer+i*256*3);
			packSize += packBlockSize;

			if (dstFile != NULL)
			{
				fwrite(m_packedDataBuffer,sizeof(char),packBlockSize,dstFile);
			}
		}

		readNokori -= loadLength;
	}


	return packSize;
}


int PackBlock(char* sourcePtr)
{
	int packedSize = 0;
	*m_packedDataBuffer = 0;
	packedSize++;

	char tmp[256];
	int szTotal = 0;

	char zeroMaskWork[32];
	char pack3Work[256];///3?
	char pack8Work[256];

	unsigned char bitPattern8[8];
	
	for (int i=0;i<7;i++)
	{
		bitPattern8[i] = 1 << i;
	}
	bitPattern8[7] = 0x80;

	for (int n=0;n<3;n++)
	{
		int sz = 0;
		tmp[0] = sourcePtr[n];

		for (int i=1;i<256;i++)
		{
			tmp[i] = sourcePtr[i*3+n] - sourcePtr[(i-1)*3+n];
		}


		unsigned char  zeroMask = 0;
		int pZeroMask = 0;
		int pack8Size = 0;
		int pack3Size = 0;
		int pack3Bit = 0;
		int pack3BitLength = 0;

		for (int i=1;i<256;i++)
		{
			if ((i & 7) == 0)
			{
				zeroMask = 0;
			}

			char c = tmp[i];
			if (c != 0)
			{
				zeroMask |= bitPattern8[i & 7];

				if ((c>=-4) && (c<=3))
				{
					int cc = (int)c;
					cc &= 0x7;
					pack3Bit |= (cc << pack3BitLength);
				}
				else
				{
					pack8Work[pack8Size] = c;
					pack8Size++;
				}

				pack3BitLength += 3;

				if (pack3BitLength >= 8)
				{
					pack3Work[pack3Size] = pack3Bit & 0xff;
					pack3Size++;
					
					pack3BitLength -= 8;
					pack3Bit >>= 8;
				}
			}


			if ((i & 7) == 7)
			{
				zeroMaskWork[pZeroMask] = zeroMask;
				pZeroMask++;
			}
		}

		if (pack3BitLength > 0)
		{
			pack3Work[pack3Size] = pack3Bit & 0xff;
			pack3Size++;

			pack3Bit = 0;
			pack3BitLength = 0;
		}

		sz += 1;
		sz += 1;
		sz += 32;
		sz += pack3Size;
		sz += pack8Size;

		if (sz > 256)
		{
			//no-pack

			for (int i=0;i<256;i++)
			{
				*(m_packedDataBuffer+packedSize) = sourcePtr[i*3+n];
				packedSize++;
			}
		}
		else
		{
			*m_packedDataBuffer |= (1<<n);//pack flag


			*(m_packedDataBuffer+packedSize) = tmp[0];
			packedSize++;

			*(m_packedDataBuffer+packedSize) = pack3Size;
			packedSize++;

			for (int i=0;i<32;i++)
			{
				*(m_packedDataBuffer+packedSize) = zeroMaskWork[i];
				packedSize++;
			}

			for (int i=0;i<pack3Size;i++)
			{
				*(m_packedDataBuffer+packedSize) = pack3Work[i];
				packedSize++;
			}

			for (int i=0;i<pack8Size;i++)
			{
				*(m_packedDataBuffer+packedSize) = pack8Work[i];
				packedSize++;
			}
		}
	}


	return packedSize;
}


void NonPackCopy(FILE* dstFile,LPSTR dirName,LPSTR filename)
{
	int writeSize = 0;

	int packType = 0;
	fwrite(&packType,sizeof(int),1,dstFile);
	writeSize += sizeof(int);

	FILE* file = NULL;
	char loadfilename[1024];
	wsprintf(loadfilename,"%s\\%s",dirName,filename);

	fopen_s(&file,loadfilename,"rb");
	if (file == NULL)
	{
		MessageBox(NULL,loadfilename,"Load Error",MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	fseek(file,0,SEEK_END);
	int ln = ftell(file);
	fseek(file,0,SEEK_SET);
	
	int packedSize = ln;
	fwrite(&packedSize,sizeof(int),1,dstFile);
	writeSize += sizeof(int);

	int makedSize = ln;
	fwrite(&makedSize,sizeof(int),1,dstFile);
	writeSize += sizeof(int);

	int dummy = 0;
	fwrite(&dummy,sizeof(int),1,dstFile);
	writeSize += sizeof(int);


	int nokori = ln;
	while (nokori > 0)
	{
		int loadSize = nokori;
		if (loadSize > m_readBufferSize)
		{
			loadSize = m_readBufferSize;
		}

		fread(m_readBuffer,sizeof(char),loadSize,file);
		fwrite(m_readBuffer,sizeof(char),loadSize,dstFile);
		writeSize += loadSize;

		nokori -= loadSize;
	}

	m_dataOffset += writeSize;

	fclose(file);
}


void AddFileName(LPSTR filename)
{
	int ln = strlen(filename);
	int ln2 = (ln+1+3) & ~3;

	if ((m_filenamePointer + ln2) > m_filenameBufferSize)
	{
		//re-create
		char* tmp = new char[m_filenameBufferSize + 1024*256];
		memcpy(tmp,m_filenameBuffer,m_filenameBufferSize);

		delete [] m_filenameBuffer;
		m_filenameBuffer = tmp;
		m_filenameBufferSize += 1024*256;
	}

	if (m_fileKosuu >= m_filenameTableKosuu)
	{
		int* tmp2 = new int[m_filenameTableKosuu + 256];
		memcpy(tmp2,m_filenameTable,sizeof(int)*m_filenameTableKosuu);

		delete [] m_filenameTable;
		m_filenameTable = tmp2;
		m_filenameTableKosuu += 256;
	}

	for (int i=0;i<4;i++)
	{
		*(m_filenameBuffer+m_filenamePointer+ln2-4+i) = 0;
	}
	memcpy(m_filenameBuffer+m_filenamePointer,filename,ln);

	m_filenameTable[m_fileKosuu] = m_filenamePointer;

	m_filenamePointer += ln2;
	m_fileKosuu++;
}


/*_*/


