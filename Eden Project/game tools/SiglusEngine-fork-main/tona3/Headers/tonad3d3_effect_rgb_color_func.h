#pragma once

#include "tonad3d3_effect_func_interface.h"


namespace NT3
{


class C_d3d_effect_rgb_color_func : public C_d3d_effect_func_interface {
public:
	C_d3d_effect_rgb_color_func()
		:C_d3d_effect_func_interface("RGBColor")
	{
	}

	// この関数で使うグローバル変数

	// MRBDを使わないときに使用するコード
	std::string initCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s(float4 org) {\n"
			"\treturn org;\n"
			"}\n\n", funcName.c_str());
		return str;
	}

	// MRBDを使う時に実際に使用するコード
	std::string realCode() {
		char str[1024];
		sprintf_s( str,

			"float4 %s(float4 org) {\n"
			"\tfloat4 color = org;\n"

			// rgb
			"\tcolor = lerp(color, g_C1, g_C1.w);\n"

			// rgbrep
			"\tcolor += g_C2;\n"

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