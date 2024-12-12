//
//
//






#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"
#include "..\..\systemNNN\nyanLib\include\myGraphics.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"


#include "aviSave.h"

/*
typedef struct {
    DWORD dwMicroSecPerFrame;
    DWORD dwMaxBytesPerSec;
    DWORD dwReserved1;
    DWORD dwFlags;
    DWORD dwTotalFrames;
    DWORD dwInitialFrames;
    DWORD dwStreams;
    DWORD dwSuggestedBufferSize;
    DWORD dwWidth;
    DWORD dwHeight;
    DWORD dwReserved[4];
} MainAVIHeader;

typedef struct {
    FOURCC fccType;
    FOURCC fccHandler;
    DWORD dwFlags;
    DWORD  dwPriority;
    DWORD dwInitialFrames;
    DWORD  dwScale;
    DWORD  dwRate;
    DWORD  dwStart;
    DWORD  dwLength;
    DWORD dwSuggestedBufferSize;
    DWORD  dwQuality;
    DWORD  dwSampleSize;
   // RECT   rcFrame;
	short top,left,right,bottom;
} AVIStreamHeader;
*/



CAviSave::CAviSave(int sizeX,int sizeY)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_file = NULL;
	m_buffer = new char[m_sizeX*m_sizeY*3];
	SetMovieSize(0);	//“™”{
}	

CAviSave::~CAviSave()
{
	End();
}

void CAviSave::End(void)
{
	if (m_file != NULL) CloseAviFile();
	DELETEARRAY(m_buffer);
}


