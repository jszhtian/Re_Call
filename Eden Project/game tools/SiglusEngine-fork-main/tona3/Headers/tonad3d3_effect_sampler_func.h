// C_d3d_effect_sampler_func.h

#pragma once

// ��ŏ���
#include "tonad3d3_effect_mrbd_color_func.h"
#include "tonad3d3_effect_rgb_color_func.h"
#include "tonad3d3_effect_vertex_color_func.h"
#include "tonad3d3_effect_tone_curve_func.h"

namespace NT3
{


class C_d3d_effect_sampler_func {
	std::string funcName;
	D3DXVECTOR4 initVal;
	std::string samplerName;

	bool is2DProj;

public:
	// �R���X�g���N�^
	C_d3d_effect_sampler_func(std::string funcName, D3DXVECTOR4 &initVal, std::string samplerName) :
		funcName(funcName), initVal(initVal), samplerName(samplerName), is2DProj(false) {}

	// 2DProj�p
	C_d3d_effect_sampler_func(std::string funcName, D3DXVECTOR4 &initVal, std::string samplerName, bool is2dprj) :
		funcName(funcName), initVal(initVal), samplerName(samplerName), is2DProj(is2dprj) {}

	// �����l�֐��o��
	std::string initCode() {
		char str[1024];
		sprintf_s( str,
			"float4 %s( float2 uv ) {\n"
			"\treturn float4(%f, %f, %f, %f);\n"
			"}\n\n", funcName.c_str(), initVal.x, initVal.y, initVal.z, initVal.w);
		return str;
	}

	// �T���v�����O�֐��o��
	std::string samplerCode() {
		char str[1024];
		if(!is2DProj) {
			sprintf_s( str,
				"float4 %s( float2 uv ) {\n"
				"\treturn tex2D(%s, uv);\n"
				"}\n\n", funcName.c_str(), samplerName.c_str());
		}
		else {
			sprintf_s( str,
				"float4 %s( float4 uv ) {\n"
				"\treturn tex2Dproj(%s, uv);\n"
				"}\n\n", funcName.c_str(), samplerName.c_str());
		}
		return str;
	}

	// �T���v���Ăяo��
	std::string operator () (const std::string uvVal) {
		return funcName + "( " + uvVal + " )";
	}

	// �֐������擾
	std::string getFuncName() const { return funcName; } 
};


}