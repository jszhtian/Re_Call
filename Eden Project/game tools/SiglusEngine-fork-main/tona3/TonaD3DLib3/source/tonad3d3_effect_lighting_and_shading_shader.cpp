#include "tonad3d3_pch.h"
#include "tonad3d3_effect_lighting_and_shading_shader.h"
#include "tonad3d3_effect_lighting_shader.h"
#include "tonad3d3_effect_source_val_name_manager.h"

#include "tonad3d3_effect_parts_shadow.h"

namespace NT3
{


// セマンティクス情報の再構築
DWORD C_d3d_effect_lighting_and_shading_shader::restructSemanticsBit(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so, DWORD bit)
{
	// 足りないセマンティクスを追加する
	if(false);

	// 影なし
	else if(st == SHADING_TYPE_NONE) {
		// ライティングの種類で分岐
		switch(lt) {
			case LIGHTING_TYPE_NONE:
				break;
			case LIGHTING_TYPE_LAMBERT:
				if(so & SHADER_OPTION_RIM_LIGHT)
					bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// RimLightColor
				break;
			case LIGHTING_TYPE_BLINN_PHONG:
				bit |= C_d3d_effect_semantics_info::TEXCOORD6;	// Diffuse
				bit |= C_d3d_effect_semantics_info::TEXCOORD7;	// Specular
				if(so & SHADER_OPTION_RIM_LIGHT)
					bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// RimLightColor
				break;
			case LIGHTING_TYPE_PP_BLINN_PHONG:
				bit |= C_d3d_effect_semantics_info::TEXCOORD4;	// Normal
				bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// Eye
				bit |= C_d3d_effect_semantics_info::TEXCOORD6;	// Diffuse

				if(so & SHADER_OPTION_RIM_LIGHT)
					bit |= C_d3d_effect_semantics_info::TEXCOORD7;	// RimLightColor

				break;
			case LIGHTING_TYPE_PP_HALF_LAMBERT:
				bit |= C_d3d_effect_semantics_info::TEXCOORD4;	// Normal
				if(so & SHADER_OPTION_RIM_LIGHT)
					bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// RimLightColor
				break;
			case LIGHTING_TYPE_TOON:
				//bit |= C_d3d_effect_semantics_info::TEXCOORD4;	// ToonTexCoord
				bit |= C_d3d_effect_semantics_info::TEXCOORD4;	// Normal
				if(so & SHADER_OPTION_RIM_LIGHT)
					bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// RimLightColor
				break;
			case LIGHTING_TYPE_FFP:
				bit |= C_d3d_effect_semantics_info::TEXCOORD6;	// Diffuse
				bit |= C_d3d_effect_semantics_info::TEXCOORD7;	// Specular
				break;
			case LIGHTING_TYPE_PP_FFP:
				bit |= C_d3d_effect_semantics_info::TEXCOORD6;	// Normal
				bit |= C_d3d_effect_semantics_info::TEXCOORD7;	// Pos
				break;

			case LIGHTING_TYPE_BUMP:
				bit |= C_d3d_effect_semantics_info::TEXCOORD4;	// WorldSpaceNormal
				bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// WorldSpaceTangent
				bit |= C_d3d_effect_semantics_info::TEXCOORD6;	// WorldSpaceBinormal
				bit |= C_d3d_effect_semantics_info::TEXCOORD7;	// WorldSpaceEye
				break;
			case LIGHTING_TYPE_PARALLAX:
				bit |= C_d3d_effect_semantics_info::TEXCOORD4;	// WorldSpaceNormal
				bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// WorldSpaceTangent
				bit |= C_d3d_effect_semantics_info::TEXCOORD6;	// WorldSpaceBinormal
				bit |= C_d3d_effect_semantics_info::TEXCOORD7;	// WorldSpaceEye
				break;
			default:
				break;
		}
	}

	// 深度バッファシャドウ
	// ShadowMapUV と Depth が必要
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		// ライティングの種類で分岐
		switch(lt) {
			case LIGHTING_TYPE_NONE:
				bit |= C_d3d_effect_semantics_info::TEXCOORD2;	// shadow map uv
				bit |= C_d3d_effect_semantics_info::TEXCOORD3;	// depth
				break;
			case LIGHTING_TYPE_LAMBERT:
				bit |= C_d3d_effect_semantics_info::TEXCOORD2;	// shadow map uv
				bit |= C_d3d_effect_semantics_info::TEXCOORD3;	// depth
				bit |= C_d3d_effect_semantics_info::TEXCOORD7;	// Ambient
				if(so & SHADER_OPTION_RIM_LIGHT)
					bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// RimLightColor
				break;
			case LIGHTING_TYPE_BLINN_PHONG:
				bit |= C_d3d_effect_semantics_info::TEXCOORD2;	// shadow map uv
				bit |= C_d3d_effect_semantics_info::TEXCOORD3;	// depth
				bit |= C_d3d_effect_semantics_info::TEXCOORD4;	// Ambient

				bit |= C_d3d_effect_semantics_info::TEXCOORD6;	// Diffuse
				bit |= C_d3d_effect_semantics_info::TEXCOORD7;	// Specular
				if(so & SHADER_OPTION_RIM_LIGHT)
					bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// RimLightColor
				break;
			case LIGHTING_TYPE_PP_BLINN_PHONG:
				bit |= C_d3d_effect_semantics_info::TEXCOORD2;	// shadow map uv
				bit |= C_d3d_effect_semantics_info::TEXCOORD3;	// depth
				bit |= C_d3d_effect_semantics_info::TEXCOORD7;	// Ambient

				bit |= C_d3d_effect_semantics_info::TEXCOORD4;	// Normal
				bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// Eye
				bit |= C_d3d_effect_semantics_info::TEXCOORD6;	// Diffuse
				if(so & SHADER_OPTION_RIM_LIGHT)
					bit |= C_d3d_effect_semantics_info::TEXCOORD1;	// RimLightColor
				break;
			case LIGHTING_TYPE_PP_HALF_LAMBERT:
				bit |= C_d3d_effect_semantics_info::TEXCOORD2;	// shadow map uv
				bit |= C_d3d_effect_semantics_info::TEXCOORD3;	// depth

				bit |= C_d3d_effect_semantics_info::TEXCOORD4;	// Normal
				if(so & SHADER_OPTION_RIM_LIGHT)
					bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// RimLightColor
				break;
			case LIGHTING_TYPE_TOON:
				bit |= C_d3d_effect_semantics_info::TEXCOORD2;	// shadow map uv
				bit |= C_d3d_effect_semantics_info::TEXCOORD3;	// depth

				bit |= C_d3d_effect_semantics_info::TEXCOORD4;	// Normal
				if(so & SHADER_OPTION_RIM_LIGHT)
					bit |= C_d3d_effect_semantics_info::TEXCOORD5;	// RimLightColor
				break;
			default:
				break;
		}
	}

