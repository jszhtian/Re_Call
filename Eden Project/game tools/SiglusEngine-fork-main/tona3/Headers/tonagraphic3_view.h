#pragma		once

#include	"tonagraphic3_dib.h"
#include	"tonagraphic3_sprite.h"

namespace NT3
{

// ****************************************************************
// ビュー
// ================================================================
class C_view : public C_dib
{
public:
	struct OPTION
	{
		S_sprite_param	sp;			// スプライトパラメータ
		ARRAY<C_rect>	dr_list;	// 描画矩形

		bool operator == (const OPTION& rhs)	{	return sp == rhs.sp;	}	// sp だけを比較！
		bool operator != (const OPTION& rhs)	{	return sp != rhs.sp;	}	// sp だけを比較！
	};

public:
	C_view();
	~C_view();

	bool		create(int width, int height, int depth);
	void		destroy();
	void		clear_sprite_list();						// スプライトリストのクリア
	void		regist_sprite(C_sprite* sprite);			// スプライトを登録

	// 画面更新関係
	void			begin_scene();					// シーン開始
	void			update_redraw_block_table();	// 再描画ブロックテーブルを更新
	void			update_redraw_rect_list();		// 再描画矩形リストを更新
	void			draw_sprite_list();				// 描画
	ARRAY<C_rect>&	get_redraw_rect_list()	{	return redraw_rect_list;	}
	void			set_redraw_block_full();

private:
	typedef	std::map<C_sprite *, OPTION>		SPRITEMAP;	// スプライトマップ
	typedef	std::pair<C_sprite *, OPTION>	SPRITEPAIR;	// スプライトペア
	SPRITEMAP	old_map;		// 前回描画時のマップ
	SPRITEMAP	new_map;		// 今回描画時のマップ

	typedef ARRAY<C_sprite *>	SPRITELIST;
	SPRITELIST	sprite_list;	// スプライトリスト
	
	typedef ARRAY<C_rect>		RECTLIST;
	RECTLIST	redraw_rect_list;	// 再描画矩形リスト

	typedef BYTE		BLOCKTYPE;

	int					redraw_block_cnt_x;
	int					redraw_block_cnt_y;
	ARRAY<BLOCKTYPE>	redraw_block_table;

	BLOCKTYPE*	get_redraw_block_ptr(int x, int y)	{	return redraw_block_table.get() + (y * redraw_block_cnt_x) + x;	}
	void		create_redraw_block_table();															// 再描画ブロックテーブルの構築
	void		clear_redraw_block_table();																// 再描画ブロックテーブルをクリア（全体）
	void		clear_redraw_block(int block_l, int block_t, int block_r, int block_b);					// 再描画ブロックテーブルをクリア（範囲指定）
	void		set_redraw_block(int block_l, int block_t, int block_r, int block_b);					// 再描画ブロックをセット

	void		update_redraw_block_table(C_sprite* sprite);			// 再描画ブロックテーブルを更新（スプライトから）
	void		update_redraw_block_table(ARRAY<C_rect>& rect_list);	// 再描画ブロックテーブルを更新（矩形リストから）
	void		update_redraw_block_table(C_rect rect);					// 再描画ブロックテーブルを更新（矩形から）
	void		magnify_redraw_block(int l, int t, int* w, int* h);		// 再描画ブロックを広げる
	void		draw_sprite(C_sprite* sprite, C_rect* rect = NULL);		// スプライトを描画

};

}
