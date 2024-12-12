


#pragma once

#include "tonad3d3_effect_source_creator.h"
#include "tonad3d3_effect_global_texture_val.h"
#include "tonad3d3_effect_global_sampler_val.h"

#include "tonad3d3_effect_global_val_pack.h"


namespace NT3
{


class C_d3d_effect_source_creator_skinned_mesh : public C_d3d_effect_source_creator {
private:
	C_d3d_effect_global_val	MAX_MATRICES;
	C_d3d_effect_global_val	mWorldMatrixArray;
	C_d3d_effect_global_val	mWorld;
	C_d3d_effect_global_val	mLocal;
	C_d3d_effect_global_val	mView;
	C_d3d_effect_global_val	mProj;
	C_d3d_effect_global_val	mLightVP;
	C_d3d_effect_global_val	mLightWVP;
	C_d3d_effect_global_val	CurNumBones;

	C_d3d_effect_global_texture_val	meshTexture;
	C_d3d_effect_global_sampler_val	meshSampler;
	C_d3d_effect_global_texture_val	shadowTexture;
	C_d3d_effect_global_sampler_val	shadowSampler;
	C_d3d_effect_global_texture_val	toonTexture;
	C_d3d_effect_global_sampler_val	toonSampler;
	C_d3d_effect_global_texture_val	normalTexture;
	C_d3d_effect_global_sampler_val	normalSampler;

	C_d3d_effect_global_val_pack		globalValPack;
	
public:
	C_d3d_effect_source_creator_skinned_mesh();
	virtual ~C_d3d_effect_source_creator_skinned_mesh();

	virtual void initialize(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so);

	// �O���[�o���ϐ���o�^
	virtual void registUserVals();

	// ���_�V�F�[�_�R�[�h
	virtual void vertexShaderCode(MBSTR& out, C_d3d_effect_vertex_input_semantics_struct& In, C_d3d_effect_vertex_output_semantics_struct& Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so);

	// �s�N�Z���V�F�[�_�R�[�h
	virtual void pixelShaderCode(MBSTR& out, C_d3d_effect_pixel_input_semantics_struct& In, C_d3d_effect_pixel_output_semantics_struct& Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so);

	// �֐��R�[�h
	virtual void functionCode( MBSTR& out, C_d3d_effect_func_info &funcInfo );

	// �V�F�[�_��(key)�𐶐�
//	void createKeyFileName(C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo);
};





}