#pragma		once

namespace NT3
{

// ****************************************************************
// �E�B���h�E�A�^�b�`�\����
// ================================================================
const int 		WINDOW_ATTACH_REP_SIZE = 16;

struct WINDOW_ATTACH
{
	int			mod;						// 1=�ړ��� 2=�T�C�Y�ύX��
	HWND		h_wnd;						// �A�^�b�`���̃E�B���h�E�n���h��
	int			x, y;						// �A�^�b�`�J�n���̃}�E�X���W
	int			w, h;						// �A�^�b�`���̃E�B���h�E�T�C�Y
	bool		left, right, top, bottom;	// ���ꂼ��̌����ɂ������t���O
	ARRAY<HWND>	h_wnd_list;					// �A�^�b�`���s���E�B���h�E���X�g
};

struct WINDOW_ATTACH_PROC
{
	HWND width_h_wnd, height_h_wnd;
	int width_mod, height_mod;
	bool desktop_width_flag, desktop_height_flag;
};

// ****************************************************************
// ���̃E�B���h�E
// ================================================================
class C_window_base
{
public:
	C_window_base();
	~C_window_base();

	static	bool	regist_class(CTSTR& class_name, int icon_id, int menu_id, int brush_type);
	static	void	unregist_class(CTSTR& class_name);

	// �E�B���h�E�̍\�z�Ɣj��
	bool	create(CTSTR& name, CTSTR& class_name, CTSTR& caption, int x, int y, int client_w, int client_h, bool is_menu, DWORD style, DWORD ex_style, bool limit_min, bool attach, HWND h_parent_wnd);
	void	close();			// �E�B���h�E�����
	void	destroy();			// �E�B���h�E��j�󂷂�
	bool	bind(HWND h_wnd);	// �E�B���h�E�n���h���ƃC���X�^���X�����т���
	void	unbind();			// �E�B���h�E�n���h���ƃC���X�^���X�����т�����������
	bool	is_open()	{	return m_h_wnd != NULL;	}

	// ini �ɕۑ�
	void	save_ini_str(CTSTR& key, CTSTR& str, CTSTR& project_name = _T(""));		// ini �ɕ������ۑ�
	void	save_ini_bool(CTSTR& key, bool value, CTSTR& project_name = _T(""));	// ini �� bool �l��ۑ�
	void	save_ini_int(CTSTR& key, int value, CTSTR& project_name = _T(""));		// ini �ɐ�����ۑ�
	void	save_ini_pos(CTSTR& key, C_point pos, CTSTR& project_name = _T(""));		// ini �Ɉʒu��ۑ�
	void	save_ini_rect(CTSTR& key, C_rect rect, CTSTR& project_name = _T(""));	// ini �ɋ�`��ۑ�

	// ini ����擾
	TSTR	load_ini_str(CTSTR& key, CTSTR& def, CTSTR& project_name = _T(""));		// ini ���當�����Ǎ�
	bool	load_ini_bool(CTSTR& key, bool def, CTSTR& project_name = _T(""));		// ini ���� bool �l��Ǎ�
	int		load_ini_int(CTSTR& key, int def, CTSTR& project_name = _T(""));		// ini ���琮����Ǎ�
	C_point	load_ini_pos(CTSTR& key, C_point def, CTSTR& project_name = _T(""));		// ini ����ʒu��Ǎ�
	C_rect	load_ini_rect(CTSTR& key, C_rect def, CTSTR& project_name = _T(""));		// ini �����`��Ǎ�

	// ini ����폜�ikey �� _T("") �� project_name �݂̂Ȃ�Aproject_name �ɑ�����L�[��S�č폜����j
	void	delete_ini(CTSTR& key, CTSTR& project_name);							// ini ����폜

	// ini ���� project �ȉ����폜�ikey �� _T("") �� project_name �݂̂Ȃ�Aproject_name �ɑ�����L�[��S�č폜����j
	void	delete_ini_project(CTSTR& key, CTSTR& project_name);					// ini ���� project �ȉ����폜

