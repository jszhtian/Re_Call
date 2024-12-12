//
// komadata.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanLib\include\areacontrol.h"

#include "..\..\systemNNN\nyanLib\include\myGraphics.h"
#include "..\..\systemNNN\nyanLib\include\allGraphics.h"

#include "..\..\systemNNN\nnnutillib\cutinControl.h"


#include "..\..\systemNNN\nyanEffectLib\effectstruct.h"

#include "case.h"
#include "messageData.h"

//#include "myApplication.h"

#include "messagedata.h"
#include "komadata.h"

#include "..\..\systemNNN\nyanLib\include\picture.h"

#include "..\..\systemNNN\nyanEffectLib\effectList.h"
//#include "..\cyclib\scriptcommand.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"
#include "effectParam.h"
#include "allEffectParam.h"

#include "undoMemoryObject.h"


#include "..\..\systemNNN\nyanLib\include\effect.h"

CAllEffectParam* CKomaData::m_allEffectParam = NULL;

EFFECT CKomaData::m_tmpEffectData;

RECT CKomaData::m_taihiRect[2][6];

int CKomaData::m_taihiPic[2];
char CKomaData::m_taihiFilename[2][256];
BOOL CKomaData::m_smSaveFlag = TRUE;

int CKomaData::m_frameOfKoma = 30;
int CKomaData::m_frameOfKomaType = 1;

int CKomaData::m_layerMax = 16;

int CKomaData::m_effectAdjustTable[][64]=
{
	{29, 0,5, 1,4, 2,7, 3,6, 4,-1, 5,5, 6,0, 7,1, 8,2,9,3, -1},	//suna
	{60, 0,2, 1,0, 2,1, 3,5, 4,-1, 5,2, 6,3, 7,4, -1},//maskwipe
	{50, 0,0, 1,2, 2,3, 3,4, 4,5, 5,6, 6,7, 7,1, 8,0, 9,-1,10,-1,11,-1,12,-1,13,-1,14,-1, -1},//grad
	{71, 0,0,1,1,2,2,3,3, 4,4, 5,4, 6,5, 7,6, 8,7, 9,8,10,9, 11,10, 12,11, 13,12, 14,13, 15,-3,16,-4,17,-5, -1},//hotaru
	{65, 0,0,1,1,2,2,3,3,4,4,5,5,6,6, 7,-1, 8,0, 9,7,10,9,11,10, 12,-1, 13,0, -1},

	{-1,},
};

int CKomaData::m_effectAdjustDefault[]=
{
	0,
	0,100,15,-1,20,0,0,0,0
};

//CKomaData::CKomaData(CEffect* lpEffect,CMyApplication* app) : CCase(app)
CKomaData::CKomaData(CEffect* lpEffect) : CCase()
{
	m_effect = lpEffect;

//	m_messageDataKosuuMax = 16;
//	m_messageDataKosuu = 0;
//	m_messageDataArray = new CMessageData*[m_messageDataKosuuMax];

	m_editFlag = TRUE;
	m_miniPicFlag = FALSE;
	m_largePicFlag = FALSE;
	m_veryLargePicFlag = FALSE;

	m_overrapCount = 0;
	m_overrapType = 0;
	m_windowOffFlag = 0;
	m_komaName = NULL;

	m_effectVersion = 4;
	m_bgmNumber = 0;
	m_frameTime = 0;

	m_windowNumber = 0;
	m_cursorNumber = 0;
	m_mouseNumber = 0;

	m_autoMessage = 0;
	m_cannotClick = 0;
	m_cannotSkip = 0;

	m_optionOff = 0;
	m_cutinFlag = 0;

	m_preload = 0;
	m_expStatus = 0;
	int i;
	for (i=0;i<32;i++)
	{
		m_varControl[i] = 0;
	}

	m_selectEffectLayer = -1;
	m_selectLayer = -1;

	ZeroMemory(m_paraKosuu,sizeof(int)*32);
	ZeroMemory(m_selectParam,sizeof(int)*32);
	m_largePic = NULL;
	m_miniPic = NULL;
	m_veryLargePic = NULL;

	m_demoFlag = 0;

	for (i=0;i<16;i++)
	{
		m_exMusicParam[i] = -1;
	}
	m_exMusicParam[1] = 0;//delta volume
	m_exMusicParam[4] = 0;//direct volume

	for (i=0;i<32;i++)
	{
		m_varControlLayer[i] = 0;
	}

//	m_miniPic = NULL;

	for (i=0;i<32;i++)
	{
		ZeroMemory(&m_effectData[i],sizeof(EFFECT));
		ZeroMemory(&m_picFileName[i][0],256);
		m_effectData[i].flag = FALSE;
		m_effectData[i].command = -1;
		m_effectData[i].pic = -1;
	}
}


CKomaData::~CKomaData()
{
	End();
}

void CKomaData::End(void)
{
	DELETEARRAY(m_komaName);
	DELETEARRAY(m_miniPic);
	DELETEARRAY(m_largePic);
	DELETEARRAY(m_veryLargePic);

//	for (int i=0;i<m_messageDataKosuuMax;i++)
//	{
//		ENDDELETECLASS(m_messageDataArray[i]);
//	}
//	DELETEARRAY(m_messageDataArray);
}


void CKomaData::Make256x192(int* buffer)
{
//	if ((m_veryLargePicFlag == TRUE) && (m_veryLargePic != NULL))
//	{
//		memcpy(buffer,m_veryLargePic,sizeof(int)*128*96);
//		return;
//	}
//	CreateVeryLargePicBuffer();

	MakePic();

//	int* ptr0 = (int*)CPicture::m_lpScreenBuffer;
	int* ptr0 = CMyGraphics::GetScreenBuffer();

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	int mlx = screenSizeX / 256;
	int mly = screenSizeY / 192;

	int dv = mlx * mly;

	for (int y=0;y<192;y++)
	{
		for (int x=0;x<256;x++)
		{
			int r = 0;
			int g = 0;
			int b = 0;

			int* ptr1 = ptr0 + x * mlx + y * mly * screenSizeX;

			for (int j=0;j<mly;j++)
			{
				int* ptr = ptr1;

				for (int i=0;i<mlx;i++)
				{
					int rgb = *ptr;
					ptr++;

					int rr = (rgb >> 16) & 0xff;
					int gg = (rgb >> 8 ) & 0xff;
					int bb = (rgb      ) & 0xff;

					r += rr;
					g += gg;
					b += bb;
				}
				ptr1 += screenSizeX;
			}

			r /= dv;
			g /= dv;
			b /= dv;

			int d = (r << 16) | (g << 8) | b;
			*buffer = d;


			buffer++;
		}
	}
//	m_veryLargePicFlag = TRUE;
}

void CKomaData::Make128x96(int* buffer)
{
//	if ((m_veryLargePicFlag == TRUE) && (m_veryLargePic != NULL))
//	{
//		memcpy(buffer,m_veryLargePic,sizeof(int)*128*96);
//		return;
//	}
//	CreateVeryLargePicBuffer();

	MakePic();

//	int* ptr0 = (int*)CPicture::m_lpScreenBuffer;
	int* ptr0 = CMyGraphics::GetScreenBuffer();

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	int mlx = screenSizeX / 128;
	int mly = screenSizeY / 96;

	int dv = mlx * mly;

	for (int y=0;y<96;y++)
	{
		for (int x=0;x<128;x++)
		{
			int r = 0;
			int g = 0;
			int b = 0;

			int* ptr1 = ptr0 + x * mlx + y * mly * screenSizeX;

			for (int j=0;j<mly;j++)
			{
				int* ptr = ptr1;

				for (int i=0;i<mlx;i++)
				{
					int rgb = *ptr;
					ptr++;

					int rr = (rgb >> 16) & 0xff;
					int gg = (rgb >> 8 ) & 0xff;
					int bb = (rgb      ) & 0xff;

					r += rr;
					g += gg;
					b += bb;
				}
				ptr1 += screenSizeX;
			}

			r /= dv;
			g /= dv;
			b /= dv;

			int d = (r << 16) | (g << 8) | b;
			*buffer = d;


			buffer++;
		}
	}
//	m_veryLargePicFlag = TRUE;
}