	return bit;
}

// 頂点シェーダコード
MBSTR C_d3d_effect_lighting_and_shading_shader::getVertexShaderCode(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so, MBSTR varNameNormal)
{
	MBSTR out = "";

	// ここは影の種類、有無に関わらず必ず通る
	//out += C_d3d_effect_lighting_shader::getVertexShaderCode(C_d3d_effect_lighting_shader::TYPE(lt), varNameNormal);
	// やっぱめんどうだけどもう1回こっちに書く

	// 影なしなら普通に
	if(st == SHADING_TYPE_NONE) {
		out += C_d3d_effect_lighting_shader::getVertexShaderCode(lt, varNameNormal);
	}
	// 影ありなら
	else {
		out += getVertexShaderCodeWithShadow(lt, st, varNameNormal);
	}
	
	if(so & SHADER_OPTION_RIM_LIGHT && lt != LIGHTING_TYPE_NONE)
		out += RimLightCalcVSVS(lt, st);


	return out;
}

// ピクセルシェーダコード
MBSTR C_d3d_effect_lighting_and_shading_shader::getPixelShaderCode(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so)
{
	MBSTR out = "";

	// 影なしなら何も気にせずに
	if(st == SHADING_TYPE_NONE) {
		// C_d3d_effect_lighting_shaderのコードをそのまま取得
		out += C_d3d_effect_lighting_shader::getPixelShaderCode(lt);
	}
	// 影ありなら
	else {
		// 各種影ありライティングコード生成関数を呼び出す

		out += getPixelShaderCodeWithShadow(lt, st);

		// if(shadowCheck(st)) {
		//		影色？影色＋ライティング色？
		// }
		// else {
		//		各種ライティング
		// }
	}

	if(so & SHADER_OPTION_RIM_LIGHT && lt != LIGHTING_TYPE_NONE)
		out += RimLightCalcVSPS(lt, st);

	return out;
}


