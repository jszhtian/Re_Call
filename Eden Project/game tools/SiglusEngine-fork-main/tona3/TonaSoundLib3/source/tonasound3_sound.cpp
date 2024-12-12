#include	"tonasound3_pch.h"
#include	"tonasound3_sound.h"

namespace NT3
{

// ****************************************************************
// サウンド
// ================================================================
C_sound::C_sound()
{
	init();
}

C_sound::~C_sound()
{
	release();
}

// ****************************************************************
// サウンドの初期化
// ================================================================
void C_sound::init()
{
	channel_cnt = 0;
	bit_cnt_per_sample = 0;
	sample_cnt_per_sec = 0;
	data_size = 0;
	data.clear();
}

// ****************************************************************
// サウンドの解放
// ================================================================
void C_sound::release()
{
	init();
}

// ****************************************************************
// サウンドを作成
// ================================================================
bool C_sound::create(int channel_cnt_, int bit_cnt_per_sample_, int sample_cnt_per_sec_, int data_size_)
{
	// 現在のサウンドを解放
	release();

	// パラメータをセット
	channel_cnt = channel_cnt_;
	bit_cnt_per_sample = bit_cnt_per_sample_;
	sample_cnt_per_sec = sample_cnt_per_sec_;
	data_size = data_size_;
	// 空のバッファの作成
	data.resize(data_size);

	return true;
}

// ****************************************************************
// ストリームからサウンドをロード
//		stream は open している必要があります。
// ================================================================
bool C_sound::load_sound(C_sound_stream_base* stream)
{
	// 現在のサウンドを解放
	release();

	// バッファを確保して読み込む
	data_size = stream->get_data_size();
	data.resize(data_size);

	if (stream->read(data.get(), (int)data.size()) != data.size())
		return false;

	// パラメータをセット
	channel_cnt = stream->get_channel_cnt();
	bit_cnt_per_sample = stream->get_bit_cnt_per_sample();
	sample_cnt_per_sec = stream->get_sample_cnt_per_sec();
	data_size = stream->get_data_size();

	/*
	// 口パクボリュームを 1/60s ごとに作成（チャンネル１固定）
	int mouse_cnt = data_size / channel_cnt / (bit_cnt_per_sample / 8) * 60 / sample_cnt_per_sec;
	mouse_volume.resize(mouse_cnt);

	if (bit_cnt_per_sample == 8) {
		
		for (int i = 0; i < mouse_cnt; i++) {
			int total_volume = 0;

			// 先頭サンプル
			char* start_ptr = (char *)data.get() + i * sample_cnt_per_sec * channel_cnt / 60;
			// 末尾サンプル
			char* end_ptr = (char *)data.get() + (i + 1) * sample_cnt_per_sec * channel_cnt / 60;

			int last_volume = 0;
			for (char* p = start_ptr; p < end_ptr; p += channel_cnt) {
				total_volume += std::abs(last_volume - (int)(*p));
				last_volume = (int)(*p);
			}

			int average_volume = total_volume / ((int)(end_ptr - start_ptr) / channel_cnt);

			mouse_volume[i] = (float)average_volume / 255;
		}
	}

		// 高速処理のため、あえてエラーチェックを行ってません。異常なサンプル数を与えると落ちます。
		BYTE* ptr = data.get() + (get_channel_cnt() * sample_no + channel_no) * (get_bit_cnt_per_sample() / 8);

		if (false);
		else if (get_bit_cnt_per_sample() == 8)		return *(BYTE *)ptr;
		else if (get_bit_cnt_per_sample() == 16)	return *(short *)ptr;
	}
	*/


	return true;
}

// ****************************************************************
// wave を保存
// ================================================================
bool C_sound::save_wave(CTSTR& file_path)
{
	if (!is_ready())		return false;

	// ヘッダ構築
	wave_header header;
	memcpy(&header.ck_riff, "RIFF", 4);
	header.riff_size = sizeof(wave_header) - 8 + data_size;
	memcpy(&header.riff_type, "WAVE", 4);
	memcpy(&header.ck_fmt, "fmt ", 4);
	header.fmt_size = 16;
	header.fmt_id = WAVE_FORMAT_PCM;
	header.channel_cnt = channel_cnt;
	header.sample_cnt_per_sec = sample_cnt_per_sec;
	header.byte_cnt_per_sec = sample_cnt_per_sec * (bit_cnt_per_sample / 8) * channel_cnt;
	header.block_size = (bit_cnt_per_sample / 8) * channel_cnt;
	header.bit_cnt_per_sample = bit_cnt_per_sample;
	memcpy(&header.ck_data, "data", 4);
	header.data_size = data_size;

	// ファイルのオープン
	C_file file;
	if (!file.open(file_path, _T("wb")))
		return false;

	file.write(&header, sizeof(wave_header));
	file.write(data.get(), data_size);

	return true;
}

// ****************************************************************
// サウンドを読み込み
// ================================================================
int C_sound::read(BYTE* buf, int pos, int size)
{
	// サイズ制限
	size = limit(0, size, get_data_size() - pos);

	// 読み込み
	memcpy(buf, data.get() + pos, size);

	return size;
}

// ****************************************************************
// サンプルの取得
// ================================================================
int C_sound::get_sample(int sample_no, int channel_no)
{
	// 高速処理のため、あえてエラーチェックを行ってません。異常なサンプル数を与えると落ちます。
	BYTE* ptr = data.get() + (get_channel_cnt() * sample_no + channel_no) * (get_bit_cnt_per_sample() / 8);

	if (false);
	else if (get_bit_cnt_per_sample() == 8)		return *(BYTE *)ptr;
	else if (get_bit_cnt_per_sample() == 16)	return *(short *)ptr;
	
	return 0;
}

// ****************************************************************
// アドレスの取得
// ================================================================
BYTE* C_sound::get_ptr()
{
	return data.get();
}

BYTE* C_sound::get_ptr(int sample_no, int channel_no)
{
	return data.get() + (get_channel_cnt() * sample_no + channel_no) * (get_bit_cnt_per_sample() / 8);
}



// ****************************************************************
// サウンドストリーム
// ================================================================
C_sound_stream::C_sound_stream()
{
	read_pos = 0;
}

C_sound_stream::~C_sound_stream()
{
}

void C_sound_stream::set_sound(BSP<C_sound> sound_)
{
	sound = sound_;

	// パラメータを設定
	set_param(sound->get_channel_cnt(), sound->get_bit_cnt_per_sample(), sound->get_sample_cnt_per_sec(), sound->get_data_size());
}

int C_sound_stream::read(BYTE* buf, int size)
{
	if (!sound || !sound->is_ready())	return false;

	int read_size = sound->read(buf, read_pos, size);
	read_pos += read_size;

	return read_size;
}

bool C_sound_stream::set_read_pos(int pos)
{
	read_pos = pos;
	return true;
}

int C_sound_stream::get_read_pos()
{
	return read_pos;
}

}
