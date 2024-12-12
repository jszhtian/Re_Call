#include	"tonawindow3_pch.h"
#include	"tonawindow3_window.h"

namespace NT3
{

// ****************************************************************
// �E�B���h�E�쐬
//		name : �E�B���h�E�̖��O�iini �ۑ����Ɏg�p�j
//		class_name : �E�B���h�E�N���X��
//		caption : �E�B���h�E�^�C�g��
//		x : �E�B���h�E�̕\���ʒu�w�iCW_USEDEFAULT : �f�t�H���g�A-1 : �����j
//		y : �E�B���h�E�̕\���ʒu�x�iCW_USEDEFAULT : �f�t�H���g�A-1 : �����j
//		client_w : �E�B���h�E�̃N���C�A���g�̈�̕��iCW_USEDEFAULT : �f�t�H���g�j
//		client_h : �E�B���h�E�̃N���C�A���g�̈�̍����iCW_USEDEFAULT : �f�t�H���g�j
//		window_style : �E�B���h�E�X�^�C��
//			WS_CAPTION : �L���v�V����
//			WS_SYSMENU : �V�X�e�����j���[
//			WS_MINIMIZEBOX : �ŏ����{�^��
//		window_style_ex : �g���E�B���h�E�X�^�C��
//		icon_id : �A�C�R���̃��\�[�X�h�c�i0 �Ŗ����j
//		menu_id : ���j���[�̃��\�[�X�h�c�i0 �Ŗ����j
//		brush_type : �w�i��h��Ԃ��u���V�i-1 �Ŗ����j
//		is_limit_min : �E�B���h�E�̍ŏ���`�𐧌�
//		is_attach : ���̃E�B���h�E�ɃA�^�b�`
//		h_parent_wnd : �e�E�B���h�E�n���h��
//		adjust_way : �e�ɂ����������i�ݷ� �̕����Ŏw��j
// ================================================================
bool C_window::create(CTSTR& name, CTSTR& class_name, CTSTR& caption, int x, int y, int client_w, int client_h, int icon_id, int menu_id, int brush_type, DWORD style, DWORD ex_style, bool is_limit_min, bool is_attach, HWND h_parent_wnd, int adjust_way)
{
	// �N���X����ۑ�����
	m_class_name = class_name;

	// ���E�B���h�E�N���X�̖���
	// �{���͂��肦�Ȃ��B�N���X�̓o�^�ƃE�B���h�E�̍쐬���ꏏ�����ɂ������������ł���|���c�B
	// WM_DESTROY �̒��ł� UnregisterClass �����s���Ă錏�ɑ΂��鉼���u�B
	C_window_base::unregist_class(m_class_name);

	// �E�B���h�E�N���X�̓o�^
	if (!C_window_base::regist_class(m_class_name, icon_id, menu_id, brush_type))
		return false;

	// �E�B���h�E�̍쐬
	if (!C_window_base::create(name, m_class_name, caption, x, y, client_w, client_h, menu_id > 0, style, ex_style, is_limit_min, is_attach, h_parent_wnd))
		return false;

	return true;
}

// ****************************************************************
// �p�������E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_window::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	// WM_DESTROY �݂̂�����
	switch (msg){
		case WM_DESTROY:	if (!on_destroy())		return FALSE;	break;
	}

	// ���N���X�̃E�B���h�E�v���V�[�W�����Ăяo��
	return C_window_base::window_proc(msg, wp, lp);
}

// ****************************************************************
// �E�B���h�E���j�󂳂ꂽ
// ================================================================
bool C_window::on_destroy()
{
	// �E�B���h�E�N���X�̖���
	C_window_base::unregist_class(m_class_name);

	// �������ꎸ�s���Ă�˂�ˁc�B�S���C�Â��Ȃ������B
	// WM_DESTROY �̒��ł͂܂� UnregisterClass �ł��Ȃ��݂����B
	// �������B�������Ȃ��Ƃ����Ȃ��B

	// �p�����[�^�̏�����
	m_class_name.clear();

	return true;
}

}
