// C_d3d_effect_semantics_struct.h

#pragma once


#include "tonad3d3_effect_semantics_info.h"
#include "tonad3d3_effect_global_val.h"


namespace NT3
{



class C_d3d_effect_semantics_struct {
public:
	// �Z�}���e�B�N�X�p�ϐ�
	class SemanticsVal : public C_d3d_effect_global_val {
		MBSTR structName;		// �\���̖�
	public:
		SemanticsVal() {}
		SemanticsVal(MBSTR structName, MBSTR typeName, MBSTR valName, MBSTR semanticsName) :
		  C_d3d_effect_global_val(typeName, valName, "", semanticsName), structName(structName) {}
		// �e�탁���o�ϐ��o��
#define OutStr(name)	virtual MBSTR name() const { return structName + "." + valName + "." + #name; }
		OutStr(x);
		OutStr(y);
		OutStr(z);
		OutStr(w);
		OutStr(r);
		OutStr(g);
		OutStr(b);
		OutStr(a);
		OutStr(xy);
		OutStr(xyz);
		OutStr(rg);
		OutStr(rgb);
#undef OutStr
		// �ϐ����g�p
		virtual MBSTR n() const { return structName + "." + valName; }

		// �ϐ������擾
		MBSTR originValName() const { return valName; }

		// ����p���Z�q
		operator MBSTR () {
			return structName + "." + valName;
		}
	};

protected:
	MBSTR structName;		// �\���̖�
	std::map<MBSTR, BSP<SemanticsVal> > valMap;	// �ϐ��}�b�v

public:
	C_d3d_effect_semantics_struct(MBSTR structName) : structName(structName) {}

	// ����
	bool find(MBSTR src, MBSTR&res) {
		std::map<MBSTR, BSP<SemanticsVal> >::iterator it = valMap.find(src);
		if ( it != valMap.end() ) {
			res = it->second->n();
			return true;
		}
		// �Ȃ��I
		return false;
	}
private:
	virtual bool SemanticsCheck() = 0;
};


}