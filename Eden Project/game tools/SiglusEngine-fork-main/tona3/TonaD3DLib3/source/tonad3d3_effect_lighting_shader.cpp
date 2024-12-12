#include "tonad3d3_pch.h"
#include "tonad3d3_effect_lighting_shader.h"
#include "tonad3d3_effect_source_val_name_manager.h"

namespace NT3
{

// ���_�V�F�[�_�R�[�h
MBSTR C_d3d_effect_lighting_shader::getVertexShaderCode(LIGHTING_TYPE type, MBSTR varNameNormal)
{
	MBSTR out = "";

	switch(type) {
		case LIGHTING_TYPE_NONE:																	break;
		case LIGHTING_TYPE_LAMBERT:			out = LambertVS(varNameNormal);							break;
		case LIGHTING_TYPE_BLINN_PHONG:		out = BlinnPhongVS(varNameNormal);						break;
		case LIGHTING_TYPE_PP_BLINN_PHONG:	out = PerPixelBlinnPhongVS(varNameNormal);				break;
		case LIGHTING_TYPE_PP_HALF_LAMBERT:	out = PerPixelHalfLambertVS(varNameNormal);				break;
		case LIGHTING_TYPE_TOON:			out = ToonVS(varNameNormal);							break;
		case LIGHTING_TYPE_FFP:				out = FixedFunctionPipelineVS(varNameNormal);			break;
		case LIGHTING_TYPE_PP_FFP:			out = PerPixelFixedFunctionPipelineVS(varNameNormal);	break;
		case LIGHTING_TYPE_BUMP:			out = BumpMappingVS(varNameNormal);						break;
		case LIGHTING_TYPE_PARALLAX:		out = ParallaxMappingVS(varNameNormal);					break;

		default:																					break;
	}

	return out;
}

// �s�N�Z���V�F�[�_�R�[�h
MBSTR C_d3d_effect_lighting_shader::getPixelShaderCode(LIGHTING_TYPE type)
{
	MBSTR out = "";

	switch(type) {
		case LIGHTING_TYPE_NONE:														break;
		case LIGHTING_TYPE_LAMBERT:			out = LambertPS();							break;
		case LIGHTING_TYPE_BLINN_PHONG:		out = BlinnPhongPS();						break;
		case LIGHTING_TYPE_PP_BLINN_PHONG:	out = PerPixelBlinnPhongPS();				break;
		case LIGHTING_TYPE_PP_HALF_LAMBERT:	out = PerPixelHalfLambertPS();				break;
		case LIGHTING_TYPE_TOON:			out = ToonPS();								break;
		case LIGHTING_TYPE_FFP:				out = FixedFunctionPipelinePS();			break;
		case LIGHTING_TYPE_PP_FFP:			out = PerPixelFixedFunctionPipelinePS();	break;
		case LIGHTING_TYPE_BUMP:			out = BumpMappingPS();						break;
		case LIGHTING_TYPE_PARALLAX:		out = ParallaxMappingPS();					break;
		default:																		break;
	}

	return out;
}


// �����o�[�g
MBSTR C_d3d_effect_lighting_shader::LambertVS(MBSTR varNameNormal)
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_LAMBERT;
	SHADING_TYPE st = SHADING_TYPE_NONE;
	
	out +=
#if 1
		// ���_�J���[
		"\tfloat4 L = -g_LightWorldDir;\n"
		// ���� * �g�U���ˌ�
		"\t$(Out.color0) = g_LightAmbient * g_MaterialAmbient + g_LightDiffuse * g_MaterialDiffuse * max(0, dot("+varNameNormal+", L));\n"
#else
		"\t$(Out.color0) = float4(0,0,0,1);\n"
		// �f�B���N�V���i��
		"\tfor(int i = 0; i < g_DLightNum; i++) {\n"
			"\t\tfloat4 AddColor = 0.0f;\n"
			"\t\tfloat4 L = -g_DLightWorldDir[i];\n"
			"\t\tfloat Brightness = max(0, dot("+varNameNormal+".xyz, L.xyz));\n"
			// ���� * �g�U���ˌ�
			"\t\tfloat4 Ambient = g_DLightAmbient[i] * g_MaterialAmbient;\n"
			"\t\tfloat4 Diffuse = g_DLightDiffuse[i] * g_MaterialDiffuse;\n"
			"\t\tAddColor = Ambient + Diffuse * Brightness;\n"
			"\t\t$(Out.color0) += AddColor;\n"
		"\t}\n"

		// �|�C���g
		"\tfor(int i = 0; i < g_PLightNum; i++) {\n"
			"\t\tfloat4 AddColor = 0.0f;\n"
			"\t\tfloat Attenuation = 0.0f;\n"
			// ���_���W - �|�C���g���C�g���W
			"\t\tfloat3 LightDir = "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+" - g_PLightWorldPos[i];\n"
			"\t\tfloat Dist = length(LightDir.xyz);\n"
			"\t\tfloat4 L = float4(0,0,0,1);\n"
			"\t\tfloat Brightness = 0.0f;\n"
			// (w������Range) �s�����t�ɂ���Ɠ������A�O���̓��C�e�B���O�ɂȂ邯�ǋ����������Ă����ƍ��ɖ߂�
			"\t\tif(Dist < g_PLightAttenuationsRange[i].w) {\n"
				// �����ɂ�錸��
				"\t\t\tAttenuation = 1 / (g_PLightAttenuationsRange[i].x + g_PLightAttenuationsRange[i].y * Dist + g_PLightAttenuationsRange[i].z * Dist * Dist);\n"
				"\t\t\tL.xyz = -normalize(LightDir.xyz);\n"
				// �P�x���Z�o
				"\t\t\tBrightness = max(0, dot("+varNameNormal+".xyz, L.xyz)) * Attenuation;\n"
			"\t\t}\n"

