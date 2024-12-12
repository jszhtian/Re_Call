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

	// �}�E�X
	struct MOUSE
	{
		int		x;
		int		y;
		struct BUTTON
		{
			bool	now;	// ���݂̏��
			bool	last;	// �O��̏��
			bool	up;		// ������
			bool	down;	// ������

			bool	UseUp();
			bool	UseDown();
		} left, right;
	} mouse;

private:
	CDirectInput();
	~CDirectInput();

	LPDIRECTINPUT8			pDI;		// �_�C���N�g�C���v�b�g
	LPDIRECTINPUTDEVICE8	pDevice;	// �f�o�C�X
};

#define		gDInput			CDirectInput::GetInstance()
