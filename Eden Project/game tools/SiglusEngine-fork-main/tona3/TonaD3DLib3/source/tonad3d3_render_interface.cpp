#include "tonad3d3_pch.h"
#include "tonad3d3_render_interface.h"
#include "tonad3d3_renderer.h"
#include "tonad3d3_debug_text.h"
#include "tonad3d3_camera.h"
#include "tonad3d3_light.h"
#include "tonad3d3_effect.h"
#include "tonad3d3_effect_source_creator_skinned_mesh.h"

namespace NT3
{


C_d3d_render_interface::C_d3d_render_interface()
{
	m_p_renderer = NULL;
}

C_d3d_render_interface::~C_d3d_render_interface()
{
	m_p_renderer = NULL;
}

bool C_d3d_render_interface::initialize(C_d3d_renderer* p_renderer)
{
	m_p_renderer = p_renderer;

	return true;
}

// �f�t�H���g�̃r���[�|�[�g��ݒ肷��
void C_d3d_render_interface::set_default_view_port(const C_rect& rect)
{
	m_default_view_port = rect;
}

// ****************************************************************
// �X�v���C�g���X�g�������_�[
// ================================================================
// �����_�[�X�v���C�g�̃��X�g���쐬
// �u�����h����̂��̂ƂȂ��̂��̂𕪂��Ĉ���
//
// �P�D�u�����h�Ȃ��̂��̂��y�o�b�t�@�̍X�V����ŕ`��
// �Q�D�u�����h����̂��̂��y�o�b�t�@�̍X�V�Ȃ��ŕ`��i�y�o�b�t�@�̎g�p���̂͂���j
//
// �{���́A�q�X�v���C�g���܂߂� rp ���v�Z���Ă��烊�X�g�ɕ�����K�v������
//
// ���b�V���͂ǂ������H�i�������j
// ================================================================
bool C_d3d_render_interface::render_sprite_list(ARRAY<C_d3d_sprite *>& sprite_list)
{
	int sprite_cnt = (int)sprite_list.size();
	if (sprite_cnt == 0)
		return true;

	// �p�����[�^�������X�v���C�g���܂Ƃ߂ĕ`�悷��
	int top_sprite_no = 0;
	int total_vertex_cnt = 0;
	int total_primitive_cnt = 0;
	total_vertex_cnt += sprite_list[0]->get_vertex_cnt();		// �P�ڂ̃X�v���C�g�̒��_����ǉ�
	total_primitive_cnt += sprite_list[0]->get_primitive_cnt();	// �P�ڂ̃X�v���C�g�̃v���~�e�B�u����ǉ�
	for (int sprite_no = 1; ; sprite_no++)	{
		
		// �Ō�������̓p�����[�^���ς������`��
		bool is_draw = false;
		if (sprite_no >= sprite_cnt)	{
			is_draw = true;
		}
		else	{

			C_d3d_sprite* s1 = sprite_list[top_sprite_no];
			C_d3d_sprite* s2 = sprite_list[sprite_no];
			if (false
				|| s1->rp.d3_rect		// ���Е��ł��R�c��`�̏ꍇ�A�ʁX�ɏ���
				|| s2->rp.d3_rect		// ���J�����̐ݒ肪�Ⴄ����ł��B�R�c��`�̓J��������Ȃ� set_d2_vertex_param �Ōv�Z���ׂ����ˁB�v�C���B
				|| s1->type != s2->type
				|| s1->rp.disp != s2->rp.disp
				|| s1->rp.d2_rect != s2->rp.d2_rect

//				|| s1->rp.size != s2->rp.size
//				|| s1->rp.size_fit_to_texture != s2->rp.size_fit_to_texture
//				|| s1->rp.pos != s2->rp.pos
//				|| s1->rp.center != s2->rp.center
//				|| s1->rp.scale != s2->rp.scale
//				|| s1->rp.rotate != s2->rp.rotate
				|| s1->rp.dst_clip_use != s2->rp.dst_clip_use
				|| s1->rp.dst_clip != s2->rp.dst_clip
				|| s1->rp.tr != s2->rp.tr
				|| s1->rp.mono != s2->rp.mono
				|| s1->rp.reverse != s2->rp.reverse
				|| s1->rp.bright != s2->rp.bright
				|| s1->rp.dark != s2->rp.dark
				|| s1->rp.color_r != s2->rp.color_r
				|| s1->rp.color_g != s2->rp.color_g
				|| s1->rp.color_b != s2->rp.color_b
				|| s1->rp.color_rate != s2->rp.color_rate
				|| s1->rp.color_add_r != s2->rp.color_add_r
				|| s1->rp.color_add_g != s2->rp.color_add_g
				|| s1->rp.tone_curve_no != s2->rp.tone_curve_no
				|| s1->rp.tone_curve_sat != s2->rp.tone_curve_sat
				|| s1->rp.light_no != s2->rp.light_no
				|| s1->rp.culling != s2->rp.culling
				|| s1->rp.use_mask != s2->rp.use_mask
				|| s1->rp.use_fog != s2->rp.use_fog
				|| s1->rp.alpha_test != s2->rp.alpha_test
				|| s1->rp.alpha_blend != s2->rp.alpha_blend
				|| s1->rp.blend_type != s2->rp.blend_type
				|| s1->get_vertex_buffer() != s2->get_vertex_buffer()
				|| s1->get_index_buffer() != s2->get_index_buffer()
				|| s1->get_mesh() != s2->get_mesh()
				|| s1->get_frame_set() != s2->get_frame_set()
				|| s1->get_texture(0) != s2->get_texture(0)
				|| s1->get_texture(1) != s2->get_texture(1)
				|| s1->get_texture(2) != s2->get_texture(2)
				|| s1->get_texture(3) != s2->get_texture(3)
				|| s1->get_effect() != s2->get_effect()
				|| s1->get_effect_technique() != s2->get_effect_technique()
				|| s1->get_effect_constant_f(0)[0] != s2->get_effect_constant_f(0)[0]
				|| s1->get_effect_constant_f(0)[1] != s2->get_effect_constant_f(0)[1]
				|| s1->get_effect_constant_f(0)[2] != s2->get_effect_constant_f(0)[2]
				|| s1->get_effect_constant_f(0)[3] != s2->get_effect_constant_f(0)[3]
				|| s1->get_effect_constant_f(1)[0] != s2->get_effect_constant_f(1)[0]
				|| s1->get_effect_constant_f(1)[1] != s2->get_effect_constant_f(1)[1]
				|| s1->get_effect_constant_f(1)[2] != s2->get_effect_constant_f(1)[2]
				|| s1->get_effect_constant_f(1)[3] != s2->get_effect_constant_f(1)[3]
			)
			{
				is_draw = true;
			}
		}

		if (is_draw)	{

			// draw_first_no �` (sprite_no - 1) �܂ł�`��
			C_d3d_sprite* p_sprite = sprite_list[top_sprite_no];

			// �X�v���C�g��`��
			if (!render_sprite(p_sprite, total_vertex_cnt, total_primitive_cnt))
				return false;

			// ���̕`��J�n�ʒu��ݒ�
			top_sprite_no = sprite_no;
			total_vertex_cnt = 0;
			total_primitive_cnt = 0;
		}

		// �Ō�܂ŕ`�悵����I��
		if (sprite_no >= sprite_cnt)
			break;

		total_vertex_cnt += sprite_list[sprite_no]->get_vertex_cnt();
		total_primitive_cnt += sprite_list[sprite_no]->get_primitive_cnt();
	}

	return true;
}

// ****************************************************************
// �X�v���C�g�������_�[
// ================================================================
//		�e�X�v���C�g�ƃf�[�^���������܂��B
//		�r���{�[�h�A�Q�c�X�v���C�g�A�ȂǓ���ȃp�����[�^�̐ݒ���s���܂��B
//		�e�N�X�`���A�V�F�[�_�A�G�t�F�N�g�Ȃǂ̐ݒ���s���܂��B
//		�Ō�ɃX�v���C�g�������_�[���܂��B
// ================================================================
bool C_d3d_render_interface::render_sprite(C_d3d_sprite* p_sprite, int vertex_cnt, int primitive_cnt)
{
	// �\���t���O���n�e�e�̃X�v���C�g�����O
	// �q���̕`����s��Ȃ����Ƃɒ���
	if (!p_sprite->rp.disp)
		return true;	// ����I��

	// �r���{�[�h�̏ꍇ�̓J�����ɂ��킹�ăX�v���C�g����]������
#if 0
	if (p_sprite->rp.billboard)	{

		// �J�����ɍ��킹�ăX�v���C�g����]
		D3DXVECTOR3 dir = m_camera.pint - m_camera.eye;
		dir = D3DXVECTOR3(dir.x, 0.0f, dir.z);							// xz ���ʂɓ���
		float dot = D3DXVec3Dot(&dir, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));	// ����
		float cos = dot / D3DXVec3Length(&dir);							// cos��
		float theta = acos(cos);										// ��
		if (dir.x < 0.0f)	{
			theta = D3DX_PI * 2 - theta;
		}
		p_sprite->rp.rotate.y = theta;
	}
#endif

