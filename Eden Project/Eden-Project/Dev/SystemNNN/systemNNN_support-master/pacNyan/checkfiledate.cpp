//
// checkfiledate.cpp
//

#include <windows.h>
#include <stdio.h>

#include "checkfiledate.h"


CCheckFileDate::CCheckFileDate()
{
	m_errorSkipFlag = FALSE;
}

CCheckFileDate::~CCheckFileDate()
{
	End();
}

void CCheckFileDate::End(void)
{
}


BOOL CCheckFileDate::CheckNeedUpdate(LPSTR filename,LPSTR srcDir,LPSTR dstDir,LPSTR extName,BOOL taFlag)
{
	char srcfile[1024];
	char srcfile2[1024];
	char dstfile[1024];

	wsprintf(srcfile, "%s\\%s.%s",srcDir,filename,extName);
	wsprintf(srcfile2,"%s\\%sm.bmp",srcDir,filename);
	wsprintf(dstfile,"%s\\%s.dwq",dstDir,filename);

	FILETIME srctime;
	FILETIME srctime2;
	FILETIME dsttime;


	HANDLE hDst = CreateFile(dstfile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hDst == INVALID_HANDLE_VALUE) return TRUE;
	GetFileTime(hDst,NULL,NULL,&dsttime);
	CloseHandle(hDst);

	HANDLE hSrc = CreateFile(srcfile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hSrc == INVALID_HANDLE_VALUE)
	{
		PrintFatalError(srcfile);
		return FALSE;
	}
	GetFileTime(hSrc,NULL,NULL,&srctime);
	CloseHandle(hSrc);

	if (taFlag)
	{
		//check png?
		if (_stricmp(extName,"png") == 0)
		{
			taFlag = FALSE;
		}
	}


	if (taFlag)
	{
		HANDLE hSrc2 = CreateFile(srcfile2,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hSrc2 == INVALID_HANDLE_VALUE)
		{
			PrintFatalError(srcfile2);
			return FALSE;
		}
		GetFileTime(hSrc2,NULL,NULL,&srctime2);
		CloseHandle(hSrc2);
	}

	if (CompareFileTime(&srctime,&dsttime)>0) return TRUE;

	if (taFlag)
	{
		if (CompareFileTime(&srctime2,&dsttime)>0) return TRUE;
	}

	return FALSE;

}



void CCheckFileDate::PrintFatalError(LPSTR filename)
{
	if (m_errorSkipFlag) return;

	char text[1024];
	wsprintf(text,"Not Found:%s\nエラー表示を続けますか?",filename);
	int rt = MessageBox(NULL,text,"ファイルが見つかりません",MB_OKCANCEL | MB_ICONEXCLAMATION);

	if (rt == IDCANCEL) m_errorSkipFlag = TRUE;
}



