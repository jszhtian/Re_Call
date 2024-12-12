#pragma once

#include "ScriptObject.h"

int __fastcall ConfigDisplay(ScriptObject *stack);
int __fastcall ConfigEtc(ScriptObject *stack);
int __fastcall ConfigSound(ScriptObject *stack);
int __fastcall ConfigSet(ScriptObject *stack);
int __fastcall LoadFile(ScriptObject *stack);
int __fastcall LoadQuick(ScriptObject *stack);
int __fastcall LoadTitle(ScriptObject *stack);
int __fastcall SaveFile(ScriptObject *stack);
int __fastcall SaveLoadMenu(ScriptObject *stack);
int __fastcall SaveName(ScriptObject *stack);
int __fastcall SaveQuick(ScriptObject *stack);
int __fastcall SaveTitle(ScriptObject *stack);
int __fastcall TextHistory(ScriptObject *stack);
int __fastcall TextHyphenation(ScriptObject *stack);
int __fastcall MoviePlay(ScriptObject *stack);
int __fastcall QuickCopy(ScriptObject *stack);
int __fastcall QuickState(ScriptObject *stack);

