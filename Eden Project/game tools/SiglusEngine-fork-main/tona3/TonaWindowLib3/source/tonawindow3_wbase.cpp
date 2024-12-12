#include	"tonawindow3_pch.h"
#include	"tonawindow3_wbase.h"

#pragma comment(lib, "imm32.lib")

namespace NT3
{

// ****************************************************************
// �R���X�g���N�^
// ================================================================
C_window_base::C_window_base()
{
	// �p�����[�^��������
	init_param();
}

// ****************************************************************
// �f�X�g���N�^
// ================================================================
C_window_base::~C_window_base()
{
	if (h_large_icon)	{
		DestroyIcon(h_large_icon);
		h_large_icon = NULL;
	}
	if (h_small_icon)	{
		DestroyIcon(h_small_icon);
		h_small_icon = NULL;
	}
	unbind();
}

// ****************************************************************
// �p�����[�^��������
// ================================================================
void C_window_base::init_param()
{
	// �p�����[�^��������
	m_h_wnd = NULL;
	m_old_window_proc = NULL;
	m_is_dialog = false;
	m_is_limit_window_rect_min = false;
	m_is_attach_window = false;
	m_group_no = 0;
	h_large_icon = NULL;
	h_small_icon = NULL;
}

// ****************************************************************
// �E�B���h�E�o�^
//		class_name : �E�B���h�E�N���X��
//		icon_id : �A�C�R���̃��\�[�X�h�c�i0 �Ŗ����j
//		menu_id : ���j���[�̃��\�[�X�h�c�i0 �Ŗ����j
//		brush_type : �w�i��h��Ԃ��u���V�i-1 �Ŗ����j
// ================================================================
bool C_window_base::regist_class(CTSTR& class_name, int icon_id, int menu_id, int brush_type)
{
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = window_proc_base;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = G_app.h_inst;
	wcex.hIcon = LoadIcon(G_app.h_inst, MAKEINTRESOURCE(icon_id));
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)GetStockObject(brush_type);
	wcex.lpszClassName = class_name.c_str();
	wcex.lpszMenuName = MAKEINTRESOURCE(menu_id);
	wcex.hIconSm = NULL;

	// �E�B���h�E�̓o�^
	if (!RegisterClassEx(&wcex))	{
		set_last_error_win(_T("RegisterClassEx"));
		return false;
	}

	return true;
}

void C_window_base::unregist_class(CTSTR& class_name)
{
	UnregisterClass(class_name.c_str(), G_app.h_inst);
}

// ****************************************************************
// �E�B���h�E�쐬
//		name : �E�B���h�E���iini �ۑ����Ɏg�p�j
//		class_name : �E�B���h�E�N���X��
//		caption : �E�B���h�E�^�C�g��
//		x : �E�B���h�E�̕\���ʒu�w�iCW_USEDEFAULT : �f�t�H���g�A-1 : �����j
//		y : �E�B���h�E�̕\���ʒu�x�iCW_USEDEFAULT : �f�t�H���g�A-1 : �����j
//		client_w : �E�B���h�E�̃N���C�A���g��`�̕��iCW_USEDEFAULT : �f�t�H���g�j
//		client_h : �E�B���h�E�̃N���C�A���g��`�̍����iCW_USEDEFAULT : �f�t�H���g�j
//		is_menu : ���j���[������^�Ȃ�
//		style : �E�B���h�E�X�^�C��
//		style_ex : �g���E�B���h�E�X�^�C��
//		is_limit_min : �E�B���h�E�̍ŏ���`�𐧌�
//		is_attach : ���̃E�B���h�E�ɃA�^�b�`
//		h_parent_wnd : �e�E�B���h�E�n���h��
//		adjust_way : �e�ɂ����������i�ݷ� �̕����Ŏw��j
// ================================================================
bool C_window_base::create(CTSTR& name, CTSTR& class_name, CTSTR& caption, int x, int y, int client_w, int client_h, bool is_menu, DWORD style, DWORD ex_style, bool is_limit_min, bool is_attach, HWND h_parent_wnd)
{
	if (m_h_wnd != NULL)
		return false;

	// �E�B���h�E�̖��O
	m_window_name = name;
	// �E�B���h�E�̍ŏ���`�𐧌�
	m_is_limit_window_rect_min = is_limit_min;
	// ���̃E�B���h�E�ɃA�^�b�`
	m_is_attach_window = is_attach;

	// �E�B���h�E�̑傫���̌v�Z
	C_rect client_rect(0, 0, client_w, client_h);
	AdjustWindowRectEx(&client_rect, style, is_menu ? TRUE : FALSE, ex_style);

	// �Z���^�����O
	if (x == -1)	x = (GetSystemMetrics(SM_CXSCREEN) - client_rect.width()) / 2;
	if (y == -1)	y = (GetSystemMetrics(SM_CYSCREEN) - client_rect.height()) / 2;

	// �E�B���h�E�̍\�z
	m_h_wnd = CreateWindowEx(ex_style,
		class_name.c_str(), caption.c_str(), style,
		x, y, client_rect.width(), client_rect.height(),
		h_parent_wnd, NULL, G_app.h_inst, this);

	// �\�z�Ɏ��s
	if (m_h_wnd == NULL)	{
		set_last_error_win(_T("CreateWindowEx"));
		return false;
	}

	// �ĂуE�B���h�E�n���h���ƃI�u�W�F�N�g�����т���
	// �����̃E�B���h�E�N���X�i"button" �Ȃǁj�𒼍쐬�����Ƃ��Ȃǂ́A
	// ���̎��_�ł͂܂����т��ĂȂ��̂ł�
	bind(m_h_wnd);

	// ���̃E�B���h�E�ɃA�^�b�`
	if (m_is_attach_window)
		m_attach.h_wnd_list.push_back(m_h_wnd);

	// IME �𖳌��ɂ���
//	ImmAssociateContext(h_wnd, NULL);

	return true;
}

// ****************************************************************
// �E�B���h�E�����
// ================================================================
void C_window_base::close()
{
	if (m_h_wnd == NULL)
		return;

	send_message(WM_CLOSE, 0, 0);
}

// ****************************************************************
// �E�B���h�E��j�󂷂�
// ================================================================
void C_window_base::destroy()
{
	if (m_h_wnd == NULL)
		return;

	DestroyWindow(m_h_wnd);
}

// ****************************************************************
// �E�B���h�E�v���V�[�W��
// ================================================================
LRESULT CALLBACK C_window_base::window_proc_base(HWND h_wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// �E�B���h�E�̃v���p�e�B���X�g���� C_window_base �I�u�W�F�N�g�ւ̃|�C���^�̎擾�����݂�
	C_window_base* p_target_wnd = (C_window_base *)GetProp(h_wnd, _T("this"));

	// C_window_base �I�u�W�F�N�g�ƃE�B���h�E�����ѕt�����Ă��Ȃ��ꍇ��
	// C_window_base �I�u�W�F�N�g�ƃE�B���h�E�����ѕt����
	if (!p_target_wnd)	{

		// �E�B���h�E�̏ꍇ�� CREATESTRUCT ���� Cwindow �I�u�W�F�N�g�ւ̃|�C���^���擾
		if (msg == WM_CREATE || msg == WM_NCCREATE)
			p_target_wnd = (C_window_base *)((CREATESTRUCT *)lp)->lpCreateParams;
		// �_�C�A���O�̏ꍇ
		else if (msg == WM_INITDIALOG)	{
			// �v���p�e�B�V�[�g���ǂ����𔻒�
			//		--- ������@��������Ȃ��̂ŁA�擪 DWORD �����Ė�����蔻�f���Ă��܂��B
			DWORD top = *(DWORD *)lp;
			// ���ʂ̃_�C�A���O�̏ꍇ�� lp ����C���X�^���X���擾
			if (top != sizeof(PROPSHEETPAGE))
				p_target_wnd = (C_window_base *)lp;
			// �v���p�e�B�V�[�g�̏ꍇ�� psp �\���̂���C���X�^���X���擾
			else
				p_target_wnd = (C_window_base *)((PROPSHEETPAGE *)lp)->lParam;
		}
		// �E�B���h�E�n���h����C_window_base�I�u�W�F�N�g�����ѕt����
		if (p_target_wnd)
			p_target_wnd->bind(h_wnd);
	}

	// p_target_wnd �̎擾�ɐ��������ꍇ�́AC_window_base �� window_proc ���Ăяo��
	if (p_target_wnd)	{

		p_target_wnd->window_proc_pre(msg, wp, lp);
		LRESULT result = p_target_wnd->window_proc(msg, wp, lp);
		// WM_DESTROY ���b�Z�[�W�ŃE�B���h�E�� C_window_base �I�u�W�F�N�g��؂藣��
		if (msg == WM_DESTROY)
			p_target_wnd->unbind();

		return result;
	}

	// �_�C�A���O�̏ꍇ�AFALSE ��Ԃ�
	if (GetWindowLong(h_wnd, DWL_DLGPROC))
		return FALSE;

	// �f�t�H���g�̃E�B���h�E�v���V�[�W�����Ăяo��
	return DefWindowProc(h_wnd, msg, wp, lp);
}