void CKomaData::Make64x48Pic(int* buffer)
{
	if ((m_largePicFlag == TRUE) && (m_largePic != NULL))
	{
		memcpy(buffer,m_largePic,sizeof(int)*64*48);
		return;
	}

	CreateLargePicBuffer();

	MakePic();

	int* ptr0 = CMyGraphics::GetScreenBuffer();

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

//	int* ptr0 = (int*)CPicture::m_lpScreenBuffer;
	int* buffer00 = buffer;

//	int screenSizeX = CPicture::m_screenSizeX;
//	int screenSizeY = CPicture::m_screenSizeY;
	int mlx = screenSizeX / 64;
	int mly = screenSizeY / 48;
	int dv = mlx * mly;

	for (int y=0;y<48;y++)
	{
		for (int x=0;x<64;x++)
		{
			int r = 0;
			int g = 0;
			int b = 0;

			int* ptr1 = ptr0 + x * mlx + y * mly * screenSizeX;

			for (int j=0;j<mly;j++)
			{
				int* ptr = ptr1;

				for (int i=0;i<mlx;i++)
				{
					int rgb = *ptr;
					ptr++;

					int rr = (rgb >> 16) & 0xff;
					int gg = (rgb >> 8 ) & 0xff;
					int bb = (rgb      ) & 0xff;

					r += rr;
					g += gg;
					b += bb;
				}
				ptr1 += screenSizeX;
			}

			r /= dv;
			g /= dv;
			b /= dv;

			int d = (r << 16) | (g << 8) | b;
			*buffer = d;


			buffer++;
		}
	}
	m_largePicFlag = TRUE;
	if (m_largePic != NULL)
	{
		memcpy(m_largePic,buffer00,sizeof(int)*64*48);
	}
}


void CKomaData::Make32x24Pic(int* buffer)
{
	if ((m_miniPicFlag == TRUE) && (m_miniPic != NULL))
	{
		memcpy(buffer,m_miniPic,sizeof(int)*32*24);
		return;
	}

	CreateMiniPicBuffer();
	MakePic();

	int* ptr0 = CMyGraphics::GetScreenBuffer();

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

//	int* ptr0 = (int*)CPicture::m_lpScreenBuffer;
	int* buffer00 = buffer;

//	int screenSizeX = CPicture::m_screenSizeX;
//	int screenSizeY = CPicture::m_screenSizeY;
	int mlx = screenSizeX / 32;
	int mly = screenSizeY / 24;
	int dv = mlx * mly;

	for (int y=0;y<24;y++)
	{
		for (int x=0;x<32;x++)
		{
			int r = 0;
			int g = 0;
			int b = 0;

			int* ptr1 = ptr0 + x * mlx + y * mly * screenSizeX;

			for (int j=0;j<mly;j++)
			{
				int* ptr = ptr1;

				for (int i=0;i<mlx;i++)
				{
					int rgb = *ptr;
					ptr++;

					int rr = (rgb >> 16) & 0xff;
					int gg = (rgb >> 8 ) & 0xff;
					int bb = (rgb      ) & 0xff;

					r += rr;
					g += gg;
					b += bb;
				}
				ptr1 += screenSizeX;
			}

			r /= dv;
			g /= dv;
			b /= dv;

			int d = (r << 16) | (g << 8) | b;
			*buffer = d;


			buffer++;
		}
	}


	m_miniPicFlag = TRUE;
	if (m_miniPic != NULL)
	{
		memcpy(m_miniPic,buffer00,sizeof(int)*32*24);
	}
}


void CKomaData::MakePic(void)
{
	//clear back
//	CPicture::FillScreen();
	CAllGraphics::FillScreen();


	SetAllEffect();
	LoadAllPicture();

	CAreaControl::SetNextAllPrint();
	CAreaControl::StartScene();


	int loops = GetSMFrameNumber();

	int kosuu = GetObjectKosuu();
	if (kosuu>0)
	{
		CMessageData* pMessage = (CMessageData*)GetObjectData(0);
		if (pMessage->GetMessageMode() == 3)
		{
//			loops = pMessage->GetDrawCount() / 2;
//			if (loops<1) loops = 1;
		}
	}
//loops = 1;

	for (int i=0;i<loops;i++)
	{
//		m_effect->CalcuZahyo();
	}
	
	if (loops > 0)
	{
		m_effect->CountUp(loops);
	}
	m_effect->CalcuOnly();


//OutputDebugString("[*]");


	m_effect->Print();
	m_effect->Print(TRUE);

	CAreaControl::EndScene();
}




void CKomaData::SetModifyFlag(BOOL flg)
{
	m_editFlag = flg;
	if (flg)
	{
		m_miniPicFlag = FALSE;
		m_largePicFlag = FALSE;
		m_veryLargePicFlag = FALSE;
	}
}


BOOL CKomaData::CheckModify(void)
{
	return m_editFlag;
}

BOOL CKomaData::CheckMiniPic(void)
{
	return m_miniPicFlag;
}

BOOL CKomaData::CheckLargePic(void)
{
	return m_largePicFlag;
}

BOOL CKomaData::CheckVeryLargePic(void)
{
	return m_veryLargePicFlag;
}

void CKomaData::SetMiniPic(BOOL flg,int* buffer)
{
	m_miniPicFlag = flg;
	if (flg)
	{
		if (m_miniPic == NULL)
		{
			m_miniPic = new int[32*24];
		}
		if (m_miniPic != NULL)
		{
			if (buffer != NULL)
			{
				memcpy(m_miniPic,buffer,sizeof(int)*32*24);
			}
			else
			{
				ZeroMemory(m_miniPic,sizeof(int)*32*24);
			}
		}
	}
}

void CKomaData::SetLargePic(BOOL flg)
{
	m_largePicFlag = flg;
}

void CKomaData::SetVeryLargePic(BOOL flg)
{
	m_veryLargePicFlag = flg;
}

BOOL CKomaData::LoadEffect(FILE* file,int loadType)
{
//	char head[16];
//	fread(head,sizeof(char),16,file);



	char check[4];
	fread(check,sizeof(char),4,file);
	BOOL oldVersion = TRUE;

	int version = 0;

	if ((check[0] == 'V') && (check[1] == 'e') && (check[2] == 'r'))
	{
		oldVersion = FALSE;
		version = check[3];
		version -= '0';
	}

	int layerMax = 16;
	if (version == 4)
	{
		layerMax = 32;
	}

	if (loadType & 2)
	{
		if (oldVersion)
		{
			char* ptr = (char*)&m_effectData[0];
			memcpy(ptr,check,4);

			ptr += 4;

			TaihiLayer(0);

			ZeroMemory(&m_effectData[0],sizeof(EFFECT));
			fread(ptr,sizeof(OLDEFFECT) - 4,1,file);

			if ((loadType & 1) == 0)
			{
				FuqueLayer(0);
			}

			for (int i=1;i<16;i++)
			{
				TaihiLayer(i);

				ZeroMemory(&m_effectData[i],sizeof(EFFECT));
				fread(&m_effectData[i],sizeof(OLDEFFECT),1,file);

				if ((loadType & 1) == 0)
				{
					FuqueLayer(i);
				}

			}
		}
		else
		{
			for (int i=0;i<layerMax;i++)
			{
				TaihiLayer(i);

				fread(&m_effectData[i],sizeof(EFFECT),1,file);

				if ((loadType & 1) == 0)
				{
					FuqueLayer(i);
				}
			}
			for (int i=layerMax;i<32;i++)
			{
				ZeroMemory(&m_effectData[i],sizeof(EFFECT));
				//ZeroMemory(&m_picFileName[i][0],256);
				m_effectData[i].flag = FALSE;
				m_effectData[i].command = -1;
				m_effectData[i].pic = -1;

			}
		}

		fread(m_paraKosuu,sizeof(int),layerMax,file);
		for (int i=layerMax;i<32;i++)
		{
			m_paraKosuu[i] = 0;
		}

		if (version >= 3)
		{
			fread(m_selectParam,sizeof(int),layerMax,file);
			for (int i=layerMax;i<32;i++)
			{
				m_selectParam[i] = 0;
			}

		}
	}
	else
	{
		if (oldVersion)
		{
			fseek(file,sizeof(OLDEFFECT) - 4,SEEK_CUR);

			for (int i=1;i<16;i++)
			{
				fseek(file,sizeof(OLDEFFECT),SEEK_CUR);
			}
		}
		else
		{
			for (int i=0;i<layerMax;i++)
			{
				fseek(file,sizeof(EFFECT),SEEK_CUR);
			}
		}

		fseek(file,sizeof(int)*layerMax,SEEK_CUR);

		if (version >= 3)
		{
			fseek(file,sizeof(int)*layerMax,SEEK_CUR);
		}
	}



	if (loadType & 1)
	{
		for (int i=0;i<layerMax;i++)
		{
			if (version <= 1)
			{
				fread(&m_picFileName[i][0],sizeof(char),16,file);
			}
			else
			{
				fread(&m_picFileName[i][0],sizeof(char),256,file);
			}
		}
		for (int i=layerMax;i<32;i++)
		{
			m_picFileName[i][0] = 0;
		}
		//load pic


		for (int i=0;i<32;i++)
		{

			if ((m_effectData[i].pic == -1) && (m_picFileName[i][0] != 0))
			{
				m_effectData[i].pic = i;
			}

			AdjustLayerEffect(i);
			LoadPicture(i);
		}
	}
	else
	{
		for (int i=0;i<layerMax;i++)
		{
			if (version <= 1)
			{
				fseek(file,sizeof(char)*16,SEEK_CUR);
			}
			else
			{
				fseek(file,sizeof(char)*256,SEEK_CUR);
			}
			AdjustLayerEffect(i);
		}
	}

	//modify

	SetModifyFlag();

	return TRUE;
}



