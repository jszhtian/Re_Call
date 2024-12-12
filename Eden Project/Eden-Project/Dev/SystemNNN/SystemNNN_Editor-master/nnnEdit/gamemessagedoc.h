//
// gamemessagedoc.h
//

#if !defined __TINYAN_NNNEDIT_GAMEMESSAGEDOC__
#define __TINYAN_NNNEDIT_GAMEMESSAGEDOC__

#include "mydocument.h"

class CMyInputDialog;
class CSelectDialog;


class CGameMessageDoc : public CMyDocument
{
public:
	CGameMessageDoc(CMyApplicationBase* lpApp);
	~CGameMessageDoc();
	void End(void);

//	void OnCloseButton(void);
	HBITMAP GetMessagePic(void);

	int GetMessageKosuu(void);
	int GetNowSelectNumber(void);

	void OnNewMessage(int n = -1);
	void OnSelectMessage(int n,WPARAM wParam);
	void OnDelete(int n = -1);
	void OnWindowOnOff(int n = -1);

	void OnCut(int n = -1);
	void OnCopy(int n = -1);
	void OnPaste(int n = -1);

	BOOL CheckBufferExist(void){return m_messageBufferFlag;}

	void OnClickModeButton(int n);
	void OnClickVoiceButton(int n,int channel = 0);
	void OnClickSeButton(int n,int channel = 0);
	void OnChangeSeNumber(int n,int channel = 0);

	void OnClickCommentTopButton(void);
	void OnClickCommentBottomButton(void);

	void OnClickExpStatusButton(void);
	void OnClickMessageEffectButton(void);
	void OnClickMessageEffectTimeButton(void);


	void OnChangeVoiceFileName(int n,int channel = 0);
	void OnChangeVoicePlayer(int n,int channel = 0);

	void OnVoiceContinue(int n,int channel = 0);
	void OnVoiceComplete(int n, int channel = 0);
	void OnVoiceNoWaitSameCharaVoice(int n, int channel = 0);
	void OnVoiceVolume(int n,int channel = 0);

	void OnVoiceEffect(int n,int channel = 0);

	void OnVoiceLoop(int n,int channel = 0);
	void OnVoiceStop(int n,int channel = 0);

	void OnSeStop(int n,int channel = 0);
	void OnSeVolume(int n,int channel = 0);
	void OnSeEffect(int n,int channel = 0);

	void OnSeLoop(int n,int channel = 0);

	void OnChangeMessageType(int n, int typ);

	void OnDeleteSe(int n,int channel = 0);
	void OnDeleteVoice(int n,int channel = 0);

	void OnChangeID(int n);

	void OnLockVoice(int n,int channel = 0);
	void OnUnlockVoice(int n,int channel = 0);

	void OnChangeMessageColor(int n,int col,BOOL globalFlag = FALSE);


	void OnClickMessageFontSizeTypeButton(void);
	void OnClickFaceButton(void);
	void OnClickMustFaceButton(void);
	void OnClickFixFaceButton(void);

	void OnSeFade(int n,int channel = 0);
	void OnSeVolumeOnly(int n,int channel = 0);
	void OnVoiceFade(int n,int channel = 0);
	void OnVoiceVolumeOnly(int n,int channel = 0);
	void OnSeSystem(int n, int channel = 0);

	void OnClickMusicFadeButton(void);


	CMyInputDialog* m_input;
	CSelectDialog* m_select;
	void ReCalcuScrollPara(void);

	LPSTR GetCharaName(int n);
	LPSTR GetSEName(int n);
	LPSTR GetSEFileName(int n);

	int GetScrollObjectKosuu(int vh);
//	int GetNowSelectNumber(void);

	int GetMessageColor(int n);

//	int GetBlockKosuu(void);

	int GetCommentPrintMode(int n);

	void ScrollToSelect(void);
	static char m_bunkatsu[16][64];

	void PlaySeTest(int n);

	BOOL CheckDeleteKeyOk(void);


	int GetExpStatus(int n = -1);
	int GetMessageFontSizeType(int n = -1);

	CCase* GetNowSelectCaseObject(void);

	void OnSelectNumber(int n);
	void OnDeleteKey(void);
	void OnEnterKey(void);
	void OnInsertKey(void);
	void OnEscapeKey(void);

	int GetEditorMessageGyo(void);
	int GetMessageWidthGuideLine(void);

	int GetFace(int n = -1);
	int GetMustFace(int n = -1);
	int GetFixFace(int n = -1);

	int GetMusicFadeVolume(int n = -1);
	int GetMessageEffect(int n = -1);
	int GetMessageEffectTime(int n = -1);

	void CheckAndGetUndo(CKomaData* pKoma,int start,int end);

	BOOL CheckExistUndo(void);
	BOOL OnUndo(int n = -1);
	void ClearUndo(void);


private:
	HBITMAP m_hBitmapMessage;

	int m_windowGyo;
	int m_fullGyo;

	int m_commentPrintMode[2];

	int m_textLimit;
	int m_textWords;

	char* m_seEffectNameList[16];
	char m_seEffectName[16][64];

	char* m_voiceEffectNameList[16];
	char m_voiceEffectName[16][64];
//	CNameList* m_seEffectList;

	int Bunkatsu(LPSTR mes);
	int m_testSeChannel;

	int m_messageBufferFlag;

};


#endif
/*_*/

