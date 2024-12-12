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
// 初期化
// ================================================================
bool CDirectInput::Init(HWND hWnd)
{
	HRESULT hr;

	hr = DirectInput8Create(Gapp.hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&pDI, NULL);
	if (FAILED(hr))
		return false;

	// マウスデバイスを作成
	hr = pDI->CreateDevice(GUID_SysMouse, &pDevice, NULL);
	if (FAILED(hr))
		return false;

	// データフォーマットの設定
	hr = pDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return false;

	// 協調モードの設定
	hr = pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return false;

	/*
	// バッファサイズの設定
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
// 開放
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
// 処理開始
// ================================================================
bool CDirectInput::Aquire()
{
	if (!pDevice)
		return false;

	pDevice->Acquire();

	return true;
}

// ****************************************************************
// 処理終了
// ================================================================
bool CDirectInput::Unaquire()
{
	if (!pDevice)
		return false;

	pDevice->Unacquire();

	return true;
}

// ****************************************************************
// フレーム処理
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

	// デバイスの状態の取得
	hr = pDevice->GetDeviceState(sizeof(dims), &dims);
	if (FAILED(hr))	{
		// デバイスの復元
		if (hr == DIERR_INPUTLOST)
			pDevice->Acquire();

		return false;
	}

	// マウスの状態の更新
//	mouse.x = dims.lX;		// Ｘ座標の取得
//	mouse.y = dims.lY;		// Ｙ座標の取得
	mouse.left.now = (dims.rgbButtons[0] & 0x80) ? true : false;		// 現在の左状態を取得
	mouse.right.now = (dims.rgbButtons[1] & 0x80) ? true : false;		// 現在の右状態を取得
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
