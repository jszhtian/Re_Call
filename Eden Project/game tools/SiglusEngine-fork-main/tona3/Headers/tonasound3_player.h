#pragma		once

#include	"tonasound3_dsound.h"
#include	"tonasound3_sound.h"
#include	"tonasound3_semaphore.h"

// セマフォチェックテスト。必ず 0 にしてください。詳細は下に。
#define		SEMAPHORE_TEST		0

// ****************************************************************
//	◆CHECK_SEMAPHORE について
//		全ての public なメンバ関数は、セマフォチェックを行うかどうかを CHECK_SEMAPHORE で指定できる
//		外部から呼び出すときは、必ず CHECK_SEMAPHORE = true（デフォルト値）で呼び出す
//		内部から呼び出すときは、必ず CHECK_SEMAPHORE = false で呼び出す
//	◆SEMAPHORE_TEST について
//		通常は 0。
//		1 にすることで、内部からの呼び出しに関して、CHECK_SEMAPHORE の入れ忘れをチェックできる
//		（入れ忘れている場合、コンパイルエラーとなる）
// ================================================================

#if	SEMAPHORE_TEST
#define		SC		bool CHECK_SEMAPHORE
#else
#define		SC		bool CHECK_SEMAPHORE = true
#endif

// セマフォマクロ
#define		SEMAPHORE		C_semaphore_checker semaphore_checker(CHECK_SEMAPHORE ? m_h_semaphore : NULL)

namespace NT3
{

	const int SOUND_END_POS_AUTO = -1;

	// ****************************************************************
	// プレイヤー
	// ================================================================
	class C_player
	{
	public:
		C_player();
		~C_player();

		bool	init();												// 初期化
		bool	release(SC);										// 解放（最後に一度だけ呼び出してください）
		bool	release_sound(SC);									// サウンドの解放（再生を止めるときなどに呼び出してください）
		bool	set_stream(BSP<C_sound_stream_base> stream, SC);	// ストリームをセット

		C_sound_stream_base* get_stream(SC)		{	SEMAPHORE;	return m_stream.get();	}

		bool	play(SC);					// 再生
		bool	ready(SC);					// 準備のみ
		bool	stop(SC);					// 停止＆巻き戻し
		bool	pause(SC);					// 一時停止
		bool	fade_in(int time, SC);		// フェードイン
		bool	fade_out_and_pause(int time, SC);		// フェードアウト（終了後に一時停止）
		bool	fade_out_and_release(int time, SC);		// フェードアウト（終了後に解放）

		bool	is_ready(SC)					{	SEMAPHORE;	return m_ds_buffer != NULL;				}	// 準備判定
		bool	is_playing(SC)					{	SEMAPHORE;	return m_playing;						}	// 再生中を判定（一時停止、フェードアウト中を含む）
		bool	is_pausing(SC)					{	SEMAPHORE;	return m_pause_flag;					}	// 一時停止中を判定
		bool	is_fade_out(SC)					{	SEMAPHORE;	return m_fade_out_flag;					}	// フェードアウトを判定

		int		get_channel_cnt(SC)				{	SEMAPHORE;	return m_stream ? m_stream->get_channel_cnt() : 0;			}
		int		get_bit_cnt_per_sample(SC)		{	SEMAPHORE;	return m_stream ? m_stream->get_bit_cnt_per_sample() : 0;	}
		int		get_sample_cnt_per_sec(SC)		{	SEMAPHORE;	return m_stream ? m_stream->get_sample_cnt_per_sec() : 0;	}
		int		get_data_size(SC)				{	SEMAPHORE;	return m_stream ? m_stream->get_data_size() : 0;			}
		int		get_fragment_size(SC)			{	SEMAPHORE;	return m_stream ? m_stream->get_fragment_size() : 0;		}
		int		get_sample_cnt(SC)				{	SEMAPHORE;	return m_stream ? m_stream->get_sample_cnt() : 0;			}
		int		get_length_by_millsecond(SC)	{	SEMAPHORE;	return m_stream ? m_stream->get_length_by_millsecond() : 0;	}

		void	set_range(int start_pos_sample, int end_pos_sample, int restart_pos_sample, SC);		// 再生範囲を設定
		void	set_loop(bool loop, SC);							// ループの設定
		void	set_volume(int channel, int volume, int time, SC);	// ボリュームを設定
		int		get_volume(int channel, SC);						// ボリュームを取得
		int		get_total_volume(SC);								// 合計ボリュームを取得
		void	set_play_pos_by_sample(int sample_no, SC);			// プレイ位置を設定（サンプル位置で指定）
		int		get_play_pos_by_sample(SC);							// プレイ位置を取得（サンプル位置で取得）
		void	set_play_pos_by_millsecond(int mills, SC);			// プレイ位置を設定（ﾐﾘ秒で指定）
		int		get_play_pos_by_millsecond(SC);						// プレイ位置を取得（ﾐﾘ秒で取得）

