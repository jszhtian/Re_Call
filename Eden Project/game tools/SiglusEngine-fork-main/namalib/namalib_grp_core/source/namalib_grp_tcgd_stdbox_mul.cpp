#include	"pch.h"

// 通常
void namalib_grp_tcgd_func_stdbox_mul_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_NORMAL
		__END_STDBOX_NORMAL
	}
}

// スプライト
void namalib_grp_tcgd_func_stdbox_mul_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_TR
		__END_STDBOX_SPRITE
	}
}

// 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_TR
		__END_STDBOX_TR_NORMAL
	}
}

// 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_TR
		__END_STDBOX_TR_SPRITE
	}
}

// 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_BRIGHT
		__END_STDBOX_NORMAL
	}
}

// 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_BRIGHT_TR
		__END_STDBOX_SPRITE
	}
}

// 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_BRIGHT_TR
		__END_STDBOX_TR_NORMAL
	}
}

// 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_BRIGHT_TR
		__END_STDBOX_TR_SPRITE
	}
}

// 変色 通常
void namalib_grp_tcgd_func_stdbox_mul_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_COLOR
		__END_STDBOX_NORMAL
	}
}

// 変色 スプライト
void namalib_grp_tcgd_func_stdbox_mul_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_COLOR_TR
		__END_STDBOX_SPRITE
	}
}

// 変色 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_COLOR_TR
		__END_STDBOX_TR_NORMAL
	}
}

// 変色 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_COLOR_TR
		__END_STDBOX_TR_SPRITE
	}
}

// 変色 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_COLOR_BRIGHT
		__END_STDBOX_NORMAL
	}
}

// 変色 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_COLOR_BRIGHT_TR
		__END_STDBOX_SPRITE
	}
}

// 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_COLOR_BRIGHT_TR
		__END_STDBOX_TR_NORMAL
	}
}

// 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_COLOR_BRIGHT_TR
		__END_STDBOX_TR_SPRITE
	}
}

// 反転 通常
void namalib_grp_tcgd_func_stdbox_mul_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_REVERSE
		__END_STDBOX_NORMAL
	}
}

// 反転 スプライト
void namalib_grp_tcgd_func_stdbox_mul_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_REVERSE_TR
		__END_STDBOX_SPRITE
	}
}

// 反転 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_REVERSE_TR
		__END_STDBOX_TR_NORMAL
	}
}

// 反転 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_REVERSE_TR
		__END_STDBOX_TR_SPRITE
	}
}

// 反転 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_REVERSE_BRIGHT
		__END_STDBOX_NORMAL
	}
}

// 反転 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_REVERSE_BRIGHT_TR
		__END_STDBOX_SPRITE
	}
}

// 反転 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_REVERSE_BRIGHT_TR
		__END_STDBOX_TR_NORMAL
	}
}

// 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_REVERSE_BRIGHT_TR
		__END_STDBOX_TR_SPRITE
	}
}

// 反転 変色 通常
void namalib_grp_tcgd_func_stdbox_mul_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_REVERSE_COLOR
		__END_STDBOX_NORMAL
	}
}

// 反転 変色 スプライト
void namalib_grp_tcgd_func_stdbox_mul_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_REVERSE_COLOR_TR
		__END_STDBOX_SPRITE
	}
}

// 反転 変色 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_REVERSE_COLOR_TR
		__END_STDBOX_TR_NORMAL
	}
}

// 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_REVERSE_COLOR_TR
		__END_STDBOX_TR_SPRITE
	}
}

// 反転 変色 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_REVERSE_COLOR_BRIGHT
		__END_STDBOX_NORMAL
	}
}

// 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_SPRITE
	}
}

// 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_TR_NORMAL
	}
}

// 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_TR_SPRITE
	}
}

// モノ 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_MONO
		__END_STDBOX_NORMAL
	}
}

// モノ スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_MONO_TR
		__END_STDBOX_SPRITE
	}
}

// モノ 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_MONO_TR
		__END_STDBOX_TR_NORMAL
	}
}

// モノ 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_MONO_TR
		__END_STDBOX_TR_SPRITE
	}
}

// モノ 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_MONO_BRIGHT
		__END_STDBOX_NORMAL
	}
}

// モノ 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_MONO_BRIGHT_TR
		__END_STDBOX_SPRITE
	}
}

// モノ 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_MONO_BRIGHT_TR
		__END_STDBOX_TR_NORMAL
	}
}

// モノ 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_MONO_BRIGHT_TR
		__END_STDBOX_TR_SPRITE
	}
}

// モノ 変色 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_MONO_COLOR
		__END_STDBOX_NORMAL
	}
}

// モノ 変色 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_MONO_COLOR_TR
		__END_STDBOX_SPRITE
	}
}

// モノ 変色 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_MONO_COLOR_TR
		__END_STDBOX_TR_NORMAL
	}
}

// モノ 変色 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_MONO_COLOR_TR
		__END_STDBOX_TR_SPRITE
	}
}

// モノ 変色 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_MONO_COLOR_BRIGHT
		__END_STDBOX_NORMAL
	}
}

// モノ 変色 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_MONO_COLOR_BRIGHT_TR
		__END_STDBOX_SPRITE
	}
}

// モノ 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_MONO_COLOR_BRIGHT_TR
		__END_STDBOX_TR_NORMAL
	}
}

// モノ 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_MONO_COLOR_BRIGHT_TR
		__END_STDBOX_TR_SPRITE
	}
}

