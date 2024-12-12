//
//
//

#if !defined __NNNEDIT_FRAMECHANGECONTROL__
#define __NNNEDIT_FRAMECHANGECONTROL__


class CMyApplication;

class CFrameChangeControl
{
public:
	CFrameChangeControl(CMyApplication* app);
	~CFrameChangeControl();
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

private:
	CMyApplication* m_app;

	CMyDirectSoundBuffer* m_voice[4];
	CMyDirectSound* m_directSound;
	CWaveData* m_waveData;
	CVolumeControl* m_volumeControl;
};


#endif
/*_*/

