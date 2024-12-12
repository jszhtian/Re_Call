//
// myapplication.h
//

#if !defined __NNNEDIT_MYAPPLICATION__
#define __NNNEDIT_MYAPPLICATION__


class CConfigData;


class CSystemDIB;

class CFilmData;

class CMyFrameWindow;
class CMyClientWindow;
class CMyDocument;

class CPicture;
class CEffect;

class CSelectDialog;
class CUndoMemoryObject;

class CCommonMessageWindow;

class CStoryData;

//class CConfigFile;

class CMyFileOpen;
class CMyMessage;
class CAutoSaveSubData;
class COkikaeData;
class CMyFont;
class CRubiFont;
class CScriptData;
class CNameList;
class CKomaData;

class CStartTitle;
class CBalloon;
class CMyInputDialog;
class CMySearchDialog;

class CSelectObjectOnly;

class CCommonDIB;
class CStoryBookData;
class CFilmCaseData;

#define FILM_KOSUU_MAX 1024
#define STORY_KOSUU_MAX 1024

#define MDIWINDOW_KOSUU_MAX 32

class CMiniCompiler;

class CCommandData;

class CWave;

class CConfig;
//class CCutinControl;

class CButtonPic;
class CEffectPic;

class CMyConfig;

class CMyBitmap;
class CMyGraphics;

class CEffectParam;
class CAllEffectParam;

class CMessageSetup;

class CMyDirectSound;
class CMyDirectSoundBuffer;
class CWaveData;

class CRubiMaker;

//class CAviSave;

class CMusicControl;
class CDefaultControl;
class CVolumeControl;
class CMakeUseVawBat;
class CMakeCheckVawBat;
class CMakeUseDWQBat;
class CMakeCheckDWQBat;

class CScriptSoundControl2;
class CScriptVoiceControl2;

class CMenuCheckControl;
class CMovieCreate;
class CMessageSerialControl;

class CScriptCompiler;
class CAutoVoice;
class CLockVoice;
class CDeleteVoice;
class CMakeBook;
class CSameMessageDeutch;
class CSearchOrReplace;
class CWindowUpdateControl;
class CCopyPreEffect;
class CCheckDWQ;
class CDeleteNonVoice;
class CMyOnTimer;
class CFrameChange;

class CCutinNameData;

class CChangeBGMList;
class CChangeSeList;

class CVarInitData;
class CShakin;

class CMyApplicationBase;

class CMyApplication : public CMyApplicationBase
{
public:
	CMyApplication(HINSTANCE hinstance);
	~CMyApplication();
	void End(void);

	HINSTANCE GetMyInstance(void){return m_hInstance;}

	CMyFrameWindow* GetMyFrame(void){return m_frame;}
	CMyClientWindow* GetMyClient(void){return m_client;}

	HWND GetFrameHWND(void){return m_frameHWND;}
	HWND GetClientHWND(void){return m_clientHWND;}

	void dmy0(void);
	void dmy(int cnt = 0);

	void OnCommand(WPARAM wParam,LPARAM lParam);
	void OnMove(WPARAM wParam,LPARAM lParam);

	void dmy2(int x, int y);

	CEffect* GetEffect(void){return m_effect;}

	void ShowMyWindow(int n);

	void SetEyeStatus(int layer, int st);


	void MoveDstFrom(int dx, int dy);
	void MoveDstTo(int dx, int dy);

	void AdjustDstFrom(int px, int py);
	void AdjustDstTo(int px, int py);

	void UpdateMyWindow(int windowNumber);

	HBITMAP GetCommonBitmap(int n);

	void ChangeShowHideWindow(int n);
	void ShowHideWindow(int n, BOOL flg);
	void HideWindow(int n);

	int SearchAkiFilm(void);
//	int SearchFilm(LPSTR name);

	int OnTimer(int n);

	void OnGameClick(void);

	BOOL NewScript(void);
	BOOL Load(void);
	BOOL Save(void);

