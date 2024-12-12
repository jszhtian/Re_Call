#include "tonad3d3_pch.h"
#include "tonad3d3_effect_source_creator_skinned_mesh.h"
#include "tonad3d3_effect_lighting_and_shading_shader.h"
#include "tonad3d3_effect_source_val_name_manager.h"

namespace NT3
{

C_d3d_effect_source_creator_skinned_mesh::C_d3d_effect_source_creator_skinned_mesh()
:MAX_MATRICES(		"static const int",	"MAX_MATRICES",		 "26", "")
,mWorldMatrixArray(	"float4x3",			"mWorldMatrixArray[MAX_MATRICES]", "", "")
,mView(			"float4x4",			"mView",		 "", "")
,mProj(			"float4x4",			"mProj",		 "", "")
,mWorld(			"float4x4",			"mWorld",		 "", "")
,mLocal(			"float4x4",			"mLocal",		 "", "")
,mLightVP(			"float4x4",			"mLightVP",		 "", "")
,mLightWVP(			"float4x4",			"mScaleBias",		 "", "")
,CurNumBones(		"int",				"CurNumBones",		 "1", "")
,meshTexture("MESHTEXTURE")
,meshSampler("meshSampler", meshTexture, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), "Wrap", "Wrap", false )
,shadowTexture("SHADOWTEXTURE")
,shadowSampler("shadowSampler", shadowTexture, D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
			   //"Clamp", "Clamp", true )
			   //"Clamp", "Clamp", "Linear", "Linear", "None", "0x00000000", true )
			   "Border", "Border", "Point", "Point", "Linear", "0xFFFFFFFF", true )
,toonTexture("TOONTEXTURE")
,toonSampler("toonSampler", toonTexture, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), "Clamp", "Clamp", "Linear", "Linear", "Linear", "0x00000000", false )
,normalTexture("NORMALTEXTURE")
,normalSampler("normalSampler", normalTexture, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), "Clamp", "Clamp", "Linear", "Linear", "None", "0x00000000", false )
{
	keyFileName += "skn";
}

C_d3d_effect_source_creator_skinned_mesh::~C_d3d_effect_source_creator_skinned_mesh()
{
}

void C_d3d_effect_source_creator_skinned_mesh::initialize(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so)
{
	// セマンティクス構造体の初期化
	conv.getVISem() = C_d3d_effect_vertex_input_semantics_struct("VS_INPUT",
		C_d3d_effect_semantics_info::POSITION |
		C_d3d_effect_semantics_info::BLENDWEIGHT |
		C_d3d_effect_semantics_info::BLENDINDICES |
		C_d3d_effect_semantics_info::NORMAL |
		C_d3d_effect_semantics_info::TEXCOORD |
		C_d3d_effect_semantics_info::TANGENT |
		C_d3d_effect_semantics_info::BINORMAL |
		C_d3d_effect_semantics_info::COLOR
		);
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::POSITION, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::BLENDWEIGHT, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::BLENDINDICES, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::NORMAL,   D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::TEXCOORD, D3DXVECTOR2(0.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::TANGENT,   D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::BINORMAL,   D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f));
	conv.getVISem().setInitialVal(C_d3d_effect_semantics_info::COLOR,   D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

	conv.getVOSem() = C_d3d_effect_vertex_output_semantics_struct("VS_OUTPUT",
		//C_d3d_effect_lighting_shader::restructSemanticsBit(lightType, 
		C_d3d_effect_lighting_and_shading_shader::restructSemanticsBit(lightType, shadeType, so,
			C_d3d_effect_semantics_info::POSITION |
			C_d3d_effect_semantics_info::COLOR0 |
			C_d3d_effect_semantics_info::COLOR1 |
			C_d3d_effect_semantics_info::TEXCOORD
			)
		);
	conv.getPISem() = C_d3d_effect_pixel_input_semantics_struct("PS_INPUT",
		//C_d3d_effect_lighting_shader::restructSemanticsBit(lightType, 
		C_d3d_effect_lighting_and_shading_shader::restructSemanticsBit(lightType, shadeType, so,
			C_d3d_effect_semantics_info::POSITION |
			C_d3d_effect_semantics_info::COLOR0 |
			C_d3d_effect_semantics_info::COLOR1 |
			C_d3d_effect_semantics_info::TEXCOORD
			)
		);
	conv.getPOSem() = C_d3d_effect_pixel_output_semantics_struct("PS_OUTPUT",
		C_d3d_effect_semantics_info::COLOR0
		);

}

