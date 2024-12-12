#ifndef __VARINFO_H__
#define __VARINFO_H__

//#include <cstdio>
//#include <windows.h>
#include <cstring>

 
//VM����ÿ��������������ά����
//type��
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
	//������һ��32λ�ļĴ��� ��� 
	__int32      var;
	//����һ��������
	float        fvar; 
	//�����һ��String,�͸Ķ����ָ��Ϳ����ˡ� 
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
    
    //ҪС��+����string���������ַ��� 
	//base
	VarInfo();
	VarInfo(int &rhs);
	VarInfo(char *rhs);
	VarInfo(float &rhs);
	~VarInfo();
};

#endif