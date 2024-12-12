#pragma once

namespace NT3
{


enum LIGHT_TYPE
{
	LIGHT_TYPE_NONE = -1,
	LIGHT_TYPE_DIRECTIONAL,		// ディレクショナルライト(並行光源)
	LIGHT_TYPE_POINT,			// ポイントライト(点光源)
	LIGHT_TYPE_SPOT,			// スポットライト

	LIGHT_TYPE_SPOT_SM,			// シャドウマップ用スポットライト
};

// ****************************************************************
// ライトインターフェイス
// ================================================================
class C_d3d_light_interface
{
	friend class C_d3d_light_manager;
protected:
	C_d3d_light_interface();
	
public:
	virtual ~C_d3d_light_interface();

	// 色
	void		set_diffuse(const D3DXVECTOR4& color)	{	m_diffuse = color;	}
	void		set_ambient(const D3DXVECTOR4& color)	{	m_ambient = color;	}
	void		set_specluar(const D3DXVECTOR4& color)	{	m_specular = color;	}

	D3DXVECTOR4	get_diffuse()	{	return m_diffuse;	}
	D3DXVECTOR4	get_ambient()	{	return m_ambient;	}
	D3DXVECTOR4	get_specular()	{	return m_specular;	}

	// タイプ、ID
	void		set_type(LIGHT_TYPE type)	{	m_type = type;	}
	void		set_id(int id)				{	m_id = id;		}

	LIGHT_TYPE	get_type()	{	return m_type;	}
	int			get_id()	{	return m_id;	}

protected:
	// ダイナミックキャストで種類判断するよりタイプの方が早そうなのでタイプを持たせる
	LIGHT_TYPE		m_type;
	int				m_id;

	// 各種ライト共通カラー
	D3DXVECTOR4		m_diffuse;
	D3DXVECTOR4		m_ambient;
	D3DXVECTOR4		m_specular;		// シェーダ側は未実装
};

// ****************************************************************
// ディレクショナルライト
// ================================================================
class C_d3d_directional_light : public C_d3d_light_interface
{
	friend class C_d3d_light_manager;
protected:
	D3DXVECTOR3	m_dir;
public:
	C_d3d_directional_light();
	virtual ~C_d3d_directional_light();

	void		set_dir(const D3DXVECTOR3& dir)	{	m_dir = dir;	}
	D3DXVECTOR3	get_dir()						{	return m_dir;	}

	D3DXVECTOR4	get_light_local_dir(const D3DXMATRIX& world);
	D3DXVECTOR4	get_light_world_dir();
};

// ****************************************************************
// ポイントライト
// ================================================================
class C_d3d_point_light : public C_d3d_light_interface
{
	friend class C_d3d_light_manager;
protected:
	D3DXVECTOR3	m_pos;
	float		m_attenuation0;		// 一定の減衰係数
	float		m_attenuation1;		// 線形減衰係数
	float		m_attenuation2;		// 二次減衰係数
	float		m_range;			// 光源の有効距離
public:
	C_d3d_point_light();
	virtual ~C_d3d_point_light();

	void		set_pos(const D3DXVECTOR3& pos)	{	m_pos = pos;			}
	D3DXVECTOR3	get_pos()						{	return m_pos;			}
	void		set_attenuation0(float att0)	{	m_attenuation0 = att0;	}
	float		get_attenuation0()				{	return m_attenuation0;	}
	void		set_attenuation1(float att1)	{	m_attenuation1 = att1;	}
	float		get_attenuation1()				{	return m_attenuation1;	}
	void		set_attenuation2(float att2)	{	m_attenuation2 = att2;	}
	float		get_attenuation2()				{	return m_attenuation2;	}
	void		set_range(float range)			{	m_range = range;		}
	float		get_range()						{	return m_range;			}

	D3DXVECTOR4 get_light_local_pos(const D3DXMATRIX& world, const D3DXMATRIX& view);
	D3DXVECTOR4 get_light_world_pos();
	D3DXVECTOR4 get_light_attenuations_range();
};

// ****************************************************************
// スポットライト
// ================================================================
class C_d3d_spot_light : public C_d3d_light_interface
{
	friend class C_d3d_light_manager;
protected:
	D3DXVECTOR3	m_pos;
	D3DXVECTOR3	m_dir;
	float		m_theta;			// 内部コーンの角度
	float		m_phi;				// 外部コーンの角度
	float		m_falloff;			// フォールオフ
	float		m_attenuation0;		// 一定の減衰係数
	float		m_attenuation1;		// 線形減衰係数
	float		m_attenuation2;		// 二次減衰係数
	float		m_range;			// 光源の有効距離
public:
	C_d3d_spot_light();
	virtual ~C_d3d_spot_light();

