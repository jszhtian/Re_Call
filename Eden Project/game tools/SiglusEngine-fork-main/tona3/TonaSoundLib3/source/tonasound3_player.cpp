#include	"tonasound3_pch.h"
#include	"tonasound3_player.h"

namespace NT3
{

// ****************************************************************
// プレイヤー
// ================================================================
C_player::C_player()
{
	m_h_semaphore = NULL;
	m_ds_buffer = NULL;
}

C_player::~C_player()
{
	release(true);	// ここだけは true !!!
}

// ****************************************************************
// 初期化
// ================================================================
bool C_player::init()
{
	release(true);	// ここだけは true !!!

	// セマフォ作成
	m_h_semaphore = CreateSemaphore(NULL, 1, 1, NULL);

	// パラメータの初期化
	init_param();

	return true;
}

// ****************************************************************
// パラメータを初期化
// ================================================================
void C_player::init_param()
{
	m_ds_buffer = NULL;
	m_ds_buffer_size = 0;
	m_block_size = 0;
	m_block_cnt = 0;
	m_filled_block_cnt = 0;
	m_play_pos_on_buffer_last = 0;
	m_play_pos_on_sound_last = 0;
	m_last_sample[0] = 0;
	m_last_sample[1] = 0;
	m_last_sample[2] = 0;
	m_last_sample[3] = 0;
	m_play_start_time = 0;

	m_loop = false;
	m_play_start_pos = 0;
	m_play_end_pos = 0;
	m_play_restart_pos = 0;

	m_playing = false;
	m_pause_flag = false;
	m_fade_in_flag = false;
	m_fade_in_start_time = 0;
	m_fade_in_time_len = 0;
	m_fade_in_start_value = 0;
	m_fade_in_cur_value = 0;
	m_fade_out_flag = false;
	m_fade_out_no_release = false;
	m_fade_out_start_time = 0;
	m_fade_out_time_len = 0;
	m_fade_out_start_value = 0;
	m_fade_out_cur_value = 0;

	for (int i = 0; i < VOLUME_CHANNEL_CNT; i++)	{
		volume_channel[i].volume = VOLUME_MAX;
		volume_channel[i].fade_ing = false;
		volume_channel[i].fade_start_time = 0;
		volume_channel[i].fade_end_time = 0;
		volume_channel[i].fade_start_value = 0;
		volume_channel[i].fade_end_value = 0;
	}
}

// ****************************************************************
// プレイヤーの作成
// ================================================================
bool C_player::create_player()
{
	HRESULT hr;

	// プレイヤーを解放
	release_player();

	// リーダーからサウンドのパラメータを取得
	int channel_cnt = m_stream->get_channel_cnt();
	int bit_cnt_per_sample = m_stream->get_bit_cnt_per_sample();
	int sample_cnt_per_sec = m_stream->get_sample_cnt_per_sec();

	// バッファサイズ計算
	m_ds_buffer_size = channel_cnt * (bit_cnt_per_sample / 8) * sample_cnt_per_sec * 8;	// ８秒分
	m_block_cnt = 2;
	m_block_size = m_ds_buffer_size / m_block_cnt;

	// wav フォーマットの作成
	WAVEFORMATEX wfex;
	ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
	wfex.nChannels = channel_cnt;
	wfex.nSamplesPerSec = sample_cnt_per_sec;
	wfex.wBitsPerSample = bit_cnt_per_sample;
	wfex.nAvgBytesPerSec = (bit_cnt_per_sample / 8) * sample_cnt_per_sec * channel_cnt;
	wfex.nBlockAlign = (bit_cnt_per_sample / 8) * channel_cnt;
	wfex.wFormatTag = WAVE_FORMAT_PCM;
	wfex.cbSize = 0;

	// バッファの設定
	DSBUFFERDESC dsb_desc;
	ZeroMemory(&dsb_desc, sizeof(DSBUFFERDESC));
	dsb_desc.dwSize = sizeof(DSBUFFERDESC);
	dsb_desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
	dsb_desc.dwBufferBytes = m_ds_buffer_size;
	dsb_desc.lpwfxFormat = &wfex;

	// バッファの作成
	hr = G_ds->CreateSoundBuffer(&dsb_desc, &m_ds_buffer, NULL);
	if (FAILED(hr))
		return false;

	// 再生範囲を設定
	m_play_start_pos = 0;
	m_play_end_pos = m_stream->get_data_size();
	m_play_restart_pos = 0;

	// カーソルをバッファの最初にセット
	m_ds_buffer->SetCurrentPosition(0);
	// 書き込みブロック位置をセット
	m_filled_block_cnt = 0;

	// 最初のブロックを埋める
	fill_buffer();

	// ステートの更新
	update_state();

	// ボリュームの更新
	update_volume();

	return true;
}

// ****************************************************************
// 解放
// ================================================================
bool C_player::release(bool CHECK_SEMAPHORE)
{
	{
		SEMAPHORE;	// セマフォチェック

		release_sound(false);
	}

	// セマフォ削除
	if (m_h_semaphore)
	{
		CloseHandle(m_h_semaphore);
		m_h_semaphore = NULL;
	}

	return true;
}

// ****************************************************************
// サウンドの解放
// ================================================================
bool C_player::release_sound(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	release_player();
	release_stream();

	return true;
}

// ****************************************************************
// ストリームの解放
// ================================================================
void C_player::release_stream()
{
	m_stream.reset();
}

// ****************************************************************
// プレイヤーの解放
// ================================================================
void C_player::release_player()
{
	// バッファの解放
	if (G_ds.is_ready() && m_ds_buffer)
	{
		m_ds_buffer->Release();
		m_ds_buffer = NULL;
	}

	// プレイヤーに関するパラメータを初期化
	m_ds_buffer = NULL;
	m_ds_buffer_size = 0;
	m_block_size = 0;
	m_block_cnt = 0;
	m_filled_block_cnt = 0;
	m_play_pos_on_buffer_last = 0;
	m_play_pos_on_sound_last = 0;
	m_last_sample[0] = 0;
	m_last_sample[1] = 0;
	m_last_sample[2] = 0;
	m_last_sample[3] = 0;
	m_play_start_time = 0;

	m_playing = false;
	m_pause_flag = false;
	m_fade_in_flag = false;
	m_fade_in_start_time = 0;
	m_fade_in_time_len = 0;
	m_fade_in_start_value = 0;
	m_fade_in_cur_value = 0;
	m_fade_out_flag = false;
	m_fade_out_no_release = false;
	m_fade_out_start_time = 0;
	m_fade_out_time_len = 0;
	m_fade_out_start_value = 0;
	m_fade_out_cur_value = 0;
}

// ****************************************************************
// ストリームをセット
// ================================================================
bool C_player::set_stream(BSP<C_sound_stream_base> stream, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	// サウンドを解放
	release_player();
	release_stream();

	// ストリームが準備できていなかったら失敗
	if (!stream || !stream->is_ready())	
		return false;

	// ストリームの設定
	m_stream = stream;

	// プレイヤーの作成
	if (!create_player())
		return false;

	return true;
}

// ****************************************************************
// フレーム処理
// ================================================================
bool C_player::frame(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	// 再生直後、なぜか GetCurrentPosition が不正な値を返すことがあります。
	// そのため、再生を開始してから 200 ﾐﾘ秒の間は、バッファを更新しません。
	if (::GetTickCount() - m_play_start_time >= 200)
	{
		// バッファを埋める
		fill_buffer();

		// ステータス更新
		update_state();
	}

	// ボリュームの更新
	update_volume();

	return true;
}

// ****************************************************************
// バッファを埋める
// ================================================================
bool C_player::fill_buffer()
{
	if (!m_ds_buffer || m_ds_buffer_size == 0)
		return false;

	// プレイ位置の取得（バイト）
	DWORD play_pos_on_buffer;
	m_ds_buffer->GetCurrentPosition(&play_pos_on_buffer, NULL);

	// プレイ位置をパーセントで得る
	DWORD percent = play_pos_on_buffer * 100 / m_ds_buffer_size;
	
	// プレイ位置が前回埋めたブロックに入ったら、次のブロックを埋める
	if (false);
	else if (m_filled_block_cnt == 0)	{
		block_copy(0);	// １回も読んでない場合は必ず読む
	}
	else if (m_filled_block_cnt == 1)	{
		if (0 <= percent && percent < 50)	{
			block_copy(1);
		}
	}
	else if (m_filled_block_cnt == 2)	{
		if (50 <= percent && percent < 100)	{
			block_copy(0);
		}
	}

	return true;
}

// ****************************************************************
// バッファにブロックデータをコピー
// ================================================================
bool C_player::block_copy(int block_no)
{
	HRESULT hr;

	if (!m_stream || !m_stream->is_ready())
		return false;

	// 再生が終了しているなら停止（ノイズを防ぐための予備処理）
	if (m_playing)	{
		if ((!m_loop && get_play_pos() >= m_play_end_pos)
			|| (m_fade_out_flag && (int)(::GetTickCount() - m_fade_out_start_time) > m_fade_out_time_len))	{

			release_player();	// プレイヤーを解放
			//reset(false);		// 停止して巻き戻し
			return true;
		}
	}

	// バッファをロック
	LPBYTE lp_block_add_1, lp_block_add_2;
	DWORD block_size_1, block_size_2;
	hr = m_ds_buffer->Lock(m_block_size * block_no, m_block_size, (LPVOID *)&lp_block_add_1, &block_size_1, (LPVOID *)&lp_block_add_2, &block_size_2, 0);
	if (FAILED(hr))
		return false;

	// バッファにデータをコピー
	block_copy(lp_block_add_1, block_size_1);	// バッファの前半にデータをコピー
	block_copy(lp_block_add_2, block_size_2);	// バッファの後半にデータをコピー

	// バッファをアンロック
	m_ds_buffer->Unlock(lp_block_add_1, block_size_1, lp_block_add_2, block_size_2);

	// 埋めたブロック数
	m_filled_block_cnt = block_no + 1;

	return true;
}

// ****************************************************************
// バッファにブロックデータをコピー（内部用）
// ================================================================
bool C_player::block_copy(BYTE* data_ptr, int copy_size)
{
	int offset = 0;		// 更新オフセット

	// バッファにデータをコピー
	for (offset = 0; offset < copy_size; )	{

		// 読み込むべきサイズを計算
		int read_pos = m_stream->get_read_pos();
		int to_read_size = std::min(copy_size - offset, m_play_end_pos - read_pos);
		// サウンドを読み込む
		int read_size = m_stream->read(data_ptr + offset, to_read_size);

#if 0
		// ★加工してみる
		int zure = 3000;	// short* でのオフセット
		short* s = std::max((short *)(backup.get() + start_pos + offset), (short *)backup.get() + zure);
		for (short* p = s; p < (short *)(backup.get() + start_pos + offset + read_size); p++)	{
			int result = *p;
			result = result + (int)((double)(*(p - zure)) / 1.5);
			result = limit(-32000, result, 32000);
			*p = (short)result;
		}
#endif

		// 読み込み位置の移動
		offset += read_size;
		// 最後に読み込んだ音を覚えておく
		if (read_size > 0)	{
			int byte_cnt = m_stream->get_bit_cnt_per_sample() / 8 * m_stream->get_channel_cnt();
			memcpy(m_last_sample, data_ptr + offset - byte_cnt, byte_cnt);
		}
		// 指定したサイズだけ読み込めなかった場合
		if (copy_size != offset)	{
			// ループ再生ならファイルを巻き戻す
			if (m_loop)	{
				if (m_play_restart_pos >= m_play_end_pos)	{
					break;		// 巻き戻せないので読み込み終了
				}
				else	{
					m_stream->set_read_pos(m_play_restart_pos);
				}
			}
			// ループ再生でないなら読み込み終了
			else	{
				break;
			}
		}
	}

	// 読み込んだサイズとコピーしたサイズが違う場合
	if (!m_loop && copy_size != offset)	{
		// 残りの部分を最後に読み込んだ値で埋める
		int byte_cnt = m_stream->get_bit_cnt_per_sample() / 8 * m_stream->get_channel_cnt();
		if (byte_cnt == 1)	fill_memory(data_ptr + offset, (copy_size - offset) / 1, *(BYTE *)m_last_sample);
		if (byte_cnt == 2)	fill_memory_2(data_ptr + offset, (copy_size - offset) / 2, *(WORD *)m_last_sample);
		if (byte_cnt == 4)	fill_memory_4(data_ptr + offset, (copy_size - offset) / 4, *(DWORD *)m_last_sample);
	}


	return true;
}

#if 0
// ****************************************************************
// バッファにブロックデータをコピー（内部用）
// ================================================================
bool C_player::block_copy(BYTE* data_ptr, int copy_size)
{
	int offset = 0;		// 更新オフセット

	// バッファにデータをコピー
	for (offset = 0; offset < copy_size; )	{
		// 読み込むべきサイズを計算
		int read_pos = reader->get_read_pos();
		int to_read_size = std::min(copy_size - offset, play_end_pos - read_pos);
		// サウンドを読み込む
		int read_size = reader->read(data_ptr + offset, to_read_size);

		// 読み込み位置の移動
		offset += read_size;
		// 最後に読み込んだ音を覚えておく
		if (read_size > 0)	{
			int byte_cnt = reader->get_bit_cnt_per_sample() / 8 * reader->get_channel_cnt();
			memcpy(last_sample, data_ptr + offset - byte_cnt, byte_cnt);
		}
		// 指定したサイズだけ読み込めなかった場合
		if (copy_size != offset)	{
			if (loop)	reader->set_read_pos(play_restart_pos);		// ループ再生ならファイルを巻き戻す
			else		break;										// ループ再生でないなら読み込み終了
		}
	}

	// 読み込んだサイズとコピーしたサイズが違う場合
	if (!loop && copy_size != offset)	{
		// 残りの部分を最後に読み込んだ値で埋める
		int byte_cnt = reader->get_bit_cnt_per_sample() / 8 * reader->get_channel_cnt();
		if (byte_cnt == 1)	fill_memory(data_ptr + offset, (copy_size - offset) / 1, *(BYTE *)last_sample);
		if (byte_cnt == 2)	fill_memory_2(data_ptr + offset, (copy_size - offset) / 2, *(WORD *)last_sample);
		if (byte_cnt == 4)	fill_memory_4(data_ptr + offset, (copy_size - offset) / 4, *(DWORD *)last_sample);
	}

	return true;
}
#endif

// ****************************************************************
// 再生
// ================================================================
bool C_player::play(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// 範囲チェック
	if (m_play_start_pos >= m_play_end_pos)	{
		if (m_loop)		m_play_start_pos = m_play_restart_pos;
		else			return false;
	}

	// 演奏開始
	hr = m_ds_buffer->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(hr))
		return false;

