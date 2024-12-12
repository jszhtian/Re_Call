//
// main.cpp
//

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "main.h"



//#include "checkfiledate.h"

#include "resource.h"

#include "packroutine.h"

#pragma comment(lib,"Comctl32.lib")

#if !defined _DEBUG
#pragma comment(lib,"d:\\libpng\\lib\\zlib.lib")
#pragma comment(lib,"d:\\libpng\\lib\\libpng.lib")
#else
#pragma comment(lib,"d:\\libpng\\lib\\zlibD.lib")
#pragma comment(lib,"d:\\libpng\\lib\\libpngD.lib")
#endif


//bmp jpg file to dwq format

HWND g_staticText;
HWND g_progressBar;
HWND g_progressBar2;

int m_checkButtonTable[]=
{
	IDC_CHECK_TA,
	IDC_CHECK_BG,
	IDC_CHECK_EV,
	IDC_CHECK_SYS,
	IDC_CHECK_SYSTA,
	IDC_CHECK_SYSSM,
	IDC_CHECK_SYSSC,
	IDC_CHECK_SYSSMSCTA,
	IDC_CHECK_PACK,
	IDC_CHECK_64bit,
};

int m_checkPackTable[]=
{
	0,0, 1,1, 2,2, -1,-1, 3,4, 5,5, 6,6,
};


int m_radioButtonTable[]=
{
	IDC_RADIO_ONLYNEW,
	IDC_RADIO_COMPLETE,
};


char m_packDirTable[][3][16]=
{
	{"ta","",""},
	{"bg","",""},
	{"ev","",""},
	{"sys","",""},
	{"sys","ta",""},
	{"sys","sm","sm"},
	{"sys","sc","sc"},
};



HWND m_processDialog;


BOOL m_bmpFlag = FALSE;
BOOL m_jpgFlag = FALSE;
BOOL m_pngFlag = FALSE;


HWND m_check[10];
HWND m_radio[9];

HWND m_editBox;

int m_checkButtonStatus[10];
int m_radioButtonStatus[9];
int m_version = 0;

char m_versionBuffer[256];

//CCheckFileDate* m_checkDate = NULL;
CPackRoutine* m_packRoutine = NULL;

void SetCheckButton(int n);
void SetRadioButton(int n);
void GetCheckButton(int n);
void GetRadioButton(int n);

BOOL CALLBACK DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

BOOL CheckExistDir(LPSTR dir);
BOOL CheckExistDir(LPSTR srcDir0,LPSTR srcDir1,LPSTR srcDir2);
BOOL CheckAndCreateDir(LPSTR srcDir,LPSTR dstDir);


//void PackMain(LPSTR dirName,BOOL packFlag = FALSE,BOOL GameFlag = FALSE);

//int Pack1(LPSTR filename,LPSTR filename2);
//int Pack0(LPSTR filename,LPSTR filename2,LPSTR filenameonly,BOOL gameFlag = FALSE);


//char* loadBuffer = NULL;
//char* saveBuffer = NULL;


//
// IF PACKTYPE==0__
// CUT THIS 64 BYTE
// THEN REMAKE BMP_
// PACKTYPE=0______			ここはアスキーコードの0から9までの数字がはいる
//

/*
char nopackHeader[]= "\
IF PACKTYPE==0  \
CUT THIS 64 BYTE\
THEN REMAKE BMP \
PACKTYPE=0      ";
*/

char packHeader1[]= "\
IF PACKTYPE==0  \
CUT THIS 64 BYTE\
THEN REMAKE BMP \
PACKTYPE=1      ";

char packHeader2[]= "\
BMP(NORMAL)+MASK\
                \
                \
PACKTYPE=2      ";

char packHeader3[]= "\
BMP(PACK)+MASK  \
                \
                \
PACKTYPE=3      ";

char packHeader5[]= "\
JPEG            \
                \
                \
PACKTYPE=5      ";

char packHeader7[]= "\
JPEG+MASK       \
                \
                \
PACKTYPE=7      ";

FILE* list = NULL;




int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	if (1)
	{
		for (int i=0;i<10;i++)
		{
			m_checkButtonStatus[i] = 1;
		}
	}
	m_checkButtonStatus[8] = 0;
	m_checkButtonStatus[9] = 0;

	m_radioButtonStatus[0] = 0;
	m_radioButtonStatus[1] = 1;

	FILE* file = NULL;
	fopen_s(&file,"pacnyan.cfg","rb");
	if (file != NULL)
	{
		fread(m_checkButtonStatus,sizeof(int),10,file);
		fread(m_radioButtonStatus,sizeof(int),2,file);

		fclose(file);
	}


	int rt = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG_SETUP),NULL,DlgProc,(LPARAM)NULL);
	if (rt != 0) return 1;

	InitCommonControls();

