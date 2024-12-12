#include	"tonad3d3_pch.h"
#include	"tonad3d3_device.h"
#include	"tonad3d3_texture.h"
#include	"tonad3d3_data.h"

namespace NT3
{

// ****************************************************************
// DirectX 9.0 Direct3D Device
// ================================================================
C_d3d_device::C_d3d_device()
{
	// パラメータを初期化
	ZeroMemory(&m_state.pp_window, sizeof(m_state.pp_window));
	ZeroMemory(&m_state.pp_full, sizeof(m_state.pp_full));
	m_state.device_lost = false;
}

C_d3d_device::~C_d3d_device()
{
}

// ****************************************************************
// デバイスを初期化
// ================================================================
void C_d3d_device::init()
{
	// デバイスを解放
	release();

	// パラメータを初期化
	ZeroMemory(&m_state.pp_window, sizeof(m_state.pp_window));
	ZeroMemory(&m_state.pp_full, sizeof(m_state.pp_full));
	m_state.device_lost = false;
}

// ****************************************************************
// デバイスを解放
// ================================================================
void C_d3d_device::release()
{
	m_device = NULL;
}

// ****************************************************************
// デバイスを作成
// ================================================================
//		adapter_no:
//			D3DADAPTER_DEFAULT:				プライマリ（デフォルト）
//			1 以上:							セカンダリ、…
// ================================================================
//		prenset_interval:
//			D3DPRESENT_INTERVAL_IMMEDIATE:	垂直同期を待たない
//			D3DPRESENT_INTERVAL_ONE:		垂直同期を１度だけ待つ
// ================================================================

#if 0
bool C_d3d_device::create(HWND h_wnd, bool is_window, int width, int height, int adapter_no, int present_interval, bool mipmap_use_flag)
{
	HRESULT hr;

	// デバイスを初期化する
	init();

	// パラメータの設定
	m_state.h_wnd = h_wnd;
	m_state.is_window = is_window;
	m_state.full_size.cx = width;
	m_state.full_size.cy = height;
	m_state.z_min = 0.0f;
	m_state.z_max = 1.0f;
	m_state.mipmap_use_flag = mipmap_use_flag;

	// 使えるアダプタの数を調べる
	int adapter_cnt = (int)G_d3d->GetAdapterCount();
	if (adapter_no < 0 || adapter_no >= adapter_cnt)
		adapter_no = D3DADAPTER_DEFAULT;	// 範囲外のアダプタを指定した場合はデフォルトのアダプタを使用する

	// 使用するアダプタを設定
	m_state.display_adapter = adapter_no;

	// プライマリのディスプレイアダプタを使用
	// ※マルチモニタの場合は、先に列挙して選択する必要があります。▲未実装
//	m_state.display_adapter = D3DADAPTER_DEFAULT;

	// 現在のディスプレイモードを取得
	hr = G_d3d->GetAdapterDisplayMode(m_state.display_adapter, &m_state.display_mode);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("ディスプレイモードの取得に失敗しました。"), _T("GetAdapterDisplayMode"), hr);
		return false;
	}

	// プレゼントパラメータ初期設定
	ZeroMemory(&m_state.pp_window, sizeof(m_state.pp_window));
	ZeroMemory(&m_state.pp_full, sizeof(m_state.pp_full));
	m_state.pp_window.BackBufferCount			= 1;									// バックバッファ数
	m_state.pp_window.BackBufferFormat			= D3DFMT_X8R8G8B8;						// バックバッファのフォーマット
	m_state.pp_window.SwapEffect				= D3DSWAPEFFECT_DISCARD;				// スワップエフェクト
	m_state.pp_window.EnableAutoDepthStencil	= TRUE;									// 深度／ステンシルバッファを使う
	m_state.pp_window.AutoDepthStencilFormat	= D3DFMT_D24S8;							// 深度／ステンシルバッファのフォーマット
	m_state.pp_window.MultiSampleType			= D3DMULTISAMPLE_NONE;					// マルチサンプリングを使わない（使って格段に綺麗になるなら使う予定）
	m_state.pp_window.PresentationInterval		= present_interval;						// D3DPRESENT_INTERVAL_IMMEDIATE / D3DPRESENT_INTERVAL_ONE
