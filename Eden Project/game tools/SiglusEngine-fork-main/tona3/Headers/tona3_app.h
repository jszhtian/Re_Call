#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"

namespace NT3
{

// ****************************************************************
// アプリケーション
// ================================================================
class C_app
{
public:
	C_app();
	~C_app();

	HINSTANCE		h_inst;				// アプリケーションのハンドル
	HWND			h_wnd;				// メインウィンドウのハンドル
	TSTR			command_line;		// コマンドライン
	TSTR			module_dir;			// モジュールのディレクトリ
	TSTR			module_name;		// モジュールのファイル名
	TSTR			app_name;			// アプリケーション名
	TSTR			app_dir;			// アプリケーションディレクトリ
	ARRAY<HWND>		window_list;		// ウィンドウリスト
	bool			quit_flag;			// WM_QUIT で抜けたフラグ


	bool	default_message_loop();				// メッセージループ
	bool	default_message_proc(MSG* msg);		// メッセージ処理
};

extern	C_app	G_app;



}