	// �A���t�@�e�X�g�̐ݒ�i�L���A�P�ȏオ�ʉ߁j
	if (p_sprite->rp.alpha_test)	{
		G_d3d.state.set_render_state(D3DRS_ALPHATESTENABLE, TRUE);		// �A���t�@�e�X�g����
		G_d3d.state.set_render_state(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		G_d3d.state.set_render_state(D3DRS_ALPHAREF, 0x01);
	}
	else	{
		G_d3d.state.set_render_state(D3DRS_ALPHATESTENABLE, FALSE);		// �A���t�@�e�X�g�Ȃ�
	}

	// �A���t�@�u�����h�̐ݒ�
	if (p_sprite->rp.alpha_blend)	{
		G_d3d.state.set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);		// �A���t�@�u�����h����
	}
	else	{
		G_d3d.state.set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);	// �A���t�@�u�����h�Ȃ�
	}

	// �J�����O�̐ݒ�
	if (p_sprite->rp.culling)	{
		G_d3d.state.set_render_state(D3DRS_CULLMODE, D3DCULL_CCW);		// �J�����O���s��
	}
	else	{
		G_d3d.state.set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);		// �J�����O���s��Ȃ�
	}

	// �u�����f�B���O�W����ݒ�
	render_sprite_set_blend_state(&p_sprite->rp);

	// �Q�c��`�̏ꍇ�̓���ݒ�
	if (p_sprite->rp.d2_rect)	{

		// �Q�c��`�X�v���C�g�͂y�\�[�g���s��Ȃ�
		G_d3d.state.set_render_state(D3DRS_ZENABLE, FALSE);				// �y�\�[�g���s��Ȃ�
		G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, FALSE);		// �y�o�b�t�@���X�V���Ȃ�

		// �Q�c��`�X�v���C�g�̓e�N�X�`�������[�v�����Ȃ�
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

		// �Q�c��`�̓N���b�s���O���r���[�|�[�g�Őݒ�ł���
		if (p_sprite->rp.dst_clip_use)	{

			// �r���[�|�[�g�̓f�t�H���g�̃N���b�v�Ɏ��߂�iGeForce �Ƃ����ƁA��ʊO���w�肷��Ɗ��S�ɕ\������Ȃ��O���{������܂��B�j
			C_rect clip = p_sprite->rp.dst_clip;
			clip.left = std::max(clip.left, m_default_view_port.left);
			clip.top = std::max(clip.top, m_default_view_port.top);
			clip.right = std::min(clip.right, m_default_view_port.right);
			clip.bottom = std::min(clip.bottom, m_default_view_port.bottom);
			G_d3d.device.set_view_port(clip);
		}
		else	{
			G_d3d.device.set_view_port(m_default_view_port);
		}
	}

	// �R�c��`�̏ꍇ�̓���ݒ�
	else if (p_sprite->rp.d3_rect)	{

		// �R�c��`�͂y�\�[�g���s��
		G_d3d.state.set_render_state(D3DRS_ZENABLE, TRUE);

		// ���ߏ���������ꍇ�͂y�o�b�t�@���X�V���Ȃ�
		if (!p_sprite->rp.alpha_blend)	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, TRUE);			// �y�o�b�t�@���X�V����
		}
		else	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, FALSE);		// �y�o�b�t�@���X�V���Ȃ�
		}

		// �R�c��`�X�v���C�g�̓e�N�X�`�������[�v�����Ȃ�
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
	}

	// �r���{�[�h�̏ꍇ�̓���ݒ�
	else if (p_sprite->rp.d3_billboard)	{

		// �R�c�X�v���C�g�͂y�\�[�g���s��
		G_d3d.state.set_render_state(D3DRS_ZENABLE, TRUE);

		// ���ߏ���������ꍇ�͂y�o�b�t�@���X�V���Ȃ�
		if (!p_sprite->rp.alpha_blend)	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, TRUE);			// �y�o�b�t�@���X�V����
		}
		else	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, FALSE);		// �y�o�b�t�@���X�V���Ȃ�
		}

		// �r���{�[�h�̓e�N�X�`�������[�v�����Ȃ�
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_BORDERCOLOR, 0x00000000);	// ���E�̐F�͓���
	}

	// �R�c�X�v���C�g�̏ꍇ�̓���ݒ�
	else	{

		// �R�c�X�v���C�g�͂y�\�[�g���s��
		G_d3d.state.set_render_state(D3DRS_ZENABLE, TRUE);

		// ���ߏ���������ꍇ�͂y�o�b�t�@���X�V���Ȃ�
		if (!p_sprite->rp.alpha_blend)	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, TRUE);			// �y�o�b�t�@���X�V����
		}
		else	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, FALSE);		// �y�o�b�t�@���X�V���Ȃ�
		}

		// �R�c�̏ꍇ�̓e�N�X�`�������[�v������ �� ���[�v�����Ȃ��悤�ɂ��܂����i2010/10/22 �R�c���C�v�΍�j
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_BORDERCOLOR, 0x00000000);	// ���E�̐F�͓���

		// �J�����̔��Α��ɂ���X�v���C�g�����O
		//D3DXVECTOR3 pnt = camera.pint - camera.eye;
		//D3DXVECTOR3 pos = rp.pos - camera.eye;
		//if (D3DXVec3Dot(&pnt, &pos) < 0)
		//	return false;
	}

	// �X�e�[�W�O�ɂ��ă~�b�v�}�b�v���g�����ǂ���
	// �~�b�v�}�b�v�̎g���Ȃ��e�N�X�`���̏ꍇ�̓~�b�v�}�b�v���g�p���Ȃ�
	if (p_sprite->get_texture(0) && p_sprite->get_texture(0)->is_mipmap_enable())	{
		G_d3d.state.set_sampler_state(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}
	else	{
		G_d3d.state.set_sampler_state(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		G_d3d.state.set_sampler_state(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	}

	// �s�N�Z���V�F�[�_�̏�����
	G_d3d.device->SetPixelShader(NULL);

	// �e�N�X�`���̏�����
	for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)
		G_d3d.device->SetTexture(t, NULL);

	// �X�v���C�g�^�C�v�ɂ���ďꍇ�킯
	if (false);

	// �u�^�C�v�F�|���S���v�̕`��i�Q�c�j
	else if (p_sprite->type == E_d3d_sprite_type_polygon && p_sprite->rp.d2_rect)	{

		if (!render_sprite_polygon_d2(p_sprite, &p_sprite->rp, vertex_cnt, primitive_cnt))
			return false;
	}
	// �u�^�C�v�F�|���S���v�̕`��i�r���{�[�h�j
	else if (p_sprite->type == E_d3d_sprite_type_polygon && p_sprite->rp.d3_billboard)	{

		// ���r���{�[�h�͂������񖢑Ή��ɂ��܂��B
#if 0
		// ���_���X�g���[���ɗ�������
		Gd3d.device->SetStreamSource(0, p_sprite->get_vertex_buffer()->body(), 0, p_sprite->get_FVF_size());
		Gd3d.device->SetIndices(p_sprite->get_index_buffer()->body());	// �C���f�b�N�X�o�b�t�@�̐ݒ�
		Gd3d.device->SetFVF(p_sprite->get_FVF());							// FVF �̐ݒ�

		// �e�N�X�`����ݒ肷��
		for (int t = 0; t < D3D_TEXTURE_STAGE_MAX; t++)	{
			if (p_sprite->get_texture(t))
				Gd3d.device->SetTexture(t, p_sprite->get_texture(t)->body());
		}

		// �`����s��
		Gd3d.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, p_sprite->get_vertex_count(), 0, p_sprite->get_index_count() / 3);
#endif
	}
	// �u�^�C�v�F�|���S���v�̕`��i�R�c�j
	else if (p_sprite->type == E_d3d_sprite_type_polygon)	{

		if (!render_sprite_polygon_d3(p_sprite, &p_sprite->rp, vertex_cnt, primitive_cnt))
			return false;
	}

	// �u�^�C�v�F���b�V���v�̕`��
	else if (p_sprite->type == E_d3d_sprite_type_mesh && p_sprite->get_mesh())	{

		// ���b�V����`�悷��
		if (!render_sprite_mesh(p_sprite, &p_sprite->rp, p_sprite->get_mesh().get()))
			return false;
	}

	// �u�^�C�v�F�t���[���Z�b�g�v�̕`��
	else if (p_sprite->type == E_d3d_sprite_type_frame_set && p_sprite->get_frame_set())	{
		BSP<C_d3d_frame_set> frame_set = p_sprite->get_frame_set();

		/*
		// ���[���h�ϊ��s����v�Z
		D3DXMATRIX mat_world, mat_temp;

		// ���[���h�ϊ��s���ݒ�
		float yaw = p_sprite->rp.rotate.y;
		float pitch = p_sprite->rp.rotate.x;
		float roll = p_sprite->rp.rotate.z;

		// ���[���h���W�ϊ��s����쐬(S-R-T)
		D3DXMatrixIdentity(&mat_world);
		D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixScaling(&mat_temp, p_sprite->rp.scale.x, p_sprite->rp.scale.y, p_sprite->rp.scale.z));	// �g�k
		D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixRotationYawPitchRoll(&mat_temp, yaw, pitch, roll));									// ��]
		D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixTranslation(&mat_temp, p_sprite->rp.pos.x, p_sprite->rp.pos.y, p_sprite->rp.pos.z));	// �ړ�

		//// �i���_�V�F�[�_���g��Ȃ��ꍇ�j���[���h�ϊ��s���ݒ�
		//Gd3d.device->SetTransform(D3DTS_WORLD, &mat_world);

		// ���[���h�ϊ��s������Ƀt���[���ϊ��s��𐶐�����
		frame_set->update_matrix(&mat_world);
		// �A�j���[�V������i�߂�
		frame_set->update_animation_controller();

		// �����̎��_�Ń��[���h�s��ϊ����݂̃X�L���A�j���[�V�������s��Ȃ��A�j���[�V�����s��͎Z�o�ς݁H
		// �v�������_�̏ꍇ�ǂ����邩�H
		// �t���[���̕`�揇�͂ǂ��Ȃ��Ă���̂��H
		*/

		// �S�Ẵt���[�������ɕ`�悷��
		if (!render_sprite_frame(p_sprite, &p_sprite->rp, frame_set->m_frame_root.get()))
			return false;
	}

	return true;
}

// ****************************************************************
// �X�v���C�g�F�u�����h�X�e�[�g��ݒ�
// ================================================================
bool C_d3d_render_interface::render_sprite_set_blend_state(const S_d3d_render_param* rp)
{
	if (rp->blend_type == E_d3d_blend_type_alpha)	{			// ����

		// result = d * (1 - a) + s * a

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}
	else if (rp->blend_type == E_d3d_blend_type_add)	{		// ���Z

		// result = d + s * a

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_ONE);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}
	else if (rp->blend_type == E_d3d_blend_type_sub)	{		// ���Z

		// result = d - s * a

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_ONE);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	}
	else if (rp->blend_type == E_d3d_blend_type_mul)	{		// ��Z

		// result = d * s�i�G�t�F�N�g���� a ��K�p�����Ă��܂��j

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}
	else if (rp->blend_type == E_d3d_blend_type_screen)	{	// �X�N���[��

		// result = 1 - (1 - d) * (1 - s)
		//        = (1 - s) * d + s
		//
		// �i�G�t�F�N�g���� a ��K�p�����Ă��܂��j

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_ONE);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}
	else	{												// �ʏ�

		// result = d * (1 - a) + s * a

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}

	return true;
}

