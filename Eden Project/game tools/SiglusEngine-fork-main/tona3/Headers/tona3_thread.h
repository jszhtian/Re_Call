#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"

namespace NT3
{

// ****************************************************************
// �Z�}�t�H
// ================================================================
class C_semaphore
{
public:
	C_semaphore();
	~C_semaphore();

	bool	create();
	bool	close();
	HANDLE	get_handle()	{	return m_h_semaphore;	}

private:
	HANDLE	m_h_semaphore;
};

// ****************************************************************
// �Z�}�t�H�`�F�b�J�[
// ================================================================
//		�R���X�g���N�^�Ń��b�N���J�n���A�f�X�g���N�^�ŉ�����܂��B
//		�g�����F
//		{											// �K���X�R�[�v�ň͂��܂��B
//			Csemaphore_checker semaphore_checker;	// �Ď��J�n
//			// ����									// �������s���܂��B
//		}											// �f�X�g���N�^�ŊĎ����I�����܂��B
//
// ================================================================
class C_semaphore_checker
{
public:
	C_semaphore_checker(HANDLE h_semaphore);
	~C_semaphore_checker();

private:
	HANDLE	m_h_semaphore;
};


// ****************************************************************
// �X���b�h
// ================================================================
class C_thread
{
public:
	C_thread();
	~C_thread();

	bool	create(LPTHREAD_START_ROUTINE thread_func, LPVOID pv_ref);		// DWORD WINAPI thread_func(LPVOID pv_ref);
	bool	close();
	bool	check();
	bool	get_exit_code(DWORD* p_code);
	HANDLE	get_handle()	{	return m_h_thread;	}

private:
	HANDLE	m_h_thread;
};


}

