//
//  myapplication.cpp
//



#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <commctrl.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonMacro.h"
#include "..\..\systemNNN\nyanLib\include\areaControl.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"

#include "..\..\systemNNN\nyanlib\include\mygraphics.h"
#include "..\..\systemNNN\nyanlib\include\picture.h"
#include "..\..\systemNNN\nyanLib\include\allGeo.h"

#include "..\..\systemNNN\nyanPictureLib\jpegDecoder.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nyanDirectXLib\myDirectSound.h"
#include "..\..\systemNNN\nyanDirectXLib\myDirectSoundBuffer.h"
#include "..\..\systemNNN\nnnUtilLib\waveData.h"

#include "..\..\systemNNN\nnnUtilLib\waveMusic.h"

#include "..\..\systemNNN\nnnUtilLib\basicSetup.h"
#include "..\..\systemNNN\nnnUtilLib\commonMessageWindow.h"

#include "..\..\systemNNN\nnnUtilLib\mmlControl.h"
#include "..\..\systemNNN\nnnUtilLib\musicControl.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"
#include "..\..\systemNNN\nnnUtilLib\cutinControl.h"

#include "..\..\systemNNN\nnnUtilLib\autoSaveSubData.h"
#include "..\..\systemNNN\nnnUtilLib\okikaeData.h"


#include "..\..\systemNNN\nnnUtilLib\cutinNameData.h"

#include "..\..\systemNNN\nnnUtilLib\sceneList.h"
#include "..\..\systemNNN\nnnUtilLib\sceneVoice.h"

#include "..\..\systemNNN\nyanLib\include\effect.h"
//#include "..\cyclib\cyclib.h"
#include "..\..\systemNNN\nyanEffectLib\effectlist.h"
#include "..\..\systemNNN\nnnUtilLib\scriptDefine.h"

#include "..\..\systemNNN\nnnUtilLib\shakin.h"
#include "undoMemoryObject.h"

#include "..\..\systemNNN\nyanPictureLib\pngLoader.h"

#include "..\..\SystemNNN\nyanEffectLib\calcuSpeed.h"


#include "define.h"

#include "bitmapnumber.h"

#include "commanddatatype.h"

#include "windowlist.h"

#include "myBitmap.h"
//#include "systemdib.h"


#include "effectParam.h"
#include "allEffectParam.h"

#include "configData.h"
#include "myConfig.h"

#include "case.h"



#include "messageSetup.h"


#include "myframewindow.h"
#include "myclientwindow.h"
#include "mydocument.h"

#include "mainscreendoc.h"
#include "filmdoc.h"
#include "layerdoc.h"
#include "zahyodoc.h"
#include "gamemessagedoc.h"
#include "effectdoc.h"
#include "storydoc.h"
#include "controldoc.h"
#include "filmcasedoc.h"
#include "vardoc.h"
#include "programdoc.h"
#include "storybookdoc.h"
#include "conteDoc.h"
#include "conteMessageDoc.h"


#include "starttitle.h"
#include "balloon.h"


#include "myview.h"

#include "selectDialog.h"
#include "myApplicationBase.h"
#include "myapplication.h"



#include "filmdata.h"
#include "storydata.h"
#include "komadata.h"
#include "messagedata.h"

#include "storybookdata.h"
#include "filmcasedata.h"

#include "commanddata.h"

#include "filmcasedata.h"

//#include "configparamlist.h"

#include "myfileopen.h"

#include "scriptdata.h"

#include "..\..\systemNNN\nnnUtilLib\myfont.h"
#include "..\..\systemNNN\nnnUtilLib\rubiFont.h"
#include "..\..\systemNNN\nnnUtilLib\mymessage.h"

#include "commonDIB.h"

//#include "..\cycGameLib\namelist.h"

#include "myinputdialog.h"
#include "mySearchDialog.h"

//#include "minicompiler.h"

////@@@#include "wave.h"

#include "selectobjectonly.h"

//@@@#include "config.h"

#include "buttonpic.h"
#include "effectpic.h"

//#include "aviSave.h"

#include "defaultControl.h"
#include "volumeControl.h"

#include "makeUseVawBat.h"
#include "makeCheckVawBat.h"
#include "makeUseDWQBat.h"
#include "makeCheckDWQBat.h"

#include "scriptSoundControl2.h"
#include "scriptVoiceControl2.h"

#include "menuCheckControl.h"
#include "movieCreate.h"

#include "messageSerialControl.h"
#include "scriptCompiler.h"

#include "autoVoice.h"
#include "lockVoice.h"
#include "deleteVoice.h"
#include "makeBook.h"

#include "sameMessageDeutch.h"

#include "searchorReplace.h"

#include "varInitData.h"

#include "windowUpdateControl.h"

#include "copyPreEffect.h"
#include "checkDWQ.h"
#include "deleteNonVoice.h"

#include "changeBGMList.h"
#include "changeSeList.h"

#include "myOntimer.h"

#include "frameChange.h"

#include "rubiMaker.h"


#include "..\..\systemNNN\nyanEffectLIB\commoneffect.h"
#include "..\..\systemNNN\nyanEffectAnimeLIB\effectAnimation.h"


//#include "picojson.h"


#define FILMVARNAME_KOSUU 1000
#define FILMVARNAME_KOSUU2 2200



#define VAR_FILE_NAME "nnndir\\var.txt"
#define VAR_FILE_NAME2 "nnndir\\var2.txt"
#define VARINITDATA_FILE_NAME "nnndir\\varinitdata.txt"
#define VARINITDATA_FILE_NAME2 "nnndir\\varinitdata2.txt"
#define VARMARK_FILE_NAME "nnndir\\nnn\\varmark.col"
#define VARMARK_FILE_NAME2 "nnndir\\nnn\\varmark2.col"
#define VARMARK_SETUP_FILE_NAME "nnndir\\setup\\varmark.col"
#define VARMARK_SETUP_FILE_NAME2 "nnndir\\setup\\varmark2.col"

#define VAR_FXF_FILE_NAME "spt\\var.fxf"
#define VAR_FXF_FILE_NAME2 "spt\\var2.fxf"


#define CHARA_FILE_NAME "nnndir\\charaname.txt"
#define FACE_FILE_NAME "nnndir\\facelist.txt"
#define PROJECT_FILE_NAME "nnndir\\project.txt"
#define SE_FILE_NAME "nnndir\\selist.txt"
#define FUNCTION_FILE_NAME "nnndir\\function.txt"
#define COMMAND_FILE_NAME "nnndir\\command.txt"
#define BGM_FILE_NAME "nnndir\\bgmlist.txt"
#define SETCG_FILE_NAME "nnndir\\setcg.txt"
#define VARCONTROL_FILE_NAME "nnndir\\varcontrol.txt"
#define TERMLIST_FILE_NAME "nnndir\\termlist.txt"

#define VAR_FILE_NAME_ORG "nnndir\\setup\\org\\var.org"
#define VAR_FILE_NAME_ORG2 "nnndir\\setup\\org\\var2.org"
//#define VAR_FILE_NAME_ORG "nnndir\\setup\\org\\varinitdata.org"
#define CHARA_FILE_NAME_ORG "nnndir\\setup\\org\\charaname.org"
#define FACE_FILE_NAME_ORG "nnndir\\setup\\org\\facelist.org"
#define PROJECT_FILE_NAME_ORG "nnndir\\setup\\org\\project.org"
#define SE_FILE_NAME_ORG "nnndir\\setup\\org\\selist.org"
#define FUNCTION_FILE_NAME_ORG "nnndir\\setup\\org\\function.org"
#define COMMAND_FILE_NAME_ORG "nnndir\\setup\\org\\command.org"
#define BGM_FILE_NAME_ORG "nnndir\\setup\\org\\bgmlist.org"
#define SETCG_FILE_NAME_ORG "nnndir\\setup\\org\\setcg.org"
#define VARCONTROL_FILE_NAME_ORG "nnndir\\setup\\org\\varcontrol.org"
#define TERMLIST_FILE_NAME_ORG "nnndir\\setup\\org\\termlist.org"


int CMyApplication::m_screenSizeTable[]=
{
	640,480,
	800,600,
	1024,768,
	1280,960,
	1280,1024,
	1600,1200,
	1920,1080,
	1920,1200,
	//
	320,480,
	320,460,
	480,320,
	480,300,
	//
	640,960,
	640,920,
	960,640,
	960,600,

	//
	768,1024,
	768,1004,
	1024,768,//dont use
	1024,748,
	//
	1280,720,
};


int CMyApplication::m_varBlockTable[][8]=
{
	{1,700},
	{2,400,300},
	{3,300,200,200},
	{7,100,100,100,100,100,100,100},
};

int CMyApplication::m_varBlockTable2[][8] =
{
	{ 1,1000 },
	{ 2,400,600 },
	{ 3,300,200,500 },
	{ 7,100,100,100,100,100,100,400 },
};

char CMyApplication::m_varInitName[100][32]=
{
	"nullpo","date","rnd","time","week","holiday","","","","",
	"year","month","day","hour","minute","second","","","","",
	"systemyear","systemmonth","systemday","systemhour","systemminute","systemsecond","systemweek","systemmillisecond","","",
	"mode","","","","","","","","","",
	"mousex","mousey","trig1","trig2","trig3","click1","click2","click3","wheel","",
	"listenbgmmode","listenbgmnumber","","","","expmode1","expmode2","expmode3","expmode4","expmode5",
	"expcheck1","expcheck2","expcheck3","expcheck4","expcheck5","","","","","",
	"direct3d","pixelshader","vertexshader","","","","","","","",
	"cgpercent","scenepercent","","","","","","","","",
	"answer","launch","","","","","","","","",
};

int CMyApplication::m_windowInitZahyo[]=
{
	-2, 1,0,0,256,256,  1,0,0,256,256,

	MAINSCREEN_WINDOW,1, 4,24,800,600,  1, 4,24,800,600,
	FILM_WINDOW, 1,6,530,72*6,24+48+24 +24,  1,6,530,72*6,24+48+24 +24,
	LAYER_WINDOW, 1,650,24,24+32+24+24+32,24*16 +24,  1,650,24,24+32+24+24+32,24*16 +24,
	ZAHYO_WINDOW, 1,793,26,240,240,  1,793,26,240,240,
	GAMEMESSAGE_WINDOW, 1,695,531,640,24*4 * 2 + 24,  1,695,531,640,24*4 * 2 + 24,
	EFFECT_WINDOW, 1,952,26,256-24,24*16,  1,952,26,256-24,24*16,
	STORY_WINDOW, 1,44,670,480,32*10+24, 1,44,670,480,32*10+24,
	VAR_WINDOW, 1,698,751,512,24*5 + 24,  1,698,751,512,24*5 + 24,
	PROGRAM_WINDOW, 1,1157,443,512,512,  1,1157,443,512,512,
	FILMCASE_WINDOW, 1,654,416,480,24*5 + 24,  1,654,416,480,24*5 + 24,
	STORYBOOK_WINDOW, 1,453,848,480,24*5 + 24,  1,453,848,480,24*5 + 24,
	CONTE_WINDOW, 1,448,531,256,50*8 + 24,  1,448,531,256,50*8 + 24,
	CONTEMESSAGE_WINDOW, 1,1192,26,200,800,  1,1192,26,200,800,
	CONTROL_WINDOW, 1,801,290,136,128,  1,801,290,136,128,

	-1, 1,0,0,256,256,  1,0,0,256,256,
};


CMyApplication::MYCOMMONBITMAPTABLE CMyApplication::m_commonBitmapTable[]=
{
	0,"effect_mini",
	1,"nekofont_s",
	2,"mespic",
	BITMAPNUMBER_NEW,"common_new",
	BITMAPNUMBER_OPEN,"common_open",
	BITMAPNUMBER_SAVE,"common_save",
	BITMAPNUMBER_CUT,"common_cut",
	BITMAPNUMBER_COPY,"common_copy",
	BITMAPNUMBER_PASTE,"common_paste",
	BITMAPNUMBER_DELETE,"common_delete",
	BITMAPNUMBER_UNDO,"common_undo",
	BITMAPNUMBER_DO,"common_do",
	BITMAPNUMBER_CHANGE,"common_change",
	BITMAPNUMBER_EDIT,"common_edit",
	BITMAPNUMBER_BACK,"common_back",
	BITMAPNUMBER_JUMP,"common_jump",
	BITMAPNUMBER_FOWARD,"common_foward",
	BITMAPNUMBER_OVERRAP,"common_overrap",
	BITMAPNUMBER_OVERRAPTYPE,"common_overraptype",
	BITMAPNUMBER_NOCLEAREFFECT,"common_nocleareffect",
	BITMAPNUMBER_WINDOWONOFF,"commonwindowonoff",
	BITMAPNUMBER_SEARCH,"common_search",
	BITMAPNUMBER_SETCG,"common_setcg",
	BITMAPNUMBER_BGM,"common_bgm",
	BITMAPNUMBER_FRAMESPEED,"common_framespeed",
	BITMAPNUMBER_COMMENT_TOP,"common_commentTop",
	BITMAPNUMBER_COMMENT_BOTTOM,"common_commentBottom",

	-1,"",
};

CMyApplication::CMyApplication(HINSTANCE hinstance) : CMyApplicationBase(hinstance)
{
	m_hInstance = hinstance;
	InitCommonControls();
	m_windowUpdateControl = NULL;
	m_onTimer = NULL;
	m_shakinControl = NULL;

	m_winNTFlag = FALSE;
	OSVERSIONINFO osv;
	ZeroMemory(&osv,sizeof(osv));
	osv.dwOSVersionInfoSize = sizeof(osv);
	if (GetVersionEx(&osv))
	{
		if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			if (osv.dwMajorVersion >= 4)
			{
				m_winNTFlag = TRUE;
			}
		}
	}

	m_varType = 1;


	m_nextPrintFlag = FALSE;
	m_windowMode = 0;

	m_defaultFrameTime = 50;
	SetFrameTime(m_defaultFrameTime);

	m_frame = NULL;
	m_client = NULL;
	m_frameHWND = NULL;
	m_clientHWND = NULL;
	m_effect = NULL;

	m_playMode = FALSE;
	m_komaMode = TRUE;
	m_playCommand = 0;

	m_gameCount = 0;
	m_pauseFlag = FALSE;
	m_filmPlayMode = 0;

	ZeroMemory(m_myname,sizeof(m_myname));
	ZeroMemory(m_memo,sizeof(m_memo));

	m_myname[0] = 0;
	m_memo[0] = 0;

	m_noUpdateScreenFlag = FALSE;

	int bt = 1;
	int i;
	for (i=0;i<32;i++)
	{
		m_bitTable[i] = bt;
		bt <<= 1;
	}
	m_bitTable[31] = 0x80000000;

	m_selObj = new CSelectObjectOnly();

	m_nnnConfig = new CMyConfig(GetVarType());


	int jpegMMX = GetConfig("jpegMMX");
	if (jpegMMX == 0)
	{
		CJpegDecoder::SetCalcuFloat();
	}

	int screenSizeX = 640;
	int screenSizeY = 480;


	int screenSizeMode = m_nnnConfig->GetValue("screenSizeMode");
	if (screenSizeMode != 0)
	{
		screenSizeX = m_screenSizeTable[screenSizeMode*2];
		screenSizeY = m_screenSizeTable[screenSizeMode*2+1];
	}

	LPSTR defaultNameMei = m_nnnConfig->GetText("defaultNameMei");
	if (defaultNameMei != NULL)
	{
		if ((*defaultNameMei) != 0)
		{
			CMessageData::SetDefaultName(defaultNameMei);
		}
	}

	CPngLoader::m_gammaAdjustFlag = GetConfig("pngGammaFlag");

	int layerMax = GetConfig("layerMax");
	if (layerMax > 0)
	{
		CKomaData::m_layerMax = layerMax;
	}

	m_myGraphics = new CMyGraphics(screenSizeX,screenSizeY);

	m_shakinControl = new CShakin();
	m_shakinControl->SetFrame(20);
	m_shakinControl->SetMax(GetConfig("skipMax"));
	m_shakinControl->SetKikan(GetConfig("skipInterval"));


	m_skipPrintFlag = GetConfig("printSkipFlag");
	m_framePrintFlag = GetConfig("printFrameFlag");
	m_undoEnable = GetConfig("undoEnable");
	m_undoKakuninEnable = GetConfig("undoKakuninEnable");
	m_undoAfterEnable = GetConfig("undoAfterEnable");

	m_controlKeyEnable = GetConfig("controlKeyEnable");

	m_allEffectParam = new CAllEffectParam();
	CKomaData::m_allEffectParam = m_allEffectParam;

	if (m_nnnConfig->GetValue("noSavePicFlag") != 0)
	{
		CKomaData::m_smSaveFlag = FALSE;
	}

	int komaOfFilm = GetConfig("komaOfFilm");
	CFilmData::SetKomaOfFilm(komaOfFilm);
	int komaOfFilmType = GetConfig("komaOfFilmType");
	CFilmData::SetKomaOfFilmType(komaOfFilmType);

	int frameOfKoma = GetConfig("frameOfKoma");
	CKomaData::SetFrameOfKoma(frameOfKoma);
	int frameOfKomaType = GetConfig("frameOfKomaType");
	CKomaData::SetFrameOfKoma(frameOfKomaType);


	m_autoSetCGFlag = GetConfig("autoSetCGFlag");
	m_sceneMakeFlag = GetConfig("sceneMakeFlag");
	m_checkSameCGNameFlag = GetConfig("checkSameCGNameFlag");


	m_warningDeleteScene = GetConfig("warningDeleteScene");
	m_warningDeleteCommand = GetConfig("warningDeleteCommand");
	m_warningDeleteFilm = GetConfig("warningDeleteFilm");
	m_warningDeleteKoma = GetConfig("warningDeleteKoma");
	m_warningDeleteMessage = GetConfig("warningDeleteMessage");
	m_warningDeleteParam = GetConfig("warningDeleteParam");
	m_warningDeleteVar = GetConfig("warningDeleteVar");

	int sameLoadFlag = GetConfig("sameLoadFlag");
	CMyFileOpen::SetSameZahyoFlag(sameLoadFlag);

	m_presetButtonEnableFlag = GetConfig("presetButtonEnable");

	m_messageSetup = new CMessageSetup(m_nnnConfig);

	m_cutinFlag = GetConfig("cutin");
	m_cutinBuildFlag = GetConfig("cutinBuildFlag");
//	m_cutinControl = NULL;
	m_cutinNameData = NULL;
	if (m_cutinFlag || (m_cutinBuildFlag == 1))
	{
//		m_cutinControl = new CCutinControl(TRUE);
		m_cutinNameData = new CCutinNameData("nnndir\\cutinChara.txt");
	}




	if (m_messageSetup->GetMesNameByInit())
	{
		//
	}

	int animeEffectEnale = GetConfig("animeEffectEnable");
	if (animeEffectEnale > 0)
	{
		CEffectAnimation::SetAnimeEffectEnable(animeEffectEnale);
	}
	int animeBufferMax = GetConfig("animeBufferMax");
	if (animeBufferMax > 0)
	{
		CEffectAnimation::SetAnimeBufferMax(animeBufferMax);
	}

	int animeBufferDepth = GetConfig("animeBufferDepth");
	if (animeBufferDepth > 0)
	{
		CEffectAnimation::SetBufferDepth(animeBufferDepth);
	}





	m_mesWinPicLoadFlag = FALSE;
	m_nameWinPicLoadFlag = FALSE;

	m_windowInitZahyo[1*11+4] = screenSizeX;
	m_windowInitZahyo[1*11+5] = screenSizeY;
	m_windowInitZahyo[1*11+9] = screenSizeX;
	m_windowInitZahyo[1*11+10] = screenSizeY;



	RECT rc;
	BOOL fg;
	m_nnnConfig->GetWindowZahyo("mainScreenWindow",0,&rc,&fg);
	rc.right = screenSizeX;
	rc.bottom = screenSizeY;
	m_nnnConfig->SetWindowZahyo("mainScreenWindow",0,&rc,fg);

	m_nnnConfig->GetWindowZahyo("mainScreenWindow",1,&rc,&fg);
	rc.right = screenSizeX;
	rc.bottom = screenSizeY;
	m_nnnConfig->SetWindowZahyo("mainScreenWindow",1,&rc,fg);


	m_modifyFlag = FALSE;
	m_dataExistFlag = FALSE;


	for (i=0;i<MDIWINDOW_KOSUU_MAX;i++)
	{
		m_document[i] = NULL;
	}

	for (i=0;i<50;i++)
	{
		m_commonBitmap[i] = NULL;
		m_commonHBitmap[i] = NULL;
	}

	for (i=0;i<50;i++)
	{
		int nm = m_commonBitmapTable[i].nm;
		if (nm == -1) break;

		char filename[256];
		wsprintf(filename,"nnndir\\setup\\bmp\\%s.bmp",m_commonBitmapTable[i].filename);
//OutputDebugString("\nLoadBitmap : ");
//OutputDebugString(filename);

		m_commonBitmap[nm] = new CMyBitmap(filename);
		m_commonHBitmap[nm] = m_commonBitmap[nm]->GetHBitmap();
	}



	m_projectList = new CNameList();
	if (m_projectList->LoadFile(PROJECT_FILE_NAME) == FALSE)
	{
		CopyFile(PROJECT_FILE_NAME_ORG,PROJECT_FILE_NAME,FALSE);
		m_projectList->LoadFile(PROJECT_FILE_NAME);
	}



	int varType = GetVarType();
	m_varFileName = VAR_FILE_NAME;
	if (varType > 0)
	{
		m_varFileName = VAR_FILE_NAME2;
	}

	m_varList = new CNameList();
	if (m_varList->LoadFile(m_varFileName) == FALSE)
	{
		for (int i=0;i<100;i++)
		{
			LPSTR name = &m_varInitName[i][0];
			if ((*name) != 0)
			{
				m_varList->SetName(i,name);
			}
		}
		m_varList->SetModify();
	}
	else
	{
		for (int i=0;i<100;i++)
		{
			LPSTR name = &m_varInitName[i][0];
			if ((*name) != 0)
			{
				LPSTR name2 = m_varList->GetName(i);
				if (strcmp(name,name2) != 0)
				{
					m_varList->SetModify();
					m_varList->SetName(i,name);
				}
			}
		}
	}




	//1000こにする
	int varMax = GetVarKosuu();
	m_varList->AdjustNameKosuu(varMax);



	m_varInitData = new CVarInitData();
	if (varType == 0)
	{
		m_varInitData->Load(VARINITDATA_FILE_NAME);
	}
	else
	{
		m_varInitData->Load(VARINITDATA_FILE_NAME2);
	}

	m_charaList = new CNameList();
	if (m_charaList->LoadFile(CHARA_FILE_NAME) == FALSE)
	{
		CopyFile(CHARA_FILE_NAME_ORG,CHARA_FILE_NAME,FALSE);
		m_charaList->LoadFile(CHARA_FILE_NAME);
	}

	m_faceList = new CNameList();
	if (m_faceList->LoadFile(FACE_FILE_NAME) == FALSE)
	{
		CopyFile(FACE_FILE_NAME_ORG,FACE_FILE_NAME,FALSE);
		m_faceList->LoadFile(FACE_FILE_NAME);
	}

	m_seList = new CNameList();
	if (m_seList->LoadFile(SE_FILE_NAME) == FALSE)
	{
		CopyFile(SE_FILE_NAME_ORG,SE_FILE_NAME,FALSE);
		m_seList->LoadFile(SE_FILE_NAME);
	}

	m_bgmList = new CNameList();
	if (m_bgmList->LoadFile(BGM_FILE_NAME) == FALSE)
	{
		CopyFile(BGM_FILE_NAME_ORG,BGM_FILE_NAME,FALSE);
		m_bgmList->LoadFile(BGM_FILE_NAME);
	}

	m_functionList = new CNameList();
	if (m_functionList->LoadFile(FUNCTION_FILE_NAME) == FALSE)
	{
		CopyFile(FUNCTION_FILE_NAME_ORG,FUNCTION_FILE_NAME,FALSE);
		m_functionList->LoadFile(FUNCTION_FILE_NAME);
	}
	CCommandData::m_functionList = m_functionList;

	m_commandList = new CNameList();
	if (m_commandList->LoadFile(COMMAND_FILE_NAME) == FALSE)
	{
		CopyFile(COMMAND_FILE_NAME_ORG,COMMAND_FILE_NAME,FALSE);
		m_commandList->LoadFile(COMMAND_FILE_NAME);
	}
	CCommandData::m_commandList = m_commandList;

	m_setCGList = new CNameList();
	if (m_setCGList->LoadFile(SETCG_FILE_NAME) == FALSE)
	{
		CopyFile(SETCG_FILE_NAME_ORG,SETCG_FILE_NAME,FALSE);
		m_setCGList->LoadFile(SETCG_FILE_NAME);
	}


	m_varControlList = new CNameList();
	if (m_varControlList->LoadFile(VARCONTROL_FILE_NAME) == FALSE)
	{
		CopyFile(VARCONTROL_FILE_NAME_ORG,VARCONTROL_FILE_NAME,FALSE);
		m_varControlList->LoadFile(VARCONTROL_FILE_NAME);
	}

	m_termFlag = GetConfig("termFlag");
	m_termList = new CNameList();
	if (m_termList->LoadFile(TERMLIST_FILE_NAME) == FALSE)
	{
		CopyFile(TERMLIST_FILE_NAME_ORG,TERMLIST_FILE_NAME,FALSE);
		m_varControlList->LoadFile(TERMLIST_FILE_NAME);
	}


	m_gameXTXList = new CNameList();
	m_gameXTXList->LoadInit("game");
	{
		int customParamN = m_gameXTXList->SearchName2("SpeedCustomParam");
		if (customParamN != -1)
		{
			int customParam = atol(m_gameXTXList->GetName(customParamN+1));
			CCalcuSpeed::m_customParam = customParam;

		}
	}


	m_balloonFlag = GetConfig("balloonFlag");
	m_zahyoPrintType = GetConfig("zahyoPrintType");

	//check
	if (1)
	{
		int kosuu = m_varControlList->GetNameKosuu() / 2;
		for (int i=2;i<kosuu;i++)
		{
			LPSTR controlVarName = m_varControlList->GetName(i*2+1);
			int found = -1;
			int n = m_varList->GetNameKosuu();
			for (int k=0;k<n;k++)
			{
				LPSTR varName = m_varList->GetName(k);
				if (strcmp(controlVarName,varName) == 0)
				{
					found = k;
					break;
				}
			}

			if (found == -1)
			{
				MessageBox(NULL,controlVarName,"varControl.txt内の変数名がありません",MB_OK | MB_ICONEXCLAMATION);
			}
		}
	}

	m_effect = new CEffect();

	for (i=0;i<32;i++)
	{
		CPicture* lpPic = m_effect->GetPicture(i);
		if (lpPic != NULL)
		{
			lpPic->ReSize(screenSizeX,screenSizeY);
		}
	}

	m_scriptData = new CScriptData();

	m_undoObject = NULL;

	m_messageWindow = new CCommonMessageWindow();





