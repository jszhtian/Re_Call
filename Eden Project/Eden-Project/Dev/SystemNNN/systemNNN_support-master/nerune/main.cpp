//
// main.cpp
//

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>


#include "..\..\systemNNN\nyanLib\include\commonMacro.h"

#include "nameList.h"

#include "main.h"




#include "checkfiledate.h"

#include "resource.h"

#include "myFindFile.h"

#include "packroutine.h"

#include "myLoad.h"
#include "mySave.h"

#pragma comment(lib,"Comctl32.lib")


int m_checkButtonTable[]=
{
	IDC_CHECK_BGM,
	IDC_CHECK_VOICE,
	IDC_CHECK_SYS,
	IDC_CHECK_SE,
	IDC_CHECK_PACK,
};



int m_checkPackTable[]=
{
	0,0, 1,1, 2,2, -1,-1, 3,4, 5,5, 6,6,
};



int m_radioButtonTable[]=
{
	IDC_RADIO_ONLYNEW,
	IDC_RADIO_COMPLETE,

	IDC_RADIO_OGG,
	IDC_RADIO_WAVE,

	IDC_RADIO_OGG2,
	IDC_RADIO_WAVE2,

	IDC_RADIO_OGG3,
	IDC_RADIO_WAVE3,
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


int m_checkButtonStatus[9];
int m_radioButtonStatus[9];
int m_version = 0;

char m_versionBuffer[256];

CCheckFileDate* m_checkDate = NULL;
CPackRoutine* m_packRoutine = NULL;
CMyFindFile* m_myFindFile = NULL;
CMyFindFile* m_myFindFile2 = NULL;
CMyLoad* m_load = NULL;
CMySave* m_save = NULL;

CNameList* m_charaList = NULL;
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
	int i;
	for (i=0;i<9;i++)
	{
		m_checkButtonStatus[i] = 0;
	}

	m_radioButtonStatus[0] = 0;
	m_radioButtonStatus[1] = 1;
	m_radioButtonStatus[2] = 1;
	m_radioButtonStatus[3] = 0;
	m_radioButtonStatus[4] = 1;
	m_radioButtonStatus[5] = 0;
	m_radioButtonStatus[6] = 0;
	m_radioButtonStatus[7] = 1;

	FILE* file = NULL;

	fopen_s(&file,"nerune.cfg","rb");
	//FILE* file = fopen("nerune.cfg","rb");
	if (file != NULL)
	{
		for (i=0;i<9;i++)
		{
			fread(m_checkButtonStatus,sizeof(int),9,file);
		}
		fread(m_radioButtonStatus,sizeof(int),9,file);

		fclose(file);
	}

	InitCommonControls();

	int rt = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG_SETUP),NULL,DlgProc,(LPARAM)NULL);
	if (rt != 0) return 1;

	m_checkDate = new CCheckFileDate();
	m_load = new CMyLoad();
	m_save = new CMySave();

	m_buffer = new char[1024*1024*12];
	m_headerBuffer = new char[1024*1024];

	m_myFindFile = new CMyFindFile();
	m_myFindFile2 = new CMyFindFile();
//	m_packRoutine = new CPackRoutine();


	if (CheckExistDir("wav"))
	{
		if (CheckExistDir("wav\\sys")) m_wavSysFlag = TRUE;
		if (CheckExistDir("wav\\se")) m_wavSeFlag = TRUE;
	}


	if (m_radioButtonStatus[6])
	{
		m_wavSysFlag = TRUE;
		m_wavSeFlag = TRUE;
	}


	if (CheckExistDir("bgm")) m_bgmFlag = TRUE;

	if (CheckExistDir("cdwave")) m_voiceFlag = TRUE;




	m_processDialog = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG_PROCESS),NULL,DlgProc2);
//	SendMessage(m_progressBar,PBM_SETPOS,30,0);
	ShowWindow(m_processDialog,SW_SHOW);


	BOOL compFlag = FALSE;
	if (m_radioButtonStatus[1]) compFlag = TRUE;