// ****************************************************************
// �G�t�F�N�g���J�n�i�Q�c�j
// ================================================================
bool C_d3d_render_interface::render_sprite_begin_effect_d2(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, ID3DXEffect** pp_effect)
{
	HRESULT hr;

	ID3DXEffect* p_effect = p_sprite->get_effect();
	D3DXHANDLE h_technique = NULL;
	UINT pass_cnt = 0;

	// �G�t�F�N�g���w�肳��Ă��Ȃ��ꍇ�̓f�t�H���g�̃G�t�F�N�g���g��
	if (p_effect == NULL)	{

		// �G�t�F�N�g�̐���
		if (rp->use_mask && rp->light_no >= 0)	{
			set_last_error(_T("�G�t�F�N�g�̑g�ݍ��킹�ŃG���[���������܂����B(1)"), _T("SetEffect"));
			return false;
		}
		if (!rp->d3_rect && rp->light_no >= 0)	{
			set_last_error(_T("�G�t�F�N�g�̑g�ݍ��킹�ŃG���[���������܂����B(2)"), _T("SetEffect"));
			return false;
		}

		// �G�t�F�N�g���f�t�H���g�ɐݒ肷��
		p_effect = m_p_renderer->get_default_effect()->body();

		// �e�N�j�b�N���擾����
		h_technique = p_sprite->get_effect_technique();
		int tec_type[10] = {0};

		// �e�N�j�b�N���w�肳��Ă��Ȃ��ꍇ�͎�������
		if (h_technique == NULL)	{

			// �Q�c
			tec_type[0] = 0;
			// ���C�g
			tec_type[1] = 0;
			// �t�H�O
			tec_type[2] = 0;
			// �e�N�X�`��
			if (p_sprite->get_texture(0) && p_sprite->get_texture(0)->body())	{
				tec_type[3] = 1;
			}
			// diffuse
			if (rp->filter.color < 0xffffffff || rp->tr < 255)	{
				tec_type[4] = 1;
			}
			// mono, reverse, bright, dark
			if (rp->mono > 0 || rp->reverse > 0 || rp->bright > 0 || rp->dark > 0)	{
				tec_type[5] = 1;
			}
			// color, color_add
			if (rp->color_rate > 0 || rp->color_add_r > 0 || rp->color_add_g > 0 || rp->color_add_b > 0)	{
				tec_type[6] = 1;
			}
			// �g�[���J�[�u
			if (rp->tone_curve_no >= 0)	{
				tec_type[7] = 1;
			}
			// �}�X�N
			if (rp->use_mask == 1)	{
				tec_type[8] = 1;
			}
			// ��Z�id * s * a �� Direct3D �ł͂ł��Ȃ��̂ŁAs = s * a ���Ɍv�Z���Ă����A��� d * s �Ƃ���B�j
			if (rp->blend_type == E_d3d_blend_type_mul)	{
				tec_type[9] = 1;
			}
			// �X�N���[��
			if (rp->blend_type == E_d3d_blend_type_screen)	{
				tec_type[9] = 2;
			}

			// �G�t�F�N�g���ʂ��S���Ȃ��Ȃ�G�t�F�N�g���g��Ȃ��i�e�N�X�`���͏����j
			if (tec_type[1] == 0 && tec_type[3] == 0 && tec_type[4] == 0 && tec_type[5] == 0 && tec_type[6] == 0 && tec_type[7] == 0 && tec_type[8] == 0 && tec_type[9] == 0)	{

				// �G�t�F�N�g��������
				p_effect = NULL;
			}
		}

		// ���ǃG�t�F�N�g���g���ꍇ
		if (p_effect)	{

			// �e�N�j�b�N���w�肳��Ă��Ȃ��ꍇ�͎�������
			if (h_technique == NULL)	{
				h_technique = m_p_renderer->get_default_effect()->m_sprite_technique[tec_type[0]][tec_type[1]][tec_type[2]][tec_type[3]][tec_type[4]][tec_type[5]][tec_type[6]][tec_type[7]][tec_type[8]][tec_type[9]];

				// �G�t�F�N�g�̃e�N�X�`����ݒ肷��
				for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)	{
					if (p_sprite->get_texture(t) && p_sprite->get_texture(t)->body())	{
						hr = p_effect->SetTexture(m_p_renderer->get_default_effect()->m_texture[t], p_sprite->get_texture(t)->body());
						if (FAILED(hr))	{
							set_last_error_d3d(_T("�G�t�F�N�g�̃e�N�X�`���̐ݒ�Ɏ��s���܂����B"), _T("SetTexture"), hr);
							return false;
						}
					}
				}

				// �G�t�F�N�g�̒萔��ݒ肷��
				if (rp->mono > 0 || rp->reverse > 0 || rp->bright > 0 || rp->dark > 0 || rp->color_rate > 0 || rp->color_add_r > 0 || rp->color_add_g > 0 || rp->color_add_b > 0)	{

					// mono, reverse, bright, dark ��萔�Ƃ��đ���
					float c0[4] = {(float)rp->mono / 255, (float)rp->reverse / 255, (float)rp->bright / 255, (float)rp->dark / 255};
					hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[0], c0, 4);
					if (FAILED(hr))	{
						set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
						return false;
					}

					// r, g, b, rate ��萔�Ƃ��đ���
					float c1[4] = {(float)rp->color_r / 255, (float)rp->color_g / 255, (float)rp->color_b / 255, (float)rp->color_rate / 255};
					hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[1], c1, 4);
					if (FAILED(hr))	{
						set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
						return false;
					}

					// r, g, b, ��萔�Ƃ��đ���
					float c2[4] = {(float)rp->color_add_r / 255, (float)rp->color_add_g / 255, (float)rp->color_add_b / 255};
					hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[2], c2, 4);
					if (FAILED(hr))	{
						set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
						return false;
					}
				}
				if (rp->tone_curve_no >= 0)	{

					// �g�[���J�[�u����萔�Ƃ��đ���
					float c3[4] = {((float)(BYTE)rp->tone_curve_no + 0.5f) / 255, (float)rp->tone_curve_sat / 255};
					hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[3], c3, 4);
					if (FAILED(hr))	{
						set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
						return false;
					}
				}
			}

			// �e�N�j�b�N���w�肳��Ă���ꍇ�͂���ɏ]��
			else	{

				// �G�t�F�N�g�̃e�N�X�`����ݒ肷��
				for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)	{
					if (p_sprite->get_texture(t) && p_sprite->get_texture(t)->body())	{
						hr = p_effect->SetTexture(m_p_renderer->get_default_effect()->m_texture[t], p_sprite->get_texture(t)->body());
						if (FAILED(hr))	{
							set_last_error_d3d(_T("�G�t�F�N�g�̃e�N�X�`���̐ݒ�Ɏ��s���܂����B"), _T("SetTexture"), hr);
							return false;
						}
					}
				}

				// �G�t�F�N�g�̒萔�𑗂�
				hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[0], p_sprite->get_effect_constant_f(0), 4);
				if (FAILED(hr))	{
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
					return false;
				}
				hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[1], p_sprite->get_effect_constant_f(1), 4);
				if (FAILED(hr))	{
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
					return false;
				}
			}

			// �G�t�F�N�g�̃e�N�j�b�N��ݒ肷��
			hr = p_effect->SetTechnique(h_technique);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("�G�t�F�N�g�e�N�j�b�N�̐ݒ�Ɏ��s���܂����B"), _T("SetTechnique"), hr);
				return false;
			}

			// �G�t�F�N�g���J�n����
			hr = p_effect->Begin(&pass_cnt, 0);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("�G�t�F�N�g�̊J�n�Ɏ��s���܂����B"), _T("Begin"), hr);
				return false;
			}

			// �p�X�����s����
			hr = p_effect->BeginPass(0);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("�G�t�F�N�g�̃p�X�̐ݒ�Ɏ��s���܂����B"), _T("BeginPass"), hr);
				return false;
			}

			// �g�p�����G�t�F�N�g��Ԃ�
			*pp_effect = p_effect;
		}

		// �G�t�F�N�g���g��Ȃ��ꍇ
		else	{

			// �e�N�X�`����ݒ肷��
			for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)	{
				if (p_sprite->get_texture(t) && p_sprite->get_texture(t)->body())	{
					hr = G_d3d.device->SetTexture(t, p_sprite->get_texture(t)->body());
					if (FAILED(hr))	{
						set_last_error_d3d(_T("�G�t�F�N�g�̃e�N�X�`���̐ݒ�Ɏ��s���܂����B"), _T("SetTexture"), hr);
						return false;
					}
				}
			}
		}
	}

	// �G�t�F�N�g���w�肳��Ă���ꍇ�F��������

	return true;
}

// ****************************************************************
// �G�t�F�N�g���I���i�Q�c�j
// ================================================================
bool C_d3d_render_interface::render_sprite_end_effect_d2(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect)
{
	HRESULT hr;

	if (p_effect == NULL)
		return true;

	hr = p_effect->EndPass();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�G�t�F�N�g�̃p�X�̏I���Ɏ��s���܂����B"), _T("EndPass"), hr);
		return false;
	}

	hr = p_effect->End();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�G�t�F�N�g�̏I���Ɏ��s���܂����B"), _T("End"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// �G�t�F�N�g���J�n�i�R�c�j
// ================================================================
bool C_d3d_render_interface::render_sprite_begin_effect_d3(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, ID3DXEffect** pp_effect)
{
	HRESULT hr;
	ID3DXEffect* p_effect = m_p_renderer->get_default_effect()->body();

	// �e�N�X�`����ݒ肷��
	int texture_cnt = 0;
	for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)	{
		if (p_sprite->get_texture(t) && p_sprite->get_texture(t)->body())	{
			TSTR name = str_format(_T("Tex%02d"), t);
			hr = p_effect->SetTexture(TSTR_to_MBSTR(name).c_str(), p_sprite->get_texture(t)->body());
			if (FAILED(hr))	{
				set_last_error_d3d(_T("�G�t�F�N�g�̃e�N�X�`���̐ݒ�Ɏ��s���܂����B"), _T("SetTexture"), hr);
				return false;
			}

			texture_cnt = t + 1;	// �ő�l���e�N�X�`�����ɂȂ�
		}
	}

	// �G�t�F�N�g�̐ݒ葱��
	if (!render_sprite_begin_effect_d3_sub(p_sprite, rp, NULL, texture_cnt, p_effect))
		return false;

	*pp_effect = p_effect;
	return true;
}

bool C_d3d_render_interface::render_sprite_begin_effect_d3_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_mtrl* p_mtrl, ID3DXEffect** pp_effect)
{
	HRESULT hr;
	ID3DXEffect* p_effect = m_p_renderer->get_default_effect()->body();

	// �e�N�X�`����ݒ肷��
	int texture_cnt = 0;
	if (p_mtrl->texture && p_mtrl->texture->body())	{
		TSTR name = str_format(_T("Tex%02d"), texture_cnt);
		hr = p_effect->SetTexture(TSTR_to_MBSTR(name).c_str(), p_mtrl->texture->body());
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�G�t�F�N�g�̃e�N�X�`���̐ݒ�Ɏ��s���܂����B"), _T("SetTexture"), hr);
			return false;
		}

		p_sprite->set_texture(p_mtrl->texture);

		texture_cnt = 1;	// �ő�l���e�N�X�`�����ɂȂ�
	}
	//p_sprite->rp.light_no = 0;

	// �}�e���A���̐F��ݒ肷��
	

	// �G�t�F�N�g�̐ݒ葱��
	if (!render_sprite_begin_effect_d3_sub(p_sprite, rp, p_mtrl, texture_cnt, p_effect))
		return false;

	*pp_effect = p_effect;
	return true;
}

