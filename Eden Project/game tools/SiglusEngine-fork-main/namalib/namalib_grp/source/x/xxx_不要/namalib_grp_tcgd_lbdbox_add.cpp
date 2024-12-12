#include	"namalib_grp_pch.h"

// 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_NORMAL
		__END_LBDBOX_NORMAL
	}
}

// スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_TR
		__END_LBDBOX_SPRITE
	}
}

// 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// 色味 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_BRIGHT
		__END_LBDBOX_NORMAL
	}
}

// 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_BRIGHT_TR
		__END_LBDBOX_SPRITE
	}
}

// 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_BRIGHT_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_BRIGHT_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// 変色 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_color_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_COLOR
		__END_LBDBOX_NORMAL
	}
}

// 変色 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_color_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_COLOR_TR
		__END_LBDBOX_SPRITE
	}
}

// 変色 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_color_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_COLOR_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// 変色 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_color_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_COLOR_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// 変色 色味 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_color_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_COLOR_BRIGHT
		__END_LBDBOX_NORMAL
	}
}

// 変色 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_color_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_COLOR_BRIGHT_TR
		__END_LBDBOX_SPRITE
	}
}

// 変色 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_color_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_COLOR_BRIGHT_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// 変色 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_color_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_COLOR_BRIGHT_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// 反転 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_REVERSE
		__END_LBDBOX_NORMAL
	}
}

// 反転 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_REVERSE_TR
		__END_LBDBOX_SPRITE
	}
}

// 反転 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_REVERSE_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// 反転 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_REVERSE_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// 反転 色味 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_REVERSE_BRIGHT
		__END_LBDBOX_NORMAL
	}
}

// 反転 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_REVERSE_BRIGHT_TR
		__END_LBDBOX_SPRITE
	}
}

// 反転 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_REVERSE_BRIGHT_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// 反転 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_REVERSE_BRIGHT_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// 反転 変色 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_color_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_REVERSE_COLOR
		__END_LBDBOX_NORMAL
	}
}

// 反転 変色 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_color_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_REVERSE_COLOR_TR
		__END_LBDBOX_SPRITE
	}
}

// 反転 変色 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_color_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_REVERSE_COLOR_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// 反転 変色 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_color_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_REVERSE_COLOR_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// 反転 変色 色味 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_color_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_REVERSE_COLOR_BRIGHT
		__END_LBDBOX_NORMAL
	}
}

// 反転 変色 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_color_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_REVERSE_COLOR_BRIGHT_TR
		__END_LBDBOX_SPRITE
	}
}

// 反転 変色 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_color_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_REVERSE_COLOR_BRIGHT_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// 反転 変色 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_reverse_color_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_REVERSE_COLOR_BRIGHT_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// モノ 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_MONO
		__END_LBDBOX_NORMAL
	}
}

// モノ スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_MONO_TR
		__END_LBDBOX_SPRITE
	}
}

// モノ 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_MONO_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// モノ 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_MONO_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// モノ 色味 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_MONO_BRIGHT
		__END_LBDBOX_NORMAL
	}
}

// モノ 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_MONO_BRIGHT_TR
		__END_LBDBOX_SPRITE
	}
}

// モノ 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_MONO_BRIGHT_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// モノ 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_MONO_BRIGHT_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// モノ 変色 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_color_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_MONO_COLOR
		__END_LBDBOX_NORMAL
	}
}

// モノ 変色 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_color_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_MONO_COLOR_TR
		__END_LBDBOX_SPRITE
	}
}

// モノ 変色 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_color_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_MONO_COLOR_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// モノ 変色 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_color_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_MONO_COLOR_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// モノ 変色 色味 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_color_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_MONO_COLOR_BRIGHT
		__END_LBDBOX_NORMAL
	}
}

// モノ 変色 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_color_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_MONO_COLOR_BRIGHT_TR
		__END_LBDBOX_SPRITE
	}
}

// モノ 変色 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_color_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_MONO_COLOR_BRIGHT_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// モノ 変色 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_color_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_MONO_COLOR_BRIGHT_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// モノ 反転 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_MONO_REVERSE
		__END_LBDBOX_NORMAL
	}
}

// モノ 反転 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_MONO_REVERSE_TR
		__END_LBDBOX_SPRITE
	}
}

// モノ 反転 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_MONO_REVERSE_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// モノ 反転 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_MONO_REVERSE_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// モノ 反転 色味 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_MONO_REVERSE_BRIGHT
		__END_LBDBOX_NORMAL
	}
}

// モノ 反転 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_MONO_REVERSE_BRIGHT_TR
		__END_LBDBOX_SPRITE
	}
}

// モノ 反転 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_MONO_REVERSE_BRIGHT_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// モノ 反転 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_MONO_REVERSE_BRIGHT_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// モノ 反転 変色 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_color_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_MONO_REVERSE_COLOR
		__END_LBDBOX_NORMAL
	}
}

// モノ 反転 変色 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_color_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_MONO_REVERSE_COLOR_TR
		__END_LBDBOX_SPRITE
	}
}

// モノ 反転 変色 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_color_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_MONO_REVERSE_COLOR_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// モノ 反転 変色 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_color_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_MONO_REVERSE_COLOR_TR
		__END_LBDBOX_TR_SPRITE
	}
}

// モノ 反転 変色 色味 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_color_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_NORMAL
		__DRAW_ADD_MONO_REVERSE_COLOR_BRIGHT
		__END_LBDBOX_NORMAL
	}
}

// モノ 反転 変色 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_color_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_SPRITE
		__DRAW_ADD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_LBDBOX_SPRITE
	}
}

// モノ 反転 変色 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_color_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_NORMAL
		__DRAW_ADD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_LBDBOX_TR_NORMAL
	}
}

// モノ 反転 変色 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdbox_add_mono_reverse_color_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDBOX_TR_SPRITE
		__DRAW_ADD_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_LBDBOX_TR_SPRITE
	}
}

