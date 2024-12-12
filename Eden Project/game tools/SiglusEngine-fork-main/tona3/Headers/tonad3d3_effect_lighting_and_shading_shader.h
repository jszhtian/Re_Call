#pragma once

#include "tonad3d3_effect_semantics_info.h"

namespace NT3
{

// ���C�e�B���O�̎��
enum LIGHTING_TYPE {
	LIGHTING_TYPE_NONE = 0,
	LIGHTING_TYPE_LAMBERT,
	LIGHTING_TYPE_BLINN_PHONG,
	LIGHTING_TYPE_PP_BLINN_PHONG,
	LIGHTING_TYPE_PP_HALF_LAMBERT,

	LIGHTING_TYPE_TOON,
	LIGHTING_TYPE_FFP,
	LIGHTING_TYPE_PP_FFP,

	LIGHTING_TYPE_BUMP,
	LIGHTING_TYPE_PARALLAX,
};

// �e�̎��
enum SHADING_TYPE {
	SHADING_TYPE_NONE = 0,
	SHADING_TYPE_DEPTH_BUFFER,
};

// �V�F�[�_�I�v�V����
enum SHADER_OPTION {
	SHADER_OPTION_NONE			= 0 << 0,
	SHADER_OPTION_RIM_LIGHT		= 1 << 0,
	//SHADER_OPTION_HOGE	= 1 << 1,
};

class C_d3d_effect_lighting_and_shading_shader {
public:
	// �Z�}���e�B�N�X���̍č\�z
	static DWORD restructSemanticsBit(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so, DWORD bit);

	// ���_�V�F�[�_�R�[�h
	static MBSTR getVertexShaderCode(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so, MBSTR varNameNormal);
	// �s�N�Z���V�F�[�_�R�[�h
	static MBSTR getPixelShaderCode(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so);
private:
	// �e��e����R�[�h
	static MBSTR getVertexShaderCodeWithShadow(LIGHTING_TYPE lt, SHADING_TYPE st, MBSTR varNameNormal);
	static MBSTR getPixelShaderCodeWithShadow(LIGHTING_TYPE lt, SHADING_TYPE st);



	// �e��V�F�[�_�R�[�h

	// �e�̏������蕪�Ƃ�
	bool ShaderChecker(SHADING_TYPE st);
	// �e�t���̊�b�ƂȂ�R�[�h
	MBSTR ShadingBaseCodeVS(SHADING_TYPE st);
	MBSTR ShadingBaseCodePS(SHADING_TYPE st);

	// ���C�e�B���O�̊�b�ƂȂ�R�[�h�A�e���ǂ��Ȃ��Ă�����
	MBSTR LightingBaseCodeVS(LIGHTING_TYPE lt);
	MBSTR LightingBaseCodePS(LIGHTING_TYPE lt);


	// �V�F�[�_�I�v�V�����R�[�h
	// �������C�g(���_�V�F�[�_�v�Z)
	static MBSTR RimLightCalcVSVS(LIGHTING_TYPE lt, SHADING_TYPE st);
	static MBSTR RimLightCalcVSPS(LIGHTING_TYPE lt, SHADING_TYPE st);

	// �������C�g(�s�N�Z���V�F�[�_�v�Z)������
	static MBSTR RimLightCalcPSVS(LIGHTING_TYPE lt, SHADING_TYPE st);
	static MBSTR RimLightCalcPSPS(LIGHTING_TYPE lt, SHADING_TYPE st);
};
// varNameNormal�݂����Ȃ̑��₷�H


}