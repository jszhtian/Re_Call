#include	"tonad3d3_pch.h"
#include	"tonad3d3_state.h"
#include	"tonad3d3_renderer.h"


namespace NT3
{


// ****************************************************************
// D3D �r���[������
// ================================================================
bool C_d3d_renderer::init()
{
	// �s���������
	D3DXMatrixIdentity(&m_mat_view);
	D3DXMatrixIdentity(&m_mat_proj);

	m_shadow_map_tex = NULL;
	m_shadow_map_surface = NULL;
	m_shadow_map_depth_stencil_buffer = NULL;

	m_default_depth_stencil_buffer = NULL;
	m_default_back_buffer = NULL;

	m_light_bloom_tex = NULL;
	m_light_bloom_surface = NULL;

	m_fog_near = 0.0f;
	m_fog_far = 0.0f;

	m_render_default.initialize(this);
	m_pre_render_shadow_map.initialize(this);

	return true;
}

// ****************************************************************
// �o�b�N�o�b�t�@���擾
// ================================================================
C_d3d_surface C_d3d_renderer::get_back_buffer()
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("�f�o�C�X����������Ă��܂���B"), _T("get_back_buffer"));
		return false;
	}

	// �o�b�N�o�b�t�@�̃T�[�t�F�X���擾����
	IDirect3DSurface9* p_back_buffer = NULL;
	hr = G_d3d.device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &p_back_buffer);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("GetBackBuffer"), hr);
		return C_d3d_surface();
	}

	// GetBackBuffer �͓����ŎQ�ƃJ�E���^���グ�Ă��܂��BBIP �ŊǗ����Ȃ��ƃ��[�N���Ă��܂��܂��B
	return BIP<IDirect3DSurface9>(p_back_buffer, false);
}

// ****************************************************************
// �����_�[�^�[�Q�b�g��ݒ�
// ================================================================
bool C_d3d_renderer::set_render_target(IDirect3DSurface9* surface)
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("�f�o�C�X����������Ă��܂���B"), _T("set_render_target"));
		return false;
	}

	// �����_�[�^�[�Q�b�g��ݒ�
	hr = G_d3d.device->SetRenderTarget(0, surface);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("SetRenderTarget"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// �[�x�X�e���V���o�b�t�@��ݒ�
// ================================================================
bool C_d3d_renderer::set_dpst_buffer(IDirect3DSurface9* surface)
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("�f�o�C�X����������Ă��܂���B"), _T("set_dpst_buffer"));
		return false;
	}

	// �[�x���X�e���V���o�b�t�@��ݒ�
	hr = G_d3d.device->SetDepthStencilSurface(surface);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("SetDepthStencilSurface"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// �[�x�X�e���V���o�b�t�@���擾
// ================================================================
C_d3d_surface C_d3d_renderer::get_dpst_buffer()
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("�f�o�C�X����������Ă��܂���B"), _T("get_dpst_buffer"));
		return false;
	}

	// �[�x�X�e���V���o�b�t�@�̃T�[�t�F�X���擾���Ă���
	IDirect3DSurface9* p_dpst_buffer = NULL;
	hr = G_d3d.device->GetDepthStencilSurface(&p_dpst_buffer);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("GetDepthStencilSurface"), hr);
		return C_d3d_surface();
	}

	// GetDepthStencilSurface �͓����ŎQ�ƃJ�E���^���グ�Ă��܂��BBIP �ŊǗ����Ȃ��ƃ��[�N���Ă��܂��܂��B
	return BIP<IDirect3DSurface9>(p_dpst_buffer, false);
}

// ****************************************************************
// �����_�[�^�[�Q�b�g���N���A
// ================================================================
bool C_d3d_renderer::clear_render_target(D3DCOLOR back_color)
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
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

// �o�b�t�@�̂�
bool C_d3d_renderer::clear_render_target_buffer(D3DCOLOR back_color)
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("�f�o�C�X����������Ă��܂���B"), _T("clear_render_target_buffer"));
		return false;
	}

	// �o�b�N�o�b�t�@���N���A
	DWORD clear_target = D3DCLEAR_TARGET;
	hr = G_d3d.device->Clear(0, NULL, clear_target, back_color, 1.0f, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Clear"), hr);
		return false;
	}

	return true;
}