// 派生クラスの変数を登録
void C_d3d_effect_source_creator_skinned_mesh::registUserVals()
{
	// グローバル変数パックの登録
	std::vector<C_d3d_effect_global_val>::iterator it;
	std::vector<C_d3d_effect_global_val>::iterator begin = globalValPack.begin();
	std::vector<C_d3d_effect_global_val>::iterator end = globalValPack.end();
	for(it = begin; it != end; ++it) {
		conv.regist(*it);
	}

	// グローバル変数の登録	
	conv.regist(MAX_MATRICES);		
	conv.regist(mWorldMatrixArray);	
	conv.regist(mWorld);
	conv.regist(mLocal);
	conv.regist(mView);
	conv.regist(mProj);
	conv.regist(mLightVP);
	conv.regist(mLightWVP);
	conv.regist(CurNumBones);

	// テクスチャ、サンプラの登録
	conv.regist(meshTexture);
	conv.regist(meshSampler);

	conv.regist(shadowTexture);
	conv.regist(shadowSampler);

	conv.regist(toonTexture);
	conv.regist(toonSampler);

	conv.regist(normalTexture);
	conv.regist(normalSampler);
}

// 頂点シェーダコード
void C_d3d_effect_source_creator_skinned_mesh::vertexShaderCode(MBSTR& out, C_d3d_effect_vertex_input_semantics_struct& In, C_d3d_effect_vertex_output_semantics_struct& Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so)
{
	out +=
		// 変数宣言
		"\tVS_OUTPUT Out = (VS_OUTPUT)0;\n"
		"\tfloat3 "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+" = 0.0f;\n"
		"\tfloat3 "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_NORMAL)+" = 0.0f;\n"
		"\tfloat LastWeight = 0.0f;\n"
		"\tint NumBones = $(CurNumBones);\n"
		//"\tfloat3 LightDir = 0.0f;\n"

		// Compensate for lack of UBYTE4 on Geforce3
		"\tint4 IndexVector = ($(In.blendIndices));\n"
		// cast the vectors to arrays for use in the for loop below
		"\tfloat BlendWeightsArray[4] = (float[4])$(In.blendWeight);\n"
		"\tint   IndexArray[4]        = (int[4])IndexVector;\n"		
		
		//// calculate the pos/normal using the "normal" weights 
		////        and accumulate the weights to calculate the last weight
		//"\tfor (int iBone = 0; iBone < NumBones-1; iBone++) {\n"
		//"\t	LastWeight = LastWeight + BlendWeightsArray[iBone]; \n"
		//"\t	Pos += mul( $(In.pos), mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];\n"
		//"\t	Normal += mul( $(In.normal), mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];\n"
		////"\t	LightDir += mul( g_LightLocalDir, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];\n"
		//"\t}\n"
		//"\tLastWeight = 1.0f - LastWeight;\n"

		//// Now that we have the calculated weight, add in the final influence
		//"\tPos += (mul( $(In.pos), mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight);\n"
		//"\tNormal += (mul( $(In.normal), mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight); \n"
		////"\tLightDir += mul( g_LightLocalDir, mWorldMatrixArray[NumBones-1]) * LastWeight;\n"



		"\tfloat4x3 Comb = (float4x3)0;\n"
		// calculate the pos/normal using the "normal" weights 
		//        and accumulate the weights to calculate the last weight
		//"\tfor (int iBone = 0; iBone < NumBones-1; iBone++) {\n"
		"\tfor (int iBone = 0; (iBone < 3) && (iBone < NumBones-1); iBone++) {\n"
			"\t\tLastWeight += BlendWeightsArray[iBone]; \n"
			"\t\tComb += mWorldMatrixArray[IndexArray[iBone]] * BlendWeightsArray[iBone];\n"
		"\t}\n"
		"\tComb += mWorldMatrixArray[IndexArray[NumBones-1]] * (1.0f - LastWeight);\n"
		
		// Now that we have the calculated weight, add in the final influence
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+" = mul(float4($(In.pos).xyz, 1.0f), Comb);\n"
		//"\tNormal = mul(float4($(In.normal).xyz, 1.0f), Comb);\n"
		//"\tNormal = mul(float4($(In.normal).xyz, 1.0f), transpose(inverse(Comb)));\n"
		//"\tNormal = mul(float4($(In.normal).xyz, 1.0f), Comb-mWorld);\n"
		
		// 平行移動部分をカット
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_NORMAL)+".xyz = mul($(In.normal).xyz, (float3x3)Comb);\n"

		//"\tPos = mul(float4($(In.pos).xyz, 1.0f), mWorld);\n"
		//"\tNormal = mul(float4($(In.normal).xyz, 1.0f), mWorld);\n"


		// transform position from world space into view and then projection space
		"\t$(Out.pos) = mul(float4("+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz, 1.0f), mView);\n"
		"\t$(Out.pos) = mul($(Out.pos), mProj);\n"

		// normalize normals
		// 拡大縮小している可能性があるので正規化
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_NORMAL)+".xyz = normalize("+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_NORMAL)+".xyz);\n"
		//"\tNormal.xyz = normalize(In.normal.xyz);\n"
		//"\tNormal = normalize(mul(float4($(In.normal).xyz, 1.0f), mWorld));\n"

		// デフォルトカラー(頂点カラー なければ白1.0)
		"\t$(Out.color0) = $(In.color);\n"

		// ライティング用コード
		//+ C_d3d_effect_lighting_shader::getVertexShaderCode(lightType, "Normal") +
		+ C_d3d_effect_lighting_and_shading_shader::getVertexShaderCode(lightType, shadeType, so, get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_NORMAL)) +

		//// Shade (Ambient + etc.)
		//"\t$(Out.color).xyz = MaterialAmbient.xyz + Diffuse(Normal) * MaterialDiffuse.xyz;\n"
		////"\t$(Out.color).xyz = MaterialAmbient.xyz + MaterialDiffuse.xyz;\n"
		//"\t$(Out.color).w = 1.0f;\n"

		// copy the input texture coordinate through
		"\t$(Out.tex0).xy  = $(In.uv);\n"
		"\t$(Out.color1)  = $(In.color);\n"

		"\treturn Out;\n\n"

		;
}