	// フラグＯＮ
	m_playing = true;
	m_play_start_time = ::GetTickCount();
	m_pause_flag = false;
	m_fade_in_flag = false;
	m_fade_in_start_time = 0;
	m_fade_in_time_len = 0;
	m_fade_in_start_value = 0;
	m_fade_in_cur_value = 0;

	return true;
}

// ****************************************************************
// 準備のみ
// ================================================================
bool C_player::ready(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// 演奏開始
	hr = m_ds_buffer->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(hr))
		return false;

	// 演奏停止
	hr = m_ds_buffer->Stop();
	if (FAILED(hr))
		return false;

	// フラグＯＮ
	m_playing = true;
	m_play_start_time = ::GetTickCount();
	m_pause_flag = true;
	m_fade_in_flag = false;
	m_fade_in_start_time = 0;
	m_fade_in_time_len = 0;
	m_fade_in_start_value = 0;
	m_fade_in_cur_value = 0;

	return true;
}

// ****************************************************************
// 再生（フェードイン）
// ================================================================
bool C_player::fade_in(int fade_time, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// 演奏開始
	hr = m_ds_buffer->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(hr))
		return false;

	// フェードアウトは終了させる
	m_fade_out_flag = false;

	// フラグの設定
	m_playing = true;
	m_play_start_time = ::GetTickCount();
	m_pause_flag = false;
	m_fade_in_start_time = ::GetTickCount();
	m_fade_in_time_len = fade_time;
	m_fade_in_start_value = 0;	// フェードインは必ず最初からはじめる
	m_fade_in_flag = true;

	// ボリュームを更新（フェードインのために必須）
	update_volume();

	return true;
}

