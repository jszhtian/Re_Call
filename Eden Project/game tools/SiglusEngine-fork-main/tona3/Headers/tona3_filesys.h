#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"
#include	"tona3_stream.h"

#include	<shlobj.h>

namespace NT3
{

// ****************************************************************
// �t�@�C�����̕�����Ɋւ��鑀��
// ================================================================
TSTR	get_file_name(CTSTR& file_path);					// �t���p�X����t�@�C�������擾�i"aaa\\bbb\\ccc.dat" �� "ccc.dat"�j
TSTR	get_dir_path(CTSTR& file_path);						// �t���p�X����f�B���N�g���p�X���擾�i"aaa\\bbb\\ccc.dat" �� "aaa\\bbb"�j
TSTR	get_upper_dir_path(CTSTR& dir_path);				// �P��Ɉړ������f�B���N�g���p�X���擾�i"aaa\\bbb\\ccc" �� "aaa\\bbb"�j
TSTR	get_extension(CTSTR& file_path);					// �g���q���擾�i"aaa\\bbb\\ccc.dat" �� "dat"�j
bool	check_extension(CTSTR& file_path, CTSTR& ext);		// �g���q�𔻒�i"aaa\\bbb\\ccc.dat" == "dat"�j
void	change_extension(TSTR& file_path, CTSTR& ext);		// �g���q��ύX�����p�X���擾�i"aaa\\bbb\\ccc.dat" �� "aaa\\bbb\\ccc.ext"�j
TSTR	change_extension_ret(CTSTR& file_path, CTSTR& ext);	// �g���q��ύX�����p�X���擾�i"aaa\\bbb\\ccc.dat" �� "aaa\\bbb\\ccc.ext"�j�i�߂�l�Ŏ擾�j
void	erase_extension(TSTR& file_path);					// �g���q���폜�����p�X���擾�i"aaa\\bbb\\ccc.dat" �� "aaa\\bbb\\ccc"�j
TSTR	erase_extension_ret(CTSTR& file_path);				// �g���q���폜�����p�X���擾�i"aaa\\bbb\\ccc.dat" �� "aaa\\bbb\\ccc"�j�i�߂�l�Ŏ擾�j
bool	compare_file_path(TSTR lhs, TSTR rhs);				// �Q�̃p�X���r����

// ****************************************************************
// �t�@�C���̑����ȂǂɊւ��鑀��
// ================================================================
int		file_get_type(CTSTR& file_path);					// �t�@�C���̎�ނ��擾�i-1:���݂��Ȃ��A0:�f�B���N�g���A1:�t�@�C���j
UINT64	file_get_size(CTSTR& file_path);					// �t�@�C���̃T�C�Y���擾
bool	file_is_read_only(CTSTR& file_path);				// �ǂݎ���p�𔻒�
bool	file_set_read_only(CTSTR& file_path, bool onoff);	// �ǂݎ���p��ݒ�
bool	file_is_hidden(CTSTR& file_path);					// �B���t�@�C���𔻒�
bool	file_set_hidden(CTSTR& file_path, bool onoff);		// �B���t�@�C����ݒ�
bool	file_is_system(CTSTR& file_path);					// �V�X�e���t�@�C���𔻒�
bool	file_set_system(CTSTR& file_path, bool onoff);		// �V�X�e���t�@�C����ݒ�
bool	file_get_time(CTSTR& file_path, FILETIME* create, FILETIME* access, FILETIME* write);	// �t�@�C���������擾
bool	file_set_time(CTSTR& file_path, FILETIME* create, FILETIME* access, FILETIME* write);	// �t�@�C��������ݒ�
int		file_cmp_write_time(CTSTR& file_path_1, CTSTR& file_path_2);	// �X�V�������r�i-1:�����Â� 0:������ 1:�����V�����j
TSTR	file_get_product_version(CTSTR& file_path);

// ****************************************************************
// ���W�X�g������֐�
// ================================================================
bool	reg_check_key(HKEY root, CTSTR& path);								// ���W�X�g���̃L�[�����݂��邩�`�F�b�N
int		reg_check_sub_key(HKEY root, CTSTR& path);							// ���W�X�g���̃L�[���T�u�L�[���������`�F�b�N
bool	reg_delete_key(HKEY root, CTSTR& path);								// ���W�X�g���̃L�[���폜����
bool	reg_set_value_str(HKEY root, CTSTR& path, CTSTR& name, CTSTR& str);	// ���W�X�g���̒l��ݒ肷��i������j
bool	reg_get_value_str(HKEY root, CTSTR& path, CTSTR& name, TSTR& str);	// ���W�X�g���̒l���擾����i������j

// ****************************************************************
// �t�@�C���V�X�e���֘A�̑���
// ================================================================
bool	filesys_copy_file(CTSTR& from, CTSTR& to);		// WinAPI �� CopyFile ���g���ăt�@�C�����R�s�[�i�P�̃t�@�C���̂݁j
bool	filesys_rename_file(CTSTR& from, CTSTR& to);	// WinAPI �� MoveFile ���g���ăt�@�C�����R�s�[�i�P�̃t�@�C���̂݁j
bool	filesys_delete_file(CTSTR& file_path);			// WinAPI �� DeleteFile ���g���ăt�@�C�����폜�i�P�̃t�@�C���̂݁j
bool	filesys_remove_directory(CTSTR& dir_path);		// WinAPI �� RemoveDirectory ���g���ăf�B���N�g�����폜�i��f�B���N�g���̂݁j
bool	filesys_copy_directory_with_contain_files(CTSTR& src_path, CTSTR& dst_path, bool is_cancel_read_only, bool is_contain_directory);		// �f�B���N�g�����R�s�[�i���ɓ����Ă���t�@�C���y�уf�B���N�g�����R�s�[����j
bool	filesys_remove_directory_with_contain_files(CTSTR& dir_path, bool is_delete_current_directory, bool is_delete_directory, bool is_delete_read_only);		// �f�B���N�g�����폜�i���ɓ����Ă���t�@�C���y�уf�B���N�g�����폜����j
bool	shell_remove_file(CTSTR& file_path);					// Shell ���g���ăt�@�C�������ݔ��Ɉړ��i�t�@�C�����ɒ��ӁI �K�� shell_file_name_connect ���g���Ă��������j
TSTR	shell_file_path_connect(ARRAY<TSTR> file_path_list);	// �t�@�C������ Shell �֐��p�ɘA���i"a.dat\0b.dat\0c.dat\0\0")

// ���֗��֐�
bool	create_dir_from_file_path(CTSTR& file_path);	// ��C�Ƀf�B���N�g���쐬�i"aaa\\bbb\\ccc.dat" �� "aaa\\bbb" ���쐬�j
bool	create_dir_from_dir_path(CTSTR& dir_path);		// ��C�Ƀf�B���N�g���쐬�i"aaa\\bbb" �� "aaa\\bbb" ���쐬�j

// ****************************************************************
// �V�F���Ƃ��v���Z�X�Ƃ��F�X
// ================================================================
TSTR	get_droped_link_path(WPARAM wp);										// WM_DROPFILE ����t�@�C�������擾
TSTR	get_shortcut_link(CTSTR& shortcut_path);								// �V���[�g�J�b�g���烊���N��̃t�@�C�����擾
bool	get_my_doument_path(TSTR& my_document_path);							// �}�C�h�L�������g�̃p�X���擾
HANDLE	open_process(CTSTR& command, DWORD *process_id = NULL);					// �v���Z�X���J��
bool	check_process_from_id(DWORD process_id);								// �v���Z�X�h�c���炻�̃v���Z�X�����s�����ǂ������肷��
bool	shell_execute_open(CTSTR& file_path);									// �t�@�C�����֘A�t����ꂽ���W���[���ŊJ��
bool	shell_execute_open_by_module(CTSTR& file_path, CTSTR& module_path);		// �t�@�C�������W���[���ŊJ���H�i�������j
void	open_file_by_editor(TSTR txt_path, int line, CTSTR& editor_path);		// �G�f�B�^�Ńe�L�X�g�t�@�C�����J��
bool	open_file_by_browser(CTSTR& html_path);									// �u���E�U�łg�s�l�k�t�@�C�����J��
bool	open_dir(CTSTR& dir_path);												// �G�N�X�v���[���Ńt�H���_���J��
bool	str_copy_to_clip_board_in_MB(CTSTR& str);								// ��������N���b�v�{�[�h�փR�s�[
TSTR	str_copy_from_clip_board_in_MB();										// ��������N���b�v�{�[�h����擾
bool	resource_get_data(LPCTSTR name, LPCTSTR type, BUFFER& dst_buffer);		// ���\�[�X���擾����

// ****************************************************************
// �t�@�C�������\����
// ================================================================
struct FILEFINDINFO
{
	FILEFINDINFO();
	TSTR	rltv_path;		// �����X�^�[�g�ʒu����̑��΃p�X
	bool	is_directory;	// �f�B���N�g��
	UINT64	file_size;		// �t�@�C���T�C�Y
};

// ****************************************************************
// �t�@�C������
// ================================================================
class C_file_find
{
public:

