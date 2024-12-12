//
//
//

#if !defined __NNNEDIT_MAKEUSEVAWBAT__
#define __NNNEDIT_MAKEUSEVAWBAT__


class CMyApplication;
class CNameList;

class CMakeUseVawBat
{
public:
	CMakeUseVawBat(CMyApplicationBase* app);
	~CMakeUseVawBat();
	void End(void);

	BOOL MakeBat(void);
private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CNameList* m_seList;
	CNameList* m_charaList;

	int SearchPicNameSub(LPSTR name, int kosuu, char* buff);

};

#endif
/*_*/

