#include	"tonainput3_pch.h"
#include	"tonainput3_dinput.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// ****************************************************************
// DirectInput
// ================================================================
CDirectInput::CDirectInput()
{
	pDI = NULL;
}

CDirectInput::~CDirectInput()
{
	Release();
}

// ****************************************************************
// ������
// ================================================================
bool CDirectInput::Init(HWND hWnd)
{
	HRESULT hr;

	hr = DirectInput8Create(Gapp.hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&pDI, NULL);
	if (FAILED(hr))
		return false;

	// �}�E�X�f�o�C�X���쐬
	hr = pDI->CreateDevice(GUID_SysMouse, &pDevice, NULL);
	if (FAILED(hr))
		return false;

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	hr = pDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return false;

	// �������[�h�̐ݒ�
	hr = pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return false;

	/*
	// �o�b�t�@�T�C�Y�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = 1000;
	hr = pDevice->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
	if (FAILED(hr))
		return false;
		*/

	return true;
}

// ****************************************************************
// �J��
// ================================================================
void CDirectInput::Release()
{
	Unaquire();

	if (pDI)	{
		pDI->Release();
		pDI = NULL;
	}
}

// ****************************************************************
// �����J�n
// ================================================================
bool CDirectInput::Aquire()
{
	if (!pDevice)
		return false;

	pDevice->Acquire();

	return true;
}

// ****************************************************************
// �����I��
// ================================================================
bool CDirectInput::Unaquire()
{
	if (!pDevice)
		return false;

	pDevice->Unacquire();

	return true;
}

// ****************************************************************
// �t���[������
// ================================================================
bool CDirectInput::Frame()
{
//	HRESULT hr;

	/*
	DIDEVICEOBJECTDATA od;
	DWORD items = 1;

	hr = pDevice->GetDeviceData(sizeof(od), &od, &items, 0);
	if (hr == DIERR_INPUTLOST)	{
		pDevice->Acquire();
		return false;
	}
	else if (FAILED(hr) || items == 0)
		return false;

	switch (od.dwOfs)	{
		case DIMOFD_BUTTON0:
			od.
			break;
	}
	*/

	/*
	DIMOUSESTATE dims;

	// �f�o�C�X�̏�Ԃ̎擾
	hr = pDevice->GetDeviceState(sizeof(dims), &dims);
	if (FAILED(hr))	{
		// �f�o�C�X�̕���
		if (hr == DIERR_INPUTLOST)
			pDevice->Acquire();

		return false;
	}

	// �}�E�X�̏�Ԃ̍X�V
//	mouse.x = dims.lX;		// �w���W�̎擾
//	mouse.y = dims.lY;		// �x���W�̎擾
	mouse.left.now = (dims.rgbButtons[0] & 0x80) ? true : false;		// ���݂̍���Ԃ��擾
	mouse.right.now = (dims.rgbButtons[1] & 0x80) ? true : false;		// ���݂̉E��Ԃ��擾
	mouse.left.up = false;		if (mouse.left.last && !mouse.left.now)		mouse.left.up = true;
	mouse.left.down = false;	if (!mouse.left.last && mouse.left.now)		mouse.left.down = true;
	mouse.right.up = false;		if (mouse.right.last && !mouse.right.now)	mouse.right.up = true;
	mouse.right.down = false;	if (!mouse.right.last && mouse.right.now)	mouse.right.down = true;
	mouse.left.last = mouse.left.now;
	mouse.right.last = mouse.right.now;
	*/

	return true;
}


bool CDirectInput::MOUSE::BUTTON::UseUp()
{
	if (up)	{
		up = false;
		return true;
	}

	return false;
}

bool CDirectInput::MOUSE::BUTTON::UseDown()
{
	if (down)	{
		down = false;
		return true;
	}

	return false;
}
