#pragma		once

#include	"tonad3d3_surface.h"

namespace NT3
{

// ****************************************************************
// Direct3D デバイスステート
// ================================================================
struct C_d3d_device_state
{
	UINT					display_adapter;			// ディスプレイアダプタ
	D3DDISPLAYMODE			display_mode;				// ディスプレイモード

	D3DDEVTYPE				device_type;				// デバイスのタイプ
	D3DCAPS9				device_caps;				// デバイスの能力
	UINT					vm_size;					// ビデオメモリの最大容量
	DWORD					vertex_process_type;		// 頂点処理方法
	D3DPRESENT_PARAMETERS*	p_pp;						// プレゼントパラメータ（現在のモード）
	D3DPRESENT_PARAMETERS	pp_window;					// プレゼントパラメータ（ウィンドウ）
	D3DPRESENT_PARAMETERS	pp_full;					// プレゼントパラメータ（フルスクリーン）
	D3DFORMAT				fmt_render_texture_window;	// レンダーテクスチャのフォーマット(ウインドウ)
	D3DFORMAT				fmt_render_texture_full;	// レンダーテクスチャのフォーマット(フルスクリーン)

	bool					device_lost;				// デバイス消失フラグ
	bool					device_can_reset;			// デバイスリセット可能フラグ
	bool					mipmap_use_flag;			// ミップマップを使うフラグ
	bool					pow2_limit_flag;			// 必ず 2^n に補正するフラグ
	HRESULT					hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture;	// CheckDeviceFormat(D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE) の結果 
	HRESULT					hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture;	// CheckDeviceFormat(D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE) の結果 
	HRESULT					hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture;	// CheckDeviceFormat(D3DUSAGE_QUERY_FILTER, D3DRTYPE_TEXTURE) の結果 
	HRESULT					hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture;	// CheckDeviceFormat(D3DUSAGE_QUERY_FILTER, D3DRTYPE_TEXTURE) の結果 

	HWND					h_wnd;						// ターゲットウィンドウ
	bool					is_window;					// ウィンドウ／フルスクリーン
	int						shadow_map_size;			// シャドウマップのサイズ
	C_size					full_size;					// フルスクリーン時のウィンドウサイズ
	float					z_min;						// ビューポートの最手前のＺ値
	float					z_max;						// ビューポートの最奥のＺ値
};

// ****************************************************************
// Direct3D デバイス
// ================================================================
class C_d3d_device
{
public:
	C_d3d_device();
	~C_d3d_device();

//	bool	create(HWND h_wnd, bool is_window, int width, int height, int adapter_no, int present_interval, bool mipmap_use_flag);	// デバイスを構築
//	bool	create(HWND h_wnd, bool is_window, int width, int height, int shadow_map_size, int adapter_no, int present_interval, bool mipmap_use_flag);	// デバイスを構築
	bool	create(HWND h_wnd, bool is_window, int width, int height, int shadow_map_size, int adapter_no, DWORD vertex_process_type, int present_interval, bool mipmap_use_flag, bool pow2_limit_flag);

	void	release();									// デバイスを解放
	bool	restore();									// デバイスを復元
	bool	present(C_rect* p_rect = NULL);				// スクリーンへ転送

	bool	check_device_lost();						// デバイスの消失を判定
	bool	change_display_mode(bool is_window);		// ディスプレイモードの変更
	bool	change_screen_size(C_size size);			// スクリーンサイズの変更
	bool	change_present_interval(int interval);		// 描画間隔を変更

	bool	can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture();		// テクスチャに D3DUSAGE_AUTOGENMIPMAP が使えるかを調べる
	bool	can_set_D3DPTFILTERCAPS_MIPFPOINT_for_texture();	// テクスチャに D3DPTFILTERCAPS_MIPFPOINT が使えるかどうかを調べる
	bool	can_set_D3DPTFILTERCAPS_MIPFLINEAR_for_texture();	// テクスチャに D3DPTFILTERCAPS_MIPFLINEAR が使えるかどうかを調べる
	bool	can_enable_D3DUSAGE_AUTOGENMIPMAP_for_texture();	// テクスチャに D3DUSAGE_AUTOGENMIPMAP を使ってミップマップが作られるかどうかを調べる
	bool	can_use_not_pow2_texture();							// 2^n 以外のテクスチャが使えるかどうかを調べる

	// レンダーターゲット系（NULLを渡せば何もしない）
	bool	set_render_target(DWORD rt_idx, IDirect3DSurface9* rt);
	bool	get_render_target(DWORD rt_idx, IDirect3DSurface9** rt);
	bool	set_depth_stencil_surface(IDirect3DSurface9* dss);
	bool	get_depth_stencil_surface(IDirect3DSurface9** dss);
	bool	set_viewport(D3DVIEWPORT9* vp);
	bool	get_viewport(D3DVIEWPORT9* vp);

	bool	clear_render_target(D3DCOLOR back_color);

	IDirect3DDevice9*				body()					{	return m_device.get();			}	// デバイスを取得
	IDirect3DDevice9*				operator ->()			{	return m_device.get();			}	// デバイスを取得
	const C_d3d_device_state&		state() const			{	return m_state;					}	// ステートを取得
	const D3DPRESENT_PARAMETERS*	pp() const				{	return m_state.p_pp;			}	// プレゼントパラメータを取得
	const D3DCAPS9&					device_caps() const		{	return m_state.device_caps;		}	// デバイスの能力を取得
	int								get_shadow_map_size()	{	return m_state.shadow_map_size;	}	// シャドウマップのサイズ
	float							get_z_min()				{	return m_state.z_min;			}	// ビューポートの最手前のＺ値
	float							get_z_max()				{	return m_state.z_max;			}	// ビューポートの最奥のＺ値

	bool							is_lost()				{	return m_state.device_lost;			}	// デバイス消失フラグ
	bool							can_reset()				{	return m_state.device_can_reset;	}	// デバイス消失フラグ

	bool	set_view_port(C_rect rect);									// ビューポートの設定
	bool	set_view_port(C_point point, C_size size);					// ビューポートの設定
	bool	set_view_port(int x, int y, int width, int height);			// ビューポートの設定

	// リソースを作成
	BIP<IDirect3DVertexBuffer9>		create_vertex_buffer();

private:

	BIP<IDirect3DDevice9>	m_device;		// Direct3D デバイス
	C_d3d_device_state		m_state;		// ステート

	void	init();							// デバイスを初期化

	bool	select_device();				// デバイスの選択
	bool	check_device();					// デバイスのチェック
	bool	create_device();				// デバイスの作成

	bool	check_adapter_format();			// ディスプレイとバックバッファのフォーマットのチェック
	bool	check_depth_stencil_format();	// 深度／ステンシルバッファのフォーマットのチェック
	bool	check_render_texture_format();	// レンダーテクスチャのフォーマットのチェック
	bool	check_multisample_type();		// フルシーンアンチエイリアシングのチェック
	bool	check_display_mode();			// ディスプレイモードのチェック
	bool	check_device_caps();			// デバイスの能力のチェック
};

}

