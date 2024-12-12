//
// komadata.h
//

#if !defined __NNNEDIT_KOMADATA__
#define __NNNEDIT_KOMADATA__

class CMessageData;

class CEffect;
class CCase;
class CAllEffectParam;
class CUndoMemoryObject;

class CKomaData : public CCase
{
public:
//	CKomaData(CEffect* lpEffect,CMyApplication* app);
	CKomaData(CEffect* lpEffect);
	~CKomaData();
	void End(void);



	void MakePic(void);
	void Make64x48Pic(int* buffer);
	void Make32x24Pic(int* buffer);

	void Make128x96(int* buffer);
	void Make256x192(int* buffer);


	void SetModifyFlag(BOOL flg = TRUE);
	BOOL CheckModify(void);
	BOOL CheckMiniPic(void);
	BOOL CheckLargePic(void);
	BOOL CheckVeryLargePic(void);

	void SetMiniPic(BOOL flg = TRUE,int* buffer = NULL);
	void SetLargePic(BOOL flg = TRUE);
	void SetVeryLargePic(BOOL flg = TRUE);

	BOOL LoadEffect(FILE* file,int loadType = 3);
	BOOL SaveEffect(FILE* file);

	BOOL LoadLayerEffect(int layer,FILE* file,int loadType = 3);
	BOOL SaveLayerEffect(int layer,FILE* file);

	BOOL LoadEffectByFileName(LPSTR filename,int loadType);
	BOOL LoadLayerEffectByFileName(int layer,LPSTR filename,int loadType);

	void SetSelectParam(int layer,int paramNumber);
	int GetSelectParam(int layer);

