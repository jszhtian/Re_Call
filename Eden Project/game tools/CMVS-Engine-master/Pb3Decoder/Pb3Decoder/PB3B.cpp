#include "PB3B.h"
#include "JBP1.h"
#include "BMPMaker.h"
#include "WinFile.h"

/// Decoding
///
/// Remarks: pvSrc is decoded first
///
/// @param pclArc     Archive
/// @param pvSrc      Compressed data
/// @param dwSrcSize  Compressed data size
/// @param pfnDecrypt Decoding function
///

//ToDo : ����Type�ĸ���λ�ж��Ƿ���Ҫxor����
BOOL CPB3B::Decode(wstring& OutName, void* pvSrc, DWORD dwSrcSize)
{
	this->FileName = OutName;
	BYTE* pbtSrc = (BYTE*)pvSrc;

	// Decryption
	Decrypt(pbtSrc, dwSrcSize);

	// Get image information
	WORD wType = *(WORD*)&pbtSrc[28];
	long lWidth = *(WORD*)&pbtSrc[30];
	long lHeight = *(WORD*)&pbtSrc[32];
	WORD wBpp = *(WORD*)&pbtSrc[34];

	switch (wType)
	{
	case 1:
		Decode1(pbtSrc, dwSrcSize, lWidth, lHeight, wBpp);
		break;

	case 2:
	case 3:
		Decode3(pbtSrc, dwSrcSize, lWidth, lHeight, wBpp);
		break;

	case 4:
		Decode4(pbtSrc, dwSrcSize, lWidth, lHeight, wBpp);
		break;

	case 5:
		Decode5(pbtSrc, dwSrcSize, lWidth, lHeight, wBpp);
		break;

	case 6:
		Decode6(pbtSrc, dwSrcSize, lWidth, lHeight, wBpp);
		break;

	default: // Unknown
	{
		WCHAR Info[100] = { 0 };
		wsprintfW(Info, L"Type : %x", wType);
		MessageBoxW(NULL, Info, L"Unknown", 0);
	}
		break;
	}

	return TRUE;
}

/// Decryption
///
/// @param pbtTarget Decoded data
/// @param dwSize    Decoded data size
///
void CPB3B::Decrypt(BYTE* pbtTarget, DWORD dwSize)
{
	for (int i = 8; i < 52; i += 2)
	{
		*(WORD*)&pbtTarget[i] ^= *(WORD*)&pbtTarget[dwSize - 3];
	}

	for (int i = 8, j = 0; i < 52; i++, j++)
	{
		pbtTarget[i] -= pbtTarget[dwSize - 47 + j];
	}
}

/// Decode 1
///
/// @param pclArc     Archive
/// @param pbtSrc     Compressed data
/// @param dwSrcSize  Compressed data size
/// @param lWidth     Width
/// @param lHeight    Height
/// @param wBpp       Number of bits
///
BOOL CPB3B::Decode1(const BYTE* pbtSrc, DWORD dwSrcSize, long lWidth, long lHeight, WORD wBpp)
{
	// Ensure output buffer
	DWORD          dwDstSize = (lWidth * lHeight * (wBpp >> 3));
	YCMemory<BYTE> clmbtDst(dwDstSize);

	// Decompress
	Decomp1(&clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp);

	// Output
	/*
	CImage clImage;
	clImage.Init(pclArc, lWidth, lHeight, wBpp);
	clImage.WriteReverse(&clmbtDst[0], dwDstSize);
	*/

	WriteBMP32W(this->FileName.c_str(), &clmbtDst[0], dwDstSize, lWidth, -lHeight, wBpp >> 3);
	return TRUE;
}