// �y�o�b�t�@�̂�
bool C_d3d_renderer::clear_render_target_z_buffer()
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("�f�o�C�X����������Ă��܂���B"), _T("clear_render_target_z_buffer"));
		return false;
	}

	// �o�b�N�o�b�t�@���N���A
	DWORD clear_target = D3DCLEAR_ZBUFFER;
	hr = G_d3d.device->Clear(0, NULL, clear_target, 0, 1.0f, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Clear"), hr);
		return false;
	}

	return true;
}

// �X�e���V���o�b�t�@�̂�
bool C_d3d_renderer::clear_render_target_stencil()
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("�f�o�C�X����������Ă��܂���B"), _T("clear_render_target_stencil"));
		return false;
	}

	// �o�b�N�o�b�t�@���N���A
	DWORD clear_target = D3DCLEAR_STENCIL;
	hr = G_d3d.device->Clear(0, NULL, clear_target, 0, 1.0f, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Clear"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// �A�b�v�f�[�g
// ================================================================
bool C_d3d_renderer::update()
{
	ARRAY<C_d3d_sprite *>::iterator it = sprite_list.begin();
	for(; it != sprite_list.end(); ++it) {
		C_d3d_sprite* p_sprite = *it;

		// �u�^�C�v�F�t���[���Z�b�g�v�̕`��
		if (p_sprite->type == E_d3d_sprite_type_frame_set && p_sprite->get_frame_set())	{
			BSP<C_d3d_frame_set> frame_set = p_sprite->get_frame_set();

			// ���[���h�ϊ��s����v�Z
			D3DXMATRIX mat_world, mat_temp;

			// ���[���h�ϊ��s���ݒ�
			float yaw = p_sprite->rp.rotate.y;
			float pitch = p_sprite->rp.rotate.x;
			float roll = p_sprite->rp.rotate.z;

			// ���[���h���W�ϊ��s����쐬(S-R-T)
			D3DXMatrixIdentity(&mat_world);
			D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixScaling(&mat_temp, p_sprite->rp.scale.x, p_sprite->rp.scale.y, p_sprite->rp.scale.z));		// �g�k
			D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixRotationQuaternion(&mat_temp, &p_sprite->rp.quaternion));									// ��]
			//D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixRotationYawPitchRoll(&mat_temp, yaw, pitch, roll));										// ��]
			D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixTranslation(&mat_temp, p_sprite->rp.pos.x, p_sprite->rp.pos.y, p_sprite->rp.pos.z));		// �ړ�

			//// �i���_�V�F�[�_���g��Ȃ��ꍇ�j���[���h�ϊ��s���ݒ�
			//Gd3d.device->SetTransform(D3DTS_WORLD, &mat_world);

			// �A�j���[�V������i�߂�(D3DXFRAME::TransformationMatrix ���L�[�t���[���ɍ��킹�čX�V���Ă���)
			frame_set->update_animation_controller();

			// ���[���h�ϊ��s������Ƀt���[���ϊ��s��𐶐�����
			// mat_world �� D3DXFRAME::TransformationMatrix ���� �����s�� S_d3d_frame::CombinedTransformationMatrix ���Z�o
			frame_set->update_matrix(&mat_world);

			// �t���[���Z�b�g�̃A�b�v�f�[�g(���܂������Ȃ������̂ŕۗ�)
			//if(!update_frame_set(p_sprite, &p_sprite->rp, frame_set->m_frame_root.get()))
			//	return false;
		}
	}

	return true;
}
// ****************************************************************
// �t���[���Z�b�g�̃A�b�v�f�[�g
// ================================================================
bool C_d3d_renderer::update_frame_set(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_frame* frame)
{
	// ���b�V��������΃��b�V����`��I(����pNextMeshContainer���������Ȃ���size�͏��1�H)
	for (int mesh_no = 0; mesh_no < (int)frame->m_mesh_list.size(); mesh_no ++)	{
		BSP<C_d3d_mesh> mesh = frame->m_mesh_list[mesh_no];
		S_d3d_skinned_mesh_container* p_mc = (S_d3d_skinned_mesh_container *)frame->body()->pMeshContainer;

		// �X�L�����Ȃ��ꍇ
		if (!p_mc->pSkinInfo)	{
			// �������Ȃ�
		}
		// �X�L��������ꍇ
		else	{
			ID3DXSkinInfo* skin = p_mc->pSkinInfo;
			D3DXBONECOMBINATION* p_bone_conbination = (D3DXBONECOMBINATION *)p_mc->pBoneCombinationBuf->GetBufferPointer();
			UINT matrix_index;

			for (int i = 0; i < (int)p_mc->NumAttributeGroups; i++)	{

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
				// ���͓��ɉ������Ȃ�
			}
		}

	}
	// �Z��t���[����`��
	if (frame->m_sibling)	{
		if (!update_frame_set(p_sprite, rp, frame->m_sibling.get()))
			return false;
	}
	// �q�t���[����`��
	if (frame->m_first_child)	{
		if (!update_frame_set(p_sprite, rp, frame->m_first_child.get()))
			return false;
	}

	return true;
}

