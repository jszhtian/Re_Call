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
// ファイルパスからファイル名を得る
//		"aaa\\bbb\\ccc.dat" → "ccc.dat"
// ================================================================
TSTR get_file_name(CTSTR& path)
{
	int pos = (int)path.rfind('\\');
	if (pos == TSTR::npos)
		return path;

	return TSTR(path, pos + 1);
}

// ****************************************************************
// ファイルパスからディレクトリパスを得る
//		"aaa\\bbb\\ccc.dat" → "aaa\\bbb"
// ================================================================
TSTR get_dir_path(CTSTR& path)
{
	int pos = (int)path.rfind('\\');
	if (pos == TSTR::npos)
		return path;

	return TSTR(path, 0, pos);
}

// ****************************************************************
// １つ上のディレクトリパスを得る
//		"aaa\\bbb\\ccc" → "aaa\\bbb"
// ================================================================
TSTR get_upper_dir_path(CTSTR& path)
{
	int pos = (int)path.rfind('\\');
	if (pos == TSTR::npos)
		return path;

	return TSTR(path, 0, pos);
}

// ****************************************************************
// 拡張子を取得
//		"aaa\\bbb\\ccc.dat" → "dat"
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
// 拡張子を判定
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
// 拡張子を変更したパスを取得
//		"aaa\\bbb\\ccc.dat" → "aaa\\bbb\\ccc.ext"
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
// 拡張子を削除したパスを取得
//		"aaa\\bbb\\ccc.dat" → "aaa\\bbb\\ccc"
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
// ディレクトリの作成（ファイルパスから）
//		"aaa\bbb.txt" → "aaa" を作成
// ================================================================

#if 1

bool create_dir_from_file_path(CTSTR& file_path)
{
	// 最後のファイルをのぞいてディレクトリパスにします。
	TSTR dir_path = get_dir_path(file_path);

	return create_dir_from_dir_path(dir_path);
}

#else

bool create_dir_from_file_path(CTSTR& file_path)
{
	if (file_path.empty())
		return true;

	// MakeSureDirectoryPathExists は、ワイド文字版がないっぽい
	MBSTR mb_str = TSTR_to_MBSTR(file_path);
	if (!MakeSureDirectoryPathExists(mb_str.c_str()))	{
		set_last_error_win(_T("MakeSureDirectoryPathExists"));		// Windows エラー
		return false;
	}

	return true;
}

#endif

// ****************************************************************
// ディレクトリの作成（ディレクトリパスから）
//		"aaa\bbb" → "aaa\bbb" を作成
// ================================================================

#if 1

bool create_dir_from_dir_path(CTSTR& dir_path)
{
	int error_code;

	// ファイルの存在判定
	// 既にディレクトりがある場合は、成功を返します。
	// 既に同名のファイルがある場合は、SHCreateDirectoryEx がエラーを返すことになります。
	if (file_get_type(dir_path) == 0)
		return true;	// 既に存在するので成功。

	// 絶対パスに直す
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

	// 末尾が '\\' でない場合、'\\' をつける
	TSTR file_path = dir_path;
	if (file_path[file_path.length() - 1] != '\\')
		file_path += _T("\\");

	return create_dir_from_file_path(file_path);
}

#endif

// ****************************************************************
// ２つのパスを比較する
// ================================================================
void erase_illegal_yen(TCHAR* str)
{
	TCHAR* p = str;
	TCHAR* q = str;

	for (; *p != _T('\0'); )	{

		// \\ が２つ続いた場合は p のみを進める
		if (*p == _T('\\') && *(p + 1) == _T('\\'))	{				
			p ++;
		}
		// 末尾の \\ だった場合は終了
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
	// 小文字に直す
	str_to_lower(lhs);
	str_to_lower(rhs);

	// . や .. を補正する
	TCHAR lhs_buf[MAX_PATH] = {0};
	TCHAR rhs_buf[MAX_PATH] = {0};
	::PathCanonicalize(lhs_buf, lhs.c_str());
	::PathCanonicalize(rhs_buf, rhs.c_str());

	// 連続した \\ や末尾の \\ を取る
	erase_illegal_yen(lhs_buf);
	erase_illegal_yen(rhs_buf);

	// 比較する
	return _tcscmp(lhs_buf, rhs_buf) == 0;
}

// ****************************************************************
// ファイルの種類を調べる
//		-1: 存在しない
//		 0: ディレクトリ
//		 1: ファイル
// ================================================================
int file_get_type(CTSTR& path)
{
	DWORD attr = GetFileAttributes(path.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES)
		return -1;		// 存在しない

	// ファイルの種類を調べる
	if (attr & FILE_ATTRIBUTE_DIRECTORY)	return 0;	// ディレクトリ
	else									return 1;	// ファイル;
}

// ****************************************************************
// ファイルのサイズを調べる
// ================================================================
UINT64 file_get_size(CTSTR& path)
{
	WIN32_FILE_ATTRIBUTE_DATA fd;
	if (!GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &fd))
		return 0;

	// ファイルサイズを取得
	UINT64 size = ((UINT64)fd.nFileSizeHigh << 32) + fd.nFileSizeLow;
	return size;
}

// ****************************************************************
// ファイルの属性を判定
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
// ファイルの属性を設定
// ================================================================
bool file_set_read_only(CTSTR& file_path, bool onoff)
{
	DWORD attr = ::GetFileAttributes(file_path.c_str());	// 属性を取得
	if (onoff)	attr |= FILE_ATTRIBUTE_READONLY;			// on
	else		attr &= ~FILE_ATTRIBUTE_READONLY;			// off

	return ::SetFileAttributes(file_path.c_str(), attr) > 0;
}