//	m_state.pp_window.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;	// 深度ステンシルバッファを破棄できる（環境によっては速くなるらしい）
	m_state.pp_full = m_state.pp_window;												// フルスクリーンモードにウィンドウモードのパラメータをコピー

	m_state.pp_window.Windowed					= TRUE;					// ウィンドウモード
	m_state.pp_window.hDeviceWindow				= m_state.h_wnd;		// ウィンドウハンドル
	m_state.pp_full.Windowed					= FALSE;				// フルスクリーンモード
	m_state.fmt_render_texture_window			= D3DFMT_X8R8G8B8;		// ウィンドウモードのレンダーターゲットのフォーマット（Mirage 3 において、D3DFMT_A8R8G8B8 だと正常に描画されない）
	m_state.fmt_render_texture_full				= D3DFMT_X8R8G8B8;		// フルスクリーンモードのレンダーターゲットのフォーマット（Mirage 3 において、D3DFMT_A8R8G8B8 だと正常に描画されない）

	// デバイスの選択
	if (!select_device())
		return false;

	// デバイスのチェック
	if (!check_device())
		return false;

	// 現在のプレゼントパラメータの設定
	m_state.p_pp = m_state.is_window ? &m_state.pp_window : &m_state.pp_full;

	// デバイスの作成
	if (!create_device())
		return false;

	// ビデオメモリの最大容量を取得
	m_state.vm_size = m_device->GetAvailableTextureMem();

	// ビューポートを初期化
	if (!set_view_port(0, 0, m_state.p_pp->BackBufferWidth, m_state.p_pp->BackBufferHeight))
		return false;

	// デバイスの初期化に成功
	return true;
}
#endif


bool C_d3d_device::create(HWND h_wnd, bool is_window, int width, int height, int shadow_map_size, int adapter_no, DWORD vertex_process_type, int present_interval, bool mipmap_use_flag, bool pow2_limit_flag)
{
	HRESULT hr;

	// デバイスを初期化する
	init();

	// パラメータの設定
	m_state.h_wnd = h_wnd;
	m_state.is_window = is_window;
	m_state.full_size.cx = width;
	m_state.full_size.cy = height;
	m_state.shadow_map_size = shadow_map_size;
	m_state.z_min = 0.0f;
	m_state.z_max = 1.0f;
	m_state.vertex_process_type = vertex_process_type;
	m_state.mipmap_use_flag = mipmap_use_flag;
	m_state.pow2_limit_flag = pow2_limit_flag;

	// 使えるアダプタの数を調べる
	int adapter_cnt = (int)G_d3d->GetAdapterCount();
	if (adapter_no < 0 || adapter_no >= adapter_cnt)
		adapter_no = D3DADAPTER_DEFAULT;	// 範囲外のアダプタを指定した場合はデフォルトのアダプタを使用する

	// 使用するアダプタを設定
	m_state.display_adapter = adapter_no;

	// プライマリのディスプレイアダプタを使用
	// ※マルチモニタの場合は、先に列挙して選択する必要があります。▲未実装
//	m_state.display_adapter = D3DADAPTER_DEFAULT;

	// 現在のディスプレイモードを取得
	hr = G_d3d->GetAdapterDisplayMode(m_state.display_adapter, &m_state.display_mode);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("ディスプレイモードの取得に失敗しました。"), _T("GetAdapterDisplayMode"), hr);
		return false;
	}

	// プレゼントパラメータ初期設定
	ZeroMemory(&m_state.pp_window, sizeof(m_state.pp_window));
	ZeroMemory(&m_state.pp_full, sizeof(m_state.pp_full));
	m_state.pp_window.BackBufferCount			= 1;									// バックバッファ数
	m_state.pp_window.BackBufferFormat			= D3DFMT_X8R8G8B8;						// バックバッファのフォーマット
	m_state.pp_window.SwapEffect				= D3DSWAPEFFECT_DISCARD;				// スワップエフェクト
	m_state.pp_window.EnableAutoDepthStencil	= TRUE;									// 深度／ステンシルバッファを使う
	m_state.pp_window.AutoDepthStencilFormat	= D3DFMT_D24S8;							// 深度／ステンシルバッファのフォーマット
	m_state.pp_window.MultiSampleType			= D3DMULTISAMPLE_NONE;					// マルチサンプリングを使わない（使って格段に綺麗になるなら使う予定）
	m_state.pp_window.PresentationInterval		= present_interval;						// D3DPRESENT_INTERVAL_IMMEDIATE / D3DPRESENT_INTERVAL_ONE
