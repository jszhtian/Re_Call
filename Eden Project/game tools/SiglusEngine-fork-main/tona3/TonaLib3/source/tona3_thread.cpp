#include	"tona3_pch.h"
#include	"tona3_thread.h"
#include	"tona3_error.h"

namespace NT3
{

// ****************************************************************
// �Z�}�t�H�F�R���X�g���N�^
// ================================================================
C_semaphore::C_semaphore()
{
	m_h_semaphore = NULL;
}

// ****************************************************************
// �Z�}�t�H�F�f�X�g���N�^
// ================================================================
C_semaphore::~C_semaphore()
{
	close();
}

// ****************************************************************
// �Z�}�t�H�F�Z�}�t�H�쐬
// ================================================================
bool C_semaphore::create()
{
	if (!close())
		return false;

	// �Z�}�t�H�쐬
	m_h_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
	if (m_h_semaphore == NULL)	{
		set_last_error_win(_T("CreateSemaphore"));
		return false;
	}

	return true;
}

// ****************************************************************
// �Z�}�t�H�F�Z�}�t�H�����
// ================================================================
bool C_semaphore::close()
{
	if (m_h_semaphore)	{
		if (CloseHandle(m_h_semaphore) == 0)	{
			set_last_error_win(_T("CloseHandle"));
			return false;
		}

		m_h_semaphore = NULL;
	}

	return true;
}

// ****************************************************************
// �Z�}�t�H�`�F�b�J�[�F�R���X�g���N�^�E�f�X�g���N�^
// ================================================================
C_semaphore_checker::C_semaphore_checker(HANDLE h_semaphore)
{
	m_h_semaphore = h_semaphore;

	// �Z�}�t�H�`�F�b�N
	if (m_h_semaphore)
		WaitForSingleObject(h_semaphore, INFINITE);
}

C_semaphore_checker::~C_semaphore_checker()
{
	// �Z�}�t�H���
	if (m_h_semaphore)
		ReleaseSemaphore(m_h_semaphore, 1, NULL);
}

// ****************************************************************
// �X���b�h�F�R���X�g���N�^
// ================================================================
C_thread::C_thread()
{
	m_h_thread = NULL;
}

// ****************************************************************
// �X���b�h�F�f�X�g���N�^
// ================================================================
C_thread::~C_thread()
{
	close();
}

// ****************************************************************
// �X���b�h�F�X���b�h���쐬
// ================================================================
bool C_thread::create(LPTHREAD_START_ROUTINE thread_func, LPVOID pv_ref)
{
	if (!close())
		return false;

	// �X���b�h���쐬
	DWORD thread_id = 0;
	m_h_thread = CreateThread(NULL, 0, thread_func, pv_ref, 0, &thread_id);
	if (m_h_thread == NULL)	{
		set_last_error_win(_T("CreateThread"));
		return false;
	}

	return true;
}

// ****************************************************************
// �X���b�h�F�X���b�h�����
// ================================================================
bool C_thread::close()
{
	if (m_h_thread)	{
		if (CloseHandle(m_h_thread) == 0)	{
			set_last_error_win(_T("CloseHandle"));
			return false;
		}

		m_h_thread = NULL;
	}

	return true;
}

// ****************************************************************
// �X���b�h�F�X���b�h�𔻒肷��
// ================================================================
bool C_thread::check()
{
	if (m_h_thread)	{
		DWORD result = ::WaitForSingleObject(m_h_thread, 0);
		if (result == WAIT_TIMEOUT)	{
			return true;	// �X���b�h���s��
		}
	}

	return false;
}

// ****************************************************************
// �X���b�h�F�X���b�h�̖߂�l���擾����
// ================================================================
bool C_thread::get_exit_code(DWORD* p_code)
{
	DWORD result = 0;
	if (!GetExitCodeThread(m_h_thread, &result))	{
		set_last_error_win(_T("GetExitCodeThread"));
		return false;
	}

	if (p_code)
		*p_code = result;

	return true;
}

}

