#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_pct.h"

#define		__DR	1	// ��`�̎Z�o�� disp_rect ���g���X�C�b�`

namespace NT3
{

// ****************************************************************
// �R���X�g���N�^
// ================================================================
C_pct::C_pct()
{
	width = 0;
	height = 0;
}

// ****************************************************************
// �f�X�g���N�^
// ================================================================
C_pct::~C_pct()
{
}

// ****************************************************************
// �j��
// ================================================================
void C_pct::destroy()
{
	chip_list.clear();

	width = 0;
	height = 0;
	center = C_point(0, 0);
	disp_rect = C_rect(0, 0, 0, 0);
}

// ****************************************************************
// �s�N�`�����쐬
// ================================================================
bool C_pct::create(ARRAY< BSP<C_dib_chip> > chip_list_, int width_, int height_, C_point center_, C_rect disp_rect_)
{
	destroy();

	chip_list = chip_list_;
	width = width_;
	height = height_;
	center = center_;
	disp_rect = disp_rect_;

	return true;
}

// ****************************************************************
// ������ǂݍ���
// ================================================================
bool C_pct::load_moji(TCHAR moji, C_argb color, LOGFONT* lf, C_text_format* tf)
{
	MAT2 mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };

	return load_moji(moji, color, lf, tf, mat);
}

bool C_pct::load_moji(TCHAR moji, C_argb color, LOGFONT* lf, C_text_format* tf, MAT2 mat)
{
	destroy();

	BSP<C_dib_chip> chip(new C_dib_chip);

	// �`�b�v�ɕ�����ǂݍ���
	int pos_x, pos_y;
	if (!chip->load_moji(moji, color, lf, &pos_x, &pos_y, mat))
		return false;

	// �`�b�v���X�g�ɒǉ�
	chip->x = pos_x;
	chip->y = tf->height - pos_y;
	chip->sprite = true;
	chip_list.push_back(chip);

	// �`���`�̐ݒ�
	width = tf->width;
	height = tf->height;
	disp_rect = C_rect(chip->x, chip->y, chip->x + chip->get_width(), chip->y + chip->get_height());

	return true;
}

// ****************************************************************
// bmp ��ǂݍ���
// ================================================================
bool C_pct::load_bmp(CTSTR& file_path)
{
	destroy();

	// �`�b�v�� bmp ��ǂݍ���
	BSP<C_dib_chip> chip(new C_dib_chip);
	if (!chip->load_bmp(file_path))
		return false;

	// �`�b�v�̐ݒ�
	chip->sprite = true;		// bmp �͓��߂Ȃ��I
	chip_list.push_back(chip);	// �`�b�v���X�g�ɒǉ�

	// �`���`�̐ݒ�
	width = chip->get_width();
	height = chip->get_height();
	disp_rect = C_rect(0, 0, width, height);

	return true;
}

// ****************************************************************
// png ��ǂݍ���
// ================================================================
bool C_pct::load_png(CTSTR& file_path)
{
	destroy();

	// �`�b�v�� png ��ǂݍ���
	BSP<C_dib_chip> chip(new C_dib_chip);
	if (!chip->load_png(file_path))
		return false;

	// �`�b�v�̐ݒ�
	chip->sprite = true;		// png �͓��߂���I
	chip_list.push_back(chip);	// �`�b�v���X�g�ɒǉ�

	// �`���`�̐ݒ�
	width = chip->get_width();
	height = chip->get_height();
	disp_rect = C_rect(0, 0, width, height);

	return true;
}

// ****************************************************************
// jpg ��ǂݍ���
// ================================================================
/*
bool C_pct::load_jpg(CTSTR& file_path)
{
	destroy();

	// �`�b�v�� jpg ��ǂݍ���
	BSP<C_dib_chip> chip(new C_dib_chip);
	if (!chip->load_jpg(file_path))
		return false;

	// �`�b�v�̐ݒ�
	chip->sprite = true;		// jpg �͓��߂Ȃ��I
	chip_list.push_back(chip);	// �`�b�v���X�g�ɒǉ�

	// �`���`�̐ݒ�
	width = chip->get_width();
	height = chip->get_height();
	disp_rect = C_rect(0, 0, width, height);

	return true;
}
*/

// ****************************************************************
// �h���N�G�R�����X�^�[�b�f��ǂݍ���
// ================================================================
#if 0

