//
// messagedata.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonmacro.h"

#include "case.h"
#include "undoMemoryObject.h"
#include "messagedata.h"

int CMessageData::m_defaultDrawTime = 10;

#define MAX_MYMESSAGE_LENGTH 4096

#define NOWKAKUCHOKOSUU 16

char CMessageData::m_defaultName[256] = "デフォ子";

int CMessageData::m_bitPattern[]=
{
	0x00000001,0x00000002,0x00000004,0x00000008,
	0x00000010,0x00000020,0x00000040,0x00000080,
	0x00000100,0x00000200,0x00000400,0x00000800,
	0x00001000,0x00002000,0x00004000,0x00008000,
	0x00010000,0x00020000,0x00040000,0x00080000,
	0x00100000,0x00200000,0x00400000,0x00800000,
	0x01000000,0x02000000,0x04000000,0x08000000,
	0x10000000,0x20000000,0x40000000,(int)0x80000000,
};


char CMessageData::m_separator[] = "\r\n----------------------------------------------------------------------------\r\n";
char CMessageData::m_emphasis1[]  = "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\r\n";
char CMessageData::m_emphasis9[]  = "┃                                                        ┃";
char CMessageData::m_emphasis2[]  = "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\r\n";
char CMessageData::m_emphasis3[]  =   "                                                                ";

char CMessageData::m_haraponnobaka[] = "※※※ 同一のせりふですにゃん ※※※";
char CMessageData::m_tmpMessage[1024];
char CMessageData::m_tmpMessage2[16384];

int CMessageData::m_defaultDrawCount = 5;



int CMessageData::m_whiteSpace = 0;
char CMessageData::m_whiteSpaceStr[1024]={""};



//
// se,voice control data
//
// 0:volume
// 1:flg
// 2:teii1
// 3:teii2
// 4:movetime
// 5:
// 6:fade time (拡張)
// 7:
//

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
// 12systemse
// 13 complete




CMessageData::CMessageData() : CCase()
{
	m_messageMode = 0;	//0:print 1:lprint 2:append 3:draw
	m_messageBuffer = new char[512];
	m_messageBufferSize = 512;

	m_drawCount = m_defaultDrawTime;	//default = 0.5秒

	m_voiceFileFlag = 0;
	m_seFileFlag = 0;
	m_expStatus = 0;
	m_messageEffect = 0;
	m_messageEffectTime = 0;

	m_messageFontSizeType = 0;

	m_voiceCharaNumber = 0;
	m_voiceNumber = 0;

	for (int i=0;i<16;i++)
	{
		m_seNumber[i] = 0;
	}

	m_voiceChannel = 0;
	m_seChannel = 0;
	m_face = 0;
	m_mustFace = 0;
	m_musicFade = 0;

	m_messageColor = 0;

	m_kakuchoWorkKosuu = NOWKAKUCHOKOSUU;
	m_cannotClickFlag = 0;

	m_autoMessageTime = 0;
	m_autoMessageSpeed = 0;

	m_serialNumber = 0;

	m_renameFileFlag = FALSE;

	m_voiceLockFlag = 0;

	m_voiceFileName = NULL;
	m_voiceFileNameSize = 0;
	m_seFileName = NULL;
	m_seFileNameSize = 0;

	m_seControlFlag = 0;
	m_seControlData = NULL;
	m_seControlDataSize = 0;
	m_voiceControlFlag = 0;
	m_voiceControlData = NULL;
	m_voiceControlDataSize = 0;
}



CMessageData::~CMessageData()
{
	End();
}

void CMessageData::End(void)
{
	DELETEARRAY(m_voiceControlData);
	DELETEARRAY(m_seControlData);

	DELETEARRAY(m_messageBuffer);

	DELETEARRAY(m_voiceFileName);
	DELETEARRAY(m_seFileName);
}


LPSTR CMessageData::GetMessageData(void)
{
	return m_messageBuffer;
}

LPSTR CMessageData::GetMessageDataSkipAllComment(void)
{
	int gyo = GetMessageGyo();
	m_tmpMessage2[0] = 0;
	BOOL found = FALSE;
	int ptr = 0;

	for (int i=0;i<gyo;i++)
	{
		int ln = GetMessage1Gyo(i);
		if (m_tmpMessage[0] == '/')
		{
			if (m_tmpMessage[1] == '/') continue;
		}

#if _MSC_VER >= 1400
		strcat_s(m_tmpMessage2+ptr,16384,m_tmpMessage);
		ptr += ln;
		strcat_s(m_tmpMessage2+ptr,16384,"\x00d\x00a");
		ptr += 2;
#else
		strcat(m_tmpMessage2+ptr,m_tmpMessage);
		ptr += ln;
		strcat(m_tmpMessage2+ptr,"\x00d\x00a");
		ptr += 2;
#endif

		found = TRUE;
	}

	return m_tmpMessage2;
}


LPSTR CMessageData::GetMessageDataSkipComment(void)
{
	LPSTR mes0 = GetMessageData();
	LPSTR mes = mes0;
	int ln = strlen(mes0);
	
	if (ln >= 4)
	{
		BOOL flg = TRUE;

		while (flg)
		{
			if ((*mes) != '/') break;
			if ((*(mes+1)) != '/') break;

			mes += 2;
			for (int i=0;i<ln-2;i++)
			{
				if (*(mes+i) == 0x00d)
				{
					if (*(mes+i+1) == 0x00a)
					{
						mes += i;
						mes += 2;
					}
					break;
				}
			}

			ln = strlen(mes);
			if (ln < 4) break;
		}
	}

	return mes;
}


void CMessageData::SetMessageData(LPSTR mes)
{
	if (mes == NULL)
	{
		m_messageBuffer[0] = 0;
		return;
	}

	int ln = strlen(mes);
	SetMessageData(mes,ln);
}






void CMessageData::CheckAndExpandMessageWork(int ln)
{
	if (ln>m_messageBufferSize)
	{
		int sz = (ln+511)/512;
		sz *= 512;
		char* tmp = new char[sz];

		if (m_messageBufferSize > 0)
		{
			memcpy(tmp,m_messageBuffer,m_messageBufferSize);
		}
		
		ZeroMemory(tmp+m_messageBufferSize,sz-m_messageBufferSize);

		DELETEARRAY(m_messageBuffer);
		m_messageBufferSize = sz;
		m_messageBuffer = tmp;
	}
}



void CMessageData::CheckAndExpandVoiceWork(int ln)
{
	if (ln>m_voiceFileNameSize)
	{
		int sz = (ln+63)/64;
		sz *= 64;
		char* tmp = new char[sz];

		if (m_voiceFileNameSize > 0)
		{
			memcpy(tmp,m_voiceFileName,m_voiceFileNameSize);
		}
		
		ZeroMemory(tmp+m_voiceFileNameSize,sz-m_voiceFileNameSize);

		DELETEARRAY(m_voiceFileName);
		m_voiceFileNameSize = sz;
		m_voiceFileName = tmp;
	}
}

void CMessageData::CheckAndExpandSeWork(int ln)
{
	if (ln>m_seFileNameSize)
	{
		int sz = (ln+63)/64;
		sz *= 64;
		char* tmp = new char[sz];

		if (m_seFileNameSize > 0)
		{
			memcpy(tmp,m_seFileName,m_seFileNameSize);
		}
		
		ZeroMemory(tmp+m_seFileNameSize,sz-m_seFileNameSize);

		DELETEARRAY(m_seFileName);
		m_seFileNameSize = sz;
		m_seFileName = tmp;
	}
}




void CMessageData::SetMessageData(LPSTR mes,int ln)
{
	if (mes == NULL)
	{
		m_messageBuffer[0] = 0;
		return;
	}

	if (ln>(MAX_MYMESSAGE_LENGTH-2)) ln = MAX_MYMESSAGE_LENGTH-2;

	CheckAndExpandMessageWork(ln+2);

	memcpy(m_messageBuffer,mes,ln);
	m_messageBuffer[ln] = 0;
	m_messageBuffer[ln+1] = 0;
}


void CMessageData::GetDrawCountMessage(char* buf)
{
	wsprintf(buf,"%d",m_drawCount);
}

