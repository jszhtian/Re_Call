//
//
//

#if !defined __NNNEDIT_SCRIPTCOMPILER__
#define __NNNEDIT_SCRIPTCOMPILER__


class CMyApplication;
class CMiniCompiler;
class CNameList;
class CTermControl;

class CCutinNameData;

class CScriptCompiler
{
public:
	CScriptCompiler(CMyApplicationBase* app);
	~CScriptCompiler();
	void End(void);

	BOOL StartBuild(void);

	BOOL CompileFilm(FILE* makeJsonFlag = NULL);
	BOOL CompileStory(void);
	int CompileStory2(CStoryData* pStory,int n);
	BOOL CompileCommandData(CCommandData* pCommand,LPSTR storyName = NULL,int n = 0);


	int GetPass2ErrorLine(void){return m_pass2ErrorLine;}
	void SetStartKoma(CKomaData* pKoma,int nextType,int nextWindowMode,FILE* makeJsonFlag = NULL,int tab = 0);

	BOOL BuildSpt(BOOL bMesFlag = TRUE,FILE* makeJsonFlag = NULL);
	void AllBuild(FILE* makeJsonFlag = NULL);
	BOOL KaisekiScenePass1(void);
	BOOL KaisekiScenePass2(int n);
	void SearchAndSetCG(LPSTR filename,int layer,FILE* makeJsonFlag = NULL,int tab = 0);
	void SearchAndSetCGByVar(LPSTR filename,int layer,int varNumber,FILE* makeJsonFlag = NULL,int tab = 0);
	BOOL LoadCGList(void);
	int CheckSameCGList(void);

	static char m_unknownStoryName[];
	static char m_unknownErrorMessage[];
	static char m_unknownErrorCaption[];

	void SetBufferSetCG(BOOL flg = TRUE){m_bufferSetCG = flg;}

	BOOL InitAngou(void);
private:
	CMyApplicationBase* m_app;
	CMiniCompiler* m_miniCompiler;

	CNameList* m_commandList;
	CNameList* m_functionList;
	CNameList* m_varList;
	CNameList* m_projectList;
	int m_compilingStoryNumber;

	int m_storyJumpTable[4096];
	int m_filmJumpTable[4096];
	int m_komaOffsetByFilm[1024];
	int m_komaJumpTable[8192];
	int m_komaTableKosuu;
	int m_cgUseFlag[320][64];	//100 chara 64*32flag max
	void SetUseCg(int pl, int nm);
	BOOL CheckUseCG(int pl, int nm);
	BOOL m_cgDataLoadFlag;

	int m_pass2ErrorLine;
	void SetPass2ErrorLine(int n){m_pass2ErrorLine = n;}

	int SearchFilm(LPSTR filmName, int searchStart = 0,int noCheckNumber = -1);
	int SearchStory(LPSTR storyName, int searchStart = 0,int noCheckNumber = -1);
	int SearchScript(LPSTR sptName, int searchStart = 0,int noCheckNumber = -1);

	HWND m_frameHWND;

	int* m_sceneWork;
	int* m_charaOffsetTable;

	int m_sceneDataSize;
	int m_sceneCharaKosuu;

	BOOL m_kaisekiScenePass1Flag;
	BOOL m_kaisekiScenePass2Flag;

	CNameList* m_sceneDataList;
	BOOL m_sceneDataLoadFlag;

	int m_cgDataListKosuu;
	CNameList* m_cgDataList[100];
	BOOL m_makeCgListFlag;
	LPSTR SkipCGCommand(LPSTR name);
	int m_setcgListKosuu;
//	int m_cgUseFlag[320][64];	//100 chara 64*32flag max
	int m_bitTable[32];

	void PrintCompileCommandError(CCommandData* pCommand,LPSTR storyName,int n,LPSTR errorMessage=NULL,LPSTR errorCaption = NULL);

	int m_rubiBuildFlag;
	int m_cutinBuildFlag;
	CRubiMaker* m_rubiMaker;

	int GetMessageGyo(LPSTR mes);
	int GetMessage1Gyo(int n,LPSTR mes);

	CCutinNameData* m_cutinNameData;
	BOOL CheckTagPicFile(LPSTR tagName,LPSTR* fileList);

	char m_tmpMessage[1024];
	char m_rubiMakedBuffer[1024];

	BOOL m_bufferSetCG;

	int GetBaseEffect(CFilmData* pFilm,int komaPlace,int layer,int* foundKomaPlace = NULL);
	BOOL CheckMeUseEffect(CFilmData* pFilm,int komaPlace,int layer);

	char* m_cgVoiceData;
	char* m_cgListDataOnly;
	BOOL CheckAndSplitCGList(LPSTR name);

	int m_targetMachine;

	CNameList* m_varControlList;
	int* m_varListNumber;

	int m_varControlConfig;
	int m_varControlSetCGMax;

	int m_voiceFlagNumber;
	void AddVoiceFlagFilename(LPSTR filename);
	void ClearVoiceFlag(void);
	int m_voiceFlagWorkMax;
	char* m_voiceFlagFileName;
	int m_termConfig;
	CTermControl* m_termControl;

	void SortAndOutputVoiceFlagFilename(void);

};

#endif
/*_*/

