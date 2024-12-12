#include	"tonasound3_pch.h"
#include	"tonasound3_stream.h"

namespace NT3
{
	C_sound_stream_base::C_sound_stream_base()
	{
		init();
	}

	void C_sound_stream_base::init()
	{
		channel_cnt = 0;
		bit_cnt_per_sample = 0;
		sample_cnt_per_sec = 0;
		data_size = 0;
	}

	void C_sound_stream_base::set_param(int channel_cnt_, int bit_cnt_per_sample_, int sample_cnt_per_sec_, int data_size_)
	{
		channel_cnt = channel_cnt_;
		bit_cnt_per_sample = bit_cnt_per_sample_;
		sample_cnt_per_sec = sample_cnt_per_sec_;
		data_size = data_size_;
	}
}