//	m_miniCompiler = new CMiniCompiler();

//	m_miniCompiler->SetCommandList(m_commandList);
//	m_miniCompiler->SetFunctionList(m_functionList);
//	m_miniCompiler->SetVarList(m_varList);


	WNDCLASSEX wc;

	// フレームウィンドウのクラスの登録
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW | (CS_DBLCLKS*0);
	wc.lpfnWndProc   = (WNDPROC)CMyFrameWindow::staticFrameWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hInstance;
//	wc.hIcon         = NULL;//LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MAIN));
	wc.hIcon         = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_ICON_NNNEDIT));
	wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
//	wc.lpszMenuName  = "DUMMY_MENU";
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU_FRAMEWINDOW);
	wc.lpszClassName = MYCLASSNAME;
	wc.hIconSm       = NULL;//LoadImage(hInstance,MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!RegisterClassEx(&wc)) return;





              // Ｈｅｌｌｏ　ＭＤＩ子ウィンドウのクラスの登録
	wc.lpfnWndProc   = (WNDPROC)CMyView::staticViewWndProc;
	wc.cbWndExtra    = sizeof(LONG);
//	wc.hIcon         = NULL;//LoadIcon(hInstance,MAKEINTRESOURCE(IDI_HELLO));
	wc.hIcon         = LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_ICON_NNNEDIT));
	wc.lpszClassName = MYCHILDCLASSNAME;//pHelloClass;
	wc.hIconSm       = NULL;//LoadImage(hInstance,MAKEINTRESOURCE(IDI_HELLO),  IMAGE_ICON,16,16,LR_DEFAULTCOLOR);

	if (!RegisterClassEx(&wc)) return;



	m_frame = new CMyFrameWindow(this);
	m_frameHWND = m_frame->GetHWND();
	if (m_frameHWND == NULL) return;

	m_clientHWND = m_frame->GetMyClientHWND();



//	m_buttonPic = new CButtonPic(m_frameHWND,m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_BUTTON));
//	m_effectPic = new CEffectPic(m_frameHWND,m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_EFFECTMINI));

	m_mesWinPic = new CPicture();
	m_nameWinPic = new CPicture();
	CheckAndLoadMesWinPic();
	CheckAndLoadNameWinPic();

	m_commonDIB = new CCommonDIB();




	m_rubiPrintFlag = GetConfig("rubiPrintFlag");
	m_rubiBuildFlag = GetConfig("rubiBuildFlag"); 
	m_rubiMaker = new CRubiMaker(m_rubiPrintFlag | m_rubiBuildFlag);
	if (m_rubiPrintFlag)
	{
		CMyFont::m_rubiUseFlag = 1;
		int rubiPercent = GetConfig("rubiPercent");
		CMyFont::m_rubiPercent = rubiPercent;
	}

	SetRubiParam();

	m_kanjiCode = GetConfig("kanjiCode");
	if (m_kanjiCode == 1)
	{
		CMyFont::m_codeByte = 1;
	}

	m_okikaeData = new COkikaeData();
	CMyMessage::m_okikaeData = m_okikaeData;

	
	m_font = new CMyFont(m_clientHWND);
	m_rubiFont = new CRubiFont(m_clientHWND);
	m_message = new CMyMessage(m_font,m_rubiFont);


	m_input = new CMyInputDialog(m_frameHWND,m_hInstance,1024);
//	m_search = new CMySearchDialog(m_frameHWND,m_hInstance,1024);




	m_defaultControl = new CDefaultControl(this);
	m_volumeControl = new CVolumeControl(m_input,m_nnnConfig);

	m_effectCopyMoveFlag = GetConfig("effectCopyMoveFlag");




	CMessageData::SetWhiteSpace(GetConfig("daihonWhiteSpaceNumber"));




	m_directSound = new CMyDirectSound(m_frameHWND);
	m_directSound->SetPrimaryFormat(2,44100,16);
	m_directSound->Start(FALSE);
	
	m_scriptSoundControl = new CScriptSoundControl2(this);
	m_scriptVoiceControl = new CScriptVoiceControl2(this,GetConfig("voicePack"));
	m_musicControl = new CMusicControl(m_directSound);


	m_menuCheckControl = new CMenuCheckControl(this);


	ShowWindow(m_frameHWND,SW_SHOW);


	m_startTitle =  new CStartTitle(m_clientHWND,m_hInstance);
	m_startTitle->Show();
	m_startTitle->Start();

	m_balloon = new CBalloon(m_frameHWND,m_hInstance);


	m_file = new CMyFileOpen(m_frameHWND,m_hInstance,"nnn");
	m_selectDialog = new CSelectDialog(m_frameHWND,m_hInstance);



	m_overrapPic = new CPicture(screenSizeX,screenSizeY);
	m_overrapPic2 = new CPicture(screenSizeX,screenSizeY);

	m_document[MAINSCREEN_WINDOW] = new CMainScreenDoc(this);
	((CMainScreenDoc*)m_document[MAINSCREEN_WINDOW])->SetMessageSetup(m_messageSetup);

	m_document[FILM_WINDOW] = new CFilmDoc(this);
	m_document[LAYER_WINDOW] = new CLayerDoc(this);
	m_document[ZAHYO_WINDOW] = new CZahyoDoc(this);		
	m_document[GAMEMESSAGE_WINDOW] = new CGameMessageDoc(this);	
	m_document[EFFECT_WINDOW] = new CEffectDoc(this);	

	m_document[STORY_WINDOW] = new CStoryDoc(this);	
	m_document[CONTROL_WINDOW] = new CControlDoc(this);	

	m_document[FILMCASE_WINDOW] = new CFilmCaseDoc(this);
	m_document[STORYBOOK_WINDOW] = new CStoryBookDoc(this);

	m_document[VAR_WINDOW] = new CVarDoc(this);
	m_document[PROGRAM_WINDOW] = new CProgramDoc(this);

	m_document[CONTE_WINDOW] = new CConteDoc(this);
	m_document[CONTEMESSAGE_WINDOW] = new CConteMessageDoc(this);



	m_autoVoice = new CAutoVoice(this);
	m_lockVoice = new CLockVoice(this);
	m_deleteVoice = new CDeleteVoice(this);
	m_makeBook = new CMakeBook(this);
	m_makeUseVawBat = new CMakeUseVawBat(this);
	m_makeCheckVawBat = new CMakeCheckVawBat(this);
	m_makeUseDWQBat = new CMakeUseDWQBat(this);
	m_makeCheckDWQBat = new CMakeCheckDWQBat(this);

	m_changeBGMList = new CChangeBGMList(this);
	m_changeSeList = new CChangeSeList(this);





	//NewScript();
	//configしだい
	if (0)
	{
//		NewScript();
	}
	else
	{
		LPSTR filenameonly = m_nnnConfig->GetText("lastEditFileNameOnly");

		char currentDir[1024];
		GetCurrentDirectory(1024,currentDir);


		char allFileName[2048];
		wsprintf(allFileName,"%s\\nnndir\\nnn\\%s",currentDir,filenameonly);


//@@		m_file->InitFileName(filename);
		m_file->InitFileName(allFileName);

//@@		FILE* file = fopen(filename,"rb");
		FILE* file = CMyFile::Open(allFileName,"rb");
		if (file != NULL)
		{
			LoadRoutine(file);
		}
		else
		{
			SetModify(FALSE);
			NewScript();
		}
	}



	m_windowMode = m_nnnConfig->GetValue("windowMode");

	if (m_windowMode == 0)
	{
		m_frame->CheckButtonAndMenu(ID_BUTTON_FILMMODE,ID_MENU_FILMMODE);
	}
	else
	{
		m_frame->CheckButtonAndMenu(ID_BUTTON_STORYMODE,ID_MENU_STORYMODE);
	}



















	StoryIsChanged();
	FilmIsChanged();


	MoveAllWindow();

	int frame = m_nnnConfig->GetValue("frameTime");
	if (frame > 0)
	{
		m_defaultFrameTime = frame;
		SetDefaultFrameTime();
	}

//	m_aviSave = new CAviSave(screenSizeX,screenSizeY);

	m_movieCreate = new CMovieCreate(this);

	m_menuCheckControl->CheckAllMenu();
	m_messageSerialControl = new CMessageSerialControl(this);


	m_scriptCompiler = new CScriptCompiler(this);
	if (1)
	{
		int md = GetConfig("bufferSetCG");
		m_scriptCompiler->SetBufferSetCG(md);
	}




	m_sameMessageDeutch = new CSameMessageDeutch(this);
	m_searchOrReplace = new CSearchOrReplace(this);
	m_windowUpdateControl = new CWindowUpdateControl(this);

	m_copyPreEffect = new CCopyPreEffect(this);
	m_checkDWQ = new CCheckDWQ(this);
	m_deleteNonVoice = new CDeleteNonVoice(this);

	m_frameChange = new CFrameChange(this);

	StoryIsChanged();
	FilmIsChanged();

	m_onTimer = new CMyOnTimer(this);
DebugStart();
}



CMyApplication::~CMyApplication()
{
	End();
}

void CMyApplication::End(void)
{
	if (m_nnnConfig != NULL)
	{
		BOOL old = GetConfig("sameLoadFlag");
		BOOL flg = CMyFileOpen::GetSameZahyoFlag();
		if (old != flg)
		{
			SetConfig("sameLoadFlag",flg);
		}
	}


	ENDDELETECLASS(m_onTimer);
	ENDDELETECLASS(m_frameChange);
	ENDDELETECLASS(m_deleteNonVoice);
	ENDDELETECLASS(m_checkDWQ);
	ENDDELETECLASS(m_copyPreEffect);
	ENDDELETECLASS(m_windowUpdateControl);
	ENDDELETECLASS(m_searchOrReplace);
	ENDDELETECLASS(m_sameMessageDeutch);
	ENDDELETECLASS(m_scriptCompiler);
	ENDDELETECLASS(m_messageSerialControl);
	ENDDELETECLASS(m_movieCreate);

//	ENDDELETECLASS(m_aviSave);

	ENDDELETECLASS(m_projectList);
//	DELETEARRAY(m_sceneWork);
//	DELETEARRAY(m_charaOffsetTable);
//	ENDDELETECLASS(m_sceneDataList);
//	for (int i=0;i<100;i++)
//	{
//		ENDDELETECLASS(m_cgDataList[i]);
//	}
	ENDDELETECLASS(m_faceList);
	ENDDELETECLASS(m_charaList);
	ENDDELETECLASS(m_seList);
	ENDDELETECLASS(m_functionList);
	ENDDELETECLASS(m_commandList);
	ENDDELETECLASS(m_bgmList);
	ENDDELETECLASS(m_setCGList);
	ENDDELETECLASS(m_termList);
	ENDDELETECLASS(m_varControlList);
	ENDDELETECLASS(m_gameXTXList);

	ENDDELETECLASS(m_menuCheckControl);
	ENDDELETECLASS(m_musicControl);

	ENDDELETECLASS(m_scriptVoiceControl);
	ENDDELETECLASS(m_scriptSoundControl);

	if (m_directSound != NULL)
	{
		m_directSound->Stop();
	}
	ENDDELETECLASS(m_directSound);

	ENDDELETECLASS(m_volumeControl);
	ENDDELETECLASS(m_defaultControl);
//	ENDDELETECLASS(m_search);
	ENDDELETECLASS(m_input);

	if (m_varInitData != NULL)
	{
		if (m_varInitData->CheckModify())
		{
			int rt = MessageBox(NULL,"変数初期値が変更されています。\n保存しますか。\n保存することをすすめるにゃ","確認",MB_YESNO | MB_ICONEXCLAMATION);
			if (rt == IDYES)
			{
				SaveVarInitData();
			}
		}
		ENDDELETECLASS(m_varInitData);
	}

	if (m_varList != NULL)
	{
		BOOL flg = FALSE;
		if (m_varList->CheckModify()) flg = TRUE;
		CVarDoc* varDoc = (CVarDoc*)m_document[VAR_WINDOW];
		if (varDoc != NULL)
		{
			if (varDoc->CheckMarkEdit()) flg = TRUE;
		}

		if (flg)
		{
			int rt = MessageBox(NULL,"変数名が変更されています。\n保存しますか。\n保存することをすすめるにゃ","確認",MB_YESNO | MB_ICONEXCLAMATION);
			if (rt == IDYES)
			{
				SaveVarName();
				if (varDoc != NULL)
				{
					varDoc->SaveMarkColor();
				}
			}
		}

		ENDDELETECLASS(m_varList);
	}




//DebugLog("[2]");
	
	//delete doc
	ENDDELETECLASS(m_balloon);
	ENDDELETECLASS(m_startTitle);

	int i;
	for (i=0;i<MDIWINDOW_KOSUU_MAX;i++)
	{
		CMyDocument* doc = m_document[i];
		if (doc != NULL)
		{
			//get zahyo
			CMyView* view = doc->GetView();
			if (view != NULL)
			{
				RECT rc;
				view->GetWindowZahyo(&rc);
				if (view != NULL)
				{
////////////////////					m_config->SetWindowZahyo(i,&rc);

//					char mes[256];
//					sprintf(mes,"[%d %d]",rc.left,rc.top);
//					OutputDebugString(mes);
				}

			}
		}

		ENDDELETECLASS(m_document[i]);
	}

//	for (i=0;i<100;i++)
//	{
//		ENDDELETECLASS(m_setCGNameList[i]);
//	}



	ENDDELETECLASS(m_overrapPic);
	ENDDELETECLASS(m_overrapPic2);


	ENDDELETECLASS(m_frame);

	ENDDELETECLASS(m_changeSeList);
	ENDDELETECLASS(m_changeBGMList);

	ENDDELETECLASS(m_makeCheckDWQBat);
	ENDDELETECLASS(m_makeUseDWQBat);
	ENDDELETECLASS(m_makeCheckVawBat);
	ENDDELETECLASS(m_makeUseVawBat);
	ENDDELETECLASS(m_makeBook);
	ENDDELETECLASS(m_deleteVoice);
	ENDDELETECLASS(m_lockVoice);
	ENDDELETECLASS(m_autoVoice);

	for (i=0;i<50;i++)
	{
		ENDDELETECLASS(m_commonBitmap[i]);
	}

	ENDDELETECLASS(m_effect);

	ENDDELETECLASS(m_messageWindow);


	ENDDELETECLASS(m_selectDialog);
	ENDDELETECLASS(m_file);

	ENDDELETECLASS(m_undoObject);
	ENDDELETECLASS(m_scriptData);

	ENDDELETECLASS(m_message);
	ENDDELETECLASS(m_rubiFont);
	ENDDELETECLASS(m_font);
	ENDDELETECLASS(m_okikaeData);

	ENDDELETECLASS(m_nameWinPic);
	ENDDELETECLASS(m_mesWinPic);
	ENDDELETECLASS(m_commonDIB);

	ENDDELETECLASS(m_selObj);


//@@@	ENDDELETECLASS(m_configMenu);

	ENDDELETECLASS(m_cutinNameData);
//	ENDDELETECLASS(m_cutinControl);

	ENDDELETECLASS(m_messageSetup);
	ENDDELETECLASS(m_allEffectParam);
	ENDDELETECLASS(m_shakinControl);
	ENDDELETECLASS(m_myGraphics);


	ENDDELETECLASS(m_nnnConfig);
}


BOOL CMyApplication::SaveVarName(void)
{
	if (m_varList == NULL) return FALSE;


	//1000こにする




	BOOL b = m_varList->SaveFile(m_varFileName);

	m_varList->SetModify(FALSE);

	CVarDoc* varDoc = (CVarDoc*)m_document[VAR_WINDOW];
	if (varDoc != NULL)
	{
		varDoc->SaveMarkColor();
	}

	return b;
}


BOOL CMyApplication::SaveVarInitData(void)
{
	int type = GetVarType();
	if (type == 0)
	{
		BOOL b = m_varInitData->Save(VARINITDATA_FILE_NAME);
		return b;
	}
	BOOL b = m_varInitData->Save(VARINITDATA_FILE_NAME2);
	return b;
}



void CMyApplication::SetNextPrint(BOOL flg)
{
	m_nextPrintFlag = flg;
}



int CMyApplication::OnTimer(int n)
{
	if (m_onTimer == NULL) return 0;

	if (m_shakinControl != NULL)
	{
		m_shakinControl->BeginFrame(n);
	}

	int rt = m_onTimer->OnTimer(n);

	if (m_shakinControl != NULL)
	{
		m_shakinControl->EndFrame();
	}

	if (m_balloon != NULL)
	{
		m_balloon->OnTimer();
	}

	return rt;
}

void CMyApplication::FlipToScreen(void)
{
	((CMainScreenDoc*)m_document[MAINSCREEN_WINDOW])->FlipToScreen();
}

void CMyApplication::PrintOverrap(int type,int count,int countMax)
{
	m_effect->PrintSimpleWipe(type,m_overrapPic,m_overrapPic2,count,countMax);
}


void CMyApplication::OnGameClick(void)
{
	//message end?

	BOOL b = FALSE;

	CGameMessageDoc* pDoc = (CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW];
	CMessageData* pMessage = (CMessageData*)(pDoc->GetNowSelectMessage());
	if (pMessage == NULL)
	{
		b = TRUE;
	}
	else
	{
		int md = pMessage->GetMessageMode();
		if (md != 3)
		{
			b = TRUE;
		}
		else
		{
			m_gameCount = pMessage->GetDrawCount();
		}
	}

	if (b)
	{
		OnNextMessage();
	}
}



void CMyApplication::OnNextMessage(void)
{
//OutputDebugString("@");

	CGameMessageDoc* pMessageDoc = (CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW];
	if (pMessageDoc == NULL) return;

	CKomaData* pKoma = pMessageDoc->GetNowSelectKoma();	//へん
	if (pKoma == NULL) return;

	m_gameCount = 0;

	int n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();

	n++;
	if (n<kosuu)
	{
		pKoma->SetSelectNumber(n);

//OutputDebugString("OnNextMessage():nextMessage");
		CheckAndPlaySound();



		MessageIsChanged();
		return;
	}

	SetNewEffect(1);
//OutputDebugString("OnNextMessage():nextKoma");
	CheckAndPlaySound();

/*


	CFilmDoc* pFilmDoc = (CFilmDoc*)m_document[FILM_WINDOW];
	if (pFilmDoc == NULL) return;

	CFilmData* pFilm = pFilmDoc->GetNowSelectFilm();
	int n2 = pFilm->GetNowSelectNumber();
	int kosuu2 = pFilm->GetObjectKosuu();

	n2++;
	if (n2<kosuu2)
	{
		BOOL overrapFlag = FALSE;

		pFilm->SetSelectNumber(n2);

		pKoma = pFilm->GetKoma(n2);
		if (pKoma != NULL)
		{
			pKoma->SetSelectNumber(0);
			if (pKoma->GetOverrapCount() > 0)
			{
				CAreaControl::SetNextAllPrint();
				m_effect->BeginEffect();
				m_effect->Print(FALSE,TRUE);
				m_effect->EndEffect();
				m_overrapPic->GetScreen(0,0,640,480);

				//next pic

				pKoma->SetAllEffect();
				pKoma->LoadAllPicture();

				CAreaControl::SetNextAllPrint();
				m_effect->BeginEffect();
				m_effect->Print(FALSE,TRUE);
				m_effect->EndEffect();
				m_overrapPic2->GetScreen(0,0,640,480);

				m_overrapPic->Put(0,0,FALSE);
				overrapFlag = TRUE;

			}
		}

		m_noUpdateScreenFlag = overrapFlag;
		KomaIsChanged();
		m_noUpdateScreenFlag = FALSE;
		m_effect->ResetCount();
		return;
	}
	*/
}