			"\t\tfloat4 Ambient = g_PLightAmbient[i] * g_MaterialAmbient;\n"
			"\t\tfloat4 Diffuse = g_PLightDiffuse[i] * g_MaterialDiffuse;\n"
			"\t\tAddColor = Ambient + Diffuse * Brightness;\n"
			"\t\t$(Out.color0) += AddColor;\n"
		"\t}\n"

		// �X�|�b�g
		"\tfor(int i = 0; i < g_SLightNum; i++) {\n"
			// ���_���W - �|�C���g���C�g���W
			"\t\tfloat3 SLightDir = g_SLightWorldPos[i] - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+";\n"
			"\t\tfloat Rho = dot(normalize(SLightDir.xyz), normalize(-g_SLightWorldDir[i].xyz));\n"
			"\t\tfloat CosTheta = (g_SLightThetaPhiFalloff[i].x);\n"
			"\t\tfloat CosPhi = (g_SLightThetaPhiFalloff[i].y);\n"
			"\t\tfloat SpotPower = 0.0f;\n"
			"\t\tif(Rho > CosTheta) {\n"
				// �ő�Ǝ�
				"\t\t\tSpotPower = 1.0f;\n"
			"\t\t}\n"
			"\t\telse if(Rho <= CosPhi) {\n"
				// ���C�e�B���O�Ȃ�
				// ��������0����Ă邩�����Ȃ�
			"\t\t}\n"
			"\t\telse {\n"
				// �X�|�b�g���C�g�v�Z
				"\t\t\tSpotPower = pow( (Rho - CosPhi) / (CosTheta - CosPhi), g_SLightThetaPhiFalloff[i].z);\n"
			"\t\t}\n"

			// �ȉ��A���������v�Z
			"\t\tfloat4 AddColor = 0.0f;\n"
			"\t\tfloat Attenuation = 0.0f;\n"
			"\t\tfloat3 LightDir = "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+" - g_SLightWorldPos[i];\n"
			"\t\tfloat Dist = length(LightDir.xyz);\n"
			"\t\tfloat4 L = float4(0,0,0,1);\n"
			"\t\tfloat Brightness = 0.0f;\n"
			// (w������Range) �s�����t�ɂ���Ɠ������A�O���̓��C�e�B���O�ɂȂ邯�ǋ����������Ă����ƍ��ɖ߂�
			"\t\tif(Dist < g_SLightAttenuationsRange[i].w) {\n"
				// �����ɂ�錸��
				"\t\t\tAttenuation = 1 / (g_SLightAttenuationsRange[i].x + g_SLightAttenuationsRange[i].y * Dist + g_SLightAttenuationsRange[i].z * Dist * Dist);\n"
				"\t\t\tL.xyz = -normalize(LightDir.xyz);\n"
				// �P�x���Z�o
				"\t\t\tBrightness = max(0, dot("+varNameNormal+".xyz, L.xyz)) * Attenuation;\n"
			"\t\t}\n"

			"\t\tfloat4 Ambient = g_SLightAmbient[i] * g_MaterialAmbient;\n"
			"\t\tfloat4 Diffuse = g_SLightDiffuse[i] * g_MaterialDiffuse;\n"
			"\t\tAddColor = Ambient + Diffuse * Brightness * SpotPower;\n"
			"\t\t$(Out.color0) += AddColor;\n"
		"\t}\n"
#endif

		;

	return out;
}
MBSTR C_d3d_effect_lighting_shader::LambertPS()
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_LAMBERT;
	SHADING_TYPE st = SHADING_TYPE_NONE;


	out +=
		// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
		"\t$(Out.color0).a = 1.0f;\n"
		;

	return out;
}


// �u�����t�H��
MBSTR C_d3d_effect_lighting_shader::BlinnPhongVS(MBSTR varNameNormal)
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_BLINN_PHONG;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
#if 1//SHADER_SINGLE_LIGHT
		"\tfloat3 N = "+varNameNormal+";\n"
		// ���[�J�����W�n�ł̃��C�g�x�N�g��
		"\tfloat3 L = -g_LightWorldDir;\n"
		//"\tfloat3 E = normalize(g_CameraLocalPos - $(In.pos).xyz);\n"
		"\tfloat3 E = normalize(g_CameraWorldPos - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz);\n"
		//"\tfloat3 E = normalize(g_CameraLocalPos - mul($(In.pos), mWorld).xyz);\n"
		//"\tfloat3 E = normalize(g_CameraLocalPos - mul($(In.pos), Comb-mWorld).xyz);\n"
		//"\tfloat3 E = normalize(g_CameraLocalPos - mul($(In.pos).xyz, (float3x3)Comb));\n"
		// �n�[�t�x�N�g��
		"\tfloat3 H = normalize(L+E);\n"
		
		"\tfloat4 diffuse = g_LightAmbient * g_MaterialAmbient + g_LightDiffuse * g_MaterialDiffuse * max(0, dot(N, L));\n"
		"\tfloat4 specular = pow(max(0,dot(N, H)), g_MaterialSpecularPower);\n"
		
		"\t$(Out.color0) = diffuse + specular;\n"

		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_DIFFUSE, lt, st)+" = diffuse;\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_SPECULAR, lt, st)+" = specular;\n"
