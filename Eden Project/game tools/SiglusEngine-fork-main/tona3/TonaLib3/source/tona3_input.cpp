#include	"tona3_pch.h"
#include	"tona3_input.h"
#include	"tona3_error.h"
#include	"tona3_app.h"

#define __USE_DINPUT	1



namespace NT3
{

#if 0

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

C_direct_input::C_direct_input()
{
	m_p_dinput = NULL;
	m_p_dev_keyboard = NULL;
	m_p_dev_joypad = NULL;
}

C_direct_input::~C_direct_input()
{
}

bool C_direct_input::init_dinput(HWND h_wnd)
{
#ifdef	__USE_DINPUT

	HRESULT hr;
	DIPROPDWORD diprop;

	// DirectInput�̍쐬
	hr = DirectInput8Create(G_app.h_inst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_p_dinput, NULL);
	if (FAILED(hr))	{
		set_last_error(_T("DirectInput8 �I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B"), _T("init_dinput"));
		return false;
	}

	// �L�[�{�[�h�f�o�C�X�I�u�W�F�N�g�̍쐬
	hr = m_p_dinput->CreateDevice(GUID_SysKeyboard, &m_p_dev_keyboard, NULL);
	if (FAILED(hr))	{
		set_last_error(_T("DirectInputDevice8 �I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B(Keyboard)"), _T("init_dinput"));
		return false;
	}

	// ���[�h��ݒ�
	hr = m_p_dev_keyboard->SetCooperativeLevel(h_wnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))	{
		set_last_error(_T("�t�H�A�O���E���h����r���I���[�h�̐ݒ�Ɏ��s���܂����B(Keyboard)"), _T("init_dinput"));
		return false;
	}

	// �f�[�^�`����ݒ�
	hr = m_p_dev_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))	{
		set_last_error(_T("c_dfDIKeyboard�̐ݒ�Ɏ��s���܂����B(Keyboard)"), _T("init_dinput"));
		return false;
	}

	// �o�b�t�@�T�C�Y��ݒ�
	ZeroMemory(&diprop, sizeof(DIPROPDWORD));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = 100;
	hr = m_p_dev_keyboard->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
	if (FAILED(hr))	{
		set_last_error(_T("�o�b�t�@�T�C�Y�̐ݒ�Ɏ��s���܂����B(Keyboard)"), _T("init_dinput"));
		return false;
	}

	// ���͐���J�n
	m_p_dev_keyboard->Acquire();

	/*
	// �p�b�h�f�o�C�X�I�u�W�F�N�g�̗�
	hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, (LPVOID)(this), DIEDBSFL_ATTACHEDONLY);
	if (FAILED(hr) || m_pDIDeviceJoypad == NULL)	{
		set_last_error(_T("DirectInputDevice8�I�u�W�F�N�g�̗񋓂Ɏ��s(Joypad)"), _T("InitDInput"));
		//set_last_error("JoyPad�̌��o�Ɏ��s���܂����B�L�[�{�[�h�ŃQ�[�������y���݂�������");
		return true;
	}

	// �f�[�^�`����ݒ�
	hr = m_pDIDeviceJoypad->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))	{
		set_last_error(_T("c_dfDIJoystick2�̐ݒ�Ɏ��s(Joypad)"), _T("InitDInput"));
		//set_last_error("JoyPad�̌��o�Ɏ��s���܂����B�L�[�{�[�h�ŃQ�[�������y���݂�������");
		return true;
	}

	// ���[�h��ݒ�
	hr = m_pDIDeviceJoypad->SetCooperativeLevel(hwndApp, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))	{
		set_last_error(_T("�t�H�A�O���E���h����r���I���[�h�̐ݒ�Ɏ��s(Joypad)"), _T("InitDInput"));
		//set_last_error("JoyPad�̌��o�Ɏ��s���܂����B�L�[�{�[�h�ŃQ�[�������y���݂�������");
		return true;
	}

	// ���̒l�͈̔͂�ݒ�
	hr = m_pDIDeviceJoypad->EnumObjects(EnumAxesCallback, (LPVOID)(this), DIDFT_AXIS);
	if (FAILED(hr))	{
		set_last_error(_T("���̗񋓂Ɏ��s(Joypad)"), _T("InitDInput"));
		//set_last_error("JoyPad�̌��o�Ɏ��s���܂����B�L�[�{�[�h�ŃQ�[�������y���݂�������");
		return true;
	}

	// �o�b�t�@�T�C�Y��ݒ�
	ZeroMemory(&diprop, sizeof(DIPROPDWORD));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = 100;
	hr = m_pDIDeviceJoypad->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
	if (FAILED(hr))	{
		set_last_error(_T("�o�b�t�@�T�C�Y�̐ݒ�Ɏ��s(Joypad)"), _T("InitDInput"));
		//set_last_error("JoyPad�̌��o�Ɏ��s���܂����B�L�[�{�[�h�ŃQ�[�������y���݂�������");
		return true;
	}

	// ���͐���J�n
	m_pDIDeviceJoypad->Acquire();
	*/

	return true;

#else
	return true;	// �g��Ȃ��ꍇ�͏������������s��Ȃ�
#endif

}

