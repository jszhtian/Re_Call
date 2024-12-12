#include	"pch.h"
#include	"namalib_sys_text_analyze.h"

// ****************************************************************
// ���p����R�s�[�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
// ================================================================
bool Cnamalib_text_analyze::_ta_copy_hankaku_control(TSTR::iterator& itr, TSTR& dst_text, int err_line, NAMALIB_SIMPLE_ERR_LIST *err_list, bool control_moji_copy)
{
	bool ret = true;
	NAMALIB_SIMPLE_ERR_STRUCT err;

	// ���p����̎n�܂�̋L��
	if (control_moji_copy)	{	dst_text += *itr;	}
	itr++;

	while (1)	{
		if (_is_line_end(*itr))	{		// �G���[�I��
			if (err_list != NULL)	{
				err.line = err_line;
				err.msg = _T("���p���䂪�����Ă��܂���B�������́A���p����̓r���ŉ��s���Ă��܂��񂩁H");
				err_list->push_back(err);
			}
			ret = false; break;
		}
		else if (*itr == _T('"'))	{		// ���p����I��
			if (control_moji_copy)	{	dst_text += *itr;	}
			itr++; break;
		}
		else						{		// ���p���䕶��
			dst_text += *itr++;
		}
	}

	return ret;
}

// ****************************************************************
// ���p����擾�i�����p����̃G���[�����͂��Ă��Ȃ��j
// ================================================================
TSTR Cnamalib_text_analyze::_ta_get_hankaku_control(TSTR::iterator& itr)
{
	TSTR str;

	itr++;		// ���p����̎n�܂�̋L��

	while (1)	{
		if (*itr == _T('"'))	{		// ���p����I��
			itr++; break;
		}
		else					{		// ���p���䕶��
			str += *itr++;
		}
	}

	return str;
}

// ****************************************************************
// �s�擾
// ================================================================
TSTR Cnamalib_text_analyze::_ta_get_line(TSTR::iterator& itr)
{
	TSTR str;

	while (1)	{
		// �I���^���s
		if (_is_line_end(*itr))	{
			break;
		}
		// ���̑��̕���
		else	{
			str += *itr++;
		}
	}

	return str;
}

// ****************************************************************
// ��؂蕶������
// ================================================================
bool Cnamalib_text_analyze::_ta_check_kugiri(TCHAR c)
{
	if ((c == _T(' ')) || (c == _T('\t')) || (c == _T(',')))	{	return true;	}
	return false;
}

// ****************************************************************
// ��؂�W�����v�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
// ================================================================
void Cnamalib_text_analyze::_ta_jump_kugiri(TSTR::iterator& itr)
{
	while (1)	{
		// ��؂蕶��
		if ((*itr == _T(' ')) || (*itr == _T('\t')) || (*itr == _T(',')))	{	itr++;	}
		// ���̑��̕���
		else																{	break;	}
	}
}

// ****************************************************************
// �s�W�����v�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
// ================================================================
void Cnamalib_text_analyze::_ta_jump_line(TSTR::iterator& itr)
{
	while (1)	{
		// �I���^���s
		if (_is_line_end(*itr))	{	break;	}
		// ���̑��̕���
		else					{	itr++;	}
	}
}

// ****************************************************************
// �g�[�N���擾�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
// ================================================================
TSTR Cnamalib_text_analyze::_ta_get_token(TSTR::iterator& itr, bool jump_kugiri_mod)
{
	TSTR token;
	bool moji_get = false;

	// ��؂�W�����v
	if (jump_kugiri_mod)	{	_ta_jump_kugiri(itr);	}

	while (1)	{
		// �g�[�N������
		if (is_alpha(*itr) || *itr == _T('_') || *itr == _T('@'))	{
			token += *itr++;
			moji_get = true;
		}
		// �擪�Ɏg���Ȃ��g�[�N������
		else if (is_num(*itr))	{
			if (moji_get)	{	token += *itr++;	}
			else			{	break;				}
		}
		// ���̑��̕���
		else	{	break;	}
	}

	return token;
}

// ****************************************************************
// �g�[�N�����`�F�b�N����
// ================================================================
int Cnamalib_text_analyze::_ta_check_token(TSTR& token, int cnt, ...)
{
	TCHAR *str;
	if (token.empty())	{	return -1;	}
	va_list arg;
	va_start(arg, cnt);
	for (int i = 0; i < cnt; i++)	{
		str = va_arg(arg, TCHAR *);
		if (str_get_upper(token) == str)	{	return i;	}
	}
	va_end(arg);
	return -1;
}

