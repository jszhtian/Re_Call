#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_graphic.h"

namespace NT3
{

// ****************************************************************
// �p���b�g
// ================================================================
C_palette::C_palette()
{
	// �A���t�@�u�����h�e�[�u��
//	p_blend = &blend[0][255];
//	for (int a = 0; a < 256; a++)	{
//		for (int ds = -255; ds < 256; ds++)	{
//			blend[a][ds + 255] = BYTE(ds * a / 255);
//		}
//	}

	// GGO_GRAY8 �e�[�u��
	p_ggo_gray8 = ggo_gray8;
	for (int p = 0; p < 256; p++)
		ggo_gray8[p] = BYTE(p * 255 / 64);

	// �t�@�~�R���e�[�u��
//	p_famicom = famicom;
}

// ****************************************************************
// �p���b�g�F�t�@�~�R���p���b�g
// ================================================================

/*
BYTE Cpalette::famicom[] = {
	0x78, 0x78, 0x78, 0x20, 0x00, 0xB0, 0x28, 0x00, 0xB8, 0x60, 0x10, 0xA0, 0x98, 0x20, 0x78, 0xB0,
	0x10, 0x30, 0xA0, 0x30, 0x00, 0x78, 0x40, 0x00, 0x48, 0x58, 0x00, 0x38, 0x68, 0x00, 0x38, 0x6C,
	0x00, 0x30, 0x60, 0x40, 0x30, 0x50, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xB0, 0xB0, 0xB0, 0x40, 0x60, 0xF8, 0x40, 0x40, 0xFF, 0x90, 0x40, 0xF0, 0xD8, 0x40, 0xC0, 0xD8,
	0x40, 0x60, 0xE0, 0x50, 0x00, 0xC0, 0x70, 0x00, 0x88, 0x88, 0x00, 0x50, 0xA0, 0x00, 0x48, 0xA8,
	0x10, 0x48, 0xA0, 0x68, 0x40, 0x90, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xE8, 0xE8, 0xE8, 0x60, 0xA0, 0xFF, 0x50, 0x80, 0xFF, 0xA0, 0x70, 0xFF, 0xF0, 0x60, 0xFF, 0xFF,
	0x60, 0xB0, 0xFF, 0x78, 0x30, 0xFF, 0xA0, 0x00, 0xE8, 0xD0, 0x20, 0x98, 0xE8, 0x00, 0x70, 0xF0,
	0x40, 0x70, 0xE0, 0x90, 0x60, 0xD0, 0xE0, 0x78, 0x78, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0x90, 0xD0, 0xFF, 0xA0, 0xB8, 0xFF, 0xC0, 0xB0, 0xFF, 0xE0, 0xB0, 0xFF, 0xFF,
	0xB8, 0xE8, 0xFF, 0xC8, 0xB8, 0xFF, 0xD8, 0xA0, 0xFF, 0xF0, 0x90, 0xC8, 0xF0, 0x80, 0xA0, 0xF0,
	0xA0, 0xA0, 0xFF, 0xC8, 0xA0, 0xFF, 0xF0, 0xA0, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
*/

// ****************************************************************
// �p���b�g�F�t�@�~�R���p���b�g�̍쐬
// ================================================================

/*
void Cpalette::make_FC_palette(BYTE* pal_src, BYTE* pal_dst, int pal_dst_pos, int pal_dst_count)
{
	pal_dst += pal_dst_pos * 4;
	for (int n = 0; n < pal_dst_count; n++)	{
		*pal_dst ++ = 0x0f;			// 00: �� or ����
		*pal_dst ++ = *pal_src ++;	// 01
		*pal_dst ++ = *pal_src ++;	// 02
		*pal_dst ++ = *pal_src ++;	// 03
	}
}
*/

// ****************************************************************
// �`��\����
// ================================================================
S_disp_param::S_disp_param()
{
	// �����Ԃ��[���Ŗ��߂�imemcmp �ł̔�r���\�ɂ��邽�߁j
	ZeroMemory(this, sizeof(S_disp_param));

	// �ʏ�̏�����
	init();
}

void S_disp_param::init()
{
	// ��{�p�����[�^
	pos = C_point(0, 0);
	center = C_point(0, 0);
	sprite = false;
	blend_type = TRUE_COLOR_GRP_DRAW_MOD_STD;
	alpha = 255;
	mono = 0;
	reverse = 0;
	bright = 0;
	color = C_rgb(0, 0, 0);
	color_rate = 0;
	rep_color_r = 0;
	rep_color_g = 0;
	rep_color_b = 0;
	dst_clip_use = false;
	dst_clip = C_rect(0, 0, 0, 0);
	src_rect_use = false;
	src_rect = C_rect(0, 0, 0, 0);

	// �^�C���p�����[�^
	tile.ptr = NULL;
	tile.size = C_size(0, 0);
	tile.center = C_point(0, 0);
	tile.reverse = false;
	tile.shift = 0;
	tile.add = 0;

	// �v���[���p�����[�^
	plane.rect = C_rect(0, 0, 0, 0);
	plane.color = C_argb(0, 0, 0, 0);
	plane.type = E_disp_plane_type_box;

	// �t�B���^�p�����[�^
	filter.r = 0;
	filter.g = 0;
	filter.b = 0;
	filter.a = 255;

	// �g���p�����[�^
	scale = C_size(1000, 1000);
	rotate = 0;
}

// ****************************************************************
// �`��F�v���[��
// ================================================================
void tc_draw_plane(BYTE* dst, int dst_w, int dst_h, C_point dst_center, S_disp_param* dp)
{
	tc_draw_plane(dst, dst_w, dst_h, dst_center, dp, true, NULL, NULL);
}

void tc_draw_plane(BYTE* dst, int dst_w, int dst_h, C_point dst_center, S_disp_param* dp_, bool draw_flag, bool* ret_draw_rect_flag, C_rect* ret_draw_rect)
{
	S_disp_param dp = *dp_;

#if ___USE_NG___

	// �����x���v�Z
	BYTE alpha = (BYTE)((int)dp.alpha * dp.plane.color.a / 255);

	// NG ���C�u�������g���ĕ`��
	if (dp.plane.fill)	{
		true_color_boxfill(
			dst, dst_w, dst_h, 
			dp.pos.x, dp.pos.y, dp.pos.x + dp.plane.size.cx - 1, dp.pos.y + dp.plane.size.cy - 1, 
			dp.plane.color.r, dp.plane.color.g, dp.plane.color.b, 
			-1, -1, -1, -1, 0, 
			dp.tile.ptr, dp.tile.shift, dp.tile.add, dp.tile.size.cx, dp.tile.size.cy, dp.tile.center.x, dp.tile.center.y, dp.tile.reverse, 
			dp.mono, dp.reverse, dp.color.r, dp.color.g, dp.color.b, dp.color_rate, 
			dp.rep_color_r, dp.rep_color_g, dp.rep_color_b, 
			dp.bright, alpha, dp.blend_type
			);
	}
	else	{
		true_color_box(
			dst, dst_w, dst_h, 
			dp.pos.x, dp.pos.y, dp.pos.x + dp.plane.size.cx - 1, dp.pos.y + dp.plane.size.cy - 1, 
			dp.plane.color.r, dp.plane.color.g, dp.plane.color.b, 
			-1, -1, -1, -1, 0, 
			dp.tile.ptr, dp.tile.shift, dp.tile.add, dp.tile.size.cx, dp.tile.size.cy, dp.tile.center.x, dp.tile.center.y, dp.tile.reverse, 
			dp.mono, dp.reverse, dp.color.r, dp.color.g, dp.color.b, dp.color_rate, 
			dp.rep_color_r, dp.rep_color_g, dp.rep_color_b, 
			dp.bright, alpha, dp.blend_type
			);
	}

#elif ___USE_NAMALIB_GRP___

	// tona ��` �� nama ��`�ɕϊ�
	dp.dst_clip.right -= 1;
	dp.dst_clip.bottom -= 1;
	dp.src_rect.right -= 1;
	dp.src_rect.bottom -= 1;

	// �{�b�N�X��`��
	Cnamalib_grp	grp;
	grp.true_color_box_draw(
		dst,
		dst_w, dst_h, dst_center.x, dst_center.y,
		dp.dst_clip_use, dp.dst_clip.left, dp.dst_clip.top, dp.dst_clip.right, dp.dst_clip.bottom,
		dp.plane.color.r, dp.plane.color.g, dp.plane.color.b, dp.plane.color.a, dp.sprite ? 1 : 0,
		dp.plane.type, 
		dp.plane.rect.left,			// (���������Fleft > right �̏ꍇ�C�����K�v�j
		dp.plane.rect.top, 			// (���������Fbottom > top �̏ꍇ�C�����K�v�j
		dp.plane.rect.right - 1, 	// (���������Fleft > right �̏ꍇ�C�����K�v�j
		dp.plane.rect.bottom - 1, 	// (���������Fbottom > top �̏ꍇ�C�����K�v�j
		dp.center.x, dp.center.y,
		dp.src_rect_use, dp.src_rect.left, dp.src_rect.top, dp.src_rect.right, dp.src_rect.bottom,
		dp.pos.x, dp.pos.y,
		dp.blend_type, dp.alpha, dp.mono, dp.reverse, dp.bright,
		dp.tile.ptr, dp.tile.size.cx, dp.tile.size.cy, dp.tile.center.x, dp.tile.center.y, dp.tile.reverse, dp.tile.shift, dp.tile.add,
		dp.color.r, dp.color.g, dp.color.b, dp.color_rate,
		dp.rep_color_r, dp.rep_color_g, dp.rep_color_b,
		dp.filter.r, dp.filter.g, dp.filter.b, dp.filter.a,
		dp.scale.cx, dp.scale.cy, dp.rotate, 
		draw_flag, ret_draw_rect_flag, ret_draw_rect);

	// nama ��` �� tona ��`�ɕϊ�
	if (ret_draw_rect)	{
		ret_draw_rect->right += 1;
		ret_draw_rect->bottom += 1;
	}

#endif
}

// ****************************************************************
// �`��Fdib
// ================================================================
void tc_draw_dib(BYTE* dst, int dst_w, int dst_h, C_point dst_center, BYTE* src, int src_w, int src_h, S_disp_param* dp)
{
	tc_draw_dib(dst, dst_w, dst_h, dst_center, src, src_w, src_h, dp, true, NULL, NULL);
}

void tc_draw_dib(BYTE* dst, int dst_w, int dst_h, C_point dst_center, BYTE* src, int src_w, int src_h, S_disp_param* dp_, bool draw_flag, bool* ret_draw_rect_flag, C_rect* ret_draw_rect)
{
	S_disp_param dp = *dp_;

#if ___USE_NG___

	// NG ���C�u�������g���ĕ`��
	true_color_grp_disp(
		dst, dst_w, dst_h, 
		src, src_w, src_h, 0, 0, dp.sprite ? 1 : 0, 
		dp.pos.x, dp.pos.y, dp.dst_clip.left, dp.dst_clip.top, dp.dst_clip.right - 1, dp.dst_clip.bottom - 1, 0, 
		dp.tile.ptr, dp.tile.shift, dp.tile.add, dp.tile.size.cx, dp.tile.size.cy, dp.tile.center.x, dp.tile.center.y, dp.tile.reverse, 
		dp.mono, dp.reverse, dp.color.r, dp.color.g, dp.color.b, dp.color_rate, 
		dp.rep_color_r, dp.rep_color_g, dp.rep_color_b, 
		dp.bright, dp.alpha, dp.blend_type
		);

#elif ___USE_NAMALIB_GRP___

	// tona ��` �� nama ��`�ɕϊ�
	dp.dst_clip.right -= 1;
	dp.dst_clip.bottom -= 1;
	dp.src_rect.right -= 1;
	dp.src_rect.bottom -= 1;

	Cnamalib_grp	grp;
	grp.true_color_grp_draw(
		dst,
		dst_w, dst_h, dst_center.x, dst_center.y,
		dp.dst_clip_use, dp.dst_clip.left, dp.dst_clip.top, dp.dst_clip.right, dp.dst_clip.bottom,
		src, dp.sprite ? 1 : 0,
		src_w, src_h, dp.center.x, dp.center.y,
		dp.src_rect_use, dp.src_rect.left, dp.src_rect.top, dp.src_rect.right, dp.src_rect.bottom,
		dp.pos.x, dp.pos.y,
		dp.blend_type, dp.alpha, dp.mono, dp.reverse, dp.bright,
		dp.tile.ptr, dp.tile.size.cx, dp.tile.size.cy, dp.tile.center.x, dp.tile.center.y, dp.tile.reverse, dp.tile.shift, dp.tile.add,
		dp.color.r, dp.color.g, dp.color.b, dp.color_rate,
		dp.rep_color_r, dp.rep_color_g, dp.rep_color_b,
		dp.filter.r, dp.filter.g, dp.filter.b, dp.filter.a,
		dp.scale.cx, dp.scale.cy, dp.rotate, 
		draw_flag, ret_draw_rect_flag, ret_draw_rect);

	// nama ��` �� tona ��`�ɕϊ�
	if (ret_draw_rect)	{
		ret_draw_rect->right += 1;
		ret_draw_rect->bottom += 1;
	}

#endif
}

void tc_draw_dib_easy(BYTE* dst, int dst_w, int dst_h, BYTE* src, int src_w, int src_h, int x, int y)
{
	if (dst == 0 || src == 0)	{	return;	}

	int ex1 = 0;
	int ey1 = 0;
	int ex2 = dst_w - 1;
	int ey2 = dst_h - 1;

	// �\���͈͊O�`�F�b�N�P
	if (x > ex2)	{	return;	}
	if (y > ey2)	{	return;	}

	// ���W����
	int src_free_x, src_free_y;
	int dst_free_x, dst_free_y;
	int draw_cnt_x, draw_cnt_y;
	if (x < 0)	{
		src_free_x = (-x);
		draw_cnt_x = src_w - src_free_x;
		dst_free_x = 0;
	}
	else	{
		src_free_x = 0;
		draw_cnt_x = src_w;
		dst_free_x = x;
	}
	if (y < 0)	{
		src_free_y = (-y);
		draw_cnt_y = src_h - src_free_y;
		dst_free_y = 0;
	}
	else	{
		src_free_y = 0;
		draw_cnt_y = src_h;
		dst_free_y = y;
	}

	// �\���͈͊O�`�F�b�N�Q
	if (draw_cnt_x <= 0)	{	return;	}
	if (draw_cnt_y <= 0)	{	return;	}

	// �n�_���~�b�g�C��
	int tmp;
	if(ex1 > dst_free_x){
		tmp = ex1 - dst_free_x;
		src_free_x += tmp;
		draw_cnt_x -= tmp;
		dst_free_x = ex1;
	}
	if(ey1 > dst_free_y){
		tmp = ey1 - dst_free_y;
		src_free_y += tmp;
		draw_cnt_y -= tmp;
		dst_free_y = ey1;
	}

	// �\���͈͊O�`�F�b�N�R
	if (ex2 < dst_free_x)	{	return;	}
	if (ey2 < dst_free_y)	{	return;	}

	// �I�_���~�b�g�C��
	tmp = ex2 - dst_free_x + 1;
	if (tmp < draw_cnt_x)	{	draw_cnt_x = tmp;	}
	tmp = ey2 - dst_free_y + 1;
	if (tmp < draw_cnt_y)	{	draw_cnt_y = tmp;	}

	// �\���͈͊O�`�F�b�N�S
	if (draw_cnt_x <= 0)	{	return;	}
	if (draw_cnt_y <= 0)	{	return;	}

	// �A�h���X�Z�o
	src += (src_free_x + (src_free_y * src_w)) * sizeof(DWORD);
	dst += (dst_free_x + (dst_free_y * dst_w)) * sizeof(DWORD);

	// �`��p�����[�^
	int add_dst_addr = (dst_w - draw_cnt_x) * sizeof(DWORD);
	int add_src_addr = (src_w - draw_cnt_x) * sizeof(DWORD);

	// �`��{��
	_asm{
		cld
		mov		edi, dst
		mov		esi, src
		mov		ecx, draw_cnt_y
		mov		eax, draw_cnt_x
		mov		edx, add_dst_addr
		mov		ebx, add_src_addr
	grplv_loop_y:
		push	ecx
		mov		ecx, eax

		rep		movsd

		pop		ecx
		add		edi, edx
		add		esi, ebx
		loop	grplv_loop_y
	}
}




#if 0

// ****************************************************************
// �`��֐�
// ================================================================
void tc_draw_box(
		BYTE* dst, int dst_w, int dst_h,	// �`���
		int x, int y, int w, int h,			// �{�b�N�X��`
		BYTE a, BYTE r, BYTE g, BYTE b,		// �{�b�N�X�F
		bool sprite, int blend_type,		// ���߂���A�������@
		BYTE alpha, BYTE mono, BYTE reverse, int bright,			// �s�����x�A���m�N���x�A���]�x�A���Óx
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,	// �q�f�a�ϊ�
		int rep_color_r, int rep_color_g, int rep_color_b,			// �q�f�a�␳
		bool dst_clip, int dst_clip_x, int dst_clip_y, int dst_clip_w, int dst_clip_h,	// �`���N���b�v
		bool src_rect, int src_rect_x, int src_rect_y, int src_rect_w, int src_rect_h	// �`���`
	)
{

	// �N���b�v�̍쐬
	if (!dst_clip)	{
		dst_clip_x = 0;
		dst_clip_y = 0;
		dst_clip_w = dst_w;
		dst_clip_h = dst_h;
	}

	// ��`�̍쐬
	if (!src_rect)	{
		src_rect_x = 0;
		src_rect_y = 0;
		src_rect_w = w;
		src_rect_h = h;
	}

	// �N���b�s���O
	int new_clip_x = Max(Max(Max(0, x), dst_clip_x), x + src_rect_x);
	int new_clip_y = Max(Max(Max(0, y), dst_clip_y), y + src_rect_y);
	int new_clip_w = Min(Min(Min(dst_w, x + w), dst_clip_x + dst_clip_w), x + src_rect_x + src_rect_w) - new_clip_x;
	int new_clip_h = Min(Min(Min(dst_h, y + h), dst_clip_y + dst_clip_h), y + src_rect_y + src_rect_h) - new_clip_y;
	if (new_clip_w <= 0 || new_clip_h <= 0)		return;

	// �{�b�N�X��`
	x = new_clip_x;
	y = new_clip_y;
	w = new_clip_w;
	h = new_clip_h;

#if ___USE_NG___
	true_color_boxfill(dst, dst_w, dst_h, x, y, x + w - 1, y + h - 1, r, g, b, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, mono, reverse, color_r, color_g, color_b, color_rate, rep_color_r, rep_color_g, rep_color_b, bright, a, blend_type);
#else
	// �����^�C�v�ɂ���ĕ���
	if (!sprite)	tc_draw_box_copy(dst, dst_w, dst_h, x, y, w, h, a, r, g, b, blend_type, alpha, mono, reverse, bright, color_r, color_g, color_b, color_rate, rep_r, rep_g, rep_b);
	else			tc_draw_box_blend(dst, dst_w, dst_h, x, y, w, h, a, r, g, b, blend_type, alpha, mono, reverse, bright, color_r, color_g, color_b, color_rate, rep_r, rep_g, rep_b);
#endif
}

void tc_draw_dib(
		BYTE* dst, int dst_w, int dst_h,	// �`���
		BYTE* src, int src_w, int src_h,	// �`�挳
		int x, int y,						// �`��ʒu
		bool sprite, int blend_type,		// ���߂���A�������@
		BYTE alpha, BYTE mono, BYTE reverse, int bright,			// �s�����x�A���m�N���x�A���]�x�A���Óx
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,	// �q�f�a�ϊ�
		int rep_color_r, int rep_color_g, int rep_color_b,			// �q�f�a�␳
		bool dst_clip_use, int dst_clip_x, int dst_clip_y, int dst_clip_w, int dst_clip_h,	// �`���N���b�v
		bool src_rect_use, int src_rect_x, int src_rect_y, int src_rect_w, int src_rect_h	// �`���`
	)
{
	// �N���b�v�̍쐬
	C_rect dst_clip;
	if (!dst_clip_use)	dst_clip = C_rect(0, 0, dst_w, dst_h);
	else				dst_clip = C_rect(dst_clip_x, dst_clip_y, dst_clip_w, dst_clip_h);

	// �`���`�̍쐬
	C_rect src_rect;
	if (!src_rect_use)	src_rect = C_rect(0, 0, src_w, src_h);
	else				src_rect = C_rect(src_rect_x, src_rect_y, src_rect_w, src_rect_h);

	// �`���`�̕␳
	src_rect.left = Max(0, src_rect.left);
	src_rect.top = Max(0, src_rect.top);
	src_rect.right = Min(src_w, src_rect.right);
	src_rect.bottom = Min(src_h, src_rect.bottom);
	if (src_rect.left >= src_rect.right || src_rect.top >= src_rect.bottom)		return;

	// �N���b�s���O
	dst_clip.left = Max(Max(0, x + src_rect.left), dst_clip.left);
	dst_clip.top = Max(Max(0, y + src_rect.top), dst_clip.top);
	dst_clip.right = Min(Min(dst_w, x + src_rect.right), dst_clip.right);
	dst_clip.bottom = Min(Min(dst_h, y + src_rect.bottom), dst_clip.bottom);
	if (dst_clip.left >= dst_clip.right || dst_clip.top >= dst_clip.bottom)		return;

	// �`���`�̍Đݒ�
	src_rect.left = dst_clip.left - x;
	src_rect.top = dst_clip.top - y;
	src_rect.right = dst_clip.right - x;
	src_rect.bottom = dst_clip.bottom - y;

	// ���W�̍Đݒ�
	x = dst_clip.left;
	y = dst_clip.top;

	// �����^�C�v�ɂ���ĕ���
#if ___USE_NG___
	true_color_grp_disp(dst, dst_w, dst_h, src, src_w, src_h, 0, 0, sprite ? 1 : 0, x, y, dst_clip.left, dst_clip.top, dst_clip.right - 1, dst_clip.bottom - 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, mono, reverse, color_r, color_g, color_b, color_rate, rep_color_r, rep_color_g, rep_color_b, bright, alpha, blend_type);
#else
	if (!sprite)	tc_draw_dib_copy(dst, dst_w, dst_h, src, src_w, src_h, x, y, alpha, mono, reverse, bright, color_r, color_g, color_b, color_rate, rep_r, rep_g, rep_b, blend_type, src_rect.left, src_rect.top, src_rect.width(), src_rect.height());
	else			tc_draw_dib_blend(dst, dst_w, dst_h, src, src_w, src_h, x, y, alpha, mono, reverse, bright, color_r, color_g, color_b, color_rate, rep_r, rep_g, rep_b, blend_type, src_rect.left, src_rect.top, src_rect.width(), src_rect.height());
#endif
}

// ****************************************************************
// �`��֐��F�R�s�[
// ================================================================
void tc_draw_box_copy(
		BYTE* dst, int dst_w, int dst_h, 
		int x, int y, int w, int h,			// �{�b�N�X��`
		BYTE a, BYTE r, BYTE g, BYTE b,		// �{�b�N�X�F
		int blend_type,												// �������@
		BYTE alpha, BYTE mono, BYTE reverse, int bright,			// �s�����x�A���m�N���x�A���]�x�A���Óx
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,	// �q�f�a�ϊ�
		int rep_color_r, int rep_color_g, int rep_color_b			// �q�f�a�␳
	)
{
	// �J�n�ʒu
	BYTE* dst_start = dst + (y * dst_w + x) * 4;
	// �s�b�`
	int dst_pitch = dst_w * 4;
	// �s�����x
	a = (BYTE)((int)a * alpha / 255);
	// �F
	DWORD color = ARGB(a, r, g, b);

	_asm	{
		cld
		mov		edi, dst_start	// �]����̐ݒ�
		mov		ecx, h			// ���������[�v�J�E���^�ɃZ�b�g

	loop_y:

		push	edi				// �]����̌��݂̈ʒu���L��
		push	ecx				// ���[�v�J�E���^���L��
		mov		ecx, w			// �������[�v�J�E���^�ɃZ�b�g

		mov		eax, color
		rep		stosd

		pop		ecx				// ���[�v�J�E���^�����o��
		pop		edi				// �]����̍��W�����ɖ߂�
		add		edi, dst_pitch

		loop	loop_y
	}
}

void tc_draw_dib_copy(
		BYTE* dst, int dst_w, int dst_h,	// �`���
		BYTE* src, int src_w, int src_h,	// �`�挳
		int x, int y,						// �`��ʒu
		int blend_type,													// �������@
		BYTE alpha, BYTE mono, BYTE reverse, int bright,				// �s�����x�A���m�N���x�A���]�x�A���Óx
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,		// �q�f�a�ϊ�
		int rep_color_r, int rep_color_g, int rep_color_b,				// �q�f�a�␳
		int src_rect_x, int src_rect_y, int src_rect_w, int src_rect_h	// �`���`
	)
{
	// �J�n�ʒu
	BYTE* dst_start = dst + (y * dst_w + x) * 4;
	BYTE* src_start = src + (src_rect_y * src_w + src_rect_x) * 4;
	// �s�b�`
	int dst_pitch = dst_w * 4;
	int src_pitch = src_w * 4;

	_asm	{
		cld
		mov		esi, src_start	// �]�����̐ݒ�
		mov		edi, dst_start	// �]����̐ݒ�
		mov		ecx, src_rect_h	// ���������[�v�J�E���^�ɃZ�b�g

	loop_y:

		push	esi				// �]�����̌��݂̈ʒu���L��
		push	edi				// �]����̌��݂̈ʒu���L��
		push	ecx				// ���[�v�J�E���^���L��
		mov		ecx, src_rect_w	// �������[�v�J�E���^�ɃZ�b�g

		rep		movsd

		pop		ecx				// ���[�v�J�E���^�����o��
		pop		edi				// �]����̍��W�����ɖ߂�
		pop		esi				// �]�����̍��W�����ɖ߂�
		add		esi, src_pitch
		add		edi, dst_pitch

		loop	loop_y
	}
}

// ****************************************************************
// �`��֐��F�u�����h
// ================================================================
// �p���b�g�H
#define		BLEND(col)				\
__asm		xor		eax, eax		\
__asm		xor		ebx, ebx		\
__asm		mov		bl,  [esi+3]	\
__asm		shl		ebx, 9			\
__asm		add		ebx, pal		\
__asm		mov		al,  [esi+col]	\
__asm		add		ebx, eax		\
__asm		mov		al,  [edi+col]	\
__asm		sub		ebx, eax		\
__asm		add		al,  [ebx]		\
__asm		mov		[edi+col], al

void tc_draw_box_blend(
		BYTE* dst, int dst_w, int dst_h, 
		int x, int y, int w, int h,			// �{�b�N�X��`
		BYTE a, BYTE r, BYTE g, BYTE b,		// �{�b�N�X�F
		int blend_type,												// �������@
		BYTE alpha, BYTE mono, BYTE reverse, int bright,			// �s�����x�A���m�N���x�A���]�x�A���Óx
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,	// �q�f�a�ϊ�
		int rep_color_r, int rep_color_g, int rep_color_b			// �q�f�a�␳
	)
{
	// �J�n�ʒu
	BYTE* dst_start = dst + (y * dst_w + x) * 4;
	// �s�b�`
	int dst_pitch = dst_w * 4;
	// �s�����x
	a = (BYTE)((int)a * alpha / 255);
	// �p���b�g
	BYTE* pal_0 = Cpalette::get_instance().p_blend + (a * 512) + b;
	BYTE* pal_1 = Cpalette::get_instance().p_blend + (a * 512) + g;
	BYTE* pal_2 = Cpalette::get_instance().p_blend + (a * 512) + r;

	_asm	{
		cld
		mov		edi, dst_start	// �]����̐ݒ�
		mov		ecx, h			// ���������[�v�J�E���^�ɃZ�b�g

	loop_y:

		push	edi				// �]����̌��݂̈ʒu���L��
		push	ecx				// ���[�v�J�E���^���L��
		mov		ecx, w			// �������[�v�J�E���^�ɃZ�b�g

	loop_x:

		xor		eax, eax
		mov		ebx, pal_0
		mov		al,  [edi+0]
		sub		ebx, eax
		add		al,  [ebx]
		mov		[edi+0], al

		xor		eax, eax
		mov		ebx, pal_1
		mov		al,  [edi+1]
		sub		ebx, eax
		add		al,  [ebx]
		mov		[edi+1], al

		xor		eax, eax
		mov		ebx, pal_2
		mov		al,  [edi+2]
		sub		ebx, eax
		add		al,  [ebx]
		mov		[edi+2], al

		add		edi, 4

	loop	loop_x

		pop		ecx				// ���[�v�J�E���^�����o��
		pop		edi				// �]����̍��W�����ɖ߂�
		add		edi, dst_pitch

	loop	loop_y
	}
}

// �p���b�g�H
#if 1
#define		BLEND(col)				\
__asm		xor		eax, eax		\
__asm		xor		ebx, ebx		\
__asm		mov		bl,  [esi+3]	\
__asm		shl		ebx, 9			\
__asm		add		ebx, pal		\
__asm		mov		al,  [esi+col]	\
__asm		add		ebx, eax		\
__asm		mov		al,  [edi+col]	\
__asm		sub		ebx, eax		\
__asm		add		al,  [ebx]		\
__asm		mov		[edi+col], al
#else
#define		BLEND(col)				\
__asm		xor		eax, eax		\
__asm		xor		ebx, ebx		\
__asm		mov		al,  [esi+col]	\
__asm		mov		bl,  [edi+col]	\
__asm		sub		eax, ebx		\
__asm		mul		[esi+3]			\
__asm		shr		eax, 8			\
__asm		add		al,  [edi+col]	\
__asm		mov		[edi+col], al
#endif

void tc_draw_dib_blend(
		BYTE* dst, int dst_w, int dst_h,	// �`���
		BYTE* src, int src_w, int src_h,	// �`�挳
		int x, int y,						// �`��ʒu
		int blend_type,													// �������@
		BYTE alpha, BYTE mono, BYTE reverse, int bright,				// �s�����x�A���m�N���x�A���]�x�A���Óx
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,		// �q�f�a�ϊ�
		int rep_color_r, int rep_color_g, int rep_color_b,				// �q�f�a�␳
		int src_rect_x, int src_rect_y, int src_rect_w, int src_rect_h	// �`���`
	)
{
	// �J�n�ʒu
	BYTE* dst_start = dst + (y * dst_w + x) * 4;
	BYTE* src_start = src + (src_rect_y * src_w + src_rect_x) * 4;
	// �s�b�`
	int dst_pitch = dst_w * 4;
	int src_pitch = src_w * 4;
	// �p���b�g
	BYTE* pal = Cpalette::get_instance().p_blend;

	_asm	{
		cld
		mov		esi, src_start	// �]�����̐ݒ�
		mov		edi, dst_start	// �]����̐ݒ�
		mov		ecx, src_rect_h	// ���������[�v�J�E���^�ɃZ�b�g

	loop_y:

		push	esi				// �]�����̌��݂̈ʒu���L��
		push	edi				// �]����̌��݂̈ʒu���L��
		push	ecx				// ���[�v�J�E���^���L��
		mov		ecx, src_rect_w	// �������[�v�J�E���^�ɃZ�b�g

	loop_x:

		BLEND(0)
		BLEND(1)
		BLEND(2)

		add		esi, 4
		add		edi, 4
	loop	loop_x

		pop		ecx				// ���[�v�J�E���^�����o��
		pop		edi				// �]����̍��W�����ɖ߂�
		pop		esi				// �]�����̍��W�����ɖ߂�
		add		esi, src_pitch
		add		edi, dst_pitch

	loop	loop_y
	}
}

#endif


};
