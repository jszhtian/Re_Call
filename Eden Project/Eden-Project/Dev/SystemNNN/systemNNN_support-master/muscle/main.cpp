//
// Muscle.cpp
//


#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "main.h"


#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "..\..\systemNNN\nnnUtilLib\commonSystemfile.h"

#include "..\..\systemNNN\nnnUtilLib\namelist.h"


//#pragma comment(lib,"Comctl32.lib")

#if !defined _DEBUG
//#pragma comment(lib,"..\\nyanlib\\lib\\nyanLib2005.lib")
//#pragma comment(lib,"..\\nyanlib\\lib\\nnnUtilLib2005.lib")
#else
//#pragma comment(lib,"..\\nyanlib\\lib\\nyanLib2005D.lib")
//#pragma comment(lib,"..\\nyanlib\\lib\\nnnUtilLib2005D.lib")
#endif

//#define __TAIKEN_VERSION__


BOOL GetInitGameParam(int* lpVar, LPSTR name);
BOOL GetInitGameString(LPSTR* lpStr, LPSTR name);
BOOL LoadSetupFile(LPSTR filenameonly, int varKosuu = 64);


char* m_buffer = NULL;
int m_dataSize;

CNameList* m_setup = NULL;


int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType,  LPARAM lParam);
CCommonSystemFile::SYSTEMDATA* m_systemdata;
CCommonSystemFile::MESSAGEREADFLAG* m_messageReadFlag;
CCommonSystemFile::SYSTEMVAR* m_systemVar;
CCommonSystemFile::CGFLAG* m_cgFlag;
CCommonSystemFile::SCENEFLAG* m_sceneFlag;
CCommonSystemFile::FILMFLAG* m_filmFlag;
CCommonSystemFile::VOICEFLAG* m_voiceFlag;
CCommonSystemFile::MOVIEFLAG* m_movieFlag;
CCommonSystemFile::MUSICFLAG* m_musicFlag;


char m_normalSystemFileName[] = "sysfile";
char m_taikenSystemFileName[] = "tsysfile";
LPSTR m_systemFileName = NULL;
int m_taikenFlag = 0;


HWND m_cgCheck;
HWND m_sceneCheck;
HWND m_movieCheck;
HWND m_voiceCheck;
HWND m_messageCheck;
HWND m_musicCheck;

int m_cg = 0;
int m_scene = 0;
int m_message = 0;
int m_voice = 0;
int m_movie = 0;
int m_music = 0;

int m_editFlag = 0;


//void DataByLoad(void);
//void DataForSave(void);
void GetCheckButton(void);

void ChangeAllFlag(int rt);




HINSTANCE m_hInstance = NULL;


int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	HANDLE mx = NULL;
	m_hInstance = hInstance;

	LoadSetupFile("game",512);

	LPSTR mutexName = "nnnConfigMutexName";
	GetInitGameString(&mutexName,"mutexName");

//	mx = CreateMutex(NULL,TRUE,"BaraByTinyan");
	mx = CreateMutex(NULL,TRUE,mutexName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL,"二重起動及び、ゲーム動作中は起動できません","Error",MB_OK);
		ENDDELETECLASS(m_setup);
		return 1;
	}


	m_systemFileName = m_normalSystemFileName;
	GetInitGameParam(&m_taikenFlag,"taikenFlag");
	if (m_taikenFlag != 0)
	{
		m_systemFileName = m_taikenSystemFileName;
	}




	m_buffer = new char[65536*4];


	if (LoadSystemFile() == FALSE)
	{
		char mes[256];
		wsprintf(mes,"sav\\%s.savがみつかりませんでした",m_systemFileName);
		MessageBox(NULL,mes,"Error",MB_OK);
		if (mx != NULL)
		{
			ReleaseMutex( mx );
		}
		DELETEARRAY(m_buffer);		
//		DELETEARRAY(m_fontName);
		ENDDELETECLASS(m_setup);
		return 1;
	}


	//DataByLoad();


	int rt = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,DlgProc0,(LPARAM)NULL);
//	int rt = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG_NNNCONFIG),NULL,DlgProc,(LPARAM)NULL);

	if ((rt == 0) || (rt == 2))
	{
		//change
		ChangeAllFlag(rt);


		if (m_editFlag)
		{
			SaveSystemFile();
		}
	}

	if (mx != NULL)
	{
		ReleaseMutex( mx );
	}

	DELETEARRAY(m_buffer);		
	ENDDELETECLASS(m_setup);

	return 0;
}








