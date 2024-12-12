#include "tonad3d3_pch.h"
#include "tonad3d3_pre_render.h"
#include "tonad3d3_renderer.h"

namespace NT3
{


bool C_d3d_pre_render_shadow_map::render_sprite_list(ARRAY<C_d3d_sprite *>& sprite_list)
{
	ARRAY<C_d3d_sprite *>::iterator it = sprite_list.begin();
	for(; it != sprite_list.end(); ++it) {
		C_d3d_sprite* p_sprite = *it;

		// �u�^�C�v�F�t���[���Z�b�g�v�̕`��
		if (p_sprite->type == E_d3d_sprite_type_frame_set && p_sprite->get_frame_set())	{
			BSP<C_d3d_frame_set> frame_set = p_sprite->get_frame_set();

			// ���ۂɃv�������_���s��
			// �S�Ẵt���[�������ɕ`�悷��
			if (!render_sprite_frame(p_sprite, &p_sprite->rp, frame_set->m_frame_root.get()))
				return false;
		}
	}

	return true;
}




bool C_d3d_pre_render_shadow_map::render_sprite_frame_per_mesh_set_effect_constant(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect, ID3DXSkinInfo* p_skin_info)
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

	// �X�L�����b�V���̏ꍇ
	if(p_mc->pSkinInfo) {
		// ���[���h�ϊ��s��z��̐ݒ�
		hr = p_effect->SetMatrixArray("mWorldMatrixArray", p_mc->SkinnedBlendMatrices.get(), p_mc->NumPaletteEntries);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrixArray"), hr);
			return false;
		}
		// �{�[�����̐ݒ�
		hr = p_effect->SetInt("CurNumBones", p_mc->NumInfl);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetInt"), hr);
			return false;
		}
	}
	// ��X�L�����b�V���̏ꍇ
	else {
		// ���[���h�ϊ��s��z��̐ݒ�
		hr = p_effect->SetMatrixArray("mWorldMatrixArray", &p_frame->body()->CombinedTransformationMatrix, 1);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrixArray"), hr);
			return false;
		}
		// �{�[�����̐ݒ�
		hr = p_effect->SetInt("CurNumBones", 1);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetInt"), hr);
			return false;
		}
	}

	// �V���h�E�}�b�v�֘A
	{
		// ���C�g���猩���ˉe��Ԃւ̕ϊ��s��
		D3DXMATRIX light_view_proj = (*m_p_renderer->get_light_view_matrix()) * (*m_p_renderer->get_light_proj_matrix());
		hr = p_effect->SetMatrix("mLightVP", &light_view_proj);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("�G�t�F�N�g�̒萔�̐ݒ�Ɏ��s���܂����B"), _T("SetMatrix"), hr);
			return false;
		}
	}


	return true;
}


// �t���[���`�掞�̃��b�V�����̕`�����(�v�������_�^�C�v���V���h�E�}�b�v�Ȃ�)
bool C_d3d_pre_render_shadow_map::render_sprite_frame_per_mesh_draw_condition(C_d3d_mesh* p_mesh)
{
	S_d3d_mesh_param* p_mp = p_mesh->get_mesh_param_ptr();

	if(p_mp->pre_render_type & S_d3d_mesh_param::SHADOW_MAP) {
		return true;
	}

	return false;
}

// ���b�V�����̃G�t�F�N�g����
ID3DXEffect* C_d3d_pre_render_shadow_map::render_sprite_frame_per_mesh_create_effect(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info)
{
	return create_effect_for_shadow_map_from_mesh(p_mesh, p_mtrl, p_skin_info);
}












}