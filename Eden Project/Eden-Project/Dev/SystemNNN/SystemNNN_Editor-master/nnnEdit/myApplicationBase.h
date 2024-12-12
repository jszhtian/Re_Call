//
// myapplication.h
//

#if !defined __NNNEDIT_MYAPPLICATIONBASE__
#define __NNNEDIT_MYAPPLICATIONBASE__


class CConfigData;


class CSystemDIB;

class CFilmData;

class CMyFrameWindow;
class CMyClientWindow;
class CMyDocument;

class CPicture;
class CEffect;

class CSelectDialog;

class CStoryData;

//class CConfigFile;

class CMyFileOpen;
class CMyMessage;
class CMyFont;
class CRubiFont;
class CScriptData;
class CNameList;
class CKomaData;

class CStartTitle;
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

class CScriptSoundControl;
class CScriptVoiceControl;

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

class CMyApplicationBase
{
public:
	CMyApplicationBase(HINSTANCE hinstance);
	virtual ~CMyApplicationBase();
	virtual void End(void);

	virtual HINSTANCE GetMyInstance(void) = 0;
	virtual HINSTANCE GetHInstance(void) = 0;

	virtual CMyFrameWindow* GetMyFrame(void) = 0;
	virtual CMyClientWindow* GetMyClient(void) = 0;

	virtual HWND GetFrameHWND(void) = 0;
	virtual HWND GetClientHWND(void) = 0;

	virtual CNameList* GetProjectList(void) = 0;
	virtual CNameList* GetSeList(void) = 0;

	virtual CNameList* GetNameList(void) = 0;
	virtual CNameList* GetSEList(void) = 0;
	virtual CNameList* GetFunctionList(void) = 0;
	virtual CNameList* GetCommandList(void) = 0;
	virtual CNameList* GetBGMList(void) = 0;
	virtual CNameList* GetVarControlList(void) = 0;

	virtual void PrintMessageWindow(bool nameFlag = true) {};

	virtual CNameList* GetCharaList(void) = 0;
	virtual CNameList* GetSetCGList(void) = 0;

	virtual CStoryBookData* GetStoryBookData(void) = 0;
	virtual CFilmCaseData* GetFilmCaseData(void) = 0;

	virtual CMyInputDialog* GetInputDialog(void) = 0;
	virtual CVolumeControl* GetVolumeControl(void) = 0;

	virtual LPSTR GetFileNameOnly(void) = 0;
	virtual void SetFileNameOnly(LPSTR filenameOnly) = 0;

	virtual BOOL CheckSave(BOOL varCheckFlag = FALSE) = 0;
	virtual BOOL LoadByFileNameOnly(LPSTR name) = 0;

	virtual LPSTR GetMyName(void) = 0;
	virtual LPSTR GetMemo(void) = 0;


	virtual void ChangeShowHideWindow(int n) = 0;
	virtual void ShowHideWindow(int n, BOOL flg) = 0;
	virtual void HideWindow(int n) = 0;

	virtual void UpdateMyWindow(int windowNumber) = 0;

	virtual HBITMAP GetCommonBitmap(int n) = 0;
	virtual CStoryData* GetNowSelectStory(void) = 0;
	virtual CFilmData* GetNowSelectFilm(void) = 0;
	virtual CKomaData* GetNowSelectKoma(void) = 0;
	virtual void FilmIsChanged(void) = 0;
	virtual void KomaIsChanged(void) = 0;
	virtual void MessageIsChanged(void) = 0;
	virtual void LayerIsChanged(void) = 0;
	virtual void EffectIsChanged(void) = 0;
	virtual void ZahyoIsChanged(void) = 0;

	virtual void StoryIsChanged(void) = 0;
	virtual void CommandIsChanged(void) = 0;
	virtual void ProgramIsChanged(void) = 0;
	virtual void VarIsChanged(void) = 0;

	virtual void UpdateMainScreen(int cnt = 0) = 0;

	virtual void ConteIsChanged(void) = 0;
	virtual void ConteMessageIsChanged(void) = 0;

	virtual void WindowIsMoved(int windowNumber, int x, int y) = 0;
	virtual void WindowSizeIsChanged(int windowNumber, int x, int y) = 0;

	virtual void WindowIsShowHide(int windowNumber, BOOL showFlag) = 0;
	virtual HGDIOBJ SelObjOnly(HDC hdc, HGDIOBJ obj) = 0;

	virtual void GetInitWindowZahyo(LPSTR windowName, int windowNumber, RECT* lpRect, BOOL* lpWindowFlag) = 0;
	virtual int GetWindowMode(void) = 0;
	virtual SIZE GetDesktopSize(void) = 0;

	virtual void ChangeWindowIfCan(int windowNumber) = 0;
	virtual BOOL SaveRoutine(int n) = 0;

	virtual void PreClickMouse(int windowNumber) = 0;

	virtual int GetNowSelectPictureLayer(void) = 0;
	virtual int GetNowSelectEffectLayer(void) = 0;

	virtual void OnTabKey(int windowNumber,int delta = 1) = 0;

