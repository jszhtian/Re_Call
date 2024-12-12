#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"

#include "..\..\systemNNN\nyanDirectXLib\myDirectSound.h"
#include "..\..\systemNNN\nyanDirectXLib\myDirectSoundBuffer.h"
#include "..\..\systemNNN\nnnUtilLib\waveData.h"

//#include "..\nnnUtilLib\waveMusic.h"
//#include "..\nnnUtilLib\mmlControl.h"
//#include "..\nnnUtilLib\musicControl.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"


#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "volumeControl.h"

//#include "filmCaseData.h"
//#include "filmData.h"
//#include "komaData.h"
//#include "messageData.h"


#include "scriptVoiceControl2.h"


CScriptVoiceControl2::CScriptVoiceControl2(CMyApplicationBase* app,BOOL voicePack)
{
	m_app = app;
//	m_seList = m_app->GetSeList();
	m_directSound = m_app->GetDirectSound();
	m_volumeControl = m_app->GetVolumeControl();

	SetReadType(voicePack);

	m_voice[0] = new CMyDirectSoundBuffer(m_directSound->GetDirectSound(),FALSE);
	m_voice[1] = new CMyDirectSoundBuffer(m_directSound->GetDirectSound(),TRUE);
	m_voice[2] = new CMyDirectSoundBuffer(m_directSound->GetDirectSound(),FALSE);
	m_voice[3] = new CMyDirectSoundBuffer(m_directSound->GetDirectSound(),TRUE);

	m_waveData = new CWaveData();
}


CScriptVoiceControl2::~CScriptVoiceControl2()
{
	End();
}


void CScriptVoiceControl2::End(void)
{
	ENDDELETECLASS(m_waveData);
	for (int i=3;i>=0;i--)
	{
		ENDDELETECLASS(m_voice[i]);
	}
}




BOOL CScriptVoiceControl2::LoadScriptVoice(LPSTR filename,int channel)
{
	if (m_readType)
	{
		if (m_waveData->LoadPackedWave(filename) == FALSE) return FALSE;
	}
	else
	{
		if (m_waveData->LoadWave(filename) == FALSE) return FALSE;
	}

//	LPSTR filename = m_seList->GetName(nm*2);
//	m_waveData->LoadSystemWave("se",filename);
	int stereo = m_waveData->GetChannel();
	int sampleRate = m_waveData->GetSampleRate();
	int bitRate = m_waveData->GetBitRate();
	char* realPtr = (char*)(m_waveData->GetRealDataPtr());
	int realSize = m_waveData->GetRealDataSize();
	m_voice[channel]->SetData(realPtr,realSize,stereo,sampleRate,bitRate);

	return TRUE;
}


void CScriptVoiceControl2::StopScriptVoice(int channel)
{
	m_voice[channel]->Stop();
}

void CScriptVoiceControl2::PlayScriptVoice(int channel)
{
	m_voice[channel]->Play();
}

void CScriptVoiceControl2::SetScriptVoiceVolume(int channel,int vol)
{
	vol += m_volumeControl->GetVoiceVolume();//m_voiceVolume;
	if (vol<0) vol = 0;
	if (vol>100) vol = 100;

	m_voice[channel]->SetVolume(vol);
}

void CScriptVoiceControl2::SetScriptVoiceTeii(int channel,int x,int y,int z)
{
	if (x>=128) x -= 256;
	if (y>=128) y -= 256;
	if (z>=128) z -= 256;
	float fx = (float)x;
	float fy = (float)y;
	float fz = (float)z;

	fx *= 0.1f;
	fy *= 0.1f;
	fz *= 0.1f;

	m_voice[channel]->SetStartTeii(fx,fy,fz);
}

void CScriptVoiceControl2::SetScriptVoiceMoveTime(int channel,int moveTime)
{
	m_voice[channel]->SetMoveTime(moveTime);
}

void CScriptVoiceControl2::SetScriptVoiceTeii2(int channel,int x2,int y2,int z2)
{
	if (x2>=128) x2 -= 256;
	if (y2>=128) y2 -= 256;
	if (z2>=128) z2 -= 256;
	float fx2 = (float)x2;
	float fy2 = (float)y2;
	float fz2 = (float)z2;

	fx2 *= 0.1f;
	fy2 *= 0.1f;
	fz2 *= 0.1f;

	m_voice[channel]->SetEndTeii(fx2,fy2,fz2);
}

void CScriptVoiceControl2::SetScriptVoiceDoppler(int channel,BOOL dop)
{
	m_voice[channel]->SetDoppler(dop);
}


void CScriptVoiceControl2::PlayScriptMessageVoice(CMessageData* pMessage,int channel)
{
	LPSTR filename = pMessage->GetVoiceFileName(channel);

	StopScriptVoice(channel);

	BOOL f = LoadScriptVoice(filename,channel);
	
	if (f)
	{
		int vol = pMessage->GetVoiceVolume(channel);
		SetScriptVoiceVolume(channel,vol);

		if (pMessage->CheckVoiceTeiiExist(channel))
		{
			int x = pMessage->GetVoiceTeii(0,channel);
			int y = pMessage->GetVoiceTeii(1,channel);
			int z = pMessage->GetVoiceTeii(2,channel);
			SetScriptVoiceTeii(channel,x,y,z);

			if (pMessage->CheckVoiceMoveFlag(channel))
			{
				int moveTime = pMessage->GetVoiceMoveTime(channel);
				SetScriptVoiceMoveTime(channel,moveTime);

				int x2 = pMessage->GetVoiceTeii2(0,channel);
				int y2 = pMessage->GetVoiceTeii2(1,channel);
				int z2 = pMessage->GetVoiceTeii2(2,channel);
				SetScriptVoiceTeii2(channel,x2,y2,z2);

				BOOL dop = pMessage->CheckVoiceDopplerSoutou(channel);
				SetScriptVoiceDoppler(channel,dop);
			}
		}


		//
		PlayScriptVoice(channel);
	}
}