bool C_d3d_render_interface::render_sprite_begin_effect_d3_sub(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, const C_d3d_mtrl* p_mtrl, int texture_cnt, ID3DXEffect* p_effect)
{
	HRESULT hr;

	// �e�N�j�b�N���擾����
	D3DXHANDLE h_technique = p_sprite->get_effect_technique();
	int tec_type[10] = {0};

	// �e�N�j�b�N���w�肳��Ă��Ȃ��ꍇ�͎�������
	if (h_technique == NULL)	{

		// �G�t�F�N�g�̐���
		if (rp->use_mask && rp->light_no >= 0)	{
			set_last_error(_T("�G�t�F�N�g�̑g�ݍ��킹�ŃG���[���������܂����B(1)"), _T("SetEffect"));
			return false;
		}

		// �R�c
		tec_type[0] = 1;
		// ���C�g
		if (p_sprite->rp.light_no >= 0)	{
			tec_type[1] = 1;
		}
		// �t�H�O
		if (m_p_renderer->m_fog_use && p_sprite->rp.use_fog)	{
			tec_type[2] = 1;
		}
		// �e�N�X�`��
		if (p_sprite->get_texture(0) && p_sprite->get_texture(0)->body())	{
			tec_type[3] = 1;
		}
		// diffuse�i���C�g�̓s���ォ�Ȃ炸�ݒ肷��j
		tec_type[4] = 1;
		// mono, reverse, bright, dark
		if (rp->mono > 0 || rp->reverse > 0 || rp->bright > 0 || rp->dark > 0)	{
			tec_type[5] = 1;
		}
		// color, color_add
		if (rp->color_rate > 0 || rp->color_add_r > 0 || rp->color_add_g > 0 || rp->color_add_b > 0)	{
			tec_type[6] = 1;
		}
		// �g�[���J�[�u
		if (rp->tone_curve_no >= 0)	{
			tec_type[7] = 1;
		}
		// �}�X�N
		if (rp->use_mask == 1)	{
			tec_type[8] = 1;
		}
		// ��Z�id * s * a �� Direct3D �ł͂ł��Ȃ��̂ŁAs = s * a ���Ɍv�Z���Ă����A��� d * s �Ƃ���B�j
		if (rp->blend_type == E_d3d_blend_type_mul)	{
			tec_type[9] = 1;
		}
		// �X�N���[��
		if (rp->blend_type == E_d3d_blend_type_screen)	{
			tec_type[9] = 2;
		}

		// �e�N�j�b�N���擾����
		h_technique = m_p_renderer->get_default_effect()->m_sprite_technique[tec_type[0]][tec_type[1]][tec_type[2]][tec_type[3]][tec_type[4]][tec_type[5]][tec_type[6]][tec_type[7]][tec_type[8]][tec_type[9]];

		// �e�N�j�b�N��ݒ肷��
		hr = p_effect->SetTechnique(h_technique);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�G�t�F�N�g�e�N�j�b�N�̐ݒ�Ɏ��s���܂����B"), _T("SetTechnique"), hr);
			return false;
		}

		// �G�t�F�N�g�̒萔��ݒ肷��
		if (rp->mono > 0 || rp->reverse > 0 || rp->bright > 0 || rp->dark > 0 || rp->color_rate > 0 || rp->color_add_r > 0 || rp->color_add_g > 0 || rp->color_add_b > 0)	{

			// mono, reverse, bright, dark ��萔�Ƃ��đ���
			float c0[4] = {(float)rp->mono / 255, (float)rp->reverse / 255, (float)rp->bright / 255, (float)rp->dark / 255};
			hr = p_effect->SetFloatArray("c0", c0, 4);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
				return false;
			}

			// r, g, b, rate ��萔�Ƃ��đ���
			float c1[4] = {(float)rp->color_r / 255, (float)rp->color_g / 255, (float)rp->color_b / 255, (float)rp->color_rate / 255};
			hr = p_effect->SetFloatArray("c1", c1, 4);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
				return false;
			}

			// r, g, b, ��萔�Ƃ��đ���
			float c2[4] = {(float)rp->color_add_r / 255, (float)rp->color_add_g / 255, (float)rp->color_add_b / 255};
			hr = p_effect->SetFloatArray("c2", c2, 4);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
				return false;
			}
		}
		if (rp->tone_curve_no >= 0)	{

			// �g�[���J�[�u����萔�Ƃ��đ���
			float c3[4] = {((float)(BYTE)rp->tone_curve_no + 0.5f) / 255, (float)rp->tone_curve_sat / 255};
			hr = p_effect->SetFloatArray("c3", c3, 4);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
				return false;
			}
		}
	}

	// �e�N�j�b�N���w�肳��Ă���ꍇ�͂���ɏ]��
	else	{

		// �e�N�j�b�N��ݒ肷��
		hr = p_effect->SetTechnique(h_technique);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�G�t�F�N�g�e�N�j�b�N�̐ݒ�Ɏ��s���܂����B"), _T("SetTechnique"), hr);
			return false;
		}

		// �萔�𑗂�
		hr = p_effect->SetFloatArray("c0", p_sprite->get_effect_constant_f(0), 4);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
			return false;
		}
		hr = p_effect->SetFloatArray("c1", p_sprite->get_effect_constant_f(1), 4);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloatArray"), hr);
			return false;
		}
	}

	D3DXMATRIX mat_temp, mat_world, mat_view, mat_proj, mat_view_proj;

	// ���[���h���W�ϊ��s����쐬
	D3DXMatrixIdentity(&mat_world);
	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixScaling(&mat_temp, rp->scale.x, rp->scale.y, rp->scale.z));					// �g�k
	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixRotationYawPitchRoll(&mat_temp, rp->rotate.y, rp->rotate.x, rp->rotate.z));	// ��]
	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixTranslation(&mat_temp, rp->pos.x, rp->pos.y, rp->pos.z));						// �ړ�

	// �r���[�Ǝˉe�̍����s������߂Ă���
	mat_view = *m_p_renderer->get_view_matrix();
	mat_proj = *m_p_renderer->get_proj_matrix();
	D3DXMatrixMultiply(&mat_view_proj, m_p_renderer->get_view_matrix(), m_p_renderer->get_proj_matrix());

	// �s���ݒ肷��
	p_effect->SetMatrix("g_mat_world", &mat_world);
	p_effect->SetMatrix("g_mat_view_proj", &mat_view_proj);

	// �s��Ƃ͕ʂɐݒ肷��i���C�g�̌v�Z�Ɏg���j
	float canera_dir[3] = {m_p_renderer->m_camera_dir.x, m_p_renderer->m_camera_dir.y, m_p_renderer->m_camera_dir.z};
	p_effect->SetFloatArray("g_camera_dir", canera_dir, 3);

	// �}�e���A���J���[��ݒ肷��
	if (p_mtrl)	{
		p_effect->SetFloatArray("g_mtrl_diffuse", (float *)&p_mtrl->mtrl.Diffuse, 4);
	}
	else	{
		float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		p_effect->SetFloatArray("g_mtrl_diffuse", white, 4);
	}

	// ���C�g��ݒ肷��
	if (p_sprite->rp.light_no >= 0)	{
		C_d3d_light light = G_d3d.state.get_light(p_sprite->rp.light_no);
		float light_pos[4] = {light.Position.x, light.Position.y, light.Position.z, 1.0f};	// x, y, z, use
		float light_dir[3] = {light.Direction.x, light.Direction.y, light.Direction.z};
		float light_diffuse[4] = {light.Diffuse.r, light.Diffuse.g, light.Diffuse.b, light.Diffuse.a};
		float light_ambient[4] = {light.Ambient.r, light.Ambient.g, light.Ambient.b, light.Ambient.a};
		//int camera_light_calc_type = light.camera_light_calc_type;
		//int ambient_light_calc_type = light.ambient_light_calc_type;

		p_effect->SetFloatArray("g_light_pos", light_pos, 4);
		//p_effect->SetFloatArray("g_light_dir", light_dir, 3);
		//p_effect->SetFloatArray("g_light_diffuse", light_diffuse, 4);
		p_effect->SetFloatArray("g_light_ambient", light_ambient, 4);
		//p_effect->SetInt("g_camera_light_calc_type", camera_light_calc_type);
		//p_effect->SetInt("g_ambient_light_calc_type", ambient_light_calc_type);
	}
	if (m_p_renderer->m_fog_use && p_sprite->rp.use_fog)	{
		const int TNM_TEXTURE_STAGE_FOG = 3;
		if (p_sprite->get_texture(TNM_TEXTURE_STAGE_FOG))	{
			float fog_tex_width = (float)p_sprite->get_texture(TNM_TEXTURE_STAGE_FOG)->get_width_ex();
			float fog_tex_height = (float)p_sprite->get_texture(TNM_TEXTURE_STAGE_FOG)->get_height_ex();
			float aspect = fog_tex_height / m_default_view_port.height();

			float fog_w = m_default_view_port.width() / fog_tex_width * aspect;
			float fog_h = m_default_view_port.height() / fog_tex_height;
			float fog_x = - m_p_renderer->m_fog_x / fog_tex_width * aspect - 0.5f / m_default_view_port.width();
			float fog_y = 0.5f / m_default_view_port.height();

			float fog_param[4] = {fog_x, fog_y, fog_w, fog_h};	// x, y, w, h
			float fog_range[4] = {1.0f, m_p_renderer->m_fog_near, m_p_renderer->m_fog_far, 0.0f};	// use, near, far, (keep)
			float camera_pos[3] = {m_p_renderer->m_camera_eye.x, m_p_renderer->m_camera_eye.y, m_p_renderer->m_camera_eye.z};

			p_effect->SetFloatArray("g_fog_param", fog_param, 4);
			p_effect->SetFloatArray("g_fog_range", fog_range, 4);
			p_effect->SetFloatArray("g_camera_pos", camera_pos, 3);
		}
	}

	// �G�t�F�N�g���J�n����
	UINT pass_cnt = 0;
	hr = p_effect->Begin(&pass_cnt, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�G�t�F�N�g�̊J�n�Ɏ��s���܂����B"), _T("Begin"), hr);
		return false;
	}

	// �p�X�����s����
	hr = p_effect->BeginPass(0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�G�t�F�N�g�̃p�X�̎��s�Ɏ��s���܂����B"), _T("BeginPass"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// �G�t�F�N�g���J�n�i�R�c�j�]�v�Ȏ��͂��Ȃ�
// ================================================================
bool C_d3d_render_interface::render_sprite_begin_effect(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect)
{
	HRESULT hr;

	// �G�t�F�N�g���J�n����
	UINT pass_cnt = 0;
	hr = p_effect->Begin(&pass_cnt, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�G�t�F�N�g�̊J�n�Ɏ��s���܂����B"), _T("Begin"), hr);
		return false;
	}

	// �p�X�����s����
	hr = p_effect->BeginPass(0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�G�t�F�N�g�̃p�X�̎��s�Ɏ��s���܂����B"), _T("BeginPass"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// �G�t�F�N�g���I���i�R�c�j
// ================================================================
bool C_d3d_render_interface::render_sprite_end_effect_d3(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect)
{
	HRESULT hr;

	hr = p_effect->EndPass();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�G�t�F�N�g�̃p�X�̏I���Ɏ��s���܂����B"), _T("EndPass"), hr);
		return false;
	}

	hr = p_effect->End();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�G�t�F�N�g�̏I���Ɏ��s���܂����B"), _T("End"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// �X�v���C�g�F�|���S���������_�[
// ================================================================
bool C_d3d_render_interface::render_sprite_polygon_d2(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, int vertex_cnt, int primitive_cnt)
{
	HRESULT hr;

	if (!p_sprite->get_vertex_buffer() || !p_sprite->get_index_buffer())
		return true;	// �G���[�ł͂Ȃ�

#if 1	// �ŐV�o�[�W�����i�C���f�b�N�X�o�b�t�@���g�������o�[�W�����j

	// �G�t�F�N�g���J�n����
	ID3DXEffect* p_effect = NULL;
	if (!render_sprite_begin_effect_d2(p_sprite, rp, &p_effect))
		return false;

	// ���_���X�g���[���ɗ�������
	// SetStreamSource �̑�R�����͂O�ȊO���w�肵�Ă͂����܂���BGeForce 7300 GS �œ����܂���B�i�U�e�X�^�W�I�}�V���Ŋm�F�j
	// �v���O�����̃~�X�Ȃ̂�����ڂ̎d�l�Ȃ̂��o�O�Ȃ̂�������܂���B
	G_d3d.device->SetStreamSource(0, p_sprite->get_vertex_buffer()->body(), 0, p_sprite->get_FVF_size());
	G_d3d.device->SetIndices(p_sprite->get_index_buffer()->body());		// �C���f�b�N�X�o�b�t�@�̐ݒ�
	G_d3d.device->SetFVF(p_sprite->get_FVF());							// FVF �̐ݒ�

	// �`����s��
	// DrawIndexedPrimitive �̑�Q�����ɂO�ȊO���w�肵�Ă͂����܂���BIntel GMA 4500MHD �œ����܂���B�i�g�c����ƒ�p�o�b�Ŋm�F�j
	// �v���O�����̃~�X�Ȃ̂�����ڂ̎d�l�Ȃ̂��o�O�Ȃ̂�������܂���B
	hr = G_d3d.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, p_sprite->get_vertex_offset(), vertex_cnt, p_sprite->get_index_offset(), primitive_cnt);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("DrawIndexedPrimitive"), hr);
		return false;
	}

	// �G�t�F�N�g���I������
	if (!render_sprite_end_effect_d2(p_sprite, p_effect))
		return false;

#elif 0	// Intel GMA 4500MHD �œ����Ȃ������o�[�W�����i�g�c����ƒ�p�o�b�Ŋm�F�j

	// ���_���X�g���[���ɗ�������
	Gd3d.device->SetStreamSource(0, p_sprite->get_vertex_buffer()->body(), 0, p_sprite->get_FVF_size());
	Gd3d.device->SetIndices(p_sprite->get_index_buffer()->body());	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	Gd3d.device->SetFVF(p_sprite->get_FVF());						// FVF �̐ݒ�

	// �G�t�F�N�g���J�n����
	ID3DXEffect* p_effect = NULL;
	if (!render_sprite_begin_effect_d2(p_sprite, rp, &p_effect))
		return false;

	// �`����s��
	hr = Gd3d.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, p_sprite->get_vertex_offset(), 0, p_sprite->get_vertex_count(), 0, p_sprite->get_index_count() / 3);
	if (FAILED(hr))
		return set_my_last_error_d3d(_T("���_�̕`��Ɏ��s���܂����B"), hr);

	// �G�t�F�N�g���I������
	if (!render_sprite_end_effect_d2(p_sprite, p_effect))
		return false;

