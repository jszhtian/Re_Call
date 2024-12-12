#include	"pch.h"
#include	"namalib_sys_text_analyze.h"

namespace Nnama
{

// ****************************************************************
// �s����
// ================================================================

// �擾
TSTR _nta_get_line(TSTR::const_iterator& itr)
{
	TSTR str;
	while (1)	{
		if (_nta_check_end(*itr))	{	break;				}		// �I���^���s
		else						{	str += *itr++;		}		// ���̑��̕���
	}
	return str;
}

// �擾�̂݁i�C�e���[�^��i�߂Ȃ��j
TSTR _nta_get_only_line(TSTR::const_iterator& start_itr)
{
	TSTR::const_iterator itr = start_itr;
	return _nta_get_line(itr);
}

// �W�����v
void _nta_jump_line(TSTR::const_iterator& itr)
{
	while (1)	{
		if (_nta_check_end(*itr))	{	break;	}		// �I���^���s
		else						{	itr++;	}		// ���̑��̕���
	}
}

// ****************************************************************
// �󔒕�������
// ================================================================

#define		SPACE_CHECK(c)			(c == _T(' ') || c == _T('\t'))

// ����
bool _nta_check_space(TCHAR moji)
{
	if (SPACE_CHECK(moji))	{	return true;	}
	return false;
}

// �擾
TSTR _nta_get_space(TSTR::const_iterator& itr)
{
	TSTR str;
	while (1)	{
		if (SPACE_CHECK(*itr))	{	str += *itr++;	}	// �󔒕���
		else					{	break;			}	// ���̑��̕���
	}
	return str;
}

// �擾�̂݁i�C�e���[�^��i�߂Ȃ��j
TSTR _nta_get_only_space(TSTR::const_iterator& start_itr)
{
	TSTR::const_iterator itr = start_itr;
	return _nta_get_space(itr);
}

// �W�����v
void _nta_jump_space(TSTR::const_iterator& itr)
{
	while (1)	{
		if (SPACE_CHECK(*itr))	{	itr++;	}	// �󔒕���
		else					{	break;	}	// ���̑��̕���
	}
}

// ���݈ʒu���u�O�v�̋󔒕����擾�i���݈ʒu�̕����͔��肵�Ȃ��j
TSTR _nta_get_space_forward(CTSTR& str, TSTR::const_iterator itr)
{
	if (str.empty())	{	return TSTR();	}
	TSTR::const_iterator top = str.begin();
	if (itr == top)		{	return TSTR();	}
	itr--;
	TSTR::const_iterator space_itr = str.end();
	while (1)	{
		if (_nta_check_end(*itr))			{	break;	}	// �I���^���s
		else if (_nta_check_space(*itr))	{				// �󔒕���
			space_itr = itr;	if (itr == top)	{	break;	}	itr--;
		}
		else								{	break;	}	// ���̑�
	}

	if (space_itr != str.end())	{	return _nta_get_space(space_itr);	}
	return TSTR();
}

// ��납��󔒕����폜
void _nta_delete_space_backward(TSTR& str)
{
	if (str.empty())	{	return;	}
	TSTR::iterator itr = str.end() - 1;
	TSTR::iterator top = str.begin();
	TSTR::iterator space_itr = str.end();
	while (1)	{
		if (_nta_check_space(*itr))	{						// �󔒕���
			space_itr = itr;	if (itr == top)	{	break;	}	itr--;
		}
		else									{	break;	}	// ���̑�
	}
	if (space_itr != str.end())	{	str.erase(space_itr, str.end());	}
}

// ****************************************************************
// ��؂蕶������
// ================================================================

#define		KUGIRI_CHECK(c)			(c == _T(' ') || c == _T('\t') || c == _T('\n') || c == _T('\0'))

// ����
bool _nta_check_kugiri(TCHAR moji)
{
	if (KUGIRI_CHECK(moji))	{	return true;	}
	return false;
}

// ****************************************************************
// �I�[��������
// ================================================================

#define		END_CHECK(c)			(c == _T('\n') || c == _T('\0'))

// ����
bool _nta_check_end(TCHAR moji)
{
	if (END_CHECK(moji))	{	return true;	}
	return false;
}

// ****************************************************************
// �g�[�N������
// ================================================================

#define		HANKAKU_TOKEN_CHECK(c)			(is_num(c) || is_alpha(c) || c == _T('@') || c == _T('_') || c == _T('*'))
#define		ZENKAKU_TOKEN_CHECK(c)			(is_zenkaku(c))

// �擾�i���p�̂݁j
TSTR _nta_get_token_hankaku_only(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (HANKAKU_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// �g�[�N������
		else							{	break;				}	// ���̑��̕���
	}
	return token;
}

// �擾�i�S�p�̂݁j
TSTR _nta_get_token_zenkaku_only(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (ZENKAKU_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// �g�[�N������
		else							{	break;				}	// ���̑��̕���
	}
	return token;
}

// �擾
TSTR _nta_get_token(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (ZENKAKU_TOKEN_CHECK(*itr) || HANKAKU_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// �g�[�N������
		else														{	break;				}	// ���̑��̕���
	}
	return token;
}

// �擾�i���l�p�j
TSTR _nta_get_token_for_num(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (HANKAKU_TOKEN_CHECK(*itr) || *itr == _T('-'))	{	token += *itr++;	}	// �g�[�N������
		else												{	break;				}	// ���̑��̕���
	}
	return token;
}

