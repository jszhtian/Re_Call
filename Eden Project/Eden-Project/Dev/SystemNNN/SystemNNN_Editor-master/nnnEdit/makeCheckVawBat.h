//
//
//

#if !defined __NNNEDIT_MAKECHECKVAWBAT__
#define __NNNEDIT_MAKECHECKVAWBAT__


class CMyApplication;
class CNameList;

class CMakeCheckVawBat
{
public:
	CMakeCheckVawBat(CMyApplicationBase* app);
	~CMakeCheckVawBat();
	void End(void);

	BOOL MakeBat(void);
private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CNameList* m_seList;

	int SearchPicNameSub(LPSTR name, int kosuu, char* buff);

};

#endif
/*_*/

