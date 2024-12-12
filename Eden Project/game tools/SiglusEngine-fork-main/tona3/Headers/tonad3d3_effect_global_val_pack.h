#pragma once


#include "tonad3d3_effect_global_val.h"
#include "tonad3d3_light_manager.h"


namespace NT3
{




// 2Dと3D用は分けた方が良いかも
class C_d3d_effect_global_val_pack {
private:
	std::vector<C_d3d_effect_global_val> vals;

public:
	C_d3d_effect_global_val_pack()
	{
		// 色味など
		// モノクロ用(固定)値
		vals.push_back( C_d3d_effect_global_val("float4", "g_Mono", "float4(0.2989f, 0.5886f, 0.1145f, 0.0f)", "") );
		// mono, reverse, bright, dark
		vals.push_back( C_d3d_effect_global_val("float4", "g_C0", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// r, g, b, rate
		vals.push_back( C_d3d_effect_global_val("float4", "g_C1", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// r, g, b
		vals.push_back( C_d3d_effect_global_val("float4", "g_C2", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// tonecurve_no, tonecurve_sat
		vals.push_back( C_d3d_effect_global_val("float4", "g_C3", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );

		// マテリアル
		// マテリアルのディフューズ(拡散光)
		vals.push_back( C_d3d_effect_global_val("float4", "g_MaterialDiffuse", "float4(0.0f, 0.0f, 0.0f, 1.0f)", "") );
		// マテリアルのアンビエント(環境光、周囲光)
		vals.push_back( C_d3d_effect_global_val("float4", "g_MaterialAmbient", "float4(0.0f, 0.0f, 0.0f, 1.0f)", "") );
		// マテリアルのスペキュラ(反射光)
		vals.push_back( C_d3d_effect_global_val("float4", "g_MaterialSpecular", "float4(1.0f, 1.0f, 1.0f, 1.0f)", "") );
		// マテリアルのスペキュラパワー
		vals.push_back( C_d3d_effect_global_val("float", "g_MaterialSpecularPower", "float(1.0f)", "") );
		// マテリアルのエミッシブ(自己照明)
		vals.push_back( C_d3d_effect_global_val("float4", "g_MaterialEmissive", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );

		// 各種ライト
		// 各種ライトの配列用変数
		MBSTR array_num;
		// ディレクショナルライト
		array_num = TSTR_to_MBSTR(tostr(C_d3d_light_manager::OBJ_DIRECTIONAL_LIGHT_MAX));
		vals.push_back( C_d3d_effect_global_val("static const int", "DLIGHT_MAX", array_num, "") );
		// 実際のライトの数
		vals.push_back( C_d3d_effect_global_val("int", "g_DLightNum", "0", "") );
		// ライトのディフューズ
		vals.push_back( C_d3d_effect_global_val("float4", "g_DLightDiffuse[DLIGHT_MAX]", array_num+"@float4(0.7f, 0.7f, 0.7f, 1.0f)@", "") );
		// ライトのアンビエント
		vals.push_back( C_d3d_effect_global_val("float4", "g_DLightAmbient[DLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ライトのスペキュラ
		vals.push_back( C_d3d_effect_global_val("float4", "g_DLightSpecular[DLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ライトの方向(ローカル座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_DLightLocalDir[DLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ライトの方向(ワールド座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_DLightWorldDir[DLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );


		// ポイントライト
		array_num = TSTR_to_MBSTR(tostr(C_d3d_light_manager::OBJ_POINT_LIGHT_MAX));
		vals.push_back( C_d3d_effect_global_val("static const int", "PLIGHT_MAX", array_num, "") );
		// 実際のライトの数
		vals.push_back( C_d3d_effect_global_val("int", "g_PLightNum", "0", "") );
		// ライトのディフューズ
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightDiffuse[PLIGHT_MAX]", array_num+"@float4(0.7f, 0.7f, 0.7f, 1.0f)@", "") );
		// ライトのアンビエント
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightAmbient[PLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ライトのスペキュラ
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightSpecular[PLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ライトの座標(ローカル座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightLocalPos[PLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ライトの座標(ワールド座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightWorldPos[PLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ライトの減衰系パラメータ(atten0, atten1, atten2, range)
		vals.push_back( C_d3d_effect_global_val("float4", "g_PLightAttenuationsRange[PLIGHT_MAX]", array_num+"@float4(0.001f, 0.0f, 0.0f, 0.0f)@", "") );
		

		// スポットライト
		array_num = TSTR_to_MBSTR(tostr(C_d3d_light_manager::OBJ_SPOT_LIGHT_MAX));
		vals.push_back( C_d3d_effect_global_val("static const int", "SLIGHT_MAX", array_num, "") );
		// 実際のライトの数
		vals.push_back( C_d3d_effect_global_val("int", "g_SLightNum", "0", "") );
		// ライトのディフューズ
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightDiffuse[SLIGHT_MAX]", array_num+"@float4(0.7f, 0.7f, 0.7f, 1.0f)@", "") );
		// ライトのアンビエント
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightAmbient[SLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ライトのスペキュラ
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightSpecular[SLIGHT_MAX]", array_num+"@float4(0.3f, 0.3f, 0.3f, 1.0f)@", "") );
		// ライトの方向(ローカル座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightLocalDir[SLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ライトの方向(ワールド座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightWorldDir[SLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ライトの座標(ローカル座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightLocalPos[SLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ライトの座標(ワールド座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightWorldPos[SLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );
		// ライトの減衰系パラメータ(atten0, atten1, atten2, range)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightAttenuationsRange[SLIGHT_MAX]", array_num+"@float4(0.001f, 0.0f, 0.0f, 0.0f)@", "") );
		// スポットライト系パラメータ(theta/2, phi/2, falloff, w未使用)
		vals.push_back( C_d3d_effect_global_val("float4", "g_SLightThetaPhiFalloff[SLIGHT_MAX]", array_num+"@float4(0.0f, 0.0f, 0.0f, 0.0f)@", "") );

		// グローバルアンビエント(固定機能パイプライン再現用)
		vals.push_back( C_d3d_effect_global_val("float4", "g_GlobalAmbient", "float4(0.0f, 0.0f, 0.0f, 1.0f)", "") );



		// 従来のライト
		// ライトの方向(ローカル座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightLocalDir", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// ライトの方向(ワールド座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightWorldDir", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// ライトの座標(ローカル座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightLocalPos", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// ライトの座標(ワールド座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightWorldPos", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// ライトのディフューズ
		//vals.push_back( C_d3d_effect_global_val("float4", "g_LightDiffuse", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightDiffuse", "float4(0.7f, 0.7f, 0.7f, 1.0f)", "") );
		// ライトのアンビエント
		//vals.push_back( C_d3d_effect_global_val("float4", "g_LightAmbient", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightAmbient", "float4(0.3f, 0.3f, 0.3f, 1.0f)", "") );
		// ライトの減衰率(W成分不要)
		vals.push_back( C_d3d_effect_global_val("float4", "g_LightAttenuations", "float4(0.001f, 0.0f, 0.0f, 0.0f)", "") );

		// カメラ
		// カメラの座標(ローカル座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_CameraLocalPos", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );
		// カメラの座標(ワールド座標系)
		vals.push_back( C_d3d_effect_global_val("float4", "g_CameraWorldPos", "float4(0.0f, 0.0f, 0.0f, 0.0f)", "") );


		// 深度バッファシャドウ用
		// バイアス値
		vals.push_back( C_d3d_effect_global_val("float", "g_DBSBias", "float(0.03f)", "") );

		// リムライト用
		// リムライトの適用範囲と強度用パラメータ
		vals.push_back( C_d3d_effect_global_val("float", "g_RimLightPower", "float(0.5f)", "") );
		// リムライトの色(テストの時は分かりやすい色が良い)
		//vals.push_back( C_d3d_effect_global_val("float4", "g_RimLightColor", "float4(1.0f, 0.0f, 0.0f, 0.0f)", "") );
		vals.push_back( C_d3d_effect_global_val("float4", "g_RimLightColor", "float4(1.0f, 1.0f, 1.0f, 1.0f)", "") );
		//vals.push_back( C_d3d_effect_global_val("float4", "g_RimLightColor", "float4(-1.0f, -1.0f, -1.0f, 0.0f)", "") );


		// 頂点カラー用
		// 頂点カラー倍率(オブジェクトにデータがない場合スキンメッシュは1.0fを入れるのでrate=1.0fだと見た目に変化なし)
		vals.push_back( C_d3d_effect_global_val("float", "g_VertexColorRate", "float(1.0f)", "") );
		//vals.push_back( C_d3d_effect_global_val("float", "g_VertexColorRate", "float(1.5f)", "") );

		// スキンメッシュのワールド→ローカル変換用
		// ワールド行列の逆行列
		//vals.push_back( C_d3d_effect_global_val("float4x4", "mInvWorld", "", "") );

		// 視差マッピング用
		// 視差マッピングの最大の高さ
		vals.push_back( C_d3d_effect_global_val("float", "g_ParallaxMaxHeight", "float(0.016f)", "") );




	}

	std::vector<C_d3d_effect_global_val>::iterator begin()
	{
		return vals.begin();
	}
	std::vector<C_d3d_effect_global_val>::iterator end()
	{
		return vals.end();
	}	
};





}