//	m_state.pp_window.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;	// 深度ステンシルバッファを破棄できる（環境によっては速くなるらしい）
	m_state.pp_full = m_state.pp_window;												// フルスクリーンモードにウィンドウモードのパラメータをコピー

	m_state.pp_window.Windowed					= TRUE;					// ウィンドウモード
	m_state.pp_window.hDeviceWindow				= m_state.h_wnd;		// ウィンドウハンドル
	m_state.pp_full.Windowed					= FALSE;				// フルスクリーンモード
	m_state.fmt_render_texture_window			= D3DFMT_X8R8G8B8;		// ウィンドウモードのレンダーターゲットのフォーマット（Mirage 3 において、D3DFMT_A8R8G8B8 だと正常に描画されない）
	m_state.fmt_render_texture_full				= D3DFMT_X8R8G8B8;		// フルスクリーンモードのレンダーターゲットのフォーマット（Mirage 3 において、D3DFMT_A8R8G8B8 だと正常に描画されない）

	// デバイスの選択
	if (!select_device())
		return false;

	// デバイスのチェック
	if (!check_device())
		return false;

	// 現在のプレゼントパラメータの設定
	m_state.p_pp = m_state.is_window ? &m_state.pp_window : &m_state.pp_full;

	// デバイスの作成
	if (!create_device())
		return false;

	// ビデオメモリの最大容量を取得
	m_state.vm_size = m_device->GetAvailableTextureMem();

	// ビューポートを初期化
	if (!set_view_port(0, 0, m_state.p_pp->BackBufferWidth, m_state.p_pp->BackBufferHeight))
		return false;

	// デバイスの初期化に成功
	return true;
}

// ****************************************************************
// ビューポートを初期化する
// ================================================================
bool C_d3d_device::set_view_port(C_rect rect)
{
	return set_view_port(rect.left, rect.top, rect.width(), rect.height());
}

bool C_d3d_device::set_view_port(C_point point, C_size size)
{
	return set_view_port(point.x, point.y, size.cx, size.cy);
}

bool C_d3d_device::set_view_port(int x, int y, int width, int height)
{
	HRESULT hr;

	// ビューポートの設定
	D3DVIEWPORT9 vp;
	vp.X = x;
	vp.Y = y;
	vp.Width = width;
	vp.Height = height;
	vp.MinZ = m_state.z_min;
	vp.MaxZ = m_state.z_max;
	hr = m_device->SetViewport(&vp);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("SetViewport"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// デバイスを選択
// ================================================================
bool C_d3d_device::select_device()
{
	HRESULT hr;

	// デバイスは HAL を使う
	m_state.device_type = D3DDEVTYPE_HAL;

	// デバイスをチェック
	hr = G_d3d->GetDeviceCaps(m_state.display_adapter, m_state.device_type, &m_state.device_caps);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("HAL デバイス情報が取得できませんでした。\nご使用中のビデオカードが DirectX 9.0 に対応していません。または無効になっています。\nこのアプリケーションを起動するには、DirectX 9.0 に完全対応したビデオカードが必要です。"), _T("GetDeviceCaps"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// デバイスをチェック
// ================================================================
bool C_d3d_device::check_device()
{
	// ディスプレイとバックバッファのフォーマットのチェック
	if (!check_adapter_format())
		return false;
	// 深度／ステンシルバッファのフォーマットのチェック
	if (!check_depth_stencil_format())
		return false;
	// レンダリングテクスチャのフォーマットのチェック
	if (!check_render_texture_format())
		return false;
	// フルシーンアンチエイリアシングのチェック
	if (!check_multisample_type())
		return false;
	// ディスプレイモードのチェック
	if (!check_display_mode())
		return false;
	// デバイスの能力のチェック
	if (!check_device_caps())
		return false;


	return true;
}

// テクスチャに D3DUSAGE_AUTOGENMIPMAP が使えるかを調べる
bool C_d3d_device::can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture()
{
	if (!m_state.mipmap_use_flag)
		return false;	// ミップマップを使わない
	if (!(m_state.device_caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP))
		return false;	// ミップマップの自動生成ができない
	if (FAILED(m_state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture))
		return false;	// X8R8G8B8 テクスチャに対してミップマップが自動生成できない
	if (FAILED(m_state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture))
		return false;	// A8R8G8B8 テクスチャに対してミップマップが自動生成できない

	return true;
}

// テクスチャに D3DUSAGE_AUTOGENMIPMAP を使ってミップマップが作られるかどうかを調べる
bool C_d3d_device::can_enable_D3DUSAGE_AUTOGENMIPMAP_for_texture()
{
	if (!m_state.mipmap_use_flag)
		return false;	// ミップマップを使わない
	if (!can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture())
		return false;	// ミップマップが自動生成できない
	if (m_state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture == D3DOK_NOAUTOGEN)
		return false;	// ミップマップの自動生成に対応しているがミップマップは作られない
	if (m_state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture == D3DOK_NOAUTOGEN)
		return false;	// ミップマップの自動生成に対応しているがミップマップは作られない

	return true;
}

// テクスチャに D3DPTFILTERCAPS_MIPFPOINT が使えるかどうかを調べる
bool C_d3d_device::can_set_D3DPTFILTERCAPS_MIPFPOINT_for_texture()
{
	if (!m_state.mipmap_use_flag)
		return false;	// ミップマップを使わない
	if (!can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture())
		return false;	// ミップマップが自動生成できない
	if (!(m_state.device_caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFPOINT))
		return false;	// ミップマップの POINT 補完に対応していない

	return true;
}

// テクスチャに D3DPTFILTERCAPS_MIPFLINEAR が使えるかどうかを調べる
bool C_d3d_device::can_set_D3DPTFILTERCAPS_MIPFLINEAR_for_texture()
{
	if (!m_state.mipmap_use_flag)
		return false;	// ミップマップを使わない
	if (!can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture())
		return false;	// ミップマップが自動生成できない
	if (FAILED(m_state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture))
		return false;	// リソースに対して D3DTEXF_POINT 以外に対応していない
	if (FAILED(m_state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture))
		return false;	// リソースに対して D3DTEXF_POINT 以外に対応していない
	if (!(m_state.device_caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR))
		return false;	// ミップマップの LINEAR 補完に対応していない

	return true;
}

// 2^n 以外のテクスチャが使えるかどうかを調べる
bool C_d3d_device::can_use_not_pow2_texture()
{
	if (m_state.pow2_limit_flag)
		return false;	// フラグで制限されているので使えない
	if (m_state.device_caps.TextureCaps & D3DPTEXTURECAPS_POW2)
		return false;	// ハードウェアの制限で使えない

	return true;
}

bool C_d3d_device::set_render_target(DWORD rt_idx, IDirect3DSurface9* rt)
{
	HRESULT hr;
	// レンダーターゲット
	//if(rt) {			// NULL渡しでレンダーターゲット解除
		hr = G_d3d.device.body()->SetRenderTarget(rt_idx, rt);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("SetRenderTarget"), hr);
			return false;
		}
	//}
	return true;
}
bool C_d3d_device::set_depth_stencil_surface(IDirect3DSurface9* dss)
{
	HRESULT hr;
	// 深度ステンシル
	if(dss) {
		hr = G_d3d.device.body()->SetDepthStencilSurface(dss);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("SetDepthStencilSurface"), hr);
			return false;
		}
	}
	return true;
}
bool C_d3d_device::set_viewport(D3DVIEWPORT9* vp)
{
	HRESULT hr;
	// ビューポート
	if(vp) {
		hr = G_d3d.device.body()->SetViewport(vp);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("SetViewport"), hr);
			return false;
		}
	}
	return true;
}

