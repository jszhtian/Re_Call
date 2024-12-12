#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_view.h"

const int 	REDRAW_BLOCK_W = 16;
const int	REDRAW_BLOCK_H = 16;

namespace NT3
{

// ****************************************************************
// �r���[
// ================================================================
C_view::C_view()
{
	redraw_block_cnt_x = 0;
	redraw_block_cnt_y = 0;
}

C_view::~C_view()
{
}

// ****************************************************************
// �r���[�̍쐬
// ================================================================
bool C_view::create(int width, int height, int depth)
{
	destroy();

	if (!C_dib::create(width, height, depth))
		return false;

	// �ĕ`��u���b�N�e�[�u���̍\�z
	create_redraw_block_table();

	return true;
}

// ****************************************************************
// �r���[�̊J��
// ================================================================
void C_view::destroy()
{
	redraw_block_cnt_x = 0;
	redraw_block_cnt_y = 0;

	sprite_list.clear();
	redraw_rect_list.clear();
	redraw_block_table.clear();
	old_map.clear();
	new_map.clear();

	C_dib::destroy();
}

// ****************************************************************
// �ĕ`��u���b�N�e�[�u���̍\�z
// ================================================================
void C_view::create_redraw_block_table()
{
	// �ĕ`��u���b�N�̌����v�Z
	redraw_block_cnt_x = (get_width() - 1) / REDRAW_BLOCK_W + 1;
	redraw_block_cnt_y = (get_height() - 1) / REDRAW_BLOCK_H + 1;

	// �ĕ`��u���b�N�e�[�u���̍\�z
	redraw_block_table.resize(redraw_block_cnt_x * redraw_block_cnt_y);

	// �ĕ`��u���b�N�e�[�u���̃N���A
	clear_redraw_block_table();
}

// ****************************************************************
// �ĕ`��u���b�N�e�[�u���̃N���A
// ================================================================
void C_view::clear_redraw_block_table()
{
	ZeroMemory(redraw_block_table.get(), redraw_block_cnt_x * redraw_block_cnt_y * sizeof(BLOCKTYPE));
}

void C_view::set_redraw_block_full()
{
	fill_memory(redraw_block_table.get(), redraw_block_cnt_x * redraw_block_cnt_y * sizeof(BLOCKTYPE), -1);
}

void C_view::clear_redraw_block(int block_l, int block_t, int block_r, int block_b)
{
	block_l = std::max(block_l, 0);
	block_t = std::max(block_t, 0);
	block_r = std::min(block_r, redraw_block_cnt_x);
	block_b = std::min(block_b, redraw_block_cnt_y);

	if (block_l >= block_r)		return;
	if (block_t >= block_b)		return;

	// false �� 0 �ł��邱�Ƃ�����ɉ��肵�Ă܂�
	for (int y = block_t; y < block_b; y++)	{
		BLOCKTYPE* block_ptr = get_redraw_block_ptr(block_l, y);
		ZeroMemory(block_ptr, (block_r - block_l) * sizeof(BLOCKTYPE));
	}
}

void C_view::set_redraw_block(int block_l, int block_t, int block_r, int block_b)
{
	block_l = std::max(block_l, 0);
	block_t = std::max(block_t, 0);
	block_r = std::min(block_r, redraw_block_cnt_x);
	block_b = std::min(block_b, redraw_block_cnt_y);

	if (block_l >= block_r)		return;
	if (block_t >= block_b)		return;

	// true �� 1 �ł��邱�Ƃ�����ɉ��肵�Ă܂�
	for (int y = block_t; y < block_b; y++)	{
		BLOCKTYPE* block_ptr = get_redraw_block_ptr(block_l, y);
		fill_memory(block_ptr, (block_r - block_l) * sizeof(BLOCKTYPE), -1);
	}
}

// ****************************************************************
// �X�v���C�g���X�g�����
// ================================================================
void C_view::clear_sprite_list()
{
	sprite_list.clear();
}

// ****************************************************************
// �X�v���C�g�����X�g�ɓo�^
// ================================================================
void C_view::regist_sprite(C_sprite* sprite)
{
	sprite_list.push_back(sprite);
}

// ****************************************************************
// �V�[���̊J�n
// ================================================================
void C_view::begin_scene()
{
	// �}�b�v�̓���ւ�
	old_map = SPRITEMAP(new_map.begin(), new_map.end());
	new_map.clear();

	// �ĕ`��u���b�N�̃N���A
	clear_redraw_block_table();
	redraw_rect_list.clear();
}

// ****************************************************************
// �ĕ`���`���X�V
// ================================================================
void C_view::update_redraw_block_table()
{
	// �ĕ`��u���b�N�e�[�u�����X�V
	for (SPRITELIST::iterator itr = sprite_list.begin(); itr != sprite_list.end(); ++itr)
		update_redraw_block_table(*itr);

	// �c���Ă���O��̕\����`��ǉ�
	for (SPRITEMAP::iterator itr = old_map.begin(); itr != old_map.end(); ++itr)
		update_redraw_block_table(itr->second.dr_list);
}

// ****************************************************************
// �ĕ`��u���b�N�}�b�v���X�V
// ================================================================
void C_view::update_redraw_block_table(C_sprite* sprite)
{
	// �\���p�����[�^���v�Z
	OPTION opt;
	sprite->get_sp(&opt.sp);

	// �\������ꍇ
	if (opt.sp.disp && opt.sp.alpha > 0)	{

		// �O��̃��X�g����X�v���C�g��T��
		SPRITEMAP::iterator last = old_map.find(sprite);
		// ���������ꍇ
		if (last != old_map.end())	{
			// �\���p�����[�^�ɕύX���������ꍇ
			if (opt.sp != last->second.sp)	{
				// �`���`�����߂�
				sprite->check_disp_rect(this, opt.dr_list);
				// �ĕ`��u���b�N�̍X�V
				update_redraw_block_table(last->second.dr_list);
				update_redraw_block_table(opt.dr_list);
			}
			// �\���p�����[�^�ɕύX���Ȃ������ꍇ
			else	{
				// �`���`��O�񂩂�R�s�[
				opt.dr_list = last->second.dr_list;
			}
			// �O��̃��X�g����폜
			old_map.erase(last);
		}
		// ������Ȃ������ꍇ
		else	{
			// �`���`�����߂�
			sprite->check_disp_rect(this, opt.dr_list);
			// �ĕ`��u���b�N�̍X�V
			update_redraw_block_table(opt.dr_list);
		}
		// �\�����X�g�ɉ�����
		SPRITEPAIR pair(sprite, opt);
		new_map.insert(pair);
	}
}

// ****************************************************************
// �ĕ`��u���b�N�}�b�v�X�V�i��`���X�g����j
// ================================================================
void C_view::update_redraw_block_table(ARRAY<C_rect>& rect_list)
{
	for (ARRAY<C_rect>::iterator itr = rect_list.begin(); itr != rect_list.end(); ++itr)	{
		update_redraw_block_table(*itr);
	}
}

// ****************************************************************
// �ĕ`��u���b�N�}�b�v�X�V�i��`����j
// ================================================================
void C_view::update_redraw_block_table(C_rect rect)
{
	if (rect.width() <= 0 || rect.height() <= 0)
		return;

	// ��`����A�ĕ`��u���b�N�̈ʒu���v�Z
	int left = rect.left / REDRAW_BLOCK_W;
	int top = rect.top / REDRAW_BLOCK_H;
	int right = (rect.right - 1) / REDRAW_BLOCK_W + 1;
	int bottom = (rect.bottom - 1) / REDRAW_BLOCK_H + 1;

	// �ĕ`��u���b�N�X�V
	set_redraw_block(left, top, right, bottom);
}

// ****************************************************************
// �ĕ`���`���X�V
// ================================================================
void C_view::update_redraw_rect_list()
{
	// �ĕ`�悷��ׂ���`�̕`��
	for (int block_l = 0; block_l < redraw_block_cnt_x; block_l++)	{
		BLOCKTYPE* block_ptr = get_redraw_block_ptr(block_l, 0);
		for (int block_t = 0; block_t < redraw_block_cnt_y; block_t++, block_ptr += redraw_block_cnt_x)	{
			if (*block_ptr)	{
				*block_ptr = false;

				// �ĕ`���`���L����
				int block_r, block_b;
				magnify_redraw_block(block_l, block_t, &block_r, &block_b);
				clear_redraw_block(block_l, block_t, block_r, block_b);

				// �ĕ`���`�̒ǉ�
				C_rect redraw_rect(block_l * REDRAW_BLOCK_W, block_t * REDRAW_BLOCK_H, block_r * REDRAW_BLOCK_W, block_b * REDRAW_BLOCK_H);
				redraw_rect_list.push_back(redraw_rect);
			}
		}
	}
}

// ****************************************************************
// �X�v���C�g���X�g��`��
// ================================================================
void C_view::draw_sprite_list()
{
	// �S��`�ɑ΂��đS�X�v���C�g��`��
	for (RECTLIST::iterator rect_itr = redraw_rect_list.begin(); rect_itr != redraw_rect_list.end(); ++rect_itr)	{
		for (SPRITELIST::iterator sprite_itr = sprite_list.begin(); sprite_itr != sprite_list.end(); ++sprite_itr)
			draw_sprite(*sprite_itr, &*rect_itr);
	}
}

// ****************************************************************
// �X�v���C�g�̕`��
// ================================================================
void C_view::draw_sprite(C_sprite* sprite, C_rect* rect)
{
	sprite->draw_to_dib(this, rect);
}

// ****************************************************************
// �ĕ`��̈�̍��ォ��E�������߂�
// ================================================================
void C_view::magnify_redraw_block(int left, int top, int* right_, int* bottom_)
{
	int right, bottom;

	// ���ɍĕ`���`��L�΂�
	BLOCKTYPE* block_p = get_redraw_block_ptr(left, top + 1);
	for (bottom = top + 1; bottom < redraw_block_cnt_y; bottom++, block_p += redraw_block_cnt_x)	{
		if (!*block_p)		// �����ĕ`��u���b�N�̏ꍇ
			break;			// ���[�m��
	}

	// �E�ɍĕ`���`��L�΂�
	for (right = left + 1; right < redraw_block_cnt_x; right++)	{

		// ��[���牺�[�ɂ����āA
		// �E�̗񂪑S�čĕ`��̈�ł��邩�ǂ����𒲂ׂ�
		bool inc = true;
		BLOCKTYPE* block_p = get_redraw_block_ptr(right, top);
		for (int y = top; y < bottom; y++, block_p += redraw_block_cnt_x)	{
			if (!*block_p)	{
				inc = false;
				break;
			}
		}
		if (!inc)		// �E�ɍĕ`��u���b�N�łȂ����̂�������
			break;		// �E�[�m��
	}

	*right_ = right;
	*bottom_ = bottom;
}
};
