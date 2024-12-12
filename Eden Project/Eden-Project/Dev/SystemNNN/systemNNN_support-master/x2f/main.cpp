//
// main.cpp
//

#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>

//#include <file.h>
//#include <commctrl.h>

#include "main.h"

#pragma comment(lib,"shlwapi.lib")


void Angou(WIN32_FIND_DATA* lpFind);
int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow);

int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{

/*
	FILE* file = NULL;
	fopen_s(&file,"spt\\var.fxf","rb");
	if (file != NULL)
	{
		FILE* file2 = NULL;
		fopen_s(&file2,"var.txt","wb");
		if (file2 != NULL)
		{
			char buf[16384];
			char buf2[16384];

			int ln = fread(buf,sizeof(char),16384,file);

			for (int i=0;i<ln;i++)
			{
				char cc = buf[i];
				cc ^= 0xff;

				buf2[i] = cc;
			}

			fwrite(buf2,sizeof(char),ln,file2);

			fclose(file2);
		}
		fclose(file);
		MessageBox(NULL,"‚¨‚í‚Á‚½‚É‚á","OK",MB_OK);
	}

	return 0;
*/



//	BOOL createFlag = compFlag;

	WIN32_FIND_DATA findData;
	int kosuu = 0;

	HANDLE handle = FindFirstFile("*.xtx",&findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			Angou(&findData);
		}

		WIN32_FIND_DATA findData2;
		while (FindNextFile(handle,&findData2) != 0)
		{
			Angou(&findData2);
		}
		FindClose(handle);
	}
	MessageBox(NULL,"‚¨‚í‚Á‚½‚É‚á","x2f",MB_OK);
	return 0;
}


void Angou(WIN32_FIND_DATA* lpFind)
{
	LPSTR fname = lpFind->cFileName;

	LPSTR ext = PathFindExtension(fname);
	if (_stricmp(ext,".xtx") != 0) return;

	LPSTR filenameonly0 = PathFindFileName(fname);
	char filenameonly[1024];
	wsprintf(filenameonly,"%s",filenameonly0);
	
	PathRemoveExtension(filenameonly);

	char newName[1024];
	wsprintf(newName,"%s.fxf",filenameonly);

	FILE* file = NULL;
	fopen_s(&file,fname,"rb");
	if (file == NULL) return;

	FILE* file2 = NULL;
	fopen_s(&file2,newName,"wb");

	if (file2 != NULL)
	{
		BOOL flg = TRUE;
		char buf[8192];

		while (flg)
		{
			int ln = fread(buf,sizeof(char),8192,file);
			
			for (int i=0;i<ln;i++)
			{
				buf[i] ^= 0xff;
			}

			if (ln>0)
			{
				fwrite(buf,sizeof(char),ln,file2);
			}

			if (ln<8192)
			{
				flg = FALSE;
			}
		}
	}

	fclose(file2);
	fclose(file);
}


/*_*/



