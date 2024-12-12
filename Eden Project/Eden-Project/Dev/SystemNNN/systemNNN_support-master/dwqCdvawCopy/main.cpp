//
// main.cpp
//


#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <commctrl.h>

#include "resource.h"

#include "..\..\systemNNN\nnnUtilLib\getCommandLineParam.h"

#pragma comment(lib,"Comctl32.lib")



HWND globalHWND = NULL;

char m_tmp[768];
int Pack(void);

int m_a[256];


//pack format
//int kosuu
//int filetable offset=16
//int filenamedata offset
//int packdataoffset
//

//
//filetable
//{
//	nameoffset(fromfilenamedata offset)
//  dataoffset(from packdataoffset)
//}
//


//data
// int type	0:nopack 1:pack
// int packedsize
// int makedsize
// int 0
// data...
//


int m_fileKosuu = 0;
char* m_filenameBuffer = NULL;
int m_filenameBufferSize = 0;
int m_filenamePointer = 0;

int* m_filenameTable = NULL;
int m_filenameTableKosuu = 0;

int* m_fileSetTable = NULL;
int m_fileSetTableKosuu = 0;


char* m_readBuffer = NULL;
int m_readBufferSize = 0;

char* m_writeBuffer = NULL;
int m_writeBufferSize = 0;

char* m_packedDataBuffer = NULL;
int m_packedDataBufferSize = 0;

int m_dataOffset = 0;

int m_returnCode = 0;
int m_returnParam = 0;

int m_progData1 = 0;
int m_progData2 = 0;

//void CreateDummyFile(LPSTR dirName);

int m_errorFlag;


unsigned int __stdcall MyThread(void* arg);
void SendEventMessage(int n);

char m_copyMessage[1024];

//
BOOL UnpackSource(LPSTR name);
BOOL CheckExistSource(LPSTR sourceFolder,LPSTR name);
BOOL CheckExistSourceDir(LPSTR sourceFolder,LPSTR dirName);

BOOL UnpackNyp(LPSTR sourceFolder,LPSTR targetFolder,LPSTR name);
BOOL CopyMyFolder(LPSTR sourceFolder,LPSTR targetFolder,LPSTR name);

BOOL NonPackRestore(LPSTR targetFileName,FILE* srcFile,int packedSize,int makedSize);
BOOL PackRestore(LPSTR targetFileName,FILE* srcFile,int packedSize,int makedSize);

void SetCopyMessage(LPSTR filename);
void SetProgBar(int prog1,int prog2);




BOOL CALLBACK DlgProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

HWND m_processDialog = NULL;

HWND g_staticText0 = NULL;
HWND g_staticText = NULL;
HWND g_progressBar = NULL;
HWND g_progressBar2 = NULL;

int m_globalProgBase = 0;
int m_globalProgDelta = 50;

char m_buff1[1024];
char m_buff2[1024];
char m_buff3[1024];


LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

HBITMAP m_hBitmap = NULL;


int picSizeX = 80;
int picSizeY = 80;

HANDLE m_event[2];

//void ProcLoop(void);

int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	char cur[1024];
	GetCurrentDirectory(1024,cur);




//	char buff1[1024];
//	char buff2[1024];
//	char buff3[1024];
	m_buff1[0] = 0;
	m_buff2[0] = 0;
	m_buff3[0] = 0;

	char* lplpBuff[3];

	lplpBuff[0] = m_buff1;
	lplpBuff[1] = m_buff2;
	lplpBuff[2] = m_buff3;

	int cmd = 0;
#if !defined _DEBUG
	cmd = CGetCommandLineParam::GetParam(3,1024,lplpBuff,lpCmdLine);
#else
	cmd = 3;
	memcpy(m_buff1,"Install",sizeof("Install")+1);
	memcpy(m_buff2,"testSource\\",sizeof("testSource\\")+1);
	memcpy(m_buff3,"testTarget",sizeof("testTarget")+1);
#endif





	m_hBitmap = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP1));

    WNDCLASS wndclass;

    wndclass.style         = CS_NOCLOSE;
	wndclass.lpfnWndProc   = MainWndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
    wndclass.hCursor       = NULL;
    wndclass.hbrBackground = NULL;
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = "dwqCdvawCopyClass";
    RegisterClass(&wndclass);


	DWORD dwExStyle = WS_EX_TOPMOST;
