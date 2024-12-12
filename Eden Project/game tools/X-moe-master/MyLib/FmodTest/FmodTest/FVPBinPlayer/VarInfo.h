#ifndef __VARINFO_H__
#define __VARINFO_H__

//#include <cstdio>
//#include <windows.h>
#include <cstring>

 
//VM里面每个变量都是这样维护的
//type：
//0 bool0
//1 bool1
class VarInfo
{
public:
	enum
	{
		pINT   = 1,
		pFLOAT = 2,
		STRING = 3,
		pError = 0xFF
	}            type;
	//变量是一个32位的寄存器 大概 
	__int32      var;
	//对于一个浮点数
	float        fvar; 
	//如果是一个String,就改动这个指针就可以了。 
	char        str[0xFF];
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
    
    //要小心+对于string就是连接字符串 
	//base
	VarInfo();
	VarInfo(int &rhs);
	VarInfo(char *rhs);
	VarInfo(float &rhs);
	~VarInfo();
};

#endif