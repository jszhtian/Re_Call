#include	"pch.h"

// ****************************************************************
// �g�D���[�J���[�O���t�B�b�N�`��
// ================================================================

// �p�����[�^�R�s�[�}�N��
#define		__DRAW_PARAM_COPY_MAC__ \
	_dp.dst_ptr = dst_ptr; \
	_dp.dst_size.cx = dst_width; \
	_dp.dst_size.cy = dst_height; \
	_dp.dst_center.x = dst_center_x; \
	_dp.dst_center.y = dst_center_y; \
	_dp.dst_area_flag   = dst_area_flag; \
	_dp.dst_area.left   = dst_area_left; \
	_dp.dst_area.top    = dst_area_top; \
	_dp.dst_area.right  = dst_area_right; \
	_dp.dst_area.bottom = dst_area_bottom; \
	\
	_dp.src_type = src_type; \
	_dp.src_center.x = src_center_x; \
	_dp.src_center.y = src_center_y; \
	_dp.src_area_flag   = src_area_flag; \
	_dp.src_area.left   = src_area_left; \
	_dp.src_area.top    = src_area_top; \
	_dp.src_area.right  = src_area_right; \
	_dp.src_area.bottom = src_area_bottom; \
	\
	_dp.draw_pos.x = draw_x; \
	_dp.draw_pos.y = draw_y; \
	\
	_dp.draw_mod = draw_mod; \
	\
	_dp.tr = tr; \
	\
	_dp.tile_ptr = tile_ptr; \
	_dp.tile_size.cx = tile_width; \
	_dp.tile_size.cy = tile_height; \
	_dp.tile_shift = tile_shift; \
	_dp.tile_add = tile_add; \
	\
	_dp.scale_width  = scale_width; \
	_dp.scale_height = scale_height; \
	_dp.rotate       = rotate; \
	\
	_dp.draw_flag = draw_flag; \
	_dp.ret_draw_area_flag = ret_draw_area_flag; \
	_dp.ret_draw_area = ret_draw_area; \
	\
	if (_dp.draw_flag)	{ \
		_dp.tile_center.x = tile_center_x; \
		_dp.tile_center.y = tile_center_y; \
		_dp.tile_reverse = tile_reverse; \
		\
		_dp.mono = mono; \
		_dp.reverse = reverse; \
		_dp.bright = bright; \
		\
		_dp.color_r = color_r; \
		_dp.color_g = color_g; \
		_dp.color_b = color_b; \
		_dp.color_rate = color_rate; \
		\
		_dp.palette_r = palette_r; \
		_dp.palette_g = palette_g; \
		_dp.palette_b = palette_b; \
		\
		_dp.flt_r = flt_r; \
		_dp.flt_g = flt_g; \
		_dp.flt_b = flt_b; \
		_dp.flt_tr = flt_tr; \
	}

// ****************************************************************
// �摜�`��

static int zzzzz = 0;

bool Cnamalib_grp::true_color_grp_draw(
	BYTE *dst_ptr,
	int dst_width, int dst_height, int dst_center_x, int dst_center_y,
	bool dst_area_flag, int dst_area_left, int dst_area_top, int dst_area_right, int dst_area_bottom,
	BYTE *src_ptr, int src_type,
	int src_width, int src_height, int src_center_x, int src_center_y,
	bool src_area_flag, int src_area_left, int src_area_top, int src_area_right, int src_area_bottom,
	int draw_x, int draw_y,
	TRUE_COLOR_GRP_DRAW_MOD draw_mod, BYTE tr, BYTE mono, BYTE reverse, int bright,
	BYTE *tile_ptr, int tile_width, int tile_height, int tile_center_x, int tile_center_y, bool tile_reverse, BYTE tile_shift, int tile_add,
	BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,
	int palette_r, int palette_g, int palette_b,
	int flt_r, int flt_g, int flt_b, BYTE flt_tr,
	int scale_width, int scale_height, int rotate,
	bool draw_flag, bool *ret_draw_area_flag, RECT *ret_draw_area)
{
	__DRAW_PARAM_COPY_MAC__
	_dp.src_ptr = src_ptr;
	_dp.src_size.cx = src_width;
	_dp.src_size.cy = src_height;

	// �ϊ����[�h
	TCGD_EX_MOD ex_mod = TCGD_EX_MOD_STANDARD;

	if (!_tcgd_func_param(false))	{	return false;	}
	if (!_fdp.scale_rotate_flag)	{	return _tcgd_func_draw_graphic_standard(ex_mod);		}
	else							{	return _tcgd_func_draw_graphic_scale_rotate(ex_mod);	}
}

// ****************************************************************
// �{�b�N�X�`��
bool Cnamalib_grp::true_color_box_draw(
	BYTE *dst_ptr,
	int dst_width, int dst_height, int dst_center_x, int dst_center_y,
	bool dst_area_flag, int dst_area_left, int dst_area_top, int dst_area_right, int dst_area_bottom,
	BYTE src_r, BYTE src_g, BYTE src_b, BYTE src_a, int src_type,
	int src_box_type, int src_box_x1, int src_box_y1, int src_box_x2, int src_box_y2, int src_center_x, int src_center_y,
	bool src_area_flag, int src_area_left, int src_area_top, int src_area_right, int src_area_bottom,
	int draw_x, int draw_y,
	TRUE_COLOR_GRP_DRAW_MOD draw_mod, BYTE tr, BYTE mono, BYTE reverse, int bright,
	BYTE *tile_ptr, int tile_width, int tile_height, int tile_center_x, int tile_center_y, bool tile_reverse, BYTE tile_shift, int tile_add,
	BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,
	int palette_r, int palette_g, int palette_b,
	int flt_r, int flt_g, int flt_b, BYTE flt_tr,
	int scale_width, int scale_height, int rotate,
	bool draw_flag, bool *ret_draw_area_flag, RECT *ret_draw_area)
{
	__DRAW_PARAM_COPY_MAC__
	_dp.src_color.r = src_r;
	_dp.src_color.g = src_g;
	_dp.src_color.b = src_b;
	_dp.src_color.a = src_a;
	_dp.src_box.type = src_box_type;
	_dp.src_box.x1 = src_box_x1;
	_dp.src_box.y1 = src_box_y1;
	_dp.src_box.x2 = src_box_x2;
	_dp.src_box.y2 = src_box_y2;

	// �ϊ����[�h
	TCGD_EX_MOD ex_mod = TCGD_EX_MOD_STANDARD;
	switch (draw_mod)	{
		case TRUE_COLOR_GRP_DRAW_MOD_CHG:
			ex_mod = TCGD_EX_MOD_CHANGE;
			_dp.src_color.a = 255;		// �������x���O���ƃp�����[�^�������ɔ�\�����肳��Ă��܂��B
			break;
	}

	if (!_tcgd_func_param(true))	{	return false;	}

	switch (_dp.src_box.type)	{
		case 0:		// ���C��
			return _tcgd_func_draw_line(_dp.src_box.x1, _dp.src_box.y1, _dp.src_box.x2, _dp.src_box.y2, ex_mod);
			break;
		case 1:		// �G���v�e�B�[�{�b�N�X
			return _tcgd_func_draw_empty_box(_dp.src_box.x1, _dp.src_box.y1, _dp.src_box.x2, _dp.src_box.y2, ex_mod);
			break;
		case 2:		// �t�B���{�b�N�X
			if (!_fdp.scale_rotate_flag)	{	return _tcgd_func_draw_graphic_standard(ex_mod);		}
			else							{	return _tcgd_func_draw_graphic_scale_rotate(ex_mod);	}
			break;
	}
	return false;
}

