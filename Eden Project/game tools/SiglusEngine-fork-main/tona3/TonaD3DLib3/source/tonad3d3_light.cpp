#include "tonad3d3_pch.h"
#include "tonad3d3_light.h"
#include "tonad3d3_d3d.h"

namespace NT3
{


// ****************************************************************
// ライトインターフェイス
// ================================================================
C_d3d_light_interface::C_d3d_light_interface()
{
	m_type = LIGHT_TYPE_NONE;
	m_id = -1;

	// 各種ライト共通カラー
	m_diffuse = D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f);
	m_ambient = D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f);
	m_specular = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
}

C_d3d_light_interface::~C_d3d_light_interface()
{
}


// ****************************************************************
// ディレクショナルライト
// ================================================================
C_d3d_directional_light::C_d3d_directional_light()
{
	m_type = LIGHT_TYPE_DIRECTIONAL;
	m_id = -1;

	m_dir = ZERO_VECTOR;
}

C_d3d_directional_light::~C_d3d_directional_light()
{
}

D3DXVECTOR4 C_d3d_directional_light::get_light_local_dir(const D3DXMATRIX& world)
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec;
	D3DXMATRIX inv_world;
	D3DXMatrixInverse(&inv_world, NULL, &world);
	D3DXVec3Transform(&vec, &m_dir, &inv_world);
	D3DXVec3Normalize((D3DXVECTOR3*)&vec, (D3DXVECTOR3*)&vec);

	return vec;
}
D3DXVECTOR4 C_d3d_directional_light::get_light_world_dir()
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec = D3DXVECTOR4(m_dir.x, m_dir.y, m_dir.z, 1.0f);
	return vec;
}


// ****************************************************************
// ポイントライト
// ================================================================
C_d3d_point_light::C_d3d_point_light()
{
	m_type = LIGHT_TYPE_POINT;
	m_id = -1;

	m_pos = ZERO_VECTOR;
	// 減衰係数が全て0だと下の式で0除算が発生するので0.001fにしておく
	// Atten = 1/( att0i + att1i * d + att2i * d2)
	m_attenuation0 = 0.001f;
	m_attenuation1 = 0.0f;
	m_attenuation2 = 0.0f;
	m_range = 100.0f;
}

C_d3d_point_light::~C_d3d_point_light()
{
}

D3DXVECTOR4 C_d3d_point_light::get_light_local_pos(const D3DXMATRIX& world, const D3DXMATRIX& view)
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec;
	D3DXMATRIX mat;		// ローカル座標系からビュー座標系への変換行列の逆行列

	mat = world * view;
	D3DXMatrixInverse(&mat, NULL, &mat);
	vec = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVec4Transform(&vec, &vec, &mat);
	
	return vec;
}

D3DXVECTOR4 C_d3d_point_light::get_light_world_pos()
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec = D3DXVECTOR4(m_pos.x, m_pos.y, m_pos.z, 1.0f);
	return vec;
}

D3DXVECTOR4 C_d3d_point_light::get_light_attenuations_range()
{
	D3DXVECTOR4 parameters = D3DXVECTOR4(m_attenuation0, m_attenuation1, m_attenuation2, m_range);
	return parameters;
}


// ****************************************************************
// スポットライト
// ================================================================
C_d3d_spot_light::C_d3d_spot_light()
{
	m_type = LIGHT_TYPE_SPOT;
	m_id = -1;

	m_pos = ZERO_VECTOR;
	m_dir = ZERO_VECTOR;

	// 最大照射される範囲と全く照射されない範囲を設定
	// また間の範囲の計算結果はfalloff乗されるけど数値を変更しても大差ないので1.0fがいい
	m_theta = D3DXToRadian(15.0f);
	m_phi = D3DXToRadian(60.0f);
	m_falloff = 1.0f;

	// 減衰係数が全て0だと下の式で0除算が発生するので0.001fにしておく
	// Atten = 1/( att0i + att1i * d + att2i * d2)
	m_attenuation0 = 0.001f;
	m_attenuation1 = 0.0f;
	m_attenuation2 = 0.0f;
	m_range = 100.0f;
}

C_d3d_spot_light::~C_d3d_spot_light()
{
}

D3DXVECTOR4 C_d3d_spot_light::get_light_local_pos(const D3DXMATRIX& world, const D3DXMATRIX& view)
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec;
	D3DXMATRIX mat;		// ローカル座標系からビュー座標系への変換行列の逆行列

	mat = world * view;
	D3DXMatrixInverse(&mat, NULL, &mat);
	vec = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVec4Transform(&vec, &vec, &mat);
	
	return vec;
}
D3DXVECTOR4 C_d3d_spot_light::get_light_world_pos()
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec = D3DXVECTOR4(m_pos.x, m_pos.y, m_pos.z, 1.0f);
	return vec;
}
D3DXVECTOR4 C_d3d_spot_light::get_light_local_dir(const D3DXMATRIX& world)
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec;
	D3DXMATRIX inv_world;
	D3DXMatrixInverse(&inv_world, NULL, &world);
	D3DXVec3Transform(&vec, &m_dir, &inv_world);
	D3DXVec3Normalize((D3DXVECTOR3*)&vec, (D3DXVECTOR3*)&vec);

	return vec;
}
D3DXVECTOR4 C_d3d_spot_light::get_light_world_dir()
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec = D3DXVECTOR4(m_dir.x, m_dir.y, m_dir.z, 1.0f);
	return vec;
}

