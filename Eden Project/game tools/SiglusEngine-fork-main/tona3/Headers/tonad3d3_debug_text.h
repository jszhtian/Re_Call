//============================================================================================
// Name : tonad3d3_debug_text.h
// Spec : デバッグ用文字列表示ヘッダ
// Author : 鈴木健太郎
//============================================================================================
#pragma once

#include	"tonad3d3_d3d.h"


namespace NT3
{

/**
//
* デバッグ用テキスト描画クラス
* バッファにデータを貯めて描画する
* 色は全統一されるので複数回変更した場合は最後のが適用される
* あくまで基本機能のみでとどめておく
*/ 
class C_d3d_debug_text {
	/**
	//
	* デバッグ用テキストデータ構造体
	*/ 
	struct S_debug_text_data {
		TSTR	str;			// デバッグ文字列
		int		x;				// 出力X座標
		int		y;				// 出力Y座標
	};
private:
	BIP<ID3DXFont>				m_font;				// フォントオブジェクト
	ARRAY<S_debug_text_data>	m_array;
	unsigned long				m_color;			// テキストカラー

public:
	// コンストラクタ
	C_d3d_debug_text();	

	// デストラクタ
	~C_d3d_debug_text();


	// 初期化
	bool init(bool release_flag);

	// 解放
	void release();

	// 構築
	bool create(bool init_flag);


	// 文字の色を変更する
	// @param[in] r カラーR
	// @param[in] g カラーG
	// @param[in] b カラーB
	void set_color(int r, int g, int b);

	// テキストを描画する
	void draw();

	// 編集した文字列をバッファに貯める
	// @param[in] x X座標
	// @param[in] y Y座標
	// @param[in] format 可変長引数
	void printf(int x, int y, TCHAR* format, ...);
	void printf(TCHAR* format, ...);

	// デバイスの消失時処理
	bool on_lost_device();
	bool on_reset_device();
};

// 唯一のインスタンス
extern C_d3d_debug_text G_debug_text;


}