	// ini �֘A
	void	save_ini_window_pos();				// ini �ɃE�B���h�E�ʒu��ۑ�
	void	load_ini_window_pos();				// ini ����E�B���h�E�ʒu��Ǎ�
	void	save_ini_window_rect();				// ini �ɃE�B���h�E��`��ۑ�
	void	load_ini_window_rect(C_rect def = C_rect(0, 0, 0, 0));	// ini ����E�B���h�E��`��Ǎ�
	void	save_ini_show();					// ini �ɕ\���^��\����ۑ�
	void	load_ini_show(bool def);			// ini ����\���^��\����Ǎ�

	void	set_main_window();				// ���C���E�B���h�E�ɐݒ�i�j�󂳂ꂽ�Ƃ��� WM_QUIT ���΂��悤�ɂȂ�܂��j
	HWND	get_handle();					// �E�B���h�E�n���h�����擾
	WORD	get_id();						// �E�B���h�E id ���擾
	TSTR	get_class();					// �E�B���h�E�N���X���擾
	TSTR	get_name();						// ���O���擾
	void	set_name(CTSTR& name);			// ���O��ݒ�
	void	set_adjust_way(int adjust_way);	// �e�ւ̂�����������ݒ�
	int		get_group_no();					// �O���[�v�ԍ����擾
	void	set_group_no(int group_no);		// �O���[�v�ԍ���ݒ�

	C_window_base*	get_parent();				// �e�E�B���h�E���擾
	HWND			get_parent_handle();		// �e�̃E�B���h�E�n���h�����擾
	TSTR			get_parent_name();			// �e�E�B���h�E�̖��O���擾

	void	set_window_pos(C_point pos);		// �E�B���h�E�̈ʒu��ݒ�i�X�N���[�����W�n�Ŏw��j
	void	set_client_pos(C_point pos);		// �N���C�A���g�̈�̈ʒu��ݒ�i�X�N���[�����W�n�Ŏw��j
	void	set_window_size(C_size size);		// �E�B���h�E�̑傫����ݒ�
	void	set_client_size(C_size size);		// �N���C�A���g�̈�̑傫����ݒ�
	void	set_window_rect(C_rect rect);		// �E�B���h�E�̈ʒu�Ƒ傫����ݒ�i�X�N���[�����W�n�Ŏw��j
	void	set_client_rect(C_rect rect);		// �N���C�A���g�̈�̈ʒu�Ƒ傫����ݒ�i�X�N���[�����W�n�Ŏw��j
	C_rect	get_window_rect();					// �E�B���h�E�̈ʒu�Ƒ傫�����擾�i�X�N���[�����W�n�Ŏ擾�j
	C_rect	get_client_rect();					// �N���C�A���g�̈�̈ʒu�Ƒ傫�����擾�i�N���C�A���g���W�n�Ŏ擾�j
	C_rect	get_rltv_rect();					// �E�B���h�E�̈ʒu�Ƒ傫�����擾�i�e�̃N���C�A���g���W�n�Ŏ擾�j

