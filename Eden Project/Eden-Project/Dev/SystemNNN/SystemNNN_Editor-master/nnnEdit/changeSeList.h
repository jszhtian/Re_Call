//
//
//

#if !defined __NNNEDIT_CHANGESELIST__
#define __NNNEDIT_CHANGESELIST__


class CMyApplicationBase;
class CNameList;

class CChangeSeList
{
public:
	CChangeSeList(CMyApplicationBase* app);
	~CChangeSeList();
	void End(void);

	BOOL AllChange(void);

private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	CNameList* m_charaList;

	CNameList* m_seList;
	CNameList* m_seList2;

	BOOL CheckRoutine(void);
	BOOL ChangeSeRoutine(void);
	int* m_fromToTable;

	void PrintError(LPSTR nnnName,LPSTR filmName,int koma,int mes,LPSTR firstMes);
};


#endif
/*_*/

