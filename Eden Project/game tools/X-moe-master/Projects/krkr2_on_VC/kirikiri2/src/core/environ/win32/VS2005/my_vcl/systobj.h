#pragma once

#include "AnsiString.h"

//----------------------------------------------------------------------------
// TPersistent����
namespace System {
	class TObject
	{
	public:
		TObject();
		virtual ~TObject(); // dynamic_cast���g���̂ɁAvirtual�Ȋ֐����~�����B�f�X�g���N�^����
		// �{���̖߂�l��ShortString�B�߂�ǂ��̂ŁAAnsiString�ɁB���ƁAtypeid���Z�q���g���s���Astatic�ɂł��Ȃ�����
		AnsiString ClassName() const;

		virtual void Dispatch(void*);
	};
};
typedef System::TObject TObject;