bool file_set_hidden(CTSTR& file_path, bool onoff)
{
	DWORD attr = ::GetFileAttributes(file_path.c_str());	// 属性を取得
	if (onoff)	attr |= FILE_ATTRIBUTE_HIDDEN;				// on
	else		attr &= ~FILE_ATTRIBUTE_HIDDEN;				// off

	return ::SetFileAttributes(file_path.c_str(), attr) > 0;
}

bool file_set_system(CTSTR& file_path, bool onoff)
{
	DWORD attr = ::GetFileAttributes(file_path.c_str());	// 属性を取得
	if (onoff)	attr |= FILE_ATTRIBUTE_SYSTEM;				// on
	else		attr &= ~FILE_ATTRIBUTE_SYSTEM;				// off

	return ::SetFileAttributes(file_path.c_str(), attr) > 0;
}

// ****************************************************************
// ファイルタイムを取得
//		create:		作成日時
//		access:		アクセス日時
//		write:		更新日時
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
	CloseHandle(h_file);	// 閉じるの忘れないでね

	return true;
}

// ****************************************************************
// ファイルタイムを設定
//		create:		作成日時
//		access:		アクセス日時
//		write:		更新日時
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
	CloseHandle(h_file);	// 閉じるの忘れないでね

	return true;
}

// ****************************************************************
// ファイルの更新日時を比較
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
// ファイルのプロダクトバージョンを取得
// ================================================================

// バージョンを受け取るための構造体
struct GFV_LANGANDCODEPAGE
{
	WORD language;
	WORD codepage;
};

// バージョンを受け取る
TSTR file_get_product_version(CTSTR& file_path)
{
	// バージョン情報のサイズを取得する
	DWORD info_handle = 0;	// ハンドルって言ってるけど必ず 0 が返ってくるそうです。
	DWORD info_size = GetFileVersionInfoSize(file_path.c_str(), &info_handle);
	if (info_size <= 0)
		return _T("");

	bool ret = false;

	// バージョン情報を取得する
	ARRAY<BYTE> info(info_size);
	GetFileVersionInfo(file_path.c_str(), info_handle, info_size, info.get());

	// バージョン情報のロケールを取得する
	GFV_LANGANDCODEPAGE* lang_p = NULL;
	UINT lang_size = 0;
	VerQueryValue(info.get(), _T("\\VarFileInfo\\Translation"), (void **)&lang_p, &lang_size);
	if (lang_size < sizeof(GFV_LANGANDCODEPAGE))
		return _T("");

	// 製品バージョンを取得する
	TCHAR* produce_version_p = NULL;
	UINT produce_version_size = 0;
	TSTR block_str = str_format(_T("\\StringFileInfo\\%04x%04x\\ProductVersion"), lang_p[0].language, lang_p[0].codepage);
	VerQueryValue(info.get(), (TCHAR *)block_str.c_str(), (VOID **)&produce_version_p, &produce_version_size);
	if (produce_version_size == 0)
		return _T("");

	return produce_version_p;
}