/// Decode 3
///
/// @param pclArc     Archive
/// @param pbtSrc     Compressed data
/// @param dwSrcSize  Compressed data size
/// @param lWidth     Width
/// @param lHeight    Height
/// @param wBpp       Number of bits
///
BOOL CPB3B::Decode3(const BYTE* pbtSrc, DWORD dwSrcSize, long lWidth, long lHeight, WORD wBpp)
{
	// Ensure output buffer
	DWORD          dwDstSize = (lWidth * lHeight * (wBpp >> 3));
	YCMemory<BYTE> clmbtDst(dwDstSize);

	// Decompress
	Decomp3(&clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp);

	// Output
	//CImage clImage;
	//clImage.Init(pclArc, lWidth, lHeight, wBpp);
	//clImage.WriteReverse(&clmbtDst[0], dwDstSize);

	WriteBMP32W(this->FileName.c_str(), &clmbtDst[0], dwDstSize, lWidth, -lHeight, wBpp >> 3);

	return TRUE;
}

/// Decode 4
///
/// @param pclArc     Archive
/// @param pbtSrc     Compressed data
/// @param dwSrcSize  Compressed data size
/// @param lWidth     Width
/// @param lHeight    Height
/// @param wBpp       Number of bits
///
BOOL CPB3B::Decode4(const BYTE* pbtSrc, DWORD dwSrcSize, long lWidth, long lHeight, WORD wBpp)
{
	//pclArc->OpenFile();
	//pclArc->WriteFile(pbtSrc, dwSrcSize);

	WriteBMP32W(this->FileName.c_str(), (BYTE*)pbtSrc, dwSrcSize, lWidth, lHeight, wBpp >> 3);

	return TRUE;
}

/// Decode 5
///
/// @param pclArc     Archive
/// @param pbtSrc     Compressed data
/// @param dwSrcSize  Compressed data size
/// @param lWidth     Width
/// @param lHeight    Height
/// @param wBpp       Number of bits
///
BOOL CPB3B::Decode5(const BYTE* pbtSrc, DWORD dwSrcSize, long lWidth, long lHeight, WORD wBpp)
{
	// Ensure base image buffer
	DWORD          dwBaseSize = (lWidth * lHeight * (wBpp >> 3));
	YCMemory<BYTE> clmbtBase(dwBaseSize);

	// Decompress base image
	Decomp5(&clmbtBase[0], dwBaseSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp);

	// Output base image
	//CImage clImage;
	//clImage.Init(pclArc, lWidth, lHeight, wBpp);
	//clImage.WriteReverse(&clmbtBase[0], dwBaseSize);
	//clImage.Close();

	WriteBMP32W(this->FileName.c_str(), &clmbtBase[0], dwBaseSize, lWidth, -lHeight, wBpp >> 3);

	return TRUE;
}

wstring get_file_prefix(const std::wstring& filename) 
{
	wstring temp(filename);

	wstring::size_type pos = temp.find_last_of(L".");

	if (pos != wstring::npos) {
		temp = temp.substr(0, pos);
	}

	return temp;
}

