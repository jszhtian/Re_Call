//
// zahyodoc.h
//

#if !defined __TINYAN_NNNEDIT_ZAHYODOC__
#define __TINYAN_NNNEDIT_ZAHYODOC__

#include "mydocument.h"

class CZahyoDoc : public CMyDocument
{
public:
	CZahyoDoc(CMyApplicationBase* lpApp);
	~CZahyoDoc();
	void End(void);

	void GetZahyoRect(int n, RECT* lpRect);
	void SetZahyoRect(int n, RECT* lpRect);

	int GetNowSelectLayer(void);

	BOOL CheckSwitchFlag(int n);
	void EnableSwitch(int n, BOOL flg = TRUE);
	int GetSelectSwitch(void);
	int GetSwitchKosuu(void);

	void OnClickSwitch(int n);

	void SetSelectSwitch(int n);

	void OnLeftKey(void);
	void OnRightKey(void);
	void OnUpKey(void);
	void OnDownKey(void);
	void OnEnterKey(void);
	void OnSpaceKey(void);
	void OnEscapeKey(void);


	int GetPresetButtonEnable(void);
	POINT GetPresetZahyo(int n);
	void SetPresetZahyo(int n,POINT pt);
	SIZE GetScreenSize(void){return m_screenSize;}

	void UpdateMainWindow(void);

private:
	RECT m_srcFromRect;
	RECT m_srcToRect;
	RECT m_dstFromRect;
	RECT m_dstToRect;

	BOOL m_switchFlag[16];
	int m_nowSelectSwitch;
	int m_switchKosuu;
	SIZE m_screenSize;
	POINT m_preset[7];
};


#endif
/*_*/

