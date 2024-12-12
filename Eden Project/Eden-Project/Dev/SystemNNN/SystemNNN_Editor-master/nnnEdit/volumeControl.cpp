#include <windows.h>
#include <stdio.h>

#include "case.h"
#include "messageData.h"
//#include "myApplication.h"

#include "myConfig.h"
#include "myInputDialog.h"

#include "volumeControl.h"



//CVolumeControl::CVolumeControl(CMyApplication* app)
CVolumeControl::CVolumeControl(CMyInputDialog* inputDialog,CMyConfig* config)
{
//	m_app = app;
//	m_input = m_app->GetInputDialog();
	m_input = inputDialog;
	m_config = config;

	m_soundVolume = m_config->GetValue("soundVolume");
	m_musicVolume = m_config->GetValue("musicVolume");
	m_voiceVolume = m_config->GetValue("voiceVolume");
	m_movieVolume = m_config->GetValue("movieVolume");
	m_soundVoiceVolume = m_config->GetValue("soundVoiceVolume");

//	m_soundVolume = m_app->GetConfig("soundVolume");
//	m_musicVolume = m_app->GetConfig("musicVolume");
//	m_voiceVolume = m_app->GetConfig("voiceVolume");
//	m_movieVolume = m_app->GetConfig("movieVolume");
//	m_soundVoiceVolume = m_app->GetConfig("soundVoiceVolume");
}


CVolumeControl::~CVolumeControl()
{
	End();
}

void CVolumeControl::End(void)
{
}


void CVolumeControl::SetMusicVolume(void)
{
	int volume = m_musicVolume;
	if (m_input->GetNumber(m_musicVolume,&volume))
	{
		if (volume < 1) volume = 1;
		if (volume > 100) volume = 100;

		m_musicVolume = volume;
		m_config->SetValue("musicVolume",m_musicVolume);
	}
}

void CVolumeControl::SetSoundVolume(void)
{
	int volume = m_soundVolume;
	if (m_input->GetNumber(m_soundVolume,&volume))
	{
		if (volume < 1) volume = 1;
		if (volume > 100) volume = 100;

		m_soundVolume = volume;
		m_config->SetValue("soundVolume",m_soundVolume);
	}
}

void CVolumeControl::SetVoiceVolume(void)
{
	int volume = m_voiceVolume;
	if (m_input->GetNumber(m_voiceVolume,&volume))
	{
		if (volume < 1) volume = 1;
		if (volume > 100) volume = 100;

		m_voiceVolume = volume;
		m_config->SetValue("voiceVolume",m_voiceVolume);
	}
}

void CVolumeControl::SetMovieVolume(void)
{
	int volume = m_movieVolume;
	if (m_input->GetNumber(m_movieVolume,&volume))
	{
		if (volume < 1) volume = 1;
		if (volume > 100) volume = 100;

		m_movieVolume = volume;
		m_config->SetValue("movieVolume",m_movieVolume);
	}
}

void CVolumeControl::SetSoundVoiceVolume(void)
{
	int volume = m_soundVoiceVolume;
	if (m_input->GetNumber(m_soundVoiceVolume,&volume))
	{
		if (volume < 1) volume = 1;
		if (volume > 100) volume = 100;

		m_soundVoiceVolume = volume;
		m_config->SetValue("soundVoiceVolume",m_soundVoiceVolume);
	}
}