void CMessageData::SetDrawCountMessage(LPSTR mes)
{
	if (mes == NULL)
	{
		m_drawCount = 0;
		return;
	}

	int ans = 0;
	int ln = strlen(mes);
	if (ln ==0)
	{
		m_drawCount = 0;
		return;
	}


	for (int i=0;i<ln;i++)
	{
		char c = *mes;
		mes++;
		int d = c & 0xff;
		d -= '0';
		if ((d<0) || (d>=10)) break;

		ans *= 10;
		ans += d;
	}

	m_drawCount = ans;
}

int CMessageData::CalcuNeedVoiceFileSize(int flg)
{
	if (flg == 0) return 0;
	if (flg == 1) return 64;

	int sz = 0;
	for (int i=0;i<16;i++)
	{
		if (flg & 1) sz = (i+1)*64;
		flg >>= 1;
	}

	return sz;
}

int CMessageData::CalcuNeedSeFileSize(int flg)
{
	if (flg == 0) return 0;
	if (flg == 1) return 64;

	int sz = 0;
	for (int i=0;i<16;i++)
	{
		if (flg & 1) sz = (i+1)*64;
		flg >>= 1;
	}

	return sz;
}


BOOL CMessageData::Load(FILE* file,CUndoMemoryObject* memory)
{
	char head[16];
	int tmp[16+16];
//	char name[64];

	for (int i=16;i<32;i++)
	{
		tmp[i] = 0;
	}

//	fread(head,sizeof(char),16,file);	//skip header
//	fread(tmp,sizeof(int),16,file);
	CaseRead(head,sizeof(char),16,file,memory);	//skip header
	CaseRead(tmp,sizeof(int),16,file,memory);


	m_messageMode = tmp[0];
	m_drawCount = tmp[1];
	m_voiceCharaNumber = tmp[2];
	m_voiceNumber = tmp[3];
	m_seNumber[0] = tmp[4];
	m_voiceFileFlag = tmp[5];
	m_seFileFlag = tmp[6];
	m_serialNumber = tmp[7];
	m_renameFileFlag = tmp[8];
	m_voiceLockFlag = tmp[9];

	int version = tmp[10];

	int messageSize = tmp[11];
	m_seControlFlag = tmp[12];
	m_voiceControlFlag = tmp[13];

	m_messageColor = tmp[14];


	m_kakuchoWorkKosuu = tmp[15];	//max16
	m_cannotClickFlag = 0;

	m_autoMessageTime = 0;
	m_autoMessageSpeed = 0;
	m_expStatus = 0;
	m_messageEffect = 0;
	m_messageEffectTime = 0;

	m_face = 0;
	m_mustFace = 0;

	if (m_kakuchoWorkKosuu > 0)
	{
		//read

//		fread(&tmp[16],sizeof(int),m_kakuchoWorkKosuu,file);
		CaseRead(&tmp[16],sizeof(int),m_kakuchoWorkKosuu,file,memory);
		m_cannotClickFlag = tmp[16];
		if (m_kakuchoWorkKosuu > 2)
		{
			m_autoMessageTime = tmp[17];
			m_autoMessageSpeed = tmp[18];
		}

		if (m_kakuchoWorkKosuu > 3)
		{
			m_expStatus = tmp[19];
		}

		if (m_kakuchoWorkKosuu > 4)
		{
			m_messageFontSizeType = tmp[20];
		}


		if (m_kakuchoWorkKosuu > 5)
		{
			m_face = tmp[21];
		}

		if (m_kakuchoWorkKosuu > 6)
		{
			m_mustFace = tmp[22];
		}

		if (m_kakuchoWorkKosuu > 7)
		{
			m_musicFade = tmp[23];
		}

		if (m_kakuchoWorkKosuu > 8)
		{
			m_messageEffect = tmp[24];
		}

		if (m_kakuchoWorkKosuu > 9)
		{
			m_messageEffectTime = tmp[25];
		}
	}

	m_kakuchoWorkKosuu = NOWKAKUCHOKOSUU;	//追加




	if (version == 0)
	{
		messageSize = 512;
		m_voiceFileFlag = 0;
		if (tmp[5])
		{
			m_voiceFileFlag = 1;
		}

		m_seFileFlag = 0;
		if (tmp[6])
		{
			m_seFileFlag = 1;
		}

		m_voiceLockFlag = 0;
		if (tmp[9])
		{
			m_voiceLockFlag = 1;
		}

		for (int i=1;i<16;i++)
		{
			m_seNumber[i] = 0;
		}
	}



	CheckAndExpandMessageWork(messageSize);
//	fread(m_messageBuffer,sizeof(char),messageSize,file);
	CaseRead(m_messageBuffer,sizeof(char),messageSize,file,memory);


	int voiceFileSize = CalcuNeedVoiceFileSize(m_voiceFileFlag);
	int seFileSize = CalcuNeedSeFileSize(m_seFileFlag);






	CheckAndExpandVoiceWork(voiceFileSize);
	int flg = m_voiceFileFlag;
	int i;
	for (i=0;i<16;i++)
	{
		if (flg & 1)
		{
			//fread(m_voiceFileName+i*64,sizeof(char),64,file);
			CaseRead(m_voiceFileName+i*64,sizeof(char),64,file,memory);
		}

		if ((flg  == 0) || (flg == 1)) break;

		flg >>= 1;
	}


	int seChannel = 0;
	if (version >= 1)
	{
		//load 1-15 by 0は前の場所に
		if (m_seFileFlag > 1)
		{
			BOOL flg0 = m_seFileFlag;
			flg0 >>= 1;

			for (int i=1;i<16;i++)
			{
				if (flg0 & 1)
				{
//					fread(&m_seNumber[i],sizeof(int),1,file);
					CaseRead(&m_seNumber[i],sizeof(int),1,file,memory);
					if (seChannel == 0) seChannel = i;
				}
				else
				{
					m_seNumber[i] = 0;
				}

				if ((flg0  == 0) || (flg0 == 1)) break;

				flg0 >>= 1;
			}
		}
	}



	CheckAndExpandSeWork(seFileSize);
	flg = m_seFileFlag;
	for (i=0;i<16;i++)
	{
		if (flg & 1)
		{
//			fread(m_seFileName+i*64,sizeof(char),64,file);
			CaseRead(m_seFileName+i*64,sizeof(char),64,file,memory);
		}

		if ((flg  == 0) || (flg == 1)) break;

		flg >>= 1;
	}


	if (m_voiceControlFlag)
	{
		int kosuuMax = GetMaxBit(m_voiceControlFlag);
		CreateVoiceControl(kosuuMax);

		for (i=0;i<16;i++)
		{
			if (m_voiceControlFlag & m_bitPattern[i])
			{
//				fread(m_voiceControlData+i*8,sizeof(int),8,file);
				CaseRead(m_voiceControlData+i*8,sizeof(int),8,file,memory);
			}
		}
	}

	if (m_seControlFlag)
	{
		int kosuuMax = GetMaxBit(m_seControlFlag);
		CreateSeControl(kosuuMax);

		for (i=0;i<16;i++)
		{
			if (m_seControlFlag & m_bitPattern[i])
			{
		//		fread(m_seControlData+i*8,sizeof(int),8,file);
				CaseRead(m_seControlData+i*8,sizeof(int),8,file,memory);
			}
		}
	}

	SetSeChannel(seChannel);

	return TRUE;
}


int CMessageData::GetMaxBit(int d)
{
	if (d == 0) return 0;
	if (d == 1) return 1;

	for (int i=31;i>=0;i--)
	{
		if (m_bitPattern[i] & d) return i+1;
	}

	return 0;
}


void CMessageData::CreateSeControl(int n)
{
	int sz = n * 8;
	
	if (sz > m_seControlDataSize)
	{
		int* tmp = new int[sz];
		if (m_seControlDataSize > 0)
		{
			memcpy(tmp,m_seControlData,m_seControlDataSize*sizeof(int));
		}
		ZeroMemory(tmp+m_seControlDataSize,(sz-m_seControlDataSize)*sizeof(int));

		DELETEARRAY(m_seControlData);
		m_seControlDataSize = sz;
		m_seControlData = tmp;
	}

	m_seControlFlag |= (1 << (n-1));

}


