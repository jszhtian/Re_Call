#pragma		once

#include	"tona3_string.h"
#include	"tona3_array.h"
#include	"tona3_util.h"
#include	"tona3_xml.h"

// �Ō�� undef ����̂�Y��Ȃ��ł�������
#define		INI_ARG		CTSTR& tag_00 = _T(""), CTSTR& tag_01 = _T(""), CTSTR& tag_02 = _T(""), CTSTR& tag_03 = _T("")

namespace NT3
{

// ****************************************************************
// ini �֘A
// ================================================================

class C_ini_file
{
public:

	// �V���O���g��
	static C_ini_file& body()
	{
		static C_ini_file singleton;
		return singleton;
	}

	// �O���錾
	struct S_ini_item;

	// ini �^�O���ƃf�[�^�̃}�b�v
	typedef	std::map<TSTR, S_ini_item>	T_ini_map;
	typedef	std::pair<TSTR, S_ini_item>	T_ini_pair;

	// ini �c���[�̃A�C�e���\����
	struct S_ini_item
	{
		// �ǂ��炩�̃f�[�^���g�p�����
		T_ini_map	item;
		TSTR		value;
	};

	void		clear_data();							// �f�[�^�N���A
	void		load_ini_file(CTSTR& path);				// ini �t�@�C����ǂݍ��ށi�t�@�C���p�X���w�肵�ēǂݍ��ށj
	void		load_ini_file();						// ini �t�@�C����ǂݍ��ށi���W���[�����̂h�m�h�t�@�C����ǂݍ��ށj
	void		save_ini_file();						// ini �t�@�C����ۑ�
	TSTR		get_module_ini_file_path();				// ���W���[������ ini �t�@�C���p�X���擾

	// ini �ɓo�^
	void		save_str(CTSTR& key, CTSTR& value, INI_ARG);				// ini �ɕ������o�^
	void		save_bool(CTSTR& key, bool value, INI_ARG);					// ini ��bool�l��o�^
	void		save_int(CTSTR& key, int value, INI_ARG);					// ini �ɐ�����o�^
	void		save_int64(CTSTR& key, __int64 value, INI_ARG);				// ini �ɐ�����o�^
	void		save_long(CTSTR& key, long value, INI_ARG);					// ini �ɐ�����o�^
	void		save_point(CTSTR& key, C_point point, INI_ARG);				// ini �ɓ_��o�^
	void		save_rect(CTSTR& key, C_rect rect, INI_ARG);				// ini �ɋ�`��o�^
	void		save_int_array(CTSTR& key, ARRAY<int>& value, INI_ARG);		// ini �ɐ������X�g��o�^

	// ini ����擾
	TSTR		load_str(CTSTR& key, CTSTR& def, INI_ARG);					// ini ���當������擾
	bool		load_bool(CTSTR& key, bool def, INI_ARG);					// ini ���� bool �l���擾
	int			load_int(CTSTR& key, int def, INI_ARG);						// ini ���琮�����擾
	__int64		load_int64(CTSTR& key, __int64 def, INI_ARG);				// ini ���琮�����擾
	long		load_long(CTSTR& key, long def, INI_ARG);					// ini ���琮�����擾
	float		load_float(CTSTR& key, float def, INI_ARG);					// ini ���珬�����擾
	C_point		load_point(CTSTR& key, C_point def, INI_ARG);				// ini ����_���擾
	C_rect		load_rect(CTSTR& key, C_rect def, INI_ARG);					// ini �����`���擾
	ARRAY<int>	load_int_array(CTSTR& key, ARRAY<int>& def, INI_ARG);		// ini ���� int �^���X�g���擾

	// ini ����폜
	void		delete_value(CTSTR& key, INI_ARG);							// ini ����폜

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

	void		get_nums(TSTR& str, ARRAY<int>* num_list);					// �����񂩂琔�l�𕡐��擾

	S_ini_item	top;		// ini �c���[�̐擪
	TSTR		file_path;	// �t�@�C���p�X

	// �R���X�g���N�^���B������
	C_ini_file()	{}
	~C_ini_file()	{}
};

}

#undef		INI_ARG
