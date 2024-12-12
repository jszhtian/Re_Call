//
// filmdoc.h
//

#if !defined __TINYAN_NNNEDIT_FILMDOC__
#define __TINYAN_NNNEDIT_FILMDOC__

#include "mydocument.h"

class CNameList;
class CFilmData;
class CMyInputDialog;
class CSelectDialog;
class CMiniPic;

class CFilmDoc : public CMyDocument
{
public:
	CFilmDoc(CMyApplicationBase* lpApp);
	~CFilmDoc();
	void End(void);

	void OnNewKoma(int n = -1);
	void OnCutKoma(int n = -1);
	void OnCopyKoma(int n = -1);
	void OnPasteKoma(int n = -1);
	void OnDelete(int n = -1);
	//void OnUndo(int n = -1);
	void OnOverrap(int n = -1);

	void OnOverrapType(int n = -1);
	void OnSetCG(int n = -1);
//	void OnNoClearEffect(void);
	void OnBGM(int n = -1);
	void OnFrame(int n = -1);
	void OnWindowOff(int n = -1);

	void OnDemo(int n = -1);

	void OnWindowNumber(int n = -1);
	void OnCursorNumber(int n = -1);
	void OnMouseNumber(int n = -1);

	void OnAutoMessage(int n = -1);
	void OnCannotClick(int n = -1);
	void OnCannotSkip(int n = -1);

	void OnOptionOff(int n = -1);
	void OnCutin(int n = -1);

	void OnPreload(int n = -1);
	void OnExpStatus(int n = -1);
	void OnVarControl(int n = -1);

	void OnClickFilm(int n,WPARAM wParam);
	void OnChangeKomaName(int n = -1);

	CFilmData* GetNowSelectFilm(void);

	void InitMiniPicTable(void);

	HBITMAP GetHBitmap(int placeNumber);

	void ReCalcuScrollPara(void);
	int GetSetCG(int n = -1);

	BOOL CheckBufferExist(void);
	int GetBGM(int n = -1);

	int GetScrollObjectKosuu(int vh);
	int GetNowSelectNumber(void);

	int GetFrameTime(int n = -1);

	static char m_defaultKomaName[];

	void ScrollToSelect(void);

	int GetDemoFlag(int n = -1);
	void PlayMusicTest(int n);

	int GetWindowNumber(void);
	int GetCursorNumber(void);
	int GetMouseNumber(void);

	int GetAutoMessage(void);
	int GetCannotClick(void);
	int GetCannotSkip(void);

	int GetOptionOff(void);
	int GetCutinFlag(void);
	int GetPreload(void);

	int GetExpStatus(void);
	int GetVarControl(int n);
	int CheckVarControl(void);


	CCase* GetNowSelectCaseObject(void);

	void OnSelectNumber(int n);
	void OnDeleteKey(void);
	void OnEnterKey(void);
	void OnInsertKey(void);

	void OnSpaceKey(void);
	void OnEscapeKey(void);

	void CheckAndGetUndo(CFilmData* pFilm,int start,int end);
	BOOL CheckExistUndo(void);
	BOOL OnUndo(int n = -1);
	void ClearUndo(void);

private:

	void AddMiniPicBuffer(int n, int place = -1);
	void DeleteMiniPic(int place, int n = 1);
	void InsertMiniPic(int place, int n = 1);

	CMiniPic** m_miniPic;


//	HBITMAP CreateMyBitmap(LPVOID* lplpBuffer);


	int m_miniPicKosuu;
//	int m_miniPicKosuuMax;
	int* m_miniPicNumberTable;
//	int** m_lplpBuffer;

//	int m_miniPicKosuuMax;
//	BOOL* m_miniPicFlag;
//	HBITMAP* m_miniPicHBitmap;


	CMyInputDialog* m_input;
	CSelectDialog* m_selectDialog;
	CSelectDialog* m_overrapDialog;
	CNameList* m_overrapName;

	BOOL m_komaBufferFlag;
//	CKomaData* m_komaBufferFlag;

	CNameList* m_bgmTypeList0;
	CNameList* m_bgmTypeList1;
	CNameList* m_bgmTypeList2;
	CNameList* m_bgmNameList;


};


#endif
/*_*/