	void	set_def_window_rect(C_rect rect);	// �f�t�H���g�̃E�B���h�E�̈ʒu�Ƒ傫����ݒ�i�X�N���[�����W�n�Őݒ�j
	C_rect	get_def_window_rect();				// �f�t�H���g�̃E�B���h�E�̈ʒu�Ƒ傫�����擾�i�X�N���[�����W�n�Ŏ擾�j
	void	set_def_client_width(int width);	// �f�t�H���g�̃N���C�A���g�̈�̕���ݒ�i�N���C�A���g���W�n�Őݒ�j
	void	set_def_client_height(int height);	// �f�t�H���g�̃N���C�A���g�̈�̍�����ݒ�i�N���C�A���g���W�n�Őݒ�j
	void	set_def_client_rect(C_rect rect);	// �f�t�H���g�̃N���C�A���g�̈�̈ʒu�Ƒ傫����ݒ�i�N���C�A���g���W�n�Őݒ�j
	C_rect	get_def_client_rect();				// �f�t�H���g�̃N���C�A���g�̈�̈ʒu�Ƒ傫�����擾�i�N���C�A���g���W�n�Ŏ擾�j
	void	set_def_rltv_rect(C_rect rect);		// �f�t�H���g�̃E�B���h�E�̈ʒu�Ƒ傫����ݒ�i�e�̃N���C�A���g���W�n�Őݒ�j
	C_rect	get_def_rltv_rect();				// �f�t�H���g�̃E�B���h�E�̈ʒu�Ƒ傫�����擾�i�e�̃N���C�A���g���W�n�Ŏ擾�j

	void	set_def_rect_from_now_rect();			// ���݂̋�`�ɍ��킹�ăf�t�H���g�̋�`���Đݒ肷��
	void	set_def_window_rect_from_now_rect();	// ���݂̋�`�ɍ��킹�ăf�t�H���g�̃E�B���h�E��`���Đݒ肷��
	void	set_def_client_rect_from_now_rect();	// ���݂̋�`�ɍ��킹�ăf�t�H���g�̃N���C�A���g��`���Đݒ肷��
	void	set_def_rltv_rect_from_now_rect();		// ���݂̋�`�ɍ��킹�ăf�t�H���g�̑��΋�`���Đݒ肷��

	void	set_def_rect_from_now_size();			// ���݂̃T�C�Y�ɍ��킹�ăf�t�H���g�̋�`���Đݒ肷��
	void	set_def_window_rect_from_now_size();	// ���݂̃T�C�Y�ɍ��킹�ăf�t�H���g�̃E�B���h�E��`���Đݒ肷��
	void	set_def_client_rect_from_now_size();	// ���݂̃T�C�Y�ɍ��킹�ăf�t�H���g�̃N���C�A���g��`���Đݒ肷��
	void	set_def_rltv_rect_from_now_size();		// ���݂̃T�C�Y�ɍ��킹�ăf�t�H���g�̑��΋�`���Đݒ肷��

	void	set_window_size_with_def_size(int width, int height);			// �E�B���h�E�T�C�Y��ݒ肵�ăf�t�H���g�̋�`���Đݒ肷��
	void	set_window_size_with_def_size(C_size size);						// �E�B���h�E�T�C�Y��ݒ肵�ăf�t�H���g�̋�`���Đݒ肷��
	void	add_window_size_with_def_size(int rep_width, int rep_height);	// �E�B���h�E�T�C�Y��ݒ肵�ăf�t�H���g�̋�`���Đݒ肷��
	void	add_window_size_with_def_size(C_size rep_size);					// �E�B���h�E�T�C�Y��ݒ肵�ăf�t�H���g�̋�`���Đݒ肷��

	void	set_rltv_pos_with_def_rect(int x, int y);				// ���΍��W��ݒ肵�ăf�t�H���g�̋�`���Đݒ肷��
	void	set_rltv_pos_with_def_rect(C_point pos);				// ���΍��W��ݒ肵�ăf�t�H���g�̋�`���Đݒ肷��
	void	add_rltv_pos_with_def_rect(int rep_x, int rep_y);		// ���΍��W�𒲐����ăf�t�H���g�̋�`���Đݒ肷��
	void	add_rltv_pos_with_def_rect(C_point rep_pos);			// ���΍��W�𒲐����ăf�t�H���g�̋�`���Đݒ肷��

