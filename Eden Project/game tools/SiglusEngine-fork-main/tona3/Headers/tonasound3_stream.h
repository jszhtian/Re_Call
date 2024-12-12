#pragma		once

namespace NT3
{

	// ****************************************************************
	// �T�E���h�X�g���[��
	//		�t�@�C���⃁��������T�E���h��ǂݍ��ނ��߂̊��N���X�ł��B
	//		������p�����Ċe��T�E���h�̓ǂݍ��݃N���X������Ă��������B
	// ================================================================
	class C_sound_stream_base
	{
	public:
		C_sound_stream_base();
		virtual	~C_sound_stream_base()	{}

		virtual	bool	open(CTSTR& file_path, DWORD offset = 0, DWORD size = 0)		{	return false;	}	// �J��
		virtual	void	close()															{	return;			}	// ����
		virtual	int		read(BYTE* buf, int size)										{	return 0;		}	// �f�[�^��ǂݍ���
		virtual	bool	set_read_pos(int pos)											{	return false;	}	// �ǂ݂��݈ʒu��ݒ�
		virtual	int		get_read_pos()													{	return 0;		}	// �ǂ݂��݈ʒu���擾
		virtual BYTE*	get_ptr()														{	return NULL;	}	// �T�E���h�ւ̃|�C���^

		bool	is_ready()					{	return data_size > 0;		}	// ��������
		int		get_channel_cnt()			{	return channel_cnt;			}	// �`�����l�������擾
		int		get_bit_cnt_per_sample()	{	return bit_cnt_per_sample;	}	// �r�b�g���[�g���擾
		int		get_sample_cnt_per_sec()	{	return sample_cnt_per_sec;	}	// �T���v�����O���[�g���擾
		int		get_data_size()				{	return data_size;			}	// �f�[�^�T�C�Y���擾
		int		get_fragment_size()			{	return (bit_cnt_per_sample / 8) * channel_cnt;				}	// �t���O�����g�T�C�Y���擾
		int		get_sample_cnt()			{	return data_size / get_fragment_size();						}	// �T���v�������擾
		int		get_length_by_millsecond()	{	return get_sample_cnt() * 1000 / get_sample_cnt_per_sec();	}	// �������~���b�Ŏ擾

	protected:

		void	init();		// ������
		void	set_param(int channel_cnt, int bit_cnt_per_sample, int sample_cnt_per_sec, int data_size);	// �p�����[�^�ݒ�

	private:

		// WAVE ���
		int		channel_cnt;			// ����=1�A��ڵ=2
		int		bit_cnt_per_sample;		// 8bit or 16bit
		int		sample_cnt_per_sec;		// 22050Hz, 44100Hz, ...
		int		data_size;				// �f�[�^�T�C�Y
	};

};
