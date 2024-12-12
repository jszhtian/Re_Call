//
// myconfig.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"
#include "configData.h"


#include "myconfig.h"


#define MYCONFIGKOSUUMAX 1024



CMyConfig::MYCONFIGTEXTTYPE CMyConfig::m_initDataString[]=
{
	{"lastEditFileNameOnly","新規"},
	{"mesWinFilename","@dummy"},
	{"nameWinFilename","@dummy"},
	{"defaultNameMei","デフォ子"},

	{"",""},
};



CMyConfig::MYCONFIGVALUETYPE CMyConfig::m_initDataValue[]=
{
	{"screenSizeMode",1},

	{"windowMode",1},
	{"windowPosX",32},
	{"windowPosY",32},
	{"windowSizeX",1200},
	{"windowSizeY",960},

	{"frameTime",20},

	{"varBlockType",1},
	{"varBlockNumber",2},
	{"varBlock1",400},
	{"varBlock2",300},

	{"inputTextLimitNT",0},

	{"commentTopPrintFlag",1},
	{"commentBottomPrintFlag",0},

	{"noSavePicFlag",0},
	{"noSaveNNNPicFlag",0},

	{"effectMenuPrintMode",1},
	{"effectBarPrintMode",1},
	{"effectMenuUpDown",0},
	{"effectLeftRight",0},
	{"effectNamePrintFlag",1},
	{"effectDescPrintFlag",0},
	{"effectParamDescPrintFlag",0},

	{"komaOfFilm",1},
	{"komaOfFilmType",0},
	{"frameOfKoma",25},
	{"frameOfKomaType",1},

	{"editFrame",0},
	{"editFrameType",0},

	{"autoSetCGFlag",1},
	{"sceneMakeFlag",1},

	{"checkSameCGNameFlag",1},

	{"warningDeleteParam",1},
	{"warningDeleteKoma",1},
	{"warningDeleteMessage",0},
	{"warningDeleteFilm",1},
	{"warningDeleteCommand",0},
	{"warningDeleteStory",1},
	{"warningDeleteVar",0},


	{"mesWinPrintMode",0},	//0,1(box),2(pic)

	{"mesWinPrintX",0},
	{"mesWinPrintY",400},
	{"mesWinSizeX",800},
	{"mesWinSizeY",200},
	{"mesPrintDeltaX",16},	
	{"mesPrintDeltaY",16},	
	{"mesPrintX",66},	
	{"mesPrintY",352},	
	{"mesLPrintX",66},	
	{"mesLPrintY",64},	
	{"mesAppendX",66},	
	{"mesAppendY",256},	

	{"nameWinPrintMode",0},	//0,1(box),2(pic)

	{"nameWinPrintX",20},
	{"nameWinPrintY",360},
	{"nameWinSizeX",100},
	{"nameWinSizeY",40},
	{"namePrintDeltaX",16},	
	{"namePrintDeltaY",16},	

	{"nameAutoOffFlag",0},


	{"mesPrintNextY",30},



	{"mesFontSize",24},
	{"nameFontSize",24},


	{"mesNameByInit",0},	//initから設定を読み込む()

	{"soundVolume",70},
	{"musicVolume",70},
	{"voiceVolume",70},

	{"effectCopyMoveFlag",0},

	{"defaultBGLayer",0},
	{"defaultTALayer",4},
	{"defaultOverrapTime",5},

	{"defaultMovieRate",30},
	{"defaultMovieScale",1},

	{"movieSize",0},
	{"moviePackType",0},

	{"selFilmCommand",1},
	{"selStoryCommand",1},

	{"selCalcuCommand",1},

	{"mainScreenDBLClick",0},
	{"filmDBLClick",0},
	{"conteDBLClick",0},
	{"layerDBLClick",0},
	{"zahyoDBLClick",0},
	{"gameMessageDBLClick",0},
	{"effectDBLClick",0},
	{"storyDBLClick",0},
	{"varDBLClick",0},
	{"programDBLClick",0},
	{"controlDBLClick",0},
	{"filmCaseDBLClick",0},
	{"storyBookDBLClick",0},
	{"conteMessageDBLClick",0},

	{"deleteKeyFilm",0},
	{"deleteKeyKoma",0},
	{"deleteKeyMessage",1},
	{"deleteKeyStory",0},
	{"deleteKeyCommand",1},
	{"deleteKeyVar",0},


	{"autoSetOverrapFlag",1},

	{"angouXorCode",-1},
	{"angouXorAddCode",0},
	{"angouIncAddCode",0},

	{"cutin",0},

	{"voicePack",0},

	{"rubiPrintFlag",0},
	{"rubiPercent",40},

	{"rubiBuildFlag",0},
	{"rubiColorR",255},
	{"rubiColorG",255},
	{"rubiColorB",255},
	{"rubiColorFix",0},
	{"rubiSukima",1},
	{"rubiDeltaY",0},

	{"kanjiCode",0},

	{"cutinBuildFlag",0},

	{"loadEffectType",2},

	{"jpegMMX",1},
	{"backupNNN",1},

	{"editorMessageGyo",4},
	{"messageWidthGuideLine",24},

	{"skipMax",3},
	{"skipInterval",10},

	{"printFrameFlag",1},
	{"printSkipFlag",1},

	{"layerCopyEffectMode",2},
	{"sceneCheckMode",0},

	{"pngGammaFlag",0},

	{"bufferSetCG",1},
	{"initAngou",0},

	{"dwqFilterLength",0},

	{"sameLoadFlag",0},

	{"targetMachine",0},

	{"varControlSetCG",0},

	{"daihonWhiteSpaceNumber",0},

	{"presetButtonEnable",1},

	{"presetX_0_0",-300},
	{"presetY_0_0",0},
	{"presetX_0_1",-200},
	{"presetY_0_1",0},
	{"presetX_0_2",-100},
	{"presetY_0_2",0},
	{"presetX_0_3",0},
	{"presetY_0_3",0},
	{"presetX_0_4",100},
	{"presetY_0_4",0},
	{"presetX_0_5",200},
	{"presetY_0_5",0},
	{"presetX_0_6",300},
	{"presetY_0_6",0},


	{"addDefaultVoice",0},

	{"termFlag",0},

	{"balloonFlag",0},

	{"zahyoPrintType",0},
	{"layerMax",32},

	{"undoEnable",0},
	{"undoKakuninEnable",1},
	{"undoAfterEnable",1},

	{"controlKeyEnable",0},

	{"",0},
};





