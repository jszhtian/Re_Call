// C_d3d_effect_vertex_output_semantics_struct.h

#pragma once


#include "tonad3d3_effect_semantics_struct.h"


namespace NT3
{





class C_d3d_effect_vertex_output_semantics_struct : public C_d3d_effect_semantics_struct {
public:
	//BSP<SemanticsVal> pos, color, tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7, tex8;
	BSP<SemanticsVal> pos, color0, color1, tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7;

public:
	C_d3d_effect_vertex_output_semantics_struct() : C_d3d_effect_semantics_struct("") {}
	// float4ìùàÍÇ»ÇÃÇÕââéZÇ≈égÇ§éûóp
	C_d3d_effect_vertex_output_semantics_struct(std::string structName, unsigned semanticsBit) : C_d3d_effect_semantics_struct(structName),
		pos  (new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::POSITION     ? "pos"          : "**/error/**", "POSITION"    )),
		//color(new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::COLOR        ? "color"        : "**/error/**", "COLOR"       )),
		color0(new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::COLOR0        ? "color0"        : "**/error/**", "COLOR0"       )),
		color1(new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::COLOR1        ? "color1"        : "**/error/**", "COLOR1"       )),
		tex0 (new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::TEXCOORD0    ? "tex0"         : "**/error/**", "TEXCOORD0"   )),
		tex1 (new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::TEXCOORD1    ? "tex1"         : "**/error/**", "TEXCOORD1"   )),
		tex2 (new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::TEXCOORD2    ? "tex2"         : "**/error/**", "TEXCOORD2"   )),
		tex3 (new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::TEXCOORD3    ? "tex3"         : "**/error/**", "TEXCOORD3"   )),
		tex4 (new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::TEXCOORD4    ? "tex4"         : "**/error/**", "TEXCOORD4"   )),
		tex5 (new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::TEXCOORD5    ? "tex5"         : "**/error/**", "TEXCOORD5"   )),
		tex6 (new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::TEXCOORD6    ? "tex6"         : "**/error/**", "TEXCOORD6"   )),
		tex7 (new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::TEXCOORD7    ? "tex7"         : "**/error/**", "TEXCOORD7"   ))
//		tex8 (new SemanticsVal("Out", "float4", semanticsBit & C_d3d_effect_semantics_info::TEXCOORD8    ? "tex8"         : "**/error/**", "TEXCOORD8"   ))
	{
		// É}ÉbÉvÇ÷ìoò^
		//BSP<SemanticsVal> v[] = {pos, color, tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7, tex8};
		BSP<SemanticsVal> v[] = {pos, color0, color1, tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7};
		const int sz = sizeof(v) / sizeof(v[0]);
		for ( int i = 0; i < sz; i++ )
			valMap.insert( std::pair<std::string, BSP<SemanticsVal> >(v[i]->n(), v[i]) );
	}
	~C_d3d_effect_vertex_output_semantics_struct() {}

	std::string decl() {
		if(!SemanticsCheck()) return "";

		//BSP<SemanticsVal> v[] = {pos, color, tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7, tex8};
		BSP<SemanticsVal> v[] = {pos, color0, color1, tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7};
		std::string str = "struct " + this->structName + "{\n";
		for ( int i = 0; i < 11; i++ ) {
			if (v[i]->originValName() != "**/error/**")
				str += "\t" + v[i]->decl();
		}
		str += "};\n\n";
		return str;
	}
private:
	bool SemanticsCheck() {
		if(pos) return true;
		//if(color) return true;
		if(color0) return true;
		if(color1) return true;
		if(tex0) return true;
		if(tex1) return true;
		if(tex2) return true;
		if(tex3) return true;
		if(tex4) return true;
		if(tex5) return true;
		if(tex6) return true;
		if(tex7) return true;
		//if(tex8) return true;

		return false;
	}
};


}