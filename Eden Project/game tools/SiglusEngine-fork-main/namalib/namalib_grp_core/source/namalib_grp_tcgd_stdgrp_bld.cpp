#include	"pch.h"

// 通常
void namalib_grp_tcgd_func_stdgrp_bld_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
#if 0
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_NORMAL
		__END_BLDSTDGRP_NORMAL
#else
		__DRAW_BLDSTDGRP_BLD_NORMAL_EX
#endif
	}
}

// スプライト
void namalib_grp_tcgd_func_stdgrp_bld_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_BRIGHT
		__END_BLDSTDGRP_NORMAL
	}
}

// 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_BRIGHT_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_BRIGHT_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_BRIGHT_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// 変色 通常
void namalib_grp_tcgd_func_stdgrp_bld_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_COLOR
		__END_BLDSTDGRP_NORMAL
	}
}

// 変色 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_COLOR_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// 変色 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_COLOR_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// 変色 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_COLOR_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// 変色 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_COLOR_BRIGHT
		__END_BLDSTDGRP_NORMAL
	}
}

// 変色 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// 反転 通常
void namalib_grp_tcgd_func_stdgrp_bld_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_REVERSE
		__END_BLDSTDGRP_NORMAL
	}
}

// 反転 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_REVERSE_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// 反転 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_REVERSE_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// 反転 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_REVERSE_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// 反転 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_REVERSE_BRIGHT
		__END_BLDSTDGRP_NORMAL
	}
}

// 反転 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// 反転 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// 反転 変色 通常
void namalib_grp_tcgd_func_stdgrp_bld_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_REVERSE_COLOR
		__END_BLDSTDGRP_NORMAL
	}
}

// 反転 変色 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_REVERSE_COLOR_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// 反転 変色 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_REVERSE_COLOR_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_REVERSE_COLOR_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// 反転 変色 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_REVERSE_COLOR_BRIGHT
		__END_BLDSTDGRP_NORMAL
	}
}

// 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// モノ 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_MONO
		__END_BLDSTDGRP_NORMAL
	}
}

// モノ スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_MONO_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// モノ 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_MONO_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// モノ 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_MONO_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// モノ 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_MONO_BRIGHT
		__END_BLDSTDGRP_NORMAL
	}
}

// モノ 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_MONO_BRIGHT_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// モノ 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_MONO_BRIGHT_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// モノ 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_MONO_BRIGHT_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// モノ 変色 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_MONO_COLOR
		__END_BLDSTDGRP_NORMAL
	}
}

// モノ 変色 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_MONO_COLOR_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// モノ 変色 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_MONO_COLOR_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// モノ 変色 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_MONO_COLOR_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// モノ 変色 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_MONO_COLOR_BRIGHT
		__END_BLDSTDGRP_NORMAL
	}
}

// モノ 変色 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_MONO_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// モノ 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_MONO_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// モノ 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_MONO_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// モノ 反転 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_MONO_REVERSE
		__END_BLDSTDGRP_NORMAL
	}
}

// モノ 反転 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_MONO_REVERSE_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// モノ 反転 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_MONO_REVERSE_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// モノ 反転 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_MONO_REVERSE_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// モノ 反転 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_MONO_REVERSE_BRIGHT
		__END_BLDSTDGRP_NORMAL
	}
}

// モノ 反転 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_MONO_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// モノ 反転 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_MONO_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// モノ 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_MONO_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// モノ 反転 変色 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_MONO_REVERSE_COLOR
		__END_BLDSTDGRP_NORMAL
	}
}

// モノ 反転 変色 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_MONO_REVERSE_COLOR_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// モノ 反転 変色 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_MONO_REVERSE_COLOR_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// モノ 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_MONO_REVERSE_COLOR_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// モノ 反転 変色 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_NORMAL
		__DRAW_BLD_MONO_REVERSE_COLOR_BRIGHT
		__END_BLDSTDGRP_NORMAL
	}
}

// モノ 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_SPRITE
		__DRAW_BLD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_SPRITE
	}
}

// モノ 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_NORMAL
		__DRAW_BLD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TR_NORMAL
	}
}

// モノ 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_mono_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TR_SPRITE
		__DRAW_BLD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TR_SPRITE
	}
}

// タイル 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL_EX
		__DRAW_BLD_TILE_NORMAL
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_BRIGHT
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_BRIGHT_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル 変色 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_COLOR
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル 変色 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_COLOR_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル 変色 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_COLOR_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル 変色 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_COLOR_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル 変色 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_COLOR_BRIGHT
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル 変色 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル 反転 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_REVERSE
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル 反転 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_REVERSE_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル 反転 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_REVERSE_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル 反転 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_REVERSE_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル 反転 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_REVERSE_BRIGHT
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル 反転 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル 反転 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル 反転 変色 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_REVERSE_COLOR
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル 反転 変色 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_REVERSE_COLOR_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル 反転 変色 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_REVERSE_COLOR_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_REVERSE_COLOR_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル 反転 変色 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_REVERSE_COLOR_BRIGHT
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル モノ 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_MONO
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル モノ スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_MONO_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル モノ 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_MONO_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル モノ 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_MONO_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル モノ 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_MONO_BRIGHT
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル モノ 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_MONO_BRIGHT_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル モノ 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_MONO_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル モノ 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_MONO_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル モノ 変色 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_MONO_COLOR
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル モノ 変色 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_MONO_COLOR_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル モノ 変色 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_MONO_COLOR_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル モノ 変色 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_MONO_COLOR_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル モノ 変色 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_MONO_COLOR_BRIGHT
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル モノ 変色 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_MONO_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル モノ 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_MONO_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル モノ 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_MONO_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_MONO_REVERSE
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル モノ 反転 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_MONO_REVERSE_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル モノ 反転 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_MONO_REVERSE_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_MONO_REVERSE_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_MONO_REVERSE_BRIGHT
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル モノ 反転 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_MONO_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル モノ 反転 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_MONO_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_MONO_REVERSE_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 変色 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_MONO_REVERSE_COLOR
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル モノ 反転 変色 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_MONO_REVERSE_COLOR_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル モノ 反転 変色 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_MONO_REVERSE_COLOR_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_MONO_REVERSE_COLOR_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 変色 色味 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_NORMAL
		__DRAW_BLD_TILE_MONO_REVERSE_COLOR_BRIGHT
		__END_BLDSTDGRP_TILE_NORMAL
	}
}

// タイル モノ 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_SPRITE
		__DRAW_BLD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_SPRITE
	}
}

// タイル モノ 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_NORMAL
		__DRAW_BLD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdgrp_bld_tile_mono_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_BLDSTDGRP_TILE_TR_SPRITE
		__DRAW_BLD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_BLDSTDGRP_TILE_TR_SPRITE
	}
}