	LPSTR GetNewFilmName(void);
	LPSTR GetNewStoryName(void);

	HGDIOBJ SelObjOnly(HDC hdc, HGDIOBJ obj);


//	CFilmCaseData* GetFilmCase(void);
//	CStoryBookData* GetStoryData(void);

	CStoryData* GetNowSelectStory(void);
//	int CompileStory2(CStoryData* pStory,int n);


	int GetEffectCopyMoveFlag(void){return m_effectCopyMoveFlag;}
	CFilmData* GetNowSelectFilm(void);
	CKomaData* GetNowSelectKoma(void);

	void FilmIsChanged(void);
	void KomaIsChanged(void);
	void MessageIsChanged(void);
	void LayerIsChanged(void);
	void EffectIsChanged(void);
	void ZahyoIsChanged(void);

	void StoryIsChanged(void);
	void CommandIsChanged(void);
	void ProgramIsChanged(void);
	void VarIsChanged(void);

	void UpdateMainScreen(int cnt = 0);

	void ConteIsChanged(void);
	void ConteMessageIsChanged(void);

//	int GetNowSelectLayer(void);

	int GetNowSelectPictureLayer(void);
	int GetNowSelectEffectLayer(void);

	BOOL GetPlayMode(void);
	int GetSubCommand(void);
	void SetPlayMode(BOOL md = TRUE, int subCommand = -1, BOOL countClearFlag = TRUE);
	void SetKomaMode(BOOL md = TRUE);

	BOOL CheckPlayMode(void);
	BOOL CheckPauseMode(void);

	int GetPlayCommand(void);
	BOOL CheckKomaMode(void);

//	void AddFilmVarName(LPSTR name);
//	void DeleteFilmVarName(LPSTR name);
//	int SearchFilmVarName(LPSTR name);

	void PrintMessageWindow(bool nameFlag) override;

	void MoveRectRoutine(int layer, int md, int dx, int dy);//dummy

	void SetWindowName(int windowNumber, LPSTR name);

	int SearchFilm(LPSTR filmName, int searchStart = 0,int noCheckNumber = -1);
	int SearchStory(LPSTR storyName, int searchStart = 0,int noCheckNumber = -1);
	int SearchScript(LPSTR sptName, int searchStart = 0,int noCheckNumber = -1);

	CFilmData* GetFilmData(int n);

	CMyDirectSound* GetDirectSound(void){return m_directSound;}

	CMessageSetup* GetMessageSetup(void){return m_messageSetup;}

	void SetNextPrint(BOOL flg = TRUE);

	BOOL GetNextPrintFlag(void){return m_nextPrintFlag;}
	BOOL GetPauseFlag(void){return m_pauseFlag;}
	int GetGameCount(void){return m_gameCount;}
	void GameCountUp(int cnt = 1){m_gameCount += cnt;}

	void FlipToScreen(void);
	void SetModify(BOOL flg = TRUE);
	BOOL CheckModify(void);

	void PrintOverrap(int type,int count ,int countMax);
//	int GetConfigData(int typ, int para0 = -1, LPVOID para1 = NULL);

	int GetConfig(LPSTR name);
	LPSTR GetConfigString(LPSTR name);
	void SetConfig(LPSTR name,int value);
	void SetConfig(LPSTR name,LPSTR mes);

//	LPSTR CreateUUIDString(void);
	LPSTR CreateRandomString(void);


	void WindowIsMoved(int windowNumber, int x, int y);
	void WindowSizeIsChanged(int windowNumber, int x, int y);

	void WindowIsShowHide(int windowNumber, BOOL showFlag);

	void OnNextMessage(void);

	int GetZahyoSwitch(void);

	void SetNowKomaEffectRect(int layer);

	void JumpStory(int n);

	void SetVarName(int n, LPSTR name);
	LPSTR GetVarName(int n);

