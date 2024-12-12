//
// storydoc.h
//

#if !defined __TINYAN_NNNEDIT_STORYDOC__
#define __TINYAN_NNNEDIT_STORYDOC__

#include "mydocument.h"

class CMyInputDialog;
class CNameList;
class CSelectDialog;

class CStoryDoc : public CMyDocument
{
public:
	CStoryDoc(CMyApplicationBase* lpApp);
	~CStoryDoc();
	void End(void);

//	void OnCloseButton(void);


	void ReCalcuScrollPara(void);

	void OnClickCommand(int n);

	void OnChangeType(int n, int cmd);

	void OnNewCommand(int n = -1);
//	void OnChangeCommand(int n = -1);
	void OnCutCommand(int n = -1);
	void OnCopyCommand(int n = -1);
	void OnPasteCommand(int n = -1);
	void OnDelete(int n = -1);
//	void OnUndo(int n = -1);

	void OnBackCommand(int n = -1);
	void OnFowardCommand(int n = -1);
	void OnJumpCommand(int n = -1);


	void OnEditCommand(int n = -1);
	void OnSelectCommand(int n = -1);
	void OnChangeSelectStory(int n = -1);
	void OnChangeSelectID(int n = -1);

	int SearchFilm(LPSTR filmName, int searchStart=0);

	static int m_commandToTypeTable[];

	CFilmData* GetFilmData(int n);

	CNameList* GetFunctionList(void);
	CNameList* GetCommandList(void);
	CNameList* GetBGMList(void);

	int GetScrollObjectKosuu(int vh);
	int GetNowSelectNumber(void);

	void OnChangeMarkColor(int n,int color,int backColor,int blockFlag);

	void OnEnterKey(void);
	void OnDeleteKey(void);
	void OnInsertKey(void);
//	void SelectPrevObject(void);
//	void SelectNextObject(void);

	void OnEscapeKey(void);

	CCase* GetNowSelectCaseObject(void);
	void OnSelectNumber(int n);
	
	void CheckAndGetUndo(CStoryData* pStory,int start,int end);
	BOOL CheckExistUndo(void);
	BOOL OnUndo(int n = -1);
	void ClearUndo(void);
	void CheckAndGetUndoAll(void);

private:
	int CommandToType(int cmd);
	int CommandToPara(int cmd);
	int CommandToSubKosuu(int cmd);

	CMyInputDialog* m_inputDialog;
	CSelectDialog* m_selectDialog;

	LPSTR* m_ppMesBuffer;
	int m_mesBufferKosuu;

	void CreateMessageBuffer(int n);
	void SubParentHave(int n,int delta = 1);
	int SearchParent(int n,int level);

	BOOL CheckLeftShiftKey(void);
	BOOL CheckRightShiftKey(void);

};


#endif
/*_*/

