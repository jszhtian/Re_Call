//
//
//

#if !defined __NNNEDIT_VARSEARCHORREPLACE__
#define __NNNEDIT_VARSEARCHORREPLACE__

class CMyApplicationBase;
class CNameList;
class CMyVarSearchDialog;
class CVarSearchOrReplace
{
public:
	CVarSearchOrReplace(CMyApplicationBase* app);
	~CVarSearchOrReplace();
	void End(void);

	void SearchOrReplace(BOOL replaceFlag,int nowSelectNumber,int blockStartNumber,int blockEndNumber);

private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CMyVarSearchDialog* m_search;

};

#endif
/*_*/

