//
// storybookdata.h
//

#if !defined __NNNEDIT_STORYBOOKDATA__
#define __NNNEDIT_STORYBOOKDATA__

//#include "case.h"
class CUndoMemoryObject;

class CStoryBookData : public CCase
{
public:
	CStoryBookData();
	virtual ~CStoryBookData();
	void End(void);

	BOOL LoadAll(int n, FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL DeleteAll(void);

	//åpè≥
	BOOL Load(FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL Save(FILE* file,CUndoMemoryObject* memory = NULL);
	LPSTR GetMyName(void);
	void SetMyName(LPSTR name);
	CCase* NewObject(void);
	BOOL CopyData(CCase* lpFrom){return TRUE;}

	BOOL CopyOriginalData(CCase* lpCase){return TRUE;}
	int Clean(void){return 0;}

	int SearchStory(LPSTR storyName,int searchStart = 0,int noCheckNumber = -1);

	void OutputScriptSource(FILE* file);
	void OutputScriptSourceJson(FILE* file);

private:

};


#endif
/*_*/

