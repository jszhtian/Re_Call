#include "tonad3d3_pch.h"
#include "tonad3d3_effect_lighting_and_shading_shader.h"
#include "tonad3d3_effect_lighting_shader.h"
#include "tonad3d3_effect_source_val_name_manager.h"

#include "tonad3d3_effect_parts_shadow.h"

namespace NT3
{


// �Z�}���e�B�N�X���̍č\�z
DWORD C_d3d_effect_lighting_and_shading_shader::restructSemanticsBit(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so, DWORD bit)
{
	// ����Ȃ��Z�}���e�B�N�X��ǉ�����
	if(false);

	// �e�Ȃ�
	else if(st == SHADING_TYPE_NONE) {
		// ���C�e�B���O�̎�ނŕ���
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

	// �[�x�o�b�t�@�V���h�E
	// ShadowMapUV �� Depth ���K�v
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		// ���C�e�B���O�̎�ނŕ���
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

// ���_�V�F�[�_�R�[�h
MBSTR C_d3d_effect_lighting_and_shading_shader::getVertexShaderCode(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so, MBSTR varNameNormal)
{
	MBSTR out = "";

	// �����͉e�̎�ށA�L���Ɋւ�炸�K���ʂ�
	//out += C_d3d_effect_lighting_shader::getVertexShaderCode(C_d3d_effect_lighting_shader::TYPE(lt), varNameNormal);
	// ����ς߂�ǂ������ǂ���1�񂱂����ɏ���

	// �e�Ȃ��Ȃ畁�ʂ�
	if(st == SHADING_TYPE_NONE) {
		out += C_d3d_effect_lighting_shader::getVertexShaderCode(lt, varNameNormal);
	}
	// �e����Ȃ�
	else {
		out += getVertexShaderCodeWithShadow(lt, st, varNameNormal);
	}
	
	if(so & SHADER_OPTION_RIM_LIGHT && lt != LIGHTING_TYPE_NONE)
		out += RimLightCalcVSVS(lt, st);


	return out;
}

// �s�N�Z���V�F�[�_�R�[�h
MBSTR C_d3d_effect_lighting_and_shading_shader::getPixelShaderCode(LIGHTING_TYPE lt, SHADING_TYPE st, SHADER_OPTION so)
{
	MBSTR out = "";

	// �e�Ȃ��Ȃ牽���C�ɂ�����
	if(st == SHADING_TYPE_NONE) {
		// C_d3d_effect_lighting_shader�̃R�[�h�����̂܂܎擾
		out += C_d3d_effect_lighting_shader::getPixelShaderCode(lt);
	}
	// �e����Ȃ�
	else {
		// �e��e���胉�C�e�B���O�R�[�h�����֐����Ăяo��

		out += getPixelShaderCodeWithShadow(lt, st);

		// if(shadowCheck(st)) {
		//		�e�F�H�e�F�{���C�e�B���O�F�H
		// }
		// else {
		//		�e�탉�C�e�B���O
		// }
	}

	if(so & SHADER_OPTION_RIM_LIGHT && lt != LIGHTING_TYPE_NONE)
		out += RimLightCalcVSPS(lt, st);

	return out;
}


// �����������
// �e��e���胉�C�e�B���O�R�[�h(VS)
MBSTR C_d3d_effect_lighting_and_shading_shader::getVertexShaderCodeWithShadow(LIGHTING_TYPE lt, SHADING_TYPE st, MBSTR varNameNormal)
{
	MBSTR out = "";

	// �܂��e�̎�ނŕ��򂷂�
	if(false);
	// �e�Ȃ�
	else if(st == SHADING_TYPE_NONE) {
		// �e�����邩�H�ŕ��򂵂Ă���̂ł����͐�΂ɒʂ�Ȃ�
	}
	// �[�x�o�b�t�@�V���h�E
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		// �����Ŋe�탉�C�e�B���O�R�[�h�ɕ��򂷂�
		switch(lt) {
			case LIGHTING_TYPE_NONE:
				out +=
					""
					;
				break;
			case LIGHTING_TYPE_LAMBERT:
				out +=
					// ���_�J���[
					"\tfloat4 L = -g_LightWorldDir;\n"
					// ���� * �g�U���ˌ�
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
					// ���[�J�����W�n�ł̃��C�g�x�N�g��
					"\tfloat3 L = -g_LightWorldDir;\n"
					"\tfloat3 E = normalize(g_CameraLocalPos - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz);\n"
					// �n�[�t�x�N�g��
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
					// ���X�^���C�U�Ő��`���(�H)�����̂Ő��K���̓s�N�Z���V�F�[�_��
					"\tfloat3 E = g_CameraWorldPos - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz;\n"

					"\tfloat4 diffuse = g_LightDiffuse * g_MaterialDiffuse * max(0, dot(normalize(N), normalize(L)));\n"
					"\tfloat4 ambient = g_LightAmbient * g_MaterialAmbient;\n"

					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = N;\n"
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_EYE, lt, st)+".xyz = E;\n"
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_DIFFUSE, lt, st)+" = diffuse;\n"

					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_AMBIENT, lt, st)+" = ambient;\n"

					"\t$(Out.color0) = diffuse;\n"
					/*
					// �V���h�E�}�b�v
					"\tfloat3 TempPos = mul(float4(Pos.xyz, 1.0f), Comb);\n"

					"\tfloat3 TempPos2 = mul(float4(Pos.xyz, 1.0f), Comb);\n"
					"\tTempPos2 = mul(TempPos2, mLightVP);\n"
					"\tTempPos2 = mul(TempPos2, mLightWVP);\n"
					

					"\t$(Out.tex2).xyz = TempPos2;\n"
					//"\t$(Out.tex2) = mul(Pos, mLightWVP);\n"
					// ��r�̂��߂̐[�x�l
					"\t$(Out.tex3) = mul(TempPos, mLightVP);\n"
					//"\t$(Out.tex3) = mul(Pos, mLightVP);\n"
					*/
					;
				break;
			case LIGHTING_TYPE_PP_HALF_LAMBERT:
				out +=
					// �n�[�t�����o�[�g�p
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = "+varNameNormal+";\n"
					;
				break;
			case LIGHTING_TYPE_TOON:
				out +=
					// �g�D�[���p�@��
					"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = "+varNameNormal+";\n"
					;
				break;
			default:
				break;
		}