bool C_pct::load_DQ3_monster(BYTE* rom_data, int monster_id)
{
	// �N���A
	destroy();

	// �O���t�B�b�N�G���g��
	BYTE* ad_entry = (BYTE *)(rom_data + 0x08ED3 + monster_id * 5);

	// �O���t�B�b�N�G���g��������擾
	int block_cnt = *ad_entry;
	BYTE* ad_gra = (BYTE *)(rom_data + *(WORD *)(ad_entry + 1)) + 16;
	BYTE* ad_pal = (BYTE *)(rom_data + *(WORD *)(ad_entry + 3)) + 16;

	// �p���b�g�n�b�V���e�[�u���쐬
	BYTE pal[256] = {0};
	// ���r�b�g�擾
	int pal_info = *ad_pal++;
	for (int i = 0; i < 2; i++)	{
		int pal_dst_pos, pal_dst_cnt;
		if (i == 0)	{	pal_dst_pos = 4;	pal_dst_cnt = pal_info & 0x0f;	}
		else		{	pal_dst_pos = 1;	pal_dst_cnt = pal_info >> 4;	}
		Cpalette::get_instance().make_FC_palette(ad_pal, pal, pal_dst_pos, pal_dst_cnt);
		ad_pal += 3 * pal_dst_cnt;
	}

	// �O���t�B�b�N�A�h���X����
	int ad_gra_hosei = ( ( *(rom_data + 0x08EC1 + monster_id / 8) ) >> (7 - monster_id % 8) ) & 1;
	ad_gra += ad_gra_hosei * 0x4000;

	// �`�b�v�̍ŏ���`
//	disp.draw_rect.left = 99999;
//	disp.draw_rect.top = 99999;
//	disp.draw_rect.right = 0;
//	disp.draw_rect.bottom = 0;
	width = 0;
	height = 0;

	// �O���t�B�b�N�\����
	struct GRA_HEADER	{
		BYTE	data[3];
		int		block;
	};
	struct GRA_BLOCK	{
		BYTE	data[16];
	};

	// �W�J�f�[�^
	ARRAY<GRA_HEADER>	header_list;
	ARRAY<GRA_BLOCK>	block_list;

	// �O���t�B�b�N�f�[�^�̍\�z
	for (int b = 0; b < block_cnt; )	{

		GRA_HEADER header;
		GRA_BLOCK block;

		// �w�b�_�̍\�z
		header.data[0] = ad_gra[0];
		header.data[1] = ad_gra[1];
		header.data[2] = ad_gra[2];
		header.block = b;
		header_list.push_back(header);

		if (header.data[0] & 0x40)	ad_gra += 2;
		else						ad_gra += 3;

		// �O���t�B�b�N�f�[�^����̏ꍇ
		if (header.data[0] & 0x80)	{

			BYTE c_data = 0;	// ���ʃf�[�^
			WORD e_data = 0;	// �⑫�f�[�^

			// ���ʃf�[�^�ƕ⑫�f�[�^�𓾂�
			if (header.data[0] & 0x08)	{
				if (header.data[0] & 0x01)	c_data = *ad_gra++;
				else						c_data = 0;
				e_data = *(WORD *)(ad_gra);
				ad_gra += 2;
			}
			else	{
				c_data = 0;
				e_data = 0xFFFF;
			}

			// �f�[�^�W�J
			for (int cnt=0; cnt<16; cnt++)	{
				if (e_data & 0x8000)	block.data[cnt] = *ad_gra++;
				else					block.data[cnt] = c_data;
				e_data <<= 1;
			}

			block_list.push_back(block);
			b++;
		}
	}

	// �`�b�v�̍\�z
	for (int h = 0; h < (int)header_list.size(); h++)	{

		GRA_HEADER header = header_list[h];
		GRA_BLOCK block = block_list[header.block];

		bool rev_x = (header.data[0] & 0x02) > 0;
		bool rev_y = (header.data[0] & 0x04) > 0;

		// �p���b�g�ǉ����
		int pal_pos = (header.data[0] & 0x70 ^ 0x40) >> 4;
		if (header.data[0] & 0x40)
			pal_pos += 1;

		// �`�b�v�̓ǂݍ���
		BSP<C_dib_chip> chip(new C_dib_chip);
		chip->load_fc_chip(block.data, pal, pal_pos, rev_x, rev_y);

		// �ǉ����̃Z�b�g
		if (header.data[0] & 0x40)	{	// �����Ȃ�
			chip->x = ( header.data[1] & 0x0f ) * 8;
			chip->y = ( (header.data[1] >> 4) + ((header.data[1] >> 4) & 0x08) * (-2) ) * 8 + 0x10;
			chip->beta = true;
		}
		else	{						// ��������
			chip->x = ( header.data[1] & 0x80 ) * (-2) + header.data[1];
			chip->y = ( header.data[2] & 0x80 ) * (-2) + header.data[2] + 0x10;
			chip->beta = false;
		}

		// �`�b�v�̈ʒu�����`���v�Z
//		disp.draw_rect.left		= min( disp.draw_rect.left,   chip->X() );
//		disp.draw_rect.top		= min( disp.draw_rect.top,    chip->Y() );
//		disp.draw_rect.right	= max( disp.draw_rect.right,  chip->X()+15 );
//		disp.draw_rect.bottom	= max( disp.draw_rect.bottom, chip->Y()+15 );
		width  = max(width,  chip->x + chip->get_width());
		height = max(height, chip->y + chip->get_height());

		// �`�b�v�����X�g�ɒǉ�
		chip_list.push_back(chip);
	}

	return true;
	
}
#endif

