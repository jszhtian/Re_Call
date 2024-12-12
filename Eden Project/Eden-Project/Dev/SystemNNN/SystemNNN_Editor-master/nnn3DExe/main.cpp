//
// main.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\nyanLib\include\commonmacro.h"
#include "..\nnnUtilLib\commongameVersion.h"

#include "..\nnnLib\mainControl.h"
#include "..\nnnLib\gameCallBack.h"

#include "game.h"
#include "main.h"

#include "..\nyanlib\include\libinclude.h"


int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	//
	// �K�v�Ȃ炱���ő̌��Ńt���O�Ȃǂ����Ă�
	//

//	CCommonGameVersion::SetTaikenVersion();
//	CCommonGameVersion::SetLoadOk();
//	CCommonGameVersion::SetSaveOk();

	m_game = NULL;
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
		//�ʂ���܂��ɓ�����systemfile->save�������Ȃ�
	}

	ENDDELETECLASS(m_game);
	ENDDELETECLASS(m_mainControl);

	return 0;
}

/*_*/