#else	// GeForce 7300 GS �œ����Ȃ������o�[�W�����i�U�e�X�^�W�I�}�V���Ŋm�F�j

	// ���_���X�g���[���ɗ�������
	Gd3d.device->SetStreamSource(0, p_sprite->get_vertex_buffer()->body(), p_sprite->get_vertex_offset() * p_sprite->get_FVF_size(), p_sprite->get_FVF_size());
	Gd3d.device->SetIndices(p_sprite->get_index_buffer()->body());	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	Gd3d.device->SetFVF(p_sprite->get_FVF());						// FVF �̐ݒ�

	// �G�t�F�N�g���J�n����
	ID3DXEffect* p_effect = NULL;
	if (!render_sprite_begin_effect_d2(p_sprite, rp, &p_effect))
		return false;

	// �`����s��
	hr = Gd3d.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, p_sprite->get_vertex_count(), 0, p_sprite->get_index_count() / 3);
	if (FAILED(hr))
		return set_my_last_error_d3d(_T("���_�̕`��Ɏ��s���܂����B"), hr);

	// �G�t�F�N�g���I������
	if (!render_sprite_end_effect_d2(p_sprite, p_effect))
		return false;

#endif

	return true;
}

// ****************************************************************
// �X�v���C�g�F�|���S���������_�[�i�R�c�j
// ================================================================
bool C_d3d_render_interface::render_sprite_polygon_d3(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, int vertex_cnt, int primitive_cnt)
{
	HRESULT hr;

	// ���_���X�g���[���ɗ�������
	// SetStreamSource �̑�R�����͂O�ȊO���w�肵�Ă͂����܂���BGeForce 7300 GS �œ����܂���B�i�U�e�X�^�W�I�}�V���Ŋm�F�j
	// �v���O�����̃~�X�Ȃ̂�����ڂ̎d�l�Ȃ̂��o�O�Ȃ̂�������܂���B
	if (p_sprite && p_sprite->get_vertex_buffer())	{
		G_d3d.device->SetStreamSource(0, p_sprite->get_vertex_buffer()->body(), 0, p_sprite->get_FVF_size());
		G_d3d.device->SetIndices(p_sprite->get_index_buffer()->body());		// �C���f�b�N�X�o�b�t�@�̐ݒ�
		G_d3d.device->SetFVF(p_sprite->get_FVF());							// FVF �̐ݒ�

		// �G�t�F�N�g���J�n����
		ID3DXEffect* p_effect = NULL;
		if (!render_sprite_begin_effect_d3(p_sprite, rp, &p_effect))
			return false;

		// �`����s��
		// DrawIndexedPrimitive �̑�Q�����ɂO�ȊO���w�肵�Ă͂����܂���BIntel GMA 4500MHD �œ����܂���B�i�g�c����ƒ�p�o�b�Ŋm�F�j
		// �v���O�����̃~�X�Ȃ̂�����ڂ̎d�l�Ȃ̂��o�O�Ȃ̂�������܂���B
		hr = G_d3d.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, p_sprite->get_vertex_offset(), vertex_cnt, p_sprite->get_index_offset(), primitive_cnt);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("DrawIndexedPrimitive"), hr);
			return false;
		}

		// �G�t�F�N�g���I������
		if (!render_sprite_end_effect_d3(p_sprite, p_effect))
			return false;
	}

	return true;
}

// ****************************************************************
// �X�v���C�g�F���b�V���������_�[
// ================================================================
bool C_d3d_render_interface::render_sprite_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_mesh* mesh)
{
	// �}�e���A����S�ĕ`�悷��

	// �A���t�@�u�����h���s�����̂ƍs��Ȃ����̂ɕ�����
	std::vector<int> alpha_list, no_alpha_list;
	for (int m = 0; m < mesh->get_mtrl_cnt(); m++)	{
		// ���b�V���̕\���t���O�������Ă���ꍇ
		if (mesh->get_mtrl(m)->disp)	{
			if (mesh->get_mtrl(m)->alpha_blend)		alpha_list.push_back(m);
			else									no_alpha_list.push_back(m);
		}
	}

	// �A���t�@�u�����h���s��Ȃ����̂�`��
	G_d3d.state.set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
	for (int i = 0; i < (int)no_alpha_list.size(); i++)	{
		int mtrl_no = no_alpha_list[i];

		// �}�e���A����`��
		if (!render_sprite_mesh_mtrl(p_sprite, rp, mesh, mtrl_no))
			return false;
	}

	// �A���t�@�u�����h���s�����̂�`��
	G_d3d.state.set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
	for (int i = 0; i < (int)alpha_list.size(); i++)	{
		int mtrl_no = alpha_list[i];

		// �}�e���A����`��
		if (!render_sprite_mesh_mtrl(p_sprite, rp, mesh, mtrl_no))
			return false;
	}

	return true;
}

// ****************************************************************
// �X�v���C�g�F���b�V���̃}�e���A���������_�[
// ================================================================
bool C_d3d_render_interface::render_sprite_mesh_mtrl(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_mesh* mesh, int mtrl_no)
{
	C_d3d_mtrl* mtrl = mesh->get_mtrl(mtrl_no);
	if (!mtrl)
		return false;

	// �G�t�F�N�g���J�n����
	ID3DXEffect* p_effect = NULL;
	if (!render_sprite_begin_effect_d3_mesh(p_sprite, rp, mtrl, &p_effect))
		return false;

	// ���̃}�e���A���̃��b�V����`��
	(*mesh)->DrawSubset(mtrl_no);

	// �G�t�F�N�g���I������
	if (!render_sprite_end_effect_d3(p_sprite, p_effect))
		return false;

	return true;
}



