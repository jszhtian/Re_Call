#include "Forms.hpp"

class MyFrameTCustomForm : public wxFrame
{
	friend TCustomForm;
private:
	TCustomForm * m_pForm;
	int m_nLastX, m_nLastY;
	bool m_bOnCloseCalledBeforeDestroy;
public:
	//------------------------------------
	MyFrameTCustomForm(TCustomForm * pForm) :
		wxFrame(NULL, -1, _T("Canvas Frame"), wxPoint(10, 10), wxSize(300, 300), wxDEFAULT_FRAME_STYLE)
	{
		m_pForm = pForm;
		m_bOnCloseCalledBeforeDestroy = false;
	}
	//------------------------------------
	// �f�X�g���N�^������BMyFrameTCustomForm��wxFrame�͑΂œ��삷��B
	// wxWidgets�̃t���[�����[�N����A���ڂ��̃N���X�̃C���X�^���X���j�󂳂�Ă��܂��P�[�X������
	// ���̏ꍇ�ɁATForm�̃f�X�g���N�^���� delete�����s���č���B
	// ���z�́A�Е��� delete ���悤�Ƃ�����A�݂��ɑ����� delete����\�����H�@���܂������Ȃ��E�E
	virtual ~MyFrameTCustomForm()
	{
		if ( m_pForm )
		{
			m_pForm->m_wxFrame = NULL;
			TCustomForm * p = m_pForm;
			m_pForm = NULL;
			delete p;
		}
	}
	//------------------------------------
	void OnPaint(wxPaintEvent& event)
	{
		wxPaintDC dc(this);
		int i, n = m_pForm->ComponentCount;
		for(i=0; i < n; i++)
		{
			TComponent* _p = m_pForm->Components[i];
			TPaintBox* p = dynamic_cast<TPaintBox*>(_p);
			if ( p && p->OnPaint ) p->OnPaint( p );
		}
		if ( m_pForm->OnPaint ) m_pForm->OnPaint( m_pForm );
	}
	//------------------------------------
	void OnClick()
	{
		int i, n = m_pForm->ComponentCount;
		for(i=0; i < n; i++)
		{
			TComponent* _p = m_pForm->Components[i];
			TPaintBox* p = dynamic_cast<TPaintBox*>(_p);
			if ( p && p->OnClick ) p->OnClick( p );
		}
		if ( m_pForm->OnClick ) m_pForm->OnClick( m_pForm );
	}
	//------------------------------------
	void OnMouseDown(wxMouseEvent& event)
	{
		TMouseButton btn;
		if (event.LeftDown())
		{
			btn = mbLeft;
			m_nLastX = event.m_x;
			m_nLastY = event.m_y;
		}
		else
		{
			m_nLastX = m_nLastY = -1;
			if (event.RightDown())  btn = mbRight;
			else if (event.MiddleDown()) btn = mbMiddle;
		}

		TShiftState shift;
		if (event.AltDown()  ) shift << ssAlt;
		if (event.ShiftDown()) shift << ssShift;

		int i, n = m_pForm->ComponentCount;
		for(i=0; i < n; i++)
		{
			TComponent* _p = m_pForm->Components[i];
			TPaintBox* p = dynamic_cast<TPaintBox*>(_p);
			if ( p && p->OnMouseDown ) p->OnMouseDown( p, btn, shift, event.m_x, event.m_y );
		}
		if ( m_pForm->OnMouseDown ) m_pForm->OnMouseDown( m_pForm, btn, shift, event.m_x, event.m_y );
		event.Skip();
	}
	//------------------------------------
	void OnMouseUp(wxMouseEvent& event)
	{
		TMouseButton btn;
		if (event.LeftUp())
		{
			btn = mbLeft;
			if ( abs(m_nLastX - event.m_x) < 4 && abs(m_nLastY - event.m_y) < 4 )
				OnClick();
		}
		else
		{
			m_nLastX = m_nLastY = -1;
			if (event.RightUp())  btn = mbRight;
			else if (event.MiddleUp()) btn = mbMiddle;
		}

		TShiftState shift;
		if (event.AltDown()  ) shift << ssAlt;
		if (event.ShiftDown()) shift << ssShift;

		int i, n = m_pForm->ComponentCount;
		for(i=0; i < n; i++)
		{
			TComponent* _p = m_pForm->Components[i];
			TPaintBox* p = dynamic_cast<TPaintBox*>(_p);
			if ( p && p->OnMouseUp ) p->OnMouseUp( p, btn, shift, event.m_x, event.m_y );
		}
		if ( m_pForm->OnMouseUp ) m_pForm->OnMouseUp( m_pForm, btn, shift, event.m_x, event.m_y );
	}
	//------------------------------------
	void OnLeftDClick(wxMouseEvent& event)
	{
		int i, n = m_pForm->ComponentCount;
		for(i=0; i < n; i++)
		{
			TComponent* _p = m_pForm->Components[i];
			TPaintBox* p = dynamic_cast<TPaintBox*>(_p);
			if ( p && p->OnDblClick ) p->OnDblClick( p );
		}
		if ( m_pForm->OnDblClick ) m_pForm->OnDblClick( m_pForm );
	}
	//------------------------------------
	void OnMotion(wxMouseEvent& event)
	{
		TShiftState shift;
		if (event.AltDown()  ) shift << ssAlt;
		if (event.ShiftDown()) shift << ssShift;

		int i, n = m_pForm->ComponentCount;
		for(i=0; i < n; i++)
		{
			TComponent* _p = m_pForm->Components[i];
			TPaintBox* p = dynamic_cast<TPaintBox*>(_p);
			if ( p && p->OnMouseMove ) p->OnMouseMove( p, shift, event.m_x, event.m_y );
		}
		if ( m_pForm->OnMouseMove ) m_pForm->OnMouseMove( m_pForm, shift, event.m_x, event.m_y );
	}

