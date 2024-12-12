#if !defined __NNNEDIT_DELETEVOICE__
#define __NNNEDIT_DELETEVOICE__


class CMyApplication;
class CNameList;

class CDeleteVoice
{
public:
	CDeleteVoice(CMyApplicationBase* app);
	~CDeleteVoice();
	void End(void);

	BOOL DeleteVoice(void);
	BOOL DeleteVoiceAll(void);


private:
	BOOL DeleteVoiceRoutine(void);

	BOOL FinalAnswer(void);

	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CNameList* m_charaList;
};


#endif
/*_*/

