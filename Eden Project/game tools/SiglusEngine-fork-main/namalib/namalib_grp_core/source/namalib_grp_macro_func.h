#pragma		once

#define		__NAMALIB_SRCTYPE_FUNC_MACRO__(SRCTYPE, srctype, drawmod) \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_NORMAL \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_BRIGHT \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_BRIGHT_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_BRIGHT_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_BRIGHT_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_color_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_COLOR \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_color_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_COLOR_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_color_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_COLOR_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_color_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_COLOR_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_color_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_COLOR_BRIGHT \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_color_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_COLOR_BRIGHT_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_color_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_COLOR_BRIGHT_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_color_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_COLOR_BRIGHT_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_REVERSE \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_REVERSE_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_REVERSE_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_REVERSE_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_REVERSE_BRIGHT \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_REVERSE_BRIGHT_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_REVERSE_BRIGHT_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_REVERSE_BRIGHT_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_color_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_REVERSE_COLOR \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_color_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_REVERSE_COLOR_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_color_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_REVERSE_COLOR_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_color_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_REVERSE_COLOR_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_color_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_REVERSE_COLOR_BRIGHT \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_color_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_REVERSE_COLOR_BRIGHT_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_color_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_REVERSE_COLOR_BRIGHT_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_reverse_color_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_REVERSE_COLOR_BRIGHT_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_MONO \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_MONO_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_MONO_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_MONO_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_MONO_BRIGHT \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_MONO_BRIGHT_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_MONO_BRIGHT_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_MONO_BRIGHT_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_color_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_MONO_COLOR \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_color_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_MONO_COLOR_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_color_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_MONO_COLOR_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_color_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_MONO_COLOR_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_color_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_MONO_COLOR_BRIGHT \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_color_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_MONO_COLOR_BRIGHT_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_color_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_MONO_COLOR_BRIGHT_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_color_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_MONO_COLOR_BRIGHT_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_MONO_REVERSE \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_MONO_REVERSE_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_MONO_REVERSE_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_MONO_REVERSE_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_MONO_REVERSE_BRIGHT \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_MONO_REVERSE_BRIGHT_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_MONO_REVERSE_BRIGHT_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_MONO_REVERSE_BRIGHT_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_color_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_MONO_REVERSE_COLOR \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_color_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_MONO_REVERSE_COLOR_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_color_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_MONO_REVERSE_COLOR_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_color_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_MONO_REVERSE_COLOR_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_color_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_NORMAL \
		__DRAW_ADD_MONO_REVERSE_COLOR_BRIGHT \
		__END_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_color_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_SPRITE \
		__DRAW_ADD_MONO_REVERSE_COLOR_BRIGHT_TR \
		__END_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_color_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_NORMAL \
		__DRAW_ADD_MONO_REVERSE_COLOR_BRIGHT_TR \
		__END_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_mono_reverse_color_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TR_SPRITE \
		__DRAW_ADD_MONO_REVERSE_COLOR_BRIGHT_TR \
		__END_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL_EX \
		__DRAW_ADD_TILE_NORMAL \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_BRIGHT \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_BRIGHT_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_BRIGHT_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_BRIGHT_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_color_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_COLOR \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_color_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_COLOR_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_color_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_COLOR_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_color_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_COLOR_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_color_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_COLOR_BRIGHT \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_color_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_COLOR_BRIGHT_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_color_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_COLOR_BRIGHT_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_color_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_COLOR_BRIGHT_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_REVERSE \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_REVERSE_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_REVERSE_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_REVERSE_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_REVERSE_BRIGHT \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_REVERSE_BRIGHT_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_REVERSE_BRIGHT_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_REVERSE_BRIGHT_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_color_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_REVERSE_COLOR \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_color_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_REVERSE_COLOR_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_color_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_REVERSE_COLOR_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_color_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_REVERSE_COLOR_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_color_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_REVERSE_COLOR_BRIGHT \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_color_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_REVERSE_COLOR_BRIGHT_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_color_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_REVERSE_COLOR_BRIGHT_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_reverse_color_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_REVERSE_COLOR_BRIGHT_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_MONO \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_MONO_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_MONO_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_MONO_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_MONO_BRIGHT \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_MONO_BRIGHT_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_MONO_BRIGHT_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_MONO_BRIGHT_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_color_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_MONO_COLOR \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_color_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_MONO_COLOR_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_color_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_MONO_COLOR_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_color_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_MONO_COLOR_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_color_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_MONO_COLOR_BRIGHT \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_color_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_MONO_COLOR_BRIGHT_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_color_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_MONO_COLOR_BRIGHT_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_color_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_MONO_COLOR_BRIGHT_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_MONO_REVERSE \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_MONO_REVERSE_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_MONO_REVERSE_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_MONO_REVERSE_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_MONO_REVERSE_BRIGHT \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_MONO_REVERSE_BRIGHT_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_MONO_REVERSE_BRIGHT_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_MONO_REVERSE_BRIGHT_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_color_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_MONO_REVERSE_COLOR \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_color_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_MONO_REVERSE_COLOR_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_color_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_MONO_REVERSE_COLOR_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_color_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_MONO_REVERSE_COLOR_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_color_bright_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_NORMAL \
		__DRAW_ADD_TILE_MONO_REVERSE_COLOR_BRIGHT \
		__END_TILE_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_color_bright_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_SPRITE \
		__DRAW_ADD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR \
		__END_TILE_SPRITE(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_color_bright_tr_normal(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_NORMAL \
		__DRAW_ADD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR \
		__END_TILE_TR_NORMAL(SRCTYPE) \
	} \
} \
void Cnamalib_grp::_tcgd_func_##srctype##_##drawmod##_tile_mono_reverse_color_bright_tr_sprite(void) \
{ \
	__PARAM_LOCAL_COPY \
	_asm{ \
		__START_TILE_TR_SPRITE \
		__DRAW_ADD_TILE_MONO_REVERSE_COLOR_BRIGHT_TR \
		__END_TILE_TR_SPRITE(SRCTYPE) \
	} \
} \

