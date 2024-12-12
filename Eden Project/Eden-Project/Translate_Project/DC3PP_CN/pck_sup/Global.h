#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <tchar.h>

FILE* fin;
static unsigned long Size;
static unsigned char* Buff;

static void SoraFileReaderW(_TCHAR* FileInput)
{
	fin = _wfopen(FileInput, L"rb");
	fseek(fin, 0, SEEK_END);
	Size = ftell(fin);
	rewind(fin);
	Buff = new unsigned char[Size];
	memset(Buff, 0, sizeof(Buff));
	fread(Buff, Size, 1, fin);
	fclose(fin);
}

static void SoraFileReaderA(char* FileInput)
{
	fin = fopen(FileInput, "rb");
	fseek(fin, 0, SEEK_END);
	Size = ftell(fin);
	rewind(fin);
	Buff = new unsigned char[Size];
	memset(Buff, 0, sizeof(Buff));
	fread(Buff, Size, 1, fin);
	fclose(fin);
}
