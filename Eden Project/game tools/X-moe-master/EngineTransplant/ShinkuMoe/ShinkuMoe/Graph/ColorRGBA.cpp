#include "ColorRGBA.h"
#include <memory>

ColorRGBA* ColorRGBA::Handle = nullptr;

ColorRGBA* ColorRGBA::Init()
{
	if (Handle == nullptr)
	{
		Handle = new ColorRGBA;
	}
	return Handle;
}

ColorRGBA* ColorRGBA::GetHandle()
{
	return Handle;
}

//@private
void ColorRGBA::Reset(int index)
{
	ColorTable[index]->a = 
	ColorTable[index]->b =
	ColorTable[index]->r =
	ColorTable[index]->g = 0;
}

ColorRGBA::ColorRGBA()
{
	memset(ColorTable, 0, sizeof(ColorTable));
}

//@public
void ColorRGBA::SetColor(int index, unsigned char r_value, unsigned char g_value, unsigned char b_value, unsigned char a_value)
{
	if(ColorTable[index] == nullptr)
		ColorTable[index] = new ColorStruct;

	ColorTable[index]->a = a_value;
	ColorTable[index]->b = b_value;
	ColorTable[index]->r = r_value;
	ColorTable[index]->g = g_value;
}

void ColorRGBA::GetColor(int index, unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a)
{
	if(ColorTable[index] == nullptr)
	{
		WriteLog(fvpError, "Trying to get an emtry color table index %d", index);
		ColorTable[index] = new ColorStruct;
	}
	r = ColorTable[index]->r;
	a = ColorTable[index]->a;
	b = ColorTable[index]->b;
	g = ColorTable[index]->g;
}

ColorRGBA::~ColorRGBA()
{
	for(int i = 0; i < 256; i++)
	{
		if(ColorTable[i])
			delete ColorTable[i];

		ColorTable[i] = nullptr;
	}
}
