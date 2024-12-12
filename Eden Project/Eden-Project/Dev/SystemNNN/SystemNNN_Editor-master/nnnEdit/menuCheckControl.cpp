#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "resource.h"

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "messageSetup.h"

#include "myFrameWindow.h"

//#include "filmCaseData.h"
//#include "filmData.h"
//#include "komaData.h"
//#include "messageData.h"
//#include "myInputDialog.h"

#include "menuCheckControl.h"


int CMenuCheckControl::m_buildMenuTable[]=
{
	ID_MENU_SETCG_SELECT,0,-1,
	ID_MENU_SETCG_OFF,0,0,
	ID_MENU_SETCG_ON,0,1,
	ID_MENU_AUTOSCENE_OFF,1,0,
	ID_MENU_AUTOSCENE_ON,1,1,
	ID_MENU_CHECKSAMECG_OFF,2,0,
	ID_MENU_CHECKSAMECG_ON,2,1,
	-1,
};

int CMenuCheckControl::m_deleteMenuTable[]=
{
//	ID_MENU_WARNING_DEL_SC_ON,0,1,
//	ID_MENU_WARNING_DEL_SC_OFF,0,0,
	ID_MENU_WARNING_DEL_SC_ONOFF,0,1,
	ID_MENU_WARNING_DEL_CM_ONOFF,1,1,
	ID_MENU_WARNING_DEL_FL_ONOFF,2,1,
	ID_MENU_WARNING_DEL_KM_ONOFF,3,1,
	ID_MENU_WARNING_DEL_MS_ONOFF,4,1,
	ID_MENU_WARNING_DEL_PR_ONOFF,5,1,

//	ID_MENU_WARNING_DEL_CM_ON,1,1,
//	ID_MENU_WARNING_DEL_CM_OFF,1,0,
//	ID_MENU_WARNING_DEL_FL_ON,2,1,
//	ID_MENU_WARNING_DEL_FL_OFF,2,0,
//	ID_MENU_WARNING_DEL_KM_ON,3,1,
//	ID_MENU_WARNING_DEL_KM_OFF,3,0,
//	ID_MENU_WARNING_DEL_MS_ON,4,1,
//	ID_MENU_WARNING_DEL_MS_OFF,4,0,
//	ID_MENU_WARNING_DEL_PR_ON,5,1,
//	ID_MENU_WARNING_DEL_PR_OFF,5,0,
	ID_MENU_WARNING_DEL_VAR,6,1,
	-1,
};

int CMenuCheckControl::m_varBlockMenuTable[]=
{
	ID_MENU_SET_VARBLOCK1,0,0,
	ID_MENU_SET_VARBLOCK2,0,1,
	ID_MENU_SET_VARBLOCK3,0,2,
	ID_MENU_SET_VARBLOCK4,0,3,
	-1,
};

int CMenuCheckControl::m_miniPicMenuTable[]=
{
	ID_MENU_SET_KOMASM_OFF,0,1,
	ID_MENU_SET_KOMASM_ON,0,0,
	ID_MENU_SET_NNNSM_OFF,1,1,
	ID_MENU_SET_NNNSM_ON,1,0,
	ID_MENU_SET_FRAMEOFKOMA_DIRECT,2,0,
	ID_MENU_SET_FRAMEOFKOMA_PERCENT,2,1,
	ID_MENU_SET_KOMAOFFILM_DIRECT,3,0,
	ID_MENU_SET_KOMAOFFILM_PERCENT,3,1,
	-1,
};




int CMenuCheckControl::m_mesNameMenuTable[]=
{
	ID_MENU_MESWINBYINIT,0,1,
	ID_MENU_MESWINBYSETUP,0,0,
	ID_MENU_MESWIN_NOTHING,1,0,
	ID_MENU_MESWIN_BOX,1,1,
	ID_MENU_MESWIN_PIC,1,2,
	ID_MENU_NAMEWIN_NOTHING,2,0,
	ID_MENU_NAMEWIN_BOX,2,1,
	ID_MENU_NAMEWIN_PIC,2,2,
	ID_MENU_NAMEWIN_AUTOOFF,3,1,
	ID_MENU_NAMEWIN_ALWAYSPRINT,3,0,

	-1,
};

