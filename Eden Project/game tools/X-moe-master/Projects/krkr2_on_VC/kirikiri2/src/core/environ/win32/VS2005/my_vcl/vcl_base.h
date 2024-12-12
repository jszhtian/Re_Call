#pragma once

//----------------------------------------------------------------------------
// �ŏ��� warning����
#pragma warning(disable: 4800)
#pragma warning(disable: 4996)

//warning C4561: '__fastcall' �� '/clr' �X�C�b�`�Ƃ̌݊���������܂��� : '__stdcall' �ɕϊ����Ă��܂��B
// __fastcall �͖����ɁB
#pragma warning(disable: 4561)

// �{���L�p�ȃ��[�j���O�N
//warning C4244: '=' : 'TJS::tjs_int64' ���� 'TJS::tjs_uint' �ւ̕ϊ��ł��B�f�[�^��������\��������܂��B
#pragma warning(disable: 4244)
//warning C4018: '<=' : signed �� unsigned �̐��l���r���悤�Ƃ��܂����B
#pragma warning(disable: 4018)
//warning C4333: '>>' : �E�V�t�g�̉񐔂��������܂��B�f�[�^��������\��������܂�
#pragma warning(disable: 4333)
//warning C4793: 'vararg' : �֐� 'bool boost::detail::function::has_empty_target(...)' �p�Ƀl�C�e�B�u �R�[�h�̐������������܂�
#pragma warning(disable: 4793)

// �\���s�\�������[�j���O�������B�c���b
#pragma warning(disable: 4819)


//----------------------------------------------------------------------------
// �w�b�_�C���N���[�h
#include <wx/wxprec.h>
#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <wx/wfstream.h>

#include <windows.h>

//----------------------------------------------------------------------------
// �Ăяo���K��n
#define DYNAMIC virtual

// �}�l�[�W�R�[�h�ƃA���}�l�[�W�R�[�h�̃����N�Ŗ�肪�N����̂ŁA__fastcall �͎E���܂�
#define __fastcall

#define __published public
#define __closure
#define _export

#define PACKAGE

#define _stdcall(x) (_stdcall x)
#define __stdcall(x) (__stdcall x)
#define __cdecl(x) (__cdecl x)

#ifndef _USERENTRY
#define _USERENTRY __cdecl
#endif

//----------------------------------------------------------------------------
// �w�b�_�C���N���[�h(���앪)
#include "vcl_enums.h"
#include "AnsiString.h"
#include "systobj.h"
#include "SysUtils.hpp"

