#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"

namespace NT3
{

// ****************************************************************
// セマフォ
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
// セマフォチェッカー
// ================================================================
//		コンストラクタでロックを開始し、デストラクタで解放します。
//		使い方：
//		{											// 必ずスコープで囲います。
//			Csemaphore_checker semaphore_checker;	// 監視開始
//			// 処理									// 処理を行います。
//		}											// デストラクタで監視を終了します。
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
// スレッド
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

