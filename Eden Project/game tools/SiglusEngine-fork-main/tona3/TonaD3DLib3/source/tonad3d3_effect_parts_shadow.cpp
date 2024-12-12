#include "tonad3d3_pch.h"
#include "tonad3d3_effect_parts_shadow.h"
#include "tonad3d3_effect_source_val_name_manager.h"

namespace NT3
{

// 頂点シェーダにおける影生成コード取得
MBSTR getShadowCodeVS(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";

	if(false);
	// 影なし
	else if(st == SHADING_TYPE_NONE) {
		// 何もしない
	}
	// 深度バッファシャドウ(ShadowMapUVとDepth が必要)
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		out +=
			// シャドウマップ
			"\tfloat4 ShadowMapUV = float4("+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+", 1.0f);\n"
			"\tShadowMapUV = mul(ShadowMapUV, mLightVP);\n"
			"\tShadowMapUV = mul(ShadowMapUV, mScaleBias);\n"
			"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_SHADOW_MAP_UV, lt, st)+" = ShadowMapUV;\n"

			// 比較のための深度値
			"\tfloat4 Depth = float4("+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+", 1.0f);\n"
			"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_DEPTH, lt, st)+" = mul(Depth, mLightVP);\n"
		;
	}

	return out;
}


// ピクセルシェーダにおける影生成コード取得
MBSTR getShadowCodePS(LIGHTING_TYPE lt, SHADING_TYPE st, MBSTR shadow_color_code, MBSTR default_color_code)
{
	MBSTR out = "";

	if(false);
	// 影なし
	else if(st == SHADING_TYPE_NONE) {
		// 何もしない
	}
	// 深度バッファシャドウ(ShadowMapUVとDepth が必要)
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		out +=
			// 影関連の一時変数代入
			"\tfloat4 TexMapUV = $(In.tex0);\n"
			"\tfloat4 ShadowMapUV = "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_SHADOW_MAP_UV, lt, st)+";\n"
			"\tfloat4 Depth = "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_DEPTH, lt, st)+";\n"
			// 影
			"\tfloat  shadow = $(shadowSampler)(ShadowMapUV).x;\n"
			"\tfloat4 decale = $(meshSampler)(TexMapUV.xy);\n"
			
			"\tif(shadow * Depth.w < Depth.z - g_DBSBias) {\n"
				// 影あり
				"\t"+shadow_color_code+
			
			"\t}\n"
			"\telse {\n"
				// 影なし
				"\t"+default_color_code+
			"\t}\n"
		;
	}

	return out;
}



















}