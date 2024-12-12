// C_d3d_effect_global_sampler_val.h

#pragma once


#include "tonad3d3_effect_global_val.h"
#include "tonad3d3_effect_global_texture_val.h"
#include "tonad3d3_effect_sampler_func.h"


namespace NT3
{



// �T���v���[�ϐ��p
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

	// ���b�s���O���[�h�w��t��
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

	// �F�X�w��t��
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


	// �錾���o��
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

	// �����l�֐��o��
	MBSTR initCode() {
		return samplerFunc.initCode();
	}

	// �T���v�����O�֐��o��
	MBSTR samplerCode() {
		return samplerFunc.samplerCode();
	}

	// �T���v���Ăяo��
	MBSTR operator () (const MBSTR uvVal) {
		return samplerFunc(uvVal);
	}

	// �e�N�X�`���Z�}���e�B�N�X�����擾
	MBSTR getTextureSemanticsName() const { return texture.getSemanticsName(); }

	// �e�N�X�`�������擾
	MBSTR getTextureName() const { return texture.n(); }

	// �֐������擾
	MBSTR getFuncName() const { return samplerFunc.getFuncName(); } 
};



}