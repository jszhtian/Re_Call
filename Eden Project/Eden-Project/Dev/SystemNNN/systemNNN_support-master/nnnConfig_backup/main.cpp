//
// Config.cpp
//




#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <commctrl.h>

#include "main.h"


#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"


#include "..\..\systemNNN\nnnUtilLib\commonSystemfile.h"

#include "..\..\systemNNN\nnnUtilLib\namelist.h"

#include "..\..\systemNNN\nnnUtilLib\mySaveFolder.h"

#include "..\..\systemNNN\nnnUtilLib\currentDirControl.h"

//#pragma comment(lib,"Comctl32.lib")

#if !defined _DEBUG
//#pragma comment(lib,"..\\nyanlib\\lib\\nyanLib2008.lib")
//#pragma comment(lib,"..\\nyanlib\\lib\\nnnUtilLib2008.lib")
#else
//#pragma comment(lib,"..\\nyanlib\\lib\\nyanLib2008D.lib")
//#pragma comment(lib,"..\\nyanlib\\lib\\nnnUtilLib2008D.lib")
#endif

//#define __TAIKEN_VERSION__


BOOL GetInitGameParam(int* lpVar, LPSTR name);
BOOL GetInitGameString(LPSTR* lpStr, LPSTR name);
BOOL LoadSetupFile(LPSTR filenameonly, int varKosuu = 64);

BOOL GetInitGameParam3(int* lpVar, LPSTR name);
BOOL GetInitGameString3(LPSTR* lpStr, LPSTR name);
BOOL LoadSetupFile3(LPSTR filenameonly, int varKosuu = 64);

//BOOL GetInitGameParam2(int* lpVar, LPSTR name);
//BOOL GetInitGameString2(LPSTR* lpStr, LPSTR name);
//BOOL LoadSetupFile2(LPSTR filenameonly, int varKosuu = 64);

void SetWindowModeButton(void);
void SetSkipModeButton(void);

char* m_buffer = NULL;
int m_dataSize;

CNameList* m_setup = NULL;
//CNameList* m_setup2 = NULL;
CNameList* m_setup3 = NULL;

int m_fontKosuu = 0;
int m_folderType = 0;
int m_notFound = 0;

//char* m_fontName = NULL;

int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType,  LPARAM lParam);
CCommonSystemFile::SYSTEMDATA* m_systemdata;


/*
#if !defined __TAIKEN_VERSION__
#define SYSFILE_FILENAME "sysfile.sav"
#define SYSFILEORG_FILENAME "sysfile.org"
#else
#define SYSFILE_FILENAME "tsysfile.sav"
#define SYSFILEORG_FILENAME "tsysfile.org"
#endif
*/


char m_normalSystemFileName[] = "sysfile";
char m_taikenSystemFileName[] = "tsysfile";
LPSTR m_systemFileName = NULL;
int m_taikenFlag = 0;


//char m_defaultMidiCaption[] = "MIDI";
//char m_defaultCddaCaption[] = "CD-DA";
//char m_defaultCddaVolumeCaption[] = "音楽(CDDA)";


//int m_midiButtonFlag = 1;
//int m_cddaButtonFlag = 1;

//LPSTR m_midiCaption = NULL;
//LPSTR m_cddaCaption = NULL;
//LPSTR m_cddaVolumeCaption = NULL;


int m_windowButtonFlag = 1;
int m_fullScreenButtonFlag = 1;

int m_2d3dSelect = 0;	//0:auto 2:2DOnly 3:3DOnly


//int m_normalMusicButtonFlag = 1;
//int m_directSoundButtonFlag = 1;



HWND m_windowModeRadio;
HWND m_fullModeRadio;

HWND m_skipReadRadio;
HWND m_skipAllRadio;

//HWND m_normalSoundRadio;
//HWND m_directSoundRadio;

//HWND m_bgmMidiRadio;
//HWND m_bgmCddaRadio;

HWND m_msGothicRadio;
HWND m_msMinchoRadio;

HWND m_useUserFontButton;
HWND m_fontNameList;

HWND m_noSkipFrameButton;
HWND m_useVMRButton;

HWND m_totalVolumeSlider;
HWND m_musicVolumeSlider;
HWND m_soundVolumeSlider;
HWND m_voiceVolumeSlider;
HWND m_movieVolumeSlider;
HWND m_soundVoiceVolumeSlider;
HWND m_messageSpeedSlider;

HWND m_totalCheck;
HWND m_musicCheck;
HWND m_soundCheck;
HWND m_voiceCheck;
HWND m_movieCheck;
HWND m_soundVoiceCheck;


HWND m_nnnTab;
HWND m_dialog1;
HWND m_dialog2;
HWND m_dialog3;
HWND m_dialog4;
HWND m_dialog5;
HWND m_dialog6;

HWND m_directDrawAutoRadio;
HWND m_directDrawNotUseRadio;
HWND m_2d3dAutoRadio;
HWND m_2dRadio;
HWND m_3dRadio;
HWND m_vshaderAutoRadio;
HWND m_vshaderHardRadio;
HWND m_vshaderSoftRadio;
//HWND m_shader_3_0_Radio;

HWND m_needautoShaderRadio;
HWND m_needshader_1_1_Radio;
HWND m_needshader_2_0_Radio;
HWND m_needshader_3_0_Radio;

HWND m_lowTextureButton;

int	m_screenMode = 0;
int	m_skipMode = 0;

int	m_fontMode = 0;
int	m_userFontFlag = 0;
char m_userFontName[256];
int	m_skipFrameFlag = 0;
int m_useVMRFlag = 0;

int m_2d3d = 0;
int m_shader = 0;
int m_needshader = 0;

int	m_musicVolume = 90;
int	m_soundVolume = 90;
int	m_voiceVolume = 90;
int m_movieVolume = 90;
int m_soundVoiceVolume = 90;
int m_totalVolume = 90;
int	m_messageSpeed = 2;

int m_musicSwitch = 1;
int m_soundSwitch = 1;
int m_voiceSwitch = 1;
int m_movieSwitch = 1;
int m_soundVoiceSwitch = 1;
int m_totalVolumeSwitch = 1;

int m_movieVolumeNotUse = 0;
int m_soundVoiceVolumeNotUse = 1;

int m_lowTextureFlag = 0;
int m_totalVolumeUseFlag = 0;
int m_totalVolumePrintFlag = 1;

int m_screenStrecthFlag = 0;
int m_screenSizeType = 0;
int m_screenSizeTypeMax = 0;

int m_directDrawNotUse;

void MaskFont(void);

void SetScreenModeButton(void);
void SetSkipModeButton(void);

void SetDefaultFontButton(void);
void SetUserFontButton(void);
void SetSelectUserFontList(void);
void SetSkipFrameButton(void);
void SetVMRButton(void);

void Set2D3DButton(void);
void SetShaderButton(void);
void SetDirectDrawButton(void);

void SetVolumeSlider(void);
void SetMessageSpeedSlider(void);
void SetVolumeSwitch(void);

void SetLowTextureButton(void);

void SetExpCheckButton(void);
void SetExpRadioButton(void);
void SetScreenSizeButton(void);



void DataByLoad(void);
void DataForSave(void);


void GetScreenModeButton(void);
void GetSkipModeButton(void);

void GetDefaultFontButton(void);
void GetUserFontButton(void);
void GetSelectUserFontList(void);
void GetSkipFrameButton(void);
void GetVMRButton(void);

void Get2d3dButton(void);
void GetShaderButton(void);
void GetDirectDrawButton(void);

void GetVolumeSlider(void);
void GetMessageSpeedSlider(void);
void GetVolumeSwitch(void);

void GetLowTextureButton(void);

void GetExpCheckButton(void);
void GetExpRadioButton(void);
void GetScreenSizeButton(void);


int AdjustVolumeByLoad(int vol);
int AdjustVolumeForSave(int vol);

void ReplaceName(LPSTR name,LPSTR replaceChara);

HINSTANCE m_hInstance = NULL;

int m_useExpCheck = 0;
int m_useExpRadio = 0;

int m_gameTabNumber = -1;
int m_systemTabNumber = -1;
int m_expCheckTabNumber = -1;
int m_expRadioTabNumber = -1;
int m_expRadioTabNumber2 = -1;
int m_screenSizeTabNumber = -1;

int m_codeByte = 2;

int m_volumeMin = 0;
int m_volumeMax = 100;

int m_expCheck[16];
int m_expRadio[16];

HWND m_expCheckButton[16];
HWND m_expRadioButton[16][8];
int m_expRadioButtonMax[16] = {2,2,2,2,2,2,2,2,4,4,4,4,8,8,16,0};
int m_expRadioButtonUse[16] = {2,2,2,2,2,2,2,2,4,4,4,4,8,8,16,0};



int m_expCheckID[16]=
{
	IDC_EXPCHECK1,IDC_EXPCHECK2,IDC_EXPCHECK3,IDC_EXPCHECK4,
	IDC_EXPCHECK5,IDC_EXPCHECK6,IDC_EXPCHECK7,IDC_EXPCHECK8,
	IDC_EXPCHECK9,IDC_EXPCHECK10,IDC_EXPCHECK11,IDC_EXPCHECK12,
	IDC_EXPCHECK13,IDC_EXPCHECK14,IDC_EXPCHECK15,IDC_EXPCHECK16,
};

