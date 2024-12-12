#pragma		once

namespace NT3
{

	// ****************************************************************
	// サウンドストリーム
	//		ファイルやメモリからサウンドを読み込むための基底クラスです。
	//		これを継承して各種サウンドの読み込みクラスを作ってください。
	// ================================================================
	class C_sound_stream_base
	{
	public:
		C_sound_stream_base();
		virtual	~C_sound_stream_base()	{}

		virtual	bool	open(CTSTR& file_path, DWORD offset = 0, DWORD size = 0)		{	return false;	}	// 開く
		virtual	void	close()															{	return;			}	// 閉じる
		virtual	int		read(BYTE* buf, int size)										{	return 0;		}	// データを読み込む
		virtual	bool	set_read_pos(int pos)											{	return false;	}	// 読みこみ位置を設定
		virtual	int		get_read_pos()													{	return 0;		}	// 読みこみ位置を取得
		virtual BYTE*	get_ptr()														{	return NULL;	}	// サウンドへのポインタ

		bool	is_ready()					{	return data_size > 0;		}	// 準備判定
		int		get_channel_cnt()			{	return channel_cnt;			}	// チャンネル数を取得
		int		get_bit_cnt_per_sample()	{	return bit_cnt_per_sample;	}	// ビットレートを取得
		int		get_sample_cnt_per_sec()	{	return sample_cnt_per_sec;	}	// サンプリングレートを取得
		int		get_data_size()				{	return data_size;			}	// データサイズを取得
		int		get_fragment_size()			{	return (bit_cnt_per_sample / 8) * channel_cnt;				}	// フラグメントサイズを取得
		int		get_sample_cnt()			{	return data_size / get_fragment_size();						}	// サンプル数を取得
		int		get_length_by_millsecond()	{	return get_sample_cnt() * 1000 / get_sample_cnt_per_sec();	}	// 長さをミリ秒で取得

	protected:

		void	init();		// 初期化
		void	set_param(int channel_cnt, int bit_cnt_per_sample, int sample_cnt_per_sec, int data_size);	// パラメータ設定

	private:

		// WAVE 情報
		int		channel_cnt;			// ﾓﾉﾗﾙ=1、ｽﾃﾚｵ=2
		int		bit_cnt_per_sample;		// 8bit or 16bit
		int		sample_cnt_per_sec;		// 22050Hz, 44100Hz, ...
		int		data_size;				// データサイズ
	};

};