BOOL CALLBACK C_direct_input::enum_joypad_callback(const DIDEVICEINSTANCE* p_dev_instance, LPVOID pv_ref)
{
	HRESULT hr;

	C_direct_input* p_dinput = (C_direct_input *)pv_ref;

	// �񋓂��ꂽ�p�b�h�̃C���^�[�t�F�C�X���擾
	hr = p_dinput->m_p_dinput->CreateDevice(p_dev_instance->guidInstance, &(p_dinput->m_p_dev_joypad), NULL);
	if (FAILED(hr))
		return DIENUM_CONTINUE;		// �擾�Ɏ��s�����̂ŁA���̃f�o�C�X��T��

	// �p�b�h�̔\�͂𒲂ׂ�
	p_dinput->m_dev_caps.dwSize = sizeof(DIDEVCAPS);
	hr = p_dinput->m_p_dev_joypad->GetCapabilities(&(p_dinput->m_dev_caps));
	if (FAILED(hr))	{
		p_dinput->m_p_dev_joypad->Release();
		p_dinput->m_p_dev_joypad = NULL;
		return DIENUM_CONTINUE;		// �擾�Ɏ��s�����̂ŁA���̃f�o�C�X��T�� 
	}

	// �p�b�h�擾����
	return DIENUM_STOP;				// �P��������I��
}