// ****************************************************************
// レジストリ処理関数
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
// レジストリのキーが存在するかチェック
//		失敗：get_my_last_error
// ================================================================
bool reg_check_key(HKEY root, CTSTR& path)
{
	DWORD error_code = ERROR_SUCCESS;

	HKEY h_key = NULL;
	if ((error_code = RegOpenKeyEx(root, path.c_str(), 0, KEY_READ, &h_key)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegOpenKeyEx"));		// Windows エラー
		return false;
	}

	BSP<C_registry_key> key(new C_registry_key(h_key));

	return true;	// 存在する
}

// ****************************************************************
// レジストリのキーがサブキーを持つかをチェック
//		失敗：get_my_last_error
// ================================================================
int reg_check_sub_key(HKEY root, CTSTR& path)
{
	DWORD error_code = ERROR_SUCCESS;

	HKEY h_key = NULL;
	error_code = RegOpenKeyEx(root, path.c_str(), 0, KEY_READ, &h_key);
	if (error_code != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegOpenKeyEx"));		// Windows エラー
		return -1;	// 失敗
	}
	BSP<C_registry_key> key(new C_registry_key(h_key));

	TCHAR buf[256];
	DWORD buf_len = 0;
	FILETIME ft = {0};
	error_code = RegEnumKeyEx(h_key, 0, buf, &buf_len, 0, NULL, NULL, &ft);
	if (error_code != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegEnumKeyEx"));		// Windows エラー
		return -1;	// 失敗
	}

	if (error_code == ERROR_NO_MORE_ITEMS)
		return 0;	// サブキーがない
	
	return 1;	// サブキーがある
}

// ****************************************************************
// レジストリのキーを削除する
//		失敗：get_my_last_error
// ================================================================
bool reg_delete_key(HKEY root, CTSTR& path)
{
	DWORD error_code = ERROR_SUCCESS;

	HKEY h_key = NULL;
	if ((error_code = RegOpenKeyEx(root, path.c_str(), 0, KEY_READ, &h_key)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegOpenKeyEx"));		// Windows エラー
		return false;
	}

	BSP<C_registry_key> key(new C_registry_key(h_key));

	// レジストリを削除
	if ((error_code = RegDeleteKey(root, path.c_str())) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegDeleteKey"));		// Windows エラー
		return false;
	}

	return true;
}

// ****************************************************************
// レジストリの値を取得する（文字列）
//		失敗：get_my_last_error
// ================================================================
bool reg_get_value_str(HKEY root, CTSTR& path, CTSTR& name, TSTR& str)
{
	DWORD error_code = ERROR_SUCCESS;

	HKEY h_key = NULL;
	if ((error_code = RegOpenKeyEx(root, path.c_str(), 0, KEY_READ, &h_key)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegOpenKeyEx"));		// Windows エラー
		return false;
	}

	BSP<C_registry_key> key(new C_registry_key(h_key));
	
	DWORD type = 0;
	BYTE buffer[1024];
	DWORD buffer_size = 1024;
	if ((error_code = RegQueryValueEx(h_key, name.c_str(), 0, &type, buffer, &buffer_size)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegQueryValueEx"));	// Windows エラー
		return false;
	}

	str = TSTR((TCHAR *)buffer, (TCHAR *)(buffer + buffer_size - 2));	// -2: [\0]
	return true;
}

// ****************************************************************
// レジストリの値を設定する（文字列）
// ================================================================
bool reg_set_value_str(HKEY root, CTSTR& path, CTSTR& name, CTSTR& str)
{
	DWORD error_code = ERROR_SUCCESS;

	// レジストリを開く
	HKEY h_key = NULL;
	if ((error_code = RegCreateKeyEx(root, path.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &h_key, NULL)) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("RegCreateKeyEx"));		// Windows エラー
		return false;
	}

	BSP<C_registry_key> key(new C_registry_key(h_key));

	// 値を書き込む
	if ((error_code = RegSetValueEx(h_key, name.c_str(), 0, REG_SZ, (const BYTE *)str.c_str(), (DWORD)(str.size() + 1) * sizeof(TCHAR))) != ERROR_SUCCESS)	{
		set_last_error_win(error_code, _T("Cregistry_key"));		// Windows エラー
		return false;
	}

	return true;
}

// ****************************************************************
// ファイルをコピー（WinAPI バージョン）
// ================================================================
bool filesys_copy_file(CTSTR& from, CTSTR& to)
{
	if (!::CopyFile(from.c_str(), to.c_str(), FALSE))	{
		set_last_error_win(_T("CopyFile"));		// Windows エラー
		return false;
	}

	return true;
}

// ****************************************************************
// ファイルをリネーム（WinAPI バージョン）
// ================================================================
bool filesys_rename_file(CTSTR& from, CTSTR& to)
{
	if (!::MoveFile(from.c_str(), to.c_str()))	{
		set_last_error_win(_T("MoveFile"));		// Windows エラー
		return false;
	}

	return true;
}

// ****************************************************************
// ファイルを削除（WinAPI バージョン）
// ================================================================
bool filesys_delete_file(CTSTR& file_path)
{
	if (!::DeleteFile(file_path.c_str()))	{
		set_last_error_win(_T("DeleteFile"));	// Windows エラー
		return false;
	}

	return true;
}

// ****************************************************************
// ディレクトリを削除（WinAPI バージョン）
// ================================================================
bool filesys_remove_directory(CTSTR& dir_path)
{
	if (!::RemoveDirectory(dir_path.c_str()))	{
		set_last_error_win(_T("RemoveDirectory"));	// Windows エラー
		return false;
	}

	return true;
}

// ****************************************************************
// ディレクトリをコピー（中に入っているファイル及びディレクトリもコピーする）
// ================================================================
bool filesys_copy_directory_with_contain_files(CTSTR& src_path, CTSTR& dst_path, bool is_cancel_read_only, bool is_contain_directory)
{
	// コピー元とコピー先が同じ
	if (str_to_upper_ret(src_path) == str_to_upper_ret(dst_path))	{
		set_last_error_invalid(_T("filesys_copy_directory"));		// 無効なパラメータ
		return false;
	}

	// コピー元が存在しない
	if (file_get_type(src_path) != 0)	{
		set_last_error_invalid(_T("filesys_copy_directory"));		// 無効なパラメータ
		return false;
	}

	// コピー先が存在しない
	if (file_get_type(dst_path) != 0)	{
		set_last_error_invalid(_T("filesys_copy_directory"));		// 無効なパラメータ
		return false;
	}

	// ファイルリスト作成（ツリーなし）
	C_file_find flst;
	flst.find(src_path, _T("*"), _T("*"), false, true, is_contain_directory);

	// コピー開始
	TSTR src_file_path;
	TSTR dst_file_path;
	for (int i = 0; i < flst.get_file_cnt(); i++)	{
		src_file_path = src_path + _T("\\") + flst[i].rltv_path;
		dst_file_path = dst_path + _T("\\") + flst[i].rltv_path;
		if (flst[i].is_directory)	{
			// ディレクトリ作成
			::MakeSureDirectoryPathExists(TSTR_to_MBSTR(dst_file_path + _T("\\")).c_str());
			// 再帰呼び出し
			filesys_copy_directory_with_contain_files(src_file_path, dst_file_path, is_cancel_read_only, is_contain_directory);
		}
		else	{
			// コピーする
			filesys_copy_file(src_file_path, dst_file_path);
			// 読み取り専用属性を解除する
			if (is_cancel_read_only)
				file_set_read_only(dst_file_path, false);
		}
	}

	return true;
}

// ****************************************************************
// ディレクトリを削除（中に入っているファイル及びディレクトリも削除する）
// ================================================================
bool filesys_remove_directory_with_contain_files(CTSTR& dir_path, bool is_delete_current_directory, bool is_delete_directory, bool is_delete_read_only)
{
	// 安全確認：３文字以下はパスじゃない
	if ((int)dir_path.size() <= 3)	{
		set_last_error_invalid(_T("filesys_remove_directory"));		// 無効なパラメータ
		return false;
	}

	// 安全確認：２文字目の文字を確認
	if (dir_path[1] != _T(':'))	{
		set_last_error_invalid(_T("filesys_remove_directory"));		// 無効なパラメータ
		return false;
	}

	// 安全確認：３文字目の文字を確認
	if (dir_path[2] != _T('\\'))	{
		set_last_error_invalid(_T("filesys_remove_directory"));		// 無効なパラメータ
		return false;
	}

	// 安全確認：念のためディレクトリの存在を確認する
	if (file_get_type(dir_path) != 0)	{
		set_last_error_invalid(_T("filesys_remove_directory"));		// 無効なパラメータ
		return false;
	}

	// ファイルリスト作成（ツリーなし）
	C_file_find flst;
	flst.find(dir_path, _T("*"), _T("*"), false, true, true);

	// 削除開始
	TSTR path;
	for (int i = 0; i < flst.get_file_cnt(); i++)	{
		path = flst.get_full_path(i);
		if (flst[i].is_directory)	{
			// 再帰呼び出し
			filesys_remove_directory_with_contain_files(path, is_delete_directory, is_delete_directory, is_delete_read_only);
		}
		else	{
			// 読み取り専用属性を解除する
			if (is_delete_read_only)	{
				file_set_read_only(path, false);	
			}
			// ファイル削除
			if (!file_is_read_only(path))	{
				filesys_delete_file(path);
			}
		}
	}

	if (!is_delete_current_directory)	{
		return true;		// ディレクトリは削除しないので成功です
	}

	// 読み取り専用属性を解除する
	if (is_delete_read_only)	{
		file_set_read_only(dir_path, false);	
	}

	if (file_is_read_only(dir_path))	{
		return false;		// 読み取り専用なので削除できず失敗です
	}

	// ディレクトリ削除
	return filesys_remove_directory(dir_path);
}

// ****************************************************************
// Shell を使ってファイルをゴミ箱に移動
// ================================================================
bool shell_remove_file(CTSTR& file_path)
{
	SHFILEOPSTRUCT fos;
	ZeroMemory(&fos, sizeof(fos));

	fos.hwnd   = G_app.h_wnd;
	fos.wFunc  = FO_DELETE;				// 削除	
	fos.fFlags = FOF_ALLOWUNDO;			// ごみ箱に移動
	fos.pFrom  = file_path.c_str();		// 削除するファイル
	fos.pTo    = NULL;

	if (SHFileOperation(&fos) != 0)	{
		set_last_error_invalid(_T("SHFileOperation"));		// 無効なパラメータ
		return false;
	}

	return true;
}

// ****************************************************************
// ファイル名を Shell 関数用に連結（"a.dat\0b.dat\0c.dat\0\0")
// ================================================================
TSTR shell_file_path_connect(ARRAY<TSTR> file_path_list)
{
	TSTR result;

	// ファイル名に使えない "?" でファイル名を連結する
	for (ARRAY<TSTR>::iterator itr = file_path_list.begin(); itr != file_path_list.end(); ++itr)
		result += *itr + _T("?");

	// 末尾にもう１個 "?" をつける
	result += _T("?");

	// "?" を "\0" に置換する
	for (TSTR::iterator itr = result.begin(); itr != result.end(); ++itr)	{
		if (*itr == _T('?'))
			*itr = _T('\0');
	}

	return result;
}

// ****************************************************************
// WM_DROP メッセージからリンク先を取得
// ================================================================
TSTR get_droped_link_path(WPARAM wp)
{
	// ショートカットパスの取得
	TCHAR shortcut_path_t[MAX_PATH];
	int a = DragQueryFile((HDROP)wp, 0, shortcut_path_t, MAX_PATH);
	DragFinish((HDROP)wp);
	TSTR shortcut_path = shortcut_path_t;

	// リンクパスの取得
	return get_shortcut_link(shortcut_path);
}

// ****************************************************************
// マイドキュメントのパスを取得
// ================================================================
bool get_my_doument_path(TSTR& my_document_path)
{
	HRESULT hr;

	TCHAR my_document_dir_buf[MAX_PATH] = {0};
	hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, my_document_dir_buf);
	if (hr == E_FAIL)	{
		set_last_error(_T("パスが存在しません。"), _T("SHGetFolderPath"));
		return false;
	}
	else if (hr == E_INVALIDARG)	{
		set_last_error(_T("無効なパラメータです。"), _T("SHGetFolderPath"));
		return false;
	}

	my_document_path = my_document_dir_buf;
	return true;
}

// ****************************************************************
// ショートカットのリンク先を得る
// ================================================================
TSTR get_shortcut_link(CTSTR& shortcut_path)
{
	HRESULT hr;

	TSTR link_path;			// リンク先パス
	bool succeed = false;	// 成功フラグ

	// ShellLink インターフェースの取得
	IShellLink* pSL = NULL;
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&pSL);
	if (SUCCEEDED(hr) && hr != S_FALSE) {

		// PersistFile インターフェースの取得
		IPersistFile* pPF;
		hr = pSL->QueryInterface(IID_IPersistFile, (void **)&pPF);
		if (SUCCEEDED(hr) && hr != S_FALSE) {

			// ショートカットパスをワイド文字列に変換
			WSTR shortcut_path_w = TSTR_to_WSTR(shortcut_path);

			// ショートカットの読み込み
			hr = pPF->Load(shortcut_path_w.c_str(), STGM_READ);
			if (SUCCEEDED(hr) && hr != S_FALSE) {

				// リンク先の検索
				hr = pSL->Resolve(NULL, SLR_ANY_MATCH);
				if (SUCCEEDED(hr) && hr != S_FALSE) {

					// リンク先のパスを取得
					TCHAR link_path_t[MAX_PATH];
					hr = pSL->GetPath(link_path_t, MAX_PATH, NULL, 0);
					if (SUCCEEDED(hr) && hr != S_FALSE) {

						link_path = link_path_t;
						succeed = true;	// 成功
					}
				}
			}
			pPF->Release();
		}
		pSL->Release();
	}

	// 失敗した場合、ショートカットパスをそのままリンクパスにコピー
	if (!succeed)
		link_path = shortcut_path;

	return link_path;
}

// ****************************************************************
// プロセスを開く
// ================================================================
HANDLE open_process(CTSTR& command, DWORD *process_id)
{
	// CreateProcess に渡す構造体の初期化
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOWNORMAL;

	// エディタの起動
	TCHAR t_command[4096] = {0};
	_tcscpy_s(t_command, 4096, command.c_str());
	CreateProcess(NULL, t_command, NULL, NULL, false, 0, NULL, NULL, &si, &pi);

	// プロセスＩＤの取得
	if (process_id)	{
		*process_id = pi.dwProcessId;
	}

	return pi.hProcess;
}

// ****************************************************************
// プロセスＩＤからそのプロセスが実行中かどうか判定する
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
// ファイルを関連付けられたモジュールで開く
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
// ファイルをモジュールで開く？（未完成）
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
// エディタでファイルを開く
//		file_path:		開くファイル
//		line:			行番号
//		editor_path:	エディタのパス
// ================================================================

void open_file_by_editor(TSTR file_path, int line, CTSTR& editor_path)
{
	// ファイルパスを絶対パスに変換
	file_path = to_full_path(file_path);

	// エディタパス実行ファイル名取得
	TSTR editor = get_file_name(editor_path);
	str_to_upper(editor);	// 大文字に変換

	// エディタの種類によって、コマンドラインをセット
	TCHAR command[MAX_PATH];

	// マイフェス８対策
	bool mifes_ver8 = false;

	if (false);
	else if (editor == _T("MIW.EXE"))	{		// マイフェス
		TSTR version = file_get_product_version(editor_path);
		if (version.substr(0, 2) == _T("8.") || version.substr(0, 2) == _T("9."))	{
			_stprintf_s(command, MAX_PATH, _T("%s /+%d \"%s\""), editor_path.c_str(), line, file_path.c_str());
			mifes_ver8 = true;
		}
		else	{
			_stprintf_s(command, MAX_PATH, _T("%s \"%s+%d\""), editor_path.c_str(), file_path.c_str(), line);
		}
	}
	else if (editor == _T("SAKURA.EXE"))	{	// サクラ
		_stprintf_s(command, MAX_PATH, _T("%s \"%s\" -Y:%d"), editor_path.c_str(), file_path.c_str(), line);
	}
	else if (editor == _T("HIDEMARU.EXE"))	{	// 秀丸
		_stprintf_s(command, MAX_PATH, _T("%s /j%d \"%s\""), editor_path.c_str(), line, file_path.c_str());
	}
	else if (editor == _T("WZEDITOR.EXE"))	{	// ＷＺ
		_stprintf_s(command, MAX_PATH, _T("%s /j%d \"%s\""), editor_path.c_str(), line, file_path.c_str());
	}
	else if (editor == _T("WZEDIT32.EXE"))	{	// ＷＺ３２（行数が指定方法不明）
		_stprintf_s(command, MAX_PATH, _T("%s \"%s\""), editor_path.c_str(), file_path.c_str());
	}
	else if (editor == _T("K2EDITOR.EXE"))	{	// Ｋ２
		_stprintf_s(command, MAX_PATH, _T("%s \"%s\" /j%d"), editor_path.c_str(), file_path.c_str(), line);
	}
	else if (editor == _T("EMEDITOR.EXE"))	{	// Ｅｍ
		_stprintf_s(command, MAX_PATH, _T("%s /l %d \"%s\""), editor_path.c_str(), line, file_path.c_str());
	}
	else if (editor == _T("PEGGY.EXE"))	{		// ペギー
		_stprintf_s(command, MAX_PATH, _T("%s -L%d \"%s\""), editor_path.c_str(), line, file_path.c_str());
	}
	else if (editor == _T("PEGGYPRO.EXE"))	{	// ペギープロ
		_stprintf_s(command, MAX_PATH, _T("%s -L%d \"%s\""), editor_path.c_str(), line, file_path.c_str());
	}
	else	{		// その他
		return;
	}

	// エディタの作業ディレクトリを設定
	TSTR work_path = get_dir_path(editor_path);

	// CreateProcess に渡す構造体の初期化
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb          = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOWNORMAL;

	// エディタの起動
	CreateProcess(NULL, command, NULL, NULL, false, 0, NULL, work_path.c_str(), &si, &pi);

	// マイフェス８対策
	if (mifes_ver8)	{	WaitForInputIdle(pi.hProcess, 500);	}	// 起動されたプロセスがアイドル状態、もしくは指定されたタイムアウトまで待つ
}

// ****************************************************************
// ブラウザでファイルを開く
// ================================================================
bool open_file_by_browser(CTSTR& html_path)
{
	// ShellExecute で開いた場合、Windows7 & IE8 で開けないことがあります。
	// UAC 絡みだと思います。IE8 のバグのようです。
	// 詳しくは文書番号 974966: 「ShellExecute 関数で CD や DVD 上の HTML ファイルを起動すると Internet Explorer 8 がすぐに終了する」を参照して下さい。

	// ↓失敗例
	//::shell_execute_open(html_path);

	// 代替案として、AssocQueryString で html に関連付けられたアプリ探し、CreateProcess で開きます。
	// 第２引数を ASSOCSTR_COMMAND にすることで、"" を含めた文字列を取得できます。

	//TCHAR browser_path_buf[MAX_PATH] = {0};
	//DWORD str_len = 0;
	//hr = AssocQueryString(ASSOCF_NOTRUNCATE, ASSOCSTR_COMMAND, _T(".htm"), _T("open"), browser_path_buf, &str_len);
	//if (FAILED(hr))
	//	return false;

	// ▲↑でなぜかエラーが出るので、とりあえず FindExecutable で…。
	// これだとフルパスにスペースが入っていると取得できないなどの問題がある…らしいのですが、なぜか動きました。修正されたのかな？
	TCHAR browser_path_buf[MAX_PATH] = {0};
	HINSTANCE h_inst = FindExecutable(html_path.c_str(), NULL, browser_path_buf);
	if (h_inst < (HINSTANCE)32)	{	// 32 以下で失敗
		switch ((int)(__int64)h_inst)	{
			case SE_ERR_FNF:		set_last_error(_T("The specified file was not found."), _T("FindExecutable"));						break;
			case SE_ERR_NOASSOC:	set_last_error(_T("There is no association for the specified file type."), _T("FindExecutable"));	break;
			case SE_ERR_OOM:		set_last_error(_T("The system is out of memory or resources."), _T("FindExecutable"));				break;
			default:				set_last_error(_T("不明なエラーです。"), _T("FindExecutable"));										break;
		}
		return false;
	}

	TSTR browser_path = browser_path_buf;

	// ブラウザを開きます。

	// CreateProcess に渡す構造体の初期化
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	si.cb          = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOWNORMAL;
	TSTR command = browser_path + _T(" \"") + html_path + _T("\"");

	// エディタの起動
	if (!CreateProcess(NULL, (LPWSTR)command.c_str(), NULL, NULL, false, 0, NULL, NULL, &si, &pi))	{
		set_last_error_win(_T("CreateProcess"));
		return false;
	}

	return true;
}

// ****************************************************************
// 文字列をクリップボードへコピー
// ================================================================
bool str_copy_to_clip_board_in_MB(CTSTR& t_str_)
{
	if (!OpenClipboard(NULL))	return false;
	if (!EmptyClipboard())		return false;

	TSTR t_str = t_str_;

	// 改行を 0x0a → 0x0d 0x0a に変換
	TSTR str_a, str_da;
	str_a += 0x0a;
	str_da += 0x0d;
	str_da += 0x0a;
	t_str = str_replace_ret(t_str, str_a, str_da);

	// マルチバイト文字列に変換
	MBSTR mb_str = TSTR_to_MBSTR(t_str);

	// 領域を確保して文字列をコピー
	HANDLE h_mem = GlobalAlloc(GMEM_MOVEABLE, (mb_str.length() + 1));
	char* p_mem = (char *)GlobalLock(h_mem);
	strcpy_s(p_mem, mb_str.length() + 1, mb_str.c_str());
	GlobalUnlock(h_mem);

	// クリップボードに領域を渡す
	if (!SetClipboardData(CF_TEXT, h_mem))	{
		CloseClipboard();
		return false;
	}
	
	CloseClipboard();

	// ※重要※
	// GlobalAlloc で確保したメモリは Windows に渡しているので、
	// ここで GlobalFree を実行してはいけません。

	return true;
}

// ****************************************************************
// 文字列をクリップボードからコピー
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
//		GlobalFree(hd);			// いらない（あると落ちる）
	}

	CloseClipboard();
	return str;
}

