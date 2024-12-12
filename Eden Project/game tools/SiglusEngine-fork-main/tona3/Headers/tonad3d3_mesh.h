#pragma		once

#include	"tonad3d3_texture.h"
#include	"tonad3d3_effect_lighting_and_shading_shader.h"

namespace NT3
{
	// ****************************************************************
	// D3D �}�e���A��
	// ================================================================
	struct C_d3d_mtrl
	{
		C_d3d_mtrl();

		D3DMATERIAL9			mtrl;			// �}�e���A�����
		BSP<C_d3d_texture>		texture;		// �e�N�X�`��
		bool					disp;			// �\���t���O
		bool					alpha_blend;	// �A���t�@�u�����h
	};

	// ****************************************************************
	// D3D ���b�V���̃p�����[�^
	// ================================================================
	struct S_d3d_mesh_param
	{
		enum PRE_RENDER_TYPE {
			//NONE		= 1 << 0,
			NONE		= 0 << 0,
			SHADOW_MAP	= 1 << 0,
			//HOGE		= 1 << 1,
		};

		S_d3d_mesh_param();


		// ���C���`��p
		TSTR						effect_old_key;			// ���t���[���������p�L�[
		ID3DXEffect*				p_effect;				// ���t���[���������p�G�t�F�N�g
		// ���C�g���猩���[�x�l�p
		TSTR						shadow_map_effect_old_key;		// ���t���[���������p�L�[
		ID3DXEffect*				p_shadow_map_effect;			// ���t���[���������p�G�t�F�N�g


		LIGHTING_TYPE				lighting_type;			// ���C�e�B���O
		SHADING_TYPE				shading_type;			// �V�F�[�f�B���O
		SHADER_OPTION				shader_option;			// �V�F�[�_�I�v�V����

		PRE_RENDER_TYPE				pre_render_type;		// �v�������_�^�C�v

		bool						use_mesh_tex;			// 

		bool						use_mrbd;				// 
		D3DXVECTOR4					mrbd;					// mono, reverse, bright, dark
		bool						use_rgb;				// 
		D3DXVECTOR4					rgb_rate;				// r, g, b, rate
		D3DXVECTOR4					add_rgb;				// add_r, add_g, add_b

		// �����R�X�g�ƃ����b�g���������ĂȂ��̂Ō��
		//bool						use_tone_curve;			// �g�[���J�[�u

		bool						use_mul_vertex_color;	// 
		float						mul_vertex_color_rate;	// ���_�J���[�̏�Z����(�f�t�H���g1.0f)

		float						depth_buffer_shadow_bias;	// �[�x�o�b�t�@�V���h�E�̃o�C�A�X�l

		D3DXVECTOR4					rim_light_color;		// 
		float						rim_light_power;		// �������C�g�̓K�p�͈͂Ƌ��x

		// �g�D�[���}�b�v�p
		BSP<C_d3d_texture>			toon_tex;

		// �o���v�}�b�v�p
		BSP<C_d3d_texture>			normal_tex;

		// �e�탉�C�g�p
		ARRAY<int>					directional_light_id_list;
		ARRAY<int>					point_light_id_list;
		ARRAY<int>					spot_light_id_list;


		// �������C�g�A�g�[���J�[�u�Ȃǂ������\��
		// �X�N���v�g�ŃX�v���C�g�ɐݒ�o���镨�S���K�v����

		// Z�o�b�t�@�Ƃ��J�����O�����肻��

		// RenderState�n�͍\���̉����邩IDirect3DStateBlock9���g���H
		//D3DCULL						cull_mode;			// �J�����O���[�h
	};


	// ****************************************************************
	// D3D ���b�V��
	// ================================================================
	class C_d3d_mesh
	{
	public:
		C_d3d_mesh();
		C_d3d_mesh(BIP<ID3DXMesh> mesh);
		~C_d3d_mesh();

		void	init();		// ������
		void	release();	// ���

		bool	load_x(CTSTR& file_path, DWORD FVF, ARRAY<TSTR>& texture_name_list);	// x �t�@�C����ǂݍ���
		bool	add_material(C_d3d_mtrl mtrl);											// �}�e���A����ǉ�����
		bool	set_texture(int mtrl_no, BSP<C_d3d_texture> texture);					// �}�e���A���Ƀe�N�X�`����ݒ�

		int			get_mtrl_cnt()			{ return (int)mtrl_list.size(); }	// �}�e���A�������擾
		C_d3d_mtrl*	get_mtrl(int mtrl_no)	{ return &mtrl_list[mtrl_no]; }		// �}�e���A�����擾
		D3DXVECTOR3	get_size()				{ return size; }					// ���E�{�b�N�X�̃T�C�Y���擾

		ID3DXMesh*	body()					{ return mesh.get(); }
		ID3DXMesh*	operator ->()			{ return mesh.get(); }

		S_d3d_mesh_param* get_mesh_param_ptr()	{ return &m_param; }

		bool	set_toon_texture(BSP<C_d3d_texture> texture);
		bool	set_normal_texture(BSP<C_d3d_texture> texture);

	private:

		BIP<ID3DXMesh>				mesh;		// ���b�V��
		ARRAY<C_d3d_mtrl>			mtrl_list;	// �}�e���A��
		D3DXVECTOR3					size;		// ���b�V���̃T�C�Y

		// ���b�V��1�P�ʂŐ��䂷��\�����������(�I�u�W�F�N�g�P�ʂ̕���sprite�ցH)
		// �X�L�����b�V������������Ȃ����b�V�������̃N���X���g�p����
		S_d3d_mesh_param			m_param;	// �p�����[�^�Q
	};
}



