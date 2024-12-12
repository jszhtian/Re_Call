#include	"pch.h"
#include	"namalib_sys_text_analyze.h"
#include	"namalib_sys_text_format.h"

namespace Nnama
{

// ****************************************************************
// �R�����g�폜
// ================================================================

// �ϊ��i���p����̃G���[�`�F�b�N�����˂Ă���j
bool _ntf_delete_comment(TSTR& text, int* err_line, TSTR& err_msg)
{
	TSTR::iterator itr = text.begin();
	int line_no = 1;
	TSTR new_text;

	while (1)	{
		// �I��
		if (*itr == _T('\0'))		{
			new_text += *itr++;
			text = new_text;
			return true;
		}

		// ���s
		else if (*itr == _T('\n'))	{
			new_text += *itr++;		line_no++;
		}

		// �s�R�����g�i;�j
		else if (*itr == _T(';'))	{
			_nta_jump_line(itr);
		}

		// �s�R�����g�i//�j
		else if (*itr == _T('/') && *(itr + 1) == _T('/'))	{
			_nta_jump_line(itr);
		}

		// �u���b�N�R�����g�i/* �` */�j
		else if (*itr == _T('/') && *(itr + 1) == _T('*'))	{
			itr += 2;
			*err_line = line_no;
			while (1)				{
				// �u���b�N�R�����g�G���[
				if (*itr == _T('\0'))	{
					err_msg = _T("�u���b�N�R�����g�������Ă��܂���B");
					return false;
				}
				// �u���b�N�R�����g���̉��s
				else if (*itr == _T('\n'))	{
					new_text += *itr++;		line_no++;
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

		// �L���ȕ���
		else						{
			new_text += *itr++;
		}
	}
}

// �ϊ��i//* = ssa�j�i���p����̃G���[�`�F�b�N�����˂Ă���j
bool _ntf_delete_comment_ssa(TSTR& text, int* err_line, TSTR& err_msg)
{
	TSTR::iterator itr = text.begin();
	int line_no = 1;
	TSTR new_text;

	while (1)	{
		// �I��
		if (*itr == _T('\0'))		{
			new_text += *itr++;
			text = new_text;
			return true;
		}

		// ���s
		else if (*itr == _T('\n'))	{
			new_text += *itr++;		line_no++;
		}

		// �s�R�����g�i//*�j
		else if (*itr == _T('/') && *(itr + 1) == _T('/') && *(itr + 2) == _T('*'))	{
			_nta_jump_line(itr);
		}

		// �L���ȕ���
		else						{
			new_text += *itr++;
		}
	}
}

// ****************************************************************
// �啶����
// ================================================================

// �擾
TSTR _ntf_ret_change_to_upper(CTSTR& text)
{
	TSTR::const_iterator itr = text.begin();
	TSTR new_text;

	while (1)	{
		// �I��
		if (*itr == _T('\0'))	{	new_text += *itr++;		return new_text;	}
		// ������
		else if (is_small_alpha(*itr))	{	new_text += *itr++ + LARGE_ALPHA_ADD;	}
		// ���̑��̕���
		else	{	new_text += *itr++;		}
	}
}

// �ϊ�
void _ntf_change_to_upper(TSTR& text)
{
	text = _ntf_ret_change_to_upper(text);
}

// ****************************************************************
// �s���̕s�v�ȋ󔒕������폜
// ================================================================

// �擾
TSTR _ntf_ret_delete_line_end_space(TSTR& text)
{
	TSTR new_text;
	TSTR::iterator itr = text.begin();
	while (1)	{
		// �I��
		if (*itr == _T('\0'))		{	new_text += *itr++;	return new_text;	}
		// ���s
		else if (*itr == _T('\n'))	{	new_text += *itr++;	}
		// �󔒕���
		else if (_nta_check_space(*itr))	{
			TSTR::iterator backup_itr = itr;
			while (1)	{
				// �󔒃W�����v
				_nta_jump_space(itr);
				// �I��
				if (_nta_check_end(*itr))	{	break;	}
				// ���̑�
				else	{	new_text += TSTR(backup_itr, itr);	break;	}
			}
		}
		// ���̑�
		else	{	new_text += *itr++;	}
	}
}

// �ϊ�
void _ntf_delete_line_end_space(TSTR& text)
{
	text = _ntf_ret_delete_line_end_space(text);
}

// ****************************************************************
// ����̕s�v�ȋ󔒍s���폜
// ================================================================

// �擾
TSTR _ntf_ret_delete_backward_space_line(TSTR& text)
{
	TSTR new_text;
	TSTR::iterator itr = text.begin();
	while (1)	{
		// �I��
		if (*itr == _T('\0'))		{	new_text += *itr++;	return new_text;	}
		// ���s�������͋󔒕���
		else if (*itr == _T('\n') || _nta_check_space(*itr))	{
			TSTR::iterator backup_itr = itr;
			while (1)	{
				// �󔒃W�����v
				_nta_jump_space(itr);
				// �I��
				if (*itr == _T('\0'))	{	break;	}
				// ���s
				else if (*itr == _T('\n'))	{	itr++;	}
				// ���̑�
				else	{	new_text += TSTR(backup_itr, itr);	break;	}
			}
		}
		// ���̑�
		else	{
			new_text += _nta_get_line(itr);
			if (*itr == _T('\n'))	{	new_text += *itr++;	}
		}
	}
}

// �ϊ�
void _ntf_delete_backward_space_line(TSTR& text)
{
	text = _ntf_ret_delete_backward_space_line(text);
}

// ****************************************************************
// �C���f���g�}��
// ================================================================

// �擾
TSTR _ntf_ret_insert_indent(CTSTR& text, bool is_first_line, bool is_last_line, CTSTR& indent)
{
	TSTR::const_iterator itr = text.begin();
	TSTR new_text;

	while (1)	{
		if (is_first_line)	{
			if (is_last_line)	{	new_text += indent;	}
			else if (*itr != _T('\0'))	{	new_text += indent;	}
		}
		while (1)	{
			// �I��
			if (*itr == _T('\0'))	{	new_text += *itr++;	return new_text;	}
			// ���s
			else if (*itr == _T('\n'))	{
				new_text += *itr++;
				// �A��������s�̓C���f���g���Ȃ�
				while (1)	{
					if (*itr == _T('\n'))	{	new_text += *itr++;	}
					else	{	break;	}
				}
				break;
			}
			// �I���^���s
			else if (_nta_check_end(*itr))	{	break;	}
			// ���̑��̕���
			else	{	new_text += *itr++;		}
		}
		is_first_line = true;
	}
}

// �ϊ�
void _ntf_insert_indent(TSTR& text, bool is_first_line, bool is_last_line, CTSTR& indent)
{
	text = _ntf_ret_insert_indent(text, is_first_line, is_last_line, indent);
}

// ****************************************************************
// �P�s��
// ================================================================

// �擾
TSTR _ntf_ret_change_to_line(CTSTR& text)
{
	TSTR::const_iterator itr = text.begin();
	TSTR new_text;
	bool hankaku_space_insert = true;

	while (1)	{
		// �s���̃^�u�y�є��p�X�y�[�X���P�̔��p�X�y�[�X�ɂ���
		if (_nta_check_space(*itr))	{
			itr++;	_nta_jump_space(itr);
			// ���p�X�y�[�X�u������
			if (hankaku_space_insert == false && !_nta_check_end(*itr))	{
				new_text += _T(' ');
				hankaku_space_insert = true;
			}
		}

		while (1)	{
			// �I��
			if (*itr == _T('\0'))	{
				// �Ō�ɒǉ��������������p�X�y�[�X�Ȃ�폜����
				if (!new_text.empty())	{
					if (*(new_text.end() - 1) == _T(' '))	{	new_text.erase(new_text.end() - 1, new_text.end());	}
				}
				new_text += *itr++;
				return new_text;
			}
			// ���s
			else if (*itr == _T('\n'))	{
				itr++;
				// ���p�X�y�[�X�u������
				if (hankaku_space_insert == false)	{
					new_text += _T(' ');
					hankaku_space_insert = true;
				}
				break;
			}
			else	{
				// �s���̃^�u�y�є��p�X�y�[�X���폜����
				if (_nta_check_space(*itr))	{
					TSTR space_str = _nta_get_space(itr);
					// �s���łȂ��Ȃ炻�̂܂܎c��
					if (!_nta_check_end(*itr))	{
						new_text += space_str;
					}
				}
				// ���̑��̕���
				else	{	new_text += *itr++;		}

				hankaku_space_insert = false;
			}
		}
	}
}

// �ϊ�
void _ntf_change_to_line(TSTR& text)
{
	text = _ntf_ret_change_to_line(text);
}

}