	CNameList* GetNameList(void){return m_varList;}
	CNameList* GetSEList(void){return m_seList;}
	CNameList* GetFunctionList(void){return m_functionList;}
	CNameList* GetCommandList(void){return m_commandList;}
	CNameList* GetBGMList(void){return m_bgmList;}

	CNameList* GetCharaList(void){return m_charaList;}
	CNameList* GetSetCGList(void){return m_setCGList;}
	CNameList* GetVarControlList(void){return m_varControlList;}

//	CNameList* GetSceneDataList(void){return m_sceneDataList;}
	int SearchVarName(LPSTR name,int searchStart = -1);

	void SetPauseFlag(BOOL flg = TRUE){m_pauseFlag = flg;}

	void CountUp(int deltaCount = 1);
	void CheckNextMessage(void);

	void PreClickMouse(int windowNumber);

	LPSTR GetCharaName(int n);
	LPSTR GetSEName(int n);
	LPSTR GetSEFileName(int n);
	LPSTR GetBGMName(int n);
	LPSTR GetBGMFileName(int n);

	BOOL SaveVarName(void);
	BOOL SaveVarInitData(void);

	BOOL MakeBook(void);
	BOOL LoadByFileNameOnly(LPSTR name);

	void ChangeMemo(void);

//	BOOL CompileCommandData(CCommandData* pCommand);

//	BOOL CompileStory(void);
//	BOOL CompileFilm(void);

	BOOL CheckIamSystemScript(void);

	BOOL ConfigMenu(void);


	void SetNewEffect(int nxt);

//	BOOL MakeUseDWQBat(void);
//	BOOL MakeUseVAWBat(void);

//	BOOL MakeCheckDWQBat(void);
//	BOOL MakeCheckVAWBat(void);

//	BOOL SetAllMessageSerial(void);
//	BOOL SetMessageSerial(void);

	BOOL AllLockVoice(BOOL flg = TRUE);
	BOOL LockVoice(BOOL flg);

	void PlayVoice(LPSTR voiceName);
	void PlaySound(LPSTR soundName);

	void PlaySeTest(int n,int channel);
	void PlayMusicTest(int n);


	void CheckAndPlaySound(void);

//	BOOL CheckDWQ(void);
//	BOOL CheckDWQAll(void);

//	BOOL DeleteNonVoice(BOOL selFlag = TRUE);
//	BOOL DeleteAllNonVoice(void);

	BOOL PutByDIB(HDC hdc, int x, int y, int sizeX, int sizeY, int* buffer);

	HBITMAP GetFilmLargePic(int n);



//	void PutButton(HDC hdc, int x, int y, int buttonNumber, int buttonType);
//	void PutEffect(HDC hdc, int x, int y, int num);

	void GetInitWindowZahyo(LPSTR windowName, int windowNumber, RECT* lpRect, BOOL* lpWindowFlag);
	int GetWindowMode(void);

	static int m_windowInitZahyo[];


//	static BOOL CALLBACK MyProceedDialogProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
//	static HWND m_staticText;
//	static HWND m_progressBar;
//	static HWND m_progressBar2;
//	HWND m_proceedHWND;
	static char m_varInitName[100][32];

//	static char m_bookTypeName[4][16];
//	static char m_bookTypeDesc[4][16];

	void GetFrameWindowRect(RECT* lpRect);
	void SetFrameWindowRect(RECT* lpRect);


	void PrintHint(void);

	SIZE GetDesktopSize(void);

	int GetFrameTime(void) {return m_frameTime;}
	int GetDefaultFrameTime(void) {return m_defaultFrameTime;}
	void SetFrameTime(int frame);
	void SetDefaultFrameTime(void){SetFrameTime(GetDefaultFrameTime());}

	typedef struct _tagMYCOMMONBITMAPTABLE
	{
		int nm;
		char filename[60];
	} MYCOMMONBITMAPTABLE;

	static MYCOMMONBITMAPTABLE m_commonBitmapTable[50];

	static int m_screenSizeTable[];


