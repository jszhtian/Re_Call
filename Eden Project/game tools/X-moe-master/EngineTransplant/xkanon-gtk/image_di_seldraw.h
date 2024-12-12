/*  image_di_seldraw.h
 *       SEL 描画関数のテンプレートなど
 */
/*
 *
 *  Copyright (C) 2002-   Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

#ifndef __KANON_DI_IMAGE_SELDRAW_H__
#define __KANON_DI_IMAGE_SELDRAW_H__

#include <vector>
#include "image_di.h"
#include "image_di_impl.h"
#include "typelist.h"

struct Bpp16Mask : Bpp16 { enum {with_mask = 1}; };
struct Bpp16NoMask : Bpp16 { enum {with_mask = 0}; };
struct Bpp32Mask : Bpp32 { enum {with_mask = 1}; };
struct Bpp32NoMask : Bpp32 { enum {with_mask = 0}; };

enum { NoMask = 0, WithMask = 1};

/*********************************************
**
**	class SelDrawBase
**
**	SelDraw クラスの基底クラス 	
**	基礎的な情報と実装の仮想関数 Exec を持つ。
**
*/
struct SelDrawBase {
	int sel_no;
	bool use_special_maskfunc;

	SelDrawBase(int _sel, const selno_list& sellist, bool use_m);
	virtual ~SelDrawBase(){}
	virtual int Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) = 0;
};

/********************************************
**
**	マクロ：RegisterSel を支援する
**
**	必要な SelDraw の特殊化と init 定義を行う
*/

#define RegisterSelMacro(sel_id, sel_list, is_mask) \
	template<int Bpp, int IsMask> struct SelDrawImpl ## sel_id : Drawer< Int2Type<Bpp> > { \
	enum {bpp = Bpp}; enum {BiPP = Bpp}; enum {ByPP = Bpp/8}; enum {DifByPP = Bpp==16 ? 3 : sizeof(int) };\
		void SetMiddleColor(char* dest, char* src, int c) { Drawer< Int2Type<Bpp> >::SetMiddleColor(dest,src,c);} \
		void Copy1Pixel(char* dest, char* src) { Drawer< Int2Type<Bpp> >::Copy1Pixel(dest,src);} \
		DifImage* MakeDifImage(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel) { return Drawer< Int2Type<Bpp> >::MakeDifImage(dest,src,mask,sel); } \
		char* CalcKido(char* data, int dbpl, int width, int height, int max) { return Drawer< Int2Type<Bpp> >::CalcKido(data, dbpl, width, height, max); } \
		void BlockDifDraw(DifImage* image, BlockFadeData* instance) { Drawer< Int2Type<Bpp> >::BlockDifDraw(image,instance);} \
		int Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count); \
	}; \
	struct SelDraw ## sel_id : SelDrawBase { \
		SelDraw ## sel_id(void) : SelDrawBase(sel_id, sel_list, is_mask) {} \
		SelDrawImpl ## sel_id<16,0> draw16; \
		SelDrawImpl ## sel_id<16,1> draw16m; \
		SelDrawImpl ## sel_id<32,0> draw32; \
		SelDrawImpl ## sel_id<32,1> draw32m; \
		int Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) { \
			int ret = 2; \
			if (src.bypp == 2) { \
				if (mask) ret = draw16m.Exec(dest, src, mask, sel, count); \
				else ret = draw16.Exec(dest, src, mask, sel, count); \
			} else { \
				if (mask) ret = draw32m.Exec(dest, src, mask, sel, count); \
				else ret = draw32.Exec(dest, src, mask, sel, count); \
			} \
			return ret; \
		} \
	}; \
	static SelDraw ## sel_id instance_sel ## sel_id; \
	template<int Bpp, int IsMask> \
	int SelDrawImpl ## sel_id<Bpp,IsMask>


#endif /* __KANON_DI_IMAGE_SELDRAW_H__ */