// �g�[�N�����擾���`�F�b�N����i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
int Cnamalib_text_analyze::_ta_get_check_token(TSTR::iterator& itr, bool jump_kugiri_mod, int cnt, ...)
{
	TCHAR *str;
	TSTR token = _ta_get_token(itr, jump_kugiri_mod);
	if (token.empty())	{	return -1;	}
	va_list arg;
	va_start(arg, cnt);
	for (int i = 0; i < cnt; i++)	{
		str = va_arg(arg, TCHAR *);
		if (str_get_upper(token) == str)	{	return i;	}
	}
	va_end(arg);
	return -1;
}

// ****************************************************************
// �g�[�N�������X�g�`�F�b�N����
// ================================================================
int Cnamalib_text_analyze::_ta_find_token_list(TSTR& token, int list_cnt, TCHAR **top)
{
	if (token.empty())	{	return -1;	}
	TCHAR **end = top + list_cnt;
	TCHAR **res = std::find(top, end, str_get_upper(token));
	if (res == end)	{
		return -1;
	}
	return (int)((PTR)res - (PTR)top) / sizeof(TCHAR **);
}

// �g�[�N�����擾�����X�g�`�F�b�N����i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
int Cnamalib_text_analyze::_ta_get_find_token_list(TSTR::iterator& itr, bool jump_kugiri_mod, int list_cnt, TCHAR **top)
{
	TSTR token = _ta_get_token(itr, jump_kugiri_mod);
	if (token.empty())	{	return -1;	}
	return _ta_find_token_list(token, list_cnt, top);
}

// ****************************************************************
// �P�����`�F�b�N�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
// ================================================================
bool Cnamalib_text_analyze::_ta_check_moji(TSTR::iterator& itr, TCHAR moji, bool jump_kugiri_mod)
{
	TCHAR target_moji;

	// ��؂�W�����v
	if (jump_kugiri_mod)	{	_ta_jump_kugiri(itr);	}

	// �啶����
	if (is_small_alpha(moji))	{	moji += LARGE_ALPHA_ADD;	}

	// �啶����
	target_moji = *itr;
	if (is_small_alpha(target_moji))	{	target_moji += LARGE_ALPHA_ADD;		}

	// ��r
	if (target_moji != moji)	{	return false;	}

	// �������΂�
	itr++;

	return true;
}

// ****************************************************************
// �w��̕����̌�́u���p����̕����v���P�擾
// ================================================================
bool Cnamalib_text_analyze::_ta_get_hanakau_control(TSTR::iterator& start_itr, TCHAR moji, TSTR& str)
{
	TSTR::iterator itr = start_itr;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}

	_ta_jump_kugiri(itr);
	if (*itr != _T('"'))	{	return false;	}

	str = _ta_get_hankaku_control(itr);

	// �ԋp
	start_itr = itr;

	return true;
}

// ****************************************************************
// �h�m�s�l���擾
// ================================================================
bool Cnamalib_text_analyze::_ta_get_int_num(TSTR::iterator& start_itr, int *num_)
{
	TSTR::iterator itr = start_itr;

	// ����
	int sign = 1;

	// ��������
	if (*itr == _T('+'))		{				itr++;	_ta_jump_kugiri(itr);	}
	else if (*itr == _T('-'))	{	sign = -1;	itr++;	_ta_jump_kugiri(itr);	}

	// ���l����
	bool ok = false;
	int num = 0;
	while (1)	{
		if (is_num(*itr))	{	num = num * 10 + (*itr - _T('0'));	ok = true;	itr++;	}
		else				{	break;	}
	}

	// ���l���擾���Ă��Ȃ�
	if (!ok)	{	return false;	}

	// ����������
	num *= sign;

	// �ԋp
	start_itr = itr;
	if (num_)	{	*num_ = num;	}

	return true;
}

// �����Ȃ��h�m�s�l���擾
bool Cnamalib_text_analyze::_ta_get_unsigned_int_num(TSTR::iterator& start_itr, int *num_)
{
	TSTR::iterator itr = start_itr;

	// ���l����
	bool ok = false;
	int num = 0;
	while (1)	{
		if (is_num(*itr))	{	num = num * 10 + (*itr - _T('0'));	ok = true;	itr++;	}
		else				{	break;	}
	}

	// ���l���擾���Ă��Ȃ�
	if (!ok)	{	return false;	}

	// �ԋp
	start_itr = itr;
	if (num_)	{	*num_ = num;	}

	return true;
}

