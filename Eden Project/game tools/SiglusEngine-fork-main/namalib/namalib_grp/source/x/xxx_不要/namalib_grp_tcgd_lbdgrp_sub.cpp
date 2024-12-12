#include	"namalib_grp_pch.h"

// 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_NORMAL
		__END_LBDGRP_NORMAL
	}
}

// スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_TR
		__END_LBDGRP_SPRITE
	}
}

// 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// 色味 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_BRIGHT
		__END_LBDGRP_NORMAL
	}
}

// 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_BRIGHT_TR
		__END_LBDGRP_SPRITE
	}
}

// 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_BRIGHT_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_BRIGHT_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// 変色 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_color_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_COLOR
		__END_LBDGRP_NORMAL
	}
}

// 変色 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_color_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_COLOR_TR
		__END_LBDGRP_SPRITE
	}
}

// 変色 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_color_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_COLOR_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// 変色 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_color_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_COLOR_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// 変色 色味 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_color_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_COLOR_BRIGHT
		__END_LBDGRP_NORMAL
	}
}

// 変色 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_color_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_COLOR_BRIGHT_TR
		__END_LBDGRP_SPRITE
	}
}

// 変色 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_color_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_COLOR_BRIGHT_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// 変色 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_color_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_COLOR_BRIGHT_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// 反転 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_REVERSE
		__END_LBDGRP_NORMAL
	}
}

// 反転 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_REVERSE_TR
		__END_LBDGRP_SPRITE
	}
}

// 反転 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_REVERSE_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// 反転 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_REVERSE_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// 反転 色味 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_REVERSE_BRIGHT
		__END_LBDGRP_NORMAL
	}
}

// 反転 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_REVERSE_BRIGHT_TR
		__END_LBDGRP_SPRITE
	}
}

// 反転 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_REVERSE_BRIGHT_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// 反転 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_REVERSE_BRIGHT_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// 反転 変色 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_color_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_REVERSE_COLOR
		__END_LBDGRP_NORMAL
	}
}

// 反転 変色 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_color_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_REVERSE_COLOR_TR
		__END_LBDGRP_SPRITE
	}
}

// 反転 変色 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_color_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_REVERSE_COLOR_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// 反転 変色 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_color_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_REVERSE_COLOR_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// 反転 変色 色味 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_color_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_REVERSE_COLOR_BRIGHT
		__END_LBDGRP_NORMAL
	}
}

// 反転 変色 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_color_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_REVERSE_COLOR_BRIGHT_TR
		__END_LBDGRP_SPRITE
	}
}

// 反転 変色 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_color_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_REVERSE_COLOR_BRIGHT_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// 反転 変色 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_reverse_color_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_REVERSE_COLOR_BRIGHT_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// モノ 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_MONO
		__END_LBDGRP_NORMAL
	}
}

// モノ スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_MONO_TR
		__END_LBDGRP_SPRITE
	}
}

// モノ 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_MONO_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// モノ 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_MONO_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// モノ 色味 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_MONO_BRIGHT
		__END_LBDGRP_NORMAL
	}
}

// モノ 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_MONO_BRIGHT_TR
		__END_LBDGRP_SPRITE
	}
}

// モノ 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_MONO_BRIGHT_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// モノ 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_MONO_BRIGHT_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// モノ 変色 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_color_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_MONO_COLOR
		__END_LBDGRP_NORMAL
	}
}

// モノ 変色 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_color_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_MONO_COLOR_TR
		__END_LBDGRP_SPRITE
	}
}

// モノ 変色 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_color_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_MONO_COLOR_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// モノ 変色 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_color_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_MONO_COLOR_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// モノ 変色 色味 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_color_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_MONO_COLOR_BRIGHT
		__END_LBDGRP_NORMAL
	}
}

// モノ 変色 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_color_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_MONO_COLOR_BRIGHT_TR
		__END_LBDGRP_SPRITE
	}
}

// モノ 変色 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_color_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_MONO_COLOR_BRIGHT_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// モノ 変色 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_color_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_MONO_COLOR_BRIGHT_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// モノ 反転 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_MONO_REVERSE
		__END_LBDGRP_NORMAL
	}
}

// モノ 反転 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_MONO_REVERSE_TR
		__END_LBDGRP_SPRITE
	}
}

// モノ 反転 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_MONO_REVERSE_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// モノ 反転 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_MONO_REVERSE_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// モノ 反転 色味 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_MONO_REVERSE_BRIGHT
		__END_LBDGRP_NORMAL
	}
}

// モノ 反転 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_MONO_REVERSE_BRIGHT_TR
		__END_LBDGRP_SPRITE
	}
}

// モノ 反転 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_MONO_REVERSE_BRIGHT_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// モノ 反転 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_MONO_REVERSE_BRIGHT_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// モノ 反転 変色 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_color_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_MONO_REVERSE_COLOR
		__END_LBDGRP_NORMAL
	}
}

// モノ 反転 変色 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_color_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_MONO_REVERSE_COLOR_TR
		__END_LBDGRP_SPRITE
	}
}

// モノ 反転 変色 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_color_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_MONO_REVERSE_COLOR_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// モノ 反転 変色 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_color_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_MONO_REVERSE_COLOR_TR
		__END_LBDGRP_TR_SPRITE
	}
}

// モノ 反転 変色 色味 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_color_bright_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_NORMAL
		__DRAW_SUB_MONO_REVERSE_COLOR_BRIGHT
		__END_LBDGRP_NORMAL
	}
}

// モノ 反転 変色 色味 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_color_bright_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_SPRITE
		__DRAW_SUB_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_LBDGRP_SPRITE
	}
}

// モノ 反転 変色 色味 透明 通常
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_color_bright_tr_normal(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_NORMAL
		__DRAW_SUB_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_LBDGRP_TR_NORMAL
	}
}

// モノ 反転 変色 色味 透明 スプライト
void Cnamalib_grp::_tcgd_func_lbdgrp_sub_mono_reverse_color_bright_tr_sprite(void)
{
	__PARAM_LOCAL_COPY_LBDP
	_asm{
		__START_LBDGRP_TR_SPRITE
		__DRAW_SUB_MONO_REVERSE_COLOR_BRIGHT_TR
		__END_LBDGRP_TR_SPRITE
	}
}