BOOL CALLBACK C_direct_input::enum_axes_callback(LPCDIDEVICEOBJECTINSTANCE p_doi, LPVOID pv_ref)
{
	HRESULT hr;

	C_direct_input* p_dinput = (C_direct_input *)pv_ref;

	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = p_doi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	hr = p_dinput->m_p_dev_joypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
		return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

bool C_direct_input::release_dinput()
{

	if (m_p_dev_keyboard)	{
		m_p_dev_keyboard->Unacquire();
		m_p_dev_keyboard->Release();
		m_p_dev_keyboard = NULL;
	}

	if (m_p_dev_joypad)	{
		m_p_dev_joypad->Unacquire();
		m_p_dev_joypad->Release();
		m_p_dev_joypad = NULL;
	}

	if (m_p_dinput)	{
		m_p_dinput->Release();
		m_p_dinput = NULL;
	}

	return true;
}

bool C_direct_input::get_input_from_joypad(DIJOYSTATE2& dijs)
{
	HRESULT hr;

	// �p�b�h���g�p����Ă��Ȃ��Ȃ� false
	if (!m_p_dev_joypad)
		return false;

	// �f�[�^�̗L�����f�o�C�X�Ƀ|�[�����O����
	hr = m_p_dev_joypad->Poll();
	if (FAILED(hr))	{
		hr = m_p_dev_joypad->Acquire();
		return true;
	}

	// �p�b�h�f�[�^���擾
	hr = m_p_dev_joypad->GetDeviceState(sizeof(DIJOYSTATE2), &dijs);
	if (FAILED(hr))	{
		hr = m_p_dev_joypad->Acquire();
		return false;
	}

	return true;
}


bool C_direct_input::get_input_from_keyboard(int size, BYTE key[])
{
	HRESULT hr;

	// �L�[�{�[�h���g�p����Ă��Ȃ��Ȃ� false
	if (!m_p_dev_keyboard)
		return false;

	// DirectInput�ɂ��L�[�{�[�h����̓��͂��`�F�b�N
	hr = m_p_dev_keyboard->GetDeviceState(size, key);
	if (FAILED(hr))	{
		m_p_dev_keyboard->Acquire();
		return true;
	}

	return true;
}

C_direct_input G_dinput;

#endif

// ****************************************************************
// �}�E�X�J�[�\���̈ʒu���擾
// ================================================================
C_point get_mouse_pos()
{
	C_point pos;
	::GetCursorPos(&pos);
	return pos;
}

// ****************************************************************
// �L�[�{�[�h�̏�Ԃ��擾
// ================================================================
bool is_key_down(int winkey_code)
{
	return (::GetKeyState(winkey_code) & 0x8000) ? true : false;
}

// ****************************************************************
// �{�^���F�N���A
// ================================================================
void C_input_state::BUTTON::clear()
{
	down_stock = 0;
	up_stock = 0;
	down_up_stock = 0;

	for (int i = 0; i < T3_HISTORY_NUM; i++)
	{
		prev_time[i] = 0;
		prev_x[i] = 0;
		prev_y[i] = 0;
	}
}

// ****************************************************************
// �{�^���F����
// ================================================================
void C_input_state::BUTTON::use()
{
	down_stock = 0;
	up_stock = 0;
	down_up_stock = 0;
}

// ****************************************************************
// �{�^���F�t���[��
// ================================================================
//		�l���ŏI�l�܂Ői��ł���΃N���A���܂��B
// ================================================================
void C_input_state::BUTTON::frame()
{
	// �ŏI�l�܂Ői�񂾏�Ԃ��N���A
	if (down_stock == 1)			down_stock = 0;
	if (up_stock == 1)				up_stock = 0;
	if (down_up_stock == 2)			down_up_stock = 0;
	if (flick_stock == 1)			flick_stock = 0;

	// 10ms �ȏ�o���Ă���Η������X�V
	int new_time = (int)::timeGetTime();
	if (prev_time[0] == 0 || new_time - prev_time[0] >= 10)
	{
		for (int i = T3_HISTORY_NUM - 1; i >= 1; i--)
		{
			prev_time[i] = prev_time[i - 1];
			prev_x[i] = prev_x[i - 1];
			prev_y[i] = prev_y[i - 1];
		}

		prev_time[0] = new_time;
		prev_x[0] = move_x;
		prev_y[0] = move_y;
	}
}

// ****************************************************************
// �{�^���F����������ݒ�
// ================================================================
void C_input_state::BUTTON::set_move(int x, int y)
{
	static int range = 32;

	move_x += x;
	move_y += y;
}

// ****************************************************************
// �{�^���F��������ݒ�
// ================================================================
void C_input_state::BUTTON::set_down(int x, int y)
{
	real_state = 1;

	if (down_stock == 0)	{
		down_stock = 1;			// �u�������v��i�߂�
	}
	if (down_up_stock == 0)	{
		down_up_stock = 1;		// �u�����ĕ������v��i�߂�
	}

	// �u�t���b�N�v��i�߂�
	move_x = 0;
	move_y = 0;

	for (int i = 0; i < T3_HISTORY_NUM; i++)
	{
		prev_time[i] = 0;
		prev_x[i] = 0;
		prev_y[i] = 0;
	}
}

// ****************************************************************
// �{�^���F��������ݒ�
// ================================================================
void C_input_state::BUTTON::set_up(int x, int y)
{
	real_state = 0;

	if (up_stock == 0)
	{
		up_stock = 1;			// �u�������v��i�߂�
	}
	if (down_up_stock == 1)
	{
		down_up_stock = 2;		// �u�����ĕ������v��i�߂�
	}

	// �u�t���b�N�v��i�߂�
	if (prev_time[T3_HISTORY_NUM - 1] > 0)
	{
		int diff_time = prev_time[0] - prev_time[T3_HISTORY_NUM - 1];
		double diff_pixel_x = (double)(prev_x[0] - prev_x[T3_HISTORY_NUM - 1]);
		double diff_pixel_y = (double)(prev_y[0] - prev_y[T3_HISTORY_NUM - 1]);
		double diff_pixel = std::sqrt(diff_pixel_x * diff_pixel_x + diff_pixel_y * diff_pixel_y);
		double speed_pixel = diff_pixel * 1000 / diff_time;

		HDC h_dc = GetDC(0);
		int monitor_dpi_x = GetDeviceCaps(h_dc, LOGPIXELSX);
		int monitor_dpi_y = GetDeviceCaps(h_dc, LOGPIXELSY);
		ReleaseDC(0, h_dc);

		double diff_mm_x = (double)diff_pixel_x / monitor_dpi_x * 0.0254f * 1000;
		double diff_mm_y = (double)diff_pixel_y / monitor_dpi_y * 0.0254f * 1000;
		double diff_mm = std::sqrt(diff_mm_x * diff_mm_x + diff_mm_y * diff_mm_y);
		double speed_mm = diff_mm * 1000 / diff_time;

		double angle = atan2(diff_mm_x, diff_mm_y);

		flick_angle = angle;
		flick_pixel = speed_pixel;
		flick_mm = speed_mm;

		if (speed_mm >= 180)
		{
			flick_stock = 1;
		}

		//message_box(str_format(_T("dpi x=%d, y=%d\n�t���b�N�v������ = %d\n�t���b�N���� = %lf[px/s], %lf[mm/s]")
		//	, monitor_dpi_x, monitor_dpi_y, diff_time, speed_pixel, speed_mm
		//	) + (speed_mm >= 180 ? _T("\n\n�t���b�N�ł��I(180mm/s �ȏ�)") : _T("\n\n�t���b�N�ł͂���܂���B(180mm/s ����)")), _T("Siglus"), MB_OK);
	}
}

// ****************************************************************
// �{�^���F���������폜
// ================================================================
//		�u�P�t���[�����ɉ������������𓯎��ɏ������Ȃ��v�Ƃ��Ɏg���܂��B
//		down_up ���P�x�� 0��2 �ɂȂ����Ƃ��ɌĂяo���ĉ������B
// ================================================================
void C_input_state::BUTTON::unset_up()
{
	if (real_state == 0 && up_stock == 1 && down_up_stock == 2)	{
		real_state = 1;
		up_stock = 0;
		down_up_stock = 1;
	}
}

// ****************************************************************
// �{�^���F���A����Ԃ�ݒ�
// ================================================================
void C_input_state::BUTTON::set_real_state(int state)
{
	real_state = state;
}

// ****************************************************************
// �{�^���F�u�������v���X�g�b�N����Ă��邩�𔻒�
// ================================================================
bool C_input_state::BUTTON::check_down_stock()
{
	if (down_stock == 1)
		return true;

	return false;
}

// ****************************************************************
// �{�^���F�u�������v���X�g�b�N����Ă��邩�𔻒�
// ================================================================
bool C_input_state::BUTTON::check_up_stock()
{
	if (up_stock == 1)
		return true;

	return false;
}

// ****************************************************************
// �{�^���F�u�����ĕ������v���X�g�b�N����Ă��邩�𔻒�
// ================================================================
bool C_input_state::BUTTON::check_down_up_stock()
{
	if (down_up_stock == 2)
		return true;

	return false;
}

// ****************************************************************
// �{�^���F�u�t���b�N�v���X�g�b�N����Ă��邩�𔻒�
// ================================================================
bool C_input_state::BUTTON::check_flick_stock()
{
	if (flick_stock == 1)
		return true;

	return false;
}

/*
#define		M_PI		3.14159f

// ****************************************************************
// �{�^���F�u�t���b�N���v���X�g�b�N����Ă��邩�𔻒�
// ================================================================
bool C_input_state::BUTTON::check_flick_up()
{
	if (flick_stock == 1)
	{
		if (flick_angle < - M_PI * 3 / 4 || M_PI * 3 / 4 <= flick_angle)
		{
			return true;
		}
	}

	return false;
}

// ****************************************************************
// �{�^���F�u�t���b�N���v���X�g�b�N����Ă��邩�𔻒�
// ================================================================
bool C_input_state::BUTTON::check_flick_left()
{
	if (flick_stock == 1)
	{
		if (-M_PI * 3 / 4 <= flick_angle && flick_angle < -M_PI * 1 / 4)
		{
			return true;
		}
	}

	return false;
}

// ****************************************************************
// �{�^���F�u�t���b�N���v���X�g�b�N����Ă��邩�𔻒�
// ================================================================
bool C_input_state::BUTTON::check_flick_down()
{
	if (flick_stock == 1)
	{
		if (-M_PI * 1 / 4 <= flick_angle && flick_angle < M_PI * 1 / 4)
		{
			return true;
		}
	}

	return false;
}

// ****************************************************************
// �{�^���F�u�t���b�N���v���X�g�b�N����Ă��邩�𔻒�
// ================================================================
bool C_input_state::BUTTON::check_flick_right()
{
	if (flick_stock == 1)
	{
		if (M_PI * 1 / 4 <= flick_angle && flick_angle < M_PI * 3 / 4)
		{
			return true;
		}
	}

	return false;
}
*/

// ****************************************************************
// �{�^���F�u�������v�̃X�g�b�N������
// ================================================================
bool C_input_state::BUTTON::use_down_stock()
{
	if (down_stock == 1)	{
		down_stock = 0;
		down_up_stock = 0;
		return true;
	}

	return false;
}

// ****************************************************************
// �{�^���F�u�������v�̃X�g�b�N������
// ================================================================
bool C_input_state::BUTTON::use_up_stock()
{
	if (up_stock == 1)	{
		up_stock = 0;
		down_up_stock = 0;
		return true;
	}

	return false;
}

// ****************************************************************
// �{�^���F�u�����ĕ������v�̃X�g�b�N������
// ================================================================
bool C_input_state::BUTTON::use_down_up_stock()
{
	if (down_up_stock == 2)	{
		down_stock = 0;
		up_stock = 0;
		down_up_stock = 0;
		return true;
	}

	return false;
}

// ****************************************************************
// �{�^���F�u�t���b�N�v�̃X�g�b�N������
// ================================================================
bool C_input_state::BUTTON::use_flick_stock()
{
	if (flick_stock == 1)	{
		down_stock = 0;
		up_stock = 0;
		down_up_stock = 0;
		flick_stock = 0;
		return true;
	}

	return false;
}

// ****************************************************************
// �z�C�[���F�N���A
// ================================================================
void C_input_state::WHEEL::clear()
{
	up_stock = 0;
	down_stock = 0;
}

// ****************************************************************
// �z�C�[���F����
// ================================================================
void C_input_state::WHEEL::use()
{
	up_stock = 0;
	down_stock = 0;
}

// ****************************************************************
// �z�C�[���F�t���[��
// ================================================================
void C_input_state::WHEEL::frame()
{
	up_stock = 0;
	down_stock = 0;
}

// ****************************************************************
// �z�C�[���F���]
// ================================================================
void C_input_state::WHEEL::set_up(int up_cnt)
{
	up_stock += up_cnt;
}

// ****************************************************************
// �z�C�[���F����]
// ================================================================
void C_input_state::WHEEL::set_down(int down_cnt)
{
	down_stock += down_cnt;
}

// ****************************************************************
// �z�C�[���FWM_MOUSEWHEEL
// ================================================================
void C_input_state::WHEEL::set_wheel_by_wm(WPARAM wp, LPARAM lp)
{
	int delta = GET_WHEEL_DELTA_WPARAM(wp);
	if (delta > 0)
		up_stock += (delta + WHEEL_DELTA - 1) / WHEEL_DELTA;		// �؂�グ
	if (delta < 0)
		down_stock += (- delta + WHEEL_DELTA - 1) / WHEEL_DELTA;	// �؂�グ
}

// ****************************************************************
// �z�C�[���F���]������
// ================================================================
bool C_input_state::WHEEL::use_up_stock()
{
	if (up_stock > 0)	{
		up_stock --;
		return true;
	}

	return false;
}

// ****************************************************************
// �z�C�[���F����]������
// ================================================================
bool C_input_state::WHEEL::use_down_stock()
{
	if (down_stock > 0)	{
		down_stock --;
		return true;
	}

	return false;
}

// ****************************************************************
// �}�E�X�F�N���A
// ================================================================
void C_input_state::MOUSE::clear()
{
	left.clear();
	right.clear();
	middle.clear();
	wheel.clear();
}

// ****************************************************************
// �}�E�X�F����
// ================================================================
void C_input_state::MOUSE::use()
{
	left.use();
	right.use();
	middle.use();
	wheel.use();
}

// ****************************************************************
// �}�E�X�F�t���[��
// ================================================================
void C_input_state::MOUSE::frame()
{
	left.frame();
	right.frame();
	middle.frame();
	wheel.frame();
}

// ****************************************************************
// �L�[�{�[�h�F�N���A
// ================================================================
void C_input_state::KEYBOARD::clear()
{
	for (int i = 0; i < T3_KEY_CNT; i++)
		key[i].clear();
}

// ****************************************************************
// �L�[�{�[�h�F����
// ================================================================
void C_input_state::KEYBOARD::use()
{
	for (int i = 0; i < T3_KEY_CNT; i++)
		key[i].use();
}

// ****************************************************************
// �L�[�{�[�h�F�t���[��
// ================================================================
void C_input_state::KEYBOARD::frame()
{
	for (int i = 0; i < T3_KEY_CNT; i++)
		key[i].frame();
}

// ****************************************************************
// �L�[�{�[�h�FWM_KEYDOWN
// ================================================================
void C_input_state::KEYBOARD::set_down_by_wm(WPARAM wp, LPARAM lp)
{
	if (((lp >> 30) & 1) == 0)	{	// ����̂�
		key[wp].set_down(0, 0);

		// Windows �g���L�[
		int win_extend_key = -1;

		if (false);
		else if (wp == VK_SHIFT && ((lp >> 24) & 1) == 0)		win_extend_key = VK_LSHIFT;
		else if (wp == VK_SHIFT && ((lp >> 24) & 1) == 1)		win_extend_key = VK_RSHIFT;
		else if (wp == VK_CONTROL && ((lp >> 24) & 1) == 0)		win_extend_key = VK_LCONTROL;
		else if (wp == VK_CONTROL && ((lp >> 24) & 1) == 1)		win_extend_key = VK_RCONTROL;
		else if (wp == VK_MENU && ((lp >> 24) & 1) == 0)		win_extend_key = VK_LMENU;
		else if (wp == VK_MENU && ((lp >> 24) & 1) == 1)		win_extend_key = VK_RMENU;

		if (0 <= win_extend_key && win_extend_key < 256)
			key[win_extend_key].set_down(0, 0);
	}
}

// ****************************************************************
// �L�[�{�[�h�FWM_KEYUP
// ================================================================
void C_input_state::KEYBOARD::set_up_by_wm(WPARAM wp, LPARAM lp)
{
	if (((lp >> 30) & 1) == 1)	{	// ����̂�
		key[wp].set_up(0, 0);

		// Windows �g���L�[
		int extend_key = -1;

		if (false);
		else if (wp == VK_SHIFT && ((lp >> 24) & 1) == 0)		extend_key = VK_LSHIFT;
		else if (wp == VK_SHIFT && ((lp >> 24) & 1) == 1)		extend_key = VK_RSHIFT;
		else if (wp == VK_CONTROL && ((lp >> 24) & 1) == 0)		extend_key = VK_LCONTROL;
		else if (wp == VK_CONTROL && ((lp >> 24) & 1) == 1)		extend_key = VK_RCONTROL;
		else if (wp == VK_MENU && ((lp >> 24) & 1) == 0)		extend_key = VK_LMENU;
		else if (wp == VK_MENU && ((lp >> 24) & 1) == 1)		extend_key = VK_RMENU;

		if (0 <= extend_key && extend_key < 256)
			key[extend_key].set_up(0, 0);
	}
}

// ****************************************************************
// �L�[�{�[�h�F������������
// ================================================================
int C_input_state::KEYBOARD::use_down_stock()
{
	for (int i = 0; i < T3_KEY_CNT; i++)	{
		if (key[i].use_down_stock())
			return i;	// �L�[�R�[�h��Ԃ�
	}

	return -1;
}

// ****************************************************************
// �L�[�{�[�h�F������������
// ================================================================
int C_input_state::KEYBOARD::use_up_stock()
{
	for (int i = 0; i < T3_KEY_CNT; i++)	{
		if (key[i].use_up_stock())
			return i;	// �L�[�R�[�h��Ԃ�
	}

	return -1;
}

// ****************************************************************
// �X�e�[�g�F�N���A
// ================================================================
void C_input_state::clear()
{
	mouse.clear();
	keyboard.clear();
}

// ****************************************************************
// �X�e�[�g�F����
// ================================================================
void C_input_state::use()
{
	mouse.use();
	keyboard.use();
}

// ****************************************************************
// �X�e�[�g�F�t���[��
// ================================================================
void C_input_state::frame()
{
	mouse.frame();
	keyboard.frame();
}

// ****************************************************************
// ���́F�R���X�g���N�^
// ================================================================
C_input_base::C_input_base()
{
	clear();
}

// ****************************************************************
// ���́F�N���A
// ================================================================
void C_input_base::clear()
{
	now.clear();
	next.clear();
}

// ****************************************************************
// ���́F�t���[������
// ================================================================
void C_input_base::frame()
{
	C_point last_pos = now.mouse.pos;

	// �t���[������
	now.frame();

	// �l�N�X�g�����Ԃ��擾
	now.mouse.pos = next.mouse.pos;
	now.mouse.left.set_real_state(next.mouse.left.get_real_state());
	now.mouse.right.set_real_state(next.mouse.right.get_real_state());
	now.mouse.middle.set_real_state(next.mouse.middle.get_real_state());
	if (next.mouse.left.check_down_stock())
		now.mouse.left.set_down(now.mouse.pos.x, now.mouse.pos.y);
	if (next.mouse.left.check_up_stock())
		now.mouse.left.set_up(now.mouse.pos.x, now.mouse.pos.y);
	if (next.mouse.right.check_down_stock())
		now.mouse.right.set_down(now.mouse.pos.x, now.mouse.pos.y);
	if (next.mouse.right.check_up_stock())
		now.mouse.right.set_up(now.mouse.pos.x, now.mouse.pos.y);
	if (next.mouse.middle.check_down_stock())
		now.mouse.middle.set_down(now.mouse.pos.x, now.mouse.pos.y);
	if (next.mouse.middle.check_up_stock())
		now.mouse.middle.set_up(now.mouse.pos.x, now.mouse.pos.y);
	now.mouse.left.set_move(next.mouse.pos.x - last_pos.x, next.mouse.pos.y - last_pos.y);
	now.mouse.right.set_move(next.mouse.pos.x - last_pos.x, next.mouse.pos.y - last_pos.y);
	now.mouse.wheel.set_down(next.mouse.wheel.get_down_cnt());
	now.mouse.wheel.set_up(next.mouse.wheel.get_up_cnt());

	for (int i = 0; i < T3_KEY_CNT; i++)	{
		now.keyboard.key[i].set_real_state(next.keyboard.key[i].get_real_state());
		if (next.keyboard.key[i].check_down_stock())
			now.keyboard.key[i].set_down(0, 0);
		if (next.keyboard.key[i].check_up_stock())
			now.keyboard.key[i].set_up(0, 0);
	}

	// �l�N�X�g��S�ď���
	next.use();
}

// ****************************************************************
// �W�����́F�t���[������
// ================================================================

#if 0
struct S_dil_to_vk_table
{
	int		vk;		// Virtual Key
	int		dik;	// Direct Input Key
};

S_dil_to_vk_table G_dik_to_vk_table[] = 
{
	// 000
	// 001
	// 002
	// 003
	// 004
	// 005
	// 006
	// 007
	{VK_BACK,		DIK_BACK},
	{VK_TAB,		DIK_TAB},
	// 010
	// 011
	// 012
	{VK_RETURN,		DIK_RETURN},
	// 014
	// 015
	{VK_SHIFT,		DIK_LSHIFT},	{VK_SHIFT,		DIK_RSHIFT},
	{VK_CONTROL,	DIK_LCONTROL},	{VK_CONTROL,	DIK_RCONTROL},
	{VK_MENU,		DIK_LMENU},		{VK_MENU,		DIK_RMENU},

	/*
	{DIK_ESCAPE,	VK_ESCAPE},
	{DIK_1,			'1'},
	{DIK_2,			'2'},
	{DIK_3,			'3'},
	{DIK_4,			'4'},
	{DIK_5,			'5'},
	{DIK_6,			'6'},
	{DIK_7,			'7'},
	{DIK_8,			'8'},
	{DIK_9,			'9'},
	{DIK_0,			'0'},
	{DIK_MINUS,		VK_MINUS},		// -
	{DIK_EQUALS,	VK_EQUALS},
	{DIK_BACK,		VK_BACK},
	{DIK_TAB,		VK_TAB},		// [tab]
	{DIK_Q,			'Q'},
	{DIK_W,			'W'},
	{DIK_E,			'E'},
	{DIK_R,			'R'},
	{DIK_T,			'T'},
	{DIK_Y,			'Y'},
	{DIK_U,			'U'},
	{DIK_I,			'I'},
	{DIK_O,			'O'},
	{DIK_P,			'P'},
	{DIK_LBRACKET,	'['},
	{DIK_RBRACKET,	']'},
	{DIK_RETURN,	VK_RETURN},
	{DIK_LCONTROL,	VK_LCONTROL},
	*/
};
#endif
#if 0
#define DIK_MINUS           0x0C    /* - on main keyboard */
#define DIK_EQUALS          0x0D
#define DIK_BACK            0x0E    /* backspace */
#define DIK_TAB             0x0F
#define DIK_Q               0x10
#define DIK_W               0x11
#define DIK_E               0x12
#define DIK_R               0x13
#define DIK_T               0x14
#define DIK_Y               0x15
#define DIK_U               0x16
#define DIK_I               0x17
#define DIK_O               0x18
#define DIK_P               0x19
#define DIK_LBRACKET        0x1A
#define DIK_RBRACKET        0x1B
#define DIK_RETURN          0x1C    /* Enter on main keyboard */
#define DIK_LCONTROL        0x1D
#define DIK_A               0x1E
#define DIK_S               0x1F
#define DIK_D               0x20
#define DIK_F               0x21
#define DIK_G               0x22
#define DIK_H               0x23
#define DIK_J               0x24
#define DIK_K               0x25
#define DIK_L               0x26
#define DIK_SEMICOLON       0x27
#define DIK_APOSTROPHE      0x28
#define DIK_GRAVE           0x29    /* accent grave */
#define DIK_LSHIFT          0x2A
#define DIK_BACKSLASH       0x2B
#define DIK_Z               0x2C
#define DIK_X               0x2D
#define DIK_C               0x2E
#define DIK_V               0x2F
#define DIK_B               0x30
#define DIK_N               0x31
#define DIK_M               0x32
#define DIK_COMMA           0x33
#define DIK_PERIOD          0x34    /* . on main keyboard */
#define DIK_SLASH           0x35    /* / on main keyboard */
#define DIK_RSHIFT          0x36
#define DIK_MULTIPLY        0x37    /* * on numeric keypad */
#define DIK_LMENU           0x38    /* left Alt */
#define DIK_SPACE           0x39
#define DIK_CAPITAL         0x3A
#define DIK_F1              0x3B
#define DIK_F2              0x3C
#define DIK_F3              0x3D
#define DIK_F4              0x3E
#define DIK_F5              0x3F
#define DIK_F6              0x40
#define DIK_F7              0x41
#define DIK_F8              0x42
#define DIK_F9              0x43
#define DIK_F10             0x44
#define DIK_NUMLOCK         0x45
#define DIK_SCROLL          0x46    /* Scroll Lock */
#define DIK_NUMPAD7         0x47
#define DIK_NUMPAD8         0x48
#define DIK_NUMPAD9         0x49
#define DIK_SUBTRACT        0x4A    /* - on numeric keypad */
#define DIK_NUMPAD4         0x4B
#define DIK_NUMPAD5         0x4C
#define DIK_NUMPAD6         0x4D
#define DIK_ADD             0x4E    /* + on numeric keypad */
#define DIK_NUMPAD1         0x4F
#define DIK_NUMPAD2         0x50
#define DIK_NUMPAD3         0x51
#define DIK_NUMPAD0         0x52
#define DIK_DECIMAL         0x53    /* . on numeric keypad */
#define DIK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define DIK_F11             0x57
#define DIK_F12             0x58
#define DIK_F13             0x64    /*                     (NEC PC98) */
#define DIK_F14             0x65    /*                     (NEC PC98) */
#define DIK_F15             0x66    /*                     (NEC PC98) */
#define DIK_KANA            0x70    /* (Japanese keyboard)            */
#define DIK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define DIK_CONVERT         0x79    /* (Japanese keyboard)            */
#define DIK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define DIK_YEN             0x7D    /* (Japanese keyboard)            */
#define DIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define DIK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define DIK_PREVTRACK       0x90    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
#define DIK_AT              0x91    /*                     (NEC PC98) */
#define DIK_COLON           0x92    /*                     (NEC PC98) */
#define DIK_UNDERLINE       0x93    /*                     (NEC PC98) */
#define DIK_KANJI           0x94    /* (Japanese keyboard)            */
#define DIK_STOP            0x95    /*                     (NEC PC98) */
#define DIK_AX              0x96    /*                     (Japan AX) */
#define DIK_UNLABELED       0x97    /*                        (J3100) */
#define DIK_NEXTTRACK       0x99    /* Next Track */
#define DIK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define DIK_RCONTROL        0x9D
#define DIK_MUTE            0xA0    /* Mute */
#define DIK_CALCULATOR      0xA1    /* Calculator */
#define DIK_PLAYPAUSE       0xA2    /* Play / Pause */
#define DIK_MEDIASTOP       0xA4    /* Media Stop */
#define DIK_VOLUMEDOWN      0xAE    /* Volume - */
#define DIK_VOLUMEUP        0xB0    /* Volume + */
#define DIK_WEBHOME         0xB2    /* Web home */
#define DIK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define DIK_DIVIDE          0xB5    /* / on numeric keypad */
#define DIK_SYSRQ           0xB7
#define DIK_RMENU           0xB8    /* right Alt */
#define DIK_PAUSE           0xC5    /* Pause */
#define DIK_HOME            0xC7    /* Home on arrow keypad */
#define DIK_UP              0xC8    /* UpArrow on arrow keypad */
#define DIK_PRIOR           0xC9    /* PgUp on arrow keypad */
#define DIK_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define DIK_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define DIK_END             0xCF    /* End on arrow keypad */
#define DIK_DOWN            0xD0    /* DownArrow on arrow keypad */
#define DIK_NEXT            0xD1    /* PgDn on arrow keypad */
#define DIK_INSERT          0xD2    /* Insert on arrow keypad */
#define DIK_DELETE          0xD3    /* Delete on arrow keypad */
#define DIK_LWIN            0xDB    /* Left Windows key */
#define DIK_RWIN            0xDC    /* Right Windows key */
#define DIK_APPS            0xDD    /* AppMenu key */
#define DIK_POWER           0xDE    /* System Power */
#define DIK_SLEEP           0xDF    /* System Sleep */
#define DIK_WAKE            0xE3    /* System Wake */
#define DIK_WEBSEARCH       0xE5    /* Web Search */
#define DIK_WEBFAVORITES    0xE6    /* Web Favorites */
#define DIK_WEBREFRESH      0xE7    /* Web Refresh */
#define DIK_WEBSTOP         0xE8    /* Web Stop */
#define DIK_WEBFORWARD      0xE9    /* Web Forward */
#define DIK_WEBBACK         0xEA    /* Web Back */
#define DIK_MYCOMPUTER      0xEB    /* My Computer */
#define DIK_MAIL            0xEC    /* Mail */
#define DIK_MEDIASELECT     0xED    /* Media Select */
#endif

void C_std_input::frame()
{
	C_point last_pos = next.mouse.pos;

	// �}�E�X�̈ʒu�̐ݒ�
	next.mouse.pos = get_mouse_pos();

	// �L�[�̏����擾���Ă���
	BYTE key[T3_KEY_CNT] = {0};
	// �A�N�e�B�u�E�B���h�E���Ȃ���΃L�[��������Ă��Ȃ��Ɣ��f����
	if (::GetActiveWindow())	{

		// Windows ���z�L�[���󂯎��
		for (int i = 0; i < T3_WINKEY_CNT; i++)	{
			key[i] = (::GetKeyState(i) & 0x8000) ? 0x80 : 0;
		}
		//::GetKeyboardState(key);	// �����b�Z�[�W�������Ƃ������X�V����Ȃ��B���߁B

		// DirectInput ������͂��󂯎��
		//G_dinput.get_input_from_keyboard(T3_DIKEY_CNT, key + T3_DIKEY_TOP);
	}

	// ================================================================
	// state ����
	// ----------------------------------------------------------------

	// ��������
	next.mouse.left.set_move(next.mouse.pos.x - last_pos.x, next.mouse.pos.y - last_pos.y);
	next.mouse.right.set_move(next.mouse.pos.x - last_pos.x, next.mouse.pos.y - last_pos.y);

	// Windows ���z�L�[�Ɋւ��ẮA���b�Z�[�W��������Ă�̂ŁA���ꏈ���̂ݍs��

	// �E�B���h�E�O�Ń{�^���������ꂽ�Ƃ��̏���
	if (next.mouse.left.is_down() && !(key[VK_LBUTTON] & 0x80))
		next.mouse.left.set_up(next.mouse.pos.x, next.mouse.pos.y);
	if (next.mouse.right.is_down() && !(key[VK_RBUTTON] & 0x80))
		next.mouse.right.set_up(next.mouse.pos.x, next.mouse.pos.y);
	if (next.mouse.middle.is_down() && !(key[VK_MBUTTON] & 0x80))
		next.mouse.middle.set_up(next.mouse.pos.x, next.mouse.pos.y);

	// �E�B���h�E�O�ŃL�[�������ꂽ�Ƃ��̏���
	for (int i = 0; i < T3_KEY_CNT; i++)	{
		if (next.keyboard.key[i].is_down() && !(key[i] & 0x80))
			next.keyboard.key[i].set_up(0, 0);	// �L�[�������
	}

	// DirectInput �Ɋւ��Ă͂����őS�Ă̏������s��


	// Tona3 �g���L�[�̏���

	// ================================================================
	// �t���[�������inext �� now�j
	// ----------------------------------------------------------------
	C_input_base::frame();

	// ================================================================
	// real_state �̏���
	// ----------------------------------------------------------------

	// Windows ���z�L�[
	now.mouse.left.set_real_state((key[VK_LBUTTON] & 0x80) > 0 ? 1 : 0);
	now.mouse.right.set_real_state((key[VK_RBUTTON] & 0x80) > 0 ? 1 : 0);
	now.mouse.middle.set_real_state((key[VK_MBUTTON] & 0x80) > 0 ? 1 : 0);
	for (int i = 0; i < T3_WINKEY_CNT; i++)	{
		now.keyboard.key[i].set_real_state((key[i] & 0x80) > 0 ? 1 : 0);
	}

	// DirectInput �L�[
	//for (int i = 0; i < T3_DIKEY_CNT; i++)	{
	//	now.keyboard.key[T3_DIKEY_TOP + i].set_real_state((key[T3_DIKEY_TOP + i] & 0x80) > 0 ? 1 : 0);
	//}
}

// ****************************************************************
// �W�����́FWM �������
// ================================================================
void C_std_input::set_state_by_wm(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_LBUTTONDOWN:	next.mouse.left.set_down(LOWORD(lp), HIWORD(lp));		break;
		case WM_LBUTTONUP:		next.mouse.left.set_up(LOWORD(lp), HIWORD(lp));			break;
		case WM_RBUTTONDOWN:	next.mouse.right.set_down(LOWORD(lp), HIWORD(lp));		break;
		case WM_RBUTTONUP:		next.mouse.right.set_up(LOWORD(lp), HIWORD(lp));		break;
		case WM_MBUTTONDOWN:	next.mouse.middle.set_down(LOWORD(lp), HIWORD(lp));		break;
		case WM_MBUTTONUP:		next.mouse.middle.set_up(LOWORD(lp), HIWORD(lp));		break;
		case WM_MOUSEWHEEL:		next.mouse.wheel.set_wheel_by_wm(wp, lp);	break;
		case WM_KEYDOWN:		next.keyboard.set_down_by_wm(wp, lp);		break;
		case WM_KEYUP:			next.keyboard.set_up_by_wm(wp, lp);			break;
		case WM_SYSKEYDOWN:		next.keyboard.set_down_by_wm(wp, lp);		break;
		case WM_SYSKEYUP:		next.keyboard.set_up_by_wm(wp, lp);			break;
	}
}

}
