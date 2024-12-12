#pragma		once

#include	"tonad3d3_surface.h"

namespace NT3
{

// ****************************************************************
// Direct3D �f�o�C�X�X�e�[�g
// ================================================================
struct C_d3d_device_state
{
	UINT					display_adapter;			// �f�B�X�v���C�A�_�v�^
	D3DDISPLAYMODE			display_mode;				// �f�B�X�v���C���[�h

	D3DDEVTYPE				device_type;				// �f�o�C�X�̃^�C�v
	D3DCAPS9				device_caps;				// �f�o�C�X�̔\��
	UINT					vm_size;					// �r�f�I�������̍ő�e��
	DWORD					vertex_process_type;		// ���_�������@
	D3DPRESENT_PARAMETERS*	p_pp;						// �v���[���g�p�����[�^�i���݂̃��[�h�j
	D3DPRESENT_PARAMETERS	pp_window;					// �v���[���g�p�����[�^�i�E�B���h�E�j
	D3DPRESENT_PARAMETERS	pp_full;					// �v���[���g�p�����[�^�i�t���X�N���[���j
	D3DFORMAT				fmt_render_texture_window;	// �����_�[�e�N�X�`���̃t�H�[�}�b�g(�E�C���h�E)
	D3DFORMAT				fmt_render_texture_full;	// �����_�[�e�N�X�`���̃t�H�[�}�b�g(�t���X�N���[��)

	bool					device_lost;				// �f�o�C�X�����t���O
	bool					device_can_reset;			// �f�o�C�X���Z�b�g�\�t���O
	bool					mipmap_use_flag;			// �~�b�v�}�b�v���g���t���O
	bool					pow2_limit_flag;			// �K�� 2^n �ɕ␳����t���O
	HRESULT					hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture;	// CheckDeviceFormat(D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE) �̌��� 
	HRESULT					hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture;	// CheckDeviceFormat(D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE) �̌��� 
	HRESULT					hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture;	// CheckDeviceFormat(D3DUSAGE_QUERY_FILTER, D3DRTYPE_TEXTURE) �̌��� 
	HRESULT					hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture;	// CheckDeviceFormat(D3DUSAGE_QUERY_FILTER, D3DRTYPE_TEXTURE) �̌��� 

	HWND					h_wnd;						// �^�[�Q�b�g�E�B���h�E
	bool					is_window;					// �E�B���h�E�^�t���X�N���[��
	int						shadow_map_size;			// �V���h�E�}�b�v�̃T�C�Y
	C_size					full_size;					// �t���X�N���[�����̃E�B���h�E�T�C�Y
	float					z_min;						// �r���[�|�[�g�̍Ŏ�O�̂y�l
	float					z_max;						// �r���[�|�[�g�̍ŉ��̂y�l
};

// ****************************************************************
// Direct3D �f�o�C�X
// ================================================================
class C_d3d_device
{
public:
	C_d3d_device();
	~C_d3d_device();

//	bool	create(HWND h_wnd, bool is_window, int width, int height, int adapter_no, int present_interval, bool mipmap_use_flag);	// �f�o�C�X���\�z
//	bool	create(HWND h_wnd, bool is_window, int width, int height, int shadow_map_size, int adapter_no, int present_interval, bool mipmap_use_flag);	// �f�o�C�X���\�z
	bool	create(HWND h_wnd, bool is_window, int width, int height, int shadow_map_size, int adapter_no, DWORD vertex_process_type, int present_interval, bool mipmap_use_flag, bool pow2_limit_flag);

	void	release();									// �f�o�C�X�����
	bool	restore();									// �f�o�C�X�𕜌�
	bool	present(C_rect* p_rect = NULL);				// �X�N���[���֓]��

	bool	check_device_lost();						// �f�o�C�X�̏����𔻒�
	bool	change_display_mode(bool is_window);		// �f�B�X�v���C���[�h�̕ύX
	bool	change_screen_size(C_size size);			// �X�N���[���T�C�Y�̕ύX
	bool	change_present_interval(int interval);		// �`��Ԋu��ύX