void CMessageData::CreateVoiceControl(int n)
{
	int sz = n * 8;
	
	if (sz > m_voiceControlDataSize)
	{
		int* tmp = new int[sz];
		if (m_voiceControlDataSize > 0)
		{
			memcpy(tmp,m_voiceControlData,m_voiceControlDataSize*sizeof(int));
		}
		ZeroMemory(tmp+m_voiceControlDataSize,(sz-m_voiceControlDataSize)*sizeof(int));

		DELETEARRAY(m_voiceControlData);
		m_voiceControlDataSize = sz;
		m_voiceControlData = tmp;
	}

	m_voiceControlFlag |= (1<<(n-1));

}

BOOL CMessageData::Save(FILE* file,CUndoMemoryObject* memory)
{
//	fwrite("--MESSAGEDATA  ",sizeof(char),16,file);
	CaseWrite("--MESSAGEDATA  ",sizeof(char),16,file,memory);
	int tmp[16+16];
	int i;
	for (i=0;i<16+16;i++) tmp[i] = 0;


	if ((m_voiceFileFlag == FALSE) || (m_voiceFileName == FALSE))
	{
		m_voiceFileFlag = FALSE;
		m_voiceLockFlag = FALSE;
	}

	tmp[0] = m_messageMode;
	tmp[1] = m_drawCount;
	tmp[2] = m_voiceCharaNumber;
	tmp[3] = m_voiceNumber;
	tmp[4] = m_seNumber[0];
	tmp[5] = m_voiceFileFlag;
	tmp[6] = m_seFileFlag;
	tmp[7] = m_serialNumber;
	tmp[8] = m_renameFileFlag;
	tmp[9] = m_voiceLockFlag;

	tmp[10] = 1;//version 2

	tmp[14] = m_messageColor;
	tmp[15] = m_kakuchoWorkKosuu;

	if (m_kakuchoWorkKosuu>0)
	{
		tmp[16] = m_cannotClickFlag;
	}

	if (m_kakuchoWorkKosuu>2)
	{
		tmp[17] = m_autoMessageTime;
		tmp[18] = m_autoMessageSpeed;
	}

	if (m_kakuchoWorkKosuu>3)
	{
		tmp[19] = m_expStatus;
	}

	if (m_kakuchoWorkKosuu>4)
	{
		tmp[20] = m_messageFontSizeType;
	}

	if (m_kakuchoWorkKosuu > 5)
	{
		tmp[21] = m_face;
	}

	if (m_kakuchoWorkKosuu > 6)
	{
		tmp[22] = m_mustFace;
	}

	if (m_kakuchoWorkKosuu > 7)
	{
		tmp[23] = m_musicFade;
	}

	if (m_kakuchoWorkKosuu > 8)
	{
		tmp[24] = m_messageEffect;
	}

	if (m_kakuchoWorkKosuu > 9)
	{
		tmp[25] = m_messageEffectTime;
	}

 	int messageSize = strlen(m_messageBuffer);
	if (messageSize <= 512)
	{
		messageSize = 512;
	}
	else
	{
		int sz = (messageSize + 511) / 512;
		sz *= 512;
		messageSize = sz;
	}

	tmp[11] = messageSize;
	tmp[12] = m_seControlFlag;
	tmp[13] = m_voiceControlFlag;


//	fwrite(tmp,sizeof(int),16,file);
	CaseWrite(tmp,sizeof(int),16,file,memory);
	if (m_kakuchoWorkKosuu>0)
	{
		//fwrite(&tmp[16],sizeof(int),m_kakuchoWorkKosuu,file);
		CaseWrite(&tmp[16],sizeof(int),m_kakuchoWorkKosuu,file,memory);
	}

//	fwrite(m_messageBuffer,sizeof(char),messageSize,file);
	CaseWrite(m_messageBuffer,sizeof(char),messageSize,file,memory);






	int flg = m_voiceFileFlag;
	for (i=0;i<16;i++)
	{
		if (flg & 1)
		{
//			fwrite(m_voiceFileName+i*64,sizeof(char),64,file);
			CaseWrite(m_voiceFileName+i*64,sizeof(char),64,file,memory);
		}

		if ((flg  == 0) || (flg == 1)) break;

		flg >>= 1;
	}


//	if (version >= 1)
	if (1)
	{
		//load 1-15 by 0は前の場所に
		if (m_seFileFlag > 1)
		{
			BOOL flg0 = m_seFileFlag;
			flg0 >>= 1;

			for (int i=1;i<16;i++)
			{
				if (flg0 & 1)
				{
//					fwrite(&m_seNumber[i],sizeof(int),1,file);
					CaseWrite(&m_seNumber[i],sizeof(int),1,file,memory);
				}

				if ((flg0  == 0) || (flg0 == 1)) break;

				flg0 >>= 1;
			}
		}
	}


	flg = m_seFileFlag;
	for (i=0;i<16;i++)
	{
		if (flg & 1)
		{
//			fwrite(m_seFileName+i*64,sizeof(char),64,file);
			CaseWrite(m_seFileName+i*64,sizeof(char),64,file,memory);
		}

		if ((flg  == 0) || (flg == 1)) break;

		flg >>= 1;
	}


	if (m_voiceControlFlag)
	{
		for (i=0;i<16;i++)
		{
			if (m_voiceControlFlag & m_bitPattern[i])
			{
//				fwrite(m_voiceControlData+i*8,sizeof(int),8,file);
				CaseWrite(m_voiceControlData+i*8,sizeof(int),8,file,memory);
			}
		}
	}

	if (m_seControlFlag)
	{
		for (i=0;i<16;i++)
		{
			if (m_seControlFlag & m_bitPattern[i])
			{
//				fwrite(m_seControlData+i*8,sizeof(int),8,file);
				CaseWrite(m_seControlData+i*8,sizeof(int),8,file,memory);
			}
		}
	}


	return TRUE;
}


void CMessageData::SetVoiceFileName(LPSTR name,int voiceChannel)
{
	int voiceFileSize = CalcuNeedVoiceFileSize(1 << voiceChannel);
	CheckAndExpandVoiceWork(voiceFileSize);
//	if (m_voiceFileName == NULL) m_voiceFileName = new char[64];


	int ln = strlen(name);
	if (ln>62) ln = 62;
	if (ln>0) memcpy(m_voiceFileName+voiceChannel*64,name,ln);
	m_voiceFileName[voiceChannel*64+ln] = 0;
	m_voiceFileName[voiceChannel*64+ln+1] = 0;
}


void CMessageData::SetSeFileName(LPSTR name,int seChannel)
{
	int seFileSize = CalcuNeedSeFileSize(1 << seChannel);


	CheckAndExpandSeWork(seFileSize);
//	if (m_seFileName == NULL) m_seFileName = new char[64];

	int ln = strlen(name);
	if (ln>62) ln = 62;
	memcpy(m_seFileName+seChannel*64,name,ln);
	m_seFileName[seChannel*64+ln] = 0;
	m_seFileName[seChannel*64+ln+1] = 0;
}




void CMessageData::Init(LPVOID para)
{
	m_voiceCharaNumber = 0;
	m_voiceNumber = 0;
	m_voiceFileFlag = 0;
	m_seFileFlag = 0;
	m_expStatus = 0;
	m_messageEffect = 0;
	m_messageEffectTime = 0;

	m_kakuchoWorkKosuu = NOWKAKUCHOKOSUU;
	m_cannotClickFlag = 0;
	m_autoMessageTime = 0;
	m_autoMessageSpeed = 0;
	m_messageFontSizeType = 0;
	m_face = 0;
	m_mustFace = 0;
	m_musicFade = 0;

	for (int i=0;i<16;i++)
	{
		m_seNumber[i] = 0;
	}

	m_renameFileFlag = FALSE;
	
	m_serialNumber = 0;
	m_messageMode = 0;

	m_voiceChannel = 0;
	m_seChannel = 0;

	m_messageColor = 0;

	m_voiceLockFlag = 0;

	if (m_messageMode == 3)
	{
		m_drawCount = m_defaultDrawCount;
	}
	else
	{
		SetMessageData("ここに文章を入れるのにゃ");
	}

}


LPSTR CMessageData::GetMyName(void)
{
	return NULL;
}

void CMessageData::SetMyName(LPSTR name)
{
}

CCase* CMessageData::NewObject(void)
{
	return NULL;
}

