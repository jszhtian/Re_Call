#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_pct.h"

#define		__DR	1	// 矩形の算出に disp_rect を使うスイッチ

namespace NT3
{

// ****************************************************************
// コンストラクタ
// ================================================================
C_pct::C_pct()
{
	width = 0;
	height = 0;
}

// ****************************************************************
// デストラクタ
// ================================================================
C_pct::~C_pct()
{
}

// ****************************************************************
// 破壊
// ================================================================
void C_pct::destroy()
{
	chip_list.clear();

	width = 0;
	height = 0;
	center = C_point(0, 0);
	disp_rect = C_rect(0, 0, 0, 0);
}

// ****************************************************************
// ピクチャを作成
// ================================================================
bool C_pct::create(ARRAY< BSP<C_dib_chip> > chip_list_, int width_, int height_, C_point center_, C_rect disp_rect_)
{
	destroy();

	chip_list = chip_list_;
	width = width_;
	height = height_;
	center = center_;
	disp_rect = disp_rect_;

	return true;
}

// ****************************************************************
// 文字を読み込む
// ================================================================
bool C_pct::load_moji(TCHAR moji, C_argb color, LOGFONT* lf, C_text_format* tf)
{
	MAT2 mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };

	return load_moji(moji, color, lf, tf, mat);
}

bool C_pct::load_moji(TCHAR moji, C_argb color, LOGFONT* lf, C_text_format* tf, MAT2 mat)
{
	destroy();

	BSP<C_dib_chip> chip(new C_dib_chip);

	// チップに文字を読み込む
	int pos_x, pos_y;
	if (!chip->load_moji(moji, color, lf, &pos_x, &pos_y, mat))
		return false;

	// チップリストに追加
	chip->x = pos_x;
	chip->y = tf->height - pos_y;
	chip->sprite = true;
	chip_list.push_back(chip);

	// 描画矩形の設定
	width = tf->width;
	height = tf->height;
	disp_rect = C_rect(chip->x, chip->y, chip->x + chip->get_width(), chip->y + chip->get_height());

	return true;
}

// ****************************************************************
// bmp を読み込む
// ================================================================
bool C_pct::load_bmp(CTSTR& file_path)
{
	destroy();

	// チップに bmp を読み込む
	BSP<C_dib_chip> chip(new C_dib_chip);
	if (!chip->load_bmp(file_path))
		return false;

	// チップの設定
	chip->sprite = true;		// bmp は透過なし！
	chip_list.push_back(chip);	// チップリストに追加

	// 描画矩形の設定
	width = chip->get_width();
	height = chip->get_height();
	disp_rect = C_rect(0, 0, width, height);

	return true;
}

// ****************************************************************
// png を読み込む
// ================================================================
bool C_pct::load_png(CTSTR& file_path)
{
	destroy();

	// チップに png を読み込む
	BSP<C_dib_chip> chip(new C_dib_chip);
	if (!chip->load_png(file_path))
		return false;

	// チップの設定
	chip->sprite = true;		// png は透過あり！
	chip_list.push_back(chip);	// チップリストに追加

	// 描画矩形の設定
	width = chip->get_width();
	height = chip->get_height();
	disp_rect = C_rect(0, 0, width, height);

	return true;
}

// ****************************************************************
// jpg を読み込む
// ================================================================
/*
bool C_pct::load_jpg(CTSTR& file_path)
{
	destroy();

	// チップに jpg を読み込む
	BSP<C_dib_chip> chip(new C_dib_chip);
	if (!chip->load_jpg(file_path))
		return false;

	// チップの設定
	chip->sprite = true;		// jpg は透過なし！
	chip_list.push_back(chip);	// チップリストに追加

	// 描画矩形の設定
	width = chip->get_width();
	height = chip->get_height();
	disp_rect = C_rect(0, 0, width, height);

	return true;
}
*/

// ****************************************************************
// ドラクエ３モンスターＣＧを読み込む
// ================================================================
#if 0

