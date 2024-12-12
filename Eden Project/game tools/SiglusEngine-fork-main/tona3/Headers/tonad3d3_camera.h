#pragma once

namespace NT3
{

// �N���X(���)��\���^�C�v
enum CAMERA_TYPE{
	CAMERA_TYPE_NONE = -1,

	CAMERA_TYPE_NORMAL,

	CAMERA_TYPE_QUATERNION,
};

// ****************************************************************
// �J�����C���^�[�t�F�C�X
// ================================================================
class C_d3d_camera_interface
{
	// �r���[�s��p�p�����[�^�Q
	struct S_view_val
	{
		D3DXVECTOR3 pos;			// ���_
		D3DXVECTOR3 tgt;			// �����_
		D3DXVECTOR3 up;				// ������x�N�g��
	};
	
	// �ˉe�s��p�p�����[�^�Q
	struct S_proj_val
	{
		float		fov_h_deg;				// ���������̎���p(�x���@)
		float		aspect;					// �A�X�y�N�g��
		float		near_clip;				// �ߐڃN���b�v��
		float		far_clip;				// �����N���b�v��
		float		view_volume_width;	// �r���[�{�����[���̕�
		float		view_volume_height;	// �r���[�{�����[���̍���
	};
	
	// �΂ː���p�p�����[�^�Q
	struct S_spring_val
	{
		D3DXVECTOR3	pos_speed;		// ���_�p���x
		D3DXVECTOR3	tgt_speed;		// �����_�p���x
		float		constant;		// �΂˒萔
		float		friction;		// ���C�W��
		float		distance;		// ��̋���
	};

	// ������p��4���ʍ\����
	struct S_four_planes
	{
		D3DXPLANE	top;		// ���
		D3DXPLANE	bottom;		// ���
		D3DXPLANE	left;		// ����
		D3DXPLANE	right;		// �E��
	};

protected:
	C_d3d_camera_interface();

			void		calc_view_matrix();
	virtual	void		calc_proj_matrix();		// ���z�֐��Ȃ̂͊�{PerspectiveFovLH������
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

	// set�n�͊֘A����ŐV�t���O���I�t�ɂ���
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


	// get�n�͍ŐV���`�F�b�N���čŐV�łȂ���΍Čv�Z����(�ꕔ������)
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


			// �E�B���h�E�T�C�Y�ł͂Ȃ��r���[�|�[�g�T�C�Y�ł��̂��������H
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
	// �e��\����
	S_view_val		m_view_val;
	S_proj_val		m_proj_val;
	S_spring_val	m_spring_val;
	S_four_planes	m_planes;

	// �Ǘ��p
	CAMERA_TYPE		m_type;
	int				m_id;

	// ���̑�
	D3DXVECTOR3		m_dir;
	D3DXMATRIX		m_view_mat;
	D3DXMATRIX		m_proj_mat;

	// �ŐV���ǂ�������p(�g�p���ɍŐV�łȂ���΍X�V����)
	// �֘A�p�����[�^��set���ꂽ��t���O�����ł���(���܂��Ă�p�����[�^�ɒ���)
	bool			m_the_newest_dir;
	bool			m_the_newest_view_mat;
	bool			m_the_newest_proj_mat;
	bool			m_the_newest_planes;
};


// ****************************************************************
// ���ʂ̃J����
// ================================================================
class C_d3d_camera_normal : public C_d3d_camera_interface {
public:
	C_d3d_camera_normal();
	virtual ~C_d3d_camera_normal();
};


// ****************************************************************
// �N�H�[�^�j�I���J����
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
// ���̃J�����^�C�v�͉�����ɑ��삷��ƌ����̂�\��
// ��ɃX�N���v�g�ő��삵�₷���悤�ɂ���ׁH
enum CAMERA_TYPE {
	CAMERATYPE_EPU,
//	CAMERATYPE_PDRA,
};

struct C_d3d_camera
{
	C_d3d_camera();

	void			update_frame();

	// �p�����[�^
	CAMERATYPE		type;			// �^�C�v
	float			eye_radian;		// ����p
	float			aspect;			// �A�X�y�N�g��

	// �^�C�v�F�m�[�}��
	D3DXVECTOR3		eye;		// �ڂ̈ʒu
	D3DXVECTOR3		pint;		// ���Ă���ꏊ
	D3DXVECTOR3		up;			// �����

	// �^�C�v�F�v���C���[
//	D3DXVECTOR3		pos;	// �v���C���[�̈ʒu
//	float			dir;	// �v���C���[�̌���
//	float			rad;	// �J�����̔��a
//	float			ang;	// �J�����̍���
};
*/







}