BOOL CAviSave::CreateAVIFile(LPSTR filename,int totalFrame,int dwRate,int dwScale)
{
	if (m_file != NULL) CloseAviFile();

	m_totalFrame = totalFrame;
	m_dwRate = dwRate;
	m_dwScale = dwScale;

	m_dataSizeX = m_sizeX;
	m_dataSizeY = m_sizeY;

	if (m_movieSize == 1)
	{
		m_dataSizeX = 640;
		m_dataSizeY = 480;
	}
	else if (m_movieSize == 2)
	{
		m_dataSizeX = 320;
		m_dataSizeY = 240;
	}
	else if (m_movieSize == 3)
	{
		m_dataSizeX = 160;
		m_dataSizeY = 120;
	}

	if ((m_sizeX<m_dataSizeX) || (m_sizeY<m_dataSizeY))
	{
		m_dataSizeX = m_sizeX;
		m_dataSizeY = m_sizeY;
	}

	m_file = CMyFile::Open(filename,"wb");
	if (m_file == NULL) return FALSE;

	fwrite("RIFF",sizeof(char),4,m_file);
	int fourCCRIFFSize = 0;

	//AVI
	fourCCRIFFSize += 4;

	//LIST size
	fourCCRIFFSize += 8;

	int LISThdrlSize = 0;
	//hdrl
	fourCCRIFFSize += 4;
	LISThdrlSize += 4;

		//avih
		fourCCRIFFSize += (8 + sizeof(MainAVIHeader));
		LISThdrlSize += (8 + sizeof(MainAVIHeader));

		//LIST size
		fourCCRIFFSize += 8;
		LISThdrlSize += 8;
		int LISTstrlSize = 0;

			//strl
			fourCCRIFFSize += 4;
			LISThdrlSize += 4;
			LISTstrlSize += 4;
			
			//strh size
			fourCCRIFFSize += (8 + sizeof(AVIStreamHeader));
			LISThdrlSize += (8 + sizeof(AVIStreamHeader));
			LISTstrlSize += (8 + sizeof(AVIStreamHeader));

			//strf size
			fourCCRIFFSize += (8 + sizeof(BITMAPINFOHEADER));
			LISThdrlSize += (8 + sizeof(BITMAPINFOHEADER));
			LISTstrlSize += (8 + sizeof(BITMAPINFOHEADER));

	//LIST size
	fourCCRIFFSize += 8;
	int LISTmoviSize = 0;


	// movi
	fourCCRIFFSize += 4;
	LISTmoviSize += 4;

	fourCCRIFFSize += m_totalFrame * (m_dataSizeX*m_dataSizeY*3 + 8);
	LISTmoviSize += m_totalFrame * (m_dataSizeX*m_dataSizeY*3 + 8);

	//idx1
	fourCCRIFFSize += 8;
	fourCCRIFFSize += 16*m_totalFrame;

	fwrite(&fourCCRIFFSize,sizeof(int),1,m_file);

	fwrite("AVI ",sizeof(char),4,m_file);
	fwrite("LIST",sizeof(char),4,m_file);
	fwrite(&LISThdrlSize,sizeof(int),1,m_file);
	fwrite("hdrl",sizeof(char),4,m_file);

	fwrite("avih",sizeof(char),4,m_file);

	MainAVIHeader aviHeader;

	LONGLONG maxByte = m_dataSizeX * m_dataSizeY * 3;
	maxByte *= m_dwRate;
	maxByte /= m_dwScale;

	LONGLONG microSec = 1000000;
	microSec *= m_dwScale;
	microSec /= m_dwRate;

	ZeroMemory(&aviHeader,sizeof(MainAVIHeader));
	aviHeader.dwMicroSecPerFrame = (DWORD)microSec;
	aviHeader.dwMaxBytesPerSec = (DWORD)maxByte;
	aviHeader.dwFlags = 0x10;;	//AVIF_HASINDEX;
	aviHeader.dwTotalFrames = m_totalFrame;
	aviHeader.dwInitialFrames = 0;
	aviHeader.dwStreams = 1;	//‰f‘œ‚Ì‚Ý
	aviHeader.dwSuggestedBufferSize = 0;
	aviHeader.dwWidth = m_dataSizeX;
	aviHeader.dwHeight = -m_dataSizeY;

	int sz = sizeof(aviHeader);
	fwrite(&sz,sizeof(int),1,m_file);
	fwrite(&aviHeader,sizeof(MainAVIHeader),1,m_file);

	fwrite("LIST",sizeof(char),4,m_file);
	fwrite(&LISTstrlSize,sizeof(int),1,m_file);
	fwrite("strl",sizeof(char),4,m_file);

	fwrite("strh",sizeof(char),4,m_file);
	sz = sizeof(AVIStreamHeader);
	fwrite(&sz,sizeof(int),1,m_file);


	AVIStreamHeader strHeader;

	strHeader.fccType = 'sdiv';//'vids';
	strHeader.fccHandler = 0;//' BID';
	strHeader.dwFlags = 0;
	strHeader.dwPriority = 0;
	strHeader.dwInitialFrames = 0;
	strHeader.dwScale = m_dwScale;
	strHeader.dwRate = m_dwRate;
	strHeader.dwStart = 0;
	strHeader.dwLength = m_totalFrame;
    strHeader.dwSuggestedBufferSize = 0;
    strHeader.dwQuality = 10000;
    strHeader.dwSampleSize = 0;//
	strHeader.top = 0;
	strHeader.left = 0;
	strHeader.right = 0;
	strHeader.bottom = 0;

	fwrite(&strHeader,sizeof(AVIStreamHeader),1,m_file);

	fwrite("strf",sizeof(char),4,m_file);
	sz = sizeof(BITMAPINFOHEADER);
	fwrite(&sz,sizeof(int),1,m_file);

	BITMAPINFOHEADER bif;

	bif.biSize = sizeof(BITMAPINFOHEADER);
	bif.biWidth = m_dataSizeX;
	bif.biHeight = m_dataSizeY;
	bif.biPlanes = 1;
	bif.biBitCount = 24;
	bif.biCompression = BI_RGB;
	bif.biSizeImage = 0;
	bif.biXPelsPerMeter = 0;
	bif.biYPelsPerMeter = 0;
	bif.biClrUsed = 0;
	bif.biClrImportant = 0;

	fwrite(&bif,sizeof(BITMAPINFOHEADER),1,m_file);


	fwrite("LIST",sizeof(char),4,m_file);
	fwrite(&LISTmoviSize,sizeof(int),1,m_file);

	fwrite("movi",sizeof(char),4,m_file);

	return TRUE;
}



