/* Copyright (C) 2019 Nunuhara Cabbage <nunuhara@haniwa.technology>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://gnu.org/licenses/>.
 */

#include "hll.h"
#include "sact.h"
#include "gfx/types.h"
#include "system4/string.h"

#define TEX(sp_no) sact_get_texture(sp_no)

void DrawGraph_Copy(int dst, int dx, int dy, int src, int sx, int sy, int w, int h)
{
	gfx_copy(TEX(dst), dx, dy, TEX(src), sx, sy, w, h);
}

void DrawGraph_CopyBright(int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int rate)
{
	gfx_copy_bright(TEX(dst), dx, dy, TEX(src), sx, sy, w, h, rate);
}

void DrawGraph_CopyAMap(int dst, int dx, int dy, int src, int sx, int sy, int w, int h)
{
	gfx_copy_amap(TEX(dst), dx, dy, TEX(src), sx, sy, w, h);
}

void DrawGraph_CopySprite(int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int r, int g, int b)
{
	gfx_copy_sprite(TEX(dst), dx, dy, TEX(src), sx, sy, w, h, COLOR(r, g, b, 0));
}

void DrawGraph_CopyUseAMapUnder(int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int alpha)
{
	gfx_copy_use_amap_under(TEX(dst), dx, dy, TEX(src), sx, sy, w, h, alpha);
}

void DrawGraph_CopyUseAMapBorder(int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int alpha)
{
	gfx_copy_use_amap_border(TEX(dst), dx, dy, TEX(src), sx, sy, w, h, alpha);
}

void DrawGraph_CopyAMapMax(int dst, int dx, int dy, int src, int sx, int sy, int w, int h)
{
	gfx_copy_amap_max(TEX(dst), dx, dy, TEX(src), sx, sy, w, h);
}

void DrawGraph_CopyAMapMin(int dst, int dx, int dy, int src, int sx, int sy, int w, int h)
{
	gfx_copy_amap_min(TEX(dst), dx, dy, TEX(src), sx, sy, w, h);
}

void DrawGraph_BlendAMap(int dst, int dx, int dy, int src, int sx, int sy, int w, int h)
{
	gfx_blend_amap(TEX(dst), dx, dy, TEX(src), sx, sy, w, h);
}

void DrawGraph_BlendAMapAlpha(int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int alpha)
{
	gfx_blend_amap_alpha(TEX(dst), dx, dy, TEX(src), sx, sy, w, h, alpha);
}

void DrawGraph_Fill(int dst, int x, int y, int w, int h, int r, int g, int b)
{
	gfx_fill(TEX(dst), x, y, w, h, r, g, b);
}

void DrawGraph_FillAlphaColor(int dst, int x, int y, int w, int h, int r, int g, int b, int rate)
{
	gfx_fill_alpha_color(TEX(dst), x, y, w, h, r, g, b, rate);
}

void DrawGraph_FillAMap(int dst, int x, int y, int w, int h, int alpha)
{
	gfx_fill_amap(TEX(dst), x, y, w, h, alpha);
}

void DrawGraph_AddDA_DAxSA(int dst, int dx, int dy, int src, int sx, int sy, int w, int h)
{
	gfx_add_da_daxsa(TEX(dst), dx, dy, TEX(src), sx, sy, w, h);
}

void DrawGraph_CopyStretch(int dst, int dx, int dy, int dw, int dh, int src, int sx, int sy, int sw, int sh)
{
	gfx_copy_stretch(TEX(dst), dx, dy, dw, dh, TEX(src), sx, sy, sw, sh);
}

void DrawGraph_CopyStretchAMap(int dst, int dx, int dy, int dw, int dh, int src, int sx, int sy, int sw, int sh)
{
	gfx_copy_stretch_amap(TEX(dst), dx, dy, dw, dh, TEX(src), sx, sy, sw, sh);
}

void DrawGraph_DrawTextToPMap(int dst, int x, int y, struct string *s)
{
	gfx_draw_text_to_pmap(TEX(dst), x, y, s->text);
}

