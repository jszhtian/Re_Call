//
//
//

#if !defined __NNNEDIT_MENUCHECKCONTROL__
#define __NNNEDIT_MENUCHECKCONTROL__

class CMyApplication;
class CMyFrameWindow;
class CMessageSetup;

class CMenuCheckControl
{
public:
	CMenuCheckControl(CMyApplicationBase* app);
	~CMenuCheckControl();
	void End(void);

	void CheckAllMenu(void);

	void SetBuildToMenuCheck(void);
	void SetDeleteToMenuCheck(void);
	void SetVarBlockMenuCheck(void);
	void SetMesNameMenuCheck(void);
	void SetEffectMenuCheck(void);
	void SetMovieSizeMenuCheck(void);
	void SetMoviePackMenuCheck(void);
	void SetMiniPicMenuCheck(void);
	void SetFilmStorySelectMenuCheck(void);
	void SetCalcuMenuCheck(void);
	void SetDeleteKeyMenuCheck(void);
	void SetPercentOverrapMenuCheck(void);
	void SetCutinMenuCheck(void);
	void SetVoiceReadType(void);
	void SetRubiPrintType(void);
	void SetRubiColorFixType(void);
	void SetRubiBuildType(void);
	void SetKanjiCodeType(void);
	void SetCutinBuildType(void);
	void SetJpegMMX(void);
	void SetBackupNNNMode(void);
	void SetPrintFrameCheck(void);
	void SetPrintSkipCheck(void);
	void SetLayerCopyEffectModeCheck(void);
	void SetSceneCheckModeCheck(void);
	void SetPngGammaCheck(void);
	void SetBufferSetCGCheck(void);
	void SetInitAngouCheck(void);
	void SetTargetMachineCheck(void);
	void SetVarControlSetCGCheck(void);
	void SetPresetZahyoCheck(void);
	void SetAddDefaultVoiceCheck(void);
	void SetTermFlagCheck(void);
	void SetBalloonCheck(void);
	void SetZahyoTypeCheck(void);
	void SetUndoModeCheck(void);
	void SetUndoKakuninCheck(void);
	void SetUndoAfterCheck(void);
	void SetControlKeyCheck(void);

	static int m_buildMenuTable[];
	static int m_deleteMenuTable[];
	static int m_varBlockMenuTable[];
	static int m_mesNameMenuTable[];
	static int m_effectMenuTable[];
	static int m_movieSizeMenuTable[];
	static int m_moviePackMenuTable[];
	static int m_miniPicMenuTable[];
	static int m_filmStorySelectMenuTable[];
	static int m_calcuMenuTable[];
	static int m_deleteKeyMenuTable[];
	static int m_percentOverrapMenuTable[];
	static int m_cutinMenuTable[];
	static int m_voiceReadTypeTable[];
	static int m_rubiPrintTypeTable[];
	static int m_rubiColorFixTypeTable[];
	static int m_rubiBuildTypeTable[];
	static int m_kanjiCodeTable[];
	static int m_cutinBuildTypeTable[];
	static int m_jpegMMXTypeTable[];
	static int m_backupNNNTypeTable[];
	static int m_framePrintTypeTable[];
	static int m_skipPrintTypeTable[];
	static int m_layerCopyEffectModeTypeTable[];
	static int m_sceneCheckModeTypeTable[];
	static int m_pngGammaTypeTable[];
	static int m_bufferSetCGTypeTable[];
	static int m_initAngouTypeTable[];
	static int m_targetMachineTable[];
	static int m_varControlSetCGTable[];
	static int m_presetZahyoTable[];
	static int m_addDefaultVoiceTable[];
	static int m_termFlagTable[];
	static int m_balloonFlagTable[];
	static int m_zahyoPrintTypeTable[];
	static int m_undoEnableTable[];
	static int m_undoKakuninTable[];
	static int m_undoAfterTable[];
	static int m_controlKeyTable[];

private:
	CMyApplicationBase* m_app;
	CMyFrameWindow* m_frame;
	CMessageSetup* m_messageSetup;

	void SetMenuCheckRoutine(int* table,int* checkData);

};


#endif
/*_*/

