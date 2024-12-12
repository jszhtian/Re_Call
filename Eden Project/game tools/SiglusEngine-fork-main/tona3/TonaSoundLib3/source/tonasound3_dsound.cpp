#include	"tonasound3_pch.h"
#include	"tonasound3_dsound.h"

namespace NT3
{

// ****************************************************************
// �C���X�^���X
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
// DirectSound �̏�����
// ================================================================
bool C_direct_sound::init(HWND h_wnd)
{
	HRESULT hr;

	// DirectSound �C���^�[�t�F�C�X�̎擾
	hr = DirectSoundCreate(NULL, &p_ds, NULL);
	if (FAILED(hr))
	{
		set_last_error_dsound(hr, _T("DirectSoundCreate"));
		return false;
	}

	// DirectSound �̏�����
	hr = p_ds->Initialize(NULL);
	if (FAILED(hr) && hr != DSERR_ALREADYINITIALIZED)
	{
		set_last_error_dsound(hr, _T("IDirectSound::Initialize"));
		return false;
	}

	// �������x����ݒ�
	hr = p_ds->SetCooperativeLevel(h_wnd, DSSCL_PRIORITY);	// �D��
	if (FAILED(hr))
	{
		set_last_error_dsound(hr, _T("IDirectSound::SetCooperativeLevel"));
		return false;
	}

	// �v���C�}���o�b�t�@�̐ݒ�
	DSBUFFERDESC dsb_desc;
	ZeroMemory(&dsb_desc, sizeof(DSBUFFERDESC));
	dsb_desc.dwSize = sizeof(DSBUFFERDESC);
	dsb_desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER;
	dsb_desc.dwBufferBytes = 0;
	dsb_desc.lpwfxFormat = NULL;

	// �v���C�}���o�b�t�@�̍쐬
	hr = p_ds->CreateSoundBuffer(&dsb_desc, &p_dsb_primary, NULL);
	if (FAILED(hr))
	{
		set_last_error_dsound(hr, _T("IDirectSound::CreateSoundBuffer"));
		return false;
	}

	// �v���C�}���o�b�t�@�� wave �t�H�[�}�b�g��ݒ�
	if (!set_state(2, 16, 44100))
		return false;

	return true;
}

// ****************************************************************
// DirectSound �̉��
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
// �X�e�[�^�X��ݒ�
//		channel_cnt:		�`�����l�����B1=���فA2=��ڵ �̂ݑΉ��B
//		bit_cnt_per_sample:	�r�b�g���[�g�B8bit / 16bit �̂ݑΉ��B
//		sample_cnt_for_sec:	�T���v�����O���[�g�B11025kHz / 22050kHz / 44100kHz / 48000kHz �̂ݑΉ��B
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
// �X�e�[�^�X���擾
//		channel_cnt:		�`�����l�����B
//		bit_cnt_per_sample:	�r�b�g���[�g�B
//		sample_cnt_for_sec:	�T���v�����O���[�g�B
// ================================================================
void C_direct_sound::get_state(int* channel_cnt, int* bit_cnt_per_sample, int* sample_cnt_for_sec)
{
	*channel_cnt = m_channel_cnt;
	*bit_cnt_per_sample = m_bit_cnt_per_sample;
	*sample_cnt_for_sec = m_sample_cnt_for_sec;
}

// ****************************************************************
// DirectSound �G���[���b�Z�[�W
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

// �G���[�ԍ����� DirectSound �̃G���[���b�Z�[�W���擾����
TSTR get_dsound_error_msg(HRESULT hr)
{
	TSTR str;

	if (hr == DSERR_NODRIVER)
	{
		str = _T("DSERR_NODRIVER�F���p�\�ȃT�E���h�h���C�o�����݂��܂���B");
	}
	else
	{
		// �G���[��������\�z
		str = _T("hr = ") + tostr(hr, 16) + _T(" (") + get_dsound_hresult_str(hr) + _T(")");
	}

	return str;
}

// �Ō�̃G���[�� DirectSound �ɐݒ肷��
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
		if (!error_str.empty())		error_str += _T("\n");	// ���s
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
		if (!error_str.empty())		error_str += _T("\n");	// ���s
		error_str += dsound_error;
	}

	add_last_error(error_str, error_func);
}

}