/// Decode 6
///
/// @param pclArc     Archive
/// @param pbtSrc     Compressed data
/// @param dwSrcSize  Compressed data size
/// @param lWidth     Width
/// @param lHeight    Height
/// @param wBpp       Number of bits
/// @param pfnDecrypt Decryption function
///
BOOL CPB3B::Decode6(const BYTE* pbtSrc, DWORD dwSrcSize, long lWidth, long lHeight, WORD wBpp)
{
	// Ensure output buffer
	DWORD dwDstSize = (lWidth * lHeight * (wBpp >> 3));
	YCMemory<BYTE> clmbtDst(dwDstSize);

	// Get base image file name
	BYTE abtBaseFileName[64];
	memcpy(abtBaseFileName, pbtSrc + 52, 32);

	// Decrypt base image filename
	const static DWORD	adwCrypt[] =
	{
		0x9CF375A6, 0xA37869C5, 0x794FA53E, 0xC73AFE59
	};

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			((DWORD*)abtBaseFileName)[i * 4 + j] ^= adwCrypt[j];
		}
	}

	abtBaseFileName[32] = '\0';

	std::string clsBaseFileName = (char*)abtBaseFileName;
	clsBaseFileName += ".pb3";

	/*
	if (clsBaseFileName == "ast-si0a_100.pb3")
	{
		clsBaseFileName = "st-si0a_100a.pb3";
	}
	*/

	WCHAR WideName[260] = { 0 };
	MultiByteToWideChar(932, 0, clsBaseFileName.c_str(), clsBaseFileName.length(), WideName, 260);
	WinFile BaseFile;
	HRESULT Result = S_OK;
	if (BaseFile.Open(WideName, WinFile::FileRead) != S_OK)
	{
		WCHAR Err[1024] = { 0 };
		wsprintfW(Err, L"Image[%s] needs Base Image[%s] in current path!", this->FileName.c_str(), WideName);
		//MessageBoxW(NULL, Err, L"FATAL", MB_OK);
		//return FALSE;
		wprintf(Err);
		wprintf(L"\n");
		wprintf(L"Now Ignored!!\n");
		Result = S_FALSE;

		wstring Name = get_file_prefix(this->FileName);
		Name += L"%";
		Name += get_file_prefix(wstring(WideName));
		Name += L".txt";

		CHAR UTF8Log[2048] = { 0 };
		WideCharToMultiByte(CP_UTF8, 0, Err, lstrlenW(Err), UTF8Log, 2048, nullptr, nullptr);
		FILE* log = _wfopen(Name.c_str(), L"wb");
		fwrite(UTF8Log, 1, lstrlenA(UTF8Log), log);
		fclose(log);
	}

	// Base image exists
	if (Result == S_OK)
	{
		DWORD          dwBaseSrcSize = BaseFile.GetSize32();
		YCMemory<BYTE> clmbtBaseSrc(dwBaseSrcSize);

		// Read base image
		BaseFile.Read(&clmbtBaseSrc[0], dwBaseSrcSize);

		// Decrypt base image
		//pfnDecrypt(&clmbtBaseSrc[0], dwBaseSrcSize, pclArc, *pstBaseFileInfo);
		Decrypt(&clmbtBaseSrc[0], dwBaseSrcSize);

		// Decompress base image
		Decomp5(&clmbtDst[0], dwDstSize, &clmbtBaseSrc[0], dwBaseSrcSize, lWidth, lHeight, wBpp);
	}
	else // Base image does not exist
	{
		ZeroMemory(&clmbtDst[0], dwDstSize);
	}

	// Decompress difference image
	Decomp6(&clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp);

	// Output
	//CImage clImage;
	//clImage.Init(pclArc, lWidth, lHeight, wBpp);
	//clImage.WriteReverse(&clmbtDst[0], dwDstSize);

	if (Result == S_OK)
	{
		wstring Name = get_file_prefix(this->FileName);
		Name += L"%";
		Name += get_file_prefix(wstring(WideName));
		Name += L".bmp";

		WriteBMP32W(Name.c_str(), &clmbtDst[0], dwDstSize, lWidth, -lHeight, wBpp >> 3);
	}
	else
	{
		WriteBMP32W(this->FileName.c_str(), &clmbtDst[0], dwDstSize, lWidth, -lHeight, wBpp >> 3);
	}
	return TRUE;
}

