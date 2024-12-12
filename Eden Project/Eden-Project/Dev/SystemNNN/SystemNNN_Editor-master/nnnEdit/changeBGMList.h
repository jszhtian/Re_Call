//
//
//

#if !defined __NNNEDIT_CHANGEBGMLIST__
#define __NNNEDIT_CHANGEBGMLIST__


class CMyApplicationBase;
class CNameList;

class CChangeBGMList
{
public:
	CChangeBGMList(CMyApplicationBase* app);
	~CChangeBGMList();
	void End(void);

	BOOL AllChange(void);

private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CNameList* m_charaList;

	CNameList* m_bgmList;
	CNameList* m_bgmList2;

	BOOL CheckRoutine(void);
	BOOL ChangeBGMRoutine(void);
	int* m_fromToTable;

	void PrintError(LPSTR nnnName,LPSTR filmName,int koma);
};


#endif
/*_*/