int CMenuCheckControl::m_effectMenuTable[]=
{
	ID_MENUITEM_EFFECTCOPY_KOTEI,0,0,
	ID_MENUITEM_EFFECTCOPY_MOVE,0,1,
	-1,
};


int CMenuCheckControl::m_movieSizeMenuTable[]=
{
	ID_MENU_MOVIE_DEFAULTSIZE,0,0,
	ID_MENU_MOVIE_640x480,0,1,
	ID_MENU_MOVIE_320x240,0,2,
	ID_MENU_MOVIE_160x120,0,3,
	-1,
};

int CMenuCheckControl::m_moviePackMenuTable[]=
{
	ID_MENU_MOVIE_SPEEDPACK,0,0,
	ID_MENU_MOVIE_FULLPACK,0,1,
	-1,
};

int CMenuCheckControl::m_filmStorySelectMenuTable[]=
{
	ID_MENU_SELFILM_NO,0,0,
	ID_MENU_SELFILM_SEL,0,1,
	ID_MENU_SELFILM_INPUT,0,2,
	ID_MENU_SELSTORY_NO,1,0,
	ID_MENU_SELSTORY_SEL,1,1,
	ID_MENU_SELSTORY_INPUT,1,2,
	-1,
};

int CMenuCheckControl::m_calcuMenuTable[]=
{
	ID_MENU_SELCALCU_NO,0,0,
	ID_MENU_SELCALCU_INPUT,0,1,
	-1,
};

int CMenuCheckControl::m_deleteKeyMenuTable[]=
{
	ID_MENU_DELKEY_FILM,0,1,
	ID_MENU_DELKEY_KOMA,1,1,
	ID_MENU_DELKEY_MESSAGE,2,1,
	ID_MENU_DELKEY_STORY,3,1,
	ID_MENU_DELKEY_COMMAND,4,1,
	ID_MENU_DELKEY_VAR,5,1,
	-1,
};

int CMenuCheckControl::m_percentOverrapMenuTable[]=
{
	ID_MENU_PERCENT_OVERRAP,0,1,
	-1,
};

int CMenuCheckControl::m_cutinMenuTable[]=
{
	ID_CUTIN_ENABLE,0,1,
	ID_CUTIN_DISABLE,0,0,
	-1,
};

int CMenuCheckControl::m_voiceReadTypeTable[]=
{
	ID_VOICE_NORMAL,0,0,
	ID_VOICE_PACK,0,1,
	-1,
};

int CMenuCheckControl::m_rubiPrintTypeTable[]=
{
	ID_RUBI_NOTPRINT,0,0,
	ID_RUBI_PRINT,0,1,
	ID_RUBI_HYO_PRINT,0,2,
	-1,
};

int CMenuCheckControl::m_rubiColorFixTypeTable[]=
{
	ID_RUBICOLORFIX,0,1,
	-1,
};

int CMenuCheckControl::m_rubiBuildTypeTable[]=
{
	ID_MENU_BUILDRUBI_OK,0,1,
	ID_MENU_BUILDRUBI_NOT,0,0,
	ID_MENU_BUILDRUBI_SELECT,0,-1,
	-1,
};

int CMenuCheckControl::m_kanjiCodeTable[]=
{
	ID_CODE_KANJI,0,0,
	ID_CODE_ALPHA,0,1,
	-1,
};

int CMenuCheckControl::m_cutinBuildTypeTable[]=
{
	ID_MENU_BUILDCUTIN_OK,0,1,
	ID_MENU_BUILDCUTIN_NOT,0,0,
	ID_MENU_BUILDCUTIN_SELECT,0,-1,
	-1,
};

