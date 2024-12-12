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
	// 頂点シェーダやピクセルシェーダの生成支援クラス
	class CodeConverter {
		std::map<MBSTR, C_d3d_effect_global_val> globalMap;
		std::map<MBSTR, C_d3d_effect_global_sampler_val> samplerMap;
		C_d3d_effect_vertex_input_semantics_struct viSem;
		C_d3d_effect_vertex_output_semantics_struct voSem;
		C_d3d_effect_pixel_input_semantics_struct piSem;
		C_d3d_effect_pixel_output_semantics_struct poSem;

		int index_for_sort;

		// サンプラー関数コード出力
		MBSTR samplerFuncOut( C_d3d_effect_texture_info &texInfo, C_d3d_effect_global_sampler_val &sampler );

	public:
		CodeConverter();

		// 頂点シェーダ入力セマンティクス実体取得
		C_d3d_effect_vertex_input_semantics_struct &getVISem() { return viSem; }

		// 頂点シェーダ出力セマンティクス実体取得
		C_d3d_effect_vertex_output_semantics_struct &getVOSem() { return voSem; }

		// ピクセルシェーダ入力セマンティクス実体取得
		C_d3d_effect_pixel_input_semantics_struct &getPISem() { return piSem; }

		// ピクセルシェーダ出力セマンティクス実体取得
		C_d3d_effect_pixel_output_semantics_struct &getPOSem() { return poSem; }

		// 置き換え変数を登録
		void regist(C_d3d_effect_global_val &val);
		void regist(C_d3d_effect_global_sampler_val &sampler);

		// 頂点シェーダコンバート
		MBSTR convert(MBSTR src);

		// 置換変数名を取得
		MBSTR find(MBSTR src);

		// グローバル変数を書き出し
		void globalValCode( MBSTR& out );

		// 置き換えシェーダ内関数の書き出し
		void samplerFunctionsCode( MBSTR& out, C_d3d_effect_texture_info &texInfo );
	};

protected:
	CodeConverter conv;						// 本文シェーダコンバータ


	MBSTR	keyFileName;				// 生成されたシェーダ名(key)

protected:
	// コンバータへ各変数を登録
	void registValsEntry();
	virtual void registUserVals();	// 派生クラスのグローバル変数を登録します

	// グローバル変数コード
	void globalValCode( MBSTR& out );

	// 頂点シェーダの構造体宣言
	void vertexShaderStructsCode( MBSTR& out );

	// 頂点シェーダエントリ宣言部作成( VS_OUTPUT vs_main()...な部分）
	void vertexEntryCode( MBSTR& out );

	// 頂点シェーダの欠落セマンティクスの初期化コード挿入
	void missingVertexSemanticsInitializeCode( MBSTR& out, C_d3d_effect_semantics_info &modelSemInfo, C_d3d_effect_vertex_input_semantics_struct &In );

	// 頂点シェーダコード
	virtual void vertexShaderCode( MBSTR& out, C_d3d_effect_vertex_input_semantics_struct &In, C_d3d_effect_vertex_output_semantics_struct &Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so );

	// 頂点シェーダ終端（"};"だけ）
	void vertexTerminateCode( MBSTR& out );

	// ピクセルシェーダ出力構造体宣言
	void pixelShaderOutputStructsCode( MBSTR& out );

	// テクスチャサンプリング関数実装部
	void samplerFunctionsCode(  MBSTR& out, C_d3d_effect_texture_info &texInfo );

	// ピクセルシェーダエントリ宣言部作成( PS_OUTPUT ps_main()...な部分）
	void pixelEntryCode( MBSTR& out );

	// ピクセルシェーダコード
	virtual void pixelShaderCode( MBSTR& out, C_d3d_effect_pixel_input_semantics_struct &In, C_d3d_effect_pixel_output_semantics_struct &Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so );

	// ピクセルシェーダ終端（"};"だけ）
	void pixelTerminateCode( MBSTR& out );

	// テクニックコード
	void techniqueCode( MBSTR& out );

	// コードをコンバート
	void convert( MBSTR& out );

	// 関数コード
	virtual void functionCode( MBSTR& out, C_d3d_effect_func_info &funcInfo );

	// 初期化用
	void init(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so);
	virtual void initialize(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so);

	// シェーダ名(key)を生成
	/*
	void createKeyFileName(C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo, C_d3d_effect_func_info &funcInfo, LIGHTING_TYPE lightType, SHADING_TYPE shadeType);
	virtual void createKeyFileName(C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo) = 0;
	*/

public:
	C_d3d_effect_source_creator();
	~C_d3d_effect_source_creator();

	// シェーダ生成
	void createShader( MBSTR& out, C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo, C_d3d_effect_func_info &funcInfo, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so );

	// 生成されたシェーダ名(key)を取得
	MBSTR getKeyFileName();
};


}