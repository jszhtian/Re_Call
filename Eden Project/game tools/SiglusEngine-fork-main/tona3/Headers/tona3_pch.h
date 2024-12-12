#pragma		once

#define		STRICT					// �^�`�F�b�N�������ɍs��
#define		WINVER			0x0600	// Windows Vista/7/8/10
#define		_WIN32_WINNT	0x0600	// Windows Vista/7/8/10
#define		WIN32_LEAN_AND_MEAN		// �g�p�p�x�̒Ⴂ API �����O�H winsock.h ���C���N���[�h���邽�߂ɂ͕K�{

#include	<windows.h>				// WIN32_LEAN_AND_MEAN ����`����Ă���̂ŁA�F�X�ȗ�����Ă܂��B
									// �������ő��̃w�b�_��F�X�ƃC���N���[�h����K�v������܂��B

#include	<tchar.h>				// TCHAR
#include	<objbase.h>				// COM
#include	<commctrl.h>			// �R�����R���g���[��
#include	<commdlg.h>				// �R�����_�C�A���O
#include	<richedit.h>			// ���b�`�G�f�B�b�g
#include	<mmreg.h>
#include	<mmsystem.h>
#include	<winsock2.h>
//#include	<dinput.h>

// ****************************************************************
// C �W�����C�u����
// ================================================================
#include	<ctime>
#include	<cmath>
#include	<locale.h>				// ���P�[��
#include	<errno.h>				// errno

// �Â��W�����C�u�����������N����Ă���ꍇ�A�J�b�g����B
// �ƂȂR�ł́A��{�I�� libcmt/libcmtd (MT/MTd) ���g���܂��B
// libc/libcd (ML/MLd) �������N����Ă���ꍇ�A��������̂ŃJ�b�g����K�v������̂ł��B
#pragma comment(linker, "/nodefaultlib:\"LIBC\" /nodefaultlib:\"LIBCD\"")
#pragma comment(linker, "/nodefaultlib:\"LIBCP\" /nodefaultlib:\"LIBCPD\"")
#pragma comment(linker, "/nodefaultlib:\"MSVCRT\" /nodefaultlib:\"MSVCRTD\"")

// �Â��}�N���������i����� STL �� std::max �� std::min ���g���Ă��������j
#undef		max
#undef		min

// ****************************************************************
// ���̑����C�u����
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
//		�悭�g�����L�|�C���^�̂݃C���N���[�h�B
//		����ȊO���g���ꍇ�͌ʂɃC���N���[�h���Ă��������B
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
// COM �� intrusive_ptr �Ŏg���ꍇ�̎Q�ƃJ�E���^����֐�
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
// �^��`
// ================================================================
typedef		UINT_PTR			PTR;
typedef		unsigned __int64	QWORD;

// ****************************************************************
// ���b�Z�[�W
// ================================================================
#define		WM_SOCKET_SELECT		(WM_APP + 0)
#define		WM_SOCKET_HOST			(WM_APP + 4)

// ****************************************************************
// �e�X�g�p�t���O
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