	void		set_pos(const D3DXVECTOR3& pos)	{	m_pos = pos;			}
	D3DXVECTOR3	get_pos()						{	return m_pos;			}
	void		set_dir(const D3DXVECTOR3& dir)	{	m_dir = dir;			}
	D3DXVECTOR3	get_dir()						{	return m_dir;			}
	void		set_theta(float theta)			{	m_theta = theta;		}
	float		get_theta()						{	return m_theta;			}
	void		set_phi(float phi)				{	m_phi = phi;			}
	float		get_phi()						{	return m_phi;			}
	void		set_falloff(float falloff)		{	m_falloff = falloff;	}
	float		get_falloff()					{	return m_falloff;		}
	void		set_attenuation0(float att0)	{	m_attenuation0 = att0;	}
	float		get_attenuation0()				{	return m_attenuation0;	}
	void		set_attenuation1(float att1)	{	m_attenuation1 = att1;	}
	float		get_attenuation1()				{	return m_attenuation1;	}
	void		set_attenuation2(float att2)	{	m_attenuation2 = att2;	}
	float		get_attenuation2()				{	return m_attenuation2;	}
	void		set_range(float range)			{	m_range = range;		}
	float		get_range()						{	return m_range;			}

	D3DXVECTOR4 get_light_local_pos(const D3DXMATRIX& world, const D3DXMATRIX& view);
	D3DXVECTOR4 get_light_world_pos();
	D3DXVECTOR4	get_light_local_dir(const D3DXMATRIX& world);
	D3DXVECTOR4	get_light_world_dir();
	D3DXVECTOR4 get_light_theta_phi_falloff();
	D3DXVECTOR4 get_light_cos_theta_phi_falloff();
	D3DXVECTOR4 get_light_attenuations_range();
};


// ****************************************************************
// シャドウマップ用スポットライト(減衰率とか範囲は追加するかも)
// ================================================================
class C_d3d_shadow_map_spot_light : public C_d3d_light_interface
{
	friend class C_d3d_light_manager;
protected:
	D3DXVECTOR3 m_pos;					// 視点
	D3DXVECTOR3 m_tgt;					// 注視点
	D3DXVECTOR3 m_up;					// 上向きベクトル

	float		m_near_clip;			// 近接クリップ面
	float		m_far_clip;				// 遠方クリップ面
	float		m_view_volume_width;	// ビューボリュームの幅
	float		m_view_volume_height;	// ビューボリュームの高さ
public:
	C_d3d_shadow_map_spot_light();
	virtual ~C_d3d_shadow_map_spot_light();

	void		set_pos(const D3DXVECTOR3& pos)	{	m_pos = pos;	}
	D3DXVECTOR3	get_pos()						{	return m_pos;	}
	void		set_tgt(const D3DXVECTOR3& tgt)	{	m_tgt = tgt;	}
	D3DXVECTOR3	get_tgt()						{	return m_tgt;	}
	void		set_up(const D3DXVECTOR3& up)	{	m_up = up;		}
	D3DXVECTOR3	get_up()						{	return m_up;	}

	void		set_near_clip(float near_clip)	{	m_near_clip = near_clip;	}
	float		get_near_clip()					{	return m_near_clip;			}
	void		set_far_clip(float far_clip)	{	m_far_clip = far_clip;		}
	float		get_far_clip()					{	return m_far_clip;			}
	void		set_view_volume_width(float view_volume_width)		{	m_view_volume_width = view_volume_width;	}
	float		get_view_volume_width()								{	return m_view_volume_width;					}
	void		set_view_volume_height(float view_volume_height)	{	m_view_volume_height = view_volume_height;	}
	float		get_view_volume_height()							{	return m_view_volume_height;				}


	D3DXVECTOR4	get_light_local_dir(const D3DXMATRIX& world);
	D3DXVECTOR4	get_light_world_dir();

	D3DXMATRIX	get_view_matrix();
	D3DXMATRIX	get_proj_matrix();
};

// シャドウマップ用ポイントライト(全方位シャドウマップ)


// シャドウマップ用ライトはどうするか？
// 座標があるから平行光源にはならないはず
// 全方位っぽくもないので点光源でもない、つまりスポットライト？
// となるとライティング計算もスポットライトとして行うべき？
// 理論的にはおかしくなるけどライティングとシャドウのライトを別にする？
// 


/*
struct C_d3d_light
{
	C_d3d_light();
	void set_param_directional(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir);
	void set_param_point();
	void set_param_spot();

	// パラメータ
	LIGHT_TYPE		type;

	D3DXVECTOR3		pos;		// 平行光源では使わない
	D3DXVECTOR3		dir;		// 点では使わない(正規化しておくこと)

	// 各種ライト共通カラー
	D3DXVECTOR4		diffuse;
	D3DXVECTOR4		ambient;
	D3DXVECTOR4		specular;	// シェーダ側は未実装

	// 未実装
	float			range;		// 最大適用距離
	float			theta0;		// theta0の範囲まで常に最大出力
	float			theta1;		// theta0からtheta1に近づくほど弱まっていく

	// シャドウマップ用のライトはどうするか未定
};
*/


}