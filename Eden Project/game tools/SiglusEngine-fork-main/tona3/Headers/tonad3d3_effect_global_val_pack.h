#pragma once


#include "tonad3d3_effect_global_val.h"
#include "tonad3d3_light_manager.h"


namespace NT3
{




// 2D��3D�p�͕����������ǂ�����
class C_d3d_effect_global_val_pack {
private:
	std::vector<C_d3d_effect_global_val> vals;

public:
	C_d3d_effect_global_val_pack()
	{
		// �F���Ȃ�
		// ���m�N���p(�Œ�)�l
		vals.push_back( C_d3d_effect_global_val("float4", "g_Mono", "float4(0.2989f, 0.5886f, 0.1145f, 0.0f)", "") );
		// mono, reverse, bright, dark
		vals.push_back( C_d3d_effect_global_val("float4", "g_C0", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// r, g, b, rate
		vals.push_back( C_d3d_effect_global_val("float4", "g_C1", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// r, g, b
		vals.push_back( C_d3d_effect_global_val("float4", "g_C2", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// tonecurve_no, tonecurve_sat
		vals.push_back( C_d3d_effect_global_val("float4", "g_C3", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );

		// �}�e���A��
		// �}�e���A���̃f�B�t���[�Y(�g�U��)
		vals.push_back( C_d3d_effect_global_val("float4", "g_MaterialDiffuse", "float4(0.0f, 0.0f, 0.0f, 1.0f)", "") );
		// �}�e���A���̃A���r�G���g(�����A���͌�)
		vals.push_back( C_d3d_effect_global_val("float4", "g_MaterialAmbient", "float4(0.0f, 0.0f, 0.0f, 1.0f)", "") );
		// �}�e���A���̃X�y�L����(���ˌ�)
		vals.push_back( C_d3d_effect_global_val("float4", "g_MaterialSpecular", "float4(1.0f, 1.0f, 1.0f, 1.0f)", "") );
		// �}�e���A���̃X�y�L�����p���[
		vals.push_back( C_d3d_effect_global_val("float", "g_MaterialSpecularPower", "float(1.0f)", "") );
		// �}�e���A���̃G�~�b�V�u(���ȏƖ�)
		vals.push_back( C_d3d_effect_global_val("float4", "g_MaterialEmissive", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );

		// �e�탉�C�g
		// �e�탉�C�g�̔z��p�ϐ�
		MBSTR array_num;
		// �f�B���N�V���i�����C�g
		array_num = TSTR_to_MBSTR(tostr(C_d3d_light_manager::OBJ_DIRECTIONAL_LIGHT_MAX));
		vals.push_back( C_d3d_effect_global_val("static const int", "DLIGHT_MAX", array_num, "") );
		// ���ۂ̃��C�g�̐�
		vals.push_back( C_d3d_effect_global_val("int", "g_DLightNum", "0", "") );
		// ���C�g�̃f�B�t���[�Y
		vals.push_back( C_d3d_effect_global_val("float4", "g_DLightDiffuse[DLIGHT_MAX]", array_num+"@float4(0.7f, 0.7f, 0.7f, 1.0f)@", "") );
		// ���C�g�̃A���r�G���g
		vals.push_back( C_d3d_effect_global_val("float4", "g_DLightAmbient[DLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ���C�g�̃X�y�L����
		vals.push_back( C_d3d_effect_global_val("float4", "g_DLightSpecular[DLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ���C�g�̕���(���[�J�����W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_DLightLocalDir[DLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ���C�g�̕���(���[���h���W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_DLightWorldDir[DLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );


		// �|�C���g���C�g
		array_num = TSTR_to_MBSTR(tostr(C_d3d_light_manager::OBJ_POINT_LIGHT_MAX));
		vals.push_back( C_d3d_effect_global_val("static const int", "PLIGHT_MAX", array_num, "") );
		// ���ۂ̃��C�g�̐�
		vals.push_back( C_d3d_effect_global_val("int", "g_PLightNum", "0", "") );
		// ���C�g�̃f�B�t���[�Y
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightDiffuse[PLIGHT_MAX]", array_num+"@float4(0.7f, 0.7f, 0.7f, 1.0f)@", "") );
		// ���C�g�̃A���r�G���g
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightAmbient[PLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ���C�g�̃X�y�L����
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightSpecular[PLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ���C�g�̍��W(���[�J�����W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightLocalPos[PLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ���C�g�̍��W(���[���h���W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightWorldPos[PLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ���C�g�̌����n�p�����[�^(atten0, atten1, atten2, range)
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightAttenuationsRange[PLIGHT_MAX]", array_num+"@float4(0.001f, 0.0f, 0.0f, 0.0f)@", "") );
		

		// �X�|�b�g���C�g
		array_num = TSTR_to_MBSTR(tostr(C_d3d_light_manager::OBJ_SPOT_LIGHT_MAX));
		vals.push_back( C_d3d_effect_global_val("static const int", "SLIGHT_MAX", array_num, "") );
		// ���ۂ̃��C�g�̐�
		vals.push_back( C_d3d_effect_global_val("int", "g_SLightNum", "0", "") );
		// ���C�g�̃f�B�t���[�Y
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightDiffuse[SLIGHT_MAX]", array_num+"@float4(0.7f, 0.7f, 0.7f, 1.0f)@", "") );
		// ���C�g�̃A���r�G���g
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightAmbient[SLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ���C�g�̃X�y�L����
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightSpecular[SLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ���C�g�̕���(���[�J�����W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightLocalDir[SLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ���C�g�̕���(���[���h���W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightWorldDir[SLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ���C�g�̍��W(���[�J�����W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightLocalPos[SLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ���C�g�̍��W(���[���h���W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightWorldPos[SLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ���C�g�̌����n�p�����[�^(atten0, atten1, atten2, range)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightAttenuationsRange[SLIGHT_MAX]", array_num+"@float4(0.001f, 0.0f, 0.0f, 0.0f)@", "") );
		// �X�|�b�g���C�g�n�p�����[�^(theta/2, phi/2, falloff, w���g�p)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightThetaPhiFalloff[SLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );

		// �O���[�o���A���r�G���g(�Œ�@�\�p�C�v���C���Č��p)
		vals.push_back( C_d3d_effect_global_val("float4", "g_GlobalAmbient", "float4(0.0f, 0.0f, 0.0f, 1.0f)", "") );



		// �]���̃��C�g
		// ���C�g�̕���(���[�J�����W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightLocalDir", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// ���C�g�̕���(���[���h���W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightWorldDir", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// ���C�g�̍��W(���[�J�����W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightLocalPos", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// ���C�g�̍��W(���[���h���W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightWorldPos", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// ���C�g�̃f�B�t���[�Y
		//vals.push_back( C_d3d_effect_global_val("float4", "g_LightDiffuse", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightDiffuse", "float4(0.7f, 0.7f, 0.7f, 1.0f)", "") );
		// ���C�g�̃A���r�G���g
		//vals.push_back( C_d3d_effect_global_val("float4", "g_LightAmbient", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightAmbient", "float4(0.3f, 0.3f, 0.3f, 1.0f)", "") );
		// ���C�g�̌�����(W�����s�v)
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightAttenuations", "float4(0.001f, 0.0f, 0.0f, 0.0f)", "") );

		// �J����
		// �J�����̍��W(���[�J�����W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_CameraLocalPos", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// �J�����̍��W(���[���h���W�n)
		vals.push_back( C_d3d_effect_global_val("float4", "g_CameraWorldPos", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );


		// �[�x�o�b�t�@�V���h�E�p
		// �o�C�A�X�l
		vals.push_back( C_d3d_effect_global_val("float", "g_DBSBias", "float(0.03f)", "") );

		// �������C�g�p
		// �������C�g�̓K�p�͈͂Ƌ��x�p�p�����[�^
		vals.push_back( C_d3d_effect_global_val("float", "g_RimLightPower", "float(0.5f)", "") );
		// �������C�g�̐F(�e�X�g�̎��͕�����₷���F���ǂ�)
		//vals.push_back( C_d3d_effect_global_val("float4", "g_RimLightColor", "float4(1.0f, 0.0f, 0.0f, 0.0f)", "") );
		vals.push_back( C_d3d_effect_global_val("float4", "g_RimLightColor", "float4(1.0f, 1.0f, 1.0f, 1.0f)", "") );
		//vals.push_back( C_d3d_effect_global_val("float4", "g_RimLightColor", "float4(-1.0f, -1.0f, -1.0f, 0.0f)", "") );


		// ���_�J���[�p
		// ���_�J���[�{��(�I�u�W�F�N�g�Ƀf�[�^���Ȃ��ꍇ�X�L�����b�V����1.0f������̂�rate=1.0f���ƌ����ڂɕω��Ȃ�)
		vals.push_back( C_d3d_effect_global_val("float", "g_VertexColorRate", "float(1.0f)", "") );
		//vals.push_back( C_d3d_effect_global_val("float", "g_VertexColorRate", "float(1.5f)", "") );

		// �X�L�����b�V���̃��[���h�����[�J���ϊ��p
		// ���[���h�s��̋t�s��
		//vals.push_back( C_d3d_effect_global_val("float4x4", "mInvWorld", "", "") );

		// �����}�b�s���O�p
		// �����}�b�s���O�̍ő�̍���
		vals.push_back( C_d3d_effect_global_val("float", "g_ParallaxMaxHeight", "float(0.016f)", "") );




	}

	std::vector<C_d3d_effect_global_val>::iterator begin()
	{
		return vals.begin();
	}
	std::vector<C_d3d_effect_global_val>::iterator end()
	{
		return vals.end();
	}	
};





}