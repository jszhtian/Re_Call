//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2007 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Base Layer Bitmap implementation
//---------------------------------------------------------------------------
#include "tjsCommHead.h"

#include "LayerBitmapIntf.h"
#include "MsgIntf.h"
#include "Resampler.h"
#include "tvpgl.h"
#include "argb.h"
#include "tjsUtils.h"


//#define TVP_FORCE_BILINEAR


//---------------------------------------------------------------------------
// To forcing bilinear interpolation, define TVP_FORCE_BILINEAR.

#ifdef TVP_FORCE_BILINEAR
	#define TVP_BILINEAR_FORCE_COND true
#else
	#define TVP_BILINEAR_FORCE_COND false
#endif
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// intact ( does not affect ) gamma adjustment data
tTVPGLGammaAdjustData TVPIntactGammaAdjustData =
{ 1.0, 0, 255, 1.0, 0, 255, 1.0, 0, 255 };
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
#define RET_VOID
#define BOUND_CHECK(x) \
{ \
	tjs_int i; \
	if(rect.left < 0) rect.left = 0; \
	if(rect.top < 0) rect.top = 0; \
	if(rect.right > (i=GetWidth())) rect.right = i; \
	if(rect.bottom > (i=GetHeight())) rect.bottom = i; \
	if(rect.right - rect.left <= 0 || rect.bottom - rect.top <= 0) \
		return x; \
}