	bool	can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture();		// �e�N�X�`���� D3DUSAGE_AUTOGENMIPMAP ���g���邩�𒲂ׂ�
	bool	can_set_D3DPTFILTERCAPS_MIPFPOINT_for_texture();	// �e�N�X�`���� D3DPTFILTERCAPS_MIPFPOINT ���g���邩�ǂ����𒲂ׂ�
	bool	can_set_D3DPTFILTERCAPS_MIPFLINEAR_for_texture();	// �e�N�X�`���� D3DPTFILTERCAPS_MIPFLINEAR ���g���邩�ǂ����𒲂ׂ�
	bool	can_enable_D3DUSAGE_AUTOGENMIPMAP_for_texture();	// �e�N�X�`���� D3DUSAGE_AUTOGENMIPMAP ���g���ă~�b�v�}�b�v������邩�ǂ����𒲂ׂ�
	bool	can_use_not_pow2_texture();							// 2^n �ȊO�̃e�N�X�`�����g���邩�ǂ����𒲂ׂ�

	// �����_�[�^�[�Q�b�g�n�iNULL��n���Ή������Ȃ��j
	bool	set_render_target(DWORD rt_idx, IDirect3DSurface9* rt);
	bool	get_render_target(DWORD rt_idx, IDirect3DSurface9** rt);
	bool	set_depth_stencil_surface(IDirect3DSurface9* dss);
	bool	get_depth_stencil_surface(IDirect3DSurface9** dss);
	bool	set_viewport(D3DVIEWPORT9* vp);
	bool	get_viewport(D3DVIEWPORT9* vp);

	bool	clear_render_target(D3DCOLOR back_color);

	IDirect3DDevice9*				body()					{	return m_device.get();			}	// �f�o�C�X���擾
	IDirect3DDevice9*				operator ->()			{	return m_device.get();			}	// �f�o�C�X���擾
	const C_d3d_device_state&		state() const			{	return m_state;					}	// �X�e�[�g���擾
	const D3DPRESENT_PARAMETERS*	pp() const				{	return m_state.p_pp;			}	// �v���[���g�p�����[�^���擾
	const D3DCAPS9&					device_caps() const		{	return m_state.device_caps;		}	// �f�o�C�X�̔\�͂��擾
	int								get_shadow_map_size()	{	return m_state.shadow_map_size;	}	// �V���h�E�}�b�v�̃T�C�Y
	float							get_z_min()				{	return m_state.z_min;			}	// �r���[�|�[�g�̍Ŏ�O�̂y�l
	float							get_z_max()				{	return m_state.z_max;			}	// �r���[�|�[�g�̍ŉ��̂y�l

	bool							is_lost()				{	return m_state.device_lost;			}	// �f�o�C�X�����t���O
	bool							can_reset()				{	return m_state.device_can_reset;	}	// �f�o�C�X�����t���O

	bool	set_view_port(C_rect rect);									// �r���[�|�[�g�̐ݒ�
	bool	set_view_port(C_point point, C_size size);					// �r���[�|�[�g�̐ݒ�
	bool	set_view_port(int x, int y, int width, int height);			// �r���[�|�[�g�̐ݒ�

	// ���\�[�X���쐬
	BIP<IDirect3DVertexBuffer9>		create_vertex_buffer();

private:

	BIP<IDirect3DDevice9>	m_device;		// Direct3D �f�o�C�X
	C_d3d_device_state		m_state;		// �X�e�[�g

	void	init();							// �f�o�C�X��������

	bool	select_device();				// �f�o�C�X�̑I��
	bool	check_device();					// �f�o�C�X�̃`�F�b�N
	bool	create_device();				// �f�o�C�X�̍쐬

	bool	check_adapter_format();			// �f�B�X�v���C�ƃo�b�N�o�b�t�@�̃t�H�[�}�b�g�̃`�F�b�N
	bool	check_depth_stencil_format();	// �[�x�^�X�e���V���o�b�t�@�̃t�H�[�}�b�g�̃`�F�b�N
	bool	check_render_texture_format();	// �����_�[�e�N�X�`���̃t�H�[�}�b�g�̃`�F�b�N
	bool	check_multisample_type();		// �t���V�[���A���`�G�C���A�V���O�̃`�F�b�N
	bool	check_display_mode();			// �f�B�X�v���C���[�h�̃`�F�b�N
	bool	check_device_caps();			// �f�o�C�X�̔\�͂̃`�F�b�N
};

}

