//
//
//

#if !defined __NNNEDIT_MAKEBOOK__
#define __NNNEDIT_MAKEBOOK__


class CMyApplication;
class CNameList;

class CMakeBook
{
public:
	CMakeBook(CMyApplicationBase* app);
	~CMakeBook();
	void End(void);

	void SetWhiteSpace(int n);
	BOOL MakeBook(int allFlag,int makeBookMode = 0);
	static char m_bookTypeName[][16];
	static char m_bookTypeDesc[][16];

private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CNameList* m_charaList;
	int m_makeBookMode;
	int m_makeBookAllFlag;
	int m_voiceCount[256];
	void PrintUtilLoadError(LPSTR filename);
	void SetErrorSkip(BOOL flg = TRUE){m_errorSkipFlag = flg;}
	BOOL CheckErrorSkip(void){return m_errorSkipFlag;}
	BOOL m_errorSkipFlag;

//	int m_whiteSpace;
//	char* m_whiteSpaceStr;

};




#endif
/*_*/

