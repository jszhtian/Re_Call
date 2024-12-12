#pragma		once

#include	<Wbemidl.h>

#include	"tona3_string.h"

// ****************************************************************
// ���ƂȂR�F�G���[
// ================================================================
// �G���[���̎󂯎����F
//		�EGetLastError
//		�Eerrno
//		�E�����Œ�`�����G���[
//		�E�Ȃ�
// ================================================================

namespace NT3
{

// ****************************************************************
// CRT�iC RunTime�j�G���[
// ================================================================

// �Ō�̃G���[���� CRT �G���[�̃G���[���b�Z�[�W���擾����
TSTR get_crt_last_error_msg();

// ****************************************************************
// Windows �G���[
// ================================================================

// Windows �̃G���[���b�Z�[�W���擾����
TSTR get_win_error_msg(DWORD error_code);

// ****************************************************************
// HRESULT �G���[
// ================================================================

// HRESULT �̃G���[���b�Z�[�W���擾����
TSTR get_hresult_error_msg(HRESULT hr);

// ****************************************************************
// WBEM �G���[
// ================================================================

// WBEM �̃G���[���b�Z�[�W���擾����
TSTR get_wbem_error_msg(HRESULT hr);

// ****************************************************************
// �ƂȂR�F�G���[
// ================================================================

// �ƂȂR�G���[�^�C�v
enum E_ERROR_TYPE
{
	ERROR_TYPE_NONE,	// �G���[�Ȃ�
	ERROR_TYPE_CRT,		// CRT �G���[
	ERROR_TYPE_WIN,		// Windows API �G���[
	ERROR_TYPE_HRESULT,	// HRESULT �G���[
	ERROR_TYPE_WBEM,	// WBEM �G���[
	ERROR_TYPE_OTHERS,	// ���̑��̃G���[
};

// �ƂȂR�G���[�\����
struct S_ERROR
{
	E_ERROR_TYPE	error_type;		// �G���[�^�C�v
	int				error_no;		// �G���[�ԍ�
	TSTR			error_str;		// �G���[���b�Z�[�W
	TSTR			error_func;		// �G���[�����������֐���
};

// �Ō�̃G���[��ݒ�
void set_last_error(E_ERROR_TYPE error_type, int error_no, const TSTR& error_func);
void set_last_error(const TSTR& error_str, const TSTR& error_func);

// �Ō�̃G���[��ǉ�
void add_last_error(const TSTR& error_str, const TSTR& error_func);

// �Ō�̃G���[�� CRT �G���[�ɐݒ�
void set_last_error_crt(const TSTR& error_func);

// �Ō�̃G���[�� Windows �G���[�ɐݒ�
void set_last_error_win(const TSTR& error_func);
void set_last_error_win(int error_no, const TSTR& error_func);

// �Ō�̃G���[�� HRESULT �G���[�ɐݒ�
void set_last_error_hr(HRESULT hr, const TSTR& error_func);

// �Ō�̃G���[�� WBEM �G���[�ɐݒ�
void set_last_error_wbem(HRESULT hr, const TSTR& error_func);

// �Ō�̃G���[���u�����ȃp�����[�^�v�ɐݒ�
void set_last_error_invalid(const TSTR& error_func);

// �Ō�̃G���[�̃G���[���b�Z�[�W���擾
TSTR get_last_error_msg();

// �Ō�̃G���[���N���A
void clear_last_error();

// �Ō�̃G���[�����݂��邩�𔻒�
bool is_error();

}

