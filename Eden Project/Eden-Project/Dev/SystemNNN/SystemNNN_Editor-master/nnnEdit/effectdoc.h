//
// effectdoc.h
//

#if !defined __TINYAN_NNNEDIT_EFFECTDOC__
#define __TINYAN_NNNEDIT_EFFECTDOC__

#include "mydocument.h"

class CEffectParam;
class CMyInputDialog;

class CEffectDoc : public CMyDocument
{
public:
	CEffectDoc(CMyApplicationBase* lpApp);
	~CEffectDoc();
	void End(void);

//	void OnCloseButton(void);

	HBITMAP GetEffectMini(void);
	HBITMAP GetMyFont(void);

	int GetEditMode(void);
//	int GetNowSelectLayer(void);

	void ClickSelButton(int n);
	void ClickDeltaButton(int n, int delta,BOOL wheelFlag = FALSE);
	void ClickDeltaButton0(int delta,BOOL wheelFlag = FALSE);

	int GetNowSelectEffectLayer(void);

	CEffectParam* GetEffectParam(int n);
	int GetNowSelectParam(void);

	void ChangeSelectParam(int selectParam);

	void OnEnterKey(void);
	void OnLeftKey(void);
	void OnRightKey(void);
	void OnUpKey(void);
	void OnDownKey(void);

	void OnEscapeKey(void);
private:
	HBITMAP m_hBitmapEffectMini;
	HBITMAP m_hBitmapFont;

	int m_editMode;

	void DeltaRoutine(CKomaData* pKoma,int layer,int n,int delta,BOOL wheelFlag);

	void CenterRoutine(CKomaData* pKoma,int layer,int n);

	void OnUpDownKey(int delta);

	CMyInputDialog* m_inputDialog;

};


#endif
/*_*/

