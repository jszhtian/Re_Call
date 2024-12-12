#include	"tonad3d3_pch.h"
#include	"tonad3d3_d3d.h"
#include	"tonad3d3_vertex.h"

namespace NT3
{
	
// ****************************************************************
// D3D 頂点バッファ
// ================================================================
C_d3d_vertex_buffer::C_d3d_vertex_buffer()
{
	m_vertex_cnt = 0;
	m_FVF = 0;
	m_FVF_size = 0;
	m_usage = 0;
	m_pool = D3DPOOL_DEFAULT;
}

C_d3d_vertex_buffer::~C_d3d_vertex_buffer()
{
}

// ****************************************************************
// 頂点バッファを初期化
// ================================================================
void C_d3d_vertex_buffer::init()
{
	release();

	m_vertex_cnt = 0;
	m_FVF = 0;
	m_FVF_size = 0;
	m_usage = 0;
	m_pool = D3DPOOL_DEFAULT;
}

// ****************************************************************
// 頂点バッファを開放
// ================================================================
void C_d3d_vertex_buffer::release()
{
	m_buffer = NULL;
}

// ****************************************************************
// 頂点バッファを作成
// ================================================================
bool C_d3d_vertex_buffer::create_vertex_buffer(int vertex_cnt, DWORD FVF, int FVF_size, DWORD usage, D3DPOOL pool)
{
	HRESULT hr;

	int buffer_size = vertex_cnt * FVF_size;

	// 頂点バッファの作成
	IDirect3DVertexBuffer9* p_buffer = NULL;
	hr = G_d3d.device->CreateVertexBuffer(buffer_size, usage, FVF, pool, &p_buffer, NULL);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("CreateVertexBuffer"), hr);
		return false;
	}

	m_buffer = BIP<IDirect3DVertexBuffer9>(p_buffer, false);

	// パラメータを設定
	m_vertex_cnt = vertex_cnt;
	m_FVF = FVF;
	m_FVF_size = FVF_size;
	m_usage = usage;
	m_pool = pool;

	return true;
}

// ****************************************************************
// 頂点バッファをロック
// ================================================================
bool C_d3d_vertex_buffer::lock(int offset, int size, void** pp_buffer, DWORD flag)
{
	HRESULT hr;

	if (!m_buffer)	{
		set_last_error(_T("頂点バッファが準備されていません。"), _T("C_d3d_vertex_buffer::lock"));
		return false;
	}

	hr = m_buffer->Lock(offset, size, pp_buffer, flag);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Lock"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// 頂点バッファをアンロック
// ================================================================
bool C_d3d_vertex_buffer::unlock()
{
	HRESULT hr;

	if (!m_buffer)	{
		set_last_error(_T("頂点バッファが準備されていません。"), _T("C_d3d_vertex_buffer::unlock"));
		return false;
	}

	hr = m_buffer->Unlock();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Unlock"), hr);
		return false;
	}

	return true;
}

}
