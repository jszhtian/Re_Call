//
//
//

#if !defined __NNNEDIT_SAMEMESSAGEDEUTCH__
#define __NNNEDIT_SAMEMESSAGEDEUTCH__


class CMyApplicationBase;
class CNameList;

class CSameMessageDeutch
{
public:
	CSameMessageDeutch(CMyApplicationBase* app);
	~CSameMessageDeutch();
	void End(void);

	BOOL SameMessageToDeutch(void);

private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
};






#endif
/*_*/

