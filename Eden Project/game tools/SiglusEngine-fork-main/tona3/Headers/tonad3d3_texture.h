#pragma		once

#include	"tonad3d3_d3d.h"
#include	"tonad3d3_surface.h"
#include	"g00.h"

namespace NT3
{

// ****************************************************************
// D3D テクスチャ
// ================================================================
class C_d3d_texture
{
public:
	C_d3d_texture();
	~C_d3d_texture();

	void	init();																// テクスチャを初期化
	void	release();															// テクスチャを解放
	bool	create(int width, int height);										// テクスチャを構築
	bool	create(int width, int height, UINT mip_level, DWORD usage, D3DFORMAT format, D3DPOOL pool);		// テクスチャを構築（フルパラメータ）
	bool	clear_color_ex(C_argb color);																	// 指定色で塗りつぶし
	bool	clear_color(C_argb color);																		// 指定色で塗りつぶし
	bool	draw_rect(C_rect rect, C_argb moji_color);														// 矩形を描画
	bool	load_moji(TCHAR moji, int moji_size, C_argb moji_color);										// 文字をロード（簡易指定）
	bool	load_moji(TCHAR moji, LOGFONT* lf,   C_argb moji_color);										// 文字をロード（フォントを指定）
	bool	load_moji(TCHAR moji, int moji_size, C_argb moji_color, MOJI_FACE_TYPE face_type, bool tategaki, HDC h_dc);		// 文字をロード（デバイスコンテキストを指定）
	bool	load_file(CTSTR& file_path);																	// 画像ファイルをロード
	bool	load_g00(CTSTR& file_path);																		// g00 をロード
	bool	load_g00_cut(C_g00_cut* g00_cut, bool fuchidori);												// g00 のカットをロード
	bool	draw_g00_cut(C_g00_cut* g00_cut, int x, int y, int blend_type);									// g00 カットを書き込み
	bool	add_edge_for_filterring(D3DLOCKED_RECT* p_texture_bit);											// フィルタリング処理のためにエッジを加える（ロックしません）

	int		get_width()				{ return m_width; }
	int		get_height()			{ return m_height; }
	C_size	get_size()				{ return C_size(m_width, m_height); }
	int		get_width_ex()			{ return m_width_ex; }
	int		get_height_ex()			{ return m_height_ex; }
	C_size	get_size_ex()			{ return C_size(m_width_ex, m_height_ex); }
	int		get_original_width()	{ return m_org_width; }
	int		get_original_height()	{ return m_org_height; }
	C_size	get_original_size()		{ return C_size(m_org_width, m_org_height); }
	C_point	get_center()			{ return m_center; }
	C_rect	get_rect()				{ return C_rect(0 - m_center.x, 0 - m_center.y, m_width - m_center.x, m_height - m_center.y); }
	bool	get_blend()				{ return m_blend; }
	bool	is_mipmap_enable()		{ return m_mipmap_enable; }

	void	set_dirty_flag(bool flag)	{ m_dirty = flag; }
	bool	is_dirty()					{ return m_dirty; }

	C_argb	get_color(int x, int y);							// 色を取得
	void	set_center(C_point pos)	{	m_center = pos;	}		// 中心座標を設定

	bool	lock_rect(UINT level, D3DLOCKED_RECT* texture_bit, C_rect* rect, DWORD flags);	// ロック
	bool	unlock_rect(UINT level);														// アンロック
	bool	get_render_target_data(int level, IDirect3DSurface9* p_surface);				// レンダーターゲットのデータを取得する

	C_d3d_surface	get_surface(int level);		// サーフェスを取得する

	IDirect3DTexture9*	body()			{	return m_texture.get();	}
	IDirect3DTexture9*	operator ->()	{	return m_texture.get();	}

private:

	BIP<IDirect3DTexture9>	m_texture;

	int		m_width;			// テクスチャの幅
	int		m_height;			// テクスチャの高さ
	int		m_width_ex;			// 実際のテクスチャの幅（2^n に調整済み）
	int		m_height_ex;		// 実際のテクスチャの高さ（2^n に調整済み）
	int		m_org_width;		// 元画像の幅
	int		m_org_height;		// 元画像の高さ
	C_point	m_center;			// 中心座標
	bool	m_blend;			// 透明ビットをを含む
	bool	m_mipmap_enable;	// ミップマップが使える
	bool	m_dirty;			// 更新が必要フラグ
};

}
