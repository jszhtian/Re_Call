//
//
//

#if !defined __NNNEDIT_DELETENONVOICE__
#define __NNNEDIT_DELETENONVOICE__

class CMyApplication;
class CNameList;
class CMyInputDialog;

class CDeleteNonVoice
{
public:
	CDeleteNonVoice(CMyApplicationBase* app);
	~CDeleteNonVoice();
	void End(void);

	BOOL DeleteNonVoice(BOOL selFlag = TRUE);
	BOOL DeleteAllNonVoice(void);

private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CMyInputDialog* m_input;

	int m_deletePlayerNumber;

};


#endif
/*_*/