// ****************************************************************
// 停止
// ================================================================
bool C_player::stop(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	if (!set_play_pos(0))
		return false;

	return true;
}

// ****************************************************************
// 一時停止
// ================================================================
bool C_player::pause(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// 演奏停止
	hr = m_ds_buffer->Stop();
	if (FAILED(hr))
		return false;

	return true;
}

// ****************************************************************
// フェードアウト（終了後に一時停止）
// ================================================================
bool C_player::fade_out_and_pause(int time_len, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	if (!m_stream || !m_ds_buffer)
		return false;

	// フェードアウト中の場合
	if (m_fade_out_flag)
	{
		// 先に終わる方のフェードアウトを優先する
		int fade_out_end_time_old = m_fade_out_start_time + m_fade_out_time_len;
		int fade_out_end_time_new = ::GetTickCount() + time_len;
		if (fade_out_end_time_old - fade_out_end_time_new > 0)
		{
			// フェードアウト開始
			m_fade_out_flag = true;
			m_fade_out_no_release = true;
			m_fade_out_start_time = ::GetTickCount();
			m_fade_out_time_len = time_len;
			m_fade_out_start_value = m_fade_out_cur_value;
		}
	}

	// フェードアウト中でない場合
	else
	{
		// フェードアウト開始
		m_fade_out_flag = true;
		m_fade_out_no_release = true;
		m_fade_out_start_time = ::GetTickCount();
		m_fade_out_time_len = time_len;
		m_fade_out_start_value = VOLUME_MAX;
	}

	// ボリューム更新
	update_volume();

	return true;
}