// ****************************************************************
// �p�����[�^����
bool Cnamalib_grp::_tcgd_func_param(bool grp_or_box)
{
	int tmp;
	bool line_mode = false;

	// ���^�[���p�̕`���`������
	if (_dp.ret_draw_area_flag)	{	*_dp.ret_draw_area_flag = false;	}

	// �`�悪���S�����Ȃ̂ł����Ń��^�[��
	if (_dp.tr == 0)	{	return false;	}

	// �g��k���{�����O�Ȃ̂ł����Ń��^�[��
	if (_dp.scale_width == 0 || _dp.scale_height == 0)	{	return false;	}

	// �`�悪���S�����Ȃ̂ł����Ń��^�[��
	if (_dp.draw_mod == TRUE_COLOR_GRP_DRAW_MOD_FLT)	{
		if(_dp.flt_tr == 0)	{	return false;	}
		// �����x�̍���
		else if (_dp.flt_tr != 255)	{
			_dp.tr = (BYTE)(((int)_dp.tr * (int)_dp.flt_tr) / 255);
			if (_dp.tr == 0)	{	return false;	}
		}
	}

	// �`��悪�w�肳��Ă��Ȃ��̂ł����Ń��^�[��
	if (_dp.dst_ptr == NULL)	{	return false;	}

	// �`���̃T�C�Y���O�ȉ��Ȃ̂ł����Ń��^�[��
	if (_dp.dst_size.cx <= 0 || _dp.dst_size.cy <= 0)	{	return false;	}

	// �摜�ݒ�
	if (!grp_or_box)	{
		// �`�挳���w�肳��Ă��Ȃ��̂ł����Ń��^�[��
		if (_dp.src_ptr == NULL)	{	return false;	}

		// �`�挳�̃T�C�Y���O�ȉ��Ȃ̂ł����Ń��^�[��
		if (_dp.src_size.cx <= 0 || _dp.src_size.cy <= 0)	{	return false;	}

		// �`�挳�͈͐ݒ�i�͈͂��w�肳��Ă��Ȃ��ꍇ�j
		if (!_dp.src_area_flag)	{
			_dp.src_area.left    = -_dp.src_center.x;
			_dp.src_area.top     = -_dp.src_center.y;
			_dp.src_area.right   = -_dp.src_center.x + (_dp.src_size.cx - 1);
			_dp.src_area.bottom  = -_dp.src_center.y + (_dp.src_size.cy - 1);
		}
	}
	// �{�b�N�X�ݒ�
	else	{
		if (_dp.draw_mod == TRUE_COLOR_GRP_DRAW_MOD_NULL)	{
			// �p�����[�^���m�t�k�k�ɕ␳
			_dp.src_color.a = 0;
			_dp.src_color.r = 0;
			_dp.src_color.g = 0;
			_dp.src_color.b = 0;
			_dp.src_type = 0;
		}
		else	{
			// �{�b�N�X�F�̓����x���O�Ȃ̂ŕ`�悵�Ȃ�
			if (_dp.src_color.a == 0)	{	return false;	}
			// �{�b�N�X�� src_type ������
			_dp.src_type = (_dp.src_color.a == 255) ? 0 : 1;
		}

		// �{�b�N�X�F��`�挳�ɐݒ�
		_dp.src_ptr = (BYTE *)&_dp.src_color;

		// �n�_�I�_��`�� (0, 0) �̍��W�n�ɕ␳����
		tmp = min(_dp.src_box.x1, _dp.src_box.x2);
		_dp.src_center.x    -= tmp;
		_dp.src_box.x1      -= tmp;
		_dp.src_box.x2      -= tmp;
		tmp = min(_dp.src_box.y1, _dp.src_box.y2);
		_dp.src_center.y    -= tmp;
		_dp.src_box.y1      -= tmp;
		_dp.src_box.y2      -= tmp;

		// �T�C�Y�Z�o
		if (_dp.src_box.x1 <= _dp.src_box.x2)	{	_dp.src_size.cx = _dp.src_box.x2 - _dp.src_box.x1 + 1;	}
		else									{	_dp.src_size.cx = _dp.src_box.x1 - _dp.src_box.x2 + 1;	}
		if (_dp.src_box.y1 <= _dp.src_box.y2)	{	_dp.src_size.cy = _dp.src_box.y2 - _dp.src_box.y1 + 1;	}
		else									{	_dp.src_size.cy = _dp.src_box.y1 - _dp.src_box.y2 + 1;	}

		// �`�挳�͈͐ݒ�i�͈͂��w�肳��Ă��Ȃ��ꍇ�j
		if (!_dp.src_area_flag)	{
			_dp.src_area.left   = -_dp.src_center.x + min(_dp.src_box.x1, _dp.src_box.x2);
			_dp.src_area.top    = -_dp.src_center.y + min(_dp.src_box.y1, _dp.src_box.y2);
			_dp.src_area.right  = -_dp.src_center.x + max(_dp.src_box.x1, _dp.src_box.x2);
			_dp.src_area.bottom = -_dp.src_center.y + max(_dp.src_box.y1, _dp.src_box.y2);
		}

		// ���C�����[�h�n�m
		if (_dp.src_box.type == 0 || _dp.src_box.type == 1)	{	line_mode = true;	}
	}

	// �g��k����]����
	_fdp.scale_rotate_flag = false;
	if (_dp.scale_width != 1000 || _dp.scale_height != 1000 || _dp.rotate != 0)	{
		_fdp.scale_rotate_flag = true;		// �t���O�n�m
	}

	// �`����W
	_dp.draw_pos.x += _dp.dst_center.x;
	_dp.draw_pos.y += _dp.dst_center.y;
	if (!_fdp.scale_rotate_flag && !line_mode)	{
		// ���ʏ�`��
		_dp.draw_pos.x -= _dp.src_center.x;
		_dp.draw_pos.y -= _dp.src_center.y;
	}

	// �`���͈͐ݒ�i�͈͂��w�肳��Ă��Ȃ��ꍇ�j
	if (!_dp.dst_area_flag)	{
		_dp.dst_area.left    = -_dp.dst_center.x;
		_dp.dst_area.top     = -_dp.dst_center.y;
		_dp.dst_area.right   = -_dp.dst_center.x + (_dp.dst_size.cx - 1);
		_dp.dst_area.bottom  = -_dp.dst_center.y + (_dp.dst_size.cy - 1);
	}

	// �`�挳�͈͕␳
	if (_dp.src_area.left > _dp.src_area.right || _dp.src_area.top > _dp.src_area.bottom)	{	return false;	}
	_dp.src_area.left    = max(                  0, _dp.src_area.left   + _dp.src_center.x);
	_dp.src_area.top     = max(                  0, _dp.src_area.top    + _dp.src_center.y);
	_dp.src_area.right   = min(_dp.src_size.cx - 1, _dp.src_area.right  + _dp.src_center.x);
	_dp.src_area.bottom  = min(_dp.src_size.cy - 1, _dp.src_area.bottom + _dp.src_center.y);
	if (
		_dp.src_area.left >  _dp.src_area.right		||	_dp.src_area.left >= _dp.src_size.cx	||
		_dp.src_area.top  >  _dp.src_area.bottom	||	_dp.src_area.top  >= _dp.src_size.cy
	)	{	return false;	}	// �`��͈͊O�Ȃ̂ł����Ń��^�[��

	// �`���͈͕␳
	_dp.dst_area.left    = max(                  0, _dp.dst_area.left   + _dp.dst_center.x);
	_dp.dst_area.top     = max(                  0, _dp.dst_area.top    + _dp.dst_center.y);
	_dp.dst_area.right   = min(_dp.dst_size.cx - 1, _dp.dst_area.right  + _dp.dst_center.x);
	_dp.dst_area.bottom  = min(_dp.dst_size.cy - 1, _dp.dst_area.bottom + _dp.dst_center.y);
	if (!_fdp.scale_rotate_flag && !line_mode)	{
		// ���ʏ�`��
		_dp.dst_area.left    = max(_dp.src_area.left   + _dp.draw_pos.x, _dp.dst_area.left);
		_dp.dst_area.top     = max(_dp.src_area.top    + _dp.draw_pos.y, _dp.dst_area.top);
		_dp.dst_area.right   = min(_dp.src_area.right  + _dp.draw_pos.x, _dp.dst_area.right);
		_dp.dst_area.bottom  = min(_dp.src_area.bottom + _dp.draw_pos.y, _dp.dst_area.bottom);
	}
	if (
		_dp.dst_area.left >  _dp.dst_area.right		||	_dp.dst_area.left >= _dp.dst_size.cx	||
		_dp.dst_area.top  >  _dp.dst_area.bottom	||	_dp.dst_area.top  >= _dp.dst_size.cy
	)	{	return false;	}	// �`��͈͊O�Ȃ̂ł����Ń��^�[��

	// �^�C��
	bool tile_mode = false;
	if (_dp.tile_ptr && _dp.tile_size.cx > 0 && _dp.tile_size.cy > 0)	{
		int tile_add_min;
		tile_mode = true;
		if (_dp.tile_shift < 0)		{	_dp.tile_shift = 0;	}
		else if(_dp.tile_shift > 8)	{	_dp.tile_shift = 8;	}
		tile_add_min = -(255 << _dp.tile_shift);
		if (_dp.tile_add < tile_add_min)	{	_dp.tile_add = tile_add_min;	}
		else if(_dp.tile_add > 255)			{	_dp.tile_add = 255;		}
		if (_dp.tile_add == tile_add_min)	{	return false;		}		// �`�悳��Ȃ��̂ł����Ń��^�[��
	}

	// �g��k����]
	if (_fdp.scale_rotate_flag)	{
		// �g��k���{��
		if (_dp.scale_width < -20000)		{	_dp.scale_width  = -20000;	}
		else if(_dp.scale_width > 20000)	{	_dp.scale_width  =  20000;	}
		if (_dp.scale_height < -20000)		{	_dp.scale_height = -20000;	}
		else if(_dp.scale_height > 20000)	{	_dp.scale_height =  20000;	}
		// ��]�p�x
		_dp.rotate %= 3600;		if (_dp.rotate < 0)	{	_dp.rotate += 3600;	}
	}

	// �`�悷��ꍇ�̂ݏ�������p�����[�^
	if (_dp.draw_flag)	{

		// �^�C��
		if (tile_mode)	{
			_fdp.tile_ptr     = _dp.tile_ptr;
			_fdp.tile_size    = _dp.tile_size;
			_fdp.tile_reverse = _dp.tile_reverse;
			_fdp.tile_shift   = _dp.tile_shift;
			_fdp.tile_add     = _dp.tile_add;
			_fdp.tile_xl_flow = _dp.tile_size.cx;
			_fdp.tile_yl_flow = _dp.tile_size.cy * _dp.tile_size.cx;
		}

		// �����x
		_fdp.tr4 = _dp.tr << 2;
		_fdp.tr_table_adr = Gnamalib_grp_table_tr_adr[_dp.tr];

		// �A���t�@�����p�����x
		_fdp.alphablend_a = _dp.tr;

		// �F��
		if (_dp.bright < -255)		{	_dp.bright = -255;		}	else if (_dp.bright > 255)		{	_dp.bright = 255;		}
		if (_dp.palette_r < -255)	{	_dp.palette_r = -255;	}	else if (_dp.palette_r > 255)	{	_dp.palette_r = 255;	}
		if (_dp.palette_g < -255)	{	_dp.palette_g = -255;	}	else if (_dp.palette_g > 255)	{	_dp.palette_g = 255;	}
		if (_dp.palette_b < -255)	{	_dp.palette_b = -255;	}	else if (_dp.palette_b > 255)	{	_dp.palette_b = 255;	}
		if(_dp.bright < 0){
			_fdp.bright_r = _dp.palette_r + (((255 + _dp.palette_r) * _dp.bright) / 255);
			_fdp.bright_g = _dp.palette_g + (((255 + _dp.palette_g) * _dp.bright) / 255);
			_fdp.bright_b = _dp.palette_b + (((255 + _dp.palette_b) * _dp.bright) / 255);
		}
		else if(_dp.bright > 0){
			_fdp.bright_r = _dp.palette_r + (((255 - _dp.palette_r) * _dp.bright) / 255);
			_fdp.bright_g = _dp.palette_g + (((255 - _dp.palette_g) * _dp.bright) / 255);
			_fdp.bright_b = _dp.palette_b + (((255 - _dp.palette_b) * _dp.bright) / 255);
		}
		else	{
			_fdp.bright_r = _dp.palette_r;
			_fdp.bright_g = _dp.palette_g;
			_fdp.bright_b = _dp.palette_b;
		}
		if (_dp.draw_mod == TRUE_COLOR_GRP_DRAW_MOD_SUB)	{		// ���Z���[�h
			_fdp.bright_r = -_fdp.bright_r;
			_fdp.bright_g = -_fdp.bright_g;
			_fdp.bright_b = -_fdp.bright_b;
		}
		if (_fdp.bright_r >= 0)	{	_fdp.bright_r_table_adr = Gnamalib_grp_table_tr_adr[255 - _fdp.bright_r];	_fdp.bright_r_flag = 255;	}
		else					{	_fdp.bright_r_table_adr = Gnamalib_grp_table_tr_adr[255 + _fdp.bright_r];	_fdp.bright_r_flag = 0;		}
		if (_fdp.bright_g >= 0)	{	_fdp.bright_g_table_adr = Gnamalib_grp_table_tr_adr[255 - _fdp.bright_g];	_fdp.bright_g_flag = 255;	}
		else					{	_fdp.bright_g_table_adr = Gnamalib_grp_table_tr_adr[255 + _fdp.bright_g];	_fdp.bright_g_flag = 0;		}
		if (_fdp.bright_b >= 0)	{	_fdp.bright_b_table_adr = Gnamalib_grp_table_tr_adr[255 - _fdp.bright_b];	_fdp.bright_b_flag = 255;	}
		else					{	_fdp.bright_b_table_adr = Gnamalib_grp_table_tr_adr[255 + _fdp.bright_b];	_fdp.bright_b_flag = 0;		}

		// �ϐF
		_fdp.color_r = _dp.color_r;
		_fdp.color_g = _dp.color_g;
		_fdp.color_b = _dp.color_b;
		_fdp.color_table_adr = Gnamalib_grp_table_tr_adr[255 - _dp.color_rate];

		// ���]
		_fdp.reverse_table_adr = Gnamalib_grp_table_tr_adr[255 - _dp.reverse];

		// ���m
		_fdp.mono = (255 - _dp.mono) * 4;

		// �t�B���^�[
		if (_dp.draw_mod == TRUE_COLOR_GRP_DRAW_MOD_FLT)	{
			_fdp.flt_r  = _dp.flt_r;
			_fdp.flt_g  = _dp.flt_g;
			_fdp.flt_b  = _dp.flt_b;
		}

		// �`��X�C�b�`
		_fdp.fs_sprite		= (_dp.src_type == 0)												? false : true; 
		_fdp.fs_tr			= (_dp.tr == 255)													? false : true; 
		_fdp.fs_bright		= (_fdp.bright_r == 0 && _fdp.bright_g == 0 && _fdp.bright_b == 0)	? false : true; 
		_fdp.fs_color		= (_dp.color_rate == 0)												? false : true; 
		_fdp.fs_reverse		= (_dp.reverse == 0)												? false : true; 
		_fdp.fs_mono		= (_dp.mono == 0)													? false : true; 
		_fdp.fs_tile		= (_dp.tile_ptr==0 || _dp.tile_add==255)							? false : true; 

		if (TRUE_COLOR_GRP_DRAW_MOD_STD <= _dp.draw_mod && _dp.draw_mod <= TRUE_COLOR_GRP_DRAW_MOD_BLD)	{
			_fdp.fs_draw_mod = _dp.draw_mod;
		}
		else	{
			_fdp.fs_draw_mod = TRUE_COLOR_GRP_DRAW_MOD_STD;
		}

		_fdp.fs_grp_or_box = grp_or_box;
	}

	return true;
}

