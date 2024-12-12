#include	"pch.h"

// ****************************************************************
// •`‰æ•ªŠò

#define		__DRAW_SWITCH_FUNC_SPRITE(func_name) \
					if (!_fdp->fs_sprite)	{ \
						namalib_grp_tcgd_func_##func_name##_normal(_fdp); \
					} \
					else	{ \
						namalib_grp_tcgd_func_##func_name##_sprite(_fdp); \
					}

#define		__DRAW_SWITCH_FUNC_TR(func_name) \
					if (!_fdp->fs_tr)	{ \
						__DRAW_SWITCH_FUNC_SPRITE(func_name) \
					} \
					else	{ \
						__DRAW_SWITCH_FUNC_SPRITE(##func_name##_tr) \
					}

#define		__DRAW_SWITCH_FUNC_BRIGHT(func_name) \
					if (!_fdp->fs_bright)	{ \
						__DRAW_SWITCH_FUNC_TR(func_name) \
					} \
					else	{ \
						__DRAW_SWITCH_FUNC_TR(##func_name##_bright) \
					}

#define		__DRAW_SWITCH_FUNC_COLOR(func_name) \
					if (!_fdp->fs_color)	{ \
						__DRAW_SWITCH_FUNC_BRIGHT(func_name) \
					} \
					else	{ \
						__DRAW_SWITCH_FUNC_BRIGHT(##func_name##_color) \
					}

#define		__DRAW_SWITCH_FUNC_REVERSE(func_name) \
					if (!_fdp->fs_reverse)	{ \
						__DRAW_SWITCH_FUNC_COLOR(func_name) \
					} \
					else	{ \
						__DRAW_SWITCH_FUNC_COLOR(##func_name##_reverse) \
					}

#define		__DRAW_SWITCH_FUNC_MONO(func_name) \
					if (!_fdp->fs_mono)	{ \
						__DRAW_SWITCH_FUNC_REVERSE(func_name) \
					} \
					else	{ \
						__DRAW_SWITCH_FUNC_REVERSE(##func_name##_mono) \
					}

#define		__DRAW_SWITCH_FUNC_TILE(func_name) \
					if (!_fdp->fs_tile)	{ \
						__DRAW_SWITCH_FUNC_MONO(func_name) \
					} \
					else	{ \
						__DRAW_SWITCH_FUNC_MONO(##func_name##_tile) \
					}

#define		__DRAW_SWITCH_FUNC_DRAW_MOD(func_name) \
					switch (_fdp->fs_draw_mod)	{ \
						case TRUE_COLOR_GRP_DRAW_MOD_STD:	__DRAW_SWITCH_FUNC_TILE(##func_name##_std)	break; \
						case TRUE_COLOR_GRP_DRAW_MOD_ADD:	__DRAW_SWITCH_FUNC_TILE(##func_name##_add)	break; \
						case TRUE_COLOR_GRP_DRAW_MOD_SUB:	__DRAW_SWITCH_FUNC_TILE(##func_name##_sub)	break; \
						case TRUE_COLOR_GRP_DRAW_MOD_RSUB:	__DRAW_SWITCH_FUNC_TILE(##func_name##_rsub)	break; \
						case TRUE_COLOR_GRP_DRAW_MOD_FLT:	__DRAW_SWITCH_FUNC_TILE(##func_name##_flt)	break; \
						case TRUE_COLOR_GRP_DRAW_MOD_MUL:	__DRAW_SWITCH_FUNC_TILE(##func_name##_mul)	break; \
						case TRUE_COLOR_GRP_DRAW_MOD_BLD:	__DRAW_SWITCH_FUNC_TILE(##func_name##_bld)	break; \
					}

void namalib_grp_tcgd_func_draw_switch(TCGD_FUNC_PARAM *_fdp, int func_type, TCGD_EX_MOD ex_mod)
{
	switch (ex_mod)	{
		// •`‰æƒ‚[ƒh
		case TCGD_EX_MOD_STANDARD:
			switch (func_type)	{
				case 0:		// ’Êí•`‰æ
					if (!_fdp->fs_grp_or_box)	{	__DRAW_SWITCH_FUNC_DRAW_MOD(stdgrp)	}
					else						{	__DRAW_SWITCH_FUNC_DRAW_MOD(stdbox)	}
					break;
				case 1:		// ‚x²Šî€‚Ìƒ‰ƒCƒ“®•`‰æ
					if (!_fdp->fs_grp_or_box)	{	__DRAW_SWITCH_FUNC_DRAW_MOD(lbdgrp)	}
					else						{	__DRAW_SWITCH_FUNC_DRAW_MOD(lbdbox)	}
					break;
				case 2:		// ƒ‰ƒCƒ“•`‰æ
					__DRAW_SWITCH_FUNC_DRAW_MOD(lined)
					break;
			}
			break;
		// •ÏŠ·ƒ‚[ƒh
		case TCGD_EX_MOD_CHANGE:
			switch (func_type)	{
				case 0:		// ’Êí•`‰æ
					__DRAW_SWITCH_FUNC_TILE(chgstdbox_std)
					break;
				case 1:		// ‚x²Šî€‚Ìƒ‰ƒCƒ“®•`‰æ
					__DRAW_SWITCH_FUNC_TILE(chglbdbox_std)
					break;
				case 2:		// ƒ‰ƒCƒ“•`‰æ
					__DRAW_SWITCH_FUNC_TILE(chglined_std)
					break;
			}
			break;
	}
}

