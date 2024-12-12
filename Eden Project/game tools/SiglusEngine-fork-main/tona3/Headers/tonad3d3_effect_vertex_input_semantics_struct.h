// C_d3d_effect_vertex_input_semantics_struct.h

#pragma once
#include "tonad3d3_effect_semantics_struct.h"
#include "tonad3d3_effect_global_val.h"


namespace NT3
{




class C_d3d_effect_vertex_input_semantics_struct : public C_d3d_effect_semantics_struct {
	class VertexInputSemanticsVal : public SemanticsVal {
		std::string initValStr;		// 初期値文字列
		C_d3d_effect_semantics_info::TYPE type;	// セマンティクスタイプ
		bool isValidate;			// 有効？

	public:
		VertexInputSemanticsVal() {}
		VertexInputSemanticsVal(std::string structName, std::string typeName, std::string valName, std::string semanticsName, C_d3d_effect_semantics_info::TYPE type) :
			SemanticsVal(structName, typeName, valName, semanticsName), type(type) {}
		~VertexInputSemanticsVal() {}

		// セマンティクスタイプを取得
		C_d3d_effect_semantics_info::TYPE getType() const { return type; }

		// 初期化文字列設定
		void setInitStr( const char* init) { initValStr = init; }

		// 初期化文字列取得
		std::string getInitStr() const { return initValStr; }
	};

protected:
	BSP<VertexInputSemanticsVal> pos, blendWeight, blendIndices, normal, psize, texCoord, tangent, binormal, color, positionT;
	std::vector<BSP<VertexInputSemanticsVal> > validateSemantics;	// 有効なセマンティクス

public:
	C_d3d_effect_vertex_input_semantics_struct() : C_d3d_effect_semantics_struct("") {}
	C_d3d_effect_vertex_input_semantics_struct(std::string structName, unsigned semanticsBit) : C_d3d_effect_semantics_struct(structName),
		pos         (new VertexInputSemanticsVal("In", "float4", semanticsBit & C_d3d_effect_semantics_info::POSITION     ? "pos"          : "**/error/**", "POSITION"    , C_d3d_effect_semantics_info::POSITION)),
		blendWeight (new VertexInputSemanticsVal("In", "float4", semanticsBit & C_d3d_effect_semantics_info::BLENDWEIGHT  ? "blendWeight"  : "**/error/**", "BLENDWEIGHT" , C_d3d_effect_semantics_info::BLENDWEIGHT)),
		blendIndices(new VertexInputSemanticsVal("In", "float4", semanticsBit & C_d3d_effect_semantics_info::BLENDINDICES ? "blendIndices" : "**/error/**", "BLENDINDICES", C_d3d_effect_semantics_info::BLENDINDICES)),
		normal      (new VertexInputSemanticsVal("In", "float4", semanticsBit & C_d3d_effect_semantics_info::NORMAL       ? "normal"       : "**/error/**", "NORMAL"      , C_d3d_effect_semantics_info::NORMAL)),
		psize       (new VertexInputSemanticsVal("In", "float" , semanticsBit & C_d3d_effect_semantics_info::PSIZE        ? "psize"        : "**/error/**", "PSIZE"       , C_d3d_effect_semantics_info::PSIZE)),
		texCoord    (new VertexInputSemanticsVal("In", "float2", semanticsBit & C_d3d_effect_semantics_info::TEXCOORD     ? "uv"           : "**/error/**", "TEXCOORD0"   , C_d3d_effect_semantics_info::TEXCOORD)),
		tangent     (new VertexInputSemanticsVal("In", "float4", semanticsBit & C_d3d_effect_semantics_info::TANGENT      ? "tangent"      : "**/error/**", "TANGENT"     , C_d3d_effect_semantics_info::TANGENT)),
		binormal    (new VertexInputSemanticsVal("In", "float4", semanticsBit & C_d3d_effect_semantics_info::BINORMAL     ? "binormal"     : "**/error/**", "BINORMAL"    , C_d3d_effect_semantics_info::BINORMAL)),
		color       (new VertexInputSemanticsVal("In", "float4", semanticsBit & C_d3d_effect_semantics_info::COLOR        ? "color"        : "**/error/**", "COLOR0"      , C_d3d_effect_semantics_info::COLOR)),
		positionT   (new VertexInputSemanticsVal("In", "float4", semanticsBit & C_d3d_effect_semantics_info::POSITIONT    ? "positionT"    : "**/error/**", "POSITIONT"   , C_d3d_effect_semantics_info::POSITIONT))
	{
		// マップへ登録
		BSP<VertexInputSemanticsVal> v[] = {pos, blendWeight, blendIndices, normal, psize, texCoord, tangent, binormal, color, positionT};
		const int sz = sizeof(v) / sizeof(v[0]);
		for ( int i = 0; i < sz; i++ ) {
			valMap.insert( std::pair<std::string, BSP<SemanticsVal> >(v[i]->n(), v[i]) );
			if (v[i]->originValName() != "**/error/**")
				validateSemantics.push_back(v[i]);
		}
	}