//OutputDebugString("@");
void CMyApplication::SetNewEffect(int nxt)
{
	CGameMessageDoc* pMessageDoc = (CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW];
	if (pMessageDoc == NULL) return;

	CKomaData* pKoma = pMessageDoc->GetNowSelectKoma();	//へん
	if (pKoma == NULL) return;

	CFilmDoc* pFilmDoc = (CFilmDoc*)m_document[FILM_WINDOW];
	if (pFilmDoc == NULL) return;

	CFilmData* pFilm = pFilmDoc->GetNowSelectFilm();
	int n2 = pFilm->GetNowSelectNumber();
	int kosuu2 = pFilm->GetObjectKosuu();

//	n2++;
	n2 += nxt;

	if (n2<kosuu2)
	{
		BOOL overrapFlag = FALSE;

		pFilm->SetSelectNumber(n2);

		pKoma = pFilm->GetKoma(n2);
		if (pKoma != NULL)
		{
			int screenSizeX = CMyGraphics::GetScreenSizeX();
			int screenSizeY = CMyGraphics::GetScreenSizeY();

			int frame = pKoma->GetFrameTime();
			if (frame == -1)
			{
				SetDefaultFrameTime();
			}
			else if (frame > 0)
			{
				SetFrameTime(frame);
			}

			pKoma->SetSelectNumber(0);
			if (pKoma->GetOverrapCount() > 0)
			{
				CAreaControl::SetNextAllPrint();
				m_effect->BeginEffect();
				m_effect->Print(FALSE,TRUE);
				m_effect->EndEffect();
				m_overrapPic->GetScreen(0,0,screenSizeX,screenSizeY);

				//next pic



				m_effect->ResetCount();	//debug 2004-01-28	(追加)

				pKoma->SetAllEffect();
				pKoma->LoadAllPicture();


				
				CAreaControl::SetNextAllPrint();
				m_effect->BeginEffect();
				m_effect->CalcuOnly();	//	debug 2004-01-28	(追加)
				m_effect->Print(FALSE,TRUE);
				m_effect->EndEffect();
				m_overrapPic2->GetScreen(0,0,screenSizeX,screenSizeY);

				m_overrapPic->Put(0,0,FALSE);
				overrapFlag = TRUE;

			}
			else
			{
//OutputDebugString("SetNewEffect()");
//				CheckAndPlaySound();
			}
		}

		m_noUpdateScreenFlag = overrapFlag;
		KomaIsChanged();

		m_noUpdateScreenFlag = FALSE;

		if (pKoma == NULL)
		{
			m_effect->ResetCount();
		}
		else
		{
			for (int i=0;i<16;i++)
			{
				int eff = pKoma->GetEffect(i);
				if (eff != EFFECT_MAE)
				{
					m_effect->ResetCount(i);
				}
			}
		}

		return;
	}

	//last

	//storyMode?
	if (GetFilmPlayMode())
	{
		//exist next?
		CFilmCaseDoc* pFilmCase = (CFilmCaseDoc*)(m_document[FILMCASE_WINDOW]);
		
		CFilmCaseData* pCase = pFilmCase->GetFilmCaseData();

		int filmKosuu = pCase->GetObjectKosuu();

		int nowFilm = pFilmCase->GetNowSelectNumber();
		if (nowFilm < filmKosuu-1)
		{
			pFilmCase->OnSelectFilm(nowFilm+1);

			((CFilmDoc*)m_document[FILM_WINDOW])->OnClickFilm(0,0);
			((CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW])->OnSelectMessage(0,0);

			return;
		}
	}



	SetPlayMode(FALSE);
}






void CMyApplication::MoveAllWindow(void)
{
//	BOOL md = m_configMenu->GetPara("windowMode");
	BOOL md = m_nnnConfig->GetValue("windowMode");

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	for (int i=0;i<MDIWINDOW_KOSUU_MAX;i++)
	{
		CMyDocument* pDoc = m_document[i];
		if (pDoc != NULL)
		{
			CMyView* pView = pDoc->GetView();
			if (pView != NULL)
			{
				RECT rc;
				
//				pView->ReCalcuScrollPara();
//				m_config->GetWindowZahyo(i,&rc,m_windowMode);
//				BOOL flg = m_config->GetWindowFlag(i,m_windowMode);

				BOOL flg;

				LPSTR windowName = pView->GetMyViewWindowName();

				m_nnnConfig->GetWindowZahyo(windowName,md,&rc,&flg);


//				m_configMenu->GetWindowZahyo(i,md,&rc,&flg);
				m_frame->CheckWindow(i,flg);



				//adjust mainscreen





				pView->MoveViewWindow(&rc);
				pView->Show(flg);
			}
		}
	}

/*
	for (int i=0;i<MDIWINDOW_KOSUU_MAX;i++)
	{
		CMyDocument* pDoc = m_document[i];
		if (pDoc != NULL)
		{
			CMyView* pView = pDoc->GetView();
			if (pView != NULL)
			{
				RECT rc;
				
				pView->ReCalcuScrollPara();


				m_config->GetWindowZahyo(i,&rc,m_windowMode);
				BOOL flg = m_config->GetWindowFlag(i,m_windowMode);

				if ((rc.right > 0) && (rc.bottom > 0))
				{
					RECT rc2;
					pView->GetWindowZahyo(&rc2);
					rc.right = rc2.right;
					rc.bottom = rc2.bottom;
					pView->MoveViewWindow(&rc);

					m_frame->CheckWindow(i,flg);
					pView->Show(flg);
				}
			}
		}
	}
*/
}



void CMyApplication::OnControlKey(int windowNumber,WPARAM wParam,LPARAM lParam)
{
	if (wParam == 6)
	{
		m_searchOrReplace->SearchOrReplace(FALSE,FALSE);
	}
	if (wParam == 19)
	{
		if (Save())
		{
			CheckAndSetProject();
		}
	}
	if (wParam == 14)
	{
		if (CheckSave())
		{
			NewScript();
			MoveAllWindow();
		}
		//N
	}
	if (wParam == 15)
	{
		//O
		if (CheckSave())
		{
			if (Load())
			{
				CheckAndSetProject();
			}
			MoveAllWindow();
		}
	}

	if (wParam == 26)
	{
		//undo 
	}

}

void CMyApplication::OnCommand(WPARAM wParam,LPARAM lParam)
{
	int oldData;
	int newData;


	switch (wParam)
	{
	case ID_BUTTON_NEW:
	case ID_MENU_FILE_NEW:
		if (CheckSave() == FALSE) break;
		NewScript();
		MoveAllWindow();
		break;

	case ID_BUTTON_OPEN:
	case ID_MENU_FILE_OPEN:
		if (CheckSave() == FALSE) break;
		if (Load())
		{
			CheckAndSetProject();
		}
		MoveAllWindow();
		break;

	case ID_BUTTON_SAVE:
	case ID_MENU_FILE_SAVE:
		if (Save())
		{
			CheckAndSetProject();
		}
		break;


	case ID_BUTTON_FILMMODE:
	case ID_MENU_FILMMODE:
		m_windowMode = 0;
		m_nnnConfig->SetValue("windowMode",m_windowMode);
		MoveAllWindow();
		break;
	case ID_BUTTON_STORYMODE:
	case ID_MENU_STORYMODE:
		m_windowMode = 1;
		m_nnnConfig->SetValue("windowMode",m_windowMode);
		MoveAllWindow();
		break;

	case ID_MENU_CONFIG:
		ConfigMenu();
		break;

	case ID_SCREENSIZE_640x480:
		ChangeScreenSize(0,640,480);
		break;
	case ID_SCREENSIZE_800x600:
		ChangeScreenSize(1,800,600);
		break;
	case ID_SCREENSIZE_1024x768:
		ChangeScreenSize(2,1024,768);
		break;


	case ID_SCREENSIZE_1280x720:
		ChangeScreenSize(20,1280,720);
		break;
	case ID_SCREENSIZE_1280x960:
		ChangeScreenSize(3,1280,960);
		break;
	case ID_SCREENSIZE_1280x1024:
		ChangeScreenSize(4,1280,1024);
		break;
	case ID_SCREENSIZE_1600x1200:
		ChangeScreenSize(5,1600,1200);
		break;
	case ID_SCREENSIZE_1920x1080:
		ChangeScreenSize(6,1920,1080);
		break;
	case ID_SCREENSIZE_1920x1200:
		ChangeScreenSize(7,1920,1200);
		break;


	case ID_SCREENSIZE_320x480:
		ChangeScreenSize(8,320,480);
		break;
	case ID_SCREENSIZE_320x460:
		ChangeScreenSize(9,320,460);
		break;
	case ID_SCREENSIZE_480x320:
		ChangeScreenSize(10,480,320);
		break;
	case ID_SCREENSIZE_480x300:
		ChangeScreenSize(11,480,300);
		break;

	case ID_SCREENSIZE_640x960:
		ChangeScreenSize(12,640,960);
		break;
	case ID_SCREENSIZE_640x920:
		ChangeScreenSize(13,640,920);
		break;
	case ID_SCREENSIZE_960x640:
		ChangeScreenSize(14,960,640);
		break;
	case ID_SCREENSIZE_960x600:
		ChangeScreenSize(15,960,600);
		break;


	case ID_SCREENSIZE_768x1024:
		ChangeScreenSize(16,768,1024);
		break;
	case ID_SCREENSIZE_768x1004:
		ChangeScreenSize(17,768,1004);
		break;
//	case ID_SCREENSIZE_1024x768:
//		ChangeScreenSize(18,768,1024);
//		break;
	case ID_SCREENSIZE_1024x748:
		ChangeScreenSize(19,1024,748);
		break;




	case ID_MESSAGEWIDTHGUIDE:
		ChangeMessageGuide();
		break;
	case ID_CHANGEMESSAGEGYO:
		ChangeMessageGyo();
		break;


	case ID_MENU_SET_KOMASM_ON:
		SetConfig("noSavePicFlag",0);
		CKomaData::m_smSaveFlag = TRUE;
		m_menuCheckControl->SetMiniPicMenuCheck();
		break;
	case ID_MENU_SET_KOMASM_OFF:
		SetConfig("noSavePicFlag",1);
		CKomaData::m_smSaveFlag = FALSE;
		m_menuCheckControl->SetMiniPicMenuCheck();
		break;
	case ID_MENU_SET_NNNSM_OFF:
		SetConfig("noSaveNNNPicFlag",1);
		m_menuCheckControl->SetMiniPicMenuCheck();
		break;
	case ID_MENU_SET_NNNSM_ON:
		SetConfig("noSaveNNNPicFlag",0);
		m_menuCheckControl->SetMiniPicMenuCheck();
		break;
	case ID_MENU_INPUT_MAXTEXTEDIT:
		oldData = GetConfig("inputTextLimitNT");
		if (m_input->GetNumber(oldData,&newData))
		{
			if ((newData >= 32000) && (newData <= 256000))
			{
				SetConfig("inputTextLimitNT",newData);
				PrintNextTimeSetup();
			}
		}
		break;
	case ID_MENU_SET_FRAMEOFKOMA_DIRECT:
		SetConfig("frameOfKomaType",0);
		CKomaData::SetFrameOfKomaType(0);
		m_menuCheckControl->SetMiniPicMenuCheck();
		break;
	case ID_MENU_SET_FRAMEOFKOMA_PERCENT:
		SetConfig("frameOfKomaType",1);
		CKomaData::SetFrameOfKomaType(1);
		m_menuCheckControl->SetMiniPicMenuCheck();
		break;
	case ID_MENU_INPUT_FRAMEOFKOMA:
		oldData = GetConfig("frameOfKoma");
		if (m_input->GetNumber(oldData,&newData))
		{
			if (newData >= 0)
			{
				if (GetConfig("frameOfKomaType") == 1)
				{
					if (newData>100) newData = 100;
				}

				SetConfig("frameOfKoma",newData);
				CKomaData::SetFrameOfKoma(newData);
			}
		}

		break;

	case ID_MENU_SET_KOMAOFFILM_DIRECT:
		SetConfig("komaOfFilmType",0);
		CFilmData::SetKomaOfFilmType(0);
		m_menuCheckControl->SetMiniPicMenuCheck();
		break;
	case ID_MENU_SET_KOMAOFFILM_PERCENT:
		SetConfig("komaOfFilmType",1);
		CFilmData::SetKomaOfFilmType(1);
		m_menuCheckControl->SetMiniPicMenuCheck();
		break;
	case ID_MENU_INPUT_KOMAOFFILM:
		oldData = GetConfig("komaOfFilm");
		if (m_input->GetNumber(oldData,&newData))
		{
			if (newData >= 0)
			{
				if (GetConfig("komaOfFilmType") == 1)
				{
					if (newData>100) newData = 100;
				}

				SetConfig("komaOfFilm",newData);
				CFilmData::SetKomaOfFilm(newData);
			}
		}
		break;

	case ID_MENU_SET_VARBLOCK1:
		ChangeVarBlock(0);
		break;
	case ID_MENU_SET_VARBLOCK2:
		ChangeVarBlock(1);
		break;
	case ID_MENU_SET_VARBLOCK3:
		ChangeVarBlock(2);
		break;
	case ID_MENU_SET_VARBLOCK4:
		ChangeVarBlock(3);
		break;


	case ID_MENU_WARNING_DEL_SC_ONOFF:
		SetWarningDelScene(-1);
		break;
//	case ID_MENU_WARNING_DEL_SC_ON:
//		SetWarningDelScene(1);
//		break;
//	case ID_MENU_WARNING_DEL_SC_OFF:
//		SetWarningDelScene(0);
//		break;

	case ID_MENU_WARNING_DEL_CM_ONOFF:
		SetWarningDelCommand(-1);
		break;
//	case ID_MENU_WARNING_DEL_CM_ON:
//		SetWarningDelCommand(1);
//		break;
//	case ID_MENU_WARNING_DEL_CM_OFF:
//		SetWarningDelCommand(0);
//		break;

	case ID_MENU_WARNING_DEL_FL_ONOFF:
		SetWarningDelFilm(-1);
		break;
//	case ID_MENU_WARNING_DEL_FL_ON:
//		SetWarningDelFilm(1);
//		break;
//	case ID_MENU_WARNING_DEL_FL_OFF:
//		SetWarningDelFilm(0);
//		break;


	case ID_MENU_WARNING_DEL_KM_ONOFF:
		SetWarningDelKoma(-1);
		break;
//	case ID_MENU_WARNING_DEL_KM_ON:
//		SetWarningDelKoma(1);
//		break;
//	case ID_MENU_WARNING_DEL_KM_OFF:
//		SetWarningDelKoma(0);
//		break;


	case ID_MENU_WARNING_DEL_MS_ONOFF:
		SetWarningDelMessage(-1);
		break;
//	case ID_MENU_WARNING_DEL_MS_ON:
//		SetWarningDelMessage(1);
//		break;
//	case ID_MENU_WARNING_DEL_MS_OFF:
//		SetWarningDelMessage(0);
//		break;

	case ID_MENU_WARNING_DEL_PR_ONOFF:
		SetWarningDelParam(-1);
		break;
//	case ID_MENU_WARNING_DEL_PR_ON:
//		SetWarningDelParam(1);
//		break;
//	case ID_MENU_WARNING_DEL_PR_OFF:
//		SetWarningDelParam(0);
//		break;
	case ID_MENU_WARNING_DEL_VAR:
		ChangeWarningDelVar();
		break;

	case ID_MENU_SETCG_SELECT:
		SetAutoSetCGFlag(-1);
		break;
	case ID_MENU_SETCG_OFF:
		SetAutoSetCGFlag(0);
		break;
	case ID_MENU_SETCG_ON:
		SetAutoSetCGFlag(1);
		break;
	case ID_MENU_AUTOSCENE_OFF:
		SetSceneMakeFlag(0);
		break;
	case ID_MENU_AUTOSCENE_ON:
		SetSceneMakeFlag(1);
		break;
	case ID_MENU_CHECKSAMECG_OFF:
		SetCheckSameCGNameFlag(0);
		break;
	case ID_MENU_CHECKSAMECG_ON:
		SetCheckSameCGNameFlag(1);
		break;
	case ID_MENU_BUILDRUBI_OK:
		SetBuildRubiFlag(1);
		break;
	case ID_MENU_BUILDRUBI_NOT:
		SetBuildRubiFlag(0);
		break;
	case ID_MENU_BUILDRUBI_SELECT:
		SetBuildRubiFlag(-1);
		break;
	case ID_MENU_BUILDCUTIN_OK:
		SetBuildCutinFlag(1);
		break;
	case ID_MENU_BUILDCUTIN_NOT:
		SetBuildCutinFlag(0);
		break;
	case ID_MENU_BUILDCUTIN_SELECT:
		SetBuildCutinFlag(-1);
		break;
	case ID_MENU_TERM_ON:
		SetTermFlag(1);
		break;
	case ID_MENU_TERM_OFF:
		SetTermFlag(0);
		break;
	case ID_BALLOON_ON:
		ChangeBalloonFlag(1);
		break;
	case ID_BALLOON_OFF:
		ChangeBalloonFlag(0);
		break;
	case ID_ZAHYO_NORMAL:
		ChangeZahyoPrintType(0);
		break;
	case ID_ZAHYO_CENTER:
		ChangeZahyoPrintType(1);
		break;

	case ID_SETCGVAR_AUTO:
		SetCGVar(-1);
		break;
	case ID_SETCGVAR_NO:
		SetCGVar(0);
		break;
	case ID_SETCGVAR_SET:
		SetCGVar(1);
		break;


	case ID_TARGETMACHINE_WINDOWS:
		SetTargetMachine(0);
		break;
	case ID_TARGETMACHINE_IPHONE:
		SetTargetMachine(1);
		break;
	case ID_TARGETMACHINE_MAC:
		SetTargetMachine(2);
		break;

	case ID_PNG_GAMMA:
		ChangePngGamma();
		break;
	case ID_SCENECHECK_NO:
		SetSceneCheckMode(0);
		break;
	case ID_SCENECHECK_ERROR:
		SetSceneCheckMode(1);
		break;
	case ID_SCENECHECK_ERROROK:
		SetSceneCheckMode(2);
		break;
	case ID_MENU_BUFFERSETCG:
		ChangeBufferSetCG();
		break;


	case ID_CODE_KANJI:
		SetKanjiCode(0);
		break;
	case ID_CODE_ALPHA:
		SetKanjiCode(1);
		break;

	case ID_MENU_MESWINBYINIT:
		SetMesNameByInit(1);
		break;
	case ID_MENU_MESWINBYSETUP:
		SetMesNameByInit(0);
		break;
	case ID_MENU_MESWIN_NOTHING:
		SetMesPrintMode(0);
		break;
	case ID_MENU_MESWIN_BOX:
		SetMesPrintMode(1);
		break;
	case ID_MENU_MESWIN_PIC:
		SetMesPrintMode(2);
		break;
	case ID_MENU_MESWIN_SETSIZE:
		ChangeMesSize();
		break;
	case ID_MENU_MESWIN_SETPICFILENAME:
		ChangeMesFileName();
		break;
	case ID_MENU_MESWIN_SETPRINT:
		ChangeMesPrintZahyo();
		break;
	case ID_MENU_SETMESFONTSIZE:
		ChangeMesFontSize();
		break;
	case ID_MENU_SETMESNEXTY:
		ChangeMesNextY();
		break;
	case ID_MENU_MESWIN_SETLPRINT:
		ChangeMesLPrintZahyo();
		break;
	case ID_MENU_MESWIN_SETAPPEND:
		ChangeMesAppendZahyo();
		break;
		
	case ID_MENU_NAMEWIN_NOTHING:
		SetNamePrintMode(0);
		break;
	case ID_MENU_NAMEWIN_BOX:
		SetNamePrintMode(1);
		break;
	case ID_MENU_NAMEWIN_PIC:
		SetNamePrintMode(2);
		break;
	case ID_MENU_NAMEWIN_SETSIZE:
		ChangeNameSize();
		break;
	case ID_MENU_NAMEWIN_SETPRINT:
		ChangeNamePrintZahyo();
		break;
	case ID_MENU_SETNAMEFONTSIZE:
		ChangeNameFontSize();
		break;
	case ID_MENU_NAMEWIN_SETPICFILENAME:
		ChangeNameFileName();
		break;

	case ID_MENU_NAMEWIN_AUTOOFF:
		SetNameAutoFlag(0);
		break;
	case ID_MENU_NAMEWIN_ALWAYSPRINT:
		SetNameAutoFlag(1);
		break;

	case ID_MENU_SETMUSICVOLUME:
		//SetMusicVolume();
		m_volumeControl->SetMusicVolume();
		break;
	case ID_MENU_SETSOUNDVOLUME:
		m_volumeControl->SetSoundVolume();
		//SetSoundVolume();
		break;
	case ID_MENU_SETVOICEVOLUME:
		m_volumeControl->SetVoiceVolume();
		//SetVoiceVolume();
		break;
	case ID_MENU_SETSOUNDVOICEVOLUME:
		m_volumeControl->SetSoundVoiceVolume();
//		SetSoundVoiceVolume();
		break;


	case ID_MENUITEM_EFFECTCOPY_MOVE:
		SetEffectMenuMoveFlag(1);
		break;
	case ID_MENUITEM_EFFECTCOPY_KOTEI:
		SetEffectMenuMoveFlag(0);
		break;

	case ID_MENU_SETDEFAULTOVERRAPTIME:
		//SetDefaultOverrapTime();
		m_defaultControl->SetDefaultOverrapTime();
		break;
	case ID_MENU_SETDEFAULTBGLAYER:
		//SetDefaultBGLayer();
		m_defaultControl->SetDefaultBGLayer();
		break;
	case ID_MENU_SETDEFAULTTALAYER:
//		SetDefaultTALayer();
		m_defaultControl->SetDefaultTALayer();
		break;
	case ID_MENU_SETDEFAULTMOVIERATE:
		m_defaultControl->SetDefaultMovieRate();
		//SetDefaultMovieRate();
		break;
	case ID_MENU_SETDEFAULTMOVIESCALE:
		//SetDefaultMovieScale();
		m_defaultControl->SetDefaultMovieScale();
		break;

	case ID_MENU_MOVIE_DEFAULTSIZE:
		SetMovieSize(0);
		break;
	case ID_MENU_MOVIE_640x480:
		SetMovieSize(1);
		break;
	case ID_MENU_MOVIE_320x240:
		SetMovieSize(2);
		break;
	case ID_MENU_MOVIE_160x120:
		SetMovieSize(3);
		break;

	case ID_MENU_MOVIE_SPEEDPACK:
		SetMoviePackType(0);
		break;
	case ID_MENU_MOVIE_FULLPACK:
		SetMoviePackType(1);
		break;

	case ID_MENU_SELFILM_NO:
		SetFilmSelectMenuType(0);
		break;
	case ID_MENU_SELFILM_SEL:
		SetFilmSelectMenuType(1);
		break;
	case ID_MENU_SELFILM_INPUT:
		SetFilmSelectMenuType(2);
		break;

	case ID_MENU_SELSTORY_NO:
		SetStorySelectMenuType(0);
		break;
	case ID_MENU_SELSTORY_SEL:
		SetStorySelectMenuType(1);
		break;
	case ID_MENU_SELSTORY_INPUT:
		SetStorySelectMenuType(2);
		break;

	case ID_MENU_SELCALCU_NO:
		SetCalcuMenuType(0);
		break;
	case ID_MENU_SELCALCU_INPUT:
		SetCalcuMenuType(1);
		break;

	case ID_MENU_PERCENT_OVERRAP:
		ChangePercentOverrap();
		break;

	case ID_MENU_DELKEY_FILM:
		ChangeDeleteKey("deleteKeyFilm");
		break;
	case ID_MENU_DELKEY_KOMA:
		ChangeDeleteKey("deleteKeyKoma");
		break;
	case ID_MENU_DELKEY_MESSAGE:
		ChangeDeleteKey("deleteKeyMessage");
		break;
	case ID_MENU_DELKEY_STORY:
		ChangeDeleteKey("deleteKeyStory");
		break;
	case ID_MENU_DELKEY_COMMAND:
		ChangeDeleteKey("deleteKeyCommand");
		break;
	case ID_MENU_DELKEY_VAR:
		ChangeDeleteKey("deleteKeyVar");
		break;

	case ID_MENUITEM_ANGOU_XORCODE:
		ChangeAngouCode("angouXorCode");
		break;
	case ID_MENUITEM_ANGOU_XORADDCODE:
		ChangeAngouCode("angouXorAddCode");
		break;
	case ID_MENUITEM_ANGOU_ADDADDCODE:
		ChangeAngouCode("angouIncAddCode");
		break;
	case ID_MENUITEM_ANGOU_PRINT:
		PrintAngouCode();
		break;
	case ID_MENUITEM_ANGOU_CLEAR:
		ClearAngouCode();
		break;

	case ID_CUTIN_ENABLE:
		CutinEnable();
		break;
	case ID_CUTIN_DISABLE:
		CutinEnable(FALSE);
		break;

	case ID_VOICE_NORMAL:
		SetVoiceReadType(FALSE);
		break;
	case ID_VOICE_PACK:
		SetVoiceReadType(TRUE);
		break;

	case ID_RUBI_NOTPRINT:
		SetRubiPrint(0);
		break;
	case ID_RUBI_PRINT:
		SetRubiPrint(1);
		break;
	case ID_RUBI_HYO_PRINT:
		SetRubiPrint(2);
		break;

	case ID_RUBICOLOR:
		SetRubiColor();
		break;
	case ID_RUBICOLORFIX:
		ChangeRubiColorFix();
		break;
	case ID_RUBISIZE:
		SetRubiSize();
		break;
	case ID_RUBISUKIMA:
		SetRubiSukima();
		break;
	case ID_RUBIDELTAY:
		SetRubiDeltaY();
		break;
	case ID_JPEG_MMX:
		SetJpegMMX();
		break;
	case ID_JPEG_FLOAT:
		SetJpegMMX(FALSE);
		break;
	case ID_BACKUP_NNN_ON:
		SetBackupNNN();
		break;
	case ID_BACKUP_NNN_OFF:
		SetBackupNNN(FALSE);
		break;
	case ID_LAYERCOPYMODE_NO:
		SetLayerCopyMode(0);
		break;
	case ID_LAYERCOPYMODE_OK:
		SetLayerCopyMode(1);
		break;
	case ID_LAYERCOPYMODE_SEL:
		SetLayerCopyMode(2);
		break;

	case ID_PRESETBUTTON_ENABLE:
		SetPresetButtonOk(1);
		break;
	case ID_PRESETBUTTON_DISABLE:
		SetPresetButtonOk(0);
		break;


	case ID_WINDOW_MAINSCREEN:
///		ShowMyWindow(MAINSCREEN_WINDOW);
//		m_mainScreen->Show(TRUE);
		break;
	case ID_WINDOW_FILM:
///		ShowMyWindow(FILM_WINDOW);
//		m_film->Show(TRUE);
		break;
	case ID_WINDOW_LAYER:
//		m_layer->Show(TRUE);
///		ShowMyWindow(LAYER_WINDOW);
		break;
	case ID_WINDOW_ZAHYO:
///		ShowMyWindow(ZAHYO_WINDOW);
		break;
	case ID_WINDOW_GAMEMESSAGE:
///		ShowMyWindow(GAMEMESSAGE_WINDOW);
		break;
	case ID_WINDOW_EFFECT:
///		ShowMyWindow(EFFECT_WINDOW);
		break;


	case ID_MENU_VERSION:
		PrintVersion();
//		MessageBox(m_frameHWND,"Version 0.81\n2005-05-02","Version",MB_OK | MB_APPLMODAL);
		break;
	case ID_MENU_ABOUT:
		MessageBox(m_frameHWND,"ひみつにゃ","HELP",MB_OK | MB_APPLMODAL);
		break;
	case ID_MENU_HELP:
	case ID_BUTTON_HELP:
		ShellExecute(NULL,"open","nnndir\\help\\index.htm",NULL,NULL,SW_SHOW); 
		break;
	case ID_MENU_COMPILE_ALL:
	case ID_BUTTON_COMPILE_ALL:
		break;

	case ID_MENU_COMPILE_STORY:
	case ID_BUTTON_COMPILE_STORY:
		break;

	case ID_MENU_COMPILE_COMMAND:
	case ID_BUTTON_COMPILE_COMMAND:
		CompileCommand();
		break;


	case ID_MENU_BUILD_TXT:
	case ID_BUTTON_BUILD_TXT:
		OnBuildTxt();
		break;
	case ID_MENU_BUILD_SPT:
	case ID_BUTTON_BUILD_SPT:
		OnBuildSpt();
		break;
	case ID_MENU_BUILD_JSON:
//	case ID_BUTTON_BUILD_TXT:
		OnBuildJson();
		break;

	case ID_MENU_PROJECT_BUILD_NEW:
	case ID_BUTTON_PROJECT_BUILD_NEW:
		break;
	case ID_MENU_PROJECT_BUILD_ALL:
	case ID_BUTTON_PROJECT_BUILD_ALL:
		OnAllBuild();
		break;


	case ID_MENU_PROJECT_ADD:
		break;
	case ID_MENU_PROJECT_EDIT:
		OnDeleteProject();
		break;

	case ID_MENU_COMMAND_ADD:
		OnAddCommandList();
		break;
	case ID_MENU_COMMAND_EDIT:
		OnDeleteCommandList();
		break;
	case ID_MENU_FUNCTION_ADD:
		OnAddFunctionList();
		break;
	case ID_MENU_FUNCTION_EDIT:
		OnDeleteFunctionList();
		break;


	case ID_MENU_UTIL_COPYEFFECT:
	case ID_BUTTON_UTIL_COPYEFFECT:
		CopyPreEffect();
		break;
	case ID_MENU_UTIL_COPYEFFECT2:
	case ID_BUTTON_UTIL_COPYEFFECT2:
		CopyPreEffect(2);
		break;
	case ID_MENU_UTIL_AUTOVOICE:
		if (m_autoVoice->SetVoice())
		{
			MessageBox(m_frameHWND,"おわりましたにゃ","確認",MB_OK);
		}
		break;
	

	case ID_MENU_UTIL_MAKEBOOK_ALL:
		m_makeBook->MakeBook(1);
		break;
	case ID_MENU_UTIL_MAKEBOOK:
		m_makeBook->MakeBook(0,0);
		break;
	case ID_MENU_UTIL_MAKEBOOK1:
		m_makeBook->MakeBook(0,1);
		break;
	case ID_MENU_UTIL_MAKEBOOK2:
		m_makeBook->MakeBook(0,2);
		break;
	case ID_MENU_UTIL_MAKEBOOK3:
		m_makeBook->MakeBook(0,3);
		break;
	case ID_MENU_UTIL_MAKEBOOK4:
		m_makeBook->MakeBook(0,4);
		break;

	case ID_MENU_UTIL_SEARCH:
		m_searchOrReplace->SearchOrReplace(FALSE,FALSE);
		break;
	case ID_MENU_UTIL_REPLACE:
		m_searchOrReplace->SearchOrReplace(TRUE,FALSE);
		break;
	case ID_MENU_UTIL_SEARCH_ALL:
		m_searchOrReplace->SearchOrReplace(FALSE,TRUE);
		break;
	case ID_MENU_UTIL_REPLACE_ALL:
		m_searchOrReplace->SearchOrReplace(TRUE,TRUE);
		break;
	case ID_MENU_VARSEARCH:
		SearchVar();
		break;
	case ID_MENU_VARREPLACE:
		SearchVar(TRUE);
		break;
	case ID_MENU_UTIL_ALLAUTOVOICE:
		m_autoVoice->AllSetVoice();
		break;
	case ID_MENU_UTIL_MEMO:
		ChangeMemo();
		break;
	case ID_MENU_UTIL_SAMEMESSAGE_DEUTCH:
		//SameMessageToDeutch();
		m_sameMessageDeutch->SameMessageToDeutch();
		break;
	case ID_MENU_UTIL_USEDWQ:
		//MakeUseDWQBat();
		m_makeUseDWQBat->MakeBat();
		break;
	case ID_MENU_UTIL_CHECKUSEDWQ:
		//MakeCheckDWQBat();
		m_makeCheckDWQBat->MakeBat();
		break;
	case ID_MENU_UTIL_MESSAGESERIAL:
		m_messageSerialControl->SetMessageSerial();
		break;
	case ID_MENU_UTIL_ALLMESSAGESERIAL:
		m_messageSerialControl->SetAllMessageSerial();
		break;
	case ID_MENU_UTIL_MESSAGESERIALUPDATE:
		m_messageSerialControl->SetMessageSerial(TRUE);
		break;
	case ID_MENU_UTIL_ALLMESSAGESERIALUPDATE:
		m_messageSerialControl->SetAllMessageSerial(TRUE);
		break;
	case ID_MENU_UTIL_CHECKDWQALL:
		m_checkDWQ->CheckDWQAll();
		break;
	case ID_MENU_UTIL_CHECKDWQ:
		m_checkDWQ->CheckDWQ();
		break;
	case ID_MENU_UTIL_DELETENONPLAYERVOICE:
		m_deleteNonVoice->DeleteNonVoice();
		break;
	case ID_MENU_UTIL_ALLDELETENONPLAYERVOICE:
		m_deleteNonVoice->DeleteAllNonVoice();
		break;
	case ID_MENU_UTIL_TESTALL:
		MakeAllTextSource();
		break;
	case ID_MENU_UTIL_CHECKWAVBAT:
		//MakeCheckVAWBat();
		m_makeCheckVawBat->MakeBat();
		break;
	case ID_MENU_UTIL_USEWAV:
		//MakeUseVAWBat();
		m_makeUseVawBat->MakeBat();
		break;
	case ID_MENU_UTIL_CALCUFRAME:
		CalcuFrame();
		break;

	case ID_UTIL_CHECKSCENE:
		CheckSceneData();
		break;

	case ID_MENUITEM_ALLLOCKVOICE:
		m_lockVoice->AllLockVoice();
		break;
	case ID_MENUITEM_ALLUNLOCKVOICE:
		m_lockVoice->AllLockVoice(FALSE);
		break;
	case ID_MENU_UTIL_DELETEALLVOICE:
		m_deleteVoice->DeleteVoice();
		break;
	case ID_MENU_UTIL_DELETEALLVOICEALL:
		m_deleteVoice->DeleteVoiceAll();
		break;

	case ID_MENU_MOVIE_ALL:
		m_movieCreate->MakeMovieAll();
		break;
	case ID_MENU_MOVIE_ALLFILM:
		m_movieCreate->MakeMovieAllFilm();
		break;
	case ID_MENU_MOVIE_FILM:
		m_movieCreate->MakeMovieFilm();
		break;

	case ID_DAIHON_WHITESPACE:
		ChangeDaihonSpace();
		break;
	case ID_MENU_UTIL_FRAMECHANGE:
		m_frameChange->FrameChange();
		break;
	case ID_MENU_UTIL_ALLFRAMECHANGE:
		m_frameChange->AllFrameChange();
		break;

	case ID_MENU_UTIL_AUTOENDIF:
		CStoryData::m_adjustEnfIf = !CStoryData::m_adjustEnfIf;
		break;



	case ID_SETDEFAULTFRAME:
		ChangeDefaultFrame();
		break;
	case ID_PRINTFRAME:
		ChangePrintFrameFlag();
		break;
	case ID_PRINTSKIP:
		ChangePrintSkipFlag();
		break;
	case ID_SETMAXSKIP:
		SetMaxSkip();
		break;
	case ID_SETSKIPINTERVAL:
		SetSkipInterval();
		break;

	case ID_MENUITEM_DEBUG:
		Debug();
		break;

	case ID_MENU_UTIL_OUTPUTALLFILMNAME:
		OutputAllFilmName();
		break;

	case ID_MENU_UTIL_CHANGEBGMLIST:
		m_changeBGMList->AllChange();
		break;
	case ID_MENU_UTIL_CHANGESELIST:
		m_changeSeList->AllChange();
		break;

	case ID_INITANGOU:
		ChangeInitAngouMode();
		break;

	case ID_EXTUTIL_INITANGOU:
	case ID_EXTUTIL_PACNYAN:
	case ID_EXTUTIL_NERUNE:
	case ID_EXTUTIL_INITCHECK:
		ExtUtil(wParam);
		break;

	case ID_UNDO_ENABLE:
		ChangeUndoMode(1);
		break;
	case ID_UNDO_DISABLE:
		ChangeUndoMode(0);
		break;
	case ID_UNDO_KAKUNIN_ARI:
		ChangeUndoKakuninMode(1);
		break;
	case ID_UNDO_KAKUNIN_NASHI:
		ChangeUndoKakuninMode(0);
		break;
	case ID_UNDO_AFTER_ARI:
		ChangeUndoAfterMode(1);
		break;
	case ID_UNDO_AFTER_NASHI:
		ChangeUndoAfterMode(0);
		break;


	case ID_ADDDEFAULTVOICE:
		ChangeAddDefaultVoice();
		break;
	case ID_SETDEFAULTMEI:
		SetDefaultName();
		break;
	case ID_MENU_CTRL_ENABLE:
		SetControlKeyEnable(1);
		break;
	case ID_MENU_CTRL_DISABLE:
		SetControlKeyEnable(0);
		break;
		//	case ID_ALIEN:
//		BugBug();
		break;
	}
}