// ****************************************************************
// フェードアウト（終了後に解放）
// ================================================================
bool C_player::fade_out_and_release(int time_len, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	if (!m_stream || !m_ds_buffer)
		return false;

	// フェードアウト中の場合
	if (m_fade_out_flag)
	{
		// 先に終わる方のフェードアウトを優先する
		int fade_out_end_time_old = m_fade_out_start_time + m_fade_out_time_len;
		int fade_out_end_time_new = ::GetTickCount() + time_len;
		if (fade_out_end_time_old - fade_out_end_time_new > 0)
		{
			// フェードアウト開始
			m_fade_out_flag = true;
			m_fade_out_no_release = false;
			m_fade_out_start_time = ::GetTickCount();
			m_fade_out_time_len = time_len;
			m_fade_out_start_value = m_fade_out_cur_value;
		}
	}

	// フェードアウト中でない場合
	else
	{
		// フェードアウト開始
		m_fade_out_flag = true;
		m_fade_out_no_release = false;
		m_fade_out_start_time = ::GetTickCount();
		m_fade_out_time_len = time_len;
		m_fade_out_start_value = VOLUME_MAX;
	}

	// ボリューム更新
	update_volume();

	return true;
}

// ****************************************************************
// 再生位置を設定
// ================================================================
bool C_player::set_play_pos(int pos)
{
	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// 演奏開始
	hr = m_ds_buffer->Stop();
	if (FAILED(hr))
		return false;

	// 読み込み位置を移動
	m_stream->set_read_pos(pos);
	m_ds_buffer->SetCurrentPosition(0);
	m_filled_block_cnt = 0;
	m_play_pos_on_buffer_last = 0;
	m_play_pos_on_sound_last = pos;

	// 最初のブロックを埋める
	fill_buffer();

	// フラグＯＦＦ
	m_playing = false;

	return true;
}

