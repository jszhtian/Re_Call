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
	// �p�����[�^��������
	ZeroMemory(&m_state.pp_window, sizeof(m_state.pp_window));
	ZeroMemory(&m_state.pp_full, sizeof(m_state.pp_full));
	m_state.device_lost = false;
}

C_d3d_device::~C_d3d_device()
{
}

// ****************************************************************
// �f�o�C�X��������
// ================================================================
void C_d3d_device::init()
{
	// �f�o�C�X�����
	release();

	// �p�����[�^��������
	ZeroMemory(&m_state.pp_window, sizeof(m_state.pp_window));
	ZeroMemory(&m_state.pp_full, sizeof(m_state.pp_full));
	m_state.device_lost = false;
}

// ****************************************************************
// �f�o�C�X�����
// ================================================================
void C_d3d_device::release()
{
	m_device = NULL;
}

// ****************************************************************
// �f�o�C�X���쐬
// ================================================================
//		adapter_no:
//			D3DADAPTER_DEFAULT:				�v���C�}���i�f�t�H���g�j
//			1 �ȏ�:							�Z�J���_���A�c
// ================================================================
//		prenset_interval:
//			D3DPRESENT_INTERVAL_IMMEDIATE:	����������҂��Ȃ�
//			D3DPRESENT_INTERVAL_ONE:		�����������P�x�����҂�
// ================================================================

#if 0
bool C_d3d_device::create(HWND h_wnd, bool is_window, int width, int height, int adapter_no, int present_interval, bool mipmap_use_flag)
{
	HRESULT hr;

	// �f�o�C�X������������
	init();

	// �p�����[�^�̐ݒ�
	m_state.h_wnd = h_wnd;
	m_state.is_window = is_window;
	m_state.full_size.cx = width;
	m_state.full_size.cy = height;
	m_state.z_min = 0.0f;
	m_state.z_max = 1.0f;
	m_state.mipmap_use_flag = mipmap_use_flag;

	// �g����A�_�v�^�̐��𒲂ׂ�
	int adapter_cnt = (int)G_d3d->GetAdapterCount();
	if (adapter_no < 0 || adapter_no >= adapter_cnt)
		adapter_no = D3DADAPTER_DEFAULT;	// �͈͊O�̃A�_�v�^���w�肵���ꍇ�̓f�t�H���g�̃A�_�v�^���g�p����

	// �g�p����A�_�v�^��ݒ�
	m_state.display_adapter = adapter_no;

	// �v���C�}���̃f�B�X�v���C�A�_�v�^���g�p
	// ���}���`���j�^�̏ꍇ�́A��ɗ񋓂��đI������K�v������܂��B��������
//	m_state.display_adapter = D3DADAPTER_DEFAULT;

	// ���݂̃f�B�X�v���C���[�h���擾
	hr = G_d3d->GetAdapterDisplayMode(m_state.display_adapter, &m_state.display_mode);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�f�B�X�v���C���[�h�̎擾�Ɏ��s���܂����B"), _T("GetAdapterDisplayMode"), hr);
		return false;
	}

	// �v���[���g�p�����[�^�����ݒ�
	ZeroMemory(&m_state.pp_window, sizeof(m_state.pp_window));
	ZeroMemory(&m_state.pp_full, sizeof(m_state.pp_full));
	m_state.pp_window.BackBufferCount			= 1;									// �o�b�N�o�b�t�@��
	m_state.pp_window.BackBufferFormat			= D3DFMT_X8R8G8B8;						// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
	m_state.pp_window.SwapEffect				= D3DSWAPEFFECT_DISCARD;				// �X���b�v�G�t�F�N�g
	m_state.pp_window.EnableAutoDepthStencil	= TRUE;									// �[�x�^�X�e���V���o�b�t�@���g��
	m_state.pp_window.AutoDepthStencilFormat	= D3DFMT_D24S8;							// �[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	m_state.pp_window.MultiSampleType			= D3DMULTISAMPLE_NONE;					// �}���`�T���v�����O���g��Ȃ��i�g���Ċi�i���Y��ɂȂ�Ȃ�g���\��j
	m_state.pp_window.PresentationInterval		= present_interval;						// D3DPRESENT_INTERVAL_IMMEDIATE / D3DPRESENT_INTERVAL_ONE
//	m_state.pp_window.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;	// �[�x�X�e���V���o�b�t�@��j���ł���i���ɂ���Ă͑����Ȃ�炵���j
	m_state.pp_full = m_state.pp_window;												// �t���X�N���[�����[�h�ɃE�B���h�E���[�h�̃p�����[�^���R�s�[

	m_state.pp_window.Windowed					= TRUE;					// �E�B���h�E���[�h
	m_state.pp_window.hDeviceWindow				= m_state.h_wnd;		// �E�B���h�E�n���h��
	m_state.pp_full.Windowed					= FALSE;				// �t���X�N���[�����[�h
	m_state.fmt_render_texture_window			= D3DFMT_X8R8G8B8;		// �E�B���h�E���[�h�̃����_�[�^�[�Q�b�g�̃t�H�[�}�b�g�iMirage 3 �ɂ����āAD3DFMT_A8R8G8B8 ���Ɛ���ɕ`�悳��Ȃ��j
	m_state.fmt_render_texture_full				= D3DFMT_X8R8G8B8;		// �t���X�N���[�����[�h�̃����_�[�^�[�Q�b�g�̃t�H�[�}�b�g�iMirage 3 �ɂ����āAD3DFMT_A8R8G8B8 ���Ɛ���ɕ`�悳��Ȃ��j

	// �f�o�C�X�̑I��
	if (!select_device())
		return false;

	// �f�o�C�X�̃`�F�b�N
	if (!check_device())
		return false;

	// ���݂̃v���[���g�p�����[�^�̐ݒ�
	m_state.p_pp = m_state.is_window ? &m_state.pp_window : &m_state.pp_full;

	// �f�o�C�X�̍쐬
	if (!create_device())
		return false;

	// �r�f�I�������̍ő�e�ʂ��擾
	m_state.vm_size = m_device->GetAvailableTextureMem();

	// �r���[�|�[�g��������
	if (!set_view_port(0, 0, m_state.p_pp->BackBufferWidth, m_state.p_pp->BackBufferHeight))
		return false;

	// �f�o�C�X�̏������ɐ���
	return true;
}
#endif


