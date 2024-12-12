//
// filmcasedoc.h
//

#if !defined __TINYAN_NNNEDIT_FILMCASEDOC__
#define __TINYAN_NNNEDIT_FILMCASEDOC__

#include "mydocument.h"

class CFilmData;
class CFilmCaseData;
class CMyFileOpen;
class CMyInputDialog;

class CFilmCaseDoc : public CMyDocument
{
public:
	CFilmCaseDoc(CMyApplicationBase* lpApp);
	~CFilmCaseDoc();
	void End(void);

//	void OnCloseButton(void);

	void OnNewFilm(int n = -1);
	void OnOpenFilm(int n = -1);
	void OnSaveFilm(int n = -1);

	void OnDelete(int n = -1,BOOL ignoreUndo = FALSE);
//	void OnUndo(int n = -1);
	void OnSearch(int n = -1);

	void OnChangeName(int n);
	void OnNoClearEffect(void);
	void OnSpecialType(int n = -1);
	void OnConfigMask(int n = -1);
	void OnTaikenLevel(int n = -1);
	void OnCutin(int n = -1);
	void OnSkipFilm(int n = -1);

	int GetFilmKosuu(void);
	CFilmData* GetNowSelectFilm(void);
	CFilmData* GetFilm(int n);

//	BOOL LoadAllFilm(FILE* file,int kosuu);
//	BOOL SaveAllFilm(FILE* file);

	int GetNowSelectNumber(void);
	void SetSelectNumber(int n);

	CFilmCaseData* GetFilmCaseData(void);

	void OnSelectFilm (int n);

//	int SearchFilm(LPSTR filmName,int searchStart = 0,int noCheckNumber = -1);

	void InitLoaded(void);

	BOOL ClearAllFilm(void);
	void ReCalcuScrollPara(void);

	int GetScrollObjectKosuu(int vh);

	void OnChangeColor(int n,int col);
//	void SetFilmColor(int n,int col);
	int GetFilmColor(int n);

	void ScrollToSelect(void);


	CCase* GetNowSelectCaseObject(void);

	void OnSelectNumber(int n);
	void OnDeleteKey(void);
	void OnEnterKey(void);
	void OnInsertKey(void);

	void OnSpaceKey(void);

	void OnCut(int n = -1);
	void OnCopy(int n = -1,BOOL ignoreUndo = FALSE);
	void OnPaste(int n = -1);

	void OnRenameLayer(int n = -1);

	BOOL CheckBufferExist(void){return m_filmBufferFlag;}

	void ExchangeFilm(int n1,int n2);
	void MoveAndInsertFilm(int from,int to);

	void CheckAndGetUndo(CFilmCaseData* pFilmCase,int start,int end);
	BOOL CheckExistUndo(void);
	BOOL OnUndo(int n = -1);
	void ClearUndo(void);


private:
//	CFilmData** m_filmDataArray;
//	BOOL CreateFilmArray(int n);
//	int m_filmKosuu;
//	int m_filmKosuuMax;
//	int m_nowSelectNumber;
	CFilmCaseData* m_filmCaseData;
	CMyFileOpen* m_file;
	CMyInputDialog* m_inputDialog;
	int m_filmBufferFlag;

};


#endif
/*_*/

