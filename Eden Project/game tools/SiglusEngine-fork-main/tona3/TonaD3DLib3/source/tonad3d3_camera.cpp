#include "tonad3d3_pch.h"
#include "tonad3d3_camera.h"
#include "tonad3d3_d3d.h"

namespace NT3
{


// ********************************************************************************************************************************
// カメラインターフェイス
// ================================================================================================================================
C_d3d_camera_interface::C_d3d_camera_interface()
{
	m_type = CAMERA_TYPE_NONE;
	m_id = -1;
}

C_d3d_camera_interface::~C_d3d_camera_interface()
{
}

void C_d3d_camera_interface::initialize(const D3DXVECTOR3& pos, const D3DXVECTOR3& tgt, const D3DXVECTOR3& up,
									float fov_h_deg, float near_clip, float far_clip)
{
	set_position(pos);
	set_target(tgt);
	set_up(up);
	set_fov_h_deg(fov_h_deg);
	set_near_clip(near_clip);
	set_far_clip(far_clip);
}

void C_d3d_camera_interface::update()
{
	// 特に何もしない
	// もしかしたら後でばね制御時のソースをここに書くかも
}

void C_d3d_camera_interface::rotate_y_per_frame(float degree)
{
	// カメラの向きベクトルの逆を計算する
	D3DXVECTOR3 dir_inv = m_view_val.pos - m_view_val.tgt;

	// 回転行列を作成する
	D3DXMATRIX matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(degree));

	// カメラの向きベクトルを回転行列で回転する
	D3DXVec3TransformCoord(&dir_inv, &dir_inv, &matRotY);

	// カメラの視点を設定する
	set_position(m_view_val.tgt + dir_inv);
}
void C_d3d_camera_interface::rotate_x_per_frame(float degree)
{
	// カメラの向きベクトルの逆を計算する
	D3DXVECTOR3 dir_inv = m_view_val.pos - m_view_val.tgt;

	// 回転行列を作成する
	D3DXMATRIX matRotX;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(degree));

	// カメラの向きベクトルを回転行列で回転する
	D3DXVec3TransformCoord(&dir_inv, &dir_inv, &matRotX);

	// カメラの視点を設定する
	set_position(m_view_val.tgt + dir_inv);
}


void C_d3d_camera_interface::calc_view_matrix()
{
	D3DXMatrixLookAtLH(&m_view_mat, &m_view_val.pos, &m_view_val.tgt, &m_view_val.up);
}
void C_d3d_camera_interface::calc_proj_matrix()
{
	calc_aspect();

	// この関数は派生クラスによってオーバーロードしよう
	D3DXMatrixPerspectiveFovLH(&m_proj_mat, D3DXToRadian(m_proj_val.fov_h_deg), m_proj_val.aspect, m_proj_val.near_clip, m_proj_val.far_clip);
}

void C_d3d_camera_interface::calc_dir()
{
	m_dir = m_view_val.tgt - m_view_val.pos;
	D3DXVec3Normalize(&m_dir, &m_dir);
}

bool C_d3d_camera_interface::calc_aspect()
{
	HRESULT hr;
	// ビューポートの取得
	D3DVIEWPORT9 vp;
	hr = G_d3d.device.body()->GetViewport(&vp);

	if(FAILED(hr)) {
		set_last_error_d3d(_T("GetViewport"), hr);
		return false;
	}

	set_aspect((float)vp.Width / vp.Height);

	return true;
}