// ここから実装
// 各種影ありライティングコード(VS)
MBSTR C_d3d_effect_lighting_and_shading_shader::getVertexShaderCodeWithShadow(LIGHTING_TYPE lt, SHADING_TYPE st, MBSTR varNameNormal)
{
	MBSTR out = "";

	// まず影の種類で分岐する
	if(false);
	// 影なし
	else if(st == SHADING_TYPE_NONE) {
		// 影があるか？で分岐しているのでここは絶対に通らない
	}
	// 深度バッファシャドウ
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		// ここで各種ライティングコードに分岐する
		switch(lt) {
			case LIGHTING_TYPE_NONE:
				out +=
					""
					;
				break;
			case LIGHTING_TYPE_LAMBERT:
				out +=
					// 頂点カラー
					"\tfloat4 L = -g_LightWorldDir;\n"
					// 環境光 * 拡散反射光
					"\tfloat4 diffuse = g_LightDiffuse * g_MaterialDiffuse * max(0, dot("+varNameNormal+", L));\n"
					"\tfloat4 ambient = g_LightAmbient * g_MaterialAmbient;\n"
					"\t$(Out.color0) = diffuse;\n"
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_AMBIENT, lt, st)+" = ambient;\n"
					//"\t$(Out.color0) = g_LightAmbient * g_MaterialAmbient + g_LightDiffuse * g_MaterialDiffuse * max(0, dot("+varNameNormal+", L));\n"
					;
				break;
			case LIGHTING_TYPE_BLINN_PHONG:
				out +=
					"\tfloat3 N = "+varNameNormal+";\n"
					// ローカル座標系でのライトベクトル
					"\tfloat3 L = -g_LightWorldDir;\n"
					"\tfloat3 E = normalize(g_CameraLocalPos - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz);\n"
					// ハーフベクトル
					"\tfloat3 H = normalize(L+E);\n"
					
					"\tfloat4 diffuse = g_LightDiffuse * g_MaterialDiffuse * max(0, dot(N, L));\n"
					"\tfloat4 specular = pow(max(0,dot(N, H)), g_MaterialSpecularPower);\n"
					"\tfloat4 ambient = g_LightAmbient * g_MaterialAmbient;\n"
					
					"\t$(Out.color0) = diffuse + specular;\n"

					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_DIFFUSE, lt, st)+" = diffuse;\n"
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_SPECULAR, lt, st)+" = specular;\n"
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_AMBIENT, lt, st)+" = ambient;\n"
					;
				break;
			case LIGHTING_TYPE_PP_BLINN_PHONG:
				out +=
					"\tfloat3 N = "+varNameNormal+";\n"
					"\tfloat3 L = -g_LightWorldDir;\n"
					// ラスタライザで線形補間(？)されるので正規化はピクセルシェーダで
					"\tfloat3 E = g_CameraWorldPos - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz;\n"

					"\tfloat4 diffuse = g_LightDiffuse * g_MaterialDiffuse * max(0, dot(normalize(N), normalize(L)));\n"
					"\tfloat4 ambient = g_LightAmbient * g_MaterialAmbient;\n"

					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = N;\n"
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_EYE, lt, st)+".xyz = E;\n"
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_DIFFUSE, lt, st)+" = diffuse;\n"

					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_AMBIENT, lt, st)+" = ambient;\n"

					"\t$(Out.color0) = diffuse;\n"
					/*
					// シャドウマップ
					"\tfloat3 TempPos = mul(float4(Pos.xyz, 1.0f), Comb);\n"

					"\tfloat3 TempPos2 = mul(float4(Pos.xyz, 1.0f), Comb);\n"
					"\tTempPos2 = mul(TempPos2, mLightVP);\n"
					"\tTempPos2 = mul(TempPos2, mLightWVP);\n"
					

					"\t$(Out.tex2).xyz = TempPos2;\n"
					//"\t$(Out.tex2) = mul(Pos, mLightWVP);\n"
					// 比較のための深度値
					"\t$(Out.tex3) = mul(TempPos, mLightVP);\n"
					//"\t$(Out.tex3) = mul(Pos, mLightVP);\n"
					*/
					;
				break;
			case LIGHTING_TYPE_PP_HALF_LAMBERT:
				out +=
					// ハーフランバート用
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = "+varNameNormal+";\n"
					;
				break;
			case LIGHTING_TYPE_TOON:
				out +=
					// トゥーン用法線
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = "+varNameNormal+";\n"
					;
				break;
			default:
				break;
		}

		// 深度バッファシャドウ共通
		out += getShadowCodeVS(lt, st);
	}

	return out;
}

