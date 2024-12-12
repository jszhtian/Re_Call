#pragma		once

#include	"tonasound3_dsound.h"
#include	"tonasound3_sound.h"
#include	"tonasound3_semaphore.h"

// �Z�}�t�H�`�F�b�N�e�X�g�B�K�� 0 �ɂ��Ă��������B�ڍׂ͉��ɁB
#define		SEMAPHORE_TEST		0

// ****************************************************************
//	��CHECK_SEMAPHORE �ɂ���
//		�S�Ă� public �ȃ����o�֐��́A�Z�}�t�H�`�F�b�N���s�����ǂ����� CHECK_SEMAPHORE �Ŏw��ł���
//		�O������Ăяo���Ƃ��́A�K�� CHECK_SEMAPHORE = true�i�f�t�H���g�l�j�ŌĂяo��
//		��������Ăяo���Ƃ��́A�K�� CHECK_SEMAPHORE = false �ŌĂяo��
//	��SEMAPHORE_TEST �ɂ���
//		�ʏ�� 0�B
//		1 �ɂ��邱�ƂŁA��������̌Ăяo���Ɋւ��āACHECK_SEMAPHORE �̓���Y����`�F�b�N�ł���
//		�i����Y��Ă���ꍇ�A�R���p�C���G���[�ƂȂ�j
// ================================================================

#if	SEMAPHORE_TEST
#define		SC		bool CHECK_SEMAPHORE
#else
#define		SC		bool CHECK_SEMAPHORE = true
#endif

// �Z�}�t�H�}�N��
#define		SEMAPHORE		C_semaphore_checker semaphore_checker(CHECK_SEMAPHORE ? m_h_semaphore : NULL)

namespace NT3
{

	const int SOUND_END_POS_AUTO = -1;

	// ****************************************************************
	// �v���C���[
	// ================================================================
	class C_player
	{
	public:
		C_player();
		~C_player();

		bool	init();												// ������
		bool	release(SC);										// ����i�Ō�Ɉ�x�����Ăяo���Ă��������j
		bool	release_sound(SC);									// �T�E���h�̉���i�Đ����~�߂�Ƃ��ȂǂɌĂяo���Ă��������j
		bool	set_stream(BSP<C_sound_stream_base> stream, SC);	// �X�g���[�����Z�b�g

		C_sound_stream_base* get_stream(SC)		{	SEMAPHORE;	return m_stream.get();	}

		bool	play(SC);					// �Đ�
		bool	ready(SC);					// �����̂�
		bool	stop(SC);					// ��~�������߂�
		bool	pause(SC);					// �ꎞ��~
		bool	fade_in(int time, SC);		// �t�F�[�h�C��
		bool	fade_out_and_pause(int time, SC);		// �t�F�[�h�A�E�g�i�I����Ɉꎞ��~�j
		bool	fade_out_and_release(int time, SC);		// �t�F�[�h�A�E�g�i�I����ɉ���j

		bool	is_ready(SC)					{	SEMAPHORE;	return m_ds_buffer != NULL;				}	// ��������
		bool	is_playing(SC)					{	SEMAPHORE;	return m_playing;						}	// �Đ����𔻒�i�ꎞ��~�A�t�F�[�h�A�E�g�����܂ށj
		bool	is_pausing(SC)					{	SEMAPHORE;	return m_pause_flag;					}	// �ꎞ��~���𔻒�
		bool	is_fade_out(SC)					{	SEMAPHORE;	return m_fade_out_flag;					}	// �t�F�[�h�A�E�g�𔻒�

		int		get_channel_cnt(SC)				{	SEMAPHORE;	return m_stream ? m_stream->get_channel_cnt() : 0;			}
		int		get_bit_cnt_per_sample(SC)		{	SEMAPHORE;	return m_stream ? m_stream->get_bit_cnt_per_sample() : 0;	}
		int		get_sample_cnt_per_sec(SC)		{	SEMAPHORE;	return m_stream ? m_stream->get_sample_cnt_per_sec() : 0;	}
		int		get_data_size(SC)				{	SEMAPHORE;	return m_stream ? m_stream->get_data_size() : 0;			}
		int		get_fragment_size(SC)			{	SEMAPHORE;	return m_stream ? m_stream->get_fragment_size() : 0;		}
		int		get_sample_cnt(SC)				{	SEMAPHORE;	return m_stream ? m_stream->get_sample_cnt() : 0;			}
		int		get_length_by_millsecond(SC)	{	SEMAPHORE;	return m_stream ? m_stream->get_length_by_millsecond() : 0;	}

		void	set_range(int start_pos_sample, int end_pos_sample, int restart_pos_sample, SC);		// �Đ��͈͂�ݒ�
		void	set_loop(bool loop, SC);							// ���[�v�̐ݒ�
		void	set_volume(int channel, int volume, int time, SC);	// �{�����[����ݒ�
		int		get_volume(int channel, SC);						// �{�����[�����擾
		int		get_total_volume(SC);								// ���v�{�����[�����擾
		void	set_play_pos_by_sample(int sample_no, SC);			// �v���C�ʒu��ݒ�i�T���v���ʒu�Ŏw��j
		int		get_play_pos_by_sample(SC);							// �v���C�ʒu���擾�i�T���v���ʒu�Ŏ擾�j
		void	set_play_pos_by_millsecond(int mills, SC);			// �v���C�ʒu��ݒ�i�ؕb�Ŏw��j
		int		get_play_pos_by_millsecond(SC);						// �v���C�ʒu���擾�i�ؕb�Ŏ擾�j