CMyConfig::MYCONFIGWINDOWTYPE CMyConfig::m_initDataWindow[]=
{
	{"mainScreen",1,247,28,800,600, -1},
	{"film",1,660,535,432,120, -1},
	{"conte", 1,568,72,168,424, -1},
	{"layer",1,897,31,136,792, -1},
	{"zahyo",1,1042,32,240,240, -1},
	{"gameMessage",1,537,702,485,216, -1},
	{"effect",1,9,29,232,625, -1},
	{"story",1,887,462,319,344, -1},
	{"var",1,706,759,512,144, -1},
	{"program",1,964,448,512,512, -1},
	{"control",1,1040,300,136,128, -1},
	{"filmCase",1,7,747,480,144, -1},
	{"storyBook",1,387,745,191,144, -1},
	{"conteMessage",1,942,491,137,393, -1},


	{"",0},
};



CMyConfig::CMyConfig(int varType)
{
	m_varType = varType;

	m_configData = new CConfigData();
	if (m_configData->CheckLoadOk() == FALSE)
	{
		SetInitData();
	}
	else
	{
		AdjustInitData();
	}
}


CMyConfig::~CMyConfig()
{
	End();
}

void CMyConfig::End(void)
{
	SaveData();
	ENDDELETECLASS(m_configData);
}


LPSTR CMyConfig::GetText(LPSTR name)
{
	return m_configData->GetString(name);
}

int CMyConfig::GetValue(LPSTR name)
{
	return m_configData->GetData(name);
}


BOOL CMyConfig::SetText(LPSTR name, LPSTR text)
{
	m_configData->SetString(name, text);
	return TRUE;
}

BOOL CMyConfig::SetValue(LPSTR name, int d)
{
	m_configData->SetData(name, d);
	return TRUE;
}


BOOL CMyConfig::SaveData(void)
{
	if (m_configData != NULL)
	{
		if (m_configData->CheckEdit())
		{
			m_configData->Save();
		}
	}

	return TRUE;
}


