#pragma once

#include "tonad3d3_effect_semantics_info.h"

namespace NT3
{

// ライティングの種類
enum LIGHTING_TYPE {
	LIGHTING_TYPE_NONE = 0,
	LIGHTING_TYPE_LAMBERT,
	LIGHTING_TYPE_BLINN_PHONG,
	LIGHTING_TYPE_PP_BLINN_PHONG,
	LIGHTING_TYPE_PP_HALF_LAMBERT,

	LIGHTING_TYPE_TOON,
	LIGHTING_TYPE_FFP,
	LIGHTING_TYPE_PP_FFP,

	LIGHTING_TYPE_BUMP,
	LIGHTING_TYPE_PARALLAX,
};

// 影の種類
enum SHADING_TYPE {
	SHADING_TYPE_NONE = 0,
	SHADING_TYPE_DEPTH_BUFFER,
};

// シェーダオプション
enum SHADER_OPTION {
	SHADER_OPTION_NONE			= 0 << 0,
	SHADER_OPTION_RIM_LIGHT		= 1 << 0,
	//SHADER_OPTION_HOGE	= 1 << 1,
};

class C_d3d_effect_lighting_and_shading_shader {
public:
	// セマンティクス情報の再構築
	static DWORD restructSemanticsBit(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so, DWORD bit);

	// 頂点シェーダコード
	static MBSTR getVertexShaderCode(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so, MBSTR varNameNormal);
	// ピクセルシェーダコード
	static MBSTR getPixelShaderCode(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so);
private:
	// 各種影ありコード
	static MBSTR getVertexShaderCodeWithShadow(LIGHTING_TYPE lt, SHADING_TYPE st, MBSTR varNameNormal);
	static MBSTR getPixelShaderCodeWithShadow(LIGHTING_TYPE lt, SHADING_TYPE st);



	// 各種シェーダコード

	// 影の条件判定分とか
	bool ShaderChecker(SHADING_TYPE st);
	// 影付けの基礎となるコード
	MBSTR ShadingBaseCodeVS(SHADING_TYPE st);
	MBSTR ShadingBaseCodePS(SHADING_TYPE st);

	// ライティングの基礎となるコード、影がどうなっても共通
	MBSTR LightingBaseCodeVS(LIGHTING_TYPE lt);
	MBSTR LightingBaseCodePS(LIGHTING_TYPE lt);


	// シェーダオプションコード
	// リムライト(頂点シェーダ計算)
	static MBSTR RimLightCalcVSVS(LIGHTING_TYPE lt, SHADING_TYPE st);
	static MBSTR RimLightCalcVSPS(LIGHTING_TYPE lt, SHADING_TYPE st);

	// リムライト(ピクセルシェーダ計算)未実装
	static MBSTR RimLightCalcPSVS(LIGHTING_TYPE lt, SHADING_TYPE st);
	static MBSTR RimLightCalcPSPS(LIGHTING_TYPE lt, SHADING_TYPE st);
};
// varNameNormalみたいなの増やす？


}