void CMessageData::SetVoicePlayerNumber(int n,int voiceChannel)
{
	if (voiceChannel == 0)
	{
		int d = m_voiceCharaNumber & 0xffff0000;
		d |= n;
		m_voiceCharaNumber = d;
	}
	else
	{
		int d = m_voiceCharaNumber & 0x0ffff;
		d |= (n<<16);
		m_voiceCharaNumber = d;
	}
}

void CMessageData::SetVoiceNumber(int n,int voiceChannel)
{
//	m_voiceNumber = n;
//
//	return;
	if (voiceChannel == 0)
	{
		int d = m_voiceNumber & 0xffff0000;
		d |= n;
		m_voiceNumber = d;
	}
	else
	{
		int d = m_voiceNumber & 0x0ffff;
		d |= (n<<16);
		m_voiceNumber = d;
	}
}


void CMessageData::SetVoiceFlag(BOOL flg,int voiceChannel)
{
	int mask = 1<<voiceChannel;
	
	if (flg)
	{
		m_voiceFileFlag |= mask;
		SetVoiceStop(voiceChannel,FALSE);
	}
	else
	{
		m_voiceFileFlag &= (~mask);
		SetVoiceStop(voiceChannel,FALSE);
		if (CheckVoiceVolumeExist(voiceChannel))
		{
			SetVoiceVolume(0,voiceChannel);
		}
		ClearAllVoiceEffect(voiceChannel);
	}
}


void CMessageData::SetSEFlag(BOOL flg,int seChannel)
{
	int mask = 1<<seChannel;

	if (flg)
	{
		m_seFileFlag |= mask;
	}
	else
	{
		m_seFileFlag &= (~mask);
		ClearAllSeEffect(seChannel);
	}
}


int CMessageData::GetVoiceFlag(void)
{
	return m_voiceFileFlag;
}

BOOL CMessageData::CheckVoiceFlag(int voiceChannel)
{
	if ((1<<voiceChannel) & m_voiceFileFlag) return TRUE;
	return FALSE;
}

int CMessageData::GetSEFlag(void)
{
	return m_seFileFlag;
}



BOOL CMessageData::CheckSEFlag(int seChannel)
{
	if ((1<<seChannel) & m_seFileFlag) return TRUE;

	return FALSE;
}


int CMessageData::GetVoicePlayerNumber(int voiceChannel)
{
	if (voiceChannel == 0) return m_voiceCharaNumber & 0xffff;
	return (m_voiceCharaNumber >> 16) & 0xffff;
}


int CMessageData::GetVoiceNumber(int voiceChannel)
{
	if (voiceChannel == 0)
	{
		return m_voiceNumber & 0xffff;
	}
	else if (voiceChannel == 1)
	{
		return (m_voiceNumber>>16) & 0xffff;
	}

	return 0;
}


LPSTR CMessageData::GetVoiceFileName(int voiceChannel)
{
	if (m_voiceFileName == NULL) return NULL;

	return m_voiceFileName+voiceChannel*64;
}


//dummy routine?
LPSTR CMessageData::GetSEFileName(int seChannel)
{
	if (m_seFileName == NULL) return NULL;

	return m_seFileName + seChannel*64;
}

void CMessageData::SetSENumber(int n,int seChannel)
{
	m_seNumber[seChannel] = n;
	if (n > 0)
	{
		m_seFileFlag |= (1<<seChannel);
	}
	else
	{
		m_seFileFlag &= ~(1<<seChannel);
	}
}


int CMessageData::GetSENumber(int seChannel)
{
	return m_seNumber[seChannel];
}


BOOL CMessageData::MakeBook(FILE* file,BOOL bPrintFileNameFlag,BOOL enphasizFlag,int defNam)
{
	int ln = strlen(m_messageBuffer);

	if (enphasizFlag)
	{
		WriteWhiteSpace(file);
		fwrite(m_emphasis1,sizeof(char),62,file);
	}

	if (m_renameFileFlag)
	{
		fwrite(m_haraponnobaka,sizeof(char),36,file);
	}

//	if (enphasizFlag)
//	{
//		if (_stricmp(m_voiceFileName,"ff090047") == 0)
//		{
//			int hhh = 0;
//			hhh++;
//		}
//	}


	if (bPrintFileNameFlag)
	{
		if (m_voiceFileFlag)
		{
			for (int i=0;i<2;i++)
			{
				if (m_voiceFileFlag & m_bitPattern[i])
				{
					char voice[1024];


					LPSTR voiceFileName = m_voiceFileName;


				//	if ((m_voiceFileFlag & 1) == 0)
					if (i == 1)
					{
						if (m_voiceFileFlag & 2)
						{
							voiceFileName += 64;
						}
					}

					WriteWhiteSpace(file);

					if (enphasizFlag)
					{
						wsprintf(voice,"┃                                FILENAME=%s.wav   ┃\r\n",voiceFileName);
					}
					else
					{
						wsprintf(voice,"                                FILENAME=%s.wav\r\n",voiceFileName);
					}

					fwrite(voice,sizeof(char),strlen(voice),file);

					if (defNam)
					{
						char voice2[1024];
						char name2[1024];
						int dln = strlen(voiceFileName);
						if (dln > 1022) dln = 1022;
						if (dln > 0) memcpy(name2,voiceFileName,dln);
						name2[dln] = 0;
						name2[3] += 'a' -'0';
						if (enphasizFlag)
						{
							wsprintf(voice2,"┃                                FILENAME=%s.wav   ┃\r\n",name2);
						}
						else
						{
							wsprintf(voice2,"                                FILENAME=%s.wav\r\n",name2);
						}

						int lll = strlen(voice2);

						fwrite(voice2,sizeof(char),strlen(voice2),file);

					}

				}
			}
		}

		//サブボイスは?
	}




	//
	int defloop = 1;
	if (defNam) defloop=2;

	for (int di = 0;di<defloop;di++)
	{
		if (di != 0)
		{
			if (enphasizFlag)
			{
				fwrite(m_emphasis9,sizeof(char),60,file);
			}
			
			fwrite("\r\n",sizeof(char),2,file);
			

		}

		if (enphasizFlag)
		{
			int gyo = GetMessageGyo();
			BOOL alreadyFlag = FALSE;



			for (int i=0;i<gyo;i++)
			{
				BOOL nameFlag = FALSE;
				int mlen = GetMessage1Gyo(i);

				if (alreadyFlag == FALSE)
				{
					if ((*m_tmpMessage) != '/')
					{
						if (*(m_tmpMessage+1) != '/')
						{
							if (m_voiceFileFlag)
							{
								nameFlag = TRUE;
							}
							alreadyFlag = TRUE;
						}
					}
				}

				if (di > 0)
				{
					if ((*m_tmpMessage) == '/')
					{
						if (*(m_tmpMessage+1) == '/')
						{
							continue;
						}
					}
				}


				if (nameFlag)
				{
					if (m_whiteSpace == 0)
					{
						WriteWhiteSpace(file);
						fwrite("┃【",sizeof(char),4,file);
					}
					else
					{
						fwrite("【",sizeof(char),2,file);
					}
				}
				else
				{
					WriteWhiteSpace(file);
					fwrite("┃",sizeof(char),2,file);
				}

				if (di == 0)
				{
					fwrite(m_tmpMessage,sizeof(char),mlen,file);
				}
				else
				{
					char tmp2[4096];
					DefNameReplace(m_tmpMessage,tmp2,defNam);
					fwrite(tmp2,sizeof(char),strlen(tmp2),file);
					//change mlen
					mlen = strlen(tmp2);


				}

				if (nameFlag)
				{
					fwrite("】",sizeof(char),2,file);
					if (m_whiteSpace > 0)
					{

						int amari = m_whiteSpace - mlen - 2 - 2;

						if (amari > 0)
						{
							fwrite(m_whiteSpaceStr,sizeof(char),amari,file);

						}







						fwrite("┃",sizeof(char),2,file);
					}
				}

				int amari = 60-2-mlen-2;
				if (nameFlag) amari -= 4;

				if (amari > 0)
				{
					if (nameFlag)
					{
						if (m_whiteSpace > 0)
						{
							amari += (mlen + 4);
						}

					}

					if (amari>64) amari = 64;
					int pt = 64 - amari;
					fwrite(m_emphasis3+pt,sizeof(char),amari,file);
				}





	//			if ((!nameFlag) || (m_whiteSpace == 0))
	//			{
					fwrite("┃\r\n",sizeof(char),4,file);
	//			}
	//			else
	//			{
	//				fwrite("\r\n",sizeof(char),2,file);
	//			}
			}
		}
		else
		{
			if (m_whiteSpace == 0)
			{
				if (di == 0)
				{
					fwrite(m_messageBuffer,sizeof(char),ln,file);
				}
				else
				{
					fwrite("\r\n",sizeof(char),2,file);

					int gyo = GetMessageGyo();
					for (int i=0;i<gyo;i++)
					{
						int mlen = GetMessage1Gyo(i);

						if ((*m_tmpMessage) != '/')
						{
							if (*(m_tmpMessage+1) != '/')
							{

								char tmp2[4096];
								DefNameReplace(m_tmpMessage,tmp2,defNam);
								fwrite(tmp2,sizeof(char),strlen(tmp2),file);
								fwrite("\r\n",sizeof(char),2,file);
							}
						}
					}


//					char tmp2[4096];
//					DefNameReplace(m_messageBuffer,tmp2,defNam);
//					fwrite(tmp2,sizeof(char),strlen(tmp2),file);
				}
			}
			else
			{


				int gyo = GetMessageGyo();
				BOOL alreadyFlag = FALSE;

				for (int i=0;i<gyo;i++)
				{
					BOOL nameFlag = FALSE;
					int mlen = GetMessage1Gyo(i);

					if (alreadyFlag == FALSE)
					{
						if ((*m_tmpMessage) != '/')
						{
							if (*(m_tmpMessage+1) != '/')
							{
								if (m_voiceFileFlag)
								{
									nameFlag = TRUE;
								}
								alreadyFlag = TRUE;
							}
						}
					}

					if (!nameFlag)
					{
						WriteWhiteSpace(file);
					}

	//			if (nameFlag)
	//			{
	//				fwrite("┃【",sizeof(char),4,file);
	//			}
	//			else
	//			{
	//				fwrite("┃",sizeof(char),2,file);
	//			}

					if (di == 0)
					{
						fwrite(m_tmpMessage,sizeof(char),mlen,file);
						fwrite("\r\n",sizeof(char),2,file);
					}
					else
					{
						if ((*m_tmpMessage) != '/')
						{
							if (*(m_tmpMessage+1) != '/')
							{

								char tmp2[4096];
								DefNameReplace(m_tmpMessage,tmp2,defNam);
								fwrite(tmp2,sizeof(char),strlen(tmp2),file);
								fwrite("\r\n",sizeof(char),2,file);
							}
						}
					}


	//			if (nameFlag)
	//			{
	//				fwrite("】",sizeof(char),2,file);
	//			}

	//			int amari = 60-2-mlen-2;
	//			if (nameFlag) amari -= 4;
	//
	//			if (amari > 0)
	//			{
	//				if (amari>64) amari = 64;
	//				int pt = 64 - amari;
	//				fwrite(m_emphasis3+pt,sizeof(char),amari,file);
	//			}
	//			fwrite("┃\r\n",sizeof(char),4,file);



				}


			}
		}
	}



	if (enphasizFlag)
	{
		WriteWhiteSpace(file);
		fwrite(m_emphasis2,sizeof(char),62,file);
		fwrite(m_separator,sizeof(char),80,file);
	}
	else
	{
		fwrite(m_separator,sizeof(char),80,file);
	}

	return TRUE;
}