// 各種影ありライティングコード(PS)
MBSTR C_d3d_effect_lighting_and_shading_shader::getPixelShaderCodeWithShadow(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";

	// まず影の種類で分岐する
	if(false);
	// 影なし
	else if(st == SHADING_TYPE_NONE) {
		// 影があるか？で分岐しているのでここは絶対に通らない
	}
	// 深度バッファシャドウ
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		MBSTR shadow_color_code = "";
		MBSTR default_color_code = "";
		MBSTR final_proc_code = "";

		// ここで各種ライティングコードに分岐する
		switch(lt) {
			case LIGHTING_TYPE_NONE:
				// ライティング用算出
				out += 
					""
					;
				// 影あり
				shadow_color_code +=
					"\t$(Out.color0) = float4(0.0f, 0.0f, 0.0f, 1.0f);\n"
					;					
				// 影なし
				default_color_code +=
					"\t$(Out.color0) = float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
					;
				// 最終処理
				final_proc_code +=
					""
					;
				
				break;
			case LIGHTING_TYPE_LAMBERT:
				// ライティング用算出
				out += 
					//"\tfloat4 dif_tex = $(In.color0) * $(meshSampler)(TexMapUV.xy);\n"
					"\tfloat4 dif_tex = $(meshSampler)($(In.tex0).xy);\n"
					;
				// 影あり
				shadow_color_code +=
					"\tdif_tex.rgb *= "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_AMBIENT, lt, st)+".rgb;\n"
					"\t$(Out.color0).rgb = dif_tex.rgb;\n"
					;					
				// 影なし
				default_color_code +=
					//"\t\t$(Out.color0) = $(In.color0) * $(meshSampler)($(In.tex0).xy);\n"
					"\t$(Out.color0) = ($(In.color0)+$(In.tex7)) * $(meshSampler)($(In.tex0).xy);\n"
					;
				// 最終処理
				final_proc_code +=
					// とりあえず1.0fを入れておく/*要修正*/
					"\t$(Out.color0).a = 1.0f;\n"
					;

				break;
			case LIGHTING_TYPE_BLINN_PHONG:
				// ライティング用算出
				out += 
					//"\tfloat4 dif_tex = $(In.color0) * $(meshSampler)(TexMapUV.xy);\n"
					"\tfloat4 dif_tex = $(meshSampler)($(In.tex0).xy);\n"
					;
				// 影あり
				shadow_color_code +=
					"\tdecale.rgb *= "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_AMBIENT, lt, st)+".rgb;\n"
					"\t$(Out.color0).rgb = decale.rgb;\n"
					;					
				// 影なし
				default_color_code +=
					"\t$(Out.color0) = ("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_DIFFUSE, lt, st)+" + "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_AMBIENT, lt, st)+") * $(meshSampler)($(In.tex0).xy) + "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_SPECULAR, lt, st)+";\n"
					;
				// 最終処理
				final_proc_code +=
					// とりあえず1.0fを入れておく/*要修正*/
					"\t$(Out.color0).a = 1.0f;\n"
					;

				break;
			case LIGHTING_TYPE_PP_BLINN_PHONG:
				// ライティング用算出
				out +=
					"\tfloat3 L = -g_LightWorldDir;\n"
					"\tfloat3 N = normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+".xyz);\n"
					"\tfloat3 H = normalize(L + normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_EYE, lt, st)+".xyz));\n"
					"\tfloat4 specular = pow(max(0,dot(N, H)), g_MaterialSpecularPower);\n"
					;
				// 影あり
				shadow_color_code +=
					"\tdecale.rgb *= "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_AMBIENT, lt, st)+".rgb;\n"
					"\t$(Out.color0).rgb = decale.rgb;\n"
					;
				// 影なし
				default_color_code +=
					"\tdecale.rgb *= "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_AMBIENT, lt, st)+".rgb + "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_DIFFUSE, lt, st)+".rgb;\n"
					"\t$(Out.color0).rgb = decale.rgb + specular.rgb;\n"
					;
				// 最終処理
				final_proc_code +=
					// とりあえず1.0fを入れておく/*要修正*/
					"\t$(Out.color0).a = 1.0f;\n"
					;

				break;
			case LIGHTING_TYPE_PP_HALF_LAMBERT:
				// ライティング用算出
				out +=
					""
					;
				// 影あり
				shadow_color_code +=
					"\t\tfloat p = g_LightAmbient * g_MaterialAmbient;\n"
					"\tp = p * 0.5f + 0.5f;\n"
					"\tp = p * p;\n"
					"\t$(Out.color0) = p * $(meshSampler)($(In.tex0).xy);\n"
					//"\t\tdecale.rgb *= g_LightAmbient * g_MaterialAmbient;\n"
					//"\t\t$(Out.color0).rgb = decale.rgb;\n"
					//"\t\t$(Out.color0) = float4(0.0f, 0.0f, 0.0f, 1.0f);\n"
					;					
				// 影なし
				default_color_code +=
					"\tfloat p = g_LightAmbient * g_MaterialAmbient + g_LightDiffuse * g_MaterialDiffuse * max(0, dot(normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+".xyz), -g_LightWorldDir.xyz));\n"
					"\tp = p * 0.5f + 0.5f;\n"
					"\tp = p * p;\n"
					"\t$(Out.color0) = p * $(meshSampler)($(In.tex0).xy);\n"
					;
				// 最終処理
				final_proc_code +=
					// とりあえず1.0fを入れておく/*要修正*/
					"\t$(Out.color0).a = 1.0f;\n"
					;

				break;
			case LIGHTING_TYPE_TOON:
				// ライティング用算出
				out +=
					// トゥーン用
					"\tfloat4 L = -g_LightWorldDir;\n"
					"\tfloat3 N = normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+");\n"
					"\tfloat3 Lbrightness = max(0, dot(N, L));\n"

					// 最後の * 0.333 とかの係数を調整して巧い事テクスチャ範囲を有効活用する
					//"\tfloat toon = 0.0001f + ( Lcolor.x + Lcolor.y + Lcolor.z ) * 0.2f;\n"
					"\tfloat toon = 0.0001f + ( Lbrightness.x + Lbrightness.y + Lbrightness.z ) * 0.333f;\n"
					;
				// 影あり
				shadow_color_code +=
					// テクスチャ座標Xを0.75倍して少し左側(暗くなる側)にずらす
					"\ttoon *= 0.75f;\n"
					;					
				// 影なし
				default_color_code +=
					// とりあえず何もしない
					""
					;
				// 最終処理
				final_proc_code +=
					// 明るさに応じたテクスチャ座標を与える
					// トゥーンシェードに用いるのはX成分のみ
					// Yは暫定この位置
					"\tfloat2 ToonTexCoord = float2(toon, 0.5f);\n"
					// ハーフランバートっぽい考え方
					//"\tfloat2 ToonTexCoord = float2(toon, 0.5f)*0.5f+0.5f;\

					"\tfloat4 Temp = $(meshSampler)($(In.tex0).xy);\n"
					"\tfloat4 TexColorAdjToonSDW = $(toonSampler)(ToonTexCoord.xy);\n"
					"\tTemp *= TexColorAdjToonSDW;\n"
					"\t$(Out.color0) = Temp;\n"

					// とりあえず1.0fを入れておく/*要修正*/
					"\t\t$(Out.color0).a = 1.0f;\n"
					;

				break;
			default:
				break;
		}

		// 影コードを追加する
		out += getShadowCodePS(lt, st, shadow_color_code, default_color_code);

		// 最終処理コードを追加する
		out += final_proc_code;
	}
	//else if(st == 投影テクスチャシャドウ) {
	//}

	return out;
}








