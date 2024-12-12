//
// main.cpp
//


#include <windows.h>
#include <stdio.h>
#include <process.h>


#include <errno.h>

//#include <commctrl.h>

//#include "resource.h"

#include "..\..\systemNNN\nnnUtilLib\getCommandLineParam.h"

//#pragma comment(lib,"Comctl32.lib")




int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	char cur[1024];
	GetCurrentDirectory(1024,cur);

	char buff1[1024];
	char buff2[1024];
	char buff3[1024];
	char buff4[1024];
	char buff5[1024];


	buff1[0] = 0;
	buff2[0] = 0;
	buff3[0] = 0;
	buff4[0] = 0;
	buff5[0] = 0;

	char* lplpBuff[5];

	lplpBuff[0] = buff1;
	lplpBuff[1] = buff2;
	lplpBuff[2] = buff3;
	lplpBuff[3] = buff4;
	lplpBuff[4] = buff5;

	int cmd = CGetCommandLineParam::GetParam(5,1024,lplpBuff,lpCmdLine);

//	MessageBox(NULL,lpCmdLine,"lpCmdLine",MB_OK | MB_ICONEXCLAMATION);

	for (int i=0;i<cmd;i++)
	{
//		MessageBox(NULL,lplpBuff[i],"cmd",MB_OK);
	}

//cmd = 2;
//buff1[0] = 'a';
//buff1[1] = 0;
//buff2[0] = 'b';
//buff2[1] = 0;

	if (cmd == 0)
	{
		MessageBox(NULL,"Error:パラメータがありません","Error",MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	int cmdType = 0;
	int cmdLimit = 2;
	if (_stricmp(buff1,"uninstall") == 0)
	{
		cmdType = 1;
		cmdLimit = 2;
	}
	if (_stricmp(buff1,"exec") == 0)
	{
		cmdType = 2;
		cmdLimit = 2;
	}

	if (cmd<cmdLimit)
	{
		MessageBox(NULL,"Error:パラメータが足りません","Error",MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
//cmdType = 1;

	if (cmdType == 1)
	{
		char execFile[1024];
		char systemFolder[1024];
		GetSystemDirectory(systemFolder,1024);

//		wsprintf(execFile,"\"%s\\msiexec.exe\"",systemFolder);
		wsprintf(execFile,"%s\\msiexec.exe",systemFolder);
//		int a = E2BIG;
//		int b = EACCES;
//		int c = EINVAL;
//		int d = EMFILE;
//		int e = ENOENT;
//		int f = ENOEXEC;
//		int g = ENOMEM;







//MessageBox(NULL,execFile,"execfile",MB_OK);
//		_execl(execFile,execFile,"/x",buff2,"/qb",NULL);
		_execl(execFile,execFile,"/x",buff2,NULL);

		int er = errno;
		char emes[1024];
		wsprintf(emes,"アンインストール呼び出しエラー %d\n\nアンインストールプログラムの呼び出しに失敗しました。\nコントロールパネルのプログラムと削除から削除してください",er);
		MessageBox(NULL,emes,"Error",MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	if (cmdType == 2)
	{
		if (cmd == 2)
		{
			_execl(buff2,buff2);
		}
		else if (cmd == 3)
		{
			_execl(buff2,buff2,buff3);
		}
		else if (cmd == 4)
		{
			_execl(buff2,buff2,buff3,buff4);
		}
		else
		{
			_execl(buff2,buff2,buff3,buff4,buff5);
		}
		return 0;
	}


	return 0;
}



/*_*/


