#pragma		once

#include	<vorbis/codec.h>
#include	<theora/codec.h>
#include	<theora/theora.h>
#include	<theora/theoradec.h>

namespace NT3
{


// ****************************************************************
// C_omv_player 実体（Pimpl イディオム）
//		omv を扱う各種構造体は巨大です。
//		ヘッダに実体を置きたくないので、Pimpl イディオムを使って隠蔽します。
// ================================================================
class C_omv_player_impl
{

public:

private:
	TSTR						m_last_error;					// エラー文字列

	S_omv_header				m_omv_header;					// omv ヘッダー

	int							m_state_flag;					// 状態

	S_omv_theora_info			m_theora_info;					// シオラの情報
	S_omv_theora_play_info		m_theora_play_info;				// シオラの再生に関する情報

	C_file						m_file;							// ファイルクラス

	bool						m_loop_flag;					// ループフラグ
	bool						m_end_of_theora;				// シオラの再生終了フラグ
	int							m_seek_top;						// ファイルシークの先頭オフセット

	int							m_theora_total_page_cnt;		// シオラの総ページ数
	int							m_theora_total_packet_cnt;		// シオラの総パケット数
	int							m_theora_now_page_no;			// シオラの現在のページ番号
	int							m_theora_now_frame_no;			// シオラの現在のフレーム番号

	ARRAY<S_omv_theora_page>	m_theora_page_list;				// ページリスト
	ARRAY<S_omv_theora_packet>	m_theora_packet_list;			// シオラのパケットリスト
	BUFFER						m_theora_onmemory_buffer;		// オンメモリバッファー
	BYTE*						m_theora_onmemory_buffer_p;		// オンメモリバッファーポインタ

	ogg_stream_state			m_theora_stream;				// シオラのストリーム
	th_info						m_theora_header;				// シオラのヘッダー
	th_comment					m_theora_comment;				// シオラのコメント
	th_dec_ctx*					m_theora_decoder;				// シオラのエンコーダー

public:
	C_omv_player_impl();
	~C_omv_player_impl();

	static void		ready_table();		// static な関数。起動時に呼んでおくことで初回再生時の遅延を無くす。

	bool	init_player();
	bool	release_player();

	TSTR	get_last_error();
	bool	is_playing();
	bool	is_ready();

	void	get_theora_info(S_omv_theora_info* info);
	void	get_theora_play_info(S_omv_theora_play_info* info);
	void	reset_theora_play_info();
	int		get_theora_total_frame_cnt();
	int		get_theora_total_frame_time();

	void	update_time_only(int now_time);
	bool	ready_omv(CTSTR& file_path, bool is_loop, bool is_onmemory);
	bool	check_need_update(int now_time, int* ret_frame_no, bool by_force);
	bool	get_video_for_time(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	bool	get_video_for_frame(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	void	end_loop();

private:
	void	format_player();

	void	set_error(CTSTR& str);
	void	set_error_befor(CTSTR& str, bool return_separate);
	void	set_error_after(CTSTR& str, bool return_separate);

	int		read_page_for_theora(int page_no);
	int		read_page_for_theora_func(int page_no);
	void	empty_stream_for_theora(void);
	bool	check_player_error();
	int		time_to_frame(int now_time, bool set_m_end_of_theora);
	bool	ready_omv_func(CTSTR& file_path, bool is_loop, bool is_onmemory);
	bool	get_video_for_time_func(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	bool	get_video_for_frame_func(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	bool	get_video_for_frame_funcfunc(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	bool	get_video_for_frame_funcfuncfunc(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	void	video_write(BYTE* p_dst_org, int dst_pitch);
	void	video_write_only_alpha(BYTE* p_dst_org, int dst_pitch);

	static int yuv2rgb_table[256][256][256];
};

}