// ****************************************************************
// リソースデータを取得する
// ================================================================
bool resource_get_data(LPCTSTR name, LPCTSTR type, BUFFER& dst_buffer)
{
	HRSRC h_rsc = FindResource(G_app.h_inst, name, type);
	if (!h_rsc)	{
		set_last_error_win(_T("FindResource"));		// Windows エラー
		return false;
	}

	int size = SizeofResource(G_app.h_inst, h_rsc);
	if (!size)	{
		set_last_error_win(_T("SizeofResource"));	// Windows エラー
		return false;
	}

	HGLOBAL h_global = LoadResource(G_app.h_inst, h_rsc);
	if (!h_global)	{
		set_last_error_win(_T("LoadResource"));		// Windows エラー
		return false;
	}

	BYTE* p = (BYTE *)LockResource(h_global);
	if (!p)	{
		set_last_error_win(_T("LockResource"));		// Windows エラー
		return false;
	}

	dst_buffer.resize(size);
	memcpy(dst_buffer.get(), p, size);

	UnlockResource(h_global);

	return true;
}

// ****************************************************************
// ファイル検索
// ================================================================
FILEFINDINFO::FILEFINDINFO()
{
	is_directory = false;
	file_size = 0;
}

// ****************************************************************
// ファイル検索
// ================================================================
C_file_find::C_file_find()
{
	full_size = 0;
}