// ****************************************************************
// �p�������E�B���h�E�v���V�[�W���i�v���j
// ================================================================
void C_window_base::window_proc_pre(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_CREATE:		on_create_pre();		break;
	}
}

// ****************************************************************
// �E�B���h�E�����ꂽ�i�v���j
// ================================================================
void C_window_base::on_create_pre()
{
	if (m_h_wnd == NULL)	{	return;	}
	// �E�B���h�E��`���擾
	m_def_window_rect = get_window_rect();
	// �N���C�A���g��`���擾
	m_def_client_rect = get_client_rect();
	// �E�B���h�E��o�^����
	G_app.window_list.push_back(m_h_wnd);
}

// ****************************************************************
// �p�������E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_window_base::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	if (!m_h_wnd)
		return FALSE;

	switch (msg)	{
		case WM_CLOSE:		if (!on_close())			return FALSE;	break;
		case WM_DESTROY:	if (!on_destroy())			return FALSE;	break;
		case WM_MOVING:		if (!on_moving(wp, lp))		return FALSE;	break;
		case WM_SIZING:		if (!on_sizing(wp, lp))		return FALSE;	break;

		case WM_SYSCOMMAND:
			// �E�B���h�E�̈ړ��^�T�C�Y�ύX���J�n���ꂽ
			if ((wp & SC_MOVE) || (wp & SC_SIZE))	{
				attach_init();	// �A�^�b�`�̏���������
			}
			break;
	}

	// �Â��v���V�[�W��������ꍇ���̃v���V�[�W�����Ăяo��
	if (m_old_window_proc)
		return CallWindowProc(m_old_window_proc, m_h_wnd, msg, wp, lp);

	// �_�C�A���O�̏ꍇ�AFALSE ��Ԃ�
	if (GetWindowLong(m_h_wnd, DWL_DLGPROC))
		return FALSE;

	// �f�t�H���g�̃E�B���h�E�v���V�[�W�����Ăяo��
	return DefWindowProc(m_h_wnd, msg, wp, lp);
}

// ****************************************************************
// �E�B���h�E�n���h���ƃI�u�W�F�N�g�����т���
// ================================================================
bool C_window_base::bind(HWND h_wnd)
{
	if (!h_wnd)
		return true;

	// �n���h���̌���
	m_h_wnd = h_wnd;

	// �E�B���h�E�n���h���� C_window_base �I�u�W�F�N�g�����т���
	SetProp(m_h_wnd, _T("this"), (HANDLE)this);

	// �e����̃E�B���h�E�ʒu���v�Z
	C_window_base* p_parent_wnd = (C_window_base *)GetProp(get_parent_handle(), _T("this"));
	if (p_parent_wnd)
		m_def_rltv_rect = p_parent_wnd->screen_to_client(get_window_rect());

	// �_�C�A���O���E�B���h�E���𔻒肷��
	m_is_dialog = GetWindowLongPtr(m_h_wnd, DWL_DLGPROC) != FALSE;

	// �����̃E�B���h�E���T�u�N���X������ꍇ�́A
	// �E�B���h�E�i�_�C�A���O�j�v���V�[�W���� window_proc_base �ɒu��������
	if (GetWindowLongPtr(m_h_wnd, m_is_dialog ? DWL_DLGPROC : GWL_WNDPROC) != (PTR)window_proc_base)
		m_old_window_proc = (WNDPROC)(PTR)SetWindowLongPtr(m_h_wnd, m_is_dialog ? DWL_DLGPROC : GWL_WNDPROC, (LONG)(PTR)window_proc_base);

	return true;
}

// ****************************************************************
// �E�B���h�E�n���h�����I�u�W�F�N�g����؂藣��
// ================================================================
void C_window_base::unbind()
{
	if (!m_h_wnd)
		return;

	// �E�B���h�E���T�u�N���X������Ă���ꍇ�́A
	// �E�B���h�E�i�_�C�A���O�j�v���V�[�W�������ɖ߂�
	if (m_old_window_proc)	{
		SetWindowLongPtr(m_h_wnd, (m_is_dialog ? DWL_DLGPROC : GWL_WNDPROC), (LONG)(PTR)m_old_window_proc);
		m_old_window_proc = NULL;
	}

	// �E�B���h�E�n���h���� Cwindow �I�u�W�F�N�g����؂藣��
	RemoveProp(m_h_wnd, _T("this"));

	// ���C���E�B���h�E�̏ꍇ�� app �̃n���h����������
	if (m_h_wnd == G_app.h_wnd)
		G_app.h_wnd = NULL;
	
	// �n���h����������
	m_h_wnd = NULL;
}

// ****************************************************************
// �E�B���h�E������ꂽ
// ================================================================
bool C_window_base::on_close()
{
	if (m_h_wnd == NULL)
		return true;

	destroy();
	return true;
}

// ****************************************************************
// �E�B���h�E���j�󂳂ꂽ
// ================================================================
bool C_window_base::on_destroy()
{
	// ���̃E�B���h�E�ɃA�^�b�`������
	if (m_is_attach_window)
		m_attach.h_wnd_list.erase(std::remove(m_attach.h_wnd_list.begin(), m_attach.h_wnd_list.end(), m_h_wnd), m_attach.h_wnd_list.end());
	// �E�B���h�E���X�g����폜
	G_app.window_list.erase(std::remove(G_app.window_list.begin(), G_app.window_list.end(), m_h_wnd), G_app.window_list.end());
	// ���C���E�B���h�E�̏ꍇ�A�v���P�[�V�������I������
	if (m_h_wnd == G_app.h_wnd)
		PostQuitMessage(0);

	return true;
}

// ****************************************************************
// �E�B���h�E����������Ă���
// ================================================================
bool C_window_base::on_moving(WPARAM wp, LPARAM lp)
{
	// ���̃E�B���h�E�ɃA�^�b�`
	if (m_is_attach_window)
		attach_on_moving(wp, lp);

	return true;
}

// ****************************************************************
// �E�B���h�E�T�C�Y���ύX����Ă���
// ================================================================
bool C_window_base::on_sizing(WPARAM wp, LPARAM lp)
{
	// ���̃E�B���h�E�ɃA�^�b�`
	if (m_is_attach_window)
		attach_on_sizing(wp, lp);

	// �E�B���h�E�T�C�Y�𐧌�
	if (m_is_limit_window_rect_min)
		limit_window_size(wp, lp, m_def_window_rect);

	return true;
}

// ****************************************************************
// �E�B���h�E�̍ŏ���`�̐�������
// ================================================================
void C_window_base::limit_window_size(WPARAM wp, LPARAM lp, C_rect min_rect)
{
	C_rect* rp = (C_rect *)lp;
	int width = std::max(min_rect.width(), rp->width());
	int height = std::max(min_rect.height(), rp->height());

	switch (wp)	{
		case WMSZ_BOTTOM:
			rp->bottom = rp->top + height;
			break;
		case WMSZ_BOTTOMLEFT:
			rp->left = rp->right - width;
			rp->bottom = rp->top + height;
			break;
		case WMSZ_BOTTOMRIGHT:
			rp->right = rp->left + width;
			rp->bottom = rp->top + height;
			break;
		case WMSZ_LEFT:
			rp->left = rp->right - width;
			break;
		case WMSZ_RIGHT:
			rp->right = rp->left + width;
			break;
		case WMSZ_TOP:
			rp->top = rp->bottom - height;
			break;
		case WMSZ_TOPLEFT:
			rp->top = rp->bottom - height;
			rp->left = rp->right - width;
			break;
		case WMSZ_TOPRIGHT:
			rp->top = rp->bottom - height;
			rp->right = rp->left + width;
			break;
	}
}