// ****************************************************************
// �^�C���̕`��p�����[�^�ݒ�
void Cnamalib_grp::_tcgd_func_param_tile(int rep_x, int rep_y, bool now_ptr_add_x)
{
	int tmp;

	if (rep_x >= _dp.tile_center.x)	{
		_fdp.tile_now_pos.x = (rep_x - _dp.tile_center.x) % _dp.tile_size.cx;
	}
	else	{
		tmp = (_dp.tile_center.x - rep_x) % _dp.tile_size.cx;
		_fdp.tile_now_pos.x = (_dp.tile_size.cx - tmp) % _dp.tile_size.cx;
	}
	if (rep_y >= _dp.tile_center.y)	{
		_fdp.tile_now_pos.y = (rep_y - _dp.tile_center.y) % _dp.tile_size.cy;
	}
	else	{
		tmp = (_dp.tile_center.y - rep_y) % _dp.tile_size.cy;
		_fdp.tile_now_pos.y = (_dp.tile_size.cy - tmp) % _dp.tile_size.cy;
	}

	_fdp.tile_start_pos.x = _fdp.tile_now_pos.x;
	_fdp.tile_start_pos.y = _fdp.tile_now_pos.y;

	_fdp.tile_now_ptr = _dp.tile_ptr + (_dp.tile_size.cy * _fdp.tile_now_pos.y);	// �w���̃A�h���X�͉��Z���Ȃ�
	_fdp.tile_end_ptr = _dp.tile_ptr + (_dp.tile_size.cx * _dp.tile_size.cy);		// �w���̃A�h���X�͉��Z���Ȃ�

	if (now_ptr_add_x)	{
		_fdp.tile_now_ptr += _fdp.tile_now_pos.x;
	}
}

