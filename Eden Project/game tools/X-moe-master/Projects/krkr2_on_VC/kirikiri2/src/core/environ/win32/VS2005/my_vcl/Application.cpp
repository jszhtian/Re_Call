#include "vcl.h"
#include "Application.h"

#include "tjsCommHead.h"
#include <memory>

HINSTANCE HInstance = NULL;
TApplication* Application;

// main�֐��Ȃ��ɋ����ɉ�����������g���b�N
static __THogeInstance<TApplication, &Application> __TApplicationInstance;

#include <MainFormUnit.h>
#include <wx\wx.h>

//#define USE_DGB_FORM

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// wxWidgets��APP�����
// Define a new application type
class MyApp : public wxApp
{
protected:
	std::auto_ptr<wxFrame> m_autoFrame;
public:
    virtual bool OnInit();

    // we need to override this as the default behaviour only works when we're
    // running wxWidgets main loop, not MFC one
//    virtual void ExitMainLoop();
    wxFrame* CreateFrame();
	void OnIdle(wxIdleEvent& event);

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyApp, wxApp)
	EVT_IDLE(MyApp::OnIdle)
END_EVENT_TABLE()

IMPLEMENT_APP_NO_MAIN(MyApp)


//----------------------------------------------------------------------------
bool MyApp::OnInit()
{
//	SetExitOnFrameDelete(false);

#ifdef USE_DBG_FORM
	m_autoFrame.reset( CreateFrame() );
#endif

    return true;
}

//----------------------------------------------------------------------------
void MyApp::OnIdle(wxIdleEvent& event)
{
	bool Done;  //�i�߂�l�j�ق��̃A�v���P�[�V�����ɐ�����ڂ��Ƃ� true
	Application->OnIdle(Application, Done);
	wxApp::OnIdle(event);
}

//----------------------------------------------------------------------------
#define HELLO_QUIT 1
#define HELLO_NEW  2
wxFrame *MyApp::CreateFrame()
{
	wxFrame *subframe = new wxFrame(NULL, -1, _T("Canvas Frame"), wxPoint(10, 10), wxSize(300, 300),
        wxDEFAULT_FRAME_STYLE);

	subframe->SetTitle(_T("wxWidgets canvas frame"));

	// Give it a status line
	subframe->CreateStatusBar();

	// Make a menubar
	wxMenu *file_menu = new wxMenu;

	file_menu->Append(HELLO_NEW, _T("&New MFC Window"));
	file_menu->Append(HELLO_QUIT, _T("&Close"));

	wxMenuBar *menu_bar = new wxMenuBar;

	menu_bar->Append(file_menu, _T("&File"));

	// Associate the menu bar with the frame
	subframe->SetMenuBar(menu_bar);
    subframe->Show(true);

	// Return the main frame window
	return subframe;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TApplication::TApplication( TComponent* Owner ) : TComponent(Owner)
{
	Handle = NULL;
	//_CrtSetBreakAlloc(1225); // TJS�I�u�W�F�N�g�̐���
}

//----------------------------------------------------------------------------
TApplication::~TApplication()
{
	// �q������Ί�������(�e�N���X�̃f�X�g���N�^�ɔC�����肵�Ȃ�)
	DeleteChildren();

	// �q�����Ȃ��Ȃ�����A�I�X��wxWidgets���N���[�Y
	wxEntryCleanup();
}

//----------------------------------------------------------------------------
// OLE �I�[�g���[�V�����ւ̃|�C���^������������
// OLE �I�[�g���[�V�������g��Ȃ��A�v���P�[�V�����ł͉������Ȃ��B
void TApplication::Initialize(void)
{
	// wxEntryStart���� OleInitialize ���Ă΂��ۂɃG���[���ł�B
	// �R�R���ȑO��COM�����������ꂿ����Ă�炵���B�_�����ۂ�
	// �̂ŁA�����ɁACOM�������[�X
	try {
		OleUninitialize();// �Ƃ肠�����A������Ă݂�
		CoUninitialize(); // �Ƃɂ����������
	} catch(...)
	{
		OutputDebugString("��O�ł����ǖ������܂�...\n");
	}

	// mfctest�T���v��������� IMPLEMENT_APP_NO_MAIN(MyApp) ���g���������ڂ��Ă���
//	WXDLLIMPEXP_BASE void wxSetInstance(HINSTANCE hInst);
//	wxSetInstance(wxApp::m_hInstance);

    wxApp::m_nCmdShow = TRUE;
	
	int argc = 0;
    wxChar **argv = NULL;
	::wxEntryStart(argc, argv);
    if ( !wxTheApp || !wxTheApp->CallOnInit() )
        throw;
}

//----------------------------------------------------------------------------
void TApplication::Terminate( void )
{
	wxTheApp->ExitMainLoop();
}

//----------------------------------------------------------------------------
// ����������O�̃��b�Z�[�W��\������
void TApplication::ShowException(Exception* e)
{
	::MessageBox( NULL, e->Message.c_str(), Title.c_str(), MB_ICONERROR );
}

//----------------------------------------------------------------------------
// ���R�����BCreateForm�́A����̏����ł����g�p����܂���
void TApplication::CreateForm( const type_info * type, void *)
{
	// Application->CreateForm(__classid(TTVPMainForm), &TVPMainForm);
	// �̂݁B
	// ������΁A�ȉ��̂悤�ɂ��čς܂��B���߂�
	if ( *type == typeid(TTVPMainForm) )
		TVPMainForm = new TTVPMainForm(this);

	// �n���h���ϐ������������̂Ƃ��ɂ��̊֐��Ă΂ꂽ�Ȃ�A�R�����āA�A�������
	if ( Handle == NULL )
		Handle = TVPMainForm->Handle;
}

//----------------------------------------------------------------------------
void TApplication::Run(void)
{
//#ifdef USE_DBG_FORM
	::wxWakeUpIdle();
	wxTheApp->OnRun();
//#endif
}

//----------------------------------------------------------------------------
void TApplication::ProcessMessages()
{
}

//----------------------------------------------------------------------------
void TApplication::HandleMessage()
{
}

//----------------------------------------------------------------------------
void TApplication::BringToFront()
{
}

//----------------------------------------------------------------------------
void TApplication::Minimize()
{
}

//----------------------------------------------------------------------------
void TApplication::Restore()
{
}

//----------------------------------------------------------------------------
// ���b�Z�[�W�{�b�N�X��\������
// MessageBox ���\�b�h�́AWindows API �֐��� MessageBox �֐����J�v�Z�����������̂ł���B
int TApplication::MessageBox(const char* Text, char* Caption, int Flags)
{
	return ::MessageBox(NULL, Text, Caption, Flags);
}

//----------------------------------------------------------------------------
int TApplication::HintMouseMessage(TControl*, TMessage)
{
	return 0;
}

//----------------------------------------------------------------------------
void TApplication::CancelHint()
{
}

