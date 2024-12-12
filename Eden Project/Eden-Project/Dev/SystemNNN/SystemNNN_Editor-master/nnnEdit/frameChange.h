//
//
//

#if !defined __NNNEDIT_FRAMECHANGE__
#define __NNNEDIT_FRAMECHANGE__


class CMyApplication;
class CNameList;
class CMyInputDialog;

class CFrameChange
{
public:
	CFrameChange(CMyApplicationBase* app);
	~CFrameChange();
	void End(void);


	BOOL FrameChange(int oldFrame = -1,int newFrame = -1);
	BOOL AllFrameChange(void);


private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CNameList* m_charaList;
	CMyInputDialog* m_inputDialog;
};


#endif
/*_*/
