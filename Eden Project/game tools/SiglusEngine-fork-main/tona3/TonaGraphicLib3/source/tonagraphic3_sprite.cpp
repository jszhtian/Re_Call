#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_sprite.h"

namespace NT3
{

// ****************************************************************
// スプライトパラメータ
// ================================================================
S_sprite_param::S_sprite_param()
{
	// ★隙間もゼロで埋める（memcmp での比較を可能にするため）
	ZeroMemory(this, sizeof(S_sprite_param));

	// 通常の初期化
	init();
}

void S_sprite_param::init()
{
	disp = false;
	layer = 0;
	order = 0;
	depth = 0;
	pct_no = 0;

	S_disp_param::init();
};

// ****************************************************************
// スプライト
// ================================================================
C_sprite::C_sprite()
{
	init();
}

// ****************************************************************
// 初期化
// ================================================================
void C_sprite::init()
{
	reset_type();
	sp.init();
}

// ****************************************************************
// タイプを初期化
// ================================================================
void C_sprite::reset_type()
{
	// 共通パラメータ
	sp.type = SPRITETYPE_NONE;
	sp.change ++;

	// 「ディブ」パラメータ
	dib.reset();
	// 「ピクチャ」パラメータ
	pct.reset();
	// 「アルバム」パラメータ
	album.reset();
}

// ****************************************************************
// 比較演算子
// ================================================================
bool C_sprite::operator < (C_sprite& rhs)
{
	return sp.layer < rhs.sp.layer
		|| (sp.layer == rhs.sp.layer && sp.order < rhs.sp.order)
		|| (sp.layer == rhs.sp.layer && sp.order == rhs.sp.order && sp.depth < rhs.sp.depth)
		|| (sp.layer == rhs.sp.layer && sp.order == rhs.sp.order && sp.depth == rhs.sp.depth && sp.pos.y < rhs.sp.pos.y);
}

// ****************************************************************
// 「プレーン」作成
// ================================================================
bool C_sprite::create_plane()
{
	reset_type();
	sp.type = SPRITETYPE_PLANE;

	return true;
}

// ****************************************************************
// 「ディブ」作成
// ================================================================
bool C_sprite::create_dib(BSP<C_dib> dib_)
{
	reset_type();
	sp.type = SPRITETYPE_DIB;

	dib = dib_;

	return true;
}

// ****************************************************************
// 「ピクチャ」作成
// ================================================================
bool C_sprite::create_pct(BSP<C_pct> pct_)
{
	reset_type();
	sp.type = SPRITETYPE_PCT;

	pct = pct_;

	return true;
}

// ****************************************************************
// 「アルバム」作成
// ================================================================
bool C_sprite::create_album(BSP<C_album> album_)
{
	reset_type();
	sp.type = SPRITETYPE_ALBUM;

	album = album_;

	return true;
}

// ****************************************************************
// データが準備されているかを判定
// ================================================================
bool C_sprite::is_ready()
{
	if (false);

	// プレーン
	else if (sp.type == SPRITETYPE_PLANE)	{
		return true;
	}
	// ディブ
	else if (sp.type == SPRITETYPE_DIB)	{
		if (dib)
			return true;
	}
	// ピクチャ
	else if (sp.type == SPRITETYPE_PCT)	{
		if (pct)
			return true;
	}
	// アルバム
	else if (sp.type == SPRITETYPE_ALBUM)	{
		if (album)
			return true;
	}

	return false;
}

// ****************************************************************
// スプライトパラメータの取得
// ================================================================
void C_sprite::get_sp(S_sprite_param* sp_)
{
	*sp_ = sp;
}

// ****************************************************************
// 表示矩形の計算
//		チェックモードで描画を行い、描画した矩形を取得します。
// ================================================================
void C_sprite::check_disp_rect(C_dib* dst, ARRAY<C_rect>& ret_draw_rect_list)
{
	// 戻り値の初期化
	ret_draw_rect_list.clear();

	S_disp_param dp = sp;

	// 表示矩形の計算
	if (false);

	// プレーン
	else if (sp.type == SPRITETYPE_PLANE)	{
		bool ret_draw_rect_flag = false;
		C_rect ret_draw_rect(0, 0, 0, 0);
		dst->draw_rect(&dp, false, &ret_draw_rect_flag, &ret_draw_rect);
		if (ret_draw_rect_flag)
			ret_draw_rect_list.push_back(ret_draw_rect);
	}
	// ディブ
	else if (sp.type == SPRITETYPE_DIB)	{
		if (dib)	{
			bool ret_draw_rect_flag = false;
			C_rect ret_draw_rect(0, 0, 0, 0);
			dst->draw_dib(dib.get(), &dp, false, &ret_draw_rect_flag, &ret_draw_rect);
			if (ret_draw_rect_flag)
				ret_draw_rect_list.push_back(ret_draw_rect);
		}
	}
	// ピクチャ
	else if (sp.type == SPRITETYPE_PCT)	{
		if (pct)	{
			pct->draw_to_dib(dst, &dp, false, &ret_draw_rect_list);
		}
	}
	// アルバム
	else if (sp.type == SPRITETYPE_ALBUM)	{
		if (album)	{
			album->draw_to_dib(dst, sp.pct_no, &dp, false, &ret_draw_rect_list);
		}
	}
}

// ****************************************************************
// 矩形の計算（全体矩形。当たり判定などに使用）
// ================================================================
C_rect C_sprite::get_rect(int pct_no)
{
	C_rect rect(0, 0, 0, 0);

	// ピクチャ番号の補正（アルバム専用）
	if (pct_no == -1)
		pct_no = sp.pct_no;

	// 表示矩形の計算
	switch (sp.type)	{

		// プレーン
		case SPRITETYPE_PLANE:
			rect.left = sp.pos.x + sp.plane.rect.left;
			rect.top = sp.pos.y + sp.plane.rect.top;
			rect.right = sp.pos.x + sp.plane.rect.right;
			rect.bottom = sp.pos.y + sp.plane.rect.bottom;
			break;

		// ディブ
		case SPRITETYPE_DIB:
			if (dib)	{
				rect.left = sp.pos.x;
				rect.top = sp.pos.y;
				rect.right = sp.pos.x + dib->get_width();
				rect.bottom = sp.pos.y + dib->get_height();
			}
			break;

		// ピクチャ
		case SPRITETYPE_PCT:
			if (pct)	{
				rect.left = sp.pos.x;
				rect.top = sp.pos.y;
				rect.right = sp.pos.x + pct->get_width();
				rect.bottom = sp.pos.y + pct->get_height();
			}
			break;

		// アルバム
		case SPRITETYPE_ALBUM:
			if (album)	{
				BSP<C_pct> pct = album->get_pct(pct_no);
				if (pct)	{
					rect.left = sp.pos.x;
					rect.top = sp.pos.y;
					rect.right = sp.pos.x + pct->get_width();
					rect.bottom = sp.pos.y + pct->get_height();
				}
			}
			break;
	}

	return rect;
}

// ****************************************************************
// dib に描画
// ================================================================
void C_sprite::draw_to_dib(C_dib* dst, C_rect* clip)
{
	if (!sp.disp)
		return;

	S_disp_param dp = sp;

	// 追加クリップを合成
	if (clip)	{

		// オリジナルのクリップがある場合はクリップを合成
		if (dp.dst_clip_use)	{
			dp.dst_clip.left = std::max(clip->left, dp.dst_clip.left);
			dp.dst_clip.top = std::max(clip->top, dp.dst_clip.top);
			dp.dst_clip.right = std::min(clip->right, dp.dst_clip.right);
			dp.dst_clip.bottom = std::min(clip->bottom, dp.dst_clip.bottom);
		}
		else	{
			dp.dst_clip.left = clip->left;
			dp.dst_clip.top = clip->top;
			dp.dst_clip.right = clip->right;
			dp.dst_clip.bottom = clip->bottom;
		}

		// 新しく作成したクリップを使う
		dp.dst_clip_use = true;
	}

	switch (sp.type)	{

		// プレーン
		case SPRITETYPE_PLANE:	
			dst->draw_rect(&dp);
			break;

		// ディブ
		case SPRITETYPE_DIB:
			if (dib)	{
				dst->draw_dib(dib.get(), &dp);
			}
			break;

		// ピクチャ
		case SPRITETYPE_PCT:
			if (pct)	{
				pct->draw_to_dib(dst, &dp);
			}
			break;

		// アルバム
		case SPRITETYPE_ALBUM:
			if (album)	{
				album->draw_to_dib(dst, sp.pct_no, &dp);
			}
			break;
	}
}


}
