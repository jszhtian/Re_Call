#pragma once



#include	"tonad3d3_d3d.h"
#include	"tonad3d3_sprite.h"

namespace NT3
{


class C_d3d_renderer;



class C_d3d_render_interface
{
protected:
	C_d3d_render_interface();

public:
	virtual ~C_d3d_render_interface();

	virtual bool	initialize(C_d3d_renderer* p_renderer);
	virtual bool	render_sprite_list(ARRAY<C_d3d_sprite *>& sprite_list);

	// �f�t�H���g�̃r���[�|�[�g��ݒ肷��
	void			set_default_view_port(const C_rect& rect);

protected:
	virtual bool	render_sprite(C_d3d_sprite* sprite, int vertex_cnt, int primitive_cnt);
	virtual bool	render_sprite_set_blend_state(const S_d3d_render_param* rp);
	virtual bool	render_sprite_begin_effect_d2(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, ID3DXEffect** pp_effect);
	virtual bool	render_sprite_begin_effect_d3(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, ID3DXEffect** pp_effect);
	virtual bool	render_sprite_begin_effect_d3_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_mtrl* p_mtrl, ID3DXEffect** pp_effect);
	virtual bool	render_sprite_begin_effect_d3_sub(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, const C_d3d_mtrl* p_mtrl, int texture_cnt, ID3DXEffect* p_effect);
	// ����begin�n�͗]�v�Ȏ�������Ă���̂ŏ�����Begin()��BeginPass()�������ĂԊ֐�
	virtual bool	render_sprite_begin_effect(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect);
	virtual bool	render_sprite_end_effect_d2(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect);
	virtual bool	render_sprite_end_effect_d3(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect);
	
	virtual bool	render_sprite_polygon_d2(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, int vertex_cnt, int primitive_cnt);
	virtual bool	render_sprite_polygon_d3(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, int vertex_cnt, int primitive_cnt);
	
	virtual bool	render_sprite_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_mesh* p_mesh);
	virtual bool	render_sprite_mesh_mtrl(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_mesh* p_mesh, int mtrl_no);
	
	virtual bool	render_sprite_frame(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame);
	virtual bool	render_sprite_frame_mesh_subset(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no);
	
	virtual bool	render_sprite_frame_per_mesh_set_effect_constant_common(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect);
	virtual bool	render_sprite_frame_per_mesh_set_effect_constant_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect);
	virtual bool	render_sprite_frame_per_mesh_set_effect_constant_skinned_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect);

	// �G�t�F�N�g�萔�̐ݒ�
	virtual bool	render_sprite_frame_per_mesh_set_effect_constant(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect, ID3DXSkinInfo* p_skin_info);

	// �t���[���`�掞�̃��b�V�����̕`�����(�f�t�H���g�͏��true)
	virtual bool	render_sprite_frame_per_mesh_draw_condition(C_d3d_mesh* p_mesh);

	// �t���[���`�掞�̃��b�V�����̃G�t�F�N�g����
	virtual ID3DXEffect*	render_sprite_frame_per_mesh_create_effect(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info);


protected:
	C_rect			m_default_view_port;
	C_d3d_renderer*	m_p_renderer;

};

















}