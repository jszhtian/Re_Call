#include<map>
#include"image_di_seldraw.h"
#include"initial.h"

using namespace std;

/*********************************************
**
**	class SelDrawBase
**
*/

typedef map<int,SelDrawBase*>::iterator Sel_it;
static map<int,SelDrawBase*>* sel_list = 0;

SelDrawBase::SelDrawBase(int _sel, const selno_list& all_sels, bool use_m) : sel_no(_sel), use_special_maskfunc(use_m) {
	if (!sel_list) sel_list = new map<int, SelDrawBase*>;

	vector<int>::const_iterator it;
	for (it =all_sels.data.begin(); it != all_sels.data.end(); it++) {
		(*sel_list)[*it] = this;
	}
}

/**********************************************
**
**	CopyWithSel (実装)
**
*/

static int MaskedDraw(SelDrawBase* drawer, DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	if (count == 0) { /* 画像作製 */
		int width = sel->x2-sel->x1+1;
		int height= sel->y2-sel->y1+1;
		DI_Image* masked_src = new DI_Image;
		masked_src->CreateImage(src.width, src.height, src.bypp);
		CopyRect(*masked_src, sel->x1, sel->y1, dest, sel->x3, sel->y3, width, height);
		CopyRectWithMask(masked_src, sel->x1, sel->y1, &src, sel->x1, sel->y1, width, height, mask);
		sel->params[15] = int(masked_src);
	}
	int ret = drawer->Exec(dest, *(DI_Image*)(sel->params[15]), 0, sel, count);
	if (ret == 2) { /* 終了 */
		delete (DI_Image*)(sel->params[15]);
	}
	return ret;
}

extern int CopyWithSel(DI_Image* dest, DI_ImageMask* src, SEL_STRUCT* sel, int count) {
	SelDrawBase* drawer;
	Sel_it impl;

	dest->RecordChangedRegionAll();

	// 描画関数の決定
	if ((*sel_list).find(sel->sel_no) == (*sel_list).end()) {
		if ((*sel_list).find(4) == (*sel_list).end()) { // default sel = 4
			drawer = (*sel_list).begin()->second;
		} else {
			drawer = (*sel_list)[4];
		}
	} else 
		drawer = (*sel_list)[sel->sel_no];

	// 実行
	if (sel->kasane && src->Mask()) { // マスク付き描画
		if (drawer->use_special_maskfunc) {
			return drawer->Exec(*dest, *(DI_Image*)src, src->Mask(), sel, count);
		} else {
			return MaskedDraw(drawer, *dest, *(DI_Image*)src, src->Mask(), sel, count);
		}
	} else {
		return drawer->Exec(*dest, *(DI_Image*)src, 0, sel, count);
	}
}

