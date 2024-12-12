#pragma		once

#include	"tona3_util.h"

namespace NT3
{

// ****************************************************************
// 入力を簡易に取得
// ================================================================
C_point	get_mouse_pos();				// マウスの位置を取得（スクリーン空間）
bool	is_key_down(int winkey_code);	// Windows 仮想キーが押されているかどうか判定

// ****************************************************************
// Tona3 キーコード
// ================================================================
const int	T3_KEY_CNT = 256;

// Windows 仮想キー
const int	T3_WINKEY_TOP = 0;
const int	T3_WINKEY_CNT = 256;

// DirectInput キー
//const int	T3_DIKEY_TOP = 256;
//const int	T3_DIKEY_CNT = 256;

// フレーム履歴
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
	LPDIRECTINPUTDEVICE8 m_p_dev_keyboard;		// DirectInput キーボードデバイス
	LPDIRECTINPUTDEVICE8 m_p_dev_joypad;		// DirectInput パッドデバイスの配列
	DIDEVCAPS m_dev_caps;

	static BOOL CALLBACK enum_joypad_callback(const DIDEVICEINSTANCE* p_dev_instance, void* p_context);
	static BOOL CALLBACK enum_axes_callback(LPCDIDEVICEOBJECTINSTANCE p_dev_object_instance, LPVOID pv_ref);
};

extern	C_direct_input G_dinput;
*/

// ****************************************************************
// 入力状態
// ================================================================
struct C_input_state
{
	// --- ボタン ---
	struct BUTTON
	{
		BUTTON() : real_state(0), down_stock(0), up_stock(0), down_up_stock(0), move_x(0), move_y(0)	{}

		void	clear();					// クリア
		void	use();						// 消費
		void	frame();					// フレーム
		void	set_move(int x, int y);		// 「動かした」を設定
		void	set_down(int x, int y);		// 「押した」を設定
		void	set_up(int x, int y);		// 「放した」を設定
		void	unset_up();					// 「放した」を削除
		void	set_real_state(int state);

		bool	is_down()			{	return real_state == 1;		}	// 現在押している
		bool	is_up()				{	return real_state == 0;		}	// 現在放している
		bool	check_down_stock();			// 「押した」がストックされているかを判定
		bool	check_up_stock();			// 「放した」がストックされているかを判定
		bool	check_down_up_stock();		// 「押して放した」がストックされているかを判定
		bool	check_flick_stock();		// 「フリック」がストックされているかを判定
		bool	use_down_stock();			// 「押した」のストックを消費
		bool	use_up_stock();				// 「放した」のストックを消費
		bool	use_down_up_stock();		// 「押して放した」のストックを消費
		bool	use_flick_stock();			// 「フリック」のストックを消費

		int		get_real_state()		{	return real_state;			}	// 現在の状態を取得
		int		get_down_stock()		{	return down_stock;			}	// 現在の「押した」ストックを取得
		int		get_up_stock()			{	return up_stock;			}	// 現在の「離した」ストックを取得
		int		get_down_up_stock()		{	return down_up_stock;		}	// 現在の「押して離した」ストックを取得

		double	get_flick_angle() { return flick_angle; }
		double	get_flick_pixel() { return flick_pixel; }
		double	get_flick_mm() { return flick_mm; }

	private:

		int		real_state;				// 現在の状態（0:放している 1:押している）
		int		down_stock;				// 「押した」のストック（0:押してない 1:押した）
		int		up_stock;				// 「放した」のストック（0:放してない 1:放した）
		int		down_up_stock;			// 「押して放した」のストック（0:押してない 1:押した 2:押して放した）
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

	// --- ホイール ---
	struct WHEEL
	{
		WHEEL() : up_stock(0), down_stock(0)	{}

		void	clear();		// クリア
		void	use();			// 消費
		void	frame();		// フレーム

		void	set_up(int up_cnt);							// 上回転
		void	set_down(int down_cnt);						// 下回転
		void	set_wheel_by_wm(WPARAM wp, LPARAM lp);		// WM_MOUSEWHEEL
		bool	use_up_stock();								// 上回転を消費
		bool	use_down_stock();							// 下回転を消費
		int		get_up_cnt()	{	return up_stock;	}	// 上回転数を取得
		int		get_down_cnt()	{	return down_stock;	}	// 下回転数を取得

		int		up_stock;		// 上回転の回数
		int		down_stock;		// 下回転の回数
	};

	// --- マウス ---
	struct MOUSE
	{
		void	clear();	// クリア
		void	use();		// 消費
		void	frame();	// フレーム

		C_point	pos;
		BUTTON	left;
		BUTTON	right;
		BUTTON	middle;
		WHEEL	wheel;
	};

	// --- キーボード ---
	struct KEYBOARD
	{
		void	clear();	// クリア
		void	use();		// 消費
		void	frame();	// フレーム

		void	set_down_by_wm(WPARAM wp, LPARAM lp);
		void	set_up_by_wm(WPARAM wp, LPARAM lp);
		int		use_down_stock();
		int		use_up_stock();

		BUTTON	key[T3_KEY_CNT];
	};

	// --- ステート ---
	MOUSE		mouse;
	KEYBOARD	keyboard;

	// --- 処理 ---
	void	clear();	// クリア
	void	use();		// 消費
	void	frame();	// フレーム
};

// ****************************************************************
// 入力
// ================================================================
//
//		Cinput クラスのインスタンスは、アプリケーションごとに作ってください。
//		next に入力を与えます。通常はウィンドウメッセージで与えますが、その場合は Cstd_input を使うと簡単です。
//		frame を呼び出し、now で取り出します。
//
//		フレーム処理の先頭で frame() を呼び出します。
//
//		input.frame();
//		
//		取り出しは now を使います。
//
//		if (input.now.mouse.left.use_down())	{ ... }		// マウス左を押したときの処理
//			
// ================================================================
class C_input_base
{
public:
	C_input_base();

	// クリア
	void	clear();

	// フレーム処理
	void	frame();

	// ステート
	C_input_state	now;
	C_input_state	next;
};

// ****************************************************************
// 標準入力
// ================================================================
//
//		キーボードやマウスに対して標準的な処理を行います。
//		入力を独自に与えたりエミュレートする場合は input_base を使って下さい。
//
// ================================================================
class C_std_input : public C_input_base
{
public:

	// フレーム処理
	void	frame();

	// 標準的な入力メッセージを処理（WM_LBUTTONDOWN、WM_LBUTTONUP、WM_RBUTTONDOWN、WM_RBUTTONUP、WM_MOUSEWHEEL、WM_KEYDOWN、WM_KEYUP）
	void	set_state_by_wm(UINT msg, WPARAM wp, LPARAM lp);
};


}
