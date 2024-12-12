#pragma		once

#include	"tonad3d3_d3d.h"
#include	"tonad3d3_device.h"
#include	"tonad3d3_data.h"
#include	"tonad3d3_vertex.h"
#include	"tonad3d3_index.h"
#include	"tonad3d3_texture.h"
#include	"tonad3d3_mesh.h"
#include	"tonad3d3_frame_ex.h"

namespace NT3
{

const int	D3D_TEXTURE_CNT_MAX = 4;
const int	D3D_TONE_CURVE_NONE = -1;
const int	D3D_LIGHT_NONE = -1;

// ****************************************************************
// 頂点フォーマットの作り方
//
//		struct D3DVF_SAMPLE
//		{
//			D3DXVECTOR3		pos;			// 位置				D3DFVF_XYZ
//			FLOAT			rhw;			// 位置固定			D3DFVF_XYZRHW
//			D3DXCOLOR		color;			// ディフューズ色	D3DFVF_DIFFUSE
//			D3DXVECTOR2		tex[tex_cnt];	// ＵＶ座標			D3DFVF_TEX1 - D3DFVF_TEX8
//			static const DWORD	FVF;		// FVF
//		};
//
// ----------------------------------------------------------------
//
//		const DWORD D3DVF_SAMPLE::FVF  = D3DFVF_XYZ | D3DFVF_DIFFUSE | ...
//
// ================================================================

// ****************************************************************
// スプライトタイプ
// ================================================================
enum E_d3d_sprite_type
{
	E_d3d_sprite_type_none,			// タイプなし
	E_d3d_sprite_type_point,		// ポイントスプライト
	E_d3d_sprite_type_polygon,		// ポリゴン
	E_d3d_sprite_type_mesh,			// メッシュ
	E_d3d_sprite_type_frame_set,	// フレームセット
};

// ****************************************************************
// スプライト合成方法
// ================================================================
enum E_d3d_blend_type
{
	E_d3d_blend_type_alpha = 0,
	E_d3d_blend_type_add = 1,
	E_d3d_blend_type_sub = 2,
	E_d3d_blend_type_mul = 3,
	E_d3d_blend_type_screen = 4,
	E_d3d_blend_type_overlay = 5,
};

// ****************************************************************
// レンダーパラメータ
// ================================================================
struct S_d3d_render_param
{
	S_d3d_render_param();

	// 初期化
	void	init();

	// bool 値
	bool	disp;			// 表示フラグ
	bool	d2_rect;		// ２Ｄ矩形
	bool	d3_rect;		// ３Ｄ矩形
	bool	d3_billboard;	// ３Ｄビルボード
	bool	dst_clip_use;	// クリッピング
	bool	src_clip_use;	// クリッピング
	bool	culling;		// カリング
	bool	alpha_test;		// アルファテスト
	bool	alpha_blend;	// アルファブレンド
	bool	use_mask;		// マスク
	bool	use_fog;		// フォグ

	// 大きさ
	C_float2	size;					// 大きさ
	int			size_fit_to_texture;	// テクスチャにサイズを合わせる

	// 頂点シェーダ関連
	C_float3		pos;		// 位置
	C_float3		center;		// 中心
	C_float3		scale;		// 拡大率	（等倍 = 1.0f）
	C_float3		rotate;		// 回転		（半回転 = D3DX_PI ≒ 3.14）
	D3DXQUATERNION	quaternion;	// ★クォータニオン
	C_rect			dst_clip;	// クリッピング矩形
	C_rect			src_clip;	// クリッピング矩形

	// ピクセルシェーダ関連
	BYTE		tr;				// 不透明度		（0～255）
	BYTE		mono;			// モノクロ度	（0～255）
	BYTE		reverse;		// 反転度		（0～255）
	BYTE		bright;			// 明度			（0～255）
	BYTE		dark;			// 暗度			（0～255）
	BYTE		color_r;		// 色変化（色）	（0～255）
	BYTE		color_g;		// 色変化（色）	（0～255）
	BYTE		color_b;		// 色変化（色）	（0～255）
	BYTE		color_rate;		// 色変化（割）	（0～255）
	BYTE		color_add_r;	// 色加算		（0～255）
	BYTE		color_add_g;	// 色加算		（0～255）
	BYTE		color_add_b;	// 色加算		（0～255）
	int			tone_curve_no;	// トーンカーブ
	BYTE		tone_curve_sat;	// トーンカーブ彩度
	int			light_no;		// ライト番号
	int			mask_x;			// マスクＸ
	int			mask_y;			// マスクＹ

	// ブレンディング関連
	E_d3d_blend_type		blend_type;		// 合成方法（E_d3d_blend_type で指定。アルファブレンド、加算、減算、…）

	struct FILTER
	{
		C_argb	color;
	}	filter;
};

// ****************************************************************
// D3D スプライト
// ================================================================
class C_d3d_sprite
{
public:
	C_d3d_sprite();
	~C_d3d_sprite();

	// スプライトを解放＆初期化
	void init();
	// スプライトを初期化（解放しない）
	void init_no_release();
	// スプライトを解放
	void release();