void C_d3d_camera_interface::calc_view_planes()
{
	D3DXMATRIX proj_inv;
	// 射影行列の逆行列を得る
	D3DXMatrixInverse(&proj_inv, NULL, &get_proj_matrix());

	// 視推台の各頂点を算出
	D3DXVECTOR3 right_top(1,1,0), right_bottom(1,-1,0), left_top(-1,1,0), left_bottom(-1,-1,0);
	D3DXVec3TransformCoord(&right_top,		&right_top,		&proj_inv);
	D3DXVec3TransformCoord(&right_bottom,	&right_bottom,	&proj_inv);
	D3DXVec3TransformCoord(&left_top,		&left_top,		&proj_inv);
	D3DXVec3TransformCoord(&left_bottom,	&left_bottom,	&proj_inv);

	// クリップ平面を算出
	D3DXPlaneFromPoints(&m_planes.right,	&ZERO_VECTOR, &right_top,	&right_bottom);
	D3DXPlaneFromPoints(&m_planes.top,		&ZERO_VECTOR, &left_top,	&right_top);
	D3DXPlaneFromPoints(&m_planes.left,		&ZERO_VECTOR, &left_bottom,	&left_top);
	D3DXPlaneFromPoints(&m_planes.bottom,	&ZERO_VECTOR, &right_bottom,&left_bottom);
}

bool C_d3d_camera_interface::in_view_volune(const D3DXVECTOR3& point, float radius)
{
	// 視推台平面の算出
	if(!m_the_newest_planes) {
		calc_view_planes();
		m_the_newest_planes = true;
	}

	D3DXVECTOR3 pos;
	// ビュー行列から視推台での位置を算出
	D3DXVec3TransformCoord(&pos, &point, &get_view_matrix());
	
	// 近クリップ内かの判定
	if(pos.z + radius <= m_proj_val.near_clip)
		return false;
	// 遠クリップ内かの判定
	if(pos.z - radius >= m_proj_val.far_clip)
		return false;


	float dist;
	// 右クリップ平面との判定
	dist = pos.x * m_planes.right.a + pos.z * m_planes.right.c;
	if(dist > radius)
		return false;

	// 左クリップ平面との判定
	dist = pos.x * m_planes.left.a + pos.z * m_planes.left.c;
	if(dist > radius)
		return false;

	// 上クリップ平面との判定
	dist = pos.y * m_planes.top.b + pos.z * m_planes.top.c;
	if(dist > radius)
		return false;

	// 下クリップ平面との判定
	dist = pos.y * m_planes.bottom.b + pos.z * m_planes.bottom.c;
	if(dist > radius)
		return false;

	return true;
}


// ****************************************************************
// カメラインターフェイス：セット系
// ================================================================
void C_d3d_camera_interface::set_position(const D3DXVECTOR3& pos)
{
	m_view_val.pos = pos;

	m_the_newest_dir = false;
	m_the_newest_view_mat = false;
}
void C_d3d_camera_interface::set_target(const D3DXVECTOR3& tgt)
{
	m_view_val.tgt = tgt;

	m_the_newest_dir = false;
	m_the_newest_view_mat = false;
}
void C_d3d_camera_interface::set_up(const D3DXVECTOR3& up)
{
	m_view_val.up = up;

	m_the_newest_view_mat = false;
}


void C_d3d_camera_interface::set_fov_h_deg(float fov_h_deg)
{
	m_proj_val.fov_h_deg = fov_h_deg;

	m_the_newest_proj_mat = false;
	m_the_newest_planes = false;
}
void C_d3d_camera_interface::set_aspect(float aspect)
{
	m_proj_val.aspect = aspect;

	m_the_newest_proj_mat = false;
	m_the_newest_planes = false;
}
void C_d3d_camera_interface::set_near_clip(float near_clip)
{
	m_proj_val.near_clip = near_clip;

	m_the_newest_proj_mat = false;
	m_the_newest_planes = false;
}
void C_d3d_camera_interface::set_far_clip(float far_clip)
{
	m_proj_val.far_clip = far_clip;

	m_the_newest_proj_mat = false;
	m_the_newest_planes = false;
}
void C_d3d_camera_interface::set_view_volume_width(float view_volume_width)
{
	m_proj_val.view_volume_width = view_volume_width;

	m_the_newest_proj_mat = false;
	m_the_newest_planes = false;
}
void C_d3d_camera_interface::set_view_volume_height(float view_volume_height)
{
	m_proj_val.view_volume_height = view_volume_height;

	m_the_newest_proj_mat = false;
	m_the_newest_planes = false;
}


