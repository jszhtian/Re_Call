#pragma once

#include	"tonad3d3_d3d.h"
#include	"tonad3d3_sprite.h"
#include	"tonad3d3_effect.h"
#include	"tonad3d3_effect_source_manager.h"

#include	"tonad3d3_render.h"
#include	"tonad3d3_pre_render.h"
#include	"tonad3d3_post_render.h"

namespace NT3
{

class C_d3d_light_interface;
class C_d3d_camera_interface;

// ****************************************************************
// �c�R�c�����_���[
// ================================================================
class C_d3d_renderer
{
public:

	bool	init();									// ������
	void	clear_sprite_list();					// �X�v���C�g���X�g�̊J��
	void	regist_sprite(C_d3d_sprite* sprite);	// �X�v���C�g�̓o�^
	void	delete_sprite(C_d3d_sprite* sprite);	// �X�v���C�g�̖���

	C_d3d_surface	get_back_buffer();								// �o�b�N�o�b�t�@���擾
	bool			set_render_target(IDirect3DSurface9* surface);	// �����_�[�^�[�Q�b�g��ݒ�
	bool			set_dpst_buffer(IDirect3DSurface9* surface);	// �[�x�X�e���V���o�b�t�@��ݒ�
	C_d3d_surface	get_dpst_buffer();								// �[�x�X�e���V���o�b�t�@���擾

	// �����_�[�^�[�Q�b�g���N���A
	bool	clear_render_target(D3DCOLOR back_color = 0);
	bool	clear_render_target_buffer(D3DCOLOR back_color = 0);
	bool	clear_render_target_stencil();
	bool	clear_render_target_z_buffer();

	// �X�V
	bool	update();

	// �����_�[
	bool	render();
	bool	pre_render();
	bool	post_render();

	// �f�o�b�O�p
	bool	debug_render_shadow_map(int render_size, int x, int y);
	bool	debug_render_light_bloom_tex(int render_size, int x, int y);

	// �f�t�H���g�̃r���[�|�[�g��ݒ肷��
	void	set_default_view_port(const C_rect& rect)
	{
		m_render_default.set_default_view_port(rect);
	}

	// �A�N�Z�T���\�b�h
	// �V���h�E�}�b�v
	void set_shadow_map_tex(IDirect3DTexture9*	shadow_map_tex)
		{m_shadow_map_tex = shadow_map_tex;}
	void set_shadow_map_surface(IDirect3DSurface9*	shadow_map_surface)
		{m_shadow_map_surface = shadow_map_surface;}
	void set_shadow_map_depth_stencil_buffer(IDirect3DSurface9*	shadow_map_depth_stencil_buffer)
		{m_shadow_map_depth_stencil_buffer = shadow_map_depth_stencil_buffer;}
	void set_shadow_map_viewport(D3DVIEWPORT9 shadow_map_viewport)
		{m_shadow_map_viewport = shadow_map_viewport;}
	IDirect3DTexture9*	get_shadow_map_tex()
		{return m_shadow_map_tex;}

	// ���C�g�u���[��
	void set_light_bloom_tex(IDirect3DTexture9*	light_bloom_tex)
		{m_light_bloom_tex = light_bloom_tex;}
	void set_light_bloom_surface(IDirect3DSurface9*	light_bloom_surface)
		{m_light_bloom_surface = light_bloom_surface;}


	D3DXMATRIX*		get_view_matrix()		{	return &m_mat_view;			}
	D3DXMATRIX*		get_proj_matrix()		{	return &m_mat_proj;			}
	D3DXMATRIX*		get_light_view_matrix()	{	return &m_mat_light_view;	}
	D3DXMATRIX*		get_light_proj_matrix()	{	return &m_mat_light_proj;	}
	D3DXMATRIX*		get_scale_bias_matrix()	{	return &m_mat_scale_bias;	}

	void	set_camera(BSP<C_d3d_camera_interface> camera)	{	m_camera = camera;	}

	C_d3d_camera_interface*					get_camera()			{	return m_camera.get();		}
	C_d3d_default_effect*					get_default_effect()	{	return &m_default_effect;	}
	ARRAY< BSP<C_d3d_light_interface> >*	get_light_list()		{	return &m_light_list;		}

private:
	bool update_frame_set(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_frame* frame);

public:

	// �ϊ��s��
	D3DXMATRIX		m_mat_view;		// �r���[
	D3DXMATRIX		m_mat_proj;		// �ˉe
	D3DXVECTOR3		m_camera_eye;	// �J�����̈ʒu�i������ɍ������ by ���� 2011/10/5�j
	D3DXVECTOR3		m_camera_dir;	// �J�����̌����i������ɍ������ by ���� 2011/10/5�j

	// �f�t�H���g�̃G�t�F�N�g
	C_d3d_default_effect					m_default_effect;

	// �J����
	BSP<C_d3d_camera_interface>				m_camera;

	// ���C�g���X�g
	ARRAY< BSP<C_d3d_light_interface> >		m_light_list;

	// �t�H�O
	int										m_fog_use;
	float									m_fog_x;
	float									m_fog_near;
	float									m_fog_far;

	// ���C�g�̕ϊ��s��(��Œ���)
	D3DXMATRIX		m_mat_light_view;			// �r���[
	D3DXMATRIX		m_mat_light_proj;			// �ˉe
	D3DXMATRIX		m_mat_scale_bias;

	// �`��N���X�Q
	C_d3d_render_default		m_render_default;
	C_d3d_pre_render_shadow_map	m_pre_render_shadow_map;

private:
	// �V���h�E�}�b�v
	IDirect3DTexture9*	m_shadow_map_tex;
	IDirect3DSurface9*	m_shadow_map_surface;
	IDirect3DSurface9*	m_shadow_map_depth_stencil_buffer;	
	D3DVIEWPORT9		m_shadow_map_viewport;

	// �f�t�H���g�n
	IDirect3DSurface9*	m_default_back_buffer;
	IDirect3DSurface9*	m_default_depth_stencil_buffer;	
	D3DVIEWPORT9		m_default_viewport;

	// ���C�g�u���[��
	IDirect3DTexture9*	m_light_bloom_tex;
	IDirect3DSurface9*	m_light_bloom_surface;

	// �X�v���C�g���X�g
	ARRAY<C_d3d_sprite *>	sprite_list;
};

// ****************************************************************
// �X�v���C�g���X�g���N���A
// ================================================================
inline void C_d3d_renderer::clear_sprite_list()
{
	sprite_list.clear();
}

// ****************************************************************
// �X�v���C�g��o�^
// ================================================================
inline void C_d3d_renderer::regist_sprite(C_d3d_sprite* sprite)
{
	sprite_list.push_back(sprite);
}

// ****************************************************************
// �X�v���C�g�𖕏�
// ================================================================
inline void C_d3d_renderer::delete_sprite(C_d3d_sprite* sprite)
{
	ARRAY<C_d3d_sprite *>::iterator itr = find(sprite_list.begin(), sprite_list.end(), sprite);
	if (itr != sprite_list.end())
		sprite_list.erase(itr);

}


}