void DrawGraph_DrawTextToAMap(int dst, int x, int y, struct string *s)
{
	gfx_draw_text_to_amap(TEX(dst), x, y, s->text);
}

void DrawGraph_SetFontColor(int r, int g, int b)
{
	gfx_set_font_color(COLOR(r, g, b, 255));
}

void DrawGraph_GetFontColor(int *r, int *g, int *b)
{
	SDL_Color c = gfx_get_font_color();
	*r = c.r;
	*g = c.g;
	*b = c.b;
}

void DrawGraph_CopyRotZoom(int dst, int src, int sx, int sy, int w, int h, float rotate, float mag)
{
	gfx_copy_rot_zoom(TEX(dst), TEX(src), sx, sy, w, h, rotate, mag);
}

void DrawGraph_CopyRotZoomAMap(int dst, int src, int sx, int sy, int w, int h, float rotate, float mag)
{
	gfx_copy_rot_zoom_amap(TEX(dst), TEX(src), sx, sy, w, h, rotate, mag);
}

void DrawGraph_CopyRotZoomUseAMap(int dst, int src, int sx, int sy, int w, int h, float rotate, float mag)
{
	gfx_copy_rot_zoom_use_amap(TEX(dst), TEX(src), sx, sy, w, h, rotate, mag);
}

void DrawGraph_CopyReverseLR(int dst, int dx, int dy, int src, int sx, int sy, int w, int h)
{
	gfx_copy_reverse_LR(TEX(dst), dx, dy, TEX(src), sx, sy, w, h);
}

void DrawGraph_CopyReverseAMapLR(int dst, int dx, int dy, int src, int sx, int sy, int w, int h)
{
	gfx_copy_reverse_amap_LR(TEX(dst), dx, dy, TEX(src), sx, sy, w, h);
}

bool DrawGraph_GetAlphaColor(int surface, int x, int y, int *a)
{
	*a = sact_SP_GetAMapValue(surface, x, y);
	return true;
}

