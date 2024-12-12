#include	"pch.h"
#include	"ng_macro.h"
#include	"ng_tcrotate_macro.h"


//----------------------------------------------------------------------
// 通常表示

#define		GRPMACRLD_XX_NORMAL	\
__asm	mov		eax, [esi]		\
__asm	mov		[edi],eax

void __p_tcrld_normal(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL GRPMACRLD_P_END}
}
void __h_tcrld_normal(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL GRPMACRLD_H_END}
}
void __v_tcrld_normal(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL GRPMACRLD_V_END}
}
void __x_tcrld_normal(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal();}
		else{__h_tcrld_normal();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal();}
		else{__x_tcrld_normal();}
	}
}

//----------------------------------------------------------------------
// スプライト表示

#define		GRPMACRLD_XX_SPRITE	\
			GRPMAC_GETSRC_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_CLRSET_R

void __p_tcrld_sprite(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite();}
		else{__h_tcrld_sprite();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite();}
		else{__x_tcrld_sprite();}
	}
}

//----------------------------------------------------------------------
// 透明通常表示

#define		GRPMACRLD_XX_NORMAL_CLR	\
			GRPMAC_GETSRC_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_CLRSET_R

void __p_tcrld_normal_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_clr();}
		else{__h_tcrld_normal_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_clr();}
		else{__x_tcrld_normal_clr();}
	}
}

//----------------------------------------------------------------------
// 透明スプライト表示

void __p_tcrld_sprite_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_clr();}
		else{__h_tcrld_sprite_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_clr();}
		else{__x_tcrld_sprite_clr();}
	}
}

//----------------------------------------------------------------------
// 変色通常表示

#define		GRPMACRLD_XX_NORMAL_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_SET_R

void __p_tcrld_normal_change(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_change(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_change(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_change(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change();}
		else{__h_tcrld_normal_change();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change();}
		else{__x_tcrld_normal_change();}
	}
}

//----------------------------------------------------------------------
// 変色スプライト表示

#define		GRPMACRLD_XX_SPRITE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_change(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_change(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_change(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_change(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change();}
		else{__h_tcrld_sprite_change();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change();}
		else{__x_tcrld_sprite_change();}
	}
}

//----------------------------------------------------------------------
// 変色透明通常表示

#define		GRPMACRLD_XX_NORMAL_CHANGE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_CLRSET_R

void __p_tcrld_normal_change_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_clr();}
		else{__h_tcrld_normal_change_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_clr();}
		else{__x_tcrld_normal_change_clr();}
	}
}

//----------------------------------------------------------------------
// 変色透明スプライト表示

void __p_tcrld_sprite_change_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_clr();}
		else{__h_tcrld_sprite_change_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_clr();}
		else{__x_tcrld_sprite_change_clr();}
	}
}

//----------------------------------------------------------------------
// 明暗通常表示

#define		GRPMACRLD_XX_NORMAL_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_SET_R

void __p_tcrld_normal_bright(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_bright(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_bright(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_bright(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_bright();}
		else{__h_tcrld_normal_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_bright();}
		else{__x_tcrld_normal_bright();}
	}
}

//----------------------------------------------------------------------
// 明暗スプライト表示

#define		GRPMACRLD_XX_SPRITE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_bright(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_bright(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_bright(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_bright(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_bright();}
		else{__h_tcrld_sprite_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_bright();}
		else{__x_tcrld_sprite_bright();}
	}
}

//----------------------------------------------------------------------
// 明暗透明通常表示

#define		GRPMACRLD_XX_NORMAL_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_normal_bright_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_bright_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_bright_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_bright_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_bright_clr();}
		else{__h_tcrld_normal_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_bright_clr();}
		else{__x_tcrld_normal_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 明暗透明スプライト表示

void __p_tcrld_sprite_bright_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_bright_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_bright_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_bright_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_bright_clr();}
		else{__h_tcrld_sprite_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_bright_clr();}
		else{__x_tcrld_sprite_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 変色明暗通常表示

#define		GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SET_R

void __p_tcrld_normal_change_bright(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_bright(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_bright(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_bright(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_bright();}
		else{__h_tcrld_normal_change_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_bright();}
		else{__x_tcrld_normal_change_bright();}
	}
}

//----------------------------------------------------------------------
// 変色明暗スプライト表示

#define		GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_change_bright(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_bright(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_bright(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_bright(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_bright();}
		else{__h_tcrld_sprite_change_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_bright();}
		else{__x_tcrld_sprite_change_bright();}
	}
}

//----------------------------------------------------------------------
// 変色明暗透明通常表示

