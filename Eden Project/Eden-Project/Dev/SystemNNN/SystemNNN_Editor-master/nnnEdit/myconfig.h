//
// myconfig.h
//

#if !defined __NNNEDIT_MYCONFIG__
#define __NNNEDIT_MYCONFIG__

class CConfigData;

class CMyConfig
{
public:
	CMyConfig(int varType = 1);
	~CMyConfig();
	void End(void);

	LPSTR GetText(LPSTR name);
	int GetValue(LPSTR name);

	BOOL SetText(LPSTR name, LPSTR text);
	BOOL SetValue(LPSTR name, int d);


	typedef struct _tagMYCONFIGTEXTTYPE
	{
		char name[64];
		char text[64];
	}MYCONFIGTEXTTYPE;

	typedef struct _tagMYCONFIGVALUETYPE
	{
		char name[64];
		int value;
	}MYCONFIGVALUETYPE;

	typedef struct _tagMYCONFIGWINDOWTYPE
	{
		char name[64];
		int md,left,top,right,bottom;
		int md2,left2,top2,right2,bottom2;
	}MYCONFIGWINDOWTYPE;

	static MYCONFIGTEXTTYPE m_initDataString[];
	static MYCONFIGVALUETYPE m_initDataValue[];
	static MYCONFIGWINDOWTYPE m_initDataWindow[];

	void GetWindowZahyo(LPSTR windowName, int md, RECT* lpRect, BOOL* lpFlag);
	void SetWindowZahyo(LPSTR windowName, int md, RECT* lpRect, BOOL flag);

	int m_varType;

private:
	BOOL SaveData(void);
	void SetInitData(void);
	void AdjustInitData(void);
	CConfigData* m_configData;
};


#endif
/*_*/