//	HCURSOR cursor = LoadCursor(NULL,IDC_WAIT);
//	HCURSOR oldCursor = SetCursor(cursor);

	m_processDialog = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG_PROCESS),NULL,DlgProc2);
//	SendMessage(m_progressBar,PBM_SETPOS,30,0);
	ShowWindow(m_processDialog,SW_SHOW);

	m_packRoutine = new CPackRoutine();
	m_packRoutine->SetProgressWindow(g_staticText,g_progressBar,g_progressBar2);

	//IDC_CHECK_TA,
	//IDC_CHECK_BG,
	//IDC_CHECK_EV,
	//IDC_CHECK_SYS,
	//IDC_CHECK_SYSTA,
	//IDC_CHECK_SYSSM,
	//IDC_CHECK_SYSSC,
	//IDC_CHECK_SYSSMSCTA,

	//IDC_CHECK_PACK,

	//
	//
	///


	if (CheckExistDir("bmp")) m_bmpFlag = TRUE;
	if (CheckExistDir("jpg")) m_jpgFlag = TRUE;
	if (CheckExistDir("png")) m_pngFlag = TRUE;

	if (CheckExistDir("dwq") == FALSE)
	{
		CreateDirectory("dwq",NULL);
	}


	BOOL compFlag = FALSE;
	if (m_radioButtonStatus[1]) compFlag = TRUE;

	for (int i=0;i<7;i++)
	{
		if (m_checkButtonStatus[i])
		{
			LPSTR srcDir = m_packDirTable[i][0];
			LPSTR srcDir2 = m_packDirTable[i][1];
			LPSTR dstDir = m_packDirTable[i][2];

			BOOL taFlag = FALSE;
			if ((i == 0) || (i == 4)) taFlag = TRUE;
			
			if ((i == 5) || (i == 6))
			{
				if (m_checkButtonStatus[7]) taFlag = TRUE;
			}

			BOOL createFlag = FALSE;

			if (m_bmpFlag)
			{
				if (CheckExistDir("bmp",srcDir,srcDir2))
				{
					CheckAndCreateDir(srcDir,dstDir);

					int pos = (100*i*4+50)/28;
					SendMessage(g_progressBar,PBM_SETPOS,pos,0);

					if (m_packRoutine->CreateDWQ("bmp",srcDir,srcDir2,dstDir,taFlag,compFlag))
					{
						createFlag = TRUE;
					}
				}
			}

			if (m_jpgFlag)
			{
				if (CheckExistDir("jpg",srcDir,srcDir2))
				{
					CheckAndCreateDir(srcDir,dstDir);

					int pos = (100*(i*4+1)+50)/28;
					SendMessage(g_progressBar,PBM_SETPOS,pos,0);

					if (m_packRoutine->CreateDWQ("jpg",srcDir,srcDir2,dstDir,taFlag,compFlag))
					{
						createFlag = TRUE;
					}
				}
			}

			if (m_pngFlag)
			{
				if (CheckExistDir("png",srcDir,srcDir2))
				{
					CheckAndCreateDir(srcDir,dstDir);

					int pos = (100*(i*3+2)+50)/28;
					SendMessage(g_progressBar,PBM_SETPOS,pos,0);

					if (m_packRoutine->CreateDWQ("png",srcDir,srcDir2,dstDir,taFlag,compFlag))
					{
						createFlag = TRUE;
					}
				}
			}

			if (m_checkButtonStatus[8])
			{
				int c = m_checkPackTable[i*2];
				if (c != -1)
				{
					int c2 = m_checkPackTable[i*2+1];
					if ((m_checkButtonStatus[c]) || (m_checkButtonStatus[c2]))
					{
						int pos = (100*(i*4+3)+50)/28;
						SendMessage(g_progressBar,PBM_SETPOS,pos,0);

						if ((*dstDir) != 0)
						{
							m_packRoutine->ArcDWQ(m_checkButtonStatus[9],dstDir,m_version,dstDir);
						}
						else
						{
							m_packRoutine->ArcDWQ(m_checkButtonStatus[9],srcDir,m_version);
						}
					}
				}
			}
		}
	}





//	list = fopen("minigamefilelist.txt","wt");

//	WIN32_FIND_DATA findData;
//	char startFile[256];

//	m_packRoutine->PackInDir("bmp","bmp");
//	m_packRoutine->PackInDir("jpg","jpg");