void CMessageData::DefNameReplace(char* org,char* tmp2,int defNum)
{
	char src0[4096];
	int ln = strlen(org);
	if (ln>4094) ln = 4094;
	memcpy(src0,org,ln);
	src0[ln] = 0;
	src0[ln+1] = 0;


	char key1[] = "#名";
	char key2[] = "#姓";

	char* src = src0;
	char* key = key1;
	if (defNum == 1)
	{
		char* key = key1;
	}
	if (defNum == 2)
	{
		char* key = key2;
	}


	char* rep = m_defaultName;



	while (char* ptr = strstr(src,key))
	{
		*ptr = 0;
		memcpy(tmp2,src,strlen(src));

		tmp2 += strlen(src);
		src += strlen(src);
		src += strlen(key);

		memcpy(tmp2,rep,strlen(rep));
		tmp2 += strlen(rep);
	}

	int ln2 = strlen(src);
	if (ln2>0)
	{
		memcpy(tmp2,src,ln2);
		tmp2 += ln2;
	}
	*tmp2 = 0;
}



BOOL CMessageData::CheckExistChara(int charaNumber,int channel)
{
	if (m_messageMode == 3) return FALSE;

	if (m_voiceFileFlag == FALSE) return FALSE;
	if (GetVoicePlayerNumber(channel) == charaNumber) return TRUE;

	return FALSE;
}


int CMessageData::Pass1(void)
{
	int sz = 0;

	
	if (m_voiceFileFlag) sz += 4;
	if (m_seFileFlag) sz += 4;


	if (m_messageMode == 3)
	{
		sz += 4;
	}
	else
	{
		sz += 4;
	}

	return sz;
}

void CMessageData::SetRenameFlag(BOOL b)
{
	m_renameFileFlag = b;
}

BOOL CMessageData::GetRenameFlag(void)
{
	return m_renameFileFlag;
}

void CMessageData::SetID(int id)
{
	m_serialNumber = id;
}

int CMessageData::GetID(void)
{
	return m_serialNumber;
}

BOOL CMessageData::CheckVoiceLock(int n)
{
	if (m_voiceLockFlag & m_bitPattern[n]) return TRUE;

	return FALSE;
}

void CMessageData::SetVoiceLock(BOOL flg,int n)
{
	int bt = m_bitPattern[n];
	int bt2 = ~bt;

	m_voiceLockFlag &= bt2;

	if (flg)
	{
		m_voiceLockFlag |= bt;
	}
//	m_voiceLockFlag = flg;
}

BOOL CMessageData::CopyOriginalData(CCase* lpCase)
{
	return TRUE;
}

int CMessageData::Clean(void)
{
	return 0;
}


int CMessageData::GetMessageGyo(void)
{
	if (m_messageBuffer == NULL) return 0;

	int kosuu = 0;
	int ln = strlen(m_messageBuffer);
	int ptr = 0;

	while (ptr<ln)
	{
		if (*(m_messageBuffer+ptr) == 0x00d)
		{
			kosuu++;
			ptr++;
		}
		ptr++;
	}

	if (*(m_messageBuffer+ln-2) != 0x00d)
	{
		kosuu++;
	}

	return kosuu;
}



int CMessageData::GetMessage1Gyo(int n)
{
	if (m_messageBuffer == NULL) return 0;

	int ln = strlen(m_messageBuffer);
	
	int ptr = 0;

	int found = -1;
	if (n == 0) found = 0;
	int k = 0;

	if (n>0)
	{
		while (ptr<ln)
		{
			if (*(m_messageBuffer+ptr) == 0x00d)
			{
				k++;
				ptr += 2;
				if (n == k)
				{
					found = ptr;
					break;
				}
			}
			else
			{
				ptr++;
			}
		}
	}

	if (found == -1) return 0;

	int ptr2 = found;
	int ln2 = 0;

	while (ptr2<ln)
	{
		if (*(m_messageBuffer+ptr2) == 0x00d)
		{
			memcpy(m_tmpMessage,m_messageBuffer+found,ln2);
			m_tmpMessage[ln2] = 0;
			m_tmpMessage[ln2+1] = 0;
			return ln2;
		}

		ptr2++;
		ln2++;
	}

	memcpy(m_tmpMessage,m_messageBuffer+found,ln2);
	m_tmpMessage[ln2] = 0;
	m_tmpMessage[ln2+1] = 0;


	return ln2;
}



//
// voice control data
//
// vol,FLAG(.... loop(なし),doppler,teii2,move,teii1,stop),teii1,teii2,time,*,*,*
//

BOOL CMessageData::CheckVoiceControlFlag(int channel)
{
	if (m_voiceControlFlag == 0) return FALSE;
	int kosuuMax = GetMaxBit(m_voiceControlFlag);
	if (kosuuMax <= channel) return FALSE;
	if ((m_voiceControlFlag & m_bitPattern[channel]) == 0) return FALSE;

	return TRUE;
}

