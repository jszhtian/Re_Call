//
//
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "..\..\systemNNN\nnnUtilLib\mySaveFolder.h"

#include "configData.h"


CConfigData::CConfigData()
{
	m_nameList = new CNameList();
	
	m_loadOkFlag = FALSE;
	m_editFlag = FALSE;


	char filename[1024];
	wsprintf(filename,"%s\\config.txt",CMySaveFolder::GetFullFolder());

	if (m_nameList->LoadFile("nnndir\\setup\\config.txt",FALSE,TRUE))
	{
		m_loadOkFlag = TRUE;
	}


/*
	if (m_nameList->LoadFile(filename,FALSE,TRUE))
	{
		m_loadOkFlag = TRUE;
	}
	else
	{
		m_editFlag = TRUE;	//new data not found

		if (m_nameList->LoadFile("nnndir\\setup\\config.txt",FALSE,TRUE) == FALSE)
		{
			m_nameList->ExpandWork(512);
			MessageBox(NULL,"設定を新規に作成しました。\n設定はマイドキュメント以下に保存されます","確認",MB_OK);
		}
		else
		{
			MessageBox(NULL,"旧フォルダーの設定を読み込みました。\n設定はマイドキュメント以下に保存されるように変更されました","確認",MB_OK);
			m_loadOkFlag = TRUE;
		}
	}
*/


	m_kosuu = m_nameList->GetNameKosuu() / 2;
}



CConfigData::~CConfigData()
{
	End();
}

void CConfigData::End(void)
{
	ENDDELETECLASS(m_nameList);
}



LPSTR CConfigData::GetString(LPSTR name)
{
	int n = SearchName(name);

	if (n == -1)
	{
		n = SearchAkiOrExpand(name);
		SetString(name,"0");
	}

	return m_nameList->GetName(n*2+1);
}

int CConfigData::GetData(LPSTR name)
{
	int n = SearchName(name);
	if (n == -1)
	{
		n = SearchAkiOrExpand(name);
		SetData(name,0);
	}

	return atoi(m_nameList->GetName(n*2+1));
}

void CConfigData::SetString(LPSTR name,LPSTR mes)
{
	int n = SearchName(name);
	if (n == -1)
	{
		n = SearchAkiOrExpand(name);
	}

	m_nameList->SetName(n*2+1,mes);
	m_editFlag = TRUE;
}


void CConfigData::SetData(LPSTR name,int d)
{
	char mes[256];
	wsprintf(mes,"%d",d);
	SetString(name,mes);
}


int CConfigData::SearchName(LPSTR name)
{
	for (int i=0;i<m_kosuu;i++)
	{
		if (_stricmp(name,m_nameList->GetName(i*2)) == 0) return i;
	}

	return -1;
}


int CConfigData::SearchAkiOrExpand(LPSTR name)
{
	m_editFlag = TRUE;

	for (int i=0;i<m_kosuu;i++)
	{
		LPSTR name2 = m_nameList->GetName(i*2);
		if ((*name2) == 0)
		{
			m_nameList->SetName(i*2,name);
			return i;
		}
	}

	int n = m_kosuu;
	m_nameList->ExpandWork(64);
	m_nameList->SetName(n*2,name);
	m_nameList->SetName(n*2+1,"0");
	m_kosuu = m_nameList->GetNameKosuu() / 2;
	return n;
}







void CConfigData::Save(void)
{
	char filename[1024];
	wsprintf(filename,"%s\\config.txt",CMySaveFolder::GetFullFolder());

//	FILE* file = CMyFile::Open("nnndir\\setup\\config.txt","wb");
//	FILE* file = CMyFile::Open("filename","wb");
	FILE* file = NULL;
//	fopen_s(&file,filename,"wb");
	fopen_s(&file,"nnndir\\setup\\config.txt","wb");
	if (file != NULL)
	{
		for (int i=0;i<m_kosuu;i++)
		{
			LPSTR name = m_nameList->GetName(i*2);
			if ((*name) != 0)
			{
				char mes[1000];
				wsprintf(mes,"%s,%s\x00d\x00a",m_nameList->GetName(i*2),m_nameList->GetName(i*2+1));
				fwrite(mes,sizeof(char),strlen(mes),file);
				
//				wsprintf(mes,"[write=%d]",w);
//				OutputDebugString(mes);
			}
		}
		fclose(file);
	}
}


/*_*/


