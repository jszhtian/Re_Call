#pragma		once

#include	<Wbemidl.h>

#include	"tona3_string.h"

// ****************************************************************
// ■とな３：エラー
// ================================================================
// エラー情報の受け取り方：
//		・GetLastError
//		・errno
//		・自分で定義したエラー
//		・など
// ================================================================

namespace NT3
{

// ****************************************************************
// CRT（C RunTime）エラー
// ================================================================

// 最後のエラーから CRT エラーのエラーメッセージを取得する
TSTR get_crt_last_error_msg();

// ****************************************************************
// Windows エラー
// ================================================================

// Windows のエラーメッセージを取得する
TSTR get_win_error_msg(DWORD error_code);

// ****************************************************************
// HRESULT エラー
// ================================================================

// HRESULT のエラーメッセージを取得する
TSTR get_hresult_error_msg(HRESULT hr);

// ****************************************************************
// WBEM エラー
// ================================================================

// WBEM のエラーメッセージを取得する
TSTR get_wbem_error_msg(HRESULT hr);

// ****************************************************************
// とな３：エラー
// ================================================================

// とな３エラータイプ
enum E_ERROR_TYPE
{
	ERROR_TYPE_NONE,	// エラーなし
	ERROR_TYPE_CRT,		// CRT エラー
	ERROR_TYPE_WIN,		// Windows API エラー
	ERROR_TYPE_HRESULT,	// HRESULT エラー
	ERROR_TYPE_WBEM,	// WBEM エラー
	ERROR_TYPE_OTHERS,	// その他のエラー
};

// とな３エラー構造体
struct S_ERROR
{
	E_ERROR_TYPE	error_type;		// エラータイプ
	int				error_no;		// エラー番号
	TSTR			error_str;		// エラーメッセージ
	TSTR			error_func;		// エラーが発生した関数名
};

// 最後のエラーを設定
void set_last_error(E_ERROR_TYPE error_type, int error_no, const TSTR& error_func);
void set_last_error(const TSTR& error_str, const TSTR& error_func);

// 最後のエラーを追加
void add_last_error(const TSTR& error_str, const TSTR& error_func);

// 最後のエラーを CRT エラーに設定
void set_last_error_crt(const TSTR& error_func);

// 最後のエラーを Windows エラーに設定
void set_last_error_win(const TSTR& error_func);
void set_last_error_win(int error_no, const TSTR& error_func);

// 最後のエラーを HRESULT エラーに設定
void set_last_error_hr(HRESULT hr, const TSTR& error_func);

// 最後のエラーを WBEM エラーに設定
void set_last_error_wbem(HRESULT hr, const TSTR& error_func);

// 最後のエラーを「無効なパラメータ」に設定
void set_last_error_invalid(const TSTR& error_func);

// 最後のエラーのエラーメッセージを取得
TSTR get_last_error_msg();

// 最後のエラーをクリア
void clear_last_error();

// 最後のエラーが存在するかを判定
bool is_error();

}

