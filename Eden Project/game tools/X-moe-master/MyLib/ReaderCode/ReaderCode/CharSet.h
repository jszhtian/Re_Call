#pragma once

#include <Windows.h>

int WideCharToUtf8String(const WCHAR *in, char * out);
int Utf8ToWideCharString(const char * in, WCHAR *out);

size_t Mbstowcs(WCHAR *pwcs, const char *s, size_t n, ULONG CodePage);

#define Narrowtowidelen(X, c) Mbstowcs(NULL, (X),0, c)
#define Narrowtowide Mbstowcs