#define		GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_normal_change_bright_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_bright_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_bright_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_bright_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_bright_clr();}
		else{__h_tcrld_normal_change_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_bright_clr();}
		else{__x_tcrld_normal_change_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 変色明暗透明スプライト表示

void __p_tcrld_sprite_change_bright_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_bright_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_bright_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_bright_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_bright_clr();}
		else{__h_tcrld_sprite_change_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_bright_clr();}
		else{__x_tcrld_sprite_change_bright_clr();}
	}
}

//----------------------------------------------------------------------
// モノ通常表示

#define		GRPMACRLD_XX_NORMAL_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_SET_R

void __p_tcrld_normal_mono(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono();}
		else{__h_tcrld_normal_mono();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono();}
		else{__x_tcrld_normal_mono();}
	}
}

//----------------------------------------------------------------------
// モノスプライト表示

#define		GRPMACRLD_XX_SPRITE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_mono(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono();}
		else{__h_tcrld_sprite_mono();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono();}
		else{__x_tcrld_sprite_mono();}
	}
}

//----------------------------------------------------------------------
// モノ透明通常表示

#define		GRPMACRLD_XX_NORMAL_MONO_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_CLRSET_R

void __p_tcrld_normal_mono_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_clr();}
		else{__h_tcrld_normal_mono_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_clr();}
		else{__x_tcrld_normal_mono_clr();}
	}
}

//----------------------------------------------------------------------
// モノ透明スプライト表示

void __p_tcrld_sprite_mono_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_clr();}
		else{__h_tcrld_sprite_mono_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_clr();}
		else{__x_tcrld_sprite_mono_clr();}
	}
}

//----------------------------------------------------------------------
// モノ変色通常表示

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_SET_R

void __p_tcrld_normal_mono_change(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change();}
		else{__h_tcrld_normal_mono_change();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change();}
		else{__x_tcrld_normal_mono_change();}
	}
}

//----------------------------------------------------------------------
// モノ変色スプライト表示

#define		GRPMACRLD_XX_SPRITE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_mono_change(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change();}
		else{__h_tcrld_sprite_mono_change();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change();}
		else{__x_tcrld_sprite_mono_change();}
	}
}

//----------------------------------------------------------------------
// モノ変色透明通常表示

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_CLRSET_R

void __p_tcrld_normal_mono_change_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_clr();}
		else{__h_tcrld_normal_mono_change_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_clr();}
		else{__x_tcrld_normal_mono_change_clr();}
	}
}

//----------------------------------------------------------------------
// モノ変色透明スプライト表示

void __p_tcrld_sprite_mono_change_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_clr();}
		else{__h_tcrld_sprite_mono_change_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_clr();}
		else{__x_tcrld_sprite_mono_change_clr();}
	}
}

//----------------------------------------------------------------------
// モノ明暗通常表示

#define		GRPMACRLD_XX_NORMAL_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_SET_R

void __p_tcrld_normal_mono_bright(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_bright(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_bright(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_bright(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_bright();}
		else{__h_tcrld_normal_mono_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_bright();}
		else{__x_tcrld_normal_mono_bright();}
	}
}

//----------------------------------------------------------------------
// モノ明暗スプライト表示

#define		GRPMACRLD_XX_SPRITE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_mono_bright(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_bright(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_bright(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_bright(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_bright();}
		else{__h_tcrld_sprite_mono_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_bright();}
		else{__x_tcrld_sprite_mono_bright();}
	}
}

//----------------------------------------------------------------------
// モノ明暗透明通常表示

#define		GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_normal_mono_bright_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_bright_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_bright_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_bright_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_bright_clr();}
		else{__h_tcrld_normal_mono_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_bright_clr();}
		else{__x_tcrld_normal_mono_bright_clr();}
	}
}

//----------------------------------------------------------------------
// モノ明暗透明スプライト表示

void __p_tcrld_sprite_mono_bright_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_bright_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_bright_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_bright_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_bright_clr();}
		else{__h_tcrld_sprite_mono_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_bright_clr();}
		else{__x_tcrld_sprite_mono_bright_clr();}
	}
}

//----------------------------------------------------------------------
// モノ変色明暗通常表示

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SET_R

void __p_tcrld_normal_mono_change_bright(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_bright(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_bright(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_bright(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_bright();}
		else{__h_tcrld_normal_mono_change_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_bright();}
		else{__x_tcrld_normal_mono_change_bright();}
	}
}

//----------------------------------------------------------------------
// モノ変色明暗スプライト表示

#define		GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_mono_change_bright(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_bright(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_bright(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_bright(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_bright();}
		else{__h_tcrld_sprite_mono_change_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_bright();}
		else{__x_tcrld_sprite_mono_change_bright();}
	}
}

//----------------------------------------------------------------------
// モノ変色明暗透明通常表示

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_normal_mono_change_bright_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_bright_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_bright_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_bright_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_bright_clr();}
		else{__h_tcrld_normal_mono_change_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_bright_clr();}
		else{__x_tcrld_normal_mono_change_bright_clr();}
	}
}