BOOL CKomaData::LoadLayerEffect(int layer,FILE* file,int loadType)
{
	char check[4];
	fread(check,sizeof(char),4,file);

	int version = 3;

	if ((check[0] == 'V') && (check[1] == 'e') && (check[2] == 'r'))
	{
		version = check[3];
		version -= '0';
	}


	if (loadType & 2)
	{
		TaihiLayer(layer);

		fread(&m_effectData[layer],sizeof(EFFECT),1,file);
		fread(&m_paraKosuu[layer],sizeof(int),1,file);
		if (version >= 3)
		{
			fread(&m_selectParam[layer],sizeof(int),1,file);
		}

		if ((loadType & 1) == 0)
		{
			FuqueLayer(layer);
		}
	}
	else
	{
		fseek(file,sizeof(EFFECT),SEEK_CUR);
		int pkosuu = 0;

		fread(&pkosuu,sizeof(int),1,file);
		if (version >= 3)
		{
			fseek(file,sizeof(int),SEEK_CUR);
		}
	}

	if (loadType & 1)
	{
		fread(&m_picFileName[layer][0],sizeof(char),256,file);

		if ((m_effectData[layer].pic == -1) && (m_picFileName[layer][0] != 0))
		{
			m_effectData[layer].pic = layer;
		}

	}
	else
	{
		fseek(file,sizeof(char)*256,SEEK_CUR);
	}

	AdjustLayerEffect(layer);


	if (loadType & 1)
	{
		LoadPicture(layer);
	}

	SetModifyFlag();

	return TRUE;
}


BOOL CKomaData::LoadEffectByFileName(LPSTR filename,int loadType)
{
	char fullname[1024];
	wsprintf(fullname,"nnndir\\effect\\%s.eff",filename);

	FILE* file = NULL;
	fopen_s(&file,fullname,"rb");
	if (file == NULL)
	{
		return FALSE;
	}

	LoadEffect(file,loadType);
	fclose(file);

	return TRUE;
}

BOOL CKomaData::LoadLayerEffectByFileName(int layer,LPSTR filename,int loadType)
{
	char fullname[1024];
	wsprintf(fullname,"nnndir\\effect\\%s.lef",filename);

	FILE* file = NULL;
	fopen_s(&file,fullname,"rb");
	if (file == NULL)
	{
		return FALSE;
	}

	LoadLayerEffect(layer,file,loadType);
	fclose(file);
	return TRUE;
}


BOOL CKomaData::SaveLayerEffect(int layer,FILE* file)
{
	char vers[5] = "Ver3";
	fwrite(vers,sizeof(char),4,file);	//new version
	fwrite(&m_effectData[layer],sizeof(EFFECT),1,file);
	fwrite(&m_paraKosuu[layer],sizeof(int),1,file);
	fwrite(&m_selectParam[layer],sizeof(int),1,file);
	fwrite(&m_picFileName[layer][0],sizeof(char),256,file);
	return TRUE;
}


BOOL CKomaData::SaveEffect(FILE* file)
{
//	char head[16];
//	fread(head,sizeof(char),16,file);
//	fwrite(&m_effectData[0],sizeof(EFFECT),1,file);

	char vers[5] = "Ver3";
	if (m_layerMax == 32)
	{
		vers[3] = '4';
	}

	fwrite(vers,sizeof(char),4,file);	//new version

	int i;
	for (i=0;i<m_layerMax;i++)
	{
		fwrite(&m_effectData[i],sizeof(EFFECT),1,file);
	}

	fwrite(m_paraKosuu,sizeof(int),m_layerMax,file);
	fwrite(m_selectParam,sizeof(int),m_layerMax,file);

	for (i=0;i<m_layerMax;i++)
	{
		fwrite(&m_picFileName[i][0],sizeof(char),256,file);
	}

	return TRUE;
}



