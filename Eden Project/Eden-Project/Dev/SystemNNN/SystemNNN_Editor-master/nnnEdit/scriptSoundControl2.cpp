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


#include "scriptSoundControl2.h"


CScriptSoundControl2::CScriptSoundControl2(CMyApplicationBase* app)
{
	m_app = app;
	m_seList = m_app->GetSeList();
	m_directSound = m_app->GetDirectSound();
	m_volumeControl = m_app->GetVolumeControl();

	int i;
	for (i=0;i<4;i++)
	{
		m_sound[i] = new CMyDirectSoundBuffer(m_directSound->GetDirectSound(),FALSE);
	}
	for (i=0;i<4;i++)
	{
		m_sound[i+4] = new CMyDirectSoundBuffer(m_directSound->GetDirectSound(),TRUE);
	}
	m_waveData = new CWaveData();
}


CScriptSoundControl2::~CScriptSoundControl2()
{
	End();
}


void CScriptSoundControl2::End(void)
{
	ENDDELETECLASS(m_waveData);
	for (int i=7;i>=0;i--)
	{
		ENDDELETECLASS(m_sound[i]);
	}
}

BOOL CScriptSoundControl2::LoadScriptSound(int nm,int channel)
{
	LPSTR filename = m_seList->GetName(nm*2);
	if (m_waveData->LoadSystemWave("se",filename) == FALSE) return FALSE;
	
	int stereo = m_waveData->GetChannel();
	int sampleRate = m_waveData->GetSampleRate();
	int bitRate = m_waveData->GetBitRate();
	char* realPtr = (char*)(m_waveData->GetRealDataPtr());
	int realSize = m_waveData->GetRealDataSize();
	m_sound[channel]->SetData(realPtr,realSize,stereo,sampleRate,bitRate);

	return TRUE;
}

void CScriptSoundControl2::StopScriptSound(int channel)
{
	m_sound[channel]->Stop();
}

void CScriptSoundControl2::PlayScriptSound(int channel,BOOL loopFlag)
{
	m_sound[channel]->Play(loopFlag);
}

void CScriptSoundControl2::SetScriptSoundVolume(int channel,int vol)
{
	vol += m_volumeControl->GetSoundVolume();//m_soundVolume;
	if (vol<0) vol = 0;
	if (vol>100) vol = 100;

	m_sound[channel]->SetVolume(vol);
}


void CScriptSoundControl2::SetScriptSoundTeii(int channel,int x,int y,int z)
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

	m_sound[channel]->SetStartTeii(fx,fy,fz);
}

void CScriptSoundControl2::SetScriptSoundMoveTime(int channel,int moveTime)
{
	m_sound[channel]->SetMoveTime(moveTime);
}

void CScriptSoundControl2::SetScriptSoundTeii2(int channel,int x2,int y2,int z2)
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

	m_sound[channel]->SetEndTeii(fx2,fy2,fz2);
}

void CScriptSoundControl2::SetScriptSoundDoppler(int channel,BOOL dop)
{
	m_sound[channel]->SetDoppler(dop);
}

void CScriptSoundControl2::PlaySeTest(int n,int channel)
{
	if (n <= 0) return;

	BOOL f = LoadScriptSound(n,channel);
	if (f)
	{
		int vol = 0;
		SetScriptSoundVolume(channel,vol);
		PlayScriptSound(channel,FALSE);
	}
}

void CScriptSoundControl2::PlayScriptMessageSound(CMessageData* pMessage,int channel)
{
	//‚³‚¢‚¿‚¥‚Á‚­
	int se = pMessage->GetSENumber(channel);
	if (se<0) return;

	StopScriptSound(channel);

	BOOL f = LoadScriptSound(se,channel);
	if (f)
	{
		//effect

		BOOL loopFlag = pMessage->CheckSeLoop(channel);
		
		int vol = pMessage->GetSeVolume(channel);
		SetScriptSoundVolume(channel,vol);

		if (pMessage->CheckSeTeiiExist(channel))
		{
			int x = pMessage->GetSeTeii(0,channel);
			int y = pMessage->GetSeTeii(1,channel);
			int z = pMessage->GetSeTeii(2,channel);
			SetScriptSoundTeii(channel,x,y,z);

			if (pMessage->CheckSeMoveFlag(channel))
			{
				int moveTime = pMessage->GetSeMoveTime(channel);
				SetScriptSoundMoveTime(channel,moveTime);

				int x2 = pMessage->GetSeTeii2(0,channel);
				int y2 = pMessage->GetSeTeii2(1,channel);
				int z2 = pMessage->GetSeTeii2(2,channel);
				SetScriptSoundTeii2(channel,x2,y2,z2);

				BOOL dop = pMessage->CheckDopplerSoutou(channel);
				SetScriptSoundDoppler(channel,dop);
			}
		}
		//
		PlayScriptSound(channel,loopFlag);
	}


}

/*_*/