//----------------------------------------------------------------------
// モノ変色明暗透明スプライト表示

void __p_tcrld_sprite_mono_change_bright_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_bright_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_bright_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_bright_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_bright_clr();}
		else{__h_tcrld_sprite_mono_change_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_bright_clr();}
		else{__x_tcrld_sprite_mono_change_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 反転通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_SET_R

void __p_tcrld_normal_reverse(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse();}
		else{__h_tcrld_normal_reverse();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse();}
		else{__x_tcrld_normal_reverse();}
	}
}

//----------------------------------------------------------------------
// 反転スプライト表示

#define		GRPMACRLD_XX_SPRITE_REVERSE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_CLRSET_R

void __p_tcrld_sprite_reverse(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse();}
		else{__h_tcrld_sprite_reverse();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse();}
		else{__x_tcrld_sprite_reverse();}
	}
}

//----------------------------------------------------------------------
// 反転透明通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_CLRSET_R

void __p_tcrld_normal_reverse_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_clr();}
		else{__h_tcrld_normal_reverse_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_clr();}
		else{__x_tcrld_normal_reverse_clr();}
	}
}

//----------------------------------------------------------------------
// 反転透明スプライト表示

void __p_tcrld_sprite_reverse_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_clr();}
		else{__h_tcrld_sprite_reverse_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_clr();}
		else{__x_tcrld_sprite_reverse_clr();}
	}
}

//----------------------------------------------------------------------
// 反転変色通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_SET_R

void __p_tcrld_normal_reverse_change(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change();}
		else{__h_tcrld_normal_reverse_change();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change();}
		else{__x_tcrld_normal_reverse_change();}
	}
}

//----------------------------------------------------------------------
// 反転変色スプライト表示

#define		GRPMACRLD_XX_SPRITE_REVERSE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_reverse_change(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change();}
		else{__h_tcrld_sprite_reverse_change();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change();}
		else{__x_tcrld_sprite_reverse_change();}
	}
}

//----------------------------------------------------------------------
// 反転変色透明通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_CLRSET_R

void __p_tcrld_normal_reverse_change_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_clr();}
		else{__h_tcrld_normal_reverse_change_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_clr();}
		else{__x_tcrld_normal_reverse_change_clr();}
	}
}

//----------------------------------------------------------------------
// 反転変色透明スプライト表示

void __p_tcrld_sprite_reverse_change_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_clr();}
		else{__h_tcrld_sprite_reverse_change_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_clr();}
		else{__x_tcrld_sprite_reverse_change_clr();}
	}
}

//----------------------------------------------------------------------
// 反転明暗通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SET_R

void __p_tcrld_normal_reverse_bright(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_bright(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_bright(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_bright(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_bright();}
		else{__h_tcrld_normal_reverse_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_bright();}
		else{__x_tcrld_normal_reverse_bright();}
	}
}

//----------------------------------------------------------------------
// 反転明暗スプライト表示

#define		GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_reverse_bright(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_bright(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_bright(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_bright(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_bright();}
		else{__h_tcrld_sprite_reverse_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_bright();}
		else{__x_tcrld_sprite_reverse_bright();}
	}
}

//----------------------------------------------------------------------
// 反転明暗透明通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_normal_reverse_bright_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_bright_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_bright_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_bright_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_bright_clr();}
		else{__h_tcrld_normal_reverse_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_bright_clr();}
		else{__x_tcrld_normal_reverse_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 反転明暗透明スプライト表示

void __p_tcrld_sprite_reverse_bright_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_bright_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_bright_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_bright_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_bright_clr();}
		else{__h_tcrld_sprite_reverse_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_bright_clr();}
		else{__x_tcrld_sprite_reverse_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 反転変色明暗通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SET_R

void __p_tcrld_normal_reverse_change_bright(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_bright(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_bright(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_bright(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_bright();}
		else{__h_tcrld_normal_reverse_change_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_bright();}
		else{__x_tcrld_normal_reverse_change_bright();}
	}
}

//----------------------------------------------------------------------
// 反転変色明暗スプライト表示

#define		GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_reverse_change_bright(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_bright(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_bright(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_bright(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_bright();}
		else{__h_tcrld_sprite_reverse_change_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_bright();}
		else{__x_tcrld_sprite_reverse_change_bright();}
	}
}

//----------------------------------------------------------------------
// 反転変色明暗透明通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_normal_reverse_change_bright_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_bright_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_bright_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_bright_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_bright_clr();}
		else{__h_tcrld_normal_reverse_change_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_bright_clr();}
		else{__x_tcrld_normal_reverse_change_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 反転変色明暗透明スプライト表示

void __p_tcrld_sprite_reverse_change_bright_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_bright_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_bright_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_bright_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_bright_clr();}
		else{__h_tcrld_sprite_reverse_change_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_bright_clr();}
		else{__x_tcrld_sprite_reverse_change_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 反転モノ通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_SET_R

void __p_tcrld_normal_reverse_mono(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono();}
		else{__h_tcrld_normal_reverse_mono();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono();}
		else{__x_tcrld_normal_reverse_mono();}
	}
}

