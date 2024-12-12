#include	"tona3_pch.h"
#include	"tona3_ini.h"
#include	"tona3_app.h"
#include	"tona3_file.h"
#include	"tona3_filesys.h"
#include	"tona3_tree.h"
#include	"tona3_xml.h"

// 最後に undef するのを忘れないでください
#define		INI_ARG		CTSTR& tag_00, CTSTR& tag_01, CTSTR& tag_02, CTSTR& tag_03
#define		INI_ARG_R	tag_00, tag_01, tag_02, tag_03

namespace NT3
{

// ****************************************************************
// モジュール名の ini ファイルパスを取得
// ================================================================
TSTR C_ini_file::get_module_ini_file_path()
{
	return G_app.module_dir + _T("\\") + G_app.module_name + _T(".ini");
}

// ****************************************************************
// クリア
// ================================================================
void C_ini_file::clear_data()
{
	top.item.clear();
	top.value.clear();
}

// ****************************************************************
// ini ファイルを読み込む
// ================================================================

// ファイルパスを指定して読み込む
void C_ini_file::load_ini_file(CTSTR& path)
{
	// ファイルパスを設定
	file_path = path;

	// xml を読み込んで ini データを作成
	Cxml xml;
	Sxml_data xml_data;
	xml.load_xml(file_path, &xml_data);
	create_item_data(&top, &xml_data.top);
}

// モジュール名の ini ファイルを読み込む
void C_ini_file::load_ini_file()
{
	C_ini_file::load_ini_file(get_module_ini_file_path());
}

// ****************************************************************
// アイテムデータから xml データを作成
// ================================================================
void C_ini_file::create_xml_data(S_ini_item* ini_item, Sxml_item* xml_item)
{
	// アイテムがある場合
	if (!ini_item->item.empty())	{
		for (T_ini_map::iterator itr = ini_item->item.begin(); itr != ini_item->item.end(); ++itr)	{
			Sxml_item new_xml_item;
			new_xml_item.tag.name = itr->first;
			create_xml_data(&itr->second, &new_xml_item);
			xml_item->item.push_back(new_xml_item);
		}
	}
	// アイテムがない場合
	else	{
		xml_item->data = ini_item->value;
	}
}

// ****************************************************************
// ini ファイルを保存
// ================================================================
void C_ini_file::save_ini_file()
{
	// ディレクトリを作る
	create_dir_from_file_path(file_path);

	// xml データを作成して保存
	Cxml xml;
	Sxml_data xml_data;
	xml_data.top.tag.name = G_app.app_name;
	create_xml_data(&top, &xml_data.top);
	xml.save_xml(file_path, &xml_data);
}

// ****************************************************************
// xml データからアイテムデータを作成
// ================================================================
void C_ini_file::create_item_data(S_ini_item* ini_item, Sxml_item* xml_item)
{
	// アイテムがある場合
	if (!xml_item->item.empty())	{
		for (ARRAY<Sxml_item>::iterator itr = xml_item->item.begin(); itr != xml_item->item.end(); ++itr)	{
			S_ini_item new_ini_item;
			create_item_data(&new_ini_item, &*itr);
			ini_item->item.insert(T_ini_pair(itr->tag.name, new_ini_item));
		}
	}
	// アイテムがない場合
	else	{
		ini_item->value = xml_item->data;
	}
}

// ****************************************************************
// タグリストを作成
// ================================================================
void C_ini_file::create_tag_list(ARRAY<TSTR>& tag_list, INI_ARG)
{
	tag_list.clear();

	// タグリストを作成
	if (!tag_00.empty())		tag_list.push_back(tag_00);
	if (!tag_01.empty())		tag_list.push_back(tag_01);
	if (!tag_02.empty())		tag_list.push_back(tag_02);
	if (!tag_03.empty())		tag_list.push_back(tag_03);
}

// ****************************************************************
// アイテムの値を登録
// ================================================================
void C_ini_file::set_item_data(CTSTR& key, CTSTR& value, INI_ARG)
{
	if (key.empty())	return;

	// タグリストを作成
	ARRAY<TSTR> tag_list;
	create_tag_list(tag_list, INI_ARG_R);
	// キーもタグとして追加
	tag_list.push_back(key);

	// アイテムを追加
	set_item_data(&top, value, tag_list.begin(), tag_list.end());
}

void C_ini_file::set_item_data(S_ini_item* ini_item, CTSTR& value, ARRAY<TSTR>::iterator tag_list_begin, ARRAY<TSTR>::iterator tag_list_end)
{
	// タグがある場合
	if (tag_list_begin != tag_list_end)	{
		// タグを探す
		T_ini_map::iterator itr = ini_item->item.find(*tag_list_begin);
		// 見つからなかった場合追加
		if (itr == ini_item->item.end())	{
			// 新しいマップを作成
			S_ini_item new_ini_item;
			set_item_data(&new_ini_item, value, tag_list_begin + 1, tag_list_end);
			ini_item->item.insert(T_ini_pair(*tag_list_begin, new_ini_item));
		}
		// 見つかった場合
		else	{
			set_item_data(&itr->second, value, tag_list_begin + 1, tag_list_end);
		}
	}
	// タグがない場合
	else	{
		ini_item->value = value;
	}
}

// ****************************************************************
// アイテムの値を取得
// ================================================================
bool C_ini_file::get_item_data(CTSTR& key, TSTR& value, INI_ARG)
{
	if (key.empty())	return false;

	// タグリストを作成
	ARRAY<TSTR> tag_list;
	create_tag_list(tag_list, INI_ARG_R);
	// キーもタグとして追加
	tag_list.push_back(key);

	// アイテムを取得
	return get_item_data(&top, value, tag_list.begin(), tag_list.end());
}

bool C_ini_file::get_item_data(S_ini_item* ini_item, TSTR& value, ARRAY<TSTR>::iterator tag_list_begin, ARRAY<TSTR>::iterator tag_list_end)
{
	// タグを探す場合
	if (tag_list_begin != tag_list_end)	{
		// タグを探す
		T_ini_map::iterator itr = ini_item->item.find(*tag_list_begin);
		// 見つかった場合
		if (itr != ini_item->item.end())	{
			// 次のタグを探す
			return get_item_data(&itr->second, value, tag_list_begin + 1, tag_list_end);
		}
		// 見つからなかった場合
		else	{
			// 失敗
			return false;
		}
	}
	// データを探す場合
	else	{
		value = ini_item->value;
	}

	return true;
}

// ****************************************************************
// アイテムの値を削除
// ================================================================
void C_ini_file::del_item_data(CTSTR& key, INI_ARG)
{
	// タグリストを作成
	ARRAY<TSTR> tag_list;
	create_tag_list(tag_list, INI_ARG_R);
	// キーもタグとして追加
	if (!key.empty())	{	tag_list.push_back(key);	}

	// アイテムを削除
	del_item_data(key, &top, tag_list.begin(), tag_list.end());
}

void C_ini_file::del_item_data(CTSTR& key, S_ini_item* ini_item, ARRAY<TSTR>::iterator tag_list_begin, ARRAY<TSTR>::iterator tag_list_end)
{
	// タグがない場合
	if (tag_list_begin == tag_list_end)	{	return;	}

	// タグを探す
	T_ini_map::iterator itr = ini_item->item.find(*tag_list_begin);

	// 見つからなかった場合終了
	if (itr == ini_item->item.end())	{	return;	}

	// まだ次のタグがある
	if ((tag_list_begin + 1) != tag_list_end)	{
		del_item_data(key, &itr->second, tag_list_begin + 1, tag_list_end);
		return;
	}

	// 最後のタグなので削除
	ini_item->item.erase(itr);
}

// ****************************************************************
// 文字列から数値を複数取得
// ================================================================
void C_ini_file::get_nums(TSTR& str, ARRAY<int>* num_list)
{
	TSTR::iterator itr = str.begin();
	int num;

	while (1)	{
		// セパレータをスキップ
		str_analize_skip_separator(itr, str.end());
		// 数値を取得
		if (str_analize_get_integer(itr, str.end(), &num))		num_list->push_back(num);
		else													break;
		// セパレータをスキップ
		str_analize_skip_separator(itr, str.end());
		// コンマ
		if (itr != str.end() && *itr == _T(','))				++itr;
		else													break;
	}
}

// ****************************************************************
// ini に登録
// ================================================================

// ini に文字列を登録
void C_ini_file::save_str(CTSTR& key, CTSTR& str, INI_ARG)
{
	set_item_data(key, str, INI_ARG_R);
}

// ini に bool 値を登録
void C_ini_file::save_bool(CTSTR& key, bool value, INI_ARG)
{
	save_str(key, tostr(value ? 1 : 0), INI_ARG_R);
}

// ini に int 値を登録
void C_ini_file::save_int(CTSTR& key, int value, INI_ARG)
{
	save_str(key, tostr(value), INI_ARG_R);
}

// ini に __int64 値を登録
void C_ini_file::save_int64(CTSTR& key, __int64 value, INI_ARG)
{
	save_str(key, tostr(value), INI_ARG_R);
}

// ini に long 値を登録
void C_ini_file::save_long(CTSTR& key, long value, INI_ARG)
{
	save_str(key, tostr(value), INI_ARG_R);
}

// ini に点を登録
void C_ini_file::save_point(CTSTR& key, C_point point, INI_ARG)
{
	save_str(key, tostr(point.x) + _T(",") + tostr(point.y), INI_ARG_R);
}

// ini に矩形を登録
void C_ini_file::save_rect(CTSTR& key, C_rect rect, INI_ARG)
{
	save_str(key, tostr(rect.left) + _T(",") + tostr(rect.top) + _T(",") + tostr(rect.right) + _T(",") + tostr(rect.bottom), INI_ARG_R);
}

// ini に int 型リストを登録
void C_ini_file::save_int_array(CTSTR& key, ARRAY<int>& int_list, INI_ARG)
{
	TSTR str;
	for (ARRAY<int>::iterator itr = int_list.begin(); itr != int_list.end(); ++itr)	{
		str += tostr(*itr) + _T(",");
	}
	save_str(key, str, INI_ARG_R);
}

// ****************************************************************
// ini から取得
// ================================================================

// ini から文字列を取得
TSTR C_ini_file::load_str(CTSTR& key, CTSTR& def, INI_ARG)
{
	TSTR str;
	if (!get_item_data(key, str, INI_ARG_R))
		return def;

	return str;
}

// ini から bool 値を取得
bool C_ini_file::load_bool(CTSTR& key, bool def, INI_ARG)
{
	return load_int(key, def ? 1 : 0, INI_ARG_R) != 0 ? true : false;
}

// ini から int 値を取得
int C_ini_file::load_int(CTSTR& key, int def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// 数値を取得
	return str_to_int(str, def);
}

// ini から int64 値を取得
__int64 C_ini_file::load_int64(CTSTR& key, __int64 def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// 数値を取得
	return str_to_int64(str, def);
}

// ini から long 値を取得
long C_ini_file::load_long(CTSTR& key, long def, INI_ARG)
{
	return load_int(key, def, INI_ARG_R);
}

// ini から float 値を取得
float C_ini_file::load_float(CTSTR& key, float def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// 数値を取得
	return str_to_float(str, def);
}

// ini から点を取得
C_point C_ini_file::load_point(CTSTR& key, C_point def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// 数値リストを取得
	ARRAY<int> num_list;
	get_nums(str, &num_list);
	if (num_list.size() != 2)
		return def;

	return C_point(num_list[0], num_list[1]);
}

// ini から矩形を取得
C_rect C_ini_file::load_rect(CTSTR& key, C_rect def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// 数値リストを取得
	ARRAY<int> num_list;
	get_nums(str, &num_list);
	if (num_list.size() != 4)
		return def;

	return C_rect(num_list[0], num_list[1], num_list[2], num_list[3]);
}

// ini から int 型リストを取得
ARRAY<int> C_ini_file::load_int_array(CTSTR& key, ARRAY<int>& def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// 数値リストを取得
	ARRAY<int> int_list;
	get_nums(str, &int_list);
	return int_list;
}

// ****************************************************************
// ini から削除
// ================================================================
void C_ini_file::delete_value(CTSTR& key, INI_ARG)
{
	del_item_data(key, INI_ARG_R);
}

}