BOOL CKomaData::Load(FILE* file,CUndoMemoryObject* memory)
{
	char head[16];
//	fread(head,sizeof(char),16,file);
	CaseRead(head,sizeof(char),16,file,memory);
	int tmp[16];
	int tmp2[16];
	int tmp3[32];

//	fread(tmp,sizeof(int),16,file);
	CaseRead(tmp,sizeof(int),16,file,memory);

	if (tmp[13] & 2)
	{
//		fread(tmp2,sizeof(int),16,file);
		CaseRead(tmp2,sizeof(int),16,file,memory);
	}
	else
	{
		for (int i=0;i<16;i++)
		{
			tmp2[i] = 0;
		}
	}

	int layerMax = 16;
	if (tmp[13] & 4)
	{
		layerMax = 32;
	}

	m_overrapCount = tmp[1];
	m_overrapType = tmp[2];
	m_windowOffFlag = tmp[3];
	m_setCG = tmp[4];

	m_effectVersion = tmp[5];
	m_bgmNumber = tmp[6];

	m_frameTime = tmp[7];

	m_selectLayer = tmp[8] - 1;
	m_selectEffectLayer = tmp[9] - 1;

	m_demoFlag = tmp[12] & 3;

	m_windowNumber = tmp2[0];
	m_cursorNumber = tmp2[1];
	m_mouseNumber = tmp2[2];

	m_autoMessage = tmp2[3];
	m_cannotClick = tmp2[4];
	m_cannotSkip = tmp2[5];

	m_optionOff = tmp2[6];
	m_cutinFlag = tmp2[7];
	m_preload = tmp2[8];
	m_expStatus = tmp2[9];

	//変数制御 10bit変数番号 22bit数値 (1bit符号)
	
	if (tmp2[10])
	{
//		fread(m_varControl,sizeof(int),layerMax,file);
		CaseRead(m_varControl,sizeof(int),layerMax,file,memory);
	}
	else
	{
		for (int i=0;i<32;i++)
		{
			m_varControl[i] = 0;
		}
	}
//	m_varControl = tmp2[10];




//tmp2[11]=1;
	for (int i=0;i<32;i++)
	{
		tmp3[i] = 0;
	}
	if (tmp2[11] != 0)
	{
//		fread(tmp3,sizeof(int),layerMax,file);
		CaseRead(tmp3,sizeof(int),layerMax,file,memory);
	}


	for (int i=0;i<32;i++)
	{
		m_varControlLayer[i] = tmp3[i];
	}





//	fread(m_paraKosuu,sizeof(int),layerMax,file);
	CaseRead(m_paraKosuu,sizeof(int),layerMax,file,memory);
	for (int i=layerMax;i<32;i++)
	{
		m_paraKosuu[i] = 0;
	}

	if (m_effectVersion >= 3)
	{
//		fread(m_selectParam,sizeof(int),layerMax,file);
		CaseRead(m_selectParam,sizeof(int),layerMax,file,memory);
	}
	for (int i=layerMax;i<32;i++)
	{
		m_selectParam[i] = 0;
	}


	//ひつようならnew array
//	if (tmp[0]>m_objectKosuuMax)
//	{
//		AddArray(tmp[0] - m_objectKosuuMax);
//	}
//	m_objectKosuu = tmp[0];

	int bt = 0x1;
	for (int i=0;i<layerMax;i++)
	{
		if (tmp[15] & bt)
		{
			if (m_effectVersion == 0)
			{
				ZeroMemory(&m_effectData[i],sizeof(EFFECT));
//				fread(&m_effectData[i],sizeof(OLDEFFECT),1,file);
				CaseRead(&m_effectData[i],sizeof(OLDEFFECT),1,file,memory);
			}
			else
			{
//				fread(&m_effectData[i],sizeof(EFFECT),1,file);
				CaseRead(&m_effectData[i],sizeof(EFFECT),1,file,memory);
			}

			if (m_effectVersion < 3)
			{
				AdjustEffect(i);
			}
		}
		else
		{
			ZeroMemory(&m_effectData[i],sizeof(EFFECT));
			m_effectData[i].pic = -1;
		}
		bt <<= 1;
	}
	for (int i=layerMax;i<32;i++)
	{
		ZeroMemory(&m_effectData[i],sizeof(EFFECT));
		m_effectData[i].pic = -1;
	}




	int bt2 = 0x1;
	for (int i=0;i<layerMax;i++)
	{
		if (tmp[14] & bt2)
		{
			if (m_effectVersion <= 1)
			{
//				fread(&m_picFileName[i][0],sizeof(char),16,file);
				CaseRead(&m_picFileName[i][0],sizeof(char),16,file,memory);
			}
			else
			{
//				fread(&m_picFileName[i][0],sizeof(char),256,file);
				CaseRead(&m_picFileName[i][0],sizeof(char),256,file,memory);
			}
		}
		else
		{
			m_picFileName[i][0] = 0;
		}
		bt2 <<= 1;
	}
	for (int i=layerMax;i<32;i++)
	{
		m_picFileName[i][0] = 0;
	}

	//patch bad nop
	for (int i=0;i<32;i++)
	{
		if (m_effectData[i].flag)
		{
			if (m_effectData[i].pic != -1)
			{
				if (m_effectData[i].command == EFFECT_NOP)
				{
					m_paraKosuu[i] = 0;
				}
			}
		}
	}


	m_miniPicFlag = FALSE;
	m_largePicFlag = FALSE;
	m_veryLargePicFlag = FALSE;

	//miniPic
	if (tmp[10] & 0x1)
	{
		m_miniPicFlag = TRUE;
		CreateMiniPicBuffer();
//		fread(m_miniPic,sizeof(int),32*24,file);
		CaseRead(m_miniPic,sizeof(int),32*24,file,memory);
	}

	//largePic
	if (tmp[10] & 0x2)
	{
		m_largePicFlag = TRUE;
		CreateLargePicBuffer();
//		fread(m_largePic,sizeof(int),64*48,file);
		CaseRead(m_largePic,sizeof(int),64*48,file,memory);
	}

	if (tmp[10] & 0x4)
	{
		m_veryLargePicFlag = TRUE;
		CreateVeryLargePicBuffer();
//		fread(m_veryLargePic,sizeof(int),128*96,file);
		CaseRead(m_veryLargePic,sizeof(int),128*96,file,memory);
	}


	m_komaNameFlag = tmp[11];
	if (m_komaNameFlag)
	{
		CreateKomaNameBuffer();
//		fread(m_komaName,sizeof(char),64,file);
		CaseRead(m_komaName,sizeof(char),64,file,memory);
	}


	if (tmp[13] & 1)//拡張曲設定
	{
//		fread(m_exMusicParam,sizeof(int),16,file);
		CaseRead(m_exMusicParam,sizeof(int),16,file,memory);
	}
	else
	{
		for (int i=0;i<16;i++)
		{
			m_exMusicParam[i] = -1;
		}
		m_exMusicParam[1] = 0;//delta volume
		m_exMusicParam[4] = 0;//direct volume

		if (m_bgmNumber >= 0)
		{
			int mcmd = m_bgmNumber & 0x30000;
			if (mcmd)
			{
				m_bgmNumber &= (~0x30000);

				if (mcmd == 0x10000)
				{
					m_exMusicParam[0] = 0;	//loop
				}
				else if (mcmd == 0x20000)
				{
					//1kai
				}
				else if (mcmd == 0x30000)
				{
					//ていし
					m_bgmNumber = -1;
				}


				if (mcmd != 0x30000)
				{
					m_bgmNumber++;
				}

			}
		}
	}


//	m_selectParam = tmp[12];



//	LoadArrayObject(m_objectKosuu,file);
	LoadArrayObject(tmp[0],file,memory);






/*
	for (i=0;i<m_objectKosuu;i++)
	{
		CMessageData* lpMes = (CMessageData*)m_objectDataArray[i];
		if (lpMes == NULL)
		{
			lpMes = new CMessageData();
			m_objectDataArray[i] = lpMes;
		}

		else
		{
			lpMes->Load(file);
		}
	}
*/

	m_effectVersion = 4;

	SetModifyFlag(FALSE);


	return TRUE;
}

void CKomaData::AdjustEffect(int layer)
{
	EFFECT* lpEffect = &m_effectData[layer];

	int flag = lpEffect->flag;
	if (flag)
	{
		int eff = lpEffect->command;
		if (eff != -1)
		{
			int found = -1;
			for (int n=0;n<256;n++)
			{
				int chk = m_effectAdjustTable[n][0];
				if (chk == -1) break;
				if (eff == chk)
				{
					found = n;
					break;
				}
			}


			if (found != -1)
			{
				int para[256];
				for (int k=0;k<256;k++)
				{
					para[k] = lpEffect->para[k];
				}

				int lastNew = -1;

				for (int i=0;i<30;i++)
				{
					int newLayer = m_effectAdjustTable[found][1+i*2];
					int oldLayer = m_effectAdjustTable[found][1+i*2+1];
					if (newLayer == -1) break;

					lastNew = newLayer;

					if (oldLayer >= 0)
					{
						if (oldLayer < m_paraKosuu[layer])
						{
							para[newLayer] = lpEffect->para[oldLayer];
						}
						else
						{
							para[newLayer] = 0;
						}
					}
					else
					{
						//set default
						para[newLayer] = m_effectAdjustDefault[-oldLayer];
					}
				}

				for (int m=0;m<256;m++)
				{
					lpEffect->para[m] = para[m];
				}

				if (lastNew != -1)
				{
					int kosuu = lastNew+1;

					if (kosuu>m_paraKosuu[layer])
					{
						m_paraKosuu[layer] = kosuu;
					}
				}
			}
		}
	}
}




