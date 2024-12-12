#pragma		once

// ****************************************************************
// �e�L�X�g���
// ================================================================
class Cnamalib_text_analyze
{

public:

	// ���p����R�s�[�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
	static	bool		_ta_copy_hankaku_control(TSTR::iterator& itr, TSTR& dst_text, int err_line, NAMALIB_SIMPLE_ERR_LIST *err_list, bool control_moji_copy);

	// ���p����擾�i�����p����̃G���[�����͂��Ă��Ȃ��j
	static	TSTR		_ta_get_hankaku_control(TSTR::iterator& itr);

	// �s�擾
	static	TSTR		_ta_get_line(TSTR::iterator& itr);

	// ��؂蕶������
	static	bool		_ta_check_kugiri(TCHAR c);

	// ��؂�W�����v�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
	static	void		_ta_jump_kugiri(TSTR::iterator& itr);

	// �s�W�����v�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
	static	void		_ta_jump_line(TSTR::iterator& itr);

	// �g�[�N���擾�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
	static	TSTR		_ta_get_token(TSTR::iterator& itr, bool jump_kugiri_mod);

	// �g�[�N�����`�F�b�N����
	static	int			_ta_check_token(TSTR& token, int cnt, ...);
	// �g�[�N�����擾���`�F�b�N����i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
	static	int			_ta_get_check_token(TSTR::iterator& itr, bool jump_kugiri_mod, int cnt, ...);

	// �g�[�N�������X�g�`�F�b�N����
	static	int			_ta_find_token_list(TSTR& token, int list_cnt, TCHAR **top);
	// �g�[�N�����擾�����X�g�`�F�b�N����i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
	static	int			_ta_get_find_token_list(TSTR::iterator& itr, bool jump_kugiri_mod, int list_cnt, TCHAR **top);

	// �P�����`�F�b�N�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
	static	bool		_ta_check_moji(TSTR::iterator& itr, TCHAR moji, bool jump_kugiri_mod);

	// �w��̕����̌�́u���p����̕����v���P�擾
	static	bool		_ta_get_hanakau_control(TSTR::iterator& start_itr, TCHAR moji, TSTR& str);

	// �h�m�s�l���擾
	static	bool		_ta_get_int_num(TSTR::iterator& start_itr, int *num_);
	// �����Ȃ��h�m�s�l���擾
	static	bool		_ta_get_unsigned_int_num(TSTR::iterator& start_itr, int *num_);

	// �A������u���l�v�̌����擾
	static	int			_ta_get_nums_cnt(TSTR::iterator itr, TCHAR moji);
	// �A������u�����Ȃ����l�v�̌����擾
	static	int			_ta_get_unsigned_nums_cnt(TSTR::iterator itr, TCHAR moji);

	// �A������u���l�v���w�萔�擾�i�������Ă����Ȃ��Ă����s���܂��j
	static	bool		_ta_get_nums(TSTR::iterator& start_itr, TCHAR moji, int cnt, ...);
	// �A������u�����Ȃ����l�v���w�萔�擾�i�������Ă����Ȃ��Ă����s���܂��j
	static	bool		_ta_get_unsigned_nums(TSTR::iterator& start_itr, TCHAR moji, int cnt, ...);

	// �A������u���l�v���w�萔�擾�i�o�b�t�@�Łj�i�������Ă����Ȃ��Ă����s���܂��j
	static	bool		_ta_get_nums_buf(TSTR::iterator& start_itr, TCHAR moji, int cnt, int *nums_buf);
	// �A������u�����Ȃ����l�v���w�萔�擾�i�o�b�t�@�Łj�i�������Ă����Ȃ��Ă����s���܂��j
	static	bool		_ta_get_unsigned_nums_buf(TSTR::iterator& start_itr, TCHAR moji, int cnt, int *nums_buf);

	// �A������u���l�v���w�萔�擾�i�ϔz��Łj�i�������Ă����Ȃ��Ă����s���܂��j
	static	bool		_ta_get_nums_array(TSTR::iterator& start_itr, TCHAR moji, int cnt, ARRAY<int>& num_array, bool clear_flag);
	// �A������u�����Ȃ����l�v���w�萔�擾�i�ϔz��Łj�i�������Ă����Ȃ��Ă����s���܂��j
	static	bool		_ta_get_unsigned_nums_array(TSTR::iterator& start_itr, TCHAR moji, int cnt, ARRAY<int>& num_array, bool clear_flag);

protected:

	// ��͗p�̏I���C�e���[�^
	TSTR::iterator		_ta_analyze_end_itr;
};