/*	sprintf(startFile,"bmp\\*");

	HANDLE handle = FindFirstFile(startFile,&findData);
	if (handle != NULL)
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (_stricmp(startFile,"Debug") == 0)
			{
			}
			else
			{
			}
		}

		WIN32_FIND_DATA findData2;
		while (FindNextFile(handle,&findData2) != 0)
		{
			if (findData2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{

			}
		}
		FindClose(handle);
	}


	sprintf(startFile,"jpg\\*");

	HANDLE handle = FindFirstFile(startFile,&findData);
	if (handle != NULL)
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(startFile,"ta") == 0)
			{
			}
			else
			{
			}
		}

		WIN32_FIND_DATA findData2;
		while (FindNextFile(handle,&findData2) != 0)
		{
			if (findData2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{

			}
		}
		FindClose(handle);
	}




	PackMain("ev",FALSE,TRUE);
	PackMain("ta",TRUE);
///	PackMain("ed");
///	PackMain("bg");

	PackMain("sys");

//	PackMain("taisen");


	if (list != NULL) fclose(list);


	if (loadBuffer != NULL) delete [] loadBuffer;
	if (saveBuffer != NULL) delete [] saveBuffer;
*/

	if (m_packRoutine != NULL)
	{
		m_packRoutine->End();
		delete m_packRoutine;
		m_packRoutine = NULL;
	}

//	file = fopen("pacnyan.cfg","wb");
	fopen_s(&file,"pacnyan.cfg","wb");

	if (file != NULL)
	{
		fwrite(m_checkButtonStatus,sizeof(int),10,file);
		fwrite(m_radioButtonStatus,sizeof(int),2,file);
		fclose(file);
	}

//	SetCursor(oldCursor);

	DestroyWindow(m_processDialog);
	MessageBox(NULL,"おわりましたにゃ","End",MB_OK);

	return 0;
}


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



/*
void PackMain(LPSTR dirName,BOOL packFlag,BOOL gameFlag)
{
	char startFile[256];


	int type = 1;

#if defined __DIANA_PACK_MESSEVERSION__
	type = 2;
#endif

#if defined __DIANA_PACK_NETVERSION__
	type = 3;
#endif

if (type == 1)
{
	sprintf(startFile,"bmp\\%s\\*.bmp",dirName);
}
else if (type == 2)
{
	sprintf(startFile,"bmptaiken\\%s\\*.bmp",dirName);
}
else
{
	sprintf(startFile,"bmpnet\\%s\\*.bmp",dirName);
}

	WIN32_FIND_DATA findData;
//	HANDLE handle = FindFirstFile("bmp\\ev\\*.bmp",&findData);
	HANDLE handle = FindFirstFile(startFile,&findData);


	if (handle != NULL)
	{
		char filenameonly[256];
		for (int i=0;i<16;i++)
		{
			char c = findData.cFileName[i];
			filenameonly[i] = c;
			if (c == '.')
			{
				filenameonly[i] = 0;
				break;
			}
		}

	if (strcmp(dirName,"sys") == 0)
	{
		if (strcmp(filenameonly,"ev00_99") == 0)
		{
			int hhh = 0;
			hhh++;
		}
	}

		if (m_checkDate->CheckNeedUpdate(dirName,filenameonly,type))
		{
			char filename[256];
			char filename2[256];

if (type == 1)
{
			sprintf(filename,"bmp\\%s\\%s.bmp",dirName,filenameonly);
			sprintf(filename2,"dwq\\%s\\%s.dwq",dirName,filenameonly);
}
else if (type == 2)
{
			sprintf(filename,"bmptaiken\\%s\\%s.bmp",dirName,filenameonly);
			sprintf(filename2,"dwqtaiken\\%s\\%s.dwq",dirName,filenameonly);
}
else
{
			sprintf(filename,"bmpnet\\%s\\%s.bmp",dirName,filenameonly);
			sprintf(filename2,"dwqnet\\%s\\%s.dwq",dirName,filenameonly);
}
	//			Pack1("..\\tokittyRelease\\bmp\\ta11.bmp","..\\tokittyRelease\\dwq\\ta11.dwq");

			if(packFlag)
			{
				Pack1(filename,filename2);
			}
			else
			{
				Pack0(filename,filename2,filenameonly,gameFlag);
			}
		}
	}







	if (handle != INVALID_HANDLE_VALUE)
	{
		WIN32_FIND_DATA findData2;

		while (FindNextFile(handle,&findData2) != 0)
		{
			char filenameonly[256];
			for (int i=0;i<16;i++)
			{
				char c = findData2.cFileName[i];
				filenameonly[i] = c;
				if (c == '.')
				{
					filenameonly[i] = 0;
					break;
				}
			}

	if (strcmp(dirName,"sys") == 0)
	{
		if (strcmp(filenameonly,"ev00_99") == 0)
		{
			int hhh = 0;
			hhh++;
		}
	}

			if (m_checkDate->CheckNeedUpdate(dirName,filenameonly,type))
			{
				char filename[256];
				char filename2[256];
if (type == 1)
{
				sprintf(filename,"bmp\\%s\\%s.bmp",dirName,filenameonly);
				sprintf(filename2,"dwq\\%s\\%s.dwq",dirName,filenameonly);
}
else if (type == 2)
{
				sprintf(filename,"bmptaiken\\%s\\%s.bmp",dirName,filenameonly);
				sprintf(filename2,"dwqtaiken\\%s\\%s.dwq",dirName,filenameonly);
}
else
{
				sprintf(filename,"bmpnet\\%s\\%s.bmp",dirName,filenameonly);
				sprintf(filename2,"dwqnet\\%s\\%s.dwq",dirName,filenameonly);
}

				if(packFlag)
				{
					Pack1(filename,filename2);
				}
				else
				{
					Pack0(filename,filename2,filenameonly,gameFlag);
				}
			}

		}

		FindClose(handle);
	}
}

*/

