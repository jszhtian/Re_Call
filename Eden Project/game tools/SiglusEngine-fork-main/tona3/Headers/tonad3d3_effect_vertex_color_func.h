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

	// ���̊֐��Ŏg���O���[�o���ϐ�

	// Vertex���g��Ȃ��Ƃ��Ɏg�p����R�[�h
	std::string initCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s(float4 src, float4 vertex) {\n"
				"\treturn src;\n"
			"}\n\n", funcName.c_str());
		return str;
	}

	// Vertex���g�����Ɏ��ۂɎg�p����R�[�h
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


	// �֐��Ăяo��
	std::string operator () (const std::string colorVal) {
		return funcName + "( " + colorVal + " )";
	}
};


}