void CMyApplication::ShowMyWindow(int n)
{
	CMyDocument* pDoc = m_document[n];
	if (pDoc != NULL)
	{
		CMyView* pView = pDoc->GetView();
		if (pView != NULL)
		{
			pView->Show();
		}
	}
}


void CMyApplication::OnMove(WPARAM wParam, LPARAM lParam)
{
	CMainScreenDoc* pDoc = (CMainScreenDoc*)m_document[MAINSCREEN_WINDOW];
	if (pDoc != NULL) pDoc->Moved(wParam,lParam);
}


void CMyApplication::SetEyeStatus(int layer,int st)
{
	if (st != 0)
	{
		m_effect->SetEye(layer);
	}
	else
	{
		m_effect->SetEye(layer,FALSE);
	}

	dmy();

//	CMainScreenDoc* pDoc = (CMainScreenDoc*)m_document[MAINSCREEN_WINDOW];
//	pDoc->DmyPrint();
//	pDoc->MyInvalidateRect();
}

void CMyApplication::PrintMessageWindow(bool nameFlag)
{
	if (m_messageWindow != nullptr)
	{
		m_messageWindow->Print(TRUE, nameFlag);
	}
}

void CMyApplication::dmy(int cnt)
{
	CAreaControl::SetNextAllPrint();
	m_effect->BeginEffect();

	if (cnt == 0)
	{
		//m_effect->Calcu();
		m_effect->CalcuOnly();
		//OutputDebugString("_");
	}
	else
	{
		m_effect->CountUp(cnt);
		m_effect->CalcuOnly();
//char mes[256];
//sprintf(mes,"[%d]",cnt);
//OutputDebugString(mes);
	}

	//m_effect->Calcu(cnt);

	//m_effect->CalcuOnly(cnt);

	m_effect->Print(FALSE,TRUE);

	//message window

	m_effect->Print(TRUE,TRUE);
//	m_effect->Print();


//m_testPic->Put(0,0,FALSE);

	PrintHint();

	m_effect->EndEffect();

	((CMainScreenDoc*)m_document[MAINSCREEN_WINDOW])->FlipToScreen();

//	m_dummyDoc = new CMyDocument(this);
}



void CMyApplication::dmy0(void)
{
	CAreaControl::SetNextAllPrint();
//	m_effect->CalcuZahyo();
//	m_effect->Print();

	m_effect->BeginEffect();
	m_effect->CalcuOnly();
	m_effect->CountUp();
	m_effect->Print(FALSE,TRUE);
	m_effect->EndEffect();

//	m_dummyDoc = new CMyDocument(this);
}



void CMyApplication::AdjustDstFrom(int px, int py)
{
	m_effect->ResetCount();
}

void CMyApplication::AdjustDstTo(int px, int py)
{
	m_effect->SetCountToLast();
}

void CMyApplication::MoveDstFrom(int dx, int dy)
{
//	int layer = 6;
//	int md = 2;
//	MoveRectRoutine(layer,md,dx,dy);
	m_effect->ResetCount();
}


void CMyApplication::MoveDstTo(int dx, int dy)
{
//	int layer = 6;
//	int md = 3;
//	MoveRectRoutine(layer,md,dx,dy);




	m_effect->SetCountToLast();
}





void CMyApplication::MoveRectRoutine(int layer, int md, int dx, int dy)
{
	RECT rc;
	m_effect->GetMyRect(layer,md,&rc);
	rc.left += dx;
	rc.top += dy;
	m_effect->SetMyRect(layer,md,&rc);
//OutputDebugString("?");

	if ((md == 2) || (md == 3))
	{
		m_effect->SetMyRect(layer,5,&rc);
	}

	if ((md >= 0) && (md <= 3))
	{
		CZahyoDoc* pDoc = (CZahyoDoc*)m_document[ZAHYO_WINDOW];
		if (pDoc != NULL)
		{
			pDoc->SetZahyoRect(md,&rc);
		}
	}
}


void CMyApplication::UpdateMyWindow(int windowNumber)
{
	int st = windowNumber;
	int ed = windowNumber;

	if ((windowNumber<0) || (windowNumber >= MDIWINDOW_KOSUU_MAX))
	{
		st = 0;
		ed = 31;
	}

	for (int i=st;i<=ed;i++)
	{
		CMyDocument* pDoc = m_document[i];
		if (pDoc != NULL)
		{
			pDoc->MyInvalidateRect();
		}
	}
}

void CMyApplication::MakeAllMiniPic(void)
{
	CLayerDoc* pDoc = (CLayerDoc*)m_document[LAYER_WINDOW];

	for (int i=0;i<16;i++)
	{
		//int pic = m_effect->m_effect[i].pic;
		
		int pic = i;
		if (pDoc->CheckPicExist(i) == FALSE) pic = -1;


		if (pic == -1)
		{
			pDoc->ClearMiniPic(i);
		}
		else
		{
			CPicture* lpPic = m_effect->GetPicture(pic);
			int* ptr = (int*)(lpPic->GetBuffer());
			RECT rc;
			lpPic->GetPicSize(&rc);
			if ((rc.right>0) && (rc.bottom>0))
			{
				pDoc->PictureToMini(i,ptr,rc.right,rc.bottom);
			}
			else
			{
				pDoc->ClearMiniPic(i);
			}
		}
	}
}



HBITMAP CMyApplication::GetCommonBitmap(int n)
{
	if ((n<0) || (n>=50)) n = 0;

	return m_commonHBitmap[n];
}


void CMyApplication::ChangeShowHideWindow(int n)
{
	if (n == -1) return;

	CMyDocument* pDoc = m_document[n];
	if (pDoc != NULL)
	{
		CMyView* pView = pDoc->GetView();
		if (pView != NULL)
		{
			int md = m_nnnConfig->GetValue("windowMode");

			LPSTR windowName = pView->GetMyViewWindowName();
			char mes[256];
			wsprintf(mes,"%sFlag%d",windowName,md+1);
			BOOL flg = (BOOL)(m_nnnConfig->GetValue(mes));

//			BOOL flg = m_config->GetWindowFlag(n,m_windowMode);
//			BOOL flg = m_configMenu->GetWindowFlag(n,m_windowMode);

			ShowHideWindow(n,!flg);

			//to TOP?
			if (flg == FALSE)
			{
				m_frame->ToTop(pView->GetHWND());
			}
		}
	}
}


void CMyApplication::ShowHideWindow(int n, BOOL flg)
{
	if (n == -1) return;

	CMyDocument* pDoc = m_document[n];
	if (pDoc != NULL)
	{
		CMyView* pView = pDoc->GetView();
		if (pView != NULL)
		{
			int md = m_nnnConfig->GetValue("windowMode");
//			m_config->SetWindowFlag(n,m_windowMode,flg);
//			m_configMenu->SetWindowFlag(n,m_windowMode,flg);
			LPSTR windowName = pView->GetMyViewWindowName();
			char mes[256];
			wsprintf(mes,"%sFlag%d",windowName,md+1);
			m_nnnConfig->SetValue(mes,flg);

			m_frame->CheckWindow(n,flg);
			pView->Show(flg);
		}
	}
}

void CMyApplication::HideWindow(int n)
{
	if (n == -1) return;
	ShowHideWindow(n,FALSE);
	return;
}


BOOL CMyApplication::NewScript(void)
{
	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	pDoc->ClearAllStory();
	pDoc->OnNewStory(0);
	CStoryData* pStory = GetNowSelectStory();
	pStory->SetMyName("★トップ");

	CFilmCaseDoc* pDoc2 = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
	pDoc2->ClearAllFilm();

	pDoc->ReCalcuScrollPara();
	pDoc2->ReCalcuScrollPara();


	StoryIsChanged();
	FilmIsChanged();

	m_dataExistFlag = TRUE;
	m_modifyFlag = FALSE;

	m_playMode = FALSE;
	m_pauseFlag = FALSE;

//	SetLastFileName("nnndir\\nnn\\新規.nnn");
	SetLastFileNameOnly("新規.nnn");
	SetFileNameOnly("新規");
	SetWindowTitle("新規");

	UpdateMyWindow(-1);

	return TRUE;
}