bool C_d3d_device::get_render_target(DWORD rt_idx, IDirect3DSurface9** rt)
{
	HRESULT hr;
	// レンダーターゲット
	hr = G_d3d.device.body()->GetRenderTarget(rt_idx, rt);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("GetRenderTarget"), hr);
		return false;
	}
	return true;
}
bool C_d3d_device::get_depth_stencil_surface(IDirect3DSurface9** dss)
{
	HRESULT hr;
	// 深度ステンシル
	hr = G_d3d.device.body()->GetDepthStencilSurface(dss);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("GetDepthStencilSurface"), hr);
		return false;
	}

	return true;
}
bool C_d3d_device::get_viewport(D3DVIEWPORT9* vp)
{
	HRESULT hr;
	// ビューポート
	hr = G_d3d.device.body()->GetViewport(vp);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("GetViewport"), hr);
		return false;
	}
	return true;
}



// ****************************************************************
// ディスプレイとバックバッファのフォーマットのチェック
// ================================================================
bool C_d3d_device::check_adapter_format()
{
	HRESULT hr;

	// ウインドウモード
	hr = G_d3d->CheckDeviceType(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, m_state.pp_window.BackBufferFormat, TRUE);
	if (FAILED(hr))	{
		set_last_error_d3d(TSTR()
			+ _T("ウィンドウモード：ディスプレイが、指定されたバックバッファのフォーマットに対応していません。")
			+ _T("\n\nディスプレイの色数が 16bit になっていませんか？\n24bit / 32bit にすることでこの問題は解決する可能性があります。")
			+ _T("\n\ndisplay = ") + get_str_D3DFORMAT(m_state.display_mode.Format) + _T(", back_buffer = ") + get_str_D3DFORMAT(m_state.pp_window.BackBufferFormat), _T("CheckDeviceType"), hr);
		return false;
	}

	// フルスクリーンモード
	hr = G_d3d->CheckDeviceType(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, m_state.pp_full.BackBufferFormat, FALSE);
	if (FAILED(hr))	{
		set_last_error_d3d(TSTR()
			+ _T("フルスクリーンモード：ディスプレイが、指定されたバックバッファのフォーマットに対応していません。")
			+ _T("\n\nディスプレイの色数が 16bit になっていませんか？\n24bit / 32bit にすることでこの問題は解決する可能性があります。")
			+ _T("\n\ndisplay = ") + get_str_D3DFORMAT(m_state.pp_full.BackBufferFormat) + _T(", back_buffer = ") + get_str_D3DFORMAT(m_state.pp_full.BackBufferFormat), _T("CheckDeviceType"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// 深度／ステンシルバッファのフォーマットのチェック
// ================================================================
bool C_d3d_device::check_depth_stencil_format()
{
	HRESULT hr;

	// ウインドウモード
	if (m_state.pp_window.EnableAutoDepthStencil)	{

		// ディスプレイフォーマットに適応するかどうか判定
		hr = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_state.pp_window.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("ウィンドウモード：ディスプレイが、指定された深度／ステンシルバッファのフォーマットに対応していません。"), _T("CheckDeviceFormat"), hr);
			return false;
		}

		// レンダリングターゲット（ここではバックバッファを指す）と組み合わせて使えるかどうか判定
		hr = G_d3d->CheckDepthStencilMatch(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, m_state.pp_window.BackBufferFormat, m_state.pp_window.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("ウィンドウモード：バックバッファと、深度／ステンシルバッファのフォーマットの組み合わせが不正です。"), _T("CheckDepthStencilMatch"), hr);
			return false;
		}
	}

	// フルスクリーンモード
	if (m_state.pp_full.EnableAutoDepthStencil)	{

		// ディスプレイフォーマットに適応するかどうか判定
		hr = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_state.pp_full.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("フルスクリーンモード：ディスプレイが、指定された深度／ステンシルバッファのフォーマットに対応していません。"), _T("CheckDeviceFormat"), hr);
			return false;
		}

		// レンダリングターゲット（ここではバックバッファを指す）と組み合わせて使えるかどうか判定
		hr = G_d3d->CheckDepthStencilMatch(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, m_state.pp_full.BackBufferFormat, m_state.pp_full.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("フルスクリーンモード：バックバッファと、深度／ステンシルバッファのフォーマットの組み合わせが不正です。"), _T("CheckDepthStencilMatch"), hr);
			return false;
		}
	}

	return true;
}

