//
// storydata.h
//

#if !defined __NNNEDIT_STORYDATA__
#define __NNNEDIT_STORYDATA__


class CCommandData;
class CCase;
class CUndoMemoryObject;

class CStoryData : public CCase
{
public:
	CStoryData();
	~CStoryData();
	void End(void);



	
//	BOOL CreateCommand(int n);

//	BOOL AddCommandArray(int n, int place = -1);

//	BOOL CreateCommandData(int place = -1, int kosuu = 1);

	CCommandData* GetCommand(int n);
	void DeleteChildBlock(int n,int k);
	void DeleteWithChild(int n);

	int GetSubTotalKosuu(int n);
//	void Init(LPSTR name = NULL);
	void Init(LPVOID para = NULL);

//	int GetCommandKosuu(void);
//	int GetNowSelect(void);

//	BOOL CreateCommand(int n, int kosuu = 1);

//åpè≥
	LPSTR GetMyName(void);
	void SetMyName(LPSTR name);
	BOOL Load(FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL Save(FILE* file,CUndoMemoryObject* memory = NULL);
	CCase* NewObject(void);
	BOOL CopyOriginalData(CCase* lpFrom);
	int Clean(void){return 0;}


//	void OutputScriptSource(FILE* file);

	BOOL CheckIamTop(void);

	CCommandData* GetParentObject(int n);

	int SearchChild(int n,int k);

	CCommandData* GetBackSameLevel(int n);
	CCommandData* GetNextSameLevel(int n);

	void OutputScriptSource(FILE* file);
	void OutputScriptSourceJson(FILE* file);
	static BOOL m_adjustEnfIf;

private:
	char m_myname[64];

//	int m_commandKosuu;
//	int m_commandKosuuMax;
//	CCommandData** m_commandDataArray;

//	int m_nowSelectNumber;
};

#endif
/*_*/

