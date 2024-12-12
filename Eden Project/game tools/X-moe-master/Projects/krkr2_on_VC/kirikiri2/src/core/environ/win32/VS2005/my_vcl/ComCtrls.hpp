#pragma once

#include "vcl_base.h"
#include "Classes.hpp"
#include "Controls.hpp"
#include "ExtCtrls.hpp"
#include "Forms.hpp"
#include "Graphics.hpp"
#include "ImgList.hpp"
#include "Menus.hpp"
#include "Messages.hpp"
#include "StdCtrls.hpp"
#include "ToolWin.hpp"

class TToolBar : public TToolWindow
{
public:
	TToolBar(TComponent* owner);
};

//----------------------------------------------------------------------------
// TCustomMultiSelectListControl, TCustomListView�͗�
class TListColumn;
class TListItems;
class TListItem;
class TListView : public TCustomListControl
{
	std::vector<TListColumn> m_Column;
	TListItems* m_ListItems;
	wxListCtrl* m_wxListCtrl;
public:
	PROPERTY_ARY_R(TListColumn*, Column);
	PROPERTY_R(TListItems*, Items);
	PROPERTY_R(TListItem*, Selected);

	TListItem* GetItemAt( int X, int Y);    // �ʒu���w�肷�� X ���W�AY ���W
	bool IsEditing();
};

//----------------------------------------------------------------------------
class TListItem : public System::TObject
{
public:
	PROPERTY_VAR1(AnsiString, Caption);
	PROPERTY(bool,Selected);
	PROPERTY(TStrings*,SubItems);

	bool EditCaption(); // ���X�g���ڂ̃L���v�V�����̃C���v���[�X�ҏW���J�n����
};
//----------------------------------------------------------------------------
class TListItems : public System::TObject
{
	friend TListView;
public:
	PROPERTY(int, Count);
	PROPERTY_ARY_R(TListItem*, Item);
	PROPERTY_VAR_R(TListView*, Owner);// �{����TCustomListView�Ȃ񂾂��ǁA��

	TListItem* Add(void);
	void BeginUpdate(); // ���X�g�r���[�̍X�V��}������, �ĕ`����s���Ƃ��́A EndUpdate ���\�b�h�����s����
	void EndUpdate();
	void Delete( int Index );    // �폜���鍀�ڂ̃C���f�b�N�X
};
//----------------------------------------------------------------------------
typedef int TWidth;
class TListColumn : public System::TObject
{
public:
	PROPERTY_VAR0(TWidth, Width);
};

//----------------------------------------------------------------------------
// TCustomRichEdit �͗�
class TRichEdit : public TCustomMemo
{
public:
	TRichEdit( TComponent* );
	PROPERTY(TStrings*, Lines);
	PROPERTY(int, SelLength);
	PROPERTY(AnsiString, SelText);
	PROPERTY(int, SelStart);
//	SelStart
};
