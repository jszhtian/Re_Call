#include "vcl.h"
#include "Forms.hpp"


//----------------------------------------------------------------------------
// �{���̖߂�l��ShortString�B�߂�ǂ��̂ŁAAnsiString�ɁB���ƁAtypeid���Z�q���g���s���Astatic�ɂł��Ȃ�����
AnsiString System::TObject::ClassName() const
{
	return AnsiString( typeid(*this).name() );
}

//----------------------------------------------------------------------------
int getHintHidePause()
{
	return 250;
}
void setHintHidePause(int v)
{
}

//----------------------------------------------------------------------------
bool getShowHint()
{
	return false;
}
void setShowHint(bool v)
{
}
//----------------------------------------------------------------------------
bool getShowMainForm()
{
	return true;
}
//----------------------------------------------------------------------------
void setShowMainForm(bool v)
{
}


//----------------------------------------------------------------------------
// System.hpp
PACKAGE AnsiString __fastcall ParamStr(int Index)
{
	return AnsiString(_argv[Index]);
}



//----------------------------------------------------------------------------
int FindFirst( const AnsiString & Path, int Attr, TSearchRec & F )
{
	return 0;
}
//----------------------------------------------------------------------------
int FindNext( TSearchRec &F )
{
	return 0;
}
//----------------------------------------------------------------------------
void FindClose( TSearchRec &F )
{
	return;
}