BOOL CMyApplication::LoadRoutine(FILE* file)
{
	char tmpc[16];

	fread(tmpc,sizeof(char),16,file);
	fread(m_myname,sizeof(char),64,file);
	fread(m_memo,sizeof(char),256,file);

	int tmp[16];
	fread(tmp,sizeof(int),16,file);

	if (tmp[1] == 1)
	{
		//skip
		fseek(file,sizeof(int)*128*96,SEEK_CUR);
	}
	else if (tmp[1] == 2)
	{
		//skip
		fseek(file,sizeof(int)*256*192,SEEK_CUR);
	}

	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
	CFilmCaseData* pCase = pDoc->GetFilmCaseData();
	pCase->Load(file);

	CStoryBookDoc* pDoc2 = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	CStoryBookData* pBook = pDoc2->GetStoryBookData();
	pBook->Load(file);

	fclose(file);

	FilmIsChanged();
	StoryIsChanged();

	m_dataExistFlag = TRUE;
	m_modifyFlag = FALSE;

	m_playMode = FALSE;


	SetFileNameOnly(m_file->GetFileNameOnly());
	SetLastFileNameOnly(m_file->GetFileName());
	SetWindowTitle(m_fileNameOnly);

	pDoc->ReCalcuScrollPara();
	pDoc2->ReCalcuScrollPara();


	ClearUndo();

	UpdateMyWindow(-1);

	return TRUE;
}




BOOL CMyApplication::Load(void)
{
	FILE* file = m_file->OpenLoad(m_lastFileNameOnly,"nnndir\\nnn");
	if (file == NULL) return FALSE;

	return LoadRoutine(file);
	//return LoadByFileNameOnly(m_fileNameOnly);
}


BOOL CMyApplication::LoadByFileNameOnly(LPSTR name)
{
	char filename[1024];
	wsprintf(filename,"nnndir\\nnn\\%s.nnn",name);
	FILE* file = CMyFile::Open(filename,"rb");
	if (file == NULL) return FALSE;

	return LoadRoutine(file);
}


BOOL CMyApplication::Save(void)
{
	BOOL backupFlag = TRUE;

	if (GetConfig("backup") == 0)
	{
		backupFlag = FALSE;
	}


	FILE* file = m_file->OpenSave(m_lastFileNameOnly,"nnndir\\nnn",backupFlag);
	if (file == NULL) return FALSE;

	fwrite("NNN DATA HEADER",sizeof(char),16,file);
	fwrite(m_myname,sizeof(char),64,file);
	fwrite(m_memo,sizeof(char),256,file);

	int tmp[16];
	for (int i=0;i<16;i++) tmp[i] = 0;

	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
	CFilmCaseData* pCase = pDoc->GetFilmCaseData();
	CStoryBookDoc* pDoc2 = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	CStoryBookData* pBook = pDoc2->GetStoryBookData();

	tmp[0] = 2;	//こすう

	tmp[1] = 0;
	CKomaData* pKoma = NULL;

	if (GetConfig("noSaveNNNPicFlag") == 0)
	{
		if (pCase->GetObjectKosuu() > 0)
		{
			CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(0));
			if (pFilm != NULL)
			{
				if (pFilm->GetObjectKosuu() > 0)
				{
					int komaOfFilm = pFilm->GetSMKomaNumber();
					pKoma = (CKomaData*)(pFilm->GetObjectData(komaOfFilm));
					tmp[1] = 2;
				}
			}

		}
	}


	fwrite(tmp,sizeof(int),16,file);

	if (tmp[1] == 1)
	{
		int* ptr = m_file->Get256x192Buffer();
		pKoma->Make128x96(ptr);
		fwrite(ptr,sizeof(int),128*96,file);
	}
	else if (tmp[1] == 2)
	{
		int* ptr = m_file->Get256x192Buffer();
		pKoma->Make256x192(ptr);
		fwrite(ptr,sizeof(int),256*192,file);
	}



	pCase->Save(file);
	pBook->Save(file);

	fclose(file);

	m_modifyFlag = FALSE;

	MessageBox(m_frameHWND,"Save終了しましたにゃ","確認",MB_OK);

	ClearUndo();

	SetFileNameOnly(m_file->GetFileNameOnly());
	SetLastFileNameOnly(m_file->GetFileName());
	SetWindowTitle(m_fileNameOnly);

	return TRUE;
}


LPSTR CMyApplication::CreateRandomString(void)
{
	int rnddata = rand() % 10000;

	SYSTEMTIME tm;

	GetLocalTime(&tm);

	int year = tm.wYear;
	int month = tm.wMonth;
	int date = tm.wDay;
	int hour = tm.wHour;
	int minute = tm.wMinute;
	int second = tm.wSecond;
	int milliseconds = tm.wMilliseconds;

	wsprintf(m_randomString,"%d%d%d%d%d%d%d_%d",year,month,date,hour,minute,second,milliseconds,rnddata);

	return m_randomString;
}


LPSTR CMyApplication::GetNewFilmName(void)
{
	LPSTR rndStr = CreateRandomString();

	wsprintf(m_tmpName,"_F_%s_",rndStr);
	return m_tmpName;
}


LPSTR CMyApplication::GetNewStoryName(void)
{
	LPSTR rndStr = CreateRandomString();

	wsprintf(m_tmpName,"_S_%s_",rndStr);
	return m_tmpName;
}


CFilmData* CMyApplication::GetNowSelectFilm(void)
{
	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
	if (pDoc == NULL) return NULL;

	return pDoc->GetNowSelectFilm();
}

CStoryData* CMyApplication::GetNowSelectStory(void)
{
	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	if (pDoc == NULL) return NULL;

	CStoryBookData* pBook = pDoc->GetStoryBookData();
	if (pBook == NULL) return NULL;

	return (CStoryData*)(pBook->GetNowSelectObject());
}

void CMyApplication::FilmIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->FilmIsChanged();
}

void CMyApplication::KomaIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->KomaIsChanged();
}

void CMyApplication::MessageIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->MessageIsChanged();
}

void CMyApplication::LayerIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->LayerIsChanged();
}

void CMyApplication::EffectIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->EffectIsChanged();
}

void CMyApplication::ZahyoIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->ZahyoIsChanged();
}

void CMyApplication::StoryIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->StoryIsChanged();
}

void CMyApplication::CommandIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->CommandIsChanged();
}

void CMyApplication::ProgramIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->ProgramIsChanged();
}

void CMyApplication::VarIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->VarIsChanged();
}

void CMyApplication::ConteIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->ConteIsChanged();
}

void CMyApplication::ConteMessageIsChanged(void)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->ConteMessageIsChanged();
}


void CMyApplication::UpdateMainScreen(int cnt)
{
	if (m_noUpdateScreenFlag) return;

	CKomaData* pKoma = NULL;

	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm != NULL) pKoma = (CKomaData*)(pFilm->GetNowSelectObject());

	if (pFilm != NULL)
	{
		if (pFilm->GetNowSelectNumber() == 0)
		{
			if (pFilm->GetNoClearEffect() == FALSE)
			{
				m_effect->ClearAllEffect();

				if (m_cutinFlag)
				{
//					m_cutinControl->OnClearAllEffect();
				}
			}
		}
	}

	if (pKoma != NULL)
	{
		pKoma->SetAllEffect();
		pKoma->LoadAllPicture();

		dmy(cnt);
//		pKoma->Print();
	}
	else
	{
		m_effect->ClearAllEffect();
		dmy();
	}
}


int CMyApplication::GetNowSelectPictureLayer(void)
{
	CLayerDoc* pDoc = (CLayerDoc*)m_document[LAYER_WINDOW];
	if (pDoc == NULL) return -1;
	return pDoc->GetSelectPictureLayer();
}


int CMyApplication::GetNowSelectEffectLayer(void)
{
	CLayerDoc* pDoc = (CLayerDoc*)m_document[LAYER_WINDOW];
	if (pDoc == NULL) return -1;
	return pDoc->GetSelectEffectLayer();
}


BOOL CMyApplication::GetPlayMode(void)
{
	return m_playMode;
}


int CMyApplication::GetSubCommand(void)
{
	return m_playCommand;
}


void CMyApplication::CountUp(int deltaCount)
{
	m_effect->CountUp(deltaCount);
	m_gameCount += deltaCount;

	CheckNextMessage();
}

void CMyApplication::CheckNextMessage(void)
{
	CGameMessageDoc* pDoc = (CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW];
	CMessageData* pMessage = (CMessageData*)(pDoc->GetNowSelectMessage());
	if (pMessage != NULL)
	{
		int md = pMessage->GetMessageMode();
		if (md == 3)
		{
			CFilmDoc* pDoc2 = (CFilmDoc*)m_document[FILM_WINDOW];
			CKomaData* pKoma = (CKomaData*)(pDoc2->GetNowSelectKoma());
			int drawCount = pMessage->GetDrawCount();

			if (pKoma->GetNowSelectNumber() == 0)
			{
				drawCount += pKoma->GetOverrapCount();
			}

			if (m_gameCount>=drawCount)
			{
				OnNextMessage();
			}
		}
	}
	else
	{
		OnNextMessage();
	}
}



void CMyApplication::SetPlayMode(BOOL md, int subCommand,BOOL countClearFlag)
{
	m_playMode = md;
	if (countClearFlag) m_gameCount = 0;
	if (subCommand != -1) m_playCommand = subCommand;

	if (m_playCommand == 0)	//play
	{
		SetNewEffect(0);
	}
}



void CMyApplication::SetKomaMode(int md)
{
	m_komaMode = md;
}

void CMyApplication::SetModify(BOOL flg)
{
	m_modifyFlag = flg;
}

BOOL CMyApplication::CheckModify(void)
{
	return m_modifyFlag;
}

BOOL CMyApplication::CheckPlayMode(void)
{
	return m_playMode;
}

BOOL CMyApplication::CheckPauseMode(void)
{
	return m_pauseFlag;
}

int CMyApplication::GetPlayCommand(void)
{
	return m_playCommand;
}

BOOL CMyApplication::CheckKomaMode(void)
{
	return m_komaMode;
}


void CMyApplication::SetWindowName(int windowNumber, LPSTR name)
{
	if ((windowNumber>=0) && (windowNumber<MDIWINDOW_KOSUU_MAX))
	{
		CMyDocument* pDoc = m_document[windowNumber];
		if (pDoc != NULL)
		{
			pDoc->SetWindowName(name);
		}
	}
}

CFilmData* CMyApplication::GetFilmData(int n)
{
	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
	if (pDoc == NULL) return NULL;

	return pDoc->GetFilm(n);
}


int CMyApplication::SearchFilm(LPSTR filmName, int searchStart,int noCheckNumber)
{
	CFilmCaseData* pCase = GetFilmCaseData();
	return pCase->SearchFilm(filmName,searchStart,noCheckNumber);
}

int CMyApplication::SearchStory(LPSTR storyName,int searchStart,int noCheckNumber)
{
	CStoryBookData* pBook = GetStoryBookData();
	return pBook->SearchStory(storyName,searchStart,noCheckNumber);
}


int CMyApplication::SearchScript(LPSTR sptName, int searchStart,int noCheckNumber)
{
	noCheckNumber = -1;
	searchStart = 0;

	return m_projectList->GetNameNumber(sptName);
}


void CMyApplication::MakeAllTextSource(void)
{

	char lastFileName[256];
	memcpy(lastFileName,m_fileNameOnly,strlen(m_fileNameOnly)+1);

	if (m_modifyFlag)
	{
		int rt = MessageBox(m_frameHWND,"変更されていますにゃ\n保存しますか","警告",MB_YESNOCANCEL | MB_APPLMODAL | MB_ICONEXCLAMATION );
		if (rt == IDCANCEL) return;
		if (rt == IDYES) Save();
	}


	BOOL flg = FALSE;

	int nnnKosuu = m_projectList->GetNameKosuu();
	for (int pp=0;pp<nnnKosuu;pp++)
	{
		LPSTR nnnfilename = m_projectList->GetName(pp);
		if (nnnfilename != NULL)
		{
			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
//			memcpy(m_lastFileName,m_fileNameOnly,strlen(m_fileNameOnly)+1);

			if (LoadByFileNameOnly(nnnfilename) == FALSE)
			{
				MessageBox(m_frameHWND,nnnfilename,"Load ERROR",MB_OK | MB_ICONEXCLAMATION);
				flg = TRUE;
				break;
			}

			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
//			memcpy(m_lastFileName,m_fileNameOnly,strlen(m_fileNameOnly)+1);

			OnBuildTxt(FALSE);
		}
	}




	memcpy(m_fileNameOnly,lastFileName,strlen(lastFileName)+1);
	LoadByFileNameOnly(lastFileName);

	MessageBox(m_frameHWND,"すべておわりましたにゃ","確認",MB_OK);

}


void CMyApplication::OnBuildTxt(BOOL mesFlag)
{
	CreateDirectory("nnndir\\txt",NULL);

	char filename[256];
	wsprintf(filename,"nnndir\\txt\\%s.txt",m_fileNameOnly);

	FILE* file = CMyFile::Open(filename,"wb");
	if (file != NULL)
	{
		CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
		if (pDoc != NULL)
		{
			CStoryBookData* pBook = pDoc->GetStoryBookData();
			if (pBook != NULL)
			{
				pBook->OutputScriptSource(file);
			}
		}


		CFilmCaseDoc* pDoc2 = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
		if (pDoc2 != NULL)
		{
			CFilmCaseData* pCase = pDoc2->GetFilmCaseData();
			if (pCase != NULL)
			{
				pCase->OutputScriptSource(file);
			}
		}


		fclose(file);
		if (mesFlag)
		{
			MessageBox(m_frameHWND,"おわったにゃ","OK",MB_OK);
		}
	}
}

void CMyApplication::OnBuildJson(BOOL mesFlag)
{
	CreateDirectory("nnndir\\txt",NULL);

	char filename[256];
	wsprintf(filename,"nnndir\\txt\\%s.json",m_fileNameOnly);

//	picojson::value json;


	LPSTR text = NULL;

	FILE* file = CMyFile::Open(filename,"wb");
	if (file != NULL)
	{
		text = "{\x00d\x00a";
		fwrite(text,sizeof(char),strlen(text),file);

		//name
		char mes[1024];
		LPSTR nnnName = m_fileNameOnly;
		wsprintf(mes,"\t\"nnnName\" : \"%s\" ,\x00d\x00a\x00d\x00a",nnnName);
		fwrite(mes,sizeof(char),strlen(mes),file);




		CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
		if (pDoc != NULL)
		{
			CStoryBookData* pBook = pDoc->GetStoryBookData();
			if (pBook != NULL)
			{
				pBook->OutputScriptSourceJson(file);
			}
		}

		text = "\x00d\x00a";
		fwrite(text,sizeof(char),strlen(text),file);


		CFilmCaseDoc* pDoc2 = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
		if (pDoc2 != NULL)
		{
			CFilmCaseData* pCase = pDoc2->GetFilmCaseData();
			if (pCase != NULL)
			{

				pCase->OutputData(file,"\"FilmCase\":",1);
				pCase->OutputData(file,"[",1);


				m_scriptCompiler->AllBuild(file);


				pCase->OutputData(file,"]",1);


		//		pCase->OutputScriptSourceJson(file);
			}
		}

		text = "}\x00d\x00a";
		fwrite(text,sizeof(char),strlen(text),file);

		fclose(file);

		if (mesFlag)
		{
			MessageBox(m_frameHWND,"おわったにゃ","OK",MB_OK);
		}
	}
}


void CMyApplication::OnAllBuild(void)
{
	m_scriptCompiler->AllBuild();
}


BOOL CMyApplication::OnBuildSpt(BOOL bMesFlag)
{
	if (m_scriptCompiler->BuildSpt(bMesFlag)) return TRUE;
	UpdateMyWindow(-1);
	return FALSE;
}


void CMyApplication::SetLastFileNameOnly(LPSTR name)
{
	if (name == NULL)
	{
		m_lastFileNameOnly[0] = 0;
		return;
	}

	char nameonly[1024];
	GetFileTitle(name,nameonly,1024);

	int ln = strlen(nameonly);
	if (ln>1022) ln = 1022;
	memcpy(m_lastFileNameOnly,nameonly,ln);
	m_lastFileNameOnly[ln] = 0;
	m_lastFileNameOnly[ln+1] = 0;

//	m_config->SetFileName(0,m_lastFileName);
//	m_configMenu->SetText("lastEditFileName",m_lastFileName);
	m_nnnConfig->SetText("lastEditFileNameOnly",m_lastFileNameOnly);
}


void CMyApplication::SetFileNameOnly(LPSTR name)
{
	if (name == NULL)
	{
		m_fileNameOnly[0] = 0;
		return;
	}
	int ln = strlen(name);
	if (ln>254) ln = 254;
	memcpy(m_fileNameOnly,name,ln);
	m_fileNameOnly[ln] = 0;
	m_fileNameOnly[ln+1] = 0;
}

void CMyApplication::SetWindowTitle(LPSTR title)
{
	if (m_frameHWND != NULL)
	{
		char mes[1024];
		wsprintf(mes,"NNN - [%s]",m_fileNameOnly);
		SetWindowText(m_frameHWND,mes);
	}
}


void CMyApplication::WindowIsMoved(int windowNumber, int x, int y)
{
	if ((windowNumber<0) || (windowNumber>=MDIWINDOW_KOSUU_MAX)) return;

	RECT rc;
	CMyDocument* pDoc = m_document[windowNumber];
	if (pDoc == NULL) return;

	CMyView* pView = pDoc->GetView();
	if (pView == NULL) return;

	pView->GetWindowZahyo(&rc);

//	m_config->SetWindowZahyo(windowNumber,&rc,m_windowMode,TRUE);

	LPSTR windowName = pView->GetMyViewWindowName();

//	BOOL flg = m_configMenu->GetWindowFlag(windowNumber,m_windowMode);
	int md = m_nnnConfig->GetValue("windowMode");
	char name[256];
	wsprintf(name,"%sPosX%d",windowName,md+1);
	m_nnnConfig->SetValue(name,x);
	wsprintf(name,"%sPosY%d",windowName,md+1);
	m_nnnConfig->SetValue(name,y);
}



void CMyApplication::WindowSizeIsChanged(int windowNumber, int sizeX,int sizeY)
{
	if ((windowNumber<0) || (windowNumber>=MDIWINDOW_KOSUU_MAX)) return;


	RECT rc;
	CMyDocument* pDoc = m_document[windowNumber];
	
	if (pDoc == NULL) return;

	CMyView* pView = pDoc->GetView();
	if (pView == NULL) return;

	pView->GetWindowZahyo(&rc);

//	m_config->SetWindowZahyo(windowNumber,&rc,m_windowMode,TRUE);

	int md = m_nnnConfig->GetValue("windowMode");
	LPSTR windowName = pView->GetMyViewWindowName();
	char name[256];
	wsprintf(name,"%sSizeX%d",windowName,md+1);
	m_nnnConfig->SetValue(name,sizeX);
	wsprintf(name,"%sSizeY%d",windowName,md+1);
	m_nnnConfig->SetValue(name,sizeY);

//	BOOL flg = m_nnnConfig->GetValue(name);

//	BOOL flg = m_configMenu->GetWindowFlag(windowNumber,m_windowMode);
//	m_configMenu->SetWindowZahyo(windowNumber,m_windowMode,&rc,flg);
//	m_nnnConfig->SetWindowZahyo(windowName,md,&rc,flg);


}


void CMyApplication::WindowIsShowHide(int windowNumber, BOOL showFlag)
{
	if ((windowNumber<0) || (windowNumber>=MDIWINDOW_KOSUU_MAX)) return;

//	RECT rc;
	CMyDocument* pDoc = m_document[windowNumber];
	
	if (pDoc == NULL) return;

	CMyView* pView = pDoc->GetView();
	if (pView == NULL) return;

//	m_config->SetWindowFlag(windowNumber,m_windowMode,showFlag);
	int md = m_nnnConfig->GetValue("windowMode");
	LPSTR windowName = pView->GetMyViewWindowName();
	char name[256];
	wsprintf(name,"%sFlag%d",windowName,md+1);

//	m_configMenu->SetWindowFlag(windowNumber,m_windowMode,showFlag);
	m_nnnConfig->SetValue(name,showFlag);
}


int CMyApplication::GetZahyoSwitch(void)
{
	CZahyoDoc* pDoc = (CZahyoDoc*)m_document[ZAHYO_WINDOW];
	if (pDoc == NULL) return -1;

	return pDoc->GetSelectSwitch();
}

void CMyApplication::SetNowKomaEffectRect(int layer)
{
	CMainScreenDoc* pDoc = (CMainScreenDoc*)m_document[MAINSCREEN_WINDOW];
	if (pDoc == NULL) return;

	CKomaData* pKoma = pDoc->GetNowSelectKoma();
	if (pKoma == NULL) return;

	if (pKoma->GetEffect(layer) == EFFECT_MAE)
	{
//OutputDebugString("+");
		return;
	}
//OutputDebugString("@");

	RECT rc;
	pKoma->GetEffectRect(&rc,layer,0);
	m_effect->SetMyRect(layer,5,&rc);
	pKoma->GetEffectRect(&rc,layer,1);
	m_effect->SetMyRect(layer,2,&rc);
	pKoma->GetEffectRect(&rc,layer,2);
	m_effect->SetMyRect(layer,3,&rc);
}

void CMyApplication::JumpStory(int n)
{
	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	if (pDoc == NULL) return;

	pDoc->OnSelectStory(n);
}


void CMyApplication::SetVarName(int n, LPSTR name)
{
	m_varList->SetName(n,name);
}

LPSTR CMyApplication::GetVarName(int n)
{
	return m_varList->GetName(n);
}

int CMyApplication::SearchVarName(LPSTR name,int searchStart)
{
	return m_varList->SearchName(name,searchStart);
}


void CMyApplication::CopyPreEffect(int pre)
{
	m_copyPreEffect->CopyPreEffect(pre);
}


void CMyApplication::PreClickMouse(int windowNumber)
{
	if ((windowNumber != MAINSCREEN_WINDOW) && (windowNumber != CONTROL_WINDOW))
	{
		if (GetPlayMode())
		{
			SetPlayMode(FALSE);
			
			CZahyoDoc* pDoc0 = (CZahyoDoc*)m_document[ZAHYO_WINDOW];
			pDoc0->SetSelectSwitch(-1);
			pDoc0->UpdateMyWindow();

			//update control window
			CControlDoc* pDoc = (CControlDoc*)m_document[CONTROL_WINDOW];
			pDoc->UpdateMyWindow();
		}
	}
}

void CMyApplication::DebugStart(void)
{
	FILE* file = CMyFile::Open("debuglog.txt","wb");
	if (file != NULL)
	{
		char name[256];
		DWORD ln0 = 254;
		if (GetUserName(name,&ln0))
		{
			int ln = strlen(name);
			if (ln>254) ln = 254;
			fwrite(name,sizeof(char),ln,file);
			fwrite("\x00d\x00a",sizeof(char),2,file);
		}

		fclose(file);
	}
}

void CMyApplication::DebugLog(LPSTR mes)
{
	FILE* file = CMyFile::Open("debuglog.txt","ab");
	if (file != NULL)
	{
		fwrite(mes,sizeof(char),strlen(mes),file);
		fclose(file);
	}
}


LPSTR CMyApplication::GetCharaName(int n)
{
	return m_charaList->GetName(n*2+1);
}

LPSTR CMyApplication::GetSEName(int n)
{
	return m_seList->GetName(n*2+1);
}

