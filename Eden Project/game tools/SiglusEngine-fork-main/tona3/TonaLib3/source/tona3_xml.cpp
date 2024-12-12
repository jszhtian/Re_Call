#include	"tona3_pch.h"
#include	"tona3_xml.h"
#include	"tona3_file.h"
#include	"tona3_tree.h"

namespace NT3
{

bool Cxml::load_xml(CTSTR& file_path, Sxml_data* data)
{
	// xml を読み込み
	TSTR xml_str;
	if (!C_file::read_full_text(file_path, xml_str))
		return false;

	// 改行とタブを削除
	TSTR temp_str;
	for (TSTR::iterator p = xml_str.begin(); p != xml_str.end(); ++p)	{
		if (*p == _T('\n') || *p == _T('\t'))	{}
		else
			temp_str += *p;
	}
	xml_str = temp_str;

	// xml を解析
	TSTR::iterator p = xml_str.begin();
	TSTR::iterator end = xml_str.end();

	Sxml_item top_item;
	if (!analize_item(p, end, &top_item))
		return false;
	data->top = top_item;

	return true;
}

bool Cxml::analize_open_tag(TSTR::const_iterator& start, TSTR::const_iterator end, Sxml_tag* tag)
{
	TSTR::const_iterator p = start;
	TSTR tag_name;

	// 開きタグ
	if (!str_analize_check_moji(p, end, _T('<')))	return false;
	if (!str_analize_get_word(p, end, tag_name))	return false;
	if (!str_analize_check_moji(p, end, _T('>')))	return false;

	// ★key/value 未実装

	// 成功
	tag->name = tag_name;
	start = p;
	return true;
}

bool Cxml::analize_close_tag(TSTR::const_iterator& start, TSTR::const_iterator end, TSTR& tag_name_ret)
{
	TSTR::const_iterator p = start;
	TSTR tag_name;

	// 開きタグ
	if (!str_analize_check_moji(p, end, _T('<')))	return false;
	if (!str_analize_check_moji(p, end, _T('/')))	return false;
	if (!str_analize_get_word(p, end, tag_name))	return false;
	if (!str_analize_check_moji(p, end, _T('>')))	return false;

	// ★key/value 未実装

	// 成功
	tag_name_ret = tag_name;
	start = p;
	return true;
}

bool Cxml::analize_data(TSTR::const_iterator& start, TSTR::const_iterator end, TSTR& data_ret)
{
	TSTR::const_iterator p = start;
	TSTR data;

	while (p != end)	{
		if (false);
		// 終端文字
		else if (*p == _T('<') || *p == _T('\0'))
			break;
		// ★特殊文字
		else if (str_analize_check_str(p, end, _T("&lt;")))	{
			data += _T("<");
		}
		else if (str_analize_check_str(p, end, _T("&gt;")))	{
			data += _T(">");
		}
		else if (str_analize_check_str(p, end, _T("&amp;")))	{
			data += _T("&");
		}
		// 通常文字
		else	{
			data += *p++;
		}
	}

	// 成功
	data_ret = data;
	start = p;
	return true;
}

bool Cxml::analize_item(TSTR::const_iterator& start, TSTR::const_iterator end, Sxml_item* item_ret)
{
	Sxml_item item;

	TSTR::const_iterator p = start;

	// 開きタグ
	Sxml_tag tag;
	if (!analize_open_tag(p, end, &tag))
		return false;

	item.tag = tag;

	// 子供アイテムを解析
	while (1)	{
		Sxml_item child_item;
		if (!analize_item(p, end, &child_item))		break;
		item.item.push_back(child_item);
	}
	// 子供がいない場合はデータの可能性あり
	if (item.item.empty())	{
		TSTR data;
		analize_data(p, end, data);
		item.data = data;
	}

	// 閉じタグ
	TSTR close_tag_name;
	if (!analize_close_tag(p, end, close_tag_name))
		return false;

	// 成功
	start = p;
	*item_ret = item;
	return true;
}

bool Cxml::save_xml(CTSTR& file_path, Sxml_data* data)
{
	TSTR xml_str;
	save_item(&data->top, xml_str, 0);
	C_file::write_full_text_in_MB(file_path, xml_str);

	return true;
}

bool Cxml::save_item(Sxml_item* item, TSTR& xml_str, int depth)
{
	// 子供がいる場合
	if (!item->item.empty())	{
		// 開きタグ
		xml_str += TSTR(_T("\t")) * depth + _T("<") + item->tag.name + _T(">\n");
		// 子供
		for (ARRAY<Sxml_item>::iterator itr = item->item.begin(); itr != item->item.end(); ++itr)	{
			save_item(&*itr, xml_str, depth + 1);
		}
		// 閉じタグ
		xml_str += TSTR(_T("\t")) * depth + _T("</") + item->tag.name + _T(">\n");
	}
	// 子供がいない場合
	else	{
		xml_str += TSTR(_T("\t")) * depth + _T("<") + item->tag.name + _T(">") + xml_encode(item->data) + _T("</") + item->tag.name + _T(">\n");
	}

	return true;
}

TSTR Cxml::xml_encode(CTSTR& str)
{
	TSTR ret_str = str;
	ret_str = str_replace_ret(ret_str, _T("&"), _T("&amp;"));
	ret_str = str_replace_ret(ret_str, _T("<"), _T("&lt;"));
	ret_str = str_replace_ret(ret_str, _T(">"), _T("&gt;"));
	return ret_str;
}


}