// ****************************************************************
// �X�v���C�g�F�t���[���̃��b�V���T�u�Z�b�g��`��
// ================================================================
bool C_d3d_render_interface::render_sprite_frame_mesh_subset(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no)
{
	//HRESULT hr;

	// �g���₷���悤�Ɉꎞ�ϐ��ɑ��
	BSP<C_d3d_mesh>					mesh = p_frame->m_mesh_list[mesh_no];
	S_d3d_skinned_mesh_container*	p_mc = (S_d3d_skinned_mesh_container *)p_frame->body()->pMeshContainer;
	C_d3d_mtrl*						mtrl = mesh->get_mtrl(mtrl_no);
	
	// �}�e���A�����Ȃ��ꍇ(�G���[�����s���H)
	if (!mtrl)
		return false;

	if (!render_sprite_mesh_mtrl(p_sprite, p_rp, mesh.get(), mtrl_no))
		return false;

	/*
	// ������͖{���p�����[�^���猈�߂�ׂ�
	// �Œ�p�C�v���C���Œ��_�u�����f�B���O���s��Ȃ�
	G_d3d.state.set_render_state(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	G_d3d.state.set_render_state(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	// Z�o�b�t�@��L���ɂ���
	G_d3d.state.set_render_state(D3DRS_ZENABLE, TRUE);
	G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, TRUE);
	// �A���t�@�u�����f�B���O�𖳌��ɂ���
	G_d3d.state.set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
	// �����v���ɃJ�����O����
	G_d3d.state.set_render_state(D3DRS_CULLMODE, D3DCULL_CCW);
	*/

	// �G�t�F�N�g�𐶐��������͎擾����
	//ID3DXEffect* p_effect = render_sprite_frame_per_mesh_create_effect(mesh.get(), mtrl, p_mc->pSkinInfo);
	//if (!p_effect)
	//	return false;

	// �G�t�F�N�g�̒萔��ݒ肷��
	//if (!render_sprite_frame_per_mesh_set_effect_constant(p_sprite, p_rp, p_frame, mesh_no, mtrl_no, subset_no, p_effect, p_mc->pSkinInfo))
	//	return false;

#if 0
	DWORD fvf = (*mesh)->GetFVF();
	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	(*mesh)->GetDeclaration(pDecl);

	OutputDebugStringA("***");
	OutputDebugStringA(p_mc->Name);
	OutputDebugStringA("***\n");
#endif

	/*

	// �G�t�F�N�g���J�n����
	UINT pass_max = 0;
	hr = p_effect->Begin(&pass_max, 0);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̊J�n�Ɏ��s���܂����B"), _T("Begin"), hr);
		return false;
	}

	// �p�X�̉񐔂�����
	for (UINT pass = 0; pass < pass_max; pass++) {
		// �p�X���J�n����
		hr = p_effect->BeginPass(pass);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̃p�X�̎��s�Ɏ��s���܂����B"), _T("BeginPass"), hr);
			return false;
		}

		//D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];;
		//(*mesh)->GetDeclaration(pDecl);
		//LPDIRECT3DVERTEXDECLARATION9	m_pDecl;// ���_�錾
		//G_d3d.device.body()->CreateVertexDeclaration(pDecl, &m_pDecl);
		//G_d3d.device.body()->SetVertexDeclaration(m_pDecl);

		// ���̃}�e���A���̃��b�V����`��
		hr = (*mesh)->DrawSubset(subset_no);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("DrawSubset"), hr);
			return false;
		}

		// �p�X���I������
		hr = p_effect->EndPass();
		if (FAILED(hr))	{
			set_last_error_d3d(_T("�G�t�F�N�g�̃p�X�̏I���Ɏ��s���܂����B"), _T("EndPass"), hr);
			return false;
		}
	}

	// �G�t�F�N�g���I������
	hr = p_effect->End();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("�G�t�F�N�g�̏I���Ɏ��s���܂����B"), _T("End"), hr);
		return false;
	}
	*/

	return true;
}

