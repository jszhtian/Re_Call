//
//
//

#if !defined __NNNEDIT_MESSAGEWINDOWCONTROL__
#define __NNNEDIT_MESSAGEWINDOWCONTROL__


class CMyApplication;
class CPicture;
class CMessageWindowControl
{
public:
	CMessageWindowControl(CMyApplicationBase* app);
	~CMessageWindowControl();
	void End(void);

private:
	CMyApplicationBase* m_app;

};


#endif
/*_*/

