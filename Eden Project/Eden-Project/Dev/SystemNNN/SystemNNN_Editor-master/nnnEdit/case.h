//
// case.h
//

#if !defined __NNNEDIT_CASE__
#define __NNNEDIT_CASE__


//#include "../nnnEdit/picojson.h"

class CMessageData;
//class CMyApplication;
class CScriptData;
class CUndoMemoryObject;

class CCase
{
public:
	CCase();
	virtual ~CCase();
	void End(void);

	virtual CCase* NewObject(void) = 0;
	virtual BOOL Load(FILE* file,CUndoMemoryObject* memory = NULL) = 0;
	virtual BOOL Save(FILE* file,CUndoMemoryObject* memory = NULL) = 0;
	virtual LPSTR GetMyName(void) = 0;
	virtual void SetMyName(LPSTR name) = 0;
	virtual BOOL CopyOriginalData(CCase* lpFrom) = 0;
	virtual int Clean(void) = 0;

	int GetObjectKosuu(void){return m_objectKosuu;}
	int GetObjectKosuuMax(void){return m_objectKosuuMax;}
	int GetNowSelectNumber(void);
	int GetNowSelectSubNumber(void);
	int GetSelectStart(void);
	int GetSelectEnd(void);

	BOOL CheckInSubArea(int n);

	CCase* GetNowSelectObject(void);
	CCase* GetObjectData(int n);

	BOOL CopyData(CCase* lpFrom);

	BOOL AddArray(int n, int place = -1);
	BOOL CreateObjectData(int place = -1, int kosuu = 1);
	BOOL DeleteObjectData(int place, int kosuu = 1);

	BOOL LoadArrayObject(int n,FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL SaveArrayObject(FILE* file,CUndoMemoryObject* memory = NULL);

	void SetSelectNumber(int n);
	void SetSelectSubNumber(int n);
	virtual void Init(LPVOID para = NULL);

	virtual int Compile(CScriptData* lpScript);
	virtual void OutputScriptSource(FILE* file);
	virtual void OutputScriptSourceJson(FILE* json);

	virtual int Pass1(void);

	virtual BOOL CheckExistChara(int charaNumber);

	virtual int GetDataVersion(void);
	virtual void SetDataVersion(int version);

	void OutputScriptSourceComment(FILE* file,LPSTR mes = NULL);

	void OutputData(FILE* file,LPSTR mes,int tab = 0,BOOL crFlag = TRUE);
	BOOL CaseRead(LPVOID ptr,size_t elementSize,size_t count,FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL CaseWrite(LPVOID ptr,size_t elementSize,size_t count,FILE* file,CUndoMemoryObject* memory = NULL);

protected:

	int m_nowSelectNumber;
	int m_nowSelectSubNumber;
	int m_objectKosuu;
	int m_objectKosuuMax;
	CCase** m_objectDataArray;

	int m_minAddKosuu;

	int m_version;

//	CMyApplication* m_app;
private:

	int m_assertCheck;

};


#endif
/*_*/

