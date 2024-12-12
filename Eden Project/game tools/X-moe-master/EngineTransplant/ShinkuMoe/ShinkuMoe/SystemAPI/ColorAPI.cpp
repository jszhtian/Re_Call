#include "ColorAPI.h"
#include "ColorRGBA.h"

//rgba - abgr
int __fastcall ColorSet(ScriptObject* stack)
{
	int a = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar;
	int b = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;
	int g = stack->LocalStack[stack->CurStackFrame - 2].Value.IntVar;
	int r = stack->LocalStack[stack->CurStackFrame - 3].Value.IntVar;
	int table = stack->LocalStack[stack->CurStackFrame - 4].Value.IntVar;
	
	stack->CurStackFrame -= 5;

	if(table < 0 || table > 256)
	{
		return -1;
	}

	ColorRGBA::GetHandle()->SetColor(table, r, g, b, a);
	return 0;
}