	bool	get_my_monitor_info(MONITORINFO* info);					// �E�B���h�E�̑��݂��郂�j�^�̃n���h�����擾
	void	get_my_desktop_rect(C_rect *desktop_rect);				// �E�B���h�E�̑��݂��郂�j�^�̃f�X�N�g�b�v��`�擾
	void	get_my_desktop_work_area_rect(C_rect *work_area_rect);	// �E�B���h�E�̑��݂��郂�j�^�̃��[�N�G���A��`�擾�i�f�X�N�g�b�v����^�X�N�o�[�Ȃǂ̗̈�����O�����̈���擾�j
	void	set_pos_my_desktop_center();							// �E�B���h�E�̑��݂��郂�j�^�̒����Ɉړ�

	bool	rep_window_pos_outside_desktop(int rep_mod = 0, int outside_size = 34);		// �f�X�N�g�b�v�͈͊O�ɂ���ꍇ�A�E�B���h�E���W���f�X�N�g�b�v�̒��S�ɕ␳����

	C_point	screen_to_client(int x, int y);		// �X�N���[�����W�n �� �N���C�A���g���W�n
	C_point	screen_to_client(C_point point);	// �X�N���[�����W�n �� �N���C�A���g���W�n
	C_rect	screen_to_client(C_rect rect);		// �X�N���[�����W�n �� �N���C�A���g���W�n
	C_point	client_to_screen(int x, int y);		// �N���C�A���g���W�n �� �X�N���[�����W�n
	C_point	client_to_screen(C_point point);	// �N���C�A���g���W�n �� �X�N���[�����W�n
	C_rect	client_to_screen(C_rect rect);		// �N���C�A���g���W�n �� �X�N���[�����W�n
	C_rect	client_to_window(C_rect rect);		// �N���C�A���g���W�n �� �E�B���h�E���W�n

	void	adjust_to_parent(int dw, int dh);					// �e�E�B���h�E�ɂ��킹�Ĉʒu�𒲐�
	void	adjust_to_parent_4(int dl, int dt, int dw, int dh);	// �e�E�B���h�E�ɂ��킹�Ĉʒu�𒲐�

	void	set_top_most(bool top_most);	// ��Ɏ�O�ɕ\��
	void	set_focus();					// �t�H�[�J�X��ݒ�
	void	set_active();					// �A�N�e�B�u�ɂ���
	void	set_fore_ground();				// �őO�ʂɐݒ�
	void	set_show(bool show);			// �\���^��\����ݒ�
	bool	get_show();						// �\���^��\�����擾
	void	set_enable(bool enable);		// �L���^������ݒ�
	bool	get_enable();					// �L���^�������擾
	void	set_text(CTSTR& str);			// �e�L�X�g��ݒ�
	TSTR	get_text();						// �e�L�X�g���擾
	void	set_icon(HICON h_icon);			// �A�C�R����ݒ�
	void	set_small_icon(HICON h_icon);	// �������A�C�R����ݒ�
	void	set_icon_from_resource(int icon_id);	// �A�C�R����ݒ�i���\�[�X����j
	HICON	get_icon();						// �A�C�R�����擾
	void	set_menu(HMENU h_menu);			// ���j���[��ݒ�
	HMENU	get_menu();						// ���j���[���擾
	void	set_style(PTR style);			// �X�^�C����ݒ�
	void	add_style(PTR style);			// �X�^�C����ǉ�
	PTR		get_style();					// �X�^�C�����擾
	void	set_style_ex(PTR style);		// �g���X�^�C����ݒ�
	void	add_style_ex(PTR style);		// �g���X�^�C����ǉ�
	PTR		get_style_ex();					// �g���X�^�C�����擾
	void	set_proc(WNDPROC proc);			// �v���V�[�W����ݒ�
	WNDPROC	get_proc();						// �v���V�[�W�����擾
	void	reset_proc();					// �v���V�[�W�������ɖ߂�

	void	update();												// �X�V
	void	invalidate_rect(C_rect* rect, bool erase);				// ��`���X�V�iWM_PAINT �����M����܂��j
	void	invalidate_rect(ARRAY<C_rect>& rect_list, bool erase);	// ��`���X�V�i�����w��j�iWM_PAINT �����M����܂��j

