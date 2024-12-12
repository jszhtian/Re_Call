//
//
//

#if !defined __NNNEDIT_MAKEUSEDWQBAT__
#define __NNNEDIT_MAKEUSEDWQBAT__


class CMyApplication;
class CNameList;

class CMakeUseDWQBat
{
public:
	CMakeUseDWQBat(CMyApplicationBase* app);
	~CMakeUseDWQBat();
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