BOOL CKomaData::Save(FILE* file,CUndoMemoryObject* memory)
{
//	fwrite("-KOMADATA      ",sizeof(char),16,file);
	CaseWrite("-KOMADATA      ",sizeof(char),16,file,memory);

	int tmp[16];
	int tmp2[16];
	int tmp3[32];

	
	int i;
	for (i=0;i<16;i++) tmp[i] = 0;
	for (i=0;i<16;i++) tmp2[i] = 0;
	tmp[0] = m_objectKosuu;
	tmp[1] = m_overrapCount;
	tmp[2] = m_overrapType;
	tmp[3] = m_windowOffFlag;
	tmp[4] = m_setCG;
	tmp[5] = 3;				//effect version
	tmp[6] = m_bgmNumber;
	tmp[7] = m_frameTime;

	tmp[8] = m_selectLayer + 1;
	tmp[9] = m_selectEffectLayer + 1;

	if (m_smSaveFlag)
	{
		if (m_miniPicFlag)
		{
			tmp[10] |= 0x1;
		}

		if (m_largePicFlag)
		{
			tmp[10] |= 0x2;
		}

		if (m_veryLargePicFlag)
		{
///			tmp[10] |= 0x4;
		}
	}

	tmp[11] = m_komaNameFlag;
//	tmp[12] = m_selectParam;

	tmp[12] |= (m_demoFlag & 3);

	tmp[13] |= 1;

	tmp[13] |= 2;	//かくちょう

	if (m_layerMax == 32)
	{
		tmp[13] |= 4;	//32LAYER
	}

	int bt = 0x1;
	int bt2 = 0x1;

	for (i=0;i<m_layerMax;i++)
	{
		if (m_effectData[i].flag)
		{
			tmp[15] |= bt;
			if (m_effectData[i].pic != -1) tmp[14] |= bt2;
		}

		bt <<= 1;
		bt2 <<= 1;
	}

	tmp2[0] = m_windowNumber;
	tmp2[1] = m_cursorNumber;
	tmp2[2] = m_mouseNumber;

	tmp2[3] = m_autoMessage;
	tmp2[4] = m_cannotClick;
	tmp2[5] = m_cannotSkip;
	tmp2[6] = m_optionOff;

	tmp2[7] = m_cutinFlag;
	tmp2[8] = m_preload;
	tmp2[9] = m_expStatus;


	tmp2[10] = CheckVarControl();
//	tmp2[10] = m_varControl;


	tmp2[11] = 0;

	for (int i=0;i<m_layerMax;i++)
	{
		tmp3[i] = m_varControlLayer[i];
		if (tmp3[i] != 0)
		{
			tmp2[11] = 1;
		}
	}

//	fwrite(tmp,sizeof(int),16,file);
//	fwrite(tmp2,sizeof(int),16,file);
	CaseWrite(tmp,sizeof(int),16,file,memory);
	CaseWrite(tmp2,sizeof(int),16,file,memory);


	if (tmp2[10])
	{
//		fwrite(m_varControl,sizeof(int),m_layerMax,file);
		CaseWrite(m_varControl,sizeof(int),m_layerMax,file,memory);
	}

	if (tmp2[11])
	{
//		fwrite(tmp3,sizeof(int),m_layerMax,file);
		CaseWrite(tmp3,sizeof(int),m_layerMax,file,memory);
	}

//	fwrite(m_paraKosuu,sizeof(int),m_layerMax,file);
//	fwrite(m_selectParam,sizeof(int),m_layerMax,file);
	CaseWrite(m_paraKosuu,sizeof(int),m_layerMax,file,memory);
	CaseWrite(m_selectParam,sizeof(int),m_layerMax,file,memory);

	for (i=0;i<m_layerMax;i++)
	{
		if (m_effectData[i].flag)
		{
//			fwrite(&m_effectData[i],sizeof(EFFECT),1,file);
			CaseWrite(&m_effectData[i],sizeof(EFFECT),1,file,memory);
		}
	}

	for (i=0;i<m_layerMax;i++)
	{
		if (m_effectData[i].flag)
		{
			if (m_effectData[i].pic != -1)
			{
//				fwrite(&m_picFileName[i][0],sizeof(char),256,file);
				CaseWrite(&m_picFileName[i][0],sizeof(char),256,file,memory);
			}
		}
	}





	//miniPic
	if (tmp[10] & 0x1)
	{
//		fwrite(m_miniPic,sizeof(int),32*24,file);
		CaseWrite(m_miniPic,sizeof(int),32*24,file,memory);
	}

	//largePic
	if (tmp[10] & 0x2)
	{
//		fwrite(m_largePic,sizeof(int),64*48,file);
		CaseWrite(m_largePic,sizeof(int),64*48,file,memory);
	}

	if (tmp[10] & 0x4)
	{
//		fwrite(m_veryLargePic,sizeof(int),128*96,file);
		CaseWrite(m_veryLargePic,sizeof(int),128*96,file,memory);
	}

	if (tmp[11])
	{
//		fwrite(m_komaName,sizeof(char),64,file);
		CaseWrite(m_komaName,sizeof(char),64,file,memory);
	}

//	fwrite(m_exMusicParam,sizeof(int),16,file);
	CaseWrite(m_exMusicParam,sizeof(int),16,file,memory);


	SaveArrayObject(file,memory);
/*`
	for (i=0;i<m_objectKosuu;i++)
	{
		CMessageData* lpMes = (CMessageData*)m_objectDataArray[i];
		if (lpMes == NULL)	//用心、クラッシュ防止
		{
//			MessageBox(NULL,"no CMessageData Class in CKomaData::Save()","致命的ERROR",MB_OK);
		}
		else
		{
			lpMes->Save(file);
		}
	}
*/
	SetModifyFlag(FALSE);

	return TRUE;
}




/*
BOOL CKomaData::ResizeAray(int n)
{
	CMessageData** lpArray = new CMessageData*[n];
	for (int i=0;i<m_messageDataKosuu;i++)
	{
		*lpArray = m_messageDataArray[i];
	}
	for (i=m_messageDataKosuu;i<n;i++)
	{
		*lpArray = NULL;
	}
	
	DELETEARRAY(m_messageDataArray);

	m_messageDataArray = lpArray;
	m_messageDataKosuuMax = n;

	return TRUE;
}
*/

LPSTR CKomaData::GetMyName(void)
{
	return NULL;
}

void CKomaData::SetMyName(LPSTR name)
{
}


CCase* CKomaData::NewObject(void)
{
	CMessageData* pMes = new CMessageData();

	return pMes;
}

LPSTR CKomaData::GetPicFileName(int n)
{
	if ((n<0) || (n>255)) n = 0;
	return &m_picFileName[n][0];
}

void CKomaData::SetPicFileName(int n, LPSTR name,BOOL sameZahyoFlag)
{
	if ((n<0) || (n>255)) return;

	int ln = strlen(name);
	if (ln>254) ln = 254;
	ZeroMemory(&m_picFileName[n][0],256);
	memcpy(&m_picFileName[n][0],name,ln);
	m_effectData[n].flag = TRUE;
	m_effectData[n].pic = n;

	if (m_effectData[n].command == -1)
	{
		m_effectData[n].command = EFFECT_NOP;
	}



	RECT rc;
	SetRect(&rc,0,0,640,480);
//	RECT rc2 = m_effectData[n].src;
//	RECT rc3 = m_effectData[n].dst;

	if (sameZahyoFlag == FALSE)
	{
		m_effectData[n].src = rc;
		m_effectData[n].srcFrom = rc;
		m_effectData[n].srcTo = rc;

		m_effectData[n].dst = rc;
		m_effectData[n].dstFrom = rc;
		m_effectData[n].dstTo = rc;
	}


	SetModifyFlag();

//	if (m_effectData[n].command == -1)
//	{
//	}

}











void CKomaData::SetEffect(int layer,int eff)
{
	if ((layer<0) || (layer>=m_layerMax)) return;

	BOOL oldFlag = m_effectData[layer].flag;

	m_effectData[layer].command = eff;

	int i;
	for (i=0;i<16;i++) m_effectData[layer].para[i] = 0;//@@@@?????


	CEffectParam* effectParam = m_allEffectParam->GetEffectParam(eff);




	//int kosuu = CEffectParaList::m_para[eff].paraKosuuMin;
	int kosuu = effectParam->GetNeedParaKosuu();

	for (i=0;i<kosuu;i++)
	{
//		m_effectData[layer].para[i] = CEffectParaList::m_para[eff].effectParaData[i].paraDefault;
		m_effectData[layer].para[i] = effectParam->GetParaDefault(i);
	}

	if (oldFlag == FALSE)
	{
		m_effectData[layer].pic = -1;
	}

	m_paraKosuu[layer] = kosuu;
	m_effectData[layer].flag = TRUE;

	if (m_effectData[layer].command != EFFECT_MAE)
	{
		m_effect->SetEffectScript(layer,eff,m_paraKosuu[layer],m_effectData[layer].para);
	}
//	m_effectData[layer].countMax = m_effect->GetCountMax(layer);


	SetModifyFlag();
}


