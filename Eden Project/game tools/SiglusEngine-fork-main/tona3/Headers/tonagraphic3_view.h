#pragma		once

#include	"tonagraphic3_dib.h"
#include	"tonagraphic3_sprite.h"

namespace NT3
{

// ****************************************************************
// �r���[
// ================================================================
class C_view : public C_dib
{
public:
	struct OPTION
	{
		S_sprite_param	sp;			// �X�v���C�g�p�����[�^
		ARRAY<C_rect>	dr_list;	// �`���`

		bool operator == (const OPTION& rhs)	{	return sp == rhs.sp;	}	// sp �������r�I
		bool operator != (const OPTION& rhs)	{	return sp != rhs.sp;	}	// sp �������r�I
	};

public:
	C_view();
	~C_view();

	bool		create(int width, int height, int depth);
	void		destroy();
	void		clear_sprite_list();						// �X�v���C�g���X�g�̃N���A
	void		regist_sprite(C_sprite* sprite);			// �X�v���C�g��o�^

	// ��ʍX�V�֌W
	void			begin_scene();					// �V�[���J�n
	void			update_redraw_block_table();	// �ĕ`��u���b�N�e�[�u�����X�V
	void			update_redraw_rect_list();		// �ĕ`���`���X�g���X�V
	void			draw_sprite_list();				// �`��
	ARRAY<C_rect>&	get_redraw_rect_list()	{	return redraw_rect_list;	}
	void			set_redraw_block_full();

private:
	typedef	std::map<C_sprite *, OPTION>		SPRITEMAP;	// �X�v���C�g�}�b�v
	typedef	std::pair<C_sprite *, OPTION>	SPRITEPAIR;	// �X�v���C�g�y�A
	SPRITEMAP	old_map;		// �O��`�掞�̃}�b�v
	SPRITEMAP	new_map;		// ����`�掞�̃}�b�v

	typedef ARRAY<C_sprite *>	SPRITELIST;
	SPRITELIST	sprite_list;	// �X�v���C�g���X�g
	
	typedef ARRAY<C_rect>		RECTLIST;
	RECTLIST	redraw_rect_list;	// �ĕ`���`���X�g

	typedef BYTE		BLOCKTYPE;

	int					redraw_block_cnt_x;
	int					redraw_block_cnt_y;
	ARRAY<BLOCKTYPE>	redraw_block_table;

	BLOCKTYPE*	get_redraw_block_ptr(int x, int y)	{	return redraw_block_table.get() + (y * redraw_block_cnt_x) + x;	}
	void		create_redraw_block_table();															// �ĕ`��u���b�N�e�[�u���̍\�z
	void		clear_redraw_block_table();																// �ĕ`��u���b�N�e�[�u�����N���A�i�S�́j
	void		clear_redraw_block(int block_l, int block_t, int block_r, int block_b);					// �ĕ`��u���b�N�e�[�u�����N���A�i�͈͎w��j
	void		set_redraw_block(int block_l, int block_t, int block_r, int block_b);					// �ĕ`��u���b�N���Z�b�g

	void		update_redraw_block_table(C_sprite* sprite);			// �ĕ`��u���b�N�e�[�u�����X�V�i�X�v���C�g����j
	void		update_redraw_block_table(ARRAY<C_rect>& rect_list);	// �ĕ`��u���b�N�e�[�u�����X�V�i��`���X�g����j
	void		update_redraw_block_table(C_rect rect);					// �ĕ`��u���b�N�e�[�u�����X�V�i��`����j
	void		magnify_redraw_block(int l, int t, int* w, int* h);		// �ĕ`��u���b�N���L����
	void		draw_sprite(C_sprite* sprite, C_rect* rect = NULL);		// �X�v���C�g��`��

};

}