// ****************************************************************
// �A������u���l�v�̌����擾
// ================================================================
int Cnamalib_text_analyze::_ta_get_nums_cnt(TSTR::iterator itr, TCHAR moji)
{
	int cnt;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return 0;	}

	cnt = 0;
	while (1)	{
		_ta_jump_kugiri(itr);
		if (_ta_get_int_num(itr, NULL) == false)	{	break;	}
		cnt++;
	}

	return cnt;
}

// �A������u�����Ȃ����l�v�̌����擾
int Cnamalib_text_analyze::_ta_get_unsigned_nums_cnt(TSTR::iterator itr, TCHAR moji)
{
	int cnt;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return 0;	}

	cnt = 0;
	while (1)	{
		_ta_jump_kugiri(itr);
		if (_ta_get_unsigned_int_num(itr, NULL) == false)	{	break;	}
		cnt++;
	}

	return cnt;
}


// ****************************************************************
// �A������u���l�v���w�萔�擾�i�������Ă����Ȃ��Ă����s���܂��j
// ================================================================
bool Cnamalib_text_analyze::_ta_get_nums(TSTR::iterator& start_itr, TCHAR moji, int cnt, ...)
{
	TSTR::iterator itr = start_itr;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_nums_cnt(itr, _T('\0')) != cnt)							{	return false;	}

	va_list arg;
	va_start(arg, cnt);
	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_int_num(itr, va_arg(arg, int *));
	}
	va_end(arg);

	// �ԋp
	start_itr = itr;

	return true;
}

// �A������u�����Ȃ����l�v���w�萔�擾�i�������Ă����Ȃ��Ă����s���܂��j
bool Cnamalib_text_analyze::_ta_get_unsigned_nums(TSTR::iterator& start_itr, TCHAR moji, int cnt, ...)
{
	TSTR::iterator itr = start_itr;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_unsigned_nums_cnt(itr, _T('\0')) != cnt)				{	return false;	}

	va_list arg;
	va_start(arg, cnt);
	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_unsigned_int_num(itr, va_arg(arg, int *));
	}
	va_end(arg);

	// �ԋp
	start_itr = itr;

	return true;
}

// ****************************************************************
// �A������u���l�v���w�萔�擾�i�o�b�t�@�Łj�i�������Ă����Ȃ��Ă����s���܂��j
// ================================================================
bool Cnamalib_text_analyze::_ta_get_nums_buf(TSTR::iterator& start_itr, TCHAR moji, int cnt, int *nums_buf)
{
	TSTR::iterator itr = start_itr;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_nums_cnt(itr, _T('\0')) != cnt)							{	return false;	}

	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_int_num(itr, nums_buf);
		nums_buf++;
	}

	// �ԋp
	start_itr = itr;

	return true;
}

// �A������u�����Ȃ����l�v���w�萔�擾�i�o�b�t�@�Łj�i�������Ă����Ȃ��Ă����s���܂��j
bool Cnamalib_text_analyze::_ta_get_unsigned_nums_buf(TSTR::iterator& start_itr, TCHAR moji, int cnt, int *nums_buf)
{
	TSTR::iterator itr = start_itr;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_unsigned_nums_cnt(itr, _T('\0')) != cnt)				{	return false;	}

	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_unsigned_int_num(itr, nums_buf);
		nums_buf++;
	}

	// �ԋp
	start_itr = itr;

	return true;
}

// ****************************************************************
// �A������u���l�v���w�萔�擾�i�ϔz��Łj�i�������Ă����Ȃ��Ă����s���܂��j
// ================================================================
bool Cnamalib_text_analyze::_ta_get_nums_array(TSTR::iterator& start_itr, TCHAR moji, int cnt, ARRAY<int>& num_array, bool clear_flag)
{
	TSTR::iterator itr = start_itr;
	int num;

	if (clear_flag)	{	num_array.clear();	}

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_nums_cnt(itr, _T('\0')) != cnt)							{	return false;	}

	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_int_num(itr, &num);
		num_array.push_back(num);
	}

	// �ԋp
	start_itr = itr;

	return true;
}

// �A������u�����Ȃ����l�v���w�萔�擾�i�ϔz��Łj�i�������Ă����Ȃ��Ă����s���܂��j
bool Cnamalib_text_analyze::_ta_get_unsigned_nums_array(TSTR::iterator& start_itr, TCHAR moji, int cnt, ARRAY<int>& num_array, bool clear_flag)
{
	TSTR::iterator itr = start_itr;
	int num;

	if (clear_flag)	{	num_array.clear();	}

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_unsigned_nums_cnt(itr, _T('\0')) != cnt)				{	return false;	}

	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_unsigned_int_num(itr, &num);
		num_array.push_back(num);
	}

	// �ԋp
	start_itr = itr;

	return true;
}