// ピクセルシェーダコード
void C_d3d_effect_source_creator_skinned_mesh::pixelShaderCode(MBSTR& out, C_d3d_effect_pixel_input_semantics_struct& In, C_d3d_effect_pixel_output_semantics_struct& Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so)
{
	out += 
		"\tPS_OUTPUT Out = (PS_OUTPUT)0;\n"
		"\t$(Out.color0) =  $(In.color0) * $(meshSampler)($(In.tex0).xy);\n"

		// ライティング用コード
		//+ C_d3d_effect_lighting_shader::getPixelShaderCode(lightType) +
		+ C_d3d_effect_lighting_and_shading_shader::getPixelShaderCode(lightType, shadeType, so) +

		"\t$(Out.color0) =  VertexColor($(Out.color0), $(In.color1));\n"
		"\t$(Out.color0) =  MRBDColor($(Out.color0));\n"
		"\t$(Out.color0) =  RGBColor($(Out.color0));\n"
		//"\t$(Out.color0) *=  $(In.color);\n"
		"\treturn Out;\n"
		;
}

// 関数コード
void C_d3d_effect_source_creator_skinned_mesh::functionCode( MBSTR& out, C_d3d_effect_func_info &funcInfo )
{
	C_d3d_effect_source_creator::functionCode(out, funcInfo);
}
/*
// シェーダ名(key)を生成
void C_d3d_effect_source_creator_skinned_mesh::createKeyFileName(C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo)
{
	//if(semInfo.find(C_d3d_effect_semantics_info::POSITION)) keyFileName += "";
	if(semInfo.find(C_d3d_effect_semantics_info::POSITION))		keyFileName += "_pos";
	if(semInfo.find(C_d3d_effect_semantics_info::BLENDWEIGHT))	keyFileName += "_bw";
	if(semInfo.find(C_d3d_effect_semantics_info::BLENDINDICES))	keyFileName += "_bi";
	if(semInfo.find(C_d3d_effect_semantics_info::NORMAL))			keyFileName += "_n";
	if(semInfo.find(C_d3d_effect_semantics_info::TEXCOORD))		keyFileName += "_tc";

	if(texInfo.find("MESHTEXTURE"))					keyFileName += "_mt";
}
*/

}