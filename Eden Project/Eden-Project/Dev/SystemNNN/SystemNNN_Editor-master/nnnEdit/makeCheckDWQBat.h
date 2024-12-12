//
//
//

#if !defined __NNNEDIT_MAKECHECKDWQBAT__
#define __NNNEDIT_MAKECHECKDWQBAT__


class CMyApplication;
class CNameList;

class CMakeCheckDWQBat
{
public:
	CMakeCheckDWQBat(CMyApplicationBase* app);
	~CMakeCheckDWQBat();
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

