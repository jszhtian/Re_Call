#include	"tona3_pch.h"
#include	"tona3_filesys.h"
#include	"tona3_util.h"
#include	"tona3_app.h"
#include	"tona3_error.h"

#include	<shellapi.h>
#include	<shlwapi.h>
#include	<imagehlp.h>

#pragma		comment(lib, "shlwapi.lib")
#pragma		comment(lib, "imagehlp.lib")
#pragma		comment(lib, "version.lib")

namespace NT3
{

	TSTR to_full_path(CTSTR& rltv_path)
	{
		TCHAR full_path[1024];
		if (GetFullPathName(rltv_path.c_str(), 1024, full_path, NULL) == 0) {
			return _T("");
		}

		return full_path;
	}

// ****************************************************************
// �t�@�C���p�X����t�@�C�����𓾂�
//		"aaa\\bbb\\ccc.dat" �� "ccc.dat"
// ================================================================
TSTR get_file_name(CTSTR& path)
{
	int pos = (int)path.rfind('\\');
	if (pos == TSTR::npos)
		return path;

	return TSTR(path, pos + 1);
}

// ****************************************************************
// �t�@�C���p�X����f�B���N�g���p�X�𓾂�
//		"aaa\\bbb\\ccc.dat" �� "aaa\\bbb"
// ================================================================
TSTR get_dir_path(CTSTR& path)
{
	int pos = (int)path.rfind('\\');
	if (pos == TSTR::npos)
		return path;

	return TSTR(path, 0, pos);
}

// ****************************************************************
// �P��̃f�B���N�g���p�X�𓾂�
//		"aaa\\bbb\\ccc" �� "aaa\\bbb"
// ================================================================
TSTR get_upper_dir_path(CTSTR& path)
{
	int pos = (int)path.rfind('\\');
	if (pos == TSTR::npos)
		return path;

	return TSTR(path, 0, pos);
}

// ****************************************************************
// �g���q���擾
//		"aaa\\bbb\\ccc.dat" �� "dat"
// ================================================================
TSTR get_extension(CTSTR& path)
{
	int len = (int)path.size();
	for (int p = len - 1; p >= 0; p--)	{
		if (path[p] == _T('\\'))
			return _T("");
		if (path[p] == _T('.'))
			return TSTR(path, p + 1);
	}

	return _T("");
}

// ****************************************************************
// �g���q�𔻒�
//		"aaa\\bbb\\ccc.dat" == "dat"
// ================================================================
bool check_extension(CTSTR& path, CTSTR& ext)
{
	TSTR src = get_extension(path);

	if (str_to_upper_ret(src) == str_to_upper_ret(ext))
		return true;

	return false;
}

// ****************************************************************
// �g���q��ύX�����p�X���擾
//		"aaa\\bbb\\ccc.dat" �� "aaa\\bbb\\ccc.ext"
// ================================================================
void change_extension(TSTR& path, CTSTR& ext)
{
	path = change_extension_ret(path, ext);
}

TSTR change_extension_ret(CTSTR& path, CTSTR& ext)
{
	int len = (int)path.size();
	for (int p = len - 1; p >= 0; p--)	{
		if (path[p] == _T('\\'))
			return path + _T(".") + ext;
		if (path[p] == _T('.'))
			return TSTR(path, 0, p + 1) + ext;
	}

	return path + _T(".") + ext;
}

// ****************************************************************
// �g���q���폜�����p�X���擾
//		"aaa\\bbb\\ccc.dat" �� "aaa\\bbb\\ccc"
// ================================================================
void erase_extension(TSTR& path)
{
	path = erase_extension_ret(path);
}

TSTR erase_extension_ret(CTSTR& path)
{
	int len = (int)path.size();
	for (int p = len - 1; p >= 0; p--)	{
		if (path[p] == _T('\\'))
			return path;
		if (path[p] == _T('.'))
			return TSTR(path, 0, p);
	}

	return path;
}

// ****************************************************************
// �f�B���N�g���̍쐬�i�t�@�C���p�X����j
//		"aaa\bbb.txt" �� "aaa" ���쐬
// ================================================================

#if 1

bool create_dir_from_file_path(CTSTR& file_path)
{
	// �Ō�̃t�@�C�����̂����ăf�B���N�g���p�X�ɂ��܂��B
	TSTR dir_path = get_dir_path(file_path);

	return create_dir_from_dir_path(dir_path);
}

#else

bool create_dir_from_file_path(CTSTR& file_path)
{
	if (file_path.empty())
		return true;

	// MakeSureDirectoryPathExists �́A���C�h�����ł��Ȃ����ۂ�
	MBSTR mb_str = TSTR_to_MBSTR(file_path);
	if (!MakeSureDirectoryPathExists(mb_str.c_str()))	{
		set_last_error_win(_T("MakeSureDirectoryPathExists"));		// Windows �G���[
		return false;
	}

	return true;
}

#endif

// ****************************************************************
// �f�B���N�g���̍쐬�i�f�B���N�g���p�X����j
//		"aaa\bbb" �� "aaa\bbb" ���쐬
// ================================================================

#if 1

bool create_dir_from_dir_path(CTSTR& dir_path)
{
	int error_code;

	// �t�@�C���̑��ݔ���
	// ���Ƀf�B���N�g�肪����ꍇ�́A������Ԃ��܂��B
	// ���ɓ����̃t�@�C��������ꍇ�́ASHCreateDirectoryEx ���G���[��Ԃ����ƂɂȂ�܂��B
	if (file_get_type(dir_path) == 0)
		return true;	// ���ɑ��݂���̂Ő����B

	// ��΃p�X�ɒ���
	TSTR asb_path = to_full_path(dir_path);

	if ((error_code = SHCreateDirectoryEx(NULL, asb_path.c_str(), NULL)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("SHCreateDirectoryEx"));
		return false;
	}

	return true;
}

#else

bool create_dir_from_dir_path(CTSTR& dir_path)
{
	if (dir_path.empty())
		return true;

	// ������ '\\' �łȂ��ꍇ�A'\\' ������
	TSTR file_path = dir_path;
	if (file_path[file_path.length() - 1] != '\\')
		file_path += _T("\\");

	return create_dir_from_file_path(file_path);
}

#endif

// ****************************************************************
// �Q�̃p�X���r����
// ================================================================
void erase_illegal_yen(TCHAR* str)
{
	TCHAR* p = str;
	TCHAR* q = str;

	for (; *p != _T('\0'); )	{

		// \\ ���Q�������ꍇ�� p �݂̂�i�߂�
		if (*p == _T('\\') && *(p + 1) == _T('\\'))	{				
			p ++;
		}
		// ������ \\ �������ꍇ�͏I��
		else if (*p == _T('\\') && *(p + 1) == _T('\0'))	{
			break;
		}
		else	{
			*q++ = *p++;
		}
	}

	*q = _T('\0');
}

bool compare_file_path(TSTR lhs, TSTR rhs)
{
	// �������ɒ���
	str_to_lower(lhs);
	str_to_lower(rhs);

	// . �� .. ��␳����
	TCHAR lhs_buf[MAX_PATH] = {0};
	TCHAR rhs_buf[MAX_PATH] = {0};
	::PathCanonicalize(lhs_buf, lhs.c_str());
	::PathCanonicalize(rhs_buf, rhs.c_str());

	// �A������ \\ �▖���� \\ �����
	erase_illegal_yen(lhs_buf);
	erase_illegal_yen(rhs_buf);

	// ��r����
	return _tcscmp(lhs_buf, rhs_buf) == 0;
}

// ****************************************************************
// �t�@�C���̎�ނ𒲂ׂ�
//		-1: ���݂��Ȃ�
//		 0: �f�B���N�g��
//		 1: �t�@�C��
// ================================================================
int file_get_type(CTSTR& path)
{
	DWORD attr = GetFileAttributes(path.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES)
		return -1;		// ���݂��Ȃ�

	// �t�@�C���̎�ނ𒲂ׂ�
	if (attr & FILE_ATTRIBUTE_DIRECTORY)	return 0;	// �f�B���N�g��
	else									return 1;	// �t�@�C��;
}

// ****************************************************************
// �t�@�C���̃T�C�Y�𒲂ׂ�
// ================================================================
UINT64 file_get_size(CTSTR& path)
{
	WIN32_FILE_ATTRIBUTE_DATA fd;
	if (!GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &fd))
		return 0;