LPSTR CMyApplication::GetSEFileName(int n)
{
	return m_seList->GetName(n*2);
}

LPSTR CMyApplication::GetBGMName(int n)
{
	return m_bgmList->GetName(n*2+1);
}

LPSTR CMyApplication::GetBGMFileName(int n)
{
	return m_bgmList->GetName(n*2);
}




BOOL CMyApplication::CheckSave(BOOL varCheckFlag)
{
	if (m_modifyFlag)
	{
		int rt = MessageBox(m_frameHWND,"変更されていますにゃ\n保存しますか","警告",MB_YESNOCANCEL | MB_APPLMODAL | MB_ICONEXCLAMATION );
		if (rt == IDCANCEL) return FALSE;
		if (rt == IDYES) Save();
	}

	BOOL md1 = m_varList->CheckModify();
	BOOL md2 = m_varInitData->CheckModify();
	
	if (md1 || md2)
	{
		int rt = MessageBox(m_frameHWND,"変数名または初期値が変更されていますにゃ\n保存しますか","警告",MB_YESNOCANCEL | MB_APPLMODAL | MB_ICONEXCLAMATION );
		if (rt == IDCANCEL) return FALSE;
		if (rt == IDYES)
		{
			if (md1) SaveVarName();
			if (md2) SaveVarInitData();
		}
	}

	return TRUE;
}

void CMyApplication::ChangeMemo(void)
{
	LPSTR newText = m_input->GetText2(m_memo);
	if (newText == NULL) return;

	int ln = strlen(newText);
	if (ln>254)
	{
		MessageBox(m_frameHWND,"長すぎますにゃ","ERROR",MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	memcpy(m_memo,newText,ln);
	m_memo[ln] = 0;
	m_memo[ln+1] = 0;

	SetModify();
}



BOOL CMyApplication::CompileCommand(void)
{
	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	CCommandData* pCommand = pDoc->GetNowSelectCommand();
	if (pCommand == NULL) return FALSE;

	return m_scriptCompiler->CompileCommandData(pCommand);
}


BOOL CMyApplication::CheckIamSystemScript(void)
{
	if (m_projectList->SearchName(m_fileNameOnly) == 0) return TRUE;

	return FALSE;
}



void CMyApplication::PlayVoice(LPSTR voiceName)
{
	if (voiceName == NULL)
	{
		return;
	}

//@@@	m_wave->StopWave();
	
	char filename[16];
	wsprintf(filename,"__\\%s",voiceName);
	filename[0] = *(voiceName);
	filename[1] = *(voiceName+1);



//	if (m_wave->LoadWave(filename,TRUE))
//@@	if (m_wave->LoadWave(voiceName,TRUE))
	{
	//@@	m_wave->PlayWave();
//OutputDebugString("[PLAY]");
	}
}



void CMyApplication::PlaySound(LPSTR soundName)
{
//@@	m_wave->StopWave();
	
//@@	if (m_wave->LoadWave(soundName,FALSE))
	{
	//@@	m_wave->PlayWave();
	}
}


void CMyApplication::CheckAndPlaySound(void)
{
	CGameMessageDoc* pMessageDoc = (CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW];
	if (pMessageDoc == NULL) return;

	CKomaData* pKoma = pMessageDoc->GetNowSelectKoma();	//へん
	if (pKoma == NULL) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetNowSelectObject());
	if (pMessage == NULL) return;

	if (pMessage->GetSEFlag())
	{
		for (int ch=0;ch<8;ch++)
		{
			if (pMessage->CheckSEFlag(ch))
			{
				m_scriptSoundControl->PlayScriptMessageSound(pMessage,ch);
			}
		}
	}

	if (1)
	{
		for (int ch=0;ch<8;ch++)
		{
			if (pMessage->CheckSeStop(ch))
			{
				m_scriptSoundControl->StopScriptSound(ch);
			}
		}
	}

	if (pMessage->GetVoiceFlag())
	{
		for (int ch=0;ch<4;ch++)
		{
			if (pMessage->CheckVoiceFlag(ch))
			{
				m_scriptVoiceControl->PlayScriptMessageVoice(pMessage,ch);
			}
		}
	}
}

HGDIOBJ CMyApplication::SelObjOnly(HDC hdc, HGDIOBJ obj)
{
	return m_selObj->Sel(hdc,obj);
}

BOOL CMyApplication::PutByDIB(HDC hdc,int x, int y, int sizeX, int sizeY, int* buffer)
{
	if (m_commonDIB == NULL) return FALSE;
	return m_commonDIB->Put(hdc,x,y,sizeX,sizeY,buffer);
}


BOOL CMyApplication::ConfigMenu(void)
{
	return FALSE;
}

void CMyApplication::GetInitWindowZahyo(LPSTR windowName, int windowNumber, RECT* lpRect, BOOL* lpWindowFlag)
{
	int windowMode = GetWindowMode();
	
	m_nnnConfig->GetWindowZahyo(windowName,windowMode,lpRect,lpWindowFlag);

	if ((lpRect->right<=0) || (lpRect->bottom <=0))
	{
		int found = 0;
		for (int i=0;i<MDIWINDOW_KOSUU_MAX;i++)
		{
			if (m_windowInitZahyo[i*11] == windowNumber)
			{
				found = i;
				break;
			}
			if (m_windowInitZahyo[i*11] == -1) break;
		}


		//初期化
		RECT rc;
		for (int ii=0;ii<2;ii++)
		{
			BOOL flg = m_windowInitZahyo[found*11+ii*5+1];
			rc.left = m_windowInitZahyo[found*11+ii*5+2];
			rc.top = m_windowInitZahyo[found*11+ii*5+3];
			rc.right = m_windowInitZahyo[found*11+ii*5+4];
			rc.bottom = m_windowInitZahyo[found*11+ii*5+5];
//			m_configMenu->SetWindowZahyo(windowNumber,ii,&rc,flg);
			m_nnnConfig->SetWindowZahyo(windowName,ii,&rc,flg);


			if (windowMode == LAYER_WINDOW)
			{
				int layerMax = CKomaData::m_layerMax;
				if (layerMax > 0)
				{
					rc.bottom -= 24*16;
					rc.bottom += 24*layerMax;
				}
			}

			if ((ii == windowMode) || (ii==0))	//用心
			{
				*lpWindowFlag = flg;
				*lpRect = rc;
			}
		}
	}
}

int CMyApplication::GetWindowMode(void)
{
	return m_nnnConfig->GetValue("windowMode");
}


void CMyApplication::GetFrameWindowRect(RECT* lpRect)
{
	lpRect->left = m_nnnConfig->GetValue("windowPosX");
	lpRect->top = m_nnnConfig->GetValue("windowPosY");
	lpRect->right = m_nnnConfig->GetValue("windowSizeX");
	lpRect->bottom = m_nnnConfig->GetValue("windowSizeY");
}

void CMyApplication::SetFrameWindowRect(RECT* lpRect)
{
	m_nnnConfig->SetValue("windowPosX",lpRect->left);
	m_nnnConfig->SetValue("windowPosY",lpRect->top);
	m_nnnConfig->SetValue("windowSizeX",lpRect->right);
	m_nnnConfig->SetValue("windowSizeY",lpRect->bottom);
}


HBITMAP CMyApplication::GetFilmLargePic(int n)
{
	CFilmDoc* pDoc = (CFilmDoc*)m_document[FILM_WINDOW];
	return pDoc->GetHBitmap(n);
}


CKomaData* CMyApplication::GetNowSelectKoma(void)
{
	CFilmDoc* pDoc2 = (CFilmDoc*)m_document[FILM_WINDOW];
	CKomaData* pKoma = (CKomaData*)(pDoc2->GetNowSelectKoma());
	return pKoma;
}



void CMyApplication::PrintHint(void)
{
	//layer
	int layer = GetNowSelectPictureLayer();
	if (layer != -1)
	{
		CZahyoDoc* pDoc = (CZahyoDoc*)m_document[ZAHYO_WINDOW];
		if (pDoc != NULL)
		{
			int btn = pDoc->GetSelectSwitch();
			((CMainScreenDoc*)m_document[MAINSCREEN_WINDOW])->PrintLayerHint(btn);
		}
	}

	//effect
	if (GetPlayMode() == FALSE)
	{
		int effectLayer = GetNowSelectEffectLayer();
		if (effectLayer != -1)
		{
			CKomaData* pKoma = GetNowSelectKoma();
			if (pKoma != NULL)
			{
				int effect = pKoma->GetEffect(effectLayer);
				if (effect != -1)
				{
					((CMainScreenDoc*)m_document[MAINSCREEN_WINDOW])->PrintEffectHint(effectLayer);
				}
			}
		}
	}


	if (1)
	{
		CKomaData* pKoma = GetNowSelectKoma();
		int frame = m_frameTime;

		int komaFrame = 0;
		if (pKoma != NULL)
		{
			komaFrame = pKoma->GetFrameTime();
		}

		if (komaFrame == -1)
		{
			frame = m_defaultFrameTime;
		}
		else if (komaFrame > 0)
		{
			frame = komaFrame;
		}

		if (frame > 0)
		{
			frame = 1000/frame;
		}

		int fps = 0;
		if (m_onTimer != NULL)
		{
			fps = m_onTimer->GetFPS();
		}

		if (m_framePrintFlag)
		{
			((CMainScreenDoc*)m_document[MAINSCREEN_WINDOW])->PrintFrameTime(fps,frame);
		}

		//フレーム遅れ、スキップ数を表示する
		int shakin = 0;
		int skip = 0;
		int totalSkip = 0;

		if (m_shakinControl != NULL)
		{
			shakin = m_shakinControl->GetShakinTime(0);
			
			if (m_onTimer != NULL)
			{
				totalSkip = m_onTimer->GetTotalSkip();
				skip = m_onTimer->GetNowSkip();
			}

			int dv = GetFrameTime();
			if (dv<1) dv = 1;
			shakin /= dv;

			if (m_skipPrintFlag)
			{
				((CMainScreenDoc*)m_document[MAINSCREEN_WINDOW])->PrintShakin(shakin,skip,totalSkip);
			}
		}



	}



	((CMainScreenDoc*)m_document[MAINSCREEN_WINDOW])->PrintPlayModeIcon(m_playMode,m_filmPlayMode);






/*	int layer = GetNowSelectPictureLayer();
	if (layer != -1)
	{
		CFilmDoc* pDoc2 = (CFilmDoc*)m_document[FILM_WINDOW];
		CKomaData* pKoma = (CKomaData*)(pDoc2->GetNowSelectKoma());
		if (pKoma != NULL)
		{
			if (pKoma->GetPictureNumber(layer) != -1)
			{
				int btn = pDoc->GetSelectSwitch();
				if ((btn >= 5) && (btn <= 7))
				{
					RECT rc;
					pKoma->GetEffectRect(&rc,layer,btn-5);	//dst,dstfrom dstro

					int x = rc.left;
					int y = rc.top;

					CAllGeo::BoxFill(x,y,32,2,0,0,0);
					CAllGeo::BoxFill(x,y,2,32,0,0,0);
					CAllGeo::BoxFill(x,y,32,1,255,255,255);
					CAllGeo::BoxFill(x,y,1,32,255,255,255);

					x = rc.left + rc.right - 32;
					y = rc.top;
					CAllGeo::BoxFill(x,y,32,2,0,0,0);
					CAllGeo::BoxFill(x,y,32,1,255,255,255);
					x = rc.left + rc.right - 2;
					y = rc.top;
					CAllGeo::BoxFill(x,y,2,32,0,0,0);
					CAllGeo::BoxFill(x,y,1,32,255,255,255);

					x = rc.left;
					y = rc.top + rc.bottom - 2;
					CAllGeo::BoxFill(x,y,32,2,0,0,0);
					CAllGeo::BoxFill(x,y,32,1,255,255,255);
					x = rc.left;
					y = rc.top + rc.bottom - 32;
					CAllGeo::BoxFill(x,y,2,32,0,0,0);
					CAllGeo::BoxFill(x,y,1,32,255,255,255);

					x = rc.left + rc.right - 32;
					y = rc.top + rc.bottom - 2;
					CAllGeo::BoxFill(x,y,32,2,0,0,0);
					CAllGeo::BoxFill(x,y,32,1,255,255,255);
					x = rc.left + rc.right - 2;
					y = rc.top + rc.bottom - 32;
					CAllGeo::BoxFill(x,y,2,32,0,0,0);
					CAllGeo::BoxFill(x,y,1,32,255,255,255);

				}
			}
		}
	}*/
}

int CMyApplication::CalcuFilmTime(CFilmData* pFilm,int defaultRate)
{
	if (pFilm == NULL) return 0;

	int rate = defaultRate;
	if (rate == 0)
	{
		rate = m_defaultFrameTime;
		if (rate == 0) rate = 50;//ようじん
	}

	int frameTime = 0;	//millsecond

	int kosuu = pFilm->GetObjectKosuu();
	for (int i=0;i<kosuu;i++)
	{
		CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(i));
		if (pKoma != NULL)
		{
			int r = pKoma->GetFrameTime();
			if (r > 0)
			{
				rate = r;
			}
			else if (r == -1)
			{
				rate = m_defaultFrameTime;
			}

			int overrapTime = pKoma->GetOverrapCount();
			if (overrapTime > 0)
			{
				frameTime += rate * overrapTime;
			}

			int kosuu2 = pKoma->GetObjectKosuu();
			for (int ii=0;ii<kosuu2;ii++)
			{
				CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(ii));
				if (pMessage != NULL)
				{
					int type = pMessage->GetMessageMode();
					if (type == 3)
					{
						frameTime += (pMessage->GetDrawCount()) * rate;
					}
				}
			}
		}
	}

	return frameTime;
}



void CMyApplication::CalcuFrame(void)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return;

	int frameTime = CalcuFilmTime(pFilm);

	int mn = (frameTime/1000) / 60;
	int sc = (frameTime/1000) % 60;
	int ml = frameTime % 1000;


	char mes[256];
	wsprintf(mes,"フィルム:%s\n%d分%d秒%d%d%d",pFilm->GetMyName(),mn,sc,ml / 100,(ml/10) % 10,ml % 10);
	MessageBox(m_frameHWND,mes,"RESULT",MB_OK);

	return;
}

SIZE CMyApplication::GetDesktopSize(void)
{
	HWND hwnd = GetDesktopWindow();
	RECT rc;
	GetWindowRect(hwnd,&rc);
	SIZE sz;
	sz.cx = rc.right;
	sz.cy = rc.bottom;
	return sz;
}


int CMyApplication::GetConfig(LPSTR name)
{
	return m_nnnConfig->GetValue(name);
}

LPSTR CMyApplication::GetConfigString(LPSTR name)
{
	return m_nnnConfig->GetText(name);
}

void CMyApplication::SetConfig(LPSTR name,int value)
{
	m_nnnConfig->SetValue(name,value);
}

void CMyApplication::SetConfig(LPSTR name,LPSTR mes)
{
	m_nnnConfig->SetText(name,mes);
}


CEffectParam* CMyApplication::GetEffectParam(int n)
{
	return m_allEffectParam->GetEffectParam(n);
}

int CMyApplication::GetNowEffect(void)
{
	CFilmDoc* pDoc = (CFilmDoc*)m_document[FILM_WINDOW];
	CKomaData* pKoma = (CKomaData*)(pDoc->GetNowSelectKoma());
	if (pKoma == NULL) return -1;

	CLayerDoc* pDoc2 = (CLayerDoc*)m_document[LAYER_WINDOW];
	int layer = pDoc2->GetSelectEffectLayer();
	if (layer == -1) return -1;

	if (pKoma->GetEffectFlag(layer) == FALSE) return -1;
	int effect = pKoma->GetEffect(layer);

	return effect;
}

int CMyApplication::GetNowSelectParam(void)
{
	CEffectDoc* pDoc = (CEffectDoc*)m_document[EFFECT_WINDOW];
	return pDoc->GetNowSelectParam();
}


CStoryBookData* CMyApplication::GetStoryBookData(void)
{
	CStoryBookDoc* storyBook = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	return storyBook->GetStoryBookData();
}

CFilmCaseData* CMyApplication::GetFilmCaseData(void)
{
	CFilmCaseDoc* filmCase = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
	return filmCase->GetFilmCaseData();
}

BOOL CMyApplication::CheckWinNT(void)
{
	return m_winNTFlag;
}

void CMyApplication::CreateNextKoma(void)
{
	CFilmData* pFilm = GetNowSelectFilm();
	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if (n<kosuu)
	{
		n++;
	}

	pFilm->CreateObjectData(n);
	pFilm->SetSelectNumber(n);


	CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(n));
//	pKoma->Init();

	pKoma->CreateObjectData();
	pKoma->SetSelectNumber(0);
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(0));
//	pMessage->Init();
	pKoma->SetSelectNumber(0);


}

void CMyApplication::ChangeVarBlock(int n)
{
	int varType = GetVarType();

	int kosuu = m_varBlockTable[n][0];
	if (varType > 0)
	{
		kosuu = m_varBlockTable2[n][0];
	}


	SetConfig("varBlockNumber",kosuu);
	SetConfig("varBlockType",n);

	for (int i=0;i<kosuu;i++)
	{
		char mes[256];
		wsprintf(mes,"varBlock%d",i+1);
		if (varType == 0)
		{
			SetConfig(mes, m_varBlockTable[n][i + 1]);
		}
		else
		{
			SetConfig(mes, m_varBlockTable2[n][i + 1]);
		}
	}



	CVarDoc* pDoc = (CVarDoc*)m_document[VAR_WINDOW];
	pDoc->ReCalcuVarKosuu();

	UpdateMyWindow(VAR_WINDOW);
	m_menuCheckControl->SetVarBlockMenuCheck();
}


void CMyApplication::EditFrameIsChanged(void)
{
	CMainScreenDoc* pDoc = (CMainScreenDoc*)m_document[MAINSCREEN_WINDOW];
	pDoc->EditFrameIsChanged();
}



//build setup
void CMyApplication::SetAutoSetCGFlag(int flg)
{
	m_autoSetCGFlag = flg;
	SetConfig("autoSetCGFlag",m_autoSetCGFlag);
	m_menuCheckControl->SetBuildToMenuCheck();
}

void CMyApplication::SetSceneMakeFlag(int flg)
{
	m_sceneMakeFlag = flg;
	SetConfig("sceneMakeFlag",m_sceneMakeFlag);
	m_menuCheckControl->SetBuildToMenuCheck();
}

void CMyApplication::SetCheckSameCGNameFlag(int flg)
{
	m_checkSameCGNameFlag = flg;
	SetConfig("checkSameCGNameFlag",m_checkSameCGNameFlag);
	m_menuCheckControl->SetBuildToMenuCheck();
}

void CMyApplication::SetBuildRubiFlag(int flg)
{
	m_rubiBuildFlag = flg;
	SetConfig("rubiBuildFlag",flg);
	m_menuCheckControl->SetRubiBuildType();
}

void CMyApplication::SetBuildCutinFlag(int flg)
{
	m_cutinBuildFlag = flg;
	SetConfig("cutinBuildFlag",flg);
	m_menuCheckControl->SetCutinBuildType();
}

void CMyApplication::SetTermFlag(int n)
{
	m_termFlag = n;
	SetConfig("termFlag",n);
	m_menuCheckControl->SetTermFlagCheck();
}

void CMyApplication::ChangeBalloonFlag(int n)
{
	m_balloonFlag = n;
	SetConfig("balloonFlag",n);
	m_menuCheckControl->SetBalloonCheck();
}

void CMyApplication::ChangeZahyoPrintType(int n)
{
	m_zahyoPrintType = n;
	SetConfig("zahyoPrintType",n);
	m_menuCheckControl->SetZahyoTypeCheck();
}

void CMyApplication::SetTargetMachine(int n)
{
	SetConfig("targetMachine",n);
	m_menuCheckControl->SetTargetMachineCheck();
}


void CMyApplication::SetKanjiCode(int flg)
{
	SetConfig("kanjiCode",flg);
	m_menuCheckControl->SetKanjiCodeType();

	PrintNextTimeSetup();
}


void CMyApplication::SetWarningDelScene(int flg)
{
	if (flg == -1)
	{
		if (m_warningDeleteScene == 0)
		{
			flg = 1;
		}
		else
		{
			flg = 0;
		}
	}

	m_warningDeleteScene = flg;
	SetConfig("warningDeleteStory",m_warningDeleteScene);
	m_menuCheckControl->SetDeleteToMenuCheck();
}

void CMyApplication::SetWarningDelCommand(int flg)
{
	if (flg == -1)
	{
		if (m_warningDeleteCommand == 0)
		{
			flg = 1;
		}
		else
		{
			flg = 0;
		}
	}

	m_warningDeleteCommand = flg;
	SetConfig("warningDeleteCommand",m_warningDeleteCommand);
	m_menuCheckControl->SetDeleteToMenuCheck();
}

void CMyApplication::SetWarningDelFilm(int flg)
{
	if (flg == -1)
	{
		if (m_warningDeleteFilm == 0)
		{
			flg = 1;
		}
		else
		{
			flg = 0;
		}
	}

	m_warningDeleteFilm = flg;
	SetConfig("warningDeleteFilm",m_warningDeleteFilm);
	m_menuCheckControl->SetDeleteToMenuCheck();
}

void CMyApplication::SetWarningDelKoma(int flg)
{
	if (flg == -1)
	{
		if (m_warningDeleteKoma == 0)
		{
			flg = 1;
		}
		else
		{
			flg = 0;
		}
	}

	m_warningDeleteKoma = flg;
	SetConfig("warningDeleteKoma",m_warningDeleteKoma);
	m_menuCheckControl->SetDeleteToMenuCheck();
}

void CMyApplication::SetWarningDelMessage(int flg)
{
	if (flg == -1)
	{
		if (m_warningDeleteMessage == 0)
		{
			flg = 1;
		}
		else
		{
			flg = 0;
		}
	}

	m_warningDeleteMessage = flg;
	SetConfig("warningDeleteMessage",m_warningDeleteMessage);
	m_menuCheckControl->SetDeleteToMenuCheck();
}

void CMyApplication::SetWarningDelParam(int flg)
{
	if (flg == -1)
	{
		if (m_warningDeleteParam == 0)
		{
			flg = 1;
		}
		else
		{
			flg = 0;
		}
	}

	m_warningDeleteParam = flg;
	SetConfig("warningDeleteParam",m_warningDeleteParam);
	m_menuCheckControl->SetDeleteToMenuCheck();
}


void CMyApplication::ChangeWarningDelVar(void)
{
	if (m_warningDeleteVar == 0)
	{
		m_warningDeleteVar = 1;
	}
	else
	{
		m_warningDeleteVar = 0;
	}
	SetConfig("warningDeleteVar",m_warningDeleteVar);
	m_menuCheckControl->SetDeleteToMenuCheck();
}


////////////

void CMyApplication::SetMesNameByInit(int n)
{
	m_messageSetup->SetMesNameByInit(n);
	m_menuCheckControl->SetMesNameMenuCheck();
}

void CMyApplication::SetMesPrintMode(int n)
{
	m_messageSetup->SetMesWinPrintMode(n);
	m_menuCheckControl->SetMesNameMenuCheck();
	CheckAndLoadMesWinPic();
}

