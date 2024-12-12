#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"
#include	"tona3_stream.h"

namespace NT3
{

// ****************************************************************
// ファイル
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

	static	bool	read_full_data(CTSTR& file_path, ARRAY<BYTE>& buf);			// ファイルからデータを一括読み込み（バイナリ）
	static	bool	read_full_data(CTSTR& file_path, C_stream& stream);			// ファイルからデータを一括読み込み（バイナリ）
	static	bool	read_full_text(CTSTR& file_path, TSTR& str);				// ファイルからデータを一括読み込み（テキスト）
	static	bool	read_full_text_UTF8(CTSTR& file_path, TSTR& str);			// ファイルからデータを一括読み込み（UTF-8 テキスト）
	static	bool	read_full_text_UTF16(CTSTR& file_path, TSTR& str);			// ファイルからデータを一括読み込み（UTF-16 テキスト）
	static	bool	read_full_text(CTSTR& file_path, ARRAY<TSTR>& buf);			// ファイルからデータを一括読み込み（テキスト、行ごとに分割）
	static	bool	read_full_text_UTF8(CTSTR& file_path, ARRAY<TSTR>& buf);	// ファイルからデータを一括読み込み（UTF-8 テキスト、行ごとに分割）
	static	bool	read_full_text_UTF16(CTSTR& file_path, ARRAY<TSTR>& buf);	// ファイルからデータを一括読み込み（UTF-16 テキスト、行ごとに分割）

	static	bool	write_full_data(CTSTR& file_path, ARRAY<BYTE>& buf);		// ファイルにデータを一括書き込み（バイナリ）
	static	bool	write_full_data(CTSTR& file_path, C_stream& buf);			// ファイルにデータを一括書き込み（バイナリ）
	static	bool	write_full_data(CTSTR& file_path, void* p_data, int size);	// ファイルにデータを一括書き込み（バイナリ）
	static	bool	write_full_text(CTSTR& file_path, CMBSTR& str);				// ファイルにデータを一括書き込み（マルチバイトテキスト）
	static	bool	write_full_text_in_MB(CTSTR& file_path, CTSTR& str);		// ファイルにデータを一括書き込み（マルチバイトテキストに変換）
	static	bool	write_full_text_in_MB(CTSTR& file_path, ARRAY<TSTR>& buf);	// ファイルにデータを一括書き込み（マルチバイトテキストに変換、行ごと）
	static	bool	write_full_text_in_UTF8(CTSTR& file_path, CTSTR& str);			// ファイルにデータを一括書き込み（UTF-8 に変換）
	static	bool	write_full_text_in_UTF8(CTSTR& file_path, ARRAY<TSTR>& buf);	// ファイルにデータを一括書き込み（UTF-8 に変換、行ごと）

private:
	FILE*	fp;
	UINT64	size;

	// コピー禁止！
	C_file(C_file&);
	C_file& operator = (C_file&);
};

}