// ****************************************************************
// ファイル検索：検索開始
// ================================================================
bool C_file_find::find(CTSTR& dir, CTSTR& first_name_, CTSTR& ext_, bool is_tree_, bool is_find_file_, bool is_find_dir_)
{
	first_name = first_name_;
	ext = ext_;
	is_tree = is_tree_;
	is_find_file = is_find_file_;
	is_find_dir = is_find_dir_;

	// ファイルリストの初期化
	file.clear();
	// 検索開始パスを設定
	initial_dir = dir + _T("\\");
	// 再帰的にファイルリストを構築
	find_main(_T(""));

	return file.size() > 0;
}

// ****************************************************************
// ファイル検索：再帰検索
// ================================================================
bool C_file_find::find_main(CTSTR& rltv_dir)
{
	// rltvDir は、空文字列、もくしは末尾に "\\" がつきます
	// ex. ""
	// ex. "hoge\\"
	// ex. "hoge\\hage\\"

	HANDLE hd;
	WIN32_FIND_DATA fd;

	// 最初のファイルを検索
	TSTR file_filter = initial_dir + rltv_dir + _T("*");
	hd = FindFirstFile(file_filter.c_str(), &fd);
	if (hd != INVALID_HANDLE_VALUE)	{

		// 順に検索
		do	{
			// "." や ".." は除外
			if (_tcscmp(fd.cFileName, _T(".")) != 0 && _tcscmp(fd.cFileName, _T("..")) != 0)	{

				// 情報を構築
				FILEFINDINFO info;
				info.is_directory  = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;
				info.rltv_path     = rltv_dir + fd.cFileName;
				info.file_size     = (UINT64)fd.nFileSizeHigh * ((UINT64)MAXDWORD + 1) + fd.nFileSizeLow;
				full_size         += info.file_size;

				// ディレクトリの場合
				if (info.is_directory)	{
					// リストに追加
					if (is_find_dir)
						file.push_back(info);
					// ディレクトリ内を検索
					if (is_tree)
						find_main(info.rltv_path + _T("\\"));
				}
				// ファイルの場合
				else if (!info.is_directory && is_find_file)	{
					// ファイル名テスト
					if (first_name == _T("*") || str_to_lower_ret(first_name) == str_to_lower_ret(erase_extension_ret(info.rltv_path)))	{
						if (ext == _T("*") || str_to_lower_ret(ext) == str_to_lower_ret(get_extension(info.rltv_path)))	{
							// リストに追加
							file.push_back(info);
						}
					}
				}
			}
		}	while (FindNextFile(hd, &fd));

		// クローズ
		FindClose(hd);
	}

	return true;
}

