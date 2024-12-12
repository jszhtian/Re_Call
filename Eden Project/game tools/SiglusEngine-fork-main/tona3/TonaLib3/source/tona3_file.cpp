#include	"tona3_pch.h"
#include	"tona3_file.h"
#include	"tona3_util.h"
#include	"tona3_app.h"
#include	"tona3_error.h"

namespace NT3
{

// ****************************************************************
// fopen のラッパー
// ================================================================
//		戻り値：成功 = true、失敗 = false
// ================================================================
bool file_open(FILE** fp, const TSTR& file_path, const TSTR& mode)
{
	if (fp == NULL)	{
		set_last_error_invalid(_T("file_open"));	// 無効なパラメータ
		return false;
	}

	// fopen
	int error_no = _tfopen_s(fp, file_path.c_str(), mode.c_str());
	if (error_no != 0)	{
		set_last_error_crt(_T("fopen(\"") + mode + _T("\")"));		// CRT エラー
		return false;
	}

	return true;
}

// ****************************************************************
// fclose のラッパー
// ================================================================
//		戻り値：成功 = true、失敗 = false
// ================================================================
bool file_close(FILE* fp)
{
	if (fp == NULL)
		return true;	// 成功

	// fclose
	int res = fclose(fp);
	if (res == EOF)	{
		set_last_error_crt(_T("fclose"));	// CRT エラー（★取得できない！？要調査！）
		return false;
	}

	return true;
}

// ****************************************************************
// fread のラッパー
// ================================================================
//		戻り値：読み込んだバイト数
// ================================================================
int file_read(void* dst, int size, FILE* fp)
{
	clear_last_error();		// エラーをクリアする

	if (fp == NULL)	{
		set_last_error(_T("ファイルが開かれていません。"), _T("file_read"));
		return 0;
	}
	if (dst == NULL)	{
		set_last_error_invalid(_T("file_read"));	// 無効なパラメータ
		return 0;
	}
	if (size < 0)	{
		set_last_error_invalid(_T("file_read"));	// 無効なパラメータ
		return 0;
	}

	// fread
	int read_size = (int)fread(dst, 1, size, fp);
	if (read_size < size)	{
		if (ferror(fp))	{
			set_last_error_crt(_T("fread"));		// CRT エラー
		}
	}
	
	return read_size;
}

// ****************************************************************
// fwrite のラッパー
// ================================================================
//		戻り値：書き込んだバイト数
// ================================================================
int file_write(const void* src, int size, FILE* fp)
{
	if (fp == NULL)	{
		set_last_error(_T("ファイルが開かれていません。"), _T("file_write"));
		return 0;
	}
	if (src == NULL)	{
		set_last_error_invalid(_T("file_write"));	// 無効なパラメータ
		return 0;
	}
	if (size < 0)	{
		set_last_error_invalid(_T("file_write"));	// 無効なパラメータ
		return 0;
	}

	// fwrite
	int write_size = (int)fwrite(src, 1, size, fp);
	if (write_size < size)	{
		set_last_error_crt(_T("fwrite"));			// CRT エラー
	}

	return write_size;
}

// ****************************************************************
// fseek のラッパー
// ================================================================
//		戻り値：成功 = true、失敗 = false
// ================================================================
bool file_seek(FILE* fp, __int64 offset, int origin)
{
	if (fp == NULL)	{
		set_last_error(_T("ファイルが開かれていません。"), _T("file_seek"));
		return false;
	}

	int res = _fseeki64(fp, offset, origin);
	if (res != 0)	{
		set_last_error_crt(_T("fseek"));			// CRT エラー
		return false;
	}

	return true;
}

// ****************************************************************
// ftell のラッパー
// ================================================================
//		戻り値：成功 = true、失敗 = false
// ================================================================
__int64 file_tell(FILE* fp)
{
	clear_last_error();		// エラーをクリアする

	if (fp == NULL)	{
		set_last_error(_T("ファイルが開かれていません。"), _T("file_tell"));
		return 0;
	}

	return _ftelli64(fp);
}

// ****************************************************************
// feof のラッパー
// ================================================================
//		戻り値：終端 = true、終端でないまたはエラー = false
// ================================================================
bool file_eof(FILE* fp)
{
	clear_last_error();		// エラーをクリアする

	if (fp == NULL)	{
		set_last_error(_T("ファイルが開かれていません。"), _T("file_eof"));	// 無効なパラメータ
		return false;
	}

	// feof
	if (!feof(fp))	{
		return false;	// eof ではない（成功）
	}
	else	{
		return true;	// eof である（成功）
	}
}

// ****************************************************************
// ファイル：コンストラクタ
// ================================================================
C_file::C_file()
{
	fp = NULL;
	size = 0;
}

// ****************************************************************
// ファイル：デストラクタ
// ================================================================
C_file::~C_file()
{
	close();
}

// ****************************************************************
// ファイル：ファイルを開く
// ================================================================
bool C_file::open(CTSTR& file_path, CTSTR& mode)
{
	// ファイルを閉じる
	if (!close())
		return false;

	// ファイルを開く
	if (!file_open(&fp, file_path, mode))
		return false;

	// ファイル位置を終端に
	if (!file_seek(fp, 0, SEEK_END))
		return false;

	// ファイルサイズを取得
	size = file_tell(fp);

	// ファイル位置を元に戻す
	if (!file_seek(fp, 0, SEEK_SET))
		return false;

	return true;
}

// ****************************************************************
// ファイル：ファイルを閉じる
// ================================================================
bool C_file::close()
{
	// ファイルを閉じる
	if (!file_close(fp))
		return false;

	fp = NULL;
	size = 0;

	return true;
}

// ****************************************************************
// ファイル：読み込み
// ================================================================
int C_file::read(void* buf, int size)
{
	// ファイルを読み込む
	return file_read(buf, size, fp);
}

bool C_file::read_in_MB_with_len(TSTR& t_str)
{
	t_str.clear();

	int read_size;

	// 長さを読み込む
	int len = 0;
	read_size = read(&len, sizeof(int));
	if (read_size < sizeof(int))	{
		set_last_error(_T("文字列の長さが取得できませんでした。"), _T("read_in_MB_with_len"));
		return false;
	}

	// 長さ０の場合は空文字列を返して終了
	if (len <= 0)	{
		t_str = _T("");
		return true;
	}

	// 長さだけ文字列を読み込む
	ARRAY<char> mb_str(len);
	read_size = read(mb_str.get(), len);
	if (read_size < len)	{
		set_last_error(_T("文字列の長さが不正です。"), _T("read_in_MB_with_len"));
		return false;
	}

	// TSTR に変換
	t_str = MBSTR_to_TSTR(mb_str.get(), len);

	return true;
}

// ****************************************************************
// ファイル：書き込み
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