//SetWindowText(m_staticText,"test1");

	if ((m_checkButtonStatus[0]) && (m_bgmFlag))
	{
		if (CheckExistDir("wgq") == FALSE)
		{
			CreateDirectory("wgq",NULL);
		}
		SetWindowText(m_staticText,"音楽の変換中");
		UpdateWindow(m_staticText);

		PackMusic();
	}

	if ((m_checkButtonStatus[1]) && (m_voiceFlag))
	{
		if (CheckExistDir("cdvaw") == FALSE)
		{
			CreateDirectory("cdvaw",NULL);
		}
		LoadCharaList();
		PackVoice();
	}

	if ((m_checkButtonStatus[2]) && (m_wavSysFlag))
	{
		if (CheckExistDir("vaw") == FALSE)
		{
			CreateDirectory("vaw",NULL);
		}

		if (CheckExistDir("vaw\\sys") == FALSE)
		{
			CreateDirectory("vaw\\sys",NULL);
		}

		SetWindowText(m_staticText,"効果音の変換中");
		UpdateWindow(m_staticText);

		PackSystemSound();
	}

	if ((m_checkButtonStatus[3]) && (m_wavSeFlag))
	{
		if (CheckExistDir("vaw") == FALSE)
		{
			CreateDirectory("vaw",NULL);
		}

		if (CheckExistDir("vaw\\se") == FALSE)
		{
			CreateDirectory("vaw\\se",NULL);
		}

		SetWindowText(m_staticText,"スクリプト効果音の変換中");
		UpdateWindow(m_staticText);

		PackScriptSound();
	}

	if ((m_checkButtonStatus[4]))
	{
		if (CheckExistDir("cdvaw"))
		{
			LoadCharaList();
			KatameRoutine();
		}
	}


	if (m_myFindFile != NULL)
	{
		m_myFindFile->End();
		delete m_myFindFile;
		m_myFindFile = NULL;
	}

	if (m_myFindFile2 != NULL)
	{
		m_myFindFile2->End();
		delete m_myFindFile2;
		m_myFindFile2 = NULL;
	}

	ENDDELETECLASS(m_charaList);
	DELETEARRAY(m_buffer);
	DELETEARRAY(m_headerBuffer);

	ENDDELETECLASS(m_save);
	ENDDELETECLASS(m_load);
	ENDDELETECLASS(m_checkDate);

//	file = fopen("nerune.cfg","wb");
	fopen_s(&file,"nerune.cfg","wb");
	if (file != NULL)
	{
		for (i=0;i<9;i++)
		{
			fwrite(m_checkButtonStatus,sizeof(int),9,file);
		}
		fwrite(m_radioButtonStatus,sizeof(int),9,file);
		fclose(file);
	}


	DestroyWindow(m_processDialog);
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


BOOL CheckExistDir(LPSTR srcDir0,LPSTR srcDir1,LPSTR srcDir2)
{
	char dirName[256];
	wsprintf(dirName,"%s\\%s",srcDir0,srcDir1);
	if (CheckExistDir(dirName) == FALSE) return FALSE;
	if ((*srcDir2) == 0) return TRUE;

	wsprintf(dirName,"%s\\%s\\%s",srcDir0,srcDir1,srcDir2);
	if (CheckExistDir(dirName) == FALSE) return FALSE;

	return TRUE;
}


BOOL CheckAndCreateDir(LPSTR srcDir,LPSTR dstDir)
{
	char dirName[256];
	wsprintf(dirName,"dwq\\%s",srcDir);

	if (CheckExistDir(dirName) == FALSE)
	{
		CreateDirectory(dirName,NULL);
	}


	wsprintf(dirName,"dwq\\%s\\%s",srcDir,dstDir);
	if (CheckExistDir(dirName) == FALSE)
	{
		CreateDirectory(dirName,NULL);
	}


	return TRUE;
}





LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

} /* MainWndproc */





