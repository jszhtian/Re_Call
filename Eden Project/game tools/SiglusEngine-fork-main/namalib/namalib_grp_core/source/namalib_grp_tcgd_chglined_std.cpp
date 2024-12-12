#include	"pch.h"

// 通常
void namalib_grp_tcgd_func_chglined_std_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_NORMAL
		__END_CHGLINED_NORMAL
	}
}

// スプライト
void namalib_grp_tcgd_func_chglined_std_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_TR
		__END_CHGLINED_SPRITE
	}
}

// 透明 通常
void namalib_grp_tcgd_func_chglined_std_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// 色味 通常
void namalib_grp_tcgd_func_chglined_std_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_BRIGHT
		__END_CHGLINED_NORMAL
	}
}

// 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_BRIGHT_TR
		__END_CHGLINED_SPRITE
	}
}

// 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_BRIGHT_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_BRIGHT_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// 変色 通常
void namalib_grp_tcgd_func_chglined_std_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_COLOR
		__END_CHGLINED_NORMAL
	}
}

// 変色 スプライト
void namalib_grp_tcgd_func_chglined_std_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_COLOR_TR
		__END_CHGLINED_SPRITE
	}
}

// 変色 透明 通常
void namalib_grp_tcgd_func_chglined_std_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_COLOR_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// 変色 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_COLOR_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// 変色 色味 通常
void namalib_grp_tcgd_func_chglined_std_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_COLOR_BRIGHT
		__END_CHGLINED_NORMAL
	}
}

// 変色 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_COLOR_BRIGHT_TR
		__END_CHGLINED_SPRITE
	}
}

// 変色 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_COLOR_BRIGHT_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_COLOR_BRIGHT_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// 反転 通常
void namalib_grp_tcgd_func_chglined_std_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_REVERSE
		__END_CHGLINED_NORMAL
	}
}

// 反転 スプライト
void namalib_grp_tcgd_func_chglined_std_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_REVERSE_TR
		__END_CHGLINED_SPRITE
	}
}

// 反転 透明 通常
void namalib_grp_tcgd_func_chglined_std_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_REVERSE_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// 反転 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_REVERSE_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// 反転 色味 通常
void namalib_grp_tcgd_func_chglined_std_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_REVERSE_BRIGHT
		__END_CHGLINED_NORMAL
	}
}

// 反転 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_REVERSE_BRIGHT_TR
		__END_CHGLINED_SPRITE
	}
}

// 反転 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_REVERSE_BRIGHT_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_REVERSE_BRIGHT_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// 反転 変色 通常
void namalib_grp_tcgd_func_chglined_std_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_REVERSE_COLOR
		__END_CHGLINED_NORMAL
	}
}

// 反転 変色 スプライト
void namalib_grp_tcgd_func_chglined_std_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_REVERSE_COLOR_TR
		__END_CHGLINED_SPRITE
	}
}

// 反転 変色 透明 通常
void namalib_grp_tcgd_func_chglined_std_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_REVERSE_COLOR_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_REVERSE_COLOR_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// 反転 変色 色味 通常
void namalib_grp_tcgd_func_chglined_std_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_REVERSE_COLOR_BRIGHT
		__END_CHGLINED_NORMAL
	}
}

// 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_SPRITE
	}
}

// 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// モノ 通常
void namalib_grp_tcgd_func_chglined_std_mono_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_MONO
		__END_CHGLINED_NORMAL
	}
}

// モノ スプライト
void namalib_grp_tcgd_func_chglined_std_mono_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_MONO_TR
		__END_CHGLINED_SPRITE
	}
}

// モノ 透明 通常
void namalib_grp_tcgd_func_chglined_std_mono_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_MONO_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// モノ 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_MONO_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// モノ 色味 通常
void namalib_grp_tcgd_func_chglined_std_mono_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_MONO_BRIGHT
		__END_CHGLINED_NORMAL
	}
}

// モノ 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_MONO_BRIGHT_TR
		__END_CHGLINED_SPRITE
	}
}

// モノ 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_mono_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_MONO_BRIGHT_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// モノ 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_MONO_BRIGHT_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// モノ 変色 通常
void namalib_grp_tcgd_func_chglined_std_mono_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_MONO_COLOR
		__END_CHGLINED_NORMAL
	}
}

// モノ 変色 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_MONO_COLOR_TR
		__END_CHGLINED_SPRITE
	}
}

// モノ 変色 透明 通常
void namalib_grp_tcgd_func_chglined_std_mono_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_MONO_COLOR_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// モノ 変色 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_MONO_COLOR_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// モノ 変色 色味 通常
void namalib_grp_tcgd_func_chglined_std_mono_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_MONO_COLOR_BRIGHT
		__END_CHGLINED_NORMAL
	}
}

// モノ 変色 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_MONO_COLOR_BRIGHT_TR
		__END_CHGLINED_SPRITE
	}
}