void C_d3d_camera_interface::set_pos_tgt_for_spring(const D3DXVECTOR3& pos, const D3DXVECTOR3& tgt)
{
	// この関数の中身はフラグ付きアップデートにした方が良いかも
	// 視点と注目点の距離を取得
	D3DXVECTOR3 vec = pos - tgt;
	float dist = D3DXVec3Length(&vec);

	// ばね係数をカメラとターゲットの距離に応じて変更する
	m_spring_val.pos_speed += (pos - m_view_val.pos) * m_spring_val.constant * m_spring_val.distance / dist;
	m_spring_val.pos_speed -= m_spring_val.pos_speed * m_spring_val.friction;
	set_position(m_view_val.pos + m_spring_val.pos_speed);

	// ばね係数をカメラとターゲットの距離に応じて変更する
	m_spring_val.tgt_speed += (tgt - m_view_val.tgt) * m_spring_val.constant * m_spring_val.distance / dist;
	m_spring_val.tgt_speed -= m_spring_val.tgt_speed * m_spring_val.friction;
	set_target(m_view_val.tgt + m_spring_val.tgt_speed);
}

void C_d3d_camera_interface::set_spring_const_param(float constant, float friction, float distance)
{
	m_spring_val.constant = constant;
	m_spring_val.friction = friction;
	m_spring_val.distance = distance;
}



// ****************************************************************
// カメラインターフェイス：ゲット系
// ================================================================
CAMERA_TYPE C_d3d_camera_interface::get_type()
{
	return m_type;
}
int C_d3d_camera_interface::get_id()
{
	return m_id;
}

D3DXVECTOR3 C_d3d_camera_interface::get_position()
{
	return m_view_val.pos;
}

D3DXVECTOR3 C_d3d_camera_interface::get_target()
{
	return m_view_val.tgt;
}

D3DXVECTOR3 C_d3d_camera_interface::get_dir()
{
	if(!m_the_newest_dir) {
		calc_dir();
		m_the_newest_dir = true;
	}

	return m_dir;
}

D3DXMATRIX	C_d3d_camera_interface::get_view_matrix()
{
	if(!m_the_newest_view_mat) {
		calc_view_matrix();
		m_the_newest_view_mat = true;
	}

	return m_view_mat;
}

D3DXMATRIX	C_d3d_camera_interface::get_proj_matrix()
{
	if(!m_the_newest_proj_mat) {
		calc_proj_matrix();
		m_the_newest_proj_mat = true;
	}

	return m_proj_mat;
}

D3DXMATRIX* C_d3d_camera_interface::get_billboard_matrix(D3DXMATRIX* mat)
{
	// ビュー行列の逆行列を計算する
	D3DXMatrixInverse(mat, NULL, &get_view_matrix());
	// 平行移動はしない
	mat->_41 = 0.0f;
	mat->_42 = 0.0f;
	mat->_43 = 0.0f;

	return mat;
}

D3DXVECTOR4 C_d3d_camera_interface::get_camera_local_pos(const D3DXMATRIX& world)
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec;
	D3DXMATRIX mat;		// ローカル座標系からビュー座標系への変換行列の逆行列

	mat = world * get_view_matrix();
	D3DXMatrixInverse(&mat, NULL, &mat);
	vec = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVec4Transform(&vec, &vec, &mat);
	
	return vec;
}

D3DXVECTOR4 C_d3d_camera_interface::get_camera_world_pos()
{
	// 四次元なのはシェーダ用
	D3DXVECTOR4 vec = D3DXVECTOR4(m_view_val.pos.x, m_view_val.pos.y, m_view_val.pos.z, 1.0f);
	return vec;
}

// ********************************************************************************************************************************
// 普通のカメラ
// ================================================================================================================================
C_d3d_camera_normal::C_d3d_camera_normal()
{
	m_type = CAMERA_TYPE_NORMAL;
	m_id = -1;
}
C_d3d_camera_normal::~C_d3d_camera_normal()
{
}