bool C_pct::load_DQ3_monster(BYTE* rom_data, int monster_id)
{
	// クリア
	destroy();

	// グラフィックエントリ
	BYTE* ad_entry = (BYTE *)(rom_data + 0x08ED3 + monster_id * 5);

	// グラフィックエントリから情報取得
	int block_cnt = *ad_entry;
	BYTE* ad_gra = (BYTE *)(rom_data + *(WORD *)(ad_entry + 1)) + 16;
	BYTE* ad_pal = (BYTE *)(rom_data + *(WORD *)(ad_entry + 3)) + 16;

	// パレットハッシュテーブル作成
	BYTE pal[256] = {0};
	// 情報ビット取得
	int pal_info = *ad_pal++;
	for (int i = 0; i < 2; i++)	{
		int pal_dst_pos, pal_dst_cnt;
		if (i == 0)	{	pal_dst_pos = 4;	pal_dst_cnt = pal_info & 0x0f;	}
		else		{	pal_dst_pos = 1;	pal_dst_cnt = pal_info >> 4;	}
		Cpalette::get_instance().make_FC_palette(ad_pal, pal, pal_dst_pos, pal_dst_cnt);
		ad_pal += 3 * pal_dst_cnt;
	}

	// グラフィックアドレス調整
	int ad_gra_hosei = ( ( *(rom_data + 0x08EC1 + monster_id / 8) ) >> (7 - monster_id % 8) ) & 1;
	ad_gra += ad_gra_hosei * 0x4000;

	// チップの最小矩形
//	disp.draw_rect.left = 99999;
//	disp.draw_rect.top = 99999;
//	disp.draw_rect.right = 0;
//	disp.draw_rect.bottom = 0;
	width = 0;
	height = 0;

	// グラフィック構造体
	struct GRA_HEADER	{
		BYTE	data[3];
		int		block;
	};
	struct GRA_BLOCK	{
		BYTE	data[16];
	};

	// 展開データ
	ARRAY<GRA_HEADER>	header_list;
	ARRAY<GRA_BLOCK>	block_list;

	// グラフィックデータの構築
	for (int b = 0; b < block_cnt; )	{

		GRA_HEADER header;
		GRA_BLOCK block;

		// ヘッダの構築
		header.data[0] = ad_gra[0];
		header.data[1] = ad_gra[1];
		header.data[2] = ad_gra[2];
		header.block = b;
		header_list.push_back(header);

		if (header.data[0] & 0x40)	ad_gra += 2;
		else						ad_gra += 3;

		// グラフィックデータありの場合
		if (header.data[0] & 0x80)	{

			BYTE c_data = 0;	// 共通データ
			WORD e_data = 0;	// 補足データ

			// 共通データと補足データを得る
			if (header.data[0] & 0x08)	{
				if (header.data[0] & 0x01)	c_data = *ad_gra++;
				else						c_data = 0;
				e_data = *(WORD *)(ad_gra);
				ad_gra += 2;
			}
			else	{
				c_data = 0;
				e_data = 0xFFFF;
			}

			// データ展開
			for (int cnt=0; cnt<16; cnt++)	{
				if (e_data & 0x8000)	block.data[cnt] = *ad_gra++;
				else					block.data[cnt] = c_data;
				e_data <<= 1;
			}

			block_list.push_back(block);
			b++;
		}
	}

	// チップの構築
	for (int h = 0; h < (int)header_list.size(); h++)	{

		GRA_HEADER header = header_list[h];
		GRA_BLOCK block = block_list[header.block];

		bool rev_x = (header.data[0] & 0x02) > 0;
		bool rev_y = (header.data[0] & 0x04) > 0;

		// パレット追加情報
		int pal_pos = (header.data[0] & 0x70 ^ 0x40) >> 4;
		if (header.data[0] & 0x40)
			pal_pos += 1;

		// チップの読み込み
		BSP<C_dib_chip> chip(new C_dib_chip);
		chip->load_fc_chip(block.data, pal, pal_pos, rev_x, rev_y);

		// 追加情報のセット
		if (header.data[0] & 0x40)	{	// 透明なし
			chip->x = ( header.data[1] & 0x0f ) * 8;
			chip->y = ( (header.data[1] >> 4) + ((header.data[1] >> 4) & 0x08) * (-2) ) * 8 + 0x10;
			chip->beta = true;
		}
		else	{						// 透明あり
			chip->x = ( header.data[1] & 0x80 ) * (-2) + header.data[1];
			chip->y = ( header.data[2] & 0x80 ) * (-2) + header.data[2] + 0x10;
			chip->beta = false;
		}

		// チップの位置から矩形を計算
//		disp.draw_rect.left		= min( disp.draw_rect.left,   chip->X() );
//		disp.draw_rect.top		= min( disp.draw_rect.top,    chip->Y() );
//		disp.draw_rect.right	= max( disp.draw_rect.right,  chip->X()+15 );
//		disp.draw_rect.bottom	= max( disp.draw_rect.bottom, chip->Y()+15 );
		width  = max(width,  chip->x + chip->get_width());
		height = max(height, chip->y + chip->get_height());

		// チップをリストに追加
		chip_list.push_back(chip);
	}

	return true;
	
}
#endif