		bool	frame(SC);	// フレーム処理

		enum
		{
			VOLUME_MIN = 0,
			VOLUME_MAX = 255,
			VOLUME_CHANNEL_CNT = 8,
		};

	private:

		void	init_param();			// パラメータを初期化
		void	release_player();		// プレイヤーを解放
		void	release_stream();		// ストリームを解放
		bool	create_player();		// プレイヤーの作成

		bool	read(BYTE* buf, int size, int* read_size);	// サウンドを読みこみ
		bool	fill_buffer();								// 再生バッファを埋める
		bool	block_copy(int block_no);					// 再生バッファを埋める：ブロック指定
		bool	block_copy(BYTE* data_ptr, int data_size);	// 再生バッファを埋める：領域指定

		bool	set_play_pos(int pos);		// プレイ位置を設定
		int		get_play_pos();				// プレイ位置を取得
		bool	update_state();				// ステータスの更新
		bool	update_volume();			// ボリュームの更新

		HANDLE	m_h_semaphore;				// セマフォ

		typedef LPDIRECTSOUNDBUFFER		LPDSB;
		LPDSB	m_ds_buffer;				// サウンドバッファ
		DWORD	m_ds_buffer_size;			// サウンドバッファのサイズ
		int		m_block_size;				// ブロックサイズ
		int		m_block_cnt;				// ブロック数
		int		m_filled_block_cnt;			// 埋めたブロック数
		int		m_play_pos_on_buffer_last;	// バッファ上での再生位置（前回計測時）
		int		m_play_pos_on_sound_last;	// サウンド上での再生位置（前回計測時）
		int		m_play_start_time;			// 再生を開始した時刻
		BYTE	m_last_sample[4];			// 最後に再生した音（再生終了後はこの値で埋める）

		bool	m_loop;						// ループフラグ
		int		m_play_start_pos;			// プレイ開始位置（バイト単位）
		int		m_play_end_pos;				// プレイ終了位置（バイト単位）
		int		m_play_restart_pos;			// プレイ再開位置（バイト単位）

		bool	m_playing;					// プレイ中フラグ
		bool	m_pause_flag;				// 一時停止中
		bool	m_fade_in_flag;				// フェードイン中
		DWORD	m_fade_in_start_time;		// フェードイン開始時刻
		int		m_fade_in_time_len;			// フェードインにかける時間
		int		m_fade_in_start_value;		// フェードイン開始値
		int		m_fade_in_cur_value;		// フェードイン現在値
		bool	m_fade_out_flag;			// フェードアウト中
		bool	m_fade_out_no_release;		// フェードアウト中
		DWORD	m_fade_out_start_time;		// フェードアウト開始時刻
		int		m_fade_out_time_len;		// フェードアウトにかける時間
		int		m_fade_out_start_value;		// フェードアウト開始値
		int		m_fade_out_cur_value;		// フェードアウト現在値

		struct	S_volume_channel
		{
			int		volume;				// ボリューム（VOL_MIN ～ VOL_MAX）
			bool	fade_ing;			// ボリュームフェード中
			DWORD	fade_start_time;	// ボリュームフェード開始時刻
			int		fade_end_time;		// ボリュームフェード終了時刻
			int		fade_start_value;	// ボリュームフェード開始値
			int		fade_end_value;		// ボリュームフェード終了値
		}	volume_channel[VOLUME_CHANNEL_CNT];

		BSP<C_sound_stream_base>	m_stream;
	};


	// ****************************************************************
	// プレイマネージャ
	// ================================================================
	class C_play_manager
	{
	public:
		C_play_manager();
		~C_play_manager();

		bool	init();
		void	release();
		bool	regist_player(C_player* player);
		bool	delete_player(C_player* player);

	private:

		typedef ARRAY<C_player *>		PLAYERLIST;
		PLAYERLIST	player_list;			// プレイヤーリスト

		void		thread_main();
		HANDLE		h_thread;				// スレッドハンドル
		DWORD		thread_id;				// スレッドＩＤ
		HANDLE		h_list_semaphore;		// バッファリスト用のセマフォ

		static DWORD WINAPI thread_func(LPVOID pv_ref);
		bool		stop_thread;			// スレッドを止める
	};

}
