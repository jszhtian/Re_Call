//
// minicompiler.h
//

#if !defined __TINYAN_NNNEDIT_MINICOMPILER__
#define __TINYAN_NNNEDIT_MINICOMPILER__


/*


#define IDENTIFY_DATA 0x66660001
#define IDENTIFY_CONTROL 0x66660002
#define IDENTIFY_COMMAND 0x66660003
#define IDENTIFY_FUNCTION 0x66660004
#define IDENTIFY_CALCU 0x66660005
#define IDENTIFY_SYSTEMCOMMAND 0x66660006
#define IDENTIFY_SYSTEMFUNCTION 0x066660007


#define CODE_DATA_HEADER0 0x55550001
#define CODE_DATA_TABLE 0x55550002
#define CODE_DATA_LABEL 0x55550003
#define CODE_DATA_FILMLABEL 0x55550004




#define CODE_CONTROL_NOP 0x55560001
#define CODE_CONTROL_IF 0x55560002
#define CODE_CONTROL_ELSIF 0x55560003
#define CODE_CONTROL_ELSE 0x55560004
#define CODE_CONTROL_END 0x55560005
#define CODE_CONTROL_NEXT 0x55560006
#define CODE_CONTROL_RETURN 0x55560007
#define CODE_CONTROL_CASE 0x55560008
#define CODE_CONTROL_SUB 0x55560009
#define CODE_CONTROL_SCRIPT 0x5556000a
#define CODE_CONTROL_EXIT 0x5556000b
#define CODE_CONTROL_ENDFILM 0x5556000c
#define CODE_CONTROL_STARTMESSAGE 0x5556000d
#define CODE_CONTROL_ENDKOMA 0x5556000e

#define CODE_CONTROL_WHILE 0x55560011
#define CODE_CONTROL_GOTO 0x55560020


#define TYPE_TABLE_STR 0x44440001
#define TYPE_TABLE_MES 0x44440002
#define TYPE_TABLE_LABEL 0x44440003


//dummy
#define COMMAND_OPENCHUKAKKO1 (SCRIPT_CALCU | '{')
#define COMMAND_CLOSECHUKAKKO1 (SCRIPT_CALCU | '}')
//end dummy


#define CODE_SYSTEMFUNCTION_MUSIC 0x33330007
#define CODE_SYSTEMFUNCTION_VOICE 0x33330008
#define CODE_SYSTEMFUNCTION_SOUND 0x33330009

#define CODE_SYSTEMFUNCTION_PREPAREOVERRAP 0x33330011
#define CODE_SYSTEMFUNCTION_CLEARALLEFFECT 0x33330012
#define CODE_SYSTEMFUNCTION_SETEFFECT 0x33330013
#define CODE_SYSTEMFUNCTION_CLEAREFFECTLAYER 0x33330014
#define CODE_SYSTEMFUNCTION_LOADDWQ 0x33330015
#define CODE_SYSTEMFUNCTION_SETEFFECTRECT 0x33330016
#define CODE_SYSTEMFUNCTION_NOMESSAGEWINDOW 0x33330017
#define CODE_SYSTEMFUNCTION_SETCG 0x33330018
#define CODE_SYSTEMFUNCTION_SETSCENE 0x33330019
#define CODE_SYSTEMFUNCTION_SETFILM 0x3333001a
#define CODE_SYSTEMFUNCTION_STARTMESSAGE 0x3333001b


#define CODE_SYSTEMCOMMAND_PRINT 0x22220001
#define CODE_SYSTEMCOMMAND_LPRINT 0x22220002
#define CODE_SYSTEMCOMMAND_APPEND 0x22220003
#define CODE_SYSTEMCOMMAND_DRAW 0x22220004
#define CODE_SYSTEMCOMMAND_OVERRAP 0x22220005
#define CODE_SYSTEMCOMMAND_SELECT 0x22220006


#define CODE_USERCOMMAND 0x11110001

*/



class CNameList;

class CMiniCompiler
{
public:
	CMiniCompiler();
	~CMiniCompiler();
	void End(void);

	BOOL Pass1(void);	// コメントをはずし、文字列とそれ以外に分ける

	BOOL Pass2If(int code);
	BOOL Pass2Func(int funcNum, int type);
	BOOL Pass2Calcu(void);
	BOOL Pass2Sub(int n);
	BOOL Pass2Goto(int n);

	BOOL Pass2Message(int typ, LPSTR mes);
	BOOL Pass2Select(int typ, int serial,LPSTR mes,int selectSerial = 0);
	BOOL Pass2Case(int n);

	BOOL Pass2Simple(int typ,FILE* jsonFlag = NULL,int tab = 0);

	BOOL Pass2SimpleData(int subData);

	BOOL Pass2OpenChukakko(void);
	BOOL Pass2CloseChukakko(void);

	BOOL Pass2Script(LPSTR mes,LPSTR checkSrcFileName = NULL);
	BOOL Pass2ScriptJump(LPSTR mes,LPSTR checkSrcFileName = NULL);
	BOOL Pass2SubScript(LPSTR mes,LPSTR checkSrcFileName = NULL);