void C_player::set_play_pos_by_sample(int pos_sample, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	if (!m_stream || !m_ds_buffer)
		return;

	set_play_pos(pos_sample * (m_stream->get_bit_cnt_per_sample() / 8) * m_stream->get_channel_cnt());
}

void C_player::set_play_pos_by_millsecond(int pos_mills, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	if (!m_stream || !m_ds_buffer)
		return;

	set_play_pos((__int64)pos_mills * m_stream->get_sample_cnt_per_sec() / 1000 * get_fragment_size(false));
}

// ****************************************************************
// 再生位置を取得
// ================================================================
int C_player::get_play_pos()
{
	if (!m_stream || !m_ds_buffer)
		return 0;

	return m_play_pos_on_sound_last;
}

int C_player::get_play_pos_by_sample(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	if (!m_stream || !m_ds_buffer)
		return 0;

	return get_play_pos() / get_fragment_size(false);
}

int C_player::get_play_pos_by_millsecond(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	if (!m_stream || !m_ds_buffer)
		return 0;

	return (int)((__int64)get_play_pos() / get_fragment_size(false) * 1000 / m_stream->get_sample_cnt_per_sec());
}

// ****************************************************************
// 再生範囲の設定
// ================================================================
void C_player::set_range(int start_pos_sample, int end_pos_sample, int restart_pos_sample, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	if (!m_stream || !m_ds_buffer)
		return;

	// 範囲制限
#if 1
	end_pos_sample = end_pos_sample == SOUND_END_POS_AUTO ? get_sample_cnt(false) : limit(0, end_pos_sample, get_sample_cnt(false));
	start_pos_sample = limit(0, start_pos_sample, end_pos_sample);
	restart_pos_sample = limit(0, restart_pos_sample, end_pos_sample);
#else
	start_pos_sample = limit(0, start_pos_sample, get_sample_cnt(false));
	end_pos_sample = end_pos_sample == -1 ? get_sample_cnt(false) : limit(start_pos_sample + 1, end_pos_sample, get_sample_cnt(false));
	restart_pos_sample = limit(start_pos_sample, restart_pos_sample, end_pos_sample - 1);
#endif

	// バイト数に変換
	m_play_start_pos = start_pos_sample * get_channel_cnt(false) * get_bit_cnt_per_sample(false) / 8;
	m_play_end_pos = end_pos_sample * get_channel_cnt(false) * get_bit_cnt_per_sample(false) / 8;
	m_play_restart_pos = restart_pos_sample * get_channel_cnt(false) * get_bit_cnt_per_sample(false) / 8;

	// 再生位置の設定
	set_play_pos(m_play_start_pos);
}