// ****************************************************************
// �����_�[
// ================================================================
bool C_d3d_renderer::render()
{
	HRESULT hr;

	if (sprite_list.empty())
		return true;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("Direct3D �f�o�C�X����������Ă��܂���B"), _T("render"));
		return false;
	}

	bool ret_value = false;

	// �����_�����O�J�n
	hr = G_d3d.device->BeginScene();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("BeginScene"), hr);
	}
	else	{

		//// �u���C�g�p�X���g�p����ꍇ(��ɋP�x�`��)
		//if(use_bright_path) {
		//	DWORD RTs = G_d3d.device.device_caps().NumSimultaneousRTs;	// �����_�[�^�[�Q�b�g�̍ő吔
		//	// �}���`�����_�[�^�[�Q�b�g���g���Ȃ��ꍇ
		//	if(RTs <= 1) {
		//		// �J���[���A�P�x�������Ԃɕ`��
		//		// �J���[���̕`��
		//		// �����_�[�^�[�Q�b�g�̕ύX
		//		// �P�x���̕ύX
		//		// �����_�[�^�[�Q�b�g�̕ύX�H
		//	}
		//	// �}���`�����_�[�^�[�Q�b�g���g����ꍇ
		//	else {
		//		// �J���[����RT0�A�P�x����RT1�ɓ����`��
		//		// �����_�[�^�[�Q�b�g�̐ݒ�
		//		/// �J���[���A�P�x���̕`��
		//	}
		//}

		// �X�v���C�g���X�g�������_�[
		if (m_render_default.render_sprite_list(sprite_list))	{

			// ���������I
			ret_value = true;
		}

		// �����_�����O�I��
		G_d3d.device->EndScene();
	}

	return ret_value;
}