#else
		"\t$(Out.color0) = float4(0,0,0,1);\n"
		"\tfloat4 AddDiffuse = float4(0,0,0,0);\n"
		"\tfloat4 AddAmbient = float4(0,0,0,0);\n"
		"\tfloat4 AddSpecular = float4(0,0,0,0);\n"

		"\tfloat3 E = normalize(g_CameraWorldPos - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz);\n"
		"\tfloat3 N = "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+";\n"

		// �f�B���N�V���i��
		"\tfor(int i = 0; i < g_DLightNum; i++) {\n"
			"\t\tfloat4 AddColor = 0.0f;\n"
			"\t\tfloat4 L = -g_DLightWorldDir[i];\n"
			"\t\tfloat Brightness = max(0, dot(N, L.xyz));\n"
			// �n�[�t�x�N�g��
			"\tfloat3 H = normalize(L+E);\n"

			// ���� * �g�U���ˌ�
			"\t\tfloat4 Ambient = g_DLightAmbient[i] * g_MaterialAmbient;\n"
			"\t\tfloat4 Diffuse = g_DLightDiffuse[i] * g_MaterialDiffuse;\n"
			"\t\tfloat4 Specular = g_DLightSpecular[i] * g_MaterialSpecular * pow(max(0,dot(N, H)), g_MaterialSpecularPower);\n"

			"\t\tAddColor = Ambient + Diffuse * Brightness;\n"
			"\t\t$(Out.color0) += AddColor;\n"
		"\t}\n"

		// �|�C���g
		"\tfor(int i = 0; i < g_PLightNum; i++) {\n"
			"\t\tfloat4 AddColor = 0.0f;\n"
			"\t\tfloat Attenuation = 0.0f;\n"
			// ���_���W - �|�C���g���C�g���W
			"\t\tfloat3 LightDir = "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+" - g_PLightWorldPos[i];\n"
			"\t\tfloat Dist = length(LightDir.xyz);\n"
			"\t\tfloat4 L = float4(0,0,0,1);\n"
			"\t\tfloat Brightness = 0.0f;\n"
			// (w������Range) �s�����t�ɂ���Ɠ������A�O���̓��C�e�B���O�ɂȂ邯�ǋ����������Ă����ƍ��ɖ߂�
			"\t\tif(Dist < g_PLightAttenuationsRange[i].w) {\n"
				// �����ɂ�錸��
				"\t\t\tAttenuation = 1 / (g_PLightAttenuationsRange[i].x + g_PLightAttenuationsRange[i].y * Dist + g_PLightAttenuationsRange[i].z * Dist * Dist);\n"
				"\t\t\tL.xyz = -normalize(LightDir.xyz);\n"
				// �P�x���Z�o
				"\t\t\tBrightness = max(0, dot("+varNameNormal+".xyz, L.xyz)) * Attenuation;\n"
			"\t\t}\n"

			"\t\tfloat4 Ambient = g_PLightAmbient[i] * g_MaterialAmbient;\n"
			"\t\tfloat4 Diffuse = g_PLightDiffuse[i] * g_MaterialDiffuse;\n"
			"\t\tAddColor = Ambient + Diffuse * Brightness;\n"
			"\t\t$(Out.color0) += AddColor;\n"
		"\t}\n"

		// �X�|�b�g
		"\tfor(int i = 0; i < g_SLightNum; i++) {\n"
			// ���_���W - �|�C���g���C�g���W
			"\t\tfloat3 SLightDir = g_SLightWorldPos[i] - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+";\n"
			"\t\tfloat Rho = dot(normalize(SLightDir.xyz), normalize(-g_SLightWorldDir[i].xyz));\n"
			"\t\tfloat CosTheta = (g_SLightThetaPhiFalloff[i].x);\n"
			"\t\tfloat CosPhi = (g_SLightThetaPhiFalloff[i].y);\n"
			"\t\tfloat SpotPower = 0.0f;\n"
			"\t\tif(Rho > CosTheta) {\n"
				// �ő�Ǝ�
				"\t\t\tSpotPower = 1.0f;\n"
			"\t\t}\n"
			"\t\telse if(Rho <= CosPhi) {\n"
				// ���C�e�B���O�Ȃ�
				// ��������0����Ă邩�����Ȃ�
			"\t\t}\n"
			"\t\telse {\n"
				// �X�|�b�g���C�g�v�Z
				"\t\t\tSpotPower = pow( (Rho - CosPhi) / (CosTheta - CosPhi), g_SLightThetaPhiFalloff[i].z);\n"
			"\t\t}\n"

			// �ȉ��A���������v�Z
			"\t\tfloat4 AddColor = 0.0f;\n"
			"\t\tfloat Attenuation = 0.0f;\n"
			"\t\tfloat3 LightDir = "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+" - g_SLightWorldPos[i];\n"
			"\t\tfloat Dist = length(LightDir.xyz);\n"
			"\t\tfloat4 L = float4(0,0,0,1);\n"
			"\t\tfloat Brightness = 0.0f;\n"
			// (w������Range) �s�����t�ɂ���Ɠ������A�O���̓��C�e�B���O�ɂȂ邯�ǋ����������Ă����ƍ��ɖ߂�
			"\t\tif(Dist < g_SLightAttenuationsRange[i].w) {\n"
				// �����ɂ�錸��
				"\t\t\tAttenuation = 1 / (g_SLightAttenuationsRange[i].x + g_SLightAttenuationsRange[i].y * Dist + g_SLightAttenuationsRange[i].z * Dist * Dist);\n"
				"\t\t\tL.xyz = -normalize(LightDir.xyz);\n"
				// �P�x���Z�o
				"\t\t\tBrightness = max(0, dot("+varNameNormal+".xyz, L.xyz)) * Attenuation;\n"
			"\t\t}\n"

			"\t\tfloat4 Ambient = g_SLightAmbient[i] * g_MaterialAmbient;\n"
			"\t\tfloat4 Diffuse = g_SLightDiffuse[i] * g_MaterialDiffuse;\n"
			"\t\tAddColor = Ambient + Diffuse * Brightness * SpotPower;\n"
			"\t\t$(Out.color0) += AddColor;\n"
		"\t}\n"
