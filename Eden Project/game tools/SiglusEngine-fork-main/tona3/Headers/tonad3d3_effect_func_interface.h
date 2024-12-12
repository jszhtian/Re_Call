#pragma once


namespace NT3
{

// シェーダ関数インターフェースクラス
// 厳密に言うと純粋仮想だけのクラスではないのでインターフェースではない
class C_d3d_effect_func_interface {
protected:
	// 関数名
	std::string funcName;

	C_d3d_effect_func_interface(std::string name)
		:funcName(name)
	{
	}
public:
	virtual ~C_d3d_effect_func_interface(){}

	virtual std::string initCode() = 0;
	virtual std::string realCode() = 0;

	// 関数名を取得
	std::string getFuncName() const { return funcName; }
};


}