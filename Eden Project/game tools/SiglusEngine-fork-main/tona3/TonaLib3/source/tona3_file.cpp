#include	"tona3_pch.h"
#include	"tona3_file.h"
#include	"tona3_util.h"
#include	"tona3_app.h"
#include	"tona3_error.h"

namespace NT3
{

// ****************************************************************
// fopen �̃��b�p�[
// ================================================================
//		�߂�l�F���� = true�A���s = false
// ================================================================
bool file_open(FILE** fp, const TSTR& file_path, const TSTR& mode)
{
	if (fp == NULL)	{
		set_last_error_invalid(_T("file_open"));	// �����ȃp�����[�^
		return false;
	}

	// fopen
	int error_no = _tfopen_s(fp, file_path.c_str(), mode.c_str());
	if (error_no != 0)	{
		set_last_error_crt(_T("fopen(\"") + mode + _T("\")"));		// CRT �G���[
		return false;
	}

	return true;
}

// ****************************************************************
// fclose �̃��b�p�[
// ================================================================
//		�߂�l�F���� = true�A���s = false
// ================================================================
bool file_close(FILE* fp)
{
	if (fp == NULL)
		return true;	// ����

	// fclose
	int res = fclose(fp);
	if (res == EOF)	{
		set_last_error_crt(_T("fclose"));	// CRT �G���[�i���擾�ł��Ȃ��I�H�v�����I�j
		return false;
	}

	return true;
}

// ****************************************************************
// fread �̃��b�p�[
// ================================================================
//		�߂�l�F�ǂݍ��񂾃o�C�g��
// ================================================================
int file_read(void* dst, int size, FILE* fp)
{
	clear_last_error();		// �G���[���N���A����

	if (fp == NULL)	{
		set_last_error(_T("�t�@�C�����J����Ă��܂���B"), _T("file_read"));
		return 0;
	}
	if (dst == NULL)	{
		set_last_error_invalid(_T("file_read"));	// �����ȃp�����[�^
		return 0;
	}
	if (size < 0)	{
		set_last_error_invalid(_T("file_read"));	// �����ȃp�����[�^
		return 0;
	}

	// fread
	int read_size = (int)fread(dst, 1, size, fp);
	if (read_size < size)	{
		if (ferror(fp))	{
			set_last_error_crt(_T("fread"));		// CRT �G���[
		}
	}
	
	return read_size;
}

// ****************************************************************
// fwrite �̃��b�p�[
// ================================================================
//		�߂�l�F�������񂾃o�C�g��
// ================================================================
int file_write(const void* src, int size, FILE* fp)
{
	if (fp == NULL)	{
		set_last_error(_T("�t�@�C�����J����Ă��܂���B"), _T("file_write"));
		return 0;
	}
	if (src == NULL)	{
		set_last_error_invalid(_T("file_write"));	// �����ȃp�����[�^
		return 0;
	}
	if (size < 0)	{
		set_last_error_invalid(_T("file_write"));	// �����ȃp�����[�^
		return 0;
	}

	// fwrite
	int write_size = (int)fwrite(src, 1, size, fp);
	if (write_size < size)	{
		set_last_error_crt(_T("fwrite"));			// CRT �G���[
	}

	return write_size;
}

// ****************************************************************
// fseek �̃��b�p�[
// ================================================================
//		�߂�l�F���� = true�A���s = false
// ================================================================
bool file_seek(FILE* fp, __int64 offset, int origin)
{
	if (fp == NULL)	{
		set_last_error(_T("�t�@�C�����J����Ă��܂���B"), _T("file_seek"));
		return false;
	}

	int res = _fseeki64(fp, offset, origin);
	if (res != 0)	{
		set_last_error_crt(_T("fseek"));			// CRT �G���[
		return false;
	}

	return true;
}

// ****************************************************************
// ftell �̃��b�p�[
// ================================================================
//		�߂�l�F���� = true�A���s = false
// ================================================================
__int64 file_tell(FILE* fp)
{
	clear_last_error();		// �G���[���N���A����

	if (fp == NULL)	{
		set_last_error(_T("�t�@�C�����J����Ă��܂���B"), _T("file_tell"));
		return 0;
	}

	return _ftelli64(fp);
}

// ****************************************************************
// feof �̃��b�p�[
// ================================================================
//		�߂�l�F�I�[ = true�A�I�[�łȂ��܂��̓G���[ = false
// ================================================================
bool file_eof(FILE* fp)
{
	clear_last_error();		// �G���[���N���A����

	if (fp == NULL)	{
		set_last_error(_T("�t�@�C�����J����Ă��܂���B"), _T("file_eof"));	// �����ȃp�����[�^
		return false;
	}

	// feof
	if (!feof(fp))	{
		return false;	// eof �ł͂Ȃ��i�����j
	}
	else	{
		return true;	// eof �ł���i�����j
	}
}

// ****************************************************************
// �t�@�C���F�R���X�g���N�^
// ================================================================
C_file::C_file()
{
	fp = NULL;
	size = 0;
}

// ****************************************************************
// �t�@�C���F�f�X�g���N�^
// ================================================================
C_file::~C_file()
{
	close();
}

// ****************************************************************
// �t�@�C���F�t�@�C�����J��
// ================================================================
bool C_file::open(CTSTR& file_path, CTSTR& mode)
{
	// �t�@�C�������
	if (!close())
		return false;

	// �t�@�C�����J��
	if (!file_open(&fp, file_path, mode))
		return false;

	// �t�@�C���ʒu���I�[��
	if (!file_seek(fp, 0, SEEK_END))
		return false;

	// �t�@�C���T�C�Y���擾
	size = file_tell(fp);

	// �t�@�C���ʒu�����ɖ߂�
	if (!file_seek(fp, 0, SEEK_SET))
		return false;

	return true;
}

// ****************************************************************
// �t�@�C���F�t�@�C�������
// ================================================================
bool C_file::close()
{
	// �t�@�C�������
	if (!file_close(fp))
		return false;

	fp = NULL;
	size = 0;

	return true;
}

// ****************************************************************
// �t�@�C���F�ǂݍ���
// ================================================================
int C_file::read(void* buf, int size)
{
	// �t�@�C����ǂݍ���
	return file_read(buf, size, fp);
}

bool C_file::read_in_MB_with_len(TSTR& t_str)
{
	t_str.clear();

	int read_size;

	// ������ǂݍ���
	int len = 0;
	read_size = read(&len, sizeof(int));
	if (read_size < sizeof(int))	{
		set_last_error(_T("������̒������擾�ł��܂���ł����B"), _T("read_in_MB_with_len"));
		return false;
	}

	// �����O�̏ꍇ�͋󕶎����Ԃ��ďI��
	if (len <= 0)	{
		t_str = _T("");
		return true;
	}

	// ���������������ǂݍ���
	ARRAY<char> mb_str(len);
	read_size = read(mb_str.get(), len);
	if (read_size < len)	{
		set_last_error(_T("������̒������s���ł��B"), _T("read_in_MB_with_len"));
		return false;
	}

	// TSTR �ɕϊ�
	t_str = MBSTR_to_TSTR(mb_str.get(), len);

	return true;
}

// ****************************************************************
// �t�@�C���F��������
// ================================================================
int C_file::write(const void* buf, int size)
{
	return file_write(buf, size, fp);
}

int C_file::write(CWSTR& w_str)
{
	return write(w_str.c_str(), (int)w_str.size());
}

int C_file::write(CMBSTR& mb_str)
{
	return write(mb_str.c_str(), (int)mb_str.size());
}

void C_file::write_in_MB(CTSTR& t_str)
{
	MBSTR mb_str = TSTR_to_MBSTR(t_str);
	write(mb_str.c_str(), (int)mb_str.size());
}

void C_file::write_in_UTF8(CTSTR& t_str)
{
	MBSTR utf8_str = TSTR_to_UTF8(t_str);
	write(utf8_str.c_str(), (int)utf8_str.size());
}

void C_file::write_in_MB_with_len(CTSTR& t_str)
{
	MBSTR mb_str = TSTR_to_MBSTR(t_str);
	int len = (int)mb_str.size();

	write(&len, sizeof(int));					// ��������������
	write(mb_str.c_str(), (int)mb_str.size());	// ���������������
}

// ****************************************************************
// �t�@�C���F�V�[�N�ʒu�ړ�
// ================================================================
bool C_file::seek(__int64 offset, int origin)
{
	return file_seek(fp, offset, origin);
}

// ****************************************************************
// �t�@�C���F�V�[�N�ʒu�擾
// ================================================================
__int64 C_file::tell()
{
	return file_tell(fp);
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�ǂݍ��݁i�o�C�i���j
// ================================================================
bool C_file::read_full_data(CTSTR& file_path, ARRAY<BYTE>& buf)
{
	buf.clear();

	// �t�@�C�����J��
	C_file file;
	if (!file.open(file_path, _T("rb")))
		return false;

	// �t�@�C���f�[�^�̑S�ǂݍ���
	int file_size = (int)file.get_size();
	buf.resize(file_size);
	if (file.read(buf.get(), file_size) < file_size)
		return false;

	file.close();
	return true;
}

bool C_file::read_full_data(CTSTR& file_path, C_stream& buf)
{
	buf.clear();

	// �t�@�C�����J��
	C_file file;
	if (!file.open(file_path, _T("rb")))
		return false;

	// �t�@�C���f�[�^�̑S�ǂݍ���
	int file_size = (int)file.get_size();
	buf.resize(file_size);
	if (file.read(buf.get(), file_size) < file_size)
		return false;

	file.close();
	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�ǂݍ��݁i�e�L�X�g�j
// ================================================================
bool C_file::read_full_text(CTSTR& file_path, TSTR& str)
{
	str.clear();

	C_file file;
	if (!file.open(file_path, _T("rt")))
		return false;		// �G���[������

	// �t�@�C���f�[�^�̑S�ǂݍ���
	ARRAY<BYTE> buf((int)file.get_size());					// �����ł̃T�C�Y�̓o�C�i���ł̃T�C�Y
	int read_cnt = file.read(buf.get(), (int)buf.size());	// �����Ŏ擾�ł���̂̓e�L�X�g�ł̃T�C�Y
	buf.resize(read_cnt);									// �e�L�X�g�ł̃T�C�Y�ɒ���

	// ������ɕϊ�
	str = MBSTR_to_TSTR(buf.get(), (int)buf.size());

	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�ǂݍ��݁iUTF-8 �e�L�X�g�j
// ================================================================
bool C_file::read_full_text_UTF8(CTSTR& file_path, TSTR& str)
{
	str.clear();

	C_file file;
	if (!file.open(file_path, _T("rt")))
		return false;		// �G���[������

	// �t�@�C���f�[�^�̑S�ǂݍ���
	ARRAY<BYTE> buf((int)file.get_size());					// �����ł̃T�C�Y�̓o�C�i���ł̃T�C�Y
	int read_cnt = file.read(buf.get(), (int)buf.size());	// �����Ŏ擾�ł���̂̓e�L�X�g�ł̃T�C�Y
	buf.resize(read_cnt);									// �e�L�X�g�ł̃T�C�Y�ɒ���

	// BOM ����
	int bom_byte = 0;
	if (read_cnt >= 3 && buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF)
	{
		bom_byte = 3;
	}

	// ������ɕϊ�
	str = UTF8_to_TSTR((char *)(buf.get() + bom_byte), (int)buf.size() - bom_byte);

	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�ǂݍ��݁iUTF-16 �e�L�X�g�j
// ================================================================
bool C_file::read_full_text_UTF16(CTSTR& file_path, TSTR& str)
{
	str.clear();

	C_file file;
	if (!file.open(file_path, _T("rb")))
		return false;		// �G���[������

	// �t�@�C���f�[�^�̑S�ǂݍ���
	ARRAY<BYTE> buf((int)file.get_size());					// �����ł̃T�C�Y�̓o�C�i���ł̃T�C�Y
	int read_cnt = file.read(buf.get(), (int)buf.size());	// �����Ŏ擾�ł���̂̓e�L�X�g�ł̃T�C�Y
	buf.resize(read_cnt);									// �e�L�X�g�ł̃T�C�Y�ɒ���

	// BOM ����
	int bom_byte = 0;
	if (read_cnt >= 2 && buf[0] == 0xFF && buf[1] == 0xFE)
	{
		bom_byte = 2;
	}

	// ������ɕϊ�
	str = TSTR((TCHAR *)(buf.get() + bom_byte), (TCHAR *)(buf.get() + read_cnt));

	// \r\n �� \n �ɕϊ�
	str = str_replace_ret(str, _T("\r\n"), _T("\n"));

	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�ǂݍ��݁i�e�L�X�g�A�s���Ƃɕ����j
// ================================================================
bool C_file::read_full_text(CTSTR& file_path, ARRAY<TSTR>& buf)
{
	buf.clear();

	TSTR str;
	if (!read_full_text(file_path, str))
		return false;		// �G���[������

	separate_text_line(str, buf);
	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�ǂݍ��݁iUTF-8 �e�L�X�g�A�s���Ƃɕ����j
// ================================================================
bool C_file::read_full_text_UTF8(CTSTR& file_path, ARRAY<TSTR>& buf)
{
	buf.clear();

	TSTR str;
	if (!read_full_text_UTF8(file_path, str))
		return false;		// �G���[������

	separate_text_line(str, buf);
	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�ǂݍ��݁iUTF-16 �e�L�X�g�A�s���Ƃɕ����j
// ================================================================
bool C_file::read_full_text_UTF16(CTSTR& file_path, ARRAY<TSTR>& buf)
{
	buf.clear();

	TSTR str;
	if (!read_full_text_UTF16(file_path, str))
		return false;		// �G���[������

	separate_text_line(str, buf);
	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�������݁i�}���`�o�C�g�e�L�X�g�j
// ================================================================
bool C_file::write_full_text(CTSTR& file_path, CMBSTR& str)
{
	C_file file;
	if (!file.open(file_path, _T("wt")))
		return false;		// �G���[������

	file.write(str);
	file.close();

	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�������݁i�}���`�o�C�g�e�L�X�g�ɕϊ��j
// ================================================================
bool C_file::write_full_text_in_MB(CTSTR& file_path, CTSTR& str)
{
	C_file file;
	if (!file.open(file_path, _T("wt")))
		return false;		// �G���[������

	file.write_in_MB(str);
	file.close();

	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�������݁iUTF-8 �ɕϊ��j
// ================================================================
bool C_file::write_full_text_in_UTF8(CTSTR& file_path, CTSTR& str)
{
	C_file file;
	if (!file.open(file_path, _T("wt")))
		return false;		// �G���[������

	file.write_in_UTF8(str);
	file.close();

	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�������݁i�}���`�o�C�g�e�L�X�g�ɕϊ��A�s���Ɓj
// ================================================================
bool C_file::write_full_text_in_MB(CTSTR& file_path, ARRAY<TSTR>& str_list)
{
	C_file file;
	if (!file.open(file_path, _T("wt")))
		return false;		// �G���[������

	int str_cnt = (int)str_list.size();
	for (int i = 0; i < str_cnt - 1; ++i)
		file.write_in_MB(str_list[i] + _T("\n"));
	if (str_cnt > 0)
		file.write_in_MB(str_list[str_cnt - 1]);	// �Ō�̍s�� \n �����Ȃ��Bread_full_text �Ǝd�l�����킹�邽�߁B

	file.close();

	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�������݁iUTF-8 �ɕϊ��A�s���Ɓj
// ================================================================
bool C_file::write_full_text_in_UTF8(CTSTR& file_path, ARRAY<TSTR>& str_list)
{
	C_file file;
	if (!file.open(file_path, _T("wt")))
		return false;		// �G���[������

	int str_cnt = (int)str_list.size();
	for (int i = 0; i < str_cnt - 1; ++i)
		file.write_in_UTF8(str_list[i] + _T("\n"));
	if (str_cnt > 0)
		file.write_in_UTF8(str_list[str_cnt - 1]);	// �Ō�̍s�� \n �����Ȃ��Bread_full_text �Ǝd�l�����킹�邽�߁B

	file.close();

	return true;
}

// ****************************************************************
// �t�@�C���F�f�[�^���ꊇ�������݁i�o�C�i���j
// ================================================================
bool C_file::write_full_data(CTSTR& file_path, ARRAY<BYTE>& buf)
{
	C_file file;
	if (!file.open(file_path, _T("wb")))
		return false;		// �G���[������

	file.write(buf.get(), (int)buf.size());
	file.close();

	return true;
}

bool C_file::write_full_data(CTSTR& file_path, C_stream& buf)
{
	C_file file;
	if (!file.open(file_path, _T("wb")))
		return false;		// �G���[������

	file.write(buf.get(), (int)buf.size());
	file.close();

	return true;
}

bool C_file::write_full_data(CTSTR& file_path, void* p_data, int size)
{
	C_file file;
	if (!file.open(file_path, _T("wb")))
		return false;		// �G���[������

	file.write(p_data, size);
	file.close();

	return true;
}


}
