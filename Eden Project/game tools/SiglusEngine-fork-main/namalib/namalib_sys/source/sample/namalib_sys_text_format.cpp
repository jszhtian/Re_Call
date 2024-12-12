#include	"pch.h"
#include	"namalib_sys_text_analyze.h"
#include	"namalib_sys_text_format.h"

// ****************************************************************
// �R�����g�폜�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
// ================================================================
bool Cnamalib_text_format::_tf_delete_comment(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list)
{
	TSTR new_text;
	TSTR::iterator itr;
	int line;
	bool ret = true;
	NAMALIB_SIMPLE_ERR_STRUCT err;

	// �R�����g�폜
	line = 1;
	itr = text.begin();
	while (1)	{
		// �I��
		if (*itr == _T('\0'))		{
			new_text += *itr++; break;
		}
		// ���s
		else if (*itr == _T('\n'))	{
			new_text += *itr++; line++;
		}
		// �s�R�����g�i;�j
		else if (*itr == _T(';'))	{
			itr++;
			while (1)	{
				if (*itr == _T('\0'))			{	break;		}		// �R�����g�I��
				else if (*itr == _T('\n'))		{	break;		}		// �R�����g�I��
				else							{	itr++;		}		// �R�����g����
			}
		}
		// �s�R�����g�i//�j
		else if (*itr == _T('/') && *(itr + 1) == _T('/'))	{
			itr += 2;
			while (1)	{
				if (*itr == _T('\0'))			{	break;		}		// �R�����g�I��
				else if (*itr == _T('\n'))		{	break;		}		// �R�����g�I��
				else							{	itr++;		}		// �R�����g����
			}
		}
		// �u���b�N�R�����g�i/* �` */�j
		else if (*itr == _T('/') && *(itr + 1) == _T('*'))	{
			itr += 2;
			err.line = line;
			while (1)				{
				// �u���b�N�R�����g�G���[�I��
				if (*itr == _T('\0'))	{
					if (err_list != NULL)	{
						err.msg = _T("�u���b�N�R�����g�������Ă��܂���B");
						err_list->push_back(err);
					}
					ret = false; break;
				}
				// �u���b�N�R�����g���̉��s
				else if (*itr == _T('\n'))	{
					new_text += *itr++; line++;
				}
				// �u���b�N�R�����g�I��
				else if (*itr == _T('*') && *(itr + 1) == _T('/'))	{
					itr += 2; break;
				}
				// �u���b�N�R�����g���̕���
				else	{
					itr++;
				}
			}
		}
		// ���p����i" �` "�j
		else if (*itr == _T('"'))	{
			ret = _ta_copy_hankaku_control(itr, new_text, line, err_list, true);
		}
		// �L���ȕ���
		else						{
			new_text += *itr++;
		}

		// �G���[�I��
		if	(ret == false)			{
			break;
		}
	}

	if (ret)	{
		text = new_text;
	}

	return ret;
}

// ****************************************************************
// �啶�����i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
// ================================================================
bool Cnamalib_text_format::_tf_change_to_upper(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list)
{
	TSTR new_text;
	TSTR::iterator itr;
	int line;
	bool ret = true;
	NAMALIB_SIMPLE_ERR_STRUCT err;

	// �R�����g�폜
	line = 1;
	itr = text.begin();
	while (1)	{
		// �I��
		if (*itr == _T('\0'))		{
			new_text += *itr++; break;
		}
		// ���s
		else if (*itr == _T('\n'))	{
			new_text += *itr++; line++;
		}
		// ���p����i" �` "�j
		else if (*itr == _T('"'))	{
			ret = _ta_copy_hankaku_control(itr, new_text, line, err_list, true);
		}
		// ������
		else if (is_small_alpha(*itr))	{
			new_text += *itr + LARGE_ALPHA_ADD; itr++;
		}
		// ���̑��̕���
		else						{
			new_text += *itr++;
		}

		// �G���[�I��
		if	(ret == false)			{
			break;
		}
	}

	if (ret)	{
		text = new_text;
	}

	return ret;
}

// ****************************************************************
// �P���t�H�[�}�b�g�i�R�����g�폜�^�啶�����j�i�����荞�ރe�L�X�g�̍Ō�� \0 ��t�����Ă������j
// ================================================================
bool Cnamalib_text_format::_tf_simple_format(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list)
{
	// �R�����g�폜
	if (_tf_delete_comment(text, err_list) == false)		{
		return false;
	}

	// �啶����
	if (_tf_change_to_upper(text, err_list) == false)	{
		return false;
	}

	return true;
}