// ****************************************************************
// ループの設定
// ================================================================
void C_player::set_loop(bool loop, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	m_loop = loop;
}

// ****************************************************************
// ボリュームを設定
// ================================================================
void C_player::set_volume(int channel_no, int volume, int time, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	if (0 <= channel_no && channel_no < VOLUME_CHANNEL_CNT)	{

		// 範囲を制限
		volume = limit<int>(VOLUME_MIN, volume, VOLUME_MAX);

		// ボリュームフェードを停止
		volume_channel[channel_no].fade_ing = false;

		// ボリュームフェードを設定
		if (time > 0)	{

			// ボリュームフェードを設定
			volume_channel[channel_no].fade_ing = true;
			volume_channel[channel_no].fade_start_time = ::GetTickCount();
			volume_channel[channel_no].fade_end_time = volume_channel[channel_no].fade_start_time + time;
			volume_channel[channel_no].fade_start_value = volume_channel[channel_no].volume;
			volume_channel[channel_no].fade_end_value = volume;
		}

		// ボリュームを設定
		volume_channel[channel_no].volume = limit<int>(VOLUME_MIN, volume, VOLUME_MAX);

		// ボリュームの更新
		update_volume();
	}
}

// ****************************************************************
// ボリュームを取得
// ================================================================
int C_player::get_volume(int channel_no, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	if (0 <= channel_no && channel_no < VOLUME_CHANNEL_CNT)	{
		return volume_channel[channel_no].volume;
	}

	return 0;
}

// ****************************************************************
// 合計ボリュームを取得
// ================================================================
int C_player::get_total_volume(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// セマフォチェック

	int volume = 255;
	for (int channel_no = 0; channel_no < VOLUME_CHANNEL_CNT; channel_no++)	{
		volume = volume * volume_channel[channel_no].volume / 255;
	}

	return volume;
}

