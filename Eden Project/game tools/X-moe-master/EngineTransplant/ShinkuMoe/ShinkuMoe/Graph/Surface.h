#pragma once

#include <SDL.h>
#include "Rect.h"

struct Surface : public SDL_Surface
{
	char* Mem(const Rect& r)
	{
		return (char*)(pixels)+r.ty * pitch + r.lx * format->BytesPerPixel;
	}

	char* MemEnd(const Rect& r)
	{
		return (char*)(pixels)+(r.by - 1) * pitch + r.rx * format->BytesPerPixel;
	}
};