void CKomaData::SetAllEffect(void)
{
//dummy
//m_effect->ClearAllEffect();

	int i;
	for (i=0;i<16;i++)
	{

	}


	for (i=0;i<m_layerMax;i++)
	{
		BOOL b = TRUE;
		if (m_effectData[i].flag)
		{
			if (m_effectData[i].command == EFFECT_MAE)
			{
				b = FALSE;
			}
		}

		if (b == FALSE)	//当面の処置
		{
//OutputDebugString("*");
			continue;
		}

		if (b)
		{
			m_effect->ClearEffect(i,FALSE);
		}

		m_effect->SetEffect(i,&m_effectData[i]);

//		m_effect->m_effect[i] = m_effectData[i];
//continue;

		if (m_effectData[i].flag)
		{
			int pic = m_effectData[i].pic;
//			if (pic != -1)
			if (1)
			{
				//set filename

				//m_effect->m_effect[i].pic = pic;
//				m_effect->SetLayerPic(i,pic);
				if (pic == -1)
				{
					m_effect->SetLayerPic(i,pic);
				}
				else
				{
					m_effect->SetLayerPic(i,i);
				}

//				LPSTR filename = GetPicFileName(i);
//				CPicture* lpPic = m_effect->GetPicture(pic);
			}


			int eff = m_effectData[i].command;
			if (eff != -1)
			{
				if (pic != -1)
				{
//OutputDebugString("EFFECT_MOVE:");
					int para[4];
					//locate
					para[0] = m_effectData[i].dstFrom.left;
					para[1] = m_effectData[i].dstFrom.top;
					para[2] = m_effectData[i].dstFrom.right;
					para[3] = m_effectData[i].dstFrom.bottom;
//para[0] = 0;
//para[1] = 0;
					m_effect->SetEffectScript(i,EFFECT_LOCATE,4,para);
					//lmove
					para[0] = m_effectData[i].para[0];
					para[1] = m_effectData[i].dstTo.left - m_effectData[i].dstFrom.left;
					para[2] = m_effectData[i].dstTo.top - m_effectData[i].dstFrom.top;
					m_effect->SetEffectScript(i,EFFECT_LMOVE,3,para);
				}

//				if (eff != EFFECT_MOVE)
				if (1)
				{
					m_effect->SetEffectScript(i,eff,m_paraKosuu[i],m_effectData[i].para);
				}
			}
//		CEffect::EFFECT * effect = m_effect->m_effect[i];
//		*effect
		}
	}
}


void CKomaData::LoadAllPicture(void)
{
	for (int i=0;i<m_layerMax;i++)
	{
		if (m_effectData[i].flag)
		{
			if (m_effectData[i].command != EFFECT_MAE)
			{
				int pic = m_effectData[i].pic;
				if (pic != -1)
				{
					LoadPicture(i);
				}
			}
			else
			{
				int pic = m_effectData[i].pic;
				if (pic != -1)
				{
					LoadPicture(i);
				}

//OutputDebugString("-");
			}
		}
	}
}


void CKomaData::ReloadAllPic(void)
{
	for (int i=0;i<m_layerMax;i++)
	{
		AdjustLayerEffect(i);
		LoadPicture(i);
	}
}



void CKomaData::LoadPicture(int layer)
{
//OutputDebugString("?");
	if (m_effectData[layer].flag == FALSE) return;

	int pic = m_effectData[layer].pic;
	if (pic == -1) return;



	//CPicture* lpPic = m_effect->GetPicture(pic);
	CPicture* lpPic = m_effect->GetPicture(layer);	//debug 2005-05-02
	
	if (lpPic == NULL) return;

	LPSTR name = GetPicFileName(layer);
	if (name == NULL) return;
	if ((*name) == 0) return;


	BOOL charaFlag = FALSE;

	char filename[256];
	char filename2[256];

	char tagName[3];
	tagName[0] = *name;
	tagName[1] = *(name+1);
	tagName[2] = 0;



	if (_stricmp(tagName,"ta") == 0)
	{
		wsprintf(filename,"ta\\%s",name);
		wsprintf(filename2,"ta\\%s",name);
		charaFlag = TRUE;
	}
	else if (_stricmp(tagName,"bg") == 0)
	{
		wsprintf(filename,"bg\\%s",name);
	}
	else if (_stricmp(tagName,"ev") == 0)
	{
		wsprintf(filename,"ev\\%s",name);
	}
	else if (_stricmp(tagName,"ed") == 0)
	{
		wsprintf(filename,"ed\\%s",name);
	}
	else
	{
		wsprintf(filename,"etc\\%s",name);
	}

	if (lpPic->LoadDWQ(filename) == FALSE)
	{
		MessageBox(NULL,filename,"File not found",MB_OK | MB_ICONINFORMATION);
	}

//	if (m_app->GetCutinFlag())
//	{
//		CCutinControl* cutinControl = m_app->GetCutinControl();
//		if (cutinControl != NULL)
//		{
//			cutinControl->OnNewFile(layer,name);
//		}
//	}


/*
	if (lpPic->LoadPicture(filename))
	{
//		int* buf = lpPic->GetPictureBuffer();
//		PictureToMini(layer,buf);
		if (charaFlag)
		{
			lpPic->CutData();
			lpPic->LoadAntiAlias2(filename2);
		}
	}
	else
	{
		//load default pic
//			lpPic->
	}
*/

}




int CKomaData::GetEffect(int layer)
{



	if ((layer<0) || (layer>=m_layerMax)) return -1;
	if (m_effectData[layer].flag == FALSE) return -1;

	return m_effectData[layer].command;
}

int CKomaData::GetEffectParaKosuu(int layer)
{
	if ((layer<0) || (layer>=m_layerMax)) return 0;
	if (m_effectData[layer].flag == FALSE) return 0;
	return m_paraKosuu[layer];
}

int CKomaData::GetEffectPara(int layer,int n)
{
	if ((layer<0) || (layer>=m_layerMax)) return 0;
	if (m_effectData[layer].flag == FALSE) return 0;
	return m_effectData[layer].para[n];
}

void CKomaData::SetEffectParaKosuu(int layer,int n)
{
	if ((layer<0) || (layer>=m_layerMax)) return;
	m_paraKosuu[layer] = n;
	SetModifyFlag();
}


int CKomaData::GetPictureNumber(int layer)
{
	if (m_effectData[layer].flag == FALSE) return -1;
	if (m_effectData[layer].pic != -1) return layer;
	return -1;
}


void CKomaData::Print(void)
{
	SetAllEffect();
	LoadAllPicture();
	CAreaControl::SetNextAllPrint();



	m_effect->Print();
	m_effect->Print(TRUE);
}


void CKomaData::InitEffectDefaultPara(int layer, int n)
{
	int eff = m_effectData[layer].command;
	if (eff == -1)
	{
		return;
	}

	CEffectParam* effectParam = m_allEffectParam->GetEffectParam(eff);

//	int d = CEffectParaList::m_para[eff].effectParaData[n].paraDefault;
	int d = effectParam->GetParaDefault(n);
	m_effectData[layer].para[n] = d;

	SetModifyFlag();
}


void CKomaData::SetEffectPara(int layer, int n, int d)
{
	m_effectData[layer].para[n] = d;
	SetModifyFlag();
}

void CKomaData::DeleteEffectPic(int layer)
{
	if ((layer<0) || (layer>=m_layerMax)) return;
	m_effectData[layer].pic = -1;
	int eff = m_effectData[layer].command;
	m_picFileName[layer][0] = 0;

	if ((eff == EFFECT_NOP) /*|| (eff == EFFECT_MOVEONLY)*/) m_effectData[layer].flag = FALSE;
	if (eff == -1) m_effectData[layer].flag = FALSE;

	SetModifyFlag();
}

void CKomaData::SetEffectFlag(int layer,BOOL flag)
{
	if ((layer<0) || (layer>=m_layerMax)) return;
	m_effectData[layer].flag = flag;

	SetModifyFlag();
}

BOOL CKomaData::GetEffectFlag(int layer)
{
	if ((layer<0) || (layer>=m_layerMax)) return FALSE;
	return m_effectData[layer].flag;
}


void CKomaData::DeleteEffect(int layer)
{
	if ((layer<0) || (layer>=m_layerMax)) return;

	if ((m_effectData[layer].flag == FALSE) || (m_effectData[layer].pic == -1))
	{
		m_effectData[layer].command = -1;
		m_effectData[layer].flag = FALSE;
	}
	else
	{
		m_effectData[layer].command = EFFECT_NOP;
	}

	m_paraKosuu[layer] = 0;

	SetModifyFlag();
}

void CKomaData::SetEffectRect(RECT* lpRect, int layer, int setBit)
{
	if (setBit & 1) m_effectData[layer].dst = *lpRect;
	if (setBit & 2) m_effectData[layer].dstFrom = *lpRect;
	if (setBit & 4) m_effectData[layer].dstTo = *lpRect;
	if (setBit & 8) m_effectData[layer].src = *lpRect;
	if (setBit & 0x10) m_effectData[layer].srcFrom = *lpRect;
	if (setBit & 0x20) m_effectData[layer].srcTo = *lpRect;

}

void CKomaData::GetEffectRect(RECT* lpRect,int layer,int rectNumber)
{
	if (rectNumber == 0) *lpRect = m_effectData[layer].dst;
	if (rectNumber == 1) *lpRect = m_effectData[layer].dstFrom;
	if (rectNumber == 2) *lpRect = m_effectData[layer].dstTo;
	if (rectNumber == 3) *lpRect = m_effectData[layer].src;
	if (rectNumber == 4) *lpRect = m_effectData[layer].srcFrom;
	if (rectNumber == 5) *lpRect = m_effectData[layer].srcTo;
}