// ****************************************************************
// ステートの更新
// ================================================================
bool C_player::update_state()
{
	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// ================================================================
	// プレイ位置
	// ----------------------------------------------------------------

	// バッファ上でのプレイ位置を取得（byte）
	int play_pos_on_buffer, play_pos_on_sound;
	hr = m_ds_buffer->GetCurrentPosition((DWORD *)&play_pos_on_buffer, NULL);
	if (FAILED(hr))	{
		set_last_error(_T("GetCurrentPosition に失敗しました。"), _T("GetCurrentPosition"));
		return false;
	}

	// バッファ上での進んだ距離を調べる
	int play_len_on_buffer = (play_pos_on_buffer - m_play_pos_on_buffer_last + m_ds_buffer_size) % m_ds_buffer_size;
	//if (play_len_on_buffer > (int)ds_buffer_size / 2)
	//	return false;	// 再生開始ちょっと後、ときどき play_pos_on_buffer が謎に 0 に戻ります。それを避けるための仮処理です。
	//if (play_pos_on_buffer == 0)
	//	return false;	// ↑の対策方法を変えてみました。位置が 0 のときは無視するようにします。正常動作時、たまたま 0 が返ってきて無視されても、次のフレームで 0 以外が返ってくると思うので問題ないです。

	// サウンド上でのプレイ位置を取得（byte）
	play_pos_on_sound = m_play_pos_on_sound_last + play_len_on_buffer;

	// プレイ位置を修正
	if ((int)play_pos_on_sound > m_play_end_pos)	{
		if (m_loop)	{
			play_pos_on_sound = (play_pos_on_sound - m_play_end_pos) % (m_play_end_pos - m_play_restart_pos) + m_play_restart_pos;
		}
		else	{
			// プレイヤーを解放
			release_player();
			release_stream();
		}
	}

	// プレイ位置を退避
	m_play_pos_on_sound_last = play_pos_on_sound;
	m_play_pos_on_buffer_last = play_pos_on_buffer;

	return true;
}

// ****************************************************************
// ボリュームの更新
// ================================================================
bool C_player::update_volume()
{
	if (!m_stream || !m_ds_buffer)
		return false;

	int total_volume = VOLUME_MAX;

	// 各チャンネルのボリュームを適用する
	for (int channel_no = 0; channel_no < VOLUME_CHANNEL_CNT; channel_no++)	{
		S_volume_channel* channel = &volume_channel[channel_no];

		// ボリュームフェード中
		if (channel->fade_ing)	{
			DWORD now_time = ::GetTickCount();
			total_volume = total_volume * linear_limit<int>(now_time, channel->fade_start_time, channel->fade_start_value, channel->fade_end_time, channel->fade_end_value) / VOLUME_MAX;
			if ((int)(now_time - channel->fade_end_time) >= 0)
				channel->fade_ing = false;
		}
		// ボリュームフェード中でない
		else	{
			total_volume = total_volume * channel->volume / VOLUME_MAX;
		}
	}

	// フェードイン
	if (m_fade_in_flag)	{
		DWORD now_time = ::GetTickCount();
		int past_time = now_time - m_fade_in_start_time;
		m_fade_in_cur_value = linear_limit<int, int>(past_time, 0, m_fade_in_start_value, m_fade_in_time_len, VOLUME_MAX);
		total_volume = linear_limit<int, int>(m_fade_in_cur_value, VOLUME_MIN, VOLUME_MIN, VOLUME_MAX, total_volume);
		if ((int)(now_time - (m_fade_in_start_time + m_fade_in_time_len)) >= 0)
			m_fade_in_flag = false;
	}

	// フェードアウト
	if (m_fade_out_flag)	{
		DWORD now_time = ::GetTickCount();
		int past_time = now_time - m_fade_out_start_time;
		m_fade_out_cur_value = linear_limit<int, int>(past_time, 0, m_fade_out_start_value, m_fade_out_time_len, VOLUME_MIN);
		total_volume = linear_limit<int, int>(m_fade_out_cur_value, 0, 0, VOLUME_MAX, total_volume);
		if ((int)(now_time - (m_fade_out_start_time + m_fade_out_time_len)) >= 0)	{
			m_fade_out_flag = false;

			if (m_fade_out_no_release)
			{
				pause(false);		// フェードアウトが終わったので一時停止
			}
			else
			{
				release_player();	// フェードアウトが終わったのでプレイヤーを解放
			}
		}
	}

	// 念のため補正
	total_volume = limit<int>(VOLUME_MIN, total_volume, VOLUME_MAX);

	// DirectSound のボリュームに補正
	int dsb_volume;
	if (total_volume == 0)	{
		dsb_volume = DSBVOLUME_MIN;		// log(0) を避けるために例外処理
	}
	else	{
		dsb_volume = (int)(log((double)total_volume / (VOLUME_MAX - VOLUME_MIN)) / log(10.0f) * 5000);	// 5000 という値に深い意味はないです。耳で聴いた感覚。
		dsb_volume = limit(DSBVOLUME_MIN, dsb_volume, DSBVOLUME_MAX);
	}

	// ボリュームの設定
	if (m_ds_buffer)
		m_ds_buffer->SetVolume(dsb_volume);

	return true;
}

