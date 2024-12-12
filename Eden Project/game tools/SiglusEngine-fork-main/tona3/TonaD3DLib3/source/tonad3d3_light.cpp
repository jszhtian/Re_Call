#include "tonad3d3_pch.h"
#include "tonad3d3_light.h"
#include "tonad3d3_d3d.h"

namespace NT3
{


// ****************************************************************
// ���C�g�C���^�[�t�F�C�X
// ================================================================
C_d3d_light_interface::C_d3d_light_interface()
{
	m_type = LIGHT_TYPE_NONE;
	m_id = -1;

	// �e�탉�C�g���ʃJ���[
	m_diffuse = D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f);
	m_ambient = D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f);
	m_specular = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
}

C_d3d_light_interface::~C_d3d_light_interface()
{
}


// ****************************************************************
// �f�B���N�V���i�����C�g
// ================================================================
C_d3d_directional_light::C_d3d_directional_light()
{
	m_type = LIGHT_TYPE_DIRECTIONAL;
	m_id = -1;

	m_dir = ZERO_VECTOR;
}

C_d3d_directional_light::~C_d3d_directional_light()
{
}

D3DXVECTOR4 C_d3d_directional_light::get_light_local_dir(const D3DXMATRIX& world)
{
	// �l�����Ȃ̂̓V�F�[�_�p
	D3DXVECTOR4 vec;
	D3DXMATRIX inv_world;
	D3DXMatrixInverse(&inv_world, NULL, &world);
	D3DXVec3Transform(&vec, &m_dir, &inv_world);
	D3DXVec3Normalize((D3DXVECTOR3*)&vec, (D3DXVECTOR3*)&vec);

	return vec;
}
D3DXVECTOR4 C_d3d_directional_light::get_light_world_dir()
{
	// �l�����Ȃ̂̓V�F�[�_�p
	D3DXVECTOR4 vec = D3DXVECTOR4(m_dir.x, m_dir.y, m_dir.z, 1.0f);
	return vec;
}


// ****************************************************************
// �|�C���g���C�g
// ================================================================
C_d3d_point_light::C_d3d_point_light()
{
	m_type = LIGHT_TYPE_POINT;
	m_id = -1;

	m_pos = ZERO_VECTOR;
	// �����W�����S��0���Ɖ��̎���0���Z����������̂�0.001f�ɂ��Ă���
	// Atten = 1/( att0i + att1i * d + att2i * d2)
	m_attenuation0 = 0.001f;
	m_attenuation1 = 0.0f;
	m_attenuation2 = 0.0f;
	m_range = 100.0f;
}

C_d3d_point_light::~C_d3d_point_light()
{
}

D3DXVECTOR4 C_d3d_point_light::get_light_local_pos(const D3DXMATRIX& world, const D3DXMATRIX& view)
{
	// �l�����Ȃ̂̓V�F�[�_�p
	D3DXVECTOR4 vec;
	D3DXMATRIX mat;		// ���[�J�����W�n����r���[���W�n�ւ̕ϊ��s��̋t�s��

	mat = world * view;
	D3DXMatrixInverse(&mat, NULL, &mat);
	vec = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVec4Transform(&vec, &vec, &mat);
	
	return vec;
}

D3DXVECTOR4 C_d3d_point_light::get_light_world_pos()
{
	// �l�����Ȃ̂̓V�F�[�_�p
	D3DXVECTOR4 vec = D3DXVECTOR4(m_pos.x, m_pos.y, m_pos.z, 1.0f);
	return vec;
}

D3DXVECTOR4 C_d3d_point_light::get_light_attenuations_range()
{
	D3DXVECTOR4 parameters = D3DXVECTOR4(m_attenuation0, m_attenuation1, m_attenuation2, m_range);
	return parameters;
}


// ****************************************************************
// �X�|�b�g���C�g
// ================================================================
C_d3d_spot_light::C_d3d_spot_light()
{
	m_type = LIGHT_TYPE_SPOT;
	m_id = -1;

	m_pos = ZERO_VECTOR;
	m_dir = ZERO_VECTOR;

	// �ő�Ǝ˂����͈͂ƑS���Ǝ˂���Ȃ��͈͂�ݒ�
	// �܂��Ԃ͈̔͂̌v�Z���ʂ�falloff�悳��邯�ǐ��l��ύX���Ă��卷�Ȃ��̂�1.0f������
	m_theta = D3DXToRadian(15.0f);
	m_phi = D3DXToRadian(60.0f);
	m_falloff = 1.0f;

	// �����W�����S��0���Ɖ��̎���0���Z����������̂�0.001f�ɂ��Ă���
	// Atten = 1/( att0i + att1i * d + att2i * d2)
	m_attenuation0 = 0.001f;
	m_attenuation1 = 0.0f;
	m_attenuation2 = 0.0f;
	m_range = 100.0f;
}

C_d3d_spot_light::~C_d3d_spot_light()
{
}

