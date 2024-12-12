//
// game.cpp
//



#include <windows.h>
#include <stdio.h>
#include <d3dx9.h>

#include "..\nyanLib\include\commonMacro.h"
#include "..\nyanLib\include\picture.h"

#include "..\nyanLib\include\allGraphics.h"
#include "..\nyanLib\include\allGeo.h"

#include "..\nyanDirectX3DLib\myDirect3D.h"
#include "..\nyanDirectX3DLib\myPixelShader.h"
#include "..\nyanDirectX3DLib\myTexture.h"


#include "..\nnnUtilLib\scriptcommand.h"
#include "..\nnnUtilLib\commonGameVersion.h"
#include "..\nnnUtilLib\nameList.h"
//#include "..\nnnUtilLib\varNumber.h"
#include "..\nnnUtilLib\myKeyStatus.h"
#include "..\nnnUtilLib\inputStatus.h"
//#include "..\nnnUtilLib\gameMouse.h"
//#include "..\nnnUtilLib\messageCursor.h"
//#include "..\nnnUtilLib\sceneOptionButton.h"
//#include "..\nnnUtilLib\waveMusic.h"
#include "..\nnnUtilLib\commonSystemFile.h"

#include "..\nnnUtilLib\myFont.h"

#include "..\nnnLib\commonMode.h"
//#include "..\nnnLib\execScript.h"
#include "..\nnnLib\commonGeneral.h"
//#include "..\nnnLib\commonSelectPlace.h"
//#include "..\nnnLib\commonSelectHeroin.h"
//#include "..\nnnLib\commonPrintCalendar.h"
//#include "..\nnnLib\commonPrintMovie.h"
#include "..\nnnLib\gameCallBack.h"

#include "mode.h"

#include "game.h"

/*
int CGame::m_daysOfMonth[13]=
{
	0,	31,29,31,30,31,30,31,31,30,31,30,31,
};
*/


CGame::CGame(HWND hwnd, HINSTANCE hinstance, CCommonSystemFile* lpSystemFile, int windowSizeX, int windowSizeY, int bpp) : CGameCallBack(hwnd, hinstance, lpSystemFile, windowSizeX, windowSizeY, bpp)
{
}

void CGame::Create(void)
{
	if (CheckDebugOk()) SetDebugFlag();

	m_selectPlaceCommandNumber = -1;
	m_selectHeroinCommandNumber = -1;
	m_printCalendarCommandNumber = -1;
	m_printMovieCommandNumber = -1;

	if (m_createModeDisableFlag[SELECTPLACE_MODE] == 0)
	{
		m_selectPlaceCommandNumber = m_commandList->SearchName2("selectplace");
		if (m_selectPlaceCommandNumber != -1) m_selectPlaceCommandNumber /= 2;
	}

	if (m_createModeDisableFlag[PRINTMOVIE_MODE] == 0)
	{
		m_printMovieCommandNumber = m_commandList->SearchName2("printmovie");
		if (m_printMovieCommandNumber != -1) m_printMovieCommandNumber /= 2;
	}

//	ResistUserCommand();


	CreateAllClass();
	SetOption(FALSE);
	StartInitialMessage();
}


CGame::~CGame()
{
	End();
}

void CGame::End(void)
{
}






















///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::CreateAllClass(void)
{
	CreateCommonClass(NOTHING_MODE);
	CreateCommonClass(LOGO_MODE);
	CreateCommonClass(TITLE_MODE);
	CreateCommonClass(OPENING_MODE);
	CreateCommonClass(CONFIG_MODE);
	CreateCommonClass(PRINTMESSAGE_MODE);
	CreateCommonClass(SELECTPLACE_MODE);
	CreateCommonClass(BACKLOG_MODE);
	CreateCommonClass(PRINTOVERRAP_MODE);
	CreateCommonClass(SELECTMESSAGE_MODE);
	CreateCommonClass(SYSTEMMENU_MODE);

	CreateCommonClass(CODECERROR_MODE);
	CreateCommonClass(PRINTMOVIE_MODE);

	CreateCommonClass(SELECTHEROIN_MODE);
	CreateCommonClass(PRINTCALENDAR_MODE);

	if (CCommonGameVersion::CheckTaikenOrNetVersion() == FALSE)
	{
		CreateCommonClass(OMAKE_MODE);
		CreateCommonClass(SELECTCGCHARA_MODE);
		CreateCommonClass(SELECTCG_MODE);
		CreateCommonClass(SELECTSCENECHARA_MODE);
		CreateCommonClass(SELECTSCENE_MODE);
		CreateCommonClass(LISTENBGM_MODE);
		CreateCommonClass(PRINTCG_MODE);
//		CreateCommonClass(LOAD_MODE);
//		CreateCommonClass(SAVE_MODE);
	}

	CreateCommonClass(LOAD_MODE);
	CreateCommonClass(SAVE_MODE);

	CreateCommonClass(MINIGAME_MODE);
}