//----------------------------------------------------------------------
// 反転モノスプライト表示

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_CLRSET_R

void __p_tcrld_sprite_reverse_mono(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono();}
		else{__h_tcrld_sprite_reverse_mono();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono();}
		else{__x_tcrld_sprite_reverse_mono();}
	}
}

//----------------------------------------------------------------------
// 反転モノ透明通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_CLRSET_R

void __p_tcrld_normal_reverse_mono_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_clr();}
		else{__h_tcrld_normal_reverse_mono_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_clr();}
		else{__x_tcrld_normal_reverse_mono_clr();}
	}
}

//----------------------------------------------------------------------
// 反転モノ透明スプライト表示

void __p_tcrld_sprite_reverse_mono_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_clr();}
		else{__h_tcrld_sprite_reverse_mono_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_clr();}
		else{__x_tcrld_sprite_reverse_mono_clr();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_SET_R

void __p_tcrld_normal_reverse_mono_change(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change();}
		else{__h_tcrld_normal_reverse_mono_change();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change();}
		else{__x_tcrld_normal_reverse_mono_change();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色スプライト表示

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_reverse_mono_change(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change();}
		else{__h_tcrld_sprite_reverse_mono_change();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change();}
		else{__x_tcrld_sprite_reverse_mono_change();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色透明通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_CLRSET_R

void __p_tcrld_normal_reverse_mono_change_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_clr();}
		else{__h_tcrld_normal_reverse_mono_change_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_clr();}
		else{__x_tcrld_normal_reverse_mono_change_clr();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色透明スプライト表示

void __p_tcrld_sprite_reverse_mono_change_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_clr();}
		else{__h_tcrld_sprite_reverse_mono_change_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_clr();}
		else{__x_tcrld_sprite_reverse_mono_change_clr();}
	}
}

//----------------------------------------------------------------------
// 反転モノ明暗通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SET_R

void __p_tcrld_normal_reverse_mono_bright(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_bright(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_bright(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_bright(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_bright();}
		else{__h_tcrld_normal_reverse_mono_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_bright();}
		else{__x_tcrld_normal_reverse_mono_bright();}
	}
}

//----------------------------------------------------------------------
// 反転モノ明暗スプライト表示

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_reverse_mono_bright(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_bright(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_bright(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_bright(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_bright();}
		else{__h_tcrld_sprite_reverse_mono_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_bright();}
		else{__x_tcrld_sprite_reverse_mono_bright();}
	}
}

//----------------------------------------------------------------------
// 反転モノ明暗透明通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_normal_reverse_mono_bright_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_bright_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_bright_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_bright_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_bright_clr();}
		else{__h_tcrld_normal_reverse_mono_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_bright_clr();}
		else{__x_tcrld_normal_reverse_mono_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 反転モノ明暗透明スプライト表示

void __p_tcrld_sprite_reverse_mono_bright_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_bright_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_bright_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_bright_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_bright_clr();}
		else{__h_tcrld_sprite_reverse_mono_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_bright_clr();}
		else{__x_tcrld_sprite_reverse_mono_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色明暗通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SET_R

void __p_tcrld_normal_reverse_mono_change_bright(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_bright(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_bright(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_bright(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_bright();}
		else{__h_tcrld_normal_reverse_mono_change_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_bright();}
		else{__x_tcrld_normal_reverse_mono_change_bright();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色明暗スプライト表示

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_sprite_reverse_mono_change_bright(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_bright(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_bright(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_bright(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_bright(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_bright();}
		else{__h_tcrld_sprite_reverse_mono_change_bright();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_bright();}
		else{__x_tcrld_sprite_reverse_mono_change_bright();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色明暗透明通常表示

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

void __p_tcrld_normal_reverse_mono_change_bright_clr(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_bright_clr(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_bright_clr(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_bright_clr(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_bright_clr();}
		else{__h_tcrld_normal_reverse_mono_change_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_bright_clr();}
		else{__x_tcrld_normal_reverse_mono_change_bright_clr();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色明暗透明スプライト表示

void __p_tcrld_sprite_reverse_mono_change_bright_clr(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_bright_clr(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_bright_clr(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_bright_clr(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_bright_clr(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_bright_clr();}
		else{__h_tcrld_sprite_reverse_mono_change_bright_clr();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_bright_clr();}
		else{__x_tcrld_sprite_reverse_mono_change_bright_clr();}
	}
}

