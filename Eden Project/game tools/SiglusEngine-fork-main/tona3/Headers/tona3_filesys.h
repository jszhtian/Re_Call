#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"
#include	"tona3_stream.h"

#include	<shlobj.h>

namespace NT3
{

// ****************************************************************
// ファイル名の文字列に関する操作
// ================================================================
TSTR	get_file_name(CTSTR& file_path);					// フルパスからファイル名を取得（"aaa\\bbb\\ccc.dat" → "ccc.dat"）
TSTR	get_dir_path(CTSTR& file_path);						// フルパスからディレクトリパスを取得（"aaa\\bbb\\ccc.dat" → "aaa\\bbb"）
TSTR	get_upper_dir_path(CTSTR& dir_path);				// １つ上に移動したディレクトリパスを取得（"aaa\\bbb\\ccc" → "aaa\\bbb"）
TSTR	get_extension(CTSTR& file_path);					// 拡張子を取得（"aaa\\bbb\\ccc.dat" → "dat"）
bool	check_extension(CTSTR& file_path, CTSTR& ext);		// 拡張子を判定（"aaa\\bbb\\ccc.dat" == "dat"）
void	change_extension(TSTR& file_path, CTSTR& ext);		// 拡張子を変更したパスを取得（"aaa\\bbb\\ccc.dat" → "aaa\\bbb\\ccc.ext"）
TSTR	change_extension_ret(CTSTR& file_path, CTSTR& ext);	// 拡張子を変更したパスを取得（"aaa\\bbb\\ccc.dat" → "aaa\\bbb\\ccc.ext"）（戻り値で取得）
void	erase_extension(TSTR& file_path);					// 拡張子を削除したパスを取得（"aaa\\bbb\\ccc.dat" → "aaa\\bbb\\ccc"）
TSTR	erase_extension_ret(CTSTR& file_path);				// 拡張子を削除したパスを取得（"aaa\\bbb\\ccc.dat" → "aaa\\bbb\\ccc"）（戻り値で取得）
bool	compare_file_path(TSTR lhs, TSTR rhs);				// ２つのパスを比較する

// ****************************************************************
// ファイルの属性などに関する操作
// ================================================================
int		file_get_type(CTSTR& file_path);					// ファイルの種類を取得（-1:存在しない、0:ディレクトリ、1:ファイル）
UINT64	file_get_size(CTSTR& file_path);					// ファイルのサイズを取得
bool	file_is_read_only(CTSTR& file_path);				// 読み取り専用を判定
bool	file_set_read_only(CTSTR& file_path, bool onoff);	// 読み取り専用を設定
bool	file_is_hidden(CTSTR& file_path);					// 隠しファイルを判定
bool	file_set_hidden(CTSTR& file_path, bool onoff);		// 隠しファイルを設定
bool	file_is_system(CTSTR& file_path);					// システムファイルを判定
bool	file_set_system(CTSTR& file_path, bool onoff);		// システムファイルを設定
bool	file_get_time(CTSTR& file_path, FILETIME* create, FILETIME* access, FILETIME* write);	// ファイル日時を取得
bool	file_set_time(CTSTR& file_path, FILETIME* create, FILETIME* access, FILETIME* write);	// ファイル日時を設定
int		file_cmp_write_time(CTSTR& file_path_1, CTSTR& file_path_2);	// 更新日時を比較（-1:左が古い 0:等しい 1:左が新しい）
TSTR	file_get_product_version(CTSTR& file_path);

// ****************************************************************
// レジストリ操作関数
// ================================================================
bool	reg_check_key(HKEY root, CTSTR& path);								// レジストリのキーが存在するかチェック
int		reg_check_sub_key(HKEY root, CTSTR& path);							// レジストリのキーがサブキーを持つかをチェック
bool	reg_delete_key(HKEY root, CTSTR& path);								// レジストリのキーを削除する
bool	reg_set_value_str(HKEY root, CTSTR& path, CTSTR& name, CTSTR& str);	// レジストリの値を設定する（文字列）
bool	reg_get_value_str(HKEY root, CTSTR& path, CTSTR& name, TSTR& str);	// レジストリの値を取得する（文字列）

// ****************************************************************
// ファイルシステム関連の操作
// ================================================================
bool	filesys_copy_file(CTSTR& from, CTSTR& to);		// WinAPI の CopyFile を使ってファイルをコピー（単体ファイルのみ）
bool	filesys_rename_file(CTSTR& from, CTSTR& to);	// WinAPI の MoveFile を使ってファイルをコピー（単体ファイルのみ）
bool	filesys_delete_file(CTSTR& file_path);			// WinAPI の DeleteFile を使ってファイルを削除（単体ファイルのみ）
bool	filesys_remove_directory(CTSTR& dir_path);		// WinAPI の RemoveDirectory を使ってディレクトリを削除（空ディレクトリのみ）
bool	filesys_copy_directory_with_contain_files(CTSTR& src_path, CTSTR& dst_path, bool is_cancel_read_only, bool is_contain_directory);		// ディレクトリをコピー（中に入っているファイル及びディレクトリもコピーする）
bool	filesys_remove_directory_with_contain_files(CTSTR& dir_path, bool is_delete_current_directory, bool is_delete_directory, bool is_delete_read_only);		// ディレクトリを削除（中に入っているファイル及びディレクトリも削除する）
bool	shell_remove_file(CTSTR& file_path);					// Shell を使ってファイルをごみ箱に移動（ファイル名に注意！ 必ず shell_file_name_connect を使ってください）
TSTR	shell_file_path_connect(ARRAY<TSTR> file_path_list);	// ファイル名を Shell 関数用に連結（"a.dat\0b.dat\0c.dat\0\0")

// ↓便利関数
bool	create_dir_from_file_path(CTSTR& file_path);	// 一気にディレクトリ作成（"aaa\\bbb\\ccc.dat" → "aaa\\bbb" を作成）
bool	create_dir_from_dir_path(CTSTR& dir_path);		// 一気にディレクトリ作成（"aaa\\bbb" → "aaa\\bbb" を作成）

// ****************************************************************
// シェルとかプロセスとか色々
// ================================================================
TSTR	get_droped_link_path(WPARAM wp);										// WM_DROPFILE からファイル情報を取得
TSTR	get_shortcut_link(CTSTR& shortcut_path);								// ショートカットからリンク先のファイルを取得
bool	get_my_doument_path(TSTR& my_document_path);							// マイドキュメントのパスを取得
HANDLE	open_process(CTSTR& command, DWORD *process_id = NULL);					// プロセスを開く
bool	check_process_from_id(DWORD process_id);								// プロセスＩＤからそのプロセスが実行中かどうか判定する
bool	shell_execute_open(CTSTR& file_path);									// ファイルを関連付けられたモジュールで開く
bool	shell_execute_open_by_module(CTSTR& file_path, CTSTR& module_path);		// ファイルをモジュールで開く？（未完成）
void	open_file_by_editor(TSTR txt_path, int line, CTSTR& editor_path);		// エディタでテキストファイルを開く
bool	open_file_by_browser(CTSTR& html_path);									// ブラウザでＨＴＭＬファイルを開く
bool	open_dir(CTSTR& dir_path);												// エクスプローラでフォルダを開く
bool	str_copy_to_clip_board_in_MB(CTSTR& str);								// 文字列をクリップボードへコピー
TSTR	str_copy_from_clip_board_in_MB();										// 文字列をクリップボードから取得
bool	resource_get_data(LPCTSTR name, LPCTSTR type, BUFFER& dst_buffer);		// リソースを取得する

// ****************************************************************
// ファイル検索構造体
// ================================================================
struct FILEFINDINFO
{
	FILEFINDINFO();
	TSTR	rltv_path;		// 検索スタート位置からの相対パス
	bool	is_directory;	// ディレクトリ
	UINT64	file_size;		// ファイルサイズ
};

// ****************************************************************
// ファイル検索
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
	FFLIST	file;			// ファイルリスト
	UINT64	full_size;		// 総ファイルサイズ
	TSTR	initial_dir;	// 検索スタート位置
	TSTR	first_name;		// 検索するファイル名
	TSTR	ext;			// 検索する拡張子
	bool	is_tree;		// 再帰的に検索する
	bool	is_find_file;	// ファイルを検索する
	bool	is_find_dir;	// ディレクトリを検索する
};

// ****************************************************************
// ファイル関連のダイアログ
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
// フォルダ選択ダイアログ
// ================================================================
TSTR select_directory(CTSTR& root_path, CTSTR& initial_path, CTSTR& window_title, CTSTR& info_str);

// ****************************************************************
// アイテムＩＤリスト
// ================================================================
LPITEMIDLIST	iil_create_from_dir_path(CTSTR& dir_path);
void			iil_release(LPITEMIDLIST* iil);
TSTR			iil_get_dir_path(LPITEMIDLIST iil);

// ****************************************************************
// アイテムＩＤリスト管理クラス
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