BOOL CMessageData::CheckVoiceVolumeExist(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return FALSE;

	if (m_voiceControlData[channel*8] == 0) return FALSE;

	return TRUE;
}

void CMessageData::SetVoiceVolume(int vol,int channel)
{
	m_voiceControlFlag |= m_bitPattern[channel];
	int kosuuMax = GetMaxBit(m_voiceControlFlag);
	CreateVoiceControl(kosuuMax);

	m_voiceControlData[channel*8] = vol;
}


int CMessageData::GetVoiceVolume(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return 0;
	return m_voiceControlData[channel*8];
}


void CMessageData::SetVoiceStop(int channel,BOOL stopFlag)
{
	if (stopFlag)
	{
		m_voiceControlFlag |= m_bitPattern[channel];
		int kosuuMax = GetMaxBit(m_voiceControlFlag);
		CreateVoiceControl(kosuuMax);
		m_voiceControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_STOP;
	}
	else
	{
		if (CheckVoiceControlFlag(channel) == FALSE) return;
		m_voiceControlData[channel*8+1] &= ~VOICE_SE_FLAG_BIT_STOP;
	}
}


BOOL CMessageData::CheckVoiceOff(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return FALSE;

	if (m_voiceControlData[channel*8+1] & VOICE_SE_FLAG_BIT_STOP) return TRUE;

	return FALSE;
}

void CMessageData::SetVoiceContinue(int channel, BOOL continueFlag)
{
	if (continueFlag)
	{
		m_voiceControlFlag |= m_bitPattern[channel];
		int kosuuMax = GetMaxBit(m_voiceControlFlag);
		CreateVoiceControl(kosuuMax);
		m_voiceControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_VOICECONTINUE;
	}
	else
	{
		if (CheckVoiceControlFlag(channel) == FALSE) return;
		m_voiceControlData[channel*8+1] &= ~VOICE_SE_FLAG_BIT_VOICECONTINUE;
	}
}



BOOL CMessageData::CheckVoiceContinue(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return FALSE;

	if (m_voiceControlData[channel*8+1] & VOICE_SE_FLAG_BIT_VOICECONTINUE) return TRUE;

	return FALSE;
}

void CMessageData::ChangeVoiceComplete(int channel)
{
	m_voiceControlFlag |= m_bitPattern[channel];
	int kosuuMax = GetMaxBit(m_voiceControlFlag);
	CreateVoiceControl(kosuuMax);
	m_voiceControlData[channel * 8 + 1] ^= VOICE_SE_FLAG_BIT_VOICECOMPLETE;
}

bool CMessageData::CheckVoiceComplete(int channel)
{
	if (CheckVoiceControlFlag(channel) == false) return false;

	if (m_voiceControlData[channel * 8 + 1] & VOICE_SE_FLAG_BIT_VOICECOMPLETE) return true;

	return false;
}

void CMessageData::ChangeVoiceNoWaitSameCharaVoice(int channel)
{
	m_voiceControlFlag |= m_bitPattern[channel];
	int kosuuMax = GetMaxBit(m_voiceControlFlag);
	CreateVoiceControl(kosuuMax);
	m_voiceControlData[channel * 8 + 1] ^= VOICE_SE_FLAG_BIT_NOWAITSAMECHARA;
}

bool CMessageData::CheckVoiceNoWaitSameCharaVoice(int channel)
{
	if (CheckVoiceControlFlag(channel) == false) return false;

	if (m_voiceControlData[channel * 8 + 1] & VOICE_SE_FLAG_BIT_NOWAITSAMECHARA) return true;

	return false;
}


BOOL CMessageData::CheckVoiceTeiiExist(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return FALSE;
	if ((m_voiceControlData[channel*8+1] & VOICE_SE_FLAG_BIT_TEII) == 0) return FALSE;
	return TRUE;
}


void CMessageData::SetVoiceTeii(int xyz,int d,int channel,int cal)
{
	CreateVoiceControl(channel+1);

	int x = m_voiceControlData[channel*8+2+cal] & 0xff;
	int y = (m_voiceControlData[channel*8+2+cal]>>8) & 0xff;
	int z = (m_voiceControlData[channel*8+2+cal]>>16) & 0xff;

	if (x>=128) x -= 256;
	if (y>=128) y -= 256;
	if (z>=128) z -= 256;

	if (xyz == 0) x = d;
	if (xyz == 1) y = d;
	if (xyz == 2) z = d;

	if (x<0) x += 256;
	if (y<0) y += 256;
	if (z<0) z += 256;

	x &= 0xff;
	y &= 0xff;
	z &= 0xff;

	m_voiceControlData[channel*8+2+cal] = (z<<16) | (y<<8) | x;

	if (cal == 0)
	{
		m_voiceControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_TEII;
	}
	else if (cal == 1)
	{
		m_voiceControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_TEII2;
	}
}

int CMessageData::GetVoiceTeii(int xyz,int channel,int cal)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return FALSE;

	int x = m_voiceControlData[channel*8+2+cal] & 0xff;
	int y = (m_voiceControlData[channel*8+2+cal]>>8) & 0xff;
	int z = (m_voiceControlData[channel*8+2+cal]>>16) & 0xff;
	if (x>=128) x -= 256;
	if (y>=128) y -= 256;
	if (z>=128) z -= 256;

	if (xyz == 0) return x;
	if (xyz == 1) return y;
	if (xyz == 2) return z;

	return 0;
}

BOOL CMessageData::CheckVoiceMoveFlag(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;
	if ((m_seControlData[channel*8+1] & VOICE_SE_FLAG_BIT_MOVE) == 0) return FALSE;
	return TRUE;
}

void CMessageData::SetVoiceMoveFlag(BOOL flg,int channel)
{
	CreateVoiceControl(channel+1);
	if (flg)
	{
		m_voiceControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_MOVE;
	}
	else
	{
		m_voiceControlData[channel*8+1] &= ~VOICE_SE_FLAG_BIT_MOVE;
	}
}

BOOL CMessageData::CheckVoiceTeii2Exist(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return FALSE;
	if ((m_voiceControlData[channel*8+1] & VOICE_SE_FLAG_BIT_STOP) == 0) return FALSE;
	return TRUE;
}


void CMessageData::SetVoiceTeii2(int xyz,int d,int channel)
{
	SetVoiceTeii(xyz,d,channel,1);
}

int CMessageData::GetVoiceTeii2(int xyz,int channel)
{
	return GetVoiceTeii(xyz,channel,1);
}

BOOL CMessageData::CheckVoiceDopplerSoutou(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return FALSE;
	if ((m_voiceControlData[channel*8+1] & VOICE_SE_FLAG_BIT_DOPPLER) == 0) return FALSE;
	return TRUE;
}


void CMessageData::SetVoiceDopplerSoutou(BOOL flg,int channel)
{
	CreateVoiceControl(channel+1);
	m_voiceControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_DOPPLER;
}


int CMessageData::GetVoiceMoveTime(int channel)
{
	if (CheckVoiceMoveFlag(channel) == FALSE) return 0;
	if (CheckVoiceControlFlag(channel) == FALSE) return 0;

	return m_voiceControlData[channel*8+4];
}

void CMessageData::SetVoiceMoveTime(int tm,int channel)
{
	if (tm != 0)
	{
		SetVoiceMoveFlag(TRUE,channel);
	}
	else
	{
		SetVoiceMoveFlag(FALSE,channel);
	}

	m_voiceControlData[channel*8+4] = tm;
}

BOOL CMessageData::CheckVoiceLoop(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return FALSE;
	if ((m_voiceControlData[channel*8+1] & VOICE_SE_FLAG_BIT_LOOP) == 0) return FALSE;
	return TRUE;
}

void CMessageData::SetVoiceLoop(BOOL flg,int channel)
{
	CreateVoiceControl(channel+1);
	if (flg)
	{
		m_voiceControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_LOOP;
	}
	else
	{
		m_voiceControlData[channel*8+1] &= ~VOICE_SE_FLAG_BIT_LOOP;
	}
}


