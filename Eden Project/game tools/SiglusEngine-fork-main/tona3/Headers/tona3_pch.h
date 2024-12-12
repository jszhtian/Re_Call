#pragma		once

#define		STRICT					// 型チェックを厳密に行う
#define		WINVER			0x0600	// Windows Vista/7/8/10
#define		_WIN32_WINNT	0x0600	// Windows Vista/7/8/10
#define		WIN32_LEAN_AND_MEAN		// 使用頻度の低い API を除外？ winsock.h をインクルードするためには必須

#include	<windows.h>				// WIN32_LEAN_AND_MEAN が定義されているので、色々省略されてます。
									// おかげで他のヘッダを色々とインクルードする必要があります。

#include	<tchar.h>				// TCHAR
#include	<objbase.h>				// COM
#include	<commctrl.h>			// コモンコントロール
#include	<commdlg.h>				// コモンダイアログ
#include	<richedit.h>			// リッチエディット
#include	<mmreg.h>
#include	<mmsystem.h>
#include	<winsock2.h>
//#include	<dinput.h>

// ****************************************************************
// C 標準ライブラリ
// ================================================================
#include	<ctime>
#include	<cmath>
#include	<locale.h>				// ロケール
#include	<errno.h>				// errno

// 古い標準ライブラリがリンクされている場合、カットする。
// とな３では、基本的に libcmt/libcmtd (MT/MTd) を使います。
// libc/libcd (ML/MLd) がリンクされている場合、競合するのでカットする必要があるのです。
#pragma comment(linker, "/nodefaultlib:\"LIBC\" /nodefaultlib:\"LIBCD\"")
#pragma comment(linker, "/nodefaultlib:\"LIBCP\" /nodefaultlib:\"LIBCPD\"")
#pragma comment(linker, "/nodefaultlib:\"MSVCRT\" /nodefaultlib:\"MSVCRTD\"")

// 古いマクロを解除（代わりに STL の std::max や std::min を使ってください）
#undef		max
#undef		min

// ****************************************************************
// その他ライブラリ
// ================================================================
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "ws2_32.lib")

// ****************************************************************
// STL
// ================================================================
#include	<vector>
#include	<deque>
#include	<queue>
#include	<map>
#include	<set>
#include	<string>
#include	<fstream>

// ****************************************************************
// boost
// ================================================================
//		よく使う共有ポインタのみインクルード。
//		それ以外を使う場合は個別にインクルードしてください。
// ================================================================
#include	<boost/shared_ptr.hpp>
#include	<boost/shared_array.hpp>
#include	<boost/weak_ptr.hpp>
#include	<boost/intrusive_ptr.hpp>

#define		BSP		boost::shared_ptr
#define		BSA		boost::shared_array
#define		BSDC	boost::shared_dynamic_cast
#define		BWP		boost::weak_ptr
#define		BIP		boost::intrusive_ptr

// ****************************************************************
// COM を intrusive_ptr で使う場合の参照カウンタ操作関数
// ================================================================
inline void intrusive_ptr_add_ref(IUnknown* ptr)
{
	ptr->AddRef();
}
inline void intrusive_ptr_release(IUnknown* ptr)
{
	ptr->Release();
}

// ****************************************************************
// 型定義
// ================================================================
typedef		UINT_PTR			PTR;
typedef		unsigned __int64	QWORD;

// ****************************************************************
// メッセージ
// ================================================================
#define		WM_SOCKET_SELECT		(WM_APP + 0)
#define		WM_SOCKET_HOST			(WM_APP + 4)

// ****************************************************************
// テスト用フラグ
// ================================================================
extern	int		G_test0;
extern	int		G_test1;
extern	int		G_test2;
extern	int		G_test3;
extern	int		G_test4;
extern	int		G_test5;
extern	int		G_test6;
extern	int		G_test7;
extern	int		G_test8;
extern	int		G_test9;

#define safe_release(lpUnk) do { if ((lpUnk) != NULL) { (lpUnk)->Release(); (lpUnk) = NULL; } } while (0)
