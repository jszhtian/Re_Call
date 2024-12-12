// C_d3d_effect_source_creator.cpp

#include "tonad3d3_pch.h"
#include "tonad3d3_effect_source_creator.h"
#include "tonad3d3_effect_sampler_func.h"
#include <vector>

namespace NT3
{

// ソート用
bool compare_global_val(C_d3d_effect_global_val& lhs, C_d3d_effect_global_val& rhs)
{
	if(lhs.getIndex() < rhs.getIndex()) return true;
	return false;
}

//////////////////////////
// 本文コンバータ
////
C_d3d_effect_source_creator::CodeConverter::CodeConverter()
:index_for_sort(0)
{
}

// 置き換え変数を登録
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

// 置換変数名を取得
MBSTR C_d3d_effect_source_creator::CodeConverter::find(MBSTR src)
{
	// グローバル変数？
	std::map<MBSTR, C_d3d_effect_global_val>::iterator it = globalMap.find( src );
	if ( it != globalMap.end() )
		return it->second;

	// 頂点シェーダのセマンティクス？
	MBSTR res;
	if ( viSem.find(src, res) || voSem.find(src, res) || piSem.find(src, res) || poSem.find(src, res) )
		return res;

	// 関数名？
	{
		std::map<MBSTR, C_d3d_effect_global_sampler_val>::iterator it = samplerMap.find(src);
		if( it != samplerMap.end() )
			return it->second.getFuncName();
	}

	// ない！
	return "**/not found the val./**";
}

// 頂点シェーダコンバート
MBSTR C_d3d_effect_source_creator::CodeConverter::convert(MBSTR src)
{
	MBSTR out;

	// 「$」が出てきたら置き換え
	size_t sz = src.size();
	for ( size_t i = 0; i < sz; i++ ) {
		if ( src[i] == '$' ) {
			// 置き換え名抽出
			MBSTR name;
			i+=2;
			while(i < sz && src[i] != ')')
				name += src[i++];
			// 置き換え名を変換
			out += find(name);
			continue;
		}
		out += src[i];
	}
	return out;
}

// グローバル変数を書き出し
void C_d3d_effect_source_creator::CodeConverter::globalValCode( MBSTR& out )
{
	// グローバル変数書き出し
	{
		std::map<MBSTR, C_d3d_effect_global_val>::iterator  it = globalMap.begin();
		std::map<MBSTR, C_d3d_effect_global_val>::iterator end = globalMap.end();

		// MAPはKeyでソートされているのでVECTORに入れてソートし直す
		std::vector<C_d3d_effect_global_val> temp;
		for ( ; it != end; it++ )
			temp.push_back(it->second);

		// ソートする
		std::sort(temp.begin(), temp.end(), compare_global_val);
		
		// 書き出す
		for(unsigned int i = 0; i < temp.size(); i++) {
			out += temp[i].decl();
		}

		//for ( ; it != end; it++ )
		//	out += it->second.decl();
	}

	out += "\n";

	// サンプラ書き出し
	{
		std::map<MBSTR, C_d3d_effect_global_sampler_val>::iterator  it = samplerMap.begin();
		std::map<MBSTR, C_d3d_effect_global_sampler_val>::iterator end = samplerMap.end();
		for ( ; it != end; it++ )
			out += it->second.decl();
	}
}

// 置き換えシェーダ内関数の書き出し
void C_d3d_effect_source_creator::CodeConverter::samplerFunctionsCode( MBSTR& out, C_d3d_effect_texture_info &texInfo )
{
	// サンプラ書き出し
	{
		std::map<MBSTR, C_d3d_effect_global_sampler_val>::iterator  it = samplerMap.begin();
		std::map<MBSTR, C_d3d_effect_global_sampler_val>::iterator end = samplerMap.end();
		for ( ; it != end; it++ )
			out += samplerFuncOut(texInfo, it->second);
	}
}

// サンプラー関数コード出力
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
	// まずは初期化を行う
	init(lightType, shadeType, so);

	
	// テンプレートに従ってコード文字列を作成していく
	out = "";

	registValsEntry();												// Creator内の変数をコンバータへ登録

	globalValCode(out);												// グローバル変数コード
	out += "\n";
	functionCode(out, funcInfo);									// 関数コード
	
	// 頂点シェーダ
	vertexShaderStructsCode(out);									// 頂点シェーダの構造体宣言
	vertexEntryCode(out);											// 頂点シェーダエントリ宣言部作成( VS_OUTPUT vs_main()...な部分）
	missingVertexSemanticsInitializeCode( out, semInfo, conv.getVISem() );	// 頂点シェーダの欠落セマンティクスの初期化コード挿入
	vertexShaderCode(out, conv.getVISem(), conv.getVOSem(), lightType, shadeType, so);						// 頂点シェーダコード
	vertexTerminateCode(out);										// 頂点シェーダ終端（"};"だけ）

	// テクスチャの有無によって出力コードの変更
	samplerFunctionsCode( out, texInfo );							// テクスチャサンプリング関数実装部
	
	// ピクセルシェーダ
	pixelShaderOutputStructsCode(out);								// ピクセルシェーダの構造体宣言
	pixelEntryCode(out);											// ピクセルシェーダエントリ宣言部作成( PS_OUTPUT ps_main()...な部分）
	pixelShaderCode(out, conv.getPISem(), conv.getPOSem(), lightType, shadeType, so);			// ピクセルシェーダコード
	pixelTerminateCode(out);										// ピクセルシェーダ終端（"};"だけ）

