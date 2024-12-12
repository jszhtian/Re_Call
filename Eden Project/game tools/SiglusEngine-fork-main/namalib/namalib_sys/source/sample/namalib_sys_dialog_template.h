#pragma		once

#define		NAMALIB_DLGTPLT_BASE_LEFT		3
#define		NAMALIB_DLGTPLT_BASE_RIGHT		4
#define		NAMALIB_DLGTPLT_BASE_TOP		4
#define		NAMALIB_DLGTPLT_BASE_BOTTOM		3
#define		NAMALIB_DLGTPLT_HEIGHT			14
#define		NAMALIB_DLGTPLT_SPACE_X			4
#define		NAMALIB_DLGTPLT_SPACE_Y			4

// ****************************************************************
// �_�C�A���O�e���v���[�g
// ================================================================
class Cnamalib_dialog_template : public Cdialog_template
{

public:

	// �_�C�A���O����
	void init_standard_tool_window(CTSTR& title, bool size_change_flag);

	// �{�^���ǉ�
	void add_btn(int x, int y, int w, int h, WORD control_id, CTSTR& text, bool group_flag);
	// �e�L�X�g�ǉ�
	void add_txt(int x, int y, int w, int h, WORD control_id, CTSTR& text, bool right_flag);
	// �G�f�B�b�g�{�b�N�X�ǉ�
	void add_edt(int x, int y, int w, int h, WORD control_id, bool dropdown_flag);
	// ���X�g�r���[�ǉ��i���|�[�g�^�j
	void add_lstv(int x, int y, int w, int h, WORD control_id);

	// �z�u�p�����[�^
	struct ARRANGEMENT_PARAM{
		int x, y;				// ���ݍ��W
		int undo_x, undo_y;		// �O�̍��W
		int push_x, push_y;		// �o�t�r�g���W
		int max_x, max_y;		// �ő���W
	} ap;

	void init_ap(void);							// �z�u�p�����[�^������
	void next_ap(int ap_mod, int w, int h);		// �z�u�p�����[�^���̍��W��
	void set_size_from_ap(void);				// �z�u�p�����[�^����_�C�A���O�T�C�Y��ݒ�
	int  get_ap_max_width(void);				// �z�u�p�����[�^����A�C�e���̍ő啝��ݒ�
	int  get_ap_max_height(void);				// �z�u�p�����[�^����A�C�e���̍ő卂��ݒ�
	void undo_ap_x(void);						// �z�u�p�����[�^�w�߂�
	void undo_ap_y(void);						// �z�u�p�����[�^�x�߂�
	void set_ap_x(int x);						// �z�u�p�����[�^�w�ݒ�
	void set_ap_y(int y);						// �z�u�p�����[�^�x�ݒ�
	void add_ap_x(int x);						// �z�u�p�����[�^�w���Z
	void add_ap_y(int y);						// �z�u�p�����[�^�x���Z
	void set_ap_x_right_arrangement(int cnt, ...);	// �z�u�p�����[�^�w�ݒ�i�E�ʒu�����ꍇ�̂w���W�j
	void push_ap_x(void);						// �z�u�p�����[�^�w�o�t�r�g
	void push_ap_y(void);						// �z�u�p�����[�^�x�o�t�r�g
	void pop_ap_x(void);						// �z�u�p�����[�^�w�o�n�o
	void pop_ap_y(void);						// �z�u�p�����[�^�x�o�n�o

	// �z�u�p�����[�^�ŁF�{�^���ǉ�
	void add_ap_btn(int ap_mod, int w, int h, WORD control_id, CTSTR& text, bool group_flag);
	// �z�u�p�����[�^�ŁF�e�L�X�g�ƃG�f�B�b�g�{�b�N�X�ǉ�
	void add_ap_tedt(int ap_mod, int txt_w, WORD txt_control_id, CTSTR& text, int edt_w, WORD edt_control_id, bool dropdown_flag);
	// �z�u�p�����[�^�ŁF���X�g�r���[�ǉ��i���|�[�g�^�j
	void add_ap_lstv(int ap_mod, int w, int h, WORD control_id);
};


