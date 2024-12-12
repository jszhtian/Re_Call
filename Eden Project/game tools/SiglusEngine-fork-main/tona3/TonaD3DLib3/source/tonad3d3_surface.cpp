#include	"tonad3d3_pch.h"
#include	"tonad3d3_d3d.h"
#include	"tonad3d3_surface.h"

namespace NT3
{
	
// ****************************************************************
// D3D �T�[�t�F�X
// ================================================================
C_d3d_surface::C_d3d_surface()
{
	// �p�����[�^��������
	m_width = 0;
	m_height = 0;
	m_width_ex = 0;
	m_height_ex = 0;
}

C_d3d_surface::C_d3d_surface(BIP<IDirect3DSurface9> surface)
{
	// �����擾
	D3DSURFACE_DESC desc;
	surface->GetDesc(&desc);

	// �p�����[�^��������
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
// �T�[�t�F�X��������
// ================================================================
void C_d3d_surface::init()
{
	// �T�[�t�F�X�����
	release();

	// �p�����[�^��������
	m_width = 0;
	m_height = 0;
	m_width_ex = 0;
	m_height_ex = 0;
}

// ****************************************************************
// �T�[�t�F�X�����
// ================================================================
void C_d3d_surface::release()
{
	m_surface = NULL;
}

// ****************************************************************
// �T�[�t�F�X���쐬
// ================================================================
bool C_d3d_surface::create_surface(int width, int height, D3DFORMAT format, D3DPOOL pool)
{
	HRESULT hr;

	// �T�[�t�F�X��������
	init();

	// �p�����[�^��ݒ�
	m_width = width;
	m_height = height;

	// �傫���� 2^n �ɒ�������B2^n �������Ȃ��O���{�΍�B�������̈Ӗ����B
	// �傫�� 1px ���� GeForce9600 GT �ł������������i�����Ȃ����j�̂ŁA�ŏ� 2px �ɂ��܂��B
	if (!G_d3d.device.can_use_not_pow2_texture())	{
		for (m_width_ex = 2; m_width_ex < m_width; m_width_ex *= 2);
		for (m_height_ex = 2; m_height_ex < m_height; m_height_ex *= 2);
	}
	else	{
		m_width_ex = std::max(m_width, 2);
		m_height_ex = std::max(m_height, 2);
	}

	// CreateOffscreenPlainSurface �ŗL���ȃv�[���́A�ȉ��̂R�B
	// D3DPOOL_DEFAULT = 0,
	// D3DPOOL_SYSTEMMEM = 2,
	// D3DPOOL_SCRATCH = 3,

	// D3DPOOL_MANAGED �͎w��ł��Ȃ��̂Œ��ӁB

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
// �[�x�X�e���V���o�b�t�@���쐬
// ================================================================
bool C_d3d_surface::create_dpst_buffer(int width, int height)
{
	HRESULT hr;

	// �T�[�t�F�X��������
	init();

	// �p�����[�^��ݒ�
	m_width = width;
	m_height = height;

	// �傫���� 2^n �ɒ�������B2^n �������Ȃ��O���{�΍�B�������̈Ӗ����B
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
// D3D �o�b�t�@
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
// �o�b�t�@��������
// ================================================================
void Cd3d_buffer::init()
{
	release();
}

// ****************************************************************
// �o�b�t�@�����
// ================================================================
void Cd3d_buffer::release()
{
	m_buffer = NULL;
}



}