// ****************************************************************
// �h���N�G�R�L�����N�^�[�`�b�v�b�f
// ================================================================
#if 0

bool C_pct::load_DQ3_chara_chip(BYTE* rom_data, int chara_id, int chara_way, int step_pat)
{
	// �N���A
	destroy();

	// �p���b�g�n�b�V���e�[�u��
	BYTE pal[256];
	BYTE* ad_clr = rom_data + 0x1E562;
	Cpalette::get_instance().make_FC_palette(ad_clr, pal, 0, 64);
	pal[1] = 0x30;

	// �e����
	int pct_no = chara_id * 8 + chara_way * 2 + step_pat;
	BYTE* ad_entry   = rom_data + 0x16DE8 + pct_no * 3;
	BYTE* ad_shutoku = rom_data + 0x1744F;
	BYTE* ad_gra     = rom_data + 0x20010;

	// �e����
	int a = *ad_entry;			// ��񂻂̂P
	int b = *(ad_entry + 1);	// ��񂻂̂Q
	int c = *(ad_entry + 2);	// ��񂻂̂R

	int a_low = a & 0x0f;
	int a_high = a >> 4;

	// �p���b�g�I�t�Z�b�g
	int pal_pos = (a_low >> 2) + 4;

	// �A�h���X�␳
	b += (a_low & 0x03) << 8;

	if (a_high < 0x0f)	{

		BYTE* gra_data;		// �O���t�B�b�N�f�[�^
		BYTE* org_gra_data;	// �O���t�B�b�N�f�[�^�擪

		org_gra_data = (BYTE *)(ad_gra + (b << 4));
		gra_data = org_gra_data;
		int d = 0;

		for (int i=0; i<4; i++)	{

			// ���]���
			bool rev_x = ((c >> (6 - 2 * i)) & 0x01) > 0;
			bool rev_y = ((c >> (7 - 2 * i)) & 0x01) > 0;

			// �`�b�v�̓ǂݍ���
			BSP<C_dib_chip> chip(new C_dib_chip);
			chip->load_fc_chip(gra_data, pal, pal_pos, rev_x, rev_y);
			chip->x = i % 2 * 8;
			chip->y = i / 2 * 8;
			chip->beta = false;
			// �`�b�v�����X�g�ɒǉ�
			chip_list.push_back(chip);
			// �A�h���X�␳
			d += *(ad_shutoku + a_high * 3 + i);
			gra_data = (BYTE *)(((d & 0x80) * (-2) + d) * 0x10 + org_gra_data);
		}
	}
	else	{
		b = b * 5 + 0x172B0;
		int d = *(rom_data + b);

		for (int i = 0; i < 4; i++)	{

			BYTE* gra_data;

			// ���]���
			bool rev_x = ((c >> (6 - 2 * i)) & 0x01) > 0;
			bool rev_y = ((c >> (7 - 2 * i)) & 0x01) > 0;

			// �A�h���X�␳
			b++;
			int e = *(rom_data + b);
			gra_data = (BYTE *)(((d << (8 - i * 2)) & 0x300 + e) * 0x10 + ad_gra);
			// �`�b�v�̓ǂݍ���
			BSP<C_dib_chip> chip(new C_dib_chip);
			chip->load_fc_chip(gra_data, pal, pal_pos, rev_x, rev_y);
			chip->x = i % 2 * 8;
			chip->y = i / 2 * 8;
			chip->beta = false;
			// �`�b�v�����X�g�ɒǉ�
			chip_list.push_back(chip);
		}
	}

	return true;
}
#endif

// ****************************************************************
// �S�`�b�v���x�^�ɓW�J����
// ================================================================
BSP<C_dib_chip> C_pct::expand_chips()
{
	int chip_cnt = (int)chip_list.size();

	// �P��`�b�v���쐬
	BSP<C_dib_chip> ex_chip(new C_dib_chip);
	if (!ex_chip->create(width, height, 32))
		return BSP<C_dib_chip>();

	// �P��`�b�v�𓧖��ɓh��Ԃ�
	fill_memory_4(ex_chip->get_ptr(), width * height, 0);
	ex_chip->sprite = true;

	// �S�`�b�v��P��`�b�v�ɕ`��
	for (int c = 0; c < chip_cnt; c++)	{
		BSP<C_dib_chip> chip = chip_list[c];
		ex_chip->draw_dib_easy(chip.get(), chip->x, chip->y);
	}

	// �`�b�v���X�g��j�����A�P��`�b�v��V���ɒǉ�
	chip_list.clear();
	chip_list.push_back(ex_chip);

	return ex_chip;
}