#endif
		;

	return out;
}
MBSTR C_d3d_effect_lighting_shader::BlinnPhongPS()
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_BLINN_PHONG;
	SHADING_TYPE st = SHADING_TYPE_NONE;
	out += 
		"\t$(Out.color0) = "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_DIFFUSE, lt, st)+" * $(meshSampler)($(In.tex0).xy) + "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_SPECULAR, lt, st)+";\n"
		// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
		"\t$(Out.color0).a = 1.0f;\n"
		;
	return out;
}


// �u�����t�H��(�p�[�s�N�Z��)
MBSTR C_d3d_effect_lighting_shader::PerPixelBlinnPhongVS(MBSTR varNameNormal)
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_PP_BLINN_PHONG;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
		"\tfloat3 N = "+varNameNormal+";\n"
		"\tfloat3 L = -g_LightWorldDir;\n"
		// ���X�^���C�U�Ő��`���(�H)�����̂Ő��K���̓s�N�Z���V�F�[�_��
		"\tfloat3 E = g_CameraWorldPos - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz;\n"

		"\tfloat4 diffuse = g_LightAmbient * g_MaterialAmbient + g_LightDiffuse * g_MaterialDiffuse * max(0, dot(N, L));\n"

		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = N;\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_EYE, lt, st)+".xyz = E;\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_DIFFUSE, lt, st)+" = diffuse;\n"

		"\t$(Out.color0) = $(In.color);\n"
		;

	return out;
}
MBSTR C_d3d_effect_lighting_shader::PerPixelBlinnPhongPS()
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_PP_BLINN_PHONG;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
		"\tfloat3 L = -g_LightWorldDir;\n"
		"\tfloat3 N = normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+".xyz);\n"
		"\tfloat3 H = normalize(L + normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_EYE, lt, st)+".xyz));\n"
		"\tfloat4 specular = pow(max(0,dot(N, H)), g_MaterialSpecularPower);\n"

		"\t$(Out.color0) = "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_DIFFUSE, lt, st)+" * $(meshSampler)($(In.tex0).xy) + specular;\n"
		// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
		"\t$(Out.color0).a = 1.0f;\n"
		;


	return out;
}


// �n�[�t�����o�[�g(�p�[�s�N�Z��)
MBSTR C_d3d_effect_lighting_shader::PerPixelHalfLambertVS(MBSTR varNameNormal)
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_PP_HALF_LAMBERT;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = "+varNameNormal+";\n"
		//"\t$(Out.color0) = float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		;

	return out;
}
MBSTR C_d3d_effect_lighting_shader::PerPixelHalfLambertPS()
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_PP_HALF_LAMBERT;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
		//"\tfloat p = dot(normalize($(In.tex4).xyz), -g_LightWorldDir.xyz);\n"
		"\tfloat p = g_LightAmbient * g_MaterialAmbient + g_LightDiffuse * g_MaterialDiffuse * max(0, dot(normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+".xyz), -g_LightWorldDir.xyz));\n"
		"\tp = p * 0.5f + 0.5f;\n"
		"\tp = p * p;\n"
		"\t$(Out.color0) = p * $(meshSampler)($(In.tex0).xy);\n"
		// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
		"\t$(Out.color0).a = 1.0f;\n"
		;

	return out;
}


// �t�����g�E�C���O�d�l�̃g�D�[�����ۂ�����
MBSTR C_d3d_effect_lighting_shader::ToonVS(MBSTR varNameNormal)
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_TOON;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
#if 0
		// VS�Ń��C�e�B���O�v�Z�����
		// �V���h�E�}�b�v����ł�鎞�ɖ�肪���������̂�PS�Ōv�Z����悤�ɕύX
		"\tfloat4 L = -g_LightWorldDir;\n"
		"\tfloat3 Lcolor = max(0, dot("+varNameNormal+", L));\n"

		// �Ō�� * 0.333 �Ƃ��̌W���𒲐����čI�����e�N�X�`���͈͂�L�����p����
		//"\tfloat toon = 0.0001f + ( Lcolor.x + Lcolor.y + Lcolor.z ) * 0.2f;\n"
		"\tfloat toon = 0.0001f + ( Lcolor.x + Lcolor.y + Lcolor.z ) * 0.333f;\n"

		// ���邳�ɉ������e�N�X�`�����W��^����
		// �g�D�[���V�F�[�h�ɗp����̂�X�����̂�
		// Y�͎b�肱�̈ʒu
		"\t$(Out.tex4).xy = float2(toon, 0.5f);\n"

		// �n�[�t�����o�[�g���ۂ��l����
		//"\t$(Out.tex4).xy = float2(toon, 0.5f)*0.5f+0.5f;\n"
#endif
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = "+varNameNormal+";\n"
		;

	return out;
}
MBSTR C_d3d_effect_lighting_shader::ToonPS()
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_TOON;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
#if 0
		// VS�Ń��C�e�B���O�v�Z�����
		// �V���h�E�}�b�v����ł�鎞�ɖ�肪���������̂�PS�Ōv�Z����悤�ɕύX
		"\tfloat2 ToonTexCoord = $(In.tex4).xy;\n"
		"\tfloat4 Temp = $(meshSampler)($(In.tex0).xy);\n"
		"\tfloat4 TexColorAdjToonSDW = $(toonSampler)(ToonTexCoord.xy);\n"

		"\tTemp *= TexColorAdjToonSDW;\n"

		"\t$(Out.color0) = Temp;\n"