// ****************************************************************
// ファイル検索：クリア
// ================================================================
void C_file_find::clear()
{
	file.clear();
}

// ****************************************************************
// ファイル検索：ファイルのフルパスを得る
// ================================================================
TSTR C_file_find::get_full_path(int file_no)
{
	return initial_dir + file[file_no].rltv_path;
}

// ****************************************************************
// ファイル選択ダイアログ：使用例
// ================================================================
//
//	Cselect_file_dialog sfd;
//	sfd.add_filter(_T("BMP Files"), _T("*.bmp"));
//	sfd.add_filter(_T("PNG Files"), _T("*.png"));
//	sfd.add_filter(_T("JPEG Files"), _T("*.jpg;*.jpeg"));
//	TSTR file_path = sfd.select_open_file(Gapp.h_wnd, _T("ファイルを開く"), _T("新規ファイル.bmp"));
//
// ================================================================

// ****************************************************************
// ファイル選択ダイアログ：フィルタを追加
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
// ファイル選択ダイアログ：ファイルを開く
// ================================================================
TSTR C_select_file_dialog::select_open_file(CTSTR& title, CTSTR& def_file_name)
{
	// デフォルトのファイル名のセット
	TCHAR file_path[MAX_PATH] = {0};
	_tcscpy_s(file_path, MAX_PATH, def_file_name.c_str());

	// デフォルトの拡張子をセット
	TSTR def_ext = get_extension(def_file_name);

	// フィルタの作成
	TSTR filter;
	int filter_cnt = (int)filter_name_list.size();
	ARRAY<int> null_insert_pos_list;	// null 文字を追加する位置のリスト
	int cur_pos = 0;
	for (int f = 0; f < filter_cnt; f++)	{
		filter += filter_name_list[f] + _T(" ") + filter_type_list[f] + _T(" ");
		null_insert_pos_list.push_back(cur_pos + (int)filter_name_list[f].size());
		null_insert_pos_list.push_back(cur_pos + (int)filter_name_list[f].size() + 1 + (int)filter_type_list[f].size());
		cur_pos += (int)filter_name_list[f].size() + 1 + (int)filter_type_list[f].size() + 1;
	}

	// フィルタに '\0' を挿入
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
	ofn.lpstrFile         = file_path;		// ファイルパスを受け取るバッファ
	ofn.nMaxFile          = MAX_PATH;		// ファイルパスを受け取るバッファサイズ
	ofn.lpstrFileTitle    = NULL;			// ファイル名を受け取るバッファ
	ofn.nMaxFileTitle     = 0;				// ファイル名を受け取るバッファサイズ
	ofn.lpstrInitialDir   = NULL;			// NULL = カレントディレクトリ
	ofn.lpstrTitle        = title.c_str();	// タイトル
	ofn.Flags             = OFN_EXPLORER;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = def_ext.c_str();	// デフォルトの拡張子
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;

	if (!GetOpenFileName(&ofn))
		return _T("");

	return file_path;
}

