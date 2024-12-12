#pragma once

#include "tonad3d3_effect_func_interface.h"

namespace NT3
{

class C_d3d_effect_mrbd_color_func : public C_d3d_effect_func_interface {
public:
	C_d3d_effect_mrbd_color_func()
		:C_d3d_effect_func_interface("MRBDColor")
	{
	}

	// ���̊֐��Ŏg���O���[�o���ϐ�

	// MRBD���g��Ȃ��Ƃ��Ɏg�p����R�[�h
	std::string initCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s(float4 org) {\n"
			"\treturn org;\n"
			"}\n\n", funcName.c_str());
		return str;
	}

	// MRBD���g�����Ɏ��ۂɎg�p����R�[�h
	std::string realCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s(float4 org) {\n"
			"\tfloat4 color = org;\n"

			// reverse
			"\tfloat4 reverse = 1.0f - org;\n"
			"\tcolor = lerp(color, reverse, g_C0.y);\n"

			// mono
			"\tfloat mono_y = dot(g_Mono, org);\n"
			"\tcolor = lerp(color, mono_y, g_C0.x);\n"

			// bright
			"\tcolor = color + g_C0.z;\n"

 			// dark
			"\tcolor = color - g_C0.w;\n"

			// turn back alpha
			"\tcolor.a = org.a;\n"

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