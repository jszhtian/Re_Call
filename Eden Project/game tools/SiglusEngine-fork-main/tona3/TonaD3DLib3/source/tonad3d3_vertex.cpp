#include	"tonad3d3_pch.h"
#include	"tonad3d3_d3d.h"
#include	"tonad3d3_vertex.h"

namespace NT3
{
	
// ****************************************************************
// D3D ���_�o�b�t�@
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
// ���_�o�b�t�@��������
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
// ���_�o�b�t�@���J��
// ================================================================
void C_d3d_vertex_buffer::release()
{
	m_buffer = NULL;
}

// ****************************************************************
// ���_�o�b�t�@���쐬
// ================================================================
bool C_d3d_vertex_buffer::create_vertex_buffer(int vertex_cnt, DWORD FVF, int FVF_size, DWORD usage, D3DPOOL pool)
{
	HRESULT hr;

	int buffer_size = vertex_cnt * FVF_size;

	// ���_�o�b�t�@�̍쐬
	IDirect3DVertexBuffer9* p_buffer = NULL;
	hr = G_d3d.device->CreateVertexBuffer(buffer_size, usage, FVF, pool, &p_buffer, NULL);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("CreateVertexBuffer"), hr);
		return false;
	}

	m_buffer = BIP<IDirect3DVertexBuffer9>(p_buffer, false);

	// �p�����[�^��ݒ�
	m_vertex_cnt = vertex_cnt;
	m_FVF = FVF;
	m_FVF_size = FVF_size;
	m_usage = usage;
	m_pool = pool;

	return true;
}

// ****************************************************************
// ���_�o�b�t�@�����b�N
// ================================================================
bool C_d3d_vertex_buffer::lock(int offset, int size, void** pp_buffer, DWORD flag)
{
	HRESULT hr;

	if (!m_buffer)	{
		set_last_error(_T("���_�o�b�t�@����������Ă��܂���B"), _T("C_d3d_vertex_buffer::lock"));
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
// ���_�o�b�t�@���A�����b�N
// ================================================================
bool C_d3d_vertex_buffer::unlock()
{
	HRESULT hr;

	if (!m_buffer)	{
		set_last_error(_T("���_�o�b�t�@����������Ă��܂���B"), _T("C_d3d_vertex_buffer::unlock"));
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