	write(&len, sizeof(int));					// 長さを書き込む
	write(mb_str.c_str(), (int)mb_str.size());	// 文字列を書き込む
}

// ****************************************************************
// ファイル：シーク位置移動
// ================================================================
bool C_file::seek(__int64 offset, int origin)
{
	return file_seek(fp, offset, origin);
}

// ****************************************************************
// ファイル：シーク位置取得
// ================================================================
__int64 C_file::tell()
{
	return file_tell(fp);
}

// ****************************************************************
// ファイル：データを一括読み込み（バイナリ）
// ================================================================
bool C_file::read_full_data(CTSTR& file_path, ARRAY<BYTE>& buf)
{
	buf.clear();

	// ファイルを開く
	C_file file;
	if (!file.open(file_path, _T("rb")))
		return false;

	// ファイルデータの全読み込み
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

	// ファイルを開く
	C_file file;
	if (!file.open(file_path, _T("rb")))
		return false;

	// ファイルデータの全読み込み
	int file_size = (int)file.get_size();
	buf.resize(file_size);
	if (file.read(buf.get(), file_size) < file_size)
		return false;

	file.close();
	return true;
}

// ****************************************************************
// ファイル：データを一括読み込み（テキスト）
// ================================================================
bool C_file::read_full_text(CTSTR& file_path, TSTR& str)
{
	str.clear();

	C_file file;
	if (!file.open(file_path, _T("rt")))
		return false;		// エラー処理済

	// ファイルデータの全読み込み
	ARRAY<BYTE> buf((int)file.get_size());					// ここでのサイズはバイナリでのサイズ
	int read_cnt = file.read(buf.get(), (int)buf.size());	// ここで取得できるのはテキストでのサイズ
	buf.resize(read_cnt);									// テキストでのサイズに調整

	// 文字列に変換
	str = MBSTR_to_TSTR(buf.get(), (int)buf.size());

	return true;
}