void CAviSave::SetData(char* ptr)
{
	if (m_file == NULL) return;

	char* srcPtr = ptr;

	if (ptr == NULL)
	{
		int sizeX = m_sizeX;
		int sizeY = m_sizeY;

		char* screenPtr0 = (char*)(CMyGraphics::GetScreenBuffer());
		char* dstPtr = m_buffer;

		if ((sizeX % 8) == 0)
		{
			PacMMX();
		}
		else
		{
			for (int j=0;j<sizeY;j++)
			{
				char* screenPtr = screenPtr0;
				screenPtr += (sizeY-1-j) * sizeX * 4;

				for (int i=0;i<sizeX;i++)
				{
					*dstPtr = *screenPtr;
					dstPtr++;
					screenPtr++;
					*dstPtr = *screenPtr;
					dstPtr++;
					screenPtr++;
					*dstPtr = *screenPtr;
					dstPtr++;
					screenPtr++;
					screenPtr++;
				}
			}
		}

		srcPtr = m_buffer;
	}


	if (m_movieSize != 0)
	{
		//k¬
		if ((m_dataSizeX < m_sizeX) && (m_dataSizeY < m_sizeY))
		{
			if (m_moviePackType == 0)
			{
				Shukusho();
			}
			else
			{
				SuperShukusho();
			}
		}
	}


	fwrite("00db",sizeof(char),4,m_file);
	int szz = m_dataSizeX*m_dataSizeY*3;
	fwrite(&szz,sizeof(int),1,m_file);
	fwrite(m_buffer,sizeof(char),m_dataSizeX*m_dataSizeY*3,m_file);
}

void CAviSave::PacMMX(void)
{
	int sizeX = m_sizeX;
	int sizeY = m_sizeY;

	char* srcPtr = (char*)(CMyGraphics::GetScreenBuffer());
	char* dstPtr = m_buffer;

	srcPtr += sizeX * 4 * (sizeY-1);
	int srcPitch = sizeX * 4;

	int loopX = sizeX / 8;

	__asm
	{
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi
		pxor mm0,mm0
		mov eax,0ffffffh
		movd mm7,eax

		mov esi,srcPtr
		mov edi,dstPtr
		mov edx,sizeY
LOOP1:
		push esi

		mov ecx,loopX
LOOP2:
		movq mm1,[esi]
		movq mm5,mm1
		pand mm1,mm7
		psrlq mm5,32
		pand mm5,mm7
		psllq mm5,24
		por mm1,mm5

		movq mm2,[esi+8]
		movq mm5,mm2
		pand mm2,mm7
		psrlq mm5,32
		pand mm5,mm7
		psllq mm5,24
		por mm2,mm5

		movq mm3,[esi+16]
		movq mm5,mm3
		pand mm3,mm7
		psrlq mm5,32
		pand mm5,mm7
		psllq mm5,24
		por mm3,mm5

		movq mm4,[esi+24]
		movq mm5,mm4
		pand mm4,mm7
		psrlq mm5,32
		pand mm5,mm7
		psllq mm5,24
		por mm4,mm5

		movq mm5,mm2
		psllq mm5,48
		por mm1,mm5

		psrlq mm2,16
		movq mm5,mm3
		psllq mm5,32
		por mm2,mm5

		psrlq mm3,32
		psllq mm4,16
		por mm3,mm4

		movq[edi],mm1
		movq[edi+8],mm2
		movq[edi+16],mm3

		add esi,32
		add edi,24

		dec ecx
		jnz LOOP2

		pop esi
		sub esi,srcPitch
		dec edx
		jnz LOOP1

		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax
		emms
	}
}


void CAviSave::CloseAviFile(void)
{
	if (m_file != NULL)
	{
		//


		fwrite("idx1",sizeof(char),4,m_file);
		int sz = 16*m_totalFrame;
		fwrite(&sz,sizeof(int),1,m_file);
		int offset = 4;
		for (int n=0;n<m_totalFrame;n++)
		{
			fwrite("00db",sizeof(char),4,m_file);
			int flg = 0x10;
			fwrite(&flg,sizeof(int),1,m_file);
			sz = m_dataSizeX*m_dataSizeY*3;

			fwrite(&offset,sizeof(int),1,m_file);
			fwrite(&sz,sizeof(int),1,m_file);
			offset += sz;
			offset += 8;
		}


		fclose(m_file);
	}
}

