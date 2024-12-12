#include "tonad3d3_pch.h"
#include "tonad3d3_effect_global_texture_val.h"

namespace NT3
{



MBSTR toLowerStr(MBSTR& src)
{
	std::string res = src;
	for (size_t i = 0; i < res.size(); i++)
		res[i] = (char)tolower(res[i]);
	return res;
}


C_d3d_effect_global_texture_val::C_d3d_effect_global_texture_val(MBSTR semanticsName) :
	C_d3d_effect_global_val("texture", toLowerStr(semanticsName).c_str(), "", semanticsName)
{
}



}