void CMessageData::CheckAllVoiceEffectClear(int channel)
{
	if (CheckVoiceMoveFlag(channel)) return;
	
	if (CheckVoiceTeiiExist(channel))
	{
		//all0?
		if (GetVoiceTeii(0,channel) != 0) return;
		if (GetVoiceTeii(1,channel) != 0) return;
		if (GetVoiceTeii(2,channel) != 0) return;

		m_voiceControlData[channel*8+1] &= ~VOICE_SE_FLAG_BIT_TEII;
	}
}


//
// set control data
//
// vol,FLAG(.... loop,doppler,teii2,move,teii1,stop),teii1,teii2,time,*,*,*
//
//

BOOL CMessageData::CheckSeControlFlag(int channel)
{
	if (m_seControlFlag == 0) return FALSE;
	int kosuuMax = GetMaxBit(m_seControlFlag);
	if (kosuuMax <= channel) return FALSE;
	if ((m_seControlFlag & m_bitPattern[channel]) == 0) return FALSE;

	return TRUE;
}

BOOL CMessageData::CheckSeStop(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;

	if (m_seControlData[channel*8+1] & VOICE_SE_FLAG_BIT_STOP) return TRUE;
	return FALSE;
}

void CMessageData::SetSeStop(BOOL stopFlag,int channel)
{
	CreateSeControl(channel+1);
	
	if (stopFlag)
	{
		m_seControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_STOP;
	}
	else
	{
		m_seControlData[channel*8+1] &= ~VOICE_SE_FLAG_BIT_STOP;
	}
}


BOOL CMessageData::CheckSeVolumeExist(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;
	if (m_seControlData[channel*8] == 0) return FALSE;

	return TRUE;
}

int CMessageData::GetSeVolume(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return 0;
	return m_seControlData[channel*8];
}

void CMessageData::SetSeVolume(int volume,int channel)
{
	CreateSeControl(channel+1);
	m_seControlData[channel*8] = volume;
}

BOOL CMessageData::CheckSeTeiiExist(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;
	if ((m_seControlData[channel*8+1] & 0x2) == 0) return FALSE;
	return TRUE;
}

void CMessageData::SetSeTeii(int xyz,int d,int channel,int cal)
{
	CreateSeControl(channel+1);

	int x = m_seControlData[channel*8+2+cal] & 0xff;
	int y = (m_seControlData[channel*8+2+cal]>>8) & 0xff;
	int z = (m_seControlData[channel*8+2+cal]>>16) & 0xff;

	if (x>=128) x -= 256;
	if (y>=128) y -= 256;
	if (z>=128) z -= 256;

	if (xyz == 0) x = d;
	if (xyz == 1) y = d;
	if (xyz == 2) z = d;

	if (x<0) x += 256;
	if (y<0) y += 256;
	if (z<0) z += 256;

	x &= 0xff;
	y &= 0xff;
	z &= 0xff;

	m_seControlData[channel*8+2+cal] = (z<<16) | (y<<8) | x;

	if (cal == 0)
	{
		m_seControlData[channel*8+1] |= 0x2;
	}
	else if (cal == 1)
	{
		m_seControlData[channel*8+1] |= 0x8;
	}
}

int CMessageData::GetSeTeii(int xyz,int channel,int cal)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;

	int x = m_seControlData[channel*8+2+cal] & 0xff;
	int y = (m_seControlData[channel*8+2+cal]>>8) & 0xff;
	int z = (m_seControlData[channel*8+2+cal]>>16) & 0xff;
	if (x>=128) x -= 256;
	if (y>=128) y -= 256;
	if (z>=128) z -= 256;

	if (xyz == 0) return x;
	if (xyz == 1) return y;
	if (xyz == 2) return z;

	return 0;
}

BOOL CMessageData::CheckSeMoveFlag(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;
	if ((m_seControlData[channel*8+1] & VOICE_SE_FLAG_BIT_MOVE) == 0) return FALSE;
	return TRUE;
}

void CMessageData::SetSeMoveFlag(BOOL flg,int channel)
{
	CreateSeControl(channel+1);
	if (flg)
	{
		m_seControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_MOVE;
	}
	else
	{
		m_seControlData[channel*8+1] &= ~VOICE_SE_FLAG_BIT_MOVE;
	}
}


BOOL CMessageData::CheckSeTeii2Exist(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;
	if ((m_seControlData[channel*8+1] & VOICE_SE_FLAG_BIT_TEII2) == 0) return FALSE;
	return TRUE;
}

void CMessageData::SetSeTeii2(int xyz,int d,int channel)
{
	SetSeTeii(xyz,d,channel,1);
}

int CMessageData::GetSeTeii2(int xyz,int channel)
{
	return GetSeTeii(xyz,channel,1);
}


BOOL CMessageData::CheckDopplerSoutou(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;
	if ((m_seControlData[channel*8+1] & VOICE_SE_FLAG_BIT_DOPPLER) == 0) return FALSE;
	return TRUE;
}

void CMessageData::SetDopplerSoutou(BOOL flg,int channel)
{
	CreateSeControl(channel+1);
	m_seControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_DOPPLER;
}

int CMessageData::GetSeMoveTime(int channel)
{
	if (CheckSeMoveFlag(channel) == FALSE) return 0;
	if (CheckSeControlFlag(channel) == FALSE) return 0;

	return m_seControlData[channel*8+4];
}

void CMessageData::SetSeMoveTime(int tm,int channel)
{
	if (tm != 0)
	{
		SetSeMoveFlag(TRUE,channel);
	}
	else
	{
		SetSeMoveFlag(FALSE,channel);
	}


	m_seControlData[channel*8+4] = tm;
}

void CMessageData::SetMessageFontSizeType(int type)
{
	m_messageFontSizeType = type;
}

void CMessageData::SetFace(int face)
{
	m_face = face;
}

void CMessageData::SetMustFace(int mustFace)
{
	m_mustFace &= 0xffff0000;
	m_mustFace |= mustFace;
}

void CMessageData::SetFixFace(int fixFace)
{
	m_mustFace &= 0x0000ffff;
	m_mustFace |= (fixFace << 16);
}


BOOL CMessageData::CheckSeLoop(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;
	if ((m_seControlData[channel*8+1] & VOICE_SE_FLAG_BIT_LOOP) == 0) return FALSE;
	return TRUE;
}

void CMessageData::SetSeLoop(BOOL flg,int channel)
{
	CreateSeControl(channel+1);
	if (flg)
	{
		m_seControlData[channel*8+1] |= VOICE_SE_FLAG_BIT_LOOP;
	}
	else
	{
		m_seControlData[channel*8+1] &= ~VOICE_SE_FLAG_BIT_LOOP;
	}
}


BOOL CMessageData::CheckSeEffect(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;
	if (m_seControlData[channel*8+1] & (~0x321)) return TRUE;

	return FALSE;
}


int CMessageData::GetMusicVolume(void)
{
	int vol = m_musicFade & 0x7f;
	if (m_musicFade & 0x80)
	{
		vol *= -1;
	}
	return vol;
}


int CMessageData::GetMusicFadeTime(void)
{
	return m_musicFade >> 8;
}

void CMessageData::SetMusicVolume(int vol)
{
	if (vol < -100) vol = -100;
	if (vol > 101) vol = 101;

	if (vol < 0)
	{
		vol *= -1;
		vol |= 0x80;
	}

	m_musicFade &= 0xffffff00;
	m_musicFade |= vol;
}

void CMessageData::SetMusicFadeTime(int fadeTime)
{
	fadeTime <<= 8;
	m_musicFade &= 0xff;
	m_musicFade |= fadeTime;
}





void CMessageData::CheckAllSeEffectClear(int channel)
{
	//
	if (CheckSeMoveFlag(channel)) return;
	
	if (CheckSeTeiiExist(channel))
	{
		//all0?
		if (GetSeTeii(0,channel) != 0) return;
		if (GetSeTeii(1,channel) != 0) return;
		if (GetSeTeii(2,channel) != 0) return;

		m_seControlData[channel*8+1] &= ~0x2;
	}
}



void CMessageData::ClearAllSeEffect(int channel)
{
	if (m_seControlFlag == 0) return;
	int kosuuMax = GetMaxBit(m_seControlFlag);
	if (kosuuMax <= channel) return;
	if ((m_seControlFlag & m_bitPattern[channel]) == 0) return;

	for (int i=0;i<8;i++)
	{
		m_seControlData[channel*8+i] = 0;
	}

	m_seControlFlag &= ~m_bitPattern[channel];
}