// ****************************************************************
// �v�������_�[
// ================================================================
bool C_d3d_renderer::pre_render()
{
	HRESULT hr;

	if (sprite_list.empty())
		return true;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("Direct3D �f�o�C�X����������Ă��܂���B"), _T("render"));
		return false;
	}

	bool ret_value = false;

	// �����_�����O�J�n
	hr = G_d3d.device->BeginScene();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("BeginScene"), hr);
	}
	else	{
		// RT�ύX�����Ȃǂ𖳏����ōs���Ă���̂ŏC������

		// �����_�[�^�[�Q�b�g�Ȃǂ�ۑ�
		if(!G_d3d.device.get_render_target(0, &m_default_depth_stencil_buffer))
			return false;
		if(!G_d3d.device.get_depth_stencil_surface(&m_default_back_buffer))
			return false;
		if(!G_d3d.device.get_viewport(&m_default_viewport))
			return false;

		// �����_�[�^�[�Q�b�g�Ȃǂ��V���h�E�}�b�v�p�ɐ؂�ւ�
		if(!G_d3d.device.set_render_target(0, m_shadow_map_surface))
			return false;
		if(!G_d3d.device.set_depth_stencil_surface(m_shadow_map_depth_stencil_buffer))
			return false;
		if(!G_d3d.device.set_viewport(&m_shadow_map_viewport))
			return false;
		// �V���h�E�}�b�v�̃N���A
		hr = G_d3d.device.body()->Clear(
						0L,
						NULL,
						D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						0xFFFFFFFF,
						1.0f,
						0L
						);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("Clear"), hr);
			return false;
		}

		// �V���h�E�}�b�v�̃v�������_
		if (m_pre_render_shadow_map.render_sprite_list(sprite_list))	{
			// ���������I
			ret_value = true;
		}

		// ���̏��͂��̎��_�Ń����_�[�^�[�Q�b�g�Ȃǂ����ɖ߂��Ă���
		if(!G_d3d.device.set_render_target(0, m_default_depth_stencil_buffer))
			return false;
		if(!G_d3d.device.set_depth_stencil_surface(m_default_back_buffer))
			return false;
		if(!G_d3d.device.set_viewport(&m_default_viewport))
			return false;

		m_default_depth_stencil_buffer->Release();
		m_default_back_buffer->Release();

		// �V���h�E�}�b�v�̃N���A
		hr = G_d3d.device.body()->Clear(
						0L,
						NULL,
						D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						0x00404080,
						1.0f,
						0L
						);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("Clear"), hr);
			return false;
		}

		// �����_�����O�I��
		G_d3d.device->EndScene();
	}

	return ret_value;
}
// ****************************************************************
// �|�X�g�����_�[
// ================================================================
bool C_d3d_renderer::post_render()
{
	HRESULT hr;

	if (sprite_list.empty())
		return true;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("Direct3D �f�o�C�X����������Ă��܂���B"), _T("render"));
		return false;
	}

	bool ret_value = false;

	// �����_�����O�J�n
	hr = G_d3d.device->BeginScene();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("BeginScene"), hr);
	}
	else	{

		// �X�v���C�g���X�g�������_�[
		//if (render_sprite_list(sprite_list))	{

		//	// ���������I
		//	ret_value = true;
		//}

		// �����_�����O�I��
		G_d3d.device->EndScene();
	}

	return ret_value;
}


// ****************************************************************
// �f�o�b�O�p�V���h�E�}�b�v�̕`��
// ================================================================
bool C_d3d_renderer::debug_render_shadow_map(int render_size, int x, int y)
{
	if(!m_shadow_map_tex)
		return true;	// ���s�ł͂Ȃ��̂�true

	typedef struct {
		FLOAT       p[4];
		FLOAT       tu, tv;
	} TVERTEX;

	G_d3d.device->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
	G_d3d.device->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	G_d3d.device->SetTextureStageState(1,D3DTSS_COLOROP,    D3DTOP_DISABLE);

	TVERTEX Vertex[4] = {
		// x  y  z rhw tu tv
		{	(float)x,				(float)y,0, 1, 0, 0,},
		{	(float)x+render_size,	(float)y,0, 1, 1, 0,},
		{	(float)x+render_size,	(float)y+render_size,0, 1, 1, 1,},
		{	(float)x,				(float)y+render_size,0, 1, 0, 1,},
	};

	G_d3d.device->SetTexture( 0, m_shadow_map_tex );
	G_d3d.device->SetVertexShader(NULL);
	G_d3d.device->SetPixelShader(NULL);
	G_d3d.device->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	G_d3d.device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );
	G_d3d.device->SetTexture( 0, NULL );

	return true;
}

bool C_d3d_renderer::debug_render_light_bloom_tex(int render_size, int x, int y)
{
	if(!m_light_bloom_tex)
		return true;	// ���s�ł͂Ȃ��̂�true

	typedef struct {
		FLOAT       p[4];
		FLOAT       tu, tv;
	} TVERTEX;

	G_d3d.device->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
	G_d3d.device->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	G_d3d.device->SetTextureStageState(1,D3DTSS_COLOROP,    D3DTOP_DISABLE);

	TVERTEX Vertex[4] = {
		// x  y  z rhw tu tv
		{	(float)x,				(float)y,0, 1, 0, 0,},
		{	(float)x+render_size,	(float)y,0, 1, 1, 0,},
		{	(float)x+render_size,	(float)y+render_size,0, 1, 1, 1,},
		{	(float)x,				(float)y+render_size,0, 1, 0, 1,},
	};

	G_d3d.device->SetTexture( 0, m_light_bloom_tex );
	G_d3d.device->SetVertexShader(NULL);
	G_d3d.device->SetPixelShader(NULL);
	G_d3d.device->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	G_d3d.device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );
	G_d3d.device->SetTexture( 0, NULL );

	return true;
}

}

