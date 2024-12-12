#pragma		once

#include	"tonad3d3_d3d.h"
#include	"tonad3d3_device.h"
#include	"tonad3d3_data.h"
#include	"tonad3d3_vertex.h"
#include	"tonad3d3_index.h"
#include	"tonad3d3_texture.h"
#include	"tonad3d3_mesh.h"
#include	"tonad3d3_frame_ex.h"

namespace NT3
{

const int	D3D_TEXTURE_CNT_MAX = 4;
const int	D3D_TONE_CURVE_NONE = -1;
const int	D3D_LIGHT_NONE = -1;

// ****************************************************************
// ���_�t�H�[�}�b�g�̍���
//
//		struct D3DVF_SAMPLE
//		{
//			D3DXVECTOR3		pos;			// �ʒu				D3DFVF_XYZ
//			FLOAT			rhw;			// �ʒu�Œ�			D3DFVF_XYZRHW
//			D3DXCOLOR		color;			// �f�B�t���[�Y�F	D3DFVF_DIFFUSE
//			D3DXVECTOR2		tex[tex_cnt];	// �t�u���W			D3DFVF_TEX1 - D3DFVF_TEX8
//			static const DWORD	FVF;		// FVF
//		};
//
// ----------------------------------------------------------------
//
//		const DWORD D3DVF_SAMPLE::FVF  = D3DFVF_XYZ | D3DFVF_DIFFUSE | ...
//
// ================================================================

// ****************************************************************
// �X�v���C�g�^�C�v
// ================================================================
enum E_d3d_sprite_type
{
	E_d3d_sprite_type_none,			// �^�C�v�Ȃ�
	E_d3d_sprite_type_point,		// �|�C���g�X�v���C�g
	E_d3d_sprite_type_polygon,		// �|���S��
	E_d3d_sprite_type_mesh,			// ���b�V��
	E_d3d_sprite_type_frame_set,	// �t���[���Z�b�g
};

// ****************************************************************
// �X�v���C�g�������@
// ================================================================
enum E_d3d_blend_type
{
	E_d3d_blend_type_alpha = 0,
	E_d3d_blend_type_add = 1,
	E_d3d_blend_type_sub = 2,
	E_d3d_blend_type_mul = 3,
	E_d3d_blend_type_screen = 4,
	E_d3d_blend_type_overlay = 5,
};

// ****************************************************************
// �����_�[�p�����[�^
// ================================================================
struct S_d3d_render_param
{
	S_d3d_render_param();

	// ������
	void	init();

	// bool �l
	bool	disp;			// �\���t���O
	bool	d2_rect;		// �Q�c��`
	bool	d3_rect;		// �R�c��`
	bool	d3_billboard;	// �R�c�r���{�[�h
	bool	dst_clip_use;	// �N���b�s���O
	bool	src_clip_use;	// �N���b�s���O
	bool	culling;		// �J�����O
	bool	alpha_test;		// �A���t�@�e�X�g
	bool	alpha_blend;	// �A���t�@�u�����h
	bool	use_mask;		// �}�X�N
	bool	use_fog;		// �t�H�O

	// �傫��
	C_float2	size;					// �傫��
	int			size_fit_to_texture;	// �e�N�X�`���ɃT�C�Y�����킹��

	// ���_�V�F�[�_�֘A
	C_float3		pos;		// �ʒu
	C_float3		center;		// ���S
	C_float3		scale;		// �g�嗦	�i���{ = 1.0f�j
	C_float3		rotate;		// ��]		�i����] = D3DX_PI �� 3.14�j
	D3DXQUATERNION	quaternion;	// ���N�H�[�^�j�I��
	C_rect			dst_clip;	// �N���b�s���O��`
	C_rect			src_clip;	// �N���b�s���O��`

	// �s�N�Z���V�F�[�_�֘A
	BYTE		tr;				// �s�����x		�i0�`255�j
	BYTE		mono;			// ���m�N���x	�i0�`255�j
	BYTE		reverse;		// ���]�x		�i0�`255�j
	BYTE		bright;			// ���x			�i0�`255�j
	BYTE		dark;			// �Óx			�i0�`255�j
	BYTE		color_r;		// �F�ω��i�F�j	�i0�`255�j
	BYTE		color_g;		// �F�ω��i�F�j	�i0�`255�j
	BYTE		color_b;		// �F�ω��i�F�j	�i0�`255�j
	BYTE		color_rate;		// �F�ω��i���j	�i0�`255�j
	BYTE		color_add_r;	// �F���Z		�i0�`255�j
	BYTE		color_add_g;	// �F���Z		�i0�`255�j
	BYTE		color_add_b;	// �F���Z		�i0�`255�j
	int			tone_curve_no;	// �g�[���J�[�u
	BYTE		tone_curve_sat;	// �g�[���J�[�u�ʓx
	int			light_no;		// ���C�g�ԍ�
	int			mask_x;			// �}�X�N�w
	int			mask_y;			// �}�X�N�x

	// �u�����f�B���O�֘A
	E_d3d_blend_type		blend_type;		// �������@�iE_d3d_blend_type �Ŏw��B�A���t�@�u�����h�A���Z�A���Z�A�c�j

	struct FILTER
	{
		C_argb	color;
	}	filter;
};

// ****************************************************************
// D3D �X�v���C�g
// ================================================================
class C_d3d_sprite
{
public:
	C_d3d_sprite();
	~C_d3d_sprite();

	// �X�v���C�g�������������
	void init();
	// �X�v���C�g���������i������Ȃ��j
	void init_no_release();
	// �X�v���C�g�����
	void release();

