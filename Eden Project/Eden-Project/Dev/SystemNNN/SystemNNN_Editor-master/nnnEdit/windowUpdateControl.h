//
//
//

#if !defined __NNNEDIT_WINDOWUPDATECONTROL__
#define __NNNEDIT_WINDOWUPDATECONTROL__


class CMyApplication;

class CWindowUpdateControl
{
public:
	CWindowUpdateControl(CMyApplicationBase* app);
	~CWindowUpdateControl();
	void End(void);

	void MyUpdateWindow(int windowNumber);

	void FilmIsChanged(void);
	void KomaIsChanged(void);
	void MessageIsChanged(void);
	void LayerIsChanged(void);
	void EffectIsChanged(void);
	void ZahyoIsChanged(void);

	void StoryIsChanged(void);
	void CommandIsChanged(void);
	void ProgramIsChanged(void);
	void VarIsChanged(void);
	void ConteIsChanged(void);
	void ConteMessageIsChanged(void);

private:
	CMyApplicationBase* m_app;


};

#endif


/*_*/