// ****************************************************************
// �g��
// ================================================================
bool C_pct::magnify(int scale)
{
	CHIPLIST old_chip_list = chip_list;
	chip_list.clear();

	CHIPLIST::iterator itr;
	for (itr = old_chip_list.begin(); itr != old_chip_list.end(); ++itr)	{

		BSP<C_dib_chip> old_chip = *itr;
		BSP<C_dib_chip> new_chip(new C_dib_chip);

		new_chip->create_magnified_dib_scale(old_chip.get(), scale, scale);
		new_chip->x = old_chip->x * scale;
		new_chip->y = old_chip->y * scale;
		new_chip->sprite = old_chip->sprite;

		chip_list.push_back(new_chip);
	}

	return true;
}

// ****************************************************************
// �E�ɂX�O�x��]
// ================================================================
bool C_pct::rotate_90()
{
	CHIPLIST old_chip_list = chip_list;
	chip_list.clear();

	CHIPLIST::iterator itr;
	for (itr = old_chip_list.begin(); itr != old_chip_list.end(); ++itr)	{

		BSP<C_dib_chip> old_chip = *itr;
		BSP<C_dib_chip> new_chip(new C_dib_chip);

		new_chip->create_rotate_90_dib(old_chip.get());
		new_chip->x = height - old_chip->y - old_chip->get_height();
		new_chip->y = old_chip->x;
		new_chip->sprite = old_chip->sprite;

		chip_list.push_back(new_chip);
	}

	return true;
}

// ****************************************************************
// dib �ɕ`��
// ================================================================
void C_pct::draw_to_dib(C_dib* dib, int x, int y, bool sprite)
{
	S_disp_param dp;
	dp.pos.x = x;
	dp.pos.y = y;
	dp.sprite = sprite;
	draw_to_dib(dib, &dp);
}

void C_pct::draw_to_dib(C_dib* dib, S_disp_param* dp)
{
	draw_to_dib(dib, dp, true, NULL);
}

void C_pct::draw_to_dib(C_dib* dib, S_disp_param* dp_, bool draw_flag, ARRAY<C_rect>* ret_draw_rect_list)
{
	// �`�F�b�N���[�`���̏ꍇ�͓��ꏈ��
	// �e�`�b�v���Ƃł͂Ȃ��Adisp_rect �����`���Z�o����

#if __DR
	if (!draw_flag)	{
		S_disp_param dp = *dp_;
		dp.center += center;	// ���g�̒��S���W
		dp.plane.type = E_disp_plane_type_box_fill;
		dp.plane.color = C_argb(255, 255, 255, 255);
		dp.plane.rect = disp_rect;

		bool ret_draw_rect_flag = false;
		C_rect ret_draw_rect(0, 0, 0, 0);
		dib->draw_rect(&dp, draw_flag, &ret_draw_rect_flag, &ret_draw_rect);
		// �\����`������Ȃ�ǉ�
		if (ret_draw_rect_list && ret_draw_rect_flag)
			ret_draw_rect_list->push_back(ret_draw_rect);
	}
	// �`�惋�[�`���̏ꍇ�̓`�b�v���Ƃɏ���
	else	{
#endif

		// �S�`�b�v�����ɕ`�悷��
		for (CHIPLIST::iterator itr = chip_list.begin(); itr != chip_list.end(); ++itr)	{
			BSP<C_dib_chip> chip = *itr;
			S_disp_param dp = *dp_;

			// ���S���W�̏C��
			dp.center += center;	// ���g�̒��S���W
			dp.center.x -= chip->x;	// �`�b�v�␳
			dp.center.y -= chip->y;	// �`�b�v�␳
				// ����]��g�k�̒��S�� dp �ɕʓr�p�ӂ���K�v������H

			// �x�^�`�b�v�̏ꍇ�A���߂��Ȃ�
			if (!chip->sprite)	dp.sprite = false;

			// �`�b�v��`��
			bool ret_draw_rect_flag = false;
			C_rect ret_draw_rect(0, 0, 0, 0);
			dib->draw_dib(chip.get(), &dp, draw_flag, &ret_draw_rect_flag, &ret_draw_rect);
			// �\����`������Ȃ�ǉ�
			if (ret_draw_rect_list && ret_draw_rect_flag)
				ret_draw_rect_list->push_back(ret_draw_rect);
		}
#if __DR
	}
#endif
}



}
