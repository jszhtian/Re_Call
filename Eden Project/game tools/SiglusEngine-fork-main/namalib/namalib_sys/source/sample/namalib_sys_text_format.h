#pragma		once

// ****************************************************************
// �e�L�X�g���`
// ================================================================
class Cnamalib_text_format : public Cnamalib_text_analyze
{

public:

	// �R�����g�폜�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
	static	bool		_tf_delete_comment(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list);

	// �啶�����i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
	static	bool		_tf_change_to_upper(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list);

	// �P���t�H�[�}�b�g�i�R�����g�폜�^�啶�����j�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
	static	bool		_tf_simple_format(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list);
};


