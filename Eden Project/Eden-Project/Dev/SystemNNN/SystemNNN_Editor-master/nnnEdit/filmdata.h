//
// filmdata.h
//

#if !defined __NNNEDIT_FILMDATA__
#define __NNNEDIT_FILMDATA__


class CKomaData;
class CEffect;
//#include "case.h"

class CCase;
//class CMyApplication;
class CScriptData;
class CUndoMemoryObject;

class CFilmData : public CCase
{
public:
//	CFilmData(CEffect* lpEffect,CMyApplication* app);
	CFilmData(CEffect* lpEffect);
	~CFilmData();
	void End(void);


	void Make64x32KomaPic(int n, int* buffer);

	void MakeMiniPic(void);

	CKomaData* GetKoma(int n);

	int* GetMiniPic(void);

	void Init(LPSTR name = NULL);

	BOOL GetNoClearEffect(void);
	void SetNoClearEffect(BOOL flg);

//	BOOL AddKomaArray(int n, int place = -1);

	//åpè≥Ç‘ÇÒ
	LPSTR GetMyName(void);
	void SetMyName(LPSTR name);
	BOOL Load(FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL Save(FILE* file,CUndoMemoryObject* memory = NULL);
	CCase* NewObject(void);
	void OutputScriptSource(FILE* file);
	int Compile(CScriptData* lpScript);

	static char m_fileHeaderName[16];

	BOOL CheckExistChara(int charaNumber);

	int Pass1(void);

	void SetMiniPic(BOOL flg = TRUE);
	void SetLargePic(BOOL flg = TRUE);
	void SetVeryLargePic(BOOL flg = TRUE);

	BOOL CopyOriginalData(CCase* lpCase);
	int Clean(void);

//	BOOL CheckModify(void);
//	void SetModifyFlag(BOOL flg = TRUE);

	int GetFilmColor(void){return m_filmColor;}
	void SetFilmColor(int col){m_filmColor = col;}

	static void SetKomaOfFilm(int n){m_komaOfFilm = n;}
	static void SetKomaOfFilmType(int n){m_komaOfFilmType = n;}
	static int m_komaOfFilm;
	static int m_komaOfFilmType;

	int GetFilmSpecialTypeTime(void){return m_filmSpecialTypeTime;}
	void SetFilmSpecialTypeTime(int typeTime){m_filmSpecialTypeTime = typeTime;}

	int GetConfigMask(void){return m_configMask;}
	void SetConfigMask(int d){m_configMask = d;}
	int GetSMKomaNumber(void);

	int GetTaikenLevel(void){return m_taikenLevel;}
	void SetTaikenLevel(int level){m_taikenLevel = level;}

	int GetCutinFlag(void){return m_cutinFlag;}
	void SetCutinFlag(int flg = TRUE){m_cutinFlag = flg;}

	BOOL CheckNowCutin(int komaNumber);
	void GetNowPicList(LPSTR* picFileName,int komaNumber);

	int GetSkipToFilmEndEnable(void){return m_skiptofilmend;}
	void SetSkipToFilmEndEnable(int md = 1){m_skiptofilmend = md;}

	int GetRenameLayer(void){return m_renameLayer;}
	void SetRenameLayer(int renameLayer){m_renameLayer = renameLayer;}
private:
//	int m_komaKosuu;
//	int m_komaKosuuMax;
//	CKomaData** m_komaDataArray;
	char m_myname[64];

	CEffect* m_effect;

	BOOL m_clearEffectFlag;
	int* m_miniPic;

	int m_miniPicFlag;
//	BOOL m_modifyFlag;


	int m_filmColor;
	int m_filmSpecialTypeTime;
	int m_configMask;
	int m_taikenLevel;
	int m_cutinFlag;
	int m_skiptofilmend;
	int m_renameLayer;
};


#endif
/*_*/

