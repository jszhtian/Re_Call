/*
	---------------------------------------------------------------------------
	Copyright (c) 2003, Dominik Reichl <dominik.reichl@t-online.de>, Germany.
	All rights reserved.

	Distributed under the terms of the GNU General Public License v2.

	This software is provided 'as is' with no explicit or implied warranties
	in respect of its properties, including, but not limited to, correctness 
	and/or fitness for purpose.
	---------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdio.h>

#include "hashmgr.h"
#include "rehash.h"

CHashManager::CHashManager()
{
	m_bFullPath = false;
	m_bRecursive = true;

	SelectAllAlgorithms();

	gosthash_init();
}

CHashManager::~CHashManager()
{
}

void CHashManager::SelectAllAlgorithms(bool bSelect)
{
	int i = 0;
	for(i = 0; i < RH_MAX_ALGORITHMS; i++) m_bAlgorithm[i] = bSelect;
}

bool CHashManager::SelectAlgorithm(int nAlgorithm, bool bSelect)
{
	if(nAlgorithm < 0) return false;
	if(nAlgorithm >= RH_MAX_ALGORITHMS) return false;

	m_bAlgorithm[nAlgorithm] = bSelect;
	return true;
}

bool CHashManager::SetOption(int nOption, bool bState)
{
	if(nOption <= 0) return false;
	if(nOption >= OPT_MAX_OPTIONS) return false;

	if(nOption == OPT_FULLPATH) m_bFullPath = bState;
	if(nOption == OPT_RECURSIVE) m_bRecursive = bState;

	return true;
}

int CHashManager::HashPath(char *pszBasePath, char *pszFileSpec)
{
	recursivesafe long lHandle = 0;
	recursivesafe long lSearcherHandle = 0;
	recursivesafe finddata fdInfo;
	recursivesafe char szFull[RH_MAX_PATH];
	recursivesafe char szAll[RH_MAX_PATH];

	if(pszBasePath == NULL) return RH_INVALID_PATH;
	if(pszFileSpec == NULL) return RH_INVALID_PATH;

	fmtPath(pszBasePath);
	fmtPath(pszFileSpec);

	if(strlen(pszBasePath) == 0) getcwd(pszBasePath, RH_MAX_PATH);
	fileonly(pszFileSpec);

	strcpy(szAll, pszBasePath);
	catdirsep(szAll);
	strcat(szAll, SZ_DIR_ALL);

	#ifdef RH_DEBUG
		printf("Function HashPath: pszBasePath=%s, pszFileSpec=%s", pszBasePath, pszFileSpec);
		printf(CPS_NEWLINE);
		printf("Function HashPath: szAll=%s", szAll);
		printf(CPS_NEWLINE);
	#endif

	//////////////////////////////////////////////////////////////////////////
	// Start directory enumeration code

	lSearcherHandle = findfirst(szAll, &fdInfo);
	while(1)
	{
		if(fdInfo.attrib & _A_SUBDIR)
		{
			if((ispathnav(fdInfo.name) == false) && (m_bRecursive))
			{
				if(chdir(fdInfo.name) == 0)
				{
					getcwd(szFull, RH_MAX_PATH);

					#ifdef RH_DEBUG
						printf("Opening new scan path: %s, filemask: %s", szFull, pszFileSpec);
						printf(CPS_NEWLINE);
					#endif

					HashPath(szFull, pszFileSpec);
					chdir(SZ_LEVEL_UP);
				}
			}
		}

		if(findnext(lSearcherHandle, &fdInfo) != 0) break;
	}
	findclose(lSearcherHandle);
	lSearcherHandle = 0;

	// End directory enumeration code
	//////////////////////////////////////////////////////////////////////////

	memset(&fdInfo, 0, sizeof(finddata));
	lHandle = findfirst(pszFileSpec, &fdInfo);
	if(lHandle == EINVAL) return RH_INVALID_PATH;
	if(lHandle == ENOENT) return RH_NO_PATTERN_MATCH;
	if(lHandle == -1) return RH_CANNOT_OPEN_FILE;

	while(1)
	{
		if(fdInfo.attrib & _A_SUBDIR)
		{
			// Don't process directories here
		}
		else
		{
			if(m_bFullPath)
			{
				fullpath(szFull, fdInfo.name, RH_MAX_PATH);
				HashFile(szFull);
			}
			else
			{
				HashFile(fdInfo.name);
			}
			printf(CPS_NEWLINE);
		}

		if(findnext(lHandle, &fdInfo) != 0) break;
	}

	findclose(lHandle);
	lHandle = 0;

	return RH_SUCCESS;
}

int CHashManager::HashFile(char *pszFile)
{
	FILE *fp = NULL;
	unsigned char pBuf[SIZE_HASH_BUFFER];
	unsigned long uRead = 0;
	unsigned char pTemp[256];
	char szTemp[RH_MAX_BUFFER];
	int i = 0;

	printf("File: <");
	printf(pszFile);
	printf(">");
	printf(CPS_NEWLINE);

	fp = fopen(pszFile, "rb");
	if(fp == NULL) return RH_CANNOT_OPEN_FILE;

	if(m_bAlgorithm[HASHID_CRC16]) crc16_init(&m_crc16);
	if(m_bAlgorithm[HASHID_CRC16CCITT]) crc16ccitt_init(&m_crc16ccitt);
	if(m_bAlgorithm[HASHID_CRC32]) crc32Init(&m_crc32);
	if(m_bAlgorithm[HASHID_FCS_16]) fcs16_init(&m_fcs16);
	if(m_bAlgorithm[HASHID_FCS_32]) fcs32_init(&m_fcs32);
	if(m_bAlgorithm[HASHID_GHASH_32_3] || m_bAlgorithm[HASHID_GHASH_32_5]) m_ghash.Init();
	if(m_bAlgorithm[HASHID_GOST]) gosthash_reset(&m_gost);
	if(m_bAlgorithm[HASHID_HAVAL]) haval_start(&m_haval);
	if(m_bAlgorithm[HASHID_MD2]) m_md2.Init();
	if(m_bAlgorithm[HASHID_MD4]) MD4Init(&m_md4);
	if(m_bAlgorithm[HASHID_MD5]) MD5Init(&m_md5, 0);
	if(m_bAlgorithm[HASHID_SHA1]) sha1_begin(&m_sha1);
	if(m_bAlgorithm[HASHID_SHA2_256]) sha256_begin(&m_sha256);
	if(m_bAlgorithm[HASHID_SHA2_384]) sha384_begin(&m_sha384);
	if(m_bAlgorithm[HASHID_SHA2_512]) sha512_begin(&m_sha512);
	if(m_bAlgorithm[HASHID_SIZE_32]) sizehash32_begin(&m_uSizeHash32);
	if(m_bAlgorithm[HASHID_TIGER]) tiger_init(&m_tiger);

	while(1)
	{
		uRead = fread(pBuf, 1, SIZE_HASH_BUFFER, fp);

		if(uRead != 0)
		{
			if(m_bAlgorithm[HASHID_CRC16])
				crc16_update(&m_crc16, pBuf, uRead);

			if(m_bAlgorithm[HASHID_CRC16CCITT])
				crc16ccitt_update(&m_crc16ccitt, pBuf, uRead);

			if(m_bAlgorithm[HASHID_CRC32])
				crc32Update(&m_crc32, pBuf, uRead);

			if(m_bAlgorithm[HASHID_FCS_16])
				fcs16_update(&m_fcs16, pBuf, uRead);

			if(m_bAlgorithm[HASHID_FCS_32])
				fcs32_update(&m_fcs32, pBuf, uRead);

			if(m_bAlgorithm[HASHID_GHASH_32_3] || m_bAlgorithm[HASHID_GHASH_32_5])
				m_ghash.Update(pBuf, uRead);

			if(m_bAlgorithm[HASHID_GOST])
				gosthash_update(&m_gost, pBuf, uRead);

			if(m_bAlgorithm[HASHID_HAVAL])
				haval_hash(&m_haval, pBuf, uRead);

			if(m_bAlgorithm[HASHID_MD2])
				m_md2.Update(pBuf, uRead);

			if(m_bAlgorithm[HASHID_MD4])
				MD4Update(&m_md4, pBuf, uRead);

			if(m_bAlgorithm[HASHID_MD5])
				MD5Update(&m_md5, pBuf, uRead);

			if(m_bAlgorithm[HASHID_SHA1])
				sha1_hash(pBuf, uRead, &m_sha1);

			if(m_bAlgorithm[HASHID_SHA2_256])
				sha256_hash(pBuf, uRead, &m_sha256);

			if(m_bAlgorithm[HASHID_SHA2_384])
				sha384_hash(pBuf, uRead, &m_sha384);

			if(m_bAlgorithm[HASHID_SHA2_512])
				sha512_hash(pBuf, uRead, &m_sha512);

			if(m_bAlgorithm[HASHID_SIZE_32])
				sizehash32_hash(&m_uSizeHash32, uRead);

			if(m_bAlgorithm[HASHID_TIGER])
				tiger_process(&m_tiger, pBuf, uRead);
		}

		if(uRead != SIZE_HASH_BUFFER) break;
	}

	fclose(fp); fp = NULL;

	// SizeHash-32 is the first hash, because it's the simplest one,
	// the fastest, and most widely used one. ;-)
	if(m_bAlgorithm[HASHID_SIZE_32])
	{
		sizehash32_end(&m_uSizeHash32);
		printf(SZ_SIZEHASH_32);
		printf(SZ_HASHPRE);

		printf("%08X", m_uSizeHash32);

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_CRC16])
	{
		crc16_final(&m_crc16);
		printf(SZ_CRC16);
		printf(SZ_HASHPRE);

		printf("%04X", m_crc16);

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_CRC16CCITT])
	{
		crc16ccitt_final(&m_crc16ccitt);
		printf(SZ_CRC16CCITT);
		printf(SZ_HASHPRE);

		printf("%04X", m_crc16ccitt);

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_CRC32])
	{
		crc32Finish(&m_crc32);
		printf(SZ_CRC32);
		printf(SZ_HASHPRE);

		printf("%08X", m_crc32);

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_FCS_16])
	{
		fcs16_final(&m_fcs16);
		printf(SZ_FCS_16);
		printf(SZ_HASHPRE);

		printf("%04X", m_fcs16);

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_FCS_32])
	{
		fcs32_final(&m_fcs32);
		printf(SZ_FCS_32);
		printf(SZ_HASHPRE);

		printf("%08X", m_fcs32);

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_GHASH_32_3])
	{
		m_ghash.FinalToStr(szTemp, 3);
		printf(SZ_GHASH_32_3);
		printf(SZ_HASHPRE);

		printf(szTemp);

		printf(CPS_NEWLINE);
	}
	if(m_bAlgorithm[HASHID_GHASH_32_5])
	{
		m_ghash.FinalToStr(szTemp, 5);
		printf(SZ_GHASH_32_5);
		printf(SZ_HASHPRE);

		printf(szTemp);

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_GOST])
	{
		gosthash_final(&m_gost, pTemp);
		printf(SZ_GOST);
		printf(SZ_HASHPRE);

		for(i = 0; i < 32; i++)
		{
			fmtFixHashOutput(i);
			printf("%02X", pTemp[i]);
		}

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_HAVAL])
	{
		haval_end(&m_haval, pTemp);
		printf(SZ_HAVAL);
		printf(SZ_HASHPRE);

		for(i = 0; i < 32; i++)
		{
			fmtFixHashOutput(i);
			printf("%02X", pTemp[i]);
		}

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_MD2])
	{
		m_md2.TruncatedFinal(pTemp, 16);
		printf(SZ_MD2);
		printf(SZ_HASHPRE);

		for(i = 0; i < 16; i++)
		{
			fmtFixHashOutput(i);
			printf("%02X", pTemp[i]);
		}

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_MD4])
	{
		MD4Final(pTemp, &m_md4);
		printf(SZ_MD4);
		printf(SZ_HASHPRE);

		for(i = 0; i < 16; i++)
		{
			fmtFixHashOutput(i);
			printf("%02X", pTemp[i]);
		}

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_MD5])
	{
		MD5Final(&m_md5);
		printf(SZ_MD5);
		printf(SZ_HASHPRE);

		for(i = 0; i < 16; i++)
		{
			fmtFixHashOutput(i);
			printf("%02X", m_md5.digest[i]);
		}

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_SHA1])
	{
		sha1_end(pTemp, &m_sha1);
		printf(SZ_SHA1);
		printf(SZ_HASHPRE);

		for(i = 0; i < 20; i++)
		{
			fmtFixHashOutput(i);
			printf("%02X", pTemp[i]);
		}

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_SHA2_256])
	{
		sha256_end(pTemp, &m_sha256);
		printf(SZ_SHA2_256);
		printf(SZ_HASHPRE);

		for(i = 0; i < 32; i++)
		{
			fmtFixHashOutput(i);
			printf("%02X", pTemp[i]);
		}

		printf(CPS_NEWLINE);
	}
	if(m_bAlgorithm[HASHID_SHA2_384])
	{
		sha384_end(pTemp, &m_sha384);
		printf(SZ_SHA2_384);
		printf(SZ_HASHPRE);

		for(i = 0; i < 48; i++)
		{
			fmtFixHashOutput(i);
			printf("%02X", pTemp[i]);
		}

		printf(CPS_NEWLINE);
	}
	if(m_bAlgorithm[HASHID_SHA2_512])
	{
		sha512_end(pTemp, &m_sha512);
		printf(SZ_SHA2_512);
		printf(SZ_HASHPRE);

		for(i = 0; i < 64; i++)
		{
			fmtFixHashOutput(i);
			printf("%02X", pTemp[i]);
		}

		printf(CPS_NEWLINE);
	}

	if(m_bAlgorithm[HASHID_TIGER])
	{
		tiger_done(&m_tiger, pTemp);
		printf(SZ_TIGER);
		printf(SZ_HASHPRE);

		for(i = 0; i < 8; i++) { fmtFixHashOutput(i); printf("%02X", pTemp[7-i]); }
		for(i = 8; i < 16; i++) { fmtFixHashOutput(i); printf("%02X", pTemp[23-i]); }
		for(i = 16; i < 24; i++) { fmtFixHashOutput(i); printf("%02X", pTemp[39-i]); }

		printf(CPS_NEWLINE);
	}

	return RH_SUCCESS;
}

void CHashManager::fmtFixHashOutput(int nCursorPos)
{
	unsigned int i = 0;

	if((nCursorPos % 4 == 0) && (nCursorPos != 0))
	{
		if(nCursorPos % NUM_DIGBLOCKS_PER_LINE == 0)
		{
			printf(CPS_NEWLINE);
			printf(SZ_EMPTY);

			for(i = 0; i < strlen(SZ_HASHPRE); i++) printf(" ");
		}
		else
		{
			printf(SZ_HASHSEP);
		}
	}
}