// ****************************************************************
// プレイマネージャ
// ================================================================
C_play_manager::C_play_manager()
{
	h_thread = NULL;
	thread_id = 0;
	stop_thread = false;

	// セマフォ作成
	h_list_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
}

C_play_manager::~C_play_manager()
{
	// セマフォチェック
	WaitForSingleObject(h_list_semaphore, INFINITE);

	// セマフォ解放
	ReleaseSemaphore(h_list_semaphore, 1, NULL);

	// セマフォ削除
	if (h_list_semaphore)	{
		CloseHandle(h_list_semaphore);
		h_list_semaphore = NULL;
	}
}

// ****************************************************************
// 初期化
// ================================================================
bool C_play_manager::init()
{
	// ストリーミング監視用のスレッドを作成
	h_thread = CreateThread(NULL, 0, thread_func, this, 0, &thread_id);
	if (!h_thread)
		return false;

	return true;
}

// ****************************************************************
// 解放
// ================================================================
void C_play_manager::release()
{
	// セマフォチェック
	WaitForSingleObject(h_list_semaphore, INFINITE);

	// プレイヤーリスト初期化
	player_list.clear();

	// セマフォ解放
	ReleaseSemaphore(h_list_semaphore, 1, NULL);

	// セマフォ削除
	if (h_list_semaphore)	{
		CloseHandle(h_list_semaphore);
		h_list_semaphore = NULL;
	}

	// スレッドを停止する
	stop_thread = true;

	// スレッドの停止が完了するのを待つ
	DWORD start = GetTickCount();
	while (stop_thread)	{
		DWORD now = GetTickCount();
		if (now > start + 2000)	// ２秒たてば
			break;				// 無理やり抜ける

		Sleep(100);
	}

	CloseHandle(h_thread);
	h_thread = NULL;
}

// ****************************************************************
// プレイヤーの登録
// ================================================================
bool C_play_manager::regist_player(C_player* player)
{
	// セマフォチェック
	WaitForSingleObject(h_list_semaphore, INFINITE);

	// 指定された要素を検索し、見つからなければ登録
	PLAYERLIST::iterator itr = std::find(player_list.begin(), player_list.end(), player);
	if (itr == player_list.end())
		player_list.push_back(player);

	// セマフォ解放
	ReleaseSemaphore(h_list_semaphore, 1, NULL);

	return true;
}

// ****************************************************************
// プレイヤーの登録解除
// ================================================================
bool C_play_manager::delete_player(C_player* player)
{	
	// セマフォチェック
	WaitForSingleObject(h_list_semaphore, INFINITE);

	// 指定されたプレイヤーを全て削除
	player_list.erase(std::remove(player_list.begin(), player_list.end(), player), player_list.end());

	// セマフォ解放
	ReleaseSemaphore(h_list_semaphore, 1, NULL);

	return true;
}

// ****************************************************************
// 監視スレッド
// ================================================================
DWORD WINAPI C_play_manager::thread_func(LPVOID pv_ref)
{
	C_play_manager* manager = (C_play_manager *)pv_ref;

	// スレッドのメインプロセスを定期的に呼ぶ
	while (!manager->stop_thread)	{
		manager->thread_main();
		Sleep(20);
	}

	// スレッド終了
	manager->stop_thread = false;
	ExitThread(true);

	return 0;
}

// ****************************************************************
// スレッド監視
// ================================================================
void C_play_manager::thread_main()
{
	// セマフォチェック
	WaitForSingleObject(h_list_semaphore, INFINITE);

	// 全てのプレイヤーについてフレーム処理
	PLAYERLIST::iterator itr;
	for (itr = player_list.begin(); itr != player_list.end(); ++itr)
		(*itr)->frame();

	// セマフォ解放
	ReleaseSemaphore(h_list_semaphore, 1, NULL);
}

}