int m_expRadioCheckID[16][16]=
{
	{IDC_EXPRADIO1_1,IDC_EXPRADIO1_2},
	{IDC_EXPRADIO2_1,IDC_EXPRADIO2_2},
	{IDC_EXPRADIO3_1,IDC_EXPRADIO3_2},
	{IDC_EXPRADIO4_1,IDC_EXPRADIO4_2},
	{IDC_EXPRADIO5_1,IDC_EXPRADIO5_2},
	{IDC_EXPRADIO6_1,IDC_EXPRADIO6_2},
	{IDC_EXPRADIO7_1,IDC_EXPRADIO7_2},
	{IDC_EXPRADIO8_1,IDC_EXPRADIO8_2},
	
	{IDC_EXPRADIO9_1,IDC_EXPRADIO9_2,IDC_EXPRADIO9_3,IDC_EXPRADIO9_4},
	{IDC_EXPRADIO10_1,IDC_EXPRADIO10_2,IDC_EXPRADIO10_3,IDC_EXPRADIO10_4},
	{IDC_EXPRADIO11_1,IDC_EXPRADIO11_2,IDC_EXPRADIO11_3,IDC_EXPRADIO11_4},
	{IDC_EXPRADIO12_1,IDC_EXPRADIO12_2,IDC_EXPRADIO12_3,IDC_EXPRADIO12_4},


	{IDC_EXPRADIO13_1,IDC_EXPRADIO13_2,IDC_EXPRADIO13_3,IDC_EXPRADIO13_4,
	 IDC_EXPRADIO13_5,IDC_EXPRADIO13_6,IDC_EXPRADIO13_7,IDC_EXPRADIO13_8},
	{IDC_EXPRADIO14_1,IDC_EXPRADIO14_2,IDC_EXPRADIO14_3,IDC_EXPRADIO14_4,
	 IDC_EXPRADIO14_5,IDC_EXPRADIO14_6,IDC_EXPRADIO14_7,IDC_EXPRADIO14_8},

	{IDC_EXPRADIO15_1,IDC_EXPRADIO15_2,IDC_EXPRADIO15_3,IDC_EXPRADIO15_4,
	 IDC_EXPRADIO15_5,IDC_EXPRADIO15_6,IDC_EXPRADIO15_7,IDC_EXPRADIO15_8,
	 IDC_EXPRADIO15_9,IDC_EXPRADIO15_10,IDC_EXPRADIO15_11,IDC_EXPRADIO15_12,
	 IDC_EXPRADIO15_13,IDC_EXPRADIO15_14,IDC_EXPRADIO15_15,IDC_EXPRADIO15_16},
};



int m_expRadioCaptionID[16]=
{
	IDC_STATIC_RADIO1,IDC_STATIC_RADIO2,IDC_STATIC_RADIO3,IDC_STATIC_RADIO4,
	IDC_STATIC_RADIO5,IDC_STATIC_RADIO6,IDC_STATIC_RADIO7,IDC_STATIC_RADIO8,
	IDC_STATIC_RADIO9,IDC_STATIC_RADIO10,IDC_STATIC_RADIO11,IDC_STATIC_RADIO12,
	IDC_STATIC_RADIO13,IDC_STATIC_RADIO14,IDC_STATIC_RADIO15,0
};

int m_screenSizeID[16] = 
{
	IDC_RADIO_SCREENSIZE1,IDC_RADIO_SCREENSIZE2,IDC_RADIO_SCREENSIZE3,IDC_RADIO_SCREENSIZE4,
	IDC_RADIO_SCREENSIZE5,IDC_RADIO_SCREENSIZE6,IDC_RADIO_SCREENSIZE7,IDC_RADIO_SCREENSIZE8,
	IDC_RADIO_SCREENSIZE9,IDC_RADIO_SCREENSIZE10,IDC_RADIO_SCREENSIZE11,IDC_RADIO_SCREENSIZE12,
	IDC_RADIO_SCREENSIZE13,IDC_RADIO_SCREENSIZE14,IDC_RADIO_SCREENSIZE15,IDC_RADIO_SCREENSIZE16
};

HWND m_screenStretchButton = NULL;
HWND m_screenSizeButton[16];

BOOL m_screenSizeEnable[16] = 
{
	FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE
};

SIZE m_screenSizeTable[16]=
{
	{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
};


int SearchExpCheckID(int wParam);

int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{

	HANDLE mx = NULL;
	m_hInstance = hInstance;

	CCurrentDirControl::ChangeCurrentToExe();
/*
	int b = 0;
	b++;
	if (b == 5)
	{
		FILE* file = NULL;
		if (fopen_s(&file,"aaa","rb") == 0)
		{

			fclose(file);
		}
	}
*/

	LoadSetupFile("game",512);
//	LoadSetupFile2("mainControl",512);
	LoadSetupFile3("config",512);

	int volumeAdjustFlag = 0;
	GetInitGameParam3(&volumeAdjustFlag,"configExeVolumeAdjustFlag");
	if (volumeAdjustFlag)
	{
		GetInitGameParam3(&m_volumeMin,"volumeMin");
		GetInitGameParam3(&m_volumeMax,"volumeMax");
	}

	LPSTR mutexName = "nnnConfigMutexName";
	GetInitGameString(&mutexName,"mutexName");

	GetInitGameParam(&m_codeByte,"codeByte");

//	mx = CreateMutex(NULL,TRUE,"BaraByTinyan");
	mx = CreateMutex(NULL,TRUE,mutexName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if (m_codeByte != 1)
		{
			MessageBox(NULL,"二重起動及び、ゲーム動作中は起動できません","Error",MB_OK);
		}
		else
		{
			MessageBox(NULL,"Cannot:DoubleStart,PlayingStart","Error",MB_OK);
		}
//		ENDDELETECLASS(m_setup2);
		ENDDELETECLASS(m_setup);

		CCurrentDirControl::RestoreCurrent();

		return 1;
	}


	int folderType = 0;
	GetInitGameParam(&folderType,"saveFolderType");
	LPSTR saveCompanyFolder = NULL;
	GetInitGameString(&saveCompanyFolder,"saveCompanyFolder");
	LPSTR saveGameFolder = NULL;
	GetInitGameString(&saveGameFolder,"saveGameFolder");
	CMySaveFolder::ChangeMyFolder(saveCompanyFolder,saveGameFolder,folderType);

	m_folderType = folderType;


	GetInitGameParam(&m_useExpCheck,"useExpCheckConfig");
	GetInitGameParam(&m_useExpRadio,"useExpRadioConfig");

	GetInitGameParam(&m_totalVolumeUseFlag,"totalVolumeUseFlag");
	GetInitGameParam(&m_totalVolumePrintFlag,"totalVolumePrintFlag");

	m_systemFileName = m_normalSystemFileName;
	GetInitGameParam(&m_taikenFlag,"taikenFlag");
	if (m_taikenFlag != 0)
	{
		m_systemFileName = m_taikenSystemFileName;
	}




	m_buffer = new char[65536*4];


	if (LoadSystemFile() == FALSE)
	{
		LPSTR saveFolder = CMySaveFolder::GetFullFolder();

		char mes[1024];
		if (m_codeByte != 1)
		{
			wsprintf(mes,"%s\\%s.savがみつかりませんでした",saveFolder,m_systemFileName);
		}
		else
		{
			wsprintf(mes,"not found : %s\\%s.sav",saveFolder,m_systemFileName);
		}

		MessageBox(NULL,mes,"Error",MB_OK);
		if (mx != NULL)
		{
			ReleaseMutex( mx );
		}
		DELETEARRAY(m_buffer);		
//		DELETEARRAY(m_fontName);
//		ENDDELETECLASS(m_setup2);
		ENDDELETECLASS(m_setup);

		CCurrentDirControl::RestoreCurrent();

		return 1;
	}


	GetInitGameParam(&m_windowButtonFlag,"configWindowButtonFlag");
	GetInitGameParam(&m_fullScreenButtonFlag,"configFullScreenButtonFlag");

	GetInitGameParam(&m_movieVolumeNotUse,"movieVolumeNotUse");
	GetInitGameParam(&m_soundVoiceVolumeNotUse,"soundVoiceVolumeNotUse");


	GetInitGameParam(&m_2d3dSelect,"select2D3D");


	DataByLoad();


	int rt = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,DlgProc0,(LPARAM)NULL);
//	int rt = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG_NNNCONFIG),NULL,DlgProc,(LPARAM)NULL);

	if (rt == 0)
	{
		DataForSave();
		SaveSystemFile();
	}

	if (mx != NULL)
	{
		ReleaseMutex( mx );
	}

	DELETEARRAY(m_buffer);		
	ENDDELETECLASS(m_setup3);
//	ENDDELETECLASS(m_setup2);
	ENDDELETECLASS(m_setup);

	CCurrentDirControl::RestoreCurrent();

	return 0;
}








BOOL LoadSystemFile(void)
{
	LPSTR saveFolder = CMySaveFolder::GetFullFolder();

	char filename[256];
	wsprintf(filename,"%s\\%s.sav",saveFolder,m_systemFileName);

	FILE* file = NULL;
	fopen_s(&file,filename,"rb");

	if (file == NULL)
	{
		m_notFound = 1;
		wsprintf(filename,"sav\\%s.org",m_systemFileName);
		fopen_s(&file,filename,"rb");
	}

	if (file == NULL) return FALSE;

	m_dataSize = fread(m_buffer,sizeof(char),65536*4,file);
	fclose(file);

	char* ptr = m_buffer;
	ptr += sizeof(CCommonSystemFile::SYSTEMDATAINFO);
	m_systemdata = (CCommonSystemFile::SYSTEMDATA*)ptr;

	DataByLoad();
	return TRUE;
}


