#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_thumb.h"

namespace NT3
{

// ****************************************************************
// イメージリスト
// ================================================================
Cimage_list::Cimage_list()
{
	h_il = NULL;
}

Cimage_list::~Cimage_list()
{
	destroy();
}

// ****************************************************************
// イメージリスト：作成
// ================================================================
bool Cimage_list::create(int width, int height)
{
	destroy();

	// イメージリストを作成（初期個数 32、増加数 8）
	h_il = ImageList_Create(width, height, ILC_COLOR32, 32, 8);
	if (h_il == NULL)
		return false;

	return true;
}

// ****************************************************************
// イメージリスト：破壊
// ================================================================
void Cimage_list::destroy()
{
	if (h_il)	{
		ImageList_Destroy(h_il);
		h_il = NULL;
	}
}

// ****************************************************************
// イメージリスト：イメージを追加
// ================================================================
bool Cimage_list::add_image(HBITMAP h_bitmap)
{
	if (!h_il)
		return false;

	ImageList_Add(h_il, h_bitmap, NULL);

	return true;
}

}