HLL_UNIMPLEMENTED(void, DrawGraph, CopyColorReverse, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, Blend, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int alpha);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendSrcBright, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int alpha, int rate);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendAddSatur, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendAMapSrcOnly, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendAMapColor, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int r, int g, int b);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendAMapColorAlpha, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int r, int g, int b, int a);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendAMapBright, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int rate);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendAMapAlphaSrcBright, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int alpha, int rate);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendUseAMapColor, int dst, int dx, int dy, int alpha, int ax, int ay, int w, int h, int r, int g, int b, int rate);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendScreen, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendMultiply, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendScreenAlpha, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int alpha);
HLL_UNIMPLEMENTED(void, DrawGraph, FillAMapOverBorder, int dst, int x, int y, int w, int h, int alpha, int border);
HLL_UNIMPLEMENTED(void, DrawGraph, FillAMapUnderBorder, int dst, int x, int y, int w, int h, int alpha, int border);
HLL_UNIMPLEMENTED(void, DrawGraph, FillAMapGradationUD, int dst, int x, int y, int w, int h, int up_a, int down_a);
HLL_UNIMPLEMENTED(void, DrawGraph, FillScreen, int dst, int x, int y, int w, int h, int r, int g, int b);
HLL_UNIMPLEMENTED(void, DrawGraph, FillMultiply, int dst, int x, int y, int w, int h, int r, int g, int b);
HLL_UNIMPLEMENTED(void, DrawGraph, SaturDP_DPxSA, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, ScreenDA_DAxSA, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, SpriteCopyAMap, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int key);
HLL_UNIMPLEMENTED(void, DrawGraph, BlendDA_DAxSA, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, SubDA_DAxSA, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, BrightDestOnly, int dst, int x, int y, int w, int h, int rate);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyTextureWrap, int dst, int dx, int dy, int dw, int dh, int src, int sx, int sy, int sw, int sh, int u, int v);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyTextureWrapAlpha, int dst, int dx, int dy, int dw, int dh, int src, int sx, int sy, int sw, int sh, int u, int v, int alpha);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyStretchBlend, int dst, int dx, int dy, int dw, int dh, int src, int sx, int sy, int sw, int sh, int alpha);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyStretchBlendAMap, int dst, int dx, int dy, int dw, int dh, int src, int sx, int sy, int sw, int sh);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyStretchInterp, int dst, int dx, int dy, int dw, int dh, int src, int sx, int sy, int sw, int sh);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyStretchAMapInterp, int dst, int dx, int dy, int dw, int dh, int src, int sx, int sy, int sw, int sh);
HLL_WARN_UNIMPLEMENTED( , void, DrawGraph, SetFontName, struct string *text);
HLL_WARN_UNIMPLEMENTED(&EMPTY_STRING, struct string*, DrawGraph, GetFontName, void);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotZoom2Bilinear, int dst, float cx, float cy, int src, float scx, float scy, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateY, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateYUseAMap, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateYFixL, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateYFixR, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateYFixLUseAMap, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateYFixRUseAMap, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateX, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateXUseAMap, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateXFixU, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateXFixD, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateXFixUUseAMap, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyRotateXFixDUseAMap, int write, int dst, int src, int sx, int sy, int w, int h, float rot, float mag);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyReverseUD, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyReverseAMapUD, int dst, int dx, int dy, int src, int sx, int sy, int w, int h);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyWidthBlur, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int blur);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyHeightBlur, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int blur);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyAMapWidthBlur, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int blur);
HLL_UNIMPLEMENTED(void, DrawGraph, CopyAMapHeightBlur, int dst, int dx, int dy, int src, int sx, int sy, int w, int h, int blur);
HLL_UNIMPLEMENTED(void, DrawGraph, DrawLine, int dst, int x0, int y0, int x1, int y1, int r, int g, int b);
HLL_UNIMPLEMENTED(void, DrawGraph, DrawLineToAMap, int dst, int x0, int y0, int x1, int y1, int alpha);
HLL_UNIMPLEMENTED(void, DrawGraph, DrawPolygon, int dst, int tex, float x0, float y0, float z0, float u0, float v0, float x1, float y1, float z1, float u1, float v1, float x2, float y2, float z2, float u2, float v2);
HLL_UNIMPLEMENTED(void, DrawGraph, DrawColorPolygon, int dst, int tex, float x0, float y0, float z0, int r0, int g0, int b0, int a0, float x1, float y1, float z1, int r1, int g1, int b1, int a1, float x2, float y2, float z2, int r2, int g2, int b2, int a2);