// ****************************************************************
// �e�E�B���h�E�̃T�C�Y�ύX�ɍ��킹�ē���
// ================================================================
void C_window_base::adjust_to_parent(int dw, int dh)
{
	// ������ 0 �̂Ƃ��͈ړ����Ȃ�
	if (m_adjust_way > 0)	{

		// ���������̓e���L�[�̔z�u���Q�l�Ɂife. 7=����j
		int adjust_h = (m_adjust_way - 1) % 3;	// ���̓���
		int adjust_v = (9 - m_adjust_way) / 3;	// �c�̓���

		// �V�����E�B���h�E���N�g���v�Z
		C_rect new_rect = m_def_rltv_rect;
		if (adjust_h == 2)						new_rect.left += dw;
		if (adjust_v == 2)						new_rect.top += dh;
		if (adjust_h == 1 || adjust_h == 2)		new_rect.right += dw;
		if (adjust_v == 1 || adjust_v == 2)		new_rect.bottom += dh;

		// �ړ����čĕ`��
		set_window_rect(new_rect);
		invalidate_rect(NULL, false);
	}
}

// ****************************************************************
// �e�E�B���h�E�̃T�C�Y�ύX�ɍ��킹�ē���
// ================================================================
void C_window_base::adjust_to_parent_4(int dl, int dt, int dw, int dh)
{
	C_rect new_rect = m_def_rltv_rect;
	new_rect.left += dl;
	new_rect.top += dt;
	new_rect.right += dl + dw;
	new_rect.bottom += dt + dh;

	// �ړ����čĕ`��
	set_window_rect(new_rect);
	invalidate_rect(NULL, false);
}


// ****************************************************************
// �E�B���h�E�A�^�b�`
// ================================================================
WINDOW_ATTACH	C_window_base::m_attach;

// ****************************************************************
// �E�B���h�E�A�^�b�`�F������
// ================================================================
void C_window_base::attach_init()
{
	m_attach.mod = 0;
}

// ****************************************************************
// �E�B���h�E�A�^�b�`�i�E�B���h�E�ړ����j
// ================================================================
void C_window_base::attach_on_moving(WPARAM wp, LPARAM lp)
{
	C_rect my_rect, target_rect;
	C_point point;
	WINDOW_ATTACH_PROC wap;

	my_rect = *(C_rect *)lp;
	GetCursorPos(&point);

	// �������̋L��
	if (m_attach.h_wnd != m_h_wnd || m_attach.mod != 1){
		m_attach.h_wnd = m_h_wnd;
		m_attach.mod = 1;	// 1=�ړ���
		m_attach.left   = true;
		m_attach.right  = true;
		m_attach.top    = true;
		m_attach.bottom = true;
		m_attach.x = point.x - my_rect.left;
		m_attach.y = point.y - my_rect.top;
		m_attach.w = my_rect.width();
		m_attach.h = my_rect.height();
	}

	my_rect.left   = point.x - m_attach.x;
	my_rect.top    = point.y - m_attach.y;
	my_rect.right  = my_rect.left + m_attach.w;
	my_rect.bottom = my_rect.top  + m_attach.h;

	// �����ȊO�̃E�B���h�E�̉�
	wap.width_h_wnd  = NULL;
	wap.height_h_wnd = NULL;
	for (ARRAY<HWND>::iterator itr = m_attach.h_wnd_list.begin(); itr != m_attach.h_wnd_list.end(); ++itr)	{
		if (*itr != m_h_wnd)	{
			// �\������Ă���E�B���h�E�ɑ΂��Ă̂݃A�^�b�`
			if (GetWindowLongPtr(*itr, GWL_STYLE) & WS_VISIBLE)	{
				GetWindowRect(*itr, &target_rect);
				attach_check(*itr, &my_rect, &target_rect, &wap);
			}
		}
	}

	// �f�X�N�g�b�v�S�̂̉�
	wap.desktop_width_flag  = false;
	wap.desktop_height_flag = false;
	get_my_desktop_work_area_rect(&target_rect);
	attach_check(NULL, &my_rect, &target_rect, &wap);

	// �␳
	attach_rep(&my_rect, &wap);

	*(C_rect *)lp = my_rect;
}

// ****************************************************************
// �E�B���h�E�A�^�b�`�i�E�B���h�E�T�C�Y�ύX���j
// ================================================================
void C_window_base::attach_on_sizing(WPARAM wp, LPARAM lp)
{
	C_rect my_rect, target_rect;
	C_point point;
	WINDOW_ATTACH_PROC wap;

	my_rect = *(C_rect *)lp;
	GetCursorPos(&point);

	// �������̋L��
	if (m_attach.h_wnd != m_h_wnd || m_attach.mod != 2)	{
		m_attach.h_wnd = m_h_wnd;
		m_attach.mod = 2;	// 2=�T�C�Y�ύX��
		m_attach.left   = false;
		m_attach.right  = false;
		m_attach.top    = false;
		m_attach.bottom = false;
		if (wp == WMSZ_LEFT   || wp == WMSZ_TOPLEFT    || wp == WMSZ_BOTTOMLEFT)	{ m_attach.x = point.x - my_rect.left;   m_attach.left   = true; }
		if (wp == WMSZ_RIGHT  || wp == WMSZ_TOPRIGHT   || wp == WMSZ_BOTTOMRIGHT)	{ m_attach.x = point.x - my_rect.right;  m_attach.right  = true; }
		if (wp == WMSZ_TOP    || wp == WMSZ_TOPLEFT    || wp == WMSZ_TOPRIGHT)		{ m_attach.y = point.y - my_rect.top;    m_attach.top    = true; }
		if (wp == WMSZ_BOTTOM || wp == WMSZ_BOTTOMLEFT || wp == WMSZ_BOTTOMRIGHT)	{ m_attach.y = point.y - my_rect.bottom; m_attach.bottom = true; }
	}

	if (m_attach.left)		{ my_rect.left   = point.x - m_attach.x; }
	if (m_attach.right)		{ my_rect.right  = point.x - m_attach.x; }
	if (m_attach.top)		{ my_rect.top    = point.y - m_attach.y; }
	if (m_attach.bottom)	{ my_rect.bottom = point.y - m_attach.y; }

	// �����ȊO�̃E�B���h�E�̉�
	wap.width_h_wnd  = NULL;
	wap.height_h_wnd = NULL;
	for (ARRAY<HWND>::iterator itr = m_attach.h_wnd_list.begin(); itr != m_attach.h_wnd_list.end(); ++itr)	{
		if (*itr != m_h_wnd)	{
			// �\������Ă���E�B���h�E�ɑ΂��Ă̂݃A�^�b�`
			if (GetWindowLongPtr(*itr, GWL_STYLE) & WS_VISIBLE)	{
				GetWindowRect(*itr, &target_rect);
				attach_check(*itr, &my_rect, &target_rect, &wap);
			}
		}
	}

	// �f�X�N�g�b�v�S�̂̉�
	wap.desktop_width_flag  = false;
	wap.desktop_height_flag = false;
	get_my_desktop_work_area_rect(&target_rect);
	attach_check(NULL, &my_rect, &target_rect, &wap);

	// �␳
	attach_rep(&my_rect, &wap);

	*(C_rect *)lp = my_rect;
}

