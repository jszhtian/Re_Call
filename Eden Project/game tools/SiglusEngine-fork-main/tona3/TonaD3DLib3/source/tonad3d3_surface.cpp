#include	"tonad3d3_pch.h"
#include	"tonad3d3_d3d.h"
#include	"tonad3d3_surface.h"

namespace NT3
{
	
// ****************************************************************
// D3D サーフェス
// ================================================================
C_d3d_surface::C_d3d_surface()
{
	// パラメータを初期化
	m_width = 0;
	m_height = 0;
	m_width_ex = 0;
	m_height_ex = 0;
}

C_d3d_surface::C_d3d_surface(BIP<IDirect3DSurface9> surface)
{
	// 情報を取得
	D3DSURFACE_DESC desc;
	surface->GetDesc(&desc);

	// パラメータを初期化
	m_surface = surface;
	m_width = desc.Width;
	m_height = desc.Height;
	m_width_ex = desc.Width;
	m_height_ex = desc.Height;
}

C_d3d_surface::~C_d3d_surface()
{
}

// ****************************************************************
// サーフェスを初期化
// ================================================================
void C_d3d_surface::init()
{
	// サーフェスを解放
	release();

	// パラメータを初期化
	m_width = 0;
	m_height = 0;
	m_width_ex = 0;
	m_height_ex = 0;
}

// ****************************************************************
// サーフェスを解放
// ================================================================
void C_d3d_surface::release()
{
	m_surface = NULL;
}

// ****************************************************************
// サーフェスを作成
// ================================================================
bool C_d3d_surface::create_surface(int width, int height, D3DFORMAT format, D3DPOOL pool)
{
	HRESULT hr;

	// サーフェスを初期化
	init();

	// パラメータを設定
	m_width = width;
	m_height = height;

	// 大きさを 2^n に調整する。2^n しか作れないグラボ対策。高速化の意味も。
	// 大きさ 1px だと GeForce9600 GT でおかしかった（白くなった）ので、最小 2px にします。
	if (!G_d3d.device.can_use_not_pow2_texture())	{
		for (m_width_ex = 2; m_width_ex < m_width; m_width_ex *= 2);
		for (m_height_ex = 2; m_height_ex < m_height; m_height_ex *= 2);
	}
	else	{
		m_width_ex = std::max(m_width, 2);
		m_height_ex = std::max(m_height, 2);
	}

	// CreateOffscreenPlainSurface で有効なプールは、以下の３つ。
	// D3DPOOL_DEFAULT = 0,
	// D3DPOOL_SYSTEMMEM = 2,
	// D3DPOOL_SCRATCH = 3,

	// D3DPOOL_MANAGED は指定できないので注意。

	IDirect3DSurface9* p_surface = NULL;
	hr = G_d3d.device->CreateOffscreenPlainSurface(m_width_ex, m_height_ex, format, pool, &p_surface, NULL);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("CreateOffscreenPlainSurface"), hr);
		return false;
	}

	m_surface = BIP<IDirect3DSurface9>(p_surface, false);

	return true;
}

bool C_d3d_surface::create_surface(int width, int height)
{
	return create_surface(width, height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM);
}

// ****************************************************************
// 深度ステンシルバッファを作成
// ================================================================
bool C_d3d_surface::create_dpst_buffer(int width, int height)
{
	HRESULT hr;

	// サーフェスを初期化
	init();

	// パラメータを設定
	m_width = width;
	m_height = height;

	// 大きさを 2^n に調整する。2^n しか作れないグラボ対策。高速化の意味も。
	for (m_width_ex = 1; m_width_ex < m_width; m_width_ex *= 2);
	for (m_height_ex = 1; m_height_ex < m_height; m_height_ex *= 2);

	IDirect3DSurface9* p_surface = NULL;
	hr = G_d3d.device->CreateDepthStencilSurface(m_width_ex, m_height_ex, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &p_surface, NULL);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("CreateDepthStencilSurface"), hr);
		return false;
	}

	m_surface = BIP<IDirect3DSurface9>(p_surface, false);

	return true;
}

bool C_d3d_surface::lock_rect(D3DLOCKED_RECT* texture_bit, C_rect* rect, DWORD flags)
{
	HRESULT hr;

	hr = m_surface->LockRect(texture_bit, rect, flags);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("LockRect"), hr);
		return false;
	}

	return true;
}

bool C_d3d_surface::unlock_rect()
{
	HRESULT hr;

	hr = m_surface->UnlockRect();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("UnlockRect"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// D3D バッファ
// ================================================================
Cd3d_buffer::Cd3d_buffer()
{
}

Cd3d_buffer::Cd3d_buffer(BIP<ID3DXBuffer> buffer)
{
	m_buffer = buffer;
}

Cd3d_buffer::~Cd3d_buffer()
{
}

// ****************************************************************
// バッファを初期化
// ================================================================
void Cd3d_buffer::init()
{
	release();
}

// ****************************************************************
// バッファを解放
// ================================================================
void Cd3d_buffer::release()
{
	m_buffer = NULL;
}



}
