// C_d3d_effect_semantics_struct.h

#pragma once


#include "tonad3d3_effect_semantics_info.h"
#include "tonad3d3_effect_global_val.h"


namespace NT3
{



class C_d3d_effect_semantics_struct {
public:
	// セマンティクス用変数
	class SemanticsVal : public C_d3d_effect_global_val {
		MBSTR structName;		// 構造体名
	public:
		SemanticsVal() {}
		SemanticsVal(MBSTR structName, MBSTR typeName, MBSTR valName, MBSTR semanticsName) :
		  C_d3d_effect_global_val(typeName, valName, "", semanticsName), structName(structName) {}
		// 各種メンバ変数出力
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
		// 変数名使用
		virtual MBSTR n() const { return structName + "." + valName; }

		// 変数原名取得
		MBSTR originValName() const { return valName; }

		// 代入用演算子
		operator MBSTR () {
			return structName + "." + valName;
		}
	};

protected:
	MBSTR structName;		// 構造体名
	std::map<MBSTR, BSP<SemanticsVal> > valMap;	// 変数マップ

public:
	C_d3d_effect_semantics_struct(MBSTR structName) : structName(structName) {}

	// 検索
	bool find(MBSTR src, MBSTR&res) {
		std::map<MBSTR, BSP<SemanticsVal> >::iterator it = valMap.find(src);
		if ( it != valMap.end() ) {
			res = it->second->n();
			return true;
		}
		// ない！
		return false;
	}
private:
	virtual bool SemanticsCheck() = 0;
};


}