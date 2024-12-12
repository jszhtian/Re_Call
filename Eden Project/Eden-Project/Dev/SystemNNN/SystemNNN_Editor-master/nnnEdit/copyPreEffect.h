//
//
//

#if !defined __NNNEDIT_COPYPREEFFECT__
#define __NNNEDIT_COPYPREEFFECT__


class CMyApplication;
class CEffect;

class CCopyPreEffect
{
public:
	CCopyPreEffect(CMyApplicationBase* app);
	~CCopyPreEffect();
	void End(void);

	void CopyPreEffect(int pre = 1);

private:
	CMyApplicationBase* m_app;
	CEffect* m_effect;
};

#endif
/*_*/