		// �[�x�o�b�t�@�V���h�E����
		out += getShadowCodeVS(lt, st);
	}

	return out;
}

// �e��e���胉�C�e�B���O�R�[�h(PS)
MBSTR C_d3d_effect_lighting_and_shading_shader::getPixelShaderCodeWithShadow(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";

	// �܂��e�̎�ނŕ��򂷂�
	if(false);
	// �e�Ȃ�
	else if(st == SHADING_TYPE_NONE) {
		// �e�����邩�H�ŕ��򂵂Ă���̂ł����͐�΂ɒʂ�Ȃ�
	}
	// �[�x�o�b�t�@�V���h�E
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		MBSTR shadow_color_code = "";
		MBSTR default_color_code = "";
		MBSTR final_proc_code = "";

		// �����Ŋe�탉�C�e�B���O�R�[�h�ɕ��򂷂�
		switch(lt) {
			case LIGHTING_TYPE_NONE:
				// ���C�e�B���O�p�Z�o
				out += 
					""
					;
				// �e����
				shadow_color_code +=
					"\t$(Out.color0) = float4(0.0f, 0.0f, 0.0f, 1.0f);\n"
					;					
				// �e�Ȃ�
				default_color_code +=
					"\t$(Out.color0) = float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
					;
				// �ŏI����
				final_proc_code +=
					""
					;
				
				break;
			case LIGHTING_TYPE_LAMBERT:
				// ���C�e�B���O�p�Z�o
				out += 
					//"\tfloat4 dif_tex = $(In.color0) * $(meshSampler)(TexMapUV.xy);\n"
					"\tfloat4 dif_tex = $(meshSampler)($(In.tex0).xy);\n"
					;
				// �e����
				shadow_color_code +=
					"\tdif_tex.rgb *= "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_AMBIENT, lt, st)+".rgb;\n"
					"\t$(Out.color0).rgb = dif_tex.rgb;\n"
					;					
				// �e�Ȃ�
				default_color_code +=
					//"\t\t$(Out.color0) = $(In.color0) * $(meshSampler)($(In.tex0).xy);\n"
					"\t$(Out.color0) = ($(In.color0)+$(In.tex7)) * $(meshSampler)($(In.tex0).xy);\n"
					;
				// �ŏI����
				final_proc_code +=
					// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
					"\t$(Out.color0).a = 1.0f;\n"
					;

				break;
			case LIGHTING_TYPE_BLINN_PHONG:
				// ���C�e�B���O�p�Z�o
				out += 
					//"\tfloat4 dif_tex = $(In.color0) * $(meshSampler)(TexMapUV.xy);\n"
					"\tfloat4 dif_tex = $(meshSampler)($(In.tex0).xy);\n"
					;
				// �e����
				shadow_color_code +=
					"\tdecale.rgb *= "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_AMBIENT, lt, st)+".rgb;\n"
					"\t$(Out.color0).rgb = decale.rgb;\n"
					;					
				// �e�Ȃ�
				default_color_code +=
					"\t$(Out.color0) = ("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_DIFFUSE, lt, st)+" + "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_AMBIENT, lt, st)+") * $(meshSampler)($(In.tex0).xy) + "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_SPECULAR, lt, st)+";\n"
					;
				// �ŏI����
				final_proc_code +=
					// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
					"\t$(Out.color0).a = 1.0f;\n"
					;

				break;
			case LIGHTING_TYPE_PP_BLINN_PHONG:
				// ���C�e�B���O�p�Z�o
				out +=
					"\tfloat3 L = -g_LightWorldDir;\n"
					"\tfloat3 N = normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+".xyz);\n"
					"\tfloat3 H = normalize(L + normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_EYE, lt, st)+".xyz));\n"
					"\tfloat4 specular = pow(max(0,dot(N, H)), g_MaterialSpecularPower);\n"
					;
				// �e����
				shadow_color_code +=
					"\tdecale.rgb *= "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_AMBIENT, lt, st)+".rgb;\n"
					"\t$(Out.color0).rgb = decale.rgb;\n"
					;
				// �e�Ȃ�
				default_color_code +=
					"\tdecale.rgb *= "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_AMBIENT, lt, st)+".rgb + "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_DIFFUSE, lt, st)+".rgb;\n"
					"\t$(Out.color0).rgb = decale.rgb + specular.rgb;\n"
					;
				// �ŏI����
				final_proc_code +=
					// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
					"\t$(Out.color0).a = 1.0f;\n"
					;

				break;
			case LIGHTING_TYPE_PP_HALF_LAMBERT:
				// ���C�e�B���O�p�Z�o
				out +=
					""
					;
				// �e����
				shadow_color_code +=
					"\t\tfloat p = g_LightAmbient * g_MaterialAmbient;\n"
					"\tp = p * 0.5f + 0.5f;\n"
					"\tp = p * p;\n"
					"\t$(Out.color0) = p * $(meshSampler)($(In.tex0).xy);\n"
					//"\t\tdecale.rgb *= g_LightAmbient * g_MaterialAmbient;\n"
					//"\t\t$(Out.color0).rgb = decale.rgb;\n"
					//"\t\t$(Out.color0) = float4(0.0f, 0.0f, 0.0f, 1.0f);\n"
					;					
				// �e�Ȃ�
				default_color_code +=
					"\tfloat p = g_LightAmbient * g_MaterialAmbient + g_LightDiffuse * g_MaterialDiffuse * max(0, dot(normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+".xyz), -g_LightWorldDir.xyz));\n"
					"\tp = p * 0.5f + 0.5f;\n"
					"\tp = p * p;\n"
					"\t$(Out.color0) = p * $(meshSampler)($(In.tex0).xy);\n"
					;
				// �ŏI����
				final_proc_code +=
					// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
					"\t$(Out.color0).a = 1.0f;\n"
					;

				break;
			case LIGHTING_TYPE_TOON:
				// ���C�e�B���O�p�Z�o
				out +=
					// �g�D�[���p
					"\tfloat4 L = -g_LightWorldDir;\n"
					"\tfloat3 N = normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+");\n"
					"\tfloat3 Lbrightness = max(0, dot(N, L));\n"

					// �Ō�� * 0.333 �Ƃ��̌W���𒲐����čI�����e�N�X�`���͈͂�L�����p����
					//"\tfloat toon = 0.0001f + ( Lcolor.x + Lcolor.y + Lcolor.z ) * 0.2f;\n"
					"\tfloat toon = 0.0001f + ( Lbrightness.x + Lbrightness.y + Lbrightness.z ) * 0.333f;\n"
					;
				// �e����
				shadow_color_code +=
					// �e�N�X�`�����WX��0.75�{���ď�������(�Â��Ȃ鑤)�ɂ��炷
					"\ttoon *= 0.75f;\n"
					;					
				// �e�Ȃ�
				default_color_code +=
					// �Ƃ肠�����������Ȃ�
					""
					;
				// �ŏI����
				final_proc_code +=
					// ���邳�ɉ������e�N�X�`�����W��^����
					// �g�D�[���V�F�[�h�ɗp����̂�X�����̂�
					// Y�͎b�肱�̈ʒu
					"\tfloat2 ToonTexCoord = float2(toon, 0.5f);\n"
					// �n�[�t�����o�[�g���ۂ��l����
					//"\tfloat2 ToonTexCoord = float2(toon, 0.5f)*0.5f+0.5f;\

					"\tfloat4 Temp = $(meshSampler)($(In.tex0).xy);\n"
					"\tfloat4 TexColorAdjToonSDW = $(toonSampler)(ToonTexCoord.xy);\n"
					"\tTemp *= TexColorAdjToonSDW;\n"
					"\t$(Out.color0) = Temp;\n"

					// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
					"\t\t$(Out.color0).a = 1.0f;\n"
					;

				break;
			default:
				break;
		}

		// �e�R�[�h��ǉ�����
		out += getShadowCodePS(lt, st, shadow_color_code, default_color_code);

		// �ŏI�����R�[�h��ǉ�����
		out += final_proc_code;
	}
	//else if(st == ���e�e�N�X�`���V���h�E) {
	//}

	return out;
}