	CEffectParam* GetEffectParam(int n);
	int GetNowEffect(void);
	int GetNowSelectParam(void);

	CMyMessage* GetMyMessage(void){return m_message;}

	CStoryBookData* GetStoryBookData(void);
	CFilmCaseData* GetFilmCaseData(void);

	void DebugLog(LPSTR mes);

	BOOL CheckWinNT(void);

	void CreateNextKoma(void);

	static int m_varBlockTable[][8];
	static int m_varBlockTable2[][8];


	int GetWarningDelScene(void){return m_warningDeleteScene;}
	void SetWarningDelScene(int flg);
	int GetWarningDelCommand(void){return m_warningDeleteCommand;}
	void SetWarningDelCommand(int flg);
	int GetWarningDelFilm(void){return m_warningDeleteFilm;}
	void SetWarningDelFilm(int flg);
	int GetWarningDelKoma(void){return m_warningDeleteKoma;}
	void SetWarningDelKoma(int flg);
	int GetWarningDelMessage(void){return m_warningDeleteMessage;}
	void SetWarningDelMessage(int flg);
	int GetWarningDelParam(void){return m_warningDeleteParam;}
	void SetWarningDelParam(int flg);
	int GetWarningDelVar(void){return m_warningDeleteVar;}
	//void SetWarningDelVar(int flg);
	void ChangeWarningDelVar(void);


	int GetAutoSetCGFlag(void){return m_autoSetCGFlag;}
	void SetAutoSetCGFlag(int flg);
	int GetSceneMakeFlag(void){return m_sceneMakeFlag;}
	void SetSceneMakeFlag(int flg);
	int GetCheckSameCGNameFlag(void){return m_checkSameCGNameFlag;}
	void SetCheckSameCGNameFlag(int flg);
	int GetBuildRubi(void){return m_rubiBuildFlag;}
	void SetBuildRubiFlag(int flg);
	int GetBuildCutin(void){return m_cutinBuildFlag;}
	void SetBuildCutinFlag(int flg);
	int GetKanjiCode(void){return m_kanjiCode;}
	void SetKanjiCode(int flg);

	CPicture* GetMesWinPic(void){return m_mesWinPic;}
	CPicture* GetNameWinPic(void){return m_nameWinPic;}

	void PlayScriptMessageVoice(CMessageData* pMessage,int channel);
	void PlayScriptMessageSound(CMessageData* pMessage,int channel);

//	void SearchOrReplace(BOOL replaceFlg,BOOL allFlag);

	int CalcuFilmTime(CFilmData* pFilm,int defaultRate = 0);

//	BOOL CreateProceedDialog(void);
//	void SetProceedMessage(LPSTR mes);
//	void SetProceedBar(int pos,int n = 0);
//	BOOL DestroyProceedDialog(void);

	int GetFilmSelectDefault(void);
	int GetStorySelectDefault(void);
	int GetCalcuSelectDefault(void);

	CMyInputDialog* GetInputDialog(void){return m_input;}
	CVolumeControl* GetVolumeControl(void){return m_volumeControl;}

	LPSTR GetFileNameOnly(void){return m_fileNameOnly;}
	void SetFileNameOnly(LPSTR filenameOnly);

	CNameList* GetProjectList(void){return m_projectList;}
	CNameList* GetSeList(void){return m_seList;}
	BOOL CheckSave(BOOL varCheckFlag = FALSE);

	HINSTANCE GetHInstance(void){return m_hInstance;}
	CPicture* GetOverrapPic(void){return m_overrapPic;}
	CPicture* GetOverrapPic2(void){return m_overrapPic2;}

	LPSTR GetMyName(void){return m_myname;}
	LPSTR GetMemo(void){return m_memo;}

	void OnTabKey(int windowNumber,int delta = 1);

	void ChangeWindowIfCan(int windowNumber);
	BOOL SaveRoutine(int n);

	void ScrollToSelect(int windowNumber);

	CMyDocument* GetMyDocument(int windowNumber){return m_document[windowNumber];}
	void DataIsChanged(int windowNumber);

