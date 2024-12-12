#include	"tonawindow3_pch.h"
#include	"tonawindow3_window.h"

namespace NT3
{

// ****************************************************************
// ウィンドウ作成
//		name : ウィンドウの名前（ini 保存時に使用）
//		class_name : ウィンドウクラス名
//		caption : ウィンドウタイトル
//		x : ウィンドウの表示位置Ｘ（CW_USEDEFAULT : デフォルト、-1 : 中央）
//		y : ウィンドウの表示位置Ｙ（CW_USEDEFAULT : デフォルト、-1 : 中央）
//		client_w : ウィンドウのクライアント領域の幅（CW_USEDEFAULT : デフォルト）
//		client_h : ウィンドウのクライアント領域の高さ（CW_USEDEFAULT : デフォルト）
//		window_style : ウィンドウスタイル
//			WS_CAPTION : キャプション
//			WS_SYSMENU : システムメニュー
//			WS_MINIMIZEBOX : 最小化ボタン
//		window_style_ex : 拡張ウィンドウスタイル
//		icon_id : アイコンのリソースＩＤ（0 で無効）
//		menu_id : メニューのリソースＩＤ（0 で無効）
//		brush_type : 背景を塗りつぶすブラシ（-1 で無効）
//		is_limit_min : ウィンドウの最小矩形を制限
//		is_attach : 他のウィンドウにアタッチ
//		h_parent_wnd : 親ウィンドウハンドル
//		adjust_way : 親にくっつく方向（ﾃﾝｷｰ の方向で指定）
// ================================================================
bool C_window::create(CTSTR& name, CTSTR& class_name, CTSTR& caption, int x, int y, int client_w, int client_h, int icon_id, int menu_id, int brush_type, DWORD style, DWORD ex_style, bool is_limit_min, bool is_attach, HWND h_parent_wnd, int adjust_way)
{
	// クラス名を保存する
	m_class_name = class_name;

	// ★ウィンドウクラスの抹消
	// 本来はありえない。クラスの登録とウィンドウの作成を一緒くたにしちゃったからできる芸当…。
	// WM_DESTROY の中での UnregisterClass が失敗してる件に対する仮処置。
	C_window_base::unregist_class(m_class_name);

	// ウィンドウクラスの登録
	if (!C_window_base::regist_class(m_class_name, icon_id, menu_id, brush_type))
		return false;

	// ウィンドウの作成
	if (!C_window_base::create(name, m_class_name, caption, x, y, client_w, client_h, menu_id > 0, style, ex_style, is_limit_min, is_attach, h_parent_wnd))
		return false;

	return true;
}

// ****************************************************************
// 継承されるウィンドウプロシージャ
// ================================================================
LRESULT C_window::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	// WM_DESTROY のみここで
	switch (msg){
		case WM_DESTROY:	if (!on_destroy())		return FALSE;	break;
	}

	// 基底クラスのウィンドウプロシージャを呼び出す
	return C_window_base::window_proc(msg, wp, lp);
}

// ****************************************************************
// ウィンドウが破壊された
// ================================================================
bool C_window::on_destroy()
{
	// ウィンドウクラスの抹消
	C_window_base::unregist_class(m_class_name);

	// ★↑これ失敗してんねんね…。全く気づかなかった。
	// WM_DESTROY の中ではまだ UnregisterClass できないみたい。
	// 困った。分離しないといけない。

	// パラメータの初期化
	m_class_name.clear();

	return true;
}

}
