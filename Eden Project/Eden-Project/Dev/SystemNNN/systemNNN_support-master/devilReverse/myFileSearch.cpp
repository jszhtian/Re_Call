






#include <windows.h>
//#include <stdio.h>
//#include <string.h>
//#include <commctrl.h>


#include "myFileSearch.h"

CMyFileSearch::CMyFileSearch()
{
	int sz = sizeof(WIN32_FIND_DATA);
	m_win32FindData = new char[sz];
	m_win32FindData2 = new char[sz];
	m_startFlag = FALSE;
	m_secondFlag = FALSE;
	m_handle = NULL;

	m_win32FindDataDir = new char[sz];
	m_win32FindData2Dir = new char[sz];

	m_startFlagDir = FALSE;
	m_secondFlagDir = FALSE;
	m_handleDir = NULL;
}

CMyFileSearch::~CMyFileSearch()
{
	End();
}

void CMyFileSearch::End(void)
{
	SearchEnd();
	if (m_win32FindData != NULL)
	{
		delete [] m_win32FindData;
		m_win32FindData = NULL;
	}

	if (m_win32FindData2 != NULL)
	{
		delete [] m_win32FindData2;
		m_win32FindData2 = NULL;
	}

	SearchDirEnd();
	if (m_win32FindDataDir != NULL)
	{
		delete [] m_win32FindDataDir;
		m_win32FindDataDir = NULL;
	}


	if (m_win32FindData2Dir != NULL)
	{
		delete [] m_win32FindData2Dir;
		m_win32FindData2Dir = NULL;
	}

}


BOOL CMyFileSearch::SearchStart(LPSTR dirName,LPSTR fileName)
{
	WIN32_FIND_DATA* lpFind = (WIN32_FIND_DATA*)m_win32FindData;
	char startFile[1024];
	wsprintf(startFile,"%s\\%s",dirName,fileName);

	m_handle = FindFirstFile(startFile,lpFind);
	if (m_handle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	m_startFlag = TRUE;
	m_secondFlag = FALSE;
	return TRUE;
}


LPSTR CMyFileSearch::GetName(void)
{
	if (m_startFlag == FALSE) return NULL;

	WIN32_FIND_DATA* lpFind = (WIN32_FIND_DATA*)m_win32FindData2;

	if (m_secondFlag == FALSE)
	{
		lpFind = (WIN32_FIND_DATA*)m_win32FindData;
		m_secondFlag = TRUE;
	}
	else
	{
		if (FindNextFile(m_handle,lpFind) == 0)
		{
			return NULL;
		}
	}

	if ((lpFind->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		return GetName();
	}

	LPSTR name = lpFind->cFileName;
	if (_stricmp(name,".") == 0)
	{
		return GetName();
	}
	if (_stricmp(name,"..") == 0)
	{
		return GetName();
	}

	return name;
}



void CMyFileSearch::SearchEnd(void)
{
	if (m_startFlag == FALSE) return;
	if (m_handle != INVALID_HANDLE_VALUE) 
	{
		FindClose(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}

	m_startFlag = FALSE;
}



BOOL CMyFileSearch::SearchDirStart(LPSTR dirName)
{
	WIN32_FIND_DATA* lpFind = (WIN32_FIND_DATA*)m_win32FindDataDir;
	char startFile[1024];
	wsprintf(startFile,"%s\\*",dirName);

	m_handleDir = FindFirstFile(startFile,lpFind);
	if (m_handleDir == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	m_startFlagDir = TRUE;
	m_secondFlagDir = FALSE;
	return TRUE;
}


LPSTR CMyFileSearch::GetDir(void)
{
	if (m_startFlagDir == FALSE) return NULL;

	WIN32_FIND_DATA* lpFind = (WIN32_FIND_DATA*)m_win32FindData2Dir;

	if (m_secondFlagDir == FALSE)
	{
		lpFind = (WIN32_FIND_DATA*)m_win32FindDataDir;
		m_secondFlagDir = TRUE;
	}
	else
	{
		if (FindNextFile(m_handleDir,lpFind) == 0)
		{
			return NULL;
		}
	}

	if (!(lpFind->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		return GetDir();
	}

	LPSTR name = lpFind->cFileName;
	if (_stricmp(name,".") == 0)
	{
		return GetDir();
	}
	if (_stricmp(name,"..") == 0)
	{
		return GetDir();
	}

	return name;
}



void CMyFileSearch::SearchDirEnd(void)
{
	if (m_startFlagDir == FALSE) return;
	if (m_handleDir != INVALID_HANDLE_VALUE) 
	{
		FindClose(m_handleDir);
		m_handleDir = INVALID_HANDLE_VALUE;
	}

	m_startFlagDir = FALSE;
}

/*

	WIN32_FIND_DATA findData;
	char startFile[256];
	sprintf(startFile,"%s\\*.%s",srcDir,srcDir0);


	char mes[256];
	sprintf(mes,"DWQçÏê¨íÜ %s",srcDir);
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

	*/

