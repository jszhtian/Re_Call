//
// commanddata.h
//

#if !defined __NNNEDIT_COMMANDDATA__
#define __NNNEDIT_COMMANDDATA__

//class CMessageData;

class CNameList;
class CCase;
class CUndoMemoryObject;

class CCommandData : public CCase
{
public:
	CCommandData();
	~CCommandData();
	void End(void);



	int GetCommandType(void);
	LPSTR GetTextBuffer(void);
	BOOL SetTextBuffer(LPSTR mes);

	int GetLevel(void) {return m_level;}
	void SetLevel(int level){m_level = level;}

	int GetPara(void);

	int GetSubKosuu(void);
	void SetSubKosuu(int subKosuu);

	BOOL ChangeCommandType(int typ,int para = 0);

	static char m_commandTypeName[][16];
	static int m_needBufferSize[];
	static int m_textType[];

	int GetSerial(void);
	void SetSerial(int n);

	int GetMessageGyo(void);
	int GetMessage1Gyo(int n);

	int GetSelectMessageSerial(void);
	void SetSelectMessageSerial(int n);

//åpè≥
	BOOL Load(FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL Save(FILE* file,CUndoMemoryObject* memory = NULL);
	LPSTR GetMyName(void);
	void SetMyName(LPSTR name);
	CCase* NewObject(void);
	BOOL CopyOriginalData(CCase* lpFrom);
	int Clean(void){return 0;}

	void OutputScriptSource(FILE* file);
	void OutputScriptSourceJson(FILE* file);

	void Init(LPVOID para = NULL);

	int GetTextType(void);

	int GetSystemCommandType(void);
	int GetSystemCommandNumber(void);
	void SetSystemFunction(int funcNumber);
	void SetSystemCommand(int commandNumber);

	int GetBGMNumber(void);
	void SetBGMNumber(int n);
//	void SetBGMKaisuu(int n);
//	int GetBGMKaisuu(void);

//	int GetPara(void);
//	void SetPara(int para);


	void SetColor(int n){m_color = n;}
	void SetBackColor(int n){m_backColor = n;}
	int GetColor(void){return m_color;}
	int GetBackColor(void){return m_backColor;}

	static char m_tmpMessage[];

	static CNameList* m_functionList;
	static CNameList* m_commandList;

private:
//effect

	int m_commandType;

	int m_bufferSize;
	int m_bufferSizeMax;
	char* m_buffer;

	int m_level;
	int m_para;
	int m_subKosuu;

	int m_systemCommandType;
	int m_systemCommandNumber;

	int m_bgmNumber;
	int m_serial;

	int m_color;
	int m_backColor;

	int m_selectMessageSerial;
//	int m_messageDataKosuuMax;
//	int m_messageDataKosuu;

//	CMessageData** m_messageDataArray;
};


#endif
/*_*/
