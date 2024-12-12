#pragma once

namespace NT3
{

// クラス(種類)を表すタイプ
enum CAMERA_TYPE{
	CAMERA_TYPE_NONE = -1,

	CAMERA_TYPE_NORMAL,

	CAMERA_TYPE_QUATERNION,
};

// ****************************************************************
// カメラインターフェイス
// ================================================================
class C_d3d_camera_interface
{
	// ビュー行列用パラメータ群
	struct S_view_val
	{
		D3DXVECTOR3 pos;			// 視点
		D3DXVECTOR3 tgt;			// 注視点
		D3DXVECTOR3 up;				// 上向きベクトル
	};
	
	// 射影行列用パラメータ群
	struct S_proj_val
	{
		float		fov_h_deg;				// 水平方向の視野角(度数法)
		float		aspect;					// アスペクト比
		float		near_clip;				// 近接クリップ面
		float		far_clip;				// 遠方クリップ面
		float		view_volume_width;	// ビューボリュームの幅
		float		view_volume_height;	// ビューボリュームの高さ
	};
	
	// ばね制御用パラメータ群
	struct S_spring_val
	{
		D3DXVECTOR3	pos_speed;		// 視点用速度
		D3DXVECTOR3	tgt_speed;		// 注視点用速度
		float		constant;		// ばね定数
		float		friction;		// 摩擦係数
		float		distance;		// 基準の距離
	};

	// 視錐台用の4平面構造体
	struct S_four_planes
	{
		D3DXPLANE	top;		// 上面
		D3DXPLANE	bottom;		// 底面
		D3DXPLANE	left;		// 左面
		D3DXPLANE	right;		// 右面
	};

protected:
	C_d3d_camera_interface();

			void		calc_view_matrix();
	virtual	void		calc_proj_matrix();		// 仮想関数なのは基本PerspectiveFovLHだから
			void		calc_dir();
			bool		calc_aspect();
			void		calc_view_planes();
	
public:
	virtual				~C_d3d_camera_interface();

	virtual	void		initialize(const D3DXVECTOR3& pos, const D3DXVECTOR3& tgt, const D3DXVECTOR3& up,
									float fov_h_deg, float near_clip, float far_clip);
	virtual	void		update();

	virtual	void		rotate_y_per_frame(float degree);
	virtual	void		rotate_x_per_frame(float degree);

	virtual	bool		in_view_volune(const D3DXVECTOR3& point, float radius);

	// set系は関連する最新フラグをオフにする
			void		set_position(const D3DXVECTOR3& pos);
			void		set_target(const D3DXVECTOR3& tgt);
			void		set_up(const D3DXVECTOR3& up);

			void 		set_fov_h_deg(float fov_h_deg);
			void 		set_aspect(float aspect);
			void 		set_near_clip(float near_clip);
			void 		set_far_clip(float far_clip);
			void 		set_view_volume_width(float view_volume_width);
			void 		set_view_volume_height(float view_volume_height);

			void		set_pos_tgt_for_spring(const D3DXVECTOR3& pos, const D3DXVECTOR3& tgt);
			void		set_spring_const_param(float constant, float friction, float distance);


	// get系は最新かチェックして最新でなければ再計算する(一部を除く)
			CAMERA_TYPE	get_type();
			int			get_id();

			D3DXVECTOR3	get_position();
			D3DXVECTOR3	get_target();
			D3DXVECTOR3	get_dir();

			D3DXMATRIX	get_view_matrix();
			D3DXMATRIX	get_proj_matrix();
			D3DXMATRIX* get_billboard_matrix(D3DXMATRIX* mat);
			D3DXVECTOR4 get_camera_local_pos(const D3DXMATRIX& world);
			D3DXVECTOR4 get_camera_world_pos();


			// ウィンドウサイズではなくビューポートサイズでやるのが正しい？
			//inline D3DXMATRIX* GetWorld2ScreenMatrix(D3DXMATRIX* mat) {
			//	D3DXMATRIX matScreen = IDENTITY_MATRIX;
			//	matScreen._11 =  SCREEN_WIDTH  / 2.0f;
			//	matScreen._22 = -SCREEN_HEIGHT / 2.0f;
			//	matScreen._41 =  SCREEN_WIDTH  / 2.0f;
			//	matScreen._42 =  SCREEN_HEIGHT / 2.0f;
			//	*mat = m_MatView * m_MatProj * matScreen;
			//	return mat;
			//}

protected:
	// 各種構造体
	S_view_val		m_view_val;
	S_proj_val		m_proj_val;
	S_spring_val	m_spring_val;
	S_four_planes	m_planes;

	// 管理用
	CAMERA_TYPE		m_type;
	int				m_id;

	// その他
	D3DXVECTOR3		m_dir;
	D3DXMATRIX		m_view_mat;
	D3DXMATRIX		m_proj_mat;

	// 最新かどうか判定用(使用時に最新でなければ更新する)
	// 関連パラメータがsetされたらフラグを畳んでおく(絡まってるパラメータに注意)
	bool			m_the_newest_dir;
	bool			m_the_newest_view_mat;
	bool			m_the_newest_proj_mat;
	bool			m_the_newest_planes;
};


// ****************************************************************
// 普通のカメラ
// ================================================================
class C_d3d_camera_normal : public C_d3d_camera_interface {
public:
	C_d3d_camera_normal();
	virtual ~C_d3d_camera_normal();
};


// ****************************************************************
// クォータニオンカメラ
// ================================================================
class C_d3d_camera_quaternion : public C_d3d_camera_interface {
public:
	C_d3d_camera_quaternion();
	virtual ~C_d3d_camera_quaternion();

	void set_rotate_y_standard_tgt(float degree);
	void add_rotate_y_standard_tgt(float degree);
	void set_rotate_x_standard_tgt(float degree);
	void add_rotate_x_standard_tgt(float degree);

private:
	D3DXQUATERNION	m_quaternion;
};



/*
// このカメラタイプは何を基準に操作すると言うのを表す
// 主にスクリプトで操作しやすいようにする為？
enum CAMERA_TYPE {
	CAMERATYPE_EPU,
//	CAMERATYPE_PDRA,
};

struct C_d3d_camera
{
	C_d3d_camera();

	void			update_frame();

	// パラメータ
	CAMERATYPE		type;			// タイプ
	float			eye_radian;		// 視野角
	float			aspect;			// アスペクト比

	// タイプ：ノーマル
	D3DXVECTOR3		eye;		// 目の位置
	D3DXVECTOR3		pint;		// 見ている場所
	D3DXVECTOR3		up;			// 上方向

	// タイプ：プレイヤー
//	D3DXVECTOR3		pos;	// プレイヤーの位置
//	float			dir;	// プレイヤーの向き
//	float			rad;	// カメラの半径
//	float			ang;	// カメラの高さ
};
*/







}