// ****************************************************************
// ドラクエ３キャラクターチップＣＧ
// ================================================================
#if 0

bool C_pct::load_DQ3_chara_chip(BYTE* rom_data, int chara_id, int chara_way, int step_pat)
{
	// クリア
	destroy();

	// パレットハッシュテーブル
	BYTE pal[256];
	BYTE* ad_clr = rom_data + 0x1E562;
	Cpalette::get_instance().make_FC_palette(ad_clr, pal, 0, 64);
	pal[1] = 0x30;

	// 各種情報
	int pct_no = chara_id * 8 + chara_way * 2 + step_pat;
	BYTE* ad_entry   = rom_data + 0x16DE8 + pct_no * 3;
	BYTE* ad_shutoku = rom_data + 0x1744F;
	BYTE* ad_gra     = rom_data + 0x20010;

	// 各種情報
	int a = *ad_entry;			// 情報その１
	int b = *(ad_entry + 1);	// 情報その２
	int c = *(ad_entry + 2);	// 情報その３

	int a_low = a & 0x0f;
	int a_high = a >> 4;

	// パレットオフセット
	int pal_pos = (a_low >> 2) + 4;

	// アドレス補正
	b += (a_low & 0x03) << 8;

	if (a_high < 0x0f)	{

		BYTE* gra_data;		// グラフィックデータ
		BYTE* org_gra_data;	// グラフィックデータ先頭

		org_gra_data = (BYTE *)(ad_gra + (b << 4));
		gra_data = org_gra_data;
		int d = 0;

		for (int i=0; i<4; i++)	{

			// 反転情報
			bool rev_x = ((c >> (6 - 2 * i)) & 0x01) > 0;
			bool rev_y = ((c >> (7 - 2 * i)) & 0x01) > 0;

			// チップの読み込み
			BSP<C_dib_chip> chip(new C_dib_chip);
			chip->load_fc_chip(gra_data, pal, pal_pos, rev_x, rev_y);
			chip->x = i % 2 * 8;
			chip->y = i / 2 * 8;
			chip->beta = false;
			// チップをリストに追加
			chip_list.push_back(chip);
			// アドレス補正
			d += *(ad_shutoku + a_high * 3 + i);
			gra_data = (BYTE *)(((d & 0x80) * (-2) + d) * 0x10 + org_gra_data);
		}
	}
	else	{
		b = b * 5 + 0x172B0;
		int d = *(rom_data + b);

		for (int i = 0; i < 4; i++)	{

			BYTE* gra_data;

			// 反転情報
			bool rev_x = ((c >> (6 - 2 * i)) & 0x01) > 0;
			bool rev_y = ((c >> (7 - 2 * i)) & 0x01) > 0;

			// アドレス補正
			b++;
			int e = *(rom_data + b);
			gra_data = (BYTE *)(((d << (8 - i * 2)) & 0x300 + e) * 0x10 + ad_gra);
			// チップの読み込み
			BSP<C_dib_chip> chip(new C_dib_chip);
			chip->load_fc_chip(gra_data, pal, pal_pos, rev_x, rev_y);
			chip->x = i % 2 * 8;
			chip->y = i / 2 * 8;
			chip->beta = false;
			// チップをリストに追加
			chip_list.push_back(chip);
		}
	}

	return true;
}
#endif

// ****************************************************************
// 全チップをベタに展開する
// ================================================================
BSP<C_dib_chip> C_pct::expand_chips()
{
	int chip_cnt = (int)chip_list.size();

	// 単一チップを作成
	BSP<C_dib_chip> ex_chip(new C_dib_chip);
	if (!ex_chip->create(width, height, 32))
		return BSP<C_dib_chip>();

	// 単一チップを透明に塗りつぶし
	fill_memory_4(ex_chip->get_ptr(), width * height, 0);
	ex_chip->sprite = true;

	// 全チップを単一チップに描画
	for (int c = 0; c < chip_cnt; c++)	{
		BSP<C_dib_chip> chip = chip_list[c];
		ex_chip->draw_dib_easy(chip.get(), chip->x, chip->y);
	}

	// チップリストを破棄し、単一チップを新たに追加
	chip_list.clear();
	chip_list.push_back(ex_chip);

	return ex_chip;
}