BOOL CALLBACK DlgProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		m_staticText = GetDlgItem(hWnd,IDC_MYSTATIC);
		m_progressBar = GetDlgItem(hWnd,IDC_PROGRESS1);
		m_progressBar2 = GetDlgItem(hWnd,IDC_PROGRESS2);

	//	EnableWindow(m_progressBar,TRUE);
	//	SendMessage(m_progressBar,PBM_SETRANGE,0,MAKELPARAM(0,100));
//		SendMessage(m_progressBar,PBM_SETPOS,70,0);

	//	SetWindowText(m_staticText,"test666");

	//	HWND aaa;
	//	aaa = GetDlgItem(hWnd,IDC_EDIT1);
	//	SetWindowText(aaa,"testdayo");


		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		HWND hwnd0;
		hwnd0 = GetDesktopWindow();

		HDC hdc0;
		hdc0 = GetDC(hwnd0);

		int wx,wy;
		wx = GetDeviceCaps(hdc0,HORZRES);
		wy = GetDeviceCaps(hdc0,VERTRES);
		ReleaseDC(hwnd0,hdc0);

		MoveWindow(hWnd,(wx-sx)/2,(wy-sy)/2,sx,sy,TRUE);

		return TRUE;
		break;
	case WM_CLOSE:
		EndDialog(hWnd,1);

		break;
	}


	return 0;
}


BOOL CALLBACK DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int i;

	switch (message)
	{
	case WM_INITDIALOG:
		for (i=0;i<5;i++)
		{
			m_check[i] = GetDlgItem(hWnd,m_checkButtonTable[i]);
			SetCheckButton(i);
		}

		for (i=0;i<8;i++)
		{
			m_radio[i] = GetDlgItem(hWnd,m_radioButtonTable[i]);
			SetRadioButton(i);
		}

		m_editBox = GetDlgItem(hWnd,IDC_EDIT_VERSION);

		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		HWND hwnd0;
		hwnd0 = GetDesktopWindow();

		HDC hdc0;
		hdc0 = GetDC(hwnd0);

		int wx,wy;
		wx = GetDeviceCaps(hdc0,HORZRES);
		wy = GetDeviceCaps(hdc0,VERTRES);
		ReleaseDC(hwnd0,hdc0);

		MoveWindow(hWnd,(wx-sx)/2,(wy-sy)/2,sx,sy,TRUE);

		return 1;
		break;


	case WM_CLOSE:
		EndDialog(hWnd,1);
		break;


	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			for (i=0;i<5;i++)
			{
				GetCheckButton(i);
			}
			for (i=0;i<8;i++)
			{
				GetRadioButton(i);
			}

			if (GetWindowText(m_editBox,m_versionBuffer,32))
			{
				m_version = atoi(m_versionBuffer);
			}

			EndDialog(hWnd,0);
			break;
		case IDCANCEL:
			EndDialog(hWnd,1);
			break;
		}
		break;
	}

	return FALSE;
}


void SetCheckButton(int n)
{
	int st = BST_UNCHECKED;
	if (m_checkButtonStatus[n]) st = BST_CHECKED;

	SendMessage(m_check[n],BM_SETCHECK,st,0);
}


void SetRadioButton(int n)
{
	int st = BST_UNCHECKED;
	if (m_radioButtonStatus[n]) st = BST_CHECKED;

	SendMessage(m_radio[n],BM_SETCHECK,st,0);
}

void GetCheckButton(int n)
{
	int st = 0;
	if (SendMessage(m_check[n],BM_GETCHECK,0,0) == BST_CHECKED) st = 1;
	m_checkButtonStatus[n] = st;
}

void GetRadioButton(int n)
{
	int st = 0;
	if (SendMessage(m_radio[n],BM_GETCHECK,0,0) == BST_CHECKED) st = 1;
	m_radioButtonStatus[n] = st;
}


