#pragma once

#include "tonad3d3_light.h"

namespace NT3
{


// ライト生成者のインターフェイス
class C_d3d_light_creator_interface {
protected:
	BSP<C_d3d_light_interface>	create_instance() {
		return create();
	}
	C_d3d_light_creator_interface(){}
public:
	virtual ~C_d3d_light_creator_interface(){}
	virtual BSP<C_d3d_light_interface>	create() = 0;
};

class C_d3d_directional_light_creator : public C_d3d_light_creator_interface {
public:
	BSP<C_d3d_light_interface>	create() {
		return BSP<C_d3d_directional_light>(new C_d3d_directional_light);
	}
};
class C_d3d_point_light_creator : public C_d3d_light_creator_interface {
public:
	BSP<C_d3d_light_interface>	create() {
		return BSP<C_d3d_point_light>(new C_d3d_point_light);
	}
};
class C_d3d_spot_light_creator : public C_d3d_light_creator_interface {
public:
	BSP<C_d3d_light_interface>	create() {
		return BSP<C_d3d_spot_light>(new C_d3d_spot_light);
	}
};


// ライト生成者たちの構造体
struct S_d3d_light_creators {
	C_d3d_directional_light_creator	directional;
	C_d3d_point_light_creator		point;
	C_d3d_spot_light_creator		spot;
};



// 環境によっても違うけど300個くらいまでのデータならstd::mapより配列で管理する方が早い
// idとリストのインデックスは対応させる
// ライト管理者クラス
class C_d3d_light_manager {
public:
	static const int WORLD_LIGHT_MAX;				// 世界に配置できるライトの最大数
	static const int OBJ_DIRECTIONAL_LIGHT_MAX;		// オブジェクトに使用できるライトの最大数
	static const int OBJ_POINT_LIGHT_MAX;			// オブジェクトに使用できるライトの最大数
	static const int OBJ_SPOT_LIGHT_MAX;			// オブジェクトに使用できるライトの最大数
public:
	C_d3d_light_manager();
	~C_d3d_light_manager();


	//BSP<C_d3d_light_interface> regist(int id, BSP<C_d3d_light_interface> light);

	BSP<C_d3d_light_interface> create(int id, LIGHT_TYPE type);
	BSP<C_d3d_light_interface> search(int id);
	bool remove(int id);
	bool remove_all();
private:
	bool regist(int id, BSP<C_d3d_light_interface> obj);
private:
	// アプリで使用するライトのリスト(重複ID登録不可)
	ARRAY< BSP<C_d3d_light_interface> >		m_light_list;

	// ライト生成者たちの構造体
	S_d3d_light_creators					m_creators;
};
// 唯一のインスタンス
extern C_d3d_light_manager G_d3d_light_manager;






}