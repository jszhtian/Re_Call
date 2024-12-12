#include	"tonasound3_pch.h"
#include	"tonasound3_wave.h"

namespace NT3
{

// ******************************************************
// wave ストリーム
// ======================================================
C_wave_stream::C_wave_stream()
{
	init();
}

C_wave_stream::~C_wave_stream()
{
	close();
}

// ******************************************************
// wave 初期化
// ======================================================
void C_wave_stream::init()
{
	h_mmio = NULL;

	ZeroMemory(&ck_riff, sizeof(MMCKINFO));
	ZeroMemory(&ck_fmt, sizeof(MMCKINFO));
	ZeroMemory(&ck_list, sizeof(MMCKINFO));
	ZeroMemory(&ck_icop, sizeof(MMCKINFO));
	ZeroMemory(&ck_icrd, sizeof(MMCKINFO));
	ZeroMemory(&ck_data, sizeof(MMCKINFO));

	icop_data.clear();
	icrd_data.clear();

	read_pos = 0;

	// 基底の初期化
	C_sound_stream_base::init();
}

// ******************************************************
// wave を開く
// ======================================================
bool C_wave_stream::open(CTSTR& file_path, DWORD offset, DWORD size)
{
	close();

	// MMIO を用いてファイルを開く
	h_mmio = mmioOpen((LPTSTR)file_path.c_str(), NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (h_mmio == NULL)	{
		set_last_error(_T("ファイルが開けませんでした。"), _T("open_wave"));
		return false;
	}

	// 先頭チャンクを探す
	if (mmioDescend(h_mmio, &ck_riff, NULL, 0) != 0)	{
		set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
		return false;
	}

	// 見つけたチャンクが RIFF でない、または種類が WAVE でないなら失敗
	if (ck_riff.ckid != FOURCC_RIFF || ck_riff.fccType != mmioFOURCC('W','A','V','E'))	{
		set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
		return false;
	}

	// "RIFF" チャンクから "fmt" チャンクを探す
	ck_fmt.ckid = mmioFOURCC('f','m','t',' ');
	if (mmioDescend(h_mmio, &ck_fmt, &ck_riff, MMIO_FINDCHUNK) != 0)	{
		set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
		return false;
	}

	// "fmt" チャンクのサイズは、少なくとも sizeof(PCMWAVEFORMAT) 以上
	if (ck_fmt.cksize < sizeof(PCMWAVEFORMAT))	{
		set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
		return false;
	}

	// "fmt" チャンクを pcmWaveFormat に読み込む
	PCMWAVEFORMAT pcm_wave_format;
	if (mmioRead(h_mmio, (char *)&pcm_wave_format, sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))	{
		set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
		return false;
	}

	// WAVE_FORMAT_PCM 以外は未対応
	if (pcm_wave_format.wf.wFormatTag != WAVE_FORMAT_PCM)	{
		set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
		return false;
	}

	// "fmt" チャンクから退出
	if (mmioAscend(h_mmio, &ck_fmt, 0) != 0)	{
		set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
		return false;
	}

	// "RIFF" チャンクデータの先頭からタイプのデータ分進んだ先にシークをセット
	if (mmioSeek(h_mmio, ck_riff.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)	{
		set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
		return false;
	}

	// "LIST" チャンクを探す
	ck_list.ckid = mmioFOURCC('L','I','S','T');
	if (mmioDescend(h_mmio, &ck_list, &ck_riff, MMIO_FINDCHUNK) == 0)	{

		// チャンクのリストタイプが "INFO" の場合
		if (ck_list.fccType == mmioFOURCC('I','N','F','O'))	{

			// "LIST" チャンクデータの先頭からタイプのデータ分進んだ先にシークをセット
			if (mmioSeek(h_mmio, ck_list.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)	{
				set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
				return false;
			}

			// "ICOP" チャンクを探す
			ck_icop.ckid = mmioFOURCC('I','C','O','P');
			if (mmioDescend(h_mmio, &ck_icop, &ck_list, MMIO_FINDCHUNK) == 0)	{

				// "ICOP チャンクのデータを読み込む
				if (ck_icop.cksize > 0)	{
					icop_data.resize(ck_icop.cksize);
					if (mmioRead(h_mmio, (char *)icop_data.get(), ck_icop.cksize) != ck_icop.cksize)	{
						set_last_error(_T("ICOP チャンクが不正です。"), _T("open_wave"));
						return false;
					}
				}

				// "ICOP チャンクから退出
				if (mmioAscend(h_mmio, &ck_icop, 0) != 0)	{
					set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
					return false;
				}
			}

			// "LIST" チャンクデータの先頭からタイプのデータ分進んだ先にシークをセット
			if (mmioSeek(h_mmio, ck_list.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)	{
				set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
				return false;
			}

			// "ICRD" チャンクを探す
			ck_icop.ckid = mmioFOURCC('I','C','R','D');
			if (mmioDescend(h_mmio, &ck_icrd, &ck_list, MMIO_FINDCHUNK) == 0)	{

				// "ICRD チャンクのデータを読み込む
				if (ck_icrd.cksize > 0)	{
					icrd_data.resize(ck_icrd.cksize);
					if (mmioRead(h_mmio, (char *)icrd_data.get(), ck_icrd.cksize) != ck_icrd.cksize)	{
						set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
						return false;
					}
				}

				// "ICRD チャンクから退出
				if (mmioAscend(h_mmio, &ck_icrd, 0) != 0)	{
					set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
					return false;
				}
			}
		}

		// "LIST" チャンクから退出
		if (mmioAscend(h_mmio, &ck_list, 0) != 0)	{
			set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
			return false;
		}
	}

	// "RIFF" チャンクデータの先頭から、"WAVE" の４文字進んだ先にシークをセット
	if (mmioSeek(h_mmio, ck_riff.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)	{
		set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
		return false;
	}

	// "data" チャンクを探す
	ck_data.ckid = mmioFOURCC('d','a','t','a');
	if (mmioDescend(h_mmio, &ck_data, &ck_riff, MMIO_FINDCHUNK) != 0)	{
		set_last_error(_T("不正な wav ファイルです。"), _T("open_wave"));
		return false;
	}

	// WAVE の情報を設定
	set_param(pcm_wave_format.wf.nChannels, pcm_wave_format.wBitsPerSample, pcm_wave_format.wf.nSamplesPerSec, ck_data.cksize);

	return true;
}

// ******************************************************
// wav を閉じる
// ======================================================
void C_wave_stream::close()
{
	// MMIO を閉じる
	if (h_mmio)	{
		mmioClose(h_mmio, 0);
		h_mmio = NULL;
	}

	// 初期化
	init();
}

// ******************************************************
// wave の読み込み
//		buf:		バッファへのポインタ
//		size:		読み込むサイズ
//		戻り値：	実際に読み込んだサイズ
// ======================================================
int C_wave_stream::read(BYTE* buf, int size)
{
	if (buf == NULL || h_mmio == NULL)		return 0;

	// 読み込んだバイト数
	int read_size = 0;

	// 現在の情報を読み込む
	MMIOINFO mmio_info;
	if (mmioGetInfo(h_mmio, &mmio_info, 0) != 0)
		return 0;

	// １バイトずつ読み込んでいく
	for (int c = 0; c < size; c ++)	{

		// ピッチエンドの場合
		if (mmio_info.pchNext == mmio_info.pchEndRead)	{
			if (mmioAdvance(h_mmio, &mmio_info, MMIO_READ) != 0)
				return 0;
			if (mmio_info.pchNext == mmio_info.pchEndRead)
				return read_size;
		}

		// １バイトコピー
		*((BYTE *)buf + c) = *((BYTE *)mmio_info.pchNext);
		mmio_info.pchNext ++;
		read_size ++;
	}

	// 情報を更新
	if (mmioSetInfo(h_mmio, &mmio_info, 0) != 0)
		return 0;

	// 読み込み位置の修正
	read_pos += read_size;

	return read_size;
}

// ******************************************************
// 読み込み位置を設定
// ======================================================
bool C_wave_stream::set_read_pos(int pos)
{
	if (h_mmio == NULL)		return false;

	// 範囲補正
	pos = limit(0, pos, get_data_size());
	// "DATA" チャンクデータの先頭からシークをセット
	if (mmioSeek(h_mmio, ck_data.dwDataOffset + pos, SEEK_SET) == -1)
		return false;

	// 読み込み位置の修正
	read_pos = pos;

	return true;
}

// ******************************************************
// 読み込み位置を取得
// ======================================================
int C_wave_stream::get_read_pos()
{
	if (h_mmio == NULL)		return 0;

	// 現在の情報を読み込む
	return read_pos;
}
};