void CMyConfig::AdjustInitData(void)
{
	int i;
	for (i = 0; i < MYCONFIGKOSUUMAX; i++)
	{
		if (m_initDataString[i].name[0] == 0) break;
		if (m_configData->SearchName(m_initDataString[i].name) == -1)
		{
			SetText(m_initDataString[i].name, m_initDataString[i].text);
		}
	}


	for (i = 0; i < MYCONFIGKOSUUMAX; i++)
	{
		if (m_initDataValue[i].name[0] == 0) break;
		if (m_configData->SearchName(m_initDataValue[i].name) == -1)
		{
			SetValue(m_initDataValue[i].name, m_initDataValue[i].value);
		}
	}
}


void CMyConfig::SetInitData(void)
{

	int i;
	for (i = 0; i < MYCONFIGKOSUUMAX; i++)
	{
		if (m_initDataString[i].name[0] == 0) break;
		SetText(m_initDataString[i].name, m_initDataString[i].text);
	}


	for (i = 0; i < MYCONFIGKOSUUMAX; i++)
	{
		if (m_initDataValue[i].name[0] == 0) break;
		SetValue(m_initDataValue[i].name, m_initDataValue[i].value);
	}


	if (m_varType > 0)
	{
		SetValue("varBlock2",600);
	}


	for (i=0;i<MYCONFIGKOSUUMAX;i++)
	{
		if (m_initDataWindow[i].name[0] == 0) break;

		int md[2],left[2],top[2],right[2],bottom[2];

		md[0] = m_initDataWindow[i].md;
		left[0] = m_initDataWindow[i].left;
		top[0] = m_initDataWindow[i].top;
		right[0] = m_initDataWindow[i].right;
		bottom[0] = m_initDataWindow[i].bottom;

		left[1] = left[0];
		top[1] = top[0];
		right[1] = right[0];
		bottom[1] = bottom[0];

		md[1] = m_initDataWindow[i].md2;
		
		if (md[1] == -1)
		{
			md[1] = md[0]; 
		}
		else
		{
			left[1] = m_initDataWindow[i].left2;
			top[1] = m_initDataWindow[i].top2;
			right[1] = m_initDataWindow[i].right2;
			bottom[1] = m_initDataWindow[i].bottom2;
		}

		for (int k=0;k<2;k++)
		{
			char name[256];
			
			wsprintf(name,"%sWindowFlag%d",m_initDataWindow[i].name,k+1);
			SetValue(name,md[k]);

			wsprintf(name,"%sWindowPosX%d",m_initDataWindow[i].name,k+1);
			SetValue(name,left[k]);

			wsprintf(name,"%sWindowPosY%d",m_initDataWindow[i].name,k+1);
			SetValue(name,top[k]);

			wsprintf(name,"%sWindowSizeX%d",m_initDataWindow[i].name,k+1);
			SetValue(name,right[k]);

			wsprintf(name,"%sWindowSizeY%d",m_initDataWindow[i].name,k+1);
			SetValue(name,bottom[k]);
		}
	}
}



void CMyConfig::GetWindowZahyo(LPSTR windowName, int md, RECT* lpRect, BOOL* lpFlag)
{
	char name[256];

	wsprintf(name,"%sPosX%d",windowName,md+1);
	lpRect->left = GetValue(name);

	wsprintf(name,"%sPosY%d",windowName,md+1);
	lpRect->top = GetValue(name);

	wsprintf(name,"%sSizeX%d",windowName,md+1);
	lpRect->right = GetValue(name);

	wsprintf(name,"%sSizeY%d",windowName,md+1);
	lpRect->bottom = GetValue(name);

	wsprintf(name,"%sFlag%d",windowName,md+1);
	*lpFlag = (BOOL)GetValue(name);
}

void CMyConfig::SetWindowZahyo(LPSTR windowName, int md, RECT* lpRect, BOOL flag)
{
	char name[256];

	wsprintf(name,"%sPosX%d",windowName,md+1);
	SetValue(name,lpRect->left);

	wsprintf(name,"%sPosY%d",windowName,md+1);
	SetValue(name,lpRect->top);

	wsprintf(name,"%sSizeX%d",windowName,md+1);
	SetValue(name,lpRect->right);

	wsprintf(name,"%sSizeY%d",windowName,md+1);
	SetValue(name,lpRect->bottom);

	wsprintf(name,"%sFlag%d",windowName,md+1);
	SetValue(name,flag);
}


