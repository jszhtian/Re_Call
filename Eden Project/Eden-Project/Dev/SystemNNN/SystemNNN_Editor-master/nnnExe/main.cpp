//
// main.cpp
//








#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "resource.h"

#include "..\nyanLib\include\commonmacro.h"

#include "..\nnnUtilLib\commongameVersion.h"

#include "..\nnnLib\mainControl.h"

#include "..\nnnUtilLib\myKeyStatus.h"
#include "..\nnnLib\gameCallBack.h"

#include "..\nyanLib\include\picture.h"
#include "game.h"
#include "main.h"







#include "..\nyanlib\include\libinclude.h"

//#define __DEMO_VERSION__


int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	//
	// �K�v�Ȃ炱���ő̌��Ńt���O�Ȃǂ����Ă�
	//

//	CCommonGameVersion::SetTaikenVersion();
//	CCommonGameVersion::SetLoadOk();
//	CCommonGameVersion::SetSaveOk();

//	CCommonSystemFile::SetMaxCGCharaNumber(2);

//	CPicture* aaa = new CPicture("");
//	aaa->LoadDWQ("bg\\bg_error");
	
	
	m_game = NULL;
//	m_wheelMouse = new CWheelMouse();
//	m_systemFile = new CCommonSystemFile();
//	m_systemFile->Load(TRUE);

//	int ln = strlen("���\��");
	/*
	char* aaa = new char[1024*1024*16];
	FILE* file = fopen("akane.spt","rb");
	int ln = fread(aaa,1,1024*1024*16,file);
	fclose(file);

	for (int i=0;i<ln;i++)
	{
		char c = aaa[i];
		c ^= 0xff;
		aaa[i] = c;
	}


	file = fopen("akane2.spt","wb");
	fwrite(aaa,1,ln,file);
	fclose(file);

	delete [] aaa;
*/












	m_mainControl = new CMainControl();	//�R���X�g���N�^��wheelMouse,systemfile���w��ł��� �w��Ȃ��A�܂���NULL���Ɠ����ŕW���̂��쐬����

	if (m_mainControl->CheckSystem())
	{
		HICON icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		HWND hWnd = m_mainControl->CreateWindowRoutine(hInstance,icon);//hinstance,icon,proc    proc == NULL����CMainControl�����̕W��callBack

		CCommonSystemFile* lpSystemFile = m_mainControl->GetSystemFile();
		SIZE sz = m_mainControl->GetWindowSize();
		int bpp = m_mainControl->GetBpp();
		m_game = new CGame(hWnd,hInstance,lpSystemFile,sz.cx,sz.cy,bpp);

#if !defined _MT
		m_mainControl->Game(m_game);				//�V���O���X���b�h�p
#else
		m_mainControl->MultiThreadGame(m_game);		//�}���`�X���b�h�p
#endif

		//��̂Q�͂ǂ�����͂ʂ���܂���systemfile->save�������Ȃ�
	}

	ENDDELETECLASS(m_game);
	ENDDELETECLASS(m_mainControl);

	return 0;
}














/*_*/

