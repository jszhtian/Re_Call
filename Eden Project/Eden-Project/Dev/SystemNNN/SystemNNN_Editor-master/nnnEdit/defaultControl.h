//
//
//

#if !defined __NNNEDIT_DEFAULTCONTROL__
#define __NNNEDIT_DEFAULTCONTROL__


class CMyApplication;
class CMyInputDialog;

class CDefaultControl
{
public:
	CDefaultControl(CMyApplicationBase* app);
	~CDefaultControl();
	void End(void);

	void SetDefaultMovieRate(void);
	void SetDefaultMovieScale(void);
	void SetDefaultBGLayer(void);
	void SetDefaultTALayer(void);
	void SetDefaultOverrapTime(void);
private:
	CMyApplicationBase* m_app;
	CMyInputDialog* m_input;
};





#endif
/*_*/