	BOOL Pass2Debug(LPSTR mes);

	//BOOL Pass2BGM(int* paraPtr);
	BOOL Pass2Music(int n,int* paraPtr,LPSTR filename = NULL,FILE* jsonFlag = NULL,int tab = 0);
//	BOOL Pass2PrepareOverrap(void);
//	BOOL Pass2Overrap(int typ ,int count);
	BOOL Pass2SystemFunction(int func, int n=0, int* pData=NULL,FILE* jsonFlag = NULL,int tab = 0);
	BOOL Pass2SystemFunctionMessage(int func, LPSTR str);
	BOOL Pass2SystemFunctionLoad(int func, int n, LPSTR filename,FILE* jsonFlag = NULL,int tab = 0);

	BOOL Pass2SystemCommand(int cmd, int n, int* pData,FILE* jsonFlag = NULL,int tab = 0);
	BOOL Pass2SystemCommandMessage(int cmd, LPSTR mes,int id,int cutin,FILE* jsonFlag = NULL,int tab = 0);

	BOOL Pass2Voice(LPSTR filename,int ch,int paraKosuu,int* pPara,int defUse = 0,FILE* jsonFlag = NULL,int tab = 0);
	BOOL Pass2Sound(int seNum,int ch,int paraKosuu,int* pPara,FILE* jsonFlag = NULL,int tab = 0);
	BOOL Pass2VoiceFlag(int flagNumber,FILE* jsonFlag = NULL,int tab = 0);

	BOOL SkipComment(LPSTR* pStrSrc);
	BOOL SkipSpace(LPSTR* pStrSrc);

	int SkipSource(LPSTR ptr);
	int SkipToReturn(LPSTR ptr);
	int SkipToken(LPSTR ptr);


	char* GetErrorCode(void);
	char* GetErrorMessage(void);
	char* GetErrorMessage2(void);

	typedef struct _tagNAMETABLE
	{
		int code;
		char name[32];
	} NAMETABLE;

//	typedef struct _tagFUNCTIONTABLE
//	{
//		int code;
//		char name[32];
//		int paraKosuuMin;
//		int paraKosuuMax;
//		int paraType[32];
//	} FUNCTIONTABLE;

	typedef struct _tagENZANSHINAMETABLE
	{
		int code;
		char name[32];
		int settoken;
	} ENZANSHINAMETABLE;

	static NAMETABLE m_commandName[];
//	static FUNCTIONTABLE m_functionName[];
	static ENZANSHINAMETABLE m_enzanshiName[];
//	static NAMETABLE m_effectName[];

//	static char m_functionName[][32];
//	static char m_enzanshiName[][32];

	int GetCodeNameNumber(LPSTR ptr);
//	int GetEffectNameNumber(LPSTR name);
	int GetEnzanshiNameNumber(LPSTR name);
	int SearchToken(int n, int cd, int cd2 = -1);

	void Pass2Error(int n, LPSTR mes = NULL);

//	int SetMessage(int pc, int dat);

//	BOOL CheckEqualLet(int dat);

//	BOOL MakeObject(LPSTR filename);

//	int SearchLabelByNumber(int nm, int mx);
//	int SearchTaiouChukakko(int nm, int mx);

	static int m_juniTable[5][5];
	static int m_juniTableFrom[5];
	static int m_juniTableTo[5];

//	void MakeVoiceMessage(LPSTR filename);
//	void Check4GyoOver(LPSTR filename);

//	void MakeVarSpt(LPSTR filename);

	void Init(void);

	void SetFunctionList(CNameList* nameList);
	void SetCommandList(CNameList* commandList);
	void SetVarList(CNameList* varList);
	void SetFilmList(CNameList* filmList);
	void SetStoryList(CNameList* storyList);


	BOOL SetCompileSource(LPSTR src);

	int GetObjectSize(void);
	int* GetObjectPointer(void);

	void StartBuild(int targetMachine);
	void EndBuild(void);

	BOOL AddNowCode(void);
	BOOL AddCode(int* pCode, int n);

	void SetCodeDirect(int n, int d);
	void SetStoryKosuu(int n);

	int* GetMakedPointer(void);
	int GetMakedSize(void);

	BOOL AddStr(LPSTR str);
	BOOL AddMes(LPSTR mes);

	void ClearErrorSkip(void){m_errorSkipFlag = FALSE;}

	static char m_defaultCaption[];

	int GetSubKosuu(void){return m_subKosuu;}
	int GetSelectKosuu(void){return m_selectKosuu;}
	int GetScriptCallKosuu(void){return m_scriptCallKosuu;}
	int GetCommandCallKosuu(void){return m_commandCallKosuu;}

	int* GetSubTable(void){return m_subTable;}
	int* GetSelectTable(void){return m_selectTable;}
	int* GetScriptCallTable(void){return m_scriptCallTable;}
	int* GetCommandCallTable(void){return m_commandCallTable;}

	static char m_systemConst[][32];
	void OutputData(FILE* file,LPSTR mes,int tab = 0,BOOL crFlag = TRUE);

