#pragma once

#include <cstring>

struct tString
{
	char *buffer;
	bool isCopyByPointer;
	int length;
	int tReferCount;
	tString() : buffer(0), tReferCount(0), length(0),
		isCopyByPointer(true)
	{
		tReferCount++;
	}
	void OnChange()
	{
		isCopyByPointer = false;
		char* prev_buffer = buffer;
		buffer = new char[0x100];
		memset(buffer, 0, 0x100);
		strcpy(buffer, prev_buffer);
		tReferCount = 1;
	}
	void AddRef()
	{
		tReferCount++;
	}
	void RealseRef()
	{
		tReferCount--;
		if (tReferCount <= 0)
		{
			if (buffer)
				delete buffer;
		}
	}
	~tString()
	{
		tReferCount--;
		if (tReferCount <= 0)
		{
			if (buffer)
				delete buffer;
		}
	}
};

class VarInfo
{
public:
	enum
	{
		pINT   = 1,
		pFLOAT = 2,
		STRING = 3,
	}type;

	union
	{
		__int32 IntVar;
		float   FloatVar;
		tString* StringVar;
	}Value;
	
	//Setxx
	VarInfo &operator = (const VarInfo &var);
	bool     operator ==(const VarInfo &var);
	bool     operator >=(const VarInfo &var);
	bool     operator <=(const VarInfo &var);
	bool     operator !=(const VarInfo &var);
	bool     operator > (const VarInfo &var);
	bool     operator < (const VarInfo &var) const;
    
    friend VarInfo &operator + (const VarInfo &lhs, const VarInfo &rhs);
    friend VarInfo &operator - (const VarInfo &lhs, const VarInfo &rhs);
    friend VarInfo &operator * (const VarInfo &lhs, const VarInfo &rhs);
    friend VarInfo &operator / (const VarInfo &lhs, const VarInfo &rhs);
    friend VarInfo &operator % (const VarInfo &lhs, const VarInfo &rhs);
    
	VarInfo();
	VarInfo(int &rhs);
	VarInfo(const char *rhs);
	VarInfo(float &rhs);

	void Relase();

	~VarInfo();
};

