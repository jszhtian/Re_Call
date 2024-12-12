#include "tonad3d3_pch.h"
#include "tonad3d3_effect_source_val_name_manager.h"

namespace NT3
{

C_d3d_effect_source_val_manager::C_d3d_effect_source_val_manager()
{
}

C_d3d_effect_source_val_manager::~C_d3d_effect_source_val_manager()
{
}

MBSTR get_effect_val_name(EFFECT_VAL_TYPE evt, LIGHTING_TYPE lt, SHADING_TYPE st)
{
	MBSTR out = "**/not found the val./**";

	// ここはtonad3d3_effect_lighting_and_shading_shader.h の restructSemanticsBit()と対応させる事

	if(false);

	// 影なし
	else if(st == SHADING_TYPE_NONE) {
		switch(lt) {
			case LIGHTING_TYPE_NONE:
				break;
			case LIGHTING_TYPE_LAMBERT:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR)	out = "$(Out.tex5)";
				else if(evt == EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR)	out = "$(In.tex5)";
				break;
			case LIGHTING_TYPE_BLINN_PHONG:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_DIFFUSE)			out = "$(Out.tex6)";
				else if(evt == EFFECT_VAL_TYPE_PSI_DIFFUSE)			out = "$(In.tex6)";
				else if(evt == EFFECT_VAL_TYPE_VSO_SPECULAR)		out = "$(Out.tex7)";
				else if(evt == EFFECT_VAL_TYPE_PSI_SPECULAR)		out = "$(In.tex7)";
				else if(evt == EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR)	out = "$(Out.tex5)";
				else if(evt == EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR)	out = "$(In.tex5)";
				break;
			case LIGHTING_TYPE_PP_BLINN_PHONG:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_NORMAL)	out = "$(Out.tex4)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_NORMAL)	out = "$(In.tex4)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_EYE)		out = "$(Out.tex5)";				
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_EYE)		out = "$(In.tex5)";
				else if(evt == EFFECT_VAL_TYPE_VSO_DIFFUSE)			out = "$(Out.tex6)";				
				else if(evt == EFFECT_VAL_TYPE_PSI_DIFFUSE)			out = "$(In.tex6)";
				else if(evt == EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR)	out = "$(Out.tex7)";
				else if(evt == EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR)	out = "$(In.tex7)";
				break;
			case LIGHTING_TYPE_PP_HALF_LAMBERT:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_NORMAL)	out = "$(Out.tex4)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_NORMAL)	out = "$(In.tex4)";
				else if(evt == EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR)	out = "$(Out.tex5)";
				else if(evt == EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR)	out = "$(In.tex5)";
				break;
			case LIGHTING_TYPE_TOON:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_NORMAL)	out = "$(Out.tex4)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_NORMAL)	out = "$(In.tex4)";
				else if(evt == EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR)	out = "$(Out.tex5)";
				else if(evt == EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR)	out = "$(In.tex5)";
				break;
			case LIGHTING_TYPE_FFP:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_DIFFUSE)			out = "$(Out.tex6)";
				else if(evt == EFFECT_VAL_TYPE_PSI_DIFFUSE)			out = "$(In.tex6)";
				else if(evt == EFFECT_VAL_TYPE_VSO_SPECULAR)		out = "$(Out.tex7)";
				else if(evt == EFFECT_VAL_TYPE_PSI_SPECULAR)		out = "$(In.tex7)";
				break;
			case LIGHTING_TYPE_PP_FFP:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_NORMAL)	out = "$(Out.tex6)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_NORMAL)	out = "$(In.tex6)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_POS)		out = "$(Out.tex7)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_POS)		out = "$(In.tex7)";
				break;
			case LIGHTING_TYPE_BUMP:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_NORMAL)	out = "$(Out.tex4)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_NORMAL)	out = "$(In.tex4)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_TANGENT)	out = "$(Out.tex5)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_TANGENT)	out = "$(In.tex5)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_BINORMAL)	out = "$(Out.tex6)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_BINORMAL)	out = "$(In.tex6)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_EYE)		out = "$(Out.tex7)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_EYE)		out = "$(In.tex7)";
				break;
			case LIGHTING_TYPE_PARALLAX:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_NORMAL)	out = "$(Out.tex4)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_NORMAL)	out = "$(In.tex4)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_TANGENT)	out = "$(Out.tex5)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_TANGENT)	out = "$(In.tex5)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_BINORMAL)	out = "$(Out.tex6)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_BINORMAL)	out = "$(In.tex6)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_EYE)		out = "$(Out.tex7)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_EYE)		out = "$(In.tex7)";
				break;
			default:
				break;
		}
	}

	// 深度バッファシャドウ
	else if(st == SHADING_TYPE_DEPTH_BUFFER) {
		switch(lt) {
			case LIGHTING_TYPE_NONE:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_SHADOW_MAP_UV)	out = "$(Out.tex2)";
				else if(evt == EFFECT_VAL_TYPE_PSI_SHADOW_MAP_UV)	out = "$(In.tex2)";
				else if(evt == EFFECT_VAL_TYPE_VSO_DEPTH)			out = "$(Out.tex3)";				
				else if(evt == EFFECT_VAL_TYPE_PSI_DEPTH)			out = "$(In.tex3)";
				break;
			case LIGHTING_TYPE_LAMBERT:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_SHADOW_MAP_UV)	out = "$(Out.tex2)";
				else if(evt == EFFECT_VAL_TYPE_PSI_SHADOW_MAP_UV)	out = "$(In.tex2)";
				else if(evt == EFFECT_VAL_TYPE_VSO_DEPTH)			out = "$(Out.tex3)";				
				else if(evt == EFFECT_VAL_TYPE_PSI_DEPTH)			out = "$(In.tex3)";
				else if(evt == EFFECT_VAL_TYPE_VSO_AMBIENT)			out = "$(Out.tex7)";
				else if(evt == EFFECT_VAL_TYPE_PSI_AMBIENT)			out = "$(In.tex7)";
				else if(evt == EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR)	out = "$(Out.tex5)";
				else if(evt == EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR)	out = "$(In.tex5)";
				break;
			case LIGHTING_TYPE_BLINN_PHONG:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_SHADOW_MAP_UV)	out = "$(Out.tex2)";
				else if(evt == EFFECT_VAL_TYPE_PSI_SHADOW_MAP_UV)	out = "$(In.tex2)";
				else if(evt == EFFECT_VAL_TYPE_VSO_DEPTH)			out = "$(Out.tex3)";				
				else if(evt == EFFECT_VAL_TYPE_PSI_DEPTH)			out = "$(In.tex3)";
				else if(evt == EFFECT_VAL_TYPE_VSO_AMBIENT)			out = "$(Out.tex4)";
				else if(evt == EFFECT_VAL_TYPE_PSI_AMBIENT)			out = "$(In.tex4)";
				else if(evt == EFFECT_VAL_TYPE_VSO_DIFFUSE)			out = "$(Out.tex6)";
				else if(evt == EFFECT_VAL_TYPE_PSI_DIFFUSE)			out = "$(In.tex6)";
				else if(evt == EFFECT_VAL_TYPE_VSO_SPECULAR)		out = "$(Out.tex7)";
				else if(evt == EFFECT_VAL_TYPE_PSI_SPECULAR)		out = "$(In.tex7)";
				else if(evt == EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR)	out = "$(Out.tex5)";
				else if(evt == EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR)	out = "$(In.tex5)";
				break;
			case LIGHTING_TYPE_PP_BLINN_PHONG:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_SHADOW_MAP_UV)	out = "$(Out.tex2)";
				else if(evt == EFFECT_VAL_TYPE_PSI_SHADOW_MAP_UV)	out = "$(In.tex2)";
				else if(evt == EFFECT_VAL_TYPE_VSO_DEPTH)			out = "$(Out.tex3)";				
				else if(evt == EFFECT_VAL_TYPE_PSI_DEPTH)			out = "$(In.tex3)";
				else if(evt == EFFECT_VAL_TYPE_VSO_AMBIENT)			out = "$(Out.tex7)";
				else if(evt == EFFECT_VAL_TYPE_PSI_AMBIENT)			out = "$(In.tex7)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_NORMAL)	out = "$(Out.tex4)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_NORMAL)	out = "$(In.tex4)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_EYE)		out = "$(Out.tex5)";				
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_EYE)		out = "$(In.tex5)";
				else if(evt == EFFECT_VAL_TYPE_VSO_DIFFUSE)			out = "$(Out.tex6)";				
				else if(evt == EFFECT_VAL_TYPE_PSI_DIFFUSE)			out = "$(In.tex6)";
				else if(evt == EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR)	out = "$(Out.tex1)";
				else if(evt == EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR)	out = "$(In.tex1)";
				break;
			case LIGHTING_TYPE_PP_HALF_LAMBERT:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_SHADOW_MAP_UV)	out = "$(Out.tex2)";
				else if(evt == EFFECT_VAL_TYPE_PSI_SHADOW_MAP_UV)	out = "$(In.tex2)";
				else if(evt == EFFECT_VAL_TYPE_VSO_DEPTH)			out = "$(Out.tex3)";				
				else if(evt == EFFECT_VAL_TYPE_PSI_DEPTH)			out = "$(In.tex3)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_NORMAL)	out = "$(Out.tex4)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_NORMAL)	out = "$(In.tex4)";
				else if(evt == EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR)	out = "$(Out.tex5)";
				else if(evt == EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR)	out = "$(In.tex5)";
				break;
			case LIGHTING_TYPE_TOON:
				if(false);
				else if(evt == EFFECT_VAL_TYPE_VSO_SHADOW_MAP_UV)	out = "$(Out.tex2)";
				else if(evt == EFFECT_VAL_TYPE_PSI_SHADOW_MAP_UV)	out = "$(In.tex2)";
				else if(evt == EFFECT_VAL_TYPE_VSO_DEPTH)			out = "$(Out.tex3)";				
				else if(evt == EFFECT_VAL_TYPE_PSI_DEPTH)			out = "$(In.tex3)";
				else if(evt == EFFECT_VAL_TYPE_VSO_WORLD_NORMAL)	out = "$(Out.tex4)";
				else if(evt == EFFECT_VAL_TYPE_PSI_WORLD_NORMAL)	out = "$(In.tex4)";
				else if(evt == EFFECT_VAL_TYPE_VSO_RIM_LIGHT_COLOR)	out = "$(Out.tex5)";
				else if(evt == EFFECT_VAL_TYPE_PSI_RIM_LIGHT_COLOR)	out = "$(In.tex5)";
				break;
			default:
				break;
		}
	}

	return out;
}


// 全タイプ共通
MBSTR get_effect_val_name(EFFECT_VAL_TYPE evt)
{
	MBSTR out = "**/not found the val./**";

	if(false);
	else if(evt == EFFECT_VAL_TYPE_VS_LOCAL_POS)		out = "$(In.pos)";
	else if(evt == EFFECT_VAL_TYPE_VS_WORLD_POS)		out = "WorldPos";
	else if(evt == EFFECT_VAL_TYPE_VS_LOCAL_NORMAL)		out = "$(In.normal)";
	else if(evt == EFFECT_VAL_TYPE_VS_WORLD_NORMAL)		out = "WorldNormal";
	else if(evt == EFFECT_VAL_TYPE_VS_LIGHT_BRIGHTNESS)		out = "LightBrightness";


	else if(evt == EFFECT_VAL_TYPE_PS_LIGHT_BRIGHTNESS)		out = "LightBrightness";

	return out;
}








}