#pragma once

namespace NT3
{


enum LIGHT_TYPE
{
	LIGHT_TYPE_NONE = -1,
	LIGHT_TYPE_DIRECTIONAL,		// �f�B���N�V���i�����C�g(���s����)
	LIGHT_TYPE_POINT,			// �|�C���g���C�g(�_����)
	LIGHT_TYPE_SPOT,			// �X�|�b�g���C�g

	LIGHT_TYPE_SPOT_SM,			// �V���h�E�}�b�v�p�X�|�b�g���C�g
};

// ****************************************************************
// ���C�g�C���^�[�t�F�C�X
// ================================================================
class C_d3d_light_interface
{
	friend class C_d3d_light_manager;
protected:
	C_d3d_light_interface();
	
public:
	virtual ~C_d3d_light_interface();

	// �F
	void		set_diffuse(const D3DXVECTOR4& color)	{	m_diffuse = color;	}
	void		set_ambient(const D3DXVECTOR4& color)	{	m_ambient = color;	}
	void		set_specluar(const D3DXVECTOR4& color)	{	m_specular = color;	}

	D3DXVECTOR4	get_diffuse()	{	return m_diffuse;	}
	D3DXVECTOR4	get_ambient()	{	return m_ambient;	}
	D3DXVECTOR4	get_specular()	{	return m_specular;	}

	// �^�C�v�AID
	void		set_type(LIGHT_TYPE type)	{	m_type = type;	}
	void		set_id(int id)				{	m_id = id;		}

	LIGHT_TYPE	get_type()	{	return m_type;	}
	int			get_id()	{	return m_id;	}

protected:
	// �_�C�i�~�b�N�L���X�g�Ŏ�ޔ��f������^�C�v�̕����������Ȃ̂Ń^�C�v����������
	LIGHT_TYPE		m_type;
	int				m_id;

	// �e�탉�C�g���ʃJ���[
	D3DXVECTOR4		m_diffuse;
	D3DXVECTOR4		m_ambient;
	D3DXVECTOR4		m_specular;		// �V�F�[�_���͖�����
};

// ****************************************************************
// �f�B���N�V���i�����C�g
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
// �|�C���g���C�g
// ================================================================
class C_d3d_point_light : public C_d3d_light_interface
{
	friend class C_d3d_light_manager;
protected:
	D3DXVECTOR3	m_pos;
	float		m_attenuation0;		// ���̌����W��
	float		m_attenuation1;		// ���`�����W��
	float		m_attenuation2;		// �񎟌����W��
	float		m_range;			// �����̗L������
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
// �X�|�b�g���C�g
// ================================================================
class C_d3d_spot_light : public C_d3d_light_interface
{
	friend class C_d3d_light_manager;
protected:
	D3DXVECTOR3	m_pos;
	D3DXVECTOR3	m_dir;
	float		m_theta;			// �����R�[���̊p�x
	float		m_phi;				// �O���R�[���̊p�x
	float		m_falloff;			// �t�H�[���I�t
	float		m_attenuation0;		// ���̌����W��
	float		m_attenuation1;		// ���`�����W��
	float		m_attenuation2;		// �񎟌����W��
	float		m_range;			// �����̗L������
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
// �V���h�E�}�b�v�p�X�|�b�g���C�g(�������Ƃ��͈͂͒ǉ����邩��)
// ================================================================
class C_d3d_shadow_map_spot_light : public C_d3d_light_interface
{
	friend class C_d3d_light_manager;
protected:
	D3DXVECTOR3 m_pos;					// ���_
	D3DXVECTOR3 m_tgt;					// �����_
	D3DXVECTOR3 m_up;					// ������x�N�g��

	float		m_near_clip;			// �ߐڃN���b�v��
	float		m_far_clip;				// �����N���b�v��
	float		m_view_volume_width;	// �r���[�{�����[���̕�
	float		m_view_volume_height;	// �r���[�{�����[���̍���
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

// �V���h�E�}�b�v�p�|�C���g���C�g(�S���ʃV���h�E�}�b�v)


// �V���h�E�}�b�v�p���C�g�͂ǂ����邩�H
// ���W�����邩�畽�s�����ɂ͂Ȃ�Ȃ��͂�
// �S���ʂ��ۂ����Ȃ��̂œ_�����ł��Ȃ��A�܂�X�|�b�g���C�g�H
// �ƂȂ�ƃ��C�e�B���O�v�Z���X�|�b�g���C�g�Ƃ��čs���ׂ��H
// ���_�I�ɂ͂��������Ȃ邯�ǃ��C�e�B���O�ƃV���h�E�̃��C�g��ʂɂ���H
// 


/*
struct C_d3d_light
{
	C_d3d_light();
	void set_param_directional(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir);
	void set_param_point();
	void set_param_spot();

	// �p�����[�^
	LIGHT_TYPE		type;

	D3DXVECTOR3		pos;		// ���s�����ł͎g��Ȃ�
	D3DXVECTOR3		dir;		// �_�ł͎g��Ȃ�(���K�����Ă�������)

	// �e�탉�C�g���ʃJ���[
	D3DXVECTOR4		diffuse;
	D3DXVECTOR4		ambient;
	D3DXVECTOR4		specular;	// �V�F�[�_���͖�����

	// ������
	float			range;		// �ő�K�p����
	float			theta0;		// theta0�͈̔͂܂ŏ�ɍő�o��
	float			theta1;		// theta0����theta1�ɋ߂Â��قǎ�܂��Ă���

	// �V���h�E�}�b�v�p�̃��C�g�͂ǂ����邩����
};
*/


}