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

#include <SDL.h>
#include <SDL_ttf.h>
#include "system4.h"
#include "gfx/gfx.h"
#include "gfx/types.h"
#include "system4/utfsjis.h"

/*
 * NOTE: There are two different text rendering APIs: SACT2 and DrawGraph.
 *
 *       SACT2 is a stateless API which uses the text_metrics structure.
 *       It is implemented via the single function gfx_render_text and
 *       supports bold and outlined font styles.
 *
 *       DrawGraph is a stateful API that uses the font_metrics structure.
 *       The various gfx_setfont_* and gfx_draw_text* functions implement
 *       this API. It supports bold, underline and strikethrough font
 *       styles and two different rendering modes (pixel map and alpha map)
 */

#define MAX_FONT_SIZE 128

// TODO: install fonts on system, and provide run-time configuration option
const char *font_paths[] = {
	[FONT_GOTHIC] = "fonts/MTLc3m.ttf",
	[FONT_MINCHO] = "fonts/mincho.otf"
};

struct font {
	unsigned int size;
	enum font_face face;
	int style;
	TTF_Font *font;
};

struct _font {
	unsigned int allocated;
	struct font *fonts;
};

static struct _font font_table[2];

static struct font *font;

// current font state
static struct font_metrics font_metrics = {
	.size = 16,
	.face = FONT_GOTHIC,
	.weight = FW_BOLD,
	.underline = false,
	.strikeout = false,
	.space = 0,
	.color = {
		.r = 255,
		.g = 255,
		.b = 255,
		.a = 255
	}
};

bool gfx_set_font(enum font_face face, unsigned int size)
{
	struct _font *ff = &font_table[face];

	if (size > MAX_FONT_SIZE) {
		WARNING("Font size too large: %d", size);
		return false;
	}

	// realloc if needed
	if ((size+1) > ff->allocated) {
		ff->fonts = xrealloc(ff->fonts, sizeof(struct font) * (size+1));
		memset(ff->fonts + ff->allocated, 0, sizeof(struct font) * ((size+1) - ff->allocated));
		ff->allocated = size+1;
	}

	// open font if needed
	if (!ff->fonts[size].size) {
		if (!(ff->fonts[size].font = TTF_OpenFont(font_paths[face], size))) {
			WARNING("Error opening font: %s:%d", font_paths[face], size);
			return false;
		}
		ff->fonts[size].size = size;
		ff->fonts[size].face = face;
		ff->fonts[size].style = TTF_STYLE_NORMAL;
	}

	// set current font
	font = &ff->fonts[size];
	return true;
}

static void set_font_style(int style)
{
	if (font->style != style) {
		TTF_SetFontStyle(font->font, style);
		font->style = style;
	}
}

static int get_font_style(void)
{
	int style = 0;
	if (font_metrics.weight == FW_BOLD || font_metrics.weight == FW_BOLD2)
		style |= TTF_STYLE_BOLD;
	if (font_metrics.underline)
		style |= TTF_STYLE_UNDERLINE;
	if (font_metrics.strikeout)
		style |= TTF_STYLE_STRIKETHROUGH;
	return style ? style : TTF_STYLE_NORMAL;
}

bool gfx_set_font_size(unsigned int size)
{
	font_metrics.size = size;
	return gfx_set_font(font_metrics.face, size);
}

int gfx_get_font_size(void)
{
	return font_metrics.size;
}

bool gfx_set_font_face(enum font_face face)
{
	font_metrics.face = face;
	return gfx_set_font(face, font_metrics.size);
}

enum font_face gfx_get_font_face(void)
{
	return font_metrics.face;
}

bool gfx_set_font_weight(enum font_weight weight)
{
	font_metrics.weight = weight;
	return true;
}

enum font_weight gfx_get_font_weight(void)
{
	return font_metrics.weight;
}

bool gfx_set_font_underline(bool on)
{
	font_metrics.underline = on;
	return true;
}

bool gfx_get_font_underline(void)
{
	return font_metrics.underline;
}

bool gfx_set_font_strikeout(bool on)
{
	font_metrics.strikeout = on;
	return true;
}

bool gfx_get_font_strikeout(void)
{
	return font_metrics.strikeout;
}

bool gfx_set_font_space(int space)
{
	font_metrics.space = space;
	return true;
}

int gfx_get_font_space(void)
{
	return font_metrics.space;
}

bool gfx_set_font_color(SDL_Color color)
{
	font_metrics.color = color;
	return true;
}

SDL_Color gfx_get_font_color(void)
{
	return font_metrics.color;
}