D3DXVECTOR4 C_d3d_spot_light::get_light_theta_phi_falloff()
{
	//D3DXVECTOR4 parameters = D3DXVECTOR4(m_theta, m_phi, m_falloff, 1.0f);
	// シェーダでは theta/2 と phi/2 しか使わないのでシェーダに渡す直前で割っておく
	D3DXVECTOR4 parameters = D3DXVECTOR4(m_theta / 2.0f, m_phi / 2.0f, m_falloff, 1.0f);

	return parameters;
}

D3DXVECTOR4 C_d3d_spot_light::get_light_cos_theta_phi_falloff()
{
	//D3DXVECTOR4 parameters = D3DXVECTOR4(m_theta, m_phi, m_falloff, 1.0f);
	// シェーダでは theta/2 と phi/2 しか使わないのでシェーダに渡す直前で割っておく
	D3DXVECTOR4 parameters = D3DXVECTOR4(cos(m_theta / 2.0f), cos(m_phi / 2.0f), m_falloff, 1.0f);

	return parameters;
}

D3DXVECTOR4 C_d3d_spot_light::get_light_attenuations_range()
{
	D3DXVECTOR4 parameters = D3DXVECTOR4(m_attenuation0, m_attenuation1, m_attenuation2, m_range);
	return parameters;
}








// ****************************************************************
// シャドウマップ用スポットライト(減衰率とか範囲は追加するかも)
// ================================================================
C_d3d_shadow_map_spot_light::C_d3d_shadow_map_spot_light()
{
	m_type = LIGHT_TYPE_SPOT_SM;
	m_id = -1;

	m_pos = ZERO_VECTOR;					// 視点
	m_tgt = ZERO_VECTOR;					// 注視点
	m_up = UP_VECTOR;					// 上向きベクトル

	m_near_clip = 1.0f;			// 近接クリップ面
	m_far_clip = 1000.0f;				// 遠方クリップ面
	//m_view_volume_width = 1280.0f;	// ビューボリュームの幅
	//m_view_volume_height = 720.0f;	// ビューボリュームの高さ
	m_view_volume_width = (float)G_d3d.device.get_shadow_map_size();	// ビューボリュームの幅
	m_view_volume_height = (float)G_d3d.device.get_shadow_map_size();	// ビューボリュームの高さ
}

C_d3d_shadow_map_spot_light::~C_d3d_shadow_map_spot_light()
{
}

D3DXVECTOR4 C_d3d_shadow_map_spot_light::get_light_local_dir(const D3DXMATRIX& world)
{
	// 四次元なのはシェーダ用
	D3DXVECTOR3 dir = m_tgt - m_pos;
	D3DXVECTOR4 vec;
	D3DXMATRIX inv_world;
	D3DXMatrixInverse(&inv_world, NULL, &world);
	D3DXVec3Transform(&vec, &dir, &inv_world);
	D3DXVec3Normalize((D3DXVECTOR3*)&vec, (D3DXVECTOR3*)&vec);

	return vec;
}
D3DXVECTOR4 C_d3d_shadow_map_spot_light::get_light_world_dir()
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec = D3DXVECTOR4(
		m_tgt.x - m_pos.x,
		m_tgt.y - m_pos.y,
		m_tgt.z - m_pos.z,
		1.0f
		);
	return vec;
}

D3DXMATRIX C_d3d_shadow_map_spot_light::get_view_matrix()
{
	D3DXMATRIX mat;

	D3DXMatrixLookAtLH(&mat, &m_pos, &m_tgt, &m_up);

	return mat;
	
}
D3DXMATRIX C_d3d_shadow_map_spot_light::get_proj_matrix()
{
	D3DXMATRIX mat;

	D3DXMatrixOrthoLH(&mat, m_view_volume_width, m_view_volume_height, m_near_clip, m_far_clip);

	return mat;
}














/*
C_d3d_light::C_d3d_light()
{
	// パラメータ
	type = LIGHT_TYPE_DIRECTIONAL;

	pos = D3DXVECTOR3(0, 0, 0);
	D3DXVec3Normalize(&dir, &D3DXVECTOR3(1, -1, 1));

	// 各種ライト共通カラー
	diffuse = D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f);
	ambient = D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f);
	specular = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	// 未実装
	range = 100.0f;
	theta0 = D3DXToRadian(30);
	theta1 = D3DXToRadian(45);
}
*/


}