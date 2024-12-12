#include "tonad3d3_pch.h"
#include "tonad3d3_light_manager.h"

namespace NT3
{

// 唯一のインスタンス
C_d3d_light_manager G_d3d_light_manager;

const int C_d3d_light_manager::WORLD_LIGHT_MAX = 128;			// 世界に配置できるライトの最大数
const int C_d3d_light_manager::OBJ_DIRECTIONAL_LIGHT_MAX = 4;	// オブジェクトに使用できるライトの最大数
const int C_d3d_light_manager::OBJ_POINT_LIGHT_MAX = 4;			// オブジェクトに使用できるライトの最大数
const int C_d3d_light_manager::OBJ_SPOT_LIGHT_MAX = 4;			// オブジェクトに使用できるライトの最大数



C_d3d_light_manager::C_d3d_light_manager()
{
	m_light_list.clear();
	m_light_list.reserve(WORLD_LIGHT_MAX);
	m_light_list.resize(WORLD_LIGHT_MAX);
}

C_d3d_light_manager::~C_d3d_light_manager()
{
	remove_all();
}

BSP<C_d3d_light_interface> C_d3d_light_manager::create(int id, LIGHT_TYPE type)
{
	// NULLもどき
	BSP<C_d3d_light_interface> light = BSP<C_d3d_light_interface>();

	// ID = 配列インデックスなので最大数を越える場合はエラーとして処理
	if(id >= WORLD_LIGHT_MAX) {
		set_last_error(_T("ライトの生成に失敗しました。"), _T("create"));
		return BSP<C_d3d_light_interface>();
	}

	if(false);

	else if(type == LIGHT_TYPE_DIRECTIONAL) {
		light = m_creators.directional.create();
	}
	else if(type == LIGHT_TYPE_POINT) {
		light = m_creators.point.create();
	}
	else if(type == LIGHT_TYPE_SPOT) {
		light = m_creators.spot.create();
	}
	else {
		// エラー処理？
	}

	// 何かしら生成されていれば
	if(light) {
		// 登録する
		if(!regist(id, light)) {
			// 登録に失敗した場合は解放
			light = BSP<C_d3d_light_interface>();
		}
		else {
			// 登録に成功した時点でライトにもIDを登録する
			light->m_id = id;
		}
	}

	return light;
}

BSP<C_d3d_light_interface> C_d3d_light_manager::search(int id)
{
	return m_light_list[id];
}
bool C_d3d_light_manager::remove(int id)
{
	m_light_list[id] = BSP<C_d3d_light_interface>();
	return true;
}
bool C_d3d_light_manager::remove_all()
{
	m_light_list.clear();
	return true;
}
bool C_d3d_light_manager::regist(int id, BSP<C_d3d_light_interface> obj)
{
	// オブジェクトがなかったら失敗
	if(!obj)
		return false;

	m_light_list[id] = obj;
	return true;
}













}
