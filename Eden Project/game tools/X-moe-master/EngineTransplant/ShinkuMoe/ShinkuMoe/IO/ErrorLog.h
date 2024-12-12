#pragma once

enum
{
	fvpLog,
	fvpWarning,
	fvpError
};

bool InitLogSystem();
bool UnInitLogSystem();
void WriteLog(int Level, const char *format, ...);
