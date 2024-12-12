#include "VarInfo.h"
#include "stdafx.h"


//VarInfo也可以称为StackFrame
//毕竟Stack就是这样维护的

VarInfo::VarInfo()
{
	memset(str, 0, sizeof(str));
	var = 0;
	fvar= 0.0f; 
}

VarInfo::VarInfo(char *rhs)
{
	var = 0;
	fvar = 0.0f;
	memcpy(str,rhs,strlen(rhs));
	type= STRING;
}

VarInfo::VarInfo(int &rhs)
{
	memset(str, 0, sizeof(str));
	fvar = 0.0f;
	type = pINT;
	var  = rhs;
}

VarInfo::~VarInfo()
{
	memset(str, 0, sizeof(str));
	var = 0;
	fvar= 0.0f;
}

VarInfo &VarInfo::operator = (const VarInfo &rhs)
{
	//memcpy(this,&var,sizeof(VarInfo));
	if(rhs.type == pINT)
	{
		if(type = STRING)
		{
			memset(str, 0, sizeof(str));
		}
		type = pINT;
		var  = rhs.var;
		fvar = 0.0f;
	}
	else if(rhs.type == pFLOAT)
	{
		if(type = STRING)
		{
			memset(str, 0, sizeof(str));
		}
		fvar = rhs.fvar;
		type = pFLOAT;
		var  = 0;
	}
	else if(rhs.type == STRING)
	{
		if(type != STRING)
		{
			type = STRING;
			var = 0;
			fvar= 0;
			strcpy(str, rhs.str);
		}
		else
		{
			memset(str, 0, sizeof(str));
			strcpy(str, rhs.str);
		}
	}
	return *this;
}

bool VarInfo::operator ==(const VarInfo &rhs)
{
	//push0 push1 
	if(type == pINT&& rhs.type == pINT)
	{
		if(var == rhs.var) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(fvar == rhs.fvar) return true;
		else return false; 
	}
	else if(type == STRING && rhs.type == STRING)
	{
		if (!strcmp(str, rhs.str)) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(fvar == rhs.var) return true;
		else return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if(var == rhs.fvar) return true;
		else return false;
	}
	//剩下的这几种我不确定是否会用上 
	else 
	{
		//ErrorLog(fvpError,"VarInfo::CMP, Cannot Exec!\n");
		return false;
	}
}

bool VarInfo::operator >=(const VarInfo &rhs)
{
	if(type == pINT && rhs.type == pINT)
	{
		if(var >= rhs.var) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(fvar >= rhs.fvar) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(fvar >= rhs.var) return true;
		else return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if(var >= rhs.fvar) return true;
		else return false;
	}
	//容错 
	else return false; 
}

bool VarInfo::operator <=(const VarInfo &rhs)
{
	if(type == pINT && rhs.type == pINT)
	{
		if(var <= rhs.var) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(fvar <= rhs.fvar) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(fvar <= rhs.var) return true;
		else return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if(var <= rhs.fvar) return true;
		else return false;
	}
	//容错 
	else return false; 
}

bool VarInfo::operator > (const VarInfo &rhs)
{
	if(type == pINT && rhs.type == pINT)
	{
		if(var > rhs.var) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(fvar > rhs.fvar) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(fvar > rhs.var) return true;
		else return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if(var > rhs.fvar) return true;
		else return false;
	}
	//容错 
	else return false; 
}

bool VarInfo::operator < (const VarInfo &rhs) const
{
	if(type == pINT && rhs.type == pINT)
	{
		if(var < rhs.var) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(fvar < rhs.fvar) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(fvar < rhs.var) return true;
		else return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if(var < rhs.fvar) return true;
		else return false;
	}
	//容错 
	else return false; 
}

bool VarInfo::operator !=(const VarInfo &rhs)
{
	if(type == pINT && rhs.type == pINT)
	{
		if(var != rhs.var)return true;
		else return false;
	}
	else if(type == STRING && rhs.type == STRING)
	{
		if(strcmp(str, rhs.str)) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(fvar != rhs.fvar) return true;
		else return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(fvar != rhs.var) return true;
		else return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if(var != rhs.fvar) return true;
		else return false;
	}
	//容错 
	else return false; 
}

/*************************************/

VarInfo &operator + (const VarInfo &lhs, const VarInfo &rhs)
{
	VarInfo *ans = new VarInfo;
	if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::STRING)
	{
		if(lhs.var == 0)
		{
			strcpy(ans->str, rhs.str);
			ans->type = VarInfo::STRING;
		}
		else
		{
			ans->type = VarInfo::pError;
		}
		return *ans;
	}
	else if(lhs.type == VarInfo::STRING && rhs.type == VarInfo::pINT)
	{
		if(rhs.var == 0)
		{
			strcpy(ans->str, lhs.str);
			ans->type = VarInfo::STRING;
		}
		else
		{
			ans->type = VarInfo::pError;
		}
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->var  = lhs.var + rhs.var;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar = lhs.fvar + rhs.fvar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar  = (float)lhs.var + rhs.fvar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar  = lhs.fvar + (float)rhs.var;
		return *ans;
	}
	else if(lhs.type == VarInfo::STRING && rhs.type == VarInfo::STRING)
	{
		ans->type = VarInfo::STRING; 
		strcat(ans->str,lhs.str);
		strcat(ans->str,rhs.str);
		return *ans;
	}
	else
	{
		ans->type = VarInfo::pError;
		return *ans;
	}
}

VarInfo &operator - (const VarInfo &lhs, const VarInfo &rhs)
{
	VarInfo *ans = new VarInfo;
	if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->var  = lhs.var - rhs.var;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar = lhs.fvar - rhs.fvar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar  = (float)lhs.var - rhs.fvar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar  = lhs.fvar - (float)rhs.var;
		return *ans;
	}
	else
	{
		ans->type = VarInfo::pError;
		return *ans;
	}
}

VarInfo &operator * (const VarInfo &lhs, const VarInfo &rhs)
{
	VarInfo *ans = new VarInfo;
	if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->var  = lhs.var * rhs.var;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar = lhs.fvar * rhs.fvar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar  = (float)lhs.var * rhs.fvar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar  = lhs.fvar * (float)rhs.var;
		return *ans;
	}
	else
	{
		ans->type = VarInfo::pError;
		return *ans;
	}
}

VarInfo &operator / (const VarInfo &lhs, const VarInfo &rhs)
{
	VarInfo *ans = new VarInfo;
	if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->var  = lhs.var / rhs.var;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar = lhs.fvar / rhs.fvar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pINT;
		ans->fvar  = lhs.var / (int)rhs.fvar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->fvar  = lhs.fvar / rhs.var;
		return *ans;
	}
	else
	{
		ans->type = VarInfo::pError;
		return *ans;
	}
}

VarInfo &operator % (const VarInfo &lhs, const VarInfo &rhs)
{
	VarInfo *ans = new VarInfo;
	if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->var  = lhs.var % rhs.var;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pINT;
		ans->var = (int)lhs.fvar % (int)rhs.fvar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pINT;
		ans->fvar  = lhs.var % (int)rhs.fvar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->fvar  = (int)lhs.fvar % rhs.var;
		return *ans;
	}
	else
	{
		ans->type = VarInfo::pError;
		return *ans;
	}
}
