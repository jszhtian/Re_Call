//
// messagedata.h
//

#if !defined __TINYAN_NNNEDIT_MESSAGEDATA__
#define __TINYAN_NNNEDIT_MESSAGEDATA__

class CCase;
class CUndoMemoryObject;

#define VOICE_SE_FLAG_BIT_STOP 0x0001
#define VOICE_SE_FLAG_BIT_TEII 0x0002
#define VOICE_SE_FLAG_BIT_MOVE 0x0004
#define VOICE_SE_FLAG_BIT_TEII2 0x0008
#define VOICE_SE_FLAG_BIT_DOPPLER 0x0010
#define VOICE_SE_FLAG_BIT_LOOP 0x0020
#define VOICE_SE_FLAG_BIT_VOICECONTINUE 0x0040

#define VOICE_SE_FLAG_BIT_FADE 0x0100
#define VOICE_SE_FLAG_BIT_VOLUMEONLY 0x0200

#define VOICE_SE_FLAG_BIT_SYSTEMSE 0x1000
#define VOICE_SE_FLAG_BIT_VOICECOMPLETE 0x2000
#define VOICE_SE_FLAG_BIT_NOWAITSAMECHARA 0x4000


//
// flag bit
// 
// 0:stop
// 1:teii
// 2:move
// 3:teii2
// 4:doppler
// 5:loop
// 6:voicecontinue
//
// 8:fade flag
// 9:volumeonly
//

class CMessageData : public CCase
{
public:
	CMessageData();
	~CMessageData();
	void End(void);

	LPSTR GetMessageData(void);
	LPSTR GetMessageDataSkipComment(void);
	LPSTR GetMessageDataSkipAllComment(void);
	void SetMessageData(LPSTR mes);
	void SetMessageData(LPSTR mes, int ln);

	int GetMessageMode(void){return m_messageMode;}
	void SetMessageMode(int md){m_messageMode = md;}

	int GetDrawCount(void){return m_drawCount;}
	void SetDrawCount(int cnt){m_drawCount = cnt;}

	void GetDrawCountMessage(char* buf);
	void SetDrawCountMessage(LPSTR mes);


	void SetVoiceFileName(LPSTR name,int voiceChannel = 0);
	void SetSeFileName(LPSTR name,int seChannel = 0);

	void SetVoicePlayerNumber(int n, int voiceChannel = 0);
	void SetVoiceNumber(int n,int voiceChannel = 0);
	int GetVoicePlayerNumber(int voiceChannel = 0);
	int GetVoiceNumber(int voiceChannel = 0);

	void SetSENumber(int n,int seChannel = 0);
	int GetSENumber(int seChannel = 0);

	LPSTR GetVoiceFileName(int voiceChannel = 0);
	LPSTR GetSEFileName(int seChannel = 0);

	void SetVoiceFlag(BOOL flg = TRUE,int voiceChannel = 0);
	void SetSEFlag(BOOL flg = TRUE,int seChannel = 0);
	int GetVoiceFlag(void);
	int GetSEFlag(void);

	BOOL CheckSEFlag(int seChannel);
	BOOL CheckVoiceFlag(int voiceChannel);

	void SetRenameFlag(BOOL b = TRUE);
	BOOL GetRenameFlag(void);

	int GetID(void);
	void SetID(int id);

	int GetMessageColor(void){return m_messageColor;}
	void SetMessageColor(int col){m_messageColor = col;}

	int GetMessageGyo(void);
	int GetMessage1Gyo(int n);

	void SetDefaultDrawCount(int cnt){m_defaultDrawCount = cnt;}


	void SetVoiceStop(int channel = 0,BOOL stopFlag = TRUE);
	BOOL CheckVoiceOff(int channel = 0);

	void SetVoiceContinue(int channel = 0, BOOL continueFlag = TRUE);
	BOOL CheckVoiceContinue(int channel = 0);

	void ChangeVoiceComplete(int channel = 0);
	bool CheckVoiceComplete(int channel = 0);

	void ChangeVoiceNoWaitSameCharaVoice(int channel = 0);
	bool CheckVoiceNoWaitSameCharaVoice(int channel = 0);

	BOOL CheckVoiceVolumeExist(int channel = 0);
	void SetVoiceVolume(int vol,int channel = 0);
	int GetVoiceVolume(int channel = 0);