// ****************************************************************
// レンダリングテクスチャのフォーマットのチェック
// ================================================================
bool C_d3d_device::check_render_texture_format()
{
	HRESULT hr;

	// ウインドウモード

	// レンダーターゲットとなるテクスチャ
	hr = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, m_state.fmt_render_texture_window);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("ウィンドウモード：ディスプレイが、レンダーターゲットのテクスチャに指定されたフォーマットに対応していません。"), _T("CheckDeviceFormat"), hr);
		return false;
	}

	// 深度／ステンシルバッファと組み合わせる
	if (m_state.pp_window.EnableAutoDepthStencil)	{

		hr = G_d3d->CheckDepthStencilMatch(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, m_state.fmt_render_texture_window, m_state.pp_window.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("ウィンドウモード：レンダーターゲットのテクスチャと、深度／ステンシルバッファのフォーマットの組み合わせが不正です。"), _T("CheckDepthStencilMatch"), hr);
			return false;
		}
	}

	// フルスクリーンモード

	// レンダーターゲットとなるテクスチャ
	hr = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, m_state.fmt_render_texture_full);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("フルスクリーンモード：ディスプレイが、レンダーターゲットのテクスチャに指定されたフォーマットに対応していません。"), _T("CheckDeviceFormat"), hr);
		return false;
	}

	// 深度／ステンシルバッファと組み合わせる
	if (m_state.pp_full.EnableAutoDepthStencil)	{

		hr = G_d3d->CheckDepthStencilMatch(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, m_state.fmt_render_texture_full, m_state.pp_full.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("フルスクリーンモード：レンダーターゲットのテクスチャと、深度／ステンシルバッファのフォーマットの組み合わせが不正です。"), _T("CheckDepthStencilMatch"), hr);
			return false;
		}
	}

	return true;
}

