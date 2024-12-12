#pragma		once

#include	"tona3_util.h"

namespace NT3
{

// ****************************************************************
// ���͂��ȈՂɎ擾
// ================================================================
C_point	get_mouse_pos();				// �}�E�X�̈ʒu���擾�i�X�N���[����ԁj
bool	is_key_down(int winkey_code);	// Windows ���z�L�[��������Ă��邩�ǂ�������

// ****************************************************************
// Tona3 �L�[�R�[�h
// ================================================================
const int	T3_KEY_CNT = 256;

// Windows ���z�L�[
const int	T3_WINKEY_TOP = 0;
const int	T3_WINKEY_CNT = 256;

// DirectInput �L�[
//const int	T3_DIKEY_TOP = 256;
//const int	T3_DIKEY_CNT = 256;

// �t���[������
const int	T3_HISTORY_NUM = 2;

// ****************************************************************
// DirectInput
// ================================================================
/*
class C_direct_input
{
public:
	C_direct_input();
	~C_direct_input();

	bool init_dinput(HWND h_wnd);
	bool release_dinput();

	bool get_input_from_joypad(DIJOYSTATE2& dijs);
	bool get_input_from_keyboard(int size, BYTE key[]);

private:

	LPDIRECTINPUT8	m_p_dinput;					// DirectInput
	LPDIRECTINPUTDEVICE8 m_p_dev_keyboard;		// DirectInput �L�[�{�[�h�f�o�C�X
	LPDIRECTINPUTDEVICE8 m_p_dev_joypad;		// DirectInput �p�b�h�f�o�C�X�̔z��
	DIDEVCAPS m_dev_caps;

	static BOOL CALLBACK enum_joypad_callback(const DIDEVICEINSTANCE* p_dev_instance, void* p_context);
	static BOOL CALLBACK enum_axes_callback(LPCDIDEVICEOBJECTINSTANCE p_dev_object_instance, LPVOID pv_ref);
};

extern	C_direct_input G_dinput;
*/

// ****************************************************************
// ���͏��
// ================================================================
struct C_input_state
{
	// --- �{�^�� ---
	struct BUTTON
	{
		BUTTON() : real_state(0), down_stock(0), up_stock(0), down_up_stock(0), move_x(0), move_y(0)	{}

		void	clear();					// �N���A
		void	use();						// ����
		void	frame();					// �t���[��
		void	set_move(int x, int y);		// �u���������v��ݒ�
		void	set_down(int x, int y);		// �u�������v��ݒ�
		void	set_up(int x, int y);		// �u�������v��ݒ�
		void	unset_up();					// �u�������v���폜
		void	set_real_state(int state);

		bool	is_down()			{	return real_state == 1;		}	// ���݉����Ă���
		bool	is_up()				{	return real_state == 0;		}	// ���ݕ����Ă���
		bool	check_down_stock();			// �u�������v���X�g�b�N����Ă��邩�𔻒�
		bool	check_up_stock();			// �u�������v���X�g�b�N����Ă��邩�𔻒�
		bool	check_down_up_stock();		// �u�����ĕ������v���X�g�b�N����Ă��邩�𔻒�
		bool	check_flick_stock();		// �u�t���b�N�v���X�g�b�N����Ă��邩�𔻒�
		bool	use_down_stock();			// �u�������v�̃X�g�b�N������
		bool	use_up_stock();				// �u�������v�̃X�g�b�N������
		bool	use_down_up_stock();		// �u�����ĕ������v�̃X�g�b�N������
		bool	use_flick_stock();			// �u�t���b�N�v�̃X�g�b�N������

		int		get_real_state()		{	return real_state;			}	// ���݂̏�Ԃ��擾
		int		get_down_stock()		{	return down_stock;			}	// ���݂́u�������v�X�g�b�N���擾
		int		get_up_stock()			{	return up_stock;			}	// ���݂́u�������v�X�g�b�N���擾
		int		get_down_up_stock()		{	return down_up_stock;		}	// ���݂́u�����ė������v�X�g�b�N���擾

		double	get_flick_angle() { return flick_angle; }
		double	get_flick_pixel() { return flick_pixel; }
		double	get_flick_mm() { return flick_mm; }

	private:

		int		real_state;				// ���݂̏�ԁi0:�����Ă��� 1:�����Ă���j
		int		down_stock;				// �u�������v�̃X�g�b�N�i0:�����ĂȂ� 1:�������j
		int		up_stock;				// �u�������v�̃X�g�b�N�i0:�����ĂȂ� 1:�������j
		int		down_up_stock;			// �u�����ĕ������v�̃X�g�b�N�i0:�����ĂȂ� 1:������ 2:�����ĕ������j
		int		move_x;
		int		move_y;

		DWORD	prev_time[T3_HISTORY_NUM];
		int		prev_x[T3_HISTORY_NUM];
		int		prev_y[T3_HISTORY_NUM];

		int		flick_stock;
		double	flick_angle;
		double	flick_pixel;			// [pixel/s]
		double	flick_mm;				// [mm/s]
	};

	// --- �z�C�[�� ---
	struct WHEEL
	{
		WHEEL() : up_stock(0), down_stock(0)	{}

		void	clear();		// �N���A
		void	use();			// ����
		void	frame();		// �t���[��

		void	set_up(int up_cnt);							// ���]
		void	set_down(int down_cnt);						// ����]
		void	set_wheel_by_wm(WPARAM wp, LPARAM lp);		// WM_MOUSEWHEEL
		bool	use_up_stock();								// ���]������
		bool	use_down_stock();							// ����]������
		int		get_up_cnt()	{	return up_stock;	}	// ���]�����擾
		int		get_down_cnt()	{	return down_stock;	}	// ����]�����擾

		int		up_stock;		// ���]�̉�
		int		down_stock;		// ����]�̉�
	};

	// --- �}�E�X ---
	struct MOUSE
	{
		void	clear();	// �N���A
		void	use();		// ����
		void	frame();	// �t���[��

		C_point	pos;
		BUTTON	left;
		BUTTON	right;
		BUTTON	middle;
		WHEEL	wheel;
	};

	// --- �L�[�{�[�h ---
	struct KEYBOARD
	{
		void	clear();	// �N���A
		void	use();		// ����
		void	frame();	// �t���[��

		void	set_down_by_wm(WPARAM wp, LPARAM lp);
		void	set_up_by_wm(WPARAM wp, LPARAM lp);
		int		use_down_stock();
		int		use_up_stock();

		BUTTON	key[T3_KEY_CNT];
	};

	// --- �X�e�[�g ---
	MOUSE		mouse;
	KEYBOARD	keyboard;

	// --- ���� ---
	void	clear();	// �N���A
	void	use();		// ����
	void	frame();	// �t���[��
};

// ****************************************************************
// ����
// ================================================================
//
//		Cinput �N���X�̃C���X�^���X�́A�A�v���P�[�V�������Ƃɍ���Ă��������B
//		next �ɓ��͂�^���܂��B�ʏ�̓E�B���h�E���b�Z�[�W�ŗ^���܂����A���̏ꍇ�� Cstd_input ���g���ƊȒP�ł��B
//		frame ���Ăяo���Anow �Ŏ��o���܂��B
//
//		�t���[�������̐擪�� frame() ���Ăяo���܂��B
//
//		input.frame();
//		
//		���o���� now ���g���܂��B
//
//		if (input.now.mouse.left.use_down())	{ ... }		// �}�E�X�����������Ƃ��̏���
//			
// ================================================================
class C_input_base
{
public:
	C_input_base();

	// �N���A
	void	clear();

	// �t���[������
	void	frame();

	// �X�e�[�g
	C_input_state	now;
	C_input_state	next;
};

// ****************************************************************
// �W������
// ================================================================
//
//		�L�[�{�[�h��}�E�X�ɑ΂��ĕW���I�ȏ������s���܂��B
//		���͂�Ǝ��ɗ^������G�~�����[�g����ꍇ�� input_base ���g���ĉ������B
//
// ================================================================
class C_std_input : public C_input_base
{
public:

	// �t���[������
	void	frame();

	// �W���I�ȓ��̓��b�Z�[�W�������iWM_LBUTTONDOWN�AWM_LBUTTONUP�AWM_RBUTTONDOWN�AWM_RBUTTONUP�AWM_MOUSEWHEEL�AWM_KEYDOWN�AWM_KEYUP�j
	void	set_state_by_wm(UINT msg, WPARAM wp, LPARAM lp);
};


}
