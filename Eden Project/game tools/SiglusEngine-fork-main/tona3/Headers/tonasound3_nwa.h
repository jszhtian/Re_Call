#pragma		once

#include	"tonasound3_stream.h"

namespace NT3
{

	// ****************************************************************
	// nwa �X�g���[��
	//		�T�E���h�X�g���[�����p���B
	//		�t�@�C������X�g���[�~���O�ǂݍ��݂��o���܂��B
	// ================================================================
	class C_nwa_stream : public C_sound_stream_base
	{
	public:
		C_nwa_stream();
		~C_nwa_stream();

		// ��{�֐�
		bool	open(CTSTR& file_path, DWORD offset = 0, DWORD size = 0);		// �J��
		void	close();										// ����
		int		read(BYTE* buf, int size);						// �f�[�^��ǂݍ���
		bool	set_read_pos(int pos);							// �ǂ݂��݈ʒu��ݒ�
		int		get_read_pos();									// �ǂ݂��݈ʒu���擾

		// ����
		struct		Simpl;
		BSP<Simpl>	impl;

	};

}