	CMyInputDialog* GetMyInputDialog(void){return m_input;}

	void ChangeFilmPlayMode(void);

	BOOL CheckShowWindow(int windowNumber);

	void StopAllMusicAndSound(void);

	int GetCutinFlag(void){return m_cutinFlag;}
//	CCutinControl* GetCutinControl(void){return m_cutinControl;}
	CCutinNameData* GetCutinNameData(void){return m_cutinNameData;}

	void ReplaceVarName(int varNumber,LPSTR searchString,LPSTR replaceString);
	void SetSelectVarNumber(int varNumber);

	void SearchVar(BOOL replaceFlag = FALSE);

	CRubiMaker* GetRubiMaker(void){return m_rubiMaker;}
	int GetRubiPrintFlag(void){return m_rubiPrintFlag;}

	void ReloadCutinFile(void);

	void ReLoadSeList(void);
	void ReLoadBGMList(void);

	int GetEditorMessageGyo(void);
	int GetMessageWidthGuideLine(void);
	CVarInitData* GetVarInitData(void){return m_varInitData;}
	CShakin* GetShakinControl(void){return m_shakinControl;}

	void MoveFilmTop(void);
	int GetFilmPlayMode(void){return m_filmPlayMode;}

	int SearchFaceChara(LPSTR name);

	int GetPresetButtonEnable(void){return m_presetButtonEnableFlag;}
	CNameList* GetTermList(void);

	void OnBalloonArea(int n,POINT pt,int subType,POINT screenPos);
	void MainMouseMove(MSG msg);
	void OnControlKey(int windowNumber,WPARAM wParam,LPARAM lParam);
	int GetZahyoPrintType(void){return m_zahyoPrintType;}
	int GetUndoMode(void){return m_undoEnable;}
	int GetUndoKakuninEnable(void){return m_undoKakuninEnable;}
	int GetUndoAfterUnable(void){return m_undoAfterEnable;}
	int GetControlKeyEnable(void) { return m_controlKeyEnable; }
	void SetControlKeyEnable(int n);

	BOOL CheckUndoMode(void);
	CUndoMemoryObject* GetUndoObject(void);

	void ClearUndo(void);
	void CheckAndGetKomaUndo(void);

	int GetVarType(void);
	int GetVarKosuu(void);


	LPSTR GetVarMarkFilename(void);
	LPSTR GetVarMarkSetupFilename(void);
	LPSTR GetVarFxfFilename(void);

	CCommonMessageWindow* m_messageWindow;


private:
	HINSTANCE m_hInstance;

	CMyFrameWindow* m_frame;
	CMyClientWindow* m_client;

	COkikaeData* m_okikaeData;
	CMyFont* m_font;
	CRubiFont* m_rubiFont;
	CMyMessage* m_message;

//	CMyDocument* m_dummyDoc;
	HWND m_frameHWND;
	HWND m_clientHWND;

	CMyDocument* m_document[MDIWINDOW_KOSUU_MAX];


	HBITMAP m_commonHBitmap[50];
	CMyBitmap* m_commonBitmap[50];


//	CSystemDIB* m_systemDIB;

	CEffect* m_effect;
//	CAllEffect* m_effect;
	int* m_screenBuffer;
	char* m_picTmpBuffer;

//	CConfigFile* m_config;


	void MakeAllMiniPic(void);


	void OnBuildTxt(BOOL mesFlag = TRUE);
	void OnBuildJson(BOOL mesFlag = TRUE);
	BOOL OnBuildSpt(BOOL bMesFlag = TRUE);

	void OnAllBuild(void);

	void MakeAllTextSource(void);


	void MoveAllWindow(void);

	void SetLastFileNameOnly(LPSTR name);
//	void SetFileNameOnly(LPSTR name);
	void SetWindowTitle(LPSTR title);

	void CopyPreEffect(int pre = 1);

