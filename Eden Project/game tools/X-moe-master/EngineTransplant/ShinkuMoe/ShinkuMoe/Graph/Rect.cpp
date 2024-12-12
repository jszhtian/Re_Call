#include "Rect.h"
#include <vector>

#ifndef MAX
#define MAX(a, b) a > b ? a : b
#endif


#ifndef MIN
#define MIN(a, b) a > b ? b : a
#endif

Rect::Rect(int x1, int y1)
{
	lx = rx = x1;
	ty = by = y1;
}

Rect::Rect(int x1, int y1, int x2, int y2) 
{
	lx = MIN(x1, x2);
	rx = MAX(x1, x2);
	ty = MIN(y1, y2);
	by = MAX(y1, y2);
}

Rect::Rect(const Rect& r) 
{
	lx = r.lx;
	rx = r.rx;
	ty = r.ty;
	by = r.by;
}

bool Rect::is_inner(const Rect& inner_rect) const 
{
	Rect r = *this;
	r.intersect(inner_rect);
	return r == inner_rect;
}

bool Rect::is_nearly_inner(const Rect& inner_rect, int delta) const
{
	Rect r = *this;
	r.lx -= delta;
	r.ty -= delta;
	r.rx += delta;
	r.by += delta;
	r.intersect(inner_rect);
	return r == inner_rect;
}

bool Rect::is_crossed(const Rect& rect) const 
{
	Rect r = *this;
	r.intersect(rect);
	return !(r.empty());
}


void Rect::intersect(const Rect& r) 
{
	if (lx > r.rx) rx = lx;
	else if (rx < r.lx) lx = rx;
	else 
	{
		lx = MAX(lx, r.lx);
		rx = MIN(rx, r.rx);
	}

	if (ty > r.by) by = ty;
	else if (by < r.ty) ty = by;
	else 
	{
		ty = MAX(ty, r.ty);
		by = MIN(by, r.by);
	}
}


void Rect::join(const Rect& r) 
{
	lx = MIN(lx, r.lx);
	rx = MAX(rx, r.rx);
	ty = MIN(ty, r.ty);
	by = MAX(by, r.by);
}

void Rect::rmove(int add_x, int add_y) 
{
	lx += add_x;
	rx += add_x;
	ty += add_y;
	by += add_y;
}

void Rect::subtract(const Rect& rect, std::vector<Rect>& ret_array) const 
{
	Rect r = *this;
	r.intersect(rect);
	if (r.empty()) 
	{ 
		ret_array.push_back(*this);
		return;
	}
	if (r == *this) 
	{ // identical; no rect rests
		return;
	}
	// push top area
	if (ty != r.ty) 
	{
		ret_array.push_back(Rect(lx, ty, rx, r.ty));
	}
	// push bottom area
	if (by != r.by) 
	{
		ret_array.push_back(Rect(lx, r.by, rx, by));
	}
	// push left area
	if (lx != r.lx) 
	{
		ret_array.push_back(Rect(lx, r.ty, r.lx, r.by));
	}
	// push right area
	if (rx != r.rx) 
	{
		ret_array.push_back(Rect(r.rx, r.ty, rx, r.by));
	}
}