TSTR C_select_file_dialog::select_save_file(CTSTR& title, CTSTR& def_file_name)
{
	// デフォルトのファイル名のセット
	TCHAR file_path[MAX_PATH] = {0};
	_tcscpy_s(file_path, MAX_PATH, def_file_name.c_str());

	// デフォルトの拡張子をセット
	TSTR def_ext = get_extension(def_file_name);

	// フィルタの作成
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

	// フィルタに '\0' を挿入
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
	ofn.lpstrFile         = file_path;		// ファイルパスを受け取るバッファ
	ofn.nMaxFile          = MAX_PATH;		// ファイルパスを受け取るバッファサイズ
	ofn.lpstrFileTitle    = NULL;			// ファイル名を受け取るバッファ
	ofn.nMaxFileTitle     = 0;				// ファイル名を受け取るバッファサイズ
	ofn.lpstrInitialDir   = NULL;			// NULL = カレントディレクトリ
	ofn.lpstrTitle        = title.c_str();	// タイトル
	ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = def_ext.c_str();	// デフォルトの拡張子
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;

	if (!GetSaveFileName(&ofn))
		return _T("");

	return file_path;
}

// ****************************************************************
// アイテムＩＤリスト：パスから構築
// ================================================================
LPITEMIDLIST iil_create_from_dir_path(CTSTR& dir_path)
{
	LPITEMIDLIST iil = NULL;
	HRESULT hr;

	// ルートパスからアイテムＩＤを取得
	LPSHELLFOLDER desktop = NULL;
	if (::SHGetDesktopFolder(&desktop) == NOERROR)	{
		
		ULONG eaten, attribute;
		OLECHAR dir_path_w[MAX_PATH];	// const でない wchar_t* が必要っぽい
		wcscpy_s(dir_path_w, MAX_PATH, TSTR_to_WSTR(dir_path).c_str());
		hr = desktop->ParseDisplayName(NULL, NULL, dir_path_w, &eaten, &iil, &attribute);
		if (hr != NOERROR)
			iil = NULL;

		desktop->Release();
	}

	return iil;
}

