//
//
//

#if !defined __NNNEDIT_SEARCHORREPLACE__
#define __NNNEDIT_SEARCHORREPLACE__

class CMyApplicationBase;
class CNameList;
class CMySearchDialog;
class CSearchOrReplace
{
public:
	CSearchOrReplace(CMyApplicationBase* app);
	~CSearchOrReplace();
	void End(void);

	void SearchOrReplace(BOOL replaceFlag,BOOL allFlag);

private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CMySearchDialog* m_search;

};

#endif
/*_*/

