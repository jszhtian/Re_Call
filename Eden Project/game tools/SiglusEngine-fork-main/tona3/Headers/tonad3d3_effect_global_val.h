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

	// 宣言部出力
	virtual MBSTR decl() {
		MBSTR res = typeName + " " + valName;
		if (semanticsName.size()) {
			res += " : ";
			res += semanticsName;
		}
		if (initName.size()) {
			// 配列の初期値を同値で設定するための処理
			// find系は見つからない場合はnposが返ってくる
			if(initName.find("@") != MBSTR::npos) {
				// 3@(0,0)@ みたいな記述で渡されると{(0,0),(0,0),(0,0)}に変換する
				MBSTR::size_type first = initName.find("@");
				MBSTR::size_type last = initName.rfind("@");
				TSTR loop_num = MBSTR_to_TSTR(initName.substr(0, first));
				// @の場所を返されるので firstは+1, lastは-1
				// substrは第一引数番目から第二引数個分なのでlastは更に-first
				MBSTR body = initName.substr(first+1, last-1-first);

				res += " = {";
				res += body;
				// 先に１個分書いてるので-1
				for(int i = 0; i < str_to_int(loop_num, 0)-1; i++) {
					res += "," + body;
				}
				res += "}";
			}
			else {
				// 見つからなければ通常処理
				res += " = " + initName;
			}
		}
		res += ";\n";
		return res;
	}

	// 変数名使用
	virtual MBSTR n() const { return valName; }

	void	setIndex(int idx)	{index_for_sort = idx;}
	int		getIndex()			{return index_for_sort;}

		// 各種メンバ変数出力
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

	// 代入用演算子
	operator MBSTR () {
		return valName;
	}

	// セマンティクス名を取得
	MBSTR getSemanticsName() const { return semanticsName; }
};



}