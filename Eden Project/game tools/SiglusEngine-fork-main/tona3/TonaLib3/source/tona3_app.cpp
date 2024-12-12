#include	"tona3_pch.h"
#include	"tona3_app.h"

namespace NT3
{

// ****************************************************************
// アプリケーションのインスタンス
// ================================================================
C_app	G_app;

// ****************************************************************
// アプリケーション
// ================================================================
C_app::C_app()
{
	h_inst = NULL;
	h_wnd = NULL;
	quit_flag = false;
}

C_app::~C_app()
{
}

// ****************************************************************
// メッセージループ
// ================================================================
bool C_app::default_message_loop()
{
	MSG msg;

	// 終了フラグが立っていれば終了
	if (quit_flag)
		return false;

	// メッセージを取り出す
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))	{
		// WM_QUIT が来たら false
		if (!GetMessage(&msg, NULL, 0, 0))	{
			quit_flag = true;	// 終了フラグ
			return false;
		}
		// メッセージ処理
		if (!default_message_proc(&msg))
			return false;
	}

	// メッセージが来なかったら戻る
	return true;
}

// ****************************************************************
// メッセージ処理
// ================================================================
bool C_app::default_message_proc(MSG* msg)
{
	// ダイアログメッセージの処理
	for (ARRAY<HWND>::iterator itr = G_app.window_list.begin(); itr != G_app.window_list.end(); ++itr)	{
		if (GetWindowLong(*itr, DWL_DLGPROC))	{
			if (IsDialogMessage(*itr, msg) != 0)
				return true;
		}
	}

	// 通常のメッセージ処理
	TranslateMessage(msg);
	DispatchMessage(msg);

	return true;
}

}
