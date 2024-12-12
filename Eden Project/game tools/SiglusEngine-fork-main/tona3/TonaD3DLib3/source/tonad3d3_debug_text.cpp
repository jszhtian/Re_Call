//============================================================================================
// Name : tonad3d3_debug_text.cpp
// Spec : デバッグ用文字列表示
// Author : 鈴木健太郎
//============================================================================================
#include "tonad3d3_pch.h"
#include "tonad3d3_debug_text.h"


namespace NT3
{


// 唯一のインスタンス
C_d3d_debug_text G_debug_text;

// コンストラクタ
C_d3d_debug_text::C_d3d_debug_text()
{
	m_color = 0xffffffff;		///< テキストカラー
	m_array.clear();
}

// デストラクタ
C_d3d_debug_text::~C_d3d_debug_text()
{
}

// 初期化
bool C_d3d_debug_text::init(bool release_flag)
{
	// 解放
	if(release_flag)
		release();

	return true;
}

// 解放
void C_d3d_debug_text::release()
{
	m_font = NULL;

	m_array.clear();
}

// 構築
bool C_d3d_debug_text::create(bool init_flag)
{
	// 初期化
	if(init_flag) {
		if(!init(true))
			return false;
	}


	HRESULT hr;
	// フォントを初期化
	ID3DXFont* p_font = NULL;

	// フォントオブジェクトを作成する
	hr = D3DXCreateFont(G_d3d.device.body(),
		14,										// 論理単位の文字の高さ
		FW_DONTCARE,							// 論理単位の文字の幅
		FW_BOLD,								// タイプフェースの重み。太字など
		NULL,									// ミップマップ レベルの数
		FALSE,									// 斜体なら TRUE、それ以外は FALSE
		SHIFTJIS_CHARSET,						// フォントの文字セット
		OUT_DEFAULT_PRECIS,						// 
		DEFAULT_QUALITY,						// 
		DEFAULT_PITCH | FF_DONTCARE,			// フォントのピッチとファミリのインデックス
		_T("MS ゴシック"),						// タイプフェース名を含む文字列
		&p_font);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("D3DXCreateFont"), hr);
		return false;
	}

	// メンバで保持する
	m_font = BIP<ID3DXFont>(p_font, false);

	// 色
	m_color = D3DCOLOR_XRGB(255, 255, 255);

	// 配列データを初期化する
	m_array.clear();

	return true;
}

// 文字の色を変更する
// [in] r カラーR
// [in] g カラーG
// [in] b カラーB
void C_d3d_debug_text::set_color(int r, int g, int b)
{
	m_color = D3DCOLOR_XRGB(r, g, b);
}

// テキストを描画する
void C_d3d_debug_text::draw()
{
	RECT rect;

	for(int i = 0; i < (int)m_array.size(); i++) {
		// 描画位置を指定する(左上基準)
		rect.top  = m_array[i].y;
		rect.left = m_array[i].x;
		rect.right = 0;
		rect.bottom = 0;
		// 文字描画領域を計算する
		m_font->DrawText(NULL, m_array[i].str.c_str(), -1, &rect, DT_CALCRECT, NULL);
		// 文字を描画する(左上基準)
		m_font->DrawText(NULL, m_array[i].str.c_str(), -1, &rect, DT_LEFT | DT_TOP, m_color);
	}

	m_array.clear();
}


// 編集した文字列をバッファに貯める
// [in] x X座標
// [in] y Y座標
// [in] format 可変長引数
void C_d3d_debug_text::printf(int x, int y, TCHAR* format, ...)
{
	const int buf_size = 1024;
	TCHAR newstr[buf_size];

	// 書式文字列を処理
	va_list args;
	va_start(args, format);

	_vstprintf_s(&newstr[0], buf_size, format, args);
	//_vstprintf_s
	//my_vsnprintf_s(&newstr[0], DEBUGTEXT_LENGTH, format, args);
	
	va_end(args);
	
	/*
	// データを保存(コンストラクタ走る分遅い模様)
	S_debug_text_data data;
	data.str = newstr;
	data.x = x;
	data.y = y;
	*/

	// データを保存
	S_debug_text_data data = {newstr, x, y};
	m_array.push_back(data);
}

// コンソールに編集した文字列を描画する
void C_d3d_debug_text::printf(TCHAR* format, ...)
{
	const int buf_size = 1024;
	TCHAR newstr[buf_size];

	// 書式文字列を処理する
	va_list args;
	va_start(args, format);

	_vstprintf_s(&newstr[0], buf_size, format, args);
	va_end(args);

	// デバッグ出力する
	OutputDebugString(newstr);
}

bool C_d3d_debug_text::on_lost_device()
{
	HRESULT hr;

	hr = m_font->OnLostDevice();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("OnLostDevice"), hr);
		return false;
	}

	return true;
}

bool C_d3d_debug_text::on_reset_device()
{
	HRESULT hr;

	hr = m_font->OnResetDevice();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("OnResetDevice"), hr);
		return false;
	}

	return true;
}


}