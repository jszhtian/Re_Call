// C_d3d_effect_source_creator_ds.h

#pragma once

#include "tonad3d3_effect_source_creator.h"
#include "tonad3d3_effect_global_texture_val.h"
#include "tonad3d3_effect_global_sampler_val.h"

namespace NT3
{

class C_d3d_effect_source_creator_ds : public C_d3d_effect_source_creator {
	C_d3d_effect_global_texture_val diffuseTexture;	// ディフューズテクスチャ
	C_d3d_effect_global_texture_val specularTexture;	// スペキュラテクスチャ
	C_d3d_effect_global_sampler_val diffuseSampler;	// ディフューズサンプラ
	C_d3d_effect_global_sampler_val specularSampler;	// スペキュラサンプラ
	C_d3d_effect_global_val lightDir;					// ライト方向
	C_d3d_effect_global_val worldMatrix, viewMatrix, projMatrix;	// WVP行列

public:
	C_d3d_effect_source_creator_ds();
	virtual ~C_d3d_effect_source_creator_ds();

	// グローバル変数を登録
	virtual void registUserVals();	

	// 頂点シェーダコード
	virtual void vertexShaderCode( std::string& out, C_d3d_effect_vertex_input_semantics_struct &In, C_d3d_effect_vertex_output_semantics_struct &Out );

	// ピクセルシェーダコード
	virtual void pixelShaderCode( std::string& out, C_d3d_effect_pixel_input_semantics_struct &In, C_d3d_effect_pixel_output_semantics_struct &Out );
};


}