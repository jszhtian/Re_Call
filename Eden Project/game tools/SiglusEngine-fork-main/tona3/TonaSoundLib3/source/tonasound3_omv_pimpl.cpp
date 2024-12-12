#include	"tonasound3_pch.h"
#include	"tonasound3_omv.h"
#include	"tonasound3_omv_pimpl.h"

namespace NT3
{

#define		OMV_DEBUG_INFO_BOX_USE		0		// ▲デバッグ用メッセージボックスの表示の有無（リリース時は 0 にする事）

int C_omv_player_impl::yuv2rgb_table[256][256][256];

// ****************************************************************
// omv プレイヤー：コンストラクタ
// ================================================================
C_omv_player_impl::C_omv_player_impl()
{
	// フォーマット
	format_player();
}

// ****************************************************************
// omv プレイヤー：デストラクタ
// ================================================================
C_omv_player_impl::~C_omv_player_impl()
{
	// 解放
	release_player();
}

// ****************************************************************
// omv プレイヤーをフォーマットする
// ================================================================
void C_omv_player_impl::format_player()
{
	ZeroMemory(&m_omv_header, sizeof(m_omv_header));

	m_state_flag = 0;

	ZeroMemory(&m_theora_info, sizeof(m_theora_info));
	ZeroMemory(&m_theora_play_info, sizeof(m_theora_play_info));

	m_loop_flag = false;
	m_end_of_theora = false;
	m_seek_top = 0;
	m_theora_total_page_cnt = 0;
	m_theora_total_packet_cnt = 0;
	m_theora_now_page_no = -1;
	m_theora_now_frame_no = -1;

	m_theora_onmemory_buffer_p = NULL;

	ZeroMemory(&m_theora_stream, sizeof(m_theora_stream));

	ZeroMemory(&m_theora_header, sizeof(m_theora_header));
	th_info_init(&m_theora_header);

	ZeroMemory(&m_theora_comment, sizeof(m_theora_comment));
	th_comment_init(&m_theora_comment);

	m_theora_decoder = NULL;
}

// ****************************************************************
// omv プレイヤーを初期化する
// ================================================================
bool C_omv_player_impl::init_player()
{
	// 解放
	release_player();

	// フォーマット
	format_player();

	return true;
}

// ****************************************************************
// omv プレイヤーを解放する
// ================================================================
bool C_omv_player_impl::release_player()
{
	ogg_stream_clear(&m_theora_stream);
	th_info_clear(&m_theora_header);
	th_comment_clear(&m_theora_comment);
	if (m_theora_decoder != NULL)	{
		th_decode_free(m_theora_decoder);
	}

	m_theora_page_list.clear();
	m_theora_packet_list.clear();
	m_theora_onmemory_buffer.clear();
	m_theora_onmemory_buffer_p = NULL;

	// ファイルを閉じる
	m_file.close();

	return true;
}

// ****************************************************************
// エラーを設定
// ================================================================
void C_omv_player_impl::set_error(CTSTR& str)
{
	m_last_error = str;
}

// ****************************************************************
// エラーを設定（前に追加）
// ================================================================
void C_omv_player_impl::set_error_befor(CTSTR& str, bool return_separate)
{
	if (return_separate)	{
		m_last_error = str + _T("\n") + m_last_error;
	}
	else	{
		m_last_error = str + m_last_error;
	}
}

// ****************************************************************
// エラーを設定（後に追加）
// ================================================================
void C_omv_player_impl::set_error_after(CTSTR& str, bool return_separate)
{
	if (return_separate)	{
		m_last_error = m_last_error + _T("\n") + str;
	}
	else	{
		m_last_error = m_last_error + str;
	}
}

// ****************************************************************
// エラーを取得
// ================================================================
TSTR C_omv_player_impl::get_last_error()
{
	return m_last_error;
}

// ****************************************************************
// 再生中？
// ================================================================
bool C_omv_player_impl::is_playing()
{
	return (m_state_flag == 0 || m_end_of_theora) ? false : true;
}

// ****************************************************************
// 準備できてる？
// ================================================================
bool C_omv_player_impl::is_ready()
{
	return (m_state_flag == 0) ? false : true;
}

// ****************************************************************
// シオラの情報を取得
// ================================================================
void C_omv_player_impl::get_theora_info(S_omv_theora_info* info)
{
	// コピー
	*info = m_theora_info;
}

// ****************************************************************
// シオラの再生情報を取得
// ================================================================
void C_omv_player_impl::get_theora_play_info(S_omv_theora_play_info* info)
{
	// コピー
	*info = m_theora_play_info;

	// 現在のフレーム番号
	info->now_frame_no = (m_theora_now_frame_no < 0) ? 0 : m_theora_now_frame_no;

	// 現在のフレーム開始時間
	// 現在のフレーム終了時間
	// 現在のキーフレーム番号
	if (0 <= info->now_frame_no && info->now_frame_no < m_theora_total_packet_cnt)	{
		S_omv_theora_packet *tpacket = &m_theora_packet_list[info->now_frame_no];
		info->now_frame_time_start = tpacket->frame_time_start;
		info->now_frame_time_end = tpacket->frame_time_end;
		info->now_key_frame_no = tpacket->key_frame_packet_no;
	}
	else	{
		info->now_frame_time_start = 0;
		info->now_frame_time_end = 0;
		info->now_key_frame_no = 0;
	}
}

// ****************************************************************
// シオラの再生情報をリセットする
// ================================================================
void C_omv_player_impl::reset_theora_play_info()
{
	m_theora_play_info.packet_ok_cnt = 0;				// 表示に成功したパケット回数
	m_theora_play_info.packet_dropped_cnt = 0;			// ドロップしたパケット数
	m_theora_play_info.total_decode_frame_time = 0;		// デコードした総フレーム時間
	m_theora_play_info.buffering_cnt = 0;				// バッファリングした回数
}

// ****************************************************************
// シオラの総フレーム数を取得
// ================================================================
int C_omv_player_impl::get_theora_total_frame_cnt()
{
	return m_theora_info.frame_cnt;
}

// ****************************************************************
// シオラの総フレーム時間を取得
// ================================================================
int C_omv_player_impl::get_theora_total_frame_time()
{
	return m_theora_info.frame_time;
}

// ****************************************************************
// omv の再生の準備をする
// ================================================================

bool C_omv_player_impl::ready_omv(CTSTR& file_path, bool is_loop, bool is_onmemory)
{
	if (!ready_omv_func(file_path, is_loop, is_onmemory))	{
#if OMV_DEBUG_INFO_BOX_USE
		info_box(m_last_error);		// デバッグ用メッセージボックス
#endif
		return false;
	}
	return true;
}

bool C_omv_player_impl::ready_omv_func(CTSTR& file_path, bool is_loop, bool is_onmemory)
{
	ogg_packet pkt;

	// ファイルを開く
	if (!m_file.open(file_path, _T("rb")))	{
		set_error(_T("omv ファイルが開けませんでした。\n") + get_last_error_msg());
		return false;
	}

	// ループフラグを設定
	m_loop_flag = is_loop;

	// omv ヘッダを読み込む
	m_file.read(&m_omv_header, sizeof(m_omv_header));
	m_seek_top = sizeof(m_omv_header);

	// バージョン判定
	if (m_omv_header.major_version != omv_major_version || m_omv_header.minor_version != omv_minor_version)	{
		set_error(str_format(_T("omv バージョンが違います。\nFileVersion : %d.%d\n\nSystemVersion : %d.%d\n"), m_omv_header.major_version, m_omv_header.minor_version, omv_major_version, omv_minor_version) + get_last_error_msg());
		return false;
	}

	// シオラのページリストを読み込む
	if (m_omv_header.theora_page_cnt > 0)	{
		m_theora_page_list.resize(m_omv_header.theora_page_cnt);
		int read_size = sizeof(S_omv_theora_page) * m_omv_header.theora_page_cnt;
		m_file.read(m_theora_page_list.get(), read_size);
		m_seek_top += read_size;
		m_theora_total_page_cnt = m_omv_header.theora_page_cnt;
	}

	// シオラのパケットリストを読み込む
	if (m_omv_header.theora_packet_cnt > 0)	{
		m_theora_packet_list.resize(m_omv_header.theora_packet_cnt);
		int read_size = sizeof(S_omv_theora_packet) * m_omv_header.theora_packet_cnt;
		m_file.read(m_theora_packet_list.get(), read_size);
		m_seek_top += read_size;
		m_theora_total_packet_cnt = m_omv_header.theora_packet_cnt;
	}

	// シオラのストリームを初期化する
	ogg_stream_init(&m_theora_stream, m_omv_header.theora_serial_no);

	// シオラのセットアップＩＮＦＯを初期化する
	th_setup_info *theora_setupinfo = NULL;

	// シオラのヘッダーの解析
	if (read_page_for_theora(m_omv_header.theora_header_page_no) < 0)	{
		set_error_befor(_T("omv シオラのヘッダーが読み込めませんでした。"), true);
		return false;
	}
	if (ogg_stream_packetout(&m_theora_stream, &pkt) <= 0)	{
		set_error(_T("omv シオラのヘッダーが解析できませんでした。（パケットエラー）\n") + get_last_error_msg());
		return false;
	}
	if (th_decode_headerin(&m_theora_header, &m_theora_comment, &theora_setupinfo, &pkt) < 0)	{
		set_error(_T("omv シオラのヘッダーが解析できませんでした。") + get_last_error_msg());
		return false;
	}
	empty_stream_for_theora();		// パケットを空にする（★たぶん必要ない）

	// シオラのサブヘッダーの解析
	if (read_page_for_theora(m_omv_header.theora_subheader_page_no) < 0)	{
		set_error_befor(_T("omv シオラのサブヘッダーが読み込めませんでした。"), true);
		return false;
	}
	if (ogg_stream_packetout(&m_theora_stream, &pkt) <= 0)	{
		set_error(_T("omv シオラのサブヘッダー１が解析できませんでした。（パケットエラー）\n") + get_last_error_msg());
		return false;
	}
	if (th_decode_headerin(&m_theora_header, &m_theora_comment, &theora_setupinfo, &pkt) < 0)	{
		set_error(_T("omv シオラのサブヘッダー１が解析できませんでした。\n") + get_last_error_msg());
		return false;
	}
	if (ogg_stream_packetout(&m_theora_stream, &pkt) <= 0)	{
		set_error(_T("omv シオラのサブヘッダー２が解析できませんでした。（パケットエラー）\n") + get_last_error_msg());
		return false;
	}
	if (th_decode_headerin(&m_theora_header, &m_theora_comment, &theora_setupinfo, &pkt) < 0)	{
		set_error(_T("omv シオラのサブヘッダー２が解析できませんでした。\n") + get_last_error_msg());
		return false;
	}
	empty_stream_for_theora();		// パケットを空にする（★たぶん必要ない）

	// シオラのデコーダーを確保する
	m_theora_decoder = th_decode_alloc(&m_theora_header, theora_setupinfo);
	if (m_theora_decoder == NULL)	{
		set_error(_T("omv シオラのデコーダーの確保に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// シオラのデコーダーに pre-processing level を設定します。
	int pp_level = 0;		// 再生遅延をなくすために 0 を設定します。
	th_decode_ctl(m_theora_decoder, TH_DECCTL_SET_PPLEVEL, &pp_level, sizeof(pp_level));

	// シオラのセットアップＩＮＦＯを開放する
	th_setup_free(theora_setupinfo);
	theora_setupinfo = NULL;

	// シオラの情報を取得
	{
		m_theora_info.type = m_omv_header.theora_type;				// シオラの形式（0=ＲＧＢ 1=ＲＧＢＡ）
		m_theora_info.size = m_omv_header.theora_size;				// シオラのサイズ
		m_theora_info.center = m_omv_header.theora_center;			// シオラの中心座標
		m_theora_info.frame_cnt = m_omv_header.theora_packet_cnt;	// シオラの総フレーム数

		// シオラの総フレーム時間
		if (m_theora_total_packet_cnt > 0)	{
			// シオラの最後のフレーム時間を取得
			S_omv_theora_packet *tpacket = &m_theora_packet_list[m_theora_total_packet_cnt - 1];
			m_theora_info.frame_time = tpacket->frame_time_end;
		}
	}

	// オンメモリ
	if (is_onmemory)	{
		int cnt = (int)m_theora_page_list.size();
		if (cnt > 0)	{
			S_omv_theora_page *tpage = &m_theora_page_list[0];
			int read_offset = tpage->seek_offset;
			int read_size = 0;
			for (int i = 0; i < cnt; i++)	{
				read_size += tpage->page_size;
				tpage++;
			}
			if (read_size > 0)	{
				m_theora_onmemory_buffer.resize(read_size);
				m_theora_onmemory_buffer_p = m_theora_onmemory_buffer.get();
				if (m_theora_onmemory_buffer_p)	{
					m_file.seek(read_offset + m_seek_top, SEEK_SET);
					m_file.read(m_theora_onmemory_buffer_p, read_size);
				}
			}
		}
	}

	// 準備完了
	m_state_flag = 1;

	return true;
}

// ****************************************************************
// シオラのページを読み込む
// ================================================================

int C_omv_player_impl::read_page_for_theora(int page_no)
{
	int res = read_page_for_theora_func(page_no);
	if (res < 0)	{
		set_error_befor(_T("omv ページの読み込みエラー"), true);
		set_error_after(_T("ページ番号 ") + tostr(page_no), true);
		set_error_after(get_last_error_msg(), true);
	}
	return res;
}

int C_omv_player_impl::read_page_for_theora_func(int page_no)
{
	m_theora_play_info.buffering_cnt++;

	// ＥＲＲ：異常なページを指定しました
	if (page_no < 0)	{
		set_error(_T("omv 異常なページを指定しました。（アンダー）"));
		return -1;
	}

	// ＥＲＲ：異常なページを指定しました
	if (page_no >= m_theora_total_page_cnt)	{
		set_error(_T("omv 異常なページを指定しました。（オーバー）"));
		return -2;
	}

	S_omv_theora_page *tpage = &m_theora_page_list[page_no];

	// ＥＲＲ：サイズが０のページです
	if (tpage->page_size <= 0)	{
		set_error(_T("omv サイズが０のページです。"));
		return -3;
	}

	// バッファ確保
	ogg_sync_state oggsyncstate;
	ogg_sync_init(&oggsyncstate);
	char *syncbuf = ogg_sync_buffer(&oggsyncstate, tpage->page_size);

	// ＥＲＲ：バッファが確保できませんでした
	if (syncbuf == NULL)	{
		set_error(_T("omv バッファが確保できませんでした。"));
		return -4;
	}

	// 読み込み
	int read_size;
	if (m_theora_onmemory_buffer_p)	{
		int buffer_size = (int)m_theora_onmemory_buffer.size();
		if (tpage->seek_offset < buffer_size)	{
			read_size = buffer_size - tpage->seek_offset;
			if (tpage->page_size < read_size)	{
				read_size = tpage->page_size;
			}
		}
		else	{
			read_size = 0;
		}
		memcpy(syncbuf, m_theora_onmemory_buffer_p + tpage->seek_offset, read_size);
	}
	else	{
		m_file.seek(tpage->seek_offset + m_seek_top, SEEK_SET);
		read_size = (int)m_file.read(syncbuf, tpage->page_size);
	}

	// ＥＲＲ：要求分のサイズが読み込めませんでした
	if (tpage->page_size != read_size)	{
		ogg_sync_clear(&oggsyncstate);					// バッファをクリアする
		set_error(_T("omv 要求分のサイズが読み込めませんでした。"));
		return -5;
	}

	// バッファをＯＧＧに処理させる
	if (ogg_sync_wrote(&oggsyncstate, read_size) < 0)	{
		ogg_sync_clear(&oggsyncstate);					// バッファをクリアする
		set_error(_T("omv バッファ処理に失敗しました。"));
		return -6;
	}

	// ページを取得する
	ogg_page page_data;
	if (ogg_sync_pageout(&oggsyncstate, &page_data) < 0)	{
		ogg_sync_clear(&oggsyncstate);					// バッファをクリアする
		set_error(_T("omv ページの取得に失敗しました。"));
		return -7;
	}

	// ストリームに流し込む
	if (ogg_stream_pagein(&m_theora_stream, &page_data) < 0)	{
		ogg_sync_clear(&oggsyncstate);					// バッファをクリアする
		set_error(_T("omv ストリームの設定に失敗しました。"));
		return -8;
	}

	// バッファをクリアする
	ogg_sync_clear(&oggsyncstate);

	return (tpage->is_eos) ? 1 : 0;		// 1 = ＥＯＦです
}

// ****************************************************************
// シオラのストリームを空にする
// ================================================================
void C_omv_player_impl::empty_stream_for_theora(void)
{
	// ★ ogg_stream_clear / ogg_stream_reset を使用してはならない
	// 前のページからの続きを処理する為には、前のページのパケットを１つずつ取り出す必要があります。
	// そうしなければ、次のページのパケットが正常に取得できない事があります。
	ogg_packet pkt;
	while (1)	{
		if (ogg_stream_packetout(&m_theora_stream, &pkt) <= 0)	{	break;	}
	}
}

// ****************************************************************
// プレイヤーのエラーをチェックする
// ================================================================
bool C_omv_player_impl::check_player_error()
{
	if (m_state_flag != 1)	{
		set_error(_T("omv シオラの再生の準備ができていません。\n") + get_last_error_msg());
		return false;
	}

	if (m_theora_decoder == NULL)	{
		set_error(_T("omv シオラのデコーダーの準備ができていません。\n") + get_last_error_msg());
		return false;
	}

	if (m_theora_total_page_cnt <= 0)	{
		set_error(_T("omv シオラのページがありません。\n") + get_last_error_msg());
		return false;
	}

	if (m_theora_total_packet_cnt <= 0)	{
		set_error(_T("omv シオラのパケットがありません。\n") + get_last_error_msg());
		return false;
	}

	return true;
}

// ****************************************************************
// 時間を更新する
// ================================================================
//		m_end_of_theora が変わるだけです
// ================================================================
void C_omv_player_impl::update_time_only(int now_time)
{
	if (now_time < m_theora_info.frame_time)	{
		m_end_of_theora = false;
	}
	else	{
		m_end_of_theora = true;
	}
}

// ****************************************************************
// 時間をフレームにする
// ================================================================
int C_omv_player_impl::time_to_frame(int now_time, bool set_m_end_of_theora)
{
	int frame_no = -1;

	// 時間が範囲外の場合のフレーム
	if (now_time <= 0)	{
		now_time = 0;
		frame_no = 0;
		m_end_of_theora = false;	// 2012/06/01 追加
	}
	else if (now_time >= m_theora_info.frame_time)	{
		if (m_loop_flag)	{
			now_time %= m_theora_info.frame_time;
		}
		else	{
			// フレーム終了
			frame_no = m_theora_total_packet_cnt - 1;
			if (set_m_end_of_theora)	{
				m_end_of_theora = true;
			}
		}
	}
	else	{
		m_end_of_theora = false;	// 2012/06/01 追加
	}

	if (frame_no != -1)	{
		return frame_no;
	}

	// 現在のフレーム位置から算出する
	S_omv_theora_packet *tpacket;
	int i;

	// 時間からフレーム番号を算出
	if (0 <= m_theora_now_frame_no && m_theora_now_frame_no < m_theora_total_packet_cnt)	{
		tpacket = &m_theora_packet_list[m_theora_now_frame_no];
		for (i = m_theora_now_frame_no; i < m_theora_total_packet_cnt; i++)	{
			if (now_time <= tpacket->frame_time_end)	{
				if (now_time >= tpacket->frame_time_start)	{
					frame_no = i;
				}
				break;
			}
			tpacket++;
		}
	}

	if (frame_no != -1)	{
		return frame_no;
	}

	// 全てのフレーム位置から算出する
	tpacket = &m_theora_packet_list[0];
	for (int i = 0; i < m_theora_total_packet_cnt; i++)	{
		if (now_time <= tpacket->frame_time_end)	{
			frame_no = i;
			break;
		}
		tpacket++;
	}
	return frame_no;
}

// ****************************************************************
// ビデオを取得する（時間指定）
// ================================================================

bool C_omv_player_impl::check_need_update(int now_time, int* ret_frame_no, bool by_force)
{
	if (!check_player_error())	{
		return false;	// 更新しない
	}
	int frame_no = time_to_frame(now_time, true);

	if (!by_force && frame_no == m_theora_now_frame_no)	{
		return false;	// 更新しない
	}

	if (ret_frame_no)	*ret_frame_no = frame_no;
	return true;	// 更新する
}

bool C_omv_player_impl::get_video_for_time(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	if (!get_video_for_time_func(now_time, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag))	{
#if OMV_DEBUG_INFO_BOX_USE
		info_box(m_last_error);		// デバッグ用メッセージボックス
#endif
		return false;
	}
	return true;
}

bool C_omv_player_impl::get_video_for_time_func(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	if (ret_update_flag)	{	*ret_update_flag = false;	}		// 更新フラグをクリアにする（ＯＦＦにする）
	if (!check_player_error())	{
		return false;
	}
	int frame_no = time_to_frame(now_time, true);
	return get_video_for_frame_funcfunc(frame_no, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag);
}

// ****************************************************************
// ビデオを取得する（フレーム指定）
// ================================================================

bool C_omv_player_impl::get_video_for_frame(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	if (!get_video_for_frame_func(frame_no, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag))	{
#if OMV_DEBUG_INFO_BOX_USE
		info_box(m_last_error);		// デバッグ用メッセージボックス
#endif
		return false;
	}
	return true;
}

bool C_omv_player_impl::get_video_for_frame_func(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	if (ret_update_flag)	{	*ret_update_flag = false;	}		// 更新フラグをクリアにする（ＯＦＦにする）
	if (!check_player_error())	{
		return false;
	}

	// フレーム補正
	if (frame_no < 0)	{
		frame_no = 0;
	}
	else if (frame_no >= m_theora_total_packet_cnt)	{
		frame_no = m_theora_total_packet_cnt - 1;
	}

	// フレーム終了
	if (frame_no == (m_theora_total_packet_cnt - 1))	{
		if (!m_loop_flag)	{
			m_end_of_theora = true;
		}
	}

	return get_video_for_frame_funcfunc(frame_no, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag);
}

bool C_omv_player_impl::get_video_for_frame_funcfunc(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	if (!get_video_for_frame_funcfuncfunc(frame_no, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag))	{
		if (ret_update_flag)	{	*ret_update_flag = false;	}		// 更新フラグをＯＦＦにする
		set_error_befor(_T("フレーム番号 ") + tostr(frame_no), true);
		set_error_befor(_T("omv フレームが取得できませんでした。"), true);
		return false;
	}
	return true;
}

bool C_omv_player_impl::get_video_for_frame_funcfuncfunc(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	// 現在のフレーム
	if (!by_force && frame_no == m_theora_now_frame_no)	{
		return true;
	}

	if (ret_update_flag)	{	*ret_update_flag = true;	}		// 更新フラグをＯＮにする

	S_omv_theora_page *tpage;
	S_omv_theora_packet *tpacket;

	// 強制読み込み
	if (m_theora_now_page_no == -1 || m_theora_now_frame_no == -1)	{
		by_force = true;
	}
	else if (frame_no < m_theora_now_frame_no)	{
		by_force = true;

		// ドロップパケット
		m_theora_play_info.packet_dropped_cnt += ((m_theora_total_packet_cnt - m_theora_now_frame_no) - 1) + frame_no;
	}
	else	{

		// ドロップパケット
		m_theora_play_info.packet_dropped_cnt += (frame_no - m_theora_now_frame_no) - 1;

		// シーク判定
		S_omv_theora_packet *tpacket_now = &m_theora_packet_list[m_theora_now_frame_no];
		tpacket = &m_theora_packet_list[frame_no];
		if (tpacket_now->key_frame_packet_no != tpacket->key_frame_packet_no)	{
			tpage = &m_theora_page_list[tpacket->key_frame_page_no];
			if (tpacket_now->own_page_no < tpage->seek_page_no)	{
				by_force = true;	// シークした方が速い
			}
		}
	}

	// ＯＫカウントを進める
	m_theora_play_info.packet_ok_cnt++;

	// 解析開始
	ogg_packet pkt;
	int res, read_page_no, decode_packet_no;
	bool eos_flag = false;
	tpacket = &m_theora_packet_list[frame_no];
	int key_frame_packet_no = tpacket->key_frame_packet_no;
	int skip_for_key_frame_no = -1;
	bool video_update_flag = true;

	// 強制的に読み込む
	if (by_force)	{

		// ストリーム初期化
//		ogg_stream_clear(&m_theora_stream);
//		ogg_stream_init(&m_theora_stream, m_omv_header.theora_serial_no);
		ogg_stream_reset(&m_theora_stream);

		// キーフレームのページを読み込む
		tpage = &m_theora_page_list[tpacket->key_frame_page_no];
		read_page_no = tpage->seek_page_no;
		int back_page_cnt = (tpage->own_page_no - tpage->seek_page_no) + 1;
		while (1)	{
			res = read_page_for_theora(read_page_no);
			if (res < 0)	{
				set_error_befor(_T("omv キーフレームのページが読み込めませんでした。"), true);
				return false;
			}
			if (read_page_no == tpage->own_page_no)	{	break;	}	// 解析するページにきました。
			empty_stream_for_theora();								// パケットを空にする
			read_page_no++;
		}
		skip_for_key_frame_no = key_frame_packet_no;		// この変数はまだ使います
		if (tpage->is_eos)	{	eos_flag = true;	}		// この変数はまだ使います（ＥＯＳ）
		decode_packet_no = tpage->top_packet_no;			// この変数はまだ使います
		read_page_no++;										// この変数はまだ使います（ + 1 = 現在のページは既に読み込まれているので、次へ進める）
		tpage++;											// この変数はまだ使います（ + 1 = 現在のページは既に読み込まれているので、次へ進める）
	}
	// 現在のストリーム情報を利用します
	else	{
		tpacket = &m_theora_packet_list[m_theora_now_frame_no];
		tpage = &m_theora_page_list[tpacket->own_page_no];
		if (tpacket->key_frame_packet_no != key_frame_packet_no)	{
			// キーフレームが変わったのでキーフレームまでスキップする
			skip_for_key_frame_no = key_frame_packet_no;	// この変数はまだ使います
		}
		if (tpage->is_eos)	{	eos_flag = true;	}		// この変数はまだ使います（ＥＯＳ）
		decode_packet_no = m_theora_now_frame_no + 1;		// この変数はまだ使います（ + 1 = 現在のフレームは既に処理されているので、次へ進める）
		read_page_no = tpacket->own_page_no + 1;			// この変数はまだ使います（ + 1 = 現在のページは既に読み込まれているので、次へ進める）
		tpage++;											// この変数はまだ使います（ + 1 = 現在のページは既に読み込まれているので、次へ進める）
	}

	// 指定のキーフレームまでスキップ
	if (skip_for_key_frame_no != -1 && decode_packet_no != skip_for_key_frame_no)	{
		tpacket = &m_theora_packet_list[decode_packet_no];
		while (1)	{
			while (1)	{
				if (decode_packet_no == skip_for_key_frame_no)	{	break;	}
				if (ogg_stream_packetout(&m_theora_stream, &pkt) <= 0)	{	break;	}
				tpacket++;
				decode_packet_no++;
			}
			if (decode_packet_no == skip_for_key_frame_no)	{	break;	}

			res = read_page_for_theora(read_page_no);
			if (res < 0)	{
				set_error_befor(_T("omv 指定のキーフレームまでスキップできませんでした。"), true);
				return false;
			}
			if (tpage->is_eos)	{	eos_flag = true;	}		// ＥＯＳ
			read_page_no++;
			tpage++;
		}
	}

	// 解析する
	bool loop_out = false;
	ogg_int64_t granulepos = 0;
	while (1)	{
		while (1)	{

			// パケットの取得
			res = ogg_stream_packetout(&m_theora_stream, &pkt);
			if (res <= 0)	{
				loop_out = eos_flag;	// ＥＯＳの場合、ここで終了
				break;
			}

			// 何だろこれ…
			//if (pkt.granulepos >= 0)	{
			//	th_decode_ctl(m_theora_decoder, TH_DECCTL_SET_GRANPOS, &pkt.granulepos, sizeof(pkt.granulepos));
			//}

			// フレームの展開
			res = th_decode_packetin(m_theora_decoder, &pkt, &granulepos);
			if (res < 0)	{
				if (res != TH_DUPFRAME)	{		// TH_DUPFRAME = 展開する必要が無いフレーム
					set_error(_T("omv フレームの展開に失敗しました。\n") + get_last_error_msg());
					return false;
				}
				video_update_flag = false;
				loop_out = true;
				break;
			}

			// 目的のフレームに達しました
			if (decode_packet_no == frame_no)	{
				loop_out = true;
				break;
			}

			decode_packet_no++;
		}

		if (loop_out)
			break;

		res = read_page_for_theora(read_page_no);
		if (res < 0)	{
			set_error_befor(_T("omv 解析できませんでした。"), true);
			return false;
		}
		if (tpage->is_eos)	{	eos_flag = true;	}		// ＥＯＳ
		read_page_no++;
		tpage++;
	}

	// デコードした総フレーム時間に加算する
	tpacket = &m_theora_packet_list[frame_no];
	m_theora_play_info.total_decode_frame_time += tpacket->frame_time_end - tpacket->frame_time_start + 1;

	// ビデオを更新する
	m_theora_now_page_no = read_page_no;
	m_theora_now_frame_no = frame_no;

	if (alpha_only)	{
		video_write_only_alpha(p_dst_org, dst_pitch);
	}
	else	{
		video_write(p_dst_org, dst_pitch);
	}

	return true;
}

#if 0
struct S_omv_thread_data
{
	BYTE*	p_src_y;
	BYTE*	p_src_u;
	BYTE*	p_src_v;
	BYTE*	p_src_a;
	int		src_w;
	int		src_h;
	int		src_y_stride;
	int		src_u_stride;
	int		src_v_stride;
	int		src_a_stride;
	BYTE*	p_dst;
	int		dst_pitch;
};
DWORD WINAPI omv_thread_func(LPVOID pv_ref)
{
	S_omv_thread_data* p_td = (S_omv_thread_data *)pv_ref;

	BYTE* p_src_y = p_td->p_src_y;
	BYTE* p_src_u = p_td->p_src_u;
	BYTE* p_src_v = p_td->p_src_v;
	BYTE* p_src_a = p_td->p_src_a;
	int src_w = p_td->src_w;
	int src_h = p_td->src_h;
	int diff_y = p_td->src_y_stride - p_td->src_w;
	int diff_u = p_td->src_u_stride - p_td->src_w;
	int diff_v = p_td->src_v_stride - p_td->src_w;
	int diff_a = p_td->src_a_stride - p_td->src_w;
	BYTE* p_dst = p_td->p_dst;
	BYTE* p_dst_goal;
	int diff_dst = p_td->dst_pitch - p_td->src_w * 4;
	int i;

	for (i = 0; i < src_h; i++)	{
		p_dst_goal = p_dst + src_w * 4;
		while (p_dst < p_dst_goal)	{ 
			*p_dst++ = *p_src_y++;
			*p_dst++ = *p_src_u++;
			*p_dst++ = *p_src_v++;
			*p_dst++ = *p_src_a++;
		}
		p_src_y += diff_y;
		p_src_u += diff_u;
		p_src_v += diff_v;
		p_src_a += diff_a;
		p_dst += diff_dst;
	}

	// スレッド終了
	ExitThread(true);

	return 0;
}
#endif

// ****************************************************************
// ループを終了する
// ================================================================
void C_omv_player_impl::end_loop()
{
	m_loop_flag = false;
}

// ****************************************************************
// 変換テーブルを準備する
// ================================================================
void C_omv_player_impl::ready_table()
{
	// 準備は１度だけでいい
	static bool create_table_flag = false;
	if (!create_table_flag)	{
		BYTE r, g, b;
		for (int y = 0; y < 256; y++)	{
			for (int u = 0; u < 256; u++)	{
				for (int v = 0; v < 256; v++)	{
					r = limit(0, (int)(y                       + 1.40200 * (v - 128)), 255);
					g = limit(0, (int)(y - 0.34414 * (u - 128) - 0.71414 * (v - 128)), 255);
					b = limit(0, (int)(y + 1.77200 * (u - 128)                      ), 255);
					yuv2rgb_table[y][u][v] = (255 << 24) | (r << 16) | (g << 8) | b;
				}
			}
		}
		create_table_flag = true;
	}
}

// ****************************************************************
// ビデオデータを書き込む
// ================================================================
void C_omv_player_impl::video_write(BYTE* p_dst_org, int dst_pitch)
{
	th_ycbcr_buffer yuv;
	th_decode_ycbcr_out(m_theora_decoder, yuv);

	// 変換テーブルを準備する
	ready_table();

	// RGB の場合
	if (m_theora_info.type == OMV_THEORA_TYPE_RGB)	{

		int pic_w = m_theora_info.size.cx;
		int pic_h = m_theora_info.size.cy;

		int diff_y = yuv[0].stride - pic_w;
		int diff_u = yuv[1].stride - pic_w;
		int diff_v = yuv[2].stride - pic_w;
		int diff_dst = dst_pitch - pic_w * sizeof(C_argb);
		int i;

		BYTE* p_dst;
		BYTE* p_dst_goal;
		BYTE* p_src_y;
		BYTE* p_src_u;
		BYTE* p_src_v;

		// yuv のソースを設定
		p_src_y = yuv[0].data;
		p_src_u = yuv[1].data;
		p_src_v = yuv[2].data;
		p_dst = p_dst_org;

		for (i = 0; i < pic_h; i++)	{
			p_dst_goal = p_dst + pic_w * sizeof(C_argb);
			while (p_dst < p_dst_goal)	{

				// RGB => RGBA
				*p_dst++ = *p_src_y++;
				*p_dst++ = *p_src_u++;
				*p_dst++ = *p_src_v++;
				*p_dst++ = 255;
			}
			p_dst += diff_dst;
			p_src_y += diff_y;
			p_src_u += diff_u;
			p_src_v += diff_v;
		}
	}
	// YUV の場合
	if (m_theora_info.type == OMV_THEORA_TYPE_YUV)	{

		int cy, cu, cv;

		int pic_w = m_theora_info.size.cx;
		int pic_h = m_theora_info.size.cy;

		int diff_y = yuv[0].stride - pic_w;
		int diff_u = yuv[1].stride - pic_w;
		int diff_v = yuv[2].stride - pic_w;
		int diff_dst = dst_pitch - pic_w * sizeof(C_argb);
		int i;

		BYTE* p_dst;
		BYTE* p_dst_goal;
		BYTE* p_src_y;
		BYTE* p_src_u;
		BYTE* p_src_v;

		// yuv のソースを設定
		p_src_y = yuv[0].data;
		p_src_u = yuv[1].data;
		p_src_v = yuv[2].data;
		p_dst = p_dst_org;

		for (i = 0; i < pic_h; i++)	{
			p_dst_goal = p_dst + pic_w * sizeof(C_argb);
			while (p_dst < p_dst_goal)	{

				// YUV => RGBA
				cy = *p_src_y++;
				cu = *p_src_u++;
				cv = *p_src_v++;
				*(int *)p_dst = yuv2rgb_table[cy][cu][cv];
				p_dst += sizeof(int);
			}
			p_dst = p_dst + diff_dst;
			p_src_y += diff_y;
			p_src_u += diff_u;
			p_src_v += diff_v;
		}
	}
	// RGBA の場合
	// RGB それぞれのプレーンの下に、1/3 ずつ A が入っています
	if (m_theora_info.type == OMV_THEORA_TYPE_RGBA)	{

		int pic_w = m_theora_info.size.cx;
		int pic_h = m_theora_info.size.cy;
		int alpha_h = (pic_h + 2) / 3;
		int alpha_h_2 = alpha_h * 2;

		int diff_y = yuv[0].stride - pic_w;
		int diff_u = yuv[1].stride - pic_w;
		int diff_v = yuv[2].stride - pic_w;
		int diff_dst = dst_pitch - pic_w * sizeof(C_argb);
		int i;

		BYTE* p_dst;
		BYTE* p_dst_goal;
		BYTE* p_src_y;
		BYTE* p_src_u;
		BYTE* p_src_v;
		BYTE* p_src_a;

		// yuv のソースを設定
		p_src_y = yuv[0].data;
		p_src_u = yuv[1].data;
		p_src_v = yuv[2].data;
		p_dst = p_dst_org;

		// 上３分の１を描画
		p_src_a = yuv[0].data + yuv[0].stride * pic_h;
		for (i = 0; i < alpha_h; i++)	{
			p_dst_goal = p_dst + pic_w * sizeof(C_argb);
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_y++;
				*p_dst++ = *p_src_u++;
				*p_dst++ = *p_src_v++;
				*p_dst++ = *p_src_a++;
			}
			p_dst += diff_dst;
			p_src_y += diff_y;
			p_src_u += diff_u;
			p_src_v += diff_v;
			p_src_a += diff_y;	// y プレーンの下にα
		}
		// 中３分の１を描画
		p_src_a = yuv[1].data + yuv[1].stride * pic_h;
		for (i = alpha_h; i < alpha_h_2; i++)	{
			p_dst_goal = p_dst + pic_w * sizeof(C_argb);
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_y++;
				*p_dst++ = *p_src_u++;
				*p_dst++ = *p_src_v++;
				*p_dst++ = *p_src_a++;
			}
			p_dst += diff_dst;
			p_src_y += diff_y;
			p_src_u += diff_u;
			p_src_v += diff_v;
			p_src_a += diff_u;	// u プレーンの下にα
		}
		// 下３分の１を描画
		p_src_a = yuv[2].data + yuv[2].stride * pic_h;
		for (i = alpha_h_2; i < pic_h; i++)	{
			p_dst_goal = p_dst + pic_w * sizeof(C_argb);
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_y++;
				*p_dst++ = *p_src_u++;
				*p_dst++ = *p_src_v++;
				*p_dst++ = *p_src_a++;
			}
			p_dst += diff_dst;
			p_src_y += diff_y;
			p_src_u += diff_u;
			p_src_v += diff_v;
			p_src_a += diff_v;	// v プレーンの下にα
		}
	}
}

// ****************************************************************
// ビデオデータを書き込む（Ａのみ）
// ================================================================
void C_omv_player_impl::video_write_only_alpha(BYTE* p_dst_org, int dst_pitch)
{
	th_ycbcr_buffer yuv;
	th_decode_ycbcr_out(m_theora_decoder, yuv);

	// 変換テーブルを準備する
	ready_table();

	// RGB / YUV の場合
	if (m_theora_info.type == OMV_THEORA_TYPE_RGB || m_theora_info.type == OMV_THEORA_TYPE_YUV)	{

		int pic_w = m_theora_info.size.cx;
		int pic_h = m_theora_info.size.cy;

		int diff_y = yuv[0].stride - pic_w;
		int diff_u = yuv[1].stride - pic_w;
		int diff_v = yuv[2].stride - pic_w;
		int diff_dst = dst_pitch - pic_w * 4;
		int i;

		BYTE* p_dst;
		BYTE* p_dst_goal;

		// yuv のソースを設定
		p_dst = p_dst_org;

		for (i = 0; i < pic_h; i++)	{
			p_dst_goal = p_dst + pic_w * 4;
			while (p_dst < p_dst_goal)	{

				// RGB => RGBA（結論からすると真っ白になる）
				*p_dst++ = 255;
				*p_dst++ = 255;
				*p_dst++ = 255;
				*p_dst++ = 255;
			}
			p_dst += diff_dst;
		}
	}
	// RGBA の場合
	// RGB それぞれのプレーンの下に、1/3 ずつ A が入っています
	else if (m_theora_info.type == OMV_THEORA_TYPE_RGBA)	{

		int pic_w = m_theora_info.size.cx;
		int pic_h = m_theora_info.size.cy;
		int alpha_h = (pic_h + 2) / 3;
		int alpha_h_2 = alpha_h * 2;

		int diff_y = yuv[0].stride - pic_w;
		int diff_u = yuv[1].stride - pic_w;
		int diff_v = yuv[2].stride - pic_w;
		int diff_dst = dst_pitch - pic_w * 4;
		int i;

		BYTE* p_dst;
		BYTE* p_dst_goal;
		BYTE* p_src_a;

		// yuv のソースを設定
		p_dst = p_dst_org;

		// 上３分の１を描画
		p_src_a = yuv[0].data + yuv[0].stride * pic_h;
		for (i = 0; i < alpha_h; i++)	{
			p_dst_goal = p_dst + pic_w * 4;
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a++;
				*p_dst++ = 255;
			}
			p_dst += diff_dst;
			p_src_a += diff_y;	// y プレーンの下にα
		}
		// 中３分の１を描画
		p_src_a = yuv[1].data + yuv[1].stride * pic_h;
		for (i = alpha_h; i < alpha_h_2; i++)	{
			p_dst_goal = p_dst + pic_w * 4;
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a++;
				*p_dst++ = 255;
			}
			p_dst += diff_dst;
			p_src_a += diff_u;	// u プレーンの下にα
		}
		// 下３分の１を描画
		p_src_a = yuv[2].data + yuv[2].stride * pic_h;
		for (i = alpha_h_2; i < pic_h; i++)	{
			p_dst_goal = p_dst + pic_w * 4;
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a++;
				*p_dst++ = 255;
			}
			p_dst += diff_dst;
			p_src_a += diff_v;	// v プレーンの下にα
		}
	}
}

}