// ****************************************************************
// �E�B���h�E�A�^�b�`�F�`�F�b�N
// ================================================================
void C_window_base::attach_check(HWND h_target_wnd, C_rect *rect1, C_rect *rect2, WINDOW_ATTACH_PROC *wap)
{
	int tmp;
	if (
		rect1->left      < (rect2->right  + WINDOW_ATTACH_REP_SIZE)
		&& rect1->right  > (rect2->left   - WINDOW_ATTACH_REP_SIZE)
		&& rect1->top    < (rect2->bottom + WINDOW_ATTACH_REP_SIZE)
		&& rect1->bottom > (rect2->top    - WINDOW_ATTACH_REP_SIZE))
	{
		if (m_attach.left)	{
			tmp = rect2->left - rect1->left;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_width_flag = true;
				else						wap->width_h_wnd = h_target_wnd;
				wap->width_mod = 0;
			}
			tmp = rect2->right - rect1->left;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_width_flag = true;
				else						wap->width_h_wnd = h_target_wnd;
				wap->width_mod = 1;
			}
		}
		if (m_attach.right)	{
			tmp = rect2->left - rect1->right;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_width_flag = true;
				else						wap->width_h_wnd = h_target_wnd;
				wap->width_mod  = 2;
			}
			tmp = rect2->right - rect1->right;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_width_flag = true;
				else						wap->width_h_wnd = h_target_wnd;
				wap->width_mod  = 3;
			}
		}
		if (m_attach.top)	{
			tmp = rect2->top - rect1->top;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_height_flag = true;
				else						wap->height_h_wnd = h_target_wnd;
				wap->height_mod = 0;
			}
			tmp = rect2->bottom - rect1->top;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_height_flag = true;
				else						wap->height_h_wnd = h_target_wnd;
				wap->height_mod = 1;
			}
		}
		if (m_attach.bottom)	{
			tmp = rect2->top - rect1->bottom;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_height_flag = true;
				else						wap->height_h_wnd = h_target_wnd;
				wap->height_mod = 2;
			}
			tmp = rect2->bottom - rect1->bottom;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_height_flag = true;
				else						wap->height_h_wnd = h_target_wnd;
				wap->height_mod = 3;
			}
		}
	}
}

// ****************************************************************
// �E�B���h�E�A�^�b�`�F�␳
// ================================================================
void C_window_base::attach_rep(C_rect *rect1, WINDOW_ATTACH_PROC *wap)
{
	int tmp;
	C_rect rect2;

	if (wap->desktop_width_flag || wap->width_h_wnd != NULL)	{
		if (wap->desktop_width_flag)	{
			get_my_desktop_work_area_rect(&rect2);
		}
		else if (wap->width_h_wnd != NULL)	{
			GetWindowRect(wap->width_h_wnd, &rect2);
		}
		switch (wap->width_mod)	{
			case 0: tmp = rect2.left  - rect1->left;  break;
			case 1: tmp = rect2.right - rect1->left;  break;
			case 2: tmp = rect2.left  - rect1->right; break;
			case 3: tmp = rect2.right - rect1->right; break;
		}
		if (m_attach.left)	{ rect1->left  += tmp; }
		if (m_attach.right)	{ rect1->right += tmp; }
	}

	if (wap->desktop_height_flag || wap->height_h_wnd != NULL)	{
		if (wap->desktop_height_flag)	{
			get_my_desktop_work_area_rect(&rect2);
		}
		else if (wap->height_h_wnd != NULL)	{
			GetWindowRect(wap->height_h_wnd, &rect2);
		}
		switch (wap->height_mod)	{
			case 0: tmp = rect2.top    - rect1->top;    break;
			case 1: tmp = rect2.bottom - rect1->top;    break;
			case 2: tmp = rect2.top    - rect1->bottom; break;
			case 3: tmp = rect2.bottom - rect1->bottom; break;
		}
		if (m_attach.top)		{ rect1->top    += tmp; }
		if (m_attach.bottom)	{ rect1->bottom += tmp; }
	}
}









// ****************************************************************
// �n���h�����擾
// ================================================================
HWND C_window_base::get_handle()
{
	return m_h_wnd;
}

// ****************************************************************
// id ���擾
// ================================================================
WORD C_window_base::get_id()
{
	return (WORD)(PTR)GetWindowLongPtr(m_h_wnd, GWLP_ID);
}

// ****************************************************************
// �N���X���擾
// ================================================================
TSTR C_window_base::get_class()
{
	TCHAR buf[1024];
	GetClassName(m_h_wnd, buf, 1024);

	return buf;
}

// ****************************************************************
// ���O���擾
// ================================================================
TSTR C_window_base::get_name()
{
	return m_window_name;
}

// ****************************************************************
// ���O��ݒ�
// ================================================================
void C_window_base::set_name(CTSTR& name)
{
	m_window_name = name;
}

// ****************************************************************
// �e�ւ̂���������ݒ�
// ================================================================
void C_window_base::set_adjust_way(int adjust_way)
{
	m_adjust_way = adjust_way;
}

// ****************************************************************
// �O���[�v�ԍ����擾
// ================================================================
int C_window_base::get_group_no()
{
	return m_group_no;
}

// ****************************************************************
// �O���[�v�ԍ���ݒ�
// ================================================================
void C_window_base::set_group_no(int group_no)
{
	m_group_no = group_no;
}

// ****************************************************************
// �e�E�B���h�E�擾
// ================================================================
C_window_base* C_window_base::get_parent()
{
	return (C_window_base *)::GetProp(get_parent_handle(), _T("this"));
}

// ****************************************************************
// �e�E�B���h�E�̃n���h�����擾
// ================================================================
HWND C_window_base::get_parent_handle()
{
	return (HWND)(PTR)GetWindowLongPtr(m_h_wnd, GWLP_HWNDPARENT);
}

// ****************************************************************
// �e�E�B���h�E�̖��O���擾
// ================================================================
TSTR C_window_base::get_parent_name()
{
	C_window_base* parent = get_parent();
	if (parent)
		return parent->get_name();

	return _T("");
}

// ****************************************************************
// ���C���E�B���h�E�ɐݒ�
// ================================================================
void C_window_base::set_main_window()
{
	G_app.h_wnd = m_h_wnd;
}

// ****************************************************************
// �ʒu�Ƒ傫����ݒ�
// ================================================================
void C_window_base::set_window_pos(C_point pos)
{
	if (m_h_wnd == NULL)
		return;

	::SetWindowPos(m_h_wnd, 0, pos.x, pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);			// �ʒu
}

void C_window_base::set_client_pos(C_point pos)
{
	if (m_h_wnd == NULL)
		return;

	// �Z���^�����O
	if (pos.x == -1)	pos.x = (GetSystemMetrics(SM_CXSCREEN) - get_client_rect().width()) / 2;
	if (pos.y == -1)	pos.y = (GetSystemMetrics(SM_CYSCREEN) - get_client_rect().height()) / 2;

	// �N���C�A���g���W �� �E�B���h�E���W
	C_rect rect = client_to_window(C_rect(pos.x, pos.y, 0, 0));
	::SetWindowPos(m_h_wnd, 0, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);	// �ʒu
}

void C_window_base::set_window_size(C_size size)
{
	if (m_h_wnd == NULL)
		return;

	::SetWindowPos(m_h_wnd, 0, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);		// �傫��
}

void C_window_base::set_client_size(C_size size)
{
	if (m_h_wnd == NULL)
		return;

	// �N���C�A���g���W �� �E�B���h�E���W
	C_rect rect = client_to_window(C_rect(0, 0, size.cx, size.cy));
	::SetWindowPos(m_h_wnd, 0, 0, 0, rect.width(), rect.height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);		// �傫��
}

void C_window_base::set_window_rect(C_rect rect)
{
	if (m_h_wnd == NULL)
		return;

	::SetWindowPos(m_h_wnd, 0, rect.left, rect.top, rect.width(), rect.height(), SWP_NOZORDER | SWP_NOACTIVATE);	// �ʒu�Ƒ傫��
}

void C_window_base::set_client_rect(C_rect rect)
{
	if (m_h_wnd == NULL)
		return;

	// �N���C�A���g���W �� �E�B���h�E���W
	rect = client_to_window(rect);
	::SetWindowPos(m_h_wnd, 0, rect.left, rect.top, rect.width(), rect.height(), SWP_NOZORDER | SWP_NOACTIVATE);	// �ʒu�Ƒ傫��
}

// ****************************************************************
// �ʒu�Ƒ傫�����擾
// ================================================================
C_rect C_window_base::get_window_rect()
{
	C_rect rect;
	GetWindowRect(m_h_wnd, &rect);
	return rect;
}

C_rect C_window_base::get_client_rect()
{
	C_rect rect;
	GetClientRect(m_h_wnd, &rect);
	return rect;
}

C_rect C_window_base::get_rltv_rect()
{
	C_rect rect = get_window_rect();
	HWND parent_h_wnd = get_parent_handle();
	C_point lt = rect.left_top();		ScreenToClient(parent_h_wnd, &lt);
	C_point rb = rect.right_bottom();	ScreenToClient(parent_h_wnd, &rb);
	return C_rect(lt.x, lt.y, rb.x, rb.y);
}