	LRESULT	send_message(UINT msg, WPARAM wp, LPARAM lp);	// ���b�Z�[�W�𑗐M
	LRESULT	post_message(UINT msg, WPARAM wp, LPARAM lp);	// ���b�Z�[�W�𑗐M
	LRESULT	send_wm_size();									// WM_SIZE �𑗐M

	// �q���R���g���[�����X�g
	typedef	ARRAY<C_window_base *>	CHILDLIST;
	typedef	CHILDLIST::iterator		CHILDITR;
	CHILDITR	get_child_list_begin()	{	return m_child_list.begin();	}
	CHILDITR	get_child_list_end()	{	return m_child_list.end();		}

public:
	// �R�[���o�b�N�{�́i�S�E�B���h�E�ŋ��ʁj
	static LRESULT CALLBACK window_proc_base(HWND h_wnd, UINT msg, WPARAM wp, LPARAM lp);

protected:
	// �E�B���h�E���
	HWND		m_h_wnd;					// �E�B���h�E�n���h��
	TSTR		m_window_name;				// ���O�iini �ۑ����Ɏg�p�j
	WNDPROC		m_old_window_proc;			// �Â��E�B���h�E�v���V�[�W��
	bool		m_is_dialog;				// �_�C�A���O�t���O
	C_rect		m_def_window_rect;			// �f�t�H���g�̃E�B���h�E��`�i�X�N���[�����W�n�j
	C_rect		m_def_client_rect;			// �f�t�H���g�̃N���C�A���g��`�i�N���C�A���g���W�n�j
	C_rect		m_def_rltv_rect;			// �f�t�H���g�̃E�B���h�E��`�i�e�̃N���C�A���g���W�n�j
	bool		m_is_limit_window_rect_min;	// �ŏ���`�𐧌�
	bool		m_is_attach_window;			// ���̃E�B���h�E�ɃA�^�b�`
	int			m_adjust_way;				// �e�_�C�A���O�̃T�C�Y�ύX���ɐe�_�C�A���O�ɂ����������i1-9: �e���L�[�����j
	int			m_group_no;					// �O���[�v�ԍ��i���R�ɂ��g�����������j
	CHILDLIST	m_child_list;				// �q�����X�g
	static	WINDOW_ATTACH	m_attach;		// �E�B���h�E�A�^�b�`���
	HICON		h_large_icon;				// ��A�C�R���i�R�Q�~�R�Q�j
	HICON		h_small_icon;				// ���A�C�R���i�P�U�~�P�U�j

	void		init_param();				// ������
	
	virtual	void		window_proc_pre(UINT msg, WPARAM wp, LPARAM lp);	// �ʂ̃v���v���V�[�W���i�p������j
	virtual	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);		// �ʂ̃v���V�[�W���i�p������j

private:
	void	on_create_pre();
	bool	on_close();
	bool	on_destroy();
	bool	on_moving(WPARAM wp, LPARAM lp);
	bool	on_sizing(WPARAM wp, LPARAM lp);

	void	limit_window_size(WPARAM wp, LPARAM lp, C_rect min_rect);	// �E�B���h�E�T�C�Y�𐧌�����

	// �E�B���h�E�A�^�b�`�n
	void	attach_init();
	void	attach_on_moving(WPARAM wp, LPARAM lp);
	void	attach_on_sizing(WPARAM wp, LPARAM lp);
	void	attach_check(HWND h_wnd, C_rect *rect1, C_rect *rect2, WINDOW_ATTACH_PROC *wap);
	void	attach_rep(C_rect *rect1, WINDOW_ATTACH_PROC *wap);

};

// �E�B���h�E�֘A�̊֐�
bool	get_monitor_info_from_pos(MONITORINFO* info, C_point pos);
bool	get_desktop_rect_from_pos(C_rect *desktop_rect, C_point pos);
bool	get_work_area_rect_from_pos(C_rect *work_area_rect, C_point pos);

}