// ********************************************************************************************************************************
// クォータニオンカメラ
// ================================================================================================================================
C_d3d_camera_quaternion::C_d3d_camera_quaternion()
{
	m_type = CAMERA_TYPE_QUATERNION;
	m_id = -1;
}
C_d3d_camera_quaternion::~C_d3d_camera_quaternion()
{
}

void C_d3d_camera_quaternion::set_rotate_y_standard_tgt(float degree)
{
	D3DXQUATERNION qt(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXQuaternionRotationAxis(&qt, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXToRadian(degree));
	m_quaternion = qt;

	D3DXMATRIX mat;
	D3DXMatrixRotationQuaternion(&mat, &m_quaternion);
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir = m_view_val.pos - m_view_val.tgt;
	D3DXVec3TransformCoord(&pos, &dir, &mat);
	set_position(pos + m_view_val.tgt);
	//D3DXVec3TransformCoord(&pos, &m_view_val.pos, &mat);
	//set_position(pos);
}
void C_d3d_camera_quaternion::add_rotate_y_standard_tgt(float degree)
{
	D3DXQUATERNION qt(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXQuaternionRotationAxis(&qt, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXToRadian(degree));
	m_quaternion *= qt;

	D3DXMATRIX mat;
	D3DXMatrixRotationQuaternion(&mat, &m_quaternion);
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir = m_view_val.pos - m_view_val.tgt;
	D3DXVec3TransformCoord(&pos, &dir, &mat);
	set_position(pos);
}

void C_d3d_camera_quaternion::set_rotate_x_standard_tgt(float degree)
{
	D3DXQUATERNION qt(0.0f, 0.0f, 0.0f, 1.0f);
	//D3DXQuaternionRotationAxis(&qt, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXToRadian(degree));
	D3DXVECTOR3 dir_n = m_view_val.pos - m_view_val.tgt;
	D3DXVec3Normalize(&dir_n, &dir_n);
	D3DXQuaternionRotationAxis(&qt, &D3DXVECTOR3(-dir_n.z, 0.0f, dir_n.x), D3DXToRadian(degree));
	m_quaternion = qt;

	D3DXMATRIX mat;
	D3DXMatrixRotationQuaternion(&mat, &m_quaternion);
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir = m_view_val.pos - m_view_val.tgt;
	D3DXVec3TransformCoord(&pos, &dir, &mat);
	set_position(pos + m_view_val.tgt);
	//D3DXVec3TransformCoord(&pos, &m_view_val.pos, &mat);
	//set_position(pos);
}
void C_d3d_camera_quaternion::add_rotate_x_standard_tgt(float degree)
{
	D3DXQUATERNION qt(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXQuaternionRotationAxis(&qt, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXToRadian(degree));
	m_quaternion *= qt;

	D3DXMATRIX mat;
	D3DXMatrixRotationQuaternion(&mat, &m_quaternion);
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir = m_view_val.pos - m_view_val.tgt;
	D3DXVec3TransformCoord(&pos, &dir, &mat);
	set_position(pos);
}



/*
C_d3d_camera::C_d3d_camera()
{
	type = CAMERATYPE_EPU;
	eye_radian = D3DXToRadian(45.0f);
	aspect = 4.0f / 3.0f;

	eye = D3DXVECTOR3(0.0f, 0.0f, -30.0f);
	pint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}
void C_d3d_camera::update_frame()
{
	// カメラ情報 (EPU - eye pint up -) の計算
	switch (type)	{
		case CAMERATYPE_EPU:
			// NOP
			break;
//		case CAMERATYPE_PDRA:
//			eye.x = rad * sin(dir + D3DX_PI) * cos(ang) + pos.x;
//			eye.y = rad                      * sin(ang) + pos.y;
//			eye.z = rad * cos(dir + D3DX_PI) * cos(ang) + pos.z;
//			pint  = pos;							// カメラは主人公を見ている
//			up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 常に上
//			break;
	}
}
*/



}