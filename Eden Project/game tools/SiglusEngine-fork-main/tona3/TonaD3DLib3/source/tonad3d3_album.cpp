#include	"tonad3d3_pch.h"
#include	"tonad3d3_device.h"
#include	"tonad3d3_album.h"

#include	"g00.h"

namespace NT3
{

// ****************************************************************
// D3D アルバム
// ================================================================
C_d3d_album::C_d3d_album()
{
}

C_d3d_album::~C_d3d_album()
{
}


// ****************************************************************
// 合成 g00 を読み込む
// ================================================================

bool C_d3d_album::load_g00_composed(ARRAY<C_d3d_album_compose_param>& param_list)
{
	if (param_list.empty())
		return false;

	int file_cnt = (int)param_list.size();

	// ★一時バッファ（マルチスレッド未対応）
	static BUFFER file_data;

	// テクスチャは１つ
	texture_list.resize(1);
	
	// ファイルループ
	for (int file_i = 0; file_i < file_cnt; file_i++)	{

		// ファイル名がない場合
		if (param_list[file_i].file_path.empty())	{
			// １枚目なら空バッファを作成
			if (file_i == 0)	{
				texture_list[0] = BSP<C_d3d_texture>(new C_d3d_texture);
				texture_list[0]->create(param_list[file_i].x, param_list[file_i].y);
			}
		}
		// ファイル名がある場合
		else	{

			// g00 ファイルを読み込む
			if (!C_file::read_full_data(param_list[file_i].file_path, file_data))
				return false;

			// g00 を作成
			C_g00 g00;
			if (!g00.set_data(file_data.get(), file_data.size()))
				return false;

			// g00 の情報を取得
			S_g00_info g00_info;
			g00.get_info(&g00_info);

			// カット番号を取得
			int cut_no = param_list[file_i].cut_no;
			cut_no = limit(0, cut_no, g00_info.cut_cnt - 1);

			// カットを取得
			C_g00_cut* g00_cut = g00.get_cut(cut_no);
			if (g00_cut->is_data())	{

				// １枚目ならテクスチャを作成
				if (file_i == 0)
				{
					texture_list[0] = BSP<C_d3d_texture>(new C_d3d_texture);
					if (!texture_list[0]->load_g00_cut(g00_cut, false))
						return false;
				}
				// ２枚目以降なら合成
				else
				{
					if (!texture_list[0]->draw_g00_cut(g00_cut, param_list[file_i].x, param_list[file_i].y, param_list[file_i].blend_type))
						return false;
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// g00 を読み込む
// ================================================================
bool C_d3d_album::load_g00(CTSTR& file_path, bool futidori)
{
	// ★一時バッファ（マルチスレッド未対応）
	static BUFFER file_data;

	// g00 ファイルを読み込む
	if (!C_file::read_full_data(file_path, file_data))
		return false;

	// g00 を作成
	C_g00 g00;
	if (!g00.set_data(file_data.get(), file_data.size()))
		return false;

	// g00 の情報を取得
	S_g00_info g00_info;
	g00.get_info(&g00_info);

	// カット数のテクスチャを作成
	texture_list.resize(g00_info.cut_cnt);

	// カットの処理
	for (int cut_no = 0; cut_no < g00_info.cut_cnt; cut_no++)	{

		// カットを取得
		C_g00_cut* g00_cut = g00.get_cut(cut_no);
		if (g00_cut->is_data())	{

			// テクスチャにカットを読み込む
			texture_list[cut_no] = BSP<C_d3d_texture>(new C_d3d_texture);
			if (!texture_list[cut_no]->load_g00_cut(g00_cut, futidori))
				return false;
		}
	}

	return true;
}

// ****************************************************************
// テクスチャを追加する
// ================================================================
bool C_d3d_album::add_texture(BSP<C_d3d_texture> texture)
{
	texture_list.push_back(texture);

	return true;
}

// ****************************************************************
// 解放する
// ================================================================
void C_d3d_album::release()
{
	texture_list.clear();
}

}
