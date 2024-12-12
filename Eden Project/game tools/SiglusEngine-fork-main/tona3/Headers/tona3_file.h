#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"
#include	"tona3_stream.h"

namespace NT3
{

// ****************************************************************
// �t�@�C��
// ================================================================
class C_file
{
public:
	C_file();
	~C_file();

	bool	open(CTSTR& file_path, CTSTR& mode);
	bool	close();
	int		read(void* dst, int size);
	bool	read_in_MB_with_len(TSTR& t_str);
	int		write(const void* src, int size);
	int		write(CWSTR& w_str);
	int		write(CMBSTR& mb_str);
	void	write_in_MB(CTSTR& t_str);
	void	write_in_MB_with_len(CTSTR& t_str);
	void	write_in_UTF8(CTSTR& t_str);
	bool	seek(__int64 offset, int origin);
	__int64	tell();

	FILE*	get_fp()	{	return fp;		}
	UINT64	get_size()	{	return size;	}

	static	bool	read_full_data(CTSTR& file_path, ARRAY<BYTE>& buf);			// �t�@�C������f�[�^���ꊇ�ǂݍ��݁i�o�C�i���j
	static	bool	read_full_data(CTSTR& file_path, C_stream& stream);			// �t�@�C������f�[�^���ꊇ�ǂݍ��݁i�o�C�i���j
	static	bool	read_full_text(CTSTR& file_path, TSTR& str);				// �t�@�C������f�[�^���ꊇ�ǂݍ��݁i�e�L�X�g�j
	static	bool	read_full_text_UTF8(CTSTR& file_path, TSTR& str);			// �t�@�C������f�[�^���ꊇ�ǂݍ��݁iUTF-8 �e�L�X�g�j
	static	bool	read_full_text_UTF16(CTSTR& file_path, TSTR& str);			// �t�@�C������f�[�^���ꊇ�ǂݍ��݁iUTF-16 �e�L�X�g�j
	static	bool	read_full_text(CTSTR& file_path, ARRAY<TSTR>& buf);			// �t�@�C������f�[�^���ꊇ�ǂݍ��݁i�e�L�X�g�A�s���Ƃɕ����j
	static	bool	read_full_text_UTF8(CTSTR& file_path, ARRAY<TSTR>& buf);	// �t�@�C������f�[�^���ꊇ�ǂݍ��݁iUTF-8 �e�L�X�g�A�s���Ƃɕ����j
	static	bool	read_full_text_UTF16(CTSTR& file_path, ARRAY<TSTR>& buf);	// �t�@�C������f�[�^���ꊇ�ǂݍ��݁iUTF-16 �e�L�X�g�A�s���Ƃɕ����j

	static	bool	write_full_data(CTSTR& file_path, ARRAY<BYTE>& buf);		// �t�@�C���Ƀf�[�^���ꊇ�������݁i�o�C�i���j
	static	bool	write_full_data(CTSTR& file_path, C_stream& buf);			// �t�@�C���Ƀf�[�^���ꊇ�������݁i�o�C�i���j
	static	bool	write_full_data(CTSTR& file_path, void* p_data, int size);	// �t�@�C���Ƀf�[�^���ꊇ�������݁i�o�C�i���j
	static	bool	write_full_text(CTSTR& file_path, CMBSTR& str);				// �t�@�C���Ƀf�[�^���ꊇ�������݁i�}���`�o�C�g�e�L�X�g�j
	static	bool	write_full_text_in_MB(CTSTR& file_path, CTSTR& str);		// �t�@�C���Ƀf�[�^���ꊇ�������݁i�}���`�o�C�g�e�L�X�g�ɕϊ��j
	static	bool	write_full_text_in_MB(CTSTR& file_path, ARRAY<TSTR>& buf);	// �t�@�C���Ƀf�[�^���ꊇ�������݁i�}���`�o�C�g�e�L�X�g�ɕϊ��A�s���Ɓj
	static	bool	write_full_text_in_UTF8(CTSTR& file_path, CTSTR& str);			// �t�@�C���Ƀf�[�^���ꊇ�������݁iUTF-8 �ɕϊ��j
	static	bool	write_full_text_in_UTF8(CTSTR& file_path, ARRAY<TSTR>& buf);	// �t�@�C���Ƀf�[�^���ꊇ�������݁iUTF-8 �ɕϊ��A�s���Ɓj

private:
	FILE*	fp;
	UINT64	size;

	// �R�s�[�֎~�I
	C_file(C_file&);
	C_file& operator = (C_file&);
};

}