// ****************************************************************
// マルチサンプリングタイプのチェック
// ================================================================
bool C_d3d_device::check_multisample_type()
{
	HRESULT hr;

	// ウィンドウモード

	// バックバッファに対応しているかどうかを判定
	hr = G_d3d->CheckDeviceMultiSampleType(m_state.display_adapter, m_state.device_type, m_state.pp_window.BackBufferFormat, TRUE, m_state.pp_window.MultiSampleType, NULL);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("ウィンドウモード：指定されたマルチサンプリングタイプが、指定されたバックバッファのフォーマットに適応していません。"), _T("CheckDeviceMultiSampleType"), hr);
		return false;
	}

	// 深度／ステンシルバッファに対応しているかどうかを判定
	if (m_state.pp_window.EnableAutoDepthStencil)	{

		hr = G_d3d->CheckDeviceMultiSampleType(m_state.display_adapter, m_state.device_type, m_state.pp_window.AutoDepthStencilFormat, TRUE, m_state.pp_window.MultiSampleType, NULL);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("ウィンドウモード：指定されたマルチサンプリングタイプが、指定された深度／ステンシルバッファのフォーマットに適応していません。"), _T("CheckDeviceMultiSampleType"), hr);
			return false;
		}
	}

	// フルスクリーンモード

	// バックバッファに対応しているかどうかを判定
	hr = G_d3d->CheckDeviceMultiSampleType(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, FALSE, m_state.pp_full.MultiSampleType, NULL);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("フルスクリーンモード：指定されたマルチサンプリングタイプが、指定されたバックバッファのフォーマットに適応していません。"), _T("CheckDeviceMultiSampleType"), hr);
		return false;
	}

	// 深度／ステンシルバッファに対応しているかどうかを判定
	if (m_state.pp_window.EnableAutoDepthStencil)	{

		hr = G_d3d->CheckDeviceMultiSampleType(m_state.display_adapter, m_state.device_type, m_state.pp_full.AutoDepthStencilFormat, FALSE, m_state.pp_full.MultiSampleType, NULL);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("フルスクリーンモード：指定されたマルチサンプリングタイプが、指定された深度／ステンシルバッファのフォーマットに適応していません。"), _T("CheckDeviceMultiSampleType"), hr);
			return false;
		}
	}

	return true;
}

// ****************************************************************
// ディスプレイモードのチェック
// ================================================================
bool C_d3d_device::check_display_mode()
{
	// ウインドウモード

		// 調べる必要なし

	// フルスクリーンモード
	int w = m_state.full_size.cx;
	int h = m_state.full_size.cy;

	D3DDISPLAYMODE dmode = { 0, 0, 0, D3DFMT_UNKNOWN };
	int level = 1000000;
	int num = G_d3d->GetAdapterModeCount(m_state.display_adapter, m_state.pp_full.BackBufferFormat);
	for (int i = 0; i < num; i++)	{

		// ディスプレイモードの列挙
		D3DDISPLAYMODE dm;
		G_d3d->EnumAdapterModes(m_state.display_adapter, m_state.pp_full.BackBufferFormat, i, &dm);
		// 希望のモードとの誤差
		int l = abs((int)(w - dm.Width)) + abs((int)(h - dm.Height));
		if (l < level)	{
			// より適切なモードを選択
			dmode = dm;
			level = l;
		}
	}
	if (dmode.Format == D3DFMT_UNKNOWN)	{
		set_last_error(_T("フルスクリーン：適切なディスプレイモードが見つかりませんでした。"), _T(""));
		return false;
	}

	m_state.pp_full.BackBufferWidth  = dmode.Width;
	m_state.pp_full.BackBufferHeight = dmode.Height;
	m_state.pp_full.FullScreen_RefreshRateInHz = dmode.RefreshRate;//D3DPRESENT_RATE_DEFAULT;	// デフォルト

	return true;
}

