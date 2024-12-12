#pragma once

#include <Windows.h>
#include <string>

using std::wstring;

#ifndef BS_READ
#define BS_READ 1
#endif

#ifndef BS_WRITE
#define BS_WRITE 2
#endif

class iTextReadStream
{
public:
	virtual ULONG NTAPI Read(wstring & targ, ULONG size) = 0;
	virtual void  NTAPI Destruct() = 0;
};

class iTextWriteStream
{
public:
	virtual void NTAPI Write(const wstring & targ) = 0;
	virtual void NTAPI Destruct() = 0;
};
