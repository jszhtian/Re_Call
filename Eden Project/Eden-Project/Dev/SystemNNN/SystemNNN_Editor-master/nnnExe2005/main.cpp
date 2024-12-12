//
// main.cpp
//


#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "..\..\systemNNN\nnnUtilLib\commongameVersion.h"

#include "..\..\systemNNN\nnnLib\mainControl.h"
#include "..\..\systemNNN\nnnLib\gameCallBack.h"

#include "..\..\systemNNN\nnnUtilLib\currentDirControl.h"

#include "game.h"
#include "main.h"


#include "..\..\systemNNN\nyanlib\include\libinclude.h"

#if !defined _DEBUG
#pragma comment(lib,"d:\\libpng\\lib\\zlib.lib")

#pragma comment(lib,"d:\\libpng\\lib\\libpng.lib")
#else
#pragma comment(lib,"d:\\libpng\\lib\\zlibD.lib")
#pragma comment(lib,"d:\\libpng\\lib\\libpngD.lib")
#endif





//#define __DEMO_VERSION__

int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	CCurrentDirControl::ChangeCurrentToExe();

	//
	// �K�v�Ȃ炱���ő̌��Ńt���O�Ȃǂ����Ă�
	//
//	CCommonGameVersion::SetTaikenVersion();
//	CCommonGameVersion::SetLoadOk();
//	CCommonGameVersion::SetSaveOk();

	//
	// �K�v�Ȃ炱���œƎ���wheelMouse,systemfile���쐬����
	//
	m_game = NULL;
//	m_wheelMouse = new CWheelMouse();
//	m_systemFile = new CCommonSystemFile();
//	m_systemFile->Load(TRUE);


	m_mainControl = new CMainControl();	//�R���X�g���N�^��wheelMouse,systemfile���w��ł��� �w��Ȃ��A�܂���NULL���Ɠ����ŕW���̂��쐬����

	if (m_mainControl->CheckSystem())
	{
		HICON icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		HWND hWnd = m_mainControl->CreateWindowRoutine(hInstance,icon);//hinstance,icon,proc    proc == NULL����CMainControl�����̕W��callBack

		CCommonSystemFile* lpSystemFile = m_mainControl->GetSystemFile();
		SIZE sz = m_mainControl->GetWindowSize();
		int bpp = m_mainControl->GetBpp();
		m_game = new CGame(hWnd,hInstance,lpSystemFile,sz.cx,sz.cy,bpp);
		m_mainControl->Game(m_game);
	}

	ENDDELETECLASS(m_game);
	ENDDELETECLASS(m_mainControl);

	CCurrentDirControl::RestoreCurrent();

	return 0;
}

/*_*/