D3DXVECTOR4 C_d3d_spot_light::get_light_local_pos(const D3DXMATRIX& world, const D3DXMATRIX& view)
{
	// �l�����Ȃ̂̓V�F�[�_�p
	D3DXVECTOR4 vec;
	D3DXMATRIX mat;		// ���[�J�����W�n����r���[���W�n�ւ̕ϊ��s��̋t�s��

	mat = world * view;
	D3DXMatrixInverse(&mat, NULL, &mat);
	vec = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVec4Transform(&vec, &vec, &mat);
	
	return vec;
}
D3DXVECTOR4 C_d3d_spot_light::get_light_world_pos()
{
	// �l�����Ȃ̂̓V�F�[�_�p
	D3DXVECTOR4 vec = D3DXVECTOR4(m_pos.x, m_pos.y, m_pos.z, 1.0f);
	return vec;
}
D3DXVECTOR4 C_d3d_spot_light::get_light_local_dir(const D3DXMATRIX& world)
{
	// �l�����Ȃ̂̓V�F�[�_�p
	D3DXVECTOR4 vec;
	D3DXMATRIX inv_world;
	D3DXMatrixInverse(&inv_world, NULL, &world);
	D3DXVec3Transform(&vec, &m_dir, &inv_world);
	D3DXVec3Normalize((D3DXVECTOR3*)&vec, (D3DXVECTOR3*)&vec);

	return vec;
}
D3DXVECTOR4 C_d3d_spot_light::get_light_world_dir()
{
	// �l�����Ȃ̂̓V�F�[�_�p
	D3DXVECTOR4 vec = D3DXVECTOR4(m_dir.x, m_dir.y, m_dir.z, 1.0f);
	return vec;
}

D3DXVECTOR4 C_d3d_spot_light::get_light_theta_phi_falloff()
{
	//D3DXVECTOR4 parameters = D3DXVECTOR4(m_theta, m_phi, m_falloff, 1.0f);
	// �V�F�[�_�ł� theta/2 �� phi/2 �����g��Ȃ��̂ŃV�F�[�_�ɓn�����O�Ŋ����Ă���
	D3DXVECTOR4 parameters = D3DXVECTOR4(m_theta / 2.0f, m_phi / 2.0f, m_falloff, 1.0f);

	return parameters;
}

D3DXVECTOR4 C_d3d_spot_light::get_light_cos_theta_phi_falloff()
{
	//D3DXVECTOR4 parameters = D3DXVECTOR4(m_theta, m_phi, m_falloff, 1.0f);
	// �V�F�[�_�ł� theta/2 �� phi/2 �����g��Ȃ��̂ŃV�F�[�_�ɓn�����O�Ŋ����Ă���
	D3DXVECTOR4 parameters = D3DXVECTOR4(cos(m_theta / 2.0f), cos(m_phi / 2.0f), m_falloff, 1.0f);

	return parameters;
}

D3DXVECTOR4 C_d3d_spot_light::get_light_attenuations_range()
{
	D3DXVECTOR4 parameters = D3DXVECTOR4(m_attenuation0, m_attenuation1, m_attenuation2, m_range);
	return parameters;
}








// ****************************************************************
// �V���h�E�}�b�v�p�X�|�b�g���C�g(�������Ƃ��͈͂͒ǉ����邩��)
// ================================================================
C_d3d_shadow_map_spot_light::C_d3d_shadow_map_spot_light()
{
	m_type = LIGHT_TYPE_SPOT_SM;
	m_id = -1;

	m_pos = ZERO_VECTOR;					// ���_
	m_tgt = ZERO_VECTOR;					// �����_
	m_up = UP_VECTOR;					// ������x�N�g��

	m_near_clip = 1.0f;			// �ߐڃN���b�v��
	m_far_clip = 1000.0f;				// �����N���b�v��
	//m_view_volume_width = 1280.0f;	// �r���[�{�����[���̕�
	//m_view_volume_height = 720.0f;	// �r���[�{�����[���̍���
	m_view_volume_width = (float)G_d3d.device.get_shadow_map_size();	// �r���[�{�����[���̕�
	m_view_volume_height = (float)G_d3d.device.get_shadow_map_size();	// �r���[�{�����[���̍���
}

C_d3d_shadow_map_spot_light::~C_d3d_shadow_map_spot_light()
{
}

D3DXVECTOR4 C_d3d_shadow_map_spot_light::get_light_local_dir(const D3DXMATRIX& world)
{
	// �l�����Ȃ̂̓V�F�[�_�p
	D3DXVECTOR3 dir = m_tgt - m_pos;
	D3DXVECTOR4 vec;
	D3DXMATRIX inv_world;
	D3DXMatrixInverse(&inv_world, NULL, &world);
	D3DXVec3Transform(&vec, &dir, &inv_world);
	D3DXVec3Normalize((D3DXVECTOR3*)&vec, (D3DXVECTOR3*)&vec);

	return vec;
}
D3DXVECTOR4 C_d3d_shadow_map_spot_light::get_light_world_dir()
{
	// �l�����Ȃ̂̓V�F�[�_�p
	D3DXVECTOR4 vec = D3DXVECTOR4(
		m_tgt.x - m_pos.x,
		m_tgt.y - m_pos.y,
		m_tgt.z - m_pos.z,
		1.0f
		);
	return vec;
}

D3DXMATRIX C_d3d_shadow_map_spot_light::get_view_matrix()
{
	D3DXMATRIX mat;

	D3DXMatrixLookAtLH(&mat, &m_pos, &m_tgt, &m_up);

	return mat;
	
}
D3DXMATRIX C_d3d_shadow_map_spot_light::get_proj_matrix()
{
	D3DXMATRIX mat;

	D3DXMatrixOrthoLH(&mat, m_view_volume_width, m_view_volume_height, m_near_clip, m_far_clip);

	return mat;
}














/*
C_d3d_light::C_d3d_light()
{
	// �p�����[�^
	type = LIGHT_TYPE_DIRECTIONAL;

	pos = D3DXVECTOR3(0, 0, 0);
	D3DXVec3Normalize(&dir, &D3DXVECTOR3(1, -1, 1));

	// �e�탉�C�g���ʃJ���[
	diffuse = D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f);
	ambient = D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f);
	specular = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	// ������
	range = 100.0f;
	theta0 = D3DXToRadian(30);
	theta1 = D3DXToRadian(45);
}
*/


}