int CMenuCheckControl::m_jpegMMXTypeTable[]=
{
	ID_JPEG_FLOAT,0,0,
	ID_JPEG_MMX,0,1,
	-1,
};

int CMenuCheckControl::m_backupNNNTypeTable[]=
{
	ID_BACKUP_NNN_OFF,0,0,
	ID_BACKUP_NNN_ON,0,1,
	-1,
};


int CMenuCheckControl::m_framePrintTypeTable[]=
{
	ID_PRINTFRAME,0,1,
	-1,
};

int CMenuCheckControl::m_skipPrintTypeTable[]=
{
	ID_PRINTSKIP,0,1,
	-1,
};

int CMenuCheckControl::m_layerCopyEffectModeTypeTable[]=
{
	ID_LAYERCOPYMODE_NO,0,0,
	ID_LAYERCOPYMODE_OK,0,1,
	ID_LAYERCOPYMODE_SEL,0,2,
	-1,
};

int CMenuCheckControl::m_sceneCheckModeTypeTable[]=
{
	ID_SCENECHECK_NO,0,0,
	ID_SCENECHECK_ERROR,0,1,
	ID_SCENECHECK_ERROROK,0,2,
	-1,
};

int CMenuCheckControl::m_pngGammaTypeTable[]=
{
	ID_PNG_GAMMA,0,1,
	-1,
};

int CMenuCheckControl::m_bufferSetCGTypeTable[]=
{
	ID_MENU_BUFFERSETCG,0,1,
	-1,
};

int CMenuCheckControl::m_initAngouTypeTable[]=
{
	ID_INITANGOU,0,1,
	-1,
};

int CMenuCheckControl::m_targetMachineTable[]=
{
	ID_TARGETMACHINE_WINDOWS,0,0,
	ID_TARGETMACHINE_IPHONE,0,1,
//	ID_TARGETMACHINE_MAC,0,2,

	-1,
};

int CMenuCheckControl::m_varControlSetCGTable[]=
{
	ID_SETCGVAR_AUTO,0,-1,
	ID_SETCGVAR_NO,0,0,
	ID_SETCGVAR_SET,0,1,
	-1,
};

int CMenuCheckControl::m_presetZahyoTable[]=
{
	ID_PRESETBUTTON_DISABLE,0,0,
	ID_PRESETBUTTON_ENABLE,0,1,
	-1,
};

int CMenuCheckControl::m_addDefaultVoiceTable[]=
{
	ID_ADDDEFAULTVOICE,0,1,
	-1,
};

int CMenuCheckControl::m_termFlagTable[]=
{
	ID_MENU_TERM_ON,0,1,
	ID_MENU_TERM_OFF,0,0,
	-1,
};

int CMenuCheckControl::m_balloonFlagTable[]=
{
	ID_BALLOON_ON,0,1,
	ID_BALLOON_OFF,0,0,
	-1,
};

int CMenuCheckControl::m_zahyoPrintTypeTable[]=
{
	ID_ZAHYO_NORMAL,0,0,
	ID_ZAHYO_CENTER,0,1,
	-1,
};


int CMenuCheckControl::m_undoEnableTable[]=
{
	ID_UNDO_ENABLE,0,1,
	ID_UNDO_DISABLE,0,0,
	-1
};

int CMenuCheckControl::m_undoKakuninTable[] = 
{
	ID_UNDO_KAKUNIN_ARI,0,1,
	ID_UNDO_KAKUNIN_NASHI,0,0,
	-1
};

int CMenuCheckControl::m_undoAfterTable[] = 
{
	ID_UNDO_AFTER_ARI,0,1,
	ID_UNDO_AFTER_NASHI,0,0,
	-1
};

int CMenuCheckControl::m_controlKeyTable[] =
{
	ID_MENU_CTRL_ENABLE,0,1,
	ID_MENU_CTRL_DISABLE,0,0,
	-1
};




