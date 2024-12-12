#pragma		once

#include	"tonasound3_stream.h"

namespace NT3
{

	// ****************************************************************
	// wave �w�b�_
	// ================================================================
	struct wave_header
	{
		BYTE	ck_riff[4];
		DWORD	riff_size;
		BYTE	riff_type[4];
		BYTE	ck_fmt[4];
		DWORD	fmt_size;
		WORD	fmt_id;
		WORD	channel_cnt;
		DWORD	sample_cnt_per_sec;
		DWORD	byte_cnt_per_sec;
		WORD	block_size;
		WORD	bit_cnt_per_sample;
		BYTE	ck_data[4];
		DWORD	data_size;
	};

	// ****************************************************************
	// wave �X�g���[��
	//		�T�E���h�X�g���[�����p���B
	//		�t�@�C������X�g���[�~���O�ǂݍ��݂��o���܂��B
	// ================================================================
	class C_wave_stream : public C_sound_stream_base
	{
	public:
		C_wave_stream();
		~C_wave_stream();

		// ��{�֐�
		bool	open(CTSTR& file_path, DWORD offset = 0, DWORD size = 0);		// �J��
		void	close();										// ����
		int		read(BYTE* buf, int size);						// �f�[�^��ǂݍ���
		bool	set_read_pos(int pos);							// �ǂ݂��݈ʒu��ݒ�
		int		get_read_pos();									// �ǂ݂��݈ʒu���擾

		// �����擾
		BYTE*	get_icop_ptr()		{	return icop_data.get();			}	// "ICOP" �`�����N�f�[�^
		int		get_icop_size()		{	return (int)icop_data.size();	}	// "ICOP" �`�����N�f�[�^�T�C�Y
		BYTE*	get_icrd_ptr()		{	return icrd_data.get();			}	// "ICRD" �`�����N�f�[�^
		int		get_icrd_size()		{	return (int)icrd_data.size();	}	// "ICRD" �`�����N�f�[�^�T�C�Y

	private:

		void	init();		// ������

		HMMIO		h_mmio;			// MMIO �n���h��
		MMCKINFO	ck_riff;		// "RIFF" �`�����N
		MMCKINFO	ck_fmt;			// "fmt" �`�����N
		MMCKINFO	ck_list;		// "LIST" �`�����N
		MMCKINFO	ck_icop;		// "ICOP" �`�����N
		MMCKINFO	ck_icrd;		// "ICRD" �`�����N
		MMCKINFO	ck_data;		// "data" �`�����N

		BUFFER		icop_data;		// ICOP �`�����N�̃f�[�^
		BUFFER		icrd_data;		// ICRD �`�����N�̃f�[�^
		int			read_pos;		// �ǂ݂��݈ʒu
	};
};