// ****************************************************************
// 拡大
// ================================================================
bool C_pct::magnify(int scale)
{
	CHIPLIST old_chip_list = chip_list;
	chip_list.clear();

	CHIPLIST::iterator itr;
	for (itr = old_chip_list.begin(); itr != old_chip_list.end(); ++itr)	{

		BSP<C_dib_chip> old_chip = *itr;
		BSP<C_dib_chip> new_chip(new C_dib_chip);

		new_chip->create_magnified_dib_scale(old_chip.get(), scale, scale);
		new_chip->x = old_chip->x * scale;
		new_chip->y = old_chip->y * scale;
		new_chip->sprite = old_chip->sprite;

		chip_list.push_back(new_chip);
	}

	return true;
}

// ****************************************************************
// 右に９０度回転
// ================================================================
bool C_pct::rotate_90()
{
	CHIPLIST old_chip_list = chip_list;
	chip_list.clear();

	CHIPLIST::iterator itr;
	for (itr = old_chip_list.begin(); itr != old_chip_list.end(); ++itr)	{

		BSP<C_dib_chip> old_chip = *itr;
		BSP<C_dib_chip> new_chip(new C_dib_chip);

		new_chip->create_rotate_90_dib(old_chip.get());
		new_chip->x = height - old_chip->y - old_chip->get_height();
		new_chip->y = old_chip->x;
		new_chip->sprite = old_chip->sprite;

		chip_list.push_back(new_chip);
	}

	return true;
}

// ****************************************************************
// dib に描画
// ================================================================
void C_pct::draw_to_dib(C_dib* dib, int x, int y, bool sprite)
{
	S_disp_param dp;
	dp.pos.x = x;
	dp.pos.y = y;
	dp.sprite = sprite;
	draw_to_dib(dib, &dp);
}

void C_pct::draw_to_dib(C_dib* dib, S_disp_param* dp)
{
	draw_to_dib(dib, dp, true, NULL);
}

void C_pct::draw_to_dib(C_dib* dib, S_disp_param* dp_, bool draw_flag, ARRAY<C_rect>* ret_draw_rect_list)
{
	// チェックルーチンの場合は特殊処理
	// 各チップごとではなく、disp_rect から矩形を算出する

#if __DR
	if (!draw_flag)	{
		S_disp_param dp = *dp_;
		dp.center += center;	// 自身の中心座標
		dp.plane.type = E_disp_plane_type_box_fill;
		dp.plane.color = C_argb(255, 255, 255, 255);
		dp.plane.rect = disp_rect;

		bool ret_draw_rect_flag = false;
		C_rect ret_draw_rect(0, 0, 0, 0);
		dib->draw_rect(&dp, draw_flag, &ret_draw_rect_flag, &ret_draw_rect);
		// 表示矩形があるなら追加
		if (ret_draw_rect_list && ret_draw_rect_flag)
			ret_draw_rect_list->push_back(ret_draw_rect);
	}
	// 描画ルーチンの場合はチップごとに処理
	else	{
#endif

		// 全チップを順に描画する
		for (CHIPLIST::iterator itr = chip_list.begin(); itr != chip_list.end(); ++itr)	{
			BSP<C_dib_chip> chip = *itr;
			S_disp_param dp = *dp_;

			// 中心座標の修正
			dp.center += center;	// 自身の中心座標
			dp.center.x -= chip->x;	// チップ補正
			dp.center.y -= chip->y;	// チップ補正
				// ★回転や拡縮の中心を dp に別途用意する必要がある？

			// ベタチップの場合、透過しない
			if (!chip->sprite)	dp.sprite = false;

			// チップを描画
			bool ret_draw_rect_flag = false;
			C_rect ret_draw_rect(0, 0, 0, 0);
			dib->draw_dib(chip.get(), &dp, draw_flag, &ret_draw_rect_flag, &ret_draw_rect);
			// 表示矩形があるなら追加
			if (ret_draw_rect_list && ret_draw_rect_flag)
				ret_draw_rect_list->push_back(ret_draw_rect);
		}
#if __DR
	}
#endif
}



}