BOOL SaveSystemFile(void)
{
	if ((m_notFound == 1) && (m_folderType != 0))
	{
		CMySaveFolder::CreateSubFolder();
	}

	LPSTR saveFolder = CMySaveFolder::GetFullFolder();

	char filename[256];
	wsprintf(filename,"%s\\%s.sav",saveFolder,m_systemFileName);

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

		m_nnnTab = GetDlgItem(hWnd,IDC_NNNTAB);



		TC_ITEM tcItem[6];
		tcItem[0].mask = TCIF_TEXT;
		tcItem[0].pszText = "ゲーム設定";
		if (m_codeByte == 1)
		{
			tcItem[0].pszText = "GameSetup";
		}
		m_gameTabNumber = TabCtrl_InsertItem(m_nnnTab,0,&tcItem[0]);

		tcItem[1].mask = TCIF_TEXT;
		tcItem[1].pszText = "システム設定";
		if (m_codeByte == 1)
		{
			tcItem[1].pszText = "SystemSetup";	
		}
		m_systemTabNumber = TabCtrl_InsertItem(m_nnnTab,1,&tcItem[1]);


		m_expCheckTabNumber = -1;

		if (m_useExpCheck > 0)
		{

			tcItem[2].mask = TCIF_TEXT;
			tcItem[2].pszText = "拡張チェック設定";

			if (m_codeByte == 1)
			{
				tcItem[2].pszText = "ExCheckSetup";	
			}

			m_expCheckTabNumber = TabCtrl_InsertItem(m_nnnTab,2,&tcItem[2]);
		}

		m_expRadioTabNumber = -1;

		if (m_useExpRadio > 0)
		{
			int noCreate = 0;
			GetInitGameParam(&noCreate,"disableExpRadioTab1");
			if (noCreate == 0)
			{
				tcItem[3].mask = TCIF_TEXT;
				tcItem[3].pszText = "拡張ラジオ設定";
				if (m_codeByte == 1)
				{
					tcItem[3].pszText = "ExRadioSetup";	
				}

				m_expRadioTabNumber = TabCtrl_InsertItem(m_nnnTab,3,&tcItem[3]);
			}
		}

		m_expRadioTabNumber2 = -1;
		if (m_useExpRadio > 12)
		{
			tcItem[4].mask = TCIF_TEXT;
			tcItem[4].pszText = "拡張ラジオ設定2";
			if (m_codeByte == 1)
			{
				tcItem[4].pszText = "ExRadioSetup2";	
			}

			m_expRadioTabNumber2 = TabCtrl_InsertItem(m_nnnTab,4,&tcItem[4]);
		}

		GetInitGameParam(&m_screenSizeTypeMax,"screenSizeTypeNumber");

		m_screenSizeTabNumber = -1;
		if (m_screenSizeTypeMax > 1)
		{
			tcItem[5].mask = TCIF_TEXT;
			tcItem[5].pszText = "スクリーンサイズ設定";
			if (m_codeByte == 1)
			{
				tcItem[5].pszText = "screenSizeSetup";	
			}

			m_screenSizeTabNumber = TabCtrl_InsertItem(m_nnnTab,5,&tcItem[5]);
		}


		HWND child;
		HWND child2;
		HWND child3;
		HWND child4;
		HWND child5;
		HWND child6;

		if (m_codeByte == 1)
		{
			SetWindowText(GetDlgItem(hWnd,IDCANCEL),"Cancel");
		}

		child = CreateDialog(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_NNNCONFIG),hWnd,DlgProc1);
		child2 = CreateDialog(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_NNNCONFIG2),hWnd,DlgProc2);
		child3 = CreateDialog(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_NNNCONFIG3),hWnd,DlgProc3);
		child4 = CreateDialog(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_NNNCONFIG4),hWnd,DlgProc4);
		child5 = CreateDialog(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_NNNCONFIG5),hWnd,DlgProc5);
		child6 = CreateDialog(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_NNNCONFIG6),hWnd,DlgProc6);


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

		ShowWindow(child,SW_SHOW);
		ShowWindow(child2,SW_HIDE);
		ShowWindow(child3,SW_HIDE);
		ShowWindow(child4,SW_HIDE);
		ShowWindow(child5,SW_HIDE);
		ShowWindow(child6,SW_HIDE);

		return 1;
		break;


	case WM_CLOSE:
		EndDialog(hWnd,1);
		break;


	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			//get data

			GetScreenModeButton();
			GetSkipModeButton();
			GetDefaultFontButton();
			GetUserFontButton();
			GetSelectUserFontList();
			GetSkipFrameButton();
			GetVMRButton();
			Get2d3dButton();
			GetDirectDrawButton();
			GetShaderButton();

			GetVolumeSlider();
			GetMessageSpeedSlider();
			GetVolumeSwitch();

			GetLowTextureButton();
			GetExpCheckButton();
			GetExpRadioButton();
			GetScreenSizeButton();

			EndDialog(hWnd,0);
			break;
		case IDCANCEL:
			EndDialog(hWnd,1);
			break;
		}
		break;

	case WM_NOTIFY:
		if (((NMHDR*)lParam)->code == TCN_SELCHANGE)
		{
			int cm = TabCtrl_GetCurSel(m_nnnTab);
			if (cm == m_gameTabNumber)
			{
				ShowWindow(m_dialog2,SW_HIDE);
				ShowWindow(m_dialog3,SW_HIDE);
				ShowWindow(m_dialog4,SW_HIDE);
				ShowWindow(m_dialog5,SW_HIDE);
				ShowWindow(m_dialog6,SW_HIDE);
				ShowWindow(m_dialog1,SW_SHOW);
			}
			else if (cm == m_systemTabNumber)
			{
				ShowWindow(m_dialog1,SW_HIDE);
				ShowWindow(m_dialog3,SW_HIDE);
				ShowWindow(m_dialog4,SW_HIDE);
				ShowWindow(m_dialog5,SW_HIDE);
				ShowWindow(m_dialog6,SW_HIDE);
				ShowWindow(m_dialog2,SW_SHOW);
			}
			else if (cm == m_expCheckTabNumber )
			{
				ShowWindow(m_dialog1,SW_HIDE);
				ShowWindow(m_dialog2,SW_HIDE);
				ShowWindow(m_dialog4,SW_HIDE);
				ShowWindow(m_dialog5,SW_HIDE);
				ShowWindow(m_dialog6,SW_HIDE);
				ShowWindow(m_dialog3,SW_SHOW);
			}
			else if (cm == m_expRadioTabNumber )
			{
				ShowWindow(m_dialog1,SW_HIDE);
				ShowWindow(m_dialog2,SW_HIDE);
				ShowWindow(m_dialog3,SW_HIDE);
				ShowWindow(m_dialog5,SW_HIDE);
				ShowWindow(m_dialog6,SW_HIDE);
				ShowWindow(m_dialog4,SW_SHOW);
			}
			else if (cm == m_expRadioTabNumber2 )
			{
				ShowWindow(m_dialog1,SW_HIDE);
				ShowWindow(m_dialog2,SW_HIDE);
				ShowWindow(m_dialog3,SW_HIDE);
				ShowWindow(m_dialog4,SW_HIDE);
				ShowWindow(m_dialog6,SW_HIDE);
				ShowWindow(m_dialog5,SW_SHOW);
			}
			else if (cm == m_screenSizeTabNumber )
			{
				ShowWindow(m_dialog1,SW_HIDE);
				ShowWindow(m_dialog2,SW_HIDE);
				ShowWindow(m_dialog3,SW_HIDE);
				ShowWindow(m_dialog4,SW_HIDE);
				ShowWindow(m_dialog5,SW_HIDE);
				ShowWindow(m_dialog6,SW_SHOW);
			}

		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK DlgProc1( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		m_dialog1 = hWnd;

		m_windowModeRadio = GetDlgItem(hWnd,IDC_RADIO_WINDOW);
		m_fullModeRadio = GetDlgItem(hWnd,IDC_RADIO_FULL);

		m_skipReadRadio = GetDlgItem(hWnd,IDC_RADIO_SKIPREAD);
		m_skipAllRadio = GetDlgItem(hWnd,IDC_RADIO_SKIPALL);

//		m_msGothicRadio = GetDlgItem(hWnd,IDC_RADIO_MSGOTHIC);
//		m_msMinchoRadio = GetDlgItem(hWnd,IDC_RADIO_MSMINCHO);

//		m_useUserFontButton = GetDlgItem(hWnd,IDC_CHECK_USERFONT);
//		m_fontNameList = GetDlgItem(hWnd,IDC_COMBO_FONTNAME);

//		m_noSkipFrameButton = GetDlgItem(hWnd,IDC_CHECK_NOSKIPFRAME);
//		m_useVMRButton = GetDlgItem(hWnd,IDC_CHECK_VMR);

		m_musicVolumeSlider = GetDlgItem(hWnd,IDC_SLIDER_MUSICVOLUME);
		m_soundVolumeSlider = GetDlgItem(hWnd,IDC_SLIDER_SOUNDVOLUME);
		m_voiceVolumeSlider = GetDlgItem(hWnd,IDC_SLIDER_VOICEVOLUME);
		m_movieVolumeSlider = GetDlgItem(hWnd,IDC_SLIDER_MOVIEVOLUME);
		m_soundVoiceVolumeSlider = GetDlgItem(hWnd,IDC_SLIDER_SOUNDVOICEVOLUME);
		m_totalVolumeSlider = GetDlgItem(hWnd,IDC_SLIDER_TOTALVOLUME);

		m_messageSpeedSlider = GetDlgItem(hWnd,IDC_SLIDER_MESSAGESPEED);

		m_musicCheck = GetDlgItem(hWnd,IDC_CHECK_MUSICSWITCH);
		m_soundCheck = GetDlgItem(hWnd,IDC_CHECK_SOUNDSWITCH);
		m_voiceCheck = GetDlgItem(hWnd,IDC_CHECK_VOICESWITCH);
		m_movieCheck = GetDlgItem(hWnd,IDC_CHECK_MOVIESWITCH);
		m_soundVoiceCheck = GetDlgItem(hWnd,IDC_CHECK_SOUNDVOICESWITCH);
		m_totalCheck = GetDlgItem(hWnd,IDC_CHECK_TOTALSWITCH);

		if (m_windowButtonFlag == 0) EnableWindow(m_windowModeRadio,FALSE);
		if (m_fullScreenButtonFlag == 0) EnableWindow(m_fullModeRadio,FALSE);


		if (m_movieVolumeNotUse)
		{
			EnableWindow(m_movieCheck,FALSE);
			EnableWindow(m_movieVolumeSlider,FALSE);
		}

		if (m_soundVoiceVolumeNotUse)
		{
			EnableWindow(m_soundVoiceCheck,FALSE);
			EnableWindow(m_soundVoiceVolumeSlider,FALSE);
		}

		if (m_totalVolumePrintFlag == 0)
		{
			ShowWindow(m_totalCheck,SW_HIDE);
			ShowWindow(m_totalVolumeSlider,SW_HIDE);
		}


//		HDC hdc;
//		hdc = GetDC(hWnd);
		
//		LOGFONT lf;
//		ZeroMemory(&lf,sizeof(lf));
//		lf.lfCharSet = SHIFTJIS_CHARSET;
//		lf.lfFaceName[0] = 0;
//		lf.lfPitchAndFamily = 0;
		
//		EnumFontFamiliesEx(hdc,&lf,(FONTENUMPROC)EnumFontFamExProc,(LPARAM)0,0);
//
//		ReleaseDC(hWnd,hdc);



		SetScreenModeButton();
		SetSkipModeButton();
//		SetDefaultFontButton();
//		SetUserFontButton();
//		SetSelectUserFontList();
//		SetSkipFrameButton();
//		SetVMRButton();

		SendMessage(m_musicVolumeSlider,TBM_SETRANGE,(WPARAM)FALSE,(LPARAM)MAKELONG(0,100));
		SendMessage(m_soundVolumeSlider,TBM_SETRANGE,(WPARAM)FALSE,(LPARAM)MAKELONG(0,100));
		SendMessage(m_voiceVolumeSlider,TBM_SETRANGE,(WPARAM)FALSE,(LPARAM)MAKELONG(0,100));
		SendMessage(m_movieVolumeSlider,TBM_SETRANGE,(WPARAM)FALSE,(LPARAM)MAKELONG(0,100));
		SendMessage(m_soundVoiceVolumeSlider,TBM_SETRANGE,(WPARAM)FALSE,(LPARAM)MAKELONG(0,100));
		SendMessage(m_totalVolumeSlider,TBM_SETRANGE,(WPARAM)FALSE,(LPARAM)MAKELONG(0,100));

		SendMessage(m_messageSpeedSlider,TBM_SETRANGE,(WPARAM)FALSE,(LPARAM)MAKELONG(0,4));

		if (m_codeByte == 1)
		{
			//patch for ranba

			SetWindowText(GetDlgItem(hWnd, IDC_STATIC_SCREENMODE), "Display Mode");
			SetWindowText(m_windowModeRadio, "Windowed");
			SetWindowText(m_fullModeRadio, "Fullscreen");

			SetWindowText(GetDlgItem(hWnd, IDC_STATIC_SKIP), "Skip Mode");
			SetWindowText(m_skipAllRadio, "Skip All");
			SetWindowText(m_skipReadRadio, "Skip Read Only");

			SetWindowText(m_musicCheck, "Music");
			SetWindowText(m_soundCheck, "Sounds");
			SetWindowText(m_voiceCheck, "Voices");
			SetWindowText(m_movieCheck, "Movies");
			SetWindowText(m_soundVoiceCheck, "Voice SE");
			SetWindowText(m_totalCheck, "Master");

			SetWindowText(GetDlgItem(hWnd, IDC_STATIC_MESSAGESPEED), "Text Speed");

			/*
			SetWindowText(GetDlgItem(hWnd,IDC_STATIC_SCREENMODE),"Screen");
			SetWindowText(m_windowModeRadio,"Window");
			SetWindowText(m_fullModeRadio,"Full");

			SetWindowText(GetDlgItem(hWnd,IDC_STATIC_SKIP),"Skip");
			SetWindowText(m_skipAllRadio,"All");
			SetWindowText(m_skipReadRadio,"Read");

			SetWindowText(m_musicCheck,"Music");
			SetWindowText(m_soundCheck,"Sound");
			SetWindowText(m_voiceCheck,"Voice");
			SetWindowText(m_movieCheck,"Movie");
			SetWindowText(m_soundVoiceCheck,"SoundVoice");
			SetWindowText(m_totalCheck,"Total");

			SetWindowText(GetDlgItem(hWnd,IDC_STATIC_MESSAGESPEED),"MessageSpeed");
			*/
		}

		SetVolumeSlider();
		SetMessageSpeedSlider();
		SetVolumeSwitch();

//		MaskFont();

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


		MoveWindow(hWnd,32,32,sx,sy,TRUE);

		return 1;
		break;

/*
	case WM_CLOSE:
		EndDialog(hWnd,1);
		break;

	case WM_SHOWWINDOW:
		OutputDebugString("show");
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_CHECK_USERFONT:
			int old2;
			old2 = m_userFontFlag;
			GetUserFontButton();
			if (m_userFontFlag != old2)
			{
				MaskFont();
				UpdateWindow(hWnd);
			}

			break;

		case IDOK:
			//get data

			GetScreenModeButton();
			GetSkipModeButton();
			GetDefaultFontButton();
			GetUserFontButton();
			GetSelectUserFontList();
			GetSkipFrameButton();
			GetVMRButton();

			GetVolumeSlider();
			GetMessageSpeedSlider();
			GetVolumeSwitch();

			EndDialog(hWnd,0);
			break;
		case IDCANCEL:
			EndDialog(hWnd,1);
			break;
		}
*/

		break;
	}

	return FALSE;
}