// ****************************************************************
// �f�t�H���g�̈ʒu�Ƒ傫�����擾�^�ݒ�
// ================================================================
C_rect C_window_base::get_def_window_rect()
{
	return m_def_window_rect;
}

void C_window_base::set_def_window_rect(C_rect rect)
{
	m_def_window_rect = rect;
}

C_rect C_window_base::get_def_client_rect()
{
	return m_def_client_rect;
}

void C_window_base::set_def_client_width(int width)
{
	C_rect new_rect = m_def_client_rect;

	new_rect.right = new_rect.left + width;

	m_def_client_rect = new_rect;
}

void C_window_base::set_def_client_height(int height)
{
	C_rect new_rect = m_def_client_rect;

	new_rect.bottom = new_rect.top + height;

	m_def_client_rect = new_rect;
}

void C_window_base::set_def_client_rect(C_rect rect)
{
	m_def_client_rect = rect;
}

C_rect C_window_base::get_def_rltv_rect()
{
	return m_def_rltv_rect;
}

void C_window_base::set_def_rltv_rect(C_rect rect)
{
	m_def_rltv_rect = rect;
}

// ****************************************************************
// ���݂̋�`�ɍ��킹�ăf�t�H���g�̋�`���Đݒ肷��
// ================================================================

void C_window_base::set_def_rect_from_now_rect()
{
	set_def_window_rect_from_now_rect();
	set_def_client_rect_from_now_rect();
	set_def_rltv_rect_from_now_rect();
}

// ���݂̋�`�ɍ��킹�ăf�t�H���g�̃E�B���h�E��`���Đݒ肷��
void C_window_base::set_def_window_rect_from_now_rect()
{
	set_def_window_rect(get_window_rect());
}

// ���݂̋�`�ɍ��킹�ăf�t�H���g�̃N���C�A���g��`���Đݒ肷��
void C_window_base::set_def_client_rect_from_now_rect()
{
	set_def_client_rect(get_client_rect());
}

// ���݂̋�`�ɍ��킹�ăf�t�H���g�̑��΋�`���Đݒ肷��
void C_window_base::set_def_rltv_rect_from_now_rect()
{
	C_rect rect = get_window_rect();
	HWND parent_h_wnd = get_parent_handle();
	C_point lt = rect.left_top();		ScreenToClient(parent_h_wnd, &lt);
	C_point rb = rect.right_bottom();	ScreenToClient(parent_h_wnd, &rb);
	set_def_rltv_rect(C_rect(lt.x, lt.y, rb.x, rb.y));
}

// ****************************************************************
// ���݂̃T�C�Y�ɍ��킹�ăf�t�H���g�̋�`���Đݒ肷��
// ================================================================

void C_window_base::set_def_rect_from_now_size()
{
	set_def_window_rect_from_now_size();
	set_def_client_rect_from_now_size();
	set_def_rltv_rect_from_now_size();
}

// ���݂̃T�C�Y�ɍ��킹�ăf�t�H���g�̃E�B���h�E��`���Đݒ肷��
void C_window_base::set_def_window_rect_from_now_size()
{
	C_rect now_w_rect = get_window_rect();
	C_rect def_w_rect = get_def_window_rect();
	set_def_window_rect(C_rect(def_w_rect.left, def_w_rect.top, def_w_rect.left + now_w_rect.width(), def_w_rect.top + now_w_rect.height()));
}

// ���݂̃T�C�Y�ɍ��킹�ăf�t�H���g�̃N���C�A���g��`���Đݒ肷��
void C_window_base::set_def_client_rect_from_now_size()
{
	C_rect now_w_rect = get_window_rect();
	C_rect def_c_rect = get_def_client_rect();
	set_def_window_rect(C_rect(def_c_rect.left, def_c_rect.top, def_c_rect.left + now_w_rect.width(), def_c_rect.top + now_w_rect.height()));
}

// ���݂̃T�C�Y�ɍ��킹�ăf�t�H���g�̑��΋�`���Đݒ肷��
void C_window_base::set_def_rltv_rect_from_now_size()
{
	C_rect now_w_rect = get_window_rect();
	C_rect def_r_rect = get_def_rltv_rect();
	set_def_rltv_rect(C_rect(def_r_rect.left, def_r_rect.top, def_r_rect.left + now_w_rect.width(), def_r_rect.top + now_w_rect.height()));
}

// ****************************************************************
// �E�B���h�E�T�C�Y��ݒ肵�ăf�t�H���g�̋�`���Đݒ肷��
// ================================================================

void C_window_base::set_window_size_with_def_size(int width, int height)
{
	set_window_size(C_size(width, height));
	set_def_rect_from_now_rect();
}

void C_window_base::set_window_size_with_def_size(C_size size)
{
	set_window_size_with_def_size(size.cx, size.cy);
}

// ****************************************************************
// �E�B���h�E�T�C�Y��ݒ肵�ăf�t�H���g�̋�`���Đݒ肷��
// ================================================================

void C_window_base::add_window_size_with_def_size(int rep_width, int rep_height)
{
	C_rect rect = get_window_rect();
	set_window_size_with_def_size(rect.width() + rep_width, rect.height() + rep_height);
}

void C_window_base::add_window_size_with_def_size(C_size rep_size)
{
	add_window_size_with_def_size(rep_size.cx, rep_size.cy);
}

// ****************************************************************
// ���΍��W��ݒ肵�ăf�t�H���g�̋�`���Đݒ肷��
// ================================================================

void C_window_base::set_rltv_pos_with_def_rect(int x, int y)
{
	set_window_pos(C_point(x, y));
	set_def_rect_from_now_rect();
}

void C_window_base::set_rltv_pos_with_def_rect(C_point pos)
{
	set_rltv_pos_with_def_rect(pos.x, pos.y);
}

// ****************************************************************
// ���΍��W�𒲐����ăf�t�H���g�̋�`���Đݒ肷��
// ================================================================

void C_window_base::add_rltv_pos_with_def_rect(int rep_x, int rep_y)
{
	C_rect rect = get_rltv_rect();
	set_rltv_pos_with_def_rect(rect.left + rep_x, rect.top + rep_y);
}

void C_window_base::add_rltv_pos_with_def_rect(C_point rep_pos)
{
	add_rltv_pos_with_def_rect(rep_pos.x, rep_pos.y);
}

// ****************************************************************
// �E�B���h�E�̑��݂��郂�j�^�̃f�X�N�g�b�v��`�擾
// ================================================================
void C_window_base::get_my_desktop_rect(C_rect *desktop_rect)
{
	C_rect window_rect = get_window_rect();

	get_desktop_rect_from_pos(desktop_rect, C_point(window_rect.left + window_rect.width() / 2, window_rect.top + window_rect.height() / 2));
}

// ****************************************************************
// �E�B���h�E�̑��݂��郂�j�^�̃��[�N�G���A��`�擾�i�f�X�N�g�b�v����^�X�N�o�[�Ȃǂ̗̈�����O�����̈���擾�j
// ================================================================
void C_window_base::get_my_desktop_work_area_rect(C_rect *work_area_rect)
{
	C_rect window_rect = get_window_rect();

	get_work_area_rect_from_pos(work_area_rect, C_point(window_rect.left + window_rect.width() / 2, window_rect.top + window_rect.height() / 2));
}

// ****************************************************************
// �E�B���h�E�̑��݂��郂�j�^�̒����Ɉړ�
// ================================================================
void C_window_base::set_pos_my_desktop_center()
{
	C_rect my_desktop_rect;
	get_my_desktop_rect(&my_desktop_rect);

	int x = (my_desktop_rect.width() - get_window_rect().width()) / 2 + my_desktop_rect.left;
	int y = (my_desktop_rect.height() - get_window_rect().height()) / 2 + my_desktop_rect.top;
	set_window_pos(C_point(x, y));
}

