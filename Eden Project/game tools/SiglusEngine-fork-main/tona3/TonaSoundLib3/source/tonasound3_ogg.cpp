#include	"tonasound3_pch.h"
#include	"tonasound3_ogg.h"

#include	<ogg/ogg.h>
#include	<vorbis/vorbisfile.h>

namespace NT3
{

// ****************************************************************
// ogg 情報構造体
// ================================================================
struct Sogg_param
{
	FILE*	fp;				// ファイルポインタ
	DWORD	file_offset;	// ファイルオフセット
	DWORD	file_size;		// ファイルサイズ
	BYTE	xor;			// XOR
};

// ****************************************************************
// C_ogg_stream 実装構造体
// ================================================================
struct C_ogg_stream::Simpl
{
	OggVorbis_File	ovf;		// ogg 情報
	Sogg_param		ogg_param;	// ogg パラメータ
};

// ****************************************************************
// ogg
// ================================================================
C_ogg_stream::C_ogg_stream(BYTE xor)
{
	// 実装の作成
	impl = BSP<Simpl>(new Simpl);
	impl->ogg_param.xor = xor;			// 固定…初期化ルーチンで初期化しないこと！

	// 初期化
	init();
}

C_ogg_stream::~C_ogg_stream()
{
	close();

	// 実装の破壊
	impl.reset();
}

// ****************************************************************
// vorbis 用のコールバック関数群
// ================================================================
size_t ovopcallback_read_func(void* buf, size_t data_size, size_t to_read_cnt, void* param)
{
	Sogg_param* info = (Sogg_param *)param;
	size_t read_cnt = fread(buf, data_size, to_read_cnt, info->fp);
	
	BYTE* p = (BYTE *)buf;
	for (int i = 0; i < (int)(data_size * read_cnt); i++)	{
		*p++ ^= info->xor;
	}

	return read_cnt;
}

int ovopcallback_close_func(void* param)
{
	Sogg_param* info = (Sogg_param *)param;
	return fclose(info->fp) == 0 ? 0 : -1;
}

int ovopcallback_seek_func(void* param, ogg_int64_t offset, int whence)
{
	// ファイルオフセット分ずらす
	Sogg_param* info = (Sogg_param *)param;

	switch (whence)	{
		case SEEK_SET:
			return _fseeki64(info->fp, info->file_offset + offset, whence) == 0 ? 0 : -1;
		case SEEK_CUR:
			return _fseeki64(info->fp, offset, whence) == 0 ? 0 : -1;
		case SEEK_END:

			if (info->file_size == 0)	{	// サイズが指定されていない場合
				return _fseeki64(info->fp, offset, whence) == 0 ? 0 : -1;
			}
			else	{						// サイズが指定されている場合
				return _fseeki64(info->fp, info->file_offset + info->file_size - offset, SEEK_SET) == 0 ? 0 : -1;
			}
	}
	return -1;
}

long ovopcallback_tell_func(void* param)
{
	// ファイルオフセット分ずらす
	Sogg_param* info = (Sogg_param *)param;
	return (long)_ftelli64(info->fp) - info->file_offset;
}

// ****************************************************************
// ogg 初期化
// ================================================================
void C_ogg_stream::init()
{
	ZeroMemory(&impl->ovf, sizeof(impl->ovf));
	impl->ogg_param.fp = NULL;
	impl->ogg_param.file_offset = 0;
	read_pos = 0;

	// 基底の初期化
	C_sound_stream_base::init();
}

// ****************************************************************
// ogg を開く
// ================================================================
bool C_ogg_stream::open(CTSTR& file_path, DWORD offset, DWORD size)
{
	close();

	// ファイルオフセットをセット
	impl->ogg_param.file_offset = offset;

	// ファイルを開く
	_tfopen_s(&impl->ogg_param.fp, file_path.c_str(), _T("rb"));
	if (!impl->ogg_param.fp)
		return false;

	// ファイルサイズを取得する
	impl->ogg_param.file_size = size;

	// ファイルオフセットの位置にシークセット
	fseek(impl->ogg_param.fp, impl->ogg_param.file_offset, SEEK_SET);

	// コールバック関数のセット
	ov_callbacks cb;
	cb.read_func  = ovopcallback_read_func;
	cb.seek_func  = ovopcallback_seek_func;
	cb.close_func = ovopcallback_close_func;
	cb.tell_func  = ovopcallback_tell_func;

	// ogg を開く
	if (ov_open_callbacks(&impl->ogg_param, &impl->ovf, NULL, 0, cb) < 0)	{
		fclose(impl->ogg_param.fp);	// ここだけは fclose で閉じる必要がある
		init();						// 初期化
		return false;
	}

	// シーク可能かを調べる
	if (ov_seekable(&impl->ovf) == 0)	{
		close();		// 閉じる
		return false;
	}

	// ogg の情報を取得
	vorbis_info ogg_info = *ov_info(&impl->ovf, -1);
	// サンプル数の取得
	int sample_cnt = (int)ov_pcm_total(&impl->ovf, -1);
	// wave 情報をセット
	set_param(ogg_info.channels, 16, ogg_info.rate, sample_cnt * (16 / 8) * ogg_info.channels);

	return true;
}

// ****************************************************************
// ogg を閉じる
// ================================================================
void C_ogg_stream::close()
{
	// ogg を閉じる
	if (impl->ogg_param.fp)
		ov_clear(&impl->ovf);

	// 初期化する
	init();
}

// ****************************************************************
// ogg の読み込み
// ================================================================
int C_ogg_stream::read(BYTE* buf, int to_read_size)
{
	if (buf == NULL || impl->ogg_param.fp == NULL)
		return 0;

	// 残りバイト数
	int rest_size = to_read_size;

	BYTE* p = buf;
	int dummy;

	while (1)	{
		// ogg を読み込む
		int read_size = ov_read(&impl->ovf, (char *)p, rest_size, 0, 2, 1, &dummy);
		if (read_size == 0)
			break;	// EOF

		// 読み込み成功！
		if (read_size > 0)	{
			p += read_size;
			rest_size -= read_size;
			if (rest_size <= 0)
				break;
		}

		// 読み込み失敗！
		else if (!read_err_check(read_size))	{
			// 残りバッファを 0 で埋める
			if (rest_size > 0)	{
				ZeroMemory(p, rest_size);
			}
			break;
		}
	}

	read_pos += to_read_size - rest_size;
	return to_read_size - rest_size;
}

// ****************************************************************
// 読み込み位置を設定
// ================================================================
bool C_ogg_stream::set_read_pos(int pos)
{
	if (!impl->ogg_param.fp)		return false;

	// ogg ではサンプル位置を指定
	int pos_sample = pos / (get_bit_cnt_per_sample() / 8) / get_channel_cnt();

	// 範囲補正
	pos_sample = limit(0, pos_sample, get_sample_cnt());

	// シーク
	ov_pcm_seek(&impl->ovf, pos_sample);

	// 読み込み位置を修正
	read_pos = pos;

	return true;
}

// ****************************************************************
// 読み込み位置を取得
// ================================================================
int C_ogg_stream::get_read_pos()
{
	if (!impl->ogg_param.fp)		return 0;

	return read_pos;
}

// ****************************************************************
// ogg 読み込みエラーチェック
// ================================================================
bool C_ogg_stream::read_err_check(int res)
{
	switch (res)	{
		case OV_FALSE:		// -1 真でない。もしくは利用できるデータが無い。
			return true;

		case OV_EOF:		// -2
			return true;

		case OV_HOLE:		// -3 Vorbisfile はビットストリーム内にデータの損失や間違ったデータに遭遇した。回復は通常自動的に行われ、この戻り値は情報源としての目的にのみ使われる。
			return true;

		case OV_EREAD:		// -128 デコードのために圧縮データを取り出すさいに読み込みエラーが起こった。
			return false;

		case OV_EFAULT:		// -129 デコード状態に内部的な矛盾があった。続行は難しそうだ。
			return false;

		case OV_EIMPL:		// -130 未実装の機能である。
			return true;

		case OV_EINVAL:		// -131 無効な引数であるか、完全に初期化されてない引数を libvorbisfile に渡した。
			return false;

		case OV_ENOTVORBIS:	// -132 与えられたファイルやデータがOggVorbisデータであると認識できない。
			return false;

		case OV_EBADHEADER:	// -133 ファイルやデータはOggVorbisストリームのように見えるが、間違ったヘッダか解読不能なヘッダが含まれている。
			return false;

		case OV_EVERSION:	// -134 与えられたストリームのビットストリームフォーマットはサポートしていない。
			return false;

		case OV_ENOTAUDIO:	// -135 
			return true;

		case OV_EBADPACKET:	// -136 
			return true;

		case OV_EBADLINK:	// -137 与えられたリンクはVorbisデータストリーム内に存在しているが、ゴミや間違いのせいで解読できない。
			return false;

		case OV_ENOSEEK:	// -138 与えられたストリームは移動不可能である。
			return true;

		default:
			return true;
	}
}

}
