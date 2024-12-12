//
// scriptdata.h
//

#if !defined __TINYAN_NNNEDIT_SCRIPTDATA__
#define __TINYAN_NNNEDIT_SCRIPTDATA__

class CScriptData
{
public:
	CScriptData();
	~CScriptData();
	void End(void);

	void SetPC(int pc);
	int GetPC(void);

	int SetFunction(int funcNum, int kosuu = 0,int* paraPtr = NULL);
	int SetScript(int cmd, LPVOID para = NULL, int kosuu = 1);
//	int SetEffect(int effect, int kosuu = 0, int* paraPtr = NULL);

	int SetPrint(int cmd, int n, LPSTR mes);

	int SetTextSub(LPSTR mes);

	void Init(void);

private:
	int m_pc;
	int m_objectSizeMax;
	int* m_object;

	int m_messagePointer;
	int m_messageSizeMax;
	int* m_messageData;
	int* m_messageTable;
	
	int m_textPointer;
	int m_textSizeMax;
	int* m_textData;
	int* m_textTable;

	int m_messageNumberMax;
	int m_textNumberMax;
};

#endif
/*_*/

