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

#ifndef ___HASH_MANAGER_H___
#define ___HASH_MANAGER_H___

#include "rhsyscfg.h"
#include "clutil.h"

#include "algo/crc16.h"
#include "algo/crc32.h"
#include "algo/fcs.h"
#include "algo/ghash.h"
#include "algo/gosthash.h"
#include "algo/haval.h"
#include "algo/md2.h"
#include "algo/md4.h"
#include "algo/md5.h"
#include "algo/sizehash.h"
#include "algo/sha1.h"
#include "algo/sha2.h"
#include "algo/tiger.h"

#define HASHID_NULL       0
#define HASHID_SIZE_32    1
#define HASHID_CRC16      2
#define HASHID_CRC16CCITT 3
#define HASHID_CRC32      4
#define HASHID_FCS_16     5
#define HASHID_FCS_32     6
#define HASHID_GHASH_32_3 7
#define HASHID_GHASH_32_5 8
#define HASHID_GOST       9
#define HASHID_HAVAL      10
#define HASHID_MD2        11
#define HASHID_MD4        12
#define HASHID_MD5        13
#define HASHID_SHA1       14
#define HASHID_SHA2_256   15
#define HASHID_SHA2_384   16
#define HASHID_SHA2_512   17
#define HASHID_TIGER      18
#define RH_MAX_ALGORITHMS 19

#define OPT_NULL          0
#define OPT_FULLPATH      1
#define OPT_RECURSIVE     2
#define OPT_MAX_OPTIONS   3

// You may use all 12 characters
#define SZ_EMPTY       "            "
#define SZ_SIZEHASH_32 "Size-32     "
#define SZ_CRC16       "CRC-16      "
#define SZ_CRC16CCITT  "CRC-16-CCITT"
#define SZ_CRC32       "CRC-32      "
#define SZ_FCS_16      "FCS-16      "
#define SZ_FCS_32      "FCS-32      "
#define SZ_GHASH_32_3  "GHash-32-3  "
#define SZ_GHASH_32_5  "GHash-32-5  "
#define SZ_GOST        "GOST-Hash   "
#define SZ_HAVAL       "HAVAL-5-256 "
#define SZ_MD2         "MD2         "
#define SZ_MD4         "MD4         "
#define SZ_MD5         "MD5         "
#define SZ_SHA1        "SHA-1       "
#define SZ_SHA2_256    "SHA-256     "
#define SZ_SHA2_384    "SHA-384     "
#define SZ_SHA2_512    "SHA-512     "
#define SZ_TIGER       "Tiger       "

#define SZ_HASHPRE     " : "
#define SZ_HASHSEP     " "

#define SIZE_HASH_BUFFER       16384
#define NUM_DIGBLOCKS_PER_LINE 24

class CHashManager
{
public:
	// Constructor and destructor
	CHashManager();
	virtual ~CHashManager();

	void SelectAllAlgorithms(bool bSelect = true);
	bool SelectAlgorithm(int nAlgorithm, bool bSelect = true);
	bool SetOption(int nOption, bool bState);

	int HashPath(char *pszBasePath, char *pszFileSpec);
	int HashFile(char *pszFile);

private:
	void fmtFixHashOutput(int nCursorPos);

	bool m_bAlgorithm[RH_MAX_ALGORITHMS];

	bool m_bFullPath;
	bool m_bRecursive;

	unsigned short m_crc16;
	unsigned short m_crc16ccitt;
	unsigned long m_crc32;
	unsigned short m_fcs16;
	unsigned long m_fcs32;
	CGHash m_ghash;
	GostHashCtx m_gost;
	haval_state m_haval;
	CMD2 m_md2;
	MD4_CTX m_md4;
	MD5_CTX m_md5;
	sha1_ctx m_sha1;
	sha256_ctx m_sha256;
	sha384_ctx m_sha384;
	sha512_ctx m_sha512;
	unsigned long m_uSizeHash32;
	tiger_hash_state m_tiger;
};

#endif // ___HASH_MANAGER_H___
