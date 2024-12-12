#if !defined _NNNEDIT_RUBIMAKER_
#define _NNNEDIT_RUBIMAKER_

class CNameList;
class CRubiMaker
{
public:
	CRubiMaker(BOOL createFlag = FALSE);
	~CRubiMaker();
	void End(void);

	void Load(void);

	LPSTR RubiConvert(LPSTR mes);

private:
	static char m_suujiMessage[][4];

	CNameList* m_list;
	BOOL m_loadFlag;
	BOOL m_loadErrorFlag;
	int m_rubiKosuu;
	int* m_kanjiLengthTable;

	char* m_makedBuffer;

	void CreateTable(void);
	void PrintRubiError(LPSTR typeMes,LPSTR orgMes);
};

#endif
/*_*/