// モノ 反転 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_MONO_REVERSE
		__END_STDBOX_NORMAL
	}
}

// モノ 反転 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_MONO_REVERSE_TR
		__END_STDBOX_SPRITE
	}
}

// モノ 反転 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_MONO_REVERSE_TR
		__END_STDBOX_TR_NORMAL
	}
}

// モノ 反転 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_MONO_REVERSE_TR
		__END_STDBOX_TR_SPRITE
	}
}

// モノ 反転 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_MONO_REVERSE_BRIGHT
		__END_STDBOX_NORMAL
	}
}

// モノ 反転 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_MONO_REVERSE_BRIGHT_TR
		__END_STDBOX_SPRITE
	}
}

// モノ 反転 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_MONO_REVERSE_BRIGHT_TR
		__END_STDBOX_TR_NORMAL
	}
}

// モノ 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_MONO_REVERSE_BRIGHT_TR
		__END_STDBOX_TR_SPRITE
	}
}

// モノ 反転 変色 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_MONO_REVERSE_COLOR
		__END_STDBOX_NORMAL
	}
}

// モノ 反転 変色 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_MONO_REVERSE_COLOR_TR
		__END_STDBOX_SPRITE
	}
}

// モノ 反転 変色 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_MONO_REVERSE_COLOR_TR
		__END_STDBOX_TR_NORMAL
	}
}

// モノ 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_MONO_REVERSE_COLOR_TR
		__END_STDBOX_TR_SPRITE
	}
}

// モノ 反転 変色 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_NORMAL
		__DRAW_MUL_MONO_REVERSE_COLOR_BRIGHT
		__END_STDBOX_NORMAL
	}
}

// モノ 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_SPRITE
		__DRAW_MUL_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_SPRITE
	}
}

// モノ 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_NORMAL
		__DRAW_MUL_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_TR_NORMAL
	}
}

// モノ 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_mono_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TR_SPRITE
		__DRAW_MUL_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_TR_SPRITE
	}
}

// タイル 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_NORMAL
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_BRIGHT
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_BRIGHT_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_BRIGHT_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_BRIGHT_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル 変色 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_COLOR
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル 変色 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_COLOR_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル 変色 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_COLOR_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル 変色 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_COLOR_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル 変色 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_COLOR_BRIGHT
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル 変色 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル 反転 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_REVERSE
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル 反転 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_REVERSE_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル 反転 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_REVERSE_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル 反転 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_REVERSE_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル 反転 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_REVERSE_BRIGHT
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル 反転 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_REVERSE_BRIGHT_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル 反転 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_REVERSE_BRIGHT_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_REVERSE_BRIGHT_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル 反転 変色 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_REVERSE_COLOR
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル 反転 変色 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_REVERSE_COLOR_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル 反転 変色 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_REVERSE_COLOR_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_REVERSE_COLOR_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル 反転 変色 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_REVERSE_COLOR_BRIGHT
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル モノ 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_MONO
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル モノ スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_MONO_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル モノ 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_MONO_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル モノ 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_MONO_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル モノ 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_MONO_BRIGHT
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル モノ 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_MONO_BRIGHT_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル モノ 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_MONO_BRIGHT_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル モノ 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_MONO_BRIGHT_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル モノ 変色 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_MONO_COLOR
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル モノ 変色 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_MONO_COLOR_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル モノ 変色 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_MONO_COLOR_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル モノ 変色 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_MONO_COLOR_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル モノ 変色 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_MONO_COLOR_BRIGHT
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル モノ 変色 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_MONO_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル モノ 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_MONO_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル モノ 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_MONO_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_MONO_REVERSE
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル モノ 反転 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_MONO_REVERSE_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル モノ 反転 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_MONO_REVERSE_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_MONO_REVERSE_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_MONO_REVERSE_BRIGHT
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル モノ 反転 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_MONO_REVERSE_BRIGHT_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル モノ 反転 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_MONO_REVERSE_BRIGHT_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_MONO_REVERSE_BRIGHT_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 変色 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_color_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_MONO_REVERSE_COLOR
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル モノ 反転 変色 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_color_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_MONO_REVERSE_COLOR_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル モノ 反転 変色 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_color_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_MONO_REVERSE_COLOR_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 変色 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_color_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_MONO_REVERSE_COLOR_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

// タイル モノ 反転 変色 色味 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_color_bright_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_NORMAL
		__DRAW_MUL_TILE_MONO_REVERSE_COLOR_BRIGHT
		__END_STDBOX_TILE_NORMAL
	}
}

// タイル モノ 反転 変色 色味 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_color_bright_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_SPRITE
		__DRAW_MUL_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_SPRITE
	}
}

// タイル モノ 反転 変色 色味 透明 通常
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_color_bright_tr_normal(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_NORMAL
		__DRAW_MUL_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_TR_NORMAL
	}
}

// タイル モノ 反転 変色 色味 透明 スプライト
void namalib_grp_tcgd_func_stdbox_mul_tile_mono_reverse_color_bright_tr_sprite(TCGD_FUNC_PARAM *_fdp)
{
	__PARAM_LOCAL_COPY
	_asm{
		__START_STDBOX_TILE_TR_SPRITE
		__DRAW_MUL_TILE_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_STDBOX_TILE_TR_SPRITE
	}
}