//ŠÈˆÕk¬
void CAviSave::Shukusho(void)
{
	for (int j=0;j<m_dataSizeY;j++)
	{
		char* src = m_buffer;
		char* dst = m_buffer;
		dst += m_dataSizeX * j * 3;
		int j2 = (j * m_sizeY) / m_dataSizeY;
		src += m_sizeX * j2 * 3;

		for (int i=0;i<m_dataSizeX;i++)
		{
			int i2 = (i * m_sizeX) / m_dataSizeX;
			char* src2 = src + i2 * 3;

			*dst=*src2;
			dst++;
			src2++;
			*dst=*src2;
			dst++;
			src2++;
			*dst=*src2;
			dst++;
			src2++;
		}
	}
}

void CAviSave::SuperShukusho(void)
{
	int sizeX = 256 * m_sizeX;
	sizeX /= m_dataSizeX;

	int sizeY = 256*m_sizeY;
	sizeY /= m_dataSizeY;

	double menseki = (double)sizeX;
	menseki *= (double)sizeY;
	menseki /= 65536.0;


	for (int j=0;j<m_dataSizeY;j++)
	{
		for (int i=0;i<m_dataSizeX;i++)
		{
			char* dst = m_buffer;
			dst += i*3;
			dst += j*3*m_dataSizeX;

			double r = 0.0;
			double g = 0.0;
			double b = 0.0;

			int startX = i * 256 * m_sizeX;
			startX /= m_dataSizeX;

			int startY = j * 256 * m_sizeY;
			startY /= m_dataSizeY;

			int endX = startX + sizeX - 1;
			int endY = startY + sizeY - 1;

			int blockStartX = startX / 256;
			int blockStartY = startY / 256;
			int blockEndX = endX / 256;
			int blockEndY = endY / 256;

			int amariX = startX % 256;
			int amariY = startY % 256;

			int blockStartSizeX = 256 - amariX;
			int blockStartSizeY = 256 - amariY;

			int blockEndSizeX = (endX % 256) + 1;
			int blockEndSizeY = (endY % 256) + 1;


			for (int y=blockStartY;y<=blockEndY;y++)
			{
				int blockSizeY = 256;
				if (y == blockStartY) blockSizeY = blockStartSizeY;
				if (y == blockEndY) blockSizeY = blockEndSizeY;

				for (int x=blockStartX;x<=blockEndX;x++)
				{
					int blockSizeX = 256;
					if (x == blockStartX) blockSizeX = blockStartSizeX;
					if (x == blockEndX) blockSizeX = blockEndSizeX;

					double blockSize = (double)(blockSizeX * blockSizeY);
					blockSize /= 65536.0;


					char* src = m_buffer;
					src += x * 3;
					src += y * 3 * m_sizeX; 

					int r0 = (int)(*src);
					src++;
					int g0 = (int)(*src);
					src++;
					int b0 = (int)(*src);
					r0 &= 0xff;
					g0 &= 0xff;
					b0 &= 0xff;

					double r1 = (double)r0;
					double g1 = (double)g0;
					double b1 = (double)b0;

					r1 *= blockSize;
					g1 *= blockSize;
					b1 *= blockSize;

					r += r1;
					g += g1;
					b += b1;
				}
			}

			r /= menseki;
			g /= menseki;
			b /= menseki;

			int rr = (int)(r+0.5);
			int gg = (int)(g+0.5);
			int bb = (int)(b+0.5);
			if (rr<0) rr = 0;
			if (rr>255) rr = 255;
			if (gg<0) gg = 0;
			if (gg>255) gg = 255;
			if (bb<0) bb = 0;
			if (bb>255) bb = 255;

			*dst = (char)rr;
			dst++;
			*dst = (char)gg;
			dst++;
			*dst = (char)bb;
		}
	}
}


/*_*/