static void get_glyph(TTF_Font *f, Texture *dst, char *msg, SDL_Color color)
{
	SDL_Surface *s = TTF_RenderUTF8_Blended(f, msg, color);
	if (!s) {
		WARNING("Text rendering failed: %s", msg);
		return;
	}
	if (s->format->format != SDL_PIXELFORMAT_BGRA32) {
		WARNING("Wrong pixel format from SDL_ttf");
	}
	gfx_init_texture_with_pixels(dst, s->w, s->h, s->pixels, GL_BGRA);
	SDL_FreeSurface(s);
}

static void render_glyph(Texture *dst, Texture *glyph, Point pos)
{
	GLuint fbo = gfx_set_framebuffer(GL_DRAW_FRAMEBUFFER, dst, pos.x, pos.y, glyph->w, glyph->h);

	GLfloat mw_transform[16] = {
		[0]  = glyph->w,
		[5]  = glyph->h,
		[10] = 1,
		[15] = 1
	};
	GLfloat wv_transform[16] = {
		[0]  =  2.0 / glyph->w,
		[5]  =  2.0 / glyph->h,
		[10] =  2,
		[12] = -1,
		[13] = -1,
		[15] =  1
	};
	struct gfx_render_job job = {
		.texture = glyph->handle,
		.world_transform = mw_transform,
		.view_transform = wv_transform,
		.data = glyph
	};
	gfx_render(&job);

	gfx_reset_framebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

static int sact_to_sdl_fontstyle(int style)
{
	switch (style) {
	case FW_NORMAL:
	case FW_NORMAL2:
		return 0;
	case FW_BOLD:
	case FW_BOLD2:
		return TTF_STYLE_BOLD;
	default:
		WARNING("Unknown fontstyle: %d", style);
		return 0;
	}
}

// NOTE: This is the SACT2 text rendering interface.
// FIXME: If char spacing is set up (via SACT2.SetTextCharSpace), this doesn't do the right
//        thing. Need to render each character individually and add the specified spacing.
//        Should add a char_space field to text_metrics to keep this function stateless.
int gfx_render_text(Texture *dst, Point pos, char *msg, struct text_metrics *tm)
{
	if (!font)
		return 0;
	if (!msg[0])
		return 0;
	if (font->size != tm->size || font->face != tm->face)
		gfx_set_font(tm->face, tm->size);

	set_font_style(sact_to_sdl_fontstyle(tm->weight));

	int width;
	char *conv = sjis2utf(msg, strlen(msg));
	TTF_SizeUTF8(font->font, conv, &width, NULL);

	pos.y -= (TTF_FontAscent(font->font) - font->size * 0.9);

	glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
	if (tm->outline_left || tm->outline_up || tm->outline_right || tm->outline_down) {
		Texture outline;
		get_glyph(font->font, &outline, conv, tm->outline_color);
		// XXX: This wont work if the outline size is larger than the stroke size
		//      of the font itself.
		if (tm->outline_left) {
			Point p = { pos.x - tm->outline_left, pos.y };
			render_glyph(dst, &outline, p);
		}
		if (tm->outline_up) {
			Point p = { pos.x, pos.y - tm->outline_up };
			render_glyph(dst, &outline, p);
		}
		if (tm->outline_right) {
			Point p = { pos.x + tm->outline_right, pos.y };
			render_glyph(dst, &outline, p);
		}
		if (tm->outline_down) {
			Point p = { pos.x, pos.y + tm->outline_down };
			render_glyph(dst, &outline, p);
		}
		gfx_delete_texture(&outline);
	}

	Texture glyph;
	get_glyph(font->font, &glyph, conv, tm->color);
	render_glyph(dst, &glyph, pos);
	gfx_delete_texture(&glyph);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);

	free(conv);
	return width;
}

void gfx_draw_text_to_amap(Texture *dst, int x, int y, char *text)
{
	if (!font)
		return;

	set_font_style(get_font_style());

	char *conv = sjis2utf(text, strlen(text));
	Point pos = { x, y };

	glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ONE, GL_ZERO);

	Texture glyph;
	get_glyph(font->font, &glyph, conv, font_metrics.color);
	render_glyph(dst, &glyph, pos);
	gfx_delete_texture(&glyph);
	free(conv);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
}

void gfx_draw_text_to_pmap(Texture *dst, int x, int y, char *text)
{
	if (!font)
		return;

	set_font_style(get_font_style());

	char *conv = sjis2utf(text, strlen(text));
	Point pos = { x, y };

	//glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ONE, GL_ZERO);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);

	Texture glyph;
	get_glyph(font->font, &glyph, conv, font_metrics.color);
	render_glyph(dst, &glyph, pos);
	gfx_delete_texture(&glyph);
	free(conv);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
}

void gfx_font_init(void)
{
	if (TTF_Init() == -1)
		ERROR("Failed to initialize SDL_ttf: %s", TTF_GetError());
	gfx_set_font(FONT_GOTHIC, 16);
}
