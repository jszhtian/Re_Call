#pragma		once

#include	"tonasound3_wave.h"
#include	"tonasound3_stream.h"

namespace NT3
{

	// ****************************************************************
	// サウンド
	//		オンメモリにサウンドを格納します。
	// ================================================================
	class C_sound
	{
	public:
		C_sound();
		~C_sound();

		bool	create(int channel_cnt, int bit_cnt_per_sample, int sample_cnt_per_sec, int data_size);
		void	release();

		// サウンドストリームからデータを読み込む
		bool	load_sound(C_sound_stream_base* stream);

		// サウンドを保存
		bool	save_wave(CTSTR& file_path);

		// サウンドを読み込む
		int		read(BYTE* buf, int pos, int size);
		BYTE*	get_ptr();
		BYTE*	get_ptr(int sample_no, int channel_no);
		int		get_sample(int sample_no, int channel_no);
		BUFFER&	get_buffer()	{	return data;	}

		bool	is_ready()					{	return data_size > 0;		}
		int		get_channel_cnt()			{	return channel_cnt;			}
		int		get_bit_cnt_per_sample()	{	return bit_cnt_per_sample;	}
		int		get_sample_cnt_per_sec()	{	return sample_cnt_per_sec;	}
		int		get_data_size()				{	return data_size;			}
		int		get_fragment_size()			{	return (bit_cnt_per_sample / 8) * channel_cnt;	}
		int		get_sample_cnt()			{	return data_size / get_fragment_size();			}
		void	update_data_size()			{	data_size = (int)data.size();					}

	private:

		void	init();

		// WAVE 情報
		int		channel_cnt;			// ﾓﾉﾗﾙ=1、ｽﾃﾚｵ=2
		int		bit_cnt_per_sample;		// 8bit or 16bit
		int		sample_cnt_per_sec;		// 22050Hz, 44100Hz, ...
		int		data_size;				// データサイズ
		BUFFER	data;					// データ

		ARRAY<float>	mouse_volume;	// 口パクボリューム
	};

	// ****************************************************************
	// サウンドストリーム
	//		C_sound からサウンドをストリームとして読み込むクラス
	// ================================================================
	class C_sound_stream : public C_sound_stream_base
	{
	public:
		C_sound_stream();
		~C_sound_stream();

		void	set_sound(BSP<C_sound> sound);	// サウンドをセット
		int		read(BYTE* buf, int size);		// データを読み込む
		bool	set_read_pos(int pos);			// 読みこみ位置を設定
		int		get_read_pos();					// 読みこみ位置を取得

		virtual BYTE*	get_ptr()				// サウンドへのポインタ
		{
			return sound ? sound->get_ptr() : NULL;
		}

	private:
		BSP<C_sound>	sound;
		int				read_pos;
	};
}
