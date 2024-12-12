#pragma		once

#include	"tonagraphic3_pct.h"
#include	"tonagraphic3_album.h"

namespace NT3
{

// ****************************************************************
// スプライトタイプ
// ================================================================
enum SPRITETYPE
{
	SPRITETYPE_NONE,		// なし
	SPRITETYPE_PLANE,		// プレーン
	SPRITETYPE_DIB,			// ディブ
	SPRITETYPE_PCT,			// ピクチャ
	SPRITETYPE_ALBUM,		// アルバム
};

const TSTR StrSPRITETYPE[] = {
	_T("NONE"), 
	_T("PLANE"), 
	_T("DIB"), 
	_T("PCT"), 
	_T("ALBUM"), 
};

// ****************************************************************
// スプライトパラメータ
// ================================================================
struct S_sprite_param : public S_disp_param
{
	S_sprite_param();
	void	init();

	SPRITETYPE	type;		// スプライトタイプ
	int			change;		// 変更フラグ
	bool		disp;		// 描画フラグ
	int			layer;		// レイヤー
	int			order;		// オーダー
	int			depth;		// 深度
	int			pct_no;		// ピクチャ番号

	bool operator == (const S_sprite_param& rhs)	{ return pod_equal(this, &rhs); }
	bool operator != (const S_sprite_param& rhs)	{ return !pod_equal(this, &rhs); }
};

// ****************************************************************
// スプライト
// ================================================================
class C_sprite
{
public:
	C_sprite();
	~C_sprite()	{}

	void		init();									// 初期化
	bool		create_plane();							// プレーンの作成
	bool		create_dib(BSP<C_dib> dib);				// ディブの作成
	bool		create_pct(BSP<C_pct> pct);				// ピクチャの作成
	bool		create_album(BSP<C_album> album);		// アルバムの作成

	bool		is_ready();								// データが準備されているかを判定（ディブ、ピクチャ、アルバム）
	C_rect		get_rect(int pct_no = -1);				// 全体矩形の取得（当たり判定などに使用する）（pct_no = -1: 現在のピクチャ（アルバム専用））
	void		get_sp(S_sprite_param* sp);				// 表示パラメータの取得（ビュー内部で使用）
	void		check_disp_rect(C_dib* dib, ARRAY<C_rect>& ret_draw_rect_list);	// 描画矩形の取得（ビュー内部で使用）

	void		draw_to_dib(C_dib* dib, C_rect* clip = NULL);	// dib に描画

	// 共通パラメータ
	S_sprite_param	sp;		// スプライトパラメータ

	// 「ディブ」パラメータ
	BSP<C_dib>		dib;		// ディブ
	// 「ピクチャ」パラメータ
	BSP<C_pct>		pct;		// ピクチャ
	// 「アルバム」パラメータ
	BSP<C_album>	album;		// アルバム

	// 並び替え用の比較演算子
	bool		operator < (C_sprite& rhs);

private:
	void		reset_type();
};



};
