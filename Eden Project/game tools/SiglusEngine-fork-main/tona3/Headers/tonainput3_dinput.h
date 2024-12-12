#pragma		once

// ****************************************************************
// DirectInput
// ================================================================
class CDirectInput
{
public:

	static	CDirectInput&	GetInstance()
	{
		static CDirectInput instance;
		return instance;
	}

	bool	Init(HWND hWnd);
	void	Release();
	bool	Aquire();
	bool	Unaquire();
	bool	Frame();

	LPDIRECTINPUT8	operator ->()
	{
		return pDI;
	}

	// マウス
	struct MOUSE
	{
		int		x;
		int		y;
		struct BUTTON
		{
			bool	now;	// 現在の状態
			bool	last;	// 前回の状態
			bool	up;		// 押した
			bool	down;	// 離した

			bool	UseUp();
			bool	UseDown();
		} left, right;
	} mouse;

private:
	CDirectInput();
	~CDirectInput();

	LPDIRECTINPUT8			pDI;		// ダイレクトインプット
	LPDIRECTINPUTDEVICE8	pDevice;	// デバイス
};

#define		gDInput			CDirectInput::GetInstance()
