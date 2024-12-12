#pragma once

#include "tonad3d3_effect_func_interface.h"

// 今のところ未使用


namespace NT3
{

class C_d3d_effect_tone_curve_func : public C_d3d_effect_func_interface {
public:
	C_d3d_effect_tone_curve_func()
		:C_d3d_effect_func_interface("ToneCurve")
	{
	}

	// この関数で使うグローバル変数

	// トーンカーブを使わないときに使用するコード
	std::string initCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s(float4 org) {\n"
			"\treturn org;\n"
			"}\n\n", funcName.c_str());
		return str;
	}

	// トーンカーブを使う時に実際に使用するコード
	std::string realCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s(float4 org) {\n"
			"\tfloat4 color = org;\n"
			"\tfloat2 tonecurve_pos;\n"
			"\tfloat4 tonecurve_color;\n"

			// tonecurve sat
			//"\tfloat tonecurve_sat = dot(mono, org);\n"	// 何故かmono使ってるからg_C3.aに変更してみる
			"\tfloat tonecurve_sat = dot(g_C3.a, org);\n"
			"\tcolor = lerp(color, tonecurve_sat, g_C3.g);\n"

			// tonecurve r
			// my_sampler_tex01_tonecurveってのはサンプラ使わないかもしれないのに
			// 全部に宣言追加ってこと？それは良いとしてもこのMRBD系のfuncは間違いな気がする
			// どちらかと言えばsampler_func系に合わせるべきかも
			// そもそも3Dでトーンカーブいるのかって話
			"\ttonecurve_pos = float2(color.r, g_C3.r);\n"
			"\ttonecurve_color = tex2D(my_sampler_tex01_tonecurve, tonecurve_pos);\n"
			"\tcolor.r = tonecurve_color.r;\n"

			// tonecurve g
			"\ttonecurve_pos = float2(color.g, g_C3.r);\n"
			"\ttonecurve_color = tex2D(my_sampler_tex01_tonecurve, tonecurve_pos);\n"
			"\tcolor.g = tonecurve_color.g;\n"

			// tonecurve b
			"\ttonecurve_pos = float2(color.b, g_C3.r);\n"
			"\ttonecurve_color = tex2D(my_sampler_tex01_tonecurve, tonecurve_pos);\n"
			"\tcolor.b = tonecurve_color.b;\n"

			// turn back alpha
			"\tcolor.a = org.a;\n"

			// return color
			"\treturn color;\n"
			"}\n\n", funcName.c_str());
		
		return str;
	}


	// 関数呼び出し
	std::string operator () (const std::string colorVal) {
		return funcName + "( " + colorVal + " )";
	}
};


}