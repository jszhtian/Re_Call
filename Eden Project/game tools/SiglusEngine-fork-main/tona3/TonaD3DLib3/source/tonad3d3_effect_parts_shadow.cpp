#include "tonad3d3_pch.h"
#include "tonad3d3_effect_parts_shadow.h"
#include "tonad3d3_effect_source_val_name_manager.h"

namespace NT3
{

// ���_�V�F�[�_�ɂ�����e�����R�[�h�擾
MBSTR getShadowCodeVS(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";

	if(false);
	// �e�Ȃ�
	else if(st == SHADING_TYPE_NONE) {
		// �������Ȃ�
	}
	// �[�x�o�b�t�@�V���h�E(ShadowMapUV��Depth ���K�v)
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		out +=
			// �V���h�E�}�b�v
			"\tfloat4 ShadowMapUV = float4("+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+", 1.0f);\n"
			"\tShadowMapUV = mul(ShadowMapUV, mLightVP);\n"
			"\tShadowMapUV = mul(ShadowMapUV, mScaleBias);\n"
			"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_SHADOW_MAP_UV, lt, st)+" = ShadowMapUV;\n"

			// ��r�̂��߂̐[�x�l
			"\tfloat4 Depth = float4("+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+", 1.0f);\n"
			"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_DEPTH, lt, st)+" = mul(Depth, mLightVP);\n"
		;
	}

	return out;
}


// �s�N�Z���V�F�[�_�ɂ�����e�����R�[�h�擾
MBSTR getShadowCodePS(LIGHTING_TYPE lt, SHADING_TYPE st, MBSTR shadow_color_code, MBSTR default_color_code)
{
	MBSTR out = "";

	if(false);
	// �e�Ȃ�
	else if(st == SHADING_TYPE_NONE) {
		// �������Ȃ�
	}
	// �[�x�o�b�t�@�V���h�E(ShadowMapUV��Depth ���K�v)
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		out +=
			// �e�֘A�̈ꎞ�ϐ����
			"\tfloat4 TexMapUV = $(In.tex0);\n"
			"\tfloat4 ShadowMapUV = "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_SHADOW_MAP_UV, lt, st)+";\n"
			"\tfloat4 Depth = "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_DEPTH, lt, st)+";\n"
			// �e
			"\tfloat  shadow = $(shadowSampler)(ShadowMapUV).x;\n"
			"\tfloat4 decale = $(meshSampler)(TexMapUV.xy);\n"
			
			"\tif(shadow * Depth.w < Depth.z - g_DBSBias) {\n"
				// �e����
				"\t"+shadow_color_code+
			
			"\t}\n"
			"\telse {\n"
				// �e�Ȃ�
				"\t"+default_color_code+
			"\t}\n"
		;
	}

	return out;
}



















}