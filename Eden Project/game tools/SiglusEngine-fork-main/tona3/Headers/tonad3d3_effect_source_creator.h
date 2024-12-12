// C_d3d_effect_source_creator.h

#pragma once

#include "tonad3d3_effect_semantics_info.h"
#include "tonad3d3_effect_texture_info.h"
#include "tonad3d3_effect_global_sampler_val.h"
#include "tonad3d3_effect_vertex_input_semantics_struct.h"
#include "tonad3d3_effect_vertex_output_semantics_struct.h"
#include "tonad3d3_effect_pixel_input_semantics_struct.h"
#include "tonad3d3_effect_pixel_output_semantics_struct.h"

#include "tonad3d3_effect_func_info.h"
#include "tonad3d3_effect_lighting_shader.h"
#include "tonad3d3_effect_lighting_and_shading_shader.h"


namespace NT3
{


class C_d3d_effect_source_creator {
protected:
	// ���_�V�F�[�_��s�N�Z���V�F�[�_�̐����x���N���X
	class CodeConverter {
		std::map<MBSTR, C_d3d_effect_global_val> globalMap;
		std::map<MBSTR, C_d3d_effect_global_sampler_val> samplerMap;
		C_d3d_effect_vertex_input_semantics_struct viSem;
		C_d3d_effect_vertex_output_semantics_struct voSem;
		C_d3d_effect_pixel_input_semantics_struct piSem;
		C_d3d_effect_pixel_output_semantics_struct poSem;

		int index_for_sort;

		// �T���v���[�֐��R�[�h�o��
		MBSTR samplerFuncOut( C_d3d_effect_texture_info &texInfo, C_d3d_effect_global_sampler_val &sampler );

	public:
		CodeConverter();

		// ���_�V�F�[�_���̓Z�}���e�B�N�X���̎擾
		C_d3d_effect_vertex_input_semantics_struct &getVISem() { return viSem; }

		// ���_�V�F�[�_�o�̓Z�}���e�B�N�X���̎擾
		C_d3d_effect_vertex_output_semantics_struct &getVOSem() { return voSem; }

		// �s�N�Z���V�F�[�_���̓Z�}���e�B�N�X���̎擾
		C_d3d_effect_pixel_input_semantics_struct &getPISem() { return piSem; }

		// �s�N�Z���V�F�[�_�o�̓Z�}���e�B�N�X���̎擾
		C_d3d_effect_pixel_output_semantics_struct &getPOSem() { return poSem; }

		// �u�������ϐ���o�^
		void regist(C_d3d_effect_global_val &val);
		void regist(C_d3d_effect_global_sampler_val &sampler);

		// ���_�V�F�[�_�R���o�[�g
		MBSTR convert(MBSTR src);

		// �u���ϐ������擾
		MBSTR find(MBSTR src);

		// �O���[�o���ϐ��������o��
		void globalValCode( MBSTR& out );

		// �u�������V�F�[�_���֐��̏����o��
		void samplerFunctionsCode( MBSTR& out, C_d3d_effect_texture_info &texInfo );
	};

protected:
	CodeConverter conv;						// �{���V�F�[�_�R���o�[�^


	MBSTR	keyFileName;				// �������ꂽ�V�F�[�_��(key)

protected:
	// �R���o�[�^�֊e�ϐ���o�^
	void registValsEntry();
	virtual void registUserVals();	// �h���N���X�̃O���[�o���ϐ���o�^���܂�

	// �O���[�o���ϐ��R�[�h
	void globalValCode( MBSTR& out );

	// ���_�V�F�[�_�̍\���̐錾
	void vertexShaderStructsCode( MBSTR& out );

	// ���_�V�F�[�_�G���g���錾���쐬( VS_OUTPUT vs_main()...�ȕ����j
	void vertexEntryCode( MBSTR& out );

	// ���_�V�F�[�_�̌����Z�}���e�B�N�X�̏������R�[�h�}��
	void missingVertexSemanticsInitializeCode( MBSTR& out, C_d3d_effect_semantics_info &modelSemInfo, C_d3d_effect_vertex_input_semantics_struct &In );

	// ���_�V�F�[�_�R�[�h
	virtual void vertexShaderCode( MBSTR& out, C_d3d_effect_vertex_input_semantics_struct &In, C_d3d_effect_vertex_output_semantics_struct &Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so );

	// ���_�V�F�[�_�I�[�i"};"�����j
	void vertexTerminateCode( MBSTR& out );

	// �s�N�Z���V�F�[�_�o�͍\���̐錾
	void pixelShaderOutputStructsCode( MBSTR& out );

	// �e�N�X�`���T���v�����O�֐�������
	void samplerFunctionsCode(  MBSTR& out, C_d3d_effect_texture_info &texInfo );

	// �s�N�Z���V�F�[�_�G���g���錾���쐬( PS_OUTPUT ps_main()...�ȕ����j
	void pixelEntryCode( MBSTR& out );

	// �s�N�Z���V�F�[�_�R�[�h
	virtual void pixelShaderCode( MBSTR& out, C_d3d_effect_pixel_input_semantics_struct &In, C_d3d_effect_pixel_output_semantics_struct &Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so );

	// �s�N�Z���V�F�[�_�I�[�i"};"�����j
	void pixelTerminateCode( MBSTR& out );

	// �e�N�j�b�N�R�[�h
	void techniqueCode( MBSTR& out );

	// �R�[�h���R���o�[�g
	void convert( MBSTR& out );

	// �֐��R�[�h
	virtual void functionCode( MBSTR& out, C_d3d_effect_func_info &funcInfo );

	// �������p
	void init(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so);
	virtual void initialize(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so);

	// �V�F�[�_��(key)�𐶐�
	/*
	void createKeyFileName(C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo, C_d3d_effect_func_info &funcInfo, LIGHTING_TYPE lightType, SHADING_TYPE shadeType);
	virtual void createKeyFileName(C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo) = 0;
	*/

public:
	C_d3d_effect_source_creator();
	~C_d3d_effect_source_creator();

	// �V�F�[�_����
	void createShader( MBSTR& out, C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo, C_d3d_effect_func_info &funcInfo, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so );

	// �������ꂽ�V�F�[�_��(key)���擾
	MBSTR getKeyFileName();
};


}