//
// layerdoc.h
//

#if !defined __NNNEDIT_LAYERDOC__
#define __NNNEDIT_LAYERDOC__

#define LAYER_KOSUU_MAX 32

//#include "mydocument.h"

class CNameList;
class CMyFileOpen;
class CMyInputDialog;
class CSelectDialog;

class CLayerDoc : public CMyDocument
{
public:
	CLayerDoc(CMyApplicationBase* lpApp);
	~CLayerDoc();
	void End(void);

//	void OnCloseButton(void);

//	void ClickWriteButton(int layer);
	void ClickEyeButton(int layer);

	void OnClickPictureButton(int layer);
	void OnClickEffectButton(int layer);

//	int GetWriteStatus(int layer);
	int GetEyeStatus(int layer);

	void ClearMiniPic(int n);
	void PictureToMini(int n, int* src, int x = 640, int y = 480);

	void ChangePictureName(int layer, LPSTR name, CKomaData* pKoma = NULL,BOOL sameZahyoFlag = FALSE);
	void DeletePic(int layer);

	void ChangeEffect(int layer, int eff);

//	int GetNowWriteLayer(void);

	int GetSelectPictureLayer(void);
	int GetSelectEffectLayer(void);

	void OnOpenEffect(int n = -1);
	void OnSaveEffect(int n = -1);
	void OnCopyPreEffect(int n = -1);
	void OnCopyPreEffect2(int n = -1);

	void OnUndoEffect(int n = -1);

	void OnLoadLayerEffect(int layer);
	void OnSaveLayerEffect(int layer);

	BOOL CheckPicExist(int layer,CKomaData* pKoma = NULL);


	BOOL CheckKomaSelected(void);
	void SelectPictureFile(int layer,int cmd,HWND hwnd = NULL);
	void SelectPictureFileByName(int layer,int cmd,HWND hwnd = NULL);
	void SetVarControl(int layer,int cmd,HWND hwnd = NULL);

	int GetPicTypeKosuu(void){return m_picTypeKosuu;}

	void MoveLayer(int fromLayer,int toLayer);
	void CopyLayer(int fromLayer,int toLayer);
	void ExchangeLayer(int fromLayer,int toLayer);

	void MoveEffect(int fromLayer,int toLayer);
	void CopyEffect(int fromLayer,int toLayer);
	void ExchangeEffect(int fromLayer,int toLayer);

	void OnLeftKey(void);
	void OnRightKey(void);
	void OnUpKey(void);
	void OnDownKey(void);

	void OnSpaceKey(void);
	void OnEnterKey(void);

	int GetCopyPreEffectMode(void);
	void SetCopyPreEffectMode(int md){m_copyPreEffectMode = md;}

	int GetVarControlLayerCheckPre(int layer);
	void CheckAndGetUndo(void);


private:
	void SetWriteStatus(int layer,int st);
	void SetEyeStatus(int layer, int st);


	int m_writeStatus[LAYER_KOSUU_MAX];
	int m_eyeStatus[LAYER_KOSUU_MAX];

//	int m_writeLayer;

//	int m_selectPictureLayer;
//	int m_selectEffectLayer;

	CMyFileOpen* m_file;
	CMyFileOpen* m_file2;

	CNameList* m_picTypeList;
	int m_picTypeKosuu;

	BOOL CheckCopyPreEffect(void);
	int m_copyPreEffectMode;
	CMyInputDialog* m_inputDialog;
	CSelectDialog* m_selectDialog;


//	int m_typeNameListKosuu;
};


#endif
/*_*/

