#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_g00.h"

#include	<g00.h>

namespace NT3
{

// ****************************************************************
// dib チップに g00 チップを読み込む
// ================================================================
bool load_g00_chip_to_dib(C_dib_chip* dib_chip, C_g00_chip* g00_chip)
{
	// g00 チップの情報を取得
	S_g00_chip_info chip_info;
	g00_chip->get_info(&chip_info);

	// dib チップの作成
	if (!dib_chip->create(chip_info.width, chip_info.height, 32))
		return false;

	dib_chip->x = chip_info.x;
	dib_chip->y = chip_info.y;
	dib_chip->sprite = chip_info.sprite;

	// g00 データの読み込み
	g00_chip->get_data(dib_chip->get_ptr(), dib_chip->get_width() * 4);

	return true;
}

// ****************************************************************
// ピクチャに g00 カットを読み込む
// ================================================================
bool load_g00_cut_to_pct(C_pct* pct, C_g00_cut* g00_cut)
{
	// カットの情報を取得
	S_g00_cut_info cut_info;
	g00_cut->get_info(&cut_info);

	// チップの読み込み
	ARRAY< BSP<C_dib_chip> > chip_list;
	for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{
		BSP<C_dib_chip> chip(new C_dib_chip);
		if (!load_g00_chip_to_dib(chip.get(), g00_cut->get_chip(chip_no)))
			return false;

		chip_list.push_back(chip);
	}

	// ピクチャの作成
	pct->create(chip_list, cut_info.width, cut_info.height, cut_info.center, cut_info.disp_rect);

	return true;
}

// ****************************************************************
// アルバムに g00 を読み込む
// ================================================================
bool load_g00_to_album(C_album* album, CTSTR& file_path)
{
	// g00 ファイルを読み込む
	BUFFER buffer;
	if (!C_file::read_full_data(file_path, buffer))
		return false;

	// g00 の作成
	C_g00 g00;
	g00.set_data(buffer.get(), buffer.size());

	// g00 の情報を読み込む
	S_g00_info g00_info;
	g00.get_info(&g00_info);

	// カットを読み込む
	ARRAY< BSP<C_pct> > pct_list;
	for (int pct_no = 0; pct_no < g00_info.cut_cnt; pct_no++)	{
		BSP<C_pct> pct(new C_pct);
		if (!load_g00_cut_to_pct(pct.get(), g00.get_cut(pct_no)))
			return false;

		pct_list.push_back(pct);
	}

	// アルバムの作成
	album->create(pct_list);

	return true;
}

// ****************************************************************
// ディブに g00 チップを読み込む
// ================================================================
bool load_g00_chip_to_dib_expand(C_dib* dib, C_g00_chip* g00_chip)
{
	// g00 チップの情報を取得
	S_g00_chip_info chip_info;
	g00_chip->get_info(&chip_info);

	// g00 データの読み込み
	g00_chip->get_data(dib->get_ptr(chip_info.x, chip_info.y), dib->get_width() * 4);

	return true;
}

// ****************************************************************
// ディブに g00 カットを読み込む
// ================================================================
bool load_g00_cut_to_dib_expand(C_dib* dib, C_g00_cut* g00_cut)
{
	// カットの情報を取得
	S_g00_cut_info cut_info;
	g00_cut->get_info(&cut_info);

	// ディブを作成
	if (!dib->create(cut_info.width, cut_info.height, 32))
		return false;

	dib->clear_color(C_argb(0, 0, 0, 0));

	// チップの読み込み
	for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{
		if (!load_g00_chip_to_dib_expand(dib, g00_cut->get_chip(chip_no)))
			return false;
	}

	return true;
}

// ****************************************************************
// アルバムに g00 を読み込む
// ================================================================
bool load_g00_to_dib_expand(C_dib* dib, CTSTR& file_path, int cut_no)
{
	// g00 ファイルを読み込む
	BUFFER buffer;
	if (!C_file::read_full_data(file_path, buffer))
		return false;

	// g00 の作成
	C_g00 g00;
	g00.set_data(buffer.get(), buffer.size());

	// g00 の情報を読み込む
	S_g00_info g00_info;
	g00.get_info(&g00_info);

	// カットを読み込む
	if (!load_g00_cut_to_dib_expand(dib, g00.get_cut(cut_no)))
		return false;

	return true;
}


};