BOOL CALLBACK DlgProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		m_dialog2 = hWnd;

		m_noSkipFrameButton = GetDlgItem(hWnd,IDC_CHECK_NOSKIPFRAME);
		m_useVMRButton = GetDlgItem(hWnd,IDC_CHECK_VMR);

		m_msGothicRadio = GetDlgItem(hWnd,IDC_RADIO_MSGOTHIC);
		m_msMinchoRadio = GetDlgItem(hWnd,IDC_RADIO_MSMINCHO);

		m_useUserFontButton = GetDlgItem(hWnd,IDC_CHECK_USERFONT);
		m_fontNameList = GetDlgItem(hWnd,IDC_COMBO_FONTNAME);

		m_lowTextureButton = GetDlgItem(hWnd,IDC_CHECK_LOWTEXTURE);

		HDC hdc;
		hdc = GetDC(hWnd);
		
		LOGFONT lf;
		ZeroMemory(&lf,sizeof(lf));
		lf.lfCharSet = SHIFTJIS_CHARSET;
		lf.lfFaceName[0] = 0;
		lf.lfPitchAndFamily = 0;
		
		EnumFontFamiliesEx(hdc,&lf,(FONTENUMPROC)EnumFontFamExProc,(LPARAM)0,0);

		ReleaseDC(hWnd,hdc);


		m_directDrawAutoRadio = GetDlgItem(hWnd,IDC_RADIO_DIRECTDRAW_AUTO);
		m_directDrawNotUseRadio = GetDlgItem(hWnd,IDC_RADIO_DIRECTDRAW_NOTUSE);

		m_2d3dAutoRadio = GetDlgItem(hWnd,IDC_RADIO_2D3D_AUTO);
		m_2dRadio = GetDlgItem(hWnd,IDC_RADIO_2D3D_2D);
		m_3dRadio = GetDlgItem(hWnd,IDC_RADIO_2D3D_3D);
		m_needautoShaderRadio = GetDlgItem(hWnd,IDC_RADIO_NEEDSHADER_AUTO);
		m_needshader_1_1_Radio = GetDlgItem(hWnd,IDC_RADIO_NEEDSHADER_1_1);
		m_needshader_2_0_Radio = GetDlgItem(hWnd,IDC_RADIO_NEEDSHADER_2_0);
		m_needshader_3_0_Radio = GetDlgItem(hWnd,IDC_RADIO_NEEDSHADER_3_0);

		m_vshaderAutoRadio = GetDlgItem(hWnd,IDC_RADIO_VSHADER_AUTO);
		m_vshaderHardRadio = GetDlgItem(hWnd,IDC_RADIO_VSHADER_HARD);
		m_vshaderSoftRadio = GetDlgItem(hWnd,IDC_RADIO_VSHADER_SOFT);