// ****************************************************************
// �W���̉摜�`��i�t�B���{�b�N�X�`��j
bool Cnamalib_grp::_tcgd_func_draw_graphic_standard(TCGD_EX_MOD ex_mod)
{
	int tmp;

	// �`��͈͊O�Ȃ̂ł����Ń��^�[��
	if (_dp.draw_pos.x > _dp.dst_area.right)	{	return false;	}
	if (_dp.draw_pos.y > _dp.dst_area.bottom)	{	return false;	}

	// �`�揈���p�����[�^�ݒ�
	if (_dp.draw_pos.x < 0)	{
		_fdp.src_jump_pixel_x_cnt = -_dp.draw_pos.x;
		_fdp.draw_pixel_x_cnt = _dp.src_size.cx - _fdp.src_jump_pixel_x_cnt;
		if (_fdp.draw_pixel_x_cnt <= 0)	{	return false;	}		// �`�搔���O�ȉ��Ȃ̂ł����Ń��^�[��
		_fdp.dst_jump_pixel_x_cnt = 0;
	}
	else	{
		_fdp.src_jump_pixel_x_cnt = 0;
		_fdp.draw_pixel_x_cnt = _dp.src_size.cx;
		_fdp.dst_jump_pixel_x_cnt = _dp.draw_pos.x;
	}
	if (_dp.draw_pos.y < 0)	{
		_fdp.src_jump_pixel_y_cnt = -_dp.draw_pos.y;
		_fdp.draw_pixel_y_cnt = _dp.src_size.cy - _fdp.src_jump_pixel_y_cnt;
		if (_fdp.draw_pixel_y_cnt <= 0)	{	return false;	}		// �`�搔���O�ȉ��Ȃ̂ł����Ń��^�[��
		_fdp.dst_jump_pixel_y_cnt = 0;
	}
	else	{
		_fdp.src_jump_pixel_y_cnt = 0;
		_fdp.draw_pixel_y_cnt = _dp.src_size.cy;
		_fdp.dst_jump_pixel_y_cnt = _dp.draw_pos.y;
	}

	// �`�揈���p�����[�^�C���i�`��͈͏C���j
	if (_fdp.dst_jump_pixel_x_cnt < _dp.dst_area.left)	{
		tmp = _dp.dst_area.left - _fdp.dst_jump_pixel_x_cnt;
		_fdp.src_jump_pixel_x_cnt += tmp;
		_fdp.draw_pixel_x_cnt -= tmp;
		_fdp.dst_jump_pixel_x_cnt = _dp.dst_area.left;
	}
	tmp = (_dp.dst_area.right - _fdp.dst_jump_pixel_x_cnt) + 1;
	if (_fdp.draw_pixel_x_cnt > tmp)	{	_fdp.draw_pixel_x_cnt = tmp;		}
	if (_fdp.draw_pixel_x_cnt <= 0)	{	return false;	}		// �`�搔���O�ȉ��Ȃ̂ł����Ń��^�[��

	if (_fdp.dst_jump_pixel_y_cnt < _dp.dst_area.top)		{
		tmp = _dp.dst_area.top - _fdp.dst_jump_pixel_y_cnt;
		_fdp.src_jump_pixel_y_cnt += tmp;
		_fdp.draw_pixel_y_cnt -= tmp;
		_fdp.dst_jump_pixel_y_cnt = _dp.dst_area.top;
	}
	tmp = (_dp.dst_area.bottom - _fdp.dst_jump_pixel_y_cnt) + 1;
	if (_fdp.draw_pixel_y_cnt > tmp)	{	_fdp.draw_pixel_y_cnt = tmp;		}
	if (_fdp.draw_pixel_y_cnt <= 0)	{	return false;	}		// �`�搔���O�ȉ��Ȃ̂ł����Ń��^�[��

	// ���^�[���p�̕`���`�ɃR�s�[
	if (_dp.ret_draw_area_flag && _dp.ret_draw_area)	{
		*_dp.ret_draw_area_flag    = true;
		_dp.ret_draw_area->left   = _fdp.dst_jump_pixel_x_cnt;
		_dp.ret_draw_area->right  = _dp.ret_draw_area->left + _fdp.draw_pixel_x_cnt - 1;
		_dp.ret_draw_area->top    = _fdp.dst_jump_pixel_y_cnt;
		_dp.ret_draw_area->bottom = _dp.ret_draw_area->top  + _fdp.draw_pixel_y_cnt - 1;
	}

	// �`�F�b�N�݂̂̏ꍇ
	if (!_dp.draw_flag)	{	return true;	}

	// �^�C���̕`��p�����[�^�ݒ�
	if (_dp.tile_ptr)	{	_tcgd_func_param_tile(_fdp.dst_jump_pixel_x_cnt, _fdp.dst_jump_pixel_y_cnt, false);	}

	// �`��A�h���X�ݒ�
	_fdp.dst_draw_ptr = _dp.dst_ptr + (((_fdp.dst_jump_pixel_y_cnt * _dp.dst_size.cx) + _fdp.dst_jump_pixel_x_cnt) << 2);
	if (!_fdp.fs_grp_or_box)	{
		_fdp.src_draw_ptr = _dp.src_ptr + (((_fdp.src_jump_pixel_y_cnt * _dp.src_size.cx) + _fdp.src_jump_pixel_x_cnt) << 2);
	}
	else{
		_fdp.src_draw_ptr = _dp.src_ptr;
	}

	// ���̃A�h���X�ւ̉��Z�o�C�g��
	_fdp.dst_next_adr_add_byte = (_dp.dst_size.cx - _fdp.draw_pixel_x_cnt) << 2;
	if (!_fdp.fs_grp_or_box)	{
		_fdp.src_next_adr_add_byte = (_dp.src_size.cx - _fdp.draw_pixel_x_cnt) << 2;
	}

	// �`��
	namalib_grp_tcgd_func_draw_switch(&_fdp, 0, ex_mod);

	return true;
}