	virtual void ReLoadSeList(void) = 0;
	virtual void ReLoadBGMList(void) = 0;

	virtual HBITMAP GetFilmLargePic(int n) = 0;


	virtual CEffect* GetEffect(void) = 0;
	virtual CEffectParam* GetEffectParam(int n) = 0;

	virtual void SetPlayMode(BOOL md = TRUE, int subCommand = -1, BOOL countClearFlag = TRUE) = 0;
	virtual void SetKomaMode(BOOL md = TRUE) = 0;


	virtual BOOL GetPlayMode(void) = 0;
	virtual int GetSubCommand(void) = 0;

	virtual BOOL CheckPlayMode(void) = 0;
	virtual BOOL CheckPauseMode(void) = 0;

	virtual int GetPlayCommand(void) = 0;
	virtual BOOL CheckKomaMode(void) = 0;

	virtual void SetPauseFlag(BOOL flg = TRUE) = 0;

	virtual void CountUp(int deltaCount = 1) = 0;
	virtual void CheckNextMessage(void) = 0;

	virtual void StopAllMusicAndSound(void) = 0;

	virtual void MoveFilmTop(void) = 0;
	virtual int GetFilmPlayMode(void) = 0;

	virtual void ChangeFilmPlayMode(void) = 0;


	virtual int GetWarningDelScene(void) = 0;
	virtual void SetWarningDelScene(int flg) = 0;
	virtual int GetWarningDelCommand(void) = 0;
	virtual void SetWarningDelCommand(int flg) = 0;
	virtual int GetWarningDelFilm(void) = 0;
	virtual void SetWarningDelFilm(int flg) = 0;
	virtual int GetWarningDelKoma(void) = 0;
	virtual void SetWarningDelKoma(int flg) = 0;
	virtual int GetWarningDelMessage(void) = 0;
	virtual void SetWarningDelMessage(int flg) = 0;
	virtual int GetWarningDelParam(void) = 0;
	virtual void SetWarningDelParam(int flg) = 0;
	virtual int GetWarningDelVar(void) = 0;
	//void SetWarningDelVar(int flg);
	virtual void ChangeWarningDelVar(void) = 0;



	virtual void SetModify(BOOL flg = TRUE) = 0;
	virtual BOOL CheckModify(void) = 0;


	virtual LPSTR GetNewFilmName(void) = 0;
	virtual LPSTR GetNewStoryName(void) = 0;


	virtual int GetConfig(LPSTR name) = 0;
	virtual LPSTR GetConfigString(LPSTR name) = 0;
	virtual void SetConfig(LPSTR name,int value) = 0;
	virtual void SetConfig(LPSTR name,LPSTR mes) = 0;

	virtual void PlayVoice(LPSTR voiceName) = 0;
	virtual void PlaySound(LPSTR soundName) = 0;

	virtual void PlaySeTest(int n,int channel) = 0;
	virtual void PlayMusicTest(int n) = 0;

	virtual CVarInitData* GetVarInitData(void) = 0;
	virtual CShakin* GetShakinControl(void) = 0;

	virtual void SetVarName(int n, LPSTR name) = 0;
	virtual LPSTR GetVarName(int n) = 0;

	virtual BOOL SaveVarName(void) = 0;
	virtual BOOL SaveVarInitData(void) = 0;

	virtual void ScrollToSelect(int windowNumber) = 0;

	virtual void ReplaceVarName(int varNumber,LPSTR searchString,LPSTR replaceString) = 0;
	virtual void SetSelectVarNumber(int varNumber) = 0;

	virtual void SearchVar(BOOL replaceFlag = FALSE) = 0;

	virtual void DebugLog(LPSTR mes) = 0;
	virtual BOOL CheckIamSystemScript(void) = 0;

	virtual int GetFilmSelectDefault(void) = 0;
	virtual int GetStorySelectDefault(void) = 0;
	virtual int GetCalcuSelectDefault(void) = 0;

	virtual int SearchFilm(LPSTR filmName, int searchStart = 0,int noCheckNumber = -1) = 0;
	virtual int SearchStory(LPSTR storyName, int searchStart = 0,int noCheckNumber = -1) = 0;
	virtual int SearchScript(LPSTR sptName, int searchStart = 0,int noCheckNumber = -1) = 0;

	virtual void JumpStory(int n) = 0;
	virtual CFilmData* GetFilmData(int n) = 0;

	virtual void SetEyeStatus(int layer, int st) = 0;
	virtual void OnGameClick(void) = 0;

	virtual void AdjustDstFrom(int px, int py) = 0;
	virtual void AdjustDstTo(int px, int py) = 0;

	virtual void MoveDstFrom(int dx, int dy) = 0;
	virtual void MoveDstTo(int dx, int dy) = 0;

	virtual int GetZahyoSwitch(void) = 0;
	virtual void SetNowKomaEffectRect(int layer) = 0;
	virtual void SetNextPrint(BOOL flg = TRUE) = 0;

	virtual int GetNowSelectParam(void) = 0;

