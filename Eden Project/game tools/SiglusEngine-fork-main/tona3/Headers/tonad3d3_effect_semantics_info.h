// C_d3d_effect_semantics_info.h

#pragma once

namespace NT3
{


class C_d3d_effect_semantics_info {
public:
	enum TYPE {
		POSITION		= 1 << 0,
		BLENDWEIGHT		= 1 << 1,
		BLENDINDICES	= 1 << 2,
		NORMAL			= 1 << 3,
		PSIZE			= 1 << 4,
		TEXCOORD		= 1 << 5,
		TANGENT			= 1 << 6,
		BINORMAL		= 1 << 7,
		COLOR			= 1 << 8,
		POSITIONT		= 1 << 9,
		DEPTH			= 1 << 10,
		TEXCOORD0		= 1 << 5,
		TEXCOORD1		= 1 << 11,
		TEXCOORD2		= 1 << 12,
		TEXCOORD3		= 1 << 13,
		TEXCOORD4		= 1 << 14,
		TEXCOORD5		= 1 << 15,
		TEXCOORD6		= 1 << 16,
		TEXCOORD7		= 1 << 17,
		TEXCOORD8		= 1 << 18,
		COLOR0			= 1 << 8,
		COLOR1			= 1 << 19,
		//COLOR2			= 1 << 20,
		//COLOR3			= 1 << 21,
	};
	//static const unsigned int typeNum = 19;	// タイプの数

	std::vector<TYPE> typeAry;  // コンテナ

	C_d3d_effect_semantics_info() {}
	~C_d3d_effect_semantics_info() {}

	// 指定のセマンティクスがあるかチェック
	bool find(TYPE type) {
		for (size_t i = 0; i < typeAry.size(); i++) {
			if ( typeAry[i] == type )
				return true;
		}
		return false;
	}

	// セマンティクスビットを取得
	unsigned int getBit() {
		unsigned int bit = 0;
		for ( size_t i = 0; i < typeAry.size(); i++ ) {
			bit |= typeAry[i];
		}
		return bit;
	}
};



}