CMenuCheckControl::CMenuCheckControl(CMyApplicationBase* app)
{
	m_app = app;
	m_frame = m_app->GetMyFrame();
	m_messageSetup = m_app->GetMessageSetup();
}


CMenuCheckControl::~CMenuCheckControl()
{
	End();
}

void CMenuCheckControl::End(void)
{
}



void CMenuCheckControl::CheckAllMenu(void)
{
	SetBuildToMenuCheck();
	SetDeleteToMenuCheck();
	SetVarBlockMenuCheck();
	SetMesNameMenuCheck();
	SetEffectMenuCheck();
	SetMovieSizeMenuCheck();
	SetMoviePackMenuCheck();
	SetMiniPicMenuCheck();
	SetFilmStorySelectMenuCheck();
	SetCalcuMenuCheck();
	SetDeleteKeyMenuCheck();
	SetPercentOverrapMenuCheck();
	SetCutinMenuCheck();
	SetVoiceReadType();
	SetRubiPrintType();
	SetRubiColorFixType();
	SetRubiBuildType();
	SetKanjiCodeType();
	SetCutinBuildType();
	SetJpegMMX();
	SetBackupNNNMode();
	SetPrintFrameCheck();
	SetPrintSkipCheck();
	SetLayerCopyEffectModeCheck();
	SetSceneCheckModeCheck();
	SetPngGammaCheck();
	SetBufferSetCGCheck();
	SetInitAngouCheck();
	SetTargetMachineCheck();
	SetVarControlSetCGCheck();
	SetPresetZahyoCheck();
	SetAddDefaultVoiceCheck();
	SetTermFlagCheck();
	SetBalloonCheck();
	SetZahyoTypeCheck();
	SetUndoModeCheck();
	SetUndoKakuninCheck();
	SetUndoAfterCheck();
	SetControlKeyCheck();
}


void CMenuCheckControl::SetBuildToMenuCheck(void)
{
	int checkData[3];
	checkData[0] = m_app->GetAutoSetCGFlag();
	checkData[1] = m_app->GetSceneMakeFlag();
	checkData[2] = m_app->GetCheckSameCGNameFlag();

	SetMenuCheckRoutine(m_buildMenuTable,checkData);
}

void CMenuCheckControl::SetEffectMenuCheck(void)
{
	int checkData[1];
//	checkData[0] = m_effectCopyMoveFlag;
	checkData[0] = m_app->GetEffectCopyMoveFlag();

//

//	SetMenuCheckRoutine(m_buildMenuTable,checkData);
	SetMenuCheckRoutine(m_effectMenuTable,checkData);
}

void CMenuCheckControl::SetDeleteToMenuCheck(void)
{
	int checkData[7];

	checkData[0] = m_app->GetWarningDelScene();
	checkData[1] = m_app->GetWarningDelCommand();
	checkData[2] = m_app->GetWarningDelFilm();
	checkData[3] = m_app->GetWarningDelKoma();
	checkData[4] = m_app->GetWarningDelMessage();
	checkData[5] = m_app->GetWarningDelParam();
	checkData[6] = m_app->GetWarningDelVar();

	SetMenuCheckRoutine(m_deleteMenuTable,checkData);
}


void CMenuCheckControl::SetMesNameMenuCheck(void)
{
	int checkData[4];

	checkData[0] = m_messageSetup->GetMesNameByInit();
	checkData[1] = m_messageSetup->GetMesWinPrintMode();
	checkData[2] = m_messageSetup->GetNameWinPrintMode();
	checkData[3] = m_messageSetup->GetNameAutoOffFlag();

	SetMenuCheckRoutine(m_mesNameMenuTable,checkData);
}

void CMenuCheckControl::SetMovieSizeMenuCheck(void)
{
	int checkData[1];

	checkData[0] = m_app->GetConfig("movieSize");
	SetMenuCheckRoutine(m_movieSizeMenuTable,checkData);
}