	BOOL CheckVoiceTeiiExist(int channel = 0);
	void SetVoiceTeii(int xyz,int d,int channel = 0,int cal = 0);
	int GetVoiceTeii(int xyz,int channel = 0,int cal = 0);

	BOOL CheckVoiceMoveFlag(int channel = 0);
	void SetVoiceMoveFlag(BOOL flg = TRUE,int channel = 0);

	BOOL CheckVoiceTeii2Exist(int channel = 0);
	void SetVoiceTeii2(int xyz,int d,int channel = 0);
	int GetVoiceTeii2(int xyz,int channel = 0);

	BOOL CheckVoiceDopplerSoutou(int channel = 0);
	void SetVoiceDopplerSoutou(BOOL flg = TRUE,int channel = 0);

	int GetVoiceMoveTime(int channel = 0);
	void SetVoiceMoveTime(int tm,int channel = 0);

	BOOL CheckVoiceLoop(int channel = 0);
	void SetVoiceLoop(BOOL flg = TRUE,int channel = 0);

	BOOL CheckVoiceEffect(int channel = 0);

	void CheckAllVoiceEffectClear(int channel = 0);


	BOOL CheckSEIsSystem(int channel = 0);
	void SetSEIsSystem(BOOL flg = TRUE, int channel = 0);




	BOOL CheckSeStop(int channel = 0);
	void SetSeStop(BOOL stopFlag = TRUE,int channel = 0);

	BOOL CheckSeVolumeExist(int channel = 0);
	int GetSeVolume(int channel = 0);
	void SetSeVolume(int volume,int channel = 0);


	BOOL CheckSeTeiiExist(int channel = 0);
	void SetSeTeii(int xyz,int d,int channel = 0,int cal = 0);
	int GetSeTeii(int xyz,int channel = 0,int cal = 0);

	BOOL CheckSeMoveFlag(int channel = 0);
	void SetSeMoveFlag(BOOL flg = TRUE,int channel = 0);

	BOOL CheckSeTeii2Exist(int channel = 0);
	void SetSeTeii2(int xyz,int d,int channel = 0);
	int GetSeTeii2(int xyz,int channel = 0);

	BOOL CheckDopplerSoutou(int channel = 0);
	void SetDopplerSoutou(BOOL flg = TRUE,int channel = 0);

	int GetSeMoveTime(int channel = 0);
	void SetSeMoveTime(int tm,int channel = 0);

	BOOL CheckSeLoop(int channel = 0);
	void SetSeLoop(BOOL flg = TRUE,int channel = 0);

	BOOL CheckSeEffect(int channel = 0);

	void CheckAllSeEffectClear(int channel = 0);

	BOOL Replace(int start,int ln, LPSTR mes);
//
	int GetSEFadeInOut(int channel = 0);
	void SetSEFadeInOut(int flg = 1,int channel = 0);

	int GetSEVolumeOnly(int channel = 0);
	void SetSEVolumeOnly(int flg = 1,int channel = 0);

	int GetSEFadeTime(int channel = 0);
	void SetSEFadeTime(int tm,int channel = 0);


	int GetVoiceFadeInOut(int channel = 0);
	void SetVoiceFadeInOut(int flg = 1,int channel = 0);

	int GetVoiceVolumeOnly(int channel = 0);
	void SetVoiceVolumeOnly(int flg = 1,int channel = 0);

	int GetVoiceFadeTime(int channel = 0);
	void SetVoiceFadeTime(int tm,int channel = 0);

	static char m_defaultName[256];
	static void SetDefaultName(char* name);


//Œp³
	LPSTR GetMyName(void);
	void SetMyName(LPSTR name);
	BOOL Load(FILE* file,CUndoMemoryObject* memory = NULL);
	BOOL Save(FILE* file,CUndoMemoryObject* memory = NULL);
	CCase* NewObject(void);

	void Init(LPVOID para = NULL);

	BOOL MakeBook(FILE* file,BOOL bPrintFileNameFlag = TRUE,BOOL enphasisFlag = FALSE,int defNam = 0);

	BOOL CheckExistChara(int charaNumber,int voiceChannel = 0);

	int Pass1(void);

	BOOL CheckVoiceLock(int n = 0);
	void SetVoiceLock(BOOL flg = TRUE,int n = 0);

	static int m_defaultDrawTime;

	BOOL CopyOriginalData(CCase* lpCase);
	int Clean(void);

