//
//
//

#if !defined __NNNEDIT_LOCKVOICE__
#define __NNNEDIT_LOCKVOICE__


class CMyApplication;
class CNameList;

class CLockVoice
{
public:
	CLockVoice(CMyApplicationBase* app);
	~CLockVoice();
	void End(void);

	BOOL AllLockVoice(BOOL flg = TRUE);
	BOOL LockVoice(BOOL flg);

private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CNameList* m_charaList;
};


#endif
/*_*/