/// Decompression 1
///
/// @param pbtDst     Destination
/// @param dwDstSize  Destination size
/// @param pbtSrc     Compressed data
/// @param dwSrcSize  Compressed data size
/// @param lWidth     Width
/// @param lHeight    Height
/// @param wBpp       Number of bits
///
BOOL CPB3B::Decomp1(BYTE* pbtDst, DWORD dwDstSize, const BYTE* pbtSrc, DWORD dwSrcSize, long lWidth, long lHeight, WORD wBpp)
{
	DWORD dwOffsetOfFlags = *(DWORD*)&pbtSrc[44];
	DWORD dwOffsetOfCompData = *(DWORD*)&pbtSrc[48];
	long  lWidthOfBlock = *(DWORD*)&pbtSrc[24];
	long  lHeightOfBlock = *(DWORD*)&pbtSrc[24];
	WORD  wByteCount = (wBpp >> 3);
	long  lLine = (lWidth * wByteCount);

	// Ensure LZSS decompression buffer
	DWORD dwTempSize = (lWidth * lHeight);
	YCMemory<BYTE> clmbtTemp(dwTempSize);

	// Decompress
	for (WORD wColor = 0; wColor < wByteCount; wColor++)
	{
		const BYTE* pbtWorkOfSrc = &pbtSrc[dwOffsetOfFlags];
		DWORD dwWork = (wByteCount * 4);
		for (WORD i = 0; i < wColor; i++)
		{
			dwWork += ((DWORD*)pbtWorkOfSrc)[i];
		}
		pbtWorkOfSrc += dwWork;

		// Get the LZSS size after decompression
		DWORD dwLZSSDecodeSize = *(DWORD*)&pbtWorkOfSrc[8];

		// Get pointer to flag data
		const BYTE* pbtFlags = pbtWorkOfSrc + *(DWORD*)&pbtWorkOfSrc[0] + *(DWORD*)&pbtWorkOfSrc[4] + 12;

		// Get pointer to decompressed data
		const BYTE* pbtCompData = &pbtSrc[dwOffsetOfCompData];
		dwWork = (wByteCount * 4);
		for (WORD i = 0; i < wColor; i++)
		{
			dwWork += ((DWORD*)pbtCompData)[i];
		}
		pbtCompData += dwWork;

		// �o�b�t�@�s���ɑΏ�
		// dwLZSSDecodeSize�Ŗ���o�b�t�@�m�ۂ���ΑΏ�����K�v�Ȃ����ǁA�������m�ہ^�j���͍ŏ����ɗ}������
		if (dwTempSize < dwLZSSDecodeSize)
		{
			clmbtTemp.resize(dwLZSSDecodeSize);
		}

		// LZSS Decompression
		DWORD dwFlagsSize = 0xFFFFFFFF;    // Unknown size
		DWORD dwCompDataSize = 0xFFFFFFFF; // Unknown size
		DecompLZSS(&clmbtTemp[0], dwLZSSDecodeSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize);

		// Decompress compressed blocks
		BYTE* pbtWorkOfDst = &pbtDst[wColor];
		long  lBlockCountOfWidth = ((lWidth + (lWidthOfBlock - 1)) / lWidthOfBlock);
		long  lBlockCountOfHeight = ((lHeight + (lHeightOfBlock - 1)) / lHeightOfBlock);
		BYTE  btCode = 0x80;
		DWORD dwTempPtr = 0;
		DWORD dwFlagsPtr = 0;
		DWORD dwCompDataPtr = 0;

		pbtFlags = pbtWorkOfSrc + 12;
		pbtCompData = pbtWorkOfSrc + *(DWORD*)&pbtWorkOfSrc[0] + 12;

		for (long lY = 0, lBlockY = 0; lBlockY < lBlockCountOfHeight; lY += lHeightOfBlock, lBlockY++)
		{
			BYTE* pbtWorkOfDst2 = pbtWorkOfDst;
			long  lHeightOfBlockMax = ((lY + lHeightOfBlock) > lHeight) ? (lHeight - lY) : lHeightOfBlock;

			for (long lX = 0, lBlockX = 0; lBlockX < lBlockCountOfWidth; lX += lWidthOfBlock, lBlockX++)
			{
				if (btCode == 0)
				{
					// 8-block processing
					dwFlagsPtr++;
					btCode = 0x80;
				}

				// Processing 1 block
				BYTE* pbtWorkOfDst3 = pbtWorkOfDst2;
				long  lWidthOfBlockMax = ((lX + lWidthOfBlock) > lWidth) ? (lWidth - lX) : lWidthOfBlock;

				// Compressed
				if (pbtFlags[dwFlagsPtr] & btCode)
				{
					for (long i = 0; i < lHeightOfBlockMax; i++)
					{
						for (long j = 0; j < lWidthOfBlockMax; j++)
						{
							pbtWorkOfDst3[j * wByteCount] = pbtCompData[dwCompDataPtr];
						}

						pbtWorkOfDst3 += lLine;
					}

					dwCompDataPtr++;
				}
				else // Not compressed
				{
					for (long i = 0; i < lHeightOfBlockMax; i++)
					{
						for (long j = 0; j < lWidthOfBlockMax; j++)
						{
							pbtWorkOfDst3[j * wByteCount] = clmbtTemp[dwTempPtr++];
						}

						pbtWorkOfDst3 += lLine;
					}
				}

				// Point to the next block
				pbtWorkOfDst2 += (lWidthOfBlock * wByteCount);
				btCode >>= 1;
			}

			pbtWorkOfDst += (lLine * lHeightOfBlock);
		}
	}

	return TRUE;
}

