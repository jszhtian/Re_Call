//
// storybookdoc.h
//

#if !defined __NNNEDIT_STORYBOOKDOC__
#define __NNNEDIT_STORYBOOKDOC__

#include "mydocument.h"

class CStoryBookData;
class CStoryData;
class CMyFileOpen;
class CMyInputDialog;

class CStoryBookDoc : public CMyDocument
{
public:
	CStoryBookDoc(CMyApplicationBase* lpApp);
	~CStoryBookDoc();
	void End(void);

//	void OnCloseButton(void);

	CStoryData* GetStory(int n);
	CStoryData* GetNowSelectStory(void);
	int GetStoryKosuu(void);
	int GetNowSelectNumber(void);

	void OnNewStory(int n = -1);
	void OnOpenStory(int n = -1);
	void OnSaveStory(int n = -1);
	void OnDelete(int n = -1);
//	void OnUndo(int n = -1);
	void OnSearch(int n = -1);

	void OnChangeName(int n);

	void OnSelectStory(int n);

//	BOOL LoadAllStory(FILE* file, int kosuu);
//	BOOL SaveAllStory(FILE* file);

	CStoryBookData* GetStoryBookData(void);
//	int SearchStory(LPSTR storyName,int searchStart = 0,int noCheckNumber = -1);
	void InitLoaded(void);

	void ClearAllStory(void);

	void ReCalcuScrollPara(void);

	int GetScrollObjectKosuu(int vh);

	CCase* GetNowSelectCaseObject(void);

	void OnSelectNumber(int n);
	void OnDeleteKey(void);
	void OnEnterKey(void);
	void OnInsertKey(void);
	void OnSpaceKey(void);

	void CheckAndGetUndo(CStoryBookData* pStoryBook,int start,int end);
	BOOL CheckExistUndo(void);
	BOOL OnUndo(int n = -1);
	void ClearUndo(void);

private:
//	int m_storyKosuu;
//	int m_storyKosuuMax;
//	CStoryData** m_storyDataArray;
//	int m_nowSelectNumber;
//
//	BOOL CreateStoryArray(int n);

	CStoryBookData* m_storyBookData;
	CMyFileOpen* m_file;
	CMyInputDialog* m_inputDialog;

};


#endif
/*_*/

