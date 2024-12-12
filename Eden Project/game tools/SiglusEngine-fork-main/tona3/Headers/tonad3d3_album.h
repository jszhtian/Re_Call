#pragma		once

#include	"tonad3d3_d3d.h"
#include	"tonad3d3_surface.h"
#include	"tonad3d3_texture.h"

namespace NT3
{

	// ****************************************************************
	// D3D 合成パラメータ
	// ================================================================
	struct C_d3d_album_compose_param
	{
		TSTR	file_name;		// 作業用
		TSTR	file_path;
		int		cut_no;
		int		x;
		int		y;
		int		blend_type;

		C_d3d_album_compose_param()
		{
			cut_no = 0;
			x = 0;
			y = 0;
			blend_type = 0;
		}

		void init()
		{
			file_name.clear();
			file_path.clear();
			cut_no = 0;
			x = 0;
			y = 0;
			blend_type = 0;
		}
	};

	// ****************************************************************
	// D3D アルバム
	// ================================================================
	class C_d3d_album
	{
	public:
		C_d3d_album();
		~C_d3d_album();

		bool	load_g00(CTSTR& file_path, bool fuchidori);
		bool	add_texture(BSP<C_d3d_texture> texture);
		void	release();

		// 合成テクスチャを読み込む
		bool	load_g00_composed(ARRAY<C_d3d_album_compose_param>& param_list);

		int					get_texture_cnt();
		BSP<C_d3d_texture>	get_texture(int cut_no);

		ARRAY< BSP<C_d3d_texture> >&	get_texture_list()	{	return texture_list;	}

	private:
		ARRAY< BSP<C_d3d_texture> >		texture_list;
	};


	inline int C_d3d_album::get_texture_cnt()
	{
		return (int)texture_list.size();
	}

	inline BSP<C_d3d_texture> C_d3d_album::get_texture(int cut_no)
	{
		if (cut_no < 0 || (int)texture_list.size() <= cut_no)
			return BSP<C_d3d_texture>();

		return texture_list[cut_no];
	}


}
