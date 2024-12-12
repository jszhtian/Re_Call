#pragma		once

namespace NT3
{

const int omv_major_version = 1;
const int omv_minor_version = 1;

// ****************************************************************
// omv ヘッダ構造体
// ================================================================

const int OMV_THEORA_TYPE_RGB = 0;
const int OMV_THEORA_TYPE_RGBA = 1;
const int OMV_THEORA_TYPE_YUV = 2;

struct S_omv_header
{
	int			header_size;				// ヘッダーサイズ

	BYTE		major_version;				// バージョン
	BYTE		minor_version;				// バージョン
	BYTE		dummy[2];					// ダミー（４バイト調整）

	BYTE		keep00[32];					// 32

	int			theora_type;				// シオラの形式（0=ＲＧＢ 1=ＲＧＢＡ）
	C_size		theora_size;				// シオラのサイズ
	C_point		theora_center;				// シオラの中心座標
	int			theora_us_per_frame;		// シオラの１フレームのマイクロ秒
	int			theora_serial_no;			// シオラのシリアル番号
	int			theora_header_page_no;		// シオラのヘッダーのページ番号
	int			theora_subheader_page_no;	// シオラのサブヘッダーのページ番号
	int			theora_page_cnt;			// シオラのページ数
	int			theora_packet_cnt;			// シオラのパケット数

	BYTE		keep01[32];					// 32

	int			vorbis_serial_no;			// ヴォービスのシリアル番号
	int			vorbis_header_page_no;		// ヴォービスのヘッダーのページ番号（★未使用）
	int			vorbis_subheader_page_no;	// ヴォービスのサブヘッダーのページ番号（★未使用）
	int			vorbis_page_cnt;			// ヴォービスのページ数（★未使用）
	int			vorbis_packet_cnt;			// ヴォービスのパケット数（★未使用）

	BYTE		keep02[32];					// 32
};

// ****************************************************************
// omv ページ構造体
// ================================================================

const int OMV_PAGE_TYPE_HEADER = 0;
const int OMV_PAGE_TYPE_SUBHEADER = 1;
const int OMV_PAGE_TYPE_DATA = 2;
const int OMV_PAGE_TYPE_EOS = 3;

struct S_omv_theora_page
{
	int			own_page_no;				// 自身のページ番号
	bool		is_eos;						// ＥＯＳです
	bool		is_key_page;				// キーページです（false = データは前のページから続いている）
	int			page_size;					// ページサイズ
	int			seek_offset;				// シークオフセット
	int			seek_page_no;				// シークページ番号
	int			packet_cnt;					// パケット数
	int			top_packet_no;				// 先頭パケット番号
};

// ****************************************************************
// omv ページ構造体
// ================================================================
struct S_omv_theora_packet
{
	int		own_packet_no;					// 自身のパケット番号
	int		own_page_no;					// 自身のページ番号
	int		own_packet_no_in_page;			// 自身のページ内のパケット番号
	bool	is_key_frame;					// キーフレームです
	int		key_frame_packet_no;			// キーフレームのパケット番号
	int		key_frame_page_no;				// キーフレームのページ番号
	int		frame_time_start;				// フレームの開始時間
	int		frame_time_end;					// フレームの終了時間
};

// ****************************************************************
// omv シオラの情報
// ================================================================
struct S_omv_theora_info
{
	int			type;						// シオラの形式（0=ＲＧＢ 1=ＲＧＢＡ）
	C_size		size;						// シオラのサイズ
	C_point		center;						// シオラの中心座標
	int			frame_cnt;					// シオラの総フレーム数
	int			frame_time;					// シオラの総フレーム時間
};

// ****************************************************************
// omv シオラのプレイ情報
// ================================================================
struct S_omv_theora_play_info
{
	int			now_frame_no;				// 現在のフレーム番号
	int			now_frame_time_start;		// 現在のフレーム開始時間
	int			now_frame_time_end;			// 現在のフレーム終了時間
	int			now_key_frame_no;			// 現在のキーフレーム番号

	int			packet_ok_cnt;				// 表示に成功したパケット回数
	int			packet_dropped_cnt;			// ドロップしたパケット数
	int			total_decode_frame_time;	// デコードした総フレーム時間
	int			buffering_cnt;				// バッファリングした回数
};

// ****************************************************************
// omv プレイヤー
// ================================================================
class	C_omv_player_impl;

class C_omv_player
{

public:

private:
	BSP<C_omv_player_impl>	pi;		

public:
	C_omv_player();
	~C_omv_player();

	static void		ready_table();		// static な関数。起動時に呼んでおくことで初回再生時の遅延を無くす。

	bool	init_player();				// プレイヤーを初期化する
	bool	release_player();			// プレイヤーを解放する

	bool	is_playing();				// 再生中？（準備できていない、もしくはワンショット再生の場合で再生が完了している場合は false が返る）
	bool	is_ready();					// 準備できてる？

	bool	ready_omv(CTSTR& file_path, bool is_loop, bool is_onmemory);		// omv を準備する
	void	end_loop();															// ループを終了する

	void	update_time_only(int now_time);
	bool	check_need_update(int now_time, int* ret_frame_no, bool by_force);
	bool	get_video_for_time(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);	// 時間を指定してビデオを取得する
	bool	get_video_for_frame(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);	// フレームを指定してビデオを取得する

	void	get_theora_info(S_omv_theora_info* info);							// シオラの情報を取得する
	void	get_theora_play_info(S_omv_theora_play_info* info);					// シオラの再生に関する情報を取得する
	void	reset_theora_play_info();											// シオラの再生に関する情報をリセットする

	int		get_theora_total_frame_cnt();										// シオラの総フレーム数を取得
	int		get_theora_total_frame_time();										// シオラの総フレーム時間を取得

	TSTR	get_last_error();													// エラーを取得する

private:

};

}


