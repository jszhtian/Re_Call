#include	"tona3_pch.h"
#include	"tona3_xml.h"
#include	"tona3_file.h"
#include	"tona3_tree.h"

namespace NT3
{

bool Cxml::load_xml(CTSTR& file_path, Sxml_data* data)
{
	// xml ��ǂݍ���
	TSTR xml_str;
	if (!C_file::read_full_text(file_path, xml_str))
		return false;

	// ���s�ƃ^�u���폜
	TSTR temp_str;
	for (TSTR::iterator p = xml_str.begin(); p != xml_str.end(); ++p)	{
		if (*p == _T('\n') || *p == _T('\t'))	{}
		else
			temp_str += *p;
	}
	xml_str = temp_str;

	// xml �����
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

	// �J���^�O
	if (!str_analize_check_moji(p, end, _T('<')))	return false;
	if (!str_analize_get_word(p, end, tag_name))	return false;
	if (!str_analize_check_moji(p, end, _T('>')))	return false;

	// ��key/value ������

	// ����
	tag->name = tag_name;
	start = p;
	return true;
}

bool Cxml::analize_close_tag(TSTR::const_iterator& start, TSTR::const_iterator end, TSTR& tag_name_ret)
{
	TSTR::const_iterator p = start;
	TSTR tag_name;

	// �J���^�O
	if (!str_analize_check_moji(p, end, _T('<')))	return false;
	if (!str_analize_check_moji(p, end, _T('/')))	return false;
	if (!str_analize_get_word(p, end, tag_name))	return false;
	if (!str_analize_check_moji(p, end, _T('>')))	return false;

	// ��key/value ������

	// ����
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
		// �I�[����
		else if (*p == _T('<') || *p == _T('\0'))
			break;
		// �����ꕶ��
		else if (str_analize_check_str(p, end, _T("&lt;")))	{
			data += _T("<");
		}
		else if (str_analize_check_str(p, end, _T("&gt;")))	{
			data += _T(">");
		}
		else if (str_analize_check_str(p, end, _T("&amp;")))	{
			data += _T("&");
		}
		// �ʏ핶��
		else	{
			data += *p++;
		}
	}

	// ����
	data_ret = data;
	start = p;
	return true;
}

bool Cxml::analize_item(TSTR::const_iterator& start, TSTR::const_iterator end, Sxml_item* item_ret)
{
	Sxml_item item;

	TSTR::const_iterator p = start;

	// �J���^�O
	Sxml_tag tag;
	if (!analize_open_tag(p, end, &tag))
		return false;

	item.tag = tag;

	// �q���A�C�e�������
	while (1)	{
		Sxml_item child_item;
		if (!analize_item(p, end, &child_item))		break;
		item.item.push_back(child_item);
	}
	// �q�������Ȃ��ꍇ�̓f�[�^�̉\������
	if (item.item.empty())	{
		TSTR data;
		analize_data(p, end, data);
		item.data = data;
	}

	// ���^�O
	TSTR close_tag_name;
	if (!analize_close_tag(p, end, close_tag_name))
		return false;

	// ����
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
	// �q��������ꍇ
	if (!item->item.empty())	{
		// �J���^�O
		xml_str += TSTR(_T("\t")) * depth + _T("<") + item->tag.name + _T(">\n");
		// �q��
		for (ARRAY<Sxml_item>::iterator itr = item->item.begin(); itr != item->item.end(); ++itr)	{
			save_item(&*itr, xml_str, depth + 1);
		}
		// ���^�O
		xml_str += TSTR(_T("\t")) * depth + _T("</") + item->tag.name + _T(">\n");
	}
	// �q�������Ȃ��ꍇ
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