// ****************************************************************
// デバイスの能力のチェック
// ================================================================
bool C_d3d_device::check_device_caps()
{
	// ピクセルシェーダに対応していないグラボの場合はここでエラーを返します。
	if (m_state.device_caps.PixelShaderVersion < D3DPS_VERSION(2, 0))	{
		set_last_error(_T("ご使用のビデオカードがピクセルシェーダ 2.0 に対応していません。\nこのアプリケーションを起動するには、ピクセルシェーダ 2.0 に対応したビデオカードが必要です。"), _T(""));
		return false;
	}

	// 2048×2048 のテクスチャが必要です。
	if (m_state.device_caps.MaxTextureWidth < 2048 || m_state.device_caps.MaxTextureHeight < 2048)	{
		set_last_error(_T("作成できる最大テクスチャサイズが小さすぎます。\n現在=") + tostr(m_state.device_caps.MaxTextureWidth) + _T(",") + tostr(m_state.device_caps.MaxTextureHeight) + _T("必要=2048×2048"), _T(""));
		return false;
	}

	//
	// ミップマップの対応状況を調べる
	//

	// ミップマップの自動生成に対応しているか
	if (!(m_state.device_caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP))	{
		// error_box(_T("D3DCAPS2_CANAUTOGENMIPMAP に対応していません。"));
		// 旧岩谷さんマシンで対応していない。これの対応をどうするか…。D3DOK_NOAUTOGEN ならエラー処理いらんのとちゃう？
	}
	// D3DFMT_X8R8G8B8 のテクスチャでミップマップが自動生成できるかどうか
	m_state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE, D3DFMT_X8R8G8B8);
	if (FAILED(m_state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture))	{
		// error_box(_T("D3DUSAGE_AUTOGENMIPMAP に対応していません。") + create_my_last_error_d3d(state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8));
		// ここを通るマシンはあるのかなぁ…。あるなら AUTOGEN_ を切らなければいけない。
	}
	else if (m_state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture == D3DOK_NOAUTOGEN)	{
		// error_box(_T("D3DUSAGE_AUTOGENMIPMAP に対応してますが、ミップマップは作られません。") + create_my_last_error_d3d(state.hr_D3DUSAGE_AUTOGENMIPMAP));
		// 旧岩谷さんマシンで対応していない。汚くなるだけでエラーは出ない。エラーログに出すだけにしよう。
	}
	// D3DFMT_A8R8G8B8 のテクスチャでミップマップが自動生成できるかどうか
	m_state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8);
	if (FAILED(m_state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture))	{
		// error_box(_T("D3DUSAGE_AUTOGENMIPMAP に対応していません。") + create_my_last_error_d3d(state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8));
		// ここを通るマシンはあるのかなぁ…。あるなら AUTOGEN_ を切らなければいけない。
	}
	else if (m_state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture == D3DOK_NOAUTOGEN)	{
		// error_box(_T("D3DUSAGE_AUTOGENMIPMAP に対応してますが、ミップマップは作られません。") + create_my_last_error_d3d(state.hr_D3DUSAGE_AUTOGENMIPMAP));
		// 旧岩谷さんマシンで対応していない。汚くなるだけでエラーは出ない。エラーログに出すだけにしよう。
	}

	// ↑ここまででエラーが出るようならミップマップフィルタを D3DTEXF_NONE にしないといけない。

	// D3DFMT_X8R8G8B8 のテクスチャに対して D3DTEXF_POINT 以外のフィルタリングに対応しているかどうか
	m_state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_QUERY_FILTER, D3DRTYPE_TEXTURE, D3DFMT_X8R8G8B8);
	if (FAILED(m_state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture))	{
		// error_box(_T("QUERY_FILTER に対応していません。") + create_my_last_error_d3d(state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8));
		// ここを通るグラボはないっぽい？対応していなければ全て POINT にしなければいけない。
	}
	// D3DFMT_A8R8G8B8 のテクスチャに対して D3DTEXF_POINT 以外のフィルタリングに対応しているかどうか
	m_state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_QUERY_FILTER, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8);
	if (FAILED(m_state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture))	{
		// error_box(_T("QUERY_FILTER に対応していません。") + create_my_last_error_d3d(state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8));
		// ここを通るグラボはないっぽい？対応していなければ全て POINT にしなければいけない。
	}
	// ミップマップに対して D3DTEXF_LINEAR に対応しているかどうか
	if (!(m_state.device_caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR))	{
		// error_box(_T("D3DPTFILTERCAPS_MIPFLINEAR に対応していません。"));
		// ここを通るグラボはないっぽい？対応していなければ MIPPOINT にしよう。
	}

	return true;
}

// ****************************************************************
// デバイスを作成
// ================================================================
bool C_d3d_device::create_device()
{
	HRESULT hr;

	IDirect3DDevice9* p_devide = NULL;

#if 1
	// 頂点処理をハードウェアで行なってみる
	if (m_state.vertex_process_type == D3DCREATE_HARDWARE_VERTEXPROCESSING)	{
		hr = G_d3d->CreateDevice(m_state.display_adapter, m_state.device_type, m_state.h_wnd, m_state.vertex_process_type, &m_state.pp_window, &p_devide);
		if (FAILED(hr))	{
			m_state.vertex_process_type = D3DCREATE_SOFTWARE_VERTEXPROCESSING;	// 失敗したのでソフトウェアを試みる
		}
	}
	// 頂点処理をソフトウェアで行なってみる
	if (m_state.vertex_process_type == D3DCREATE_SOFTWARE_VERTEXPROCESSING)	{
		hr = G_d3d->CreateDevice(m_state.display_adapter, m_state.device_type, m_state.h_wnd, m_state.vertex_process_type, &m_state.pp_window, &p_devide);
		if (FAILED(hr))	{

			// デバイスの作成に失敗
			set_last_error_d3d(_T("CreateDevice"), hr);
			return false;
		}
	}
#else
	// 頂点処理をハードウェアで行なってみる
	m_state.vertex_process_type = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	hr = G_d3d->CreateDevice(m_state.display_adapter, m_state.device_type, m_state.h_wnd, m_state.vertex_process_type, &m_state.pp_window, &p_devide);
	if (FAILED(hr))	{

		// 頂点処理をソフトウェアで行なってみる
		m_state.vertex_process_type = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		hr = G_d3d->CreateDevice(m_state.display_adapter, m_state.device_type, m_state.h_wnd, m_state.vertex_process_type, &m_state.pp_window, &p_devide);
		if (FAILED(hr))	{

			// デバイスの作成に失敗
			set_last_error_d3d(_T("CreateDevice"), hr);
			return false;
		}
	}
#endif

	m_device = BIP<IDirect3DDevice9>(p_devide, false);

	return true;
}

