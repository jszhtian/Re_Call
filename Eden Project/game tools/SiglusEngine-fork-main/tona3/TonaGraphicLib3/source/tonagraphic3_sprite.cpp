#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_sprite.h"

namespace NT3
{

// ****************************************************************
// �X�v���C�g�p�����[�^
// ================================================================
S_sprite_param::S_sprite_param()
{
	// �����Ԃ��[���Ŗ��߂�imemcmp �ł̔�r���\�ɂ��邽�߁j
	ZeroMemory(this, sizeof(S_sprite_param));

	// �ʏ�̏�����
	init();
}

void S_sprite_param::init()
{
	disp = false;
	layer = 0;
	order = 0;
	depth = 0;
	pct_no = 0;

	S_disp_param::init();
};

// ****************************************************************
// �X�v���C�g
// ================================================================
C_sprite::C_sprite()
{
	init();
}

// ****************************************************************
// ������
// ================================================================
void C_sprite::init()
{
	reset_type();
	sp.init();
}

// ****************************************************************
// �^�C�v��������
// ================================================================
void C_sprite::reset_type()
{
	// ���ʃp�����[�^
	sp.type = SPRITETYPE_NONE;
	sp.change ++;

	// �u�f�B�u�v�p�����[�^
	dib.reset();
	// �u�s�N�`���v�p�����[�^
	pct.reset();
	// �u�A���o���v�p�����[�^
	album.reset();
}

// ****************************************************************
// ��r���Z�q
// ================================================================
bool C_sprite::operator < (C_sprite& rhs)
{
	return sp.layer < rhs.sp.layer
		|| (sp.layer == rhs.sp.layer && sp.order < rhs.sp.order)
		|| (sp.layer == rhs.sp.layer && sp.order == rhs.sp.order && sp.depth < rhs.sp.depth)
		|| (sp.layer == rhs.sp.layer && sp.order == rhs.sp.order && sp.depth == rhs.sp.depth && sp.pos.y < rhs.sp.pos.y);
}

// ****************************************************************
// �u�v���[���v�쐬
// ================================================================
bool C_sprite::create_plane()
{
	reset_type();
	sp.type = SPRITETYPE_PLANE;

	return true;
}

// ****************************************************************
// �u�f�B�u�v�쐬
// ================================================================
bool C_sprite::create_dib(BSP<C_dib> dib_)
{
	reset_type();
	sp.type = SPRITETYPE_DIB;

	dib = dib_;

	return true;
}

// ****************************************************************
// �u�s�N�`���v�쐬
// ================================================================
bool C_sprite::create_pct(BSP<C_pct> pct_)
{
	reset_type();
	sp.type = SPRITETYPE_PCT;

	pct = pct_;

	return true;
}

// ****************************************************************
// �u�A���o���v�쐬
// ================================================================
bool C_sprite::create_album(BSP<C_album> album_)
{
	reset_type();
	sp.type = SPRITETYPE_ALBUM;

	album = album_;

	return true;
}

// ****************************************************************
// �f�[�^����������Ă��邩�𔻒�
// ================================================================
bool C_sprite::is_ready()
{
	if (false);

	// �v���[��
	else if (sp.type == SPRITETYPE_PLANE)	{
		return true;
	}
	// �f�B�u
	else if (sp.type == SPRITETYPE_DIB)	{
		if (dib)
			return true;
	}
	// �s�N�`��
	else if (sp.type == SPRITETYPE_PCT)	{
		if (pct)
			return true;
	}
	// �A���o��
	else if (sp.type == SPRITETYPE_ALBUM)	{
		if (album)
			return true;
	}

	return false;
}

// ****************************************************************
// �X�v���C�g�p�����[�^�̎擾
// ================================================================
void C_sprite::get_sp(S_sprite_param* sp_)
{
	*sp_ = sp;
}

// ****************************************************************
// �\����`�̌v�Z
//		�`�F�b�N���[�h�ŕ`����s���A�`�悵����`���擾���܂��B
// ================================================================
void C_sprite::check_disp_rect(C_dib* dst, ARRAY<C_rect>& ret_draw_rect_list)
{
	// �߂�l�̏�����
	ret_draw_rect_list.clear();

	S_disp_param dp = sp;

	// �\����`�̌v�Z
	if (false);

	// �v���[��
	else if (sp.type == SPRITETYPE_PLANE)	{
		bool ret_draw_rect_flag = false;
		C_rect ret_draw_rect(0, 0, 0, 0);
		dst->draw_rect(&dp, false, &ret_draw_rect_flag, &ret_draw_rect);
		if (ret_draw_rect_flag)
			ret_draw_rect_list.push_back(ret_draw_rect);
	}
	// �f�B�u
	else if (sp.type == SPRITETYPE_DIB)	{
		if (dib)	{
			bool ret_draw_rect_flag = false;
			C_rect ret_draw_rect(0, 0, 0, 0);
			dst->draw_dib(dib.get(), &dp, false, &ret_draw_rect_flag, &ret_draw_rect);
			if (ret_draw_rect_flag)
				ret_draw_rect_list.push_back(ret_draw_rect);
		}
	}
	// �s�N�`��
	else if (sp.type == SPRITETYPE_PCT)	{
		if (pct)	{
			pct->draw_to_dib(dst, &dp, false, &ret_draw_rect_list);
		}
	}
	// �A���o��
	else if (sp.type == SPRITETYPE_ALBUM)	{
		if (album)	{
			album->draw_to_dib(dst, sp.pct_no, &dp, false, &ret_draw_rect_list);
		}
	}
}

// ****************************************************************
// ��`�̌v�Z�i�S�̋�`�B�����蔻��ȂǂɎg�p�j
// ================================================================
C_rect C_sprite::get_rect(int pct_no)
{
	C_rect rect(0, 0, 0, 0);

	// �s�N�`���ԍ��̕␳�i�A���o����p�j
	if (pct_no == -1)
		pct_no = sp.pct_no;

	// �\����`�̌v�Z
	switch (sp.type)	{

		// �v���[��
		case SPRITETYPE_PLANE:
			rect.left = sp.pos.x + sp.plane.rect.left;
			rect.top = sp.pos.y + sp.plane.rect.top;
			rect.right = sp.pos.x + sp.plane.rect.right;
			rect.bottom = sp.pos.y + sp.plane.rect.bottom;
			break;

		// �f�B�u
		case SPRITETYPE_DIB:
			if (dib)	{
				rect.left = sp.pos.x;
				rect.top = sp.pos.y;
				rect.right = sp.pos.x + dib->get_width();
				rect.bottom = sp.pos.y + dib->get_height();
			}
			break;

		// �s�N�`��
		case SPRITETYPE_PCT:
			if (pct)	{
				rect.left = sp.pos.x;
				rect.top = sp.pos.y;
				rect.right = sp.pos.x + pct->get_width();
				rect.bottom = sp.pos.y + pct->get_height();
			}
			break;

		// �A���o��
		case SPRITETYPE_ALBUM:
			if (album)	{
				BSP<C_pct> pct = album->get_pct(pct_no);
				if (pct)	{
					rect.left = sp.pos.x;
					rect.top = sp.pos.y;
					rect.right = sp.pos.x + pct->get_width();
					rect.bottom = sp.pos.y + pct->get_height();
				}
			}
			break;
	}

	return rect;
}

// ****************************************************************
// dib �ɕ`��
// ================================================================
void C_sprite::draw_to_dib(C_dib* dst, C_rect* clip)
{
	if (!sp.disp)
		return;

	S_disp_param dp = sp;

	// �ǉ��N���b�v������
	if (clip)	{

		// �I���W�i���̃N���b�v������ꍇ�̓N���b�v������
		if (dp.dst_clip_use)	{
			dp.dst_clip.left = std::max(clip->left, dp.dst_clip.left);
			dp.dst_clip.top = std::max(clip->top, dp.dst_clip.top);
			dp.dst_clip.right = std::min(clip->right, dp.dst_clip.right);
			dp.dst_clip.bottom = std::min(clip->bottom, dp.dst_clip.bottom);
		}
		else	{
			dp.dst_clip.left = clip->left;
			dp.dst_clip.top = clip->top;
			dp.dst_clip.right = clip->right;
			dp.dst_clip.bottom = clip->bottom;
		}

		// �V�����쐬�����N���b�v���g��
		dp.dst_clip_use = true;
	}

	switch (sp.type)	{

		// �v���[��
		case SPRITETYPE_PLANE:	
			dst->draw_rect(&dp);
			break;

		// �f�B�u
		case SPRITETYPE_DIB:
			if (dib)	{
				dst->draw_dib(dib.get(), &dp);
			}
			break;

		// �s�N�`��
		case SPRITETYPE_PCT:
			if (pct)	{
				pct->draw_to_dib(dst, &dp);
			}
			break;

		// �A���o��
		case SPRITETYPE_ALBUM:
			if (album)	{
				album->draw_to_dib(dst, sp.pct_no, &dp);
			}
			break;
	}
}


}