//		m_shader_3_0_Radio = GetDlgItem(hWnd,IDC_RADIO_SHADER_3_0);

		if (m_codeByte == 1)
		{

			SetWindowText(m_directDrawAutoRadio, "Auto");
			SetWindowText(m_directDrawNotUseRadio, "Disabled");

			SetWindowText(GetDlgItem(hWnd, IDC_STATIC_2D3D), "2D/3D");
			SetWindowText(m_2d3dAutoRadio, "Auto");
			SetWindowText(m_2dRadio, "2D");
			SetWindowText(m_3dRadio, "3D");

			SetWindowText(GetDlgItem(hWnd, IDC_STATIC_PIXELSHADER), "PixelShader");
			SetWindowText(m_needautoShaderRadio, "disabled");
			SetWindowText(m_needshader_1_1_Radio, "1.1");
			SetWindowText(m_needshader_2_0_Radio, "2.0");
			SetWindowText(m_needshader_3_0_Radio, "3.0");

			SetWindowText(GetDlgItem(hWnd, IDC_STATIC_SOFTVERTEXSHADER), "Soft V Shader");
			SetWindowText(m_vshaderAutoRadio, "Auto");
			SetWindowText(m_vshaderSoftRadio, "Soft");
			SetWindowText(m_vshaderHardRadio, "Hard");

			SetWindowText(GetDlgItem(hWnd, IDC_STATIC_MOVIE), "Movies");
			SetWindowText(m_useVMRButton, "Use VMR9");

			SetWindowText(GetDlgItem(hWnd, IDC_STATIC_FONT), "Font");
			SetWindowText(m_msGothicRadio, "Consolas");
			SetWindowText(m_msMinchoRadio, "Courier New");
			SetWindowText(m_useUserFontButton, "Use Font:");


			SetWindowText(m_noSkipFrameButton, "Don't Skip Frames");

			SetWindowText(GetDlgItem(hWnd, IDC_STATIC_LOWTEXTURE), "Low-Res Textures");
			SetWindowText(m_lowTextureButton, "Enabled");

			/*

			SetWindowText(m_directDrawAutoRadio,"auto");
			SetWindowText(m_directDrawNotUseRadio,"not Use");

			SetWindowText(GetDlgItem(hWnd,IDC_STATIC_2D3D),"Selece 2D 3D");
			SetWindowText(m_2d3dAutoRadio,"Auto");
			SetWindowText(m_2dRadio,"2D");
			SetWindowText(m_3dRadio,"3D");

			SetWindowText(GetDlgItem(hWnd,IDC_STATIC_PIXELSHADER),"PixelShader");
			SetWindowText(m_needautoShaderRadio,"not");
			SetWindowText(m_needshader_1_1_Radio,"1.1");
			SetWindowText(m_needshader_2_0_Radio,"2.0");
			SetWindowText(m_needshader_3_0_Radio,"3.0");

			SetWindowText(GetDlgItem(hWnd,IDC_STATIC_SOFTVERTEXSHADER),"Soft V Shader");
			SetWindowText(m_vshaderAutoRadio,"Auto");
			SetWindowText(m_vshaderSoftRadio,"Soft");
			SetWindowText(m_vshaderHardRadio,"Hard");

			SetWindowText(GetDlgItem(hWnd,IDC_STATIC_MOVIE),"Movie");
			SetWindowText(m_useVMRButton,"Use VMR9");

			SetWindowText(GetDlgItem(hWnd,IDC_STATIC_FONT),"Font");
			SetWindowText(m_msGothicRadio,"Consolas");
			SetWindowText(m_msMinchoRadio,"Courier New");
			SetWindowText(m_useUserFontButton,"UseFont");


			SetWindowText(m_noSkipFrameButton,"NoSkipFrame");

			SetWindowText(GetDlgItem(hWnd,IDC_STATIC_LOWTEXTURE),"LowTexture");
			SetWindowText(m_lowTextureButton,"Low");
			*/

		}

		LPSTR replaceFontNameChara;
		replaceFontNameChara = "?";
		GetInitGameString(&replaceFontNameChara,"replaceFontnameChara");

		LPSTR font1;
		font1 = NULL;
		if (GetInitGameString(&font1,"gothicFontName"))
		{
			//replace?
			if (replaceFontNameChara != NULL)
			{
				ReplaceName(font1,replaceFontNameChara);
			}

			SetWindowText(m_msGothicRadio,font1);
		}
		LPSTR font2;
		font2 = NULL;
		if (GetInitGameString(&font2,"minchoFontName"))
		{
			//replace?
			if (replaceFontNameChara != NULL)
			{
				ReplaceName(font2,replaceFontNameChara);
			}

			SetWindowText(m_msMinchoRadio,font2);
		}





		SetDefaultFontButton();
		SetUserFontButton();
		SetSelectUserFontList();
		SetSkipFrameButton();
		SetVMRButton();

		SetDirectDrawButton();
		Set2D3DButton();
		SetShaderButton();

		SetLowTextureButton();

		MaskFont();

		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		MoveWindow(hWnd,32,32,sx,sy,TRUE);

		return 1;
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_CHECK_USERFONT:
			int old2;
			old2 = m_userFontFlag;
			GetUserFontButton();
			if (m_userFontFlag != old2)
			{
				MaskFont();
				UpdateWindow(hWnd);
			}

			break;
		}
		break;

	case WM_CLOSE:
//		EndDialog(hWnd,1);
		break;
	}

	return FALSE;
}



BOOL CALLBACK DlgProc3( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		m_dialog3 = hWnd;


		for (int i=0;i<16;i++)
		{
			m_expCheckButton[i] = GetDlgItem(hWnd,m_expCheckID[i]);
		}

		for (int i=0;i<m_useExpCheck;i++)
		{
			char name[256];
			wsprintf(name,"disableExpCheckConfig%d",i+1);
			int dis = 0;
			GetInitGameParam(&dis,name);
			if (dis == 0)
			{
				EnableWindow(m_expCheckButton[i],TRUE);
			}
			
			//title
			LPSTR cap = NULL;
			wsprintf(name,"expCheckConfigCaption%d",i+1);
			if (GetInitGameString(&cap,name))
			{
				SetWindowText(m_expCheckButton[i],cap);
			}
		}


		SetExpCheckButton();


		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		MoveWindow(hWnd,32,32,sx,sy,TRUE);

		return 1;
		break;

	case WM_COMMAND:
		int expnumber;
		expnumber = SearchExpCheckID(wParam);
		if (expnumber >= 0)
		{

			UpdateWindow(hWnd);
		}

		break;

	case WM_CLOSE:
//		EndDialog(hWnd,1);
		break;
	}

	return FALSE;
}


BOOL CALLBACK DlgProc4( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		m_dialog4 = hWnd;

		for (int i=0;i<12;i++)
		{
			int kosuu = m_expRadioButtonMax[i];
			for (int k=0;k<kosuu;k++)
			{
				m_expRadioButton[i][k] = GetDlgItem(hWnd,m_expRadioCheckID[i][k]);
			}

			if (i<m_useExpRadio)
			{
				char name[256];
				wsprintf(name,"disableExpRadioConfig%d",i+1);
				int dis = 0;
				GetInitGameParam(&dis,name);
				if (dis == 0)
				{
					for (int k=0;k<kosuu;k++)
					{
					//	EnableWindow(m_expRadioButton[i][k],TRUE);
						ShowWindow(m_expRadioButton[i][k],SW_SHOW);

						LPSTR cap = NULL;
						wsprintf(name,"expRadioConfigCaption%d_%d",i+1,k+1);
						if (GetInitGameString(&cap,name))
						{
							SetWindowText(m_expRadioButton[i][k],cap);
						}

					}
					//title
					if (1)
					{
						LPSTR cap = NULL;
						wsprintf(name,"expRadioConfigStaticCation%d",i+1);
						HWND captionHWnnd = GetDlgItem(hWnd,m_expRadioCaptionID[i]);
						if (GetInitGameString(&cap,name))
						{
							SetWindowText(captionHWnnd,cap);
						}
						ShowWindow(captionHWnnd,SW_SHOW);
					}
				}
			}
		}


		SetExpRadioButton();

		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		MoveWindow(hWnd,32,32,sx,sy,TRUE);

		return 1;
		break;

	case WM_COMMAND:
		switch (wParam)
		{
//				UpdateWindow(hWnd);

		case 1:
		default:
			break;
		}
		break;

	case WM_CLOSE:
//		EndDialog(hWnd,1);
		break;
	}

	return FALSE;
}


BOOL CALLBACK DlgProc5( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		m_dialog5 = hWnd;


		for (int i=12;i<16;i++)
		{
			int kosuu = m_expRadioButtonMax[i];
			for (int k=0;k<kosuu;k++)
			{
				m_expRadioButton[i][k] = GetDlgItem(hWnd,m_expRadioCheckID[i][k]);
			}

			if (i<m_useExpRadio)
			{
				char name[256];
				wsprintf(name,"disableExpRadioConfig%d",i+1);
				int dis = 0;
				GetInitGameParam(&dis,name);
				if (dis == 0)
				{
					for (int k=0;k<kosuu;k++)
					{
					//	EnableWindow(m_expRadioButton[i][k],TRUE);
						ShowWindow(m_expRadioButton[i][k],SW_SHOW);

						LPSTR cap = NULL;
						wsprintf(name,"expRadioConfigCaption%d_%d",i+1,k+1);
						if (GetInitGameString(&cap,name))
						{
							SetWindowText(m_expRadioButton[i][k],cap);
						}

					}
					//title
					if (1)
					{
						LPSTR cap = NULL;
						wsprintf(name,"expRadioConfigStaticCation%d",i+1);
						HWND captionHWnnd = GetDlgItem(hWnd,m_expRadioCaptionID[i]);
						if (GetInitGameString(&cap,name))
						{
							SetWindowText(captionHWnnd,cap);
						}
						ShowWindow(captionHWnnd,SW_SHOW);
					}
				}
			}
		}

		//SetExpRadioButton();

		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		MoveWindow(hWnd,32,32,sx,sy,TRUE);

		return 1;
		break;

	case WM_COMMAND:
		switch (wParam)
		{
//				UpdateWindow(hWnd);

		case 1:
		default:
			break;
		}
		break;

	case WM_CLOSE:
//		EndDialog(hWnd,1);
		break;
	}

	return FALSE;
}