void CMenuCheckControl::SetMoviePackMenuCheck(void)
{
	int checkData[1];

	checkData[0] = m_app->GetConfig("moviePackType");
	SetMenuCheckRoutine(m_moviePackMenuTable,checkData);
}

void CMenuCheckControl::SetMiniPicMenuCheck(void)
{
	int checkData[4];
	checkData[0] = m_app->GetConfig("noSavePicFlag");
	checkData[1] = m_app->GetConfig("noSaveNNNPicFlag");
	checkData[2] = m_app->GetConfig("frameOfKomaType");
	checkData[3] = m_app->GetConfig("komaOfFilmType");


	SetMenuCheckRoutine(m_miniPicMenuTable,checkData);
}

void CMenuCheckControl::SetFilmStorySelectMenuCheck(void)
{
	int checkData[2];
	checkData[0] = m_app->GetConfig("selFilmCommand");
	checkData[1] = m_app->GetConfig("selStoryCommand");

	SetMenuCheckRoutine(m_filmStorySelectMenuTable,checkData);
}

void CMenuCheckControl::SetCalcuMenuCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("selCalcuCommand");
	SetMenuCheckRoutine(m_calcuMenuTable,checkData);
}

void CMenuCheckControl::SetDeleteKeyMenuCheck(void)
{
	int checkData[6];

	checkData[0] = m_app->GetConfig("deleteKeyFilm");
	checkData[1] = m_app->GetConfig("deleteKeyKoma");
	checkData[2] = m_app->GetConfig("deleteKeyMessage");
	checkData[3] = m_app->GetConfig("deleteKeyStory");
	checkData[4] = m_app->GetConfig("deleteKeyCommand");
	checkData[5] = m_app->GetConfig("deleteKeyVar");

	SetMenuCheckRoutine(m_deleteKeyMenuTable,checkData);
}

void CMenuCheckControl::SetCutinMenuCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("cutin");
	SetMenuCheckRoutine(m_cutinMenuTable,checkData);
}

void CMenuCheckControl::SetVoiceReadType(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("voicePack");
	SetMenuCheckRoutine(m_voiceReadTypeTable,checkData);
}

void CMenuCheckControl::SetRubiPrintType(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("rubiPrintFlag");
	SetMenuCheckRoutine(m_rubiPrintTypeTable,checkData);
}

void CMenuCheckControl::SetRubiColorFixType(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("rubiColorFix");
	SetMenuCheckRoutine(m_rubiColorFixTypeTable,checkData);
}

void CMenuCheckControl::SetRubiBuildType(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("rubiBuildFlag");
	SetMenuCheckRoutine(m_rubiBuildTypeTable,checkData);
}

void CMenuCheckControl::SetKanjiCodeType(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("kanjiCode");
	SetMenuCheckRoutine(m_kanjiCodeTable,checkData);
}

void CMenuCheckControl::SetCutinBuildType(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("cutinBuildFlag");
	SetMenuCheckRoutine(m_cutinBuildTypeTable,checkData);
}

void CMenuCheckControl::SetMenuCheckRoutine(int* table,int* checkData)
{
	for (int i=0;i<256;i++)
	{
		int menuID = table[i*3];
		if (menuID == -1) return;

		BOOL flag = FALSE;
		int n = table[i*3+1];

		if (checkData[n] == table[i*3+2]) flag = TRUE;
		m_frame->SetCheckMenu(menuID,flag);
	}
}

void CMenuCheckControl::SetVarBlockMenuCheck(void)
{
	int checkData[1];

	checkData[0] = m_app->GetConfig("varBlockType");
	SetMenuCheckRoutine(m_varBlockMenuTable,checkData);
}

void CMenuCheckControl::SetPercentOverrapMenuCheck(void)
{
	int checkData[1];

	checkData[0] = m_app->GetConfig("autoSetOverrapFlag");
	SetMenuCheckRoutine(m_percentOverrapMenuTable,checkData);
}