void CGame::ReceiveUserCommand(int cmd, int paraKosuu, int* paraPtr)
{
	m_skipNextCommandFlag = FALSE;
	m_userCommand = cmd;
	GetUserPara(paraKosuu, paraPtr);

//	if (m_userCommand == m_selectHeroinCommandNumber)
//	{
//		CSelectHeroin* selectHeroin = (CSelectHeroin*)m_general[SELECTHEROIN_MODE];
//		selectHeroin->StartSelectHeroin();
//		SetReturnCode(SELECTHEROIN_MODE);
//	}


}















void CGame::ReceiveUserFunction(int cmd, int paraKosuu, int* paraPtr)
{
	m_userFunction = cmd;
	GetUserPara(paraKosuu, paraPtr);

	if (m_userFunction == m_adjustDateFunction)
	{
		if (m_dayMonthVar != -1)
		{
			int d = GetVarData(m_dayMonthVar);
////			d = AdjustDate(d);
			SetVarData(m_dayMonthVar,d);
		}
	}



}














/*
int CGame::AdjustDate(int daymonth)
{
	int month = daymonth / 100;
	int day = daymonth % 100;

	if (day > m_daysOfMonth[month])
	{
		day -= m_daysOfMonth[month];
		month++;
	}

	return month * 100 + day;
}
*/



















void CGame::UserCommandCallBack(void)
{
	if (m_userCommand == m_selectPlaceCommandNumber)
	{
//		CCommonSelectPlace* selectPlace = (CCommonSelectPlace*)(m_general[SELECTPLACE_MODE]);
//
		SetReturnCode(SELECTPLACE_MODE);
	}

	if (m_userCommand == m_selectHeroinCommandNumber)
	{
//		CCommonSelectHeroin* selectHeroin = (CCommonSelectHeroin*)(m_general[SELECTHEROIN_MODE]);
//		SetReturnCode(SELECTHEROIN_MODE);
	}

	if (m_userCommand == m_printCalendarCommandNumber)
	{
//		CCommonPrintCalendar* printCalendar = (CCommonPrintCalendar*)(m_general[PRINTCALENDAR_MODE]);
//		SetReturnCode(PRINTCALENDAR_MODE);
	}

	if (m_userCommand == m_printMovieCommandNumber)
	{
//		m_waveMusic->PlayBGM("testmml",0,TRUE,30);
//		m_waveMusic2->StopBGM(TRUE,40);

///		CCommonPrintMovie* printMovie = (CCommonPrintMovie*)(m_general[PRINTMOVIE_MODE]);
		//set movie number


		SetReturnCode(PRINTMOVIE_MODE);
	}

}





















































BOOL CGame::CheckDebugOk(void)
{
	FILE* file00 = NULL;
	fopen_s(&file00,"debug666.txt","rb");
	if (file00 != NULL)
	{
		fclose(file00);
		return TRUE;
	}
	return FALSE;
}

/*
void CGame::GetExtDataForSave(LPVOID ptr,int extNumber)
{
	int* ptr2 = (int*)ptr;
	*ptr2 = 0x12345678;
}

void CGame::SetExtDataByLoad(LPVOID ptr,int extNumber)
{
	int* ptr2 = (int*)ptr;
	char mes[256];
	wsprintf(mes,"[load=%X]",*ptr2);
	OutputDebugString(mes);
}
*/


void CGame::AfterPrint(void)
{
//	m_dmy->GradationShapeBlt(0,0,0,0,100,100, 64,128,192, 192,64,32);

	//m_dmy->ChangeTranslateBlt(0,0,0,0,100,100,  30,70,m_dmy2,0,0);
//	CAllGeo::TransBoxFill(100,100,300,300,192,128,64 , 60);
//	CAllGraphics::Fade(20, 64,192,128);
//	m_font->Print(0,0,0,0,800,600);
}














































/*_*/