/// Decompression 3
///
/// @param pbtDst     Destination
/// @param dwDstSize  Destination size
/// @param pbtSrc     Compressed data
/// @param dwSrcSize  Compressed data size
/// @param lWidth     Width
/// @param lHeight    Height
/// @param wBpp       Number of bits
///
BOOL CPB3B::Decomp3(BYTE* pbtDst, DWORD dwDstSize, const BYTE* pbtSrc, DWORD dwSrcSize, long lWidth, long lHeight, WORD wBpp)
{
	// Get alpha value
	const BYTE* pbtAlpha = pbtSrc + *(DWORD*)&pbtSrc[44];
	DWORD dwAlphaSize = *(DWORD*)&pbtSrc[48];

	// Decompress
	CJBP1 clJBP1;
	clJBP1.Decomp(pbtDst, &pbtSrc[52], wBpp, pbtAlpha, dwAlphaSize);

	return TRUE;
}

/// Decompression 4
///
/// @param pbtDst     Destination
/// @param dwDstSize  Destination size
/// @param pbtSrc     Compressed data
/// @param dwSrcSize  Compressed data size
/// @param lWidth     Width
/// @param lHeight    Height
/// @param wBpp       Number of bits
///
BOOL CPB3B::Decomp4(BYTE* pbtDst, DWORD dwDstSize, const BYTE* pbtSrc, DWORD dwSrcSize, long lWidth, long lHeight, WORD wBpp)
{
	return TRUE;
}

/// Decompression 5
///
/// @param pbtDst     Destination
/// @param dwDstSize  Destination size
/// @param pbtSrc     Compressed data
/// @param dwSrcSize  Compressed data size
/// @param lWidth     Width
/// @param lHeight    Height
/// @param wBpp       Number of bits
///
BOOL CPB3B::Decomp5(BYTE* pbtDst, DWORD dwDstSize, const BYTE* pbtSrc, DWORD dwSrcSize, long lWidth, long lHeight, WORD wBpp)
{
	// LZSS decompression buffer
	DWORD dwTempSize = (lWidth * lHeight);
	YCMemory<BYTE> aclmbtTemp[4];

	// Decompress base image
	for (int i = 0; i < 4; i++)
	{
		// Ensure LZSS decompression buffer
		aclmbtTemp[i].resize(dwTempSize);

		// Get pointer to flag data
		const BYTE* pbtFlags = pbtSrc + 84 + *(DWORD*)&pbtSrc[i * 8 + 52];

		// Get pointer to compressed data
		const BYTE* pbtCompData = pbtSrc + 84 + *(DWORD*)&pbtSrc[i * 8 + 56];

		// LZSS Decompression
		DWORD dwFlagsSize = 0xFFFFFFFF;    // Unknown size
		DWORD dwCompDataSize = 0xFFFFFFFF; // Unknown size
		DecompLZSS(&aclmbtTemp[i][0], dwTempSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize);
	}

	// Store
	pbtDst[0] = aclmbtTemp[0][0];
	pbtDst[1] = aclmbtTemp[1][0];
	pbtDst[2] = aclmbtTemp[2][0];
	pbtDst[3] = aclmbtTemp[3][0];

	for (DWORD i = 1; i < dwTempSize; i++)
	{
		pbtDst[i * 4 + 0] = aclmbtTemp[0][i] + pbtDst[i * 4 - 4];
		pbtDst[i * 4 + 1] = aclmbtTemp[1][i] + pbtDst[i * 4 - 3];
		pbtDst[i * 4 + 2] = aclmbtTemp[2][i] + pbtDst[i * 4 - 2];
		pbtDst[i * 4 + 3] = aclmbtTemp[3][i] + pbtDst[i * 4 - 1];
	}

	return TRUE;
}