//----------------------------------------------------------------------------
// ���� #define �� typedef
// ��`�̂�
#define PROPERTY(type,Name) public: virtual type get##Name() const; virtual void set##Name(type); \
	__declspec( property(get=get##Name, put=set##Name) ) type Name;

// �Ǐo����p
#define PROPERTY_R(type,Name) public: virtual type get##Name() const; \
	__declspec( property(get=get##Name) ) type Name;

// ��`�̂�(�z��)
#define PROPERTY_ARY(type,Name) public: virtual type get##Name(int) const; virtual void set##Name(int,type); \
	__declspec( property(get=get##Name, put=set##Name) ) type Name[];

// ��`�̂�(�z��) �Ǐo����p
#define PROPERTY_ARY_R(type,Name) public: virtual type get##Name(int) const; \
	__declspec( property(get=get##Name) ) type Name[];

// ��`�̂�(�����z�֐�)
#define PROPERTY_abstract(type,Name) public: virtual type get##Name() const =0; virtual void set##Name(type)=0; \
	__declspec( property(get=get##Name, put=set##Name) ) type Name;

// �ϐ��ƃQ�b�^�Z�b�^�̎����܂�
#define PROPERTY_VAR0(type,Name) public: virtual type get##Name() const {return m_##Name;} virtual void set##Name(type _v){m_##Name=_v;} \
	protected: type m_##Name; public: __declspec( property(get=get##Name, put=set##Name) ) type Name;

// �ϐ��ƃQ�b�^�̂ݎ����܂�
#define PROPERTY_VAR1(type,Name) public: virtual type get##Name() const {return m_##Name;} virtual void set##Name(type _v); \
	protected: type m_##Name; public: __declspec( property(get=get##Name, put=set##Name) ) type Name;

// �ϐ��ƃQ�b�^�̂ݎ����܂�, �Z�b�^�͒�`���̖���
#define PROPERTY_VAR_R(type,Name) public: virtual type get##Name() const {return m_##Name;} \
	protected: type m_##Name; public: __declspec( property(get=get##Name) ) type Name;

// ����̕ϐ���Ώۂɂ�����
#define PROPERTY_VAR_sp(type,Name,Var) public: virtual type get##Name() const {return Var;} virtual void set##Name(type _v){Var=_v;} \
	__declspec( property(get=get##Name, put=set##Name) ) type Name;

//----------------------------------------------------------------------------
// main�֐��Ȃ��ɋ����ɉ�����������g���b�N1
template <class T, T** p>
class __THogeInstance
{
public:
	__THogeInstance()
	{
		*p = new T;
	}
	virtual ~__THogeInstance()
	{
		delete *p;
	}
};

// main�֐��Ȃ��ɋ����ɉ�����������g���b�N2
#define CALL_ON_STARTUP(x,y)     \
class x##StartUp         \
{                        \
public:                  \
	x##StartUp() { x(); }\
} x##StartUpInstance;

//----------------------------------------------------------------------------
// �Ȃ񂩂��̎�̃N���X��VCL�ɑ���
template <class T, int VMIN, int VMAX>
class Set
{
	bool m_Val[VMAX-VMIN+1];
public:
	typedef Set<T,VMIN,VMAX> _Type;
	Set() { memset(m_Val, 0, sizeof(bool) * (VMAX-VMIN+1) ); }
	Set( const _Type& s) { memcpy(m_Val, s.m_Val, sizeof(bool) * (VMAX-VMIN+1) ); }
	bool Contains(int V) { if ( V >= VMIN && V <= VMAX ) return m_Val[V-VMIN]; return false;  }
	_Type& operator<< (int V) { if ( V >= VMIN && V <= VMAX ) m_Val[V-VMIN] = true;  return *this; }
	_Type& operator>> (int V) { if ( V >= VMIN && V <= VMAX ) m_Val[V-VMIN] = false; return *this; }
	bool operator==(const _Type& V)
	{
		for(int i=0; i < VMAX-VMIN; i++)
			if ( m_Val[i] != V.m_Val[i] ) return false;
		return true;
	}
};

//----------------------------------------------------------------------------
// VCL�ł͂��̌^��`������炵���BVC�ɂ͖���
typedef unsigned int dliNotification;
// �ȉ��� BCB�ŗL����
#define MAXDRIVE MAX_PATH
#define MAXDIR MAX_PATH

#define _argv __argv
#define _argc __argc

#define __emit__(x,y,z) __asm _emit x; __asm _emit y; __asm _emit z;
//__pfnDliNotifyHook
//static FARPROC WINAPI DllLoadHook(dliNotification dliNotify,  DelayLoadInfo * pdli)
//dliNotification

//----------------------------------------------------------------------------
#define EVENT_FUNC1(x, y) boost::bind(boost::mem_fn(&x::y), this, _1)
#define EVENT_FUNC2(x, y) boost::bind(boost::mem_fn(&x::y), this, _1, _2)
#define EVENT_FUNC3(x, y) boost::bind(boost::mem_fn(&x::y), this, _1, _2, _3)
#define EVENT_FUNC4(x, y) boost::bind(boost::mem_fn(&x::y), this, _1, _2, _3, _4)
#define EVENT_FUNC5(x, y) boost::bind(boost::mem_fn(&x::y), this, _1, _2, _3, _4, _5)

//----------------------------------------------------------------------------
typedef Set<TFontStyle, fsBold, fsStrikeOut>  TFontStyles;
typedef Set<int,mbYes,mbHelp> TMsgDlgButtons;


//----------------------------------------------------------------------------
struct TCreateParams
{
  char*        Caption;          // �E�C���h�E�̃L���v�V����
  int          Style;            // ���A�X�^�C���iWS_xxx�j
  int          ExStyle;          // ���A�g���X�^�C���iWS_EX_xxx�j
  int          X;                // ���A���[�� X ���W
  int          Y;                // ���A��[�� Y ���W
  int          Width;            // ���A��
  int          Height;           // ���A����
  HWND         WndParent;        // �e�E�C���h�E�̃E�C���h�E�n���h��
  void*        Param;            // WM_CREATE ���b�Z�[�W�� LParam �Ƃ��ēn�����|�C���^
  tagWNDCLASSA WindowClass;      // WNDCLASSEX �\����
  char         WinClassName[64]; // �E�C���h�E�̃N���X��
};

//----------------------------------------------------------------------------
struct TSearchRec
{
  int               Time;        // �t�@�C���̃^�C���X�^���v
  int               Size;        // �t�@�C���̃T�C�Y�i�P�ʁF�o�C�g�j
  int               Attr;        // �t�@�C�������iFileGetAttr �֐����Q�Ɓj
  AnsiString        Name;        // �t�@�C����
  int               ExcludeAttr; // �܂܂Ȃ��t�@�C������
  int               FindHandle;  // �����n���h��
  _WIN32_FIND_DATAA FindData;    // WIN32_FIND_DATA �\����
};

//----------------------------------------------------------------------------
class TPoint : public POINT
{
public:
	TPoint() { x = y = 0; }
	TPoint(int _x,int _y) { x = _x; y = _y; }
	TPoint(const POINT& pt) { x = pt.x; y = pt.y; }
};
//----------------------------------------------------------------------------
class TRect : public RECT
{
public:
	TRect() {}

	TRect(const TPoint& TL, const TPoint& BR)
	{
		left   = TL.x;
		top    = TL.y;
		right  = BR.x;
		bottom = BR.y;
	}

	TRect(int l, int t, int r, int b)
	{
		left   = l;
		top    = t;
		right  = r;
		bottom = b;
	}

	TRect(RECT& r)
	{
		left   = r.left;
		top    = r.top;
		right  = r.right;
		bottom = r.bottom;
	}

	int Width () const { return right  - left; }
	int Height() const { return bottom - top ; }

	bool operator ==(const TRect& rc) const
	{
		return left ==  rc.left  && top == rc.top &&
			right == rc.right && bottom == rc.bottom;
	}

	bool operator !=(const TRect& rc) const 
	{  return !(rc==*this); }

	PROPERTY_VAR_sp(LONG,Left  , left  );
	PROPERTY_VAR_sp(LONG,Top   , top   );
	PROPERTY_VAR_sp(LONG,Right , right );
	PROPERTY_VAR_sp(LONG,Bottom, bottom);
};


extern PACKAGE AnsiString __fastcall ParamStr(int Index);
extern AnsiString Trim(const AnsiString&);

extern int FindFirst(const AnsiString & Path, int Attr, TSearchRec & F );
extern int FindNext(TSearchRec & F);
extern void FindClose(TSearchRec & F);

extern HINSTANCE HInstance;

// min, max���Astd::�ȉ��ɂ���ׂ��炵���B
// �}�N���ׂ͒��āAinline�֐��łɒu��������
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace std
{
	template <class T1,class T2> inline T1 min(T2 a, T2 b) { return (a>b)?b:a; }
	template <class T1,class T2> inline T1 max(T2 a, T2 b) { return (a>b)?a:b; }
};

// ���̑��A�֐��������蓖�Ă�}�N���u��
#define fnsplit _splitpath
#define strcasecmp _stricmp
#define strncasecmp _strnicmp

