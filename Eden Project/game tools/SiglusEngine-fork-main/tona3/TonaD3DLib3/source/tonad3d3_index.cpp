#include	"tonad3d3_pch.h"
#include	"tonad3d3_d3d.h"
#include	"tonad3d3_index.h"

namespace NT3
{
	
// ****************************************************************
// D3D インデックスバッファ
// ================================================================
C_d3d_index_buffer::C_d3d_index_buffer()
{
	m_index_cnt = 0;
	m_usage = 0;
	m_pool = D3DPOOL_DEFAULT;
}

C_d3d_index_buffer::~C_d3d_index_buffer()
{
}

// ****************************************************************
// インデックスバッファを初期化
// ================================================================
void C_d3d_index_buffer::init()
{
	release();

	m_index_cnt = 0;
	m_usage = 0;
	m_pool = D3DPOOL_DEFAULT;
}

// ****************************************************************
// インデックスバッファを解放
// ================================================================
void C_d3d_index_buffer::release()
{
	m_buffer = NULL;
}

// ****************************************************************
// インデックスバッファを作成
// ================================================================
bool C_d3d_index_buffer::create_index_buffer(int index_cnt, DWORD usage, D3DPOOL pool)
{
	HRESULT hr;

	// インデックスバッファの作成
	IDirect3DIndexBuffer9* p_buffer = NULL;
	hr = G_d3d.device->CreateIndexBuffer(index_cnt * sizeof(WORD), usage, D3DFMT_INDEX16, pool, &p_buffer, NULL);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("CreateIndexBuffer"), hr);
		return false;
	}

	m_buffer = BIP<IDirect3DIndexBuffer9>(p_buffer, false);

	// パラメータの設定
	m_index_cnt = index_cnt;
	m_usage = usage;
	m_pool = pool;

	return true;
}

// ****************************************************************
// インデックスバッファをロック
// ================================================================
bool C_d3d_index_buffer::lock(int offset, int size, void** pp_buffer, DWORD flag)
{
	HRESULT hr;

	if (!m_buffer)	{
		set_last_error(_T("インデックスバッファが準備されていません。"), _T("C_d3d_index_buffer::lock"));
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
// インデックスバッファをアンロック
// ================================================================
bool C_d3d_index_buffer::unlock()
{
	HRESULT hr;

	if (!m_buffer)	{
		set_last_error(_T("インデックスバッファが準備されていません。"), _T("C_d3d_index_buffer::unlock"));
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