	C_file_find();
	~C_file_find()	{}

	void	clear();
	bool	find(CTSTR& dir, CTSTR& first_name, CTSTR& extension, bool is_tree, bool is_find_file, bool is_find_dir);

	bool	empty()						{	return file.empty();		}
	int		get_file_cnt()				{	return (int)file.size();	}
	UINT64	get_full_size()				{	return full_size;			}
	TSTR	get_full_path(int file_no);

	FILEFINDINFO&	operator[](int file_no)
	{
		return file[file_no];
	}

private:
	bool	find_main(CTSTR& rltv_dir);

	typedef ARRAY<FILEFINDINFO> FFLIST;
	FFLIST	file;			// �t�@�C�����X�g
	UINT64	full_size;		// ���t�@�C���T�C�Y
	TSTR	initial_dir;	// �����X�^�[�g�ʒu
	TSTR	first_name;		// ��������t�@�C����
	TSTR	ext;			// ��������g���q
	bool	is_tree;		// �ċA�I�Ɍ�������
	bool	is_find_file;	// �t�@�C������������
	bool	is_find_dir;	// �f�B���N�g������������
};

// ****************************************************************
// �t�@�C���֘A�̃_�C�A���O
// ================================================================
class C_select_file_dialog
{
public:
	void	add_filter(CTSTR& name, CTSTR& type);
	TSTR	select_open_file(CTSTR& title, CTSTR& def_file_name);
	TSTR	select_save_file(CTSTR& title, CTSTR& def_file_name);

private:
	ARRAY<TSTR>		filter_name_list;
	ARRAY<TSTR>		filter_type_list;
};

// ****************************************************************
// �t�H���_�I���_�C�A���O
// ================================================================
TSTR select_directory(CTSTR& root_path, CTSTR& initial_path, CTSTR& window_title, CTSTR& info_str);

// ****************************************************************
// �A�C�e���h�c���X�g
// ================================================================
LPITEMIDLIST	iil_create_from_dir_path(CTSTR& dir_path);
void			iil_release(LPITEMIDLIST* iil);
TSTR			iil_get_dir_path(LPITEMIDLIST iil);

// ****************************************************************
// �A�C�e���h�c���X�g�Ǘ��N���X
// ================================================================
class Citem_id_list
{
public:
	Citem_id_list(CTSTR& dir_path);
	~Citem_id_list();

	LPITEMIDLIST	get();

private:
	void			release();

	LPITEMIDLIST	iil;
};



}
