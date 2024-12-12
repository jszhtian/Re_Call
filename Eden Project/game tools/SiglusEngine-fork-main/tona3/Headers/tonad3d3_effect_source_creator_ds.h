// C_d3d_effect_source_creator_ds.h

#pragma once

#include "tonad3d3_effect_source_creator.h"
#include "tonad3d3_effect_global_texture_val.h"
#include "tonad3d3_effect_global_sampler_val.h"

namespace NT3
{

class C_d3d_effect_source_creator_ds : public C_d3d_effect_source_creator {
	C_d3d_effect_global_texture_val diffuseTexture;	// �f�B�t���[�Y�e�N�X�`��
	C_d3d_effect_global_texture_val specularTexture;	// �X�y�L�����e�N�X�`��
	C_d3d_effect_global_sampler_val diffuseSampler;	// �f�B�t���[�Y�T���v��
	C_d3d_effect_global_sampler_val specularSampler;	// �X�y�L�����T���v��
	C_d3d_effect_global_val lightDir;					// ���C�g����
	C_d3d_effect_global_val worldMatrix, viewMatrix, projMatrix;	// WVP�s��

public:
	C_d3d_effect_source_creator_ds();
	virtual ~C_d3d_effect_source_creator_ds();

	// �O���[�o���ϐ���o�^
	virtual void registUserVals();	

	// ���_�V�F�[�_�R�[�h
	virtual void vertexShaderCode( std::string& out, C_d3d_effect_vertex_input_semantics_struct &In, C_d3d_effect_vertex_output_semantics_struct &Out );

	// �s�N�Z���V�F�[�_�R�[�h
	virtual void pixelShaderCode( std::string& out, C_d3d_effect_pixel_input_semantics_struct &In, C_d3d_effect_pixel_output_semantics_struct &Out );
};


}