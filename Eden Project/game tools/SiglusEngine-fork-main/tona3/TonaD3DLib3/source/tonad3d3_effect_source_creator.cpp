// C_d3d_effect_source_creator.cpp

#include "tonad3d3_pch.h"
#include "tonad3d3_effect_source_creator.h"
#include "tonad3d3_effect_sampler_func.h"
#include <vector>

namespace NT3
{

// �\�[�g�p
bool compare_global_val(C_d3d_effect_global_val& lhs, C_d3d_effect_global_val& rhs)
{
	if(lhs.getIndex() < rhs.getIndex()) return true;
	return false;
}

//////////////////////////
// �{���R���o�[�^
////
C_d3d_effect_source_creator::CodeConverter::CodeConverter()
:index_for_sort(0)
{
}

// �u�������ϐ���o�^
void C_d3d_effect_source_creator::CodeConverter::regist(C_d3d_effect_global_val &val)
{
	index_for_sort++;
	val.setIndex(index_for_sort);
	globalMap.insert( std::pair<MBSTR, C_d3d_effect_global_val>(val.n(), val) );
}

void C_d3d_effect_source_creator::CodeConverter::regist(C_d3d_effect_global_sampler_val &sampler)
{
	samplerMap.insert( std::pair<MBSTR, C_d3d_effect_global_sampler_val>(sampler.n(), sampler) );
}

// �u���ϐ������擾
MBSTR C_d3d_effect_source_creator::CodeConverter::find(MBSTR src)
{
	// �O���[�o���ϐ��H
	std::map<MBSTR, C_d3d_effect_global_val>::iterator it = globalMap.find( src );
	if ( it != globalMap.end() )
		return it->second;

	// ���_�V�F�[�_�̃Z�}���e�B�N�X�H
	MBSTR res;
	if ( viSem.find(src, res) || voSem.find(src, res) || piSem.find(src, res) || poSem.find(src, res) )
		return res;

	// �֐����H
	{
		std::map<MBSTR, C_d3d_effect_global_sampler_val>::iterator it = samplerMap.find(src);
		if( it != samplerMap.end() )
			return it->second.getFuncName();
	}

	// �Ȃ��I
	return "**/not found the val./**";
}

// ���_�V�F�[�_�R���o�[�g
MBSTR C_d3d_effect_source_creator::CodeConverter::convert(MBSTR src)
{
	MBSTR out;

	// �u$�v���o�Ă�����u������
	size_t sz = src.size();
	for ( size_t i = 0; i < sz; i++ ) {
		if ( src[i] == '$' ) {
			// �u�����������o
			MBSTR name;
			i+=2;
			while(i < sz && src[i] != ')')
				name += src[i++];
			// �u����������ϊ�
			out += find(name);
			continue;
		}
		out += src[i];
	}
	return out;
}

// �O���[�o���ϐ��������o��
void C_d3d_effect_source_creator::CodeConverter::globalValCode( MBSTR& out )
{
	// �O���[�o���ϐ������o��
	{
		std::map<MBSTR, C_d3d_effect_global_val>::iterator  it = globalMap.begin();
		std::map<MBSTR, C_d3d_effect_global_val>::iterator end = globalMap.end();

		// MAP��Key�Ń\�[�g����Ă���̂�VECTOR�ɓ���ă\�[�g������
		std::vector<C_d3d_effect_global_val> temp;
		for ( ; it != end; it++ )
			temp.push_back(it->second);

		// �\�[�g����
		std::sort(temp.begin(), temp.end(), compare_global_val);
		
		// �����o��
		for(unsigned int i = 0; i < temp.size(); i++) {
			out += temp[i].decl();
		}

		//for ( ; it != end; it++ )
		//	out += it->second.decl();
	}

	out += "\n";

	// �T���v�������o��
	{
		std::map<MBSTR, C_d3d_effect_global_sampler_val>::iterator  it = samplerMap.begin();
		std::map<MBSTR, C_d3d_effect_global_sampler_val>::iterator end = samplerMap.end();
		for ( ; it != end; it++ )
			out += it->second.decl();
	}
}

// �u�������V�F�[�_���֐��̏����o��
void C_d3d_effect_source_creator::CodeConverter::samplerFunctionsCode( MBSTR& out, C_d3d_effect_texture_info &texInfo )
{
	// �T���v�������o��
	{
		std::map<MBSTR, C_d3d_effect_global_sampler_val>::iterator  it = samplerMap.begin();
		std::map<MBSTR, C_d3d_effect_global_sampler_val>::iterator end = samplerMap.end();
		for ( ; it != end; it++ )
			out += samplerFuncOut(texInfo, it->second);
	}
}

// �T���v���[�֐��R�[�h�o��
MBSTR C_d3d_effect_source_creator::CodeConverter::samplerFuncOut( C_d3d_effect_texture_info &texInfo, C_d3d_effect_global_sampler_val &sampler )
{
	return ( texInfo.find(sampler.getTextureSemanticsName().c_str()) ? sampler.samplerCode() : sampler.initCode() );
}

//////////////////////////
// C_d3d_effect_source_creator
////
C_d3d_effect_source_creator::C_d3d_effect_source_creator()
{
}

C_d3d_effect_source_creator::~C_d3d_effect_source_creator()
{
}

void C_d3d_effect_source_creator::createShader( MBSTR& out, C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo, C_d3d_effect_func_info &funcInfo, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so )
{
	// �܂��͏��������s��
	init(lightType, shadeType, so);

	
	// �e���v���[�g�ɏ]���ăR�[�h��������쐬���Ă���
	out = "";

	registValsEntry();												// Creator���̕ϐ����R���o�[�^�֓o�^

	globalValCode(out);												// �O���[�o���ϐ��R�[�h
	out += "\n";
	functionCode(out, funcInfo);									// �֐��R�[�h
	
	// ���_�V�F�[�_
	vertexShaderStructsCode(out);									// ���_�V�F�[�_�̍\���̐錾
	vertexEntryCode(out);											// ���_�V�F�[�_�G���g���錾���쐬( VS_OUTPUT vs_main()...�ȕ����j
	missingVertexSemanticsInitializeCode( out, semInfo, conv.getVISem() );	// ���_�V�F�[�_�̌����Z�}���e�B�N�X�̏������R�[�h�}��
	vertexShaderCode(out, conv.getVISem(), conv.getVOSem(), lightType, shadeType, so);						// ���_�V�F�[�_�R�[�h
	vertexTerminateCode(out);										// ���_�V�F�[�_�I�[�i"};"�����j

	// �e�N�X�`���̗L���ɂ���ďo�̓R�[�h�̕ύX
	samplerFunctionsCode( out, texInfo );							// �e�N�X�`���T���v�����O�֐�������
	
	// �s�N�Z���V�F�[�_
	pixelShaderOutputStructsCode(out);								// �s�N�Z���V�F�[�_�̍\���̐錾
	pixelEntryCode(out);											// �s�N�Z���V�F�[�_�G���g���錾���쐬( PS_OUTPUT ps_main()...�ȕ����j
	pixelShaderCode(out, conv.getPISem(), conv.getPOSem(), lightType, shadeType, so);			// �s�N�Z���V�F�[�_�R�[�h
	pixelTerminateCode(out);										// �s�N�Z���V�F�[�_�I�[�i"};"�����j

	// �e�N�j�b�N
	techniqueCode(out);												// �e�N�j�b�N�R�[�h

	convert(out);													// �V�F�[�_���������u��


	// �����܂ł̌��ʂ���t�@�C����(key)�𐶐�����
//	createKeyFileName(semInfo, texInfo, funcInfo, lightType, shadeType);
}

// �R���o�[�^�֊e�ϐ���o�^
void C_d3d_effect_source_creator::registValsEntry()
{
	registUserVals();
}
void C_d3d_effect_source_creator::registUserVals()
{
}

// �O���[�o���ϐ��R�[�h
void C_d3d_effect_source_creator::globalValCode( MBSTR& out )
{
	conv.globalValCode( out );
}

// ���_�V�F�[�_�̍\���̐錾
void C_d3d_effect_source_creator::vertexShaderStructsCode( MBSTR& out )
{
	out += conv.getVISem().decl();
	out += conv.getVOSem().decl();
}

// ���_�V�F�[�_�G���g���錾���쐬( VS_OUTPUT vs_main()...�ȕ����j
void C_d3d_effect_source_creator::vertexEntryCode( MBSTR& out )
{
	out += "VS_OUTPUT vs_main( VS_INPUT In ) {\n";
}

// ���_�V�F�[�_�̌����Z�}���e�B�N�X�̏������R�[�h�}��
void C_d3d_effect_source_creator::missingVertexSemanticsInitializeCode( MBSTR& out, C_d3d_effect_semantics_info &modelSemInfo, C_d3d_effect_vertex_input_semantics_struct &In )
{
	In.missingVertexSemanticsInitializeCode(out, modelSemInfo);
	out += "\n";
}

// ���_�V�F�[�_�R�[�h
void C_d3d_effect_source_creator::vertexShaderCode( MBSTR& out, C_d3d_effect_vertex_input_semantics_struct &In, C_d3d_effect_vertex_output_semantics_struct &Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so )
{
	out +=	"\tVS_OUTPUT Out = (VS_OUTPUT)0;\n"
			"\tOut.pos = In.pos;\n"
			"\treturn Out;\n";
}

// ���_�V�F�[�_�I�[�i"};"�����j
void C_d3d_effect_source_creator::vertexTerminateCode( MBSTR& out )
{
	out += "};\n";
	out +=	"\n";
}

// �s�N�Z���V�F�[�_�o�͍\���̐錾
void C_d3d_effect_source_creator::pixelShaderOutputStructsCode( MBSTR& out )
{
	out += conv.getPISem().decl();
	out += conv.getPOSem().decl();
}

// �e�N�X�`���T���v�����O�֐�������
void C_d3d_effect_source_creator::samplerFunctionsCode( MBSTR& out, C_d3d_effect_texture_info &texInfo )
{
	conv.samplerFunctionsCode( out, texInfo );
}

// �s�N�Z���V�F�[�_�G���g���錾���쐬( PS_OUTPUT ps_main()...�ȕ����j
void C_d3d_effect_source_creator::pixelEntryCode( MBSTR& out )
{
	out +=	"PS_OUTPUT ps_main( VS_OUTPUT In ) {\n";
}

// �s�N�Z���V�F�[�_�R�[�h
void C_d3d_effect_source_creator::pixelShaderCode( MBSTR& out, C_d3d_effect_pixel_input_semantics_struct &In, C_d3d_effect_pixel_output_semantics_struct &Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so )
{
	out +=	"\tPS_OUTPUT Out = (PS_OUTPUT)0;\n"
			"\tOut.color = float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
			"\treturn Out;\n";
}

// �s�N�Z���V�F�[�_�I�[�i"};"�����j
void C_d3d_effect_source_creator::pixelTerminateCode( MBSTR& out )
{
	out +=	"}\n";
	out +=	"\n";
}

// �e�N�j�b�N�R�[�h
void C_d3d_effect_source_creator::techniqueCode( MBSTR& out )
{
	out +=	"technique tech {\n"
			"\tpass p0 {\n"
			"\t\tVertexShader = compile vs_2_0 vs_main();\n"
			"\t\tPixelShader  = compile ps_2_0 ps_main();\n"
			"\t}\n"
			"}\n";
}

// �V�F�[�_�R�[�h��W�J
void C_d3d_effect_source_creator::convert( MBSTR& out )
{		
	out = conv.convert(out);
}


// �֐��R�[�h�����x���֐�
MBSTR supportFunctionCode(C_d3d_effect_func_info& funcInfo, C_d3d_effect_func_info::TYPE type, C_d3d_effect_func_interface* func)
{
	return funcInfo.find(type) ? func->realCode() : func->initCode();
}

// �֐��R�[�h
void C_d3d_effect_source_creator::functionCode( MBSTR& out, C_d3d_effect_func_info &funcInfo )
{
	/*
	if(funcInfo.find(C_d3d_effect_func_info::MRBD))		;
	if(funcInfo.find(C_d3d_effect_func_info::RGB))		;
	if(funcInfo.find(C_d3d_effect_func_info::TONECURVE))	;
	if(funcInfo.find(C_d3d_effect_func_info::MUL))		;
	//if(funcInfo.find(C_d3d_effect_func_info::MASK))		;
	if(funcInfo.find(C_d3d_effect_func_info::VERTEX))		;
	*/

	out += supportFunctionCode(funcInfo, C_d3d_effect_func_info::MRBD,		&C_d3d_effect_mrbd_color_func());
	out += supportFunctionCode(funcInfo, C_d3d_effect_func_info::RGB,		&C_d3d_effect_rgb_color_func());
	out += supportFunctionCode(funcInfo, C_d3d_effect_func_info::VERTEX,	&C_d3d_effect_vertex_color_func());
}

// �������p
void C_d3d_effect_source_creator::init(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so)
{
	initialize(lightType, shadeType, so);
}

void C_d3d_effect_source_creator::initialize(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so)
{
}
/*
// �V�F�[�_��(key)�𐶐�
void C_d3d_effect_source_creator::createKeyFileName(C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo, C_d3d_effect_func_info &funcInfo, LIGHTING_TYPE lightType, SHADING_TYPE shadeType)
{
	// �h���N���X�̊֐��Ăяo��
	createKeyFileName(semInfo, texInfo);

	if(funcInfo.find(C_d3d_effect_func_info::MRBD))		keyFileName += "_mrbd";
	if(funcInfo.find(C_d3d_effect_func_info::RGB))		keyFileName += "_rgb";
	if(funcInfo.find(C_d3d_effect_func_info::TONECURVE))	keyFileName += "_tc";
	if(funcInfo.find(C_d3d_effect_func_info::MUL))		keyFileName += "_mul";
	//if(funcInfo.find(C_d3d_effect_func_info::MASK))		keyFileName += "_mask";

	if(false);
	else if(lightType == C_d3d_effect_lighting_shader::NONE)				keyFileName += "";
	else if(lightType == C_d3d_effect_lighting_shader::LAMBERT)			keyFileName += "_lamb";
	else if(lightType == C_d3d_effect_lighting_shader::BLINN_PHONG)		keyFileName += "_blph";
	else if(lightType == C_d3d_effect_lighting_shader::PP_BLINN_PHONG)	keyFileName += "_ppbp";
	else if(lightType == C_d3d_effect_lighting_shader::PP_HALF_LAMBERT)	keyFileName += "_pphl";



	//keyFileName += ".fx";
}
*/
// �������ꂽ�V�F�[�_��(key)���擾
MBSTR C_d3d_effect_source_creator::getKeyFileName()
{
	return keyFileName;
}


}