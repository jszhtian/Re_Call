//
// filmcasedata.h
//

#if !defined __NNNEDIT_FILMCASEDATA__
#define __NNNEDIT_FILMCASEDATA__


class CEffect;
class CFilmData;
//class CMyApplication;
class CCase;
//#include "case.h"
class CUndoMemoryObject;


class CFilmCaseData : public CCase
{
public:
//	CFilmCaseData(CEffect* lpEffect,CMyApplication* app);
	CFilmCaseData(CEffect* lpEffect);
	~CFilmCaseData();
	void End(void);

	BOOL LoadAll(int n,FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL ClearAllFilm(void);

//	int GetFilmKosuu(void);
//	CFilmData* GetNowSelectFilm(void);
//	CFilmData* GetFilm(int n);
//	int GetNowSelectNumber(void);

//	static char m_myname[64];

	void ExchangeData(int n1,int n2);
	void MoveAndInsertData(int from,int to);

//åpè≥
	LPSTR GetMyName(void);
	void SetMyName(LPSTR name);
	BOOL Load(FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL Save(FILE* file,CUndoMemoryObject* memory = NULL);
	CCase* NewObject(void);

//	int Compile(CScriptData* lpScript);
	void OutputScriptSource(FILE* file);
	void OutputScriptSourceJson(FILE* file);

	int Pass1(void);

	BOOL CopyOriginalData(CCase* lpCase);
	int Clean(void);
	int SearchFilm(LPSTR filmName,int searchStart = 0,int noCheckNumber = -1);

private:
	CEffect* m_effect;

//	int m_filmKosuu;
//	int m_filmKosuuMax;
//	CFilmData** m_filmDataArray;
//	int m_nowSelectNumber;

	int m_labelTableKosuu;
	int* m_labelTable;

};


#endif
/*_*/