//---------------------------------------------------------------------------
// tTVPBaseBitmap
//---------------------------------------------------------------------------
tTVPBaseBitmap::tTVPBaseBitmap(tjs_uint w, tjs_uint h, tjs_uint bpp) :
		tTVPNativeBaseBitmap(w, h, bpp)
{
}
//---------------------------------------------------------------------------
tTVPBaseBitmap::~tTVPBaseBitmap()
{
}
//---------------------------------------------------------------------------
void tTVPBaseBitmap::SetSizeWithFill(tjs_uint w, tjs_uint h, tjs_uint32 fillvalue)
{
	// resize, and fill the expanded region with specified value.

	tjs_uint orgw = GetWidth();
	tjs_uint orgh = GetHeight();

	SetSize(w, h);

	if(w > orgw && h > orgh)
	{
		// both width and height were expanded
		tTVPRect rect;
		rect.left = orgw;
		rect.top = 0;
		rect.right = w;
		rect.bottom = h;
		Fill(rect, fillvalue);

		rect.left = 0;
		rect.top = orgh;
		rect.right = orgw;
		rect.bottom = h;
		Fill(rect, fillvalue);
	}
	else if(w > orgw)
	{
		// width was expanded
		tTVPRect rect;
		rect.left = orgw;
		rect.top = 0;
		rect.right = w;
		rect.bottom = h;
		Fill(rect, fillvalue);
	}
	else if(h > orgh)
	{
		// height was expanded
		tTVPRect rect;
		rect.left = 0;
		rect.top = orgh;
		rect.right = w;
		rect.bottom = h;
		Fill(rect, fillvalue);
	}
}
//---------------------------------------------------------------------------
tjs_uint32 tTVPBaseBitmap::GetPoint(tjs_int x, tjs_int y) const
{
	// get specified point's color or color index
	if(x < 0 || y < 0 || x >= (tjs_int)GetWidth() || y >= (tjs_int)GetHeight())
		TVPThrowExceptionMessage(TVPOutOfRectangle);

	if(Is32BPP())
		return  *( (const tjs_uint32*)GetScanLine(y) + x); // 32bpp
	else
		return  *( (const tjs_uint8*)GetScanLine(y) + x); // 8bpp
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::SetPoint(tjs_int x, tjs_int y, tjs_uint32 value)
{
	// set specified point's color(and opacity) or color index
	if(x < 0 || y < 0 || x >= (tjs_int)GetWidth() || y >= (tjs_int)GetHeight())
		TVPThrowExceptionMessage(TVPOutOfRectangle);

	if(Is32BPP())
		*( (tjs_uint32*)GetScanLineForWrite(y) + x) = value; // 32bpp
	else
		*( (tjs_uint8*)GetScanLine(y) + x) = value; // 8bpp

	return true;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::SetPointMain(tjs_int x, tjs_int y, tjs_uint32 color)
{
	// set specified point's color (mask is not touched)
	// for 32bpp
	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	if(x < 0 || y < 0 || x >= (tjs_int)GetWidth() || y >= (tjs_int)GetHeight())
		TVPThrowExceptionMessage(TVPOutOfRectangle);

	tjs_uint32 *addr = (tjs_uint32*)GetScanLineForWrite(y) + x;
	*addr &= 0xff000000;
	*addr += color & 0xffffff;

	return true;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::SetPointMask(tjs_int x, tjs_int y, tjs_int mask)
{
	// set specified point's mask (color is not touched)
	// for 32bpp
	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	if(x < 0 || y < 0 || x >= (tjs_int)GetWidth() || y >= (tjs_int)GetHeight())
		TVPThrowExceptionMessage(TVPOutOfRectangle);

	tjs_uint32 *addr = (tjs_uint32*)GetScanLineForWrite(y) + x;
	*addr &= 0x00ffffff;
	*addr += (mask & 0xff) << 24;

	return true;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::Fill(tTVPRect rect, tjs_uint32 value)
{
	// fill target rectangle represented as "rect", with color ( and opacity )
	// passed by "value".
	// value must be : 0xAARRGGBB (for 32bpp) or 0xII ( for 8bpp )
	BOUND_CHECK(false);

	if(!IsIndependent())
	{
		if(rect.left == 0 && rect.top == 0 &&
			rect.right == (tjs_int)GetWidth() && rect.bottom == (tjs_int)GetHeight())
		{
			// cover overall
			IndependNoCopy(); // indepent with no-copy
		}
	}

	if(Is32BPP())
	{
		// 32bpp
		tjs_int pitch = GetPitchBytes();
		tjs_uint8 *sc = (tjs_uint8*)GetScanLineForWrite(rect.top);
		tjs_int height = rect.bottom - rect.top;
		tjs_int width = rect.right - rect.left;

		if(height * width >= 64*1024/4)
		{
			for(;rect.top < rect.bottom; rect.top++)
			{
				tjs_uint32 * p = (tjs_uint32*)sc + rect.left;
				TVPFillARGB_NC(p, width, value);
				sc += pitch;
			}
		}
		else
		{
			for(;rect.top < rect.bottom; rect.top++)
			{
				tjs_uint32 * p = (tjs_uint32*)sc + rect.left;
				TVPFillARGB(p, width, value);
				sc += pitch;
			}
		}
	}
	else
	{
		// 8bpp
		tjs_int pitch = GetPitchBytes();
		tjs_uint8 *sc = (tjs_uint8*)GetScanLineForWrite(rect.top);

		for(;rect.top < rect.bottom; rect.top++)
		{
			tjs_uint8 * p = (tjs_uint8*)sc + rect.left;
			memset(p, value, rect.right - rect.left);
			sc += pitch;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::FillColor(tTVPRect rect, tjs_uint32 color, tjs_int opa)
{
	// fill rectangle with specified color.
	// this ignores destination alpha (destination alpha will not change)
	// opa is fill opacity if opa is positive value.
	// negative value of opa is not allowed.
	BOUND_CHECK(false);

	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	if(opa == 0) return false; // no action

	if(opa < 0) opa = 0;
	if(opa > 255) opa = 255;

	tjs_int pitch = GetPitchBytes();
	tjs_uint8 *sc = (tjs_uint8*)GetScanLineForWrite(rect.top);
	tjs_int width = rect.right - rect.left;

	if(opa == 255)
	{
		// complete opaque fill
		for(;rect.top < rect.bottom; rect.top++)
		{
			tjs_uint32 * p = (tjs_uint32*)sc + rect.left;
			TVPFillColor(p, width, color);
			sc += pitch;
		}
	}
	else
	{
		// alpha fill
		for(;rect.top < rect.bottom; rect.top++)
		{
			tjs_uint32 * p = (tjs_uint32*)sc + rect.left;
			TVPConstColorAlphaBlend(p, width, color, opa);
			sc += pitch;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::BlendColor(tTVPRect rect, tjs_uint32 color, tjs_int opa,
	bool additive)
{
	// fill rectangle with specified color.
	// this considers destination alpha (additive or simple)

	BOUND_CHECK(false);

	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	if(opa == 0) return false; // no action
	if(opa < 0) opa = 0;
	if(opa > 255) opa = 255;

	if(opa == 255)
	{
		// complete opaque fill
		color |= 0xff000000;

		if(!IsIndependent())
		{
			if(rect.left == 0 && rect.top == 0 &&
				rect.right == (tjs_int)GetWidth() && rect.bottom == (tjs_int)GetHeight())
			{
				// cover overall
				IndependNoCopy(); // indepent with no-copy
			}
		}

		tjs_int pitch = GetPitchBytes();
		tjs_uint8 *sc = (tjs_uint8*)GetScanLineForWrite(rect.top);
		tjs_int width = rect.right - rect.left;

		for(;rect.top < rect.bottom; rect.top++)
		{
			tjs_uint32 * p = (tjs_uint32*)sc + rect.left;
			TVPFillARGB(p, width, color);
			sc += pitch;
		}
	}
	else
	{
		// alpha fill
		tjs_int pitch = GetPitchBytes();
		tjs_uint8 *sc = (tjs_uint8*)GetScanLineForWrite(rect.top);
		tjs_int width = rect.right - rect.left;

		if(!additive)
		{
			for(;rect.top < rect.bottom; rect.top++)
			{
				tjs_uint32 * p = (tjs_uint32*)sc + rect.left;
				TVPConstColorAlphaBlend_d(p, width, color, opa);
				sc += pitch;
			}
		}
		else
		{
			for(;rect.top < rect.bottom; rect.top++)
			{
				tjs_uint32 * p = (tjs_uint32*)sc + rect.left;
				TVPConstColorAlphaBlend_a(p, width, color, opa);
				sc += pitch;
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::RemoveConstOpacity(tTVPRect rect, tjs_int level)
{
	// remove constant opacity from bitmap. ( similar to PhotoShop's eraser tool )
	// level is a strength of removing ( 0 thru 255 )
	// this cannot work with additive alpha mode.

	BOUND_CHECK(false);

	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	if(level == 0) return false; // no action
	if(level < 0) level = 0;
	if(level > 255) level = 255;

	tjs_int pitch = GetPitchBytes();
	tjs_uint8 *sc = (tjs_uint8*)GetScanLineForWrite(rect.top);
	tjs_int width = rect.right - rect.left;

	if(level == 255)
	{
		// completely remove opacity
		for(;rect.top < rect.bottom; rect.top++)
		{
			tjs_uint32 * p = (tjs_uint32*)sc + rect.left;
			TVPFillMask(p, width, 0);
			sc += pitch;
		}
	}
	else
	{
		for(;rect.top < rect.bottom; rect.top++)
		{
			tjs_uint32 * p = (tjs_uint32*)sc + rect.left;
			TVPRemoveConstOpacity(p, width, level);
			sc += pitch;
		}

	}

	return true;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::FillMask(tTVPRect rect, tjs_int value)
{
	// fill mask with specified value.
	BOUND_CHECK(false);

	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	tjs_int pitch = GetPitchBytes();
	tjs_uint8 *sc = (tjs_uint8*)GetScanLineForWrite(rect.top);
	tjs_int width = rect.right - rect.left;


	for(;rect.top < rect.bottom; rect.top++)
	{
		tjs_uint32 * p = (tjs_uint32*)sc + rect.left;
		TVPFillMask(p, width, value);
		sc += pitch;
	}

	return true;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::CopyRect(tjs_int x, tjs_int y, const tTVPBaseBitmap *ref,
		tTVPRect refrect, tjs_int plane)
{
	// copy bitmap rectangle.
	// TVP_BB_COPY_MAIN in "plane" : main image is copied
	// TVP_BB_COPY_MASK in "plane" : mask image is copied
	// "plane" is ignored if the bitmap is 8bpp
	// the source rectangle is ( "refrect" ) and the destination upper-left corner
	// is (x, y).
	if(!Is32BPP()) plane = (TVP_BB_COPY_MASK|TVP_BB_COPY_MAIN);
	if(x == 0 && y == 0 && refrect.left == 0 && refrect.top == 0 &&
		refrect.right == (tjs_int)ref->GetWidth() &&
		refrect.bottom == (tjs_int)ref->GetHeight() &&
		(tjs_int)GetWidth() == refrect.right &&
		(tjs_int)GetHeight() == refrect.bottom &&
		plane == (TVP_BB_COPY_MASK|TVP_BB_COPY_MAIN) &&
		(bool)!Is32BPP() == (bool)!ref->Is32BPP())
	{
		// entire area of both bitmaps
		AssignBitmap(*ref);
		return true;
	}

	// bound check
	tjs_int bmpw, bmph;

	bmpw = ref->GetWidth();
	bmph = ref->GetHeight();

	if(refrect.left < 0)
		x -= refrect.left, refrect.left = 0;
	if(refrect.right > bmpw)
		refrect.right = bmpw;

	if(refrect.left >= refrect.right) return false;

	if(refrect.top < 0)
		y -= refrect.top, refrect.top = 0;
	if(refrect.bottom > bmph)
		refrect.bottom = bmph;

	if(refrect.top >= refrect.bottom) return false;

	bmpw = GetWidth();
	bmph = GetHeight();

	tTVPRect rect;
	rect.left = x;
	rect.top = y;
	rect.right = rect.left + refrect.get_width();
	rect.bottom = rect.top + refrect.get_height();

	if(rect.left < 0)
	{
		refrect.left += -rect.left;
		rect.left = 0;
	}

	if(rect.right > bmpw)
	{
		refrect.right -= (rect.right - bmpw);
		rect.right = bmpw;
	}

	if(refrect.left >= refrect.right) return false; // not drawable

	if(rect.top < 0)
	{
		refrect.top += -rect.top;
		rect.top = 0;
	}

	if(rect.bottom > bmph)
	{
		refrect.bottom -= (rect.bottom - bmph);
		rect.bottom = bmph;
	}

	if(refrect.top >= refrect.bottom) return false; // not drawable


	// transfer
	tjs_int pixelsize = (Is32BPP()?sizeof(tjs_uint32):sizeof(tjs_uint8));
	tjs_int dpitch = GetPitchBytes();
	tjs_int spitch = ref->GetPitchBytes();
	tjs_int w;
	tjs_int wbytes = (w = refrect.get_width()) * pixelsize;
	tjs_int h = refrect.get_height();

	if(ref == this && rect.top > refrect.top)
	{
		// backward copy
		tjs_uint8 * dest = (tjs_uint8*)GetScanLineForWrite(rect.bottom-1) +
			rect.left*pixelsize;
		const tjs_uint8 * src = (const tjs_uint8*)ref->GetScanLine(refrect.bottom-1) +
			refrect.left*pixelsize;

		switch(plane)
		{
		case TVP_BB_COPY_MAIN:
			while(h--)
			{
				TVPCopyColor((tjs_uint32*)dest, (const tjs_uint32*)src, w);
				dest -= dpitch;
				src -= spitch;
			}
			break;
		case TVP_BB_COPY_MASK:
			while(h--)
			{
				TVPCopyMask((tjs_uint32*)dest, (const tjs_uint32*)src, w);
				dest -= dpitch;
				src -= spitch;
			}
			break;
		case TVP_BB_COPY_MAIN|TVP_BB_COPY_MASK:
			while(h--)
			{
				memmove(dest, src, wbytes);
				dest -= dpitch;
				src -= spitch;
			}
			break;
		}
	}
	else
	{
		// forward copy
		tjs_uint8 * dest = (tjs_uint8*)GetScanLineForWrite(rect.top) +
			rect.left*pixelsize;
		const tjs_uint8 * src = (const tjs_uint8*)ref->GetScanLine(refrect.top) +
			refrect.left*pixelsize;

		switch(plane)
		{
		case TVP_BB_COPY_MAIN:
			while(h--)
			{
				TVPCopyColor((tjs_uint32*)dest, (const tjs_uint32*)src, w);
				dest += dpitch;
				src += spitch;
			}
			break;
		case TVP_BB_COPY_MASK:
			while(h--)
			{
				TVPCopyMask((tjs_uint32*)dest, (const tjs_uint32*)src, w);
				dest += dpitch;
				src += spitch;
			}
			break;
		case TVP_BB_COPY_MAIN|TVP_BB_COPY_MASK:
			while(h--)
			{
				memmove(dest, src, wbytes);
				dest += dpitch;
				src += spitch;
			}
			break;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::Blt(tjs_int x, tjs_int y, const tTVPBaseBitmap *ref,
		tTVPRect refrect, tTVPBBBltMethod method, tjs_int opa, bool hda)
{
	// blt src bitmap with various methods.

	// hda option ( hold destination alpha ) holds distination alpha,
	// but will select more complex function ( and takes more time ) for it ( if
	// can do )

	// this function does not matter whether source and destination bitmap is
	// overlapped.

	if(opa == 255 && method == bmCopy && !hda)
	{
		return CopyRect(x, y, ref, refrect);
	}

	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	if(opa == 0) return false; // opacity==0 has no action

	// bound check
	tjs_int bmpw, bmph;

	bmpw = ref->GetWidth();
	bmph = ref->GetHeight();

	if(refrect.left < 0)
		x -= refrect.left, refrect.left = 0;
	if(refrect.right > bmpw)
		refrect.right = bmpw;

	if(refrect.left >= refrect.right) return false;

	if(refrect.top < 0)
		y -= refrect.top, refrect.top = 0;
	if(refrect.bottom > bmph)
		refrect.bottom = bmph;

	if(refrect.top >= refrect.bottom) return false;

	bmpw = GetWidth();
	bmph = GetHeight();


	tTVPRect rect;
	rect.left = x;
	rect.top = y;
	rect.right = rect.left + refrect.get_width();
	rect.bottom = rect.top + refrect.get_height();

	if(rect.left < 0)
	{
		refrect.left += -rect.left;
		rect.left = 0;
	}

	if(rect.right > bmpw)
	{
		refrect.right -= (rect.right - bmpw);
		rect.right = bmpw;
	}

	if(refrect.left >= refrect.right) return false; // not drawable

	if(rect.top < 0)
	{
		refrect.top += -rect.top;
		rect.top = 0;
	}

	if(rect.bottom > bmph)
	{
		refrect.bottom -= (rect.bottom - bmph);
		rect.bottom = bmph;
	}

	if(refrect.top >= refrect.bottom) return false; // not drawable


	// blt
 	tjs_int dpitch = GetPitchBytes();
	tjs_int spitch = ref->GetPitchBytes();
	tjs_int w = refrect.get_width();
	tjs_int h = refrect.get_height();

	// Blt performs always forward transfer;
	// this does not consider the duplicated area of the same bitmap.

	tjs_uint8 * dest = (tjs_uint8*)GetScanLineForWrite(rect.top) +
		rect.left*sizeof(tjs_uint32);
	const tjs_uint8 * src = (const tjs_uint8*)ref->GetScanLine(refrect.top) +
		refrect.left*sizeof(tjs_uint32);

#define TVP_BLEND_4(basename) /* blend for 4 types (normal, opacity, HDA, HDA opacity) */ \
	if(opa == 255)                                                            \
	{                                                                         \
		if(!hda)                                                              \
		{                                                                     \
			while(h--)                                                        \
				basename((tjs_uint32*)dest, (tjs_uint32*)src, w),             \
				dest+=dpitch, src+=spitch;                                    \
                                                                              \
		}                                                                     \
		else                                                                  \
		{                                                                     \
			while(h--)                                                        \
				basename##_HDA((tjs_uint32*)dest, (tjs_uint32*)src, w),       \
				dest+=dpitch, src+=spitch;                                    \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		if(!hda)                                                              \
		{                                                                     \
			while(h--)                                                        \
				basename##_o((tjs_uint32*)dest, (tjs_uint32*)src, w, opa),    \
				dest+=dpitch, src+=spitch;                                    \
		}                                                                     \
		else                                                                  \
		{                                                                     \
			while(h--)                                                        \
				basename##_HDA_o((tjs_uint32*)dest, (tjs_uint32*)src, w, opa),\
				dest+=dpitch, src+=spitch;                                    \
		}                                                                     \
	}


	switch(method)
	{
	case bmCopy:
		// constant ratio alpha blendng
		if(opa == 255 && hda)
		{
			while(h--)
				TVPCopyColor((tjs_uint32*)dest, (tjs_uint32*)src, w),
					dest+=dpitch, src+=spitch;
		}
		else if(!hda)
		{
			while(h--)
				TVPConstAlphaBlend((tjs_uint32*)dest, (tjs_uint32*)src, w, opa),
					dest+=dpitch, src+=spitch;
		}
		else
		{
			while(h--)
				TVPConstAlphaBlend_HDA((tjs_uint32*)dest, (tjs_uint32*)src, w, opa),
					dest+=dpitch, src+=spitch;
		}
		break;

	case bmCopyOnAlpha:
		// constant ratio alpha blending (assuming source is opaque)
		// with consideration of destination alpha
		if(opa == 255)
			while(h--)
				TVPCopyOpaqueImage((tjs_uint32*)dest, (tjs_uint32*)src, w),
				dest+=dpitch, src+=spitch;
		else
			while(h--)
				TVPConstAlphaBlend_d((tjs_uint32*)dest, (tjs_uint32*)src, w, opa),
				dest+=dpitch, src+=spitch;
		break;


	case bmAlpha:
		// alpha blending, ignoring destination alpha
		TVP_BLEND_4(TVPAlphaBlend);
		break;

	case bmAlphaOnAlpha:
		// alpha blending, with consideration of destination alpha
		if(opa == 255)
			while(h--)
				TVPAlphaBlend_d((tjs_uint32*)dest, (tjs_uint32*)src, w),
				dest+=dpitch, src+=spitch;
		else
			while(h--)
				TVPAlphaBlend_do((tjs_uint32*)dest, (tjs_uint32*)src, w, opa),
				dest+=dpitch, src+=spitch;
		break;

	case bmAdd:
		// additive blending ( this does not consider distination alpha )
		TVP_BLEND_4(TVPAddBlend);
		break;

	case bmSub:
		// subtractive blending ( this does not consider distination alpha )
		TVP_BLEND_4(TVPSubBlend);
		break;

	case bmMul:
		// multiplicative blending ( this does not consider distination alpha )
		TVP_BLEND_4(TVPMulBlend);
		break;


	case bmDodge:
		// color dodge mode ( this does not consider distination alpha )
		TVP_BLEND_4(TVPColorDodgeBlend);
		break;


	case bmDarken:
		// darken mode ( this does not consider distination alpha )
		TVP_BLEND_4(TVPDarkenBlend);
		break;


	case bmLighten:
		// lighten mode ( this does not consider distination alpha )
		TVP_BLEND_4(TVPLightenBlend);
		break;


	case bmScreen:
		// screen multiplicative mode ( this does not consider distination alpha )
		TVP_BLEND_4(TVPScreenBlend);
		break;


	case bmAddAlpha:
		// Additive Alpha
		TVP_BLEND_4(TVPAdditiveAlphaBlend);
		break;


	case bmAddAlphaOnAddAlpha:
		// Additive Alpha on Additive Alpha
		if(opa == 255)
		{
			while(h--)
				TVPAdditiveAlphaBlend_a((tjs_uint32*)dest, (tjs_uint32*)src, w),
				dest+=dpitch, src+=spitch;
		}
		else
		{
			while(h--)
				TVPAdditiveAlphaBlend_ao((tjs_uint32*)dest, (tjs_uint32*)src, w, opa),
				dest+=dpitch, src+=spitch;
		}
		break;


	case bmAddAlphaOnAlpha:
		// additive alpha on simple alpha
		// Not yet implemented
		break;

	case bmAlphaOnAddAlpha:
		// simple alpha on additive alpha
		if(opa == 255)
		{
			while(h--)
				TVPAlphaBlend_a((tjs_uint32*)dest, (tjs_uint32*)src, w),
				dest+=dpitch, src+=spitch;
		}
		else
		{
			while(h--)
				TVPAlphaBlend_ao((tjs_uint32*)dest, (tjs_uint32*)src, w, opa),
				dest+=dpitch, src+=spitch;
		}
		break;

	case bmCopyOnAddAlpha:
		// constant ratio alpha blending (assuming source is opaque)
		// with consideration of destination additive alpha
		if(opa == 255)
			while(h--)
				TVPCopyOpaqueImage((tjs_uint32*)dest, (tjs_uint32*)src, w),
				dest+=dpitch, src+=spitch;
		else
			while(h--)
				TVPConstAlphaBlend_a((tjs_uint32*)dest, (tjs_uint32*)src, w, opa),
				dest+=dpitch, src+=spitch;
		break;


	case bmPsNormal:
		// Photoshop compatible normal blend
		// (may take the same effect as bmAlpha)
		TVP_BLEND_4(TVPPsAlphaBlend);
		break;

	case bmPsAdditive:
		// Photoshop compatible additive blend
		TVP_BLEND_4(TVPPsAddBlend);
		break;

	case bmPsSubtractive:
		// Photoshop compatible subtractive blend
		TVP_BLEND_4(TVPPsSubBlend);
		break;

	case bmPsMultiplicative:
		// Photoshop compatible multiplicative blend
		TVP_BLEND_4(TVPPsMulBlend);
		break;

	case bmPsScreen:
		// Photoshop compatible screen blend
		TVP_BLEND_4(TVPPsScreenBlend);
		break;

	case bmPsOverlay:
		// Photoshop compatible overlay blend
		TVP_BLEND_4(TVPPsOverlayBlend);
		break;

	case bmPsHardLight:
		// Photoshop compatible hard light blend
		TVP_BLEND_4(TVPPsHardLightBlend);
		break;

	case bmPsSoftLight:
		// Photoshop compatible soft light blend
		TVP_BLEND_4(TVPPsSoftLightBlend);
		break;

	case bmPsColorDodge:
		// Photoshop compatible color dodge blend
		TVP_BLEND_4(TVPPsColorDodgeBlend);
		break;

	case bmPsColorDodge5:
		// Photoshop 5.x compatible color dodge blend
		TVP_BLEND_4(TVPPsColorDodge5Blend);
		break;

	case bmPsColorBurn:
		// Photoshop compatible color burn blend
		TVP_BLEND_4(TVPPsColorBurnBlend);
		break;

	case bmPsLighten:
		// Photoshop compatible compare (lighten) blend
		TVP_BLEND_4(TVPPsLightenBlend);
		break;

	case bmPsDarken:
		// Photoshop compatible compare (darken) blend
		TVP_BLEND_4(TVPPsDarkenBlend);
		break;

	case bmPsDifference:
		// Photoshop compatible difference blend
		TVP_BLEND_4(TVPPsDiffBlend);
		break;

	case bmPsDifference5:
		// Photoshop 5.x compatible difference blend
		TVP_BLEND_4(TVPPsDiff5Blend);
		break;

	case bmPsExclusion:
		// Photoshop compatible exclusion blend
		TVP_BLEND_4(TVPPsExclusionBlend);
		break;


	default:
				 ;
	}

	return true;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// template function for strech loop
//---------------------------------------------------------------------------

// define function pointer types for stretching line
typedef TVP_GL_FUNC_PTR_DECL(void, tTVPStretchFunction,
	(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int srcstart, tjs_int srcstep));
typedef TVP_GL_FUNC_PTR_DECL(void, tTVPStretchWithOpacityFunction,
	(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int srcstart, tjs_int srcstep, tjs_int opa));
typedef TVP_GL_FUNC_PTR_DECL(void, tTVPBilinearStretchFunction,
	(tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2,
	tjs_int blend_y, tjs_int srcstart, tjs_int srcstep));
typedef TVP_GL_FUNC_PTR_DECL(void, tTVPBilinearStretchWithOpacityFunction,
	(tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2,
	tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa));


//---------------------------------------------------------------------------

// declare stretching function object class
class tTVPStretchFunctionObject
{
	tTVPStretchFunction Func;
public:
	tTVPStretchFunctionObject(tTVPStretchFunction func) : Func(func) {;}
	void operator () (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int srcstart, tjs_int srcstep)
	{
		Func(dest, len, src, srcstart, srcstep);
	}
};

class tTVPStretchWithOpacityFunctionObject
{
	tTVPStretchWithOpacityFunction Func;
	tjs_int Opacity;
public:
	tTVPStretchWithOpacityFunctionObject(tTVPStretchWithOpacityFunction func, tjs_int opa) :
		Func(func), Opacity(opa) {;}
	void operator () (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int srcstart, tjs_int srcstep)
	{
		Func(dest, len, src, srcstart, srcstep, Opacity);
	}
};

class tTVPBilinearStretchFunctionObject
{
protected:
	tTVPBilinearStretchFunction Func;
public:
	tTVPBilinearStretchFunctionObject(tTVPBilinearStretchFunction func) : Func(func) {;}
	void operator () (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2,
	tjs_int blend_y, tjs_int srcstart, tjs_int srcstep)
	{
		Func(dest, destlen, src1, src2, blend_y, srcstart, srcstep);
	}
};

#define TVP_DEFINE_BILINEAR_STRETCH_FUNCTION(func, one) class \
t##func##FunctionObject : \
	public tTVPBilinearStretchFunctionObject \
{ \
public: \
	t##func##FunctionObject() : \
		tTVPBilinearStretchFunctionObject(func) {;} \
	void DoOnePixel(tjs_uint32 *dest, tjs_uint32 color) \
	{ one; } \
};


class tTVPBilinearStretchWithOpacityFunctionObject
{
protected:
	tTVPBilinearStretchWithOpacityFunction Func;
	tjs_int Opacity;
public:
	tTVPBilinearStretchWithOpacityFunctionObject(tTVPBilinearStretchWithOpacityFunction func, tjs_int opa) :
		Func(func), Opacity(opa) {;}
	void operator () (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2,
	tjs_int blend_y, tjs_int srcstart, tjs_int srcstep)
	{
		Func(dest, destlen, src1, src2, blend_y, srcstart, srcstep, Opacity);
	}
};

#define TVP_DEFINE_BILINEAR_STRETCH_WITH_OPACITY_FUNCTION(func, one) class \
t##func##FunctionObject : \
	public tTVPBilinearStretchWithOpacityFunctionObject \
{ \
public: \
	t##func##FunctionObject(tjs_int opa) : \
		tTVPBilinearStretchWithOpacityFunctionObject(func, opa) {;} \
	void DoOnePixel(tjs_uint32 *dest, tjs_uint32 color) \
	{ one; } \
};

//---------------------------------------------------------------------------

// declare streting function object for bilinear interpolation
TVP_DEFINE_BILINEAR_STRETCH_FUNCTION(
	TVPInterpStretchCopy,
	*dest = color);

TVP_DEFINE_BILINEAR_STRETCH_WITH_OPACITY_FUNCTION(
	TVPInterpStretchConstAlphaBlend,
	*dest = TVPBlendARGB(*dest, color, Opacity));

TVP_DEFINE_BILINEAR_STRETCH_FUNCTION(
	TVPInterpStretchAdditiveAlphaBlend,
	*dest = TVPAddAlphaBlend_n_a(*dest, color));

TVP_DEFINE_BILINEAR_STRETCH_WITH_OPACITY_FUNCTION(
	TVPInterpStretchAdditiveAlphaBlend_o,
	*dest = TVPAddAlphaBlend_n_a_o(*dest, color, Opacity));

//---------------------------------------------------------------------------

// declare stretching loop function
#define TVP_DoStretchLoop_ARGS  x_ref_start, y_ref_start, x_len, y_len, \
						destp, destpitch, x_step, \
						y_step, refp, refpitch
template <typename tFunc>
void tTVPBaseBitmap::TVPDoStretchLoop(
		tFunc func,
		tjs_int x_ref_start,
		tjs_int y_ref_start,
		tjs_int x_len, tjs_int y_len,
		tjs_uint8 * destp, tjs_int destpitch,
		tjs_int x_step, tjs_int y_step,
		const tjs_uint8 * refp, tjs_int refpitch)
{
	tjs_int y_ref = y_ref_start;
	while(y_len--)
	{
		func(
			(tjs_uint32*)destp,
			x_len,
			(const tjs_uint32*)(refp + (y_ref>>16)*refpitch),
			x_ref_start,
			x_step);
		y_ref += y_step;
		destp += destpitch;
	}
}
//---------------------------------------------------------------------------


// declare stretching loop function for bilinear interpolation

#define TVP_DoBilinearStretchLoop_ARGS  rw, rh, dw, dh, \
						srccliprect, x_ref_start, y_ref_start, x_len, y_len, \
						destp, destpitch, x_step, \
						y_step, refp, refpitch
template <typename tStretchFunc>
void tTVPBaseBitmap::TVPDoBiLinearStretchLoop(
		tStretchFunc stretch,
		tjs_int rw, tjs_int rh,
		tjs_int dw, tjs_int dh,
		const tTVPRect & srccliprect,
		tjs_int x_ref_start,
		tjs_int y_ref_start,
		tjs_int x_len, tjs_int y_len,
		tjs_uint8 * destp, tjs_int destpitch,
		tjs_int x_step, tjs_int y_step,
		const tjs_uint8 * refp, tjs_int refpitch)
{
/*
	memo
          0         1         2         3         4
          .         .         .         .                  center = 1.5 = (4-1) / 2
          ------------------------------                 reference area
     ----------++++++++++----------++++++++++
                         ^                                 4 / 1  step 4   ofs =  1.5   = ((4-1) - (1-1)*4) / 2
               ^                   ^                       4 / 2  step 2   ofs =  0.5   = ((4-1) - (2-1)*2) / 2
          ^         ^         ^         *                  4 / 4  step 1   ofs =  0     = ((4-1) - (4-1)*1) / 2
         *       ^       ^       ^       *                 4 / 5  steo 0.8 ofs = -0.1   = ((4-1) - (5-1)*0.8) / 2
        *    ^    ^    ^    ^    ^    ^    *               4 / 8  step 0.5 ofs = -0.25

*/



	// adjust start point
	if(x_step >= 0)
		x_ref_start += (((rw-1)<<16) - (dw-1)*x_step)/2;
	else
		x_ref_start -= (((rw-1)<<16) + (dw-1)*x_step)/2 - x_step;
	if(y_step >= 0)
		y_ref_start += (((rh-1)<<16) - (dh-1)*y_step)/2;
	else
		y_ref_start -= (((rh-1)<<16) + (dh-1)*y_step)/2 - y_step;

	// horizontal destination line is splitted into three parts;
	// 1. left fraction (x_ref < srccliprect.left               (lf)
	//                or x_ref >= srccliprect.right - 1)
	// 2. center                                 (c)
	// 3. right fraction (x_ref >= srccliprect.right - 1        (rf)
	//                or x_ref < srccliprect.left)

	tjs_int ref_left_limit  = (srccliprect.left)<<16;
	tjs_int ref_right_limit = (srccliprect.right-1)<<16;

	tjs_int y_ref = y_ref_start;
	while(y_len--)
	{
		tjs_int y1 = y_ref >> 16;
		tjs_int y2 = y1+1;
		tjs_int y_blend = (y_ref & 0xffff) >> 8;
		if(y1 < srccliprect.top)
			y1 = srccliprect.top;
		else if(y1 >= srccliprect.bottom)
			y1 = srccliprect.bottom - 1;
		if(y2 < srccliprect.top)
			y2 = srccliprect.top;
		else if(y2 >= srccliprect.bottom)
			y2 = srccliprect.bottom - 1;

		const tjs_uint32 * l1 =
			(const tjs_uint32*)(refp + refpitch * y1);
		const tjs_uint32 * l2 =
			(const tjs_uint32*)(refp + refpitch * y2);


		// perform left and right fractions
		tjs_int x_remain = x_len;
		tjs_uint32 * dp;
		tjs_int x_ref;

		// from last point
		if(x_remain)
		{
			dp = (tjs_uint32*)destp + (x_len - 1);
			x_ref = x_ref_start + (x_len - 1) * x_step;
			if(x_ref < ref_left_limit)
			{
				tjs_uint color =
					TVPBlendARGB(
						*(l1 + srccliprect.left),
						*(l2 + srccliprect.left), y_blend);
				do
					stretch.DoOnePixel(dp, color), dp-- , x_ref -= x_step, x_remain --;
				while(x_ref < ref_left_limit && x_remain);
			}
			else if(x_ref >= ref_right_limit)
			{
				tjs_uint color =
					TVPBlendARGB(
						*(l1 + srccliprect.right - 1),
						*(l2 + srccliprect.right - 1), y_blend);
				do
					stretch.DoOnePixel(dp, color), dp-- , x_ref -= x_step, x_remain --;
				while(x_ref >= ref_right_limit && x_remain);
			}
		}

		// from first point
		if(x_remain)
		{
			dp = (tjs_uint32*)destp;
			x_ref = x_ref_start;
			if(x_ref < ref_left_limit)
			{
				tjs_uint color =
					TVPBlendARGB(
						*(l1 + srccliprect.left),
						*(l2 + srccliprect.left), y_blend);
				do
					stretch.DoOnePixel(dp, color), dp++ , x_ref += x_step, x_remain --;
				while(x_ref < ref_left_limit && x_remain);
			}
			else if(x_ref >= ref_right_limit)
			{
				tjs_uint color =
					TVPBlendARGB(
						*(l1 + srccliprect.right - 1),
						*(l2 + srccliprect.right - 1), y_blend);
				do
					stretch.DoOnePixel(dp, color), dp++ , x_ref += x_step, x_remain --;
				while(x_ref >= ref_right_limit && x_remain);
			}
		}

		// perform center part
		// (this may take most time of this function)
		if(x_remain)
		{
			stretch(
				dp,
				x_remain,
				l1, l2, y_blend,
				x_ref,
				x_step);
		}

		// step to the next line
		y_ref += y_step;
		destp += destpitch;
	}
}

//---------------------------------------------------------------------------
bool tTVPBaseBitmap::StretchBlt(tTVPRect cliprect,
		tTVPRect destrect, const tTVPBaseBitmap *ref,
		tTVPRect refrect, tTVPBBBltMethod method, tjs_int opa,
			bool hda, tTVPBBStretchType mode)
{
	// do stretch blt
	// stFastLinear is enabled only in following condition:
	// -------TODO: write corresponding condition--------

	// stLinear and stCubic mode are enabled only in following condition:
	// any magnification, opa:255, method:bmCopy, hda:false
	// no reverse, destination rectangle is within the image.

	// extract stretch type
	tTVPBBStretchType type = (tTVPBBStretchType)(mode & stTypeMask);

	// source and destination check
	tjs_int dw = destrect.get_width(), dh = destrect.get_height();
	tjs_int rw = refrect.get_width(), rh = refrect.get_height();

	if(!dw || !rw || !dh || !rh) return false; // nothing to do

	if(dw == rw && dh == rh)
	{
		return Blt(destrect.left, destrect.top, ref, refrect, method, opa, hda);
			// no stretch; do normal blt
	}

	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	// compute pitch, step, etc. needed for stretching copy/blt
	tjs_int w = GetWidth();
	tjs_int h = GetHeight();
	tjs_int refw = ref->GetWidth();
	tjs_int refh = ref->GetHeight();


	// check clipping rect
	if(cliprect.left < 0) cliprect.left = 0;
	if(cliprect.top < 0) cliprect.top = 0;
	if(cliprect.right > w) cliprect.right = w;
	if(cliprect.bottom > h) cliprect.bottom = h;

	// check mode
	if((type == stLinear || type == stCubic) && !hda && opa==255 && method==bmCopy
		&& dw > 0 && dh > 0 && rw > 0 && rh > 0 &&
		destrect.left >= cliprect.left && destrect.top >= cliprect.top &&
		destrect.right <= cliprect.right && destrect.bottom <= cliprect.bottom)
	{
		// takes another routine
		TVPResampleImage(this, destrect, ref, refrect, type==stLinear?1:2);
		return true;
	}


	// check transfer direction
	bool x_dir = true, y_dir = true; // direction;  true:forward, false:backward

	if(dw < 0)
		x_dir = !x_dir, std::swap(destrect.left, destrect.right), dw = -dw;
	if(dh < 0)
		y_dir = !y_dir, std::swap(destrect.top, destrect.bottom), dh = -dh;

	if(rw < 0)
		x_dir = !x_dir, std::swap(refrect.left, refrect.right), rw = -rw;
	if(rh < 0)
		y_dir = !y_dir, std::swap(refrect.top, refrect.bottom), rh = -rh;

	// ref bound check
	if(refrect.left >= refrect.right ||
		refrect.top >= refrect.bottom) return false;
	if(refrect.left < 0 || refrect.right > refw ||
		refrect.top < 0 || refrect.bottom > refh)
		TVPThrowExceptionMessage(TVPSrcRectOutOfBitmap);

	// make srccliprect
	tTVPRect srccliprect;
	if(mode & stRefNoClip)
		srccliprect.left = 0,
		srccliprect.top = 0,
		srccliprect.right = refw,
		srccliprect.bottom = refh; // no clip; all the bitmap will be the source
	else
		srccliprect = refrect; // clip; the source is limited to the source rectangle

	// compute step
	tjs_int x_step = (rw << 16) / dw;
	tjs_int y_step = (rh << 16) / dh;

//	bool x_2x = x_step == 32768;
//	bool y_2x = y_step == 32768;

	tjs_int x_ref_start, y_ref_start;
	tjs_int x_dest_start, y_dest_start;
	tjs_int x_len, y_len;

	if(x_dir)
	{
		// x; forward
		if(destrect.left < cliprect.left)
			x_dest_start = cliprect.left,
			x_ref_start = (refrect.left << 16) + x_step * (cliprect.left - destrect.left);
		else
			x_dest_start = destrect.left,
			x_ref_start = (refrect.left << 16);

		if(destrect.right > cliprect.right)
			x_len = cliprect.right - x_dest_start;
		else
			x_len = destrect.right - x_dest_start;
	}
	else
	{
		// x; backward
		x_step = -x_step;

		if(destrect.left < cliprect.left)
			x_ref_start = ((refrect.right << 16) - 1) + x_step * (cliprect.left - destrect.left),
			x_dest_start = cliprect.left;
		else
			x_ref_start = ((refrect.right << 16) - 1),
			x_dest_start = destrect.left;

		if(destrect.right > cliprect.right)
			x_len = cliprect.right - x_dest_start;
		else
			x_len = destrect.right - x_dest_start;
	}
	if(x_len <= 0) return false;

	if(y_dir)
	{
		// y; forward
		if(destrect.top < cliprect.top)
			y_dest_start = cliprect.top,
			y_ref_start = (refrect.top << 16) + y_step * (cliprect.top - destrect.top);
		else
			y_dest_start = destrect.top,
			y_ref_start = (refrect.top << 16);

		if(destrect.bottom > cliprect.bottom)
			y_len = cliprect.bottom - y_dest_start;
		else
			y_len = destrect.bottom - y_dest_start;
	}
	else
	{
		// y; backward
		y_step = -y_step;

		if(destrect.top < cliprect.top)
			y_ref_start = ((refrect.bottom << 16) - 1) + y_step * (cliprect.top - destrect.top),
			y_dest_start = cliprect.top;
		else
			y_ref_start = ((refrect.bottom << 16) - 1),
			y_dest_start = destrect.top;

		if(destrect.bottom > cliprect.bottom)
			y_len = cliprect.bottom - y_dest_start;
		else
			y_len = destrect.bottom - y_dest_start;
	}

	if(y_len <= 0) return false;

	// independent check
	if(method == bmCopy && opa == 255 && !hda && !IsIndependent())
	{
		if(destrect.left == 0 && destrect.top == 0 &&
			destrect.right == (tjs_int)GetWidth() && destrect.bottom == (tjs_int)GetHeight())
		{
			// cover overall
			IndependNoCopy(); // indepent with no-copy
		}
	}

	const tjs_uint8 * refp = (const tjs_uint8*)ref->GetScanLine(0);
	tjs_uint8 * destp = (tjs_uint8*)GetScanLineForWrite(y_dest_start);
	destp += x_dest_start * sizeof(tjs_uint32);
	tjs_int refpitch = ref->GetPitchBytes();
	tjs_int destpitch = GetPitchBytes();


	// transfer
	switch(method)
	{
	case bmCopy:
		if(opa == 255)
		{
			// stretching copy
			if(TVP_BILINEAR_FORCE_COND || type >= stFastLinear)
			{
				if(TVP_BILINEAR_FORCE_COND || !hda)
				{
					TVPDoBiLinearStretchLoop( // bilinear interpolation
						tTVPInterpStretchCopyFunctionObject(),
						TVP_DoBilinearStretchLoop_ARGS);
				}
				else
				{
					TVPDoStretchLoop(tTVPStretchFunctionObject(TVPStretchColorCopy),
						TVP_DoStretchLoop_ARGS);
				}
			}
			else
			{
				if(!hda)
					TVPDoStretchLoop(tTVPStretchFunctionObject(TVPStretchCopy),
						TVP_DoStretchLoop_ARGS);
				else
					TVPDoStretchLoop(tTVPStretchFunctionObject(TVPStretchColorCopy),
						TVP_DoStretchLoop_ARGS);
			}
		}
		else
		{
			// stretching constant ratio alpha blendng
			if(TVP_BILINEAR_FORCE_COND || type >= stFastLinear)
			{
				// bilinear interpolation
				if(TVP_BILINEAR_FORCE_COND || !hda)
				{
					TVPDoBiLinearStretchLoop( // bilinear interpolation
						tTVPInterpStretchConstAlphaBlendFunctionObject(opa),
						TVP_DoBilinearStretchLoop_ARGS);
				}
				else
				{
					TVPDoStretchLoop(
						tTVPStretchWithOpacityFunctionObject(TVPStretchConstAlphaBlend_HDA, opa),
						TVP_DoStretchLoop_ARGS);
				}
			}
			else
			{
				if(!hda)
					TVPDoStretchLoop(
						tTVPStretchWithOpacityFunctionObject(TVPStretchConstAlphaBlend, opa),
						TVP_DoStretchLoop_ARGS);
				else
					TVPDoStretchLoop(
						tTVPStretchWithOpacityFunctionObject(TVPStretchConstAlphaBlend_HDA, opa),
						TVP_DoStretchLoop_ARGS);
			}
		}
		break;

	case bmCopyOnAlpha:
		// constant ratio alpha blending, with consideration of destination alpha
		if(opa == 255)
		{
			// full opaque stretching copy
			TVPDoStretchLoop(
				tTVPStretchFunctionObject(TVPStretchCopyOpaqueImage),
				TVP_DoStretchLoop_ARGS);
		}
		else
		{
			// stretching constant ratio alpha blending
			TVPDoStretchLoop(
				tTVPStretchWithOpacityFunctionObject(TVPStretchConstAlphaBlend_d, opa),
				TVP_DoStretchLoop_ARGS);
		}
		break;

	case bmAlpha:
		// alpha blending, ignoring destination alpha
		if(opa == 255)
		{
			if(!hda)
				TVPDoStretchLoop(
					tTVPStretchFunctionObject(TVPStretchAlphaBlend),
					TVP_DoStretchLoop_ARGS);
			else
				TVPDoStretchLoop(
					tTVPStretchFunctionObject(TVPStretchAlphaBlend_HDA),
					TVP_DoStretchLoop_ARGS);
		}
		else
		{
			if(!hda)
				TVPDoStretchLoop(
					tTVPStretchWithOpacityFunctionObject(TVPStretchAlphaBlend_o, opa),
					TVP_DoStretchLoop_ARGS);
			else
				TVPDoStretchLoop(
					tTVPStretchWithOpacityFunctionObject(TVPStretchAlphaBlend_HDA_o, opa),
					TVP_DoStretchLoop_ARGS);
		}
		break;

	case bmAlphaOnAlpha:
		// stretching alpha blending, with consideration of destination alpha
		if(opa == 255)
			TVPDoStretchLoop(
				tTVPStretchFunctionObject(TVPStretchAlphaBlend_d),
				TVP_DoStretchLoop_ARGS);
		else
			TVPDoStretchLoop(
				tTVPStretchWithOpacityFunctionObject(TVPStretchAlphaBlend_do, opa),
				TVP_DoStretchLoop_ARGS);
		break;

	case bmAddAlpha:
		// additive alpha blending
		if(opa == 255)
		{
			if(TVP_BILINEAR_FORCE_COND || !hda)
			{
				if(TVP_BILINEAR_FORCE_COND || type >= stFastLinear)
				{
					TVPDoBiLinearStretchLoop( // bilinear interpolation
						tTVPInterpStretchAdditiveAlphaBlendFunctionObject(),
						TVP_DoBilinearStretchLoop_ARGS);
				}
				else
				{
					TVPDoStretchLoop(
						tTVPStretchFunctionObject(TVPStretchAdditiveAlphaBlend),
						TVP_DoStretchLoop_ARGS);
				}
			}
			else
			{
				TVPDoStretchLoop(
					tTVPStretchFunctionObject(TVPStretchAdditiveAlphaBlend_HDA),
					TVP_DoStretchLoop_ARGS);
			}
		}
		else
		{
			if(TVP_BILINEAR_FORCE_COND || !hda)
			{
				if(TVP_BILINEAR_FORCE_COND || type >= stFastLinear)
				{
					TVPDoBiLinearStretchLoop( // bilinear interpolation
						tTVPInterpStretchAdditiveAlphaBlend_oFunctionObject(opa),
						TVP_DoBilinearStretchLoop_ARGS);
				}
				else
				{
					TVPDoStretchLoop(
						tTVPStretchWithOpacityFunctionObject(TVPStretchAdditiveAlphaBlend_o, opa),
						TVP_DoStretchLoop_ARGS);
				}
			}
			else
			{
				TVPDoStretchLoop(
					tTVPStretchWithOpacityFunctionObject(TVPStretchAdditiveAlphaBlend_HDA_o, opa),
					TVP_DoStretchLoop_ARGS);
			}
		}
		break;

	case bmAddAlphaOnAddAlpha:
		// additive alpha on additive alpha
		if(opa == 255)
			TVPDoStretchLoop(
				tTVPStretchFunctionObject(TVPStretchAdditiveAlphaBlend_a),
				TVP_DoStretchLoop_ARGS);
		else
			TVPDoStretchLoop(
				tTVPStretchWithOpacityFunctionObject(TVPStretchAdditiveAlphaBlend_ao, opa),
				TVP_DoStretchLoop_ARGS);
		break;

	case bmAddAlphaOnAlpha:
		// additive alpha on simple alpha
		; // yet not implemented
		break;

	case bmAlphaOnAddAlpha:
		// simple alpha on additive alpha
		if(opa == 255)
			TVPDoStretchLoop(
				tTVPStretchFunctionObject(TVPStretchAlphaBlend_a),
				TVP_DoStretchLoop_ARGS);
		else
			TVPDoStretchLoop(
				tTVPStretchWithOpacityFunctionObject(TVPStretchAlphaBlend_ao, opa),
				TVP_DoStretchLoop_ARGS);
		break;

	case bmCopyOnAddAlpha:
		// constant ratio alpha blending (assuming source is opaque)
		// with consideration of destination additive alpha
		if(opa == 255)
			TVPDoStretchLoop(
				tTVPStretchFunctionObject(TVPStretchCopyOpaqueImage),
				TVP_DoStretchLoop_ARGS);
		else
			TVPDoStretchLoop(
				tTVPStretchWithOpacityFunctionObject(TVPStretchConstAlphaBlend_a, opa),
				TVP_DoStretchLoop_ARGS);
		break;


	default:
		; // yet not implemented
	}

	return true;
}

//---------------------------------------------------------------------------
// template function for affine loop
//---------------------------------------------------------------------------

// define function pointer types for transforming line
typedef TVP_GL_FUNC_PTR_DECL(void, tTVPAffineFunction,
	(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch));
typedef TVP_GL_FUNC_PTR_DECL(void, tTVPAffineWithOpacityFunction,
	(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa));
typedef TVP_GL_FUNC_PTR_DECL(void, tTVPBilinearAffineFunction,
	(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch));
typedef TVP_GL_FUNC_PTR_DECL(void, tTVPBilinearAffineWithOpacityFunction,
	(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa));

//---------------------------------------------------------------------------

// declare affine function object class
class tTVPAffineFunctionObject
{
	tTVPAffineFunction Func;
public:
	tTVPAffineFunctionObject(tTVPAffineFunction func) : Func(func) {;}
	void operator () (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)
	{
		Func(dest, len, src, sx, sy, stepx, stepy, srcpitch);
	}
};

class tTVPAffineWithOpacityFunctionObject
{
	tTVPAffineWithOpacityFunction Func;
	tjs_int Opacity;
public:
	tTVPAffineWithOpacityFunctionObject(tTVPAffineWithOpacityFunction func, tjs_int opa) :
		Func(func), Opacity(opa) {;}
	void operator () (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)
	{
		Func(dest, len, src, sx, sy, stepx, stepy, srcpitch, Opacity);
	}
};

class tTVPBilinearAffineFunctionObject
{
protected:
	tTVPBilinearAffineFunction Func;
public:
	tTVPBilinearAffineFunctionObject(tTVPBilinearAffineFunction func) : Func(func) {;}
	void operator () (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)
	{
		Func(dest, len, src, sx, sy, stepx, stepy, srcpitch);
	}
};

#define TVP_DEFINE_BILINEAR_AFFINE_FUNCTION(func, one) class \
t##func##FunctionObject : \
	public tTVPBilinearAffineFunctionObject \
{ \
public: \
	t##func##FunctionObject() : \
		tTVPBilinearAffineFunctionObject(func) {;} \
	void DoOnePixel(tjs_uint32 *dest, tjs_uint32 color) \
	{ one; } \
};


class tTVPBilinearAffineWithOpacityFunctionObject
{
protected:
	tTVPBilinearAffineWithOpacityFunction Func;
	tjs_int Opacity;
public:
	tTVPBilinearAffineWithOpacityFunctionObject(tTVPBilinearAffineWithOpacityFunction func, tjs_int opa) :
		Func(func), Opacity(opa) {;}
	void operator () (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,
	tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)
	{
		Func(dest, len, src, sx, sy, stepx, stepy, srcpitch, Opacity);
	}
};

#define TVP_DEFINE_BILINEAR_AFFINE_WITH_OPACITY_FUNCTION(func, one) class \
t##func##FunctionObject : \
	public tTVPBilinearAffineWithOpacityFunctionObject \
{ \
public: \
	t##func##FunctionObject(tjs_int opa) : \
		tTVPBilinearAffineWithOpacityFunctionObject(func, opa) {;} \
	void DoOnePixel(tjs_uint32 *dest, tjs_uint32 color) \
	{ one; } \
};

//---------------------------------------------------------------------------

// declare affine transformation function object for bilinear interpolation
TVP_DEFINE_BILINEAR_AFFINE_FUNCTION(
	TVPInterpLinTransCopy,
	*dest = color);

TVP_DEFINE_BILINEAR_AFFINE_WITH_OPACITY_FUNCTION(
	TVPInterpLinTransConstAlphaBlend,
	*dest = TVPBlendARGB(*dest, color, Opacity));


TVP_DEFINE_BILINEAR_AFFINE_FUNCTION(
	TVPInterpLinTransAdditiveAlphaBlend,
	*dest = TVPAddAlphaBlend_n_a(*dest, color));

TVP_DEFINE_BILINEAR_AFFINE_WITH_OPACITY_FUNCTION(
	TVPInterpLinTransAdditiveAlphaBlend_o,
	*dest = TVPAddAlphaBlend_n_a_o(*dest, color, Opacity));

//---------------------------------------------------------------------------

// declare affine loop function
#define TVP_DoAffineLoop_ARGS  sxs, sys, \
		dest, l, len, src, srcpitch, sxl, syl, srcrect
template <typename tFuncStretch, typename tFuncAffine>
void tTVPBaseBitmap::TVPDoAffineLoop(
		tFuncStretch stretch,
		tFuncAffine affine,
		tjs_int sxs,
		tjs_int sys,
		tjs_uint8 *dest,
		tjs_int l,
		tjs_int len,
		const tjs_uint8 *src,
		tjs_int srcpitch,
		tjs_int sxl,
		tjs_int syl,
		const tTVPRect & srcrect)
{
	tjs_int len_remain = len;

	// skip "out of source rectangle" points
	// from last point
	sxl += 32768; // do +0.5 to rounding
	syl += 32768; // do +0.5 to rounding

	tjs_int spx, spy;
	tjs_uint32 *dp;
	dp = (tjs_uint32*)dest + l + len-1;
	spx = (len-1)*sxs + sxl;
	spy = (len-1)*sys + syl;

	while(len_remain > 0)
	{
		tjs_int sx, sy;
		sx = spx >> 16;
		sy = spy >> 16;
		if(sx >= srcrect.left && sx < srcrect.right &&
			sy >= srcrect.top && sy < srcrect.bottom)
			break;
		dp--;
		spx -= sxs;
		spy -= sys;
		len_remain --;
	}

	// from first point
	spx = sxl;
	spy = syl;
	dp = (tjs_uint32*)dest + l;

	while(len_remain > 0)
	{
		tjs_int sx, sy;
		sx = spx >> 16;
		sy = spy >> 16;
		if(sx >= srcrect.left && sx < srcrect.right &&
			sy >= srcrect.top && sy < srcrect.bottom)
			break;
		dp++;
		spx += sxs;
		spy += sys;
		len_remain --;
	}

	if(len_remain > 0)
	{
		// transfer a line
		if(sys == 0)
			stretch(dp, len_remain,
				(tjs_uint32*)(src + (spy>>16) * srcpitch), spx, sxs);
		else
			affine(dp, len_remain,
				(tjs_uint32*)src, spx, spy, sxs, sys, srcpitch);
	}
}

//---------------------------------------------------------------------------
// declare affine loop function for bilinear interpolation

#define TVP_DoBilinearAffineLoop_ARGS  sxs, sys, \
		dest, l, len, src, srcpitch, sxl, syl, srccliprect, srcrect
template <typename tFuncStretch, typename tFuncAffine>
void tTVPBaseBitmap::TVPDoBilinearAffineLoop(
		tFuncStretch stretch,
		tFuncAffine affine,
		tjs_int sxs,
		tjs_int sys,
		tjs_uint8 *dest,
		tjs_int l,
		tjs_int len,
		const tjs_uint8 *src,
		tjs_int srcpitch,
		tjs_int sxl,
		tjs_int syl,
		const tTVPRect & srccliprect,
		const tTVPRect & srcrect)
{
	// bilinear interpolation copy
	tjs_int len_remain = len;
	tjs_int spx, spy;
	tjs_int sx, sy;
	tjs_uint32 *dp;

	// skip "out of source rectangle" points
	// from last point
	sxl += 32768; // do +0.5 to rounding
	syl += 32768; // do +0.5 to rounding

	dp = (tjs_uint32*)dest + l + len-1;
	spx = (len-1)*sxs + sxl;
	spy = (len-1)*sys + syl;

	while(len_remain > 0)
	{
		tjs_int sx, sy;
		sx = spx >> 16;
		sy = spy >> 16;
		if(sx >= srcrect.left && sx < srcrect.right &&
			sy >= srcrect.top && sy < srcrect.bottom)
			break;
		dp--;
		spx -= sxs;
		spy -= sys;
		len_remain --;
	}

	// from first point
	spx = sxl;
	spy = syl;
	dp = (tjs_uint32*)dest + l;

	while(len_remain > 0)
	{
		tjs_int sx, sy;
		sx = spx >> 16;
		sy = spy >> 16;
		if(sx >= srcrect.left && sx < srcrect.right &&
			sy >= srcrect.top && sy < srcrect.bottom)
			break;
		dp++;
		l++; // step l forward
		spx += sxs;
		spy += sys;
		len_remain --;
	}

	sxl = spx;
	syl = spy;

	sxl -= 32768; // take back the original
	syl -= 32768; // take back the original

#define FIX_SX_SY	\
	if(sx < srccliprect.left) \
		sx = srccliprect.left, fixed_count ++; \
	if(sx >= srccliprect.right) \
		sx = srccliprect.right - 1, fixed_count++; \
	if(sy < srccliprect.top) \
		sy = srccliprect.top, fixed_count++; \
	if(sy >= srccliprect.bottom) \
		sy = srccliprect.bottom - 1, fixed_count++;


	// from last point
	spx = (len_remain-1)*sxs + sxl/* - 32768*/;
	spy = (len_remain-1)*sys + syl/* - 32768*/;
	dp = (tjs_uint32*)dest + l + len_remain-1;

	while(len_remain > 0)
	{
		tjs_int fixed_count = 0;
		tjs_uint32 c00, c01, c10, c11;
		tjs_int blend_x, blend_y;

		sx = (spx >> 16);
		sy = (spy >> 16);
		FIX_SX_SY
		c00 = *((const tjs_uint32*)(src + sy * srcpitch) + sx);

		sx = (spx >> 16) + 1;
		sy = (spy >> 16);
		FIX_SX_SY
		c01 = *((const tjs_uint32*)(src + sy * srcpitch) + sx);

		sx = (spx >> 16);
		sy = (spy >> 16) + 1;
		FIX_SX_SY
		c10 = *((const tjs_uint32*)(src + sy * srcpitch) + sx);

		sx = (spx >> 16) + 1;
		sy = (spy >> 16) + 1;
		FIX_SX_SY
		c11 = *((const tjs_uint32*)(src + sy * srcpitch) + sx);

		if(!fixed_count) break;

		blend_x = (spx & 0xffff) >> 8;
		blend_x += blend_x>>7; // adjust blend ratio
		blend_y = (spy & 0xffff) >> 8;
		blend_y += blend_y>>7;

		affine.DoOnePixel(dp, TVPBlendARGB(
			TVPBlendARGB(c00, c01, blend_x),
			TVPBlendARGB(c10, c11, blend_x),
				blend_y));

		dp--;
		spx -= sxs;
		spy -= sys;
		len_remain --;
	}

	// from first point
	spx = sxl/* - 32768*/;
	spy = syl/* - 32768*/;
	dp = (tjs_uint32*)dest + l;

	while(len_remain > 0)
	{
		tjs_int fixed_count = 0;
		tjs_uint32 c00, c01, c10, c11;
		tjs_int blend_x, blend_y;

		sx = (spx >> 16);
		sy = (spy >> 16);
		FIX_SX_SY
		c00 = *((const tjs_uint32*)(src + sy * srcpitch) + sx);

		sx = (spx >> 16) + 1;
		sy = (spy >> 16);
		FIX_SX_SY
		c01 = *((const tjs_uint32*)(src + sy * srcpitch) + sx);

		sx = (spx >> 16);
		sy = (spy >> 16) + 1;
		FIX_SX_SY
		c10 = *((const tjs_uint32*)(src + sy * srcpitch) + sx);

		sx = (spx >> 16) + 1;
		sy = (spy >> 16) + 1;
		FIX_SX_SY
		c11 = *((const tjs_uint32*)(src + sy * srcpitch) + sx);

		if(!fixed_count) break;

		blend_x = (spx & 0xffff) >> 8;
		blend_x += blend_x>>7; // adjust blend ratio
		blend_y = (spy & 0xffff) >> 8;
		blend_y += blend_y>>7;

		affine.DoOnePixel(dp, TVPBlendARGB(
			TVPBlendARGB(c00, c01, blend_x),
			TVPBlendARGB(c10, c11, blend_x),
				blend_y));

		dp++;
		spx += sxs;
		spy += sys;
		len_remain --;
	}

	if(len_remain > 0)
	{
		// do center part (this may takes most time)
		if(sys == 0)
		{
			// do stretch
			const tjs_uint8 * l1 = src + (spy >> 16) * srcpitch;
			const tjs_uint8 * l2 = l1 + srcpitch;
			stretch(
				dp,
				len_remain,
				(const tjs_uint32*)l1,
				(const tjs_uint32*)l2,
				(spy & 0xffff) >> 8,
				spx,
				sxs);
		}
		else
		{
			// do affine
			affine(dp, len_remain,
				(tjs_uint32*)src, spx, spy, sxs, sys, srcpitch);
		}
	}
}
//---------------------------------------------------------------------------
static inline tjs_int floor_16(tjs_int x)
{
	// take floor of 16.16 fixed-point format
	return (x >> 16) - (x < 0);
}
static inline tjs_int div_16(tjs_int x, tjs_int y)
{
	// return x * 65536 / y
	return (tjs_int)((tjs_int64)x * 65536 / y);
}
static inline tjs_int mul_16(tjs_int x, tjs_int y)
{
	// return x * y / 65536
	return (tjs_int)((tjs_int64)x * y / 65536);
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::AffineBlt(tTVPRect destrect, const tTVPBaseBitmap *ref,
		tTVPRect refrect, const tTVPPointD * points_in,
			tTVPBBBltMethod method, tjs_int opa,
			tTVPRect * updaterect,
			bool hda, tTVPBBStretchType mode, bool clear, tjs_uint32 clearcolor)
{
	// unlike other drawing methods, 'destrect' is the clip rectangle of the
	// destination bitmap.
	// affine transformation coordinates are to be applied on zero-offset
	// source rectangle:
	// (0, 0) - (refreft.right - refrect.left, refrect.bottom - refrect.top)

	// points are given as destination points, corresponding to three source
	// points; upper-left, upper-right, bottom-left.

	// if 'clear' is true, area which is out of the affine destination and
	// within the destination bounding box, is to be filled with value 'clearcolor'.

	// returns false if the updating rect is not updated

	// extract stretch type
	tTVPBBStretchType type = (tTVPBBStretchType)(mode & stTypeMask);

	// check source rectangle
	if(refrect.left >= refrect.right ||
		refrect.top >= refrect.bottom) return false;
	if(refrect.left < 0 || refrect.top < 0 ||
		refrect.right > (tjs_int)ref->GetWidth() ||
		refrect.bottom > (tjs_int)ref->GetHeight())
		TVPThrowExceptionMessage(TVPOutOfRectangle);

	// multiply source rectangle points by 65536 (16.16 fixed-point)
	// note that each pixel has actually 1.0 x 1.0 size
	// eg. a pixel at 0,0 may have (-0.5, -0.5) - (0.5, 0.5) area
	tTVPRect srcrect = refrect; // unmultiplied source rectangle is saved as srcrect
	refrect.left   = refrect.left   * 65536 - 32768;
	refrect.top    = refrect.top    * 65536 - 32768;
	refrect.right  = refrect.right  * 65536 - 32768;
	refrect.bottom = refrect.bottom * 65536 - 32768;

	// create point list in fixed point real format
	tTVPPoint points[3];
	for(tjs_int i = 0; i < 3; i++)
		points[i].x = points_in[i].x * 65536, points[i].y = points_in[i].y * 65536;

	// check destination rectangle
	if(destrect.left < 0) destrect.left = 0;
	if(destrect.top < 0) destrect.top = 0;
	if(destrect.right > (tjs_int)GetWidth()) destrect.right = GetWidth();
	if(destrect.bottom > (tjs_int)GetHeight()) destrect.bottom = GetHeight();

	if(destrect.left >= destrect.right ||
		destrect.top >= destrect.bottom) return false; // not drawable

	// vertex points
	tjs_int points_x[4];
	tjs_int points_y[4];

	// check each vertex and find most-top/most-bottom/most-left/most-right points
	tjs_int scanlinestart, scanlineend; // most-top/most-bottom
	tjs_int leftlimit, rightlimit; // most-left/most-right

	// - upper-left (p0)
	points_x[0] = points[0].x;
	points_y[0] = points[0].y;
	leftlimit = points_x[0];
	rightlimit = points_x[0];
	scanlinestart = points_y[0];
	scanlineend = points_y[0];

	// - upper-right (p1)
	points_x[1] = points[1].x;
	points_y[1] = points[1].y;
	if(leftlimit > points_x[1]) leftlimit = points_x[1];
	if(rightlimit < points_x[1]) rightlimit = points_x[1];
	if(scanlinestart > points_y[1]) scanlinestart = points_y[1];
	if(scanlineend < points_y[1]) scanlineend = points_y[1];

	// - bottom-right (p2)
	points_x[2] = points[1].x - points[0].x + points[2].x;
	points_y[2] = points[1].y - points[0].y + points[2].y;
	if(leftlimit > points_x[2]) leftlimit = points_x[2];
	if(rightlimit < points_x[2]) rightlimit = points_x[2];
	if(scanlinestart > points_y[2]) scanlinestart = points_y[2];
	if(scanlineend < points_y[2]) scanlineend = points_y[2];

	// - bottom-left (p3)
	points_x[3] = points[2].x;
	points_y[3] = points[2].y;
	if(leftlimit > points_x[3]) leftlimit = points_x[3];
	if(rightlimit < points_x[3]) rightlimit = points_x[3];
	if(scanlinestart > points_y[3]) scanlinestart = points_y[3];
	if(scanlineend < points_y[3]) scanlineend = points_y[3];

	// rough check destrect intersections
	if(floor_16(leftlimit) >= destrect.right) return false;
	if(floor_16(rightlimit) < destrect.left) return false;
	if(floor_16(scanlinestart) >= destrect.bottom) return false;
	if(floor_16(scanlineend) < destrect.top) return false;

	// compute sxstep and systep (step count for source image)
	tjs_int sxstep, systep;

	{
		double dv01x = (points[1].x - points[0].x) * (1.0 / 65536.0);
		double dv01y = (points[1].y - points[0].y) * (1.0 / 65536.0);
		double dv02x = (points[2].x - points[0].x) * (1.0 / 65536.0);
		double dv02y = (points[2].y - points[0].y) * (1.0 / 65536.0);

		double x01, x02, s01, s02;

		if     (points[1].y == points[0].y)
		{
			sxstep = (tjs_int)((refrect.right - refrect.left) / dv01x);
			systep = 0;
		}
		else if(points[2].y == points[0].y)
		{
			sxstep = 0;
			systep = (tjs_int)((refrect.bottom - refrect.top) / dv02x);
		}
		else
		{
			x01 = dv01x / dv01y;
			s01 = (refrect.right - refrect.left) / dv01y;

			x02 = dv02x / dv02y;
			s02 = (refrect.top - refrect.bottom) / dv02y;

			double len = x01 - x02;

			sxstep = (tjs_int)(s01 / len);
			systep = (tjs_int)(s02 / len);
		}
	}

	// prepare to transform...
	tjs_int yc    = (scanlinestart + 32768) / 65536;
	tjs_int yclim = (scanlineend   + 32768) / 65536;

	if(destrect.top > yc) yc = destrect.top;
	if(destrect.bottom <= yclim) yclim = destrect.bottom - 1;
	if(yc >= destrect.bottom || yclim < 0)
		return false; // not drawable

	tjs_uint8 * dest = (tjs_uint8*)GetScanLineForWrite(yc);
	tjs_int destpitch = GetPitchBytes();
	const tjs_uint8 * src = (const tjs_uint8 *)ref->GetScanLine(0);
	tjs_int srcpitch = ref->GetPitchBytes();

	yc    = yc    * 65536;
	yclim = yclim * 65536;

	// make srccliprect
	tTVPRect srccliprect;
	if(mode & stRefNoClip)
		srccliprect.left = 0,
		srccliprect.top = 0,
		srccliprect.right = (tjs_int)ref->GetWidth(),
		srccliprect.bottom = (tjs_int)ref->GetHeight(); // no clip; all the bitmap will be the source
	else
		srccliprect = srcrect; // clip; the source is limited to the source rectangle


	// process per a line
	tjs_int mostupper  = -1;
	tjs_int mostbottom = -1;
	bool firstline = true;


	for(; yc <= yclim; yc+=65536, dest += destpitch)
	{
		// transfer a line

		// skip out-of-range lines
		tjs_int yl = yc;
		if(yl < scanlinestart)
			continue;
		if(yl >= scanlineend)
			continue;

		// actual write line
		tjs_int y = (yc+32768) / 65536;

		// find line intersection
		// line codes are:
		// p0 .. p1  : 0
		// p1 .. p2  : 1
		// p2 .. p3  : 2
		// p3 .. p0  : 3
		tjs_int line_code0, line_code1;
		tjs_int where0, where1;
		tjs_int where, code;

		for(code = 0; code < 4; code ++)
		{
			tjs_int ip0 = code;
			tjs_int ip1 = (code + 1) & 3;
			if     (points_y[ip0] == yl && points_y[ip1] == yl)
			{
				where = points_x[ip1] > points_x[ip0] ? 0 : 65536 - 1;
				code += 8;
				break;
			}
		}
		if(code < 8)
		{
			for(code = 0; code < 4; code ++)
			{
				tjs_int ip0 = code;
				tjs_int ip1 = (code + 1) & 3;
				if     (points_y[ip0] == yl && points_y[ip1] == yl)
				{
					where = points_x[ip1] > points_x[ip0] ? 0 : 65536 - 1;
					code += 4;
					break;
				}
				else if(points_y[ip0] <= yl && points_y[ip1] > yl)
				{
					where = div_16(yl - points_y[ip0], points_y[ip1] - points_y[ip0]);
					break;
				}
				else if(points_y[ip0] >  yl && points_y[ip1] <= yl)
				{
					where = div_16(points_y[ip0] - yl, points_y[ip0] - points_y[ip1]);
					break;
				}
			}
		}
		line_code0 = code;
		where0 = where;

		if(line_code0 < 4)
		{
			for(code = 0; code < 4; code ++)
			{
				if(code == line_code0) continue;
				tjs_int ip0 = code;
				tjs_int ip1 = (code + 1) & 3;
				if     (points_y[ip0] == yl && points_y[ip1] == yl)
				{
					where = points_x[ip1] > points_x[ip0] ? 0 : 65536 - 1;
					break;
				}
				else if(points_y[ip0] <= yl && points_y[ip1] >  yl)
				{
					where = div_16(yl - points_y[ip0], points_y[ip1] - points_y[ip0]);
					break;
				}
				else if(points_y[ip0] >  yl && points_y[ip1] <= yl)
				{
					where = div_16(points_y[ip0]- yl , points_y[ip0] - points_y[ip1]);
					break;
				}
			}
			line_code1 = code;
			where1 = where;
		}
		else
		{
			line_code0 &= 3;
			line_code1 = line_code0;
			where1 = 65535 - where0;
		}

		// compute intersection point
		tjs_int ll, rr, sxl, syl, sxr, syr;
		switch(line_code0)
		{
		case 0:
			ll  = mul_16(points_x[1] - points_x[0]   , where0) + points_x[0];
			sxl = mul_16(refrect.right - refrect.left, where0) + refrect.left;
			syl = refrect.top;
			break;
		case 1:
			ll  = mul_16(points_x[2] - points_x[1]   , where0) + points_x[1];
			sxl = refrect.right;
			syl = mul_16(refrect.bottom - refrect.top, where0) + refrect.top;
			break;
		case 2:
			ll  = mul_16(points_x[3] - points_x[2]   , where0) + points_x[2];
			sxl = mul_16(refrect.left - refrect.right, where0) + refrect.right;
			syl = refrect.bottom;
			break;
		case 3:
			ll  = mul_16(points_x[0] - points_x[3]   , where0) + points_x[3];
			sxl = refrect.left;
			syl = mul_16(refrect.top - refrect.bottom, where0) + refrect.bottom;
			break;
		}

		switch(line_code1)
		{
		case 0:
			rr  = mul_16(points_x[1] - points_x[0]   , where1) + points_x[0];
			sxr = mul_16(refrect.right - refrect.left, where1) + refrect.left;
			syr = refrect.top;
			break;
		case 1:
			rr  = mul_16(points_x[2] - points_x[1]   , where1) + points_x[1];
			sxr = refrect.right;
			syr = mul_16(refrect.bottom - refrect.top, where1) + refrect.top;
			break;
		case 2:
			rr  = mul_16(points_x[3] - points_x[2]   , where1) + points_x[2];
			sxr = mul_16(refrect.left - refrect.right, where1) + refrect.right;
			syr = refrect.bottom;
			break;
		case 3:
			rr  = mul_16(points_x[0] - points_x[3]   , where1) + points_x[3];
			sxr = refrect.left;
			syr = mul_16(refrect.top - refrect.bottom, where1) + refrect.bottom;
			break;
		}


		// l, r, sxs, sys, and len
		tjs_int sxs, sys, len;
		sxs = sxstep;
		sys = systep;
		if(ll > rr)
		{
			std::swap(ll, rr);
			std::swap(sxl, sxr);
			std::swap(syl, syr);
		}

		// round l and r to integer
		tjs_int l, r;

		l = ((ll-1) / 65536) + 1; // ceil
		sxl += mul_16(65535 - ((ll-1) % 65536), sxs); // adjust source start point x
		syl += mul_16(65535 - ((ll-1) % 65536), sys); // adjust source start point y

		r = ((rr-1) / 65536) + 1; // ceil, note that at this point r is *NOT* inclusive

		// - clip widh destrect.left and destrect.right
		if(l < destrect.left)
		{
			tjs_int d = destrect.left - l;
			sxl += d * sxs;
			syl += d * sys;
			l = destrect.left;
		}
		if(r > destrect.right) r = destrect.right;

		// - compute horizontal length
		len = r - l;

		// - transfer
		if(len > 0)
		{
			// fill left and right of the line if 'clear' is specified
			if(clear)
			{
				tjs_int clen;

				int ll = l + 1;
				if(ll > destrect.right) ll = destrect.right;
				clen = ll - destrect.left;
				if(clen > 0)
					(hda?TVPFillColor:TVPFillARGB)((tjs_uint32*)dest + destrect.left, clen, clearcolor);
				int rr = r - 1;
				if(rr < destrect.left) rr = destrect.left;
				clen = destrect.right - rr;
				if(clen > 0)
					(hda?TVPFillColor:TVPFillARGB)((tjs_uint32*)dest + rr, clen, clearcolor);
			}


			// update updaterect
			if(firstline)
			{
				leftlimit = l;
				rightlimit = r;
				firstline = false;
				mostupper = mostbottom = y;
			}
			else
			{
				if(l < leftlimit) leftlimit = l;
				if(r > rightlimit) rightlimit = r;
				mostbottom = y;
			}

			// transfer using each blend function
			switch(method)
			{
			case bmCopy:
				if(opa == 255)
				{
					if(TVP_BILINEAR_FORCE_COND || !hda)
					{
						if(TVP_BILINEAR_FORCE_COND || type >= stFastLinear)
						{
							// bilinear interpolation
							TVPDoBilinearAffineLoop(
								tTVPInterpStretchCopyFunctionObject(),
								tTVPInterpLinTransCopyFunctionObject(),
								TVP_DoBilinearAffineLoop_ARGS);
						}
						else
						{
							// point on point (nearest) copy
							TVPDoAffineLoop(
								tTVPStretchFunctionObject(TVPStretchCopy),
								tTVPAffineFunctionObject(TVPLinTransCopy),
								TVP_DoAffineLoop_ARGS);
						}
					}
					else
					{
						TVPDoAffineLoop(
							tTVPStretchFunctionObject(TVPStretchColorCopy),
							tTVPAffineFunctionObject(TVPLinTransColorCopy),
							TVP_DoAffineLoop_ARGS);
					}
				}
				else
				{
					if(TVP_BILINEAR_FORCE_COND || !hda)
					{
						if(TVP_BILINEAR_FORCE_COND || type >= stFastLinear)
						{
							// bilinear interpolation
							TVPDoBilinearAffineLoop(
								tTVPInterpStretchConstAlphaBlendFunctionObject(opa),
								tTVPInterpLinTransConstAlphaBlendFunctionObject(opa),
								TVP_DoBilinearAffineLoop_ARGS);
						}
						else
						{
							TVPDoAffineLoop(
								tTVPStretchWithOpacityFunctionObject(TVPStretchConstAlphaBlend, opa),
								tTVPAffineWithOpacityFunctionObject(TVPLinTransConstAlphaBlend, opa),
								TVP_DoAffineLoop_ARGS);
						}
					}
					else
					{
						TVPDoAffineLoop(
							tTVPStretchWithOpacityFunctionObject(TVPStretchConstAlphaBlend_HDA, opa),
							tTVPAffineWithOpacityFunctionObject(TVPLinTransConstAlphaBlend_HDA, opa),
							TVP_DoAffineLoop_ARGS);
					}
				}
				break;

			case bmCopyOnAlpha:
				// constant ratio alpha blending, with consideration of destination alpha
				if(opa == 255)
					TVPDoAffineLoop(
						tTVPStretchFunctionObject(TVPStretchCopyOpaqueImage),
						tTVPAffineFunctionObject(TVPLinTransCopyOpaqueImage),
						TVP_DoAffineLoop_ARGS);
				else
					TVPDoAffineLoop(
						tTVPStretchWithOpacityFunctionObject(TVPStretchConstAlphaBlend_d, opa),
						tTVPAffineWithOpacityFunctionObject(TVPLinTransConstAlphaBlend_d, opa),
						TVP_DoAffineLoop_ARGS);
				break;

			case bmAlpha:
				// alpha blending, ignoring destination alpha
				if(opa == 255)
				{
					if(!hda)
						TVPDoAffineLoop(
							tTVPStretchFunctionObject(TVPStretchAlphaBlend),
							tTVPAffineFunctionObject(TVPLinTransAlphaBlend),
							TVP_DoAffineLoop_ARGS);
					else
						TVPDoAffineLoop(
							tTVPStretchFunctionObject(TVPStretchAlphaBlend_HDA),
							tTVPAffineFunctionObject(TVPLinTransAlphaBlend_HDA),
							TVP_DoAffineLoop_ARGS);
				}
				else
				{
					if(!hda)
						TVPDoAffineLoop(
							tTVPStretchWithOpacityFunctionObject(TVPStretchAlphaBlend_o, opa),
							tTVPAffineWithOpacityFunctionObject(TVPLinTransAlphaBlend_o, opa),
							TVP_DoAffineLoop_ARGS);
					else
						TVPDoAffineLoop(
							tTVPStretchWithOpacityFunctionObject(TVPStretchAlphaBlend_HDA_o, opa),
							tTVPAffineWithOpacityFunctionObject(TVPLinTransAlphaBlend_HDA_o, opa),
							TVP_DoAffineLoop_ARGS);
				}
				break;

			case bmAlphaOnAlpha:
				// alpha blending, with consideration of destination alpha
				if(opa == 255)
					TVPDoAffineLoop(
						tTVPStretchFunctionObject(TVPStretchAlphaBlend_d),
						tTVPAffineFunctionObject(TVPLinTransAlphaBlend_d),
						TVP_DoAffineLoop_ARGS);
				else
					TVPDoAffineLoop(
						tTVPStretchWithOpacityFunctionObject(TVPStretchAlphaBlend_do, opa),
						tTVPAffineWithOpacityFunctionObject(TVPLinTransAlphaBlend_do, opa),
						TVP_DoAffineLoop_ARGS);
				break;

			case bmAddAlpha:
				// additive alpha blending, ignoring destination alpha
				if(opa == 255)
				{
					if(TVP_BILINEAR_FORCE_COND || !hda)
					{
						if(TVP_BILINEAR_FORCE_COND || type >= stFastLinear)
						{
							// bilinear interpolation
							TVPDoBilinearAffineLoop(
								tTVPInterpStretchAdditiveAlphaBlendFunctionObject(),
								tTVPInterpLinTransAdditiveAlphaBlendFunctionObject(),
								TVP_DoBilinearAffineLoop_ARGS);
						}
						else
						{
							TVPDoAffineLoop(
								tTVPStretchFunctionObject(TVPStretchAdditiveAlphaBlend),
								tTVPAffineFunctionObject(TVPLinTransAdditiveAlphaBlend),
								TVP_DoAffineLoop_ARGS);
						}
					}
					else
					{
						TVPDoAffineLoop(
							tTVPStretchFunctionObject(TVPStretchAdditiveAlphaBlend_HDA),
							tTVPAffineFunctionObject(TVPLinTransAdditiveAlphaBlend_HDA),
							TVP_DoAffineLoop_ARGS);
					}
				}
				else
				{
					if(TVP_BILINEAR_FORCE_COND || !hda)
					{
						if(TVP_BILINEAR_FORCE_COND || type >= stFastLinear)
						{
							// bilinear interpolation
							TVPDoBilinearAffineLoop(
								tTVPInterpStretchAdditiveAlphaBlend_oFunctionObject(opa),
								tTVPInterpLinTransAdditiveAlphaBlend_oFunctionObject(opa),
								TVP_DoBilinearAffineLoop_ARGS);
						}
						else
						{
							TVPDoAffineLoop(
								tTVPStretchWithOpacityFunctionObject(TVPStretchAlphaBlend_o, opa),
								tTVPAffineWithOpacityFunctionObject(TVPLinTransAdditiveAlphaBlend_o, opa),
								TVP_DoAffineLoop_ARGS);
						}
					}
					else
					{
						TVPDoAffineLoop(
							tTVPStretchWithOpacityFunctionObject(TVPStretchAdditiveAlphaBlend_HDA_o, opa),
							tTVPAffineWithOpacityFunctionObject(TVPLinTransAdditiveAlphaBlend_HDA_o, opa),
							TVP_DoAffineLoop_ARGS);
					}
				}
				break;

			case bmAddAlphaOnAddAlpha:
				// additive alpha blending, with consideration of destination additive alpha
				if(opa == 255)
					TVPDoAffineLoop(
						tTVPStretchFunctionObject(TVPStretchAlphaBlend_a),
						tTVPAffineFunctionObject(TVPLinTransAdditiveAlphaBlend_a),
						TVP_DoAffineLoop_ARGS);
				else
					TVPDoAffineLoop(
						tTVPStretchWithOpacityFunctionObject(TVPStretchAlphaBlend_ao, opa),
						tTVPAffineWithOpacityFunctionObject(TVPLinTransAdditiveAlphaBlend_ao, opa),
						TVP_DoAffineLoop_ARGS);
				break;

			case bmAddAlphaOnAlpha:
				// additive alpha on simple alpha
				; // yet not implemented
				break;

			case bmAlphaOnAddAlpha:
				// simple alpha on additive alpha
				if(opa == 255)
					TVPDoAffineLoop(
						tTVPStretchFunctionObject(TVPStretchAlphaBlend_a),
						tTVPAffineFunctionObject(TVPLinTransAlphaBlend_a),
						TVP_DoAffineLoop_ARGS);
				else
					TVPDoAffineLoop(
						tTVPStretchWithOpacityFunctionObject(TVPStretchAlphaBlend_ao, opa),
						tTVPAffineWithOpacityFunctionObject(TVPLinTransAlphaBlend_ao, opa),
						TVP_DoAffineLoop_ARGS);
				break;

			case bmCopyOnAddAlpha:
				// constant ratio alpha blending (assuming source is opaque)
				// with consideration of destination additive alpha
				if(opa == 255)
					TVPDoAffineLoop(
						tTVPStretchFunctionObject(TVPStretchCopyOpaqueImage),
						tTVPAffineFunctionObject(TVPLinTransCopyOpaqueImage),
						TVP_DoAffineLoop_ARGS);
				else
					TVPDoAffineLoop(
						tTVPStretchWithOpacityFunctionObject(TVPStretchConstAlphaBlend_a, opa),
						tTVPAffineWithOpacityFunctionObject(TVPLinTransConstAlphaBlend_a, opa),
						TVP_DoAffineLoop_ARGS);
				break;

			default:
				; // yet not implemented
			}
		}
	}

	// clear upper and lower area of the affine transformation
	if(clear)
	{
		tjs_int h;
		tjs_uint8 * dest = (tjs_uint8*)GetScanLineForWrite(0);
		tjs_uint8 * p;
		if(mostupper == -1 && mostbottom == -1)
		{
			// special case: nothing was drawn;
			// clear entire area of the destrect
			mostupper  = destrect.bottom;
			mostbottom = destrect.bottom - 1;
		}

		h = mostupper - destrect.top;
		if(h > 0)
		{
			p = dest + destrect.top * destpitch;
			do
				(hda?TVPFillColor:TVPFillARGB)((tjs_uint32*)p + destrect.left,
					destrect.right - destrect.left, clearcolor),
				p += destpitch;
			while(--h);
		}

		h = destrect.bottom - (mostbottom + 1);
		if(h > 0)
		{
			p = dest + (mostbottom + 1) * destpitch;
			do
				(hda?TVPFillColor:TVPFillARGB)((tjs_uint32*)p + destrect.left,
					destrect.right - destrect.left, clearcolor),
				p += destpitch;
			while(--h);
		}

	}

	// fill members of updaterect
	if(updaterect)
	{
		if(clear)
		{
			// clear is specified
			*updaterect = destrect;
				// update rectangle is the same as the destination rectangle
		}
		else if(!firstline)
		{
			// update area is being
			updaterect->left = leftlimit;
			updaterect->right = rightlimit + 1;
			updaterect->top = mostupper;
			updaterect->bottom = mostbottom + 1;
		}
	}

	return clear || !firstline;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::AffineBlt(tTVPRect destrect, const tTVPBaseBitmap *ref,
		tTVPRect refrect, const t2DAffineMatrix & matrix,
			tTVPBBBltMethod method, tjs_int opa,
			tTVPRect * updaterect,
			bool hda, tTVPBBStretchType type, bool clear, tjs_uint32 clearcolor)
{
	// do transformation using 2D affine matrix.
	//  x' =  ax + cy + tx
	//  y' =  bx + dy + ty
	tTVPPointD points[3];
	int rp = refrect.get_width();
	int bp = refrect.get_height();

	// note that a pixel has actually 1 x 1 size, so
	// a pixel at (0,0) covers (-0.5, -0.5) - (0.5, 0.5).

#define CALC_X(x, y) (matrix.a * (x) + matrix.c * (y) + matrix.tx)
#define CALC_Y(x, y) (matrix.b * (x) + matrix.d * (y) + matrix.ty)

	// - upper-left
	points[0].x = CALC_X(-0.5, -0.5);
	points[0].y = CALC_Y(-0.5, -0.5);

	// - upper-right
	points[1].x = CALC_X(rp - 0.5, -0.5);
	points[1].y = CALC_Y(rp - 0.5, -0.5);

/*	// - bottom-right
	points[2].x = CALC_X(rp - 0.5, bp - 0.5);
	points[2].y = CALC_Y(rp - 0.5, bp - 0.5);
*/

	// - bottom-left
	points[2].x = CALC_X(-0.5, bp - 0.5);
	points[2].y = CALC_Y(-0.5, bp - 0.5);

	return AffineBlt(destrect, ref, refrect, points, method, opa, updaterect, hda, type, clear, clearcolor);
}
//---------------------------------------------------------------------------
// some blur operation template functions to select algorithm by base integer type
template <typename tARGB, typename base_int_type>
void TVPAddSubVertSum(base_int_type *dest, const tjs_uint32 *addline,
	const tjs_uint32 *subline, tjs_int len)
{
}
template <>
void TVPAddSubVertSum<tTVPARGB<tjs_uint16>, tjs_uint16 >(tjs_uint16 *dest,
	const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	TVPAddSubVertSum16(dest, addline, subline, len);
}
template <>
void TVPAddSubVertSum<tTVPARGB_AA<tjs_uint16>, tjs_uint16 >(tjs_uint16 *dest,
	const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	TVPAddSubVertSum16_d(dest, addline, subline, len);
}
template <>
void TVPAddSubVertSum<tTVPARGB<tjs_uint32>, tjs_uint32 >(tjs_uint32 *dest,
	const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	TVPAddSubVertSum32(dest, addline, subline, len);
}
template <>
void TVPAddSubVertSum<tTVPARGB_AA<tjs_uint32>, tjs_uint32 >(tjs_uint32 *dest,
	const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	TVPAddSubVertSum32_d(dest, addline, subline, len);
}

template <typename tARGB, typename base_int_type>
void TVPDoBoxBlurAvg(tjs_uint32 *dest, base_int_type *sum,
	const base_int_type * add, const base_int_type * sub,
		tjs_int n, tjs_int len)
{
}
template <>
void TVPDoBoxBlurAvg<tTVPARGB<tjs_uint16>, tjs_uint16 >(tjs_uint32 *dest, tjs_uint16 *sum,
	const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)
{
	TVPDoBoxBlurAvg16(dest, sum, add, sub, n, len);
}
template <>
void TVPDoBoxBlurAvg<tTVPARGB_AA<tjs_uint16>, tjs_uint16  >(tjs_uint32 *dest, tjs_uint16 *sum,
	const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)
{
	TVPDoBoxBlurAvg16_d(dest, sum, add, sub, n, len);
}
template <>
void TVPDoBoxBlurAvg<tTVPARGB<tjs_uint32>, tjs_uint32  >(tjs_uint32 *dest, tjs_uint32 *sum,
	const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len)
{
	TVPDoBoxBlurAvg32(dest, sum, add, sub, n, len);
}
template <>
void TVPDoBoxBlurAvg<tTVPARGB_AA<tjs_uint32>, tjs_uint32  >(tjs_uint32 *dest, tjs_uint32 *sum,
	const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len)
{
	TVPDoBoxBlurAvg32_d(dest, sum, add, sub, n, len);
}


//---------------------------------------------------------------------------


template <typename tARGB>
void tTVPBaseBitmap::DoBoxBlurLoop(const tTVPRect &rect, const tTVPRect & area)
{
	// Box-Blur template function used by tTVPBaseBitmap::DoBoxBlur family.
	// Based on contributed blur code by yun, say thanks to him.

	typedef tARGB::base_int_type base_type;

	tjs_int width = GetWidth();
	tjs_int height = GetHeight();

	tjs_int dest_buf_size = area.top <= 0 ? (1-area.top) : 0;

	tjs_int vert_sum_left_limit = rect.left + area.left;
	if(vert_sum_left_limit < 0) vert_sum_left_limit = 0;
	tjs_int vert_sum_right_limit = (rect.right-1) + area.right;
	if(vert_sum_right_limit >= width) vert_sum_right_limit = width - 1;


	tARGB * vert_sum = NULL; // vertical sum of the pixel
	tjs_uint32 * * dest_buf = NULL; // destination pixel temporary buffer

	tjs_int vert_sum_count;

	try
	{
		// allocate buffers
		vert_sum = (tARGB*)TJSAlignedAlloc(sizeof(tARGB) *
			(vert_sum_right_limit - vert_sum_left_limit + 1 + 1), 4); // use 128bit aligned allocation

		if(dest_buf_size)
		{
			dest_buf = new tjs_uint32 * [dest_buf_size];
			for(tjs_int i = 0; i < dest_buf_size; i++)
				dest_buf[i] = new tjs_uint32[rect.right - rect.left];
		}

		// initialize vert_sum
		{
			for(tjs_int i = vert_sum_right_limit - vert_sum_left_limit + 1 -1; i>=0; i--)
				vert_sum[i].Zero();

			tjs_int v_init_start = rect.top + area.top;
			if(v_init_start < 0) v_init_start = 0;
			tjs_int v_init_end = rect.top + area.bottom;
			if(v_init_end >= height) v_init_end = height - 1;
			vert_sum_count = v_init_end - v_init_start + 1;
			for(tjs_int y = v_init_start; y <= v_init_end; y++)
			{
				const tjs_uint32 * add_line;
				add_line = (const tjs_uint32*)GetScanLine(y);
				tARGB * vs = vert_sum;
				for(int x = vert_sum_left_limit; x <= vert_sum_right_limit; x++)
					*(vs++) += add_line[x];
			}
		}

		// prepare variables to be used in following loop
		tjs_int h_init_start = rect.left + area.left; // this always be the same value as vert_sum_left_limit
		if(h_init_start < 0) h_init_start = 0;
		tjs_int h_init_end = rect.left + area.right;
		if(h_init_end >= width) h_init_end = width - 1;

		tjs_int left_frac_len =
			rect.left + area.left < 0 ? -(rect.left + area.left) : 0;
		tjs_int right_frac_len =
			rect.right + area.right >= width ? rect.right + area.right - width + 1: 0;
		tjs_int center_len = rect.right - rect.left - left_frac_len - right_frac_len;

		if(center_len < 0)
		{
			left_frac_len = rect.right - rect.left;
			right_frac_len = 0;
			center_len = 0;
		}
		tjs_int left_frac_lim = rect.left + left_frac_len;
		tjs_int center_lim = rect.left + left_frac_len + center_len;

		// for each line
		tjs_int dest_buf_free = dest_buf_size;
		tjs_int dest_buf_wp = 0;

		for(tjs_int y = rect.top; y < rect.bottom; y++)
		{
			// rotate dest_buf
			if(dest_buf_free == 0)
			{
				// dest_buf is full;
				// write last dest_buf back to the bitmap
				memcpy(
					rect.left + ((tjs_uint32*)GetScanLineForWrite(y - dest_buf_size)),
					dest_buf[dest_buf_wp],
					(rect.right - rect.left) * sizeof(tjs_uint32));
			}
			else
			{
				dest_buf_free --;
			}

			// build initial sum
			tARGB sum;
			sum.Zero();
			tjs_int horz_sum_count = h_init_end - h_init_start + 1;

			for(tjs_int x = h_init_start; x <= h_init_end; x++)
				sum += vert_sum[x - vert_sum_left_limit];

			// process a line
			tjs_uint32 *dp = dest_buf[dest_buf_wp];
			tjs_int x = rect.left;

			//- do left fraction part
			for(; x < left_frac_lim; x++)
			{
				tARGB tmp = sum;
				tmp.average(horz_sum_count * vert_sum_count);

				*(dp++) = tmp;

				// update sum
				if(x + area.left >= 0)
				{
					sum -= vert_sum[x + area.left - vert_sum_left_limit];
					horz_sum_count --;
				}
				if(x + area.right + 1 < width)
				{
					sum += vert_sum[x + area.right + 1 - vert_sum_left_limit];
					horz_sum_count ++;
				}
			}

			//- do center part
			if(center_len > 0)
			{
				// uses function in tvpgl
				TVPDoBoxBlurAvg<tARGB>(dp, (base_type*)&sum,
					(const base_type *)(vert_sum + x + area.right + 1 - vert_sum_left_limit),
					(const base_type *)(vert_sum + x + area.left - vert_sum_left_limit),
					horz_sum_count * vert_sum_count,
					center_len);
				dp += center_len;
			}
			x = center_lim;

			//- do right fraction part
			for(; x < rect.right; x++)
			{
				tARGB tmp = sum;
				tmp.average(horz_sum_count * vert_sum_count);

				*(dp++) = tmp;

				// update sum
				if(x + area.left >= 0)
				{
					sum -= vert_sum[x + area.left - vert_sum_left_limit];
					horz_sum_count --;
				}
				if(x + area.right + 1 < width)
				{
					sum += vert_sum[x + area.right + 1 - vert_sum_left_limit];
					horz_sum_count ++;
				}
			}

			// update vert_sum
			if(y != rect.bottom - 1)
			{
				const tjs_uint32 * sub_line;
				const tjs_uint32 * add_line;
				sub_line =
					y + area.top < 0 ?
						(const tjs_uint32 *)NULL :
						(const tjs_uint32 *)GetScanLine(y + area.top);
				add_line =
					y + area.bottom + 1 >= height ?
						(const tjs_uint32 *)NULL :
						(const tjs_uint32 *)GetScanLine(y + area.bottom + 1);

				if(sub_line && add_line)
				{
					// both sub_line and add_line are available
					// uses function in tvpgl
					TVPAddSubVertSum<tARGB>((base_type*)vert_sum,
						add_line + vert_sum_left_limit,
						sub_line + vert_sum_left_limit,
						vert_sum_right_limit - vert_sum_left_limit + 1);

				}
				else if(sub_line)
				{
					// only sub_line is available
					tARGB * vs = vert_sum;
					for(int x = vert_sum_left_limit; x <= vert_sum_right_limit; x++)
						*vs -= sub_line[x], vs ++;
					vert_sum_count --;
				}
				else if(add_line)
				{
					// only add_line is available
					tARGB * vs = vert_sum;
					for(int x = vert_sum_left_limit; x <= vert_sum_right_limit; x++)
						*vs += add_line[x], vs ++;
					vert_sum_count ++;
				}
			}

			// step dest_buf_wp
			dest_buf_wp++;
			if(dest_buf_wp >= dest_buf_size) dest_buf_wp = 0;
		}

		// write remaining dest_buf back to the bitmap
		while(dest_buf_free < dest_buf_size)
		{
			memcpy(
				rect.left +
				(tjs_uint32*)(GetScanLineForWrite(rect.bottom - (dest_buf_size - dest_buf_free))),
				dest_buf[dest_buf_wp], (rect.right - rect.left) * sizeof(tjs_uint32));

			dest_buf_wp++;
			if(dest_buf_wp >= dest_buf_size) dest_buf_wp = 0;
			dest_buf_free++;
		}
	}
	catch(...)
	{
		// exception caught
		if(vert_sum) TJSAlignedDealloc(vert_sum);
		if(dest_buf_size)
		{
			if(dest_buf)
			{
				for(tjs_int i = 0 ; i < dest_buf_size; i++)
					if(dest_buf[i]) delete [] dest_buf[i];
				delete [] dest_buf;
			}
		}
		throw;
	}

	// free buffeers
	TJSAlignedDealloc(vert_sum);
	if(dest_buf_size)
	{
		for(tjs_int i = 0 ; i < dest_buf_size; i++) delete [] dest_buf[i];
		delete [] dest_buf;
	}
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::InternalDoBoxBlur(tTVPRect rect, tTVPRect area, bool hasalpha)
{
	BOUND_CHECK(false);

	if(area.right < area.left)
		std::swap(area.right, area.left);
	if(area.bottom < area.top)
		std::swap(area.bottom, area.top);

	if(area.left == 0 && area.right == 0 &&
		area.top == 0 && area.bottom == 0) return false; // no conversion occurs

	if(area.left > 0 || area.right < 0 || area.top > 0 || area.bottom < 0)
		TVPThrowExceptionMessage(TVPBoxBlurAreaMustContainCenterPixel);


	tjs_uint64 area_size = (tjs_uint64)
		(area.right - area.left + 1) * (area.bottom - area.top + 1);
	if(area_size < 256)
	{
		if(!hasalpha)
			DoBoxBlurLoop<tTVPARGB<tjs_uint16> >(rect, area);
		else
			DoBoxBlurLoop<tTVPARGB_AA<tjs_uint16> >(rect, area);
	}
	else if(area_size < (1L<<24))
	{
		if(!hasalpha)
			DoBoxBlurLoop<tTVPARGB<tjs_uint32> >(rect, area);
		else
			DoBoxBlurLoop<tTVPARGB_AA<tjs_uint32> >(rect, area);
	}
	else
		TVPThrowExceptionMessage(TVPBoxBlurAreaMustBeSmallerThan16Million);

	return true;
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::DoBoxBlur(const tTVPRect & rect, const tTVPRect & area)
{
	// Blur the bitmap with box-blur algorithm.
	// 'rect' is a rectangle to blur.
	// 'area' is an area which destination pixel refers.
	// right and bottom of 'area' *does contain* pixels in the boundary.
	// eg. area:(-1,-1,1,1)  : Blur is to be performed using average of 3x3
	//                          pixels around the destination pixel.
	//     area:(-10,0,10,0) : Blur is to be performed using average of 21x1
	//                          pixels around the destination pixel. This results
	//                          horizontal blur.

	return InternalDoBoxBlur(rect, area, false);
}
//---------------------------------------------------------------------------
bool tTVPBaseBitmap::DoBoxBlurForAlpha(const tTVPRect & rect, const tTVPRect &area)
{
	return InternalDoBoxBlur(rect, area, true);
}
//---------------------------------------------------------------------------
void tTVPBaseBitmap::UDFlip(const tTVPRect &rect)
{
	// up-down flip for given rectangle

	if(rect.left < 0 || rect.top < 0 || rect.right > (tjs_int)GetWidth() ||
		rect.bottom > (tjs_int)GetHeight())
				TVPThrowExceptionMessage(TVPSrcRectOutOfBitmap);

	tjs_int h = (rect.bottom - rect.top) /2;
	tjs_int w = rect.right - rect.left;
	tjs_int pitch = GetPitchBytes();
	tjs_uint8 * l1 = (tjs_uint8*)GetScanLineForWrite(rect.top);
	tjs_uint8 * l2 = (tjs_uint8*)GetScanLineForWrite(rect.bottom - 1);


	if(Is32BPP())
	{
		// 32bpp
		l1 += rect.left * sizeof(tjs_uint32);
		l2 += rect.left * sizeof(tjs_uint32);
		while(h--)
		{
			TVPSwapLine32((tjs_uint32*)l1, (tjs_uint32*)l2, w);
			l1 += pitch;
			l2 -= pitch;
		}
	}
	else
	{
		// 8bpp
		l1 += rect.left;
		l2 += rect.left;
		while(h--)
		{
			TVPSwapLine8(l1, l2, w);
			l1 += pitch;
			l2 -= pitch;
		}
	}
}
//---------------------------------------------------------------------------
void tTVPBaseBitmap::LRFlip(const tTVPRect &rect)
{
	// left-right flip
	if(rect.left < 0 || rect.top < 0 || rect.right > (tjs_int)GetWidth() ||
		rect.bottom > (tjs_int)GetHeight())
				TVPThrowExceptionMessage(TVPSrcRectOutOfBitmap);

	tjs_int h = rect.bottom - rect.top;
	tjs_int w = rect.right - rect.left;

	tjs_int pitch = GetPitchBytes();
	tjs_uint8 * line = (tjs_uint8*)GetScanLineForWrite(rect.top);

	if(Is32BPP())
	{
		// 32bpp
		line += rect.left * sizeof(tjs_uint32);
		while(h--)
		{
			TVPReverse32((tjs_uint32*)line, w);
			line += pitch;
		}
	}
	else
	{
		// 8bpp
		line += rect.left;
		while(h--)
		{
			TVPReverse8(line, w);
			line += pitch;
		}
	}
}
//---------------------------------------------------------------------------
void tTVPBaseBitmap::DoGrayScale(tTVPRect rect)
{
	if(!Is32BPP()) return;  // 8bpp is always grayscaled bitmap

	BOUND_CHECK(RET_VOID);

	tjs_int h = rect.bottom - rect.top;
	tjs_int w = rect.right - rect.left;

	tjs_int pitch = GetPitchBytes();
	tjs_uint8 * line = (tjs_uint8*)GetScanLineForWrite(rect.top);


	line += rect.left * sizeof(tjs_uint32);
	while(h--)
	{
		TVPDoGrayScale((tjs_uint32*)line, w);
		line += pitch;
	}
}
//---------------------------------------------------------------------------
void tTVPBaseBitmap::AdjustGamma(tTVPRect rect, const tTVPGLGammaAdjustData & data)
{
	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	BOUND_CHECK(RET_VOID);

	if(!memcmp(&data, &TVPIntactGammaAdjustData, sizeof(tTVPGLGammaAdjustData)))
		return;

	tTVPGLGammaAdjustTempData temp;
	TVPInitGammaAdjustTempData(&temp, &data);

	try
	{
		tjs_int h = rect.bottom - rect.top;
		tjs_int w = rect.right - rect.left;

		tjs_int pitch = GetPitchBytes();
		tjs_uint8 * line = (tjs_uint8*)GetScanLineForWrite(rect.top);


		line += rect.left * sizeof(tjs_uint32);
		while(h--)
		{
			TVPAdjustGamma((tjs_uint32*)line, w, &temp);
			line += pitch;
		}

	}
	catch(...)
	{
		TVPUninitGammaAdjustTempData(&temp);
		throw;
	}

	TVPUninitGammaAdjustTempData(&temp);
}
//---------------------------------------------------------------------------
void tTVPBaseBitmap::AdjustGammaForAdditiveAlpha(tTVPRect rect, const tTVPGLGammaAdjustData & data)
{
	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	BOUND_CHECK(RET_VOID);

	if(!memcmp(&data, &TVPIntactGammaAdjustData, sizeof(tTVPGLGammaAdjustData)))
		return;

	tTVPGLGammaAdjustTempData temp;
	TVPInitGammaAdjustTempData(&temp, &data);

	try
	{
		tjs_int h = rect.bottom - rect.top;
		tjs_int w = rect.right - rect.left;

		tjs_int pitch = GetPitchBytes();
		tjs_uint8 * line = (tjs_uint8*)GetScanLineForWrite(rect.top);


		line += rect.left * sizeof(tjs_uint32);
		while(h--)
		{
			TVPAdjustGamma_a((tjs_uint32*)line, w, &temp);
			line += pitch;
		}

	}
	catch(...)
	{
		TVPUninitGammaAdjustTempData(&temp);
		throw;
	}

	TVPUninitGammaAdjustTempData(&temp);
}
//---------------------------------------------------------------------------
void tTVPBaseBitmap::ConvertAddAlphaToAlpha()
{
	// convert additive alpha representation to alpha representation

	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	tjs_int w = GetWidth();
	tjs_int h = GetHeight();
	tjs_int pitch = GetPitchBytes();
	tjs_uint8 * line = (tjs_uint8*)GetScanLineForWrite(0);

	while(h--)
	{
		TVPConvertAdditiveAlphaToAlpha((tjs_uint32*)line, w);
		line += pitch;
	}
}
//---------------------------------------------------------------------------
void tTVPBaseBitmap::ConvertAlphaToAddAlpha()
{
	// convert additive alpha representation to alpha representation

	if(!Is32BPP()) TVPThrowExceptionMessage(TVPInvalidOperationFor8BPP);

	tjs_int w = GetWidth();
	tjs_int h = GetHeight();
	tjs_int pitch = GetPitchBytes();
	tjs_uint8 * line = (tjs_uint8*)GetScanLineForWrite(0);

	while(h--)
	{
		TVPConvertAlphaToAdditiveAlpha((tjs_uint32*)line, w);
		line += pitch;
	}
}
//---------------------------------------------------------------------------