// ����
bool _nta_check_token(TSTR::const_iterator& start_itr, CTSTR token)
{
	if (token.empty())	{	return false;	}

	TSTR::const_iterator itr = start_itr;
	for (TSTR::const_iterator token_itr = token.begin(); token_itr != token.end(); ++token_itr)	{
		if (*itr != *token_itr)	{	return false;	}
		itr++;
	}

	start_itr = itr;	// �C�e���[�^��i�߂�
	return true;
}

// ����̂݁i�C�e���[�^��i�߂Ȃ��j
bool _nta_check_only_token(TSTR::const_iterator& start_itr, CTSTR token)
{
	if (token.empty())	{	return false;	}
	TSTR::const_iterator itr = start_itr;
	for (TSTR::const_iterator token_itr = token.begin(); token_itr != token.end(); ++token_itr)	{
		if (*itr != *token_itr)	{	return false;	}
		itr++;
	}
	return true;
}

// ****************************************************************
// ���g�[�N������
// ================================================================

#define		HANKAKU_AT_TOKEN_CHECK(c)			(is_num(c) || is_alpha(c) || c == _T('_'))
#define		ZENKAKU_AT_TOKEN_CHECK(c)			(c != _T('��') && is_zenkaku(c))

// �擾�i���p�̂݁j
TSTR _nta_get_at_token_hankaku_only(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (HANKAKU_AT_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// �g�[�N������
		else								{	break;				}	// ���̑��̕���
	}
	return token;
}

// �擾�i�S�p�̂݁j
TSTR _nta_get_at_token_zenkaku_only(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (ZENKAKU_AT_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// �g�[�N������
		else								{	break;				}	// ���̑��̕���
	}
	return token;
}

// �擾
TSTR _nta_get_at_token(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (ZENKAKU_AT_TOKEN_CHECK(*itr) || HANKAKU_AT_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// �g�[�N������
		else																{	break;				}	// ���̑��̕���
	}
	return token;
}

// ****************************************************************
// �Z�p���[�g����
// ================================================================

// ����
bool _nta_check_separate(TCHAR moji)
{
	if (ZENKAKU_TOKEN_CHECK(moji) || HANKAKU_TOKEN_CHECK(moji))	{	return false;	}	// �g�[�N������
	return true;
}

// ������̍ŏ��̕����𔻒�
bool _nta_check_separate_first_moji(CTSTR& str)
{
	if (str.empty())	{	return true;	}
	else if (_nta_check_separate(*(str.begin())))	{	return true;	}
	return false;
}

// ������̍Ō�̕����𔻒�
bool _nta_check_separate_last_moji(CTSTR& str)
{
	if (str.empty())	{	return true;	}
	else if (_nta_check_separate(*(str.end() - 1)))	{	return true;	}
	return false;
}

// ****************************************************************
// ���Z�p���[�g����
// ================================================================

// ����
bool _nta_check_at_separate(TCHAR moji)
{
	if (ZENKAKU_AT_TOKEN_CHECK(moji) || HANKAKU_AT_TOKEN_CHECK(moji))	{	return false;	}	// �g�[�N������
	return true;
}

// ������̍ŏ��̕����𔻒�
bool _nta_check_at_separate_first_moji(CTSTR& str)
{
	if (str.empty())	{	return true;	}
	else if (_nta_check_at_separate(*(str.begin())))	{	return true;	}
	return false;
}