	// ポリゴンスプライトを作成（初期化しない）
	bool create_polygon_no_init();
	// メッシュスプライトを作成（初期化しない）
	bool create_mesh_no_init();
	// フレームセットスプライトを作成（初期化しない）
	bool create_frame_set_no_init();
	// ２Ｄスプライトの頂点情報を設定
	bool set_d2_vertex_param(int vertex_offset, int vertex_cnt, int index_offset, int index_cnt, int primitive_cnt, BYTE* p_vertex_buffer, D3DXVECTOR4 p_vertex_pos[4], D3DXVECTOR2 p_texture_uv[4]);

	// --- ポリゴンデータ操作 ---
	void	set_vertex_buffer(BSP<C_d3d_vertex_buffer> vtx_buf)	{	m_vertex_buffer = vtx_buf;	}		// 頂点バッファを設定
	void	set_index_buffer(BSP<C_d3d_index_buffer> ind_buf)	{	m_index_buffer = ind_buf;	}		// インデックスバッファを設定

	BSP<C_d3d_vertex_buffer>	get_vertex_buffer()	{	return m_vertex_buffer;					}		// 頂点バッファを取得
	int							get_vertex_offset()	{	return m_vertex_offset;					}		// 頂点開始位置を取得
	int							get_vertex_cnt()	{	return m_vertex_cnt;					}		// 頂点数を取得
	int							get_vertex_size()	{	return m_vertex_cnt * get_FVF_size();	}		// 頂点バッファのサイズを取得
	BSP<C_d3d_index_buffer>		get_index_buffer()	{	return m_index_buffer;					}		// インデックスバッファを取得
	int							get_index_offset()	{	return m_index_offset;					}		// インデックス開始位置を取得
	int							get_index_cnt()		{	return m_index_cnt;						}		// インデックス数を取得
	int							get_index_size()	{	return m_index_cnt * sizeof(WORD);		}		// インデックスバッファのサイズを取得
	int							get_primitive_cnt()	{	return m_primitive_cnt;					}		// プリミティブ数を取得
	DWORD						get_FVF()			{	return m_vertex_buffer ? m_vertex_buffer->get_FVF() : 0;		}		// FVF を取得
	int							get_FVF_size()		{	return m_vertex_buffer ? m_vertex_buffer->get_FVF_size() : 0;	}		// FVF のサイズを取得

	// --- メッシュデータ操作 ---
	void					set_mesh(BSP<C_d3d_mesh> mesh)					{	m_mesh = mesh;				}	// メッシュの設定
	BSP<C_d3d_mesh>			get_mesh()										{	return m_mesh;				}	// メッシュの取得
	void					set_frame_set(BSP<C_d3d_frame_set> frame_set)	{	m_frame_set = frame_set;	}	// フレームセットの設定
	BSP<C_d3d_frame_set>	get_frame_set()									{	return m_frame_set;			}	// フレームセットの取得

	// --- テクスチャ、シェーダなどの操作
	void					clear_texture(int stage = 0);									// テクスチャをクリア
	void					set_texture(BSP<C_d3d_texture> texture, int stage = 0);			// テクスチャを設定
	BSP<C_d3d_texture>		get_texture(int stage = 0);										// テクスチャを取得
	void					set_effect(ID3DXEffect* effect);								// エフェクトを設定
	ID3DXEffect*			get_effect();													// エフェクトを取得
	void					set_effect_technique(D3DXHANDLE technique);						// エフェクトのテクニックを設定
	D3DXHANDLE				get_effect_technique();											// エフェクトのテクニックを取得
	void					set_effect_constant_f(int idx, float f0, float f1, float f2, float f3);	// エフェクトの定数を設定
	float*					get_effect_constant_f(int idx);									// エフェクトの定数を取得

	// --- レンダーデータ ---
	E_d3d_sprite_type			type;		// スプライトタイプ
	S_d3d_render_param			rp;			// スプライトパラメータ

protected:

	// プリミティブ関連
	BSP<C_d3d_vertex_buffer>	m_vertex_buffer;	// 頂点バッファ
	BSP<C_d3d_index_buffer>		m_index_buffer;		// インデックスバッファ
	int							m_vertex_offset;	// 頂点開始位置（頂点の個数で指定）
	int							m_vertex_cnt;		// 頂点数
	int							m_index_offset;		// インデックス開始位置（頂点の個数で指定）
	int							m_index_cnt;		// インデックス数
	int							m_primitive_cnt;	// プリミティブ数

	// メッシュ関連
	BSP<C_d3d_mesh>				m_mesh;			// メッシュ
	BSP<C_d3d_frame_set>		m_frame_set;	// フレームセット

	// テクスチャ関連
	BSP<C_d3d_texture>			m_texture_list[D3D_TEXTURE_CNT_MAX];	// テクスチャ

	// エフェクト関連
	ID3DXEffect*				m_effect;								// エフェクト
	D3DXHANDLE					m_effect_technique;						// エフェクトのテクニック
	float						m_effect_constant_f[2][4];				// エフェクトの定数
};


}