	~C_d3d_effect_vertex_input_semantics_struct() {}

	// 構造体宣言作成
	std::string decl() {
		if(!SemanticsCheck()) return "";

		BSP<VertexInputSemanticsVal> v[] = {pos, blendWeight, blendIndices, normal, psize, texCoord, tangent, binormal, color, positionT};
		const int sz = sizeof(v) / sizeof(v[0]);
		std::string str = "struct " + this->structName + "{\n";
		for ( int i = 0; i < sz; i++ ) {
			if (v[i]->originValName() != "**/error/**")
				str += "\t" + v[i]->decl();
		}
		str += "};\n\n";
		return str;
	}

	// 初期値設定
	//void setInitialVal( C_d3d_effect_semantics_info::TYPE type, int init ) {
	//	BSP<VertexInputSemanticsVal> v[] = {blendIndices};
	//	const int sz = sizeof(v) / sizeof(v[0]);
	//	for ( int i = 0; i < sz; i++ ) {
	//		if (v[i]->getType() == type) {
	//			char c[128]; sprintf_s(c, "%d", init);
	//			v[i]->setInitStr(c);
	//		}
	//	}
	//}
	void setInitialVal( C_d3d_effect_semantics_info::TYPE type, float init ) {
		BSP<VertexInputSemanticsVal> v[] = {psize};
		const int sz = sizeof(v) / sizeof(v[0]);
		for ( int i = 0; i < sz; i++ ) {
			if (v[i]->getType() == type) {
				char c[128]; sprintf_s(c, "%f", init);
				v[i]->setInitStr(c);
			}
		}
	}
	void setInitialVal( C_d3d_effect_semantics_info::TYPE type, D3DXVECTOR2 &init ) {
		BSP<VertexInputSemanticsVal> v[] = {texCoord};
		const int sz = sizeof(v) / sizeof(v[0]);
		for ( int i = 0; i < sz; i++ ) {
			if (v[i]->getType() == type) {
				char c[128]; sprintf_s(c, "float2( %f, %f )", init.x, init.y);
				v[i]->setInitStr(c);
			}
		}
	}
	//void setInitialVal( C_d3d_effect_semantics_info::TYPE type, D3DXVECTOR3 &init ) {
	//	BSP<VertexInputSemanticsVal> v[] = {normal};
	//	const int sz = sizeof(v) / sizeof(v[0]);
	//	for ( int i = 0; i < sz; i++ ) {
	//		if (v[i]->getType() == type) {
	//			char c[128]; sprintf_s(c, "float3( %f, %f, %f )", init.x, init.y, init.z);
	//			v[i]->setInitStr(c);
	//		}
	//	}
	//}
	void setInitialVal( C_d3d_effect_semantics_info::TYPE type, D3DXVECTOR4 &init ) {
		BSP<VertexInputSemanticsVal> v[] = {pos, blendWeight, blendIndices, normal, tangent, binormal, color, positionT};
		const int sz = sizeof(v) / sizeof(v[0]);
		for ( int i = 0; i < sz; i++ ) {
			if (v[i]->getType() == type) {
				char c[128]; sprintf_s(c, "float4( %f, %f, %f, %f )", init.x, init.y, init.z, init.w);
				v[i]->setInitStr(c);
				break;
			}
		}
	}

	// 頂点シェーダの欠落セマンティクスの初期化コード挿入
	void missingVertexSemanticsInitializeCode( std::string& out, C_d3d_effect_semantics_info &modelSemInfo ) {
		size_t sz = validateSemantics.size();
		for ( size_t i = 0; i < sz; i++ ) {
			if ( !modelSemInfo.find(validateSemantics[i]->getType()) )
				out += validateSemantics[i]->n() + " = " + validateSemantics[i]->getInitStr() + ";\n";
		}
		out += "\n";
	}
private:
	bool SemanticsCheck() {
		if(pos         ) return true;
		if(blendWeight ) return true;
		if(blendIndices) return true;
		if(normal      ) return true;
		if(psize       ) return true;
		if(texCoord    ) return true;
		if(tangent     ) return true;
		if(binormal    ) return true;
		if(color       ) return true;
		if(positionT   ) return true;

		return false;
	}
};


}