// ****************************************************************
// �f�X�N�g�b�v�͈͊O�ɂ���ꍇ�A�E�B���h�E���W���f�X�N�g�b�v�̒��S�ɕ␳����
// ================================================================
bool C_window_base::rep_window_pos_outside_desktop(int rep_mod, int outside_size)
{
	if (m_h_wnd == NULL)
		return false;

	bool ret = false;

	// ���[�N�G���A�擾
	C_rect work_area_rect;
	get_my_desktop_work_area_rect(&work_area_rect);

	// ���z�f�X�N�g�b�v��`�擾
	C_rect virtual_desktop_rect;
	virtual_desktop_rect.left = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
	virtual_desktop_rect.top = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
	virtual_desktop_rect.right = virtual_desktop_rect.left + ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	virtual_desktop_rect.bottom = virtual_desktop_rect.top + ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

	// �E�B���h�E��`���擾
	C_rect window_rect = get_window_rect();

	// �̈�␳
	C_rect wor;
	wor.left = work_area_rect.left + outside_size;
	wor.right = work_area_rect.right - outside_size;
	wor.top = work_area_rect.top + outside_size;
	wor.bottom = work_area_rect.bottom - outside_size;

	if (window_rect.left > wor.right || window_rect.right < wor.left || window_rect.top > wor.bottom || window_rect.bottom < wor.top)	{
		C_point new_window_pos = C_point(window_rect.left, window_rect.top);
		switch (rep_mod)	{
			case 1:				// ���傤�ǔ͈͓��ɔ[�܂�悤�ɂ���
				if (window_rect.left > wor.right)	{	new_window_pos.x = work_area_rect.right - window_rect.width();		}
				if (window_rect.right < wor.left)	{	new_window_pos.x = 0;	}
				if (window_rect.top > wor.bottom)	{	new_window_pos.y = work_area_rect.bottom - window_rect.height();	}
				if (window_rect.bottom < wor.top)	{	new_window_pos.y = 0;	}
				break;
			case 2:				// �����ɂ���
				{
					C_size desktop_size = C_size(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
					new_window_pos = C_point((desktop_size.cx - window_rect.width()) / 2, (desktop_size.cy - window_rect.height()) / 2);
				}
				break;
			case 0:	default:	// �␳�l���̂ݔ[�܂�悤�ɂ���
				if (window_rect.left > wor.right)	{	new_window_pos.x = work_area_rect.right - outside_size;		}
				if (window_rect.right < wor.left)	{	new_window_pos.x = -(window_rect.width() - outside_size);	}
				if (window_rect.top > wor.bottom)	{	new_window_pos.y = work_area_rect.bottom - outside_size;	}
				if (window_rect.bottom < wor.top)	{	new_window_pos.y = -(window_rect.height() - outside_size);	}	// ������Ԉ���Ă邩���Bwork_area_rect.top �����Ȃ��Ⴞ�߂���ˁH
				break;
		}
		set_window_pos(new_window_pos);
		ret = true;
	}

	// �ŏI�I�Ƀ^�X�N�o�[���f�X�N�g�b�v�͈̔͊O�ɂ���悤�Ȃ炳��ɕ␳����i�㉺�̂ݕ␳�j
	DWORD style = (DWORD)get_style();
	if (style & WS_CAPTION)	{		// �L���v�V�����o�[�������Ă���
//	{
		C_rect window_rect = get_window_rect();
		if (window_rect.top < virtual_desktop_rect.top)	{
			window_rect.top = virtual_desktop_rect.top;
			set_window_pos(C_point(window_rect.left, window_rect.top));
			ret = true;
		}
		else	{
			DWORD ex_style = (DWORD)get_style_ex();
			int caption_size = (ex_style & WS_EX_TOOLWINDOW) ? GetSystemMetrics(SM_CYSMCAPTION) : GetSystemMetrics(SM_CYCAPTION);		// �c�[���E�B���h�E�̏ꍇ�A�X���[���L���v�V����
			if (window_rect.top > work_area_rect.bottom - caption_size)	{
				window_rect.top = work_area_rect.bottom - caption_size;
				set_window_pos(C_point(window_rect.left, window_rect.top));
				ret = true;
			}
		}
	}

	return ret;
}

// ****************************************************************
// �őO�ʂɌŒ�
// ================================================================
void C_window_base::set_top_most(bool top_most)
{
	if (m_h_wnd == NULL)
		return;

	SetWindowPos(m_h_wnd, (top_most ? HWND_TOPMOST : HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

// ****************************************************************
// �t�H�[�J�X��ݒ�
// ================================================================
void C_window_base::set_focus()
{
	SetFocus(m_h_wnd);
}

// ****************************************************************
// �A�N�e�B�u�ɂ���
// ================================================================
void C_window_base::set_active()
{
	SetActiveWindow(m_h_wnd);
}

// ****************************************************************
// �őO�ʂɐݒ�
// ================================================================
void C_window_base::set_fore_ground()
{
	SetForegroundWindow(m_h_wnd);
}

// ****************************************************************
// �\���^��\����ݒ�
// ================================================================
void C_window_base::set_show(bool show)
{
	if (m_h_wnd == NULL)
		return;

	if (get_show() != show)
		ShowWindow(m_h_wnd, show ? SW_SHOW : SW_HIDE);
}

// ****************************************************************
// �\���^��\�����擾
// ================================================================
bool C_window_base::get_show()
{
	return (GetWindowLongPtr(m_h_wnd, GWL_STYLE) & WS_VISIBLE) > 0;
}

// ****************************************************************
// �L���^������ݒ�
// ================================================================
void C_window_base::set_enable(bool enable)
{
	if (m_h_wnd == NULL)
		return;	

	if (get_enable() != enable)
		EnableWindow(m_h_wnd, enable);
}

// ****************************************************************
// �L���^�������擾
// ================================================================
bool C_window_base::get_enable()
{
	if (m_h_wnd == NULL)
		return false;	

	return ::IsWindowEnabled(m_h_wnd) ? true : false;
}

// ****************************************************************
// �A�C�R�����擾
// ================================================================
HICON C_window_base::get_icon()
{
	return (HICON)(PTR)GetClassLongPtr(m_h_wnd, GCLP_HICON);
}

// ****************************************************************
// �A�C�R����ݒ�
// ================================================================
void C_window_base::set_icon(HICON h_icon)
{
	if (m_h_wnd == NULL)
		return;

	SetClassLongPtr(m_h_wnd, GCLP_HICON, (LONG)(PTR)h_icon);
}
void C_window_base::set_small_icon(HICON h_icon)
{
	if (m_h_wnd == NULL)
		return;

	SetClassLongPtr(m_h_wnd, GCLP_HICONSM, (LONG)(PTR)h_icon);
}

// ****************************************************************
// �A�C�R����ݒ�
// ================================================================
void C_window_base::set_icon_from_resource(int icon_id)
{
	if (m_h_wnd == NULL)
		return;

	// �傫���A�C�R��
	if (h_large_icon)	{
		DestroyIcon(h_large_icon);
		h_large_icon = NULL;
	}
	h_large_icon = (HICON)LoadImage(G_app.h_inst, MAKEINTRESOURCE(icon_id), IMAGE_ICON, 32, 32, 0);
	if (h_large_icon == NULL)	{
		h_large_icon = (HICON)LoadImage(G_app.h_inst, MAKEINTRESOURCE(icon_id), IMAGE_ICON, 0, 0, 0);
	}
	set_icon(h_large_icon);

	// �������A�C�R��
	if (h_small_icon)	{
		DestroyIcon(h_small_icon);
		h_small_icon = NULL;
	}
	h_small_icon = (HICON)LoadImage(G_app.h_inst, MAKEINTRESOURCE(icon_id), IMAGE_ICON, 16, 16, 0);
	if (h_small_icon == NULL)	{
		h_small_icon = (HICON)LoadImage(G_app.h_inst, MAKEINTRESOURCE(icon_id), IMAGE_ICON, 0, 0, 0);
	}
	set_small_icon(h_small_icon);
}

// ****************************************************************
// ���j���[���擾
// ================================================================
HMENU C_window_base::get_menu()
{
	return GetMenu(m_h_wnd);
}

// ****************************************************************
// ���j���[��ݒ�
// ================================================================
void C_window_base::set_menu(HMENU h_menu)
{
	if (m_h_wnd == NULL)
		return;

	SetMenu(m_h_wnd, h_menu);
}

// ****************************************************************
// �e�L�X�g���擾
// ================================================================
TSTR C_window_base::get_text()
{
	if (m_h_wnd == NULL)
		return _T("");

	TCHAR buf[1024];
	GetWindowText(m_h_wnd, buf, 1024);
	return buf;
}

// ****************************************************************
// �e�L�X�g��ݒ�
// ================================================================
void C_window_base::set_text(CTSTR& str)
{
	if (m_h_wnd == NULL)
		return;

	if (get_text() == str)
		return;

	SetWindowText(m_h_wnd, str.c_str());
}

// ****************************************************************
// �X�^�C����ݒ�
// ================================================================
void C_window_base::set_style(PTR style)
{
	if (m_h_wnd == NULL)
		return;

	SetWindowLongPtr(m_h_wnd, GWL_STYLE, (LONG)style);
}

// ****************************************************************
// �X�^�C����ǉ�
// ================================================================
void C_window_base::add_style(PTR style)
{
	if (m_h_wnd == NULL)
		return;

	set_style(get_style() | style);
}

// ****************************************************************
// �X�^�C�����擾
// ================================================================
PTR C_window_base::get_style()
{
	return (PTR)GetWindowLongPtr(m_h_wnd, GWL_STYLE);
}

// ****************************************************************
// �g���X�^�C����ݒ�
// ================================================================
void C_window_base::set_style_ex(PTR style)
{
	if (m_h_wnd == NULL)
		return;

	SetWindowLongPtr(m_h_wnd, GWL_EXSTYLE, (LONG)style);
}

// ****************************************************************
// �g���X�^�C����ǉ�
// ================================================================
void C_window_base::add_style_ex(PTR style)
{
	if (m_h_wnd == NULL)
		return;

	set_style_ex(get_style_ex() | style);
}

// ****************************************************************
// �g���X�^�C�����擾
// ================================================================
PTR C_window_base::get_style_ex()
{
	return (PTR)GetWindowLongPtr(m_h_wnd, GWL_EXSTYLE);
}

// ****************************************************************
// �v���V�[�W����ݒ�
// ================================================================
void C_window_base::set_proc(WNDPROC proc)
{
	if (m_h_wnd == NULL)
		return;

	SetWindowLongPtr(m_h_wnd, GWLP_WNDPROC, (LONG)(PTR)proc);
}

// ****************************************************************
// �v���V�[�W�����擾
// ================================================================
WNDPROC C_window_base::get_proc()
{
	return (WNDPROC)(PTR)GetWindowLongPtr(m_h_wnd, GWLP_WNDPROC);
}

// ****************************************************************
// �v���V�[�W�������ɖ߂�
// ================================================================
void C_window_base::reset_proc()
{
	if (m_h_wnd == NULL)
		return;

	SetWindowLongPtr(m_h_wnd, GWLP_WNDPROC, (LONG)(PTR)C_window_base::window_proc_base);
}

// ****************************************************************
// ���b�Z�[�W�̑��M
// ================================================================
LRESULT C_window_base::send_message(UINT msg, WPARAM wp, LPARAM lp)
{
	if (m_h_wnd == NULL)
		return FALSE;

	return ::SendMessage(m_h_wnd, msg, wp, lp);
}

LRESULT C_window_base::post_message(UINT msg, WPARAM wp, LPARAM lp)
{
	if (m_h_wnd == NULL)
		return FALSE;

	return ::PostMessage(m_h_wnd, msg, wp, lp);
}

// ****************************************************************
// WM_SIZE �𑗐M
// ================================================================
LRESULT C_window_base::send_wm_size()
{
	if (m_h_wnd == NULL)
		return FALSE;

	C_rect rect = get_client_rect();
	return send_message(WM_SIZE, NULL, MAKELPARAM(rect.width(), rect.height()));
}

// ****************************************************************
// ��ʂ��X�V
// ================================================================
void C_window_base::invalidate_rect(C_rect* rect, bool erase)
{
	if (m_h_wnd == NULL)
		return;

	::InvalidateRect(m_h_wnd, rect, erase ? TRUE : FALSE);
}

void C_window_base::invalidate_rect(ARRAY<C_rect>& rect_list, bool erase)
{
	if (m_h_wnd == NULL)
		return;

	// �S��`�ɑ΂��ĉ�ʂ��X�V
	for (ARRAY<C_rect>::iterator rect_itr = rect_list.begin(); rect_itr != rect_list.end(); ++rect_itr)
		::InvalidateRect(m_h_wnd, &*rect_itr, erase ? TRUE : FALSE);
}

// ****************************************************************
// �X�V
// ================================================================
void C_window_base::update()
{
	if (m_h_wnd == NULL)
		return;

	::UpdateWindow(m_h_wnd);
}

// ****************************************************************
// �X�N���[�����W����N���C�A���g���W�֕ϊ�
// ================================================================
C_point C_window_base::screen_to_client(int x, int y)
{
	return screen_to_client(C_point(x, y));
}

C_point C_window_base::screen_to_client(C_point point)
{
	ScreenToClient(m_h_wnd, &point);
	return point;
}

C_rect C_window_base::screen_to_client(C_rect rect)
{
	C_point lt = rect.left_top();		ScreenToClient(m_h_wnd, &lt);
	C_point rb = rect.right_bottom();	ScreenToClient(m_h_wnd, &rb);
	return C_rect(lt.x, lt.y, rb.x, rb.y);
}

// ****************************************************************
// �N���C�A���g���W����X�N���[�����W�֕ϊ�
// ================================================================
C_point C_window_base::client_to_screen(int x, int y)
{
	return client_to_screen(C_point(x, y));
}

C_point C_window_base::client_to_screen(C_point point)
{
	ClientToScreen(m_h_wnd, &point);
	return point;
}

C_rect C_window_base::client_to_screen(C_rect rect)
{
	C_point lt = rect.left_top();		ClientToScreen(m_h_wnd, &lt);
	C_point rb = rect.right_bottom();	ClientToScreen(m_h_wnd, &rb);

	return C_rect(lt.x, lt.y, rb.x, rb.y);
}

// ****************************************************************
// �N���C�A���g��`����E�B���h�E��`�֕ϊ�
// ================================================================
C_rect C_window_base::client_to_window(C_rect rect)
{
	::AdjustWindowRectEx(&rect, GetWindowLong(m_h_wnd, GWL_STYLE), (GetMenu(m_h_wnd) != NULL) ? TRUE : FALSE, (DWORD)get_style_ex());
	return rect;
}

// ****************************************************************
// ini �ɕۑ�
// ================================================================

// ini �ɕ������ۑ�
void C_window_base::save_ini_str(CTSTR& key, CTSTR& str, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().save_str(key, str, project_name, get_parent_name(), m_window_name);
}

// ini �� bool �l��ۑ�
void C_window_base::save_ini_bool(CTSTR& key, bool value, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().save_bool(key, value, project_name, get_parent_name(), m_window_name);
}

// ini �� int �l��ۑ�
void C_window_base::save_ini_int(CTSTR& key, int value, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().save_int(key, value, project_name, get_parent_name(), m_window_name);
}

// ini �Ɉʒu��ۑ�
void C_window_base::save_ini_pos(CTSTR& key, C_point pos, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().save_point(key, pos, project_name, get_parent_name(), m_window_name);
}

// ini �ɋ�`��ۑ�
void C_window_base::save_ini_rect(CTSTR& key, C_rect rect, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().save_rect(key, rect, project_name, get_parent_name(), m_window_name);
}

// ****************************************************************
// ini ����擾
// ================================================================

// ini ���當������擾
TSTR C_window_base::load_ini_str(CTSTR& key, CTSTR& def, CTSTR& project_name)
{
	if (!m_window_name.empty())
		return C_ini_file::body().load_str(key, def, project_name, get_parent_name(), m_window_name);

	return def;
}

// ini ���� bool �l���擾
bool C_window_base::load_ini_bool(CTSTR& key, bool def, CTSTR& project_name)
{
	if (!m_window_name.empty())
		return C_ini_file::body().load_bool(key, def, project_name, get_parent_name(), m_window_name);

	return def;
}

// ini ���� int �l���擾
int C_window_base::load_ini_int(CTSTR& key, int def, CTSTR& project_name)
{
	if (!m_window_name.empty())	{
		return C_ini_file::body().load_int(key, def, project_name, get_parent_name(), m_window_name);
	}

	return def;
}

// ini ����ʒu��Ǎ�
C_point C_window_base::load_ini_pos(CTSTR& key, C_point def, CTSTR& project_name)
{
	if (!m_window_name.empty())
		return C_ini_file::body().load_point(key, def, project_name, get_parent_name(), m_window_name);

	return def;
}

// ini �����`��Ǎ�
C_rect C_window_base::load_ini_rect(CTSTR& key, C_rect def, CTSTR& project_name)
{
	if (!m_window_name.empty())
		return C_ini_file::body().load_rect(key, def, project_name, get_parent_name(), m_window_name);

	return def;
}

// ****************************************************************
// ini ����폜
// ================================================================
void C_window_base::delete_ini(CTSTR& key, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().delete_value(key, project_name, get_parent_name(), m_window_name);
}

// ****************************************************************
// ini ���� project �ȉ����폜�ikey �� _T("") �� project_name �݂̂Ȃ�Aproject_name �ɑ�����L�[��S�č폜����j
// ================================================================
void C_window_base::delete_ini_project(CTSTR& key, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().delete_value(key, project_name);
}

// ****************************************************************
// ini �ɃE�B���h�E��`��ۑ��^�Ǎ�
// ================================================================

// ini �ɃE�B���h�E�ʒu��ۑ�
void C_window_base::save_ini_window_pos()
{
	if (m_h_wnd == NULL)	{	return;	}

	// �ŏ����^�ő剻�O�̃E�B���h�E��`�̎擾
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_h_wnd, &wp);

	save_ini_pos(_T("window_pos"), C_point(wp.rcNormalPosition.left, wp.rcNormalPosition.top));
}

// ini ����E�B���h�E�ʒu��Ǎ�
void C_window_base::load_ini_window_pos()
{
	// �E�B���h�E��`�̎擾
	C_point wp = load_ini_pos(_T("window_pos"), C_point(INT_MIN, INT_MIN));
	if (wp != C_point(INT_MIN, INT_MIN))
		set_window_pos(wp);

	// ���}���`���j�^�̏ꍇ�A�E�B���h�E���W���}�C�i�X���Ă͕̂��ʂɋN���肦�܂��B
}

// ini �ɃE�B���h�E��`��ۑ�
void C_window_base::save_ini_window_rect()
{
	if (m_h_wnd == NULL)
		return;

	// �ŏ����^�ő剻�O�̃E�B���h�E��`�̎擾
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_h_wnd, &wp);

	save_ini_rect(_T("window_rect"), wp.rcNormalPosition);
}

// ini ����E�B���h�E��`��Ǎ�
void C_window_base::load_ini_window_rect(C_rect def)
{
	// �E�B���h�E��`�̎擾
	C_rect wr = load_ini_rect(_T("window_rect"), def);
	if (wr.width() > 0 && wr.height() > 0)
		set_window_rect(wr);

	// ���}���`���j�^�̏ꍇ�A�E�B���h�E���W���}�C�i�X���Ă͕̂��ʂɋN���肦�܂��B
}

// ****************************************************************
// ini �ɕ\���^��\����ۑ��^�Ǎ�
// ================================================================

// ini �ɕ\���^��\����ۑ�
void C_window_base::save_ini_show()
{
	save_ini_bool(_T("window_show"), get_show());
}

// ini ����\���^��\����Ǎ�
void C_window_base::load_ini_show(bool def)
{
	set_show(load_ini_bool(_T("window_show"), def));
}

/*
// ****************************************************************
// �X�N���[���o�[�F���ݒ�
// ================================================================
void C_window_base::SetScrollInfo(int type, int min, int max, int pageSize)
{
	SCROLLINFO info;
	ZeroMemory(&info, sizeof(SCROLLINFO));
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_RANGE | SIF_PAGE;
	info.nMin = min;
	info.nMax = max;
	info.nPage = pageSize;
	::SetScrollInfo(hWnd, type, &info, TRUE);
}

// ****************************************************************
// �X�N���[���o�[�F�ʒu�̐ݒ�
// ================================================================
//void	SetScrollPos(bool tate, int pos, bool redraw)

// ****************************************************************
// �X�N���[���o�[�F���R�ȓ���
//		type: �c=SB_VERT, ��=SB_HORZ
// ================================================================
int C_window_base::ScrollNatural(int type, WPARAM wParam)
{
	// ���擾
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_ALL;
	::GetScrollInfo(hWnd, type, &info);

	// �ŏ��l�ƍő�l
	int Min = info.nMin;
	int Max = info.nMax - info.nMin - info.nPage + 1;

	// ����
	if (false);
	else if (LOWORD(wParam) == SB_ENDSCROLL)	{
		info.nPos = HIWORD(wParam);
	}
	else if (LOWORD(wParam) == SB_LEFT)	{				// ���̒[
		info.nPos = info.nMin;
	}
	else if (LOWORD(wParam) == SB_RIGHT)	{			// �E�̒[
		info.nPos = info.nMax - info.nMin - info.nPage + 1;
	}
	else if (LOWORD(wParam) == SB_LINELEFT)	{			// ��
		info.nPos = Limit(Min, info.nPos - 1, Max);
	}
	else if (LOWORD(wParam) == SB_LINERIGHT)	{		// ��
		info.nPos = Limit(Min, info.nPos + 1, Max);
	}
	else if (LOWORD(wParam) == SB_PAGELEFT)	{			// ��[]
		info.nPos = Limit(Min, int(info.nPos - info.nPage), Max);
	}
	else if (LOWORD(wParam) == SB_PAGERIGHT)	{		// []��
		info.nPos = Limit(Min, int(info.nPos + info.nPage), Max);
	}
	else if (LOWORD(wParam) == SB_THUMBPOSITION)	{	// �h���b�O��
		info.nPos = HIWORD(wParam);
	}
	else if (LOWORD(wParam) == SB_THUMBTRACK)	{		// �h���b�O�I��
		info.nPos = HIWORD(wParam);
	}

	// �ʒu�݂̂��X�V
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_POS;
	::SetScrollInfo(hWnd, type, &info, TRUE);

	return info.nPos;
}

*/






// ****************************************************************
// �f�X�N�g�b�v��̈ʒu���烂�j�^�̏����擾����
// ================================================================
bool get_monitor_info_from_pos(MONITORINFO* info, C_point pos)
{
	HMONITOR h_monitor = MonitorFromPoint(pos, MONITOR_DEFAULTTONEAREST);		// �w�肵���_���܂ރf�B�X�v���C���j�^��������Ȃ������ꍇ�́A�w�肵���_�ɍł��߂��ʒu�ɂ���f�B�X�v���C���j�^�̃n���h�����Ԃ�܂��B
	if (h_monitor != NULL)	{
		ZeroMemory(info, sizeof(MONITORINFO));
		info->cbSize = sizeof(MONITORINFO);
		if (GetMonitorInfo(h_monitor, info))
			return true;
	}

	return false;
}

// ****************************************************************
// �f�X�N�g�b�v��̈ʒu����f�X�N�g�b�v��`���擾����
// ================================================================
bool get_desktop_rect_from_pos(C_rect *desktop_rect, C_point pos)
{
	// �w����W���܂ރ��j�^�̏����擾����
	MONITORINFO mi;
	if (get_monitor_info_from_pos(&mi, pos))	{
		*desktop_rect = mi.rcMonitor;
	}
	else	{
		// ���s�����ꍇ�̓v���C�}���[���j�^�[�̃f�X�N�g�b�v��`�擾
		desktop_rect->left = 0;
		desktop_rect->top = 0;
		desktop_rect->right = ::GetSystemMetrics(SM_CXFULLSCREEN);
		desktop_rect->bottom = ::GetSystemMetrics(SM_CYFULLSCREEN);
	}

	return true;
}

// ****************************************************************
// �f�X�N�g�b�v��̈ʒu���烏�[�N�G���A��`���擾����i�f�X�N�g�b�v����^�X�N�o�[�Ȃǂ̗̈�����O�����̈�j
// ================================================================
bool get_work_area_rect_from_pos(C_rect *work_area_rect, C_point pos)
{
	// �w����W���܂ރ��j�^�̏����擾����
	MONITORINFO mi;
	if (get_monitor_info_from_pos(&mi, pos))	{
		*work_area_rect = mi.rcWork;
	}
	else	{
		// ���s�����ꍇ�̓v���C�}���[���j�^�[�̃��[�N�G���A��`�擾
		SystemParametersInfo(SPI_GETWORKAREA, 0, work_area_rect, 0);
	}

	return true;
}






}
