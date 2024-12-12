#pragma once

#include "tonad3d3_effect_semantics_info.h"
#include "tonad3d3_effect_lighting_and_shading_shader.h"

namespace NT3
{

class C_d3d_effect_lighting_shader {
public:

	// 頂点シェーダコード
	static MBSTR getVertexShaderCode(LIGHTING_TYPE type, MBSTR varNameNormal);
	// ピクセルシェーダコード
	static MBSTR getPixelShaderCode(LIGHTING_TYPE type);

private:
	// 各種シェーダコード

	// ランバート
	static MBSTR LambertVS(MBSTR varNameNormal);
	static MBSTR LambertPS();
	
	// ブリンフォン
	static MBSTR BlinnPhongVS(MBSTR varNameNormal);
	static MBSTR BlinnPhongPS();
	
	// ブリンフォン(パーピクセル)
	static MBSTR PerPixelBlinnPhongVS(MBSTR varNameNormal);
	static MBSTR PerPixelBlinnPhongPS();

	// ハーフランバート(パーピクセル)
	static MBSTR PerPixelHalfLambertVS(MBSTR varNameNormal);
	static MBSTR PerPixelHalfLambertPS();

	// フロントウイング仕様のトゥーンっぽいもの
	static MBSTR ToonVS(MBSTR varNameNormal);
	static MBSTR ToonPS();

	// 固定機能パイプラインを再現した物
	static MBSTR FixedFunctionPipelineVS(MBSTR varNameNormal);
	static MBSTR FixedFunctionPipelinePS();

	// 固定機能パイプラインを再現した物(パーピクセル)
	static MBSTR PerPixelFixedFunctionPipelineVS(MBSTR varNameNormal);
	static MBSTR PerPixelFixedFunctionPipelinePS();

	// バンプマッピング
	static MBSTR BumpMappingVS(MBSTR varNameNormal);
	static MBSTR BumpMappingPS();

	// 視差マッピング
	static MBSTR ParallaxMappingVS(MBSTR varNameNormal);
	static MBSTR ParallaxMappingPS();
};


}