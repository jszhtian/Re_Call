//
// vardoc.h
//

#if !defined __TINYAN_NNNEDIT_VARDOC__
#define __TINYAN_NNNEDIT_VARDOC__

#include "mydocument.h"

class CMyInputDialog;
class CVarSearchOrReplace;
class CVarInitData;

class CVarDoc : public CMyDocument
{
public:
	CVarDoc(CMyApplicationBase* lpApp);
	~CVarDoc();
	void End(void);

//	void OnCloseButton(void);

	int GetVarMode(void);
	int GetVarKosuu(int md = -1);
	int GetVarStart(int md = -1);
	int GetNowSelectNumber0(int md = -1);

	void OnClickModeButton(int n);
	void OnClickVar(int n);

	void OnSaveButton(void);
	void OnCutButton(void);
	void OnCopyButton(void);
	void OnPasteButton(void);

	void OnDelete(void);

	void OnChangeVarName(int n, int md = -1);

	LPSTR GetVarName(int n, int md = -1);

	LPSTR GetScrap(void);


	int GetScrollObjectKosuu(int vh);
	int GetNowSelectNumber(void);

	int GetBlockKosuu(void);


	void OnDeleteMark(int n);
	void OnChangeMarkColor(int n,int color,int backColor);

	void SaveMarkColor(void);

	int GetMarkColor(int n,int md = -1);

	BOOL CheckMarkEdit(void){return m_markChangeFlag;}
	int GetVarBlockKosuu(void){return m_varBlockKosuu;}

	void ReCalcuVarKosuu(void);

	void SelectPrevObject(int prev = -1);
	void SelectNextObject(int next = 1);

	void OnEnterKey(void);
	void OnDeleteKey(void);
	void OnInsertKey(void);

	void OnLeftKey(void);
	void OnRightKey(void);

	void OnSearchButton(BOOL replaceFlag);

	void SetSelectVarNumber(int varNumber);
	void ReplaceVarName(int varNumber,LPSTR searchString,LPSTR replaceString);

	LPSTR GetVarInitDataString(int n,int md = -1);
	void OnChangeInitData(int n);

private:
	int m_varMode;
	int m_nowSelectNumber[10];
	int m_varKosuu[10];
	int m_varStart[10];

	char m_scrapData[256];
	CMyInputDialog* m_input;

	int m_varBlockKosuu;
	BOOL m_scrapFlag;

	int m_markColor[2200];
	BOOL m_markChangeFlag;

	int m_scrapColor;

	CVarSearchOrReplace* m_search;

	CVarInitData* m_varInitData;

	char m_varTypeNameSys[32];
	char m_varTypeNameCom[32];
	char m_varTypeNameGame[32];
	char m_varTypeNamePlay[32];
	char m_varTypeNameWork[32];

};


#endif
/*_*/

