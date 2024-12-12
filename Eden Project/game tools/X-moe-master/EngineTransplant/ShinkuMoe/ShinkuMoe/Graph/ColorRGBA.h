#pragma once

#include "ErrorLog.h"

//@private
struct ColorStruct
{
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;

	ColorStruct(): a(0), r(0), g(0), b(0){}
};

class ColorRGBA
{
private:
	//[0,256)
	ColorStruct* ColorTable[256];
	ColorRGBA();

public:

	static ColorRGBA* Handle;
	static ColorRGBA* Init();
	static ColorRGBA* GetHandle();

public:

	~ColorRGBA();
	
	void SetColor(int index, unsigned char r_value, unsigned char g_value, unsigned char b_value, unsigned char a_value);
	void GetColor(int index, unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a);

private:
	void Reset(int index);
};


