//
// 
//

#if !defined __TINYAN_NNNEDIT_MESSAGESETUP__
#define __TINYAN_NNNEDIT_MESSAGESETUP__


class CMyConfig;

class CMessageSetup
{
public:
	CMessageSetup(CMyConfig* lpConfig);
	~CMessageSetup();
	void End(void);

	int GetMesNameByInit(void){return m_mesNameByInit;}
	void SetMesNameByInit(int d);
	int GetMesWinPrintMode(void){return m_mesWinPrintMode;}
	void SetMesWinPrintMode(int d);
	POINT GetMesWinZahyo(void){return m_mesWinZahyo;}
	void SetMesWinZahyo(POINT pt);
	SIZE GetMesWinSize(void){return m_mesWinSize;}
	void SetMesWinSize(SIZE sz);
	LPSTR GetMesWinFileName(void){return m_mesWinFileName;}
	void SetMesWinFileName(LPSTR filename);
	POINT GetMesPrintDelta(void){return m_mesPrintDelta;}
	void SetMesPrintDelta(POINT pt);
	int GetMesNextY(void){return m_mesNextY;}
	void SetMesNextY(int nextY);
	POINT GetMesLPrintZahyo(void){return m_mesLPrintZahyo;}
	void SetMesLPrintZahyo(POINT pt);
	int GetNameWinPrintMode(void){return m_nameWinPrintMode;}
	void SetNameWinPrintMode(int d);
	POINT GetNameWinZahyo(void){return m_nameWinZahyo;}
	void SetNameWinZahyo(POINT pt);
	SIZE GetNameWinSize(void){return m_nameWinSize;}
	void SetNameWinSize(SIZE sz);
	LPSTR GetNameWinFileName(void){return m_nameWinFileName;}
	void SetNameWinFileName(LPSTR filename);
	POINT GetNamePrintDelta(void){return m_namePrintDelta;}
	void SetNamePrintDelta(POINT pt);
	int GetNameAutoOffFlag(void){return m_nameAutoOffFlag;}
	void SetNameAutoOffFlag(int d);

	int GetMesFontSize(void){return m_mesFontSize;}
	void SetMesFontSize(int fontSize);
	int GetNameFontSize(void){return m_nameFontSize;}
	void SetNameFontSize(int fontSize);

private:
	int m_mesNameByInit;
	int m_mesWinPrintMode;
	POINT m_mesWinZahyo;
	SIZE m_mesWinSize;
	LPSTR m_mesWinFileName;
	POINT m_mesPrintDelta;
	int m_mesNextY;
	POINT m_mesLPrintZahyo;
	int m_nameWinPrintMode;
	POINT m_nameWinZahyo;
	SIZE m_nameWinSize;
	LPSTR m_nameWinFileName;
	POINT m_namePrintDelta;
	int m_nameAutoOffFlag;
	int m_mesFontSize;
	int m_nameFontSize;

	CMyConfig* m_config;
	int GetConfig(LPSTR name);
	LPSTR GetConfigString(LPSTR name);
	void SetConfig(LPSTR name,int value);
	void SetConfig(LPSTR name,LPSTR mes);

};



#endif
/*_*/