#endif
		"\tfloat4 L = -g_LightWorldDir;\n"
		"\tfloat3 N = normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+");\n"
		"\tfloat3 Lbrightness = max(0, dot(N, L));\n"

		// �Ō�� * 0.333 �Ƃ��̌W���𒲐����čI�����e�N�X�`���͈͂�L�����p����
		//"\tfloat toon = 0.0001f + ( Lbrightness.x + Lbrightness.y + Lbrightness.z ) * 0.2f;\n"
		"\tfloat toon = 0.0001f + ( Lbrightness.x + Lbrightness.y + Lbrightness.z ) * 0.333f;\n"

		// ���邳�ɉ������e�N�X�`�����W��^����
		// �g�D�[���V�F�[�h�ɗp����̂�X�����̂�
		// Y�͎b�肱�̈ʒu
		"\tfloat2 ToonTexCoord = float2(toon, 0.5f);\n"
		// �n�[�t�����o�[�g���ۂ��l����
		//"\tfloat2 ToonTexCoord = float2(toon, 0.5f)*0.5f+0.5f;\n"

		"\tfloat4 Temp = $(meshSampler)($(In.tex0).xy);\n"
		"\tfloat4 TexColorAdjToonSDW = $(toonSampler)(ToonTexCoord.xy);\n"

		"\tTemp *= TexColorAdjToonSDW;\n"

		"\t$(Out.color0) = Temp;\n"
		// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
		"\t$(Out.color0).a = 1.0f;\n"
		;

	return out;
}


// �Œ�@�\�p�C�v���C�����Č�������
MBSTR C_d3d_effect_lighting_shader::FixedFunctionPipelineVS(MBSTR varNameNormal)
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_FFP;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	MBSTR DAmb = "g_DLightAmbient[i] * g_MaterialAmbient";
	MBSTR DDif = "g_DLightDiffuse[i] * g_MaterialDiffuse";
	MBSTR DBri = "max(0, dot(N, L.xyz))";
	MBSTR DSpe = "g_DLightSpecular[i] * g_MaterialSpecular * pow(max(0,dot(N, H)), g_MaterialSpecularPower)";

	MBSTR PAmb = "g_PLightAmbient[i] * g_MaterialAmbient";
	MBSTR PDif = "g_PLightDiffuse[i] * g_MaterialDiffuse";
	MBSTR PBri = "max(0, dot("+varNameNormal+".xyz, L.xyz)) * Attenuation";
	MBSTR PSpe = "g_PLightSpecular[i] * g_MaterialSpecular * pow(max(0,dot(N, H)), g_MaterialSpecularPower) * Attenuation";

	MBSTR SAmb = "g_SLightAmbient[i] * g_MaterialAmbient";
	MBSTR SDif = "g_SLightDiffuse[i] * g_MaterialDiffuse";
	MBSTR SBri = "max(0, dot("+varNameNormal+".xyz, L.xyz)) * Attenuation";
	MBSTR SSpe = "g_SLightSpecular[i] * g_MaterialSpecular * pow(max(0,dot(N, H)), g_MaterialSpecularPower) * Attenuation";

	// oD = gA * mA + mE + (������)
	// oS = (������)
	// fC = tex * oD + Os
	out +=
		"\t$(Out.color0) = float4(0,0,0,1);\n"
		"\tfloat4 AddDiffuse = float4(0,0,0,0);\n"
		//"\tfloat4 AddAmbient = float4(0,0,0,0);\n"
		"\tfloat4 AddSpecular = float4(0,0,0,0);\n"

		"\tfloat3 E = normalize(g_CameraWorldPos - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz);\n"
		"\tfloat3 N = "+varNameNormal+";\n"

		// �f�B���N�V���i��
		"\tfor(int i = 0; i < g_DLightNum; i++) {\n"
			"\t\tfloat4 L = -g_DLightWorldDir[i];\n"
			// �n�[�t�x�N�g��
			"\tfloat3 H = normalize(L+E);\n"

			// ���C�g�̐��������Z���Ă���
			"\t\tAddDiffuse += "+DAmb+" + "+DDif+" * "+DBri+";\n"
			"\t\tAddSpecular += "+DSpe+";\n"
		"\t}\n"

		// �|�C���g
		"\tfor(int i = 0; i < g_PLightNum; i++) {\n"
			"\t\tfloat Attenuation = 0.0f;\n"
			// ���_���W - �|�C���g���C�g���W
			"\t\tfloat3 LightDir = "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+" - g_PLightWorldPos[i];\n"
			"\t\tfloat Dist = length(LightDir.xyz);\n"
			"\t\tfloat4 L = float4(0,0,0,1);\n"
			"\t\tfloat Brightness = 0.0f;\n"
			"\tfloat3 H = float3(0,1,0);\n"
			// (w������Range) �s�����t�ɂ���Ɠ������A�O���̓��C�e�B���O�ɂȂ邯�ǋ����������Ă����ƍ��ɖ߂�
			"\t\tif(Dist < g_PLightAttenuationsRange[i].w) {\n"
				// �����ɂ�錸��
				"\t\t\tAttenuation = 1 / (g_PLightAttenuationsRange[i].x + g_PLightAttenuationsRange[i].y * Dist + g_PLightAttenuationsRange[i].z * Dist * Dist);\n"
				"\t\t\tL.xyz = -normalize(LightDir.xyz);\n"
				"\t\tH = normalize(L+E);\n"
			"\t\t}\n"

			// ���C�g�̐��������Z���Ă���
			"\t\tAddDiffuse += "+PAmb+" + "+PDif+" * "+PBri+";\n"
			"\t\tAddSpecular += "+PSpe+";\n"
		"\t}\n"

		// �X�|�b�g
		"\tfor(int i = 0; i < g_SLightNum; i++) {\n"
			// ���_���W - �|�C���g���C�g���W
			"\t\tfloat3 SLightDir = g_SLightWorldPos[i] - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+";\n"
			"\t\tfloat Rho = dot(normalize(SLightDir.xyz), normalize(-g_SLightWorldDir[i].xyz));\n"
			"\t\tfloat CosTheta = (g_SLightThetaPhiFalloff[i].x);\n"
			"\t\tfloat CosPhi = (g_SLightThetaPhiFalloff[i].y);\n"
			"\t\tfloat SpotPower = 0.0f;\n"
			"\t\tif(Rho > CosTheta) {\n"
				// �ő�Ǝ�
				"\t\t\tSpotPower = 1.0f;\n"
			"\t\t}\n"
			"\t\telse if(Rho <= CosPhi) {\n"
				// ���C�e�B���O�Ȃ�
				// ��������0����Ă邩�����Ȃ�
			"\t\t}\n"
			"\t\telse {\n"
				// �X�|�b�g���C�g�v�Z
				"\t\t\tSpotPower = pow( (Rho - CosPhi) / (CosTheta - CosPhi), g_SLightThetaPhiFalloff[i].z);\n"
			"\t\t}\n"

			// �ȉ��A���������v�Z
			"\t\tfloat Attenuation = 0.0f;\n"
			"\t\tfloat3 LightDir = "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+" - g_SLightWorldPos[i];\n"
			"\t\tfloat Dist = length(LightDir.xyz);\n"
			"\t\tfloat4 L = float4(0,0,0,1);\n"
			"\t\tfloat Brightness = 0.0f;\n"
			"\tfloat3 H = float3(0,1,0);\n"
			// (w������Range) �s�����t�ɂ���Ɠ������A�O���̓��C�e�B���O�ɂȂ邯�ǋ����������Ă����ƍ��ɖ߂�
			"\t\tif(Dist < g_SLightAttenuationsRange[i].w) {\n"
				// �����ɂ�錸��
				"\t\t\tAttenuation = 1 / (g_SLightAttenuationsRange[i].x + g_SLightAttenuationsRange[i].y * Dist + g_SLightAttenuationsRange[i].z * Dist * Dist);\n"
				"\t\t\tL.xyz = -normalize(LightDir.xyz);\n"
				"\t\tH = normalize(L+E);\n"
			"\t\t}\n"

			// ���C�g�̐��������Z���Ă���
			"\t\tAddDiffuse += "+SAmb+" + "+SDif+" * "+SBri+" * SpotPower;\n"
			"\t\tAddSpecular += "+SSpe+" * SpotPower;\n"
		"\t}\n"
		
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_DIFFUSE, lt, st)+" = g_GlobalAmbient * g_MaterialAmbient + g_MaterialEmissive + AddDiffuse;\n"
		//"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_DIFFUSE, lt, st)+" = 0;\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_SPECULAR, lt, st)+" = AddSpecular;\n"

		;

	return out;
}
MBSTR C_d3d_effect_lighting_shader::FixedFunctionPipelinePS()
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_FFP;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
		"\t$(Out.color0) = $(meshSampler)($(In.tex0).xy) * "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_DIFFUSE, lt, st)+" + "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_SPECULAR, lt, st)+";\n"
		// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
		"\t$(Out.color0).a = 1.0f;\n"
		;

	return out;
}