	// テクニック
	techniqueCode(out);												// テクニックコード

	convert(out);													// シェーダ内文字列を置換


	// ここまでの結果からファイル名(key)を生成する
//	createKeyFileName(semInfo, texInfo, funcInfo, lightType, shadeType);
}

// コンバータへ各変数を登録
void C_d3d_effect_source_creator::registValsEntry()
{
	registUserVals();
}
void C_d3d_effect_source_creator::registUserVals()
{
}

// グローバル変数コード
void C_d3d_effect_source_creator::globalValCode( MBSTR& out )
{
	conv.globalValCode( out );
}

// 頂点シェーダの構造体宣言
void C_d3d_effect_source_creator::vertexShaderStructsCode( MBSTR& out )
{
	out += conv.getVISem().decl();
	out += conv.getVOSem().decl();
}

// 頂点シェーダエントリ宣言部作成( VS_OUTPUT vs_main()...な部分）
void C_d3d_effect_source_creator::vertexEntryCode( MBSTR& out )
{
	out += "VS_OUTPUT vs_main( VS_INPUT In ) {\n";
}

// 頂点シェーダの欠落セマンティクスの初期化コード挿入
void C_d3d_effect_source_creator::missingVertexSemanticsInitializeCode( MBSTR& out, C_d3d_effect_semantics_info &modelSemInfo, C_d3d_effect_vertex_input_semantics_struct &In )
{
	In.missingVertexSemanticsInitializeCode(out, modelSemInfo);
	out += "\n";
}

// 頂点シェーダコード
void C_d3d_effect_source_creator::vertexShaderCode( MBSTR& out, C_d3d_effect_vertex_input_semantics_struct &In, C_d3d_effect_vertex_output_semantics_struct &Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so )
{
	out +=	"\tVS_OUTPUT Out = (VS_OUTPUT)0;\n"
			"\tOut.pos = In.pos;\n"
			"\treturn Out;\n";
}

// 頂点シェーダ終端（"};"だけ）
void C_d3d_effect_source_creator::vertexTerminateCode( MBSTR& out )
{
	out += "};\n";
	out +=	"\n";
}

// ピクセルシェーダ出力構造体宣言
void C_d3d_effect_source_creator::pixelShaderOutputStructsCode( MBSTR& out )
{
	out += conv.getPISem().decl();
	out += conv.getPOSem().decl();
}

// テクスチャサンプリング関数実装部
void C_d3d_effect_source_creator::samplerFunctionsCode( MBSTR& out, C_d3d_effect_texture_info &texInfo )
{
	conv.samplerFunctionsCode( out, texInfo );
}

// ピクセルシェーダエントリ宣言部作成( PS_OUTPUT ps_main()...な部分）
void C_d3d_effect_source_creator::pixelEntryCode( MBSTR& out )
{
	out +=	"PS_OUTPUT ps_main( VS_OUTPUT In ) {\n";
}

// ピクセルシェーダコード
void C_d3d_effect_source_creator::pixelShaderCode( MBSTR& out, C_d3d_effect_pixel_input_semantics_struct &In, C_d3d_effect_pixel_output_semantics_struct &Out, LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so )
{
	out +=	"\tPS_OUTPUT Out = (PS_OUTPUT)0;\n"
			"\tOut.color = float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
			"\treturn Out;\n";
}

// ピクセルシェーダ終端（"};"だけ）
void C_d3d_effect_source_creator::pixelTerminateCode( MBSTR& out )
{
	out +=	"}\n";
	out +=	"\n";
}

// テクニックコード
void C_d3d_effect_source_creator::techniqueCode( MBSTR& out )
{
	out +=	"technique tech {\n"
			"\tpass p0 {\n"
			"\t\tVertexShader = compile vs_2_0 vs_main();\n"
			"\t\tPixelShader  = compile ps_2_0 ps_main();\n"
			"\t}\n"
			"}\n";
}

// シェーダコードを展開
void C_d3d_effect_source_creator::convert( MBSTR& out )
{		
	out = conv.convert(out);
}


// 関数コード生成支援関数
MBSTR supportFunctionCode(C_d3d_effect_func_info& funcInfo, C_d3d_effect_func_info::TYPE type, C_d3d_effect_func_interface* func)
{
	return funcInfo.find(type) ? func->realCode() : func->initCode();
}

// 関数コード
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

// 初期化用
void C_d3d_effect_source_creator::init(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so)
{
	initialize(lightType, shadeType, so);
}

void C_d3d_effect_source_creator::initialize(LIGHTING_TYPE lightType, SHADING_TYPE shadeType, SHADER_OPTION so)
{
}
/*
// シェーダ名(key)を生成
void C_d3d_effect_source_creator::createKeyFileName(C_d3d_effect_semantics_info &semInfo, C_d3d_effect_texture_info &texInfo, C_d3d_effect_func_info &funcInfo, LIGHTING_TYPE lightType, SHADING_TYPE shadeType)
{
	// 派生クラスの関数呼び出し
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
// 生成されたシェーダ名(key)を取得
MBSTR C_d3d_effect_source_creator::getKeyFileName()
{
	return keyFileName;
}


}