	int GetVoiceChannel(void){return m_voiceChannel;}
	void SetVoiceChannel(int ch){m_voiceChannel = ch;}
	int GetSeChannel(void){return m_seChannel;}
	void SetSeChannel(int ch){m_seChannel = ch;}

	void SetCannotClick(int md=1){m_cannotClickFlag = md;}
	int GetCannotClick(void){return m_cannotClickFlag;}

	void SetAutoMessageTime(int tm){m_autoMessageTime = tm;}
	int GetAutoMessageTime(void){return m_autoMessageTime;}

	void SetAutoMessageSpeed(int sp){m_autoMessageSpeed = sp;}
	int GetAutoMessageSpeed(void){return m_autoMessageSpeed;}

	void SetExpStatus(int st){m_expStatus = st;}
	int GetExpStatus(void){return m_expStatus;}

	void SetMessageEffect(int st){m_messageEffect = st;}
	int GetMessageEffect(void){return m_messageEffect;}
	void SetMessageEffectTime(int st){m_messageEffectTime = st;}
	int GetMessageEffectTime(void){return m_messageEffectTime;}

	static int m_bitPattern[];

	static char m_separator[];
	static char m_emphasis1[];
	static char m_emphasis2[];
	static char m_emphasis3[];
	static char m_emphasis9[];
	static char m_haraponnobaka[];

	static char m_tmpMessage[];
	static char m_tmpMessage2[];

	static int m_defaultDrawCount;

	void ClearAllSeEffect(int channel = 0);
	void ClearAllVoiceEffect(int channel = 0);

	void OutputScriptSource(FILE* file);

	LPSTR GetTmpMessage(void){return m_tmpMessage;}

	int GetMessageFontSizeType(void){return m_messageFontSizeType;}
	void SetMessageFontSizeType(int type);

	int GetFace(void){return m_face;}
	void SetFace(int face);

	int GetMustAndFixFace(void){return m_mustFace;}
	int GetMustFace(void){return m_mustFace & 0x0000ffff;}
	void SetMustFace(int mustFace);

	int GetFixFace(void){return (m_mustFace >> 16);}
	void SetFixFace(int fixFace);

	int GetSeMode(int channel);
	int GetVoiceMode(int channel);

	int GetMusicVolume(void);
	int GetMusicFadeTime(void);
	void SetMusicVolume(int vol);
	void SetMusicFadeTime(int fadeTime);


	static int m_whiteSpace;
	static char m_whiteSpaceStr[1024];
	static void SetWhiteSpace(int n);

	int CheckIncludeSeimei(int flag);

private:
	int m_messageMode;
	int m_drawCount;

	char* m_messageBuffer;

	int m_voiceCharaNumber;	//-1 nothing 0:unknown 1:chara
	int m_voiceNumber;	//‚Â‚©‚¤‚Ì‚©???
	int m_seNumber[16];		//‚Â‚©‚¤‚Ì‚©???
	int m_voiceFileFlag;
	int m_seFileFlag;
	int m_serialNumber;

	BOOL m_renameFileFlag;
	BOOL m_voiceLockFlag;

	char* m_voiceFileName;
	char* m_seFileName;

	int m_voiceFileNameSize;
	int m_seFileNameSize;

	int m_messageBufferSize;


	int m_voiceControlFlag;
	int m_seControlFlag;

	void CheckAndExpandMessageWork(int ln);
	void CheckAndExpandVoiceWork(int ln);
	void CheckAndExpandSeWork(int ln);

	int* m_seControlData;
	int* m_voiceControlData;

	int m_seControlDataSize;
	int m_voiceControlDataSize;

	int CalcuNeedVoiceFileSize(int flg);
	int CalcuNeedSeFileSize(int flg);

	void CreateVoiceControl(int n = 16);
	void CreateSeControl(int n = 16);

	int m_voiceChannel;
	int m_seChannel;

	int m_messageColor;

	int m_expStatus;
	int m_messageEffect;
	int m_messageEffectTime;

	int GetMaxBit(int d);

	BOOL CheckVoiceControlFlag(int channel);
	BOOL CheckSeControlFlag(int channel);


	int m_kakuchoWorkKosuu;
	int m_cannotClickFlag;

	int m_autoMessageTime;
	int m_autoMessageSpeed;
	int m_messageFontSizeType;

	int m_face;
	int m_mustFace;
	int m_musicFade;

	void WriteWhiteSpace(FILE* file);
	void DefNameReplace(char* org,char* tmp2,int defNum);

};

#endif
/*_*/

