#include "tonad3d3_pch.h"
#include "tonad3d3_effect_draw_depth_target.h"

namespace NT3
{


C_d3d_effect_draw_depth_target::C_d3d_effect_draw_depth_target()
{
	//m_frame_effect = NULL;
	//m_skinned_effect = NULL;
	//m_skinned_nothing_bw_effect = NULL;

	m_shadow_tex = NULL;
	m_shadow_surf = NULL;
	m_z_buffer = NULL;
}

C_d3d_effect_draw_depth_target::~C_d3d_effect_draw_depth_target()
{
	release();
}


bool C_d3d_effect_draw_depth_target::init(int tex_size)
{
	m_tex_size = tex_size;

	if(!create_shadow_map())
		return false;

	return true;
}

bool C_d3d_effect_draw_depth_target::create_shadow_map()
{
	HRESULT hr;

	hr = G_d3d.device.body()->CreateTexture(
						m_tex_size, m_tex_size,
						1, 
						D3DUSAGE_RENDERTARGET,
						D3DFMT_A8R8G8B8,
						D3DPOOL_DEFAULT,
						&m_shadow_tex,
						NULL
						);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("CreateTexture"), hr);
		return false;
	}

	hr = m_shadow_tex->GetSurfaceLevel(
						0,
						&m_shadow_surf
						);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("GetSurfaceLevel"), hr);
		return false;
	}

	hr = G_d3d.device.body()->CreateDepthStencilSurface(
						m_tex_size, m_tex_size,
						D3DFMT_D16,
						D3DMULTISAMPLE_NONE,
						0,
						TRUE,
						&m_z_buffer,
						NULL
						);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("CreateDepthStencilSurface"), hr);
		return false;
	}

	D3DVIEWPORT9 viewport = {0,0      // 左上の座標
					, m_tex_size // 幅
					, m_tex_size // 高さ
					, 0.0f,1.0f};     // 前面、後面
	m_viewport = viewport;

	return true;
}

/*
bool C_d3d_effect_draw_depth_target::create_shadow_map_effect()
{
	HRESULT hr;

	MBSTR effect_source = "";

	DWORD flag = 0;
	ID3DXBuffer* p_error = NULL;

	// メモリから読み込む
	hr = D3DXCreateEffect(G_d3d.device.body(), effect_source.c_str(), (UINT)effect_source.size(), 0, 0, flag, 0, &m_effect, &p_error);
	if(FAILED(hr)) {
		set_last_error_d3d(MBSTR_to_TSTR(p_error ? (char*)p_error->GetBufferPointer() : ""), _T("D3DXCreateEffect"), hr);
		return false;
	}

	return true;
}
*/

bool C_d3d_effect_draw_depth_target::release()
{
	if(m_z_buffer)		{ (m_z_buffer)->Release();		(m_z_buffer)=NULL; }
	if(m_shadow_surf)	{ (m_shadow_surf)->Release();	(m_shadow_surf)=NULL; }
	if(m_shadow_tex)	{ (m_shadow_tex)->Release();	(m_shadow_tex)=NULL; }

	return true;
}

/*
bool C_d3d_effect_draw_depth_target::change_render_target(DWORD rt_idx)
{
	HRESULT hr;
	// ビューポート
	D3DVIEWPORT9 viewport = {0,0		// 左上の座標
					, m_tex_size		// 幅
					, m_tex_size		// 高さ
					, 0.0f,1.0f};		// 前面、後面

	// レンダリングターゲットの変更
	if(G_d3d.device.set_render_target_depth_stencil_surface_viewport(
						rt_idx,
						m_shadow_surf,
						m_z_buffer,
						&viewport
						))
		return false;

	// シャドウマップのクリア
	hr = G_d3d.device.body()->Clear(
					0L,
					NULL,
					D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					0xFFFFFFFF,
					1.0f,
					0L
					);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("Clear"), hr);
		return false;
	}


	return true;
}
*/



}