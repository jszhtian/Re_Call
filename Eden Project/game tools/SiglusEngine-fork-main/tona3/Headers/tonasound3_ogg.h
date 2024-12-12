#pragma		once

#include	"tonasound3_stream.h"

namespace NT3
{

	// ****************************************************************
	// ogg �X�g���[��
	//		�T�E���h�X�g���[�����p���B
	//		�t�@�C������X�g���[�~���O�ǂݍ��݂��o���܂��B
	// ================================================================
	class C_ogg_stream : public C_sound_stream_base
	{
	public:
		C_ogg_stream(BYTE xor);
		~C_ogg_stream();

		// ��{�֐�
		bool	open(CTSTR& file_path, DWORD offset = 0, DWORD size = 0);	// �J��
		void	close();													// ����
		int		read(BYTE* buf, int size);									// �f�[�^��ǂݍ���
		bool	set_read_pos(int pos);										// �ǂ݂��݈ʒu��ݒ�
		int		get_read_pos();												// �ǂ݂��݈ʒu���擾

	private:

		void	init();			// ������

		struct		Simpl;
		BSP<Simpl>	impl;

		// ogg ���
		int			read_pos;	// �ǂ݂��݈ʒu

		bool	read_err_check(int res);
	};

}
