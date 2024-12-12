#pragma once

#include "tonad3d3_render_interface.h"

namespace NT3
{



// shadow map:�������猩���V�[���̐[�x�l���������񂾃e�N�X�`��
// draw depth:�[�x�l����������(��:�J�������猩��)
class C_d3d_pre_render_shadow_map : public C_d3d_render_interface {
public:
	C_d3d_pre_render_shadow_map(){};
	~C_d3d_pre_render_shadow_map(){};

	bool	render_sprite_list(ARRAY<C_d3d_sprite *>& sprite_list);

private:

	// �G�t�F�N�g�萔�̐ݒ�
	bool	render_sprite_frame_per_mesh_set_effect_constant(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect, ID3DXSkinInfo* p_skin_info);

	// �t���[���`�掞�̃��b�V�����̕`�����(�v�������_�^�C�v���V���h�E�}�b�v�Ȃ�)
	bool	render_sprite_frame_per_mesh_draw_condition(C_d3d_mesh* p_mesh);

	// ���b�V�����̃G�t�F�N�g����
	ID3DXEffect*	render_sprite_frame_per_mesh_create_effect(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info);
};

















}