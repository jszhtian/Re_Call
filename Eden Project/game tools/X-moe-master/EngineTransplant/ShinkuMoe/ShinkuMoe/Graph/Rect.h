#pragma once

#include <vector>

struct Rect
{
	int lx, rx; // x = [lx,rx)
	int ty, by; // y = [ty,by)

	Rect(int x1, int y1);
	Rect(int x1, int y1, int x2, int y2);
	Rect(const Rect& rect);
	Rect(const class Surface& rect);
	Rect(const class TextGlyph& glyph);
	void intersect(const Rect& rect);
	bool is_crossed(const Rect& rect) const;
	bool is_inner(const Rect& rect) const;
	bool is_nearly_inner(const Rect& rect, int delta) const;
	void join(const Rect& rect);
	void rmove(int add_x, int add_y);

	void subtract(const Rect& rect, std::vector<Rect>& ret_array) const;
	bool point_in(int x, int y) const;
	bool empty(void) const 
	{
		return (lx == rx) || (ty == by);
	}
	int width(void) const
	{
		return rx - lx;
	}
	int height(void) const
	{
		return by - ty;
	}
};

inline bool operator ==(const Rect& r1, const Rect& r2) 
{
	return (r1.lx == r2.lx && r1.rx == r2.rx && r1.ty == r2.ty && r1.by == r2.by);
}

inline bool Rect::point_in(int x, int y) const
{
	return (lx <= x && x < rx && ty <= y && y < by);
}
