// C_d3d_effect_texture_info.h

#pragma once

namespace NT3
{

class C_d3d_effect_texture_info {
	std::vector<std::string> texSemanticsNameAry;
public:

	C_d3d_effect_texture_info() {}
	~C_d3d_effect_texture_info() {}

	void addSemanticsName(const char* semanticsName) {
		texSemanticsNameAry.push_back(semanticsName);
	}
	bool find(const char* semanticsName) {
		for ( size_t i = 0; i < texSemanticsNameAry.size(); i++ ) {
			if (texSemanticsNameAry[i] == semanticsName)
				return true;
		}
		return false;
	}
};


}