// ****************************************************************
// �g��k����]�̉摜�`��i�t�B���{�b�N�X�`��j
bool Cnamalib_grp::_tcgd_func_draw_graphic_scale_rotate(TCGD_EX_MOD ex_mod)
{
	POINT vertex[4];
	int i;

	// �g��k���{���� double �^�ɂ���
	double double_scale_width  = (double)_dp.scale_width  / 1000;
	double double_scale_height = (double)_dp.scale_height / 1000;

	// �t��]�p�x
	int reverse_rotate = 3600 - _dp.rotate;

	// �`�挳�̒��S���W�� (0, 0) �Ƃ������W�ɕϊ�
	vertex[0].x = (_dp.src_area.left  - _dp.src_center.x);		vertex[0].y = (_dp.src_area.top    - _dp.src_center.y);
	vertex[1].x = (_dp.src_area.right - _dp.src_center.x);		vertex[1].y = (_dp.src_area.top    - _dp.src_center.y);
	vertex[2].x = (_dp.src_area.right - _dp.src_center.x);		vertex[2].y = (_dp.src_area.bottom - _dp.src_center.y);
	vertex[3].x = (_dp.src_area.left  - _dp.src_center.x);		vertex[3].y = (_dp.src_area.bottom - _dp.src_center.y);

	// ����]����̏ꍇ
	if (_dp.rotate != 0)	{
		double s_x, s_y, x[4], y[4];

		for (i = 0; i < 4; i++)	{
			// �{��
			s_x = vertex[i].x * double_scale_width;
			s_y = vertex[i].y * double_scale_height;
			// ��]
			x[i] = (int)((s_x * Gnamalib_grp_table_rotate_cos[_dp.rotate]) - (s_y * Gnamalib_grp_table_rotate_sin[_dp.rotate]));
			y[i] = (int)((s_x * Gnamalib_grp_table_rotate_sin[_dp.rotate]) + (s_y * Gnamalib_grp_table_rotate_cos[_dp.rotate]));
			// �`���̍��W�ɕϊ�
			x[i] += _dp.draw_pos.x;
			y[i] += _dp.draw_pos.y;
		}
		// ��ԏ�̍��W��T��
		int top = 0;
		double tmp = y[0];
		for (i = 1; i < 4; i++)	{
			if (tmp > y[i]) {	tmp = y[i];		top = i;	}
		}
		// ���W����ёւ���
		for (i = 0; i < 4; i++){
			vertex[i].x = (int)x[top];
			vertex[i].y = (int)y[top];
			top = (top + 1) & 0x03;
		}
	}

	// ����]�Ȃ��̏ꍇ
	else	{
		double x[4], y[4];

		for (i = 0; i < 4; i++)	{
			// �{��
			x[i] = vertex[i].x * double_scale_width;
			y[i] = vertex[i].y * double_scale_height;
			// �`���̍��W�ɕϊ�
			x[i] += _dp.draw_pos.x;
			y[i] += _dp.draw_pos.y;
		}
		// ���W����ёւ���
		if (_dp.scale_height >= 0)	{
			for (i = 0; i < 4; i++)	{
				vertex[i].x = (int)x[i];
				vertex[i].y = (int)y[i];
			}
		}
		else	{
			for (i = 0; i < 4; i++)	{
				vertex[i].x = (int)x[3 - i];
				vertex[i].y = (int)y[3 - i];
			}
		}
	}

	// �`���`�Z�o
	RECT draw_area;
	draw_area.left   = vertex[0].x;
	draw_area.top    = vertex[0].y;
	draw_area.right  = vertex[0].x;
	draw_area.bottom = vertex[0].y;
	for (i = 1; i < 4; i++)	{
		if (draw_area.left   > vertex[i].x)	{	draw_area.left   = vertex[i].x;		}
		if (draw_area.top    > vertex[i].y)	{	draw_area.top    = vertex[i].y;		}
		if (draw_area.right  < vertex[i].x)	{	draw_area.right  = vertex[i].x;		}
		if (draw_area.bottom < vertex[i].y)	{	draw_area.bottom = vertex[i].y;		}
	}
	if (draw_area.left   < _dp.dst_area.left)	{	draw_area.left   = _dp.dst_area.left;	}
	if (draw_area.top    < _dp.dst_area.top)	{	draw_area.top    = _dp.dst_area.top;	}
	if (draw_area.right  > _dp.dst_area.right)	{	draw_area.right  = _dp.dst_area.right;	}
	if (draw_area.bottom > _dp.dst_area.bottom)	{	draw_area.bottom = _dp.dst_area.bottom;	}

	// �\���͈͊O�`�F�b�N
	if (draw_area.left   > _dp.dst_area.right)	{	return false;	}
	if (draw_area.top    > _dp.dst_area.bottom)	{	return false;	}
	if (draw_area.right  < _dp.dst_area.left)	{	return false;	}
	if (draw_area.bottom < _dp.dst_area.top)	{	return false;	}

	// ���^�[���p�̕`���`�ɃR�s�[
	if (_dp.ret_draw_area_flag && _dp.ret_draw_area)	{
		*_dp.ret_draw_area_flag   = true;
		_dp.ret_draw_area->left   = draw_area.left;
		_dp.ret_draw_area->top    = draw_area.top;
		_dp.ret_draw_area->right  = draw_area.right;
		_dp.ret_draw_area->bottom = draw_area.bottom;
	}

	// �`�F�b�N�݂̂̏ꍇ
	if (!_dp.draw_flag)	{	return true;	}

	// ����]����̏ꍇ
	if(_dp.rotate != 0){
		int x1, x2, y;

		// �x����̃��C���o�b�t�@�����O
		std::vector<int> lb_buf[4];
		int lb_cnt[4], next;
		for (i = 0; i < 4; i++)	{
			next = (i + 1) & 0x03;
			if (vertex[i].y <= vertex[next].y)	{
				lb_cnt[i] = (vertex[next].y - vertex[i].y) + 1;		lb_buf[i].resize(lb_cnt[i] * 2);
				y_line_buffering(&lb_buf[i][0], lb_cnt[i], vertex[i].x, vertex[i].y, vertex[next].x, vertex[next].y, 1);
			}
			else						{
				lb_cnt[i] = (vertex[i].y - vertex[next].y) + 1;		lb_buf[i].resize(lb_cnt[i] * 2);
				y_line_buffering(&lb_buf[i][0], lb_cnt[i], vertex[i].x, vertex[i].y, vertex[next].x, vertex[next].y, 0);
			}
		}

		// ���C�������̃A�h���X�ݒ�
		int *lb_p1, *lb_p2;
		int lb_cnt1, lb_cnt2;
		int lb_flag1, lb_flag2;
		lb_flag1 = 0;	lb_cnt1 = lb_cnt[0];	lb_p1 = &lb_buf[0][0];
		lb_flag2 = 0;	lb_cnt2 = lb_cnt[3];	lb_p2 = &lb_buf[3][0] + ((lb_cnt[3] - 1) * 2);

		// ���C������
		while (1)	{
			if (lb_cnt1 == 0)	{
				if (lb_flag1)	{	break;	}	lb_flag1 = 1;	lb_cnt1 = lb_cnt[1];	lb_p1 = &lb_buf[1][0];
				lb_cnt1--;	if (lb_cnt1 == 0)	{	break;	}	lb_p1 += 2;
			}
			if (lb_cnt2 == 0)	{
				if (lb_flag2)	{	break;	}	lb_flag2 = 1;	lb_cnt2 = lb_cnt[2];	lb_p2 = &lb_buf[2][0] + ((lb_cnt[2] - 1) * 2);
				lb_cnt2--;	if (lb_cnt2 == 0)	{	break;	}	lb_p2 -= 2;
			}

			y = *(lb_p1 + 1);	if (y != *(lb_p2 + 1))	{	break;	}

			if (y >= _dp.dst_area.top && y <= _dp.dst_area.bottom)	{

				x1 = *lb_p1;	x2 = *lb_p2;	if (x1 > x2)	{	int swap_tmp;	swap_tmp = x1;	x1 = x2;	x2 = swap_tmp;	}

				if(x1 <= _dp.dst_area.right && x2 >= _dp.dst_area.left){
					// �`�挳�̍��W�ɖ߂�
					double xx1 = x1 - _dp.draw_pos.x;
					double xx2 = x2 - _dp.draw_pos.x;
					double yy  = y  - _dp.draw_pos.y;
					// �t��]
					double xxx1 = (xx1 * Gnamalib_grp_table_rotate_cos[reverse_rotate]) - (yy * Gnamalib_grp_table_rotate_sin[reverse_rotate]);
					double yyy1 = (xx1 * Gnamalib_grp_table_rotate_sin[reverse_rotate]) + (yy * Gnamalib_grp_table_rotate_cos[reverse_rotate]);
					double xxx2 = (xx2 * Gnamalib_grp_table_rotate_cos[reverse_rotate]) - (yy * Gnamalib_grp_table_rotate_sin[reverse_rotate]);
					double yyy2 = (xx2 * Gnamalib_grp_table_rotate_sin[reverse_rotate]) + (yy * Gnamalib_grp_table_rotate_cos[reverse_rotate]);
					// �{��
					xxx1 /= double_scale_width;		yyy1 /= double_scale_height;
					xxx2 /= double_scale_width;		yyy2 /= double_scale_height;
					// �`�挳�̒��S���W�� (0, 0) �Ƃ������W�ɕϊ������̂����ɖ߂�
					_fdp.lbdp_x1 = (int)(xxx1 + _dp.src_center.x);
					_fdp.lbdp_y1 = (int)(yyy1 + _dp.src_center.y);
					_fdp.lbdp_x2 = (int)(xxx2 + _dp.src_center.x);
					_fdp.lbdp_y2 = (int)(yyy2 + _dp.src_center.y);
					// �`���`����W
					_fdp.lbdp_dst_draw_pos.x = x1;	_fdp.lbdp_dst_draw_pos.y = y;
					// �`���`��A�h���X
					_fdp.lbdp_dst_draw_ptr  = _dp.dst_ptr + (((_dp.dst_size.cx * y) + x1) * 4);
					// �`���`��T�C�Y
					_fdp.lbdp_dst_draw_size = x2 - x1 + 1;
					// �]����̃G���A�␳
					if (x1 < _dp.dst_area.left)		{	x1 = _dp.dst_area.left;		}
					if (x2 > _dp.dst_area.right)	{	x2 = _dp.dst_area.right;	}
					// �`���ŏ��A�h���X	
					_fdp.lbdp_dst_draw_min_ptr = _dp.dst_ptr + (((_dp.dst_size.cx * y) + x1) * 4);
					// �`���ő�A�h���X	
					_fdp.lbdp_dst_draw_max_ptr = _dp.dst_ptr + (((_dp.dst_size.cx * y) + x2) * 4);
					// �`��
					_tcgd_func_draw_graphic_y_line(ex_mod);
				}
			}

			lb_p1 += 2;		lb_p2 -= 2;		lb_cnt1--;		lb_cnt2--;
		}
	}

	// ����]�Ȃ��̏ꍇ
	else{
		int x1, x2, y;

		if (_dp.scale_width >= 0)	{	x1 = vertex[0].x;	x2 = vertex[1].x;	}
		else						{	x1 = vertex[1].x;	x2 = vertex[0].x;	}

		if(x1 <= _dp.dst_area.right && x2 >= _dp.dst_area.left){

			// �`���`��T�C�Y
			_fdp.lbdp_dst_draw_size = x2 - x1 + 1;

			// �]����̃G���A�␳
			int xx1 = x1;
			int xx2 = x2;
			if (xx1 < _dp.dst_area.left)	{	xx1 = _dp.dst_area.left;	}
			if (xx2 > _dp.dst_area.right)	{	xx2 = _dp.dst_area.right;	}

			if (_dp.scale_width >= 0)	{	_fdp.lbdp_x1 = _dp.src_area.left;	_fdp.lbdp_x2 = _dp.src_area.right;	}
			else						{	_fdp.lbdp_x1 = _dp.src_area.right;	_fdp.lbdp_x2 = _dp.src_area.left;	}
			if(_dp.scale_height >= 0)	{	_fdp.lbdp_y1 = _dp.src_area.top;	_fdp.lbdp_y2 = _dp.src_area.top;	}
			else						{	_fdp.lbdp_y1 = _dp.src_area.bottom;	_fdp.lbdp_y2 = _dp.src_area.bottom;	}

			int dst_y_len  = vertex[3].y - vertex[0].y + 1;
			int src_y_len  = _dp.src_area.bottom - _dp.src_area.top + 1;
			int draw_alpha = 0;

			y = vertex[0].y;

			for(i=0; i<dst_y_len; i++){
				if (y >= _dp.dst_area.top && y <= _dp.dst_area.bottom)	{
					// �^�C���`���`����W
					_fdp.lbdp_dst_draw_pos.x = x1;	_fdp.lbdp_dst_draw_pos.y = y;
					// �`���`��A�h���X
					_fdp.lbdp_dst_draw_ptr  = _dp.dst_ptr + (((_dp.dst_size.cx * y) + x1) * 4);
					// �`���ŏ��A�h���X	
					_fdp.lbdp_dst_draw_min_ptr = _dp.dst_ptr + (((_dp.dst_size.cx * y) + xx1) * 4);
					// �`���ő�A�h���X	
					_fdp.lbdp_dst_draw_max_ptr = _dp.dst_ptr + (((_dp.dst_size.cx * y) + xx2) * 4);
					// �`��
					_tcgd_func_draw_graphic_y_line(ex_mod);
				}

				draw_alpha += src_y_len;
				if (_dp.scale_height >= 0)	{
					while (1)	{	if (draw_alpha < dst_y_len)	{	break;	}	draw_alpha -= dst_y_len;	_fdp.lbdp_y1++;	_fdp.lbdp_y2++;	}
				}																										
				else	{																								
					while (1)	{	if (draw_alpha < dst_y_len)	{	break;	}	draw_alpha -= dst_y_len;	_fdp.lbdp_y1--;	_fdp.lbdp_y2--;	}
				}
				y++;
			}
		}
	}

	return true;
}

