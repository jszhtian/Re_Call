//
// main.cpp
//

#include <windows.h>
#include <stdio.h>
//#include <commctrl.h>


#include "..\..\systemNNN\nyanLib\include\commonMacro.h"
#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "..\..\systemNNN\nnnUtilLib\commonSystemFile.h"
#include "main.h"

#include "..\..\systemNNN\nnnUtilLib\mySaveFolder.h"


#include "resource.h"

CNameList* m_setup = NULL;

//#pragma comment(lib,"Comctl32.lib")

BOOL LoadSetupFile(LPSTR filenameonly, int varKosuu);
BOOL GetInitGameParam(int* lpVar, LPSTR name);
BOOL GetInitGameString(LPSTR* lpStr, LPSTR name);


char m_default2DExeName[]="nnnExe.exe";
char m_default3DExeName[]="nnn3DExe.exe";
char m_defaultSelectExeName[]="nnnSelecter.exe";

//#include "..\nyanlib\include\libinclude.h"


int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	int md = 2;

	LoadSetupFile("mainControl",512);


	int taikenFlag = 0;
	GetInitGameParam(&taikenFlag,"taikenFlag");

	LPSTR saveFolder = CMySaveFolder::GetFullFolder();
	char filenamebuf[1024];


	FILE* file = NULL;
	if (taikenFlag == 0)
	{
		wsprintf(filenamebuf,"%s\\sysfile.sav",saveFolder);
		fopen_s(&file,filenamebuf,"rb");

//		fopen_s(&file,"sav\\sysfile.sav","rb");
	}
	else
	{
		wsprintf(filenamebuf,"%s\\tsysfile.sav",saveFolder);
		fopen_s(&file,filenamebuf,"rb");

//		fopen_s(&file,"sav\\tsysfile.sav","rb");
	}

	if (file == NULL)
	{
		if (taikenFlag == 0)
		{
			fopen_s(&file,"sav\\sysfile.org","rb");
		}
		else
		{
			fopen_s(&file,"sav\\tsysfile.org","rb");
		}
	}


	if (file != NULL)
	{
		fseek(file,0,SEEK_END);
		int ln = ftell(file);
		char* buf = new char[ln];
		fseek(file,0,SEEK_SET);
		fread(buf,sizeof(char),ln,file);
		fclose(file);

		char* ptr = buf;
		ptr += sizeof(CCommonSystemFile::SYSTEMDATAINFO);
	
		CCommonSystemFile::SYSTEMDATA* systemData = (CCommonSystemFile::SYSTEMDATA*)ptr;
		md = systemData->d2d3d;
		delete [] buf;
	}

	LPSTR filename = m_defaultSelectExeName;

	if (md == 2)
	{
		filename = m_default2DExeName;
		GetInitGameString(&filename,"filename2DExe");
	}
	else if (md == 3)
	{
		filename = m_default3DExeName;
		GetInitGameString(&filename,"filename3DExe");
	}

	ENDDELETECLASS(m_setup);

	ShellExecute(NULL,"open",filename,NULL,NULL,SW_SHOW);
	return 0;
}


BOOL LoadSetupFile(LPSTR filenameonly, int varKosuu)
{
	if (m_setup == NULL)
	{
		m_setup = new CNameList(varKosuu * 2);
	}

	if (m_setup == NULL) return FALSE;

	return m_setup->LoadInit(filenameonly);
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