HLL_LIBRARY(DrawGraph,
	    HLL_EXPORT(Copy, DrawGraph_Copy),
	    HLL_EXPORT(CopyBright, DrawGraph_CopyBright),
	    HLL_EXPORT(CopyAMap, DrawGraph_CopyAMap),
	    HLL_EXPORT(CopySprite, DrawGraph_CopySprite),
	    HLL_EXPORT(CopyColorReverse, DrawGraph_CopyColorReverse),
	    HLL_EXPORT(CopyUseAMapUnder, DrawGraph_CopyUseAMapUnder),
	    HLL_EXPORT(CopyUseAMapBorder, DrawGraph_CopyUseAMapBorder),
	    HLL_EXPORT(CopyAMapMax, DrawGraph_CopyAMapMax),
	    HLL_EXPORT(CopyAMapMin, DrawGraph_CopyAMapMin),
	    HLL_EXPORT(Blend, DrawGraph_Blend),
	    HLL_EXPORT(BlendSrcBright, DrawGraph_BlendSrcBright),
	    HLL_EXPORT(BlendAddSatur, DrawGraph_BlendAddSatur),
	    HLL_EXPORT(BlendAMap, DrawGraph_BlendAMap),
	    HLL_EXPORT(BlendAMapSrcOnly, DrawGraph_BlendAMapSrcOnly),
	    HLL_EXPORT(BlendAMapColor, DrawGraph_BlendAMapColor),
	    HLL_EXPORT(BlendAMapColorAlpha, DrawGraph_BlendAMapColorAlpha),
	    HLL_EXPORT(BlendAMapAlpha, DrawGraph_BlendAMapAlpha),
	    HLL_EXPORT(BlendAMapBright, DrawGraph_BlendAMapBright),
	    HLL_EXPORT(BlendAMapAlphaSrcBright, DrawGraph_BlendAMapAlphaSrcBright),
	    HLL_EXPORT(BlendUseAMapColor, DrawGraph_BlendUseAMapColor),
	    HLL_EXPORT(BlendScreen, DrawGraph_BlendScreen),
	    HLL_EXPORT(BlendMultiply, DrawGraph_BlendMultiply),
	    HLL_EXPORT(BlendScreenAlpha, DrawGraph_BlendScreenAlpha),
	    HLL_EXPORT(Fill, DrawGraph_Fill),
	    HLL_EXPORT(FillAlphaColor, DrawGraph_FillAlphaColor),
	    HLL_EXPORT(FillAMap, DrawGraph_FillAMap),
	    HLL_EXPORT(FillAMapOverBorder, DrawGraph_FillAMapOverBorder),
	    HLL_EXPORT(FillAMapUnderBorder, DrawGraph_FillAMapUnderBorder),
	    HLL_EXPORT(FillAMapGradationUD, DrawGraph_FillAMapGradationUD),
	    HLL_EXPORT(FillScreen, DrawGraph_FillScreen),
	    HLL_EXPORT(FillMultiply, DrawGraph_FillMultiply),
	    HLL_EXPORT(SaturDP_DPxSA, DrawGraph_SaturDP_DPxSA),
	    HLL_EXPORT(ScreenDA_DAxSA, DrawGraph_ScreenDA_DAxSA),
	    HLL_EXPORT(AddDA_DAxSA, DrawGraph_AddDA_DAxSA),
	    HLL_EXPORT(SpriteCopyAMap, DrawGraph_SpriteCopyAMap),
	    HLL_EXPORT(BlendDA_DAxSA, DrawGraph_BlendDA_DAxSA),
	    HLL_EXPORT(SubDA_DAxSA, DrawGraph_SubDA_DAxSA),
	    HLL_EXPORT(BrightDestOnly, DrawGraph_BrightDestOnly),
	    HLL_EXPORT(CopyTextureWrap, DrawGraph_CopyTextureWrap),
	    HLL_EXPORT(CopyTextureWrapAlpha, DrawGraph_CopyTextureWrapAlpha),
	    HLL_EXPORT(CopyStretch, DrawGraph_CopyStretch),
	    HLL_EXPORT(CopyStretchBlend, DrawGraph_CopyStretchBlend),
	    HLL_EXPORT(CopyStretchBlendAMap, DrawGraph_CopyStretchBlendAMap),
	    HLL_EXPORT(CopyStretchAMap, DrawGraph_CopyStretchAMap),
	    HLL_EXPORT(CopyStretchInterp, DrawGraph_CopyStretchInterp),
	    HLL_EXPORT(CopyStretchAMapInterp, DrawGraph_CopyStretchAMapInterp),
	    HLL_EXPORT(CopyReduce, DrawGraph_CopyStretch),
	    HLL_EXPORT(CopyReduceAMap, DrawGraph_CopyStretchAMap),
	    HLL_EXPORT(DrawTextToPMap, DrawGraph_DrawTextToPMap),
	    HLL_EXPORT(DrawTextToAMap, DrawGraph_DrawTextToAMap),
	    HLL_EXPORT(SetFontSize, gfx_set_font_size),
	    HLL_EXPORT(SetFontName, DrawGraph_SetFontName),
	    HLL_EXPORT(SetFontWeight, gfx_set_font_weight),
	    HLL_EXPORT(SetFontUnderline, gfx_set_font_underline),
	    HLL_EXPORT(SetFontStrikeOut, gfx_set_font_strikeout),
	    HLL_EXPORT(SetFontSpace, gfx_set_font_space),
	    HLL_EXPORT(SetFontColor, DrawGraph_SetFontColor),
	    HLL_EXPORT(GetFontSize, gfx_get_font_size),
	    HLL_EXPORT(GetFontName, DrawGraph_GetFontName),
	    HLL_EXPORT(GetFontWeight, gfx_get_font_weight),
	    HLL_EXPORT(GetFontUnderline, gfx_get_font_underline),
	    HLL_EXPORT(GetFontStrikeOut, gfx_get_font_strikeout),
	    HLL_EXPORT(GetFontSpace, gfx_get_font_space),
	    HLL_EXPORT(GetFontColor, DrawGraph_GetFontColor),
	    HLL_EXPORT(CopyRotZoom, DrawGraph_CopyRotZoom),
	    HLL_EXPORT(CopyRotZoomAMap, DrawGraph_CopyRotZoomAMap),
	    HLL_EXPORT(CopyRotZoomUseAMap, DrawGraph_CopyRotZoomUseAMap),
	    HLL_EXPORT(CopyRotZoom2Bilinear, DrawGraph_CopyRotZoom2Bilinear),
	    HLL_EXPORT(CopyRotateY, DrawGraph_CopyRotateY),
	    HLL_EXPORT(CopyRotateYUseAMap, DrawGraph_CopyRotateYUseAMap),
	    HLL_EXPORT(CopyRotateYFixL, DrawGraph_CopyRotateYFixL),
	    HLL_EXPORT(CopyRotateYFixR, DrawGraph_CopyRotateYFixR),
	    HLL_EXPORT(CopyRotateYFixLUseAMap, DrawGraph_CopyRotateYFixLUseAMap),
	    HLL_EXPORT(CopyRotateYFixRUseAMap, DrawGraph_CopyRotateYFixRUseAMap),
	    HLL_EXPORT(CopyRotateX, DrawGraph_CopyRotateX),
	    HLL_EXPORT(CopyRotateXUseAMap, DrawGraph_CopyRotateXUseAMap),
	    HLL_EXPORT(CopyRotateXFixU, DrawGraph_CopyRotateXFixU),
	    HLL_EXPORT(CopyRotateXFixD, DrawGraph_CopyRotateXFixD),
	    HLL_EXPORT(CopyRotateXFixUUseAMap, DrawGraph_CopyRotateXFixUUseAMap),
	    HLL_EXPORT(CopyRotateXFixDUseAMap, DrawGraph_CopyRotateXFixDUseAMap),
	    HLL_EXPORT(CopyReverseLR, DrawGraph_CopyReverseLR),
	    HLL_EXPORT(CopyReverseUD, DrawGraph_CopyReverseUD),
	    HLL_EXPORT(CopyReverseAMapLR, DrawGraph_CopyReverseAMapLR),
	    HLL_EXPORT(CopyReverseAMapUD, DrawGraph_CopyReverseAMapUD),
	    HLL_EXPORT(CopyWidthBlur, DrawGraph_CopyWidthBlur),
	    HLL_EXPORT(CopyHeightBlur, DrawGraph_CopyHeightBlur),
	    HLL_EXPORT(CopyAMapWidthBlur, DrawGraph_CopyAMapWidthBlur),
	    HLL_EXPORT(CopyAMapHeightBlur, DrawGraph_CopyAMapHeightBlur),
	    HLL_EXPORT(DrawLine, DrawGraph_DrawLine),
	    HLL_EXPORT(DrawLineToAMap, DrawGraph_DrawLineToAMap),
	    HLL_EXPORT(GetPixelColor, sact_SP_GetPixelValue),
	    HLL_EXPORT(GetAlphaColor, DrawGraph_GetAlphaColor),
	    HLL_EXPORT(DrawPolygon, DrawGraph_DrawPolygon),
	    HLL_EXPORT(DrawColorPolygon, DrawGraph_DrawColorPolygon));