	//------------------------------------
	int TranslateKey(int key)
	{
		switch(key)
		{
		case WXK_LEFT:  return VK_LEFT;
		case WXK_UP:    return VK_UP;
		case WXK_RIGHT: return VK_RIGHT;
		case WXK_DOWN:  return VK_DOWN;
		}
		return key;
	}
	//------------------------------------
	void OnKeyDown(wxKeyEvent& event)
	{
		Word key = TranslateKey( event.GetKeyCode() );

		if ( m_pForm->OnKeyDown )
			m_pForm->OnKeyDown( m_pForm, key, TShiftState() );
	}
	//------------------------------------
	void OnKeyUp(wxKeyEvent& event)
	{
		Word key = TranslateKey( event.GetKeyCode() );

		if ( m_pForm->OnKeyUp )
			m_pForm->OnKeyUp( m_pForm, key, TShiftState() );
	}
	//------------------------------------
	void OnKeyPress(wxKeyEvent& event)
	{
		char key = TranslateKey( event.GetKeyCode() );

		if ( m_pForm->OnKeyPress )
			m_pForm->OnKeyPress( m_pForm, key );
	}
	//------------------------------------
	void OnSize(wxSizeEvent& event)
	{
	}
	//------------------------------------
	void OnClose(wxCloseEvent& event)
	{
		OutputDebugString("OnClose\n");
		TCloseAction Action = caFree;
		if ( event.CanVeto() == false )
		{
			if ( m_pForm && m_pForm->OnClose )
				m_pForm->OnClose(m_pForm, Action);
			m_bOnCloseCalledBeforeDestroy = true;
			Destroy();
			return;
		}

		// ���낢��[�܂�
		// �ŏ��ɃN���[�Y�\���₢���킹
		// ���Ԃ�A���̎��_�őS�q�E�B���h�E�ɖ₢���킹������ł��傤���A�[�܂�
		bool CanClose = true;
		if ( m_pForm && m_pForm->OnCloseQuery )
			m_pForm->OnCloseQuery(m_pForm, CanClose);

		// ���ɁA�N���[�Y�Ƀg���C
		if ( CanClose == true )
		{
			if ( m_pForm && m_pForm->OnClose )
				m_pForm->OnClose(m_pForm, Action);
			switch( Action )
			{
			case caHide:
				Hide();
			case caNone:
				event.Veto();
				break;
			case caFree:
				m_bOnCloseCalledBeforeDestroy = true;
				Destroy();
				break;
			}
		}
		else
			event.Veto();
	}
	//------------------------------------
	virtual bool Destroy()
	{
		if ( m_bOnCloseCalledBeforeDestroy == false && m_pForm && m_pForm->OnClose )
		{
			TCloseAction Action = caFree;
			m_pForm->OnClose(m_pForm, Action);
		}

		bool res = wxFrame::Destroy();
		//m_pForm = NULL;
		return res;
	}


