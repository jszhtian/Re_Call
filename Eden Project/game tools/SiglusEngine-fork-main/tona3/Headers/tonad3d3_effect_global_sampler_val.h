// C_d3d_effect_global_sampler_val.h

#pragma once


#include "tonad3d3_effect_global_val.h"
#include "tonad3d3_effect_global_texture_val.h"
#include "tonad3d3_effect_sampler_func.h"


namespace NT3
{



// サンプラー変数用
class C_d3d_effect_global_sampler_val : public C_d3d_effect_global_val {
	C_d3d_effect_global_texture_val texture;
	C_d3d_effect_sampler_func samplerFunc;

	MBSTR _addressU;
	MBSTR _addressV;

	MBSTR _minFilter;
	MBSTR _magFilter;
	MBSTR _mipFilter;
	MBSTR _borderColor;

public:
	C_d3d_effect_global_sampler_val(MBSTR valName, C_d3d_effect_global_texture_val texture, D3DXVECTOR4 &initVal) :
	  C_d3d_effect_global_val("sampler", valName, ""), texture(texture), samplerFunc(MBSTR("get")+texture.n(), initVal, valName)
	  {
		  _addressU.clear();
		  _addressV.clear();
		  _minFilter.clear();
		  _magFilter.clear();
		  _mipFilter.clear();
		  _borderColor.clear();
	  }

	// ラッピングモード指定付き
	C_d3d_effect_global_sampler_val(MBSTR valName, C_d3d_effect_global_texture_val texture, D3DXVECTOR4 &initVal, MBSTR addressU, MBSTR addressV, bool is2DProj) :
	  C_d3d_effect_global_val("sampler", valName, ""), texture(texture), samplerFunc(MBSTR("get")+texture.n(), initVal, valName, is2DProj)
	  {
		  _addressU = addressU;
		  _addressV = addressV;
		  _minFilter.clear();
		  _magFilter.clear();
		  _mipFilter.clear();
		  _borderColor.clear();
	  }

	// 色々指定付き
	C_d3d_effect_global_sampler_val(MBSTR valName, C_d3d_effect_global_texture_val texture, D3DXVECTOR4 &initVal, MBSTR addressU, MBSTR addressV,
					MBSTR minFilter, MBSTR magFilter, MBSTR mipFilter, MBSTR borderColor, bool is2DProj) :
	  C_d3d_effect_global_val("sampler", valName, ""), texture(texture), samplerFunc(MBSTR("get")+texture.n(), initVal, valName, is2DProj)
	  {
		  _addressU		= addressU;
		  _addressV		= addressV;
		  _minFilter	= minFilter;
		  _magFilter	= magFilter;
		  _mipFilter	= mipFilter;
		  _borderColor	= borderColor;
	  }


	// 宣言部出力
	virtual MBSTR decl() {
		MBSTR res = typeName + " " + valName;
		res += " = sampler_state {\n";
		res += "\ttexture = <" + texture.n() + ">;\n";

		//res += "\tAddressU = Clamp;\n";
		//res += "\tAddressV = Clamp;\n";
		if(!_addressU.empty()) res += "\tAddressU = " + _addressU + ";\n";
		if(!_addressV.empty()) res += "\tAddressV = " + _addressV + ";\n";
		if(!_minFilter.empty()) res += "\tMinFilter = " + _minFilter + ";\n";
		if(!_magFilter.empty()) res += "\tMagFilter = " + _magFilter + ";\n";
		if(!_mipFilter.empty()) res += "\tMipFilter = " + _mipFilter + ";\n";
		if(!_borderColor.empty()) res += "\tBorderColor = " + _borderColor + ";\n";


		res += "};\n";
		return res;
	}

	// 初期値関数出力
	MBSTR initCode() {
		return samplerFunc.initCode();
	}

	// サンプリング関数出力
	MBSTR samplerCode() {
		return samplerFunc.samplerCode();
	}

	// サンプラ呼び出し
	MBSTR operator () (const MBSTR uvVal) {
		return samplerFunc(uvVal);
	}

	// テクスチャセマンティクス名を取得
	MBSTR getTextureSemanticsName() const { return texture.getSemanticsName(); }

	// テクスチャ名を取得
	MBSTR getTextureName() const { return texture.n(); }

	// 関数名を取得
	MBSTR getFuncName() const { return samplerFunc.getFuncName(); } 
};



}