// モノ 変色 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_mono_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_MONO_COLOR_BRIGHT_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// モノ 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_MONO_COLOR_BRIGHT_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// モノ 反転 通常
void namalib_grp_tcgd_func_chglined_std_mono_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_MONO_REVERSE
		__END_CHGLINED_NORMAL
	}
}

// モノ 反転 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_MONO_REVERSE_TR
		__END_CHGLINED_SPRITE
	}
}

// モノ 反転 透明 通常
void namalib_grp_tcgd_func_chglined_std_mono_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_MONO_REVERSE_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// モノ 反転 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_MONO_REVERSE_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// モノ 反転 色味 通常
void namalib_grp_tcgd_func_chglined_std_mono_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_MONO_REVERSE_BRIGHT
		__END_CHGLINED_NORMAL
	}
}

// モノ 反転 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_MONO_REVERSE_BRIGHT_TR
		__END_CHGLINED_SPRITE
	}
}

// モノ 反転 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_mono_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_MONO_REVERSE_BRIGHT_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// モノ 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_MONO_REVERSE_BRIGHT_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// モノ 反転 変色 通常
void namalib_grp_tcgd_func_chglined_std_mono_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_MONO_REVERSE_COLOR
		__END_CHGLINED_NORMAL
	}
}

// モノ 反転 変色 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_MONO_REVERSE_COLOR_TR
		__END_CHGLINED_SPRITE
	}
}

// モノ 反転 変色 透明 通常
void namalib_grp_tcgd_func_chglined_std_mono_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_MONO_REVERSE_COLOR_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// モノ 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_MONO_REVERSE_COLOR_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// モノ 反転 変色 色味 通常
void namalib_grp_tcgd_func_chglined_std_mono_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_NORMAL
		__DRAW_STD_MONO_REVERSE_COLOR_BRIGHT
		__END_CHGLINED_NORMAL
	}
}

// モノ 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_SPRITE
		__DRAW_STD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_SPRITE
	}
}

// モノ 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_mono_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_NORMAL
		__DRAW_STD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_TR_NORMAL
	}
}

// モノ 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_mono_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TR_SPRITE
		__DRAW_STD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_TR_SPRITE
	}
}

// タイル 通常
void namalib_grp_tcgd_func_chglined_std_tile_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_NORMAL
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル スプライト
void namalib_grp_tcgd_func_chglined_std_tile_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル 色味 通常
void namalib_grp_tcgd_func_chglined_std_tile_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_BRIGHT
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_BRIGHT_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_BRIGHT_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_BRIGHT_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル 変色 通常
void namalib_grp_tcgd_func_chglined_std_tile_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_COLOR
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル 変色 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_COLOR_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル 変色 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_COLOR_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル 変色 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_COLOR_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル 変色 色味 通常
void namalib_grp_tcgd_func_chglined_std_tile_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_COLOR_BRIGHT
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル 変色 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル 変色 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル 反転 通常
void namalib_grp_tcgd_func_chglined_std_tile_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_REVERSE
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル 反転 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_REVERSE_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル 反転 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_REVERSE_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル 反転 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_REVERSE_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル 反転 色味 通常
void namalib_grp_tcgd_func_chglined_std_tile_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_REVERSE_BRIGHT
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル 反転 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_REVERSE_BRIGHT_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル 反転 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_REVERSE_BRIGHT_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_REVERSE_BRIGHT_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル 反転 変色 通常
void namalib_grp_tcgd_func_chglined_std_tile_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_REVERSE_COLOR
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル 反転 変色 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_REVERSE_COLOR_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル 反転 変色 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_REVERSE_COLOR_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_REVERSE_COLOR_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル 反転 変色 色味 通常
void namalib_grp_tcgd_func_chglined_std_tile_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_REVERSE_COLOR_BRIGHT
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル モノ 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_MONO
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル モノ スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_MONO_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル モノ 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル モノ 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル モノ 色味 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_MONO_BRIGHT
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル モノ 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_MONO_BRIGHT_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル モノ 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_BRIGHT_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル モノ 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_BRIGHT_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル モノ 変色 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_MONO_COLOR
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル モノ 変色 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_MONO_COLOR_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル モノ 変色 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_COLOR_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル モノ 変色 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_COLOR_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル モノ 変色 色味 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_MONO_COLOR_BRIGHT
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル モノ 変色 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_MONO_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル モノ 変色 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル モノ 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル モノ 反転 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル モノ 反転 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 色味 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_BRIGHT
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル モノ 反転 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_BRIGHT_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル モノ 反転 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_BRIGHT_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_BRIGHT_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 変色 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_COLOR
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル モノ 反転 変色 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル モノ 反転 変色 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 変色 色味 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_BRIGHT
		__END_CHGLINED_TILE_NORMAL
	}
}

// タイル モノ 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_SPRITE
	}
}

// タイル モノ 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_NORMAL
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_chglined_std_tile_mono_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_CHGLINED_TILE_TR_SPRITE
		__DRAW_STD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_CHGLINED_TILE_TR_SPRITE
	}
}