void CKomaData::Init(LPVOID para)
{
	m_editFlag = TRUE;
	m_miniPicFlag = FALSE;
	m_largePicFlag = FALSE;
	m_veryLargePicFlag = FALSE;

	m_overrapCount = 0;
	m_overrapType = 0;
	m_windowOffFlag = FALSE;
	m_setCG = 0;
	m_demoFlag = 0;

	m_effectVersion = 4;
	m_bgmNumber = 0;
	m_frameTime = 0;

	m_windowNumber = 0;
	m_cursorNumber = 0;
	m_mouseNumber = 0;

	m_autoMessage = 0;
	m_cannotClick = 0;
	m_cannotSkip = 0;

	m_optionOff = 0;
	m_cutinFlag = 0;


	m_preload = 0;
	m_expStatus = 0;
	int i;
	for (i=0;i<32;i++)
	{
		m_varControl[i] = 0;
	}

	for (i=0;i<16;i++)
	{
		m_exMusicParam[i] = -1;
	}
	m_exMusicParam[0] = 0;//無限ループ
	m_exMusicParam[1] = 0;//delta volume
	m_exMusicParam[4] = 0;//direct volume

	for (i=0;i<32;i++)
	{
		m_varControlLayer[i] = 0;
	}

	m_selectEffectLayer = -1;
	m_selectLayer = -1;
	m_komaNameFlag = FALSE;

	ZeroMemory(m_paraKosuu,sizeof(int)*32);
	ZeroMemory(m_selectParam,sizeof(int)*32);

	for (i=0;i<32;i++)
	{
		ZeroMemory(&m_effectData[i],sizeof(EFFECT));
		ZeroMemory(&m_picFileName[i][0],256);
		m_effectData[i].flag = FALSE;
		m_effectData[i].command = -1;
		m_effectData[i].pic = -1;
	}

	m_objectKosuu = 0;
}



void CKomaData::OutputScriptSource(FILE* file)
{
	OutputData(file,"KomaData",2);
	OutputData(file,"{",2);

	char mes[1024];
	
	int overrapCount = GetOverrapCount();
	int overrapType = GetOverrapType();
	if (overrapCount > 0)
	{
		OutputData(file,"Overrap",3);
		OutputData(file,"{",3);
		wsprintf(mes,"overrapType=%d;",overrapType);
		OutputData(file,mes,4);
		wsprintf(mes,"overrapTime=%d;",overrapCount);
		OutputData(file,mes,4);
		OutputData(file,"}",3);
	}

	//param
	int windowOff = GetWindowOffFlag();
	int bgmNumber = GetBGMNumber();
	int frameTime = GetFrameTime();
	int windowNumber = GetWindowNumber();
	int cursorNumber = GetCursorNumber();
	int mouseNumber = GetMouseNumber();
	int autoMessage = GetAutoMessage();
	int cannotClick = GetCannotClick();
	int cannotSkip = GetCannotSkip();
	int optionOff = GetOptionOff();
	
	//bgm para??

	BOOL flg = FALSE;
	if (windowOff != 0) flg = TRUE;
	if (bgmNumber != 0) flg = TRUE;
	if (frameTime != 0) flg = TRUE;
	if (windowNumber != 0) flg = TRUE;
	if (cursorNumber != 0) flg = TRUE;
	if (mouseNumber != 0) flg = TRUE;
	if (autoMessage != 0) flg = TRUE;
	if (cannotClick != 0) flg = TRUE;
	if (cannotSkip != 0) flg = TRUE;
	if (optionOff != 0) flg = TRUE;

	if (flg)
	{
		OutputData(file,"komaParam",3);
		OutputData(file,"{",3);

		if (windowOff != 0)
		{
			wsprintf(mes,"windowOff=%d",windowOff);
			OutputData(file,mes,4);
		}

		if (bgmNumber != 0)
		{
			wsprintf(mes,"bgmNumber=%d",bgmNumber);
			OutputData(file,mes,4);
		}

		if (frameTime != 0)
		{
			wsprintf(mes,"frameTime=%d",frameTime);
			OutputData(file,mes,4);
		}

		if (windowNumber != 0)
		{
			wsprintf(mes,"windowNumber=%d",windowNumber);
			OutputData(file,mes,4);
		}

		if (cursorNumber != 0)
		{
			wsprintf(mes,"cursorNumber=%d",cursorNumber);
			OutputData(file,mes,4);
		}

		if (mouseNumber != 0)
		{
			wsprintf(mes,"mouseNumber=%d",mouseNumber);
			OutputData(file,mes,4);
		}

		if (autoMessage != 0)
		{
			wsprintf(mes,"autoMessage=%d",autoMessage);
			OutputData(file,mes,4);
		}

		if (cannotClick != 0)
		{
			wsprintf(mes,"cannotClick=%d",cannotClick);
			OutputData(file,mes,4);
		}

		if (cannotSkip != 0)
		{
			wsprintf(mes,"cannotSkip=%d",cannotSkip);
			OutputData(file,mes,4);
		}

		if (optionOff != 0)
		{
			wsprintf(mes,"optionOff=%d",optionOff);
			OutputData(file,mes,4);
		}

		OutputData(file,"}",3);
	}



	//effect

	//mess block
//	fwrite("// KOMADATA\x00d\x00a",sizeof(char),13,file);
//	fwrite("// -EFFECT\x00d\x00a",sizeof(char),12,file);
	OutputData(file,"Effect",3);
	OutputData(file,"{",3);

	for (int layer = 0;layer < m_layerMax;layer++)
	{
		if (m_effectData[layer].flag)
		{
			wsprintf(mes,"layer[%d]",layer+1);
			OutputData(file,mes,4);
			OutputData(file,"{",4);
			int eff = GetEffect(layer);
			if (eff != -1)
			{
				wsprintf(mes,"effect=%d;",eff);
				OutputData(file,mes,5);
				int paraKosuu = GetEffectParaKosuu(layer);
				wsprintf(mes,"paramNumber=%d;",paraKosuu);
				OutputData(file,mes,5);
				for (int p=0;p<paraKosuu;p++)
				{
					wsprintf(mes,"param[%d]=%d",p,GetEffectPara(layer,p));
					OutputData(file,mes,5);
				}
			}

			int pic = GetPictureNumber(layer);
			if (pic != -1)
			{
				LPSTR pname = GetPicFileName(layer);
				if ((*pname) != 0)
				{
					wsprintf(mes,"picture=\"%s\"",pname);
					OutputData(file,mes,5);

					//rect

				}
			}
			
			OutputData(file,"}",4);
		}
	}
	OutputData(file,"}",3);
	
	OutputData(file,"MessageData()",3);
	OutputData(file,"{",3);
	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* pCase = m_objectDataArray[i];
		pCase->OutputScriptSource(file);
	}
	OutputData(file,"}",3);

	OutputData(file,"}",2);
}


int CKomaData::GetOverrapCount(void)
{
	return m_overrapCount;
}

void CKomaData::SetOverrapCount(int c)
{
	m_overrapCount = c;
	SetModifyFlag();
}

int CKomaData::GetOverrapType(void)
{
	return m_overrapType;
}

void CKomaData::SetOverrapType(int c)
{
	m_overrapType = c;
	SetModifyFlag();
}

BOOL CKomaData::CheckExistChara(int charaNumber)
{
	for (int i=0;i<m_objectKosuu;i++)
	{
		CMessageData* pMessage = (CMessageData*)GetObjectData(i);
		if (pMessage != NULL)
		{
			if (pMessage->CheckExistChara(charaNumber,0)) return TRUE;
			if (pMessage->CheckExistChara(charaNumber,1)) return TRUE;
		}
	}
	return FALSE;
}


BOOL CKomaData::GetWindowOffFlag(void)
{
	return m_windowOffFlag;
}


void CKomaData::SetWindowOffFlag(int md)
{
	m_windowOffFlag = md;
}




int CKomaData::GetSetCGPlayer(void)
{
	return m_setCG >> 16;
}

int CKomaData::GetSetCGNumber(void)
{
	return m_setCG & 0xffff;
}

void CKomaData::SetSetCG(int pl, int n)
{
	m_setCG = (pl << 16) + n;
}



