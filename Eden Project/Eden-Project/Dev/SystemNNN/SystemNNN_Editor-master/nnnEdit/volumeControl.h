//
//
//

#if !defined __NNNEDIT_VOLUMECONTROL__
#define __NNNEDIT_VOLUMECONTROL__


class CMyApplication;
class CMyInputDialog;
class CMyConfig;

class CVolumeControl
{
public:
//	CVolumeControl(CMyApplication* app);
	CVolumeControl(CMyInputDialog* inputDialog,CMyConfig* config);
	~CVolumeControl();
	void End(void);

	int GetMusicVolume(void){return m_musicVolume;}
	int GetSoundVolume(void){return m_soundVolume;}
	int GetVoiceVolume(void){return m_voiceVolume;}
	int GetMovieVolume(void){return m_movieVolume;}
	int GetSoundVoiceVolume(void){return m_soundVoiceVolume;}


	void SetMusicVolume(void);
	void SetSoundVolume(void);
	void SetVoiceVolume(void);
	void SetMovieVolume(void);
	void SetSoundVoiceVolume(void);

private:
//	CMyApplication* m_app;
	CMyConfig* m_config;
	CMyInputDialog* m_input;

	int m_musicVolume;
	int m_soundVolume;
	int m_voiceVolume;
	int m_movieVolume;
	int m_soundVoiceVolume;

};

#endif
/*_*/

