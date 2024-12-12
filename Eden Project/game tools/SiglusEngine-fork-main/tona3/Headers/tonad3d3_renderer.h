#pragma once

#include	"tonad3d3_d3d.h"
#include	"tonad3d3_sprite.h"
#include	"tonad3d3_effect.h"
#include	"tonad3d3_effect_source_manager.h"

#include	"tonad3d3_render.h"
#include	"tonad3d3_pre_render.h"
#include	"tonad3d3_post_render.h"

namespace NT3
{

class C_d3d_light_interface;
class C_d3d_camera_interface;

// ****************************************************************
// Ｄ３Ｄレンダラー
// ================================================================
class C_d3d_renderer
{
public:

	bool	init();									// 初期化
	void	clear_sprite_list();					// スプライトリストの開放
	void	regist_sprite(C_d3d_sprite* sprite);	// スプライトの登録
	void	delete_sprite(C_d3d_sprite* sprite);	// スプライトの抹消

	C_d3d_surface	get_back_buffer();								// バックバッファを取得
	bool			set_render_target(IDirect3DSurface9* surface);	// レンダーターゲットを設定
	bool			set_dpst_buffer(IDirect3DSurface9* surface);	// 深度ステンシルバッファを設定
	C_d3d_surface	get_dpst_buffer();								// 深度ステンシルバッファを取得

	// レンダーターゲットをクリア
	bool	clear_render_target(D3DCOLOR back_color = 0);
	bool	clear_render_target_buffer(D3DCOLOR back_color = 0);
	bool	clear_render_target_stencil();
	bool	clear_render_target_z_buffer();

	// 更新
	bool	update();

	// レンダー
	bool	render();
	bool	pre_render();
	bool	post_render();

	// デバッグ用
	bool	debug_render_shadow_map(int render_size, int x, int y);
	bool	debug_render_light_bloom_tex(int render_size, int x, int y);

	// デフォルトのビューポートを設定する
	void	set_default_view_port(const C_rect& rect)
	{
		m_render_default.set_default_view_port(rect);
	}

	// アクセサメソッド
	// シャドウマップ
	void set_shadow_map_tex(IDirect3DTexture9*	shadow_map_tex)
		{m_shadow_map_tex = shadow_map_tex;}
	void set_shadow_map_surface(IDirect3DSurface9*	shadow_map_surface)
		{m_shadow_map_surface = shadow_map_surface;}
	void set_shadow_map_depth_stencil_buffer(IDirect3DSurface9*	shadow_map_depth_stencil_buffer)
		{m_shadow_map_depth_stencil_buffer = shadow_map_depth_stencil_buffer;}
	void set_shadow_map_viewport(D3DVIEWPORT9 shadow_map_viewport)
		{m_shadow_map_viewport = shadow_map_viewport;}
	IDirect3DTexture9*	get_shadow_map_tex()
		{return m_shadow_map_tex;}

	// ライトブルーム
	void set_light_bloom_tex(IDirect3DTexture9*	light_bloom_tex)
		{m_light_bloom_tex = light_bloom_tex;}
	void set_light_bloom_surface(IDirect3DSurface9*	light_bloom_surface)
		{m_light_bloom_surface = light_bloom_surface;}


	D3DXMATRIX*		get_view_matrix()		{	return &m_mat_view;			}
	D3DXMATRIX*		get_proj_matrix()		{	return &m_mat_proj;			}
	D3DXMATRIX*		get_light_view_matrix()	{	return &m_mat_light_view;	}
	D3DXMATRIX*		get_light_proj_matrix()	{	return &m_mat_light_proj;	}
	D3DXMATRIX*		get_scale_bias_matrix()	{	return &m_mat_scale_bias;	}

	void	set_camera(BSP<C_d3d_camera_interface> camera)	{	m_camera = camera;	}

	C_d3d_camera_interface*					get_camera()			{	return m_camera.get();		}
	C_d3d_default_effect*					get_default_effect()	{	return &m_default_effect;	}
	ARRAY< BSP<C_d3d_light_interface> >*	get_light_list()		{	return &m_light_list;		}

private:
	bool update_frame_set(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_frame* frame);

public:

	// 変換行列
	D3DXMATRIX		m_mat_view;		// ビュー
	D3DXMATRIX		m_mat_proj;		// 射影
	D3DXVECTOR3		m_camera_eye;	// カメラの位置（▲勝手に作ったよ by 末次 2011/10/5）
	D3DXVECTOR3		m_camera_dir;	// カメラの向き（▲勝手に作ったよ by 末次 2011/10/5）

	// デフォルトのエフェクト
	C_d3d_default_effect					m_default_effect;

	// カメラ
	BSP<C_d3d_camera_interface>				m_camera;

	// ライトリスト
	ARRAY< BSP<C_d3d_light_interface> >		m_light_list;

	// フォグ
	int										m_fog_use;
	float									m_fog_x;
	float									m_fog_near;
	float									m_fog_far;

	// ライトの変換行列(後で直す)
	D3DXMATRIX		m_mat_light_view;			// ビュー
	D3DXMATRIX		m_mat_light_proj;			// 射影
	D3DXMATRIX		m_mat_scale_bias;

	// 描画クラス群
	C_d3d_render_default		m_render_default;
	C_d3d_pre_render_shadow_map	m_pre_render_shadow_map;

private:
	// シャドウマップ
	IDirect3DTexture9*	m_shadow_map_tex;
	IDirect3DSurface9*	m_shadow_map_surface;
	IDirect3DSurface9*	m_shadow_map_depth_stencil_buffer;	
	D3DVIEWPORT9		m_shadow_map_viewport;

	// デフォルト系
	IDirect3DSurface9*	m_default_back_buffer;
	IDirect3DSurface9*	m_default_depth_stencil_buffer;	
	D3DVIEWPORT9		m_default_viewport;

	// ライトブルーム
	IDirect3DTexture9*	m_light_bloom_tex;
	IDirect3DSurface9*	m_light_bloom_surface;

	// スプライトリスト
	ARRAY<C_d3d_sprite *>	sprite_list;
};

// ****************************************************************
// スプライトリストをクリア
// ================================================================
inline void C_d3d_renderer::clear_sprite_list()
{
	sprite_list.clear();
}

// ****************************************************************
// スプライトを登録
// ================================================================
inline void C_d3d_renderer::regist_sprite(C_d3d_sprite* sprite)
{
	sprite_list.push_back(sprite);
}

// ****************************************************************
// スプライトを抹消
// ================================================================
inline void C_d3d_renderer::delete_sprite(C_d3d_sprite* sprite)
{
	ARRAY<C_d3d_sprite *>::iterator itr = find(sprite_list.begin(), sprite_list.end(), sprite);
	if (itr != sprite_list.end())
		sprite_list.erase(itr);

}


}
