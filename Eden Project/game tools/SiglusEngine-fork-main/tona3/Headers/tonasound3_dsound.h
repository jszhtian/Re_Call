#pragma		once

namespace NT3
{

// ****************************************************************
// DirectSound
//		諸事情により DirectSound5 を使っています。
//		6 以降で構造体のサイズが変わったため、一部のサウンドカードで動かない可能性があるためです。
// ================================================================
class C_direct_sound
{
public:
	C_direct_sound();
	~C_direct_sound();

	LPDIRECTSOUND	operator ->()	{	return p_ds;			}
	bool			is_ready()		{	return p_ds != NULL;	}	// 準備判定

	bool	init(HWND h_wnd);	// 初期化
	void	release();			// 解放
	bool	set_state(int channel_cnt, int bit_cnt_per_sample, int sample_cnt_for_sec);		// ステートを設定
	void	get_state(int* channel_cnt, int* bit_cnt_per_sample, int* sample_cnt_for_sec);	// ステートを取得

private:
	LPDIRECTSOUND			p_ds;			// ダイレクトサウンド
	LPDIRECTSOUNDBUFFER		p_dsb_primary;	// プライマリバッファ

	int		m_channel_cnt;
	int		m_bit_cnt_per_sample;
	int		m_sample_cnt_for_sec;
};

extern	C_direct_sound	G_ds;

// ****************************************************************
// エラー関数
// ================================================================

// エラー番号から Direct3D のエラーメッセージを取得する
TSTR get_dsound_error_msg(HRESULT hr);

// 最後のエラーを設定
void set_last_error_dsound(HRESULT hr, TSTR error_func);
void set_last_error_dsound(HRESULT hr, TSTR str, TSTR error_func);

void add_last_error_dsound(HRESULT hr, const TSTR& error_func);
void add_last_error_dsound(HRESULT hr, TSTR& error_str, const TSTR& error_func);

};