	// �t�@�C���T�C�Y���擾
	UINT64 size = ((UINT64)fd.nFileSizeHigh << 32) + fd.nFileSizeLow;
	return size;
}

// ****************************************************************
// �t�@�C���̑����𔻒�
// ================================================================
bool file_is_read_only(CTSTR& file_path)
{
	return (::GetFileAttributes(file_path.c_str()) & FILE_ATTRIBUTE_READONLY) > 0;
}

bool file_is_hidden(CTSTR& file_path)
{
	return (::GetFileAttributes(file_path.c_str()) & FILE_ATTRIBUTE_HIDDEN) > 0;
}

bool file_is_system(CTSTR& file_path)
{
	return (::GetFileAttributes(file_path.c_str()) & FILE_ATTRIBUTE_SYSTEM) > 0;
}

// ****************************************************************
// �t�@�C���̑�����ݒ�
// ================================================================
bool file_set_read_only(CTSTR& file_path, bool onoff)
{
	DWORD attr = ::GetFileAttributes(file_path.c_str());	// �������擾
	if (onoff)	attr |= FILE_ATTRIBUTE_READONLY;			// on
	else		attr &= ~FILE_ATTRIBUTE_READONLY;			// off

	return ::SetFileAttributes(file_path.c_str(), attr) > 0;
}

bool file_set_hidden(CTSTR& file_path, bool onoff)
{
	DWORD attr = ::GetFileAttributes(file_path.c_str());	// �������擾
	if (onoff)	attr |= FILE_ATTRIBUTE_HIDDEN;				// on
	else		attr &= ~FILE_ATTRIBUTE_HIDDEN;				// off

	return ::SetFileAttributes(file_path.c_str(), attr) > 0;
}

bool file_set_system(CTSTR& file_path, bool onoff)
{
	DWORD attr = ::GetFileAttributes(file_path.c_str());	// �������擾
	if (onoff)	attr |= FILE_ATTRIBUTE_SYSTEM;				// on
	else		attr &= ~FILE_ATTRIBUTE_SYSTEM;				// off

	return ::SetFileAttributes(file_path.c_str(), attr) > 0;
}