/// Decompress 6
///
/// Remarks: Assumes that the base image is stored in pbtDst
///
/// @param pbtDst        Destination
/// @param dwDstSize     Destination size
/// @param pbtSrc        Compressed data
/// @param dwSrcSize     Compressed data size
/// @param lWidth        Width
/// @param lHeight       Height
/// @param wBpp          Number of bits
///
BOOL CPB3B::Decomp6(BYTE* pbtDst, DWORD dwDstSize, const BYTE* pbtSrc, DWORD dwSrcSize, long lWidth, long lHeight, WORD wBpp)
{
	// Ensure LZSS decompression buffer
	DWORD dwLZSSDecodeSize = *(DWORD*)&pbtSrc[24];
	YCMemory<BYTE> clmbtTemp(dwLZSSDecodeSize);

	// LZSS Decompression
	const BYTE* pbtFlags = pbtSrc + 84;
	const BYTE* pbtCompData = pbtSrc + 84 + *(DWORD*)&pbtSrc[44];
	DWORD       dwFlagsSize = 0xFFFFFFFF;	// Unknown size
	DWORD       dwCompDataSize = 0xFFFFFFFF;	// Unknown size
	DecompLZSS(&clmbtTemp[0], dwLZSSDecodeSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize);

	// Decompression
	WORD  wByteCount = (wBpp >> 3);
	long  lLine = (lWidth * 4);
	long  lWidthOfBlock = 8;
	long  lHeightOfBlock = 8;
	long  lBlockCountOfWidth = ((lWidth + (lWidthOfBlock - 1)) / lWidthOfBlock);
	long  lBlockCountOfHeight = ((lHeight + (lHeightOfBlock - 1)) / lHeightOfBlock);
	DWORD dwFlagsPtr = 0;
	DWORD dwCompDataPtr = 0;
	BYTE  btCode = 0x80;

	pbtFlags = &clmbtTemp[8];
	pbtCompData = &clmbtTemp[*(DWORD*)&clmbtTemp[0] + 8];

	BYTE* pbtWorkOfDst = pbtDst;

	for (long lY = 0, lBlockY = 0; lBlockY < lBlockCountOfHeight; lY += lHeightOfBlock, lBlockY++)
	{
		BYTE* pbtWorkOfDst2 = pbtWorkOfDst;
		long  lBlockOfHeightMax = ((lY + lHeightOfBlock) > lHeight) ? (lHeight - lY) : lHeightOfBlock;

		for (long lX = 0, lBlockX = 0; lBlockX < lBlockCountOfWidth; lX += lWidthOfBlock, lBlockX++)
		{
			if (btCode == 0)
			{
				// Processing 8 blocks
				btCode = 0x80;
				dwFlagsPtr++;
			}

			// Process 1 block
			BYTE* pbtWorkOfDst3 = pbtWorkOfDst2;
			long  lBlockOfWidthMax = ((lX + lWidthOfBlock) > lWidth) ? (lWidth - lX) : lWidthOfBlock;

			if ((pbtFlags[dwFlagsPtr] & btCode) == 0)
			{
				// Difference
				for (long i = 0; i < lBlockOfHeightMax; i++)
				{
					for (long j = 0; j < lBlockOfWidthMax; j++)
					{
						memcpy(&pbtWorkOfDst3[j * 4], &pbtCompData[dwCompDataPtr + j * 4], 4);
					}

					dwCompDataPtr += (lBlockOfWidthMax * 4);
					pbtWorkOfDst3 += lLine;
				}
			}

			// Point to the next block
			pbtWorkOfDst2 += (lWidthOfBlock * 4);
			btCode >>= 1;
		}

		pbtWorkOfDst += (lLine * lHeightOfBlock);
	}

	return TRUE;
}
