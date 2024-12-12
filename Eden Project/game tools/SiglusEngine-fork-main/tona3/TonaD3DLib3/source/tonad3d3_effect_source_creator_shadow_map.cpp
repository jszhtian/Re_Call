#include "tonad3d3_pch.h"
#include "tonad3d3_effect_source_creator_shadow_map.h"


namespace NT3
{


C_d3d_effect_source_creator_shadow_map::C_d3d_effect_source_creator_shadow_map()
:MAX_MATRICES(		"static const int",	"MAX_MATRICES",		 "26", "")
,mWorldMatrixArray(	"float4x3",			"mWorldMatrixArray[MAX_MATRICES]", "", "")
,CurNumBones(		"int",				"CurNumBones",		 "1", "")
,mLightVP(			"float4x4",			"mLightVP",		 "", "")
{
}

C_d3d_effect_source_creator_shadow_map::~C_d3d_effect_source_creator_shadow_map()
{
}

// 初期化
void C_d3d_effect_source_creator_shadow_map::initialize(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so)
{
	// セマンティクス構造体の初期化
	conv.getVISem() = C_d3d_effect_vertex_input_semantics_struct("VS_INPUT",
		C_d3d_effect_semantics_info::POSITION |
		C_d3d_effect_semantics_info::BLENDWEIGHT |
		C_d3d_effect_semantics_info::BLENDINDICES |
		C_d3d_effect_semantics_info::NORMAL
		);
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::POSITION, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::BLENDWEIGHT, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::BLENDINDICES, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::NORMAL,   D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f));

	conv.getVOSem() = C_d3d_effect_vertex_output_semantics_struct("VS_OUTPUT",
		C_d3d_effect_semantics_info::POSITION |
		C_d3d_effect_semantics_info::TEXCOORD0
		);
	conv.getPISem() = C_d3d_effect_pixel_input_semantics_struct("PS_INPUT",
		C_d3d_effect_semantics_info::POSITION |
		C_d3d_effect_semantics_info::TEXCOORD0
		);
	conv.getPOSem() = C_d3d_effect_pixel_output_semantics_struct("PS_OUTPUT",
		C_d3d_effect_semantics_info::COLOR0
		);
}

// グローバル変数を登録
void C_d3d_effect_source_creator_shadow_map::registUserVals()
{
	// グローバル変数の登録	
	conv.regist(MAX_MATRICES);		
	conv.regist(mWorldMatrixArray);	
	conv.regist(mLightVP);
	conv.regist(CurNumBones);
}

// 頂点シェーダコード
void C_d3d_effect_source_creator_shadow_map::vertexShaderCode(MBSTR& out, C_d3d_effect_vertex_input_semantics_struct& In, C_d3d_effect_vertex_output_semantics_struct& Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so)
{
	out +=
		// 変数宣言
		"\tVS_OUTPUT Out = (VS_OUTPUT)0;\n"
		"\tfloat3 Pos = 0.0f;\n"
		"\tfloat LastWeight = 0.0f;\n"
		"\tint NumBones = $(CurNumBones);\n"
		"\tint4 IndexVector = $(In.blendIndices);\n"
		"\tfloat BlendWeightsArray[4] = (float[4])$(In.blendWeight);\n"
		"\tint   IndexArray[4]        = (int[4])IndexVector;\n"
		"\tfloat4x3 Comb = (float4x3)0;\n"

		"\tfor (int iBone = 0; (iBone < 3) && (iBone < NumBones-1); iBone++) {\n"
			"\t\tLastWeight += BlendWeightsArray[iBone]; \n"
			"\t\tComb += mWorldMatrixArray[IndexArray[iBone]] * BlendWeightsArray[iBone];\n"
		"\t}\n"
		"\tComb += mWorldMatrixArray[IndexArray[NumBones-1]] * (1.0f - LastWeight);\n"

		"\tPos = mul(float4($(In.pos).xyz, 1.0f), Comb);\n"
		"\t$(Out.pos) = mul(float4(Pos.xyz, 1.0f), mLightVP);\n"
		"\t$(Out.tex0) = $(Out.pos);\n"

		"\treturn Out;\n\n"
		;
}

// ピクセルシェーダコード
void C_d3d_effect_source_creator_shadow_map::pixelShaderCode(MBSTR& out, C_d3d_effect_pixel_input_semantics_struct& In, C_d3d_effect_pixel_output_semantics_struct& Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so)
{
	out +=
		"\tPS_OUTPUT Out = (PS_OUTPUT)0;\n"
		"\t$(Out.color0) = $(In.tex0).z / $(In.tex0).w;\n"
		"\treturn Out;\n"
		;
}









}