// ****************************************************************
// ファイル：データを一括読み込み（UTF-8 テキスト）
// ================================================================
bool C_file::read_full_text_UTF8(CTSTR& file_path, TSTR& str)
{
	str.clear();

	C_file file;
	if (!file.open(file_path, _T("rt")))
		return false;		// エラー処理済

	// ファイルデータの全読み込み
	ARRAY<BYTE> buf((int)file.get_size());					// ここでのサイズはバイナリでのサイズ
	int read_cnt = file.read(buf.get(), (int)buf.size());	// ここで取得できるのはテキストでのサイズ
	buf.resize(read_cnt);									// テキストでのサイズに調整

	// BOM 処理
	int bom_byte = 0;
	if (read_cnt >= 3 && buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF)
	{
		bom_byte = 3;
	}

	// 文字列に変換
	str = UTF8_to_TSTR((char *)(buf.get() + bom_byte), (int)buf.size() - bom_byte);

	return true;
}

// ****************************************************************
// ファイル：データを一括読み込み（UTF-16 テキスト）
// ================================================================
bool C_file::read_full_text_UTF16(CTSTR& file_path, TSTR& str)
{
	str.clear();

	C_file file;
	if (!file.open(file_path, _T("rb")))
		return false;		// エラー処理済

	// ファイルデータの全読み込み
	ARRAY<BYTE> buf((int)file.get_size());					// ここでのサイズはバイナリでのサイズ
	int read_cnt = file.read(buf.get(), (int)buf.size());	// ここで取得できるのはテキストでのサイズ
	buf.resize(read_cnt);									// テキストでのサイズに調整

	// BOM 処理
	int bom_byte = 0;
	if (read_cnt >= 2 && buf[0] == 0xFF && buf[1] == 0xFE)
	{
		bom_byte = 2;
	}

	// 文字列に変換
	str = TSTR((TCHAR *)(buf.get() + bom_byte), (TCHAR *)(buf.get() + read_cnt));

	// \r\n を \n に変換
	str = str_replace_ret(str, _T("\r\n"), _T("\n"));

	return true;
}

// ****************************************************************
// ファイル：データを一括読み込み（テキスト、行ごとに分割）
// ================================================================
bool C_file::read_full_text(CTSTR& file_path, ARRAY<TSTR>& buf)
{
	buf.clear();

	TSTR str;
	if (!read_full_text(file_path, str))
		return false;		// エラー処理済

	separate_text_line(str, buf);
	return true;
}

// ****************************************************************
// ファイル：データを一括読み込み（UTF-8 テキスト、行ごとに分割）
// ================================================================
bool C_file::read_full_text_UTF8(CTSTR& file_path, ARRAY<TSTR>& buf)
{
	buf.clear();

	TSTR str;
	if (!read_full_text_UTF8(file_path, str))
		return false;		// エラー処理済

	separate_text_line(str, buf);
	return true;
}