int CKomaData::GetBGMNumber(void)
{
	return m_bgmNumber;
}


void CKomaData::SetBGMNumber(int n)
{
	SetModifyFlag();
	m_bgmNumber = n;
}


BOOL CKomaData::CopyOriginalData(CCase* lpCase)
{
	return TRUE;
}

int CKomaData::Clean(void)
{
	return 0;
}


void CKomaData::MoveLayer(int fromLayer,int toLayer)
{
	TaihiLayer(fromLayer);
	FuqueLayer(toLayer);
	
	DeleteEffectPic(fromLayer);

	AdjustLayerEffect(fromLayer);
	AdjustLayerEffect(toLayer);

	SetModifyFlag();
}

void CKomaData::CopyLayer(int fromLayer,int toLayer)
{
	TaihiLayer(fromLayer);
	FuqueLayer(toLayer);

	AdjustLayerEffect(toLayer);

	SetModifyFlag();
}

void CKomaData::ExchangeLayer(int fromLayer,int toLayer)
{
	TaihiLayer(toLayer);
	TaihiLayer(fromLayer,1);
	FuqueLayer(toLayer,1);
	FuqueLayer(fromLayer);

	AdjustLayerEffect(fromLayer);
	AdjustLayerEffect(toLayer);

	SetModifyFlag();
}


void CKomaData::MoveEffect(int fromLayer,int toLayer)
{
	int kosuu = m_paraKosuu[fromLayer];
	int param = m_selectParam[fromLayer];

	TaihiLayer(toLayer);
	m_effectData[toLayer] = m_effectData[fromLayer];
	FuqueLayer(toLayer);
	m_paraKosuu[toLayer] = kosuu;
	m_selectParam[toLayer] = param;

	DeleteEffect(fromLayer);

	AdjustLayerEffect(fromLayer);
	AdjustLayerEffect(toLayer);

	SetModifyFlag();
}

void CKomaData::CopyEffect(int fromLayer,int toLayer)
{
	int kosuu = m_paraKosuu[fromLayer];
	int param = m_selectParam[fromLayer];
	TaihiLayer(toLayer);
	m_effectData[toLayer] = m_effectData[fromLayer];
	FuqueLayer(toLayer);
	m_paraKosuu[toLayer] = kosuu;
	m_selectParam[toLayer] = param;
	AdjustLayerEffect(toLayer);

	SetModifyFlag();
}

void CKomaData::ExchangeEffect(int fromLayer,int toLayer)
{
	int kosuu = m_paraKosuu[fromLayer];
	int kosuu2 = m_paraKosuu[toLayer];
	int param = m_selectParam[fromLayer];
	int param2 = m_selectParam[toLayer];


	TaihiLayer(toLayer);
	TaihiLayer(fromLayer,1);

	m_tmpEffectData = m_effectData[fromLayer];
	m_effectData[fromLayer] = m_effectData[toLayer];
	m_effectData[toLayer] = m_tmpEffectData;

	FuqueLayer(fromLayer,1);
	FuqueLayer(toLayer);

	m_paraKosuu[fromLayer] = kosuu2;
	m_paraKosuu[toLayer] = kosuu;
	m_selectParam[fromLayer] = param2;
	m_selectParam[toLayer] = param;

	AdjustLayerEffect(fromLayer);
	AdjustLayerEffect(toLayer);

	SetModifyFlag();
}



void CKomaData::TaihiLayer(int layer,int buf)
{
	m_taihiPic[buf] = m_effectData[layer].pic;
	memcpy(m_taihiFilename[buf],m_picFileName[layer],256);
	m_taihiRect[buf][0] = m_effectData[layer].dst;
	m_taihiRect[buf][1] = m_effectData[layer].dstFrom;
	m_taihiRect[buf][2] = m_effectData[layer].dstTo;
	m_taihiRect[buf][3] = m_effectData[layer].src;
	m_taihiRect[buf][4] = m_effectData[layer].srcFrom;
	m_taihiRect[buf][5] = m_effectData[layer].srcTo;
}



void CKomaData::FuqueLayer(int layer,int buf)
{
	m_effectData[layer].pic = m_taihiPic[buf];
	memcpy(m_picFileName[layer],m_taihiFilename[buf],256);
	m_effectData[layer].dst = m_taihiRect[buf][0];
	m_effectData[layer].dstFrom = m_taihiRect[buf][1];
	m_effectData[layer].dstTo = m_taihiRect[buf][2];
	m_effectData[layer].src = m_taihiRect[buf][3];
	m_effectData[layer].srcFrom = m_taihiRect[buf][4];
	m_effectData[layer].srcTo = m_taihiRect[buf][5];
}



void CKomaData::AdjustLayerEffect(int layer)
{
	if ((m_effectData[layer].pic == -1) || (m_picFileName[layer][0] == 0))
	{
		if (m_effectData[layer].command == -1)
		{
			m_effectData[layer].flag = FALSE;
		}

		if (m_effectData[layer].command == EFFECT_NOP)
		{
			m_effectData[layer].command = -1;
			m_effectData[layer].flag = FALSE;
		}
	}
	else
	{
		m_effectData[layer].pic = layer;

		if (m_effectData[layer].flag == FALSE)
		{
			m_effectData[layer].flag = TRUE;
			m_effectData[layer].command = EFFECT_NOP;
		}
		else
		{
			if (m_effectData[layer].command == -1)
			{
				m_effectData[layer].command = EFFECT_NOP;
			}
		}
	}
}

void CKomaData::CreateMiniPicBuffer(void)
{
	if (m_miniPic == NULL)
	{
		m_miniPic = new int[32*24];
	}
}

void CKomaData::CreateLargePicBuffer(void)
{
	if (m_largePic == NULL)
	{
		m_largePic = new int[64*48];
	}
}

void CKomaData::CreateVeryLargePicBuffer(void)
{
	if (m_veryLargePic == NULL)
	{
		m_veryLargePic = new int[128*96];
	}
}

void CKomaData::CreateKomaNameBuffer(void)
{
	if (m_komaName == NULL)
	{
		m_komaName = new char[64];
	}
}

void CKomaData::SetKomaName(LPSTR name)
{
	if (name == NULL) return;
	CreateKomaNameBuffer();
	int ln = strlen(name);
	if (ln>62) ln = 62;

	memcpy(m_komaName,name,ln);
	m_komaName[ln] = 0;
	m_komaName[ln+1] = 0;
	m_komaNameFlag = TRUE;
}


LPSTR CKomaData::GetKomaName(void)
{
	if (m_komaNameFlag == FALSE) return NULL;
	return m_komaName;
}



void CKomaData::SetSelectParam(int layer,int paramNumber)
{
	m_selectParam[layer] = paramNumber;
}

int CKomaData::GetSelectParam(int layer)
{
	return m_selectParam[layer];
}

int CKomaData::GetSMFrameNumber(void)
{
	int kosuu = GetObjectKosuu();
	if (kosuu<1) return 0;	//error!??

	CMessageData* pMessage = (CMessageData*)GetObjectData(0);
	if (pMessage->GetMessageMode() != 3) return 0;

	int frame = pMessage->GetDrawCount();

	int type = m_frameOfKomaType;

	int n = m_frameOfKoma;
	if (type==0)
	{
		if (n<0) n = 0;
		if (n>frame) n = frame;
		return n;
	}

	n *= (frame-1);
	n /= 100;

	if (n<0) n = 0;
	if (n>frame) n = frame;

	return n;
}




int CKomaData::GetExMusicParam(int n)
{
	return m_exMusicParam[n];
}

void CKomaData::SetExMusicParam(int n,int d)
{
	m_exMusicParam[n] = d;
	SetModifyFlag();
}


int CKomaData::GetDemoFlag(void)
{
	return m_demoFlag;
}

void CKomaData::SetDemoFlag(int md)
{
	m_demoFlag = md;
}


int CKomaData::GetVarControlLayer(int layer)
{
	return m_varControlLayer[layer];

}

void CKomaData::SetVarControlLayer(int layer,int d)
{
	m_varControlLayer[layer] = d;
}

int CKomaData::CheckVarControl(void)
{
	int d = 0;
	int bit = 1;
	for (int i=0;i<m_layerMax;i++)
	{
		if (m_varControl[i])
		{
			d |= bit;
		}
		bit <<= 1;
	}

	return d;
}



/*_*/