	//------------------------------------
	WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
	{
//		if ( message >= WM_USER && message < WM_USER + 200 )
		{
#ifdef _DEBUG
//			char buf[1024];
//			sprintf(buf, "WM_USER + %d(%xh)\n", message - WM_USER, message - WM_USER );
//			OutputDebugString(buf);
#endif
			TMessage msg = {message, wParam, lParam, 0};
			if ( m_pForm && m_pForm->HandleMessageMap(msg) == true )
				return msg.Result;
		}
		return wxFrame::MSWWindowProc(message, wParam, lParam);
	}

	//------------------------------------
	// TForm::Dispatch���Ă΂ꂽ�Ƃ��̓R�����s��
	WXLRESULT MSWWindowProcForDispatch(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
	{
		return wxFrame::MSWWindowProc(message, wParam, lParam);
	}
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrameTCustomForm, wxFrame)
	EVT_PAINT(MyFrameTCustomForm::OnPaint)
	EVT_SIZE(MyFrameTCustomForm::OnSize)
	EVT_CLOSE(MyFrameTCustomForm::OnClose)

	EVT_LEFT_DOWN   (MyFrameTCustomForm::OnMouseDown)
	EVT_RIGHT_DOWN  (MyFrameTCustomForm::OnMouseDown)
	EVT_MIDDLE_DOWN (MyFrameTCustomForm::OnMouseDown)
	EVT_LEFT_UP     (MyFrameTCustomForm::OnMouseUp)
	EVT_RIGHT_UP    (MyFrameTCustomForm::OnMouseUp)
	EVT_MIDDLE_UP   (MyFrameTCustomForm::OnMouseUp)
	EVT_LEFT_DCLICK (MyFrameTCustomForm::OnLeftDClick)
	EVT_MOTION      (MyFrameTCustomForm::OnMotion)

