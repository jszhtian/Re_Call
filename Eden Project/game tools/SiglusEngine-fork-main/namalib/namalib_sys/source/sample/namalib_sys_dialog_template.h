#pragma		once

#define		NAMALIB_DLGTPLT_BASE_LEFT		3
#define		NAMALIB_DLGTPLT_BASE_RIGHT		4
#define		NAMALIB_DLGTPLT_BASE_TOP		4
#define		NAMALIB_DLGTPLT_BASE_BOTTOM		3
#define		NAMALIB_DLGTPLT_HEIGHT			14
#define		NAMALIB_DLGTPLT_SPACE_X			4
#define		NAMALIB_DLGTPLT_SPACE_Y			4

// ****************************************************************
// ダイアログテンプレート
// ================================================================
class Cnamalib_dialog_template : public Cdialog_template
{

public:

	// ダイアログ準備
	void init_standard_tool_window(CTSTR& title, bool size_change_flag);

	// ボタン追加
	void add_btn(int x, int y, int w, int h, WORD control_id, CTSTR& text, bool group_flag);
	// テキスト追加
	void add_txt(int x, int y, int w, int h, WORD control_id, CTSTR& text, bool right_flag);
	// エディットボックス追加
	void add_edt(int x, int y, int w, int h, WORD control_id, bool dropdown_flag);
	// リストビュー追加（レポート型）
	void add_lstv(int x, int y, int w, int h, WORD control_id);

	// 配置パラメータ
	struct ARRANGEMENT_PARAM{
		int x, y;				// 現在座標
		int undo_x, undo_y;		// 前の座標
		int push_x, push_y;		// ＰＵＳＨ座標
		int max_x, max_y;		// 最大座標
	} ap;

	void init_ap(void);							// 配置パラメータ初期化
	void next_ap(int ap_mod, int w, int h);		// 配置パラメータ次の座標へ
	void set_size_from_ap(void);				// 配置パラメータからダイアログサイズを設定
	int  get_ap_max_width(void);				// 配置パラメータからアイテムの最大幅を設定
	int  get_ap_max_height(void);				// 配置パラメータからアイテムの最大高を設定
	void undo_ap_x(void);						// 配置パラメータＸ戻す
	void undo_ap_y(void);						// 配置パラメータＹ戻す
	void set_ap_x(int x);						// 配置パラメータＸ設定
	void set_ap_y(int y);						// 配置パラメータＹ設定
	void add_ap_x(int x);						// 配置パラメータＸ加算
	void add_ap_y(int y);						// 配置パラメータＹ加算
	void set_ap_x_right_arrangement(int cnt, ...);	// 配置パラメータＸ設定（右位置した場合のＸ座標）
	void push_ap_x(void);						// 配置パラメータＸＰＵＳＨ
	void push_ap_y(void);						// 配置パラメータＹＰＵＳＨ
	void pop_ap_x(void);						// 配置パラメータＸＰＯＰ
	void pop_ap_y(void);						// 配置パラメータＹＰＯＰ

	// 配置パラメータ版：ボタン追加
	void add_ap_btn(int ap_mod, int w, int h, WORD control_id, CTSTR& text, bool group_flag);
	// 配置パラメータ版：テキストとエディットボックス追加
	void add_ap_tedt(int ap_mod, int txt_w, WORD txt_control_id, CTSTR& text, int edt_w, WORD edt_control_id, bool dropdown_flag);
	// 配置パラメータ版：リストビュー追加（レポート型）
	void add_ap_lstv(int ap_mod, int w, int h, WORD control_id);
};


