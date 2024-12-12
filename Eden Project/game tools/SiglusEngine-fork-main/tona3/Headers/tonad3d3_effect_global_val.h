// C_d3d_effect_global_val.h

#pragma once


namespace NT3
{




class C_d3d_effect_global_val {
protected:
	MBSTR typeName;
	MBSTR valName;
	MBSTR semanticsName;
	MBSTR initName;

	int index_for_sort;

public:
	C_d3d_effect_global_val() {}
	C_d3d_effect_global_val(MBSTR typeName, MBSTR valName, MBSTR initName, MBSTR semanticsName) :
		typeName(typeName), valName(valName), initName(initName), semanticsName(semanticsName) {}
	C_d3d_effect_global_val(std::string typeName, std::string valName, std::string initName) :
		typeName(typeName), valName(valName), initName(initName) {}

	// �錾���o��
	virtual MBSTR decl() {
		MBSTR res = typeName + " " + valName;
		if (semanticsName.size()) {
			res += " : ";
			res += semanticsName;
		}
		if (initName.size()) {
			// �z��̏����l�𓯒l�Őݒ肷�邽�߂̏���
			// find�n�͌�����Ȃ��ꍇ��npos���Ԃ��Ă���
			if(initName.find("@") != MBSTR::npos) {
				// 3@(0,0)@ �݂����ȋL�q�œn������{(0,0),(0,0),(0,0)}�ɕϊ�����
				MBSTR::size_type first = initName.find("@");
				MBSTR::size_type last = initName.rfind("@");
				TSTR loop_num = MBSTR_to_TSTR(initName.substr(0, first));
				// @�̏ꏊ��Ԃ����̂� first��+1, last��-1
				// substr�͑������Ԗڂ�����������Ȃ̂�last�͍X��-first
				MBSTR body = initName.substr(first+1, last-1-first);

				res += " = {";
				res += body;
				// ��ɂP�������Ă�̂�-1
				for(int i = 0; i < str_to_int(loop_num, 0)-1; i++) {
					res += "," + body;
				}
				res += "}";
			}
			else {
				// ������Ȃ���Βʏ폈��
				res += " = " + initName;
			}
		}
		res += ";\n";
		return res;
	}

	// �ϐ����g�p
	virtual MBSTR n() const { return valName; }

	void	setIndex(int idx)	{index_for_sort = idx;}
	int		getIndex()			{return index_for_sort;}

		// �e�탁���o�ϐ��o��
#define OutStr(name)	virtual MBSTR name() const { return valName + "." + #name; }
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

	// ����p���Z�q
	operator MBSTR () {
		return valName;
	}

	// �Z�}���e�B�N�X�����擾
	MBSTR getSemanticsName() const { return semanticsName; }
};



}