//
// main.cpp
//

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>


#include "..\..\systemNNN\nyanLib\include\commonMacro.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"
//#include "miniScript.h"
//#include "waveData.h"

#include "main.h"


//#include "checkfiledate.h"
//#include "resource.h"

#include "myFindFile.h"


#pragma comment(lib,"Comctl32.lib")


int m_checkButtonTable[]=
{
	0
	//IDC_CHECK_BGM,
	//IDC_CHECK_VOICE,
	//IDC_CHECK_SYS,
	//IDC_CHECK_SE,
	//IDC_CHECK_PACK,
};



int m_checkPackTable[]=
{
	0,0, 1,1, 2,2, -1,-1, 3,4, 5,5, 6,6,
};



int m_radioButtonTable[]=
{
	0
	//IDC_RADIO_ONLYNEW,
	//IDC_RADIO_COMPLETE,

	//IDC_RADIO_OGG,
	//IDC_RADIO_WAVE,

	//IDC_RADIO_OGG2,
	//IDC_RADIO_WAVE2,

	//IDC_RADIO_OGG3,
	//IDC_RADIO_WAVE3,
};








BOOL m_wavSysFlag = FALSE;
BOOL m_wavSeFlag = FALSE;
BOOL m_voiceFlag = FALSE;
BOOL m_bgmFlag = FALSE;




HWND m_check[9];
HWND m_radio[9];

HWND m_editBox;

HWND m_processDialog;

HWND m_staticText;
HWND m_progressBar;
HWND m_progressBar2;

//CMiniScript* m_miniScript = NULL;
//CWaveData* m_waveData = NULL;

int m_checkButtonStatus[9];
int m_radioButtonStatus[9];
int m_version = 0;

char m_versionBuffer[256];

//CCheckFileDate* m_checkDate = NULL;
//CPackRoutine* m_packRoutine = NULL;
CMyFindFile* m_myFindFile = NULL;
//CMyFindFile* m_myFindFile2 = NULL;
//CMyLoad* m_load = NULL;
//CMySave* m_save = NULL;

CNameList* m_nameList = NULL;
BOOL m_charaListLoadFlag = FALSE;

void SetCheckButton(int n);
void SetRadioButton(int n);
void GetCheckButton(int n);
void GetRadioButton(int n);

void PackMusic(void);
void PackVoice(void);
void PackSystemSound(void);
void PackScriptSound(void);
void KatameRoutine(void);

void LoadCharaList(void);

BOOL CALLBACK DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

BOOL CheckExistDir(LPSTR dir);
BOOL CheckExistDir(LPSTR srcDir0,LPSTR srcDir1,LPSTR srcDir2);
BOOL CheckAndCreateDir(LPSTR srcDir,LPSTR dstDir);


int MyCompare( const void* arg1, const void* arg2 );


int MyErrorMessage(LPSTR filename,LPSTR mes,int n);

/*
//wave header
char nopackHeader[]= "\
IF PACKTYPE==0  \
CUT THIS 64 BYTE\
THEN REMAKE WAV \
PACKTYPE=0      ";

//ogg
char packHeader2[]= "\
                \
WAVE HEADER+OGG \
                \
PACKTYPE=2      ";

*/


//wave music
char packHeader4[]= "\
                \
WAVE            \
                \
PACKTYPE=4      ";


//ogg music
char packHeader6[]= "\
                \
OGG             \
                \
PACKTYPE=6      ";



FILE* list = NULL;

//nnndir\charaname.txtから判断する
//

char* m_buffer = NULL;
char* m_headerBuffer = NULL;

char* m_filenameBuffer = NULL;
int* m_filenameTable = NULL;

int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	for (int i=0;i<9;i++)
	{
//		m_checkButtonStatus[i] = 0;
	}

//	m_radioButtonStatus[0] = 0;
//	m_radioButtonStatus[1] = 1;
//	m_radioButtonStatus[2] = 1;
//	m_radioButtonStatus[3] = 0;
//	m_radioButtonStatus[4] = 1;
//	m_radioButtonStatus[5] = 0;
//	m_radioButtonStatus[6] = 0;
//	m_radioButtonStatus[7] = 1;