BOOL CMessageData::CheckVoiceEffect(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return FALSE;
	if (m_voiceControlData[channel*8+1] & (~0x321)) return TRUE;

	return FALSE;
}

void CMessageData::ClearAllVoiceEffect(int channel)
{
	if (m_voiceControlFlag == 0) return;
	int kosuuMax = GetMaxBit(m_voiceControlFlag);
	if (kosuuMax <= channel) return;
	if ((m_voiceControlFlag & m_bitPattern[channel]) == 0) return;

	for (int i=0;i<8;i++)
	{
		m_voiceControlData[channel*8+i] = 0;
	}

	m_voiceControlFlag &= ~m_bitPattern[channel];
}




int CMessageData::GetSEFadeInOut(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return 0;
	return (m_seControlData[channel*8+1] & 0x100)>> 8;
}

void CMessageData::SetSEFadeInOut(int flg,int channel)
{
	CreateSeControl(channel+1);

	if (flg)
	{
		m_seControlData[channel*8+1] |= 0x100;
	}
	else
	{
		m_seControlData[channel*8+1] &= ~0x100;
	}
}


int CMessageData::GetSEVolumeOnly(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return 0;
	return (m_seControlData[channel*8+1] & 0x200)>> 9;
}

void CMessageData::SetSEVolumeOnly(int flg ,int channel)
{
	CreateSeControl(channel+1);

	if (flg)
	{
		for (int i=0;i<8;i++)
		{
			m_seControlData[channel*8+i] = 0;
		}

		m_seControlData[channel*8+1] |= 0x200;
	}
	else
	{
		m_seControlData[channel*8+1] &= ~0x200;
	}
}

BOOL CMessageData::CheckSEIsSystem(int channel)
{
	if (CheckSeControlFlag(channel) == FALSE) return FALSE;
	return (m_seControlData[channel * 8 + 1] & 0x1000) != 0;
}


void CMessageData::SetSEIsSystem(BOOL flg, int channel)
{
	CreateSeControl(channel + 1);
	if (flg)
	{
		m_seControlData[channel * 8 + 1] |= 0x1000;
	}
	else
	{
		m_seControlData[channel * 8 + 1] &= ~0x1000;
	}
}


int CMessageData::GetSEFadeTime(int channel )
{
	if (CheckSeControlFlag(channel) == FALSE) return 0;
	return m_seControlData[channel*8+6];
}

void CMessageData::SetSEFadeTime(int tm,int channel)
{
	CreateSeControl(channel+1);
	m_seControlData[channel*8+6] = tm;
}


int CMessageData::GetVoiceFadeInOut(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return 0;
	return (m_voiceControlData[channel*8+1] & 0x100)>> 8;
}

void CMessageData::SetVoiceFadeInOut(int flg,int channel)
{
	CreateVoiceControl(channel+1);

	if (flg)
	{
		m_voiceControlData[channel*8+1] |= 0x100;
	}
	else
	{
		m_voiceControlData[channel*8+1] &= ~0x100;
	}
}

int CMessageData::GetVoiceVolumeOnly(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return FALSE;
	return (m_voiceControlData[channel*8+1] & 0x200)>> 9;
}

void CMessageData::SetVoiceVolumeOnly(int flg,int channel)
{
	CreateVoiceControl(channel+1);

	if (flg)
	{
		m_voiceControlData[channel*8+1] |= 0x200;
	}
	else
	{
		m_voiceControlData[channel*8+1] &= ~0x200;
	}
}


int CMessageData::GetVoiceFadeTime(int channel)
{
	if (CheckVoiceControlFlag(channel) == FALSE) return 0;
	return m_voiceControlData[channel*8+6];
}

void CMessageData::SetVoiceFadeTime(int tm,int channel)
{
	CreateVoiceControl(channel+1);
	m_voiceControlData[channel*8+6] = tm;
}


int CMessageData::GetSeMode(int channel)
{
	if (GetSEVolumeOnly(channel)) return 3;
	if (CheckSEFlag(channel))
	{
		return 1;
	}

	if (CheckSeStop(channel)) return 2;

	return 0;
}

int CMessageData::GetVoiceMode(int channel)
{
	if (GetVoiceVolumeOnly(channel)) return 3;

	if (CheckVoiceFlag(channel))
	{
		return 1;
	}

	if (CheckVoiceOff(channel)) return 2;

	return 0;
}






BOOL CMessageData::Replace(int start,int ln, LPSTR mes)
{
	int srcLength = strlen(m_messageBuffer);
//	int first = srcLength - start;
//	if (first < 0) return FALSE;	//error

	int center = strlen(mes);

	int last = srcLength - start - ln;
	if (last < 0) return FALSE;

	int sz = start + center + last;

	char* mes2 = new char[sz+2];

	if (start > 0) memcpy(mes2,m_messageBuffer,start);
	if (center > 0) memcpy(mes2+start,mes,center);
	if (last > 0) memcpy(mes2+start+center,m_messageBuffer+start+ln,last);
	mes2[start+center+last] = 0;
	mes2[start+center+last+1] = 0;


	CheckAndExpandMessageWork(sz+2);
	memcpy(m_messageBuffer,mes2,sz+2);
	delete [] mes2;
	
	return TRUE;
}

void CMessageData::OutputScriptSource(FILE* file)
{
	char mes[16384];
	int md = GetMessageMode();

	if (md == 3)
	{
		int drawCount = GetDrawCount();
		wsprintf(mes,"Draw(%d)",drawCount);
		OutputData(file,mes,4);
	}
	else
	{
		if (md == 0)
		{
			OutputData(file,"Print()",4);
		}
		else if (md == 1)
		{
			OutputData(file,"LPrint()",4);
		}
		else if (md == 2)
		{
			OutputData(file,"Append()",4);
		}
	}

	//sound,voice


	OutputData(file,"{",4);
	if (md != 3)
	{
		int gyo = GetMessageGyo();
		for (int i=0;i<gyo;i++)
		{
			GetMessage1Gyo(i);
			wsprintf(mes,"\"%s\"",m_tmpMessage);
			OutputData(file,mes,5);
		}
	}
	OutputData(file,"}",4);
}


void CMessageData::SetWhiteSpace(int n)
{
	if (n<0) n = 0;
	if (n>1023) n= 1023;
	for (int i=0;i<n;i++)
	{
		m_whiteSpaceStr[i] = ' ';
	}
	m_whiteSpaceStr[n] = 0;
	m_whiteSpace = n;
}

void CMessageData::WriteWhiteSpace(FILE* file)
{
	if (m_whiteSpace > 0)
	{
		fwrite(m_whiteSpaceStr,sizeof(char),m_whiteSpace,file);
	}
}

int CMessageData::CheckIncludeSeimei(int flag)
{
	if (flag == 0) return 0;


	GetMessageDataSkipAllComment();




	int ln = strlen(m_tmpMessage2);
	int rt = 0;
	
	int offset = 0;

	//名前チェック
	if (ln >= 5)
	{
		if ((*m_tmpMessage2) == '#')
		{
			if ((*(m_tmpMessage2+3)) == 0x0d)
			{
				if ((*(m_tmpMessage2+4)) == 0x0a)
				{
					char chk[4];
					memcpy(chk,m_tmpMessage2+1,2);
					chk[2] = 0;
					chk[3] = 0;
					if (flag & 1)
					{
						if (strcmp(chk,"名") == 0) offset = 4;
					}
					if (flag & 2)
					{
						if (strcmp(chk,"姓") == 0) offset = 4;
					}
				}
			}
		}
	}



	if (flag & 1)
	{
		if (strstr(m_tmpMessage2 + offset,"#名") != NULL) rt |= 1;
	}

	if (flag & 2)
	{
		if (strstr(m_tmpMessage2 + offset,"#姓") != NULL) rt |= 2;
	}

	return rt;
}

//static 
void CMessageData::SetDefaultName(char* name)
{
	int ln = strlen(name);
	if (ln>254) ln = 254;
	memcpy(m_defaultName,name,ln);
	m_defaultName[ln] = 0;
	m_defaultName[ln+1] = 0;
}



/*_*/


