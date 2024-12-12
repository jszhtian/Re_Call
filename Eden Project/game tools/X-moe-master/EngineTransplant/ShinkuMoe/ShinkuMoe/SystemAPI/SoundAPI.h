#pragma once

#include "ScriptObject.h"

int __fastcall AudioLoad(ScriptObject *stack);
int __fastcall AudioPlay(ScriptObject *stack);
int __fastcall AudioStop(ScriptObject *stack);
int __fastcall AudioSilentOn(ScriptObject *stack);
int __fastcall AudioState(ScriptObject *stack);
int __fastcall AudioType(ScriptObject *stack);
int __fastcall AudioVol(ScriptObject *stack);
int __fastcall SoundLoad(ScriptObject *stack);
int __fastcall SoundMasterVol(ScriptObject *stack);
int __fastcall SoundPlay(ScriptObject *stack);
int __fastcall SoundSilentOn(ScriptObject *stack);
int __fastcall SoundStop(ScriptObject *stack);
int __fastcall SoundType(ScriptObject *stack);
int __fastcall SoundTypeVol(ScriptObject *stack);
int __fastcall SoundVol(ScriptObject *stack);
int __fastcall SoundPan(ScriptObject *stack);
int __fastcall SoundVolPrev(ScriptObject *stack);