void PackMusic(void)
{
	if (m_myFindFile->SearchStart("bgm\\*.ogg"))
	{
		int kosuu = 0;
		while (LPSTR filename = m_myFindFile->SearchNext())
		{
			SendMessage(m_progressBar2,PBM_SETPOS,(kosuu % 10)*10,0);
			
			kosuu++;
			char src[1024];
			char dst[1024];
			wsprintf(src,"bgm\\%s.ogg",filename);
			wsprintf(dst,"wgq\\%s.wgq",filename);


			BOOL flg = TRUE;

			if (m_radioButtonStatus[0])
			{
				flg = m_checkDate->CheckNeedUpdate(src,dst);
			}

			if (flg)
			{
				FILE* file = NULL;
				FILE* file2 = NULL;
				fopen_s(&file,src,"rb");
				fopen_s(&file2,dst,"wb");


//				FILE* file = fopen(src,"rb");
//				FILE* file2 = fopen(dst,"wb");

				fwrite(packHeader6,sizeof(char),64,file2);

				while (TRUE)
				{
					int ln = fread(m_buffer,sizeof(char),1024*1024,file);
					if (ln<=0) break;
					fwrite(m_buffer,sizeof(char),ln,file2);
					if (ln<1024*1024) break;
				}

				fclose(file2);
				fclose(file);
			}
		}
		m_myFindFile->SearchEnd();
	}

	//copy mml
	if (m_myFindFile->SearchStart("bgm\\*.mml"))
	{
		int kosuu = 0;
		while (LPSTR filename = m_myFindFile->SearchNext())
		{
			SendMessage(m_progressBar2,PBM_SETPOS,(kosuu % 10)*10,0);
			kosuu++;
			char src[1024];
			char dst[1024];
			wsprintf(src,"bgm\\%s.mml",filename);
			wsprintf(dst,"wgq\\%s.mml",filename);
			CopyFile(src,dst,FALSE);
		}

		m_myFindFile->SearchEnd();
	}
}



void PackVoice(void)
{
	int ninzu = m_charaList->GetNameKosuu() / 2;
	if (ninzu < 2) return;

	for (int n=1;n<ninzu;n++)
	{
		LPSTR charaname = m_charaList->GetName(n*2);
		int pos = (100*(n-1)+50)/(ninzu-1);

		SendMessage(m_progressBar,PBM_SETPOS,pos,0);

		char searchname[1024];

		if (m_radioButtonStatus[4])
		{
			wsprintf(searchname,"cdwave\\%s*.ogg",charaname);
		}
		else
		{
			wsprintf(searchname,"cdwave\\%s*.wav",charaname);
		}

		char dirname[1024];
		wsprintf(dirname,"cdvaw\\%s",charaname);

		if (CheckExistDir(dirname) == FALSE)
		{
			CreateDirectory(dirname,NULL);
		}


		if (m_myFindFile->SearchStart(searchname))
		{
			int kosuu = 0;

			char mes[256];
			wsprintf(mes,"音声 %s(%s)の変換中",charaname,m_charaList->GetName(n*2+1));
			SetWindowText(m_staticText,mes);

			while (LPSTR filename = m_myFindFile->SearchNext())
			{
				if ((kosuu % 10) == 0)
				{
					SendMessage(m_progressBar2,PBM_SETPOS,kosuu % 100,0);
				}

				kosuu++;

				if (strlen(filename) != 8)
				{
					static int aaa = 0;
					if (aaa == 0)
					{
						aaa = 1;
						MessageBox(NULL,"ファイル名が8byteになっていません",filename,MB_OK | MB_ICONEXCLAMATION);
					}
				}

				char vawname[1024];
				char wavename[1024];
				char oggname[1024];

				wsprintf(vawname,"cdvaw\\%s\\%s.vaw",charaname,filename);
				wsprintf(wavename,"cdwave\\%s.wav",filename);
				if (m_radioButtonStatus[4])
				{
					wsprintf(oggname,"cdwave\\%s.ogg",filename);
				}
				
				BOOL flg = TRUE;
	
				if (m_radioButtonStatus[0])
				{
					if (m_radioButtonStatus[4])
					{
						flg = m_checkDate->CheckNeedUpdate(oggname,vawname);
					}
					else
					{
						flg = m_checkDate->CheckNeedUpdate(wavename,vawname);
					}
				}

				if (flg)
				{
					int sz = 0;
					if (m_radioButtonStatus[4])
					{
						sz = m_load->LoadOgg(wavename,oggname,m_headerBuffer,m_buffer);
					}
					else
					{
						sz = m_load->LoadWave(wavename,m_headerBuffer,m_buffer);
					}

					if (sz > 0)
					{
						if (m_radioButtonStatus[4])
						{
							m_save->Save(vawname,m_headerBuffer,m_buffer,sz,2);
						}
						else
						{
							m_save->Save(vawname,m_headerBuffer,m_buffer,sz,0);
						}
					}
				}
			}

			m_myFindFile->SearchEnd();
		}
	}
}