BOOL LoadSystemFile(void)
{
	char filename[256];
	wsprintf(filename,"sav\\%s.sav",m_systemFileName);

	FILE* file = NULL;
	fopen_s(&file,filename,"rb");

	if (file == NULL)
	{
		wsprintf(filename,"sav\\%s.org",m_systemFileName);
		fopen_s(&file,filename,"rb");
	}

	if (file == NULL) return FALSE;

	m_dataSize = fread(m_buffer,sizeof(char),65536*4,file);
	fclose(file);

	char* ptr = m_buffer;
	ptr += sizeof(CCommonSystemFile::SYSTEMDATAINFO);

	m_systemdata = (CCommonSystemFile::SYSTEMDATA*)ptr;
	ptr += sizeof(CCommonSystemFile::SYSTEMDATA);

	m_messageReadFlag = (CCommonSystemFile::MESSAGEREADFLAG*)ptr;
	ptr += sizeof(CCommonSystemFile::MESSAGEREADFLAG);

	m_systemVar = (CCommonSystemFile::SYSTEMVAR*)ptr;
	ptr += sizeof(CCommonSystemFile::SYSTEMVAR);

	m_cgFlag = (CCommonSystemFile::CGFLAG*)ptr;
	ptr += sizeof(CCommonSystemFile::CGFLAG);

	m_sceneFlag = (CCommonSystemFile::SCENEFLAG*)ptr;
	ptr += sizeof(CCommonSystemFile::SCENEFLAG);

	m_filmFlag = (CCommonSystemFile::FILMFLAG*)ptr;
	ptr += sizeof(CCommonSystemFile::FILMFLAG);

	m_voiceFlag = (CCommonSystemFile::VOICEFLAG*)ptr;
	ptr += sizeof(CCommonSystemFile::VOICEFLAG);

	m_movieFlag = (CCommonSystemFile::MOVIEFLAG*)ptr;
	ptr += sizeof(CCommonSystemFile::MOVIEFLAG);

	m_musicFlag = (CCommonSystemFile::MUSICFLAG*)ptr;
	ptr += sizeof(CCommonSystemFile::MUSICFLAG);










//	DataByLoad();
	return TRUE;
}


BOOL SaveSystemFile(void)
{
	char filename[256];
	wsprintf(filename,"sav\\%s.sav",m_systemFileName);

	FILE* file = NULL;
	fopen_s(&file,filename,"wb");

	if (file == NULL) return FALSE;

	fwrite(m_buffer,sizeof(char),m_dataSize,file);
	fclose(file);

	return TRUE;
}


BOOL CALLBACK DlgProc0( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:


		m_cgCheck = GetDlgItem(hWnd,IDC_SETCG);
		m_sceneCheck = GetDlgItem(hWnd,IDC_SCENE);
		m_musicCheck = GetDlgItem(hWnd,IDC_MUSIC);
		m_voiceCheck = GetDlgItem(hWnd,IDC_VOICE);
		m_messageCheck = GetDlgItem(hWnd,IDC_READMESSAGE);
		m_movieCheck = GetDlgItem(hWnd,IDC_MOVIE);


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
		case IDC_ALLON:
			//get data

			GetCheckButton();
			EndDialog(hWnd,0);
			break;
		case IDC_ALLOFF:
			//get data

			GetCheckButton();
			EndDialog(hWnd,2);
			break;

		case IDCANCEL:
			EndDialog(hWnd,1);
			break;
		}
		break;
	}

	return FALSE;
}



void GetCheckButton(void)
{
	if (SendMessage(m_cgCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_cg = 1;
	if (SendMessage(m_sceneCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_scene = 1;
	if (SendMessage(m_movieCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_movie = 1;
	if (SendMessage(m_voiceCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_voice = 1;
	if (SendMessage(m_musicCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_music = 1;
	if (SendMessage(m_messageCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_message = 1;
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


BOOL LoadSetupFile(LPSTR filenameonly, int varKosuu)
{
	if (m_setup == NULL)
	{
		m_setup = new CNameList(varKosuu * 2);
	}

	if (m_setup == NULL) return FALSE;

	return m_setup->LoadInit(filenameonly);
}

void ChangeAllFlag(int rt)
{
	int dt = 0;
	int dt1 = 0;

	if (rt == 0)
	{
		dt = -1;
		dt1 = 1;
	}


	if (m_cg)
	{
		for (int i=0;i<128/32*20*4;i++)
		{
			m_cgFlag->data[i] = dt;
		}
		m_editFlag = 1;
	}

	if (m_scene)
	{
		for (int i=0;i<128*32;i++)
		{
			m_filmFlag->flag[i] = dt;
		}
		m_editFlag = 1;
	}

	if (m_message)
	{
		for (int i=0;i<4096*64/32;i++)
		{
			m_messageReadFlag->flag[i] = dt;
		}
		m_editFlag = 1;
	}

	if (m_movie)
	{
		for (int i=0;i<256;i++)
		{
			m_movieFlag->flag[i] = dt;
		}
		m_editFlag = 1;
	}

	if (m_music)
	{
		for (int i=0;i<256;i++)
		{
			m_musicFlag->flag[i] = dt1;
		}
		m_editFlag = 1;
	}

	if (m_voice)
	{
		for (int i=0;i<256;i++)
		{
			m_voiceFlag->flag[i] = dt1;
		}
		m_editFlag = 1;
	}
}

