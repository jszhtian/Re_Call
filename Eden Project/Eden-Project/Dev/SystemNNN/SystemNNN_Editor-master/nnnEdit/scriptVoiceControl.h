//
//
//

#if !defined __NNNEDIT_SCRIPTVOICECONTROL__
#define __NNNEDIT_SCRIPTVOICECONTROL__


class CMyApplication;
class CMyDirectSound;
class CMyDirectSoundBuffer;
class CWaveData;
class CVolumeControl;

class CScriptVoiceControl
{
public:
	CScriptVoiceControl(CMyApplicationBase* app,BOOL voicePack);
	~CScriptVoiceControl();
	void End(void);

	BOOL LoadScriptVoice(LPSTR filename,int channel);
	void StopScriptVoice(int channel);
	void PlayScriptVoice(int channel);

	void SetScriptVoiceVolume(int channel,int vol);
	void SetScriptVoiceTeii(int channel,int x,int y,int z);
	void SetScriptVoiceMoveTime(int channel,int moveTime);
	void SetScriptVoiceTeii2(int channel,int x2,int y2,int z2);
	void SetScriptVoiceDoppler(int channel,BOOL dop);

	void PlayScriptMessageVoice(CMessageData* pMessage,int channel);

	void SetReadType(BOOL packFlag){m_readType = packFlag;}
private:
	CMyApplicationBase* m_app;

	CMyDirectSoundBuffer* m_voice[4];
	CMyDirectSound* m_directSound;
	CWaveData* m_waveData;
	CVolumeControl* m_volumeControl;
	int m_readType;
};


#endif
/*_*/

