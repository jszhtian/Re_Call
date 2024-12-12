//
// myloadwave.cpp
//

#include <windows.h>
#include <stdio.h>

#include "myloadwave.h"

char CMyLoadWave::m_srcDir[] = "cdwave";
char CMyLoadWave::m_dstDir[] = "cdvaw";
char CMyLoadWave::m_srcDir2[] = "wav";
char CMyLoadWave::m_dstDir2[] = "vaw";
char CMyLoadWave::m_srcDir3[] = "cdogg";
char CMyLoadWave::m_dstDir3[] = "vaw";
char CMyLoadWave::m_srcExt[] = "wav";
char CMyLoadWave::m_dstExt[] = "vaw";

char CMyLoadWave::m_srcExt3[] = "ogg";

CMyLoadWave::CMyLoadWave()
{
}

CMyLoadWave::~CMyLoadWave()
{
	End();
}

void CMyLoadWave::End(void)
{
}

int CMyLoadWave::LoadWave(LPSTR subDir,LPSTR filename,void* headerBuffer, void* dataBuffer, int type)
{
	char makedfilename[256];
	char headerfilename[256];
	if (type == 2)
	{
		sprintf(headerfilename,"%s\\%s\\%s.%s",m_srcDir,subDir,filename,m_srcExt);
		sprintf(makedfilename,"%s\\%s\\%s.%s",m_srcDir3,subDir,filename,m_srcExt3);
	}
	else if (type == 1)
	{
		sprintf(makedfilename,"%s\\%s\\%s.%s",m_srcDir,subDir,filename,m_srcExt);
	}
	else
	{
		sprintf(makedfilename,"%s\\%s\\%s.%s",m_srcDir2,subDir,filename,m_srcExt);
	}


	FILE* file = fopen(makedfilename,"rb");
	if (file == NULL) return -1;
	
	FILE* file0 = file;
	if (type == 2)
	{
		file0 = fopen(headerfilename,"rb");
	}


	fread(headerBuffer,sizeof(char),20,file0);
	char* header = (char*)headerBuffer;

	int headerSize = *((int*)(header+16));

	fread(header+20,sizeof(char),headerSize,file0);

	BOOL flg = TRUE;
	
	char* data = (char*)dataBuffer;

	int dataSize = 0;


	while(flg)
	{
		int ln = fread(dataBuffer,sizeof(char),8,file0);
		if (ln == 0)
		{
			MessageBox(NULL,"bad wave file","FATAL ERROR",MB_OK);
			flg = FALSE;
			break;
		}

		if (
			(*(data) == 'd') &&
			(*(data+1) == 'a') &&
			(*(data+2) == 't') &&
			(*(data+3) == 'a')
			)
		{
			int sz2 = *((int*)(data+4));
			int ln2 = 0;
			if ((type == 0) || (type == 1))
			{
				ln2 = fread(data+8,sizeof(char),sz2,file);
			}
			else
			{
				ln2 = fread(data+8,sizeof(char),1024*1024*8,file);	//max 8M
			}

			//noise cut
			if (type == 1)
			{
				sz2 = NoiseCut(data+8,sz2);
				NoiseCut2(data+8,sz2);
			}

//			dataSize = 8 + sz2;
			dataSize = 8 + ln2;

			*((int*)(header + 4)) = 20 + headerSize + sz2;
			*((int*)(data + 4)) = sz2;

			break;
		}
		else
		{
			//skip header
			int sz2 = *((int*)(data+4));
			int ln2 = fread(data+8,sizeof(char),sz2,file0);
		}
	}
/*
	if (type != 2)
	{
		while(flg)
		{
			int ln = fread(dataBuffer,sizeof(char),8,file);
			if (ln == 0)
			{
				MessageBox(NULL,"bad wave file","FATAL ERROR",MB_OK);
				flg = FALSE;
				break;
			}

			int sz2 = *((int*)(data+4));
			int ln2 = fread(data+8,sizeof(char),sz2,file);

			//check data

			if (
				(*(data) == 'd') &&
				(*(data+1) == 'a') &&
				(*(data+2) == 't') &&
				(*(data+3) == 'a')
				)
			{


				//noise cut
				sz2 = NoiseCut(data+8,sz2);
				NoiseCut2(data+8,sz2);

				dataSize = 8 + sz2;

				*((int*)(header + 4)) = headerSize + 4 + 8 + dataSize;
				*((int*)(data + 4)) = sz2;

				break;
			}

	//		MessageBox(NULL,filename,"ERROR BAD DATA",MB_OK);
		}
	}
	else
	{
		int ln = fread(dataBuffer,sizeof(char),1024*1024*8,file);
		if (ln == 0)
		{
			MessageBox(NULL,"bad ogg file","FATAL ERROR",MB_OK);
			flg = FALSE;
		}
		dataSize = ln;
	}
*/


	fclose(file);
	if (type == 2) fclose(file0);

	if (flg == FALSE) return 0;

	return dataSize;
}



int CMyLoadWave::NoiseCut(void* buf, int sz)
{
	short * ptr = (short*)buf;
	int lastFound = 0;
	int ln = sz / 2;

	int lvl = 64;
	int mlvl = -lvl;

	for (int i=0;i<ln;i++)
	{
		short d = *ptr;
		ptr++;
		if ((d <= mlvl) || (d >= lvl)) lastFound = i;
	}

	return lastFound * 2;
}

void CMyLoadWave::NoiseCut2(void* buf, int sz)
{
	short * ptr = (short*)buf;
	int lastFound = 0;
	int ln = sz / 2;

	int lvl = 64;
	int mlvl = -lvl;

	for (int i=0;i<ln;i++)
	{
		short d = *ptr;
		if ((d <= mlvl) || (d >= lvl))
		{
			lastFound = i;
		}
		else
		{
			if ((i-lastFound) >= 20)
			{
				*ptr = 0;
			}
		}
		ptr++;
	}

}