void CMenuCheckControl::SetJpegMMX(void)
{
	int checkData[1];

	checkData[0] = m_app->GetConfig("jpegMMX");
	SetMenuCheckRoutine(m_jpegMMXTypeTable,checkData);
}

void CMenuCheckControl::SetBackupNNNMode(void)
{
	int checkData[1];

	checkData[0] = m_app->GetConfig("backupNNN");
	SetMenuCheckRoutine(m_backupNNNTypeTable,checkData);
}

void CMenuCheckControl::SetPrintFrameCheck(void)
{
	int checkData[1];

	checkData[0] = m_app->GetConfig("PrintFrameFlag");
	SetMenuCheckRoutine(m_framePrintTypeTable,checkData);
}

void CMenuCheckControl::SetPrintSkipCheck(void)
{
	int checkData[1];

	checkData[0] = m_app->GetConfig("PrintSkipFlag");
	SetMenuCheckRoutine(m_skipPrintTypeTable,checkData);
}

void CMenuCheckControl::SetLayerCopyEffectModeCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("layerCopyEffectMode");
	SetMenuCheckRoutine(m_layerCopyEffectModeTypeTable,checkData);
}

void CMenuCheckControl::SetSceneCheckModeCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("sceneCheckMode");
	SetMenuCheckRoutine(m_sceneCheckModeTypeTable,checkData);
}

void CMenuCheckControl::SetPngGammaCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("pngGammaFlag");
	SetMenuCheckRoutine(m_pngGammaTypeTable,checkData);
}

void CMenuCheckControl::SetBufferSetCGCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("bufferSetCG");
	SetMenuCheckRoutine(m_bufferSetCGTypeTable,checkData);
}

void CMenuCheckControl::SetInitAngouCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("initAngou");
	SetMenuCheckRoutine(m_initAngouTypeTable,checkData);
}

void CMenuCheckControl::SetTargetMachineCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("targetMachine");
	SetMenuCheckRoutine(m_targetMachineTable,checkData);
}


void CMenuCheckControl::SetVarControlSetCGCheck(void)
{
	int checkData[1];
	int d = m_app->GetConfig("varControlSetCG");
	if (d>0) d = 1;
	checkData[0] = d;
	SetMenuCheckRoutine(m_varControlSetCGTable,checkData);

}

void CMenuCheckControl::SetPresetZahyoCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("presetButtonEnable");
	SetMenuCheckRoutine(m_presetZahyoTable,checkData);
}

void CMenuCheckControl::SetAddDefaultVoiceCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("addDefaultVoice");
	SetMenuCheckRoutine(m_addDefaultVoiceTable,checkData);
}

void CMenuCheckControl::SetTermFlagCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("termFlag");
	SetMenuCheckRoutine(m_termFlagTable,checkData);
}

void CMenuCheckControl::SetBalloonCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("balloonFlag");
	SetMenuCheckRoutine(m_balloonFlagTable,checkData);
}

void CMenuCheckControl::SetZahyoTypeCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("zahyoPrintType");
	SetMenuCheckRoutine(m_zahyoPrintTypeTable,checkData);
}

void CMenuCheckControl::SetUndoModeCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("undoEnable");
	SetMenuCheckRoutine(m_undoEnableTable,checkData);
}

void CMenuCheckControl::SetUndoKakuninCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("undoKakuninEnable");
	SetMenuCheckRoutine(m_undoKakuninTable,checkData);
}

void CMenuCheckControl::SetUndoAfterCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("undoAfterEnable");
	SetMenuCheckRoutine(m_undoAfterTable,checkData);
}

void CMenuCheckControl::SetControlKeyCheck(void)
{
	int checkData[1];
	checkData[0] = m_app->GetConfig("controlKeyEnable");
	SetMenuCheckRoutine(m_controlKeyTable, checkData);
}

/*_*/