/*

int Pack1(LPSTR filename,LPSTR filename2)
{
	int i,j;


	FILE* file = fopen(filename,"rb");

	int sx,sy,dy,startY,readX;

	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;
	RGBQUAD rgbquad[256];

	BOOL bError = TRUE;
	if (file != NULL)
	{
		if (fread(&bmf,sizeof(BITMAPFILEHEADER),1,file) == 1)
		{
			if (fread(&bmi,sizeof(BITMAPINFOHEADER),1,file) == 1)
			{
				//load palette
				ZeroMemory(&rgbquad,sizeof(RGBQUAD)*256);
				if (bmi.biBitCount == 8)
				{
					int p = bmi.biClrUsed;
					if (p == 0) p = 256;
					bmi.biClrUsed = p;

					fread(&rgbquad,sizeof(RGBQUAD),p,file);

					sx = bmi.biWidth;
					sy = bmi.biHeight;
					dy = -1;
					startY = sy - 1;

					if (sy<0)
					{
						sy = -sy;
						dy = -dy;
						startY = 0;
					}


if ((sx == 639) || (sx==638) || (sx==637) || (sy==481) || (sy==482) || (sy==479) || (sy==478))
{
	char errmes[256];
	sprintf(errmes,"サイズがあやしいにゃ :%s",filename);
	MessageBox(NULL,errmes,"warning",MB_OK);
}
					readX = (sx+3) & (~3);

					for (i=0;i<readX;i++)
					{
						loadBuffer[i] = 0;
					}

					int y = startY;

					y++;

					for (j=0;j<sy;j++)
					{
						fread(&loadBuffer[y*readX],sizeof(char),readX,file);
						y += dy;
					}
					
					bError = FALSE;
				}
			}
		}
		fclose(file);
	}

	if (bError)
	{
		char mes[256];
		sprintf(mes,"Load Error File=%s",filename);
		MessageBox(NULL,mes,"Error",MB_OK);
		return -1;
	}



	FILE* file2 = fopen(filename2,"wb");
	if (file2 == NULL)
	{
		MessageBox(NULL,"Save Open Error","Error",MB_OK);
		return -2;
	}



	fwrite(packHeader1,sizeof(char),64,file2);
	fwrite(&bmf,sizeof(bmf),1,file2);
	fwrite(&bmi,sizeof(bmi),1,file2);
	int bc = bmi.biClrUsed;
	if (bc == 0) bc = 256;
	fwrite(&rgbquad,sizeof(RGBQUAD),bc,file2);

	//pack main
	//xor
	for (j=sy;j>=1;j--)
	{
		for (i=0;i<sx;i++)
		{
			char c = loadBuffer[j*readX+i];
			c ^= loadBuffer[(j-1)*readX+i];
			loadBuffer[j*readX+i] = c;
		}
	}

	int savePtr = 0;
	//set header

	//pack
	for (j=0;j<sy;j++)
	{
		int pt = 0;
		int ln = 0;
		while (pt<sx)
		{
			char c = loadBuffer[(j+1)*readX+pt];
			pt++;

			if (c != 0)
			{
				if (ln>0)
				{
					saveBuffer[savePtr] = 0;
					savePtr++;
					saveBuffer[savePtr] = ln;
					savePtr++;
					ln=0;
				}


				saveBuffer[savePtr] = c;
				savePtr++;
				ln = 0;
			}
			else
			{
				ln++;
				if ((ln>=255) || (pt>=sx))
				{
					saveBuffer[savePtr] = 0;
					savePtr++;
					saveBuffer[savePtr] = ln;
					savePtr++;
					ln=0;
				}
			}
		}
	}


	fwrite(saveBuffer,sizeof(char),savePtr,file2);

	fclose(file2);

//	char mes[256];
//	sprintf(mes,"file size=%d",savePtr + sizeof(bmf) + sizeof(bmi) + sizeof(rgbquad));
//	MessageBox(NULL,mes,"Size",MB_OK);


	return 0;	//unknown error
}




int Pack0(LPSTR filename,LPSTR filename2,LPSTR filenameonly,BOOL gameFlag)
{
	FILE* file = fopen(filename,"rb");

	int sx,sy,dy,startY,readX;

	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;
	RGBQUAD rgbquad[256];

	BOOL bError = TRUE;
	if (file != NULL)
	{
		if (fread(&bmf,sizeof(BITMAPFILEHEADER),1,file) == 1)
		{
			if (fread(&bmi,sizeof(BITMAPINFOHEADER),1,file) == 1)
			{
				//load palette
				ZeroMemory(&rgbquad,sizeof(RGBQUAD)*256);
				if (bmi.biBitCount == 8)
				{
					int p = bmi.biClrUsed;
					if (p == 0) p = 256;
					bmi.biClrUsed = p;

					fread(&rgbquad,sizeof(RGBQUAD),p,file);

					sx = bmi.biWidth;
					sy = bmi.biHeight;
					dy = -1;
					startY = sy - 1;

					if (sy<0)
					{
						sy = -sy;
						dy = -dy;
						startY = 0;
					}

if ((sx == 639) || (sx==638) || (sx==637) || (sy==481) || (sy==482) || (sy==479) || (sy==478))
{
	char errmes[256];
	sprintf(errmes,"サイズがあやしいにゃ :%s",filename);
	MessageBox(NULL,errmes,"warning",MB_OK);
}

					readX = (sx+3) & (~3);

					if (startY == 0)
					{
						fread(loadBuffer,sizeof(char),readX*sy,file);
					}
					else
					{
						for (int j=0;j<sy;j++)
						{
							fread(&loadBuffer[readX * (startY - j)],sizeof(char),readX,file);
						}
					}
					
					bError = FALSE;
				}
			}
		}
		fclose(file);
	}


	if (gameFlag)
	{
		if (bmi.biWidth >= 640)
		{
			if ((bmi.biHeight >= 480) || (bmi.biHeight <= -480))
			{
				static int cnt = 0;

				char mes00[256];
				sprintf(mes00,"\"%s\",",filenameonly);

				fwrite(mes00,sizeof(char),strlen(mes00),list);

				cnt++;
				if ((cnt % 8) == 0)
				{
					fwrite("\n",sizeof(char),strlen("\n"),list);
				}
			}
		}
	}




	if (bError)
	{
		char mes[256];
		sprintf(mes,"Load Error File=%s",filename);
		MessageBox(NULL,mes,"Error",MB_OK);
		return -1;
	}



	FILE* file2 = fopen(filename2,"wb");
	if (file2 == NULL)
	{
		MessageBox(NULL,"Save Open Error","Error",MB_OK);
		return -2;
	}



	fwrite(nopackHeader,sizeof(char),64,file2);
	fwrite(&bmf,sizeof(bmf),1,file2);
	fwrite(&bmi,sizeof(bmi),1,file2);
	int bc = bmi.biClrUsed;
	if (bc == 0) bc = 256;
	fwrite(&rgbquad,sizeof(RGBQUAD),bc,file2);

	fwrite(loadBuffer,sizeof(char),readX*sy,file);

	fclose(file2);

	return 0;	//unknown error
}

*/



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
		g_staticText = GetDlgItem(hWnd,IDC_MYSTATIC);
		g_progressBar = GetDlgItem(hWnd,IDC_PROGRESS1);
		g_progressBar2 = GetDlgItem(hWnd,IDC_PROGRESS2);

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
		for (i=0;i<10;i++)
		{
			m_check[i] = GetDlgItem(hWnd,m_checkButtonTable[i]);
			SetCheckButton(i);
		}

		for (i=0;i<9;i++)
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
			for (i=0;i<10;i++)
			{
				GetCheckButton(i);
			}
			for (i=0;i<2;i++)
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



/*_*/



