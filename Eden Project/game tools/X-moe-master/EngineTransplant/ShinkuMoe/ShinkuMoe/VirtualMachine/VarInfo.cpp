#include "VarInfo.h"
#include "ErrorLog.h"


VarInfo::VarInfo()
{
	Value.StringVar = 0;
	type = pINT;
}

VarInfo::VarInfo(const char *rhs)
{
	int length = strlen(rhs);
	Value.StringVar = new tString;
	Value.StringVar->buffer = new char[length+1];
	memcpy(Value.StringVar->buffer, rhs, length+1);
	Value.StringVar->length = length;
	type= STRING;
}

VarInfo::VarInfo(int &rhs)
{
	Value.IntVar = rhs;
	type = pINT;
}

VarInfo::~VarInfo()
{
	if(type == STRING)
	{
		delete Value.StringVar;
	}
	Value.StringVar = 0;
}

VarInfo &VarInfo::operator = (const VarInfo &rhs)
{
	if (this->type == VarInfo::STRING)
	{
		this->Value.StringVar->RealseRef();
	}
	memcpy(this, &rhs, sizeof(VarInfo));
	if (rhs.type == VarInfo::STRING)
	{
		rhs.Value.StringVar->AddRef();
	}
	return *this;
}


bool VarInfo::operator ==(const VarInfo &rhs)
{
	if(type == pINT&& rhs.type == pINT)
	{
		if(Value.IntVar == rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(Value.FloatVar == Value.FloatVar)
			return true;
		else
			return false; 
	}
	else if(type == STRING && rhs.type == STRING)
	{
		if(Value.StringVar->length != rhs.Value.StringVar->length)
			return false;
		if (Value.StringVar->buffer == rhs.Value.StringVar->buffer)
			return true;
		if (!strcmp(Value.StringVar->buffer, rhs.Value.StringVar->buffer))
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(Value.FloatVar == (float)rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if((float)Value.IntVar == rhs.Value.FloatVar)
			return true;
		else
			return false;
	}
	else if(type == pINT && rhs.type == STRING)
	{
		if(Value.IntVar == 0 && rhs.Value.StringVar->length==0)
			return true;
		else 
			return false;
	}
	else if(type == STRING && rhs.type == pINT)
	{
		if(Value.StringVar->length == 0 && rhs.Value.IntVar == 0)
			return true;
		else
			return false;
	}
	else 
	{
		ErrorLog(fvpError,"Compare error with [%d][%d]\n",type, rhs.type);
		return false;
	}
}

bool VarInfo::operator >=(const VarInfo &rhs)
{
	if(type == pINT && rhs.type == pINT)
	{
		if(Value.IntVar >= rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(Value.FloatVar >= rhs.Value.FloatVar)
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(Value.FloatVar >= (float)rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if((float)Value.IntVar >= rhs.Value.FloatVar)
			return true;
		else
			return false;
	}
	//ÈÝ´í 
	else return false; 
}

bool VarInfo::operator <=(const VarInfo &rhs)
{
	if(type == pINT && rhs.type == pINT)
	{
		if(Value.IntVar <= rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(Value.FloatVar <= rhs.Value.FloatVar)
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(Value.FloatVar <= (float)rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if((float)Value.IntVar <= rhs.Value.FloatVar)
			return true;
		else
			return false;
	}
	//ÈÝ´í 
	else return false; 
}

bool VarInfo::operator > (const VarInfo &rhs)
{
	if(type == pINT && rhs.type == pINT)
	{
		if(Value.IntVar > rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(Value.FloatVar > rhs.Value.FloatVar)
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(Value.FloatVar > (float)rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if((float)Value.IntVar > rhs.Value.FloatVar)
			return true;
		else
			return false;
	}
	//ÈÝ´í 
	else return false; 
}

bool VarInfo::operator < (const VarInfo &rhs) const
{
	if(type == pINT && rhs.type == pINT)
	{
		if(Value.IntVar < rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(Value.FloatVar < rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(Value.FloatVar < (float)rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if((float)Value.IntVar < rhs.Value.FloatVar)
			return true;
		else
			return false;
	}
	//ÈÝ´í 
	else return false; 
}

bool VarInfo::operator !=(const VarInfo &rhs)
{
	if(type == pINT && rhs.type == pINT)
	{
		if(Value.IntVar != rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == STRING && rhs.type == STRING)
	{
		if (Value.StringVar->length != rhs.Value.StringVar->length)
			return false;
		//±ØÐë±È½ÏÊý¾Ý
		if (strcmp(Value.StringVar->buffer, rhs.Value.StringVar->buffer))
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pFLOAT)
	{
		if(Value.FloatVar != rhs.Value.FloatVar)
			return true;
		else
			return false;
	}
	else if(type == pFLOAT && rhs.type == pINT)
	{
		if(Value.FloatVar != (float)rhs.Value.IntVar)
			return true;
		else
			return false;
	}
	else if(type == pINT && rhs.type ==pFLOAT)
	{
		if(Value.IntVar != rhs.Value.FloatVar)
			return true;
		else
			return false;
	}
	//ÈÝ´í 
	else return false; 
}

/*************************************/

VarInfo &operator + (const VarInfo &lhs, const VarInfo &rhs)
{
	VarInfo *ans = new VarInfo;
	if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::STRING)
	{
		//if(lhs.Value.IntVar == 0)
		//{
			ans->Value.StringVar = new tString;
			int length = rhs.Value.StringVar->length;
			ans->Value.StringVar->buffer = new char[length + 1];
			ans->Value.StringVar->length = length;
			memcpy(ans->Value.StringVar->buffer, rhs.Value.StringVar->buffer, length + 1);
			ans->type = VarInfo::STRING;
		//}
		//else
		//{
			
		//}
		return *ans;
	}
	else if(lhs.type == VarInfo::STRING && rhs.type == VarInfo::pINT)
	{
		//if(rhs.Value.IntVar == 0)
		//{
			ans->Value.StringVar = new tString;
			int length = lhs.Value.StringVar->length;
			ans->Value.StringVar->buffer = new char[length + 1];
			ans->Value.StringVar->length = length;
			memcpy(ans->Value.StringVar->buffer, lhs.Value.StringVar->buffer, length + 1);
			ans->type = VarInfo::STRING;
		//}
		//else
		//{
		//	;
		//}
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->Value.IntVar= lhs.Value.IntVar + rhs.Value.IntVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar = lhs.Value.FloatVar + rhs.Value.FloatVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar  = (float)lhs.Value.IntVar + rhs.Value.FloatVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar  = lhs.Value.FloatVar + (float)rhs.Value.IntVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::STRING && rhs.type == VarInfo::STRING)
	{
		ans->type = VarInfo::STRING; 
		ans->Value.StringVar = new tString;
		int length = rhs.Value.StringVar->length + lhs.Value.StringVar->length;
		ans->Value.StringVar->buffer = new char[length + 1];
		ans->Value.StringVar->length = length;
		memcpy(ans->Value.StringVar->buffer, lhs.Value.StringVar->buffer, lhs.Value.StringVar->length);
		memcpy(ans->Value.StringVar->buffer + lhs.Value.StringVar->length,
											 rhs.Value.StringVar->buffer, rhs.Value.StringVar->length + 1);
		ans->type = VarInfo::STRING;
		return *ans;
	}
	else
	{
		ans->type = VarInfo::pINT;
		return *ans;
	}
}

VarInfo &operator - (const VarInfo &lhs, const VarInfo &rhs)
{
	VarInfo *ans = new VarInfo;
	if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->Value.IntVar  = lhs.Value.IntVar - rhs.Value.IntVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar = lhs.Value.FloatVar - rhs.Value.FloatVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar  = (float)lhs.Value.IntVar - rhs.Value.FloatVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar  = lhs.Value.FloatVar - (float)rhs.Value.IntVar;
		return *ans;
	}
	else
	{
		ans->type = VarInfo::pINT;
		return *ans;
	}
}

VarInfo &operator * (const VarInfo &lhs, const VarInfo &rhs)
{
	VarInfo *ans = new VarInfo;
	if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->Value.IntVar  = lhs.Value.IntVar * rhs.Value.IntVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar = lhs.Value.FloatVar * rhs.Value.FloatVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar  = (float)lhs.Value.IntVar * rhs.Value.FloatVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar  = lhs.Value.FloatVar * (float)rhs.Value.IntVar;
		return *ans;
	}
	else
	{
		ans->type = VarInfo::pINT;
		return *ans;
	}
}

VarInfo &operator / (const VarInfo &lhs, const VarInfo &rhs)
{
	VarInfo *ans = new VarInfo;
	if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->Value.IntVar  = lhs.Value.IntVar / rhs.Value.IntVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar = lhs.Value.FloatVar / rhs.Value.FloatVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pINT;
		ans->Value.FloatVar  = (float)lhs.Value.IntVar / rhs.Value.FloatVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pFLOAT;
		ans->Value.FloatVar  = lhs.Value.FloatVar / rhs.Value.IntVar;
		return *ans;
	}
	else
	{
		ans->type = VarInfo::pINT;
		return *ans;
	}
}

VarInfo &operator % (const VarInfo &lhs, const VarInfo &rhs)
{
	VarInfo *ans = new VarInfo;
	if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->Value.IntVar  = lhs.Value.IntVar % rhs.Value.IntVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pINT;
		ans->Value.IntVar = (int)lhs.Value.FloatVar % (int)rhs.Value.FloatVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pINT && rhs.type == VarInfo::pFLOAT)
	{
		ans->type = VarInfo::pINT;
		ans->Value.IntVar  = lhs.Value.IntVar % (int)rhs.Value.FloatVar;
		return *ans;
	}
	else if(lhs.type == VarInfo::pFLOAT && rhs.type == VarInfo::pINT)
	{
		ans->type = VarInfo::pINT;
		ans->Value.IntVar  = (int)lhs.Value.FloatVar % rhs.Value.IntVar;
		return *ans;
	}
	else
	{
		ans->type = VarInfo::pINT;
		return *ans;
	}
}


