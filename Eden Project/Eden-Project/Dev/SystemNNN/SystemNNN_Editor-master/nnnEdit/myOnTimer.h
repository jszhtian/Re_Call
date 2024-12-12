//
//
//
#if !defined __NNNEDIT_MYONTIMER__
#define __NNNEDIT_MYONTIMER__


class CMyApplicationBase;
class CEffect;
class CMyMessage;
class CMessageSetup;

class CRubiMaker;
class CCutinNameData;

class CCutinMessageWindow;
class CShakin;

class CMyOnTimer
{
public:
	CMyOnTimer(CMyApplicationBase* app);
	~CMyOnTimer();
	void End(void);

	int OnTimer(int n);

	int GetFPS(void);
	int GetNowSkip(void);
	int GetTotalSkip(void);

private:
	CMyApplicationBase* m_app;
	CEffect* m_effect;
	CMyMessage* m_message;
	CMessageSetup* m_messageSetup;

	int m_messagePrintX;
	int m_messagePrintY;
	int m_messageLPrintX;
	int m_messageLPrintY;
	int m_messageAppendX;
	int m_messageAppendY;

	int m_messageWindowPrintMode;
//	int m_messageWindowPrintX;
//	int m_messageWindowPrintY;
//	int m_messageWindowSizeX;
//	int m_messageWindowSizeY;
	POINT m_messageWindowPrintZahyo;
	SIZE m_messageWindowSize;

	int m_messageWindowColorR;
	int m_messageWindowColorG;
	int m_messageWindowColorB;
	int m_messageWindowPercent;

	int m_messageNextY;

	int m_messageFontSize;
	int m_nameFontSize;

	CRubiMaker* m_rubiMaker;

	CCutinNameData* m_cutinNameData;
	BOOL CheckTagPicFile(LPSTR tagName,LPSTR* fileList);

	CCutinMessageWindow* m_cutinMessageWindow;

	int m_fps;
	int m_fpsWork;
	int m_totalSkipFrame;
	LONGLONG m_oldTime;
	LONGLONG GetLocalTimeCount(void);

	CShakin* m_shakinControl;
	int m_skipFrame;
};


#endif
/*_*/