	void CalcuFrame(void);

//	BOOL AutoVoice(void);
//	BOOL AllAutoVoice(void);

	BOOL LoadRoutine(FILE* file);

	BOOL CompileCommand(void);

//	BOOL SameMessageToDeutch(void);


	CMiniCompiler* m_miniCompiler;

	CMyFileOpen* m_file;
	CScriptData* m_scriptData;

	CNameList* m_varList;
	CNameList* m_charaList;
	CNameList* m_projectList;
	CNameList* m_seList;
	CNameList* m_bgmList;
	CNameList* m_faceList;

	CNameList* m_functionList;
	CNameList* m_commandList;

	CNameList* m_setCGList;
	CNameList* m_varControlList;
	CNameList* m_termList;

	CNameList* m_gameXTXList;

	CStartTitle* m_startTitle;
	CBalloon* m_balloon;

	CPicture* m_overrapPic;
	CPicture* m_overrapPic2;

	CMyInputDialog* m_input;
//	CMySearchDialog* m_search;

//	CConfig* m_configMenu;

//	CWave* m_wave;

	BOOL m_modifyFlag;
	BOOL m_dataExistFlag;

	BOOL m_playMode;
	int m_playCommand;
	int m_komaMode;

	BOOL m_nextPrintFlag;
	BOOL m_pauseFlag;

	BOOL m_noUpdateScreenFlag;

	int m_windowMode;

//	int m_deletePlayerNumber;

	int m_gameCount;

	CSelectObjectOnly* m_selObj;

	CCommonDIB* m_commonDIB;


	char m_uuidText[33];
	char m_randomString[256];

	char m_tmpName[64];
	char m_myname[64];
	char m_memo[256];

//	int m_storyJumpTable[4096];
//	int m_filmJumpTable[4096];

//	int m_komaOffsetByFilm[1024];
//	int m_komaJumpTable[8192];
//	int m_komaTableKosuu;

	char m_lastFileNameOnly[1024];
	char m_fileNameOnly[256];

	void DebugStart(void);

	CButtonPic* m_buttonPic;
	CEffectPic* m_effectPic;

	CMyConfig* m_nnnConfig;

//	int m_makeBookMode;
//	int m_makeBookAllFlag;


//	int m_setcgListKosuu;


//	CNameList* m_sceneDataList;
//	int m_cgDataListKosuu;
//	CNameList* m_cgDataList[100];
//	BOOL m_sceneDataLoadFlag;
//	BOOL m_cgDataLoadFlag;
//	BOOL m_makeCgListFlag;
//	BOOL KaisekiScenePass1(void);
//	BOOL KaisekiScenePass2(int n);

//	int* m_sceneWork;
//	int* m_charaOffsetTable;

//	int m_sceneDataSize;
//	int m_sceneCharaKosuu;

//	BOOL m_kaisekiScenePass1Flag;
//	BOOL m_kaisekiScenePass2Flag;

//	BOOL LoadCGList(void);
//	int m_cgUseFlag[320][64];	//100 chara 64*32flag max
//	void SetUseCg(int pl, int nm);
//	BOOL CheckUseCG(int pl, int nm);

//	void SearchAndSetCG(LPSTR filename);

	int m_bitTable[32];

//	LPSTR SkipCGCommand(LPSTR name);

	int m_frameTime;
	int m_defaultFrameTime;

//	int CheckSameCGList(void);

	////////////////
	CConfigData* m_configData;

	CMyGraphics* m_myGraphics;

	CAllEffectParam* m_allEffectParam;

//	void PrintUtilLoadError(LPSTR filename);
//	void SetErrorSkip(BOOL flg = TRUE){m_errorSkipFlag = flg;}
//	BOOL CheckErrorSkip(void){return m_errorSkipFlag;}
//	BOOL m_errorSkipFlag;

	BOOL m_winNTFlag;
//	int m_voiceCount[256];