// シェーダオプションコード
// リムライト(頂点シェーダ計算)
MBSTR C_d3d_effect_lighting_and_shading_shader::RimLightCalcVSVS(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";

	out +=
		// オブジェクトの輪郭付近の強度が強くなるようにする
		"\tfloat RimP = 1.0f - max( 0.0f, dot(normalize("+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_NORMAL)+".xyz), normalize(g_CameraWorldPos.xyz - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz)) );\n"
		// リムライトを適応する範囲および強度を調整する
		// このあたりの計算はオブジェクトのマテリアルにより任意に変更すること
		"\tRimP = RimP * g_RimLightPower;\n"
		"\tRimP = RimP * RimP;\n"
		// ライトがカメラの正面近くにあるほど強度が強くなるようにする
		// なので -g_LightWorldDir ではなく g_LightWorldDir で正しい
		"\tfloat RimPower = max( 0.0f, dot(normalize(g_CameraWorldPos.xyz - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz), normalize(g_LightWorldDir.xyz)) );\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR, lt, st)+" = 1.0f + g_RimLightColor * RimPower * RimP;\n"

		//// 複数光源対応版
		//"\tfloat4 AddLimColor = float4(0,0,0,1);\n"
		//"\tfor(int i = 0; i < g_DLightNum; i++) {\n"
		//	"\t\tfloat RimPower = max( 0.0f, dot(normalize(g_CameraWorldPos.xyz - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz), normalize(g_DLightWorldDir[i].xyz)) );\n"
		//	"\t\tAddLimColor += 1.0f + g_DLightDiffuse[i] * RimPower * RimP;\n"
		//"\t}\n"
		//"\tfor(int i = 0; i < g_PLightNum; i++) {\n"
		//"\t}\n"
		//"\tfor(int i = 0; i < g_SLightNum; i++) {\n"
		//"\t}\n"
		////"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR, lt, st)+" = AddLimColor / (g_DLightNum+g_PLightNum+g_SLightNum);\n"
		//"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR, lt, st)+" = AddLimColor;\n"
		;

	return out;
}
MBSTR C_d3d_effect_lighting_and_shading_shader::RimLightCalcVSPS(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";

	out +=
		// リムライトのかかり具合チェック用
		//"\t$(Out.color0).rgb = 0.5f;\n"

		// リムライトカラーを乗算する
		"\t$(Out.color0).rgb *= "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR, lt, st)+";\n"
		;

	return out;
}

// リムライト(ピクセルシェーダ計算)未実装
MBSTR C_d3d_effect_lighting_and_shading_shader::RimLightCalcPSVS(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";
	out +=
		// ライトがカメラの正面近くにあるほど強度が強くなるようにする
		"\t$(Out.tex7) = max( 0.0f, dot(normalize(E), normalize(-L)) );\n"
		;

	return "";
}
MBSTR C_d3d_effect_lighting_and_shading_shader::RimLightCalcPSPS(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";
	out +=
		// オブジェクトの輪郭付近の強度が強くなるようにする
		"\tfloat P = 1.0f - max( 0.0f, dot(N, normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_EYE, lt, st)+".xyz)) );\n"
		// リムライトを適応する範囲および強度を調整する
		// このあたりの計算はオブジェクトのマテリアルにより任意に変更すること
		"\tP = P * g_RimLightPower;\n"
		"\tP = P * P;\n"
		"\t$(Out.color0) += g_RimLightColor * $(In.tex7) * P;\n"
		;
	return "";
}







}