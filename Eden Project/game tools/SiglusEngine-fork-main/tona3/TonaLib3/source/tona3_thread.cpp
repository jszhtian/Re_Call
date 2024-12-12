#include	"tona3_pch.h"
#include	"tona3_thread.h"
#include	"tona3_error.h"

namespace NT3
{

// ****************************************************************
// セマフォ：コンストラクタ
// ================================================================
C_semaphore::C_semaphore()
{
	m_h_semaphore = NULL;
}

// ****************************************************************
// セマフォ：デストラクタ
// ================================================================
C_semaphore::~C_semaphore()
{
	close();
}

// ****************************************************************
// セマフォ：セマフォ作成
// ================================================================
bool C_semaphore::create()
{
	if (!close())
		return false;

	// セマフォ作成
	m_h_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
	if (m_h_semaphore == NULL)	{
		set_last_error_win(_T("CreateSemaphore"));
		return false;
	}

	return true;
}

// ****************************************************************
// セマフォ：セマフォを閉じる
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
// セマフォチェッカー：コンストラクタ・デストラクタ
// ================================================================
C_semaphore_checker::C_semaphore_checker(HANDLE h_semaphore)
{
	m_h_semaphore = h_semaphore;

	// セマフォチェック
	if (m_h_semaphore)
		WaitForSingleObject(h_semaphore, INFINITE);
}

C_semaphore_checker::~C_semaphore_checker()
{
	// セマフォ解放
	if (m_h_semaphore)
		ReleaseSemaphore(m_h_semaphore, 1, NULL);
}

// ****************************************************************
// スレッド：コンストラクタ
// ================================================================
C_thread::C_thread()
{
	m_h_thread = NULL;
}

// ****************************************************************
// スレッド：デストラクタ
// ================================================================
C_thread::~C_thread()
{
	close();
}

// ****************************************************************
// スレッド：スレッドを作成
// ================================================================
bool C_thread::create(LPTHREAD_START_ROUTINE thread_func, LPVOID pv_ref)
{
	if (!close())
		return false;

	// スレッドを作成
	DWORD thread_id = 0;
	m_h_thread = CreateThread(NULL, 0, thread_func, pv_ref, 0, &thread_id);
	if (m_h_thread == NULL)	{
		set_last_error_win(_T("CreateThread"));
		return false;
	}

	return true;
}

// ****************************************************************
// スレッド：スレッドを閉じる
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
// スレッド：スレッドを判定する
// ================================================================
bool C_thread::check()
{
	if (m_h_thread)	{
		DWORD result = ::WaitForSingleObject(m_h_thread, 0);
		if (result == WAIT_TIMEOUT)	{
			return true;	// スレッド実行中
		}
	}

	return false;
}

// ****************************************************************
// スレッド：スレッドの戻り値を取得する
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