bool C_d3d_device::create(HWND h_wnd, bool is_window, int width, int height, int shadow_map_size, int adapter_no, DWORD vertex_process_type, int present_interval, bool mipmap_use_flag, bool pow2_limit_flag)
{
	HRESULT hr;

	// �f�o�C�X������������
	init();

	// �p�����[�^�̐ݒ�
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

	// �g����A�_�v�^�̐��𒲂ׂ�
	int adapter_cnt = (int)G_d3d->GetAdapterCount();
	if (adapter_no < 0 || adapter_no >= adapter_cnt)
		adapter_no = D3DADAPTER_DEFAULT;	// �͈͊O�̃A�_�v�^���w�肵���ꍇ�̓f�t�H���g�̃A�_�v�^���g�p����

	// �g�p����A�_�v�^��ݒ�
	m_state.display_adapter = adapter_no;

	// �v���C�}���̃f�B�X�v���C�A�_�v�^���g�p
	// ���}���`���j�^�̏ꍇ�́A��ɗ񋓂��đI������K�v������܂��B��������
//	m_state.display_adapter = D3DADAPTER_DEFAULT;

	// ���݂̃f�B�X�v���C���[�h���擾
	hr = G_d3d->GetAdapterDisplayMode(m_state.display_adapter, &m_state.display_mode);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�f�B�X�v���C���[�h�̎擾�Ɏ��s���܂����B"), _T("GetAdapterDisplayMode"), hr);
		return false;
	}

	// �v���[���g�p�����[�^�����ݒ�
	ZeroMemory(&m_state.pp_window, sizeof(m_state.pp_window));
	ZeroMemory(&m_state.pp_full, sizeof(m_state.pp_full));
	m_state.pp_window.BackBufferCount			= 1;									// �o�b�N�o�b�t�@��
	m_state.pp_window.BackBufferFormat			= D3DFMT_X8R8G8B8;						// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
	m_state.pp_window.SwapEffect				= D3DSWAPEFFECT_DISCARD;				// �X���b�v�G�t�F�N�g
	m_state.pp_window.EnableAutoDepthStencil	= TRUE;									// �[�x�^�X�e���V���o�b�t�@���g��
	m_state.pp_window.AutoDepthStencilFormat	= D3DFMT_D24S8;							// �[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	m_state.pp_window.MultiSampleType			= D3DMULTISAMPLE_NONE;					// �}���`�T���v�����O���g��Ȃ��i�g���Ċi�i���Y��ɂȂ�Ȃ�g���\��j
	m_state.pp_window.PresentationInterval		= present_interval;						// D3DPRESENT_INTERVAL_IMMEDIATE / D3DPRESENT_INTERVAL_ONE
//	m_state.pp_window.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;	// �[�x�X�e���V���o�b�t�@��j���ł���i���ɂ���Ă͑����Ȃ�炵���j
	m_state.pp_full = m_state.pp_window;												// �t���X�N���[�����[�h�ɃE�B���h�E���[�h�̃p�����[�^���R�s�[

	m_state.pp_window.Windowed					= TRUE;					// �E�B���h�E���[�h
	m_state.pp_window.hDeviceWindow				= m_state.h_wnd;		// �E�B���h�E�n���h��
	m_state.pp_full.Windowed					= FALSE;				// �t���X�N���[�����[�h
	m_state.fmt_render_texture_window			= D3DFMT_X8R8G8B8;		// �E�B���h�E���[�h�̃����_�[�^�[�Q�b�g�̃t�H�[�}�b�g�iMirage 3 �ɂ����āAD3DFMT_A8R8G8B8 ���Ɛ���ɕ`�悳��Ȃ��j
	m_state.fmt_render_texture_full				= D3DFMT_X8R8G8B8;		// �t���X�N���[�����[�h�̃����_�[�^�[�Q�b�g�̃t�H�[�}�b�g�iMirage 3 �ɂ����āAD3DFMT_A8R8G8B8 ���Ɛ���ɕ`�悳��Ȃ��j

	// �f�o�C�X�̑I��
	if (!select_device())
		return false;

	// �f�o�C�X�̃`�F�b�N
	if (!check_device())
		return false;

	// ���݂̃v���[���g�p�����[�^�̐ݒ�
	m_state.p_pp = m_state.is_window ? &m_state.pp_window : &m_state.pp_full;

	// �f�o�C�X�̍쐬
	if (!create_device())
		return false;

	// �r�f�I�������̍ő�e�ʂ��擾
	m_state.vm_size = m_device->GetAvailableTextureMem();

	// �r���[�|�[�g��������
	if (!set_view_port(0, 0, m_state.p_pp->BackBufferWidth, m_state.p_pp->BackBufferHeight))
		return false;

	// �f�o�C�X�̏������ɐ���
	return true;
}

