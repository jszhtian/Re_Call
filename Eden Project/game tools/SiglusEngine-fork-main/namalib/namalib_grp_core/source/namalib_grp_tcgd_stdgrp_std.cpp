#include	"pch.h"

// �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
#if 0
		__START_STDGRP_NORMAL
		__DRAW_STD_NORMAL
		__END_STDGRP_NORMAL
#else
		__DRAW_STDGRP_STD_NORMAL_EX
#endif
	}
}

// �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
#if 0
		__START_STDGRP_SPRITE
		__DRAW_STD_TR
		__END_STDGRP_SPRITE
#else
		__DRAW_STDGRP_STD_SPRITE_EX
#endif
	}
}

// ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_TR
		__END_STDGRP_TR_SPRITE
	}
}

// �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_BRIGHT
		__END_STDGRP_NORMAL
	}
}

// �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_BRIGHT_TR
		__END_STDGRP_SPRITE
	}
}

// �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_BRIGHT_TR
		__END_STDGRP_TR_NORMAL
	}
}

// �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_BRIGHT_TR
		__END_STDGRP_TR_SPRITE
	}
}

// �ϐF �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_COLOR
		__END_STDGRP_NORMAL
	}
}

// �ϐF �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_COLOR_TR
		__END_STDGRP_SPRITE
	}
}

// �ϐF ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_COLOR_TR
		__END_STDGRP_TR_NORMAL
	}
}

// �ϐF ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_COLOR_TR
		__END_STDGRP_TR_SPRITE
	}
}

// �ϐF �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_COLOR_BRIGHT
		__END_STDGRP_NORMAL
	}
}

// �ϐF �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_COLOR_BRIGHT_TR
		__END_STDGRP_SPRITE
	}
}

// �ϐF �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_COLOR_BRIGHT_TR
		__END_STDGRP_TR_NORMAL
	}
}

// �ϐF �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_COLOR_BRIGHT_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���] �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_REVERSE
		__END_STDGRP_NORMAL
	}
}

// ���] �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_REVERSE_TR
		__END_STDGRP_SPRITE
	}
}

// ���] ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_REVERSE_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���] ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_REVERSE_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���] �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_REVERSE_BRIGHT
		__END_STDGRP_NORMAL
	}
}

// ���] �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_REVERSE_BRIGHT_TR
		__END_STDGRP_SPRITE
	}
}

// ���] �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_REVERSE_BRIGHT_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���] �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_REVERSE_BRIGHT_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���] �ϐF �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_REVERSE_COLOR
		__END_STDGRP_NORMAL
	}
}

// ���] �ϐF �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_REVERSE_COLOR_TR
		__END_STDGRP_SPRITE
	}
}

// ���] �ϐF ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_REVERSE_COLOR_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���] �ϐF ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_REVERSE_COLOR_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���] �ϐF �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_REVERSE_COLOR_BRIGHT
		__END_STDGRP_NORMAL
	}
}

// ���] �ϐF �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_SPRITE
	}
}

// ���] �ϐF �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���] �ϐF �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���m �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_MONO
		__END_STDGRP_NORMAL
	}
}

// ���m �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_MONO_TR
		__END_STDGRP_SPRITE
	}
}

// ���m ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_MONO_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���m ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_MONO_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���m �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_MONO_BRIGHT
		__END_STDGRP_NORMAL
	}
}

// ���m �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_MONO_BRIGHT_TR
		__END_STDGRP_SPRITE
	}
}

// ���m �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_MONO_BRIGHT_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���m �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_MONO_BRIGHT_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���m �ϐF �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_MONO_COLOR
		__END_STDGRP_NORMAL
	}
}

// ���m �ϐF �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_MONO_COLOR_TR
		__END_STDGRP_SPRITE
	}
}

// ���m �ϐF ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_MONO_COLOR_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���m �ϐF ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_MONO_COLOR_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���m �ϐF �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_MONO_COLOR_BRIGHT
		__END_STDGRP_NORMAL
	}
}

// ���m �ϐF �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_MONO_COLOR_BRIGHT_TR
		__END_STDGRP_SPRITE
	}
}

// ���m �ϐF �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_MONO_COLOR_BRIGHT_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���m �ϐF �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_MONO_COLOR_BRIGHT_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���m ���] �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_MONO_REVERSE
		__END_STDGRP_NORMAL
	}
}

