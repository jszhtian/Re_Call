#include "DebugAPI.h"
#include "ScriptObject.h"

//VarInfo dstWindow
//VarInfo debug_text
int __fastcall Debmess(ScriptObject* stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}
