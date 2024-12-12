#pragma once
#include "ScriptObject.h"

int __fastcall ThreadExit(ScriptObject *stack);
int __fastcall ThreadNext(ScriptObject *stack);
int __fastcall ThreadRaise(ScriptObject *stack);
int __fastcall ThreadSleep(ScriptObject *stack);
int __fastcall ThreadStart(ScriptObject *stack);
int __fastcall ThreadWait(ScriptObject *stack);