void KatameRoutine(void)
{
	m_filenameBuffer = new char[8*65536];
	m_filenameTable = new int[65536];

	int ninzu = m_charaList->GetNameKosuu() / 2;
	if (ninzu < 2) return;

	for (int n=1;n<ninzu;n++)
	{
		int pos = (100*(n-1)+50)/(ninzu-1);
		SendMessage(m_progressBar,PBM_SETPOS,pos,0);

		LPSTR dirname = m_charaList->GetName(n*2);
		char searchname[1024];
		wsprintf(searchname,"cdvaw\\%s\\*.vaw",dirname);

		int kosuu = 0;
		if (m_myFindFile2->SearchStart(searchname))
		{
			char mes[256];
			wsprintf(mes,"音声 %s(%s)のソート中",dirname,m_charaList->GetName(n*2+1));
			SetWindowText(m_staticText,mes);
			UpdateWindow(m_staticText);

			while (LPSTR filename = m_myFindFile2->SearchNext())
			{
				if (kosuu < 65536)
				{
					int ln = strlen(filename);
					if (ln != 8)
					{
						static int aaa = 0;
						if (aaa == 0)
						{
							aaa = 1;
							MessageBox(NULL,"ファイル名が8byteになっていません",filename,MB_OK | MB_ICONEXCLAMATION);
						}
					}

					if (ln > 8)
					{
						ln = 8;
					}

					ZeroMemory(m_filenameBuffer+kosuu*8,8);
					memcpy(m_filenameBuffer+kosuu*8,filename,ln);

					//小文字にする
					for (int i=0;i<ln;i++)
					{
						char c = *(m_filenameBuffer + kosuu*8 + i);
						if ((c>='A') && (c<='Z'))
						{
							c += 0x20;
							*(m_filenameBuffer + kosuu*8+i) = c;
						}
						else
						{
							if (c & 0x80) i++;
						}

					}

					m_filenameTable[kosuu] = kosuu;
					kosuu++;
				}
				else
				{
					MessageBox(NULL,"1キャラあたりの音声の個数が65536を超えています",dirname,MB_OK | MB_ICONEXCLAMATION);
					break;
				}
			}
			m_myFindFile2->SearchEnd();
		}


		//sort
		if (kosuu >= 2)
		{
			qsort(m_filenameTable,kosuu,sizeof(int),MyCompare);
		}


		char vtbFilename[256];
		wsprintf(vtbFilename,"cdvaw\\%s.vtb",dirname);
		FILE* tableFile = NULL;
		fopen_s(&tableFile,vtbFilename,"wb");

//		FILE* tableFile = fopen(vtbFilename,"wb");

		char vpkFilename[256];
		wsprintf(vpkFilename,"cdvaw\\%s.vpk",dirname);

		FILE* packFile = NULL;
		fopen_s(&packFile,vpkFilename,"wb");

		//FILE* packFile = fopen(vpkFilename,"wb");

		//pack table
		//pack data

		int offsetData = 0;

		char mes[256];
		wsprintf(mes,"音声 %s(%s)を固めています",dirname,m_charaList->GetName(n*2+1));
		SetWindowText(m_staticText,mes);
		UpdateWindow(m_staticText);

		for (int i=0;i<kosuu;i++)
		{
			if ((i % 10) == 0)
			{
				SendMessage(m_progressBar2,PBM_SETPOS,i % 100,0);
			}

			int nm = m_filenameTable[i];
			char filenameonly[10];
			memcpy(filenameonly,m_filenameBuffer+nm*8,8);
			filenameonly[8] = 0;
			filenameonly[9] = 0;

			char filename[256];
			wsprintf(filename,"cdvaw\\%s\\%s.vaw",dirname,filenameonly);

			FILE* file = NULL;
			fopen_s(&file,filename,"rb");

			//FILE* file = fopen(filename,"rb");
			int sz = fread(m_buffer,sizeof(char),1024*1024*12,file);//max 12M
			fclose(file);

			fwrite(m_buffer,sizeof(char),sz,packFile);
			fwrite(m_filenameBuffer+nm*8,sizeof(char),8,tableFile);
			fwrite(&offsetData,sizeof(int),1,tableFile);
			offsetData += sz;
		}

		char nullData[8];
		ZeroMemory(nullData,8);
		fwrite(nullData,sizeof(char),8,tableFile);
		fwrite(&offsetData,sizeof(int),1,tableFile);

		fclose(packFile);
		fclose(tableFile);
	}


	DELETEARRAY(m_filenameTable);
	DELETEARRAY(m_filenameBuffer);
}