BOOL CALLBACK DlgProc6( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		m_dialog6 = hWnd;

		HWND cannotMessage;
		cannotMessage = GetDlgItem(hWnd,IDC_STATIC_CANNOTFULLCAPTION);
		if (m_codeByte == 2)
		{
		//	SetWindowText(cannotMessage,"");
		}
		else
		{
			SetWindowText(cannotMessage,"*:cannot support FullScreenMode");
			SetWindowText(GetDlgItem(hWnd,IDC_STATIC_SCREENSIZECAPTION),"Select screen size");
			SetWindowText(GetDlgItem(hWnd,IDC_CHECK_SCREENSTRETCH),"Stretch screen");
		}

		ShowWindow(GetDlgItem(hWnd,IDC_STATIC_SCREENSIZECAPTION),SW_SHOW);


		for (int i=0;i<16;i++)
		{
			if (i<m_screenSizeTypeMax)
			{
				//string
				int sizeX = 0;
				int sizeY = 0;
				char name[256];
				wsprintf(name,"realWindowSizeX%d",i+1);
				GetInitGameParam(&sizeX,name);
				wsprintf(name,"realWindowSizeY%d",i+1);
				GetInitGameParam(&sizeY,name);
				m_screenSizeTable[i].cx = sizeX;
				m_screenSizeTable[i].cy = sizeY;
			}
		}




		int devNum;
		devNum = 0;
		DEVMODE devMode2;
		ZeroMemory(&devMode2,sizeof(devMode2));
		devMode2.dmSize = sizeof(devMode2);
//		devMode2.dmPelsWidth = m_windowSizeX;
//		devMode2.dmPelsHeight = m_windowSizeY;
		devMode2.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY | DM_BITSPERPEL;  
//		BOOL cannot = TRUE;

		while (EnumDisplaySettings(NULL,devNum,&devMode2))
		{
			if (devMode2.dmBitsPerPel >= 16)
			{
				int sizeX = devMode2.dmPelsWidth;
				int sizeY = devMode2.dmPelsHeight;
				for (int i=0;i<m_screenSizeTypeMax;i++)
				{
					if ((sizeX == m_screenSizeTable[i].cx) && (sizeY == m_screenSizeTable[i].cy))
					{
						m_screenSizeEnable[i] = TRUE;
						break;
					}
				}
			}
			devNum++;
		}



		m_screenStretchButton = GetDlgItem(hWnd,IDC_CHECK_SCREENSTRETCH);
		for (int i=0;i<16;i++)
		{
			m_screenSizeButton[i] = GetDlgItem(hWnd,m_screenSizeID[i]);
			if (i<m_screenSizeTypeMax)
			{
				//string
				int sizeX = m_screenSizeTable[i].cx;
				int sizeY = m_screenSizeTable[i].cy;
				char name[256];
				wsprintf(name,"realWindowSizeX%d",i+1);
				GetInitGameParam(&sizeX,name);
				wsprintf(name,"realWindowSizeY%d",i+1);
				GetInitGameParam(&sizeY,name);
				//check
				BOOL enable = m_screenSizeEnable[i];

				if (enable)
				{
					wsprintf(name,"%dx%d",sizeX,sizeY);
				}
				else
				{
					if (m_codeByte == 2)
					{
						wsprintf(name,"※%dx%d",sizeX,sizeY);
					}
					else
					{
						wsprintf(name,"*%dx%d",sizeX,sizeY);
					}
				}

				SetWindowText(m_screenSizeButton[i],name);

				//show
				ShowWindow(m_screenSizeButton[i],SW_SHOW);

			}
		}

		SetScreenSizeButton();

		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		MoveWindow(hWnd,32,32,sx,sy,TRUE);

		return 1;
		break;

	case WM_COMMAND:
		switch (wParam)
		{
//				UpdateWindow(hWnd);

		case 1:
		default:
			break;
		}
		break;

	case WM_CLOSE:
//		EndDialog(hWnd,1);
		break;
	}

	return FALSE;
}

void DataForSave(void)
{
	m_systemdata->fullScreenFlag = m_screenMode;
	m_systemdata->skipMode = m_skipMode;
	m_systemdata->fontType = m_fontMode;
	m_systemdata->userFontFlag = m_userFontFlag;

	LPSTR name = m_systemdata->fontName;
	int ln = strlen(m_userFontName);
	if (ln>254) ln = 254;
	memcpy(name,m_userFontName,ln);
	name[ln] = 0;
	name[ln+1] = 0;

	m_systemdata->noSkipFrame = m_skipFrameFlag;
	m_systemdata->useVMR9 = m_useVMRFlag;

	m_systemdata->musicVolume = m_musicVolume;
	m_systemdata->soundVolume = m_soundVolume;
	m_systemdata->voiceVolume = m_voiceVolume;
	m_systemdata->movieVolume = m_movieVolume;
	m_systemdata->soundVoiceVolume = m_soundVoiceVolume;
	m_systemdata->totalVolume = m_totalVolume;

	m_systemdata->messageSpeed = m_messageSpeed;

	m_systemdata->musicSwitch = m_musicSwitch;
	m_systemdata->soundSwitch = m_soundSwitch;
	m_systemdata->voiceSwitch = m_voiceSwitch;
	m_systemdata->movieSwitch = m_movieSwitch;
	m_systemdata->soundVoiceSwitch = m_soundVoiceSwitch;
	m_systemdata->totalVolumeSwitch = m_totalVolumeSwitch;


	m_systemdata->notUseDirectDraw = m_directDrawNotUse;
	m_systemdata->d2d3d = m_2d3d;
	m_systemdata->shader = m_shader;
	m_systemdata->needShader = m_needshader;

	m_systemdata->lowTexture = m_lowTextureFlag;

	for (int i=0;i<16;i++)
	{
		m_systemdata->expCheck[i] = m_expCheck[i];
	}

	for (int i=0;i<16;i++)
	{
		m_systemdata->expMode[i] = m_expRadio[i];
	}

	
	m_systemdata->screenSizeType = m_screenSizeType;
	if (m_screenStrecthFlag)
	{
		m_systemdata->genericFlag |= 1;
	}
	else
	{
		m_systemdata->genericFlag &= ~1;
	}


	//=m_2d3d;
	//=m_shader;
}


void DataByLoad(void)
{
	m_screenMode = m_systemdata->fullScreenFlag;
	m_skipMode = m_systemdata->skipMode;
	m_fontMode = m_systemdata->fontType;
	m_userFontFlag = m_systemdata->userFontFlag;
	LPSTR name = m_systemdata->fontName;
	int ln = strlen(name);
	if (ln>254) ln = 254;
	memcpy(m_userFontName,name,ln);
	m_userFontName[ln] = 0;
	m_userFontName[ln+1] = 0;
	m_skipFrameFlag = m_systemdata->noSkipFrame;
	m_useVMRFlag = m_systemdata->useVMR9;

	m_musicVolume = m_systemdata->musicVolume;
	m_soundVolume = m_systemdata->soundVolume;
	m_voiceVolume = m_systemdata->voiceVolume;
	m_movieVolume = m_systemdata->movieVolume;
	m_soundVoiceVolume = m_systemdata->soundVoiceVolume;
	m_totalVolume = m_systemdata->totalVolume;
	m_messageSpeed = m_systemdata->messageSpeed;

	m_musicSwitch = m_systemdata->musicSwitch;
	m_soundSwitch = m_systemdata->soundSwitch;
	m_voiceSwitch = m_systemdata->voiceSwitch;
	m_movieSwitch = m_systemdata->movieSwitch;
	m_soundVoiceSwitch = m_systemdata->soundVoiceSwitch;
	m_totalVolumeSwitch = m_systemdata->totalVolumeSwitch;


	m_directDrawNotUse = m_systemdata->notUseDirectDraw;
	m_2d3d = m_systemdata->d2d3d;
	m_shader = m_systemdata->shader;
	m_needshader = m_systemdata->needShader;

	if (m_2d3dSelect != 0)
	{
		m_2d3d = m_2d3dSelect;
	}


	m_lowTextureFlag = m_systemdata->lowTexture;

	for (int i=0;i<16;i++)
	{
		m_expCheck[i] = m_systemdata->expCheck[i];
	}

	for (int i=0;i<16;i++)
	{
		m_expRadio[i] = m_systemdata->expMode[i];
	}

	m_screenSizeType = m_systemdata->screenSizeType;
	m_screenStrecthFlag = m_systemdata->genericFlag & 1;
}



void MaskFont(void)
{
	if (m_userFontFlag == 0)
	{
		EnableWindow(m_msGothicRadio,TRUE);
		EnableWindow(m_msMinchoRadio,TRUE);
		EnableWindow(m_fontNameList,FALSE);
	}
	else
	{
		EnableWindow(m_msGothicRadio,FALSE);
		EnableWindow(m_msMinchoRadio,FALSE);
		EnableWindow(m_fontNameList,TRUE);
	}
}