// ****************************************************************
// デバイスをチェック
// ================================================================
bool C_d3d_device::check_device_lost()
{
	HRESULT hr;

	// デバイスが準備できていない場合
	// ロストしているわけではないので成功を返す
	if (!m_device)
		return true;

	// デバイス消失フラグを一旦リセットする
	m_state.device_lost = false;
	m_state.device_can_reset = false;

	// デバイスの状態をチェック
	hr = m_device->TestCooperativeLevel();
	if (FAILED(hr))	{

		// デバイスが消失していてリセットできない
		if (hr == D3DERR_DEVICELOST)	{
			m_state.device_lost = true;
			return true;
		}

		// デバイスが消失しているがリセットできる
		if (hr == D3DERR_DEVICENOTRESET)	{
			m_state.device_lost = true;
			m_state.device_can_reset = true;
			return true;
		}

		// 予期せぬエラー
		return false;
	}

	return true;
}

// ****************************************************************
// デバイスを復元
// ================================================================
bool C_d3d_device::restore()
{
	HRESULT hr;

	// デバイス消失フラグを一旦リセットする
	m_state.device_lost = false;
	m_state.device_can_reset = false;

	// デバイスが準備できていない場合は失敗
	if (!m_device)
		return false;

	// デバイスの復元を試みる
	hr = m_device->Reset(m_state.p_pp);
	if (FAILED(hr))	{

		// デバイスはまだ消失している
		if (hr == D3DERR_DEVICELOST)	{
			m_state.device_lost = true;
			m_state.device_can_reset = false;
			return true;
		}

		set_last_error_d3d(_T("Reset"), hr);
		return false;
	}

	// ビューポートを初期化
	if (!set_view_port(0, 0, m_state.p_pp->BackBufferWidth, m_state.p_pp->BackBufferHeight))
		return false;

	return true;
}

// ****************************************************************
// ディスプレイへ転送
// ================================================================
bool C_d3d_device::present(C_rect* p_rect)
{
	HRESULT hr;

	// デバイスが準備できていない場合は失敗
	if (!m_device)
		return false;

	// デバイスが消失していれば復元
	if (m_state.device_lost)
		return true;

	// デバイスが消失していなければ描画（ここを elseif にしないこと！）
	if (!m_state.device_lost)	{

		// デバイス消失フラグを一旦リセットする
		m_state.device_lost = false;

		// バックバッファをフロントへ
		hr = m_device->Present(p_rect, p_rect, NULL, NULL);
		// デバイスが消失した場合
		if (hr == D3DERR_DEVICELOST)	{
			m_state.device_lost = true;
			return true;
		}
	}

	return true;
}

// ****************************************************************
// ディスプレイモードの変更
// ================================================================
bool C_d3d_device::change_display_mode(bool is_window)
{
	m_state.is_window = is_window;

	// プレゼントパラメータの設定
	if (m_state.is_window)		m_state.p_pp = &m_state.pp_window;
	else						m_state.p_pp = &m_state.pp_full;

	return true;
}

// ****************************************************************
// スクリーンサイズの変更
// ================================================================
bool C_d3d_device::change_screen_size(C_size size)
{
	m_state.pp_window.BackBufferWidth = size.cx;
	m_state.pp_window.BackBufferHeight = size.cy;

	return true;
}

// ****************************************************************
// 描画間隔の変更
// ================================================================
bool C_d3d_device::change_present_interval(int interval)
{
	m_state.pp_window.PresentationInterval = interval;
	m_state.pp_full.PresentationInterval = interval;

	return true;
}

// ****************************************************************
// レンダーターゲットをクリア
// ================================================================
bool C_d3d_device::clear_render_target(D3DCOLOR back_color)
{
	HRESULT hr;

	// デバイスが準備できていない場合は失敗
	if (!m_device)	{
		set_last_error(_T("デバイスが準備されていません。"), _T("clear_render_target"));
		return false;
	}

	// バックバッファをクリア
	DWORD clear_target = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL;
	hr = G_d3d.device->Clear(0, NULL, clear_target, back_color, 1.0f, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Clear"), hr);
		return false;
	}

	return true;
}

}