// �Œ�@�\�p�C�v���C�����Č�������(�p�[�s�N�Z��)
// ������̃V�F�[�_���f��2.0���ƃs�N�Z���ŎZ�o����̂͂قڕs�\���ۂ�
MBSTR C_d3d_effect_lighting_shader::PerPixelFixedFunctionPipelineVS(MBSTR varNameNormal)
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_PP_FFP;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=		
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = "+varNameNormal+";\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_POS, lt, st)+".xyz = "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+";\n"

		;

	return out;
}
MBSTR C_d3d_effect_lighting_shader::PerPixelFixedFunctionPipelinePS()
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_PP_FFP;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	MBSTR DAmb = "g_DLightAmbient[i] * g_MaterialAmbient";
	MBSTR DDif = "g_DLightDiffuse[i] * g_MaterialDiffuse";
	MBSTR DBri = "max(0, dot(N, L.xyz))";
	MBSTR DSpe = "g_DLightSpecular[i] * g_MaterialSpecular * pow(max(0,dot(N, H)), g_MaterialSpecularPower)";

	MBSTR PAmb = "g_PLightAmbient[i] * g_MaterialAmbient";
	MBSTR PDif = "g_PLightDiffuse[i] * g_MaterialDiffuse";
	MBSTR PBri = "max(0, dot("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+".xyz, L.xyz)) * Attenuation";
	MBSTR PSpe = "g_PLightSpecular[i] * g_MaterialSpecular * pow(max(0,dot(N, H)), g_MaterialSpecularPower) * Attenuation";

	MBSTR SAmb = "g_SLightAmbient[i] * g_MaterialAmbient";
	MBSTR SDif = "g_SLightDiffuse[i] * g_MaterialDiffuse";
	MBSTR SBri = "max(0, dot("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+".xyz, L.xyz)) * Attenuation";
	MBSTR SSpe = "g_SLightSpecular[i] * g_MaterialSpecular * pow(max(0,dot(N, H)), g_MaterialSpecularPower) * Attenuation";

	// oD = gA * mA + mE + (������)
	// oS = (������)
	// fC = tex * oD + Os
	out +=
		"\t$(Out.color0) = float4(0,0,0,1);\n"
		"\tfloat4 AddDiffuse = float4(0,0,0,0);\n"
		//"\tfloat4 AddAmbient = float4(0,0,0,0);\n"
		"\tfloat4 AddSpecular = float4(0,0,0,0);\n"

		"\tfloat3 E = normalize(g_CameraWorldPos - "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_POS, lt, st)+".xyz);\n"
		"\tfloat3 N = "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+";\n"

		// �f�B���N�V���i��
		"\tfor(int i = 0; i < g_DLightNum; i++) {\n"
			"\t\tfloat4 L = -g_DLightWorldDir[i];\n"
			// �n�[�t�x�N�g��
			"\tfloat3 H = normalize(L+E);\n"

			// ���C�g�̐��������Z���Ă���
			"\t\tAddDiffuse += "+DAmb+" + "+DDif+" * "+DBri+";\n"
			"\t\tAddSpecular += "+DSpe+";\n"
		"\t}\n"

		// �|�C���g
		"\tfor(int i = 0; i < g_PLightNum; i++) {\n"
			"\t\tfloat Attenuation = 0.0f;\n"
			// ���_���W - �|�C���g���C�g���W
			"\t\tfloat3 LightDir = "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_POS, lt, st)+" - g_PLightWorldPos[i];\n"
			"\t\tfloat Dist = length(LightDir.xyz);\n"
			"\t\tfloat4 L = float4(0,0,0,1);\n"
			"\t\tfloat Brightness = 0.0f;\n"
			"\tfloat3 H = float3(0,1,0);\n"
			// (w������Range) �s�����t�ɂ���Ɠ������A�O���̓��C�e�B���O�ɂȂ邯�ǋ����������Ă����ƍ��ɖ߂�
			"\t\tif(Dist < g_PLightAttenuationsRange[i].w) {\n"
				// �����ɂ�錸��
				"\t\t\tAttenuation = 1 / (g_PLightAttenuationsRange[i].x + g_PLightAttenuationsRange[i].y * Dist + g_PLightAttenuationsRange[i].z * Dist * Dist);\n"
				"\t\t\tL.xyz = -normalize(LightDir.xyz);\n"
				"\t\tH = normalize(L+E);\n"
			"\t\t}\n"

			// ���C�g�̐��������Z���Ă���
			"\t\tAddDiffuse += "+PAmb+" + "+PDif+" * "+PBri+";\n"
			"\t\tAddSpecular += "+PSpe+";\n"
		"\t}\n"

		// �X�|�b�g
		"\tfor(int i = 0; i < g_SLightNum; i++) {\n"
			// ���_���W - �|�C���g���C�g���W
			"\t\tfloat3 SLightDir = g_SLightWorldPos[i] - "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_POS, lt, st)+";\n"
			"\t\tfloat Rho = dot(normalize(SLightDir.xyz), normalize(-g_SLightWorldDir[i].xyz));\n"
			"\t\tfloat CosTheta = (g_SLightThetaPhiFalloff[i].x);\n"
			"\t\tfloat CosPhi = (g_SLightThetaPhiFalloff[i].y);\n"
			"\t\tfloat SpotPower = 0.0f;\n"
			"\t\tif(Rho > CosTheta) {\n"
				// �ő�Ǝ�
				"\t\t\tSpotPower = 1.0f;\n"
			"\t\t}\n"
			"\t\telse if(Rho <= CosPhi) {\n"
				// ���C�e�B���O�Ȃ�
				// ��������0����Ă邩�����Ȃ�
			"\t\t}\n"
			"\t\telse {\n"
				// �X�|�b�g���C�g�v�Z
				"\t\t\tSpotPower = pow( (Rho - CosPhi) / (CosTheta - CosPhi), g_SLightThetaPhiFalloff[i].z);\n"
			"\t\t}\n"

			// �ȉ��A���������v�Z
			"\t\tfloat Attenuation = 0.0f;\n"
			"\t\tfloat3 LightDir = "+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_POS, lt, st)+" - g_SLightWorldPos[i];\n"
			"\t\tfloat Dist = length(LightDir.xyz);\n"
			"\t\tfloat4 L = float4(0,0,0,1);\n"
			"\t\tfloat Brightness = 0.0f;\n"
			"\tfloat3 H = float3(0,1,0);\n"
			// (w������Range) �s�����t�ɂ���Ɠ������A�O���̓��C�e�B���O�ɂȂ邯�ǋ����������Ă����ƍ��ɖ߂�
			"\t\tif(Dist < g_SLightAttenuationsRange[i].w) {\n"
				// �����ɂ�錸��
				"\t\t\tAttenuation = 1 / (g_SLightAttenuationsRange[i].x + g_SLightAttenuationsRange[i].y * Dist + g_SLightAttenuationsRange[i].z * Dist * Dist);\n"
				"\t\t\tL.xyz = -normalize(LightDir.xyz);\n"
				"\t\tH = normalize(L+E);\n"
			"\t\t}\n"

			// ���C�g�̐��������Z���Ă���
			"\t\tAddDiffuse += "+SAmb+" + "+SDif+" * "+SBri+" * SpotPower;\n"
			"\t\tAddSpecular += "+SSpe+";\n"
		"\t}\n"
		
		"\tfloat4 Diffuse = g_GlobalAmbient * g_MaterialAmbient + g_MaterialEmissive + AddDiffuse;\n"
		//"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_DIFFUSE, lt, st)+" = 0;\n"
		"\tfloat4 Specular = AddSpecular;\n"

		"\t$(Out.color0) = $(meshSampler)($(In.tex0).xy) * Diffuse + Specular;\n"
		// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
		"\t$(Out.color0).a = 1.0f;\n"
		;

	return out;
}




