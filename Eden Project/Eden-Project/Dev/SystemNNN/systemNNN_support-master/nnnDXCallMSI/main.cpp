//
// main.cpp
//


#include <windows.h>
#include <stdio.h>
#include <process.h>

#include "..\..\systemNNN\nnnUtilLib\getCommandLineParam.h"

HWND globalHWND = NULL;


int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	char cur[1024];
	GetCurrentDirectory(1024,cur);

	char buff1[1024];
	char buff2[1024];
	char buff3[1024];
	buff1[0] = 0;
	buff2[0] = 0;
	buff3[0] = 0;

	char* lplpBuff[3];

	lplpBuff[0] = buff1;
	lplpBuff[1] = buff2;

	int cmd = CGetCommandLineParam::GetParam(3,1024,lplpBuff,lpCmdLine);

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

//MessageBox(NULL,sourceDir,"SourceDir",MB_OK);

		SetCurrentDirectory(sourceDir);
		ShellExecute(NULL,"open","dxsetup.exe",NULL,NULL,SW_SHOW);
	}


	SetCurrentDirectory(cur);
	return 0;
}
