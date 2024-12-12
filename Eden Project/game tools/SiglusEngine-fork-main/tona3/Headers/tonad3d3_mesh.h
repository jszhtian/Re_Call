#pragma		once

#include	"tonad3d3_texture.h"
#include	"tonad3d3_effect_lighting_and_shading_shader.h"

namespace NT3
{
	// ****************************************************************
	// D3D マテリアル
	// ================================================================
	struct C_d3d_mtrl
	{
		C_d3d_mtrl();

		D3DMATERIAL9			mtrl;			// マテリアル情報
		BSP<C_d3d_texture>		texture;		// テクスチャ
		bool					disp;			// 表示フラグ
		bool					alpha_blend;	// アルファブレンド
	};

	// ****************************************************************
	// D3D メッシュのパラメータ
	// ================================================================
	struct S_d3d_mesh_param
	{
		enum PRE_RENDER_TYPE {
			//NONE		= 1 << 0,
			NONE		= 0 << 0,
			SHADOW_MAP	= 1 << 0,
			//HOGE		= 1 << 1,
		};

		S_d3d_mesh_param();


		// メイン描画用
		TSTR						effect_old_key;			// 毎フレーム検索回避用キー
		ID3DXEffect*				p_effect;				// 毎フレーム検索回避用エフェクト
		// ライトから見た深度値用
		TSTR						shadow_map_effect_old_key;		// 毎フレーム検索回避用キー
		ID3DXEffect*				p_shadow_map_effect;			// 毎フレーム検索回避用エフェクト


		LIGHTING_TYPE				lighting_type;			// ライティング
		SHADING_TYPE				shading_type;			// シェーディング
		SHADER_OPTION				shader_option;			// シェーダオプション

		PRE_RENDER_TYPE				pre_render_type;		// プリレンダタイプ

		bool						use_mesh_tex;			// 

		bool						use_mrbd;				// 
		D3DXVECTOR4					mrbd;					// mono, reverse, bright, dark
		bool						use_rgb;				// 
		D3DXVECTOR4					rgb_rate;				// r, g, b, rate
		D3DXVECTOR4					add_rgb;				// add_r, add_g, add_b

		// 実装コストとメリットが見合ってないので後回し
		//bool						use_tone_curve;			// トーンカーブ

		bool						use_mul_vertex_color;	// 
		float						mul_vertex_color_rate;	// 頂点カラーの乗算割合(デフォルト1.0f)

		float						depth_buffer_shadow_bias;	// 深度バッファシャドウのバイアス値

		D3DXVECTOR4					rim_light_color;		// 
		float						rim_light_power;		// リムライトの適用範囲と強度

		// トゥーンマップ用
		BSP<C_d3d_texture>			toon_tex;

		// バンプマップ用
		BSP<C_d3d_texture>			normal_tex;

		// 各種ライト用
		ARRAY<int>					directional_light_id_list;
		ARRAY<int>					point_light_id_list;
		ARRAY<int>					spot_light_id_list;


		// リムライト、トーンカーブなども実装予定
		// スクリプトでスプライトに設定出来る物全部必要かも

		// Zバッファとかカリングも入りそう

		// RenderState系は構造体化するかIDirect3DStateBlock9を使う？
		//D3DCULL						cull_mode;			// カリングモード
	};


	// ****************************************************************
	// D3D メッシュ
	// ================================================================
	class C_d3d_mesh
	{
	public:
		C_d3d_mesh();
		C_d3d_mesh(BIP<ID3DXMesh> mesh);
		~C_d3d_mesh();

		void	init();		// 初期化
		void	release();	// 解放

		bool	load_x(CTSTR& file_path, DWORD FVF, ARRAY<TSTR>& texture_name_list);	// x ファイルを読み込む
		bool	add_material(C_d3d_mtrl mtrl);											// マテリアルを追加する
		bool	set_texture(int mtrl_no, BSP<C_d3d_texture> texture);					// マテリアルにテクスチャを設定

		int			get_mtrl_cnt()			{ return (int)mtrl_list.size(); }	// マテリアル数を取得
		C_d3d_mtrl*	get_mtrl(int mtrl_no)	{ return &mtrl_list[mtrl_no]; }		// マテリアルを取得
		D3DXVECTOR3	get_size()				{ return size; }					// 境界ボックスのサイズを取得

		ID3DXMesh*	body()					{ return mesh.get(); }
		ID3DXMesh*	operator ->()			{ return mesh.get(); }

		S_d3d_mesh_param* get_mesh_param_ptr()	{ return &m_param; }

		bool	set_toon_texture(BSP<C_d3d_texture> texture);
		bool	set_normal_texture(BSP<C_d3d_texture> texture);

	private:

		BIP<ID3DXMesh>				mesh;		// メッシュ
		ARRAY<C_d3d_mtrl>			mtrl_list;	// マテリアル
		D3DXVECTOR3					size;		// メッシュのサイズ

		// メッシュ1個単位で制御する可能性があるもの(オブジェクト単位の物はspriteへ？)
		// スキンメッシュもそうじゃないメッシュもこのクラスを使用する
		S_d3d_mesh_param			m_param;	// パラメータ群
	};
}



