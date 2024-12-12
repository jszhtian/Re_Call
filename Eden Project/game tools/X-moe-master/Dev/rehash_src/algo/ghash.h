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

#ifndef ___GHASH_H___
#define ___GHASH_H___

class CGHash
{
public:
	CGHash();
	virtual ~CGHash();

	void Init();
	void Update(const unsigned char *pData, unsigned long uSize);
	void FinalToStr(char *strOutput, int nHash);

private:
	unsigned long m_hash3;
	unsigned long m_hash5;
};

#endif // ___GHASH_H___
