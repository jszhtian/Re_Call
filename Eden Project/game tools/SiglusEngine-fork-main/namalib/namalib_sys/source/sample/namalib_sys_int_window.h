#pragma		once

// ****************************************************************
// ���l�\���E�B���h�E
// ================================================================
class Cnamalib_num_window : public Cdialog
{

public:
				Cnamalib_num_window();										// �R���X�g���N�^

	void		ready(CTSTR& window_title_str_, CTSTR& section_name_str_, int num_cnt_);	// ����
	void		save_ini_last_state();										// �I���O�̏�Ԃ��h�m�h�ɋL��
	bool		load_ini_last_state();										// �I���O�̏�Ԃ��h�m�h����ǂݍ���
	void		open_close(HWND parent_h_wnd, bool parent_focus);			// �J���^����i�쐬�^�j���j
	void		start_open(HWND parent_h_wnd);								// �N�����̃I�[�v��
	void		set_num(int index, int val);								// ���l�ݒ�
	void		update_proc();												// �A�b�v�f�[�g�v���Z�X

	// �Y�����t������Z�q
	int& operator [](int index)
	{
		if (0 > index || index >= (int)num_list.size()){	return dummy_num;	}
		return num_list[index];
	}

protected:
	LRESULT		window_proc(UINT msg, WPARAM wParam, LPARAM lParam);	// �E�B���h�E�v���V�[�W��

	TSTR		window_title_str;		// �E�B���h�E�^�C�g��
	TSTR		section_name_str;		// �Z�N�V������
	ARRAY<int>	num_list;				// ���l�ϐ����X�g
	ARRAY<int>	num_list_last;			// ���l�ϐ����X�g�i�X�V�O�̒l�j

	Cc_listview		_item_lstv_numlist;

private:
	int			dummy_num;
};