// ****************************************************************
// �r���[�|�[�g������������
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

	// �r���[�|�[�g�̐ݒ�
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
// �f�o�C�X��I��
// ================================================================
bool C_d3d_device::select_device()
{
	HRESULT hr;

	// �f�o�C�X�� HAL ���g��
	m_state.device_type = D3DDEVTYPE_HAL;

	// �f�o�C�X���`�F�b�N
	hr = G_d3d->GetDeviceCaps(m_state.display_adapter, m_state.device_type, &m_state.device_caps);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("HAL �f�o�C�X��񂪎擾�ł��܂���ł����B\n���g�p���̃r�f�I�J�[�h�� DirectX 9.0 �ɑΉ����Ă��܂���B�܂��͖����ɂȂ��Ă��܂��B\n���̃A�v���P�[�V�������N������ɂ́ADirectX 9.0 �Ɋ��S�Ή������r�f�I�J�[�h���K�v�ł��B"), _T("GetDeviceCaps"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// �f�o�C�X���`�F�b�N
// ================================================================
bool C_d3d_device::check_device()
{
	// �f�B�X�v���C�ƃo�b�N�o�b�t�@�̃t�H�[�}�b�g�̃`�F�b�N
	if (!check_adapter_format())
		return false;
	// �[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�̃`�F�b�N
	if (!check_depth_stencil_format())
		return false;
	// �����_�����O�e�N�X�`���̃t�H�[�}�b�g�̃`�F�b�N
	if (!check_render_texture_format())
		return false;
	// �t���V�[���A���`�G�C���A�V���O�̃`�F�b�N
	if (!check_multisample_type())
		return false;
	// �f�B�X�v���C���[�h�̃`�F�b�N
	if (!check_display_mode())
		return false;
	// �f�o�C�X�̔\�͂̃`�F�b�N
	if (!check_device_caps())
		return false;


	return true;
}

// �e�N�X�`���� D3DUSAGE_AUTOGENMIPMAP ���g���邩�𒲂ׂ�
bool C_d3d_device::can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture()
{
	if (!m_state.mipmap_use_flag)
		return false;	// �~�b�v�}�b�v���g��Ȃ�
	if (!(m_state.device_caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP))
		return false;	// �~�b�v�}�b�v�̎����������ł��Ȃ�
	if (FAILED(m_state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture))
		return false;	// X8R8G8B8 �e�N�X�`���ɑ΂��ă~�b�v�}�b�v�����������ł��Ȃ�
	if (FAILED(m_state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture))
		return false;	// A8R8G8B8 �e�N�X�`���ɑ΂��ă~�b�v�}�b�v�����������ł��Ȃ�

	return true;
}

// �e�N�X�`���� D3DUSAGE_AUTOGENMIPMAP ���g���ă~�b�v�}�b�v������邩�ǂ����𒲂ׂ�
bool C_d3d_device::can_enable_D3DUSAGE_AUTOGENMIPMAP_for_texture()
{
	if (!m_state.mipmap_use_flag)
		return false;	// �~�b�v�}�b�v���g��Ȃ�
	if (!can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture())
		return false;	// �~�b�v�}�b�v�����������ł��Ȃ�
	if (m_state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture == D3DOK_NOAUTOGEN)
		return false;	// �~�b�v�}�b�v�̎��������ɑΉ����Ă��邪�~�b�v�}�b�v�͍���Ȃ�
	if (m_state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture == D3DOK_NOAUTOGEN)
		return false;	// �~�b�v�}�b�v�̎��������ɑΉ����Ă��邪�~�b�v�}�b�v�͍���Ȃ�

	return true;
}

// �e�N�X�`���� D3DPTFILTERCAPS_MIPFPOINT ���g���邩�ǂ����𒲂ׂ�
bool C_d3d_device::can_set_D3DPTFILTERCAPS_MIPFPOINT_for_texture()
{
	if (!m_state.mipmap_use_flag)
		return false;	// �~�b�v�}�b�v���g��Ȃ�
	if (!can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture())
		return false;	// �~�b�v�}�b�v�����������ł��Ȃ�
	if (!(m_state.device_caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFPOINT))
		return false;	// �~�b�v�}�b�v�� POINT �⊮�ɑΉ����Ă��Ȃ�

	return true;
}

// �e�N�X�`���� D3DPTFILTERCAPS_MIPFLINEAR ���g���邩�ǂ����𒲂ׂ�
bool C_d3d_device::can_set_D3DPTFILTERCAPS_MIPFLINEAR_for_texture()
{
	if (!m_state.mipmap_use_flag)
		return false;	// �~�b�v�}�b�v���g��Ȃ�
	if (!can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture())
		return false;	// �~�b�v�}�b�v�����������ł��Ȃ�
	if (FAILED(m_state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture))
		return false;	// ���\�[�X�ɑ΂��� D3DTEXF_POINT �ȊO�ɑΉ����Ă��Ȃ�
	if (FAILED(m_state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture))
		return false;	// ���\�[�X�ɑ΂��� D3DTEXF_POINT �ȊO�ɑΉ����Ă��Ȃ�
	if (!(m_state.device_caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR))
		return false;	// �~�b�v�}�b�v�� LINEAR �⊮�ɑΉ����Ă��Ȃ�

	return true;
}

// 2^n �ȊO�̃e�N�X�`�����g���邩�ǂ����𒲂ׂ�
bool C_d3d_device::can_use_not_pow2_texture()
{
	if (m_state.pow2_limit_flag)
		return false;	// �t���O�Ő�������Ă���̂Ŏg���Ȃ�
	if (m_state.device_caps.TextureCaps & D3DPTEXTURECAPS_POW2)
		return false;	// �n�[�h�E�F�A�̐����Ŏg���Ȃ�

	return true;
}

bool C_d3d_device::set_render_target(DWORD rt_idx, IDirect3DSurface9* rt)
{
	HRESULT hr;
	// �����_�[�^�[�Q�b�g
	//if(rt) {			// NULL�n���Ń����_�[�^�[�Q�b�g����
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
	// �[�x�X�e���V��
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
	// �r���[�|�[�g
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
	// �����_�[�^�[�Q�b�g
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
	// �[�x�X�e���V��
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
	// �r���[�|�[�g
	hr = G_d3d.device.body()->GetViewport(vp);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("GetViewport"), hr);
		return false;
	}
	return true;
}