	void ChangeVarBlock(int n);
	void EditFrameIsChanged(void);

//	int GetPass2ErrorLine(void){return m_pass2ErrorLine;}
//	void SetPass2ErrorLine(int n){m_pass2ErrorLine = n;}

	int m_pass2ErrorLine;

	int m_autoSetCGFlag;
	int m_sceneMakeFlag;
	int m_checkSameCGNameFlag;
	int m_presetButtonEnableFlag;
//	int m_buildRubiFlag;


	int m_warningDeleteScene;
	int m_warningDeleteCommand;
	int m_warningDeleteFilm;
	int m_warningDeleteKoma;
	int m_warningDeleteMessage;
	int m_warningDeleteParam;
	int m_warningDeleteVar;

	void SetMesNameByInit(int n);
	void SetMesPrintMode(int n);
	void ChangeMesSize(void);
	void ChangeMesFileName(void);
	void ChangeMesPrintZahyo(void);
	void ChangeMesLPrintZahyo(void);
	void ChangeMesAppendZahyo(void);
	void ChangeMesFontSize(void);
	void ChangeMesNextY(void);
	void SetNamePrintMode(int n);
	void ChangeNameSize(void);
	void ChangeNameFileName(void);
	void ChangeNamePrintZahyo(void);
	void ChangeNameFontSize(void);
	void SetNameAutoFlag(int n);
	void SetCGVar(int n);
	void SetTermFlag(int n);
	void ChangeBalloonFlag(int n);
	void ChangeZahyoPrintType(int n);


	void SetEffectMenuMoveFlag(int flg);



	void SetMovieSize(int n);
	void SetMoviePackType(int n);

	void CheckAndSetProject(void);

	void SetFilmSelectMenuType(int n);
	void SetStorySelectMenuType(int n);
	void SetCalcuMenuType(int n);

	void ChangePercentOverrap(void);

	void Mijissou(void);

	BOOL m_mesWinPicLoadFlag;
	BOOL m_nameWinPicLoadFlag;

	void CheckAndLoadMesWinPic(void);
	void CheckAndLoadNameWinPic(void);

	CPicture* m_mesWinPic;
	CPicture* m_nameWinPic;

	int m_mesFontSize;
	int m_nameFontSize;

	void PrintNextTimeSetup(void);

	CMessageSetup* m_messageSetup;

//	void SetStartKoma(CKomaData* pKoma,int nextType,int nextWindowMode);

//	CMyDirectSoundBuffer* m_voice[2*2];
//	CMyDirectSoundBuffer* m_sound[8];
	CMyDirectSound* m_directSound;
//	CWaveData* m_waveData;

//	int m_musicVolume;
//	int m_soundVolume;
//	int m_voiceVolume;
	int m_effectCopyMoveFlag;

	int SearchProject(LPSTR filename);
	BOOL AddProject(LPSTR filename);
	BOOL DeleteProject(LPSTR filename);
	void OnDeleteProject(void);

	void OnAddCommandList(void);
	void OnDeleteCommandList(void);
	void OnAddFunctionList(void);
	void OnDeleteFunctionList(void);

	int m_compilingStoryNumber;
	CSelectDialog* m_selectDialog;

//	void SetDefaultOverrapTime(void);
//	void SetDefaultBGLayer(void);
//	void SetDefaultTALayer(void);

//	void SetDefaultMovieRate(void);
//	void SetDefaultMovieScale(void);

//	CAviSave* m_aviSave;

	CMusicControl* m_musicControl;

//	void MakeMovieAll(void);
//	void MakeMovieAllFilm(void);
//	void MakeMovieFilm(void);

//	BOOL OpenMovieFile(LPSTR name,int frameTime);
//	int MakeMovieFilmSub(CFilmData* pFilm,int amari = 0);

	void ChangeDeleteKey(LPSTR name);
	void CutinEnable(BOOL flg=TRUE);
	void SetVoiceReadType(BOOL packFlag = TRUE);
	void SetRubiPrint(int md);