void CMyApplication::ChangeMesSize(void)
{

	RECT rc;
	POINT pt = m_messageSetup->GetMesWinZahyo();
	SIZE sz = m_messageSetup->GetMesWinSize();

//	rc.left = GetConfig("mesWinPrintX");
//	rc.top = GetConfig("mesWinPrintY");
//	rc.right = GetConfig("mesWinSizeX");
//	rc.bottom = GetConfig("mesWinSizeY");
	rc.left = pt.x;
	rc.top = pt.y;
	rc.right = sz.cx;
	rc.bottom = sz.cy;

	RECT newRect = rc;
	if (m_input->GetRect(rc,&newRect,"メッセージウィンドウの位置とサイズを設定"))
	{
		SetConfig("mesWinPrintX",newRect.left);
		SetConfig("mesWinPrintY",newRect.top);
		SetConfig("mesWinSizeX",newRect.right);
		SetConfig("mesWinSizeY",newRect.bottom);

		PrintNextTimeSetup();
	}
}


void CMyApplication::ChangeMesFileName(void)
{
	Mijissou();
	CheckAndLoadMesWinPic();
}

void CMyApplication::ChangeMesPrintZahyo(void)
{
	POINT pt;
	pt.x = GetConfig("mesPrintX");
	pt.y = GetConfig("mesPrintY");
	POINT newPoint = pt;
	if (m_input->GetPoint(pt,&newPoint,"メッセージの表示位置を設定"))
	{
		SetConfig("mesPrintX",newPoint.x);
		SetConfig("mesPrintY",newPoint.y);

		PrintNextTimeSetup();
	}
}

void CMyApplication::ChangeMesFontSize(void)
{
	int d = GetConfig("mesFontSize");
	int newData = d;
	if (m_input->GetNumber(d,&newData,""))
	{
		if ((newData > 0) && (newData<=64))
		{
			SetConfig("mesFontSize",newData);
			PrintNextTimeSetup();
		}
	}
//	PrintNextTimeSetup();
}

void CMyApplication::ChangeMesNextY(void)
{
	int d = GetConfig("mesPrintNextY");
	int newData = d;
	if (m_input->GetNumber(d,&newData,""))
	{
		if (newData > 0)
		{
			SetConfig("mesPrintNextY",newData);
			PrintNextTimeSetup();
		}
	}
//	PrintNextTimeSetup();
}

void CMyApplication::ChangeMesLPrintZahyo(void)
{
	POINT pt;
	pt.x = GetConfig("mesLPrintX");
	pt.y = GetConfig("mesLPrintY");
	POINT newPoint = pt;
	if (m_input->GetPoint(pt,&newPoint,"全画面メッセージの表示位置を設定"))
	{
		SetConfig("mesLPrintX",newPoint.x);
		SetConfig("mesLPrintY",newPoint.y);

		PrintNextTimeSetup();
	}
}

void CMyApplication::ChangeMesAppendZahyo(void)
{
	POINT pt;
	pt.x = GetConfig("mesAppendX");
	pt.y = GetConfig("mesAppendY");
	POINT newPoint = pt;
	if (m_input->GetPoint(pt,&newPoint,"全画面追加メッセージの表示位置を設定"))
	{
		SetConfig("mesAppendX",newPoint.x);
		SetConfig("mesAppendY",newPoint.y);

		PrintNextTimeSetup();
	}
}


void CMyApplication::SetNamePrintMode(int n)
{
	m_messageSetup->SetNameWinPrintMode(n);
	m_menuCheckControl->SetMesNameMenuCheck();
	CheckAndLoadNameWinPic();
}

void CMyApplication::ChangeNameSize(void)
{
	Mijissou();
}

void CMyApplication::ChangeNameFileName(void)
{
	Mijissou();
	CheckAndLoadNameWinPic();
}

void CMyApplication::ChangeNamePrintZahyo(void)
{
	Mijissou();
}

void CMyApplication::ChangeNameFontSize(void)
{
	Mijissou();
	PrintNextTimeSetup();
}


void CMyApplication::SetNameAutoFlag(int n)
{

	m_messageSetup->SetNameAutoOffFlag(n);
	m_menuCheckControl->SetMesNameMenuCheck();
}


void CMyApplication::CheckAndLoadMesWinPic(void)
{
	m_mesWinPicLoadFlag = FALSE;

	if (m_messageSetup->GetMesWinPrintMode() == 2)
	{
		LPSTR mesWinFileName = m_messageSetup->GetMesWinFileName();

		if (mesWinFileName != NULL)
		{
			if ((*mesWinFileName) != '@')
			{
				char filename[256];
				wsprintf(filename,"sys\\%s",mesWinFileName);
				if (m_mesWinPic->LoadDWQ(filename))
				{
					m_mesWinPicLoadFlag = TRUE;
				}
				else
				{
					MessageBox(m_frameHWND,filename,"File not found",MB_OK | MB_ICONEXCLAMATION);
				}
			}
		}
	}
}


void CMyApplication::CheckAndLoadNameWinPic(void)
{
	m_nameWinPicLoadFlag = FALSE;

	if (m_messageSetup->GetNameWinPrintMode() == 2)
	{
		LPSTR nameWinFileName = m_messageSetup->GetNameWinFileName();
		if (nameWinFileName != NULL)
		{
			if ((*nameWinFileName) != '@')
			{
				char filename[256];
				wsprintf(filename,"sys\\%s",nameWinFileName);
				if (m_nameWinPic->LoadDWQ(filename))
				{
					m_nameWinPicLoadFlag = TRUE;
				}
				else
				{
					MessageBox(m_frameHWND,filename,"File not found",MB_OK | MB_ICONEXCLAMATION);
				}
			}
		}
	}
}


void CMyApplication::ChangeDeleteKey(LPSTR name)
{
	int d = GetConfig(name);
	if (d == 0)
	{
		d = 1;
	}
	else
	{
		d = 0;
	}
	SetConfig(name,d);
	m_menuCheckControl->SetDeleteKeyMenuCheck();
}

void CMyApplication::CutinEnable(BOOL flg)
{
	SetConfig("cutin",flg);
	m_menuCheckControl->SetCutinMenuCheck();
	m_cutinFlag = flg;

	if (flg)
	{
//		if (m_cutinControl == NULL)
//		{
//			m_cutinControl = new CCutinControl(TRUE);
//		}
		if (m_cutinNameData == NULL)
		{
			m_cutinNameData = new CCutinNameData("nnndir\\cutinChara.txt");
		}
	}
}



void CMyApplication::SetVoiceReadType(BOOL packFlag)
{
	SetConfig("voicePack",packFlag);
	m_menuCheckControl->SetVoiceReadType();
	m_scriptVoiceControl->SetReadType(packFlag);
}

void CMyApplication::SetRubiPrint(int md)
{
	int old = GetConfig("rubiPrintFlag");
	SetConfig("rubiPrintFlag",md);
	m_menuCheckControl->SetRubiPrintType();

	if ((md > 0) && (old == 0))
	{
		PrintNextTimeSetup();
	}

	m_rubiPrintFlag = md;

	if (md == 2)
	{
		m_rubiMaker->Load();
	}
}



void CMyApplication::ChangeAngouCode(LPSTR name)
{
	int d = GetConfig(name);

	int newData = d;

	BOOL flg = FALSE;

	if (m_input->GetNumber(d,&newData,""))
	{
		SetConfig(name,newData);
		flg = TRUE;
	}
	PrintAngouCode(flg);
}

void CMyApplication::ClearAngouCode(void)
{
	SetConfig("angouXorCode",-1);
	SetConfig("angouXorAddCode",0);
	SetConfig("angouIncAddCode",0);

	MessageBox(m_frameHWND,"暗号キーをクリアーしました","暗号",MB_OK);
}


void CMyApplication::PrintAngouCode(BOOL changeFlag)
{
	char mes[1024];
	int d1 = GetConfig("angouXorCode");
	int d2 = GetConfig("angouXorAddCode");
	int d3 = GetConfig("angouIncAddCode");

	if (changeFlag)
	{
		wsprintf(mes,"暗号キーを変更しました\n現在のキーは\ncode1=%d\ncode2=%d\ncode3=%d\nです。この暗号キーをプログラム側で設定する必要がありますので注意してください",d1,d2,d3);
	}
	else
	{
		wsprintf(mes,"現在のキーは\ncode1=%d\ncode2=%d\ncode3=%d\nです。",d1,d2,d3);
	}
	MessageBox(m_frameHWND,mes,"暗号",MB_OK);
}



void CMyApplication::Mijissou(void)
{
	MessageBox(m_frameHWND,"実装途中","AAA",MB_OK);
}

void CMyApplication::PrintNextTimeSetup(void)
{
	MessageBox(m_frameHWND,"この設定は、次回起動時から有効になります","確認",MB_OK);
}



void CMyApplication::PlayScriptMessageVoice(CMessageData* pMessage,int channel)
{
	m_scriptVoiceControl->PlayScriptMessageVoice(pMessage,channel);
}

void CMyApplication::PlaySeTest(int n,int channel)
{
	m_scriptSoundControl->PlaySeTest(n,channel);
}

void CMyApplication::PlayScriptMessageSound(CMessageData* pMessage,int channel)
{
	m_scriptSoundControl->PlayScriptMessageSound(pMessage,channel);
}


BOOL CMyApplication::SaveRoutine(int n)
{
	char filename[1024];
	wsprintf(filename,"nnndir\\nnn\\%s.nnn",m_projectList->GetName(n));
	FILE* file = CMyFile::Open(filename,"wb");
	if (file == NULL)
	{
		MessageBox(m_frameHWND,"SAVE OPEN ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	fwrite("NNN DATA HEADER",sizeof(char),16,file);
	fwrite(m_myname,sizeof(char),64,file);
	fwrite(m_memo,sizeof(char),256,file);

	int tmp[16];
	for (int i=0;i<16;i++) tmp[i] = 0;

	tmp[0] = 2;
	fwrite(tmp,sizeof(int),16,file);


	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
	CFilmCaseData* pCase = pDoc->GetFilmCaseData();
	pCase->Save(file);


	CStoryBookDoc* pDoc2 = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	CStoryBookData* pBook = pDoc2->GetStoryBookData();
	pBook->Save(file);

	fclose(file);
	return TRUE;
}

void CMyApplication::SetMovieSize(int n)
{
	SetConfig("movieSize",n);
	m_menuCheckControl->SetMovieSizeMenuCheck();
}

void CMyApplication::SetMoviePackType(int n)
{
	SetConfig("moviePackType",n);
	m_menuCheckControl->SetMoviePackMenuCheck();
}


void CMyApplication::SetFilmSelectMenuType(int n)
{
	SetConfig("selFilmCommand",n);
	m_menuCheckControl->SetFilmStorySelectMenuCheck();
}

void CMyApplication::SetStorySelectMenuType(int n)
{
	SetConfig("selStoryCommand",n);
	m_menuCheckControl->SetFilmStorySelectMenuCheck();
}

void CMyApplication::SetCalcuMenuType(int n)
{
	SetConfig("selCalcuCommand",n);
	m_menuCheckControl->SetCalcuMenuCheck();
}


void CMyApplication::ChangePercentOverrap(void)
{
	int d = GetConfig("autoSetOverrapFlag");
	if (d == 0)
	{
		d = 1;
	}
	else
	{
		d = 0;
	}
	SetConfig("autoSetOverrapFlag",d);
	
	m_menuCheckControl->SetPercentOverrapMenuCheck();
}

int CMyApplication::GetFilmSelectDefault(void)
{
	return GetConfig("selFilmCommand");
}

int CMyApplication::GetStorySelectDefault(void)
{
	return GetConfig("selStoryCommand");
}

int CMyApplication::GetCalcuSelectDefault(void)
{
	return GetConfig("selCalcuCommand");
}

void CMyApplication::SetEffectMenuMoveFlag(int flg)
{
	m_effectCopyMoveFlag = flg;
	SetConfig("effectCopyMoveFlag",m_effectCopyMoveFlag);
	m_menuCheckControl->SetEffectMenuCheck();
}



void CMyApplication::CheckAndSetProject(void)
{
	if (SearchProject(m_fileNameOnly) == -1)
	{
		int rt = MessageBox(m_frameHWND,"プロジェクトに登録しますか","追加",MB_OKCANCEL);
		if (rt == IDOK)
		{
			AddProject(m_fileNameOnly);
		}
	}
}


int CMyApplication::SearchProject(LPSTR filename)
{
	int kosuu = m_projectList->GetNameKosuu();
	for (int i=0;i<kosuu;i++)
	{
		if (_stricmp(filename,m_projectList->GetName(i)) == 0) return i;
	}
	return -1;
}


BOOL CMyApplication::AddProject(LPSTR filename)
{
	m_projectList->AddName(filename);
	FILE* file = CMyFile::Open("nnndir\\project.txt","wb");
	if (file == NULL) return FALSE;

	int kosuu = m_projectList->GetNameKosuu();
	char mes[256];
	for (int i=0;i<kosuu;i++)
	{
		wsprintf(mes,"%s\x00d\x00a",m_projectList->GetName(i));
		int ln = strlen(mes);
		fwrite(mes,sizeof(char),ln,file);
	}
	fclose(file);
	return TRUE;
}



BOOL CMyApplication::DeleteProject(LPSTR filename)
{
	int n = SearchProject(filename);
	m_projectList->DeleteBlock(n);

	FILE* file = CMyFile::Open("nnndir\\project.txt","wb");
	if (file == NULL) return FALSE;
	int kosuu = m_projectList->GetNameKosuu();
	char mes[256];
	for (int i=0;i<kosuu;i++)
	{
		wsprintf(mes,"%s\x00d\x00a",m_projectList->GetName(i));
		int ln = strlen(mes);
		fwrite(mes,sizeof(char),ln,file);
	}

	fclose(file);

	return TRUE;
}


void CMyApplication::OnDeleteProject(void)
{
	int n = m_selectDialog->GetSelect1(m_projectList);
	if (n == -1) return;

	if (n == 0)
	{
		MessageBox(m_frameHWND,"sysは、削除できません","エラー",MB_OK | MB_ICONSTOP);
		return;
	}

	char mes[256];
	wsprintf(mes,"%sをプロジェクトから削除してもよろしくて?",m_projectList->GetName(n));
	int rt = MessageBox(m_frameHWND,mes,"確認",MB_OKCANCEL | MB_ICONEXCLAMATION);
	if (rt == IDOK)
	{
		DeleteProject(m_projectList->GetName(n));
	}
}

void CMyApplication::OnAddCommandList(void)
{
}

void CMyApplication::OnDeleteCommandList(void)
{
}

void CMyApplication::OnAddFunctionList(void)
{
}

void CMyApplication::OnDeleteFunctionList(void)
{
}




void CMyApplication::PlayMusicTest(int n)
{
	OutputDebugString("[play music test]");


	int loopCount = 1;
	int vol = 100;
	int fadeInTime = 1;
	int fadeOutTime = 10;

	LPSTR filename = m_bgmList->GetName(n*2);
	m_musicControl->PlayMusic(filename,loopCount,vol,fadeInTime,fadeOutTime);
}

void CMyApplication::OnTabKey(int windowNumber,int shift)
{
	static int winTable[]=
	{
		MAINSCREEN_WINDOW,
	
		FILMCASE_WINDOW,
		FILM_WINDOW,
		GAMEMESSAGE_WINDOW,
		LAYER_WINDOW,
		EFFECT_WINDOW,
		ZAHYO_WINDOW,

		CONTROL_WINDOW,

		STORYBOOK_WINDOW,
		STORY_WINDOW,
		PROGRAM_WINDOW,
		CONTE_WINDOW,
		CONTEMESSAGE_WINDOW,
		VAR_WINDOW,
	};

	int kosuu = 14;
	int n = -1;
	int i;
	for (i=0;i<kosuu;i++)
	{
		if (winTable[i] == windowNumber)
		{
			n = i;
			break;
		}
	}

	if (n == -1) return;
	
	int delta = 1;
	if (shift) delta = kosuu -1;

	int found = -1;
	for (i=0;i<kosuu;i++)
	{
		n += delta;
		n %= kosuu;

		int winNum = winTable[n];
		if (CheckShowWindow(winNum))
		{
			found = n;
			break;
		}
	}


	if (found != -1)
	{
		ChangeWindowIfCan(winTable[n]);
	}
}


void CMyApplication::ChangeWindowIfCan(int windowNumber)
{
	if (CheckShowWindow(windowNumber) == FALSE) return;

	HWND hwnd = m_document[windowNumber]->GetViewHWND();
	if (hwnd  == NULL) return;

	m_frame->ToTop(hwnd);
}


BOOL CMyApplication::CheckShowWindow(int windowNumber)
{
	if (windowNumber == -1) return FALSE;

	CMyDocument* pDoc = m_document[windowNumber];
	if (pDoc == NULL) return FALSE;

	CMyView* pView = pDoc->GetView();
	if (pView == NULL) return FALSE;

	int md = m_nnnConfig->GetValue("windowMode");
	LPSTR windowName = pView->GetMyViewWindowName();
	char mes[256];
	wsprintf(mes,"%sFlag%d",windowName,md+1);
	return GetConfig(mes);
}



void CMyApplication::ScrollToSelect(int windowNumber)
{
	CMyDocument* pDoc = m_document[windowNumber];
	pDoc->ScrollToSelect();
}


void CMyApplication::DataIsChanged(int windowNumber)
{
	if (m_windowUpdateControl != NULL) m_windowUpdateControl->MyUpdateWindow(windowNumber);
}

void CMyApplication::ChangeFilmPlayMode(void)
{
	m_filmPlayMode++;
	m_filmPlayMode &= 1;
}

void CMyApplication::StopAllMusicAndSound(void)
{
	m_musicControl->StopMusic();

	int i;
	for (i=0;i<8;i++)
	{
		m_scriptSoundControl->StopScriptSound(i);
	}

	for (i=0;i<4;i++)
	{
		m_scriptVoiceControl->StopScriptVoice(i);
	}
}

void CMyApplication::Debug(void)
{
	LPSTR fileNameOnly = GetFileNameOnly();
	int ln = strlen(fileNameOnly);
	if (ln>254) ln = 254;

	char lastFileName[256];
	memcpy(lastFileName,fileNameOnly,ln);
	lastFileName[ln] = 0;
	lastFileName[ln+1] = 0;

	if (CheckSave() == FALSE) return;

	int wipes[256];
	int effects[256];
	int w;
	for (w=0;w<256;w++)
	{
		wipes[w] = 0;
		effects[w] = 0;
	}

	CNameList* overrapName = new CNameList();
	overrapName->LoadFile("nnndir\\setup\\list\\wipenamelist.txt");

	int nnnKosuu = m_projectList->GetNameKosuu();
	for (int pp=0;pp<nnnKosuu;pp++)
	{
		LPSTR nnnfilename = m_projectList->GetName(pp);
		if (nnnfilename != NULL)
		{
			SetFileNameOnly(nnnfilename);

			if (LoadByFileNameOnly(nnnfilename) == FALSE)
			{
				MessageBox(NULL,"ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
				continue;
			}

			SetFileNameOnly(nnnfilename);

			CFilmCaseData* pCase = GetFilmCaseData();
			if (pCase == NULL) continue;

			int filmKosuu = pCase->GetObjectKosuu();

			for (int f=0;f<filmKosuu;f++)
			{
				CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(f));
				if (pFilm != NULL)
				{
					int komaKosuu = pFilm->GetObjectKosuu();
					for (int j=0;j<komaKosuu;j++)
					{
						CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(j));
						if (pKoma != NULL)
						{
							for (int e=0;e<16;e++)
							{
								if (pKoma->GetEffectFlag(e))
								{
									int effect = pKoma->GetEffect(e);
									if (effect > 0)
									{
										effects[effect] = 1;
									}
								}
							}
						}
					}
				}
			}
		}
	}


	FILE* file = CMyFile::Open("useeffectlist.txt","wb");
	for (w=0;w<256;w++)
	{
		if (effects[w] > 0)
		{
			char mes[256];
			wsprintf(mes,"%d:\x00d\x00a",w);
			int ln = strlen(mes);
			fwrite(mes,sizeof(char),ln,file);
		}
	}

	fclose(file);

	delete overrapName;

	MessageBox(NULL,"おわりましたにゃ useeffectlist.txt","確認",MB_OK);

	SetFileNameOnly(lastFileName);
	LoadByFileNameOnly(lastFileName);

	return;

}


void CMyApplication::PrintVersion(void)
{
	char mes[] = __DATE__;

	MessageBox(m_frameHWND,mes,"Version",MB_OK | MB_APPLMODAL);
}


void CMyApplication::BugBug(void)
{
	LPSTR fileNameOnly = GetFileNameOnly();
	int ln = strlen(fileNameOnly);
	if (ln>254) ln = 254;

	char lastFileName[256];
	memcpy(lastFileName,fileNameOnly,ln);
	lastFileName[ln] = 0;
	lastFileName[ln+1] = 0;



	FILE* bugFile = CMyFile::Open("bug.txt","wb");

	int nnnKosuu = m_projectList->GetNameKosuu();
	for (int pp=0;pp<nnnKosuu;pp++)
	{
		LPSTR nnnfilename = m_projectList->GetName(pp);
		if (nnnfilename != NULL)
		{
			SetFileNameOnly(nnnfilename);

			if (LoadByFileNameOnly(nnnfilename) == FALSE)
			{
				MessageBox(NULL,"ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
				continue;
			}

			SetFileNameOnly(nnnfilename);

			CFilmCaseData* pCase = GetFilmCaseData();

			if (pCase == NULL) continue;

			int filmKosuu = pCase->GetObjectKosuu();

			for (int f=0;f<filmKosuu;f++)
			{
				CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(f));
				if (pFilm != NULL)
				{
					int komaKosuu = pFilm->GetObjectKosuu();
					for (int j=0;j<komaKosuu;j++)
					{
						CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(j));
						if (pKoma != NULL)
						{
							int k = 5;
							if (pKoma->GetEffectFlag(k))
							{
								char emes[1024];
								wsprintf(emes,"nnn=%s , film=%s koma=%d\x00d\x00a",nnnfilename,pFilm->GetMyName(),j+1);
								fwrite(emes,sizeof(char),strlen(emes),bugFile);
							}
						}
					}
				}

			}
		}
	}

	fclose(bugFile);

//	DELETEARRAY(namelist);

	MessageBox(NULL,"おわりましたにゃ","確認",MB_OK);

	SetFileNameOnly(lastFileName);
	LoadByFileNameOnly(lastFileName);
}


void CMyApplication::ChangeDefaultFrame(void)
{
	int oldData = GetConfig("frameTime");
	int newData = oldData;
	if (m_input->GetNumber(oldData,&newData,"フレーム時間をミリ秒で指定"))
	{
		if ((newData >= 1) && (newData <= 256))
		{
			SetConfig("frameTime",newData);
			PrintNextTimeSetup();
		}
	}
}


void CMyApplication::ReplaceVarName(int varNumber,LPSTR searchString,LPSTR replaceString)
{
	CVarDoc* varDoc = (CVarDoc*)(m_document[VAR_WINDOW]);
	if (varDoc != NULL)
	{
		if (varNumber >= 100)
		{
			varDoc->ReplaceVarName(varNumber,searchString,replaceString);
		}
	}
}