// ****************************************************************
// �x����̃��C�����`��
void Cnamalib_grp::_tcgd_func_draw_graphic_y_line(TCGD_EX_MOD ex_mod)
{
	int x1, y1, x2, y2, xl, yl, xp, yp, top, i;

	// �N���b�v
	x1 = _fdp.lbdp_x1;	y1 = _fdp.lbdp_y1;	x2 = _fdp.lbdp_x2;	y2 = _fdp.lbdp_y2;
	if (!_fdp.fs_grp_or_box)	{
		if (!box_cliping(&x1, &y1, &x2, &y2, _dp.src_area.left, _dp.src_area.top, _dp.src_area.right, _dp.src_area.bottom))	{	return;	}
	}

	// �擾�o�b�t�@�T�C�Y�Z�o
	xl = _fdp.lbdp_x2 - _fdp.lbdp_x1;	if (xl < 0)	{	xl = -xl;	}	xl++;	// ++ = ����
	yl = _fdp.lbdp_y2 - _fdp.lbdp_y1;	if (yl < 0)	{	yl = -yl;	}	yl++;	// ++ = ����

	// �`��s�N�Z���� �� �擪�̎擾���Ȃ��s�N�Z�������Z�o
	if (xl > yl)	{	_fdp.lbdp_dst_draw_cnt = xl;	top = _fdp.lbdp_x1 - x1;	}
	else			{	_fdp.lbdp_dst_draw_cnt = yl;	top = _fdp.lbdp_y1 - y1;	}

	// �]����o�b�t�@�̐擪�A�h���X���Z�o
	_fdp.lbdp_dst_alpha = -_fdp.lbdp_dst_draw_cnt;
	if(top != 0){
		if (top < 0)	{	top = -top;		}
		for (i = 0; i < top; i++)	{
			_fdp.lbdp_dst_alpha += _fdp.lbdp_dst_draw_size;
			while (_fdp.lbdp_dst_alpha >= 0)	{
				_fdp.lbdp_dst_draw_pos.x++;
				_fdp.lbdp_dst_draw_ptr += 4;
				_fdp.lbdp_dst_alpha -= _fdp.lbdp_dst_draw_cnt;
			}
		}
	}

	// �^�C���̕`��p�����[�^�ݒ�
	if (_dp.tile_ptr)	{	_tcgd_func_param_tile(_fdp.lbdp_dst_draw_pos.x, _fdp.lbdp_dst_draw_pos.y, false);	}

	// �A�h���X�Z�o
	if (!_fdp.fs_grp_or_box)	{
		_fdp.lbdp_src_draw_ptr = _dp.src_ptr + ((x1 + (_dp.src_size.cx * y1)) * 4);
	}
	else{
		_fdp.lbdp_src_draw_ptr = _dp.src_ptr;
	}

	// �`��p�����[�^
	xp = 1 * 4;						// �i�A�h���X�v�Z�ׂ̈ɂS�{�ɂ��Ă���j
	yp = _dp.src_size.cx * 4;		// �i�A�h���X�v�Z�ׂ̈ɂS�{�ɂ��Ă���j
	xl = x2 - x1;	if (xl < 0)	{	xl = -xl;	xp = -xp;	}
	yl = y2 - y1;	if (yl < 0)	{	yl = -yl;	yp = -yp;	}

	if (yl == 0)	{
		// �_
		if (xl == 0)	{
			_fdp.lbdp_sw    =  0;
			_fdp.lbdp_ns    =  0;
			_fdp.lbdp_cnt   =  0;
			_fdp.lbdp_alpha =  0;
		}
		// ����
		else				{
			_fdp.lbdp_sw    =  0;
			_fdp.lbdp_ns    =  xp;
			_fdp.lbdp_cnt   =  xl;
			_fdp.lbdp_alpha =  xp;
		}
	}
	else{
		// ����
		if (xl == 0)	{
			_fdp.lbdp_sw    =  0;
			_fdp.lbdp_ns    =  yp;
			_fdp.lbdp_cnt   =  yl;
			_fdp.lbdp_alpha =  yp;
		}
		// �΂�
		else				{
			if(xl > yl){
				_fdp.lbdp_sw    =  1;
				_fdp.lbdp_ns    = -xl;
				_fdp.lbdp_na    =  yl * 2;
				_fdp.lbdp_nh    =  xl * 2;
				_fdp.lbdp_cnt   =  xl;
				_fdp.lbdp_alpha =  xp;
				_fdp.lbdp_beta  =  yp;
			}
			else{
				_fdp.lbdp_sw    =  1;
				_fdp.lbdp_ns    = -yl;
				_fdp.lbdp_na    =  xl * 2;
				_fdp.lbdp_nh    =  yl * 2;
				_fdp.lbdp_cnt   =  yl;
				_fdp.lbdp_alpha =  yp;
				_fdp.lbdp_beta  =  xp;
			}
		}
	}

	// �`��
	namalib_grp_tcgd_func_draw_switch(&_fdp, 1, ex_mod);
}