		bool	frame(SC);	// �t���[������

		enum
		{
			VOLUME_MIN = 0,
			VOLUME_MAX = 255,
			VOLUME_CHANNEL_CNT = 8,
		};

	private:

		void	init_param();			// �p�����[�^��������
		void	release_player();		// �v���C���[�����
		void	release_stream();		// �X�g���[�������
		bool	create_player();		// �v���C���[�̍쐬

		bool	read(BYTE* buf, int size, int* read_size);	// �T�E���h��ǂ݂���
		bool	fill_buffer();								// �Đ��o�b�t�@�𖄂߂�
		bool	block_copy(int block_no);					// �Đ��o�b�t�@�𖄂߂�F�u���b�N�w��
		bool	block_copy(BYTE* data_ptr, int data_size);	// �Đ��o�b�t�@�𖄂߂�F�̈�w��

		bool	set_play_pos(int pos);		// �v���C�ʒu��ݒ�
		int		get_play_pos();				// �v���C�ʒu���擾
		bool	update_state();				// �X�e�[�^�X�̍X�V
		bool	update_volume();			// �{�����[���̍X�V

		HANDLE	m_h_semaphore;				// �Z�}�t�H

		typedef LPDIRECTSOUNDBUFFER		LPDSB;
		LPDSB	m_ds_buffer;				// �T�E���h�o�b�t�@
		DWORD	m_ds_buffer_size;			// �T�E���h�o�b�t�@�̃T�C�Y
		int		m_block_size;				// �u���b�N�T�C�Y
		int		m_block_cnt;				// �u���b�N��
		int		m_filled_block_cnt;			// ���߂��u���b�N��
		int		m_play_pos_on_buffer_last;	// �o�b�t�@��ł̍Đ��ʒu�i�O��v�����j
		int		m_play_pos_on_sound_last;	// �T�E���h��ł̍Đ��ʒu�i�O��v�����j
		int		m_play_start_time;			// �Đ����J�n��������
		BYTE	m_last_sample[4];			// �Ō�ɍĐ��������i�Đ��I����͂��̒l�Ŗ��߂�j

		bool	m_loop;						// ���[�v�t���O
		int		m_play_start_pos;			// �v���C�J�n�ʒu�i�o�C�g�P�ʁj
		int		m_play_end_pos;				// �v���C�I���ʒu�i�o�C�g�P�ʁj
		int		m_play_restart_pos;			// �v���C�ĊJ�ʒu�i�o�C�g�P�ʁj

		bool	m_playing;					// �v���C���t���O
		bool	m_pause_flag;				// �ꎞ��~��
		bool	m_fade_in_flag;				// �t�F�[�h�C����
		DWORD	m_fade_in_start_time;		// �t�F�[�h�C���J�n����
		int		m_fade_in_time_len;			// �t�F�[�h�C���ɂ����鎞��
		int		m_fade_in_start_value;		// �t�F�[�h�C���J�n�l
		int		m_fade_in_cur_value;		// �t�F�[�h�C�����ݒl
		bool	m_fade_out_flag;			// �t�F�[�h�A�E�g��
		bool	m_fade_out_no_release;		// �t�F�[�h�A�E�g��
		DWORD	m_fade_out_start_time;		// �t�F�[�h�A�E�g�J�n����
		int		m_fade_out_time_len;		// �t�F�[�h�A�E�g�ɂ����鎞��
		int		m_fade_out_start_value;		// �t�F�[�h�A�E�g�J�n�l
		int		m_fade_out_cur_value;		// �t�F�[�h�A�E�g���ݒl

		struct	S_volume_channel
		{
			int		volume;				// �{�����[���iVOL_MIN �` VOL_MAX�j
			bool	fade_ing;			// �{�����[���t�F�[�h��
			DWORD	fade_start_time;	// �{�����[���t�F�[�h�J�n����
			int		fade_end_time;		// �{�����[���t�F�[�h�I������
			int		fade_start_value;	// �{�����[���t�F�[�h�J�n�l
			int		fade_end_value;		// �{�����[���t�F�[�h�I���l
		}	volume_channel[VOLUME_CHANNEL_CNT];

		BSP<C_sound_stream_base>	m_stream;
	};


	// ****************************************************************
	// �v���C�}�l�[�W��
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
		PLAYERLIST	player_list;			// �v���C���[���X�g

		void		thread_main();
		HANDLE		h_thread;				// �X���b�h�n���h��
		DWORD		thread_id;				// �X���b�h�h�c
		HANDLE		h_list_semaphore;		// �o�b�t�@���X�g�p�̃Z�}�t�H

		static DWORD WINAPI thread_func(LPVOID pv_ref);
		bool		stop_thread;			// �X���b�h���~�߂�
	};

}
