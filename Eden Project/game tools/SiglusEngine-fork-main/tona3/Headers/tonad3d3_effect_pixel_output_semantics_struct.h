// C_d3d_effect_pixel_output_semantics_struct.h

#pragma once
#include "tonad3d3_effect_semantics_struct.h"


namespace NT3
{





class C_d3d_effect_pixel_output_semantics_struct : public C_d3d_effect_semantics_struct {
public:
	//BSP<SemanticsVal> color0, color1, color2, color3, depth;
	BSP<SemanticsVal> color0, color1, depth;

public:
	C_d3d_effect_pixel_output_semantics_struct() : C_d3d_effect_semantics_struct("") {}
	C_d3d_effect_pixel_output_semantics_struct(std::string structName, unsigned semanticsBit) : C_d3d_effect_semantics_struct(structName),
		color0(new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::COLOR0 ? "color0" : "**/error/**", "COLOR0" )),
		color1(new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::COLOR1 ? "color1" : "**/error/**", "COLOR1" )),
//		color2(new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::COLOR2 ? "color2" : "**/error/**", "COLOR2" )),
//		color3(new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::COLOR3 ? "color3" : "**/error/**", "COLOR3" )),
		depth (new SemanticsVal("Out", "float" , semanticsBit & C_d3d_effect_semantics_info::DEPTH  ? "depth"  : "**/error/**", "DEPTH"  ))
	{
		// ƒ}ƒbƒv‚Ö“o˜^
		//BSP<SemanticsVal> v[] = {color0, color1, color2, color3, depth};
		BSP<SemanticsVal> v[] = {color0, color1, depth};
		const int sz = sizeof(v) / sizeof(v[0]);
		for ( int i = 0; i < sz; i++ )
			valMap.insert( std::pair<std::string, BSP<SemanticsVal> >(v[i]->n(), v[i]) );
	}
	~C_d3d_effect_pixel_output_semantics_struct() {}

	std::string decl() {
		if(!SemanticsCheck()) return "";

		//BSP<SemanticsVal> v[] = {color0, color1, color2, color3, depth};
		BSP<SemanticsVal> v[] = {color0, color1, depth};
		const int sz = sizeof(v) / sizeof(v[0]);
		std::string str = "struct " + this->structName + "{\n";
		for ( int i = 0; i < sz; i++ ) {
			if (v[i]->originValName() != "**/error/**")
				str += "\t" + v[i]->decl();
		}
		str += "};\n\n";
		return str;
	}
private:
	bool SemanticsCheck() {
		if(color0) return true;
		if(color1) return true;
		//if(color2) return true;
		//if(color3) return true;
		if(depth) return true;

		return false;
	}
};



}