// �o���v�}�b�s���O
MBSTR C_d3d_effect_lighting_shader::BumpMappingVS(MBSTR varNameNormal)
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_BUMP;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
		//"\tfloat3x3 tbn_matrix;\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_TANGENT, lt, st)+".xyz = mul($(In.tangent), (float3x3)Comb);\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_BINORMAL, lt, st)+".xyz = mul($(In.binormal), (float3x3)Comb);\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = mul($(In.normal), (float3x3)Comb);\n"
		//"\ttbn_matrix = transpose(tbn_matrix);\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_EYE, lt, st)+".xyz = g_CameraWorldPos - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz;\n"
		//"\t$(Out.color0) = float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		;

	return out;
}
MBSTR C_d3d_effect_lighting_shader::BumpMappingPS()
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_BUMP;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
		"\tfloat3x3 mTangentToWorld = float3x3(normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_TANGENT, lt, st)+".xyz), normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_BINORMAL, lt, st)+".xyz), normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+".xyz));\n"
		"\tfloat3 NormalMapNormal = 2.0f * $(normalSampler)($(In.tex0).xy).xyz - 1.0f;\n"
		"\tfloat3 N = normalize(mul(NormalMapNormal, mTangentToWorld));\n"
		// ���ɒ��� float3 L = normalize(-g_LightWorldDir) ���Ɛ��K����4D�ōs���A���̌��ʂ�XYZ���g�p����̂ł��������Ȃ�
		"\tfloat3 L = normalize(-g_LightWorldDir.xyz);\n"
		"\tfloat3 E = normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_EYE, lt, st)+");\n"
		"\tfloat3 H = normalize(L+E);\n"
		//"\tfloat3 R = reflect(-E, N);\n"
		//"\t$(Out.color0) = pow(max(0,dot(N, H)), 10);\n"
		//"\t$(Out.color0) = pow(max(0,dot(R, L)), 10);\n"
		//"\t$(Out.color0) = (max(0, dot(N, L)));\n"
		"\t$(Out.color0) = $(meshSampler)($(In.tex0).xy) * (max(0, dot(N, L))) + 1.3f * pow(max(0,dot(N, H)), 100);\n"
		//"\t$(Out.color0) = $(meshSampler)($(In.tex0).xy) * (max(0, dot(N, L))) + 1.3f * pow(max(0,dot(R, L)), 10);\n"
		//"\t$(Out.color0) = $(meshSampler)($(In.tex0).xy) * (max(0, dot(N, L)));\n"
		// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
		"\t$(Out.color0).a = 1.0f;\n"
		;

	return out;
}


