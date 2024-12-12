#include "ErrorLog.h"
#include <stdio.h>
#include "Mutex.h"

static AnzCriticalSection LoggerCS;
static FILE* LoggerFile = nullptr;

bool InitLogSystem()
{
	if (!LoggerFile)
	{
		LoggerFile = fopen("ShinkuMoe.txt", "wb");
		return LoggerFile != nullptr;
	}
	return true;
}

bool UnInitLogSystem()
{
	AnzCriticalSectionHolder Holder(LoggerCS);
	if (LoggerFile)
	{
		fclose(LoggerFile);
		LoggerFile = nullptr;
	}
	return true;
}

void WriteLog(int Level, const char *format, ...)
{
	AnzCriticalSectionHolder Holder(LoggerCS);

	if (!LoggerFile)
		return;

	switch(Level)
	{
		case fvpWarning:
		fprintf(LoggerFile, "[Warning]");
		break;
		
		case fvpError:
		fprintf(LoggerFile, "[Error  ]");
		break;
		
		case fvpLog:
		fprintf(LoggerFile, "[Log    ]");
		break;
	}

	va_list ap;
    va_start(ap, format);
	vfprintf(LoggerFile, format, ap);
    va_end(ap);

	fprintf(LoggerFile, "\n");
}


