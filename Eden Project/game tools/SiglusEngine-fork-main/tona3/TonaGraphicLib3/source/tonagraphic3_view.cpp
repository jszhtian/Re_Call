#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_view.h"

const int 	REDRAW_BLOCK_W = 16;
const int	REDRAW_BLOCK_H = 16;

namespace NT3
{

// ****************************************************************
// ビュー
// ================================================================
C_view::C_view()
{
	redraw_block_cnt_x = 0;
	redraw_block_cnt_y = 0;
}

C_view::~C_view()
{
}

// ****************************************************************
// ビューの作成
// ================================================================
bool C_view::create(int width, int height, int depth)
{
	destroy();

	if (!C_dib::create(width, height, depth))
		return false;

	// 再描画ブロックテーブルの構築
	create_redraw_block_table();

	return true;
}

// ****************************************************************
// ビューの開放
// ================================================================
void C_view::destroy()
{
	redraw_block_cnt_x = 0;
	redraw_block_cnt_y = 0;

	sprite_list.clear();
	redraw_rect_list.clear();
	redraw_block_table.clear();
	old_map.clear();
	new_map.clear();

	C_dib::destroy();
}

// ****************************************************************
// 再描画ブロックテーブルの構築
// ================================================================
void C_view::create_redraw_block_table()
{
	// 再描画ブロックの個数を計算
	redraw_block_cnt_x = (get_width() - 1) / REDRAW_BLOCK_W + 1;
	redraw_block_cnt_y = (get_height() - 1) / REDRAW_BLOCK_H + 1;

	// 再描画ブロックテーブルの構築
	redraw_block_table.resize(redraw_block_cnt_x * redraw_block_cnt_y);

	// 再描画ブロックテーブルのクリア
	clear_redraw_block_table();
}

// ****************************************************************
// 再描画ブロックテーブルのクリア
// ================================================================
void C_view::clear_redraw_block_table()
{
	ZeroMemory(redraw_block_table.get(), redraw_block_cnt_x * redraw_block_cnt_y * sizeof(BLOCKTYPE));
}

void C_view::set_redraw_block_full()
{
	fill_memory(redraw_block_table.get(), redraw_block_cnt_x * redraw_block_cnt_y * sizeof(BLOCKTYPE), -1);
}

void C_view::clear_redraw_block(int block_l, int block_t, int block_r, int block_b)
{
	block_l = std::max(block_l, 0);
	block_t = std::max(block_t, 0);
	block_r = std::min(block_r, redraw_block_cnt_x);
	block_b = std::min(block_b, redraw_block_cnt_y);

	if (block_l >= block_r)		return;
	if (block_t >= block_b)		return;

	// false は 0 であることを勝手に仮定してます
	for (int y = block_t; y < block_b; y++)	{
		BLOCKTYPE* block_ptr = get_redraw_block_ptr(block_l, y);
		ZeroMemory(block_ptr, (block_r - block_l) * sizeof(BLOCKTYPE));
	}
}

void C_view::set_redraw_block(int block_l, int block_t, int block_r, int block_b)
{
	block_l = std::max(block_l, 0);
	block_t = std::max(block_t, 0);
	block_r = std::min(block_r, redraw_block_cnt_x);
	block_b = std::min(block_b, redraw_block_cnt_y);

	if (block_l >= block_r)		return;
	if (block_t >= block_b)		return;

	// true は 1 であることを勝手に仮定してます
	for (int y = block_t; y < block_b; y++)	{
		BLOCKTYPE* block_ptr = get_redraw_block_ptr(block_l, y);
		fill_memory(block_ptr, (block_r - block_l) * sizeof(BLOCKTYPE), -1);
	}
}

// ****************************************************************
// スプライトリストを解放
// ================================================================
void C_view::clear_sprite_list()
{
	sprite_list.clear();
}

// ****************************************************************
// スプライトをリストに登録
// ================================================================
void C_view::regist_sprite(C_sprite* sprite)
{
	sprite_list.push_back(sprite);
}

// ****************************************************************
// シーンの開始
// ================================================================
void C_view::begin_scene()
{
	// マップの入れ替え
	old_map = SPRITEMAP(new_map.begin(), new_map.end());
	new_map.clear();

	// 再描画ブロックのクリア
	clear_redraw_block_table();
	redraw_rect_list.clear();
}

// ****************************************************************
// 再描画矩形を更新
// ================================================================
void C_view::update_redraw_block_table()
{
	// 再描画ブロックテーブルを更新
	for (SPRITELIST::iterator itr = sprite_list.begin(); itr != sprite_list.end(); ++itr)
		update_redraw_block_table(*itr);

	// 残っている前回の表示矩形を追加
	for (SPRITEMAP::iterator itr = old_map.begin(); itr != old_map.end(); ++itr)
		update_redraw_block_table(itr->second.dr_list);
}

// ****************************************************************
// 再描画ブロックマップを更新
// ================================================================
void C_view::update_redraw_block_table(C_sprite* sprite)
{
	// 表示パラメータを計算
	OPTION opt;
	sprite->get_sp(&opt.sp);

	// 表示する場合
	if (opt.sp.disp && opt.sp.alpha > 0)	{

		// 前回のリストからスプライトを探す
		SPRITEMAP::iterator last = old_map.find(sprite);
		// 見つかった場合
		if (last != old_map.end())	{
			// 表示パラメータに変更があった場合
			if (opt.sp != last->second.sp)	{
				// 描画矩形を求める
				sprite->check_disp_rect(this, opt.dr_list);
				// 再描画ブロックの更新
				update_redraw_block_table(last->second.dr_list);
				update_redraw_block_table(opt.dr_list);
			}
			// 表示パラメータに変更がなかった場合
			else	{
				// 描画矩形を前回からコピー
				opt.dr_list = last->second.dr_list;
			}
			// 前回のリストから削除
			old_map.erase(last);
		}
		// 見つからなかった場合
		else	{
			// 描画矩形を求める
			sprite->check_disp_rect(this, opt.dr_list);
			// 再描画ブロックの更新
			update_redraw_block_table(opt.dr_list);
		}
		// 表示リストに加える
		SPRITEPAIR pair(sprite, opt);
		new_map.insert(pair);
	}
}

// ****************************************************************
// 再描画ブロックマップ更新（矩形リストから）
// ================================================================
void C_view::update_redraw_block_table(ARRAY<C_rect>& rect_list)
{
	for (ARRAY<C_rect>::iterator itr = rect_list.begin(); itr != rect_list.end(); ++itr)	{
		update_redraw_block_table(*itr);
	}
}

// ****************************************************************
// 再描画ブロックマップ更新（矩形から）
// ================================================================
void C_view::update_redraw_block_table(C_rect rect)
{
	if (rect.width() <= 0 || rect.height() <= 0)
		return;

	// 矩形から、再描画ブロックの位置を計算
	int left = rect.left / REDRAW_BLOCK_W;
	int top = rect.top / REDRAW_BLOCK_H;
	int right = (rect.right - 1) / REDRAW_BLOCK_W + 1;
	int bottom = (rect.bottom - 1) / REDRAW_BLOCK_H + 1;

	// 再描画ブロック更新
	set_redraw_block(left, top, right, bottom);
}

// ****************************************************************
// 再描画矩形を更新
// ================================================================
void C_view::update_redraw_rect_list()
{
	// 再描画するべき矩形の描画
	for (int block_l = 0; block_l < redraw_block_cnt_x; block_l++)	{
		BLOCKTYPE* block_ptr = get_redraw_block_ptr(block_l, 0);
		for (int block_t = 0; block_t < redraw_block_cnt_y; block_t++, block_ptr += redraw_block_cnt_x)	{
			if (*block_ptr)	{
				*block_ptr = false;

				// 再描画矩形を広げる
				int block_r, block_b;
				magnify_redraw_block(block_l, block_t, &block_r, &block_b);
				clear_redraw_block(block_l, block_t, block_r, block_b);

				// 再描画矩形の追加
				C_rect redraw_rect(block_l * REDRAW_BLOCK_W, block_t * REDRAW_BLOCK_H, block_r * REDRAW_BLOCK_W, block_b * REDRAW_BLOCK_H);
				redraw_rect_list.push_back(redraw_rect);
			}
		}
	}
}

// ****************************************************************
// スプライトリストを描画
// ================================================================
void C_view::draw_sprite_list()
{
	// 全矩形に対して全スプライトを描画
	for (RECTLIST::iterator rect_itr = redraw_rect_list.begin(); rect_itr != redraw_rect_list.end(); ++rect_itr)	{
		for (SPRITELIST::iterator sprite_itr = sprite_list.begin(); sprite_itr != sprite_list.end(); ++sprite_itr)
			draw_sprite(*sprite_itr, &*rect_itr);
	}
}

// ****************************************************************
// スプライトの描画
// ================================================================
void C_view::draw_sprite(C_sprite* sprite, C_rect* rect)
{
	sprite->draw_to_dib(this, rect);
}

// ****************************************************************
// 再描画領域の左上から右下を求める
// ================================================================
void C_view::magnify_redraw_block(int left, int top, int* right_, int* bottom_)
{
	int right, bottom;

	// 下に再描画矩形を伸ばす
	BLOCKTYPE* block_p = get_redraw_block_ptr(left, top + 1);
	for (bottom = top + 1; bottom < redraw_block_cnt_y; bottom++, block_p += redraw_block_cnt_x)	{
		if (!*block_p)		// 下が再描画ブロックの場合
			break;			// 下端確定
	}

	// 右に再描画矩形を伸ばす
	for (right = left + 1; right < redraw_block_cnt_x; right++)	{

		// 上端から下端にかけて、
		// 右の列が全て再描画領域であるかどうかを調べる
		bool inc = true;
		BLOCKTYPE* block_p = get_redraw_block_ptr(right, top);
		for (int y = top; y < bottom; y++, block_p += redraw_block_cnt_x)	{
			if (!*block_p)	{
				inc = false;
				break;
			}
		}
		if (!inc)		// 右に再描画ブロックでないものを見つけた
			break;		// 右端確定
	}

	*right_ = right;
	*bottom_ = bottom;
}
};