	LPSTR GetFunctionName(int type);
	typedef struct _tagMYFUNCTIONTONAME
	{
		int type;
		char name[64];
	} MYFUNCTIONTONAME;

	static MYFUNCTIONTONAME m_functionToNameTable[];
	int m_functionToNameNumber;

	int GetMessageGyo(LPSTR mes);
	int GetMessage1Gyo(int n,LPSTR mes);
	char m_tmpMessage[1024];

private:
//	BOOL m_loadFlag;
	BOOL m_pass1OkFlag;

	int GetJuni(int token);
	int GetJuniTable(int from, int to);

	int KoubunKaiseki(int n,BOOL bVarFlag = FALSE,int sz = -1);
	int KaisekiSub(int n);

	int GetFunctionNumber(LPSTR name);
	int GetSystemCommandNumber(LPSTR name);

	void SetKaisekiKekka(int pc, int n);

	int m_kaisekiKosuu;
	int m_kaisekiLength[256];
	int m_kaisekiKekkaLength[256];

	int m_kaisekiStart[256];
	int m_kaisekiData[1024];

	int m_kaisekiType[1024];

	int m_tana[1024*2];

	int m_largeConst[4096];
	int m_largeConstKosuu;

	BOOL m_pass2OkFlag;

//	CNameList* m_varList;
//	CButtonList* m_buttonlist;
//	CNameList* m_code;
	CNameList* m_placeList;

	CNameList* m_jinmeiList;

	int m_sourceFileSize;

	char* m_fileBuffer;
	int m_fileBufferSize;

//	int m_remKosuu;
//	int m_messageKosuu;
//	int m_messageKosuu2;

//	int m_messageLength1;
//	int m_messageLength2;

	int m_gyo;
//	int m_messagePointer[32767];
//	int m_messageLength[32767];

//	int m_messageLengthData[32767];
//	int m_messageLengthData2[32767];

	int m_commandKosuu;
	int m_functionKosuu;
	int m_enzanshiKosuu;

//	int m_codeListTable[256];

	int m_tokenKosuu;
	int m_token[65536];
	int m_tokenSource[65536];
	int m_tokenGyo[65536];
	int m_objToToken[65536];

	int m_dateLabelKosuu;
	int m_labelKosuu;
	int m_buttonLabelKosuu;

	int m_codeKosuu;
	int m_varKosuu;
	int m_buttonKosuu;
	int m_placeKosuu;
	int m_effectKosuu;

//	int m_dateLabelPointer[16384];
//	int m_dateLabelLength[16384];
//	int m_labelPointer[16384];
//	int m_labelLength[16384];
//	int m_buttonLabelPointer[16384];
//	int m_buttonLabelLength[16384];
//	int m_buttonLabelNumber[16384];

//	int m_dateLabelListKosuu;
//	int m_labelListKosuu;
//	int m_buttonLabelListKosuu;

//	char m_dateLabelList[16384][8];
//	char m_buttonLabelList[16384][36];
//	char m_labelList[16384][36];


	char m_errorCode[160];
	char m_errorMessage[160];
	char m_errorMessage2[160];

	int m_objectCode[4096];
	//int m_makedCode[65536*4*4];
	int* m_makedCode;
	int m_objectSize;

	void AddError(LPSTR errorMessage, int dataPtr);
	void AddErrorPass0(LPSTR errorMessage, int dataPtr);
	void AddErrorPass1(LPSTR errorMessage, int dataPtr);

	BOOL AddMessage(int dataPtr);
	BOOL AddMessage2(int dataPtr);

//	BOOL AddSub(void);
	//int* m_messageBuffer;
	//int* m_messageBuffer2;

	int m_filmKosuu;
	int m_storyKosuu;


	int m_kaisekiSrc[256];

	CNameList* m_functionList;
	CNameList* m_commandList;
	CNameList* m_varList;
	CNameList* m_filmList;
	CNameList* m_storyList;

	int m_pc;

	int m_strKosuu;
	int m_mesKosuu;

	int m_subKosuu;
	int m_mesSize;
	int m_strSize;

	int* m_strPointer;
	int* m_mesPointer;

	int m_strPointerSize;
	int m_mesPointerSize;

//	int m_strTable[4096*4];
//	int m_mesTable[4096*4];
	int* m_strTable;
	int* m_mesTable;
	int m_strTableSize;
	int m_mesTableSize;

	int* m_subTable;
	int m_subTableSize;
	int* m_scriptCallTable;
	int m_scriptCallTableSize;
	int m_scriptCallKosuu;

	int* m_commandCallTable;
	int m_commandCallTableSize;
	int m_commandCallKosuu;

	int* m_selectTable;
	int m_selectTableSize;
	int m_selectKosuu;

	BOOL m_errorSkipFlag;
	void PrintMyError(LPSTR text,LPSTR caption = NULL);

	BOOL GetSystemConst(LPSTR name,int* lpVal);

	int m_targetMachine;
	LPSTR iPhoneString(LPSTR str);

	char* m_iPhoneString;

};

#endif

/*_*/