// �V�F�[�_�I�v�V�����R�[�h
// �������C�g(���_�V�F�[�_�v�Z)
MBSTR C_d3d_effect_lighting_and_shading_shader::RimLightCalcVSVS(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";

	out +=
		// �I�u�W�F�N�g�̗֊s�t�߂̋��x�������Ȃ�悤�ɂ���
		"\tfloat RimP = 1.0f - max( 0.0f, dot(normalize("+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_NORMAL)+".xyz), normalize(g_CameraWorldPos.xyz - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz)) );\n"
		// �������C�g��K������͈͂���ы��x�𒲐�����
		// ���̂�����̌v�Z�̓I�u�W�F�N�g�̃}�e���A���ɂ��C�ӂɕύX���邱��
		"\tRimP = RimP * g_RimLightPower;\n"
		"\tRimP = RimP * RimP;\n"
		// ���C�g���J�����̐��ʋ߂��ɂ���قǋ��x�������Ȃ�悤�ɂ���
		// �Ȃ̂� -g_LightWorldDir �ł͂Ȃ� g_LightWorldDir �Ő�����
		"\tfloat RimPower = max( 0.0f, dot(normalize(g_CameraWorldPos.xyz - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz), normalize(g_LightWorldDir.xyz)) );\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR, lt, st)+" = 1.0f + g_RimLightColor * RimPower * RimP;\n"

		//// ���������Ή���
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
		// �������C�g�̂������`�F�b�N�p
		//"\t$(Out.color0).rgb = 0.5f;\n"

		// �������C�g�J���[����Z����
		"\t$(Out.color0).rgb *= "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR, lt, st)+";\n"
		;

	return out;
}

// �������C�g(�s�N�Z���V�F�[�_�v�Z)������
MBSTR C_d3d_effect_lighting_and_shading_shader::RimLightCalcPSVS(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";
	out +=
		// ���C�g���J�����̐��ʋ߂��ɂ���قǋ��x�������Ȃ�悤�ɂ���
		"\t$(Out.tex7) = max( 0.0f, dot(normalize(E), normalize(-L)) );\n"
		;

	return "";
}
MBSTR C_d3d_effect_lighting_and_shading_shader::RimLightCalcPSPS(LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "";
	out +=
		// �I�u�W�F�N�g�̗֊s�t�߂̋��x�������Ȃ�悤�ɂ���
		"\tfloat P = 1.0f - max( 0.0f, dot(N, normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_EYE, lt, st)+".xyz)) );\n"
		// �������C�g��K������͈͂���ы��x�𒲐�����
		// ���̂�����̌v�Z�̓I�u�W�F�N�g�̃}�e���A���ɂ��C�ӂɕύX���邱��
		"\tP = P * g_RimLightPower;\n"
		"\tP = P * P;\n"
		"\t$(Out.color0) += g_RimLightColor * $(In.tex7) * P;\n"
		;
	return "";
}







}