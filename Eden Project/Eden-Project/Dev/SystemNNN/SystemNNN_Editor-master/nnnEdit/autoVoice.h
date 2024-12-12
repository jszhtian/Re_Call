//
//
//

#if !defined __NNNEDIT_AUTOVOICE__
#define __NNNEDIT_AUTOVOICE__


class CMyApplicationBase;
class CNameList;
class CAutoVoice
{
public:
	CAutoVoice(CMyApplicationBase* app);
	~CAutoVoice();
	void End(void);


	BOOL SetVoice(void);
	BOOL AllSetVoice(void);


private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CNameList* m_charaList;
};


#endif
/*_*/

