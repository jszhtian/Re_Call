//
// minipiccache.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "minipiccache.h"

#include "..\..\systemNNN\nyanLib\include\picture.h"

#define FILENAMELENGTH 64





CMiniPicCache::CMiniPicCache()
{
	m_lpName = NULL;
	m_lplpBuffer = NULL;
	m_picKosuu = 0;
	m_picKosuuMax = 0;

	m_pic = new CPicture(32,24);
	AddDIBBuffer(64);
}


CMiniPicCache::~CMiniPicCache()
{
	End();
}

void CMiniPicCache::End(void)
{
	DELETEARRAY(m_lpName);
	if (m_lplpBuffer != NULL)
	{
		for (int i=0;i<m_picKosuuMax;i++)
		{
			DELETEARRAY(m_lplpBuffer[i]);
		}
		DELETEARRAY(m_lplpBuffer);
	}
	ENDDELETECLASS(m_pic);
}

int CMiniPicCache::GetBufferNumber(LPSTR filename,int nStart)
{
	if ((nStart < 0) || (nStart >= m_picKosuu)) nStart = 0;


	int n = SearchFile(filename,nStart);
	if (n != -1) return n;

	//‚Â‚­‚é
	GetPicture(filename,nStart);
	return SearchFile(filename,nStart);
}



int* CMiniPicCache::GetPicture(LPSTR filename,int nStart)
{
	if ((nStart < 0) || (nStart >= m_picKosuu)) nStart = 0;

	int n = SearchFile(filename,nStart);
	if (n != -1) return m_lplpBuffer[n];


	if (n == -1)
	{
		if (m_picKosuu==m_picKosuuMax)
		{
			AddDIBBuffer(32);
		}
		n = m_picKosuu;
		m_picKosuu++;
	}
		
	//load
	char makedFilename[64];
//	char makedFilename2[64];
//	BOOL flg = FALSE;
	if (((*filename) == 't') && ((*(filename+1)) == 'a'))
	{
		wsprintf(makedFilename,"ta\\%s",filename);
//		sprintf(makedFilename2,"ta\\%sm",filename);
//		flg = TRUE;
	}
	else if (((*filename) == 'b') && ((*(filename+1)) == 'g'))
	{
		wsprintf(makedFilename,"bg\\%s",filename);
	}
	else if (((*filename) == 'e') && ((*(filename+1)) == 'v'))
	{
		wsprintf(makedFilename,"ev\\%s",filename);
	}
	else if (((*filename) == 'e') && ((*(filename+1)) == 'd'))
	{
		wsprintf(makedFilename,"ed\\%s",filename);
	}
	else
	{
		wsprintf(makedFilename,"etc\\%s",filename);
	}

//	m_pic->LoadPicture(makedFilename);
	m_pic->LoadDWQ(makedFilename);


	//k¬ŠG‚ð‚Â‚­‚é‚É‚á
	PictureToMini(n);

	int ln = strlen(filename);
	if (ln >= FILENAMELENGTH) ln = FILENAMELENGTH - 1;

	memcpy(m_lpName+n*FILENAMELENGTH,filename,ln+1);

//DebugString("[MiniPic:");


	return m_lplpBuffer[n];
}


void CMiniPicCache::PictureToMini(int n)
{
	int* dst = m_lplpBuffer[n];

	RECT rc;
	m_pic->GetPicSize(&rc);

	int x = rc.right;
	int y = rc.bottom;

	int* src = (int*)m_pic->GetPictureBuffer();

	int loopX = x / 32;
	int loopY = y / 24;

	int div = loopX * loopY;
	if (div<1) div = 1;

//char mes[256];
//sprintf(mes,"[%d %d %d]",(int)src,x,y);
//OutputDebugString(mes);

	for (int j=0;j<24;j++)
	{
		for (int i=0;i<32;i++)
		{
			int r = 0;
			int g = 0;
			int b = 0;

			int* ptr0 = src + i * loopX + j * loopY * x;

			for (int jj=0;jj<loopY;jj++)
			{
				int* ptr = ptr0;

				for (int ii=0;ii<loopX;ii++)
				{
					int d = *ptr;
					int rr = (d>>16) & 0xff;
					int gg = (d>>8) & 0xff;
					int bb = d & 0xff;

					r += rr;
					g += gg;
					b += bb;

					ptr++;
				}
				ptr0 += x;
			}

			r /= div;
			g /= div;
			b /= div;

			if (r>255) r = 255;
			if (g>255) g = 255;
			if (b>255) b = 255;

			*dst = (r<<16) | (g<<8) | b;
			dst++;
		}
	}
}












int CMiniPicCache::SearchFile(LPSTR filename,int nStart)
{
	if (m_picKosuu == 0) return -1;
	if ((nStart < 0) || (nStart >= m_picKosuu)) nStart = 0;

	int lp = (m_picKosuu/2)+1;

	int n1 = lp;
	int n2 = (lp+1) % m_picKosuu;


	for (int i=0;i<lp;i++)
	{
		if (strcmp(filename,&m_lpName[n1*FILENAMELENGTH]) == 0) return n1;
		n1 += 1;
		n1 %= m_picKosuu;

		if (strcmp(filename,&m_lpName[n2*FILENAMELENGTH]) == 0) return n2;
		n2 += (m_picKosuu*2-1);
		n2 %= m_picKosuu;
	}

	return -1;
}


void CMiniPicCache::AddDIBBuffer(int n)
{
	char* lpName = new char[(m_picKosuuMax+n)*FILENAMELENGTH];
	if (m_picKosuu>0)
	{
		memcpy(lpName,m_lpName,m_picKosuu*FILENAMELENGTH);
	}
	DELETEARRAY(m_lpName);
	m_lpName = lpName;


	int** lplpBuffer = new int* [m_picKosuuMax+n];
	//copy
	int i;
	for (i=0;i<m_picKosuu;i++)
	{
		*(lplpBuffer+i) = m_lplpBuffer[i];
	}
	for (i=m_picKosuuMax;i<m_picKosuuMax+n;i++)
	{
		*(lplpBuffer+i) = new int[32*24];
	}

	DELETEARRAY(m_lplpBuffer);
	m_lplpBuffer = lplpBuffer;

	m_picKosuuMax += n;
}


/*_*/