bool C_d3d_render_interface::render_sprite_frame_per_mesh_set_effect_constant_common(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect)
{
	HRESULT hr;

	// �g���₷���悤�Ɉꎞ�ϐ��ɑ��
	BSP<C_d3d_mesh>					mesh = p_frame->m_mesh_list[mesh_no];
	S_d3d_skinned_mesh_container*	p_mc = (S_d3d_skinned_mesh_container *)p_frame->body()->pMeshContainer;
	C_d3d_mtrl*						mtrl = mesh->get_mtrl(mtrl_no);
	S_d3d_mesh_param*				p_mp = mesh->get_mesh_param_ptr();
	DWORD							fvf = mesh->body()->GetFVF();

	// �e�N�j�b�N�̐ݒ�
	hr = p_effect->SetTechnique("tech");
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�e�N�j�b�N�̐ݒ�Ɏ��s���܂����B"), _T("SetTechnique"), hr);
		return false;
	}

	// �r���[�ϊ��s��̐ݒ�
	hr = p_effect->SetMatrix("mView", m_p_renderer->get_view_matrix());
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrix"), hr);
		return false;
	}

	// �ˉe�ϊ��s��̐ݒ�
	hr = p_effect->SetMatrix("mProj", m_p_renderer->get_proj_matrix());
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrix"), hr);
		return false;
	}

	// �J�����̐ݒ�
	{
		// ���[���h�ł̍��W(���N���X�̂܂܂Ŗ��Ȃ�)
		hr = p_effect->SetVector("g_CameraWorldPos", &m_p_renderer->get_camera()->get_camera_world_pos());
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
			return false;
		}
		// ���[�J���ł̍��W(���N���X�̂܂܂Ŗ��Ȃ�)
		hr = p_effect->SetVector("g_CameraLocalPos", &m_p_renderer->get_camera()->get_camera_local_pos(p_frame->body()->CombinedTransformationMatrix));
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
			return false;
		}
	}

	// ���C�g�̐ݒ�
	{
		/*
		// �f�B���N�V���i��
		{
			ARRAY<D3DXVECTOR4> w_dir;
			ARRAY<D3DXVECTOR4> l_dir;
			ARRAY<D3DXVECTOR4> diffuse;
			ARRAY<D3DXVECTOR4> ambient;
			ARRAY<D3DXVECTOR4> specular;

			int light_num = 0;
			for(int i = 0; i < (int)p_mp->directional_light_id_list.size(); i++) {
				// �ݒ�\�����z�����烋�[�v�𔲂���
				if(i >= C_d3d_light_manager::OBJ_DIRECTIONAL_LIGHT_MAX)
					break;
				C_d3d_directional_light* p_light = dynamic_cast<C_d3d_directional_light*>(G_d3d_light_manager.search(p_mp->directional_light_id_list[i]).get());

				// ���C�g�̃f�[�^��ǉ����Ă���
				w_dir.push_back(p_light->get_light_world_dir());
				l_dir.push_back(p_light->get_light_local_dir(p_frame->body()->CombinedTransformationMatrix));

				diffuse.push_back(p_light->get_diffuse());
				ambient.push_back(p_light->get_ambient());
				specular.push_back(p_light->get_specular());

				// ���C�g�̐����J�E���g
				light_num++;
			}

			if(light_num > 0) {
				// ���ۂ̃��C�g�̐�
				hr = p_effect->SetInt("g_DLightNum", light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetInt"), hr);
					return false;
				}
				// ���[���h�ł̕����x�N�g��
				hr = p_effect->SetVectorArray("g_DLightWorldDir", w_dir.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// ���[�J���ł̕����x�N�g��
				hr = p_effect->SetVectorArray("g_DLightLocalDir", l_dir.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}

				// �f�B�t���[�Y
				hr = p_effect->SetVectorArray("g_DLightDiffuse", diffuse.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// �A���r�G���g
				hr = p_effect->SetVectorArray("g_DLightAmbient", ambient.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// �X�y�L����
				hr = p_effect->SetVectorArray("g_DLightSpecular", specular.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
			}
		}




		// �|�C���g
		{
			ARRAY<D3DXVECTOR4> w_pos;
			ARRAY<D3DXVECTOR4> l_pos;
			ARRAY<D3DXVECTOR4> atten012_range;
			ARRAY<D3DXVECTOR4> diffuse;
			ARRAY<D3DXVECTOR4> ambient;
			ARRAY<D3DXVECTOR4> specular;

			int light_num = 0;
			for(int i = 0; i < (int)p_mp->point_light_id_list.size(); i++) {
				// �ݒ�\�����z�����烋�[�v�𔲂���
				if(i >= C_d3d_light_manager::OBJ_POINT_LIGHT_MAX)
					break;
				C_d3d_point_light* p_light = dynamic_cast<C_d3d_point_light*>(G_d3d_light_manager.search(p_mp->point_light_id_list[i]).get());

				// ���C�g�̃f�[�^��ǉ����Ă���
				w_pos.push_back(p_light->get_light_world_pos());
				l_pos.push_back(p_light->get_light_local_pos(p_frame->body()->CombinedTransformationMatrix,  *m_p_renderer->get_view_matrix()));
				atten012_range.push_back(p_light->get_light_attenuations_range());

				diffuse.push_back(p_light->get_diffuse());
				ambient.push_back(p_light->get_ambient());
				specular.push_back(p_light->get_specular());

				// ���C�g�̐����J�E���g
				light_num++;
			}


			if(light_num > 0) {
				// ���ۂ̃��C�g�̐�
				hr = p_effect->SetInt("g_PLightNum", light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetInt"), hr);
					return false;
				}				
				// ���[���h�ł̍��W
				hr = p_effect->SetVectorArray("g_PLightWorldPos", w_pos.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// ���[�J���ł̍��W
				hr = p_effect->SetVectorArray("g_PLightLocalPos", l_pos.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// ���������p�����[�^
				hr = p_effect->SetVectorArray("g_PLightAttenuationsRange", atten012_range.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}

				// �f�B�t���[�Y
				hr = p_effect->SetVectorArray("g_PLightDiffuse", diffuse.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// �A���r�G���g
				hr = p_effect->SetVectorArray("g_PLightAmbient", ambient.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// �X�y�L����
				hr = p_effect->SetVectorArray("g_PLightSpecular", specular.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
			}
		}



		// �X�|�b�g
		{
			ARRAY<D3DXVECTOR4> w_pos;
			ARRAY<D3DXVECTOR4> l_pos;
			ARRAY<D3DXVECTOR4> w_dir;
			ARRAY<D3DXVECTOR4> l_dir;
			ARRAY<D3DXVECTOR4> atten012_range;
			ARRAY<D3DXVECTOR4> cos_theta_phi_falloff;
			ARRAY<D3DXVECTOR4> diffuse;
			ARRAY<D3DXVECTOR4> ambient;
			ARRAY<D3DXVECTOR4> specular;

			int light_num = 0;
			for(int i = 0; i < (int)p_mp->spot_light_id_list.size(); i++) {
				// �ݒ�\�����z�����烋�[�v�𔲂���
				if(i >= C_d3d_light_manager::OBJ_SPOT_LIGHT_MAX)
					break;
				C_d3d_spot_light* p_light = dynamic_cast<C_d3d_spot_light*>(G_d3d_light_manager.search(p_mp->spot_light_id_list[i]).get());

				// ���C�g�̃f�[�^��ǉ����Ă���
				w_pos.push_back(p_light->get_light_world_pos());
				l_pos.push_back(p_light->get_light_local_pos(p_frame->body()->CombinedTransformationMatrix,  *m_p_renderer->get_view_matrix()));
				w_dir.push_back(p_light->get_light_world_dir());
				l_dir.push_back(p_light->get_light_local_dir(p_frame->body()->CombinedTransformationMatrix));
				atten012_range.push_back(p_light->get_light_attenuations_range());
				cos_theta_phi_falloff.push_back(p_light->get_light_cos_theta_phi_falloff());	// cos(theta/2),cos(phi/2)���v�Z���ēn��

				diffuse.push_back(p_light->get_diffuse());
				ambient.push_back(p_light->get_ambient());
				specular.push_back(p_light->get_specular());

				// ���C�g�̐����J�E���g
				light_num++;
			}

			if(light_num > 0) {
				// ���ۂ̃��C�g�̐�
				hr = p_effect->SetInt("g_SLightNum", light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetInt"), hr);
					return false;
				}
				// ���[���h�ł̍��W
				hr = p_effect->SetVectorArray("g_SLightWorldPos", w_pos.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// ���[�J���ł̍��W
				hr = p_effect->SetVectorArray("g_SLightLocalPos", l_pos.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// ���[���h�ł̕����x�N�g��
				hr = p_effect->SetVectorArray("g_SLightWorldDir", w_dir.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// ���[�J���ł̕����x�N�g��
				hr = p_effect->SetVectorArray("g_SLightLocalDir", l_dir.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// ���������p�����[�^
				hr = p_effect->SetVectorArray("g_SLightAttenuationsRange", atten012_range.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// �X�|�b�g���C�g�p�����[�^(theta��phi�͗\�ߊ��������̂�n��)
				hr = p_effect->SetVectorArray("g_SLightThetaPhiFalloff", cos_theta_phi_falloff.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}

				// �f�B�t���[�Y
				hr = p_effect->SetVectorArray("g_SLightDiffuse", diffuse.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// �A���r�G���g
				hr = p_effect->SetVectorArray("g_SLightAmbient", ambient.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
				// �X�y�L����
				hr = p_effect->SetVectorArray("g_SLightSpecular", specular.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVectorArray"), hr);
					return false;
				}
			}
		}




		*/



		// �]���̃��C�e�B���O
		ARRAY< BSP<C_d3d_light_interface> >::iterator it = m_p_renderer->get_light_list()->begin();
		for(; it != m_p_renderer->get_light_list()->end(); ++it) {
			// ��ނ̔���
			LIGHT_TYPE light_type = it->get()->get_type();

			if(false);
			// �f�B���N�V���i�����C�g(���s����)�̏ꍇ
			else if(light_type == LIGHT_TYPE_DIRECTIONAL) {
				C_d3d_directional_light* light = (C_d3d_directional_light*)(it->get());

				// ���[���h�ł̕����x�N�g��
				hr = p_effect->SetVector("g_LightWorldDir", &light->get_light_world_dir());
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
					return false;
				}
				// ���[�J���ł̕����x�N�g��
				hr = p_effect->SetVector("g_LightLocalDir", &light->get_light_local_dir(p_frame->body()->CombinedTransformationMatrix));
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
					return false;
				}
			}
			// �|�C���g���C�g(�_����)�̏ꍇ
			else if(light_type == LIGHT_TYPE_POINT) {
			}
			// �X�|�b�g���C�g�̏ꍇ
			else if(light_type == LIGHT_TYPE_SPOT) {
			}
		}
	}



	// �e�N�X�`���̐ݒ�
	if(p_mp->use_mesh_tex && mtrl->texture) {
		hr = p_effect->SetTexture("meshtexture", mtrl->texture->body());
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̃e�N�X�`���̐ݒ�Ɏ��s���܂����B"), _T("SetTexture"), hr);
			return false;
		}
	}

	// �}�e���A���̐ݒ�
	{
		D3DXVECTOR4 v;

		// �f�B�t���[�Y�̐ݒ�
		v = D3DXVECTOR4(mtrl->mtrl.Diffuse.r, mtrl->mtrl.Diffuse.g, mtrl->mtrl.Diffuse.b, mtrl->mtrl.Diffuse.a);
		hr = p_effect->SetVector("g_MaterialDiffuse", &v);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
			return false;
		}
		// �A���r�G���g�̐ݒ�
		v = D3DXVECTOR4(mtrl->mtrl.Ambient.r, mtrl->mtrl.Ambient.g, mtrl->mtrl.Ambient.b, mtrl->mtrl.Ambient.a);
		hr = p_effect->SetVector("g_MaterialAmbient", &v);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
			return false;
		}
		// �X�y�L�����̐ݒ�
		v = D3DXVECTOR4(mtrl->mtrl.Specular.r, mtrl->mtrl.Specular.g, mtrl->mtrl.Specular.b, mtrl->mtrl.Specular.a);
		hr = p_effect->SetVector("g_MaterialSpecular", &v);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
			return false;
		}
		// �G�~�b�V�u�̐ݒ�
		v = D3DXVECTOR4(mtrl->mtrl.Emissive.r, mtrl->mtrl.Emissive.g, mtrl->mtrl.Emissive.b, mtrl->mtrl.Emissive.a);
		hr = p_effect->SetVector("g_MaterialEmissive", &v);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
			return false;
		}
		// �p���[�̐ݒ�(�X�y�L����)
		hr = p_effect->SetFloat("g_MaterialSpecularPower", mtrl->mtrl.Power);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloat"), hr);
			return false;
		}

	}

	// Siglus�p�̃J���[�ݒ�
	{
		D3DXVECTOR4 v;

		// ���_�J���[�̏�Z���̐ݒ�
		if(p_mp->use_mul_vertex_color) {
			hr = p_effect->SetFloat("g_VertexColorRate", p_mp->mul_vertex_color_rate);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloat"), hr);
				return false;
			}			
		}
		// MRBD
		if(p_mp->use_mrbd) {
			hr = p_effect->SetVector("g_C0", &p_mp->mrbd);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
				return false;
			}
		}
		// RGBRate, AddRGB
		if(p_mp->use_rgb) {
			hr = p_effect->SetVector("g_C1", &p_mp->rgb_rate);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
				return false;
			}
			hr = p_effect->SetVector("g_C2", &p_mp->add_rgb);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
				return false;
			}
		}
	}

	// �V���h�E�}�b�v�֘A
	{
		if(p_mp->shading_type == SHADING_TYPE_DEPTH_BUFFER) {
			// �V���h�E�}�b�v�̐ݒ�
			if(m_p_renderer->get_shadow_map_tex()) {
				hr = p_effect->SetTexture("shadowtexture", m_p_renderer->get_shadow_map_tex());
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̃e�N�X�`���̐ݒ�Ɏ��s���܂����B"), _T("SetTexture"), hr);
					return false;
				}
			}

			// �[�x�o�b�t�@�V���h�E�̃o�C�A�X�l�̐ݒ�
			hr = p_effect->SetFloat("g_DBSBias", p_mp->depth_buffer_shadow_bias);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloat"), hr);
				return false;
			}			
			// ���C�g���猩���ˉe��Ԃւ̕ϊ��s��
			D3DXMATRIX light_view_proj = (*m_p_renderer->get_light_view_matrix()) * (*m_p_renderer->get_light_proj_matrix());
			hr = p_effect->SetMatrix("mLightVP", &light_view_proj);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrix"), hr);
				return false;
			}
			// �o�C�A�X�s��
			hr = p_effect->SetMatrix("mScaleBias", m_p_renderer->get_scale_bias_matrix());
			if(FAILED(hr)) {
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrix"), hr);
				return false;
			}
		}
	}

	// �g�D�[���֘A
	{
		if(p_mp->lighting_type == LIGHTING_TYPE_TOON) {
			// �g�D�[���e�N�X�`���̐ݒ�
			if(p_mp->toon_tex && p_mp->toon_tex->body()) {
				hr = p_effect->SetTexture("toontexture", p_mp->toon_tex->body());
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̃e�N�X�`���̐ݒ�Ɏ��s���܂����B"), _T("SetTexture"), hr);
					return false;
				}
			}
		}
	}

	// �o���v�A�����֘A
	{
		if(p_mp->lighting_type == LIGHTING_TYPE_BUMP || p_mp->lighting_type == LIGHTING_TYPE_PARALLAX) {
			// �@���}�b�v�̐ݒ�
			if(p_mp->normal_tex && p_mp->normal_tex->body()) {
				hr = p_effect->SetTexture("normaltexture", p_mp->normal_tex->body());
				if(FAILED(hr)) {
					set_last_error_d3d(_T("�G�t�F�N�g�̃e�N�X�`���̐ݒ�Ɏ��s���܂����B"), _T("SetTexture"), hr);
					return false;
				}
			}
		}
	}

	// �������C�g�֘A
	{
		if(p_mp->shader_option & SHADER_OPTION_RIM_LIGHT) {
			// �G�~�b�V�u�̐ݒ�
			hr = p_effect->SetVector("g_RimLightColor", &p_mp->rim_light_color);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetVector"), hr);
				return false;
			}
			// �p���[�̐ݒ�(�X�y�L����)
			hr = p_effect->SetFloat("g_RimLightPower", p_mp->rim_light_power);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetFloat"), hr);
				return false;
			}
		}
	}



	//if(g_UseGauss)	p_effect->SetTexture("shadowtexture", g_pXYMap);
	//else			p_effect->SetTexture("shadowtexture", g_pShadowTex);

	//p_effect->SetMatrix("mLightVP", &g_MatLightVP);
	//p_effect->SetMatrix("mLightWVP", &(g_MatLightVP * mScaleBias));
	//p_effect->SetMatrix("mScaleBias", &mScaleBias);	

	//D3DXVECTOR4 cp;
	//cp.x = g_Camera.m_Eye.x;
	//cp.y = g_Camera.m_Eye.y;
	//cp.z = g_Camera.m_Eye.z;
	//cp.w = 1.0f;
	//g_pEffect->SetVector("g_CameraLocalPos", &CalcCameraLocalPos(frame->CombinedTransformationMatrix, g_MatView));
	//p_effect->SetVector("g_CameraLocalPos", &cp);
	//p_effect->SetVector("g_CameraWorldPos", &cp);
	
	//D3DXVECTOR4 ld;
	//ld.x = g_Light.Direction.x;
	//ld.y = g_Light.Direction.y;
	//ld.z = g_Light.Direction.z;
	//ld.w = 1.0f;
	//g_pEffect->SetVector("g_LightLocalDir", &D3DXVECTOR4(0,0,1,1));
	//g_pEffect->SetVector("g_LightLocalDir", &CalcLightLocalDir(frame->CombinedTransformationMatrix, ld));
	//p_effect->SetVector("g_LightLocalDir", &ld);
	//p_effect->SetVector("g_LightWorldDir", &ld);

	//EffectSetMaterial(p_effect, (D3DMATERIAL9*)&meshContainer->pMaterials[pBoneCombination[i].AttribId]);


	return true;
}

