#pragma once

#include "vcl_base.h"

typedef unsigned short Word;

#include "Messages.hpp"

typedef boost::function1<void, Messages::TMessage &> TWndMethod;
extern HWND AllocateHWnd( TWndMethod Method );// ���b�Z�[�W���������� TWndMethod �^
extern void DeallocateHWnd(HWND);

typedef Set<int,ssShift,ssDouble> TShiftState;

#include "Component.h"

//----------------------------------------------------------------------------
class TStrings : public System::TObject
{
protected:
	std::vector<AnsiString> m_Strings;
public:
	PROPERTY(int, Count);
	PROPERTY(AnsiString, Text);
	PROPERTY_ARY(AnsiString, Strings);

	virtual int Add(const AnsiString & s);
	virtual void Assign(TObject* Source );  // �R�s�[���� TStringList �I�u�W�F�N�g
	virtual void Delete(int Index );
	virtual void Clear();
	virtual int IndexOf(const AnsiString & s);
	virtual void Move(int CurIndex, int NewIndex);
	virtual void SaveToFile(const AnsiString &);
	virtual void LoadFromFile(const AnsiString & FileName);

	void BeginUpdate(); // ���X�g�r���[�̍X�V��}������, �ĕ`����s���Ƃ��́A EndUpdate ���\�b�h�����s����
	void EndUpdate();
};

//----------------------------------------------------------------------------
//TStringList �I�u�W�F�N�g�́ATStrings �I�u�W�F�N�g�ƂقƂ�Ǔ����@�\�����A�ȉ��Ƀ��X�g�A�b�v����@�\���ǉ�����Ă���B
//  ���X�g���ێ����镶����f�[�^���\�[�g����
//  �\�[�g����Ƃ��A�d�����镶����f�[�^���폜�ł���
//  ���X�g�̓��e��ύX����Ƃ��ɃC�x���g����������
class TStringList : public TStrings
{
public:
	std::map<AnsiString,AnsiString> Values;
	PROPERTY_ARY_R(AnsiString,Names);
	PROPERTY_VAR0(bool, Sorted);
	PROPERTY_VAR0(TDuplicates, Duplicates);
};

#include "Stream.h"

//----------------------------------------------------------------------------
class TCollectionItem : public System::TObject
{
};

class TCollection : public System::TObject
{
};


