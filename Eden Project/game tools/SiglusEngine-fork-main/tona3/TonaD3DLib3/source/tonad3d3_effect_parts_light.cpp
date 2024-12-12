#include "tonad3d3_pch.h"
#include "tonad3d3_effect_parts_light.h"
#include "tonad3d3_effect_source_val_name_manager.h"

namespace NT3
{



MBSTR get_brightness_code_vs()
{
	MBSTR out = "";

	out =
		"\tfloat4 AddBrightness(0.0f, 0.0f, 0.0f, 1.0f);\n"
		"\tfor(int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++) {\n"
			"\t\tAddBrightness += max(0, dot("+get_effect_val_name(EFFECT_VAL_TYPE_VS_WORLD_NORMAL)+", -g_LightWorldDir));\n"
		"\t}\n"
		"\tfor(int i = 0; i < POINT_LIGHT_MAX; i++) {\n"
		"\t}\n"
		"\tfor(int i = 0; i < SPOT_LIGHT_MAX; i++) {\n"
		"\t}\n"

		"\t"+get_effect_val_name(EFFECT_VAL_TYPE_VS_LIGHT_BRIGHTNESS)+" = AddBrightness;\n"
		;

	return out;
}


























}