// ****************************************************************
// アイテムＩＤリスト：解放
// ================================================================
void iil_release(LPITEMIDLIST* iil)
{
	if (*iil)
		::CoTaskMemFree(*iil);
}

// ****************************************************************
// アイテムＩＤリスト：パスの取得
// ================================================================
TSTR iil_get_dir_path(LPITEMIDLIST iil)
{
	TCHAR dir_path[MAX_PATH + 1];
	::SHGetPathFromIDList(iil, dir_path);

	return dir_path;
}

// ****************************************************************
// アイテムＩＤリスト：コンストラクタ
// ================================================================
Citem_id_list::Citem_id_list(CTSTR& dir_path)
{
	iil = iil_create_from_dir_path(dir_path);
}

// ****************************************************************
// アイテムＩＤリスト：デストラクタ
// ================================================================
Citem_id_list::~Citem_id_list()
{
	release();
}

// ****************************************************************
// アイテムＩＤリスト：解放
// ================================================================
void Citem_id_list::release()
{
	iil_release(&iil);
}

// ****************************************************************
// アイテムＩＤリスト：本体の取得
// ================================================================
LPITEMIDLIST Citem_id_list::get()
{
	return iil;
}

// ****************************************************************
// フォルダ選択ダイアログ：コールバック関数に渡す情報
// ================================================================
struct Sselect_directory_info
{
	TSTR	window_title;
	TSTR	initial_path;
};

// ****************************************************************
// フォルダ選択ダイアログ：コールバック関数
// ================================================================
int CALLBACK select_directory_proc(HWND h_wnd, UINT msg, LPARAM lp, LPARAM data)
{
	LPITEMIDLIST iil = (LPITEMIDLIST)lp;

	Sselect_directory_info* sdi = (Sselect_directory_info *)data;
	Citem_id_list iil_initial(sdi->initial_path);

	switch (msg)	{
		case BFFM_INITIALIZED:

			// ウィンドウタイトルの設定
			::SetWindowText(h_wnd, sdi->window_title.c_str());
			// 初期パスの設定
			::SendMessage(h_wnd, BFFM_SETSELECTION, FALSE, (LPARAM)iil_initial.get());
			break;

		case BFFM_SELCHANGED:
			{
				TSTR dir_path = iil_get_dir_path(iil);

				// スタティックコントロールに収まるようにパスを縮小する（... を付加する）
				HWND h_wnd_stc = ::FindWindowEx(h_wnd, 0, _T("static"), NULL);
				if (h_wnd_stc)	{
					C_rect stc_rect;
					::GetWindowRect(h_wnd_stc, &stc_rect);	// 縮小するためにコントロールの幅を取得
					if (stc_rect.width() > 0)	{
						TCHAR dir_path_buf[MAX_PATH] = {0};
						memcpy(dir_path_buf, dir_path.c_str(), dir_path.size() * sizeof(TCHAR));
						HDC h_dc = ::GetDC(h_wnd);
						::PathCompactPath(h_dc, dir_path_buf, stc_rect.width());	// パスを縮小する
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
// フォルダ選択ダイアログ
//		root_path:		ルートパス（このパス以下しか選択できない）	_T("") でデスクトップ
//		initial_path:	初期状態で選択されているパス
//		window_title:	ウィンドウタイトル
//		info_str:		ウィンドウバーの下に表示される文字列
// ================================================================
TSTR select_directory(CTSTR& root_path, CTSTR& initial_path, CTSTR& window_title, CTSTR& info_str)
{
	TCHAR dir_path_t[MAX_PATH] = {0};
	TSTR dir_path;

	// ルートパスからアイテムＩＤを取得
	Citem_id_list iil_root(root_path);
	PTR iil_root_ptr = (PTR)iil_root.get();
	if (root_path.empty())
		iil_root_ptr = CSIDL_DESKTOP;

	// フォルダ選択ダイアログに渡すパラメータ構造体
	Sselect_directory_info sdi;
	sdi.window_title = window_title;
	sdi.initial_path = initial_path;

	// フォルダ選択ダイアログの情報を設定
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = G_app.h_wnd;
	bi.pidlRoot = (ITEMIDLIST *)iil_root_ptr;
	bi.pszDisplayName = dir_path_t;
	bi.lpszTitle = info_str.c_str();
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;// | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	bi.lpfn = select_directory_proc;
	bi.lParam = (LPARAM)&sdi;

	// フォルダ選択ダイアログを実行
	LPITEMIDLIST iil = ::SHBrowseForFolder(&bi);
	if (iil != NULL)	{

		// アイテムＩＤリストからフルパスを受け取る
		dir_path = iil_get_dir_path(iil);
		// アイテムＩＤリストを解放！
		iil_release(&iil);
	}

	return dir_path;
}


}