//	DWORD dwExStyle = 0;


	int desktopWindowSizeX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int desktopWindowSizeY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	int desktopWindowStartX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int desktopWindowStartY = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int centerX = desktopWindowStartX + desktopWindowSizeX / 2;
	int centerY = desktopWindowStartY + desktopWindowSizeY / 2;


	HWND hWnd = CreateWindowEx(dwExStyle ,wndclass.lpszClassName,"NekoMeido",
								
								
								WS_POPUP |
								WS_VISIBLE ,

								centerX - picSizeX/2, centerY - picSizeY / 2,
								picSizeX,picSizeY,
								NULL, NULL, hInstance, NULL );

//	ProcLoop();

	SetForegroundWindow(hWnd);

//	ProcLoop();


	BOOL flg = FALSE;
	if (cmd < 3)
	{
		char mes[1024];
		wsprintf(mes,"%s\n個数=%d\n1:%s\n2:%s",lpCmdLine,cmd,m_buff1,m_buff2,m_buff3);

		MessageBox(hWnd,mes,"Error:パラメータが足りません",MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		if (_stricmp(m_buff1,"install") != 0)
		{
			MessageBox(hWnd,"1つめのパラメーターが/Installになっていません","Error",MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			flg = TRUE;
		}
	}


	if (flg == FALSE)
	{
		DestroyWindow(hWnd);
		return 0;
	}



	//いらない
	if (strlen(m_buff2) == 2)
	{
		if (m_buff2[1] == ':')
		{
			m_buff2[2] = '\\';
			m_buff2[3] = 0;
		}
	}

	int tln = strlen(m_buff3);
	if (tln < 1022)
	{
		m_buff3[tln] = '\\';
		m_buff3[tln+1] = 0;
	}

	m_readBufferSize = 1024*1024*9;
	m_readBuffer = new char[m_readBufferSize];

	m_writeBufferSize = 1024*1024*9;
	m_writeBuffer = new char[m_writeBufferSize];

//	BOOL errorFlag = TRUE;


	m_processDialog = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG_PROCESS),hWnd,DlgProc2);
	ShowWindow(m_processDialog,SW_SHOW);

	SetWindowText(g_staticText0,"ファイルの最終コピー中です");
	UpdateWindow(g_staticText0);

	SetForegroundWindow(m_processDialog);

	m_event[0] = CreateEvent(NULL,FALSE,FALSE,FALSE);
	m_event[1] = CreateEvent(NULL,FALSE,FALSE,FALSE);

	unsigned int threadID;
	_beginthreadex(NULL,0,MyThread,NULL,0,&threadID);


	int errorFlag = FALSE;


	MSG msg;
	while (1)
	{
		if ( PeekMessage(&msg,NULL,0,0,PM_NOREMOVE) )
		{
			int bRet = GetMessage(&msg,NULL,0,0);
			if ((bRet != 0) && (bRet != -1))
			{
				if ((m_processDialog == NULL) || (!IsDialogMessage(m_processDialog,&msg)))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		else
		{
			int rt = WaitForSingleObject(m_event[0],0);
			if (rt == WAIT_OBJECT_0)
			{
				if (m_returnCode == 0)
				{
					errorFlag = m_errorFlag;
					SetEvent(m_event[1]);
					break;
				}
				else if (m_returnCode == 1)
				{
					SetWindowText(g_staticText,m_copyMessage);
					UpdateWindow(g_staticText);
				}
				else if (m_returnCode == 2)
				{
					if (m_progData1 >= 0)
					{
						SendMessage(g_progressBar,PBM_SETPOS,m_progData1,0);
						UpdateWindow(g_progressBar);
					}

					if (m_progData2 >= 0)
					{
						SendMessage(g_progressBar2,PBM_SETPOS,m_progData2,0);
						UpdateWindow(g_progressBar2);
					}
				}
				else if (m_returnCode == 3)
				{
					UpdateWindow(m_processDialog);

				}
				SetEvent(m_event[1]);
			}
		}
		Sleep(1);
	}


/*
	if (UnpackSource("dwq"))
	{
		if (UnpackSource("cdvaw"))
		{
			errorFlag = FALSE;
		}
	}
*/




	CloseHandle(m_event[1]);
	CloseHandle(m_event[0]);

	DestroyWindow(m_processDialog);


	if (m_writeBuffer != NULL) delete [] m_writeBuffer;
	if (m_readBuffer != NULL) delete [] m_readBuffer;

	DestroyWindow(hWnd);

	if (errorFlag) return 1;
	return 0;
}


void SendEventMessage(int n)
{
	m_returnCode = n;
	SetEvent(m_event[0]);
	WaitForSingleObject(m_event[1],10000);
	ResetEvent(m_event[0]);
	ResetEvent(m_event[1]);
}

unsigned int __stdcall MyThread(void* arg)
{
	BOOL errorFlag;

	if (UnpackSource("dwq"))
	{
		if (UnpackSource("cdvaw"))
		{
			errorFlag = FALSE;
		}
	}

	SendEventMessage(0);


	m_errorFlag = errorFlag;
	return 0;
	return errorFlag;
}

BOOL UnpackSource(LPSTR name)
{
	SendEventMessage(3);

//	UpdateWindow(m_processDialog);

	if (CheckExistSource(m_buff2,name))
	{
		if (UnpackNyp(m_buff2,m_buff3,name) == FALSE) return FALSE;

	}
	else if (CheckExistSourceDir(m_buff2,name))
	{
		if (CopyMyFolder(m_buff2,m_buff3,name) == FALSE) return FALSE;
	}
	else
	{
		//no data
	}

	m_globalProgBase += m_globalProgDelta;

	return TRUE;
}

BOOL CheckExistSource(LPSTR sourceFolder,LPSTR name)
{
	FILE* file = NULL;
	char filename[1024];
	wsprintf(filename,"%s%s.nyp",sourceFolder,name);
	fopen_s(&file,filename,"rb");
	if (file != NULL)
	{
		fclose(file);
		return TRUE;
	}

	return FALSE;
}



BOOL CheckExistSourceDir(LPSTR sourceFolder,LPSTR dirName)
{
	char filename[1024];
	wsprintf(filename,"%s%s",sourceFolder,dirName);
	int at = GetFileAttributes(filename);
	if (at == -1) return FALSE;

	if (at & FILE_ATTRIBUTE_DIRECTORY) return TRUE;

	return FALSE;
}


BOOL UnpackNyp(LPSTR sourceFolder,LPSTR targetFolder,LPSTR name)
{
	FILE* file = NULL;
	char sourceFile[1024];
	wsprintf(sourceFile,"%s%s.nyp",sourceFolder,name);
	fopen_s(&file,sourceFile,"rb");
	if (file == NULL)
	{
		return FALSE;
	}

	int kosuu = 0;
	fread(&kosuu,sizeof(int),1,file);
	if (kosuu == 0)
	{
		fclose(file);
		return TRUE;
	}
	int fileTableOffset = 0;
	fread(&fileTableOffset,sizeof(int),1,file);
	int filenameDataOffset = 0;
	fread(&filenameDataOffset,sizeof(int),1,file);
	int packDataOffset = 0;
	fread(&packDataOffset,sizeof(int),1,file);

	//table

	int prog1 = 0;

	for (int i=0;i<kosuu;i++)
	{

		int sk = fileTableOffset + i * 8;
		fseek(file,sk,SEEK_SET);
		int nameOffset = 0;
		int dataOffset = 0;
		fread(&nameOffset,sizeof(int),1,file);
		fread(&dataOffset,sizeof(int),1,file);

		int sk2 = filenameDataOffset + nameOffset;
		fseek(file,sk2,SEEK_SET);
		char fname[1024];
		fread(fname,sizeof(char),1024,file);
		fname[1022] = 0;
		fname[1023] = 0;

		int sk3 = packDataOffset + dataOffset;
		fseek(file,sk3,SEEK_SET);
		
		//load and unpack

		int packType = 0;
		fread(&packType,sizeof(int),1,file);
		int packedSize = 0;
		fread(&packedSize,sizeof(int),1,file);
		int makedSize = 0;
		fread(&makedSize,sizeof(int),1,file);
		int dummy = 0;
		fread(&dummy,sizeof(int),1,file);

		char targetFileName[1024];
		wsprintf(targetFileName,"%s%s\\%s",targetFolder,name,fname);

		SetCopyMessage(fname);

		prog1 = m_globalProgBase + ((i+1) * m_globalProgDelta) / kosuu;
		SetProgBar(prog1,0);

		if (packType == 0)
		{
			if (NonPackRestore(targetFileName,file,packedSize,makedSize) == FALSE)
			{
				fclose(file);
				return FALSE;
			}
		}
		else//1
		{
			if (PackRestore(targetFileName,file,packedSize,makedSize) == FALSE)
			{
				fclose(file);
				return FALSE;
			}
		}
	}

	fclose(file);

	return TRUE;
}


BOOL NonPackRestore(LPSTR targetFileName,FILE* srcFile,int packedSize,int makedSize)
{
	int writeSize = 0;

	FILE* file = NULL;
	fopen_s(&file,targetFileName,"wb");
	if (file == NULL)
	{
		MessageBox(NULL,targetFileName,"Write Error",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	int nokori = packedSize;
	int prog2 = 0;
	int prog2Delta = (makedSize*200) / m_readBufferSize;
	if (prog2Delta > 99) prog2Delta = 99;
	if (prog2Delta < 10) prog2Delta = 10;

	while (nokori > 0)
	{
		int loadSize = nokori;
		if (loadSize > m_readBufferSize)
		{
			loadSize = m_readBufferSize;
		}

		prog2 += prog2Delta;
		prog2 %= 100;
		SetProgBar(-1,prog2);

		fread(m_readBuffer,sizeof(char),loadSize,srcFile);
		if (fwrite(m_readBuffer,sizeof(char),loadSize,file) < sizeof(char)*loadSize)
		{
			fclose(file);
			return FALSE;
		}

//		ProcLoop();

//		writeSize += loadSize;
		nokori -= loadSize;
	}

	fclose(file);
	return TRUE;
}

BOOL PackRestore(LPSTR targetFileName,FILE* srcFile,int packedSize,int makedSize)
{
	int writeSize = 0;

	FILE* file = NULL;
	fopen_s(&file,targetFileName,"wb");
	if (file == NULL)
	{
		int er = errno;
		char emes[1024];
		wsprintf(emes,"Write Error unpack (%d)",er);
		MessageBox(NULL,targetFileName,emes,MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	unsigned char bitPattern8[8];
	
	for (int i=0;i<7;i++)
	{
		bitPattern8[i] = 1 << i;
	}
	bitPattern8[7] = 0x80;

	int nokori = packedSize;
	int bufferDataSize = 0;
	int bufferDataPointer = 0;
	int writeBufferSize = 0;
	int wroteSize = 0;

	int prog1 = 0;
	int prog2 = 0;
	int prog2Delta = (makedSize*200) / m_writeBufferSize;
	if (prog2Delta > 99) prog2Delta = 99;
	if (prog2Delta < 10) prog2Delta = 10;

	while (nokori > 0)
	{
		//read buffer
		int loadSize = nokori;
		if (loadSize+(bufferDataPointer + bufferDataSize) > m_readBufferSize)
		{
			loadSize = m_readBufferSize - (bufferDataPointer + bufferDataSize);
			if (loadSize < 0) loadSize = 0;
		}

		if (loadSize > 0)
		{
			int rd = fread(m_readBuffer+(bufferDataPointer + bufferDataSize),sizeof(char),loadSize,srcFile);
			nokori -= rd;
			bufferDataSize += rd;
		}

		while ((bufferDataSize > 1024*64) || ((bufferDataSize > 0) && (nokori == 0)))
		{
			//block unpack

			char packFlag = *(m_readBuffer+bufferDataPointer);
			bufferDataPointer += sizeof(char);
			bufferDataSize -= sizeof(char);

			for (int n=0;n<3;n++)
			{
				if (packFlag & (1<<n))
				{
					char tmp[256];

					char start = *(m_readBuffer+bufferDataPointer);
					bufferDataPointer += sizeof(char);
					bufferDataSize -= sizeof(char);

					int pack3Size = (int)(*(m_readBuffer+bufferDataPointer));
					pack3Size &= 0xff;
					bufferDataPointer += sizeof(char);
					bufferDataSize -= sizeof(char);

					char* zeroMaskWork = m_readBuffer+bufferDataPointer;
					bufferDataPointer += sizeof(char) * 32;
					bufferDataSize -= sizeof(char) * 32;

					char* pack3Work = m_readBuffer+bufferDataPointer;
					bufferDataPointer += sizeof(char) * pack3Size;
					bufferDataSize -= sizeof(char) * pack3Size;

					char* pack8Work = m_readBuffer+bufferDataPointer;
					int pack8Used = 0;

					int zeroMaskPtr = 0;
					char zeroMask = 0;
					int shift3Data = 0;
					int shift3Size = 0;
					int shift3Pointer = 0;

					zeroMask = *(zeroMaskWork + zeroMaskPtr);

					tmp[0] = start;

					for (int i=1;i<256;i++)
					{
						if ((i & 7) == 0)
						{
							zeroMask = *(zeroMaskWork + zeroMaskPtr);
						}

						char dt = 0;

						if (zeroMask & bitPattern8[i & 7])
						{
							if (shift3Size < 3)
							{
								int c = (int)(*(pack3Work+shift3Pointer));
								c &= 0xff;
								c <<= shift3Size;

								shift3Data |= c;
								shift3Size += 8;

								shift3Pointer++;
							}

							int ck = shift3Data & 7;
							if (ck == 0)
							{
								dt = *(pack8Work+pack8Used);
								pack8Used++;
							}
							else
							{
								if (ck & 0x04)
								{
									ck |= 0xfffffff8;
								}
								dt = (char)ck;
							}

							shift3Data >>= 3;
							shift3Size -= 3;
						}

						start += dt;
						tmp[i] = start;

						if ((i & 7) == 7)
						{
							zeroMaskPtr++;
						}
					}

					for (int k=0;k<256;k++)
					{
						*(m_writeBuffer + writeBufferSize + k*3+n) = tmp[k];
					}

					bufferDataPointer += pack8Used;
					bufferDataSize -= pack8Used;
				}
				else
				{
					//nopack
					for (int k=0;k<256;k++)
					{
						*(m_writeBuffer + writeBufferSize + k*3+n) = *(m_readBuffer + bufferDataPointer + k);
					}
					bufferDataPointer += sizeof(char) * 256;
					bufferDataSize -= sizeof(char) * 256;
				}

			}

			writeBufferSize += 256*3;


			//if write buffer > xxx write
			if (writeBufferSize > m_writeBufferSize - 256*3*2)
			{
				int sz = writeBufferSize;
				if ((sz + wroteSize) > makedSize)
				{
					sz = makedSize - wroteSize;
				}
				if (sz > 0)
				{
					prog2 += prog2Delta;
					prog2 %= 100;
					SetProgBar(-1,prog2);

					if (fwrite(m_writeBuffer,sizeof(char),sz,file) < sizeof(char)*sz)
					{
						fclose(file);
						return FALSE;
					}
					wroteSize += sz;
				}



char mes[1024];
wsprintf(mes,"\n残り=%d 書き込み=%d",nokori/1024/1024,wroteSize/1024/1024);
OutputDebugString(mes);



				writeBufferSize = 0;
			}

		}

		if ((bufferDataSize <= 0) && (nokori <= 0))
		{
			break;
		}


		//move buff
		if ((bufferDataPointer > 0) && (bufferDataSize > 0))
		{
			memcpy(m_readBuffer,m_readBuffer+bufferDataPointer,bufferDataSize);
		}
		bufferDataPointer = 0;

//		ProcLoop();
	}

	if (writeBufferSize > 0)
	{
		int sz = writeBufferSize;
		if ((sz + wroteSize) > makedSize)
		{
			sz = makedSize - wroteSize;
		}
		if (sz > 0)
		{
			prog2 += prog2Delta;
			prog2 %= 100;
			SetProgBar(-1,prog2);

			if (fwrite(m_writeBuffer,sizeof(char),sz,file) < sizeof(char)*sz)
			{
				fclose(file);
				return FALSE;
			}
			wroteSize += sz;
		}
	//	fwrite(m_writeBuffer,sizeof(char),writeBufferSize,file);
	}

	fclose(file);

	return TRUE;
}


BOOL CopyMyFolder(LPSTR sourceFolder,LPSTR targetFolder,LPSTR name)
{
	WIN32_FIND_DATA find;
	char searchName[1024];
	wsprintf(searchName,"%s%s\\*.*",sourceFolder,name);

	HANDLE handle = FindFirstFile(searchName,&find);
	if (handle == INVALID_HANDLE_VALUE) return TRUE;

	//count
	int fileKosuu = 0;
	while (1)
	{
		if ((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			fileKosuu++;
		}
		if (FindNextFile(handle,&find) == 0) break;
	}

	FindClose(handle);


	//re-open
	handle = FindFirstFile(searchName,&find);
	if (handle == INVALID_HANDLE_VALUE) return TRUE;

	int prog1 = 0;
	int prog2 = 0;
	int dv = fileKosuu;
	if (dv<1) dv = 1;

	int fileCount = 0;

	int prog2Delta = 200 / dv;
	if (prog2Delta > 99) prog2Delta = 99;
	if (prog2Delta < 10) prog2Delta = 10;

	while (1)
	{
		LPSTR cFileName = find.cFileName;
		if ((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
//			ProcLoop();

			SetCopyMessage(cFileName);
			
			char srcFile[1024];
			char dstFile[1024];

			wsprintf(srcFile,"%s%s\\%s",sourceFolder,name,cFileName);
			wsprintf(dstFile,"%s%s\\%s",targetFolder,name,cFileName);

			prog2 += prog2Delta;
			prog2 %= 100;

			fileCount++;

			prog1 = m_globalProgBase + (fileCount * m_globalProgDelta) / dv;

			SetProgBar(prog1,prog2);

			if (CopyFile(srcFile,dstFile,FALSE) == FALSE)
			{
				FindClose(handle);
				return FALSE;
			}


			SetFileAttributes(dstFile,FILE_ATTRIBUTE_NORMAL);
			//SetFileAttributes()で変更できるのか?



		}

		if (FindNextFile(handle,&find) == 0) break;
	}

	FindClose(handle);

	return TRUE;
}



void SetCopyMessage(LPSTR filename)
{
	OutputDebugString("\nCopying...:");
	OutputDebugString(filename);

	memcpy(m_copyMessage,filename,strlen(filename)+1);
	SendEventMessage(1);



//	SetWindowText(g_staticText,filename);
//	UpdateWindow(g_staticText);
}








BOOL CALLBACK DlgProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		g_staticText0 = GetDlgItem(hWnd,IDC_MYSTATIC0);
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

void SetProgBar(int prog1,int prog2)
{
	m_progData1 = prog1;
	m_progData2 = prog2;
	SendEventMessage(2);



/*
	if (prog1 >= 0)
	{
		SendMessage(g_progressBar,PBM_SETPOS,prog1,0);
		UpdateWindow(g_progressBar);
	}

	if (prog2 >= 0)
	{
		SendMessage(g_progressBar2,PBM_SETPOS,prog2,0);
		UpdateWindow(g_progressBar2);
	}
*/

}


LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (message == WM_DESTROY)
	{
		ShowWindow(hWnd,SW_HIDE);
		PostQuitMessage(0);
		return 0;
	}

	if (message == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		HDC src;
		HGDIOBJ old;

		hdc = BeginPaint(hWnd,&ps);

		src = CreateCompatibleDC(hdc);
		old = SelectObject(src,m_hBitmap);
		BitBlt(hdc,0,0,picSizeX,picSizeY,src,0,0,SRCCOPY);
		SelectObject(src,old);
		DeleteDC(src);

		EndPaint(hWnd,&ps);
		return TRUE;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

} /* MainWndproc */

/*
void ProcLoop(void)
{
	MSG msg;
	while (1)
	{
		if ( PeekMessage(&msg,NULL,0,0,PM_NOREMOVE) )
		{
			int bRet = GetMessage(&msg,NULL,0,0);

			if ((bRet == 0) || (bRet == -1))
			{
				break;
			}


			if ((m_processDialog == NULL) || (!IsDialogMessage(m_processDialog,&msg)))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			break;
		}
	}
	Sleep(0);
}
*/


/*_*/


