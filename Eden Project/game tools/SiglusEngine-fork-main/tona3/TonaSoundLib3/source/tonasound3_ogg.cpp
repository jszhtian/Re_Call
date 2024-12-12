#include	"tonasound3_pch.h"
#include	"tonasound3_ogg.h"

#include	<ogg/ogg.h>
#include	<vorbis/vorbisfile.h>

namespace NT3
{

// ****************************************************************
// ogg ���\����
// ================================================================
struct Sogg_param
{
	FILE*	fp;				// �t�@�C���|�C���^
	DWORD	file_offset;	// �t�@�C���I�t�Z�b�g
	DWORD	file_size;		// �t�@�C���T�C�Y
	BYTE	xor;			// XOR
};

// ****************************************************************
// C_ogg_stream �����\����
// ================================================================
struct C_ogg_stream::Simpl
{
	OggVorbis_File	ovf;		// ogg ���
	Sogg_param		ogg_param;	// ogg �p�����[�^
};

// ****************************************************************
// ogg
// ================================================================
C_ogg_stream::C_ogg_stream(BYTE xor)
{
	// �����̍쐬
	impl = BSP<Simpl>(new Simpl);
	impl->ogg_param.xor = xor;			// �Œ�c���������[�`���ŏ��������Ȃ����ƁI

	// ������
	init();
}

C_ogg_stream::~C_ogg_stream()
{
	close();

	// �����̔j��
	impl.reset();
}

// ****************************************************************
// vorbis �p�̃R�[���o�b�N�֐��Q
// ================================================================
size_t ovopcallback_read_func(void* buf, size_t data_size, size_t to_read_cnt, void* param)
{
	Sogg_param* info = (Sogg_param *)param;
	size_t read_cnt = fread(buf, data_size, to_read_cnt, info->fp);
	
	BYTE* p = (BYTE *)buf;
	for (int i = 0; i < (int)(data_size * read_cnt); i++)	{
		*p++ ^= info->xor;
	}

	return read_cnt;
}

int ovopcallback_close_func(void* param)
{
	Sogg_param* info = (Sogg_param *)param;
	return fclose(info->fp) == 0 ? 0 : -1;
}

int ovopcallback_seek_func(void* param, ogg_int64_t offset, int whence)
{
	// �t�@�C���I�t�Z�b�g�����炷
	Sogg_param* info = (Sogg_param *)param;

	switch (whence)	{
		case SEEK_SET:
			return _fseeki64(info->fp, info->file_offset + offset, whence) == 0 ? 0 : -1;
		case SEEK_CUR:
			return _fseeki64(info->fp, offset, whence) == 0 ? 0 : -1;
		case SEEK_END:

			if (info->file_size == 0)	{	// �T�C�Y���w�肳��Ă��Ȃ��ꍇ
				return _fseeki64(info->fp, offset, whence) == 0 ? 0 : -1;
			}
			else	{						// �T�C�Y���w�肳��Ă���ꍇ
				return _fseeki64(info->fp, info->file_offset + info->file_size - offset, SEEK_SET) == 0 ? 0 : -1;
			}
	}
	return -1;
}

long ovopcallback_tell_func(void* param)
{
	// �t�@�C���I�t�Z�b�g�����炷
	Sogg_param* info = (Sogg_param *)param;
	return (long)_ftelli64(info->fp) - info->file_offset;
}

// ****************************************************************
// ogg ������
// ================================================================
void C_ogg_stream::init()
{
	ZeroMemory(&impl->ovf, sizeof(impl->ovf));
	impl->ogg_param.fp = NULL;
	impl->ogg_param.file_offset = 0;
	read_pos = 0;

	// ���̏�����
	C_sound_stream_base::init();
}

// ****************************************************************
// ogg ���J��
// ================================================================
bool C_ogg_stream::open(CTSTR& file_path, DWORD offset, DWORD size)
{
	close();

	// �t�@�C���I�t�Z�b�g���Z�b�g
	impl->ogg_param.file_offset = offset;

	// �t�@�C�����J��
	_tfopen_s(&impl->ogg_param.fp, file_path.c_str(), _T("rb"));
	if (!impl->ogg_param.fp)
		return false;

	// �t�@�C���T�C�Y���擾����
	impl->ogg_param.file_size = size;

	// �t�@�C���I�t�Z�b�g�̈ʒu�ɃV�[�N�Z�b�g
	fseek(impl->ogg_param.fp, impl->ogg_param.file_offset, SEEK_SET);

	// �R�[���o�b�N�֐��̃Z�b�g
	ov_callbacks cb;
	cb.read_func  = ovopcallback_read_func;
	cb.seek_func  = ovopcallback_seek_func;
	cb.close_func = ovopcallback_close_func;
	cb.tell_func  = ovopcallback_tell_func;

	// ogg ���J��
	if (ov_open_callbacks(&impl->ogg_param, &impl->ovf, NULL, 0, cb) < 0)	{
		fclose(impl->ogg_param.fp);	// ���������� fclose �ŕ���K�v������
		init();						// ������
		return false;
	}

	// �V�[�N�\���𒲂ׂ�
	if (ov_seekable(&impl->ovf) == 0)	{
		close();		// ����
		return false;
	}

	// ogg �̏����擾
	vorbis_info ogg_info = *ov_info(&impl->ovf, -1);
	// �T���v�����̎擾
	int sample_cnt = (int)ov_pcm_total(&impl->ovf, -1);
	// wave �����Z�b�g
	set_param(ogg_info.channels, 16, ogg_info.rate, sample_cnt * (16 / 8) * ogg_info.channels);

	return true;
}

// ****************************************************************
// ogg �����
// ================================================================
void C_ogg_stream::close()
{
	// ogg �����
	if (impl->ogg_param.fp)
		ov_clear(&impl->ovf);

	// ����������
	init();
}

// ****************************************************************
// ogg �̓ǂݍ���
// ================================================================
int C_ogg_stream::read(BYTE* buf, int to_read_size)
{
	if (buf == NULL || impl->ogg_param.fp == NULL)
		return 0;

	// �c��o�C�g��
	int rest_size = to_read_size;

	BYTE* p = buf;
	int dummy;

	while (1)	{
		// ogg ��ǂݍ���
		int read_size = ov_read(&impl->ovf, (char *)p, rest_size, 0, 2, 1, &dummy);
		if (read_size == 0)
			break;	// EOF

		// �ǂݍ��ݐ����I
		if (read_size > 0)	{
			p += read_size;
			rest_size -= read_size;
			if (rest_size <= 0)
				break;
		}

		// �ǂݍ��ݎ��s�I
		else if (!read_err_check(read_size))	{
			// �c��o�b�t�@�� 0 �Ŗ��߂�
			if (rest_size > 0)	{
				ZeroMemory(p, rest_size);
			}
			break;
		}
	}

	read_pos += to_read_size - rest_size;
	return to_read_size - rest_size;
}

// ****************************************************************
// �ǂݍ��݈ʒu��ݒ�
// ================================================================
bool C_ogg_stream::set_read_pos(int pos)
{
	if (!impl->ogg_param.fp)		return false;

	// ogg �ł̓T���v���ʒu���w��
	int pos_sample = pos / (get_bit_cnt_per_sample() / 8) / get_channel_cnt();

	// �͈͕␳
	pos_sample = limit(0, pos_sample, get_sample_cnt());

	// �V�[�N
	ov_pcm_seek(&impl->ovf, pos_sample);

	// �ǂݍ��݈ʒu���C��
	read_pos = pos;

	return true;
}

// ****************************************************************
// �ǂݍ��݈ʒu���擾
// ================================================================
int C_ogg_stream::get_read_pos()
{
	if (!impl->ogg_param.fp)		return 0;

	return read_pos;
}

// ****************************************************************
// ogg �ǂݍ��݃G���[�`�F�b�N
// ================================================================
bool C_ogg_stream::read_err_check(int res)
{
	switch (res)	{
		case OV_FALSE:		// -1 �^�łȂ��B�������͗��p�ł���f�[�^�������B
			return true;

		case OV_EOF:		// -2
			return true;

		case OV_HOLE:		// -3 Vorbisfile �̓r�b�g�X�g���[�����Ƀf�[�^�̑�����Ԉ�����f�[�^�ɑ��������B�񕜂͒ʏ펩���I�ɍs���A���̖߂�l�͏�񌹂Ƃ��Ă̖ړI�ɂ̂ݎg����B
			return true;

		case OV_EREAD:		// -128 �f�R�[�h�̂��߂Ɉ��k�f�[�^�����o�������ɓǂݍ��݃G���[���N�������B
			return false;

		case OV_EFAULT:		// -129 �f�R�[�h��Ԃɓ����I�Ȗ������������B���s�͓�������B
			return false;

		case OV_EIMPL:		// -130 �������̋@�\�ł���B
			return true;

		case OV_EINVAL:		// -131 �����Ȉ����ł��邩�A���S�ɏ���������ĂȂ������� libvorbisfile �ɓn�����B
			return false;

		case OV_ENOTVORBIS:	// -132 �^����ꂽ�t�@�C����f�[�^��OggVorbis�f�[�^�ł���ƔF���ł��Ȃ��B
			return false;

		case OV_EBADHEADER:	// -133 �t�@�C����f�[�^��OggVorbis�X�g���[���̂悤�Ɍ����邪�A�Ԉ�����w�b�_����Ǖs�\�ȃw�b�_���܂܂�Ă���B
			return false;

		case OV_EVERSION:	// -134 �^����ꂽ�X�g���[���̃r�b�g�X�g���[���t�H�[�}�b�g�̓T�|�[�g���Ă��Ȃ��B
			return false;

		case OV_ENOTAUDIO:	// -135 
			return true;

		case OV_EBADPACKET:	// -136 
			return true;

		case OV_EBADLINK:	// -137 �^����ꂽ�����N��Vorbis�f�[�^�X�g���[�����ɑ��݂��Ă��邪�A�S�~��ԈႢ�̂����ŉ�ǂł��Ȃ��B
			return false;

		case OV_ENOSEEK:	// -138 �^����ꂽ�X�g���[���͈ړ��s�\�ł���B
			return true;

		default:
			return true;
	}
}

}
