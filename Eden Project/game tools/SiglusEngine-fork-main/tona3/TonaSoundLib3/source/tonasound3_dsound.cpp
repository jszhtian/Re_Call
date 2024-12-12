#include	"tonasound3_pch.h"
#include	"tonasound3_dsound.h"

namespace NT3
{

// ****************************************************************
// インスタンス
// ================================================================
C_direct_sound	G_ds;

// ****************************************************************
// DirectSound
// ================================================================
C_direct_sound::C_direct_sound()
{
	p_ds = NULL;
	p_dsb_primary = NULL;
}

C_direct_sound::~C_direct_sound()
{
	release();
}

// ****************************************************************
// DirectSound の初期化
// ================================================================
bool C_direct_sound::init(HWND h_wnd)
{
	HRESULT hr;

	// DirectSound インターフェイスの取得
	hr = DirectSoundCreate(NULL, &p_ds, NULL);
	if (FAILED(hr))
	{
		set_last_error_dsound(hr, _T("DirectSoundCreate"));
		return false;
	}

	// DirectSound の初期化
	hr = p_ds->Initialize(NULL);
	if (FAILED(hr) && hr != DSERR_ALREADYINITIALIZED)
	{
		set_last_error_dsound(hr, _T("IDirectSound::Initialize"));
		return false;
	}

	// 協調レベルを設定
	hr = p_ds->SetCooperativeLevel(h_wnd, DSSCL_PRIORITY);	// 優先
	if (FAILED(hr))
	{
		set_last_error_dsound(hr, _T("IDirectSound::SetCooperativeLevel"));
		return false;
	}

	// プライマリバッファの設定
	DSBUFFERDESC dsb_desc;
	ZeroMemory(&dsb_desc, sizeof(DSBUFFERDESC));
	dsb_desc.dwSize = sizeof(DSBUFFERDESC);
	dsb_desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER;
	dsb_desc.dwBufferBytes = 0;
	dsb_desc.lpwfxFormat = NULL;

	// プライマリバッファの作成
	hr = p_ds->CreateSoundBuffer(&dsb_desc, &p_dsb_primary, NULL);
	if (FAILED(hr))
	{
		set_last_error_dsound(hr, _T("IDirectSound::CreateSoundBuffer"));
		return false;
	}

	// プライマリバッファの wave フォーマットを設定
	if (!set_state(2, 16, 44100))
		return false;

	return true;
}

// ****************************************************************
// DirectSound の解放
// ================================================================
void C_direct_sound::release()
{
	if (p_dsb_primary)
	{
		p_dsb_primary->Release();
		p_dsb_primary = NULL;
	}

	if (p_ds)
	{
		p_ds->Release();
		p_ds = NULL;
	}
}

// ****************************************************************
// ステータスを設定
//		channel_cnt:		チャンネル数。1=ﾓﾉﾗﾙ、2=ｽﾃﾚｵ のみ対応。
//		bit_cnt_per_sample:	ビットレート。8bit / 16bit のみ対応。
//		sample_cnt_for_sec:	サンプリングレート。11025kHz / 22050kHz / 44100kHz / 48000kHz のみ対応。
// ================================================================
bool C_direct_sound::set_state(int channel_cnt, int bit_cnt_per_sample, int sample_cnt_for_sec)
{
	HRESULT hr;

	WAVEFORMATEX wf;
	ZeroMemory(&wf, sizeof(WAVEFORMATEX));
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = channel_cnt;
	wf.nSamplesPerSec = sample_cnt_for_sec;
	wf.nBlockAlign = channel_cnt * bit_cnt_per_sample / 8;
	wf.nAvgBytesPerSec = sample_cnt_for_sec * wf.nBlockAlign;
	wf.wBitsPerSample = bit_cnt_per_sample;
	hr = p_dsb_primary->SetFormat(&wf);
	if (FAILED(hr))
	{
		set_last_error_hr(hr, _T("IDirectSoundBuffer::SetFormat"));
		return false;
	}

	m_channel_cnt = channel_cnt;
	m_bit_cnt_per_sample = bit_cnt_per_sample;
	m_sample_cnt_for_sec = sample_cnt_for_sec;

	return true;
}

// ****************************************************************
// ステータスを取得
//		channel_cnt:		チャンネル数。
//		bit_cnt_per_sample:	ビットレート。
//		sample_cnt_for_sec:	サンプリングレート。
// ================================================================
void C_direct_sound::get_state(int* channel_cnt, int* bit_cnt_per_sample, int* sample_cnt_for_sec)
{
	*channel_cnt = m_channel_cnt;
	*bit_cnt_per_sample = m_bit_cnt_per_sample;
	*sample_cnt_for_sec = m_sample_cnt_for_sec;
}

// ****************************************************************
// DirectSound エラーメッセージ
// ================================================================
static TSTR get_dsound_hresult_str(HRESULT hr)
{
	switch (hr)
	{
		// dsound5.h
	case DSERR_ALLOCATED:			return _T("DSERR_ALLOCATED");
	case DSERR_CONTROLUNAVAIL:		return _T("DSERR_CONTROLUNAVAIL");
	case DSERR_INVALIDPARAM:		return _T("DSERR_INVALIDPARAM");
	case DSERR_INVALIDCALL:			return _T("DSERR_INVALIDCALL");
	case DSERR_GENERIC:				return _T("DSERR_GENERIC");
	case DSERR_PRIOLEVELNEEDED:		return _T("DSERR_PRIOLEVELNEEDED");
	case DSERR_OUTOFMEMORY:			return _T("DSERR_OUTOFMEMORY");
	case DSERR_BADFORMAT:			return _T("DSERR_BADFORMAT");
	case DSERR_UNSUPPORTED:			return _T("DSERR_UNSUPPORTED");
	case DSERR_NODRIVER:			return _T("DSERR_NODRIVER");
	case DSERR_ALREADYINITIALIZED:	return _T("DSERR_ALREADYINITIALIZED");
	case DSERR_NOAGGREGATION:		return _T("DSERR_NOAGGREGATION");
	case DSERR_BUFFERLOST:			return _T("DSERR_BUFFERLOST");
	case DSERR_OTHERAPPHASPRIO:		return _T("DSERR_OTHERAPPHASPRIO");
	case DSERR_UNINITIALIZED:		return _T("DSERR_UNINITIALIZED");
	case DSERR_NOINTERFACE:			return _T("DSERR_NOINTERFACE");
	default:						return get_hresult_error_msg(hr);
	}
}

// エラー番号から DirectSound のエラーメッセージを取得する
TSTR get_dsound_error_msg(HRESULT hr)
{
	TSTR str;

	if (hr == DSERR_NODRIVER)
	{
		str = _T("DSERR_NODRIVER：利用可能なサウンドドライバが存在しません。");
	}
	else
	{
		// エラー文字列を構築
		str = _T("hr = ") + tostr(hr, 16) + _T(" (") + get_dsound_hresult_str(hr) + _T(")");
	}

	return str;
}

// 最後のエラーを DirectSound に設定する
void set_last_error_dsound(HRESULT hr, TSTR error_func)
{
	TSTR error_str = get_dsound_error_msg(hr);
	set_last_error(error_str, error_func);
}

void set_last_error_dsound(HRESULT hr, TSTR error_str, TSTR error_func)
{
	TSTR dsound_error = get_dsound_error_msg(hr);
	if (!dsound_error.empty())
	{
		if (!error_str.empty())		error_str += _T("\n");	// 改行
		error_str += dsound_error;
	}

	set_last_error(error_str, error_func);
}

void add_last_error_dsound(HRESULT hr, const TSTR& error_func)
{
	TSTR error_str = get_dsound_error_msg(hr);
	add_last_error(error_str, error_func);
}

void add_last_error_dsound(HRESULT hr, TSTR& error_str, const TSTR& error_func)
{
	TSTR dsound_error = get_dsound_error_msg(hr);
	if (!dsound_error.empty())
	{
		if (!error_str.empty())		error_str += _T("\n");	// 改行
		error_str += dsound_error;
	}

	add_last_error(error_str, error_func);
}

}
