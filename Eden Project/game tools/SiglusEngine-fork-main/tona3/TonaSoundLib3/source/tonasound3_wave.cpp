#include	"tonasound3_pch.h"
#include	"tonasound3_wave.h"

namespace NT3
{

// ******************************************************
// wave �X�g���[��
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
// wave ������
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

	// ���̏�����
	C_sound_stream_base::init();
}

// ******************************************************
// wave ���J��
// ======================================================
bool C_wave_stream::open(CTSTR& file_path, DWORD offset, DWORD size)
{
	close();

	// MMIO ��p���ăt�@�C�����J��
	h_mmio = mmioOpen((LPTSTR)file_path.c_str(), NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (h_mmio == NULL)	{
		set_last_error(_T("�t�@�C�����J���܂���ł����B"), _T("open_wave"));
		return false;
	}

	// �擪�`�����N��T��
	if (mmioDescend(h_mmio, &ck_riff, NULL, 0) != 0)	{
		set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
		return false;
	}

	// �������`�����N�� RIFF �łȂ��A�܂��͎�ނ� WAVE �łȂ��Ȃ玸�s
	if (ck_riff.ckid != FOURCC_RIFF || ck_riff.fccType != mmioFOURCC('W','A','V','E'))	{
		set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
		return false;
	}

	// "RIFF" �`�����N���� "fmt" �`�����N��T��
	ck_fmt.ckid = mmioFOURCC('f','m','t',' ');
	if (mmioDescend(h_mmio, &ck_fmt, &ck_riff, MMIO_FINDCHUNK) != 0)	{
		set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
		return false;
	}

	// "fmt" �`�����N�̃T�C�Y�́A���Ȃ��Ƃ� sizeof(PCMWAVEFORMAT) �ȏ�
	if (ck_fmt.cksize < sizeof(PCMWAVEFORMAT))	{
		set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
		return false;
	}

	// "fmt" �`�����N�� pcmWaveFormat �ɓǂݍ���
	PCMWAVEFORMAT pcm_wave_format;
	if (mmioRead(h_mmio, (char *)&pcm_wave_format, sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))	{
		set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
		return false;
	}

	// WAVE_FORMAT_PCM �ȊO�͖��Ή�
	if (pcm_wave_format.wf.wFormatTag != WAVE_FORMAT_PCM)	{
		set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
		return false;
	}

	// "fmt" �`�����N����ޏo
	if (mmioAscend(h_mmio, &ck_fmt, 0) != 0)	{
		set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
		return false;
	}

	// "RIFF" �`�����N�f�[�^�̐擪����^�C�v�̃f�[�^���i�񂾐�ɃV�[�N���Z�b�g
	if (mmioSeek(h_mmio, ck_riff.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)	{
		set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
		return false;
	}

	// "LIST" �`�����N��T��
	ck_list.ckid = mmioFOURCC('L','I','S','T');
	if (mmioDescend(h_mmio, &ck_list, &ck_riff, MMIO_FINDCHUNK) == 0)	{

		// �`�����N�̃��X�g�^�C�v�� "INFO" �̏ꍇ
		if (ck_list.fccType == mmioFOURCC('I','N','F','O'))	{

			// "LIST" �`�����N�f�[�^�̐擪����^�C�v�̃f�[�^���i�񂾐�ɃV�[�N���Z�b�g
			if (mmioSeek(h_mmio, ck_list.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)	{
				set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
				return false;
			}

			// "ICOP" �`�����N��T��
			ck_icop.ckid = mmioFOURCC('I','C','O','P');
			if (mmioDescend(h_mmio, &ck_icop, &ck_list, MMIO_FINDCHUNK) == 0)	{

				// "ICOP �`�����N�̃f�[�^��ǂݍ���
				if (ck_icop.cksize > 0)	{
					icop_data.resize(ck_icop.cksize);
					if (mmioRead(h_mmio, (char *)icop_data.get(), ck_icop.cksize) != ck_icop.cksize)	{
						set_last_error(_T("ICOP �`�����N���s���ł��B"), _T("open_wave"));
						return false;
					}
				}

				// "ICOP �`�����N����ޏo
				if (mmioAscend(h_mmio, &ck_icop, 0) != 0)	{
					set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
					return false;
				}
			}

			// "LIST" �`�����N�f�[�^�̐擪����^�C�v�̃f�[�^���i�񂾐�ɃV�[�N���Z�b�g
			if (mmioSeek(h_mmio, ck_list.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)	{
				set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
				return false;
			}

			// "ICRD" �`�����N��T��
			ck_icop.ckid = mmioFOURCC('I','C','R','D');
			if (mmioDescend(h_mmio, &ck_icrd, &ck_list, MMIO_FINDCHUNK) == 0)	{

				// "ICRD �`�����N�̃f�[�^��ǂݍ���
				if (ck_icrd.cksize > 0)	{
					icrd_data.resize(ck_icrd.cksize);
					if (mmioRead(h_mmio, (char *)icrd_data.get(), ck_icrd.cksize) != ck_icrd.cksize)	{
						set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
						return false;
					}
				}

				// "ICRD �`�����N����ޏo
				if (mmioAscend(h_mmio, &ck_icrd, 0) != 0)	{
					set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
					return false;
				}
			}
		}

		// "LIST" �`�����N����ޏo
		if (mmioAscend(h_mmio, &ck_list, 0) != 0)	{
			set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
			return false;
		}
	}

	// "RIFF" �`�����N�f�[�^�̐擪����A"WAVE" �̂S�����i�񂾐�ɃV�[�N���Z�b�g
	if (mmioSeek(h_mmio, ck_riff.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)	{
		set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
		return false;
	}

	// "data" �`�����N��T��
	ck_data.ckid = mmioFOURCC('d','a','t','a');
	if (mmioDescend(h_mmio, &ck_data, &ck_riff, MMIO_FINDCHUNK) != 0)	{
		set_last_error(_T("�s���� wav �t�@�C���ł��B"), _T("open_wave"));
		return false;
	}

	// WAVE �̏���ݒ�
	set_param(pcm_wave_format.wf.nChannels, pcm_wave_format.wBitsPerSample, pcm_wave_format.wf.nSamplesPerSec, ck_data.cksize);

	return true;
}

// ******************************************************
// wav �����
// ======================================================
void C_wave_stream::close()
{
	// MMIO �����
	if (h_mmio)	{
		mmioClose(h_mmio, 0);
		h_mmio = NULL;
	}

	// ������
	init();
}

// ******************************************************
// wave �̓ǂݍ���
//		buf:		�o�b�t�@�ւ̃|�C���^
//		size:		�ǂݍ��ރT�C�Y
//		�߂�l�F	���ۂɓǂݍ��񂾃T�C�Y
// ======================================================
int C_wave_stream::read(BYTE* buf, int size)
{
	if (buf == NULL || h_mmio == NULL)		return 0;

	// �ǂݍ��񂾃o�C�g��
	int read_size = 0;

	// ���݂̏���ǂݍ���
	MMIOINFO mmio_info;
	if (mmioGetInfo(h_mmio, &mmio_info, 0) != 0)
		return 0;

	// �P�o�C�g���ǂݍ���ł���
	for (int c = 0; c < size; c ++)	{

		// �s�b�`�G���h�̏ꍇ
		if (mmio_info.pchNext == mmio_info.pchEndRead)	{
			if (mmioAdvance(h_mmio, &mmio_info, MMIO_READ) != 0)
				return 0;
			if (mmio_info.pchNext == mmio_info.pchEndRead)
				return read_size;
		}

		// �P�o�C�g�R�s�[
		*((BYTE *)buf + c) = *((BYTE *)mmio_info.pchNext);
		mmio_info.pchNext ++;
		read_size ++;
	}

	// �����X�V
	if (mmioSetInfo(h_mmio, &mmio_info, 0) != 0)
		return 0;

	// �ǂݍ��݈ʒu�̏C��
	read_pos += read_size;

	return read_size;
}

// ******************************************************
// �ǂݍ��݈ʒu��ݒ�
// ======================================================
bool C_wave_stream::set_read_pos(int pos)
{
	if (h_mmio == NULL)		return false;

	// �͈͕␳
	pos = limit(0, pos, get_data_size());
	// "DATA" �`�����N�f�[�^�̐擪����V�[�N���Z�b�g
	if (mmioSeek(h_mmio, ck_data.dwDataOffset + pos, SEEK_SET) == -1)
		return false;

	// �ǂݍ��݈ʒu�̏C��
	read_pos = pos;

	return true;
}

// ******************************************************
// �ǂݍ��݈ʒu���擾
// ======================================================
int C_wave_stream::get_read_pos()
{
	if (h_mmio == NULL)		return 0;

	// ���݂̏���ǂݍ���
	return read_pos;
}
};
