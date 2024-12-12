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
#include <memory.h>
#include <string.h>

#include "rehash.h"
#include "clutil.h"
#include "hashmgr.h"

int main(int argc, char *argv[])
{
	int i = 0;
	bool bOnce = true;
	char szPath[RH_MAX_PATH];
	char szAbsPath[RH_MAX_PATH];
	char szOrgPath[RH_MAX_PATH];
	char szTemp[RH_MAX_BUFFER];
	int iErrorCode = 0;
	CHashManager hashmgr;

	getcwd(szOrgPath, RH_MAX_PATH);

	// No arguments?
	if(argc == 1)
	{
		printInfo();
		return(RH_NO_ARGS);
	}

	memset(szPath, 0, RH_MAX_PATH);
	bOnce = true;
	hashmgr.SelectAllAlgorithms(true);

	for(i = 1; i < argc; i++)
	{
		// Is the argument a path descriptor?
		if(isArgument(argv[i]) == false)
		{
			if(bOnce == false) strcat(szPath, " ");
			strcat(szPath, argv[i]);
			bOnce = false; // Next time add a space
			continue;
		}

		// The argument is an optional argument

		if(strlen(argv[i]) >= RH_MAX_BUFFER) continue; // Non-parsable option argument, ignore

		fmtArgument(argv[i], szTemp); // Format the argument, i.e. remove all special chars

		if(strcmp(szTemp, "help"    ) == 0) printInfo();
		if(strcmp(szTemp, "h"       ) == 0) printInfo();
		if(strcmp(szTemp, "?"       ) == 0) printInfo();
		if(strcmp(szTemp, "version" ) == 0) printInfo();
		if(strcmp(szTemp, "v"       ) == 0) printInfo();

		if(strcmp(szTemp, "fullpath") == 0) hashmgr.SetOption(OPT_FULLPATH, true);
		if(strcmp(szTemp, "f"       ) == 0) hashmgr.SetOption(OPT_FULLPATH, true);
		if(strcmp(szTemp, "nopath"  ) == 0) hashmgr.SetOption(OPT_FULLPATH, false);

		if(strcmp(szTemp, "rcrsv"   ) == 0) hashmgr.SetOption(OPT_RECURSIVE, true);
		if(strcmp(szTemp, "norcrsv" ) == 0) hashmgr.SetOption(OPT_RECURSIVE, false);
		if(strcmp(szTemp, "recur"   ) == 0) hashmgr.SetOption(OPT_RECURSIVE, true);
		if(strcmp(szTemp, "norecur" ) == 0) hashmgr.SetOption(OPT_RECURSIVE, false);
		if(strcmp(szTemp, "r"       ) == 0) hashmgr.SetOption(OPT_RECURSIVE, true);

		if(strcmp(szTemp, "all"     ) == 0) hashmgr.SelectAllAlgorithms(true);
		if(strcmp(szTemp, "a"       ) == 0) hashmgr.SelectAllAlgorithms(true);
		if(strcmp(szTemp, "none"    ) == 0) hashmgr.SelectAllAlgorithms(false);
		if(strcmp(szTemp, "n"       ) == 0) hashmgr.SelectAllAlgorithms(false);

		if(strcmp(szTemp, "crc16"   ) == 0) hashmgr.SelectAlgorithm(HASHID_CRC16, true);
		if(strcmp(szTemp, "nocrc16" ) == 0) hashmgr.SelectAlgorithm(HASHID_CRC16, false);
		if(strcmp(szTemp, "crc16c"  ) == 0) hashmgr.SelectAlgorithm(HASHID_CRC16CCITT, true);
		if(strcmp(szTemp, "nocrc16c") == 0) hashmgr.SelectAlgorithm(HASHID_CRC16CCITT, false);
		if(strcmp(szTemp, "crc32"   ) == 0) hashmgr.SelectAlgorithm(HASHID_CRC32, true);
		if(strcmp(szTemp, "nocrc32" ) == 0) hashmgr.SelectAlgorithm(HASHID_CRC32, false);

		if(strcmp(szTemp, "fcs16"   ) == 0) hashmgr.SelectAlgorithm(HASHID_FCS_16, true);
		if(strcmp(szTemp, "nofcs16" ) == 0) hashmgr.SelectAlgorithm(HASHID_FCS_16, false);
		if(strcmp(szTemp, "fcs32"   ) == 0) hashmgr.SelectAlgorithm(HASHID_FCS_32, true);
		if(strcmp(szTemp, "nofcs32" ) == 0) hashmgr.SelectAlgorithm(HASHID_FCS_32, false);

		if(strcmp(szTemp, "ghash3"  ) == 0) hashmgr.SelectAlgorithm(HASHID_GHASH_32_3, true);
		if(strcmp(szTemp, "noghash3") == 0) hashmgr.SelectAlgorithm(HASHID_GHASH_32_3, false);
		if(strcmp(szTemp, "ghash5"  ) == 0) hashmgr.SelectAlgorithm(HASHID_GHASH_32_5, true);
		if(strcmp(szTemp, "noghash5") == 0) hashmgr.SelectAlgorithm(HASHID_GHASH_32_5, false);

		if(strcmp(szTemp, "gost"    ) == 0) hashmgr.SelectAlgorithm(HASHID_GOST, true);
		if(strcmp(szTemp, "nogost"  ) == 0) hashmgr.SelectAlgorithm(HASHID_GOST, false);

		if(strcmp(szTemp, "haval"   ) == 0) hashmgr.SelectAlgorithm(HASHID_HAVAL, true);
		if(strcmp(szTemp, "nohaval" ) == 0) hashmgr.SelectAlgorithm(HASHID_HAVAL, false);

		if(strcmp(szTemp, "md2"     ) == 0) hashmgr.SelectAlgorithm(HASHID_MD2, true);
		if(strcmp(szTemp, "nomd2"   ) == 0) hashmgr.SelectAlgorithm(HASHID_MD2, false);
		if(strcmp(szTemp, "md4"     ) == 0) hashmgr.SelectAlgorithm(HASHID_MD4, true);
		if(strcmp(szTemp, "nomd4"   ) == 0) hashmgr.SelectAlgorithm(HASHID_MD4, false);
		if(strcmp(szTemp, "md5"     ) == 0) hashmgr.SelectAlgorithm(HASHID_MD5, true);
		if(strcmp(szTemp, "nomd5"   ) == 0) hashmgr.SelectAlgorithm(HASHID_MD5, false);

		if(strcmp(szTemp, "sha1"    ) == 0) hashmgr.SelectAlgorithm(HASHID_SHA1    , true);
		if(strcmp(szTemp, "nosha1"  ) == 0) hashmgr.SelectAlgorithm(HASHID_SHA1    , false);
		if(strcmp(szTemp, "sha256"  ) == 0) hashmgr.SelectAlgorithm(HASHID_SHA2_256, true);
		if(strcmp(szTemp, "nosha256") == 0) hashmgr.SelectAlgorithm(HASHID_SHA2_256, false);
		if(strcmp(szTemp, "sha384"  ) == 0) hashmgr.SelectAlgorithm(HASHID_SHA2_384, true);
		if(strcmp(szTemp, "nosha384") == 0) hashmgr.SelectAlgorithm(HASHID_SHA2_384, false);
		if(strcmp(szTemp, "sha512"  ) == 0) hashmgr.SelectAlgorithm(HASHID_SHA2_512, true);
		if(strcmp(szTemp, "nosha512") == 0) hashmgr.SelectAlgorithm(HASHID_SHA2_512, false);

		if(strcmp(szTemp, "size32"  ) == 0) hashmgr.SelectAlgorithm(HASHID_SIZE_32, true);
		if(strcmp(szTemp, "nosize32") == 0) hashmgr.SelectAlgorithm(HASHID_SIZE_32, false);

		if(strcmp(szTemp, "tiger"   ) == 0) hashmgr.SelectAlgorithm(HASHID_TIGER, true);
		if(strcmp(szTemp, "notiger" ) == 0) hashmgr.SelectAlgorithm(HASHID_TIGER, false);
	}

	// Check if a path is in the arguments
	bOnce = false;
	for(i = 1; i < argc; i++) if(isArgument(argv[i]) == 0) bOnce = true;
	if(bOnce == false) return RH_NO_PATH; // Silently exit

	fmtPath(szPath);

	strcpy(szAbsPath, szPath);
	pathonly(szAbsPath);
	if(strlen(szAbsPath) != 0)
	{
		if(chdir(szAbsPath) != 0)
		{
			printf("ERROR! Cannot change to directory: ");
			printf(szAbsPath);
			printf(CPS_NEWLINE);
			return RH_DIRECTORY_ERROR;
		}
	}

	fileonly(szPath);
	iErrorCode = hashmgr.HashPath(szAbsPath, szPath);

	if(chdir(szOrgPath) != 0) return RH_DIRECTORY_ERROR;
	return iErrorCode;
}

void printInfo()
{
	printf("ReichlSoft Hash Calculator");
	printf(CPS_NEWLINE);
	printf("Version: ");
	printf(SZ_REHASH_VERSION);
	printf(", Build: ");
	printf(__DATE__);
	printf(" ");
	printf(__TIME__);
	if(RH_TARGET_SYSTEM == RH_TARGET_SYSTEM_WINDOWS)
		printf(" - Windows");
	else if(RH_TARGET_SYSTEM == RH_TARGET_SYSTEM_LINUX)
		printf(" - Linux");
	printf(CPS_NEWLINE); printf(CPS_NEWLINE);
	printf("Free console-based hash calculator.");
	printf(CPS_NEWLINE); printf(CPS_NEWLINE);
	printf("Download latest version from ");
	printf(SZ_REHASH_HOMEPAGE);
	printf(".");
	printf(CPS_NEWLINE);
}
