#pragma once

#include "tonad3d3_effect_func_interface.h"

namespace NT3
{

class C_d3d_effect_vertex_color_func : public C_d3d_effect_func_interface {
public:
	C_d3d_effect_vertex_color_func()
		:C_d3d_effect_func_interface("VertexColor")
	{
	}

	// この関数で使うグローバル変数

	// Vertexを使わないときに使用するコード
	std::string initCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s(float4 src, float4 vertex) {\n"
				"\treturn src;\n"
			"}\n\n", funcName.c_str());
		return str;
	}

	// Vertexを使う時に実際に使用するコード
	std::string realCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s(float4 src, float4 vertex) {\n"
				"\tfloat4 color = src * vertex * g_VertexColorRate;\n"
				// turn back alpha
				"\tcolor.a = src.a;\n"
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