// �����}�b�s���O
MBSTR C_d3d_effect_lighting_shader::ParallaxMappingVS(MBSTR varNameNormal)
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_PARALLAX;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
		//"\tfloat3x3 tbn_matrix;\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_TANGENT, lt, st)+".xyz = mul($(In.tangent), (float3x3)Comb);\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_BINORMAL, lt, st)+".xyz = mul($(In.binormal), (float3x3)Comb);\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_NORMAL, lt, st)+".xyz = mul($(In.normal), (float3x3)Comb);\n"
		//"\ttbn_matrix = transpose(tbn_matrix);\n"
		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VSO_WORLD_EYE, lt, st)+".xyz = g_CameraWorldPos - "+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_POS)+".xyz;\n"
		//"\t$(Out.color0) = float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		;

	return out;
}
MBSTR C_d3d_effect_lighting_shader::ParallaxMappingPS()
{
	MBSTR out = "";
	LIGHTING_TYPE lt = LIGHTING_TYPE_PARALLAX;
	SHADING_TYPE st = SHADING_TYPE_NONE;

	out +=
		// �������o���̂Ɏg�p����̂Ő�ɎZ�o
		"\tfloat3 E = normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_EYE, lt, st)+");\n"
		"\tfloat height = $(normalSampler)($(In.tex0).xy).a;\n"
		// 0.016f�͍����̍ő�l�@�傫����������ƃJ�����������ƋC��������
		"\tfloat2 uv = $(In.tex0).xy + g_ParallaxMaxHeight * height * E.xy;\n"

		"\tfloat3 NormalMapNormal = 2.0f * $(normalSampler)(uv).xyz - 1.0f;\n"
		"\tfloat3x3 mTangentToWorld = float3x3(normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_TANGENT, lt, st)+".xyz), normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_BINORMAL, lt, st)+".xyz), normalize("+get_effect_val_name(EFFECT_VAL_TYPE_PSI_WORLD_NORMAL, lt, st)+".xyz));\n"
		
		"\tfloat3 N = normalize(mul(NormalMapNormal, mTangentToWorld));\n"
		// ���ɒ��� float3 L = normalize(-g_LightWorldDir) ���Ɛ��K����4D�ōs���A���̌��ʂ�XYZ���g�p����̂ł��������Ȃ�
		"\tfloat3 L = normalize(-g_LightWorldDir.xyz);\n"
		
		"\tfloat3 H = normalize(L+E);\n"
		//"\tfloat3 R = reflect(-E, N);\n"
		//"\t$(Out.color0) = pow(max(0,dot(N, H)), 10);\n"
		//"\t$(Out.color0) = pow(max(0,dot(R, L)), 10);\n"
		//"\t$(Out.color0) = (max(0, dot(N, L)));\n"
		"\t$(Out.color0) = $(meshSampler)(uv) * (max(0, dot(N, L))) + 1.3f * pow(max(0,dot(N, H)), 100);\n"
		//"\t$(Out.color0) = $(meshSampler)($(In.tex0).xy) * (max(0, dot(N, L))) + 1.3f * pow(max(0,dot(R, L)), 10);\n"
		//"\t$(Out.color0) = $(meshSampler)($(In.tex0).xy) * (max(0, dot(N, L)));\n"
		// �Ƃ肠����1.0f�����Ă���/*�v�C��*/
		"\t$(Out.color0).a = 1.0f;\n"
		;

	return out;
}










}