void PackSystemSound(void)
{
	char searchname[1024];
	if (m_radioButtonStatus[6])
	{
		wsprintf(searchname,"ogg\\sys\\*.ogg");
	}
	else
	{
		wsprintf(searchname,"wav\\sys\\*.wav");
	}



//	if (m_myFindFile->SearchStart("wav\\sys\\*.wav"))
	if (m_myFindFile->SearchStart(searchname))
	{
		int kosuu = 0;
		while (LPSTR filename = m_myFindFile->SearchNext())
		{
			SendMessage(m_progressBar2,PBM_SETPOS,(kosuu % 10)*10,0);
			kosuu++;

//			char src[1024];
//			char dst[1024];
//			wsprintf(src,"wav\\sys\\%s.wav",filename);
//			wsprintf(dst,"vaw\\sys\\%s.vaw",filename);


			char vawname[1024];
			char wavname[1024];
			char oggname[1024];

			wsprintf(vawname,"vaw\\sys\\%s.vaw",filename);
			
			if (m_radioButtonStatus[6])
			{
				wsprintf(wavname,"ogg\\sys\\%s.wav",filename);
			}
			else
			{
				wsprintf(wavname,"wav\\sys\\%s.wav",filename);
			}

			wsprintf(oggname,"ogg\\sys\\%s.ogg",filename);
			wsprintf(vawname,"vaw\\sys\\%s.vaw",filename);

			BOOL flg = TRUE;

			if (m_radioButtonStatus[0])
			{
				if (m_radioButtonStatus[6])
				{
					flg = m_checkDate->CheckNeedUpdate(oggname,vawname);
				}
				else
				{
					flg = m_checkDate->CheckNeedUpdate(wavname,vawname);
				}
			}

			if (flg)
			{
//				int sz = m_load->LoadWave(src,m_headerBuffer,m_buffer);

				int sz = 0;
				if (m_radioButtonStatus[6])
				{
					sz = m_load->LoadOgg(wavname,oggname,m_headerBuffer,m_buffer);
				}
				else
				{
					sz = m_load->LoadWave(wavname,m_headerBuffer,m_buffer);
				}



				if (sz > 0)
				{
					if (m_radioButtonStatus[6])
					{
						m_save->Save(vawname,m_headerBuffer,m_buffer,sz,2);
					}
					else
					{
						m_save->Save(vawname,m_headerBuffer,m_buffer,sz,0);
					}

//					m_save->Save(dst,m_headerBuffer,m_buffer,sz,0);
//					m_save->Save(vawname,m_headerBuffer,m_buffer,sz,0);
				}
			}
		}
		m_myFindFile->SearchEnd();
	}
}

