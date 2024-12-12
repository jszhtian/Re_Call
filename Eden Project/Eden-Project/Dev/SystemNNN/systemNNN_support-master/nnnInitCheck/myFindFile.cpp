//
//
//


#include <windows.h>
#include <stdio.h>

#include "myFindFile.h"


CMyFindFile::CMyFindFile()
{
	m_handle = NULL;
}


CMyFindFile::~CMyFindFile()
{
	End();
}


void CMyFindFile::End(void)
{
	if (m_handle != NULL)
	{
		FindClose(m_handle);
		m_handle = NULL;
	}
}



BOOL CMyFindFile::SearchStart(LPSTR searchName)
{
	m_handle = FindFirstFile(searchName,&m_findData);
	if (m_handle == NULL) return FALSE;

	m_firstFlag = TRUE;

	return TRUE;
}


LPSTR CMyFindFile::SearchNextDir(void)
{
	while (1)
	{
		if (m_firstFlag)
		{
			m_firstFlag = FALSE;
		}
		else
		{
			if (FindNextFile(m_handle,&m_findData) == FALSE)
			{
				FindClose(m_handle);
				m_handle = NULL;
				return NULL;
			}
		}

		if ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			continue;
		}

		if (m_findData.cFileName[0] == '.')
		{
			continue;
		}

		for (int i=0;i<1024;i++)
		{
			char c = m_findData.cFileName[i];
			m_filenameOnly[i] = c;
//			if ((c == '.') || (c == 0))
			if (c == 0)
			{
				m_filenameOnly[i] = 0;
				break;
			}
		}

		return m_filenameOnly;
	}

	return NULL;
}


LPSTR CMyFindFile::SearchNext(void)
{
	while (1)
	{
		if (m_firstFlag)
		{
			m_firstFlag = FALSE;
		}
		else
		{
			if (FindNextFile(m_handle,&m_findData) == FALSE)
			{
				FindClose(m_handle);
				m_handle = NULL;
				return NULL;
			}
		}

		if (m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}

		if (m_findData.cFileName[0] == '.')
		{
			continue;
		}

		for (int i=0;i<1024;i++)
		{
			char c = m_findData.cFileName[i];
			m_filenameOnly[i] = c;
			if ((c == '.') || (c == 0))
			{
				m_filenameOnly[i] = 0;
				break;
			}
		}

		return m_filenameOnly;
	}

	return NULL;//???
}

void CMyFindFile::SearchEnd(void)
{
	if (m_handle != NULL)
	{
		FindClose(m_handle);
		m_handle = NULL;
	}
}


/*_*/