// ****************************************************************
// ���C���`��i�g��k����]�ɂ��Ή��j
bool Cnamalib_grp::_tcgd_func_draw_line(int x1, int y1, int x2, int y2, TCGD_EX_MOD ex_mod)
{
	// �`�挳�N���b�v
	if (!box_cliping(&x1, &y1, &x2, &y2, _dp.src_area.left, _dp.src_area.top, _dp.src_area.right, _dp.src_area.bottom))	{	return false;	}

	// �`�挳�̒��S���W�� (0, 0) �Ƃ������W�ɕϊ�
	x1 -= _dp.src_center.x;
	y1 -= _dp.src_center.y;
	x2 -= _dp.src_center.x;
	y2 -= _dp.src_center.y;

	// �g��k����]�ϊ�
	if (_fdp.scale_rotate_flag)	{
		// �g��k���{���� double �^�ɂ���
		double double_scale_width  = (double)_dp.scale_width  / 1000;
		double double_scale_height = (double)_dp.scale_height / 1000;

		// �{�� ��]
		if (_dp.rotate != 0)	{
			double s_x1 = x1 * double_scale_width;
			double s_y1 = y1 * double_scale_height;
			double s_x2 = x2 * double_scale_width;
			double s_y2 = y2 * double_scale_height;
			x1 = (int)((s_x1 * Gnamalib_grp_table_rotate_cos[_dp.rotate]) - (s_y1 * Gnamalib_grp_table_rotate_sin[_dp.rotate]));
			y1 = (int)((s_x1 * Gnamalib_grp_table_rotate_sin[_dp.rotate]) + (s_y1 * Gnamalib_grp_table_rotate_cos[_dp.rotate]));
			x2 = (int)((s_x2 * Gnamalib_grp_table_rotate_cos[_dp.rotate]) - (s_y2 * Gnamalib_grp_table_rotate_sin[_dp.rotate]));
			y2 = (int)((s_x2 * Gnamalib_grp_table_rotate_sin[_dp.rotate]) + (s_y2 * Gnamalib_grp_table_rotate_cos[_dp.rotate]));
		}
		else	{
			x1 = (int)(x1 * double_scale_width);
			y1 = (int)(y1 * double_scale_height);
			x2 = (int)(x2 * double_scale_width);
			y2 = (int)(y2 * double_scale_height);
		}
	}

	// �`����W�ɕϊ�
	x1 += _dp.draw_pos.x;
	y1 += _dp.draw_pos.y;
	x2 += _dp.draw_pos.x;
	y2 += _dp.draw_pos.y;

	// �`���N���b�v
	if (!box_cliping(&x1, &y1, &x2, &y2, _dp.dst_area.left, _dp.dst_area.top, _dp.dst_area.right, _dp.dst_area.bottom))	{	return false;	}

	// ���^�[���p�̕`���`�ɃR�s�[
	if (_dp.ret_draw_area_flag && _dp.ret_draw_area)	{
		*_dp.ret_draw_area_flag   = true;
		_dp.ret_draw_area->left   = min(x1, x2);
		_dp.ret_draw_area->top    = min(y1, y2);
		_dp.ret_draw_area->right  = max(x1, x2);
		_dp.ret_draw_area->bottom = max(y1, y2);
	}

	// �`�F�b�N�݂̂̏ꍇ
	if (!_dp.draw_flag)	{	return true;	}

	// �^�C���̕`��p�����[�^�ݒ�
	if (_dp.tile_ptr)	{	_tcgd_func_param_tile(x1, y1, true);	}

	// ���C���o�b�t�@�����O
	std::vector<int> lb_buf;
	int lb_cnt, xl, yl;
	if (x1 <= x2)	{	xl = x2 - x1 + 1;	}
	else			{	xl = x1 - x2 + 1;	}
	if (y1 <= y2)	{	yl = y2 - y1 + 1;	}
	else			{	yl = y1 - y2 + 1;	}
	lb_cnt = max(xl, yl);
	lb_buf.resize(lb_cnt * 2);
	line_buffering2(&lb_buf[0], lb_cnt, x1, y1, x2, y2);

	_fdp.linedp_dst_draw_ptr = _dp.dst_ptr + (((y1 * _dp.dst_size.cx) + x1) << 2);
	_fdp.linedp_src_draw_ptr = _dp.src_ptr;
	_fdp.linedp_line_buffer_ptr = &lb_buf[0];
	_fdp.linedp_line_buffer_cnt = lb_cnt;
	_fdp.linedp_dst_add_x = 4;
	_fdp.linedp_dst_add_y = _dp.dst_size.cx * 4;

	// �`��
	namalib_grp_tcgd_func_draw_switch(&_fdp, 2, ex_mod);

	return true;
}