void PackScriptSound(void)
{
	char searchname[1024];
	if (m_radioButtonStatus[6])
	{
		wsprintf(searchname,"ogg\\se\\*.ogg");
	}
	else
	{
		wsprintf(searchname,"wav\\se\\*.wav");
	}


//	if (m_myFindFile->SearchStart("wav\\se\\*.wav"))
	if (m_myFindFile->SearchStart(searchname))
	{
		int kosuu = 0;
		while (LPSTR filename = m_myFindFile->SearchNext())
		{
			SendMessage(m_progressBar2,PBM_SETPOS,(kosuu % 10)*10,0);
			kosuu++;

			///char src[1024];
			//char dst[1024];
			//wsprintf(src,"wav\\se\\%s.wav",filename);
			//wsprintf(dst,"vaw\\se\\%s.vaw",filename);
			

			char vawname[1024];
			char wavname[1024];
			char oggname[1024];

			wsprintf(vawname,"vaw\\se\\%s.vaw",filename);
			
			if (m_radioButtonStatus[6])
			{
				wsprintf(wavname,"ogg\\se\\%s.wav",filename);
			}
			else
			{
				wsprintf(wavname,"wav\\se\\%s.wav",filename);
			}

			wsprintf(oggname,"ogg\\se\\%s.ogg",filename);
			wsprintf(vawname,"vaw\\se\\%s.vaw",filename);



			BOOL flg = TRUE;

			if (m_radioButtonStatus[0])
			{
//				flg = m_checkDate->CheckNeedUpdate(src,dst);

				if (m_radioButtonStatus[6])
				{
					flg = m_checkDate->CheckNeedUpdate(oggname,vawname);
				}
				else
				{
					flg = m_checkDate->CheckNeedUpdate(wavname,vawname);
				}

			}

			if (flg)
			{
				int sz = 0;
				if (m_radioButtonStatus[6])
				{
					sz = m_load->LoadOgg(wavname,oggname,m_headerBuffer,m_buffer);
				}
				else
				{
					sz = m_load->LoadWave(wavname,m_headerBuffer,m_buffer);
				}

//				int sz = m_load->LoadWave(src,m_headerBuffer,m_buffer);
				if (sz > 0)
				{
					if (m_radioButtonStatus[6])
					{
						m_save->Save(vawname,m_headerBuffer,m_buffer,sz,2);
					}
					else
					{
						m_save->Save(vawname,m_headerBuffer,m_buffer,sz,0);
					}

//					m_save->Save(dst,m_headerBuffer,m_buffer,sz,0);
//					m_save->Save(vawname,m_headerBuffer,m_buffer,sz,0);
				}
			}
		}

		m_myFindFile->SearchEnd();
	}
}


void LoadCharaList(void)
{
	if (m_charaList == NULL)
	{
		m_charaList = new CNameList();
	}

	if (m_charaListLoadFlag == FALSE)
	{
		m_charaList->LoadFile("nnndir\\charaname.txt");
		m_charaListLoadFlag = TRUE;
	}
}


int MyCompare( const void* arg1, const void* arg2 )
{
	int* ptr1 = (int*)arg1;
	int* ptr2 = (int*)arg2;

	int n1 = *ptr1;
	int n2 = *ptr2;

//	int offset1 = m_nameTable[n1];
//	int offset2 = m_nameTable[n2];


	char* mes1 = (char*)(m_filenameBuffer+n1*8);
	char* mes2 = (char*)(m_filenameBuffer+n2*8);

	char check1[10];
	char check2[10];

	memcpy(check1,mes1,8);
	check1[8] = 0;
	check1[9] = 0;

	memcpy(check2,mes2,8);
	check2[8] = 0;
	check2[9] = 0;

//	return _stricmp(mes1,mes2);
	return _stricmp(check1,check2);
}

/*_*/



