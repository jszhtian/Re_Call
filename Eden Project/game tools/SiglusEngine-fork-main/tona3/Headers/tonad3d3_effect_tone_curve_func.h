#pragma once

#include "tonad3d3_effect_func_interface.h"

// ���̂Ƃ��떢�g�p


namespace NT3
{

class C_d3d_effect_tone_curve_func : public C_d3d_effect_func_interface {
public:
	C_d3d_effect_tone_curve_func()
		:C_d3d_effect_func_interface("ToneCurve")
	{
	}

	// ���̊֐��Ŏg���O���[�o���ϐ�

	// �g�[���J�[�u���g��Ȃ��Ƃ��Ɏg�p����R�[�h
	std::string initCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s(float4 org) {\n"
			"\treturn org;\n"
			"}\n\n", funcName.c_str());
		return str;
	}

	// �g�[���J�[�u���g�����Ɏ��ۂɎg�p����R�[�h
	std::string realCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s(float4 org) {\n"
			"\tfloat4 color = org;\n"
			"\tfloat2 tonecurve_pos;\n"
			"\tfloat4 tonecurve_color;\n"

			// tonecurve sat
			//"\tfloat tonecurve_sat = dot(mono, org);\n"	// ���̂�mono�g���Ă邩��g_C3.a�ɕύX���Ă݂�
			"\tfloat tonecurve_sat = dot(g_C3.a, org);\n"
			"\tcolor = lerp(color, tonecurve_sat, g_C3.g);\n"

			// tonecurve r
			// my_sampler_tex01_tonecurve���Ă̂̓T���v���g��Ȃ���������Ȃ��̂�
			// �S���ɐ錾�ǉ����Ă��ƁH����͗ǂ��Ƃ��Ă�����MRBD�n��func�͊ԈႢ�ȋC������
			// �ǂ��炩�ƌ�����sampler_func�n�ɍ��킹��ׂ�����
			// ��������3D�Ńg�[���J�[�u����̂����Ęb
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


	// �֐��Ăяo��
	std::string operator () (const std::string colorVal) {
		return funcName + "( " + colorVal + " )";
	}
};


}