// ****************************************************************
// ファイル：データを一括読み込み（UTF-16 テキスト、行ごとに分割）
// ================================================================
bool C_file::read_full_text_UTF16(CTSTR& file_path, ARRAY<TSTR>& buf)
{
	buf.clear();

	TSTR str;
	if (!read_full_text_UTF16(file_path, str))
		return false;		// エラー処理済

	separate_text_line(str, buf);
	return true;
}

// ****************************************************************
// ファイル：データを一括書き込み（マルチバイトテキスト）
// ================================================================
bool C_file::write_full_text(CTSTR& file_path, CMBSTR& str)
{
	C_file file;
	if (!file.open(file_path, _T("wt")))
		return false;		// エラー処理済

	file.write(str);
	file.close();

	return true;
}

// ****************************************************************
// ファイル：データを一括書き込み（マルチバイトテキストに変換）
// ================================================================
bool C_file::write_full_text_in_MB(CTSTR& file_path, CTSTR& str)
{
	C_file file;
	if (!file.open(file_path, _T("wt")))
		return false;		// エラー処理済

	file.write_in_MB(str);
	file.close();

	return true;
}

// ****************************************************************
// ファイル：データを一括書き込み（UTF-8 に変換）
// ================================================================
bool C_file::write_full_text_in_UTF8(CTSTR& file_path, CTSTR& str)
{
	C_file file;
	if (!file.open(file_path, _T("wt")))
		return false;		// エラー処理済

	file.write_in_UTF8(str);
	file.close();

	return true;
}

// ****************************************************************
// ファイル：データを一括書き込み（マルチバイトテキストに変換、行ごと）
// ================================================================
bool C_file::write_full_text_in_MB(CTSTR& file_path, ARRAY<TSTR>& str_list)
{
	C_file file;
	if (!file.open(file_path, _T("wt")))
		return false;		// エラー処理済

	int str_cnt = (int)str_list.size();
	for (int i = 0; i < str_cnt - 1; ++i)
		file.write_in_MB(str_list[i] + _T("\n"));
	if (str_cnt > 0)
		file.write_in_MB(str_list[str_cnt - 1]);	// 最後の行は \n を入れない。read_full_text と仕様を合わせるため。

	file.close();

	return true;
}

// ****************************************************************
// ファイル：データを一括書き込み（UTF-8 に変換、行ごと）
// ================================================================
bool C_file::write_full_text_in_UTF8(CTSTR& file_path, ARRAY<TSTR>& str_list)
{
	C_file file;
	if (!file.open(file_path, _T("wt")))
		return false;		// エラー処理済

	int str_cnt = (int)str_list.size();
	for (int i = 0; i < str_cnt - 1; ++i)
		file.write_in_UTF8(str_list[i] + _T("\n"));
	if (str_cnt > 0)
		file.write_in_UTF8(str_list[str_cnt - 1]);	// 最後の行は \n を入れない。read_full_text と仕様を合わせるため。

	file.close();

	return true;
}

// ****************************************************************
// ファイル：データを一括書き込み（バイナリ）
// ================================================================
bool C_file::write_full_data(CTSTR& file_path, ARRAY<BYTE>& buf)
{
	C_file file;
	if (!file.open(file_path, _T("wb")))
		return false;		// エラー処理済

	file.write(buf.get(), (int)buf.size());
	file.close();

	return true;
}

bool C_file::write_full_data(CTSTR& file_path, C_stream& buf)
{
	C_file file;
	if (!file.open(file_path, _T("wb")))
		return false;		// エラー処理済

	file.write(buf.get(), (int)buf.size());
	file.close();

	return true;
}

bool C_file::write_full_data(CTSTR& file_path, void* p_data, int size)
{
	C_file file;
	if (!file.open(file_path, _T("wb")))
		return false;		// エラー処理済

	file.write(p_data, size);
	file.close();

	return true;
}


}
