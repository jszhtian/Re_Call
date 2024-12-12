//
// myloadwave.cpp
//

#include <windows.h>
#include <stdio.h>

#include "myload.h"


CMyLoad::CMyLoad()
{
}

CMyLoad::~CMyLoad()
{
	End();
}

void CMyLoad::End(void)
{
}

int CMyLoad::LoadWave(LPSTR waveFilename,void* headerBuffer, void* dataBuffer)
{
	FILE* file = NULL;
	fopen_s(&file,waveFilename,"rb");
	//FILE* file = fopen(waveFilename,"rb");
	if (file == NULL) return -1;
	
	fread(headerBuffer,sizeof(char),12,file);
	char* header = (char*)headerBuffer;

	int headerSize = *((int*)(header+16));//dummy
//	fseek(file,headerSize,SEEK_CUR);
//	fread(header+20,sizeof(char),headerSize,file);

	BOOL flg = TRUE;
	
	char* data = (char*)dataBuffer;

	int dataSize = 0;


	while(flg)
	{
		int ln = fread(data,sizeof(char),8,file);
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
			int ln2 = fread(data+8,sizeof(char),sz2,file);
			dataSize = 8 + ln2;
			*((int*)(header + 4)) = 12 + headerSize + sz2;
			break;
		}
		else if (
					(*(data) == 'f') &&
					(*(data+1) == 'm') &&
					(*(data+2) == 't') &&
					(*(data+3) == ' ')

				)
		{
			memcpy(header+12,data,8);
			headerSize = *((int*)(data+4));
			fread(header+12+8,sizeof(char),headerSize,file);
			headerSize += 8;
		}
		else
		{
			//skip header
			int sz2 = *((int*)(data+4));
			int ln2 = fseek(file,sz2,SEEK_CUR);
		}
	}

	fclose(file);

	if (flg == FALSE) return 0;
	return dataSize;
}


int CMyLoad::LoadOgg(LPSTR waveFilename,LPSTR oggFilename,void* headerBuffer, void* dataBuffer)
{
	FILE* file = NULL;
	fopen_s(&file,oggFilename,"rb");

	//FILE* file = fopen(oggFilename,"rb");
	if (file == NULL) return -1;

	FILE* file0 = NULL;
	fopen_s(&file0,waveFilename,"rb");

//	FILE* file0 = fopen(waveFilename,"rb");


	fread(headerBuffer,sizeof(char),12,file0);
	char* header = (char*)headerBuffer;

	int headerSize = *((int*)(header+16));//dummy
//	fseek(file0,headerSize,SEEK_CUR);

//	int headerSize = *((int*)(header+16));
//	fread(header+20,sizeof(char),headerSize,file0);



	BOOL flg = TRUE;
	
	char* data = (char*)dataBuffer;

	int dataSize = 0;


	while(flg)
	{
		int ln = fread(data,sizeof(char),8,file0);
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
			int ln2 = fread(data+8,sizeof(char),1024*1024*8,file);	//max 8M

			dataSize = 8 + ln2;

			*((int*)(header + 4)) = 12 + headerSize + sz2;
			*((int*)(data + 4)) = sz2;

			break;
		}
		else if (
					(*(data) == 'f') &&
					(*(data+1) == 'm') &&
					(*(data+2) == 't') &&
					(*(data+3) == ' ')

				)
		{
			memcpy(header+12,data,8);
			headerSize = *((int*)(data+4));
			fread(header+12+8,sizeof(char),headerSize,file0);
			headerSize += 8;
		}
		else
		{
			//skip header
			int sz2 = *((int*)(data+4));
			int ln2 = fseek(file0,sz2,SEEK_CUR);
		}
	}

	fclose(file);
	fclose(file0);

	if (flg == FALSE) return 0;

	return dataSize;
}


/*_*/

