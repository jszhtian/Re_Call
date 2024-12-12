#include "tonad3d3_pch.h"
#include "tonad3d3_effect_source_creator_ds.h"

namespace NT3
{

#define initFloat4x4 "float4x4(1.0, 0.0, 0.0, 0.0,  0.0, 1.0, 0.0, 0.0,  0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0)"

C_d3d_effect_source_creator_ds::C_d3d_effect_source_creator_ds() :
	diffuseTexture("DIFFUSETEXTURE"),
	specularTexture("SPECULARTEXTURE"),
	diffuseSampler("diffuseSampler", diffuseTexture, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
	specularSampler("specularSampler", specularTexture, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)),
	lightDir("float3", "lightDir", "float3(0.0f, 0.0f, 1.0f)", "LIGHTDIR"),
	worldMatrix("float4x4", "worldMatrix", initFloat4x4, "WORLDMATRIX"),
	viewMatrix("float4x4", "viewMatrix", initFloat4x4, "VIEWMATRIX"),
	projMatrix("float4x4", "projMatrix", initFloat4x4, "PROJMATRIX")
{
	// セマンティクス構造体の初期化
	conv.getVISem() = C_d3d_effect_vertex_input_semantics_struct("VS_INPUT", 
					C_d3d_effect_semantics_info::POSITION |
					C_d3d_effect_semantics_info::TEXCOORD |
					C_d3d_effect_semantics_info::NORMAL
					);
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::POSITION, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::TEXCOORD, D3DXVECTOR2(0.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::NORMAL,   D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f));

	conv.getVOSem() = C_d3d_effect_vertex_output_semantics_struct("VS_OUTPUT",
					C_d3d_effect_semantics_info::POSITION |
					C_d3d_effect_semantics_info::TEXCOORD0 |		// uv
					C_d3d_effect_semantics_info::TEXCOORD1 |		// normal
					C_d3d_effect_semantics_info::TEXCOORD2		// camera direct
					);
	conv.getPISem() = C_d3d_effect_pixel_input_semantics_struct("PS_INPUT",
					C_d3d_effect_semantics_info::POSITION |
					C_d3d_effect_semantics_info::TEXCOORD0 |		// uv
					C_d3d_effect_semantics_info::TEXCOORD1 |		// normal
					C_d3d_effect_semantics_info::TEXCOORD2		// camera direct
					);
	conv.getPOSem() = C_d3d_effect_pixel_output_semantics_struct("PS_OUTPUT",
					C_d3d_effect_semantics_info::COLOR0
					);
}

C_d3d_effect_source_creator_ds::~C_d3d_effect_source_creator_ds()
{
}

// 派生クラスの変数を登録
void C_d3d_effect_source_creator_ds::registUserVals()
{
	conv.regist(diffuseTexture);
	conv.regist(specularTexture);
	conv.regist(diffuseSampler);
	conv.regist(specularSampler);
	conv.regist(lightDir);
	conv.regist(worldMatrix);
	conv.regist(viewMatrix);
	conv.regist(projMatrix);
}

// 頂点シェーダコード
void C_d3d_effect_source_creator_ds::vertexShaderCode( std::string& out, C_d3d_effect_vertex_input_semantics_struct &In, C_d3d_effect_vertex_output_semantics_struct &Out )
{
	out +=
		"\tVS_OUTPUT Out = (VS_OUTPUT)0;\n"
		// 座標
		"\t$(Out.pos) = mul( $(In.pos), $(worldMatrix) );\n"
		"\t$(Out.pos) = mul( $(Out.pos), $(viewMatrix) );\n"
		"\t$(Out.pos) = mul( $(Out.pos), $(projMatrix) );\n"
		"\t$(Out.pos).z = 1.0f;"

		// UVはそのまま
		"\t$(Out.tex0).xy = $(In.uv);\n"

		// ライト方向をビュー空間に
		"\t$(Out.tex1).xyz = mul( $(lightDir), $(viewMatrix) );\n"

		// 法線をビュー空間に
		"\t$(Out.tex2).xyz = mul( $(In.normal), $(worldMatrix) );\n"
		"\t$(Out.tex2).xyz = mul( $(Out.tex2).xyz, $(viewMatrix) );\n"

		"\treturn Out;\n\n";
}

// ピクセルシェーダコード
void C_d3d_effect_source_creator_ds::pixelShaderCode( std::string& out, C_d3d_effect_pixel_input_semantics_struct &In, C_d3d_effect_pixel_output_semantics_struct &Out )
{
	out +=
		"\tPS_OUTPUT Out = (PS_OUTPUT)0;\n"

		// ディフューズカラー算出
		"\tfloat4 diffuseColor = abs(dot( -normalize($(In.tex1).xyz), normalize($(In.tex2).xyz) )) * $(diffuseSampler)($(In.tex0).xy);\n"

		// スペキュラカラー算出
		"\tfloat3 halfVec = 0.5f * (float3(0.0f, 0.0f, -1.0f) - normalize($(In.tex1).xyz));\n"
		"\tfloat4 specularPower = pow( abs( dot( halfVec, $(In.tex2).xyz ) ), 10.0f);\n"
		"\tfloat4 specularColor = specularPower * $(specularSampler)($(In.tex0).xy);\n"

		"\t$(Out.color0) = diffuseColor + specularColor;\n"

		"\treturn Out;\n";
}



}