	virtual CPicture* GetMesWinPic(void) = 0;
	virtual CPicture* GetNameWinPic(void) = 0;
	
	virtual int GetNowEffect(void) = 0;
	virtual CMyMessage* GetMyMessage(void) = 0;

	virtual int GetEditorMessageGyo(void) = 0;
	virtual int GetMessageWidthGuideLine(void) = 0;

	virtual BOOL CheckWinNT(void) = 0;
	virtual void CreateNextKoma(void) = 0;

	virtual void PlayScriptMessageVoice(CMessageData* pMessage,int channel) = 0;
	virtual void PlayScriptMessageSound(CMessageData* pMessage,int channel) = 0;

	virtual LPSTR GetCharaName(int n) = 0;
	virtual LPSTR GetSEName(int n) = 0;
	virtual LPSTR GetSEFileName(int n) = 0;
	virtual LPSTR GetBGMName(int n) = 0;
	virtual LPSTR GetBGMFileName(int n) = 0;

	virtual int GetEffectCopyMoveFlag(void) = 0;
	virtual CMyInputDialog* GetMyInputDialog(void) = 0;

	virtual int GetFrameTime(void) = 0;
	virtual int GetDefaultFrameTime(void) = 0;
	virtual void SetFrameTime(int frame) = 0;
	virtual void SetDefaultFrameTime(void) = 0;

	virtual CMessageSetup* GetMessageSetup(void) = 0;

	virtual int GetAutoSetCGFlag(void) = 0;
	virtual void SetAutoSetCGFlag(int flg) = 0;
	virtual int GetSceneMakeFlag(void) = 0;
	virtual void SetSceneMakeFlag(int flg) = 0;
	virtual int GetCheckSameCGNameFlag(void) = 0;
	virtual void SetCheckSameCGNameFlag(int flg) = 0;
	virtual int GetBuildRubi(void) = 0;
	virtual void SetBuildRubiFlag(int flg) = 0;
	virtual int GetBuildCutin(void) = 0;
	virtual void SetBuildCutinFlag(int flg) = 0;
	virtual int GetKanjiCode(void) = 0;
	virtual void SetKanjiCode(int flg) = 0;

	virtual CPicture* GetOverrapPic(void) = 0;
	virtual	CPicture* GetOverrapPic2(void) = 0;

	virtual int CalcuFilmTime(CFilmData* pFilm,int defaultRate = 0) = 0;

	virtual void GetFrameWindowRect(RECT* lpRect) = 0;
	virtual void SetFrameWindowRect(RECT* lpRect) = 0;

	virtual void OnCommand(WPARAM wParam,LPARAM lParam) = 0;
	virtual void OnMove(WPARAM wParam,LPARAM lParam) = 0;

	virtual CRubiMaker* GetRubiMaker(void) = 0;
	virtual int GetRubiPrintFlag(void) = 0;

	virtual int GetGameCount(void) = 0;
	virtual void GameCountUp(int cnt = 1) = 0;

	virtual CMyDocument* GetMyDocument(int windowNumber) = 0;

	virtual int GetCutinFlag(void) = 0;
	virtual CCutinNameData* GetCutinNameData(void) = 0;

	virtual BOOL GetNextPrintFlag(void) = 0;
	virtual BOOL GetPauseFlag(void) = 0;

	virtual void SetWindowName(int windowNumber, LPSTR name) = 0;
	virtual BOOL Save(void) = 0;
	virtual void CheckAndPlaySound(void) = 0;
	virtual void PrintHint(void) = 0;
	virtual void PrintOverrap(int type,int count ,int countMax) = 0;
	virtual void FlipToScreen(void) = 0;
	virtual CMyDirectSound* GetDirectSound(void) = 0;
	virtual void ReloadCutinFile(void) = 0;

	virtual void CopyPreEffect(int n = 1){}

	virtual void CheckSceneData(BOOL okPrintFlag = TRUE){}

	virtual int SearchFaceChara(LPSTR name){return -1;}
	virtual int GetPresetButtonEnable(void) = 0;
	virtual CNameList* GetTermList(void) = 0;

	virtual void OnBalloonArea(int n,POINT pt,int subType,POINT screenPos) {}
	virtual void OnControlKey(int windowNumber,WPARAM wParam,LPARAM lParam) {}
	virtual int GetZahyoPrintType(void){return 0;}

	virtual int GetUndoMode(void)=0;
	virtual int GetUndoKakuninEnable(void)=0;
	virtual int GetUndoAfterUnable(void)=0;
	virtual BOOL CheckUndoMode(void) = 0;
	virtual CUndoMemoryObject* GetUndoObject(void) = 0;
	virtual void CheckAndGetKomaUndo(void) = 0;


	virtual int GetVarType(void) = 0;
	virtual int GetVarKosuu(void) = 0;

	virtual LPSTR GetVarMarkFilename(void) = 0;
	virtual LPSTR GetVarMarkSetupFilename(void) = 0;
	virtual LPSTR GetVarFxfFilename(void) = 0;

protected:

private:

};






#endif
/*_*/