// ���m ���] �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_MONO_REVERSE_TR
		__END_STDGRP_SPRITE
	}
}

// ���m ���] ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_MONO_REVERSE_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���m ���] ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_MONO_REVERSE_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���m ���] �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_MONO_REVERSE_BRIGHT
		__END_STDGRP_NORMAL
	}
}

// ���m ���] �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_MONO_REVERSE_BRIGHT_TR
		__END_STDGRP_SPRITE
	}
}

// ���m ���] �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_MONO_REVERSE_BRIGHT_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���m ���] �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_MONO_REVERSE_BRIGHT_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���m ���] �ϐF �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_MONO_REVERSE_COLOR
		__END_STDGRP_NORMAL
	}
}

// ���m ���] �ϐF �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_MONO_REVERSE_COLOR_TR
		__END_STDGRP_SPRITE
	}
}

// ���m ���] �ϐF ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_MONO_REVERSE_COLOR_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���m ���] �ϐF ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_MONO_REVERSE_COLOR_TR
		__END_STDGRP_TR_SPRITE
	}
}

// ���m ���] �ϐF �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_NORMAL
		__DRAW_STD_MONO_REVERSE_COLOR_BRIGHT
		__END_STDGRP_NORMAL
	}
}

// ���m ���] �ϐF �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_SPRITE
		__DRAW_STD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_SPRITE
	}
}

// ���m ���] �ϐF �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_NORMAL
		__DRAW_STD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_TR_NORMAL
	}
}

// ���m ���] �ϐF �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_mono_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TR_SPRITE
		__DRAW_STD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_TR_SPRITE
	}
}

// �^�C�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL_EX
		__DRAW_STD_TILE_NORMAL
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_BRIGHT
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_BRIGHT_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_BRIGHT_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_BRIGHT_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� �ϐF �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_COLOR
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� �ϐF �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_COLOR_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� �ϐF ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_COLOR_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� �ϐF ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_COLOR_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� �ϐF �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_COLOR_BRIGHT
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� �ϐF �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� �ϐF �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� �ϐF �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���] �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_REVERSE
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���] �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_REVERSE_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���] ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_REVERSE_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���] ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_REVERSE_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���] �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_REVERSE_BRIGHT
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���] �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_REVERSE_BRIGHT_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���] �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_REVERSE_BRIGHT_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���] �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_REVERSE_BRIGHT_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���] �ϐF �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_REVERSE_COLOR
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���] �ϐF �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_REVERSE_COLOR_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���] �ϐF ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_REVERSE_COLOR_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���] �ϐF ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_REVERSE_COLOR_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���] �ϐF �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_REVERSE_COLOR_BRIGHT
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���] �ϐF �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���] �ϐF �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���] �ϐF �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���m �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_MONO
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���m �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_MONO_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���m ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���m ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���m �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_MONO_BRIGHT
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���m �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_MONO_BRIGHT_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���m �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_BRIGHT_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���m �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_BRIGHT_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���m �ϐF �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_MONO_COLOR
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���m �ϐF �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_MONO_COLOR_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���m �ϐF ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_COLOR_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���m �ϐF ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_COLOR_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���m �ϐF �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_MONO_COLOR_BRIGHT
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���m �ϐF �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_MONO_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���m �ϐF �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���m �ϐF �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���m ���] �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���m ���] �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���m ���] ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���m ���] ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���m ���] �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_BRIGHT
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���m ���] �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_BRIGHT_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���m ���] �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_BRIGHT_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���m ���] �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_BRIGHT_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���m ���] �ϐF �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_COLOR
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���m ���] �ϐF �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���m ���] �ϐF ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���m ���] �ϐF ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

// �^�C�� ���m ���] �ϐF �F�� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_BRIGHT
		__END_STDGRP_TILE_NORMAL
	}
}

// �^�C�� ���m ���] �ϐF �F�� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_SPRITE
	}
}

// �^�C�� ���m ���] �ϐF �F�� ���� �ʏ�
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_TR_NORMAL
	}
}

// �^�C�� ���m ���] �ϐF �F�� ���� �X�v���C�g
void namalib_grp_tcgd_func_stdgrp_std_tile_mono_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDGRP_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDGRP_TILE_TR_SPRITE
	}
}