// ****************************************************************
// �f�B�X�v���C�ƃo�b�N�o�b�t�@�̃t�H�[�}�b�g�̃`�F�b�N
// ================================================================
bool C_d3d_device::check_adapter_format()
{
	HRESULT hr;

	// �E�C���h�E���[�h
	hr = G_d3d->CheckDeviceType(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, m_state.pp_window.BackBufferFormat, TRUE);
	if (FAILED(hr))	{
		set_last_error_d3d(TSTR()
			+ _T("�E�B���h�E���[�h�F�f�B�X�v���C���A�w�肳�ꂽ�o�b�N�o�b�t�@�̃t�H�[�}�b�g�ɑΉ����Ă��܂���B")
			+ _T("\n\n�f�B�X�v���C�̐F���� 16bit �ɂȂ��Ă��܂��񂩁H\n24bit / 32bit �ɂ��邱�Ƃł��̖��͉�������\��������܂��B")
			+ _T("\n\ndisplay = ") + get_str_D3DFORMAT(m_state.display_mode.Format) + _T(", back_buffer = ") + get_str_D3DFORMAT(m_state.pp_window.BackBufferFormat), _T("CheckDeviceType"), hr);
		return false;
	}

	// �t���X�N���[�����[�h
	hr = G_d3d->CheckDeviceType(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, m_state.pp_full.BackBufferFormat, FALSE);
	if (FAILED(hr))	{
		set_last_error_d3d(TSTR()
			+ _T("�t���X�N���[�����[�h�F�f�B�X�v���C���A�w�肳�ꂽ�o�b�N�o�b�t�@�̃t�H�[�}�b�g�ɑΉ����Ă��܂���B")
			+ _T("\n\n�f�B�X�v���C�̐F���� 16bit �ɂȂ��Ă��܂��񂩁H\n24bit / 32bit �ɂ��邱�Ƃł��̖��͉�������\��������܂��B")
			+ _T("\n\ndisplay = ") + get_str_D3DFORMAT(m_state.pp_full.BackBufferFormat) + _T(", back_buffer = ") + get_str_D3DFORMAT(m_state.pp_full.BackBufferFormat), _T("CheckDeviceType"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// �[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�̃`�F�b�N
// ================================================================
bool C_d3d_device::check_depth_stencil_format()
{
	HRESULT hr;

	// �E�C���h�E���[�h
	if (m_state.pp_window.EnableAutoDepthStencil)	{

		// �f�B�X�v���C�t�H�[�}�b�g�ɓK�����邩�ǂ�������
		hr = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_state.pp_window.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�E�B���h�E���[�h�F�f�B�X�v���C���A�w�肳�ꂽ�[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�ɑΉ����Ă��܂���B"), _T("CheckDeviceFormat"), hr);
			return false;
		}

		// �����_�����O�^�[�Q�b�g�i�����ł̓o�b�N�o�b�t�@���w���j�Ƒg�ݍ��킹�Ďg���邩�ǂ�������
		hr = G_d3d->CheckDepthStencilMatch(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, m_state.pp_window.BackBufferFormat, m_state.pp_window.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�E�B���h�E���[�h�F�o�b�N�o�b�t�@�ƁA�[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�̑g�ݍ��킹���s���ł��B"), _T("CheckDepthStencilMatch"), hr);
			return false;
		}
	}

	// �t���X�N���[�����[�h
	if (m_state.pp_full.EnableAutoDepthStencil)	{

		// �f�B�X�v���C�t�H�[�}�b�g�ɓK�����邩�ǂ�������
		hr = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_state.pp_full.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�t���X�N���[�����[�h�F�f�B�X�v���C���A�w�肳�ꂽ�[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�ɑΉ����Ă��܂���B"), _T("CheckDeviceFormat"), hr);
			return false;
		}

		// �����_�����O�^�[�Q�b�g�i�����ł̓o�b�N�o�b�t�@���w���j�Ƒg�ݍ��킹�Ďg���邩�ǂ�������
		hr = G_d3d->CheckDepthStencilMatch(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, m_state.pp_full.BackBufferFormat, m_state.pp_full.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�t���X�N���[�����[�h�F�o�b�N�o�b�t�@�ƁA�[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�̑g�ݍ��킹���s���ł��B"), _T("CheckDepthStencilMatch"), hr);
			return false;
		}
	}

	return true;
}

// ****************************************************************
// �����_�����O�e�N�X�`���̃t�H�[�}�b�g�̃`�F�b�N
// ================================================================
bool C_d3d_device::check_render_texture_format()
{
	HRESULT hr;

	// �E�C���h�E���[�h

	// �����_�[�^�[�Q�b�g�ƂȂ�e�N�X�`��
	hr = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, m_state.fmt_render_texture_window);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�E�B���h�E���[�h�F�f�B�X�v���C���A�����_�[�^�[�Q�b�g�̃e�N�X�`���Ɏw�肳�ꂽ�t�H�[�}�b�g�ɑΉ����Ă��܂���B"), _T("CheckDeviceFormat"), hr);
		return false;
	}

	// �[�x�^�X�e���V���o�b�t�@�Ƒg�ݍ��킹��
	if (m_state.pp_window.EnableAutoDepthStencil)	{

		hr = G_d3d->CheckDepthStencilMatch(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, m_state.fmt_render_texture_window, m_state.pp_window.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�E�B���h�E���[�h�F�����_�[�^�[�Q�b�g�̃e�N�X�`���ƁA�[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�̑g�ݍ��킹���s���ł��B"), _T("CheckDepthStencilMatch"), hr);
			return false;
		}
	}

	// �t���X�N���[�����[�h

	// �����_�[�^�[�Q�b�g�ƂȂ�e�N�X�`��
	hr = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, m_state.fmt_render_texture_full);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�t���X�N���[�����[�h�F�f�B�X�v���C���A�����_�[�^�[�Q�b�g�̃e�N�X�`���Ɏw�肳�ꂽ�t�H�[�}�b�g�ɑΉ����Ă��܂���B"), _T("CheckDeviceFormat"), hr);
		return false;
	}

	// �[�x�^�X�e���V���o�b�t�@�Ƒg�ݍ��킹��
	if (m_state.pp_full.EnableAutoDepthStencil)	{

		hr = G_d3d->CheckDepthStencilMatch(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, m_state.fmt_render_texture_full, m_state.pp_full.AutoDepthStencilFormat);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�t���X�N���[�����[�h�F�����_�[�^�[�Q�b�g�̃e�N�X�`���ƁA�[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�̑g�ݍ��킹���s���ł��B"), _T("CheckDepthStencilMatch"), hr);
			return false;
		}
	}

	return true;
}

// ****************************************************************
// �}���`�T���v�����O�^�C�v�̃`�F�b�N
// ================================================================
bool C_d3d_device::check_multisample_type()
{
	HRESULT hr;

	// �E�B���h�E���[�h

	// �o�b�N�o�b�t�@�ɑΉ����Ă��邩�ǂ����𔻒�
	hr = G_d3d->CheckDeviceMultiSampleType(m_state.display_adapter, m_state.device_type, m_state.pp_window.BackBufferFormat, TRUE, m_state.pp_window.MultiSampleType, NULL);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�E�B���h�E���[�h�F�w�肳�ꂽ�}���`�T���v�����O�^�C�v���A�w�肳�ꂽ�o�b�N�o�b�t�@�̃t�H�[�}�b�g�ɓK�����Ă��܂���B"), _T("CheckDeviceMultiSampleType"), hr);
		return false;
	}

	// �[�x�^�X�e���V���o�b�t�@�ɑΉ����Ă��邩�ǂ����𔻒�
	if (m_state.pp_window.EnableAutoDepthStencil)	{

		hr = G_d3d->CheckDeviceMultiSampleType(m_state.display_adapter, m_state.device_type, m_state.pp_window.AutoDepthStencilFormat, TRUE, m_state.pp_window.MultiSampleType, NULL);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�E�B���h�E���[�h�F�w�肳�ꂽ�}���`�T���v�����O�^�C�v���A�w�肳�ꂽ�[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�ɓK�����Ă��܂���B"), _T("CheckDeviceMultiSampleType"), hr);
			return false;
		}
	}

	// �t���X�N���[�����[�h

	// �o�b�N�o�b�t�@�ɑΉ����Ă��邩�ǂ����𔻒�
	hr = G_d3d->CheckDeviceMultiSampleType(m_state.display_adapter, m_state.device_type, m_state.pp_full.BackBufferFormat, FALSE, m_state.pp_full.MultiSampleType, NULL);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�t���X�N���[�����[�h�F�w�肳�ꂽ�}���`�T���v�����O�^�C�v���A�w�肳�ꂽ�o�b�N�o�b�t�@�̃t�H�[�}�b�g�ɓK�����Ă��܂���B"), _T("CheckDeviceMultiSampleType"), hr);
		return false;
	}

	// �[�x�^�X�e���V���o�b�t�@�ɑΉ����Ă��邩�ǂ����𔻒�
	if (m_state.pp_window.EnableAutoDepthStencil)	{

		hr = G_d3d->CheckDeviceMultiSampleType(m_state.display_adapter, m_state.device_type, m_state.pp_full.AutoDepthStencilFormat, FALSE, m_state.pp_full.MultiSampleType, NULL);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�t���X�N���[�����[�h�F�w�肳�ꂽ�}���`�T���v�����O�^�C�v���A�w�肳�ꂽ�[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�ɓK�����Ă��܂���B"), _T("CheckDeviceMultiSampleType"), hr);
			return false;
		}
	}

	return true;
}

// ****************************************************************
// �f�B�X�v���C���[�h�̃`�F�b�N
// ================================================================
bool C_d3d_device::check_display_mode()
{
	// �E�C���h�E���[�h

		// ���ׂ�K�v�Ȃ�

	// �t���X�N���[�����[�h
	int w = m_state.full_size.cx;
	int h = m_state.full_size.cy;

	D3DDISPLAYMODE dmode = { 0, 0, 0, D3DFMT_UNKNOWN };
	int level = 1000000;
	int num = G_d3d->GetAdapterModeCount(m_state.display_adapter, m_state.pp_full.BackBufferFormat);
	for (int i = 0; i < num; i++)	{

		// �f�B�X�v���C���[�h�̗�
		D3DDISPLAYMODE dm;
		G_d3d->EnumAdapterModes(m_state.display_adapter, m_state.pp_full.BackBufferFormat, i, &dm);
		// ��]�̃��[�h�Ƃ̌덷
		int l = abs((int)(w - dm.Width)) + abs((int)(h - dm.Height));
		if (l < level)	{
			// ���K�؂ȃ��[�h��I��
			dmode = dm;
			level = l;
		}
	}
	if (dmode.Format == D3DFMT_UNKNOWN)	{
		set_last_error(_T("�t���X�N���[���F�K�؂ȃf�B�X�v���C���[�h��������܂���ł����B"), _T(""));
		return false;
	}

	m_state.pp_full.BackBufferWidth  = dmode.Width;
	m_state.pp_full.BackBufferHeight = dmode.Height;
	m_state.pp_full.FullScreen_RefreshRateInHz = dmode.RefreshRate;//D3DPRESENT_RATE_DEFAULT;	// �f�t�H���g

	return true;
}

// ****************************************************************
// �f�o�C�X�̔\�͂̃`�F�b�N
// ================================================================
bool C_d3d_device::check_device_caps()
{
	// �s�N�Z���V�F�[�_�ɑΉ����Ă��Ȃ��O���{�̏ꍇ�͂����ŃG���[��Ԃ��܂��B
	if (m_state.device_caps.PixelShaderVersion < D3DPS_VERSION(2, 0))	{
		set_last_error(_T("���g�p�̃r�f�I�J�[�h���s�N�Z���V�F�[�_ 2.0 �ɑΉ����Ă��܂���B\n���̃A�v���P�[�V�������N������ɂ́A�s�N�Z���V�F�[�_ 2.0 �ɑΉ������r�f�I�J�[�h���K�v�ł��B"), _T(""));
		return false;
	}

	// 2048�~2048 �̃e�N�X�`�����K�v�ł��B
	if (m_state.device_caps.MaxTextureWidth < 2048 || m_state.device_caps.MaxTextureHeight < 2048)	{
		set_last_error(_T("�쐬�ł���ő�e�N�X�`���T�C�Y�����������܂��B\n����=") + tostr(m_state.device_caps.MaxTextureWidth) + _T(",") + tostr(m_state.device_caps.MaxTextureHeight) + _T("�K�v=2048�~2048"), _T(""));
		return false;
	}

	//
	// �~�b�v�}�b�v�̑Ή��󋵂𒲂ׂ�
	//

	// �~�b�v�}�b�v�̎��������ɑΉ����Ă��邩
	if (!(m_state.device_caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP))	{
		// error_box(_T("D3DCAPS2_CANAUTOGENMIPMAP �ɑΉ����Ă��܂���B"));
		// ����J����}�V���őΉ����Ă��Ȃ��B����̑Ή����ǂ����邩�c�BD3DOK_NOAUTOGEN �Ȃ�G���[���������̂Ƃ��Ⴄ�H
	}
	// D3DFMT_X8R8G8B8 �̃e�N�X�`���Ń~�b�v�}�b�v�����������ł��邩�ǂ���
	m_state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE, D3DFMT_X8R8G8B8);
	if (FAILED(m_state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture))	{
		// error_box(_T("D3DUSAGE_AUTOGENMIPMAP �ɑΉ����Ă��܂���B") + create_my_last_error_d3d(state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8));
		// ������ʂ�}�V���͂���̂��Ȃ��c�B����Ȃ� AUTOGEN_ ��؂�Ȃ���΂����Ȃ��B
	}
	else if (m_state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture == D3DOK_NOAUTOGEN)	{
		// error_box(_T("D3DUSAGE_AUTOGENMIPMAP �ɑΉ����Ă܂����A�~�b�v�}�b�v�͍���܂���B") + create_my_last_error_d3d(state.hr_D3DUSAGE_AUTOGENMIPMAP));
		// ����J����}�V���őΉ����Ă��Ȃ��B�����Ȃ邾���ŃG���[�͏o�Ȃ��B�G���[���O�ɏo�������ɂ��悤�B
	}
	// D3DFMT_A8R8G8B8 �̃e�N�X�`���Ń~�b�v�}�b�v�����������ł��邩�ǂ���
	m_state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8);
	if (FAILED(m_state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture))	{
		// error_box(_T("D3DUSAGE_AUTOGENMIPMAP �ɑΉ����Ă��܂���B") + create_my_last_error_d3d(state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8));
		// ������ʂ�}�V���͂���̂��Ȃ��c�B����Ȃ� AUTOGEN_ ��؂�Ȃ���΂����Ȃ��B
	}
	else if (m_state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture == D3DOK_NOAUTOGEN)	{
		// error_box(_T("D3DUSAGE_AUTOGENMIPMAP �ɑΉ����Ă܂����A�~�b�v�}�b�v�͍���܂���B") + create_my_last_error_d3d(state.hr_D3DUSAGE_AUTOGENMIPMAP));
		// ����J����}�V���őΉ����Ă��Ȃ��B�����Ȃ邾���ŃG���[�͏o�Ȃ��B�G���[���O�ɏo�������ɂ��悤�B
	}

	// �������܂łŃG���[���o��悤�Ȃ�~�b�v�}�b�v�t�B���^�� D3DTEXF_NONE �ɂ��Ȃ��Ƃ����Ȃ��B

	// D3DFMT_X8R8G8B8 �̃e�N�X�`���ɑ΂��� D3DTEXF_POINT �ȊO�̃t�B���^�����O�ɑΉ����Ă��邩�ǂ���
	m_state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_QUERY_FILTER, D3DRTYPE_TEXTURE, D3DFMT_X8R8G8B8);
	if (FAILED(m_state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture))	{
		// error_box(_T("QUERY_FILTER �ɑΉ����Ă��܂���B") + create_my_last_error_d3d(state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8));
		// ������ʂ�O���{�͂Ȃ����ۂ��H�Ή����Ă��Ȃ���ΑS�� POINT �ɂ��Ȃ���΂����Ȃ��B
	}
	// D3DFMT_A8R8G8B8 �̃e�N�X�`���ɑ΂��� D3DTEXF_POINT �ȊO�̃t�B���^�����O�ɑΉ����Ă��邩�ǂ���
	m_state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture = G_d3d->CheckDeviceFormat(m_state.display_adapter, m_state.device_type, m_state.display_mode.Format, D3DUSAGE_QUERY_FILTER, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8);
	if (FAILED(m_state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture))	{
		// error_box(_T("QUERY_FILTER �ɑΉ����Ă��܂���B") + create_my_last_error_d3d(state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8));
		// ������ʂ�O���{�͂Ȃ����ۂ��H�Ή����Ă��Ȃ���ΑS�� POINT �ɂ��Ȃ���΂����Ȃ��B
	}
	// �~�b�v�}�b�v�ɑ΂��� D3DTEXF_LINEAR �ɑΉ����Ă��邩�ǂ���
	if (!(m_state.device_caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR))	{
		// error_box(_T("D3DPTFILTERCAPS_MIPFLINEAR �ɑΉ����Ă��܂���B"));
		// ������ʂ�O���{�͂Ȃ����ۂ��H�Ή����Ă��Ȃ���� MIPPOINT �ɂ��悤�B
	}

	return true;
}

// ****************************************************************
// �f�o�C�X���쐬
// ================================================================
bool C_d3d_device::create_device()
{
	HRESULT hr;

	IDirect3DDevice9* p_devide = NULL;

#if 1
	// ���_�������n�[�h�E�F�A�ōs�Ȃ��Ă݂�
	if (m_state.vertex_process_type == D3DCREATE_HARDWARE_VERTEXPROCESSING)	{
		hr = G_d3d->CreateDevice(m_state.display_adapter, m_state.device_type, m_state.h_wnd, m_state.vertex_process_type, &m_state.pp_window, &p_devide);
		if (FAILED(hr))	{
			m_state.vertex_process_type = D3DCREATE_SOFTWARE_VERTEXPROCESSING;	// ���s�����̂Ń\�t�g�E�F�A�����݂�
		}
	}
	// ���_�������\�t�g�E�F�A�ōs�Ȃ��Ă݂�
	if (m_state.vertex_process_type == D3DCREATE_SOFTWARE_VERTEXPROCESSING)	{
		hr = G_d3d->CreateDevice(m_state.display_adapter, m_state.device_type, m_state.h_wnd, m_state.vertex_process_type, &m_state.pp_window, &p_devide);
		if (FAILED(hr))	{

			// �f�o�C�X�̍쐬�Ɏ��s
			set_last_error_d3d(_T("CreateDevice"), hr);
			return false;
		}
	}
#else
	// ���_�������n�[�h�E�F�A�ōs�Ȃ��Ă݂�
	m_state.vertex_process_type = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	hr = G_d3d->CreateDevice(m_state.display_adapter, m_state.device_type, m_state.h_wnd, m_state.vertex_process_type, &m_state.pp_window, &p_devide);
	if (FAILED(hr))	{

		// ���_�������\�t�g�E�F�A�ōs�Ȃ��Ă݂�
		m_state.vertex_process_type = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		hr = G_d3d->CreateDevice(m_state.display_adapter, m_state.device_type, m_state.h_wnd, m_state.vertex_process_type, &m_state.pp_window, &p_devide);
		if (FAILED(hr))	{

			// �f�o�C�X�̍쐬�Ɏ��s
			set_last_error_d3d(_T("CreateDevice"), hr);
			return false;
		}
	}
#endif

	m_device = BIP<IDirect3DDevice9>(p_devide, false);

	return true;
}

// ****************************************************************
// �f�o�C�X���`�F�b�N
// ================================================================
bool C_d3d_device::check_device_lost()
{
	HRESULT hr;

	// �f�o�C�X�������ł��Ă��Ȃ��ꍇ
	// ���X�g���Ă���킯�ł͂Ȃ��̂Ő�����Ԃ�
	if (!m_device)
		return true;

	// �f�o�C�X�����t���O����U���Z�b�g����
	m_state.device_lost = false;
	m_state.device_can_reset = false;

	// �f�o�C�X�̏�Ԃ��`�F�b�N
	hr = m_device->TestCooperativeLevel();
	if (FAILED(hr))	{

		// �f�o�C�X���������Ă��ă��Z�b�g�ł��Ȃ�
		if (hr == D3DERR_DEVICELOST)	{
			m_state.device_lost = true;
			return true;
		}

		// �f�o�C�X���������Ă��邪���Z�b�g�ł���
		if (hr == D3DERR_DEVICENOTRESET)	{
			m_state.device_lost = true;
			m_state.device_can_reset = true;
			return true;
		}

		// �\�����ʃG���[
		return false;
	}

	return true;
}

// ****************************************************************
// �f�o�C�X�𕜌�
// ================================================================
bool C_d3d_device::restore()
{
	HRESULT hr;

	// �f�o�C�X�����t���O����U���Z�b�g����
	m_state.device_lost = false;
	m_state.device_can_reset = false;

	// �f�o�C�X�������ł��Ă��Ȃ��ꍇ�͎��s
	if (!m_device)
		return false;

	// �f�o�C�X�̕��������݂�
	hr = m_device->Reset(m_state.p_pp);
	if (FAILED(hr))	{

		// �f�o�C�X�͂܂��������Ă���
		if (hr == D3DERR_DEVICELOST)	{
			m_state.device_lost = true;
			m_state.device_can_reset = false;
			return true;
		}

		set_last_error_d3d(_T("Reset"), hr);
		return false;
	}

	// �r���[�|�[�g��������
	if (!set_view_port(0, 0, m_state.p_pp->BackBufferWidth, m_state.p_pp->BackBufferHeight))
		return false;

	return true;
}

// ****************************************************************
// �f�B�X�v���C�֓]��
// ================================================================
bool C_d3d_device::present(C_rect* p_rect)
{
	HRESULT hr;

	// �f�o�C�X�������ł��Ă��Ȃ��ꍇ�͎��s
	if (!m_device)
		return false;

	// �f�o�C�X���������Ă���Ε���
	if (m_state.device_lost)
		return true;

	// �f�o�C�X���������Ă��Ȃ���Ε`��i������ elseif �ɂ��Ȃ����ƁI�j
	if (!m_state.device_lost)	{

		// �f�o�C�X�����t���O����U���Z�b�g����
		m_state.device_lost = false;

		// �o�b�N�o�b�t�@���t�����g��
		hr = m_device->Present(p_rect, p_rect, NULL, NULL);
		// �f�o�C�X�����������ꍇ
		if (hr == D3DERR_DEVICELOST)	{
			m_state.device_lost = true;
			return true;
		}
	}

	return true;
}

// ****************************************************************
// �f�B�X�v���C���[�h�̕ύX
// ================================================================
bool C_d3d_device::change_display_mode(bool is_window)
{
	m_state.is_window = is_window;

	// �v���[���g�p�����[�^�̐ݒ�
	if (m_state.is_window)		m_state.p_pp = &m_state.pp_window;
	else						m_state.p_pp = &m_state.pp_full;

	return true;
}

// ****************************************************************
// �X�N���[���T�C�Y�̕ύX
// ================================================================
bool C_d3d_device::change_screen_size(C_size size)
{
	m_state.pp_window.BackBufferWidth = size.cx;
	m_state.pp_window.BackBufferHeight = size.cy;

	return true;
}

// ****************************************************************
// �`��Ԋu�̕ύX
// ================================================================
bool C_d3d_device::change_present_interval(int interval)
{
	m_state.pp_window.PresentationInterval = interval;
	m_state.pp_full.PresentationInterval = interval;

	return true;
}

// ****************************************************************
// �����_�[�^�[�Q�b�g���N���A
// ================================================================
bool C_d3d_device::clear_render_target(D3DCOLOR back_color)
{
	HRESULT hr;

	// �f�o�C�X�������ł��Ă��Ȃ��ꍇ�͎��s
	if (!m_device)	{
		set_last_error(_T("�f�o�C�X����������Ă��܂���B"), _T("clear_render_target"));
		return false;
	}

	// �o�b�N�o�b�t�@���N���A
	DWORD clear_target = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL;
	hr = G_d3d.device->Clear(0, NULL, clear_target, back_color, 1.0f, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Clear"), hr);
		return false;
	}

	return true;
}

}