	//åpè≥
	LPSTR GetMyName(void);
	void SetMyName(LPSTR name);
	BOOL Load(FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL Save(FILE* file,CUndoMemoryObject* memory = NULL);
	CCase* NewObject(void);
	void OutputScriptSource(FILE* file);

	void Init(LPVOID para = NULL);


	LPSTR GetPicFileName(int n);
	void SetPicFileName(int n, LPSTR name,BOOL sameZahyoFlag = FALSE);


	void SetAllEffect(void);

	void SetEffect(int layer, int eff);

	int GetEffect(int layer);
	int GetEffectParaKosuu(int layer);
	void SetEffectParaKosuu(int layer, int n);
	int GetEffectPara(int layer,int n);

	int GetPictureNumber(int layer);

	void LoadAllPicture(void);
	void LoadPicture(int layer);

	void Print(void);

	void InitEffectDefaultPara(int layer,int n);
	void SetEffectPara(int layer,int n, int d);

	void DeleteEffectPic(int layer);
	BOOL GetEffectFlag(int layer);
	void SetEffectFlag(int layer,BOOL flag = TRUE);
	void DeleteEffect(int layer);

	void SetEffectRect(RECT* lpRect, int layer, int setBit = 0xffffffff);
	void GetEffectRect(RECT* lpRect, int layer, int rectNumber = 0);

	int GetOverrapCount(void);
	void SetOverrapCount(int c);

	int GetOverrapType(void);
	void SetOverrapType(int type);

	int GetWindowOffFlag(void);
	void SetWindowOffFlag(int md = 1);

	BOOL CheckExistChara(int charaNumber);

	int GetSetCGPlayer(void);
	int GetSetCGNumber(void);
	void SetSetCG(int pl, int n);

	int GetBGMNumber(void);
	int GetFrameTime(void){return m_frameTime;}
	void SetFrameTime(int frame){m_frameTime = frame;}
	void SetBGMNumber(int n);

	int GetExMusicParam(int n);
	void SetExMusicParam(int n,int d);

	BOOL CopyOriginalData(CCase* lpCase);
	int Clean(void);

	static CAllEffectParam* m_allEffectParam;

	void MoveLayer(int fromLayer,int toLayer);
	void CopyLayer(int fromLayer,int toLayer);
	void ExchangeLayer(int fromLayer,int toLayer);

	void MoveEffect(int fromLayer,int toLayer);
	void CopyEffect(int fromLayer,int toLayer);
	void ExchangeEffect(int fromLayer,int toLayer);

	static EFFECT m_tmpEffectData;
	static RECT m_taihiRect[2][6];
	static int m_taihiPic[2];
	static char m_taihiFilename[2][256];

	int GetSelectLayer(void){return m_selectLayer;}
	void SetSelectLayer(int layer){m_selectLayer = layer;}
	int GetSelectEffectLayer(void){return m_selectEffectLayer;}
	void SetSelectEffectLayer(int effectLayer){m_selectEffectLayer = effectLayer;}

	static BOOL m_smSaveFlag;

	void SetKomaName(LPSTR name);
	LPSTR GetKomaName(void);

	static int m_frameOfKoma;
	static int m_frameOfKomaType;
	static void SetFrameOfKoma(int n){m_frameOfKoma = n;}
	static void SetFrameOfKomaType(int n){m_frameOfKomaType = n;}
	int GetSMFrameNumber(void);

	int GetDemoFlag(void);
	void SetDemoFlag(int md);

	int GetWindowNumber(void){return m_windowNumber;}
	int GetCursorNumber(void){return m_cursorNumber;}
	int GetMouseNumber(void){return m_mouseNumber;}
	void SetWindowNumber(int n){m_windowNumber = n;}
	void SetCursorNumber(int n){m_cursorNumber = n;}
	void SetMouseNumber(int n){m_mouseNumber = n;}

	int GetAutoMessage(void){return m_autoMessage;}
	int GetCannotClick(void){return m_cannotClick;}
	int GetCannotSkip(void){return m_cannotSkip;}
	void SetAutoMessage(int n){m_autoMessage = n;}
	void SetCannotClick(int n){m_cannotClick = n;}
	void SetCannotSkip(int n){m_cannotSkip = n;}


	int GetOptionOff(void){return m_optionOff;}
	void SetOptionOff(int n){m_optionOff = n;}
	int GetCutinFlag(void){return m_cutinFlag;}
	void SetCutinFlag(int n){m_cutinFlag = n;}

	int GetPreload(void){return m_preload;}
	void SetPreload(int n){m_preload = n;}

	int GetExpStatus(void){return m_expStatus;}
	void SetExpStatus(int n){m_expStatus = n;}

	int GetVarControl(int n){return m_varControl[n];}
	void SetVarControl(int n,int d){m_varControl[n] = d;}

	int CheckVarControl(void);

	int GetVarControlLayer(int layer);
	void SetVarControlLayer(int layer,int d);

	void ReloadAllPic(void);

	static int m_effectAdjustTable[][64];
	static int m_effectAdjustDefault[];

	static int m_layerMax;
private:
//effect

//	BOOL ResizeAray(int n);
//	int m_messageDataKosuuMax;
//	int m_messageDataKosuu;
//	CMessageData** m_messageDataArray;

	CEffect* m_effect;
	EFFECT m_effectData[32];
	int m_paraKosuu[32];
	int m_selectParam[32];

//	int m_exMusicFlag;
	int m_exMusicParam[16];
	int m_overrapCount;
	int m_overrapType;
	char m_picFileName[32][256];
	BOOL m_editFlag;
	BOOL m_miniPicFlag;
	BOOL m_largePicFlag;
	BOOL m_veryLargePicFlag;

	int m_windowOffFlag;

	int m_setCG;
	int m_effectVersion;

	int m_bgmNumber;
	int m_frameTime;
	int* m_largePic;
	int* m_miniPic;
	int* m_veryLargePic;

	void TaihiLayer(int layer,int buf = 0);
	void FuqueLayer(int layer,int buf = 0);
	void AdjustLayerEffect(int layer);

	int m_selectLayer;
	int m_selectEffectLayer;

	void CreateMiniPicBuffer(void);
	void CreateLargePicBuffer(void);
	void CreateVeryLargePicBuffer(void);

	LPSTR m_komaName;
	int m_komaNameFlag;
//	int m_selectParam;

	void CreateKomaNameBuffer(void);

	int m_demoFlag;

	int m_varControlLayer[32];

	int m_windowNumber;
	int m_cursorNumber;
	int m_mouseNumber;

	int m_autoMessage;
	int m_cannotClick;
	int m_cannotSkip;
	int m_optionOff;
	int m_cutinFlag;
	int m_preload;
	int m_expStatus;
	int m_varControl[32];


	void AdjustEffect(int layer);
};


#endif
/*_*/
