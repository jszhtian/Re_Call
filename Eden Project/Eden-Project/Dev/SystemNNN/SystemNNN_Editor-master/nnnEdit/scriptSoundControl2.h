//
//
//

#if !defined __NNNEDIT_SCRIPTSOUNDCONTROL__
#define __NNNEDIT_SCRIPTSOUNDCONTROL__


class CMyApplication;
class CMyDirectSound;
class CMyDirectSoundBuffer;
class CWaveData;
class CVolumeControl;

class CScriptSoundControl2
{
public:
	CScriptSoundControl2(CMyApplicationBase* app);
	~CScriptSoundControl2();
	void End(void);

	BOOL LoadScriptSound(int nm,int channel);
	void StopScriptSound(int channel);
	void PlayScriptSound(int channel,int loopFlag);
	void PlaySeTest(int n,int channel);

	void SetScriptSoundVolume(int channel,int vol);
	void SetScriptSoundTeii(int channel,int x,int y,int z);
	void SetScriptSoundMoveTime(int channel,int moveTime);
	void SetScriptSoundTeii2(int channel,int x2,int y2,int z2);
	void SetScriptSoundDoppler(int channel,BOOL dop);

	void PlayScriptMessageSound(CMessageData* pMessage,int channel);

private:
	CMyApplicationBase* m_app;
	CNameList* m_seList;

	CMyDirectSoundBuffer* m_sound[8];
	CMyDirectSound* m_directSound;
	CWaveData* m_waveData;
	CVolumeControl* m_volumeControl;
};

#endif
/*_*/