	EVT_KEY_DOWN    (MyFrameTCustomForm::OnKeyDown)
	EVT_KEY_UP      (MyFrameTCustomForm::OnKeyUp)
	EVT_CHAR        (MyFrameTCustomForm::OnKeyPress)
END_EVENT_TABLE()


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TScrollingWinControl::TScrollingWinControl(TComponent* owner) : TWinControl(owner)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TCustomForm::TCustomForm(TComponent* Owner) : TScrollingWinControl(Owner)
{
	m_wxFrame = new MyFrameTCustomForm(this);
	m_Handle = (HWND)m_wxFrame->GetHandle();
	m_wxFrame->SetTitle(_T("wxWidgets canvas frame"));

	// ���̃N���X�̂��߂Ƀ|�C���^�Z�b�g
	if ( m_wxWindow )
		m_wxWindow->Destroy();
	m_wxWindow = m_wxFrame;
	m_wxWindow->Show();
	// Give it a status line
	//frame->CreateStatusBar();

	// Make a menubar
	//wxMenu *file_menu = new wxMenu;

	//file_menu->Append(HELLO_NEW, _T("&New MFC Window"));
	//file_menu->Append(HELLO_QUIT, _T("&Close"));

	//wxMenuBar *menu_bar = new wxMenuBar;

	//menu_bar->Append(file_menu, _T("&File"));

	// Associate the menu bar with the frame
	//subframe->SetMenuBar(menu_bar);
}

//----------------------------------------------------------------------------
TCustomForm::~TCustomForm()
{
	// ���܂��ɃN���[�Y����Ă��Ȃ��̂�������A�C�x���g���s���Ă���
	if ( m_wxFrame )
		m_wxFrame->Destroy();

	// VCL�I�u�W�F�N�g��ׂ��Ƃ��́A�f�X�g���C
	// ���̃n���h�����ł́A�܂������Ă��Ȃ��ƃ_��������
	if ( OnDestroy )
		OnDestroy(this);

	if ( m_wxFrame )
		m_wxFrame->m_pForm = NULL;

	m_wxWindow = NULL;
	m_wxFrame = NULL;
}


//----------------------------------------------------------------------------
void TCustomForm::Dispatch(void*p)
{
	TMessage* pm = (TMessage*)p;
	pm->Result = 
		dynamic_cast<MyFrameTCustomForm*>(m_wxFrame)->MSWWindowProcForDispatch(pm->Msg, pm->WParam, pm->LParam);
}

//	PROPERTY_VAR1(bool, FreeOnTerminate);
//----------------------------------------------------------------------------
//	PROPERTY(TWindowState, WindowState);
//enum TWindowState { wsNormal=0, wsMinimized=1, wsMaximized=2 };
// ��ɁA�ő剻����Ă��邩�A�W����Ԃɂ��邩�@�̂ݒm�肽���@�̂��ȁB
void TCustomForm::setWindowState( TWindowState state )
{
	if ( state == wsNormal )
		m_wxWindow->Show();
}
TWindowState TCustomForm::getWindowState( void ) const
{
	if ( m_wxWindow->IsShown() )
		return wsNormal;
	else
		return wsMinimized;
}
//	PROPERTY_VAR0(TBorderStyle, BorderStyle );
//	PROPERTY_VAR0(unsigned int, BorderStyle);
//	PROPERTY_VAR1(TMainMenu*, Menu);
//	PROPERTY_VAR1(TIcon*, Icon);

//----------------------------------------------------------------------------
void TCustomForm::SetFocus()
{
}
//----------------------------------------------------------------------------
HRESULT TCustomForm::SetHWnd(int, HWND)
{
	return S_OK;
}
//----------------------------------------------------------------------------
int TCustomForm::ShowModal()
{
	m_wxWindow->MakeModal();
	return 0;
}
//----------------------------------------------------------------------------
void __fastcall TCustomForm::Close(void)
{
}

//----------------------------------------------------------------------------
bool __fastcall TCustomForm::IsShortCut(Messages::TWMKey &Message)
{
	return false;
}
//----------------------------------------------------------------------------
void TCustomForm::WndProc(TMessage& message)
{
	message.Result = DefWindowProc(Handle, message.Msg, message.WParam, message.LParam);
}
//----------------------------------------------------------------------------
void TCustomForm::SetZOrder(bool)
{
}


//----------------------------------------------------------------------------
// �{�[�_�[�̃Z�b�^�B�擪�������̂��̂́A�}�N���ō��֐�
// �擪�啶���̂��̂́AVCL�̃C���^�[�t�F�[�X
void TCustomForm::setBorderStyle(TBorderStyle v)
{
	SetBorderStyle(v);
}
void TCustomForm::SetBorderStyle(TFormBorderStyle Value)
{
	m_BorderStyle = Value;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TForm::TForm(TComponent* owner) : TCustomForm(owner)
{
	// BCB�p�R�[�h���ɗ͕����Ȃ����߂ɂ́A
	// �����ŁA�T�u�N���X�̃����o������������K�v������B
	// �������p�֐��̔z����O���[�o���Ŏ��Ă΂悢�̂��̂��H

	// typeid���Z�q��this�ɓK�p����ƁATForm��Ԃ��̂ŁA�T�u�N���X�̃^�C�v����ɂ͎g���Ȃ�
	// dynamic_cast���A���F�����ŗގ��̂��Ƃ����Ă���̂Ń_��
}

//----------------------------------------------------------------------------
TForm::~TForm()
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TScrollBox::TScrollBox(TComponent* owner) : TScrollingWinControl(owner)
{
	HorzScrollBar = new TScrollBar(this);
	VertScrollBar = new TScrollBar(this);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TScrollBar::TScrollBar(TComponent* owner) : TWinControl(owner)
{
}

//----------------------------------------------------------------------------
void TScrollBar::setPosition(int val)
{
}

//----------------------------------------------------------------------------
int TScrollBar::getPosition() const
{
	return 0;
}