// �X�L������Ȃ����b�V���̏ꍇ
bool C_d3d_render_interface::render_sprite_frame_per_mesh_set_effect_constant_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect)
{
	HRESULT hr;

	// �g���₷���悤�Ɉꎞ�ϐ��ɑ��
	BSP<C_d3d_mesh>					mesh = p_frame->m_mesh_list[mesh_no];
	S_d3d_skinned_mesh_container*	p_mc = (S_d3d_skinned_mesh_container *)p_frame->body()->pMeshContainer;
	C_d3d_mtrl*						mtrl = mesh->get_mtrl(mtrl_no);
	S_d3d_mesh_param*				p_mp = mesh->get_mesh_param_ptr();
	DWORD							fvf = mesh->body()->GetFVF();

	// ���[���h�ϊ��s��z��̐ݒ�
	hr = p_effect->SetMatrixArray("mWorldMatrixArray", &p_frame->body()->CombinedTransformationMatrix, 1);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrixArray"), hr);
		return false;
	}

	// ���[���h�ϊ��s��̐ݒ�
	hr = p_effect->SetMatrix("mWorld", &p_frame->body()->CombinedTransformationMatrix);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrix"), hr);
		return false;
	}
	// ���[�J���ϊ��s��̐ݒ�(�A�j���[�V������̃��[�J�����W�n�ɕϊ�����s��)
	// TransformationMatrix����Ȃ���IDENTITY_MATRIX�����H/*�v�C��*/
	hr = p_effect->SetMatrix("mLocal", &p_frame->body()->TransformationMatrix);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrix"), hr);
		return false;
	}

	// �{�[�����̐ݒ�
	hr = p_effect->SetInt("CurNumBones", 1);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetInt"), hr);
		return false;
	}

	return true;
}
// �X�L�����b�V���̏ꍇ
bool C_d3d_render_interface::render_sprite_frame_per_mesh_set_effect_constant_skinned_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect)
{
	HRESULT hr;

	// �g���₷���悤�Ɉꎞ�ϐ��ɑ��
	BSP<C_d3d_mesh>					mesh = p_frame->m_mesh_list[mesh_no];
	S_d3d_skinned_mesh_container*	p_mc = (S_d3d_skinned_mesh_container *)p_frame->body()->pMeshContainer;
	C_d3d_mtrl*						mtrl = mesh->get_mtrl(mtrl_no);
	S_d3d_mesh_param*				p_mp = mesh->get_mesh_param_ptr();
	DWORD							fvf = mesh->body()->GetFVF();

	// ���[���h�ϊ��s��z��̐ݒ�
	hr = p_effect->SetMatrixArray("mWorldMatrixArray", p_mc->SkinnedBlendMatrices.get(), p_mc->NumPaletteEntries);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrixArray"), hr);
		return false;
	}

	// ���[���h�ϊ��s��̐ݒ�
	hr = p_effect->SetMatrix("mWorld", &p_frame->body()->CombinedTransformationMatrix);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrix"), hr);
		return false;
	}
	// ���[�J���ϊ��s��̐ݒ�(�A�j���[�V������̃��[�J�����W�n�ɕϊ�����s��)
	hr = p_effect->SetMatrix("mLocal", &p_frame->body()->TransformationMatrix);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrix"), hr);
		return false;
	}

	// �{�[�����̐ݒ�
	hr = p_effect->SetInt("CurNumBones", p_mc->NumInfl);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetInt"), hr);
		return false;
	}

	return true;
}

// �G�t�F�N�g�萔�̐ݒ�
bool C_d3d_render_interface::render_sprite_frame_per_mesh_set_effect_constant(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect, ID3DXSkinInfo* p_skin_info)
{
	// �X�L�����b�V���A��X�L�����b�V�����ʂ̃G�t�F�N�g�萔�ݒ�
	if (!render_sprite_frame_per_mesh_set_effect_constant_common(p_sprite, p_rp, p_frame, mesh_no, mtrl_no, subset_no, p_effect))
		return false;

	// �G�t�F�N�g���J�n����(�萔�̐ݒ肵����ɃG�t�F�N�g���J�n)
	if (!(p_skin_info
			? render_sprite_frame_per_mesh_set_effect_constant_skinned_mesh(p_sprite, p_rp, p_frame, mesh_no, mtrl_no, subset_no, p_effect)
			: render_sprite_frame_per_mesh_set_effect_constant_mesh(p_sprite, p_rp, p_frame, mesh_no, mtrl_no, subset_no, p_effect)
			))
		return false;

	return true;
}

// ****************************************************************
// �X�v���C�g�F�t���[���������_�[
// ================================================================
bool C_d3d_render_interface::render_sprite_frame(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_frame* frame)
{
	LPSTR fname = frame->body()->Name;

	// ���b�V��������΃��b�V����`��I(����pNextMeshContainer���������Ȃ���size�͏��1�H)
	for (int mesh_no = 0; mesh_no < (int)frame->m_mesh_list.size(); mesh_no ++)	{
		BSP<C_d3d_mesh> mesh = frame->m_mesh_list[mesh_no];
		S_d3d_skinned_mesh_container* p_mc = (S_d3d_skinned_mesh_container *)frame->body()->pMeshContainer;

		// ���̃��b�V���̕`��������`�F�b�N
		if (render_sprite_frame_per_mesh_draw_condition(mesh.get())) {

			// �X�L�����Ȃ��ꍇ
			if (!p_mc->pSkinInfo)	{

				for (int m = 0; m < (int)mesh->get_mtrl_cnt(); m++)	{

					// �}�e���A����`��
					if (!render_sprite_frame_mesh_subset(p_sprite, rp, frame, mesh_no, m, m))
						return false;
				}
			}

			// �X�L��������ꍇ
			else	{
				ID3DXSkinInfo* skin = p_mc->pSkinInfo;
				D3DXBONECOMBINATION* p_bone_conbination = (D3DXBONECOMBINATION *)p_mc->pBoneCombinationBuf->GetBufferPointer();
				UINT matrix_index;

				for (int i = 0; i < (int)p_mc->NumAttributeGroups; i++)	{

					// C_d3d_renderer::update()�Ɉڍs�\�肾�������ǂ��܂������Ȃ������̂ŕۗ�
					// �ϊ��p�s��z��̎Z�o
					for(int j = 0; j < (int)p_mc->NumPaletteEntries; j++) {
						matrix_index = p_bone_conbination[i].BoneId[j];
						if(matrix_index != UINT_MAX) {
							D3DXMatrixMultiply(&p_mc->SkinnedBlendMatrices[j],
												&frame->m_offset_matrix_list[matrix_index],
												frame->m_root_matrix_list[matrix_index]);
//							G_debug_text.printf(_T("*****%s*****\n"), MBSTR_to_TSTR(frame->body()->Name).c_str());
//							draw_matrix_data(_T("SBM"), &p_mc->SkinnedBlendMatrices[j]);
//							draw_matrix_data(_T("OFFSET"), &frame->m_offset_matrix_list[matrix_index]);
//							draw_matrix_data(_T("ROOT"), frame->m_root_matrix_list[matrix_index]);
						}
					}

					/*
					DWORD blend_cnt = 0;
					// ���_�u�����f�B���O�̉񐔂����߂�
					for (int j = 0; j < (int)p_mc->NumInfl; j++)	{
						if (p_bone_conbination[i].BoneId[j] != UINT_MAX)
							blend_cnt = j;	// �Ȃ��� "="�B�T���v���������B���[��B
					}

					// ���_�u�����f�B���O�̉񐔂��X�y�b�N���I�[�o�[���Ă��Ȃ����ǂ������ׂ�
					if (G_d3d.device.device_caps().MaxVertexBlendMatrices <= blend_cnt)	{
						set_last_error(_T("MaxVertexBlendMatrices ������܂���B�X�L�����b�V����`��ł��܂���ł����B"), _T("render_sprite_frame"));
						return false;
					}

					// ���_�u�����f�B���O�̉񐔂�ݒ肷��
					G_d3d.state.set_render_state(D3DRS_VERTEXBLEND, blend_cnt);

					// ���_�u�����f�B���O�̍s��̋��߂Đݒ肷��
					for (int j = 0; j < (int)p_mc->NumInfl; j++)	{
						DWORD matrix_index = p_bone_conbination[i].BoneId[j];
						if (matrix_index != UINT_MAX)	{
							D3DXMATRIX mat_temp;
							D3DXMatrixMultiply(&mat_temp, &frame->m_offset_matrix_list[matrix_index], frame->m_root_matrix_list[matrix_index]);
							G_d3d.device->SetTransform(D3DTS_WORLDMATRIX(j), &mat_temp);
						}
					}
					*/

					// ���̑����O���[�v�Ŏg���}�e���A�����擾
					int mtrl_no = p_bone_conbination[i].AttribId;

					// �}�e���A����`��
					//if (!render_sprite_mesh_mtrl(p_sprite, rp, mesh.get(), mtrl_no, p_mc))
					if (!render_sprite_frame_mesh_subset(p_sprite, rp, frame, mesh_no, mtrl_no, i))
						return false;
				}
			}
		}
	}
	// �Z��t���[����`��
	if (frame->m_sibling)	{
		if (!render_sprite_frame(p_sprite, rp, frame->m_sibling.get()))
			return false;
	}
	// �q�t���[����`��
	if (frame->m_first_child)	{
		if (!render_sprite_frame(p_sprite, rp, frame->m_first_child.get()))
			return false;
	}



	return true;
}

// �t���[���`�掞�̃��b�V�����̕`�����(�f�t�H���g�͏��true)
bool C_d3d_render_interface::render_sprite_frame_per_mesh_draw_condition(C_d3d_mesh* p_mesh)
{
	return true;
}

// ���b�V�����̃G�t�F�N�g����
ID3DXEffect* C_d3d_render_interface::render_sprite_frame_per_mesh_create_effect(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info)
{
	return create_effect_for_mesh_from_mesh(p_mesh, p_mtrl, p_skin_info);
}








#if 0
static float get_val_from_matrix_index(int i, D3DXMATRIX* matrix)
{
	float ret = 0.0f;

	if(false);
	else if(i == 0) ret = matrix->_11;
	else if(i == 1) ret = matrix->_12;
	else if(i == 2) ret = matrix->_13;
	else if(i == 3) ret = matrix->_14;
	else if(i == 4) ret = matrix->_21;
	else if(i == 5) ret = matrix->_22;
	else if(i == 6) ret = matrix->_23;
	else if(i == 7) ret = matrix->_24;
	else if(i == 8) ret = matrix->_31;
	else if(i == 9) ret = matrix->_32;
	else if(i == 10) ret = matrix->_33;
	else if(i == 11) ret = matrix->_34;
	else if(i == 12) ret = matrix->_41;
	else if(i == 13) ret = matrix->_42;
	else if(i == 14) ret = matrix->_43;
	else if(i == 15) ret = matrix->_44;

	return ret;
}
static void draw_matrix_data(CTSTR& matrix_name, D3DXMATRIX* matrix)
{
	G_debug_text.printf(_T("%s\n"), matrix_name.c_str());
	for(int i = 0; i < 16; i++) {
		G_debug_text.printf(_T("%.2f\t"), get_val_from_matrix_index(i, matrix));
		if((i + 1) % 4 == 0) {
			G_debug_text.printf(_T("\n"));

		}
	}
}
#endif








}