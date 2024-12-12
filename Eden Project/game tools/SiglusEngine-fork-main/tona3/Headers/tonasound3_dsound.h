#pragma		once

namespace NT3
{

// ****************************************************************
// DirectSound
//		������ɂ�� DirectSound5 ���g���Ă��܂��B
//		6 �ȍ~�ō\���̂̃T�C�Y���ς�������߁A�ꕔ�̃T�E���h�J�[�h�œ����Ȃ��\�������邽�߂ł��B
// ================================================================
class C_direct_sound
{
public:
	C_direct_sound();
	~C_direct_sound();

	LPDIRECTSOUND	operator ->()	{	return p_ds;			}
	bool			is_ready()		{	return p_ds != NULL;	}	// ��������

	bool	init(HWND h_wnd);	// ������
	void	release();			// ���
	bool	set_state(int channel_cnt, int bit_cnt_per_sample, int sample_cnt_for_sec);		// �X�e�[�g��ݒ�
	void	get_state(int* channel_cnt, int* bit_cnt_per_sample, int* sample_cnt_for_sec);	// �X�e�[�g���擾

private:
	LPDIRECTSOUND			p_ds;			// �_�C���N�g�T�E���h
	LPDIRECTSOUNDBUFFER		p_dsb_primary;	// �v���C�}���o�b�t�@

	int		m_channel_cnt;
	int		m_bit_cnt_per_sample;
	int		m_sample_cnt_for_sec;
};

extern	C_direct_sound	G_ds;

// ****************************************************************
// �G���[�֐�
// ================================================================

// �G���[�ԍ����� Direct3D �̃G���[���b�Z�[�W���擾����
TSTR get_dsound_error_msg(HRESULT hr);

// �Ō�̃G���[��ݒ�
void set_last_error_dsound(HRESULT hr, TSTR error_func);
void set_last_error_dsound(HRESULT hr, TSTR str, TSTR error_func);

void add_last_error_dsound(HRESULT hr, const TSTR& error_func);
void add_last_error_dsound(HRESULT hr, TSTR& error_str, const TSTR& error_func);

};