// ****************************************************************
// �G���v�e�B�[�{�b�N�X�`��i�g��k����]�ɂ��Ή��j
bool Cnamalib_grp::_tcgd_func_draw_empty_box(int x1, int y1, int x2, int y2, TCGD_EX_MOD ex_mod)
{
	bool ret = false;
	int xl, yl;
	bool ret_draw_area_enable;
	RECT ret_draw_area;
	bool ret_draw_area_flag;

	ret_draw_area_enable = false;
	if (_dp.ret_draw_area_flag && _dp.ret_draw_area)	{
		ret_draw_area_enable = true;
	}

	if (x1 > x2)	{	int swap_tmp;	swap_tmp = x1;	x1 = x2;	x2 = swap_tmp;	}	xl = x2 - x1 + 1;
	if (y1 > y2)	{	int swap_tmp;	swap_tmp = y1;	y1 = y2;	y2 = swap_tmp;	}	yl = y2 - y1 + 1;

	// �`���`�̏�����
	if (ret_draw_area_enable)	{
		ret_draw_area.left   = _dp.ret_draw_area->left;
		ret_draw_area.top    = _dp.ret_draw_area->top;
		ret_draw_area.right  = _dp.ret_draw_area->right;
		ret_draw_area.bottom = _dp.ret_draw_area->bottom;
		ret_draw_area_flag = false;
	}

	// ���
	if (_tcgd_func_draw_line(x1, y1, x2, y1, ex_mod))	{
		if (ret_draw_area_enable)	{
			ret_draw_area.left   = _dp.ret_draw_area->left;
			ret_draw_area.top    = _dp.ret_draw_area->top;
			ret_draw_area.right  = _dp.ret_draw_area->right;
			ret_draw_area.bottom = _dp.ret_draw_area->bottom;
			ret_draw_area_flag = true;
		}
		ret = true;
	}

	if (yl > 1)	{
		// ����
		if (_tcgd_func_draw_line(x1, y2, x2, y2, ex_mod))	{
			if (ret_draw_area_enable)	{
				if (!ret_draw_area_flag || ret_draw_area.left   > _dp.ret_draw_area->left)		{	ret_draw_area.left   = _dp.ret_draw_area->left;		}
				if (!ret_draw_area_flag || ret_draw_area.top    > _dp.ret_draw_area->top)		{	ret_draw_area.top    = _dp.ret_draw_area->top;		}
				if (!ret_draw_area_flag || ret_draw_area.right  < _dp.ret_draw_area->right)		{	ret_draw_area.right  = _dp.ret_draw_area->right;	}
				if (!ret_draw_area_flag || ret_draw_area.bottom < _dp.ret_draw_area->bottom)	{	ret_draw_area.bottom = _dp.ret_draw_area->bottom;	}
				ret_draw_area_flag = true;
			}
			ret = true;
		}

		if (yl > 2)	{
			// ����
			if (_tcgd_func_draw_line(x1, y1 + 1, x1, y2 - 1, ex_mod))	{
				if (ret_draw_area_enable)	{
					if (!ret_draw_area_flag || ret_draw_area.left   > _dp.ret_draw_area->left)		{	ret_draw_area.left   = _dp.ret_draw_area->left;		}
					if (!ret_draw_area_flag || ret_draw_area.top    > _dp.ret_draw_area->top)		{	ret_draw_area.top    = _dp.ret_draw_area->top;		}
					if (!ret_draw_area_flag || ret_draw_area.right  < _dp.ret_draw_area->right)		{	ret_draw_area.right  = _dp.ret_draw_area->right;	}
					if (!ret_draw_area_flag || ret_draw_area.bottom < _dp.ret_draw_area->bottom)	{	ret_draw_area.bottom = _dp.ret_draw_area->bottom;	}
					ret_draw_area_flag = true;
				}
				ret = true;
			}

			if (xl > 1)	{
				// �E��
				if (_tcgd_func_draw_line(x2, y1 + 1, x2, y2 - 1, ex_mod))	{
					if (ret_draw_area_enable)	{
						if (!ret_draw_area_flag || ret_draw_area.left   > _dp.ret_draw_area->left)		{	ret_draw_area.left   = _dp.ret_draw_area->left;		}
						if (!ret_draw_area_flag || ret_draw_area.top    > _dp.ret_draw_area->top)		{	ret_draw_area.top    = _dp.ret_draw_area->top;		}
						if (!ret_draw_area_flag || ret_draw_area.right  < _dp.ret_draw_area->right)		{	ret_draw_area.right  = _dp.ret_draw_area->right;	}
						if (!ret_draw_area_flag || ret_draw_area.bottom < _dp.ret_draw_area->bottom)	{	ret_draw_area.bottom = _dp.ret_draw_area->bottom;	}
						ret_draw_area_flag = true;
					}
					ret = true;
				}
			}
		}
	}

	// �`���`��߂�
	if (ret && ret_draw_area_enable)	{
		_dp.ret_draw_area->left   = ret_draw_area.left;
		_dp.ret_draw_area->top    = ret_draw_area.top;
		_dp.ret_draw_area->right  = ret_draw_area.right;
		_dp.ret_draw_area->bottom = ret_draw_area.bottom;
		*_dp.ret_draw_area_flag   = ret_draw_area_flag;
	}

	return ret;
}




