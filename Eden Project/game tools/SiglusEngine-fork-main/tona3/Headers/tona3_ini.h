#pragma		once

#include	"tona3_string.h"
#include	"tona3_array.h"
#include	"tona3_util.h"
#include	"tona3_xml.h"

// 最後に undef するのを忘れないでください
#define		INI_ARG		CTSTR& tag_00 = _T(""), CTSTR& tag_01 = _T(""), CTSTR& tag_02 = _T(""), CTSTR& tag_03 = _T("")

namespace NT3
{

// ****************************************************************
// ini 関連
// ================================================================

class C_ini_file
{
public:

	// シングルトン
	static C_ini_file& body()
	{
		static C_ini_file singleton;
		return singleton;
	}

	// 前方宣言
	struct S_ini_item;

	// ini タグ名とデータのマップ
	typedef	std::map<TSTR, S_ini_item>	T_ini_map;
	typedef	std::pair<TSTR, S_ini_item>	T_ini_pair;

	// ini ツリーのアイテム構造体
	struct S_ini_item
	{
		// どちらかのデータが使用される
		T_ini_map	item;
		TSTR		value;
	};

	void		clear_data();							// データクリア
	void		load_ini_file(CTSTR& path);				// ini ファイルを読み込む（ファイルパスを指定して読み込む）
	void		load_ini_file();						// ini ファイルを読み込む（モジュール名のＩＮＩファイルを読み込む）
	void		save_ini_file();						// ini ファイルを保存
	TSTR		get_module_ini_file_path();				// モジュール名の ini ファイルパスを取得

	// ini に登録
	void		save_str(CTSTR& key, CTSTR& value, INI_ARG);				// ini に文字列を登録
	void		save_bool(CTSTR& key, bool value, INI_ARG);					// ini にbool値を登録
	void		save_int(CTSTR& key, int value, INI_ARG);					// ini に整数を登録
	void		save_int64(CTSTR& key, __int64 value, INI_ARG);				// ini に整数を登録
	void		save_long(CTSTR& key, long value, INI_ARG);					// ini に整数を登録
	void		save_point(CTSTR& key, C_point point, INI_ARG);				// ini に点を登録
	void		save_rect(CTSTR& key, C_rect rect, INI_ARG);				// ini に矩形を登録
	void		save_int_array(CTSTR& key, ARRAY<int>& value, INI_ARG);		// ini に整数リストを登録

	// ini から取得
	TSTR		load_str(CTSTR& key, CTSTR& def, INI_ARG);					// ini から文字列を取得
	bool		load_bool(CTSTR& key, bool def, INI_ARG);					// ini から bool 値を取得
	int			load_int(CTSTR& key, int def, INI_ARG);						// ini から整数を取得
	__int64		load_int64(CTSTR& key, __int64 def, INI_ARG);				// ini から整数を取得
	long		load_long(CTSTR& key, long def, INI_ARG);					// ini から整数を取得
	float		load_float(CTSTR& key, float def, INI_ARG);					// ini から小数を取得
	C_point		load_point(CTSTR& key, C_point def, INI_ARG);				// ini から点を取得
	C_rect		load_rect(CTSTR& key, C_rect def, INI_ARG);					// ini から矩形を取得
	ARRAY<int>	load_int_array(CTSTR& key, ARRAY<int>& def, INI_ARG);		// ini から int 型リストを取得

	// ini から削除
	void		delete_value(CTSTR& key, INI_ARG);							// ini から削除

private:
	void		create_tag_list(ARRAY<TSTR>& tag_list, INI_ARG);

	void		set_item_data(CTSTR& key, CTSTR& value, INI_ARG);
	void		set_item_data(S_ini_item* ini_item, CTSTR& value, ARRAY<TSTR>::iterator tag_list_begin, ARRAY<TSTR>::iterator tag_list_end);
	bool		get_item_data(CTSTR& key, TSTR& value, INI_ARG);
	bool		get_item_data(S_ini_item* ini_item, TSTR& value, ARRAY<TSTR>::iterator tag_list_begin, ARRAY<TSTR>::iterator tag_list_end);
	void		del_item_data(CTSTR& key, INI_ARG);
	void		del_item_data(CTSTR& key, S_ini_item* ini_item, ARRAY<TSTR>::iterator tag_list_begin, ARRAY<TSTR>::iterator tag_list_end);

	void		create_xml_data(S_ini_item* ini_item, Sxml_item* xml_item);
	void		create_item_data(S_ini_item* ini_item, Sxml_item* xml_item);

	void		get_nums(TSTR& str, ARRAY<int>* num_list);					// 文字列から数値を複数取得

	S_ini_item	top;		// ini ツリーの先頭
	TSTR		file_path;	// ファイルパス

	// コンストラクタを隠蔽する
	C_ini_file()	{}
	~C_ini_file()	{}
};

}

#undef		INI_ARG