void CMyApplication::SetSelectVarNumber(int varNumber)
{
	CVarDoc* varDoc = (CVarDoc*)(m_document[VAR_WINDOW]);
	if (varDoc != NULL)
	{
		varDoc->SetSelectVarNumber(varNumber);
	}
}


void CMyApplication::SearchVar(BOOL replaceFlag)
{
	CVarDoc* varDoc = (CVarDoc*)(m_document[VAR_WINDOW]);
	if (varDoc != NULL)
	{
		varDoc->OnSearchButton(replaceFlag);
	}
}



void CMyApplication::OutputAllFilmName(void)
{

	char lastFileName[256];
	memcpy(lastFileName,m_fileNameOnly,strlen(m_fileNameOnly)+1);

	if (m_modifyFlag)
	{
		int rt = MessageBox(m_frameHWND,"変更されていますにゃ\n保存しますか","警告",MB_YESNOCANCEL | MB_APPLMODAL | MB_ICONEXCLAMATION );
		if (rt == IDCANCEL) return;
		if (rt == IDYES) Save();
	}


	BOOL flg = FALSE;

	FILE* file = NULL;
	fopen_s(&file,"nnndir\\tmp\\filmnamelist.txt","wb");

	int nnnKosuu = m_projectList->GetNameKosuu();
	for (int pp=0;pp<nnnKosuu;pp++)
	{
		LPSTR nnnfilename = m_projectList->GetName(pp);
		if (nnnfilename != NULL)
		{
			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);

			if (LoadByFileNameOnly(nnnfilename) == FALSE)
			{
				MessageBox(m_frameHWND,nnnfilename,"Load ERROR",MB_OK | MB_ICONEXCLAMATION);
				flg = TRUE;
				break;
			}

			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);


			CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
			CFilmCaseData* pCase = pDoc->GetFilmCaseData();

			CStoryBookDoc* pDoc2 = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
			CStoryBookData* pBook = pDoc2->GetStoryBookData();

			char mes[1024];
			wsprintf(mes,"[%s]\x00d\x00a",nnnfilename);
			fwrite(mes,1,strlen(mes),file);


			int kosuu = pDoc->GetFilmKosuu();
			for (int i=0;i<kosuu;i++)
			{
				CFilmData* pFilm = pDoc->GetFilm(i);

				LPSTR filmname = pFilm->GetMyName();
				wsprintf(mes,"@%s %s\x00d\x00a",nnnfilename,filmname);
				fwrite(mes,1,strlen(mes),file);
			}
		}
	}


	fclose(file);


	memcpy(m_fileNameOnly,lastFileName,strlen(lastFileName)+1);
	LoadByFileNameOnly(lastFileName);

	MessageBox(m_frameHWND,"すべておわりましたにゃ","確認",MB_OK);

}

void CMyApplication::SetRubiColor(void)
{
	int rgb[3];
	rgb[0] = GetConfig("rubiColorR");
	rgb[1] = GetConfig("rubiColorG");
	rgb[2] = GetConfig("rubiColorB");

	char caption[3][32]=
	{
		"ルビ色赤","ルビ色緑","ルビ色青"
	};

	for (int i=0;i<3;i++)
	{
		int oldData = rgb[i];
		int newData = oldData;
		if (m_input->GetNumber(oldData,&newData,caption[i]))
		{
			if ((newData >= 0) && (newData <= 255))
			{
				rgb[i] = newData;
			}
		}
		else
		{
			return;
		}
	}

	SetConfig("rubiColorR",rgb[0]);
	SetConfig("rubiColorG",rgb[1]);
	SetConfig("rubiColorB",rgb[2]);

	SetRubiParam();
}


void CMyApplication::ChangeRubiColorFix(void)
{
	int md = GetConfig("rubiColorFix");
	md = 1 - md;
	SetConfig("rubiColorFix",md);
	m_menuCheckControl->SetRubiColorFixType();

	SetRubiParam();
}

void CMyApplication::SetRubiSize(void)
{
	int oldData = GetConfig("rubiPercent");
	int newData = oldData;

	if (m_input->GetNumber(oldData,&newData,"ルビサイズを%で指定(1-100)"))
	{
		if ((newData >= 1) && (newData <= 100))
		{
			SetConfig("rubiPercent",newData);
		}
	}

	PrintNextTimeSetup();
//	SetRubiParam();
}


void CMyApplication::SetRubiSukima(void)
{
	int oldData = GetConfig("rubiSukima");
	int newData = oldData;

	if (m_input->GetNumber(oldData,&newData,"ルビ文字間を指定"))
	{
		if (newData >= 0)
		{
			SetConfig("rubiSukima",newData);
		}
	}

	SetRubiParam();
}

void CMyApplication::SetRubiDeltaY(void)
{
	int oldData = GetConfig("rubiDealtaY");
	int newData = oldData;

	if (m_input->GetNumber(oldData,&newData,"ルビ表示Y座標補正を指定"))
	{
		SetConfig("rubiDeltaY",newData);
	}

	SetRubiParam();
}

void CMyApplication::SetRubiParam(void)
{
	int rubiColorR = GetConfig("rubiColorR");
	int rubiColorG = GetConfig("rubiColorG");
	int rubiColorB = GetConfig("rubiColorB");
	int rubiPercent = GetConfig("rubiPercent");
	int rubiColorFix = GetConfig("rubiColorFix");
	int rubiSukima = GetConfig("rubiSukima");
	int rubiPrintDeltaY = GetConfig("rubiDeltaY");

	CMyFont::m_rubiColorR = rubiColorR;
	CMyFont::m_rubiColorG = rubiColorG;
	CMyFont::m_rubiColorB = rubiColorB;
//	CMyFont::m_rubiPercent = rubiPercent;
	CMyFont::m_rubiColorFixFlag = rubiColorFix;
	CMyFont::m_rubiSukima = rubiSukima;
	CMyFont::m_rubiDeltaY = rubiPrintDeltaY;
}


void CMyApplication::ReloadCutinFile(void)
{
	if (m_cutinNameData == NULL)
	{
		m_cutinNameData = new CCutinNameData("nnndir\\cutinChara.txt");
	}
}

void CMyApplication::SetJpegMMX(BOOL flg)
{
	int d = 0;
	if (flg)
	{
		d = 1;
	}

	SetConfig("jpegMMX",d);
	m_menuCheckControl->SetJpegMMX();
}

void CMyApplication::SetBackupNNN(BOOL flg)
{
	int d = 0;
	if (flg)
	{
		d = 1;
	}

	SetConfig("backupNNN",d);
	m_menuCheckControl->SetBackupNNNMode();
}


void CMyApplication::ReLoadSeList(void)
{
	ENDDELETECLASS(m_seList);
	m_seList = new CNameList();
	m_seList->LoadFile(SE_FILE_NAME);
}

void CMyApplication::ReLoadBGMList(void)
{
	ENDDELETECLASS(m_bgmList);
	m_bgmList = new CNameList();
	m_bgmList->LoadFile(BGM_FILE_NAME);
}

int CMyApplication::GetEditorMessageGyo(void)
{
	return GetConfig("editorMessageGyo");
}

int CMyApplication::GetMessageWidthGuideLine(void)
{
	return GetConfig("messageWidthGuideLine");
}

void CMyApplication::ChangeMessageGyo(void)
{
	int oldData = GetEditorMessageGyo();
	int newData = oldData;

	if (m_input->GetNumber(oldData,&newData,"行数を指定"))
	{
		if ((newData > 0) && (newData <= 20))
		{
			SetConfig("editorMessageGyo",newData);
			PrintNextTimeSetup();
		}
		else
		{
			MessageBox(m_frameHWND,"設定可能な行数は、1～20までです","ERROR",MB_OK | MB_ICONEXCLAMATION);
		}
	}
}

void CMyApplication::ChangeMessageGuide(void)
{
	int oldData = GetMessageWidthGuideLine();
	int newData = oldData;
	if (m_input->GetNumber(oldData,&newData,"ガイドライン文字数を指定"))
	{
		if (newData > 0)
		{
			SetConfig("messageWidthGuideLine",newData);
			PrintNextTimeSetup();
		}
	}

}

void CMyApplication::SetFrameTime(int frame)
{
	m_frameTime = frame;
	if (m_shakinControl != NULL)
	{
		m_shakinControl->SetFrame(frame);
	}
}


void CMyApplication::ChangePrintFrameFlag(void)
{
	if (m_framePrintFlag == 0)
	{
		m_framePrintFlag = 1;
	}
	else
	{
		m_framePrintFlag = 0;
	}
	SetConfig("PrintFrameFlag",m_framePrintFlag);
	m_menuCheckControl->SetPrintFrameCheck();
}

void CMyApplication::ChangePrintSkipFlag(void)
{
	if (m_skipPrintFlag == 0)
	{
		m_skipPrintFlag = 1;
	}
	else
	{
		m_skipPrintFlag = 0;
	}
	SetConfig("PrintSkipFlag",m_skipPrintFlag);
	m_menuCheckControl->SetPrintSkipCheck();
}



void CMyApplication::SetMaxSkip(void)
{
	int oldData = GetConfig("skipMax");
	int newData = oldData;

	if (m_input->GetNumber(oldData,&newData,"最大スキップ数を指定"))
	{
		if ((newData > 0) && (newData <= 999))
		{
			SetConfig("skipMax",newData);
			m_shakinControl->SetMax(newData);
		}
	}
}

void CMyApplication::SetSkipInterval(void)
{
	int oldData = GetConfig("skipInterval");
	int newData = oldData;

	if (m_input->GetNumber(oldData,&newData,"スキップ期間を指定"))
	{
		if ((newData > 0) && (newData <= 999))
		{
			SetConfig("skipInterval",newData);
			m_shakinControl->SetKikan(newData);
		}
	}
}

void CMyApplication::SetLayerCopyMode(int md)
{
	SetConfig("layerCopyEffectMode",md);
	m_menuCheckControl->SetLayerCopyEffectModeCheck();
	((CLayerDoc*)m_document[LAYER_WINDOW])->SetCopyPreEffectMode(md);
	UpdateMyWindow(LAYER_WINDOW);
}

void CMyApplication::SetPresetButtonOk(int flg)
{
	m_presetButtonEnableFlag = flg;
	SetConfig("presetButtonEnable",flg);
	m_menuCheckControl->SetPresetZahyoCheck();
	UpdateMyWindow(ZAHYO_WINDOW);
}



void CMyApplication::MoveFilmTop(void)
{
	SetPlayMode(FALSE);

//	((CFilmCaseDoc*)m_document[FILMCASE_WINDOW])->OnSelectFilm(0);
	((CFilmDoc*)m_document[FILM_WINDOW])->OnClickFilm(0,0);
	((CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW])->OnSelectMessage(0,0);
}

void CMyApplication::CheckSceneData(BOOL okPrintFlag)
{
	CSceneList* sceneList = NULL;
	CSceneVoice* sceneVoice = NULL;

	BOOL errorFlag = FALSE;

	//check scene.dat and scnevoice
	sceneList = new CSceneList();

	if (errorFlag == FALSE)
	{
		if (sceneList->CheckLoadOk() == FALSE)
		{
			MessageBox(m_frameHWND,"spt/scene.datがありません","警告",MB_OK | MB_ICONEXCLAMATION);
			errorFlag = TRUE;
		}
	}


	if (errorFlag == FALSE)
	{
		sceneVoice = new CSceneVoice();
		if (sceneVoice->CheckLoadOk() == FALSE)
		{
			MessageBox(m_frameHWND,"nya/sceneVoice.xtxがありません","警告",MB_OK | MB_ICONEXCLAMATION);
			errorFlag = TRUE;
		}

	}

	if (errorFlag == FALSE)
	{
		int ninzu1 = sceneList->GetSceneCharaKosuu();
		int ninzu2 = sceneVoice->GetScenePlayerKosuu();
		
		if (ninzu1 != ninzu2)
		{
			char mes[1024];
			wsprintf(mes,"spt/scene.datとnya/sceneVoice.xtxの人数が違います\n\nspt/scene.dat=%d\nnya/sceneVoice.xtx=%d",ninzu1,ninzu2);
			MessageBox(m_frameHWND,mes,"警告",MB_OK | MB_ICONEXCLAMATION);
			errorFlag = TRUE;
		}

		if (errorFlag == FALSE)
		{
			for (int i=0;i<ninzu1;i++)
			{
				int kosuu1 = sceneList->GetSceneKosuu(i);
				int kosuu2 = sceneVoice->GetSceneKosuu(i);
				if (kosuu1 != kosuu2)
				{

					char mes[1024];
					wsprintf(mes,"spt/scene.datとnya/sceneVoice.xtxの%d番目のシーン数が違います\n\nspt/scene.dat=%d\nnya/sceneVoice.xtx=%d",i+1,kosuu1,kosuu2);
					MessageBox(m_frameHWND,mes,"警告",MB_OK | MB_ICONEXCLAMATION);
					errorFlag = TRUE;

					break;
				}
			}
		}
	}

	if (sceneList != NULL) delete sceneList;
	if (sceneVoice != NULL) delete sceneVoice;

	if (errorFlag == FALSE)
	{
		if (okPrintFlag)
		{
			MessageBox(m_frameHWND,"OK\n\nspt/scene.datとnya/sceneVoice.xtxに数の問題はありませんでした","問題なし",MB_OK);
		}
	}
}

void CMyApplication::SetSceneCheckMode(int md)
{
	SetConfig("sceneCheckMode",md);
	m_menuCheckControl->SetSceneCheckModeCheck();
}


void CMyApplication::ChangePngGamma(void)
{
	int md = GetConfig("pngGammaFlag");
	if (md == 0)
	{
		md = 1;
	}
	else
	{
		md = 0;
	}
	SetConfig("pngGammaFlag",md);
	m_menuCheckControl->SetPngGammaCheck();

	CPngLoader::m_gammaAdjustFlag = md;
	PrintNextTimeSetup();
}

void CMyApplication::ChangeDaihonSpace(void)
{
	int d = GetConfig("daihonWhiteSpaceNumber");
	int nm = d;

	if (m_input->GetNumber(d,&nm,"台本先頭のスペース数") == FALSE)
	{
		return;
	}
	d = nm;

	SetConfig("daihonWhiteSpaceNumber",d);
	CMessageData::SetWhiteSpace(d);
}

void CMyApplication::SetCGVar(int n)
{
	int d = n;
	if (n == 1)
	{
		d = GetConfig("varControlSetCG");
		int nm = d;
		if (m_input->GetNumber(d,&nm,"変数制御によるCG最大数") == FALSE)
		{
			return;
		}
		d = nm;
	}
	if (d<0) d = -1;

	SetConfig("varControlSetCG",d);
	m_menuCheckControl->SetVarControlSetCGCheck();
}



void CMyApplication::ChangeScreenSize(int md,int screenSizeX,int screenSizeY)
{
	SetConfig("screenSizeMode",md);

	if ((screenSizeX <= 0) || (screenSizeY <= 0))
	{
		screenSizeX = m_screenSizeTable[md*2];
		screenSizeY = m_screenSizeTable[md*2+1];
	}


	m_windowInitZahyo[1*11+4] = screenSizeX;
	m_windowInitZahyo[1*11+5] = screenSizeY;
	m_windowInitZahyo[1*11+9] = screenSizeX;
	m_windowInitZahyo[1*11+10] = screenSizeY;


	PrintNextTimeSetup();
}

void CMyApplication::ChangeBufferSetCG(void)
{
	int md = GetConfig("bufferSetCG");
	if (md == 0)
	{
		md = 1;
	}
	else
	{
		md = 0;
	}
	SetConfig("bufferSetCG",md);
	m_menuCheckControl->SetBufferSetCGCheck();

	m_scriptCompiler->SetBufferSetCG(md);
}

void CMyApplication::ChangeInitAngouMode(void)
{
	int md = GetConfig("initAngou");
	if (md == 0)
	{
		md = 1;
	}
	else
	{
		md = 0;
	}
	SetConfig("initAngou",md);
	m_menuCheckControl->SetInitAngouCheck();
}


void CMyApplication::ChangeAddDefaultVoice(void)
{
	int md = GetConfig("addDefaultVoice");
	if (md == 0)
	{
		md = 1;
	}
	else
	{
		md = 0;
	}
	SetConfig("addDefaultVoice",md);
	m_menuCheckControl->SetAddDefaultVoiceCheck();
}

void CMyApplication::SetDefaultName(void)
{
	LPSTR oldText = GetConfigString("defaultNameMei");
	LPSTR newText = m_input->GetText(oldText);
	if (newText != NULL)
	{
		if ((*newText) != 0)
		{
			CMessageData::SetDefaultName(newText);
			SetConfig("defaultNameMei",newText);
		}
	}
}



void CMyApplication::ExtUtil(WPARAM wParam)
{
	switch (wParam)
	{
	case ID_EXTUTIL_INITANGOU:
		if (m_scriptCompiler->InitAngou())
		{
			MessageBox(m_frameHWND,"init2に暗号データを作成しました","OK",MB_OK);
		}
		else
		{
			MessageBox(m_frameHWND,"暗号データの作成に失敗しました","ERROR",MB_OK | MB_ICONEXCLAMATION);
		}

		break;
	case ID_EXTUTIL_PACNYAN:
		ShellExecute(m_frameHWND,"open","pacNyan.exe",NULL,NULL,SW_SHOW);
		break;
	case ID_EXTUTIL_NERUNE:
		ShellExecute(m_frameHWND,"open","nerune.exe",NULL,NULL,SW_SHOW);
		break;
	case ID_EXTUTIL_INITCHECK:
		ShellExecute(m_frameHWND,"open","nnnInitCheck.exe",NULL,NULL,SW_SHOW);
		break;
	}

}

int CMyApplication::SearchFaceChara(LPSTR name)
{

	int n = m_faceList->SearchName2(name,TRUE,1);
	if (n == -1) return -1;

	return n / 2;
}

CNameList* CMyApplication::GetTermList(void)
{
	return m_termList;
}


void CMyApplication::OnBalloonArea(int n,POINT pt,int subType,POINT screenPos)
{
	if (n >= 0)
	{
		if (m_balloonFlag)
		{
			m_balloon->OnArea(n,pt,subType,screenPos);
		}
	}
	else
	{
		m_balloon->OnArea(n,pt,subType,screenPos);
	}
}


void CMyApplication::MainMouseMove(MSG msg)
{

	if (m_document[MAINSCREEN_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[FILM_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[LAYER_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[ZAHYO_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[GAMEMESSAGE_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[EFFECT_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[STORY_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[CONTROL_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[FILMCASE_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[STORYBOOK_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[VAR_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[PROGRAM_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[CONTE_WINDOW]->GetViewHWND() == msg.hwnd) return;
	if (m_document[CONTEMESSAGE_WINDOW]->GetViewHWND() == msg.hwnd) return;

	POINT pt;
	pt.x = LOWORD(msg.lParam);
	pt.y = HIWORD(msg.lParam);
	POINT dmy;
	dmy.x = 0;
	dmy.y = 0;
	OnBalloonArea(-1,pt,0,dmy);

//	static int a = 0;
//	if (a == 0)
//	{
//		a = 1;
//	}
//	char mes[256];
//	sprintf_s(mes,256,"[ %d , %d]",(int)msg.hwnd , (int)m_frameHWND);
//	OutputDebugString(mes);

}

void CMyApplication::ClearUndo(void)
{
	CUndoMemoryObject* undo = GetUndoObject();
	if (undo != NULL)
	{
		undo->Clear();
	}
}

void CMyApplication::ChangeUndoMode(int n)
{
	SetConfig("undoEnable",n);
	m_undoEnable = n;
	m_menuCheckControl->SetUndoModeCheck();

	ClearUndo();

}

void CMyApplication::ChangeUndoKakuninMode(int n)
{
	SetConfig("undoKakuninEnable",n);
	m_undoKakuninEnable = n;
	m_menuCheckControl->SetUndoKakuninCheck();
}

void CMyApplication::ChangeUndoAfterMode(int n)
{
	SetConfig("undoAfterEnable",n);
	m_undoAfterEnable = n;
	m_menuCheckControl->SetUndoAfterCheck();
}

void CMyApplication::SetControlKeyEnable(int n)
{
	SetConfig("controlKeyEnable", n);
	m_controlKeyEnable = n;
	m_menuCheckControl->SetControlKeyCheck();
}


BOOL CMyApplication::CheckUndoMode(void)
{
	if (GetUndoMode())
	{
		if (GetUndoKakuninEnable() == FALSE) return TRUE;
		//check

		int rt = MessageBox(NULL,"UNDOしますか","確認",MB_YESNO | MB_ICONEXCLAMATION);
		if (rt == IDYES)
		{
			return TRUE;
		}
	}

	return FALSE;
}

CUndoMemoryObject* CMyApplication::GetUndoObject(void)
{
	if (m_undoObject == NULL)
	{
		m_undoObject = new CUndoMemoryObject();
	}

	return m_undoObject;
}

void CMyApplication::CheckAndGetKomaUndo(void)
{
	if (GetUndoMode())
	{
		CUndoMemoryObject* undo = GetUndoObject();
		if (undo != NULL)
		{
			if (undo->GetUndoType() != UNDO_TYPE_KOMA)
			{
				CFilmData* pFilm = GetNowSelectFilm();

				int komaStart = pFilm->GetSelectStart();
				int komaEnd = pFilm->GetSelectEnd();
				int kosuu = pFilm->GetObjectKosuu();
				if ((komaStart>=0) && ( komaStart<kosuu) && (komaEnd >= 0) && (komaEnd < kosuu) && (komaStart <= komaEnd))
				{
					undo->Clear(UNDO_TYPE_KOMA,UNDO_DATA_MODIFY,komaStart,komaEnd);

					for (int i=komaStart;i<=komaEnd;i++)
					{
						CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(i));
						if (pKoma != NULL)
						{
							pKoma->Save(NULL,undo);
						}
					}
					KomaIsChanged();
					//OutputDebugString("*** UNDO GET KOAM ***\x00d\x00a");
				}
			
			}
		}
	}

}

int  CMyApplication::GetVarType(void)
{
	return 1;
}
int  CMyApplication::GetVarKosuu(void)
{
	int type = GetVarType();
	if (type == 0)
	{
		return 1000;
	}
	return 2200;
}


LPSTR CMyApplication::GetVarMarkFilename(void)
{
	int type = GetVarType();
	if (type == 0)
	{
		return VARMARK_FILE_NAME;
	}
	return VARMARK_FILE_NAME2;
}

LPSTR CMyApplication::GetVarMarkSetupFilename(void)
{
	int type = GetVarType();
	if (type == 0)
	{
		return VARMARK_SETUP_FILE_NAME;

	}
	return VARMARK_SETUP_FILE_NAME2;
}

LPSTR CMyApplication::GetVarFxfFilename(void)
{
	int type = GetVarType();
	if (type == 0)
	{
		return VAR_FXF_FILE_NAME;
	}
	return VAR_FXF_FILE_NAME2;
}

/*_*/