void SetScreenModeButton(void)
{
	if (m_screenMode == 0)
	{
		SendMessage(m_windowModeRadio,BM_SETCHECK,BST_CHECKED,0);
		SendMessage(m_fullModeRadio,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		SendMessage(m_windowModeRadio,BM_SETCHECK,BST_UNCHECKED,0);
		SendMessage(m_fullModeRadio,BM_SETCHECK,BST_CHECKED,0);
	}
}

void SetSkipModeButton(void)
{
	if (m_skipMode == 0)
	{
		SendMessage(m_skipReadRadio,BM_SETCHECK,BST_CHECKED,0);
		SendMessage(m_skipAllRadio,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		SendMessage(m_skipReadRadio,BM_SETCHECK,BST_UNCHECKED,0);
		SendMessage(m_skipAllRadio,BM_SETCHECK,BST_CHECKED,0);
	}
}


void SetDefaultFontButton(void)
{
	if (m_fontMode == 0)
	{
		SendMessage(m_msGothicRadio,BM_SETCHECK,BST_CHECKED,0);
		SendMessage(m_msMinchoRadio,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		SendMessage(m_msGothicRadio,BM_SETCHECK,BST_UNCHECKED,0);
		SendMessage(m_msMinchoRadio,BM_SETCHECK,BST_CHECKED,0);
	}
}

void SetUserFontButton(void)
{
	if (m_userFontFlag == 0)
	{
		SendMessage(m_useUserFontButton,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		SendMessage(m_useUserFontButton,BM_SETCHECK,BST_CHECKED,0);
	}
}


void SetSelectUserFontList(void)
{
	int found = 0;

	for (int i=0;i<m_fontKosuu;i++)
	{
		char name2[256];
		SendMessage(m_fontNameList,CB_GETLBTEXT,(WPARAM)i,(LPARAM)name2);
		if (strcmp(m_userFontName,name2) == 0)
		{
			//found
			found = i;
			break;
		}
	}

	SendMessage(m_fontNameList,CB_SETCURSEL,(WPARAM)found,(LPARAM)0);
}


void SetSkipFrameButton(void)
{
	if (m_skipFrameFlag == 0)
	{
		SendMessage(m_noSkipFrameButton,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		SendMessage(m_noSkipFrameButton,BM_SETCHECK,BST_CHECKED,0);
	}
}

void SetVMRButton(void)
{
	if (m_useVMRFlag == 0)
	{
		SendMessage(m_useVMRButton,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		SendMessage(m_useVMRButton,BM_SETCHECK,BST_CHECKED,0);
	}
}

void SetDirectDrawButton(void)
{
	if (m_directDrawNotUse == 0)
	{
		SendMessage(m_directDrawAutoRadio,BM_SETCHECK,BST_CHECKED,0);
		SendMessage(m_directDrawNotUseRadio,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		SendMessage(m_directDrawAutoRadio,BM_SETCHECK,BST_UNCHECKED,0);
		SendMessage(m_directDrawNotUseRadio,BM_SETCHECK,BST_CHECKED,0);
	}

}

void Set2D3DButton(void)
{
	SendMessage(m_2d3dAutoRadio,BM_SETCHECK,BST_UNCHECKED,0);
	SendMessage(m_2dRadio,BM_SETCHECK,BST_UNCHECKED,0);
	SendMessage(m_3dRadio,BM_SETCHECK,BST_UNCHECKED,0);


	if (m_2d3d == 0)
	{
		SendMessage(m_2d3dAutoRadio,BM_SETCHECK,BST_CHECKED,0);
	}
	else if (m_2d3d == 2)
	{
		SendMessage(m_2dRadio,BM_SETCHECK,BST_CHECKED,0);
	}
	else if (m_2d3d == 3)
	{
		SendMessage(m_3dRadio,BM_SETCHECK,BST_CHECKED,0);
	}

	if (m_2d3dSelect == 2)
	{
		EnableWindow(m_2d3dAutoRadio,FALSE);
		EnableWindow(m_3dRadio,FALSE);
	}
	else if (m_2d3dSelect == 3)
	{
		EnableWindow(m_2d3dAutoRadio,FALSE);
		EnableWindow(m_2dRadio,FALSE);
	}

}

void SetShaderButton(void)
{
	SendMessage(m_needautoShaderRadio,BM_SETCHECK,BST_UNCHECKED,0);
	SendMessage(m_needshader_1_1_Radio,BM_SETCHECK,BST_UNCHECKED,0);
	SendMessage(m_needshader_2_0_Radio,BM_SETCHECK,BST_UNCHECKED,0);
	SendMessage(m_needshader_3_0_Radio,BM_SETCHECK,BST_UNCHECKED,0);

	if (m_needshader == 0)
	{
		SendMessage(m_needautoShaderRadio,BM_SETCHECK,BST_CHECKED,0);
	}
	else if (m_needshader == 1)
	{
		SendMessage(m_needshader_1_1_Radio,BM_SETCHECK,BST_CHECKED,0);
	}
	else if (m_needshader == 2)
	{
		SendMessage(m_needshader_2_0_Radio,BM_SETCHECK,BST_CHECKED,0);
	}
	else if (m_needshader == 3)
	{
		SendMessage(m_needshader_3_0_Radio,BM_SETCHECK,BST_CHECKED,0);
	}

	SendMessage(m_vshaderAutoRadio,BM_SETCHECK,BST_UNCHECKED,0);
	SendMessage(m_vshaderHardRadio,BM_SETCHECK,BST_UNCHECKED,0);
	SendMessage(m_vshaderSoftRadio,BM_SETCHECK,BST_UNCHECKED,0);
//	SendMessage(m_shader_3_0_Radio,BM_SETCHECK,BST_UNCHECKED,0);

	if (m_shader == 0)
	{
		SendMessage(m_vshaderAutoRadio,BM_SETCHECK,BST_CHECKED,0);
	}
	else if (m_shader == 1)
	{
		SendMessage(m_vshaderSoftRadio,BM_SETCHECK,BST_CHECKED,0);
	}
	else if (m_shader == 2)
	{
		SendMessage(m_vshaderHardRadio,BM_SETCHECK,BST_CHECKED,0);
	}
//	else if (m_shader == 3)
//	{
//		SendMessage(m_shader_3_0_Radio,BM_SETCHECK,BST_CHECKED,0);
//	}
}

void SetVolumeSlider(void)
{
	int vol = AdjustVolumeByLoad(m_musicVolume);
	SendMessage(m_musicVolumeSlider,TBM_SETPOS,(WPARAM)TRUE,vol);

	vol = AdjustVolumeByLoad(m_soundVolume);
	SendMessage(m_soundVolumeSlider,TBM_SETPOS,(WPARAM)TRUE,vol);

	vol = AdjustVolumeByLoad(m_voiceVolume);
	SendMessage(m_voiceVolumeSlider,TBM_SETPOS,(WPARAM)TRUE,vol);

	vol = AdjustVolumeByLoad(m_movieVolume);
	SendMessage(m_movieVolumeSlider,TBM_SETPOS,(WPARAM)TRUE,vol);

	vol = AdjustVolumeByLoad(m_soundVoiceVolume);
	SendMessage(m_soundVoiceVolumeSlider,TBM_SETPOS,(WPARAM)TRUE,vol);

	SendMessage(m_totalVolumeSlider,TBM_SETPOS,(WPARAM)TRUE,m_totalVolume);

}


void SetMessageSpeedSlider(void)
{
	SendMessage(m_messageSpeedSlider,TBM_SETPOS,(WPARAM)TRUE,m_messageSpeed);
}


void SetVolumeSwitch(void)
{
	int st = BST_UNCHECKED;
	if (m_musicSwitch) st = BST_CHECKED;
	SendMessage(m_musicCheck,BM_SETCHECK,st,0);

	st = BST_UNCHECKED;
	if (m_soundSwitch) st = BST_CHECKED;
	SendMessage(m_soundCheck,BM_SETCHECK,st,0);

	st = BST_UNCHECKED;
	if (m_voiceSwitch) st = BST_CHECKED;
	SendMessage(m_voiceCheck,BM_SETCHECK,st,0);

	st = BST_UNCHECKED;
	if (m_movieSwitch) st = BST_CHECKED;
	SendMessage(m_movieCheck,BM_SETCHECK,st,0);

	st = BST_UNCHECKED;
	if (m_soundVoiceSwitch) st = BST_CHECKED;
	SendMessage(m_soundVoiceCheck,BM_SETCHECK,st,0);

	st = BST_UNCHECKED;
	if (m_totalVolumeSwitch) st = BST_CHECKED;
	SendMessage(m_totalCheck,BM_SETCHECK,st,0);

}

void SetLowTextureButton(void)
{
	if (m_lowTextureFlag == 0)
	{
		SendMessage(m_lowTextureButton,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else
	{
		SendMessage(m_lowTextureButton,BM_SETCHECK,BST_CHECKED,0);
	}
}

void SetExpCheckButton(void)
{
	for (int i=0;i<16;i++)
	{
		if (m_expCheck[i] == 0)
		{
			SendMessage(m_expCheckButton[i],BM_SETCHECK,BST_UNCHECKED,0);
		}
		else
		{
			SendMessage(m_expCheckButton[i],BM_SETCHECK,BST_CHECKED,0);
		}
	}
}
//get

void SetExpRadioButton(void)
{
}

void SetScreenSizeButton(void)
{
	for (int i=0;i<16;i++)
	{
		if (m_screenSizeType == i)
		{
			SendMessage(m_screenSizeButton[i],BM_SETCHECK,BST_CHECKED,0);
		}
		else
		{
			SendMessage(m_screenSizeButton[i],BM_SETCHECK,BST_UNCHECKED,0);
		}
	}
}


void GetScreenModeButton(void)
{
	if (SendMessage(m_windowModeRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_screenMode = 0;
	}

	if (SendMessage(m_fullModeRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_screenMode = 1;
	}
}

void GetSkipModeButton(void)
{
	if (SendMessage(m_skipReadRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_skipMode = 0;
	}

	if (SendMessage(m_skipAllRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_skipMode = 1;
	}
}


void GetDefaultFontButton(void)
{
	if (SendMessage(m_msGothicRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_fontMode = 0;
	}

	if (SendMessage(m_msMinchoRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_fontMode = 1;
	}
}

void GetSelectUserFontList(void)
{
	int sel = SendMessage(m_fontNameList,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if ((sel<0) || (sel>=m_fontKosuu)) sel = 0;

	char name[256];
	name[0] = 0;
	SendMessage(m_fontNameList,CB_GETLBTEXT,(WPARAM)sel,(LPARAM)name);
	int ln = strlen(name);
	if (ln>254) ln = 254;
	if (ln<0) ln = 0;
	memcpy(m_userFontName,name,ln);
	m_userFontName[ln] = 0;
	m_userFontName[ln+1] = 0;
}


void GetSkipFrameButton(void)
{
	if (SendMessage(m_noSkipFrameButton,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_skipFrameFlag = 1;
	}
	else
	{
		m_skipFrameFlag = 0;
	}
}

void GetVMRButton(void)
{
	if (SendMessage(m_useVMRButton,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_useVMRFlag = 1;
	}
	else
	{
		m_useVMRFlag = 0;
	}
}

void GetUserFontButton(void)
{
	if (SendMessage(m_useUserFontButton,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_userFontFlag = 1;
	}
	else
	{
		m_userFontFlag = 0;
	}
}

void GetDirectDrawButton(void)
{
	if (SendMessage(m_directDrawAutoRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_directDrawNotUse = 0;
	}
	else if (SendMessage(m_directDrawNotUseRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_directDrawNotUse = 1;
	}
	else
	{
		m_directDrawNotUse = 0;//error!

	}

}

void Get2d3dButton(void)
{
	if (SendMessage(m_2d3dAutoRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_2d3d = 0;
	}
	else if (SendMessage(m_2dRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_2d3d = 2;
	}
	else if (SendMessage(m_3dRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_2d3d = 3;
	}
}


void GetShaderButton(void)
{
	if (SendMessage(m_needautoShaderRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_needshader = 0;
	}
	else if (SendMessage(m_needshader_1_1_Radio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_needshader = 1;
	}
	else if (SendMessage(m_needshader_2_0_Radio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_needshader = 2;
	}
	else if (SendMessage(m_needshader_3_0_Radio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_needshader = 3;
	}

	if (SendMessage(m_vshaderAutoRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_shader = 0;
	}
	else if (SendMessage(m_vshaderSoftRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_shader = 1;
	}
	else if (SendMessage(m_vshaderHardRadio,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_shader = 2;
	}
//	else if (SendMessage(m_shader_3_0_Radio,BM_GETCHECK,0,0) == BST_CHECKED)
//	{
//		m_shader = 3;
//	}
}


void GetVolumeSlider(void)
{
	m_musicVolume = AdjustVolumeForSave(SendMessage(m_musicVolumeSlider,TBM_GETPOS,0,0));
	m_soundVolume = AdjustVolumeForSave(SendMessage(m_soundVolumeSlider,TBM_GETPOS,0,0));
	m_voiceVolume = AdjustVolumeForSave(SendMessage(m_voiceVolumeSlider,TBM_GETPOS,0,0));
	m_movieVolume = AdjustVolumeForSave(SendMessage(m_movieVolumeSlider,TBM_GETPOS,0,0));
	m_soundVoiceVolume = AdjustVolumeForSave(SendMessage(m_soundVoiceVolumeSlider,TBM_GETPOS,0,0));
	m_totalVolume = SendMessage(m_totalVolumeSlider,TBM_GETPOS,0,0);
}

void GetMessageSpeedSlider(void)
{
	m_messageSpeed = SendMessage(m_messageSpeedSlider,TBM_GETPOS,0,0);
}


void GetVolumeSwitch(void)
{
	m_musicSwitch = 0;
	m_soundSwitch = 0;
	m_voiceSwitch = 0;
	m_movieSwitch = 0;
	m_soundVoiceSwitch = 0;
	m_totalVolumeSwitch = 0;

	if (SendMessage(m_musicCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_musicSwitch = 1;
	if (SendMessage(m_soundCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_soundSwitch = 1;
	if (SendMessage(m_voiceCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_voiceSwitch = 1;
	if (SendMessage(m_movieCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_movieSwitch = 1;
	if (SendMessage(m_soundVoiceCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_soundVoiceSwitch = 1;
	if (SendMessage(m_totalCheck,BM_GETCHECK,0,0) == BST_CHECKED) m_totalVolumeSwitch = 1;
}



void GetLowTextureButton(void)
{
	if (SendMessage(m_lowTextureButton,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_lowTextureFlag = 1;
	}
	else
	{
		m_lowTextureFlag = 0;
	}
}

void GetExpCheckButton(void)
{
	for (int i=0;i<16;i++)
	{
		if (SendMessage(m_expCheckButton[i],BM_GETCHECK,0,0) == BST_CHECKED)
		{
			m_expCheck[i] = 1;
		}
		else
		{
			m_expCheck[i] = 0;
		}
	}
}

void GetExpRadioButton(void)
{
	for (int i=0;i<16;i++)
	{
		int use = m_expRadioButtonUse[i];
		for (int k=0;k<use;k++)
		{
			if (SendMessage(m_expRadioButton[i][k],BM_GETCHECK,0,0) == BST_CHECKED)
			{
				m_expRadio[i] = k;
			}
		}
	}
}


void GetScreenSizeButton(void)
{
	for (int i=0;i<16;i++)
	{
		if (SendMessage(m_screenSizeButton[i],BM_GETCHECK,0,0) == BST_CHECKED)
		{
			m_screenSizeType = i;
			break;
		}
	}

	if (SendMessage(m_screenStretchButton,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		m_screenStrecthFlag |= 1;
	}
	else
	{
		m_screenStrecthFlag &= ~1;
	}

}


int CALLBACK EnumFontFamExProc(

  ENUMLOGFONTEX *lpelfe, // pointer to logical-font data

  NEWTEXTMETRICEX *lpntme, // pointer to physical-font data

  int FontType, // type of font

  LPARAM lParam // application-defined data
)
{
	LPSTR name = lpelfe->elfLogFont.lfFaceName;
	int ln = strlen(name);

	if (strlen(name)>=254) return FALSE;

//	memcpy(m_fontName + m_fontKosuu*256,name,ln);
//	m_fontName[m_fontKosuu*256+ln] = 0;
//	m_fontName[m_fontKosuu*256+ln+1] = 0;

	SendMessage(m_fontNameList,CB_INSERTSTRING,(WPARAM)m_fontKosuu,(LPARAM)name);

	m_fontKosuu++;
	if (m_fontKosuu>=255) return FALSE;
	return TRUE;
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

BOOL GetInitGameParam3(int* lpVar, LPSTR name)
{
	int rNum = m_setup3->SearchName2(name);
	if (rNum == -1) return FALSE;

	*lpVar = atoi(m_setup3->GetName(rNum + 1));

	return TRUE;
}


BOOL GetInitGameString3(LPSTR* lpStr, LPSTR name)
{
	int rNum = m_setup3->SearchName2(name);
	if (rNum == -1) return FALSE;

	*lpStr = m_setup3->GetName(rNum + 1);

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

BOOL LoadSetupFile3(LPSTR filenameonly, int varKosuu)
{
	if (m_setup3 == NULL)
	{
		m_setup3 = new CNameList(varKosuu * 2);
	}

	if (m_setup3 == NULL) return FALSE;

	return m_setup3->LoadInit(filenameonly);
}

int SearchExpCheckID(int wParam)
{
	for (int i=0;i<16;i++)
	{
		if (m_expCheckID[i] == wParam) return i;
	}
	return -1;
}

int AdjustVolumeByLoad(int vol)
{
	int dv = m_volumeMax - m_volumeMin;
	if (dv<1) dv = 1;
	int v = vol - m_volumeMin;
	v *= 100;
	v /= dv;
	if (v<0) v = 0;
	if (v>100) v = 100;

	return v;
}

int AdjustVolumeForSave(int vol)
{
	int ml = m_volumeMax - m_volumeMin;
	int v = (vol * ml) / 100;
	v += m_volumeMin;

	return v;
}

void ReplaceName(LPSTR name,LPSTR replaceChara)
{
	if (name == NULL) return;
	if (replaceChara == NULL) return;
	char rep = *replaceChara;

	int ln = strlen(name);
	int n = 0;
	while (n<ln)
	{
		char c = *(name+n);
		if (c == rep)
		{
			*(name + n) = ' ';
		}

		n++;
		if (m_codeByte == 2)
		{
			int d = c;
			d &= 0xff;

			if ((d >= 0x80) && (d<0xa0)) n++;
			if ((d >= 0xe0) && (d<0x100)) n++;
		}
	}
}



