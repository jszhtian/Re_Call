#include	"tona3_pch.h"
#include	"tona3_ini.h"
#include	"tona3_app.h"
#include	"tona3_file.h"
#include	"tona3_filesys.h"
#include	"tona3_tree.h"
#include	"tona3_xml.h"

// �Ō�� undef ����̂�Y��Ȃ��ł�������
#define		INI_ARG		CTSTR& tag_00, CTSTR& tag_01, CTSTR& tag_02, CTSTR& tag_03
#define		INI_ARG_R	tag_00, tag_01, tag_02, tag_03

namespace NT3
{

// ****************************************************************
// ���W���[������ ini �t�@�C���p�X���擾
// ================================================================
TSTR C_ini_file::get_module_ini_file_path()
{
	return G_app.module_dir + _T("\\") + G_app.module_name + _T(".ini");
}

// ****************************************************************
// �N���A
// ================================================================
void C_ini_file::clear_data()
{
	top.item.clear();
	top.value.clear();
}

// ****************************************************************
// ini �t�@�C����ǂݍ���
// ================================================================

// �t�@�C���p�X���w�肵�ēǂݍ���
void C_ini_file::load_ini_file(CTSTR& path)
{
	// �t�@�C���p�X��ݒ�
	file_path = path;

	// xml ��ǂݍ���� ini �f�[�^���쐬
	Cxml xml;
	Sxml_data xml_data;
	xml.load_xml(file_path, &xml_data);
	create_item_data(&top, &xml_data.top);
}

// ���W���[������ ini �t�@�C����ǂݍ���
void C_ini_file::load_ini_file()
{
	C_ini_file::load_ini_file(get_module_ini_file_path());
}

// ****************************************************************
// �A�C�e���f�[�^���� xml �f�[�^���쐬
// ================================================================
void C_ini_file::create_xml_data(S_ini_item* ini_item, Sxml_item* xml_item)
{
	// �A�C�e��������ꍇ
	if (!ini_item->item.empty())	{
		for (T_ini_map::iterator itr = ini_item->item.begin(); itr != ini_item->item.end(); ++itr)	{
			Sxml_item new_xml_item;
			new_xml_item.tag.name = itr->first;
			create_xml_data(&itr->second, &new_xml_item);
			xml_item->item.push_back(new_xml_item);
		}
	}
	// �A�C�e�����Ȃ��ꍇ
	else	{
		xml_item->data = ini_item->value;
	}
}

// ****************************************************************
// ini �t�@�C����ۑ�
// ================================================================
void C_ini_file::save_ini_file()
{
	// �f�B���N�g�������
	create_dir_from_file_path(file_path);

	// xml �f�[�^���쐬���ĕۑ�
	Cxml xml;
	Sxml_data xml_data;
	xml_data.top.tag.name = G_app.app_name;
	create_xml_data(&top, &xml_data.top);
	xml.save_xml(file_path, &xml_data);
}

// ****************************************************************
// xml �f�[�^����A�C�e���f�[�^���쐬
// ================================================================
void C_ini_file::create_item_data(S_ini_item* ini_item, Sxml_item* xml_item)
{
	// �A�C�e��������ꍇ
	if (!xml_item->item.empty())	{
		for (ARRAY<Sxml_item>::iterator itr = xml_item->item.begin(); itr != xml_item->item.end(); ++itr)	{
			S_ini_item new_ini_item;
			create_item_data(&new_ini_item, &*itr);
			ini_item->item.insert(T_ini_pair(itr->tag.name, new_ini_item));
		}
	}
	// �A�C�e�����Ȃ��ꍇ
	else	{
		ini_item->value = xml_item->data;
	}
}

// ****************************************************************
// �^�O���X�g���쐬
// ================================================================
void C_ini_file::create_tag_list(ARRAY<TSTR>& tag_list, INI_ARG)
{
	tag_list.clear();

	// �^�O���X�g���쐬
	if (!tag_00.empty())		tag_list.push_back(tag_00);
	if (!tag_01.empty())		tag_list.push_back(tag_01);
	if (!tag_02.empty())		tag_list.push_back(tag_02);
	if (!tag_03.empty())		tag_list.push_back(tag_03);
}

// ****************************************************************
// �A�C�e���̒l��o�^
// ================================================================
void C_ini_file::set_item_data(CTSTR& key, CTSTR& value, INI_ARG)
{
	if (key.empty())	return;

	// �^�O���X�g���쐬
	ARRAY<TSTR> tag_list;
	create_tag_list(tag_list, INI_ARG_R);
	// �L�[���^�O�Ƃ��Ēǉ�
	tag_list.push_back(key);

	// �A�C�e����ǉ�
	set_item_data(&top, value, tag_list.begin(), tag_list.end());
}

void C_ini_file::set_item_data(S_ini_item* ini_item, CTSTR& value, ARRAY<TSTR>::iterator tag_list_begin, ARRAY<TSTR>::iterator tag_list_end)
{
	// �^�O������ꍇ
	if (tag_list_begin != tag_list_end)	{
		// �^�O��T��
		T_ini_map::iterator itr = ini_item->item.find(*tag_list_begin);
		// ������Ȃ������ꍇ�ǉ�
		if (itr == ini_item->item.end())	{
			// �V�����}�b�v���쐬
			S_ini_item new_ini_item;
			set_item_data(&new_ini_item, value, tag_list_begin + 1, tag_list_end);
			ini_item->item.insert(T_ini_pair(*tag_list_begin, new_ini_item));
		}
		// ���������ꍇ
		else	{
			set_item_data(&itr->second, value, tag_list_begin + 1, tag_list_end);
		}
	}
	// �^�O���Ȃ��ꍇ
	else	{
		ini_item->value = value;
	}
}

// ****************************************************************
// �A�C�e���̒l���擾
// ================================================================
bool C_ini_file::get_item_data(CTSTR& key, TSTR& value, INI_ARG)
{
	if (key.empty())	return false;

	// �^�O���X�g���쐬
	ARRAY<TSTR> tag_list;
	create_tag_list(tag_list, INI_ARG_R);
	// �L�[���^�O�Ƃ��Ēǉ�
	tag_list.push_back(key);

	// �A�C�e�����擾
	return get_item_data(&top, value, tag_list.begin(), tag_list.end());
}

bool C_ini_file::get_item_data(S_ini_item* ini_item, TSTR& value, ARRAY<TSTR>::iterator tag_list_begin, ARRAY<TSTR>::iterator tag_list_end)
{
	// �^�O��T���ꍇ
	if (tag_list_begin != tag_list_end)	{
		// �^�O��T��
		T_ini_map::iterator itr = ini_item->item.find(*tag_list_begin);
		// ���������ꍇ
		if (itr != ini_item->item.end())	{
			// ���̃^�O��T��
			return get_item_data(&itr->second, value, tag_list_begin + 1, tag_list_end);
		}
		// ������Ȃ������ꍇ
		else	{
			// ���s
			return false;
		}
	}
	// �f�[�^��T���ꍇ
	else	{
		value = ini_item->value;
	}

	return true;
}

// ****************************************************************
// �A�C�e���̒l���폜
// ================================================================
void C_ini_file::del_item_data(CTSTR& key, INI_ARG)
{
	// �^�O���X�g���쐬
	ARRAY<TSTR> tag_list;
	create_tag_list(tag_list, INI_ARG_R);
	// �L�[���^�O�Ƃ��Ēǉ�
	if (!key.empty())	{	tag_list.push_back(key);	}

	// �A�C�e�����폜
	del_item_data(key, &top, tag_list.begin(), tag_list.end());
}

void C_ini_file::del_item_data(CTSTR& key, S_ini_item* ini_item, ARRAY<TSTR>::iterator tag_list_begin, ARRAY<TSTR>::iterator tag_list_end)
{
	// �^�O���Ȃ��ꍇ
	if (tag_list_begin == tag_list_end)	{	return;	}

	// �^�O��T��
	T_ini_map::iterator itr = ini_item->item.find(*tag_list_begin);

	// ������Ȃ������ꍇ�I��
	if (itr == ini_item->item.end())	{	return;	}

	// �܂����̃^�O������
	if ((tag_list_begin + 1) != tag_list_end)	{
		del_item_data(key, &itr->second, tag_list_begin + 1, tag_list_end);
		return;
	}

	// �Ō�̃^�O�Ȃ̂ō폜
	ini_item->item.erase(itr);
}

// ****************************************************************
// �����񂩂琔�l�𕡐��擾
// ================================================================
void C_ini_file::get_nums(TSTR& str, ARRAY<int>* num_list)
{
	TSTR::iterator itr = str.begin();
	int num;

	while (1)	{
		// �Z�p���[�^���X�L�b�v
		str_analize_skip_separator(itr, str.end());
		// ���l���擾
		if (str_analize_get_integer(itr, str.end(), &num))		num_list->push_back(num);
		else													break;
		// �Z�p���[�^���X�L�b�v
		str_analize_skip_separator(itr, str.end());
		// �R���}
		if (itr != str.end() && *itr == _T(','))				++itr;
		else													break;
	}
}

// ****************************************************************
// ini �ɓo�^
// ================================================================

// ini �ɕ������o�^
void C_ini_file::save_str(CTSTR& key, CTSTR& str, INI_ARG)
{
	set_item_data(key, str, INI_ARG_R);
}

// ini �� bool �l��o�^
void C_ini_file::save_bool(CTSTR& key, bool value, INI_ARG)
{
	save_str(key, tostr(value ? 1 : 0), INI_ARG_R);
}

// ini �� int �l��o�^
void C_ini_file::save_int(CTSTR& key, int value, INI_ARG)
{
	save_str(key, tostr(value), INI_ARG_R);
}

// ini �� __int64 �l��o�^
void C_ini_file::save_int64(CTSTR& key, __int64 value, INI_ARG)
{
	save_str(key, tostr(value), INI_ARG_R);
}

// ini �� long �l��o�^
void C_ini_file::save_long(CTSTR& key, long value, INI_ARG)
{
	save_str(key, tostr(value), INI_ARG_R);
}

// ini �ɓ_��o�^
void C_ini_file::save_point(CTSTR& key, C_point point, INI_ARG)
{
	save_str(key, tostr(point.x) + _T(",") + tostr(point.y), INI_ARG_R);
}

// ini �ɋ�`��o�^
void C_ini_file::save_rect(CTSTR& key, C_rect rect, INI_ARG)
{
	save_str(key, tostr(rect.left) + _T(",") + tostr(rect.top) + _T(",") + tostr(rect.right) + _T(",") + tostr(rect.bottom), INI_ARG_R);
}

// ini �� int �^���X�g��o�^
void C_ini_file::save_int_array(CTSTR& key, ARRAY<int>& int_list, INI_ARG)
{
	TSTR str;
	for (ARRAY<int>::iterator itr = int_list.begin(); itr != int_list.end(); ++itr)	{
		str += tostr(*itr) + _T(",");
	}
	save_str(key, str, INI_ARG_R);
}

// ****************************************************************
// ini ����擾
// ================================================================

// ini ���當������擾
TSTR C_ini_file::load_str(CTSTR& key, CTSTR& def, INI_ARG)
{
	TSTR str;
	if (!get_item_data(key, str, INI_ARG_R))
		return def;

	return str;
}

// ini ���� bool �l���擾
bool C_ini_file::load_bool(CTSTR& key, bool def, INI_ARG)
{
	return load_int(key, def ? 1 : 0, INI_ARG_R) != 0 ? true : false;
}

// ini ���� int �l���擾
int C_ini_file::load_int(CTSTR& key, int def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// ���l���擾
	return str_to_int(str, def);
}

// ini ���� int64 �l���擾
__int64 C_ini_file::load_int64(CTSTR& key, __int64 def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// ���l���擾
	return str_to_int64(str, def);
}

// ini ���� long �l���擾
long C_ini_file::load_long(CTSTR& key, long def, INI_ARG)
{
	return load_int(key, def, INI_ARG_R);
}

// ini ���� float �l���擾
float C_ini_file::load_float(CTSTR& key, float def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// ���l���擾
	return str_to_float(str, def);
}

// ini ����_���擾
C_point C_ini_file::load_point(CTSTR& key, C_point def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// ���l���X�g���擾
	ARRAY<int> num_list;
	get_nums(str, &num_list);
	if (num_list.size() != 2)
		return def;

	return C_point(num_list[0], num_list[1]);
}

// ini �����`���擾
C_rect C_ini_file::load_rect(CTSTR& key, C_rect def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// ���l���X�g���擾
	ARRAY<int> num_list;
	get_nums(str, &num_list);
	if (num_list.size() != 4)
		return def;

	return C_rect(num_list[0], num_list[1], num_list[2], num_list[3]);
}

// ini ���� int �^���X�g���擾
ARRAY<int> C_ini_file::load_int_array(CTSTR& key, ARRAY<int>& def, INI_ARG)
{
	TSTR str = load_str(key, _T(""), INI_ARG_R);
	if (str.empty())
		return def;

	// ���l���X�g���擾
	ARRAY<int> int_list;
	get_nums(str, &int_list);
	return int_list;
}

// ****************************************************************
// ini ����폜
// ================================================================
void C_ini_file::delete_value(CTSTR& key, INI_ARG)
{
	del_item_data(key, INI_ARG_R);
}

}