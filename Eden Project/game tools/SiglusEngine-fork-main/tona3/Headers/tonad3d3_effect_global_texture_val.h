// C_d3d_effect_global_texture_val.h

#pragma once

#include "tonad3d3_effect_global_val.h"


namespace NT3
{




// �e�N�X�`���ϐ��p
class C_d3d_effect_global_texture_val : public C_d3d_effect_global_val {
public:
	C_d3d_effect_global_texture_val() {}
	C_d3d_effect_global_texture_val(MBSTR semanticsName);

	// �錾���o��
	virtual MBSTR decl() {
		MBSTR res = typeName + " " + valName;
		if (semanticsName.size()) {
			res += " : ";
			res += semanticsName;
		}
		res += ";\n";
		return res;
	}

};




}