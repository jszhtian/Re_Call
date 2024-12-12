

#pragma once


namespace NT3
{

class C_d3d_effect_func_info {
public:
	enum TYPE {
		MRBD		= 1 << 0,
		RGB			= 1 << 1,
		TONECURVE	= 1 << 2,
		MUL			= 1 << 3,
		VERTEX		= 1 << 4,
	};

	std::vector<TYPE>	typeAry;

	C_d3d_effect_func_info() {}
	~C_d3d_effect_func_info(){}

	// 指定のしたものがあるかチェック
	bool find(TYPE type) {
		for (size_t i = 0; i < typeAry.size(); i++) {
			if ( typeAry[i] == type )
				return true;
		}
		return false;
	}

	// ビットを取得
	unsigned int getBit() {
		unsigned int bit = 0;
		for ( size_t i = 0; i < typeAry.size(); i++ ) {
			bit |= typeAry[i];
		}
		return bit;
	}
};


}