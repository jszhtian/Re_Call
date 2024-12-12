#include	"pch.h"
#include	"ng_macro.h"
#include	"ng_tcrotate_macro.h"


//----------------------------------------------------------------------
// 通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL	\
			GRPMAC_GETSRC_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL GRPMACRLD_P_END}
}
void __h_tcrld_normal_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL GRPMACRLD_H_END}
}
void __v_tcrld_normal_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL GRPMACRLD_V_END}
}
void __x_tcrld_normal_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_rgbfilter();}
		else{__h_tcrld_normal_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_rgbfilter();}
		else{__x_tcrld_normal_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE	\
			GRPMAC_GETSRC_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_rgbfilter();}
		else{__h_tcrld_sprite_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_rgbfilter();}
		else{__x_tcrld_sprite_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_CLR	\
			GRPMAC_GETSRC_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_clr_rgbfilter();}
		else{__h_tcrld_normal_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_clr_rgbfilter();}
		else{__x_tcrld_normal_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_clr_rgbfilter();}
		else{__h_tcrld_sprite_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_clr_rgbfilter();}
		else{__x_tcrld_sprite_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 変色通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_change_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_rgbfilter();}
		else{__h_tcrld_normal_change_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_rgbfilter();}
		else{__x_tcrld_normal_change_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 変色スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_change_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_rgbfilter();}
		else{__h_tcrld_sprite_change_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_rgbfilter();}
		else{__x_tcrld_sprite_change_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 変色透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_CHANGE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_clr_rgbfilter();}
		else{__h_tcrld_normal_change_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_clr_rgbfilter();}
		else{__x_tcrld_normal_change_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 変色透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_change_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_clr_rgbfilter();}
		else{__h_tcrld_sprite_change_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_clr_rgbfilter();}
		else{__x_tcrld_sprite_change_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 明暗通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_bright_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_bright_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_bright_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_bright_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_bright_rgbfilter();}
		else{__h_tcrld_normal_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_bright_rgbfilter();}
		else{__x_tcrld_normal_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 明暗スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_bright_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_bright_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_bright_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_bright_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_bright_rgbfilter();}
		else{__h_tcrld_sprite_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_bright_rgbfilter();}
		else{__x_tcrld_sprite_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 明暗透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_bright_clr_rgbfilter();}
		else{__h_tcrld_normal_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_bright_clr_rgbfilter();}
		else{__x_tcrld_normal_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 明暗透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_bright_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_bright_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_bright_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_bright_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_bright_clr_rgbfilter();}
		else{__h_tcrld_sprite_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_bright_clr_rgbfilter();}
		else{__x_tcrld_sprite_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 変色明暗通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_bright_rgbfilter();}
		else{__h_tcrld_normal_change_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_bright_rgbfilter();}
		else{__x_tcrld_normal_change_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 変色明暗スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_change_bright_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_bright_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_bright_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_bright_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_bright_rgbfilter();}
		else{__h_tcrld_sprite_change_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_bright_rgbfilter();}
		else{__x_tcrld_sprite_change_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 変色明暗透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_bright_clr_rgbfilter();}
		else{__h_tcrld_normal_change_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_bright_clr_rgbfilter();}
		else{__x_tcrld_normal_change_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 変色明暗透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_change_bright_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_bright_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_bright_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_bright_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_bright_clr_rgbfilter();}
		else{__h_tcrld_sprite_change_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_bright_clr_rgbfilter();}
		else{__x_tcrld_sprite_change_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_mono_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_rgbfilter();}
		else{__h_tcrld_normal_mono_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_rgbfilter();}
		else{__x_tcrld_normal_mono_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノスプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_mono_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_rgbfilter();}
		else{__h_tcrld_sprite_mono_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_rgbfilter();}
		else{__x_tcrld_sprite_mono_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_MONO_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_mono_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_clr_rgbfilter();}
		else{__h_tcrld_normal_mono_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_clr_rgbfilter();}
		else{__x_tcrld_normal_mono_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_mono_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_clr_rgbfilter();}
		else{__h_tcrld_sprite_mono_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_clr_rgbfilter();}
		else{__x_tcrld_sprite_mono_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ変色通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_mono_change_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_rgbfilter();}
		else{__h_tcrld_normal_mono_change_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_rgbfilter();}
		else{__x_tcrld_normal_mono_change_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ変色スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_mono_change_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_rgbfilter();}
		else{__h_tcrld_sprite_mono_change_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_rgbfilter();}
		else{__x_tcrld_sprite_mono_change_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ変色透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_mono_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_clr_rgbfilter();}
		else{__h_tcrld_normal_mono_change_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_clr_rgbfilter();}
		else{__x_tcrld_normal_mono_change_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ変色透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_mono_change_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_clr_rgbfilter();}
		else{__h_tcrld_sprite_mono_change_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_clr_rgbfilter();}
		else{__x_tcrld_sprite_mono_change_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ明暗通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_mono_bright_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_bright_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_bright_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_bright_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_bright_rgbfilter();}
		else{__h_tcrld_normal_mono_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_bright_rgbfilter();}
		else{__x_tcrld_normal_mono_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ明暗スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_mono_bright_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_bright_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_bright_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_bright_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_bright_rgbfilter();}
		else{__h_tcrld_sprite_mono_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_bright_rgbfilter();}
		else{__x_tcrld_sprite_mono_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ明暗透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_mono_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_bright_clr_rgbfilter();}
		else{__h_tcrld_normal_mono_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_bright_clr_rgbfilter();}
		else{__x_tcrld_normal_mono_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ明暗透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_mono_bright_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_bright_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_bright_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_bright_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_bright_clr_rgbfilter();}
		else{__h_tcrld_sprite_mono_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_bright_clr_rgbfilter();}
		else{__x_tcrld_sprite_mono_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ変色明暗通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_mono_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_bright_rgbfilter();}
		else{__h_tcrld_normal_mono_change_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_bright_rgbfilter();}
		else{__x_tcrld_normal_mono_change_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ変色明暗スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_mono_change_bright_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_bright_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_bright_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_bright_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_bright_rgbfilter();}
		else{__h_tcrld_sprite_mono_change_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_bright_rgbfilter();}
		else{__x_tcrld_sprite_mono_change_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ変色明暗透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_mono_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_bright_clr_rgbfilter();}
		else{__h_tcrld_normal_mono_change_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_bright_clr_rgbfilter();}
		else{__x_tcrld_normal_mono_change_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// モノ変色明暗透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_mono_change_bright_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_bright_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_bright_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_bright_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_bright_clr_rgbfilter();}
		else{__h_tcrld_sprite_mono_change_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_bright_clr_rgbfilter();}
		else{__x_tcrld_sprite_mono_change_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_reverse_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_rgbfilter();}
		else{__h_tcrld_normal_reverse_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_rgbfilter();}
		else{__x_tcrld_normal_reverse_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_REVERSE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_reverse_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_rgbfilter();}
		else{__h_tcrld_sprite_reverse_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_rgbfilter();}
		else{__x_tcrld_sprite_reverse_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_reverse_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_clr_rgbfilter();}
		else{__h_tcrld_normal_reverse_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_clr_rgbfilter();}
		else{__x_tcrld_normal_reverse_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_reverse_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_clr_rgbfilter();}
		else{__h_tcrld_sprite_reverse_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_clr_rgbfilter();}
		else{__x_tcrld_sprite_reverse_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転変色通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_reverse_change_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_rgbfilter();}
		else{__h_tcrld_normal_reverse_change_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_rgbfilter();}
		else{__x_tcrld_normal_reverse_change_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転変色スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_REVERSE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_reverse_change_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_rgbfilter();}
		else{__h_tcrld_sprite_reverse_change_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_rgbfilter();}
		else{__x_tcrld_sprite_reverse_change_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転変色透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_reverse_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_clr_rgbfilter();}
		else{__h_tcrld_normal_reverse_change_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_clr_rgbfilter();}
		else{__x_tcrld_normal_reverse_change_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転変色透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_reverse_change_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_clr_rgbfilter();}
		else{__h_tcrld_sprite_reverse_change_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_clr_rgbfilter();}
		else{__x_tcrld_sprite_reverse_change_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転明暗通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_reverse_bright_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_bright_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_bright_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_bright_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_bright_rgbfilter();}
		else{__h_tcrld_normal_reverse_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_bright_rgbfilter();}
		else{__x_tcrld_normal_reverse_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転明暗スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_reverse_bright_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_bright_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_bright_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_bright_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_bright_rgbfilter();}
		else{__h_tcrld_sprite_reverse_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_bright_rgbfilter();}
		else{__x_tcrld_sprite_reverse_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転明暗透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_reverse_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_bright_clr_rgbfilter();}
		else{__h_tcrld_normal_reverse_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_bright_clr_rgbfilter();}
		else{__x_tcrld_normal_reverse_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転明暗透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_reverse_bright_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_bright_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_bright_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_bright_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_bright_clr_rgbfilter();}
		else{__h_tcrld_sprite_reverse_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_bright_clr_rgbfilter();}
		else{__x_tcrld_sprite_reverse_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転変色明暗通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_reverse_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_bright_rgbfilter();}
		else{__h_tcrld_normal_reverse_change_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_bright_rgbfilter();}
		else{__x_tcrld_normal_reverse_change_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転変色明暗スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_reverse_change_bright_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_bright_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_bright_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_bright_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_bright_rgbfilter();}
		else{__h_tcrld_sprite_reverse_change_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_bright_rgbfilter();}
		else{__x_tcrld_sprite_reverse_change_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転変色明暗透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_reverse_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_bright_clr_rgbfilter();}
		else{__h_tcrld_normal_reverse_change_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_bright_clr_rgbfilter();}
		else{__x_tcrld_normal_reverse_change_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転変色明暗透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_reverse_change_bright_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_bright_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_bright_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_bright_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_bright_clr_rgbfilter();}
		else{__h_tcrld_sprite_reverse_change_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_bright_clr_rgbfilter();}
		else{__x_tcrld_sprite_reverse_change_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_reverse_mono_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_rgbfilter();}
		else{__h_tcrld_normal_reverse_mono_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_rgbfilter();}
		else{__x_tcrld_normal_reverse_mono_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノスプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_reverse_mono_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_rgbfilter();}
		else{__h_tcrld_sprite_reverse_mono_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_rgbfilter();}
		else{__x_tcrld_sprite_reverse_mono_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_reverse_mono_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_clr_rgbfilter();}
		else{__h_tcrld_normal_reverse_mono_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_clr_rgbfilter();}
		else{__x_tcrld_normal_reverse_mono_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_reverse_mono_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_clr_rgbfilter();}
		else{__h_tcrld_sprite_reverse_mono_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_clr_rgbfilter();}
		else{__x_tcrld_sprite_reverse_mono_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_reverse_mono_change_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_rgbfilter();}
		else{__h_tcrld_normal_reverse_mono_change_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_rgbfilter();}
		else{__x_tcrld_normal_reverse_mono_change_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_reverse_mono_change_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_rgbfilter();}
		else{__h_tcrld_sprite_reverse_mono_change_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_rgbfilter();}
		else{__x_tcrld_sprite_reverse_mono_change_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_reverse_mono_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_clr_rgbfilter();}
		else{__h_tcrld_normal_reverse_mono_change_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_clr_rgbfilter();}
		else{__x_tcrld_normal_reverse_mono_change_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_reverse_mono_change_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_clr_rgbfilter();}
		else{__h_tcrld_sprite_reverse_mono_change_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_clr_rgbfilter();}
		else{__x_tcrld_sprite_reverse_mono_change_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ明暗通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_reverse_mono_bright_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_bright_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_bright_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_bright_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_bright_rgbfilter();}
		else{__h_tcrld_normal_reverse_mono_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_bright_rgbfilter();}
		else{__x_tcrld_normal_reverse_mono_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ明暗スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_reverse_mono_bright_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_bright_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_bright_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_bright_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_bright_rgbfilter();}
		else{__h_tcrld_sprite_reverse_mono_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_bright_rgbfilter();}
		else{__x_tcrld_sprite_reverse_mono_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ明暗透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_reverse_mono_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_bright_clr_rgbfilter();}
		else{__h_tcrld_normal_reverse_mono_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_bright_clr_rgbfilter();}
		else{__x_tcrld_normal_reverse_mono_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ明暗透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_reverse_mono_bright_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_bright_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_bright_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_bright_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_bright_clr_rgbfilter();}
		else{__h_tcrld_sprite_reverse_mono_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_bright_clr_rgbfilter();}
		else{__x_tcrld_sprite_reverse_mono_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色明暗通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERSET_R

void __p_tcrld_normal_reverse_mono_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_bright_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_bright_rgbfilter();}
		else{__h_tcrld_normal_reverse_mono_change_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_bright_rgbfilter();}
		else{__x_tcrld_normal_reverse_mono_change_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色明暗スプライト表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_sprite_reverse_mono_change_bright_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_bright_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_bright_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_bright_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_bright_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_bright_rgbfilter();}
		else{__h_tcrld_sprite_reverse_mono_change_bright_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_bright_rgbfilter();}
		else{__x_tcrld_sprite_reverse_mono_change_bright_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色明暗透明通常表示（ＲＧＢフィルター）

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_RGBFILTERCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_RGBFILTERCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_RGBFILTERCLRSET_R

void __p_tcrld_normal_reverse_mono_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_bright_clr_rgbfilter(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_bright_clr_rgbfilter();}
		else{__h_tcrld_normal_reverse_mono_change_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_bright_clr_rgbfilter();}
		else{__x_tcrld_normal_reverse_mono_change_bright_clr_rgbfilter();}
	}
}

//----------------------------------------------------------------------
// 反転モノ変色明暗透明スプライト表示（ＲＧＢフィルター）

void __p_tcrld_sprite_reverse_mono_change_bright_clr_rgbfilter(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_bright_clr_rgbfilter(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_bright_clr_rgbfilter(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_bright_clr_rgbfilter(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_bright_clr_rgbfilter(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_bright_clr_rgbfilter();}
		else{__h_tcrld_sprite_reverse_mono_change_bright_clr_rgbfilter();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_bright_clr_rgbfilter();}
		else{__x_tcrld_sprite_reverse_mono_change_bright_clr_rgbfilter();}
	}
}