// ������̍Ō�̕����𔻒�
bool _nta_check_at_separate_last_moji(CTSTR& str)
{
	if (str.empty())	{	return true;	}
	else if (_nta_check_at_separate(*(str.end() - 1)))	{	return true;	}
	return false;
}

// ****************************************************************
// �����񐧌�
// ================================================================

// �擾�F�_�u���N�I�[�e�[�V����
TSTR _nta_get_str_double_quotation(TSTR::const_iterator& itr, TCHAR escape_moji)
{
	TSTR token;
	if (*itr == _T('\"'))	{
		itr++;
		while (1)	{
			if (END_CHECK(*itr))			{	break;	}
			else if (*itr == _T('\"'))		{	itr++;	break;	}
			else if (*itr == escape_moji)	{	itr++;	token += *itr++;	}
			else							{	token += *itr++;	}
		}
	}
	return token;
}

// �擾�F�w��̕�����������܂�
TSTR _nta_get_str_find_moji(TSTR::const_iterator& itr, TCHAR target_moji, TCHAR escape_moji)
{
	TSTR token;
	while (1)	{
		if (END_CHECK(*itr))			{	break;	}
		else if (*itr == target_moji)	{	break;	}
		else if (*itr == escape_moji)	{	itr++;	token += *itr++;	}
		else							{	token += *itr++;	}
	}
	return token;
}

// ****************************************************************
// �A������w�蕶���̐���
// ================================================================

// �w�蕶���̐����擾
int _nta_get_moji_cnt(TSTR::const_iterator itr, TCHAR moji)
{
	int moji_cnt = 0;
	while (1)	{
		if (*itr != moji)	{	break;	}
		itr++;	moji_cnt++;
	}
	return moji_cnt;
}

// �w�蕶�����w�萔�W�����v����
void _nta_jump_moji(TSTR::const_iterator& itr, TCHAR moji, int cnt)
{
	if (cnt <= 0)	{	return;	}
	int moji_cnt = 0;
	while (1)	{
		if (*itr != moji)	{	break;	}
		itr++;	moji_cnt++;
		if (moji_cnt == cnt)	{	break;	}
	}
}

// ****************************************************************
// �󔒍s����
// ================================================================

// ����
bool _nta_check_space_line(TSTR::const_iterator itr)
{
	while (1)	{
		if (_nta_check_end(*itr))			{	return true;	}	// �I���^���s
		else if (_nta_check_space(*itr))	{	itr++;			}	// �󔒕���
		else								{	return false;	}	// ���̑��̕���
	}
}

// ���݈ʒu���u�O�v�͋󔒍s������i���݈ʒu�̕����͔��肵�Ȃ��j
bool _nta_check_space_line_forward(CTSTR& str, TSTR::const_iterator itr)
{
	if (str.empty())	{	return true;	}
	TSTR::const_iterator top = str.begin();
	if (itr == top)		{	return true;	}
	itr--;
	while (1)	{
		if (_nta_check_end(*itr))			{	return true;	}	// �I���^���s
		else if (_nta_check_space(*itr))	{						// �󔒕���
			if (itr == top)	{	return true;	}	itr--;
		}
		else								{	return false;	}	// ���̑�
	}
	return true;
}

// ���݈ʒu���u��v�͋󔒍s������i���݈ʒu�̕��������肷��j
bool _nta_check_space_line_backward(CTSTR& str, TSTR::const_iterator itr)
{
	if (str.empty())	{	return true;	}
	TSTR::const_iterator end = str.end();
	while (itr != end)	{
		if (_nta_check_end(*itr))			{	return true;	}	// �I���^���s
		else if (_nta_check_space(*itr))	{	itr++;			}	// �󔒕���
		else								{	return false;	}	// ���̑�
	}
	return true;
}

// ****************************************************************
// ���s����
// ================================================================

// ����
bool _nta_check_return_contain(CTSTR& str)
{
	if (str.empty())	{	return false;	}
	TSTR::const_iterator itr = str.begin();
	TSTR::const_iterator end = str.end();
	while (itr != end)	{
		if (*itr == _T('\n'))	{	return true;	}	// ���s
		itr++;
	}
	return false;
}

// �Ō�̕��������s������
bool _nta_check_return_last_moji(CTSTR& str)
{
	if (str.empty())	{	return true;	}
	TSTR::const_iterator itr = str.end() - 1;
	if (*itr == _T('\n'))	{	return true;	}
	return false;
}


}
