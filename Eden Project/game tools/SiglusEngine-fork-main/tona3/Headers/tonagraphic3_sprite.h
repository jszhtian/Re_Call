#pragma		once

#include	"tonagraphic3_pct.h"
#include	"tonagraphic3_album.h"

namespace NT3
{

// ****************************************************************
// �X�v���C�g�^�C�v
// ================================================================
enum SPRITETYPE
{
	SPRITETYPE_NONE,		// �Ȃ�
	SPRITETYPE_PLANE,		// �v���[��
	SPRITETYPE_DIB,			// �f�B�u
	SPRITETYPE_PCT,			// �s�N�`��
	SPRITETYPE_ALBUM,		// �A���o��
};

const TSTR StrSPRITETYPE[] = {
	_T("NONE"), 
	_T("PLANE"), 
	_T("DIB"), 
	_T("PCT"), 
	_T("ALBUM"), 
};

// ****************************************************************
// �X�v���C�g�p�����[�^
// ================================================================
struct S_sprite_param : public S_disp_param
{
	S_sprite_param();
	void	init();

	SPRITETYPE	type;		// �X�v���C�g�^�C�v
	int			change;		// �ύX�t���O
	bool		disp;		// �`��t���O
	int			layer;		// ���C���[
	int			order;		// �I�[�_�[
	int			depth;		// �[�x
	int			pct_no;		// �s�N�`���ԍ�

	bool operator == (const S_sprite_param& rhs)	{ return pod_equal(this, &rhs); }
	bool operator != (const S_sprite_param& rhs)	{ return !pod_equal(this, &rhs); }
};

// ****************************************************************
// �X�v���C�g
// ================================================================
class C_sprite
{
public:
	C_sprite();
	~C_sprite()	{}

	void		init();									// ������
	bool		create_plane();							// �v���[���̍쐬
	bool		create_dib(BSP<C_dib> dib);				// �f�B�u�̍쐬
	bool		create_pct(BSP<C_pct> pct);				// �s�N�`���̍쐬
	bool		create_album(BSP<C_album> album);		// �A���o���̍쐬

	bool		is_ready();								// �f�[�^����������Ă��邩�𔻒�i�f�B�u�A�s�N�`���A�A���o���j
	C_rect		get_rect(int pct_no = -1);				// �S�̋�`�̎擾�i�����蔻��ȂǂɎg�p����j�ipct_no = -1: ���݂̃s�N�`���i�A���o����p�j�j
	void		get_sp(S_sprite_param* sp);				// �\���p�����[�^�̎擾�i�r���[�����Ŏg�p�j
	void		check_disp_rect(C_dib* dib, ARRAY<C_rect>& ret_draw_rect_list);	// �`���`�̎擾�i�r���[�����Ŏg�p�j

	void		draw_to_dib(C_dib* dib, C_rect* clip = NULL);	// dib �ɕ`��

	// ���ʃp�����[�^
	S_sprite_param	sp;		// �X�v���C�g�p�����[�^

	// �u�f�B�u�v�p�����[�^
	BSP<C_dib>		dib;		// �f�B�u
	// �u�s�N�`���v�p�����[�^
	BSP<C_pct>		pct;		// �s�N�`��
	// �u�A���o���v�p�����[�^
	BSP<C_album>	album;		// �A���o��

	// ���ёւ��p�̔�r���Z�q
	bool		operator < (C_sprite& rhs);

private:
	void		reset_type();
};



};
