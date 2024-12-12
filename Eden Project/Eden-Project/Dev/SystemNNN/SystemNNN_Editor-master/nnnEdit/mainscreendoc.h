//
// mainscreendoc.h
//

#if !defined __TINYAN_NNNEDIT_MAINSCREENDOC__
#define __TINYAN_NNNEDIT_MAINSCREENDOC__

#include "mydocument.h"
class CMyApplication;
class CMainScreenView;
class CMyMessage;

class CPicture;

class CMessageSetup;

class CEffectParam;
class CMainScreenDoc : public CMyDocument
{
public:
	CMainScreenDoc(CMyApplicationBase* lpApp);
	~CMainScreenDoc();
	void End(void);

//	void DmyPrint(void);
	void FlipToScreen(void);
	void Moved(WPARAM wParam, LPARAM lParam);

	void OnLButtonDown(int x, int y);

	void MoveToByCenter(int x, int y);
	void MovePicture(int dx, int dy);

	void AdjustPicture(int dx, int dy);

	CEffectParam* GetEffectParam(int n);
	int GetNowEffect(void);

	void PrintLayerHint(int btn);
	void PrintEffectHint(int effectLayer);
	void PrintPlayModeIcon(int playMode,int storyMode = 0);
	BOOL GetPlayMode(void);

	void UpdateMainScreen(void);
	//int GetActiveEffectParaKosuu(void);
	CMyMessage* GetMyMessage(void);

	int GetNowSelectParam(void);

	void PrintFrameTime(int nowFrame,int setFrame);
	void PrintShakin(int shakin,int skip,int totalSkip);

	void ChangeSelectParam(int selectParam);

	int GetEffectMenuPrintMode(void);
	int GetEffectBarPrintMode(void);
	int GetEffectMenuPrintUpDown(void);
	int GetEffectPrintLeftRight(void);
	int GetEffectParamNamePrintMode(void);
	int GetEffectDescPrintMode(void);
	int GetEffectParamDescPrintMode(void);
	void OnClickEffectMenu(int n);

	void EditFrameIsChanged(void);

	int GetEditFrameCount(void);

	int GetEditFrameType(void){return m_editFrameType;}
	int GetEditFrame(void){return m_editFrame;}

	void OnClickEffectType(void);
	void OnClickEffectGaze(int d);
	void OnDragEffectGaze(int d);
	void OnUpDownEffectGaze(int d);

	CPicture* GetMesNameWinPic(int n);
	void SetMessageSetup(CMessageSetup* lpMessageSetup);

private:
	int m_effectMenuPrintMode;
	int m_effectBarPrintMode;
	int m_effectMenuPrintUpDown;
	int m_effectPrintLeftRight;
	int m_effectParamNamePrintMode;
	int m_effectDescPrintMode;
	int m_effectParamDescPrintMode;

	void GetEditFrameSetup(void);

	int m_editFrameType;
	int m_editFrame;
};

#endif
/*_*/