	void ChangeAngouCode(LPSTR name);
	void ClearAngouCode(void);
	void PrintAngouCode(BOOL changeFlag = FALSE);

	CDefaultControl* m_defaultControl;
	CVolumeControl* m_volumeControl;
	CMakeUseVawBat* m_makeUseVawBat;
	CMakeCheckVawBat* m_makeCheckVawBat;
	CMakeUseDWQBat* m_makeUseDWQBat;
	CMakeCheckDWQBat* m_makeCheckDWQBat;
	
	CScriptSoundControl2* m_scriptSoundControl;
	CScriptVoiceControl2* m_scriptVoiceControl;
	CMenuCheckControl* m_menuCheckControl;
	CMovieCreate* m_movieCreate;
	CMessageSerialControl* m_messageSerialControl;

	CScriptCompiler* m_scriptCompiler;
	CAutoVoice* m_autoVoice;
	CLockVoice* m_lockVoice;
	CDeleteVoice* m_deleteVoice;
	CMakeBook* m_makeBook;
	CSameMessageDeutch* m_sameMessageDeutch;
	CSearchOrReplace* m_searchOrReplace;
	CWindowUpdateControl* m_windowUpdateControl;
	CCopyPreEffect* m_copyPreEffect;
	CCheckDWQ* m_checkDWQ;
	CDeleteNonVoice* m_deleteNonVoice;
	CMyOnTimer* m_onTimer;
	CFrameChange* m_frameChange;

	CChangeSeList* m_changeSeList;
	CChangeBGMList* m_changeBGMList;

//	CCutinControl* m_cutinControl;
	int m_cutinFlag;
	CCutinNameData* m_cutinNameData;

	int m_filmPlayMode;

	void Debug(void);
	void PrintVersion(void);

	void ChangeDefaultFrame(void);
	void BugBug(void);

	LPVOID m_myDirect3D;
	LPVOID m_textureCacheControl;

	void OutputAllFilmName(void);

	int m_rubiPrintFlag;
	int m_rubiBuildFlag;
	int m_cutinBuildFlag;
	int m_kanjiCode;

	CRubiMaker* m_rubiMaker;
	void SetRubiParam(void);

	void SetRubiColor(void);
	void ChangeRubiColorFix(void);
	void SetRubiSize(void);
	void SetRubiSukima(void);
	void SetRubiDeltaY(void);

	void SetJpegMMX(BOOL flg = TRUE);
	void SetBackupNNN(BOOL flg = TRUE);

	void ChangeMessageGuide(void);
	void ChangeMessageGyo(void);

	CVarInitData* m_varInitData;
	CShakin* m_shakinControl;

	int m_framePrintFlag;
	int m_skipPrintFlag;

	void ChangePrintFrameFlag(void);
	void ChangePrintSkipFlag(void);
	void SetMaxSkip(void);
	void SetSkipInterval(void);

	void SetPresetButtonOk(int flg);
	void SetLayerCopyMode(int md);
	void SetSceneCheckMode(int md);

	void CheckSceneData(BOOL okPrintFlag = TRUE);
	void ChangePngGamma(void);
	
	void ChangeScreenSize(int md,int screenSizeX,int screenSizeY);
	void ChangeBufferSetCG(void);

	void ChangeInitAngouMode(void);
	void ExtUtil(WPARAM wParam);

	void SetTargetMachine(int n);
	void ChangeDaihonSpace(void);

	void ChangeAddDefaultVoice(void);
	void SetDefaultName(void);
	int m_termFlag;
	int m_balloonFlag;
	int m_zahyoPrintType;

	void ChangeUndoMode(int n);
	void ChangeUndoKakuninMode(int n);
	void ChangeUndoAfterMode(int n);
	int m_undoEnable;
	int m_undoKakuninEnable;
	int m_undoAfterEnable;
	int m_controlKeyEnable;

	int m_varType;
	LPSTR m_varFileName;

	CUndoMemoryObject* m_undoObject;
};




#endif
/*_*/