/*
	FILE* file = fopen("nerune.cfg","rb");
	if (file != NULL)
	{
		for (i=0;i<9;i++)
		{
			fread(m_checkButtonStatus,sizeof(int),9,file);
		}
		fread(m_radioButtonStatus,sizeof(int),9,file);

		fclose(file);
	}
*/



	InitCommonControls();

//	int rt = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG_SETUP),NULL,DlgProc,(LPARAM)NULL);
//	if (rt != 0) return 1;

	m_nameList = new CNameList();

	m_myFindFile = new CMyFindFile();



//	m_nameList->LoadFile("spt\\project.xtx");

//	m_miniScript = new CMiniScript();
//	m_waveData = new CWaveData();


	BOOL exitFlag = FALSE;

	if (m_myFindFile->SearchStart("init\\*.xtx"))
	{
		LPSTR initFile = NULL;
		while ((initFile = m_myFindFile->SearchNext()) != NULL)
		{
			ENDDELETECLASS(m_nameList);
			m_nameList = new CNameList();
			char filename[256];
			wsprintf(filename,"init\\%s.xtx",initFile);

			if (m_nameList->LoadFile(filename))
			{
				int kosuu = m_nameList->GetNameKosuu();
				if ((kosuu % 2) == 1)
				{
					int rt = MyErrorMessage(initFile,"個数が奇数です",0);
					if (rt == IDCANCEL)
					{
						exitFlag = TRUE;
						break;
					}
				}


				for (int i=0;i<kosuu/2;i++)
				{
					LPSTR para1 = m_nameList->GetName(i*2);
					BOOL er = FALSE;

					if (strcmp(para1,"0") == 0)
					{
						er = TRUE;
					}
					else
					{
						if (atoi(para1) != 0)
						{
							er = TRUE;
						}
					}

					if (er)
					{
						int rt = MyErrorMessage(initFile,"ずれてる可能性あり",i+1);
						if (rt == IDCANCEL)
						{
							exitFlag = TRUE;
							break;
						}
					}
				}

				for (int j=0;j<kosuu/2-1;j++)
				{
					LPSTR name1 = m_nameList->GetName(j*2);
					for (int k=j+1;k<kosuu/2;k++)
					{
						LPSTR name2 = m_nameList->GetName(k*2);
						if (_stricmp(name1,name2) == 0)
						{
							char mes[1024];
							wsprintf(mes,"同一の設定が存在します:%s",name1);
							int rt = MyErrorMessage(initFile,mes,j+1);
							if (rt == IDCANCEL)
							{
								exitFlag = TRUE;
								break;
							}
						}
					}
					if (exitFlag) break;
				}
			}
		}
	}


//	DELETEARRAY(m_buffer);
//	ENDDELETECLASS(m_waveData);
//	ENDDELETECLASS(m_miniScript);
	ENDDELETECLASS(m_myFindFile);
	ENDDELETECLASS(m_nameList);
/*
	file = fopen("nerune.cfg","wb");
	if (file != NULL)
	{
		for (i=0;i<9;i++)
		{
			fwrite(m_checkButtonStatus,sizeof(int),9,file);
		}
		fwrite(m_radioButtonStatus,sizeof(int),9,file);
		fclose(file);
	}
*/

//	DestroyWindow(m_processDialog);
	MessageBox(NULL,"おわりましたにゃ","End",MB_OK);
//	PostMessage(m_processDialog,WM_CLOSE,0,0);

	return 0;
}


/*
72章
１．コモンコントロールの準備
２．PBM_SETRANGメッセージを送信して、範囲を指定する
３．PBM_SETSTEPを送信して、１ステップの増分を指定
４．PBM_STEPITを送信して１ステップだけ増加させる
*/


BOOL CheckExistDir(LPSTR dir)
{
	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile(dir,&findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		FindClose(handle);
		return TRUE;
	}
	return FALSE;
}


int MyErrorMessage(LPSTR filename,LPSTR mes,int n)
{
	char emes[1024];
	if (n==0)
	{
		wsprintf(emes,"file=%s\n%s",filename,mes);
	}
	else
	{
		wsprintf(emes,"file=%s\n%s\n%dこめあたり",filename,mes,n);
	}

	int rt = MessageBox(NULL,emes,"ERROR",MB_OKCANCEL | MB_ICONEXCLAMATION);
	return rt;
}


/*_*/