// ****************************************************************
// �t�@�C���^�C�����擾
//		create:		�쐬����
//		access:		�A�N�Z�X����
//		write:		�X�V����
// ================================================================
bool file_get_time(CTSTR& file_path, FILETIME* create, FILETIME* access, FILETIME* write)
{
	HANDLE h_file = CreateFile(
		file_path.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (h_file == INVALID_HANDLE_VALUE)
		return false;

	GetFileTime(h_file, create, access, write);
	CloseHandle(h_file);	// ����̖Y��Ȃ��ł�

	return true;
}

// ****************************************************************
// �t�@�C���^�C����ݒ�
//		create:		�쐬����
//		access:		�A�N�Z�X����
//		write:		�X�V����
// ================================================================
bool file_set_time(CTSTR& file_path, FILETIME* create, FILETIME* access, FILETIME* write)
{
	HANDLE h_file = CreateFile(
		file_path.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (h_file == INVALID_HANDLE_VALUE)
		return false;

	SetFileTime(h_file, create, access, write);
	CloseHandle(h_file);	// ����̖Y��Ȃ��ł�

	return true;
}

// ****************************************************************
// �t�@�C���̍X�V�������r
// ================================================================
int file_cmp_write_time(CTSTR& file_path_1, CTSTR& file_path_2)
{
	FILETIME write_1, write_2;
	file_get_time(file_path_1, NULL, NULL, &write_1);
	file_get_time(file_path_2, NULL, NULL, &write_2);

	UINT64 write_1_64 = ((UINT64)write_1.dwHighDateTime << 32) + write_1.dwLowDateTime;
	UINT64 write_2_64 = ((UINT64)write_2.dwHighDateTime << 32) + write_2.dwLowDateTime;

	if (false);
	else if (write_1_64 < write_2_64)	return -1;
	else if (write_1_64 == write_2_64)	return 0;
	else if (write_1_64 > write_2_64)	return 1;

	return 0;
}


// ****************************************************************
// �t�@�C���̃v���_�N�g�o�[�W�������擾
// ================================================================

// �o�[�W�������󂯎�邽�߂̍\����
struct GFV_LANGANDCODEPAGE
{
	WORD language;
	WORD codepage;
};

// �o�[�W�������󂯎��
TSTR file_get_product_version(CTSTR& file_path)
{
	// �o�[�W�������̃T�C�Y���擾����
	DWORD info_handle = 0;	// �n���h�����Č����Ă邯�ǕK�� 0 ���Ԃ��Ă��邻���ł��B
	DWORD info_size = GetFileVersionInfoSize(file_path.c_str(), &info_handle);
	if (info_size <= 0)
		return _T("");

	bool ret = false;

	// �o�[�W���������擾����
	ARRAY<BYTE> info(info_size);
	GetFileVersionInfo(file_path.c_str(), info_handle, info_size, info.get());

	// �o�[�W�������̃��P�[�����擾����
	GFV_LANGANDCODEPAGE* lang_p = NULL;
	UINT lang_size = 0;
	VerQueryValue(info.get(), _T("\\VarFileInfo\\Translation"), (void **)&lang_p, &lang_size);
	if (lang_size < sizeof(GFV_LANGANDCODEPAGE))
		return _T("");

	// ���i�o�[�W�������擾����
	TCHAR* produce_version_p = NULL;
	UINT produce_version_size = 0;
	TSTR block_str = str_format(_T("\\StringFileInfo\\%04x%04x\\ProductVersion"), lang_p[0].language, lang_p[0].codepage);
	VerQueryValue(info.get(), (TCHAR *)block_str.c_str(), (VOID **)&produce_version_p, &produce_version_size);
	if (produce_version_size == 0)
		return _T("");

	return produce_version_p;
}

// ****************************************************************
// ���W�X�g�������֐�
// ================================================================
class C_registry_key
{
public:
	C_registry_key(HKEY h_key)	{ m_h_key = h_key; }
	~C_registry_key()			{ if (m_h_key)	RegCloseKey(m_h_key); }
private:
	HKEY	m_h_key;
};

// ****************************************************************
// ���W�X�g���̃L�[�����݂��邩�`�F�b�N
//		���s�Fget_my_last_error
// ================================================================
bool reg_check_key(HKEY root, CTSTR& path)
{
	DWORD error_code = ERROR_SUCCESS;

	HKEY h_key = NULL;
	if ((error_code = RegOpenKeyEx(root, path.c_str(), 0, KEY_READ, &h_key)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegOpenKeyEx"));		// Windows �G���[
		return false;
	}

	BSP<C_registry_key> key(new C_registry_key(h_key));

	return true;	// ���݂���
}

// ****************************************************************
// ���W�X�g���̃L�[���T�u�L�[���������`�F�b�N
//		���s�Fget_my_last_error
// ================================================================
int reg_check_sub_key(HKEY root, CTSTR& path)
{
	DWORD error_code = ERROR_SUCCESS;

	HKEY h_key = NULL;
	error_code = RegOpenKeyEx(root, path.c_str(), 0, KEY_READ, &h_key);
	if (error_code != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegOpenKeyEx"));		// Windows �G���[
		return -1;	// ���s
	}
	BSP<C_registry_key> key(new C_registry_key(h_key));

	TCHAR buf[256];
	DWORD buf_len = 0;
	FILETIME ft = {0};
	error_code = RegEnumKeyEx(h_key, 0, buf, &buf_len, 0, NULL, NULL, &ft);
	if (error_code != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegEnumKeyEx"));		// Windows �G���[
		return -1;	// ���s
	}

	if (error_code == ERROR_NO_MORE_ITEMS)
		return 0;	// �T�u�L�[���Ȃ�
	
	return 1;	// �T�u�L�[������
}

// ****************************************************************
// ���W�X�g���̃L�[���폜����
//		���s�Fget_my_last_error
// ================================================================
bool reg_delete_key(HKEY root, CTSTR& path)
{
	DWORD error_code = ERROR_SUCCESS;

	HKEY h_key = NULL;
	if ((error_code = RegOpenKeyEx(root, path.c_str(), 0, KEY_READ, &h_key)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegOpenKeyEx"));		// Windows �G���[
		return false;
	}

	BSP<C_registry_key> key(new C_registry_key(h_key));

	// ���W�X�g�����폜
	if ((error_code = RegDeleteKey(root, path.c_str())) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegDeleteKey"));		// Windows �G���[
		return false;
	}

	return true;
}

// ****************************************************************
// ���W�X�g���̒l���擾����i������j
//		���s�Fget_my_last_error
// ================================================================
bool reg_get_value_str(HKEY root, CTSTR& path, CTSTR& name, TSTR& str)
{
	DWORD error_code = ERROR_SUCCESS;

	HKEY h_key = NULL;
	if ((error_code = RegOpenKeyEx(root, path.c_str(), 0, KEY_READ, &h_key)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegOpenKeyEx"));		// Windows �G���[
		return false;
	}

	BSP<C_registry_key> key(new C_registry_key(h_key));
	
	DWORD type = 0;
	BYTE buffer[1024];
	DWORD buffer_size = 1024;
	if ((error_code = RegQueryValueEx(h_key, name.c_str(), 0, &type, buffer, &buffer_size)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegQueryValueEx"));	// Windows �G���[
		return false;
	}

	str = TSTR((TCHAR *)buffer, (TCHAR *)(buffer + buffer_size - 2));	// -2: [\0]
	return true;
}

// ****************************************************************
// ���W�X�g���̒l��ݒ肷��i������j
// ================================================================
bool reg_set_value_str(HKEY root, CTSTR& path, CTSTR& name, CTSTR& str)
{
	DWORD error_code = ERROR_SUCCESS;

	// ���W�X�g�����J��
	HKEY h_key = NULL;
	if ((error_code = RegCreateKeyEx(root, path.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &h_key, NULL)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegCreateKeyEx"));		// Windows �G���[
		return false;
	}

	BSP<C_registry_key> key(new C_registry_key(h_key));

	// �l����������
	if ((error_code = RegSetValueEx(h_key, name.c_str(), 0, REG_SZ, (const BYTE *)str.c_str(), (DWORD)(str.size() + 1) * sizeof(TCHAR))) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("Cregistry_key"));		// Windows �G���[
		return false;
	}

	return true;
}

// ****************************************************************
// �t�@�C�����R�s�[�iWinAPI �o�[�W�����j
// ================================================================
bool filesys_copy_file(CTSTR& from, CTSTR& to)
{
	if (!::CopyFile(from.c_str(), to.c_str(), FALSE))	{
		set_last_error_win(_T("CopyFile"));		// Windows �G���[
		return false;
	}

	return true;
}

// ****************************************************************
// �t�@�C�������l�[���iWinAPI �o�[�W�����j
// ================================================================
bool filesys_rename_file(CTSTR& from, CTSTR& to)
{
	if (!::MoveFile(from.c_str(), to.c_str()))	{
		set_last_error_win(_T("MoveFile"));		// Windows �G���[
		return false;
	}

	return true;
}

// ****************************************************************
// �t�@�C�����폜�iWinAPI �o�[�W�����j
// ================================================================
bool filesys_delete_file(CTSTR& file_path)
{
	if (!::DeleteFile(file_path.c_str()))	{
		set_last_error_win(_T("DeleteFile"));	// Windows �G���[
		return false;
	}

	return true;
}

// ****************************************************************
// �f�B���N�g�����폜�iWinAPI �o�[�W�����j
// ================================================================
bool filesys_remove_directory(CTSTR& dir_path)
{
	if (!::RemoveDirectory(dir_path.c_str()))	{
		set_last_error_win(_T("RemoveDirectory"));	// Windows �G���[
		return false;
	}

	return true;
}

// ****************************************************************
// �f�B���N�g�����R�s�[�i���ɓ����Ă���t�@�C���y�уf�B���N�g�����R�s�[����j
// ================================================================
bool filesys_copy_directory_with_contain_files(CTSTR& src_path, CTSTR& dst_path, bool is_cancel_read_only, bool is_contain_directory)
{
	// �R�s�[���ƃR�s�[�悪����
	if (str_to_upper_ret(src_path) == str_to_upper_ret(dst_path))	{
		set_last_error_invalid(_T("filesys_copy_directory"));		// �����ȃp�����[�^
		return false;
	}

	// �R�s�[�������݂��Ȃ�
	if (file_get_type(src_path) != 0)	{
		set_last_error_invalid(_T("filesys_copy_directory"));		// �����ȃp�����[�^
		return false;
	}

	// �R�s�[�悪���݂��Ȃ�
	if (file_get_type(dst_path) != 0)	{
		set_last_error_invalid(_T("filesys_copy_directory"));		// �����ȃp�����[�^
		return false;
	}

	// �t�@�C�����X�g�쐬�i�c���[�Ȃ��j
	C_file_find flst;
	flst.find(src_path, _T("*"), _T("*"), false, true, is_contain_directory);

	// �R�s�[�J�n
	TSTR src_file_path;
	TSTR dst_file_path;
	for (int i = 0; i < flst.get_file_cnt(); i++)	{
		src_file_path = src_path + _T("\\") + flst[i].rltv_path;
		dst_file_path = dst_path + _T("\\") + flst[i].rltv_path;
		if (flst[i].is_directory)	{
			// �f�B���N�g���쐬
			::MakeSureDirectoryPathExists(TSTR_to_MBSTR(dst_file_path + _T("\\")).c_str());
			// �ċA�Ăяo��
			filesys_copy_directory_with_contain_files(src_file_path, dst_file_path, is_cancel_read_only, is_contain_directory);
		}
		else	{
			// �R�s�[����
			filesys_copy_file(src_file_path, dst_file_path);
			// �ǂݎ���p��������������
			if (is_cancel_read_only)
				file_set_read_only(dst_file_path, false);
		}
	}

	return true;
}

// ****************************************************************
// �f�B���N�g�����폜�i���ɓ����Ă���t�@�C���y�уf�B���N�g�����폜����j
// ================================================================
bool filesys_remove_directory_with_contain_files(CTSTR& dir_path, bool is_delete_current_directory, bool is_delete_directory, bool is_delete_read_only)
{
	// ���S�m�F�F�R�����ȉ��̓p�X����Ȃ�
	if ((int)dir_path.size() <= 3)	{
		set_last_error_invalid(_T("filesys_remove_directory"));		// �����ȃp�����[�^
		return false;
	}

	// ���S�m�F�F�Q�����ڂ̕������m�F
	if (dir_path[1] != _T(':'))	{
		set_last_error_invalid(_T("filesys_remove_directory"));		// �����ȃp�����[�^
		return false;
	}

	// ���S�m�F�F�R�����ڂ̕������m�F
	if (dir_path[2] != _T('\\'))	{
		set_last_error_invalid(_T("filesys_remove_directory"));		// �����ȃp�����[�^
		return false;
	}

	// ���S�m�F�F�O�̂��߃f�B���N�g���̑��݂��m�F����
	if (file_get_type(dir_path) != 0)	{
		set_last_error_invalid(_T("filesys_remove_directory"));		// �����ȃp�����[�^
		return false;
	}

	// �t�@�C�����X�g�쐬�i�c���[�Ȃ��j
	C_file_find flst;
	flst.find(dir_path, _T("*"), _T("*"), false, true, true);

	// �폜�J�n
	TSTR path;
	for (int i = 0; i < flst.get_file_cnt(); i++)	{
		path = flst.get_full_path(i);
		if (flst[i].is_directory)	{
			// �ċA�Ăяo��
			filesys_remove_directory_with_contain_files(path, is_delete_directory, is_delete_directory, is_delete_read_only);
		}
		else	{
			// �ǂݎ���p��������������
			if (is_delete_read_only)	{
				file_set_read_only(path, false);	
			}
			// �t�@�C���폜
			if (!file_is_read_only(path))	{
				filesys_delete_file(path);
			}
		}
	}

	if (!is_delete_current_directory)	{
		return true;		// �f�B���N�g���͍폜���Ȃ��̂Ő����ł�
	}

	// �ǂݎ���p��������������
	if (is_delete_read_only)	{
		file_set_read_only(dir_path, false);	
	}

	if (file_is_read_only(dir_path))	{
		return false;		// �ǂݎ���p�Ȃ̂ō폜�ł������s�ł�
	}

	// �f�B���N�g���폜
	return filesys_remove_directory(dir_path);
}

// ****************************************************************
// Shell ���g���ăt�@�C�����S�~���Ɉړ�
// ================================================================
bool shell_remove_file(CTSTR& file_path)
{
	SHFILEOPSTRUCT fos;
	ZeroMemory(&fos, sizeof(fos));

	fos.hwnd   = G_app.h_wnd;
	fos.wFunc  = FO_DELETE;				// �폜	
	fos.fFlags = FOF_ALLOWUNDO;			// ���ݔ��Ɉړ�
	fos.pFrom  = file_path.c_str();		// �폜����t�@�C��
	fos.pTo    = NULL;

	if (SHFileOperation(&fos) != 0)	{
		set_last_error_invalid(_T("SHFileOperation"));		// �����ȃp�����[�^
		return false;
	}

	return true;
}

// ****************************************************************
// �t�@�C������ Shell �֐��p�ɘA���i"a.dat\0b.dat\0c.dat\0\0")
// ================================================================
TSTR shell_file_path_connect(ARRAY<TSTR> file_path_list)
{
	TSTR result;

	// �t�@�C�����Ɏg���Ȃ� "?" �Ńt�@�C������A������
	for (ARRAY<TSTR>::iterator itr = file_path_list.begin(); itr != file_path_list.end(); ++itr)
		result += *itr + _T("?");

	// �����ɂ����P�� "?" ������
	result += _T("?");

	// "?" �� "\0" �ɒu������
	for (TSTR::iterator itr = result.begin(); itr != result.end(); ++itr)	{
		if (*itr == _T('?'))
			*itr = _T('\0');
	}

	return result;
}

// ****************************************************************
// WM_DROP ���b�Z�[�W���烊���N����擾
// ================================================================
TSTR get_droped_link_path(WPARAM wp)
{
	// �V���[�g�J�b�g�p�X�̎擾
	TCHAR shortcut_path_t[MAX_PATH];
	int a = DragQueryFile((HDROP)wp, 0, shortcut_path_t, MAX_PATH);
	DragFinish((HDROP)wp);
	TSTR shortcut_path = shortcut_path_t;

	// �����N�p�X�̎擾
	return get_shortcut_link(shortcut_path);
}

// ****************************************************************
// �}�C�h�L�������g�̃p�X���擾
// ================================================================
bool get_my_doument_path(TSTR& my_document_path)
{
	HRESULT hr;

	TCHAR my_document_dir_buf[MAX_PATH] = {0};
	hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, my_document_dir_buf);
	if (hr == E_FAIL)	{
		set_last_error(_T("�p�X�����݂��܂���B"), _T("SHGetFolderPath"));
		return false;
	}
	else if (hr == E_INVALIDARG)	{
		set_last_error(_T("�����ȃp�����[�^�ł��B"), _T("SHGetFolderPath"));
		return false;
	}

	my_document_path = my_document_dir_buf;
	return true;
}

// ****************************************************************
// �V���[�g�J�b�g�̃����N��𓾂�
// ================================================================
TSTR get_shortcut_link(CTSTR& shortcut_path)
{
	HRESULT hr;

	TSTR link_path;			// �����N��p�X
	bool succeed = false;	// �����t���O

	// ShellLink �C���^�[�t�F�[�X�̎擾
	IShellLink* pSL = NULL;
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&pSL);
	if (SUCCEEDED(hr) && hr != S_FALSE) {

		// PersistFile �C���^�[�t�F�[�X�̎擾
		IPersistFile* pPF;
		hr = pSL->QueryInterface(IID_IPersistFile, (void **)&pPF);
		if (SUCCEEDED(hr) && hr != S_FALSE) {

			// �V���[�g�J�b�g�p�X�����C�h������ɕϊ�
			WSTR shortcut_path_w = TSTR_to_WSTR(shortcut_path);

			// �V���[�g�J�b�g�̓ǂݍ���
			hr = pPF->Load(shortcut_path_w.c_str(), STGM_READ);
			if (SUCCEEDED(hr) && hr != S_FALSE) {

				// �����N��̌���
				hr = pSL->Resolve(NULL, SLR_ANY_MATCH);
				if (SUCCEEDED(hr) && hr != S_FALSE) {

					// �����N��̃p�X���擾
					TCHAR link_path_t[MAX_PATH];
					hr = pSL->GetPath(link_path_t, MAX_PATH, NULL, 0);
					if (SUCCEEDED(hr) && hr != S_FALSE) {

						link_path = link_path_t;
						succeed = true;	// ����
					}
				}
			}
			pPF->Release();
		}
		pSL->Release();
	}

	// ���s�����ꍇ�A�V���[�g�J�b�g�p�X�����̂܂܃����N�p�X�ɃR�s�[
	if (!succeed)
		link_path = shortcut_path;

	return link_path;
}

// ****************************************************************
// �v���Z�X���J��
// ================================================================
HANDLE open_process(CTSTR& command, DWORD *process_id)
{
	// CreateProcess �ɓn���\���̂̏�����
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOWNORMAL;

	// �G�f�B�^�̋N��
	TCHAR t_command[4096] = {0};
	_tcscpy_s(t_command, 4096, command.c_str());
	CreateProcess(NULL, t_command, NULL, NULL, false, 0, NULL, NULL, &si, &pi);

	// �v���Z�X�h�c�̎擾
	if (process_id)	{
		*process_id = pi.dwProcessId;
	}

	return pi.hProcess;
}

// ****************************************************************
// �v���Z�X�h�c���炻�̃v���Z�X�����s�����ǂ������肷��
// ================================================================

struct CHECK_PROCESS_FROM_ID_STRUCT
{
	DWORD		process_id;
	bool		find;
};

BOOL CALLBACK check_process_from_id_func(HWND h_wnd, LPARAM lp)
{
	CHECK_PROCESS_FROM_ID_STRUCT *st = (CHECK_PROCESS_FROM_ID_STRUCT *)lp;
	DWORD process_id;
	DWORD thread_id = GetWindowThreadProcessId(h_wnd, &process_id);
	if (process_id == st->process_id)	{
		st->find = true;
		return FALSE;
	}
	return TRUE;
}

bool check_process_from_id(DWORD process_id)
{
	CHECK_PROCESS_FROM_ID_STRUCT st;
	st.process_id = process_id;
	st.find = false;
	EnumWindows((WNDENUMPROC)&check_process_from_id_func, (LPARAM)&st);
	return st.find;
}

// ****************************************************************
// �t�@�C�����֘A�t����ꂽ���W���[���ŊJ��
// ================================================================
bool shell_execute_open(CTSTR& file_path)
{
	SHELLEXECUTEINFO sinfo;

	ZeroMemory(&sinfo, sizeof(SHELLEXECUTEINFO));
	sinfo.cbSize		= sizeof(SHELLEXECUTEINFO);
	sinfo.hwnd			= G_app.h_wnd;
	sinfo.lpVerb		= _T("open");
	sinfo.fMask			= SEE_MASK_NOCLOSEPROCESS;
	sinfo.lpDirectory	= 0;
	sinfo.lpFile		= file_path.c_str();
	sinfo.lpParameters  = 0;
	sinfo.nShow			= SW_SHOWNORMAL;

	if (ShellExecuteEx(&sinfo) == 0)	{
		set_last_error_win(_T("ShellExecuteEx"));
		return false;
	}

	return true;
}

// ****************************************************************
// �t�@�C�������W���[���ŊJ���H�i�������j
// ================================================================
bool shell_execute_open_by_module(CTSTR& file_path, CTSTR& module_path)
{
	SHELLEXECUTEINFO sinfo;

	ZeroMemory(&sinfo, sizeof(SHELLEXECUTEINFO));
	sinfo.cbSize		= sizeof(SHELLEXECUTEINFO);
	sinfo.hwnd			= G_app.h_wnd;
	sinfo.lpVerb		= module_path.c_str();
	sinfo.fMask			= SEE_MASK_NOCLOSEPROCESS;
	sinfo.lpDirectory	= 0;
	sinfo.lpFile		= file_path.c_str();
	sinfo.lpParameters  = 0;
	sinfo.nShow			= SW_SHOWNORMAL;

	if (ShellExecuteEx(&sinfo) == 0)	{
		set_last_error_win(_T("ShellExecuteEx"));
		return false;
	}

	return true;
}

// ****************************************************************
// �G�f�B�^�Ńt�@�C�����J��
//		file_path:		�J���t�@�C��
//		line:			�s�ԍ�
//		editor_path:	�G�f�B�^�̃p�X
// ================================================================

void open_file_by_editor(TSTR file_path, int line, CTSTR& editor_path)
{
	// �t�@�C���p�X���΃p�X�ɕϊ�
	file_path = to_full_path(file_path);

	// �G�f�B�^�p�X���s�t�@�C�����擾
	TSTR editor = get_file_name(editor_path);
	str_to_upper(editor);	// �啶���ɕϊ�

	// �G�f�B�^�̎�ނɂ���āA�R�}���h���C�����Z�b�g
	TCHAR command[MAX_PATH];

	// �}�C�t�F�X�W�΍�
	bool mifes_ver8 = false;

	if (false);
	else if (editor == _T("MIW.EXE"))	{		// �}�C�t�F�X
		TSTR version = file_get_product_version(editor_path);
		if (version.substr(0, 2) == _T("8.") || version.substr(0, 2) == _T("9."))	{
			_stprintf_s(command, MAX_PATH, _T("%s /+%d \"%s\""), editor_path.c_str(), line, file_path.c_str());
			mifes_ver8 = true;
		}
		else	{
			_stprintf_s(command, MAX_PATH, _T("%s \"%s+%d\""), editor_path.c_str(), file_path.c_str(), line);
		}
	}
	else if (editor == _T("SAKURA.EXE"))	{	// �T�N��
		_stprintf_s(command, MAX_PATH, _T("%s \"%s\" -Y:%d"), editor_path.c_str(), file_path.c_str(), line);
	}
	else if (editor == _T("HIDEMARU.EXE"))	{	// �G��
		_stprintf_s(command, MAX_PATH, _T("%s /j%d \"%s\""), editor_path.c_str(), line, file_path.c_str());
	}
	else if (editor == _T("WZEDITOR.EXE"))	{	// �v�y
		_stprintf_s(command, MAX_PATH, _T("%s /j%d \"%s\""), editor_path.c_str(), line, file_path.c_str());
	}
	else if (editor == _T("WZEDIT32.EXE"))	{	// �v�y�R�Q�i�s�����w����@�s���j
		_stprintf_s(command, MAX_PATH, _T("%s \"%s\""), editor_path.c_str(), file_path.c_str());
	}
	else if (editor == _T("K2EDITOR.EXE"))	{	// �j�Q
		_stprintf_s(command, MAX_PATH, _T("%s \"%s\" /j%d"), editor_path.c_str(), file_path.c_str(), line);
	}
	else if (editor == _T("EMEDITOR.EXE"))	{	// �d��
		_stprintf_s(command, MAX_PATH, _T("%s /l %d \"%s\""), editor_path.c_str(), line, file_path.c_str());
	}
	else if (editor == _T("PEGGY.EXE"))	{		// �y�M�[
		_stprintf_s(command, MAX_PATH, _T("%s -L%d \"%s\""), editor_path.c_str(), line, file_path.c_str());
	}
	else if (editor == _T("PEGGYPRO.EXE"))	{	// �y�M�[�v��
		_stprintf_s(command, MAX_PATH, _T("%s -L%d \"%s\""), editor_path.c_str(), line, file_path.c_str());
	}
	else	{		// ���̑�
		return;
	}

	// �G�f�B�^�̍�ƃf�B���N�g����ݒ�
	TSTR work_path = get_dir_path(editor_path);

	// CreateProcess �ɓn���\���̂̏�����
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb          = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOWNORMAL;

	// �G�f�B�^�̋N��
	CreateProcess(NULL, command, NULL, NULL, false, 0, NULL, work_path.c_str(), &si, &pi);

	// �}�C�t�F�X�W�΍�
	if (mifes_ver8)	{	WaitForInputIdle(pi.hProcess, 500);	}	// �N�����ꂽ�v���Z�X���A�C�h����ԁA�������͎w�肳�ꂽ�^�C���A�E�g�܂ő҂�
}

// ****************************************************************
// �u���E�U�Ńt�@�C�����J��
// ================================================================
bool open_file_by_browser(CTSTR& html_path)
{
	// ShellExecute �ŊJ�����ꍇ�AWindows7 & IE8 �ŊJ���Ȃ����Ƃ�����܂��B
	// UAC ���݂��Ǝv���܂��BIE8 �̃o�O�̂悤�ł��B
	// �ڂ����͕����ԍ� 974966: �uShellExecute �֐��� CD �� DVD ��� HTML �t�@�C�����N������� Internet Explorer 8 �������ɏI������v���Q�Ƃ��ĉ������B

	// �����s��
	//::shell_execute_open(html_path);

	// ��ֈĂƂ��āAAssocQueryString �� html �Ɋ֘A�t����ꂽ�A�v���T���ACreateProcess �ŊJ���܂��B
	// ��Q������ ASSOCSTR_COMMAND �ɂ��邱�ƂŁA"" ���܂߂���������擾�ł��܂��B

	//TCHAR browser_path_buf[MAX_PATH] = {0};
	//DWORD str_len = 0;
	//hr = AssocQueryString(ASSOCF_NOTRUNCATE, ASSOCSTR_COMMAND, _T(".htm"), _T("open"), browser_path_buf, &str_len);
	//if (FAILED(hr))
	//	return false;

	// �����łȂ����G���[���o��̂ŁA�Ƃ肠���� FindExecutable �Łc�B
	// ���ꂾ�ƃt���p�X�ɃX�y�[�X�������Ă���Ǝ擾�ł��Ȃ��Ȃǂ̖�肪����c�炵���̂ł����A�Ȃ��������܂����B�C�����ꂽ�̂��ȁH
	TCHAR browser_path_buf[MAX_PATH] = {0};
	HINSTANCE h_inst = FindExecutable(html_path.c_str(), NULL, browser_path_buf);
	if (h_inst < (HINSTANCE)32)	{	// 32 �ȉ��Ŏ��s
		switch ((int)(__int64)h_inst)	{
			case SE_ERR_FNF:		set_last_error(_T("The specified file was not found."), _T("FindExecutable"));						break;
			case SE_ERR_NOASSOC:	set_last_error(_T("There is no association for the specified file type."), _T("FindExecutable"));	break;
			case SE_ERR_OOM:		set_last_error(_T("The system is out of memory or resources."), _T("FindExecutable"));				break;
			default:				set_last_error(_T("�s���ȃG���[�ł��B"), _T("FindExecutable"));										break;
		}
		return false;
	}

	TSTR browser_path = browser_path_buf;

	// �u���E�U���J���܂��B

	// CreateProcess �ɓn���\���̂̏�����
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	si.cb          = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOWNORMAL;
	TSTR command = browser_path + _T(" \"") + html_path + _T("\"");

	// �G�f�B�^�̋N��
	if (!CreateProcess(NULL, (LPWSTR)command.c_str(), NULL, NULL, false, 0, NULL, NULL, &si, &pi))	{
		set_last_error_win(_T("CreateProcess"));
		return false;
	}

	return true;
}

// ****************************************************************
// ��������N���b�v�{�[�h�փR�s�[
// ================================================================
bool str_copy_to_clip_board_in_MB(CTSTR& t_str_)
{
	if (!OpenClipboard(NULL))	return false;
	if (!EmptyClipboard())		return false;

	TSTR t_str = t_str_;

	// ���s�� 0x0a �� 0x0d 0x0a �ɕϊ�
	TSTR str_a, str_da;
	str_a += 0x0a;
	str_da += 0x0d;
	str_da += 0x0a;
	t_str = str_replace_ret(t_str, str_a, str_da);

	// �}���`�o�C�g������ɕϊ�
	MBSTR mb_str = TSTR_to_MBSTR(t_str);

	// �̈���m�ۂ��ĕ�������R�s�[
	HANDLE h_mem = GlobalAlloc(GMEM_MOVEABLE, (mb_str.length() + 1));
	char* p_mem = (char *)GlobalLock(h_mem);
	strcpy_s(p_mem, mb_str.length() + 1, mb_str.c_str());
	GlobalUnlock(h_mem);

	// �N���b�v�{�[�h�ɗ̈��n��
	if (!SetClipboardData(CF_TEXT, h_mem))	{
		CloseClipboard();
		return false;
	}
	
	CloseClipboard();

	// ���d�v��
	// GlobalAlloc �Ŋm�ۂ����������� Windows �ɓn���Ă���̂ŁA
	// ������ GlobalFree �����s���Ă͂����܂���B

	return true;
}

// ****************************************************************
// ��������N���b�v�{�[�h����R�s�[
// ================================================================
TSTR str_copy_from_clip_board_in_MB()
{
	TSTR str;

	if (!OpenClipboard(NULL))	return str;

	HANDLE hd = GetClipboardData(CF_TEXT);
	if (hd)	{
		char *p = (char *)GlobalLock(hd);
		str = MBSTR_to_TSTR(p);
		GlobalUnlock(hd);
//		GlobalFree(hd);			// ����Ȃ��i����Ɨ�����j
	}

	CloseClipboard();
	return str;
}

// ****************************************************************
// ���\�[�X�f�[�^���擾����
// ================================================================
bool resource_get_data(LPCTSTR name, LPCTSTR type, BUFFER& dst_buffer)
{
	HRSRC h_rsc = FindResource(G_app.h_inst, name, type);
	if (!h_rsc)	{
		set_last_error_win(_T("FindResource"));		// Windows �G���[
		return false;
	}

	int size = SizeofResource(G_app.h_inst, h_rsc);
	if (!size)	{
		set_last_error_win(_T("SizeofResource"));	// Windows �G���[
		return false;
	}

	HGLOBAL h_global = LoadResource(G_app.h_inst, h_rsc);
	if (!h_global)	{
		set_last_error_win(_T("LoadResource"));		// Windows �G���[
		return false;
	}

	BYTE* p = (BYTE *)LockResource(h_global);
	if (!p)	{
		set_last_error_win(_T("LockResource"));		// Windows �G���[
		return false;
	}

	dst_buffer.resize(size);
	memcpy(dst_buffer.get(), p, size);

	UnlockResource(h_global);

	return true;
}

// ****************************************************************
// �t�@�C������
// ================================================================
FILEFINDINFO::FILEFINDINFO()
{
	is_directory = false;
	file_size = 0;
}

// ****************************************************************
// �t�@�C������
// ================================================================
C_file_find::C_file_find()
{
	full_size = 0;
}

// ****************************************************************
// �t�@�C�������F�����J�n
// ================================================================
bool C_file_find::find(CTSTR& dir, CTSTR& first_name_, CTSTR& ext_, bool is_tree_, bool is_find_file_, bool is_find_dir_)
{
	first_name = first_name_;
	ext = ext_;
	is_tree = is_tree_;
	is_find_file = is_find_file_;
	is_find_dir = is_find_dir_;

	// �t�@�C�����X�g�̏�����
	file.clear();
	// �����J�n�p�X��ݒ�
	initial_dir = dir + _T("\\");
	// �ċA�I�Ƀt�@�C�����X�g���\�z
	find_main(_T(""));

	return file.size() > 0;
}

// ****************************************************************
// �t�@�C�������F�ċA����
// ================================================================
bool C_file_find::find_main(CTSTR& rltv_dir)
{
	// rltvDir �́A�󕶎���A�������͖����� "\\" �����܂�
	// ex. ""
	// ex. "hoge\\"
	// ex. "hoge\\hage\\"

	HANDLE hd;
	WIN32_FIND_DATA fd;

	// �ŏ��̃t�@�C��������
	TSTR file_filter = initial_dir + rltv_dir + _T("*");
	hd = FindFirstFile(file_filter.c_str(), &fd);
	if (hd != INVALID_HANDLE_VALUE)	{

		// ���Ɍ���
		do	{
			// "." �� ".." �͏��O
			if (_tcscmp(fd.cFileName, _T(".")) != 0 && _tcscmp(fd.cFileName, _T("..")) != 0)	{

				// �����\�z
				FILEFINDINFO info;
				info.is_directory  = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;
				info.rltv_path     = rltv_dir + fd.cFileName;
				info.file_size     = (UINT64)fd.nFileSizeHigh * ((UINT64)MAXDWORD + 1) + fd.nFileSizeLow;
				full_size         += info.file_size;

				// �f�B���N�g���̏ꍇ
				if (info.is_directory)	{
					// ���X�g�ɒǉ�
					if (is_find_dir)
						file.push_back(info);
					// �f�B���N�g����������
					if (is_tree)
						find_main(info.rltv_path + _T("\\"));
				}
				// �t�@�C���̏ꍇ
				else if (!info.is_directory && is_find_file)	{
					// �t�@�C�����e�X�g
					if (first_name == _T("*") || str_to_lower_ret(first_name) == str_to_lower_ret(erase_extension_ret(info.rltv_path)))	{
						if (ext == _T("*") || str_to_lower_ret(ext) == str_to_lower_ret(get_extension(info.rltv_path)))	{
							// ���X�g�ɒǉ�
							file.push_back(info);
						}
					}
				}
			}
		}	while (FindNextFile(hd, &fd));

		// �N���[�Y
		FindClose(hd);
	}

	return true;
}

// ****************************************************************
// �t�@�C�������F�N���A
// ================================================================
void C_file_find::clear()
{
	file.clear();
}

// ****************************************************************
// �t�@�C�������F�t�@�C���̃t���p�X�𓾂�
// ================================================================
TSTR C_file_find::get_full_path(int file_no)
{
	return initial_dir + file[file_no].rltv_path;
}

// ****************************************************************
// �t�@�C���I���_�C�A���O�F�g�p��
// ================================================================
//
//	Cselect_file_dialog sfd;
//	sfd.add_filter(_T("BMP Files"), _T("*.bmp"));
//	sfd.add_filter(_T("PNG Files"), _T("*.png"));
//	sfd.add_filter(_T("JPEG Files"), _T("*.jpg;*.jpeg"));
//	TSTR file_path = sfd.select_open_file(Gapp.h_wnd, _T("�t�@�C�����J��"), _T("�V�K�t�@�C��.bmp"));
//
// ================================================================

// ****************************************************************
// �t�@�C���I���_�C�A���O�F�t�B���^��ǉ�
// ================================================================
//		name: "JPEG Files"
//		type: "*.jpg;*.jpeg"
// ================================================================
void C_select_file_dialog::add_filter(CTSTR& name, CTSTR& type)
{
	filter_name_list.push_back(name);
	filter_type_list.push_back(type);
}

// ****************************************************************
// �t�@�C���I���_�C�A���O�F�t�@�C�����J��
// ================================================================
TSTR C_select_file_dialog::select_open_file(CTSTR& title, CTSTR& def_file_name)
{
	// �f�t�H���g�̃t�@�C�����̃Z�b�g
	TCHAR file_path[MAX_PATH] = {0};
	_tcscpy_s(file_path, MAX_PATH, def_file_name.c_str());

	// �f�t�H���g�̊g���q���Z�b�g
	TSTR def_ext = get_extension(def_file_name);

	// �t�B���^�̍쐬
	TSTR filter;
	int filter_cnt = (int)filter_name_list.size();
	ARRAY<int> null_insert_pos_list;	// null ������ǉ�����ʒu�̃��X�g
	int cur_pos = 0;
	for (int f = 0; f < filter_cnt; f++)	{
		filter += filter_name_list[f] + _T(" ") + filter_type_list[f] + _T(" ");
		null_insert_pos_list.push_back(cur_pos + (int)filter_name_list[f].size());
		null_insert_pos_list.push_back(cur_pos + (int)filter_name_list[f].size() + 1 + (int)filter_type_list[f].size());
		cur_pos += (int)filter_name_list[f].size() + 1 + (int)filter_type_list[f].size() + 1;
	}

	// �t�B���^�� '\0' ��}��
	int null_cnt = (int)null_insert_pos_list.size();
	for (int n = 0; n < null_cnt; n++)	{
		int pos = null_insert_pos_list[n];
		filter[pos] = _T('\0');
	}

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hInstance         = G_app.h_inst;
	ofn.hwndOwner         = G_app.h_wnd;
	ofn.lpstrFilter       = filter.c_str();
	ofn.nFilterIndex      = 0;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter    = 0;
	ofn.lpstrFile         = file_path;		// �t�@�C���p�X���󂯎��o�b�t�@
	ofn.nMaxFile          = MAX_PATH;		// �t�@�C���p�X���󂯎��o�b�t�@�T�C�Y
	ofn.lpstrFileTitle    = NULL;			// �t�@�C�������󂯎��o�b�t�@
	ofn.nMaxFileTitle     = 0;				// �t�@�C�������󂯎��o�b�t�@�T�C�Y
	ofn.lpstrInitialDir   = NULL;			// NULL = �J�����g�f�B���N�g��
	ofn.lpstrTitle        = title.c_str();	// �^�C�g��
	ofn.Flags             = OFN_EXPLORER;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = def_ext.c_str();	// �f�t�H���g�̊g���q
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;

	if (!GetOpenFileName(&ofn))
		return _T("");

	return file_path;
}

TSTR C_select_file_dialog::select_save_file(CTSTR& title, CTSTR& def_file_name)
{
	// �f�t�H���g�̃t�@�C�����̃Z�b�g
	TCHAR file_path[MAX_PATH] = {0};
	_tcscpy_s(file_path, MAX_PATH, def_file_name.c_str());

	// �f�t�H���g�̊g���q���Z�b�g
	TSTR def_ext = get_extension(def_file_name);

	// �t�B���^�̍쐬
	int filter_cnt = (int)filter_name_list.size();
	TSTR filter;
	ARRAY<int> null_insert_pos_list;
	int cur_pos = 0;
	for (int f = 0; f < filter_cnt; f++)	{
		filter += filter_name_list[f] + _T(" ") + filter_type_list[f] + _T(" ");
		null_insert_pos_list.push_back(cur_pos + (int)filter_name_list[f].size());
		null_insert_pos_list.push_back(cur_pos + (int)filter_name_list[f].size() + 1 + (int)filter_type_list[f].size());
		cur_pos += (int)filter_name_list[f].size() + 1 + (int)filter_type_list[f].size() + 1;
	}

	// �t�B���^�� '\0' ��}��
	int null_cnt = (int)null_insert_pos_list.size();
	for (int n = 0; n < null_cnt; n++)	{
		int pos = null_insert_pos_list[n];
		filter[pos] = _T('\0');
	}

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hInstance         = G_app.h_inst;
	ofn.hwndOwner         = G_app.h_wnd;
	ofn.lpstrFilter       = filter.c_str();
	ofn.nFilterIndex      = 0;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter    = 0;
	ofn.lpstrFile         = file_path;		// �t�@�C���p�X���󂯎��o�b�t�@
	ofn.nMaxFile          = MAX_PATH;		// �t�@�C���p�X���󂯎��o�b�t�@�T�C�Y
	ofn.lpstrFileTitle    = NULL;			// �t�@�C�������󂯎��o�b�t�@
	ofn.nMaxFileTitle     = 0;				// �t�@�C�������󂯎��o�b�t�@�T�C�Y
	ofn.lpstrInitialDir   = NULL;			// NULL = �J�����g�f�B���N�g��
	ofn.lpstrTitle        = title.c_str();	// �^�C�g��
	ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = def_ext.c_str();	// �f�t�H���g�̊g���q
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;

	if (!GetSaveFileName(&ofn))
		return _T("");

	return file_path;
}

// ****************************************************************
// �A�C�e���h�c���X�g�F�p�X����\�z
// ================================================================
LPITEMIDLIST iil_create_from_dir_path(CTSTR& dir_path)
{
	LPITEMIDLIST iil = NULL;
	HRESULT hr;

	// ���[�g�p�X����A�C�e���h�c���擾
	LPSHELLFOLDER desktop = NULL;
	if (::SHGetDesktopFolder(&desktop) == NOERROR)	{
		
		ULONG eaten, attribute;
		OLECHAR dir_path_w[MAX_PATH];	// const �łȂ� wchar_t* ���K�v���ۂ�
		wcscpy_s(dir_path_w, MAX_PATH, TSTR_to_WSTR(dir_path).c_str());
		hr = desktop->ParseDisplayName(NULL, NULL, dir_path_w, &eaten, &iil, &attribute);
		if (hr != NOERROR)
			iil = NULL;

		desktop->Release();
	}

	return iil;
}

// ****************************************************************
// �A�C�e���h�c���X�g�F���
// ================================================================
void iil_release(LPITEMIDLIST* iil)
{
	if (*iil)
		::CoTaskMemFree(*iil);
}

// ****************************************************************
// �A�C�e���h�c���X�g�F�p�X�̎擾
// ================================================================
TSTR iil_get_dir_path(LPITEMIDLIST iil)
{
	TCHAR dir_path[MAX_PATH + 1];
	::SHGetPathFromIDList(iil, dir_path);

	return dir_path;
}

// ****************************************************************
// �A�C�e���h�c���X�g�F�R���X�g���N�^
// ================================================================
Citem_id_list::Citem_id_list(CTSTR& dir_path)
{
	iil = iil_create_from_dir_path(dir_path);
}

// ****************************************************************
// �A�C�e���h�c���X�g�F�f�X�g���N�^
// ================================================================
Citem_id_list::~Citem_id_list()
{
	release();
}

// ****************************************************************
// �A�C�e���h�c���X�g�F���
// ================================================================
void Citem_id_list::release()
{
	iil_release(&iil);
}

// ****************************************************************
// �A�C�e���h�c���X�g�F�{�̂̎擾
// ================================================================
LPITEMIDLIST Citem_id_list::get()
{
	return iil;
}

// ****************************************************************
// �t�H���_�I���_�C�A���O�F�R�[���o�b�N�֐��ɓn�����
// ================================================================
struct Sselect_directory_info
{
	TSTR	window_title;
	TSTR	initial_path;
};

// ****************************************************************
// �t�H���_�I���_�C�A���O�F�R�[���o�b�N�֐�
// ================================================================
int CALLBACK select_directory_proc(HWND h_wnd, UINT msg, LPARAM lp, LPARAM data)
{
	LPITEMIDLIST iil = (LPITEMIDLIST)lp;

	Sselect_directory_info* sdi = (Sselect_directory_info *)data;
	Citem_id_list iil_initial(sdi->initial_path);

	switch (msg)	{
		case BFFM_INITIALIZED:

			// �E�B���h�E�^�C�g���̐ݒ�
			::SetWindowText(h_wnd, sdi->window_title.c_str());
			// �����p�X�̐ݒ�
			::SendMessage(h_wnd, BFFM_SETSELECTION, FALSE, (LPARAM)iil_initial.get());
			break;

		case BFFM_SELCHANGED:
			{
				TSTR dir_path = iil_get_dir_path(iil);

				// �X�^�e�B�b�N�R���g���[���Ɏ��܂�悤�Ƀp�X���k������i... ��t������j
				HWND h_wnd_stc = ::FindWindowEx(h_wnd, 0, _T("static"), NULL);
				if (h_wnd_stc)	{
					C_rect stc_rect;
					::GetWindowRect(h_wnd_stc, &stc_rect);	// �k�����邽�߂ɃR���g���[���̕����擾
					if (stc_rect.width() > 0)	{
						TCHAR dir_path_buf[MAX_PATH] = {0};
						memcpy(dir_path_buf, dir_path.c_str(), dir_path.size() * sizeof(TCHAR));
						HDC h_dc = ::GetDC(h_wnd);
						::PathCompactPath(h_dc, dir_path_buf, stc_rect.width());	// �p�X���k������
						dir_path = dir_path_buf;
						::ReleaseDC(h_wnd, h_dc);
					}
				}
				SendMessage(h_wnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)dir_path.c_str());
			}
			break;
	}
    return 0;
}

// ****************************************************************
// �t�H���_�I���_�C�A���O
//		root_path:		���[�g�p�X�i���̃p�X�ȉ������I���ł��Ȃ��j	_T("") �Ńf�X�N�g�b�v
//		initial_path:	������ԂőI������Ă���p�X
//		window_title:	�E�B���h�E�^�C�g��
//		info_str:		�E�B���h�E�o�[�̉��ɕ\������镶����
// ================================================================
TSTR select_directory(CTSTR& root_path, CTSTR& initial_path, CTSTR& window_title, CTSTR& info_str)
{
	TCHAR dir_path_t[MAX_PATH] = {0};
	TSTR dir_path;

	// ���[�g�p�X����A�C�e���h�c���擾
	Citem_id_list iil_root(root_path);
	PTR iil_root_ptr = (PTR)iil_root.get();
	if (root_path.empty())
		iil_root_ptr = CSIDL_DESKTOP;

	// �t�H���_�I���_�C�A���O�ɓn���p�����[�^�\����
	Sselect_directory_info sdi;
	sdi.window_title = window_title;
	sdi.initial_path = initial_path;

	// �t�H���_�I���_�C�A���O�̏���ݒ�
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = G_app.h_wnd;
	bi.pidlRoot = (ITEMIDLIST *)iil_root_ptr;
	bi.pszDisplayName = dir_path_t;
	bi.lpszTitle = info_str.c_str();
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;// | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	bi.lpfn = select_directory_proc;
	bi.lParam = (LPARAM)&sdi;

	// �t�H���_�I���_�C�A���O�����s
	LPITEMIDLIST iil = ::SHBrowseForFolder(&bi);
	if (iil != NULL)	{

		// �A�C�e���h�c���X�g����t���p�X���󂯎��
		dir_path = iil_get_dir_path(iil);
		// �A�C�e���h�c���X�g������I
		iil_release(&iil);
	}

	return dir_path;
}


}
