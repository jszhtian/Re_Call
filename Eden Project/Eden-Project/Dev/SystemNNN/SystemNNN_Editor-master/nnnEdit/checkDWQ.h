//
//
//

#if !defined __NNNEDIT_CHECKDWQ__
#define __NNNEDIT_CHECKDWQ__

class CMyApplicationBase;
class CNameList;
class CCheckDWQ
{
public:
	CCheckDWQ(CMyApplicationBase* app);
	~CCheckDWQ();
	void End(void);

	BOOL CheckDWQAll(void);
	BOOL CheckDWQ(void);


private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
};


#endif
/*_*/

