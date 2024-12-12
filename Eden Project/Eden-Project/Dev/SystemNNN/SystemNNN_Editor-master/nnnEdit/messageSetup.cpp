

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"
#include "configData.h"


#include "myConfig.h"

#include "messageSetup.h"


CMessageSetup::CMessageSetup(CMyConfig* lpConfig)
{
	m_config = lpConfig;


	m_mesNameByInit = GetConfig("mesNameByInit");

	m_mesWinPrintMode = GetConfig("mesWinPrintMode");
	m_mesWinZahyo.x = GetConfig("mesWinPrintX");
	m_mesWinZahyo.y = GetConfig("mesWinPrintY");
	m_mesWinSize.cx = GetConfig("mesWinSizeX");
	m_mesWinSize.cy = GetConfig("mesWinSizeY");
	m_mesWinFileName = GetConfigString("mesWinFileName");
	m_mesFontSize = GetConfig("mesFontSize");
	m_mesPrintDelta.x = GetConfig("mesPrintDeltaX");
	m_mesPrintDelta.y = GetConfig("mesPrintDeltaY");
	m_mesNextY = GetConfig("mesPrintNextY");
	m_mesLPrintZahyo.x = GetConfig("mesLPrintX");
	m_mesLPrintZahyo.y = GetConfig("mesLPrintY");
	m_nameWinPrintMode = GetConfig("nameWinPrintMode");
	m_nameWinZahyo.x = GetConfig("nameWinPrintX");
	m_nameWinZahyo.y = GetConfig("nameWinPrintY");
	m_nameWinSize.cx = GetConfig("nameWinSizeX");
	m_nameWinSize.cy = GetConfig("nameWinSizeY");
	m_nameWinFileName = GetConfigString("nameWinFileName");
	m_nameFontSize = GetConfig("nameFontSize");
	m_namePrintDelta.x = GetConfig("namePrintDeltaX");
	m_namePrintDelta.y = GetConfig("namePrintDeltaY");
	m_nameAutoOffFlag = GetConfig("nameAutoOffFlag");

}

CMessageSetup::~CMessageSetup()
{
	End();
}

void CMessageSetup::End(void)
{
}


int CMessageSetup::GetConfig(LPSTR name)
{
	return m_config->GetValue(name);
}


LPSTR CMessageSetup::GetConfigString(LPSTR name)
{
	return m_config->GetText(name);
}

void CMessageSetup::SetConfig(LPSTR name,int value)
{
	m_config->SetValue(name,value);
}

void CMessageSetup::SetConfig(LPSTR name,LPSTR mes)
{
	m_config->SetText(name,mes);
}



void CMessageSetup::SetMesNameByInit(int d)
{
	m_mesNameByInit = d;
	SetConfig("mesNameByInit",d);
}

void CMessageSetup::SetMesWinPrintMode(int d)
{
	m_mesWinPrintMode = d;
	SetConfig("mesWinPrintMode",d);
}

void CMessageSetup::SetMesWinZahyo(POINT pt)
{
	m_mesWinZahyo = pt;
	SetConfig("mesWinPrintX",pt.x);
	SetConfig("mesWinPrintY",pt.y);
}

void CMessageSetup::SetMesWinSize(SIZE sz)
{
	m_mesWinSize = sz;
	SetConfig("mesWinSizeX",sz.cx);
	SetConfig("mesWinSizeY",sz.cy);
}

void CMessageSetup::SetMesWinFileName(LPSTR filename)
{
	m_mesWinFileName = filename;
	SetConfig("mesWinFileName",filename);
}

void CMessageSetup::SetMesPrintDelta(POINT pt)
{
	m_mesPrintDelta = pt;
	SetConfig("mesPrintDeltaX",pt.x);
	SetConfig("mesPrintDeltaY",pt.y);
}

void CMessageSetup::SetMesNextY(int nextY)
{
	m_mesNextY = nextY;
	SetConfig("mesPrintNextY",nextY);
}

void CMessageSetup::SetMesLPrintZahyo(POINT pt)
{
	m_mesLPrintZahyo = pt;
	SetConfig("mesLPrintX",pt.x);
	SetConfig("mesLPrintX",pt.y);
}

void CMessageSetup::SetNameWinPrintMode(int d)
{
	m_nameWinPrintMode = d;
	SetConfig("nameWinPrintMode",d);
}

void CMessageSetup::SetNameWinZahyo(POINT pt)
{
	m_nameWinZahyo = pt;
	SetConfig("nameWinPrintX",pt.x);
	SetConfig("nameWinPrintY",pt.y);
}

void CMessageSetup::SetNameWinSize(SIZE sz)
{
	m_nameWinSize = sz;
	SetConfig("nameWinSizeX",sz.cx);
	SetConfig("nameWinSizeY",sz.cy);
}

void CMessageSetup::SetNameWinFileName(LPSTR filename)
{
	m_nameWinFileName = filename;
	SetConfig("nameWinFileName",filename);
}

void CMessageSetup::SetNamePrintDelta(POINT pt)
{
	m_namePrintDelta = pt;
	SetConfig("namePrintDeltaX",pt.x);
	SetConfig("namePrintDeltaY",pt.y);
}

void CMessageSetup::SetNameAutoOffFlag(int d)
{
	m_nameAutoOffFlag = d;
	SetConfig("nameAutoOffFlag",d);
}

void CMessageSetup::SetMesFontSize(int fontSize)
{
	m_mesFontSize = fontSize;
	SetConfig("mesFontSize",fontSize);
}

void CMessageSetup::SetNameFontSize(int fontSize)
{
	m_nameFontSize = fontSize;
	SetConfig("nameFontSize",fontSize);
}


/*_*/

