#pragma once

#include "tonad3d3_effect_semantics_info.h"
#include "tonad3d3_effect_lighting_and_shading_shader.h"

namespace NT3
{

class C_d3d_effect_lighting_shader {
public:

	// ���_�V�F�[�_�R�[�h
	static MBSTR getVertexShaderCode(LIGHTING_TYPE type, MBSTR varNameNormal);
	// �s�N�Z���V�F�[�_�R�[�h
	static MBSTR getPixelShaderCode(LIGHTING_TYPE type);

private:
	// �e��V�F�[�_�R�[�h

	// �����o�[�g
	static MBSTR LambertVS(MBSTR varNameNormal);
	static MBSTR LambertPS();
	
	// �u�����t�H��
	static MBSTR BlinnPhongVS(MBSTR varNameNormal);
	static MBSTR BlinnPhongPS();
	
	// �u�����t�H��(�p�[�s�N�Z��)
	static MBSTR PerPixelBlinnPhongVS(MBSTR varNameNormal);
	static MBSTR PerPixelBlinnPhongPS();

	// �n�[�t�����o�[�g(�p�[�s�N�Z��)
	static MBSTR PerPixelHalfLambertVS(MBSTR varNameNormal);
	static MBSTR PerPixelHalfLambertPS();

	// �t�����g�E�C���O�d�l�̃g�D�[�����ۂ�����
	static MBSTR ToonVS(MBSTR varNameNormal);
	static MBSTR ToonPS();

	// �Œ�@�\�p�C�v���C�����Č�������
	static MBSTR FixedFunctionPipelineVS(MBSTR varNameNormal);
	static MBSTR FixedFunctionPipelinePS();

	// �Œ�@�\�p�C�v���C�����Č�������(�p�[�s�N�Z��)
	static MBSTR PerPixelFixedFunctionPipelineVS(MBSTR varNameNormal);
	static MBSTR PerPixelFixedFunctionPipelinePS();

	// �o���v�}�b�s���O
	static MBSTR BumpMappingVS(MBSTR varNameNormal);
	static MBSTR BumpMappingPS();

	// �����}�b�s���O
	static MBSTR ParallaxMappingVS(MBSTR varNameNormal);
	static MBSTR ParallaxMappingPS();
};


}