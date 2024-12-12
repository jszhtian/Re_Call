#pragma once

#include "tonad3d3_effect_source_creator.h"


namespace NT3
{

// ���C�g���猩���[�x�l�������V�F�[�_
class C_d3d_effect_source_creator_shadow_map : public C_d3d_effect_source_creator {
private:
	C_d3d_effect_global_val	MAX_MATRICES;
	C_d3d_effect_global_val	mWorldMatrixArray;
	C_d3d_effect_global_val	CurNumBones;
	C_d3d_effect_global_val	mLightVP;

public:
	C_d3d_effect_source_creator_shadow_map();
	virtual ~C_d3d_effect_source_creator_shadow_map();

	// ������
	virtual void initialize(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so);

	// �O���[�o���ϐ���o�^
	virtual void registUserVals();

	// ���_�V�F�[�_�R�[�h
	virtual void vertexShaderCode(MBSTR& out, C_d3d_effect_vertex_input_semantics_struct& In, C_d3d_effect_vertex_output_semantics_struct& Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so);

	// �s�N�Z���V�F�[�_�R�[�h
	virtual void pixelShaderCode(MBSTR& out, C_d3d_effect_pixel_input_semantics_struct& In, C_d3d_effect_pixel_output_semantics_struct& Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so);
};


}