	// �|���S���X�v���C�g���쐬�i���������Ȃ��j
	bool create_polygon_no_init();
	// ���b�V���X�v���C�g���쐬�i���������Ȃ��j
	bool create_mesh_no_init();
	// �t���[���Z�b�g�X�v���C�g���쐬�i���������Ȃ��j
	bool create_frame_set_no_init();
	// �Q�c�X�v���C�g�̒��_����ݒ�
	bool set_d2_vertex_param(int vertex_offset, int vertex_cnt, int index_offset, int index_cnt, int primitive_cnt, BYTE* p_vertex_buffer, D3DXVECTOR4 p_vertex_pos[4], D3DXVECTOR2 p_texture_uv[4]);

	// --- �|���S���f�[�^���� ---
	void	set_vertex_buffer(BSP<C_d3d_vertex_buffer> vtx_buf)	{	m_vertex_buffer = vtx_buf;	}		// ���_�o�b�t�@��ݒ�
	void	set_index_buffer(BSP<C_d3d_index_buffer> ind_buf)	{	m_index_buffer = ind_buf;	}		// �C���f�b�N�X�o�b�t�@��ݒ�

	BSP<C_d3d_vertex_buffer>	get_vertex_buffer()	{	return m_vertex_buffer;					}		// ���_�o�b�t�@���擾
	int							get_vertex_offset()	{	return m_vertex_offset;					}		// ���_�J�n�ʒu���擾
	int							get_vertex_cnt()	{	return m_vertex_cnt;					}		// ���_�����擾
	int							get_vertex_size()	{	return m_vertex_cnt * get_FVF_size();	}		// ���_�o�b�t�@�̃T�C�Y���擾
	BSP<C_d3d_index_buffer>		get_index_buffer()	{	return m_index_buffer;					}		// �C���f�b�N�X�o�b�t�@���擾
	int							get_index_offset()	{	return m_index_offset;					}		// �C���f�b�N�X�J�n�ʒu���擾
	int							get_index_cnt()		{	return m_index_cnt;						}		// �C���f�b�N�X�����擾
	int							get_index_size()	{	return m_index_cnt * sizeof(WORD);		}		// �C���f�b�N�X�o�b�t�@�̃T�C�Y���擾
	int							get_primitive_cnt()	{	return m_primitive_cnt;					}		// �v���~�e�B�u�����擾
	DWORD						get_FVF()			{	return m_vertex_buffer ? m_vertex_buffer->get_FVF() : 0;		}		// FVF ���擾
	int							get_FVF_size()		{	return m_vertex_buffer ? m_vertex_buffer->get_FVF_size() : 0;	}		// FVF �̃T�C�Y���擾

	// --- ���b�V���f�[�^���� ---
	void					set_mesh(BSP<C_d3d_mesh> mesh)					{	m_mesh = mesh;				}	// ���b�V���̐ݒ�
	BSP<C_d3d_mesh>			get_mesh()										{	return m_mesh;				}	// ���b�V���̎擾
	void					set_frame_set(BSP<C_d3d_frame_set> frame_set)	{	m_frame_set = frame_set;	}	// �t���[���Z�b�g�̐ݒ�
	BSP<C_d3d_frame_set>	get_frame_set()									{	return m_frame_set;			}	// �t���[���Z�b�g�̎擾

	// --- �e�N�X�`���A�V�F�[�_�Ȃǂ̑���
	void					clear_texture(int stage = 0);									// �e�N�X�`�����N���A
	void					set_texture(BSP<C_d3d_texture> texture, int stage = 0);			// �e�N�X�`����ݒ�
	BSP<C_d3d_texture>		get_texture(int stage = 0);										// �e�N�X�`�����擾
	void					set_effect(ID3DXEffect* effect);								// �G�t�F�N�g��ݒ�
	ID3DXEffect*			get_effect();													// �G�t�F�N�g���擾
	void					set_effect_technique(D3DXHANDLE technique);						// �G�t�F�N�g�̃e�N�j�b�N��ݒ�
	D3DXHANDLE				get_effect_technique();											// �G�t�F�N�g�̃e�N�j�b�N���擾
	void					set_effect_constant_f(int idx, float f0, float f1, float f2, float f3);	// �G�t�F�N�g�̒萔��ݒ�
	float*					get_effect_constant_f(int idx);									// �G�t�F�N�g�̒萔���擾

	// --- �����_�[�f�[�^ ---
	E_d3d_sprite_type			type;		// �X�v���C�g�^�C�v
	S_d3d_render_param			rp;			// �X�v���C�g�p�����[�^

protected:

	// �v���~�e�B�u�֘A
	BSP<C_d3d_vertex_buffer>	m_vertex_buffer;	// ���_�o�b�t�@
	BSP<C_d3d_index_buffer>		m_index_buffer;		// �C���f�b�N�X�o�b�t�@
	int							m_vertex_offset;	// ���_�J�n�ʒu�i���_�̌��Ŏw��j
	int							m_vertex_cnt;		// ���_��
	int							m_index_offset;		// �C���f�b�N�X�J�n�ʒu�i���_�̌��Ŏw��j
	int							m_index_cnt;		// �C���f�b�N�X��
	int							m_primitive_cnt;	// �v���~�e�B�u��

	// ���b�V���֘A
	BSP<C_d3d_mesh>				m_mesh;			// ���b�V��
	BSP<C_d3d_frame_set>		m_frame_set;	// �t���[���Z�b�g

	// �e�N�X�`���֘A
	BSP<C_d3d_texture>			m_texture_list[D3D_TEXTURE_CNT_MAX];	// �e�N�X�`��

	// �G�t�F�N�g�֘A
	ID3DXEffect*				m_effect;								// �G�t�F�N�g
	D3DXHANDLE					m_effect_technique;						// �G�t�F�N�g�̃e�N�j�b�N
	float						m_effect_constant_f[2][4];				// �G�t�F�N�g�̒萔
};


}
