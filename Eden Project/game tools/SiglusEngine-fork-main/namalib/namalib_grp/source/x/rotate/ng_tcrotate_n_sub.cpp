#include	"pch.h"
#include	"ng_macro.h"
#include	"ng_tcrotate_macro.h"


//----------------------------------------------------------------------
// �ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL	\
			GRPMAC_GETSRC_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_SUBSET_R

void __p_tcrld_normal_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL GRPMACRLD_P_END}
}
void __h_tcrld_normal_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL GRPMACRLD_H_END}
}
void __v_tcrld_normal_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL GRPMACRLD_V_END}
}
void __x_tcrld_normal_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_sub();}
		else{__h_tcrld_normal_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_sub();}
		else{__x_tcrld_normal_sub();}
	}
}

//----------------------------------------------------------------------
// �X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE	\
			GRPMAC_GETSRC_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_sub();}
		else{__h_tcrld_sprite_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_sub();}
		else{__x_tcrld_sprite_sub();}
	}
}

//----------------------------------------------------------------------
// �����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_CLR	\
			GRPMAC_GETSRC_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_clr_sub();}
		else{__h_tcrld_normal_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_clr_sub();}
		else{__x_tcrld_normal_clr_sub();}
	}
}

//----------------------------------------------------------------------
// �����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_clr_sub();}
		else{__h_tcrld_sprite_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_clr_sub();}
		else{__x_tcrld_sprite_clr_sub();}
	}
}

//----------------------------------------------------------------------
// �ϐF�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_SUBSET_R

void __p_tcrld_normal_change_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_sub();}
		else{__h_tcrld_normal_change_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_sub();}
		else{__x_tcrld_normal_change_sub();}
	}
}

//----------------------------------------------------------------------
// �ϐF�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_change_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_sub();}
		else{__h_tcrld_sprite_change_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_sub();}
		else{__x_tcrld_sprite_change_sub();}
	}
}

//----------------------------------------------------------------------
// �ϐF�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_CHANGE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_change_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_clr_sub();}
		else{__h_tcrld_normal_change_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_clr_sub();}
		else{__x_tcrld_normal_change_clr_sub();}
	}
}

//----------------------------------------------------------------------
// �ϐF�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_change_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_clr_sub();}
		else{__h_tcrld_sprite_change_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_clr_sub();}
		else{__x_tcrld_sprite_change_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_SUBSET_R

void __p_tcrld_normal_bright_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_bright_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_bright_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_bright_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_bright_sub();}
		else{__h_tcrld_normal_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_bright_sub();}
		else{__x_tcrld_normal_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_bright_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_bright_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_bright_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_bright_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_bright_sub();}
		else{__h_tcrld_sprite_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_bright_sub();}
		else{__x_tcrld_sprite_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_bright_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_bright_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_bright_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_bright_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_bright_clr_sub();}
		else{__h_tcrld_normal_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_bright_clr_sub();}
		else{__x_tcrld_normal_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_bright_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_bright_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_bright_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_bright_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_bright_clr_sub();}
		else{__h_tcrld_sprite_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_bright_clr_sub();}
		else{__x_tcrld_sprite_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// �ϐF���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBSET_R

void __p_tcrld_normal_change_bright_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_bright_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_bright_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_bright_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_bright_sub();}
		else{__h_tcrld_normal_change_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_bright_sub();}
		else{__x_tcrld_normal_change_bright_sub();}
	}
}

//----------------------------------------------------------------------
// �ϐF���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_change_bright_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_bright_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_bright_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_bright_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_bright_sub();}
		else{__h_tcrld_sprite_change_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_bright_sub();}
		else{__x_tcrld_sprite_change_bright_sub();}
	}
}

//----------------------------------------------------------------------
// �ϐF���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_change_bright_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_bright_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_bright_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_bright_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_bright_clr_sub();}
		else{__h_tcrld_normal_change_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_bright_clr_sub();}
		else{__x_tcrld_normal_change_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// �ϐF���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_change_bright_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_bright_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_bright_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_bright_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_bright_clr_sub();}
		else{__h_tcrld_sprite_change_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_bright_clr_sub();}
		else{__x_tcrld_sprite_change_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_SUBSET_R

void __p_tcrld_normal_mono_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_sub();}
		else{__h_tcrld_normal_mono_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_sub();}
		else{__x_tcrld_normal_mono_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_mono_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_sub();}
		else{__h_tcrld_sprite_mono_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_sub();}
		else{__x_tcrld_sprite_mono_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_mono_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_clr_sub();}
		else{__h_tcrld_normal_mono_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_clr_sub();}
		else{__x_tcrld_normal_mono_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_mono_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_clr_sub();}
		else{__h_tcrld_sprite_mono_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_clr_sub();}
		else{__x_tcrld_sprite_mono_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_SUBSET_R

void __p_tcrld_normal_mono_change_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_sub();}
		else{__h_tcrld_normal_mono_change_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_sub();}
		else{__x_tcrld_normal_mono_change_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_mono_change_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_sub();}
		else{__h_tcrld_sprite_mono_change_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_sub();}
		else{__x_tcrld_sprite_mono_change_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_mono_change_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_clr_sub();}
		else{__h_tcrld_normal_mono_change_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_clr_sub();}
		else{__x_tcrld_normal_mono_change_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_mono_change_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_clr_sub();}
		else{__h_tcrld_sprite_mono_change_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_clr_sub();}
		else{__x_tcrld_sprite_mono_change_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���m���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_SUBSET_R

void __p_tcrld_normal_mono_bright_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_bright_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_bright_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_bright_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_bright_sub();}
		else{__h_tcrld_normal_mono_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_bright_sub();}
		else{__x_tcrld_normal_mono_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���m���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_mono_bright_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_bright_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_bright_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_bright_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_bright_sub();}
		else{__h_tcrld_sprite_mono_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_bright_sub();}
		else{__x_tcrld_sprite_mono_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���m���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_mono_bright_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_bright_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_bright_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_bright_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_bright_clr_sub();}
		else{__h_tcrld_normal_mono_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_bright_clr_sub();}
		else{__x_tcrld_normal_mono_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���m���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_mono_bright_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_bright_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_bright_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_bright_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_bright_clr_sub();}
		else{__h_tcrld_sprite_mono_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_bright_clr_sub();}
		else{__x_tcrld_sprite_mono_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBSET_R

void __p_tcrld_normal_mono_change_bright_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_bright_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_bright_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_bright_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_bright_sub();}
		else{__h_tcrld_normal_mono_change_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_bright_sub();}
		else{__x_tcrld_normal_mono_change_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_mono_change_bright_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_bright_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_bright_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_bright_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_bright_sub();}
		else{__h_tcrld_sprite_mono_change_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_bright_sub();}
		else{__x_tcrld_sprite_mono_change_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_mono_change_bright_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_bright_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_bright_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_bright_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_bright_clr_sub();}
		else{__h_tcrld_normal_mono_change_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_bright_clr_sub();}
		else{__x_tcrld_normal_mono_change_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_mono_change_bright_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_bright_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_bright_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_bright_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_bright_clr_sub();}
		else{__h_tcrld_sprite_mono_change_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_bright_clr_sub();}
		else{__x_tcrld_sprite_mono_change_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_SUBSET_R

void __p_tcrld_normal_reverse_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_sub();}
		else{__h_tcrld_normal_reverse_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_sub();}
		else{__x_tcrld_normal_reverse_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_reverse_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_sub();}
		else{__h_tcrld_sprite_reverse_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_sub();}
		else{__x_tcrld_sprite_reverse_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_reverse_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_clr_sub();}
		else{__h_tcrld_normal_reverse_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_clr_sub();}
		else{__x_tcrld_normal_reverse_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_clr_sub();}
		else{__h_tcrld_sprite_reverse_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_clr_sub();}
		else{__x_tcrld_sprite_reverse_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_SUBSET_R

void __p_tcrld_normal_reverse_change_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_sub();}
		else{__h_tcrld_normal_reverse_change_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_sub();}
		else{__x_tcrld_normal_reverse_change_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_reverse_change_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_sub();}
		else{__h_tcrld_sprite_reverse_change_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_sub();}
		else{__x_tcrld_sprite_reverse_change_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_reverse_change_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_clr_sub();}
		else{__h_tcrld_normal_reverse_change_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_clr_sub();}
		else{__x_tcrld_normal_reverse_change_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_change_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_clr_sub();}
		else{__h_tcrld_sprite_reverse_change_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_clr_sub();}
		else{__x_tcrld_sprite_reverse_change_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SUBSET_R

void __p_tcrld_normal_reverse_bright_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_bright_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_bright_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_bright_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_bright_sub();}
		else{__h_tcrld_normal_reverse_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_bright_sub();}
		else{__x_tcrld_normal_reverse_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_reverse_bright_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_bright_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_bright_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_bright_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_bright_sub();}
		else{__h_tcrld_sprite_reverse_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_bright_sub();}
		else{__x_tcrld_sprite_reverse_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_bright_clr_sub();}
		else{__h_tcrld_normal_reverse_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_bright_clr_sub();}
		else{__x_tcrld_normal_reverse_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_bright_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_bright_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_bright_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_bright_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_bright_clr_sub();}
		else{__h_tcrld_sprite_reverse_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_bright_clr_sub();}
		else{__x_tcrld_sprite_reverse_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBSET_R

void __p_tcrld_normal_reverse_change_bright_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_bright_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_bright_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_bright_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_bright_sub();}
		else{__h_tcrld_normal_reverse_change_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_bright_sub();}
		else{__x_tcrld_normal_reverse_change_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_reverse_change_bright_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_bright_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_bright_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_bright_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_bright_sub();}
		else{__h_tcrld_sprite_reverse_change_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_bright_sub();}
		else{__x_tcrld_sprite_reverse_change_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_reverse_change_bright_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_bright_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_bright_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_bright_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_bright_clr_sub();}
		else{__h_tcrld_normal_reverse_change_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_bright_clr_sub();}
		else{__x_tcrld_normal_reverse_change_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_change_bright_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_bright_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_bright_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_bright_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_bright_clr_sub();}
		else{__h_tcrld_sprite_reverse_change_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_bright_clr_sub();}
		else{__x_tcrld_sprite_reverse_change_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_SUBSET_R

void __p_tcrld_normal_reverse_mono_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_sub();}
		else{__h_tcrld_normal_reverse_mono_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_sub();}
		else{__x_tcrld_normal_reverse_mono_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_reverse_mono_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_sub();}
		else{__h_tcrld_sprite_reverse_mono_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_sub();}
		else{__x_tcrld_sprite_reverse_mono_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_reverse_mono_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_clr_sub();}
		else{__h_tcrld_normal_reverse_mono_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_clr_sub();}
		else{__x_tcrld_normal_reverse_mono_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_mono_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_clr_sub();}
		else{__h_tcrld_sprite_reverse_mono_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_clr_sub();}
		else{__x_tcrld_sprite_reverse_mono_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_SUBSET_R

void __p_tcrld_normal_reverse_mono_change_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_sub();}
		else{__h_tcrld_normal_reverse_mono_change_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_sub();}
		else{__x_tcrld_normal_reverse_mono_change_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_reverse_mono_change_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_sub();}
		else{__h_tcrld_sprite_reverse_mono_change_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_sub();}
		else{__x_tcrld_sprite_reverse_mono_change_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_reverse_mono_change_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_clr_sub();}
		else{__h_tcrld_normal_reverse_mono_change_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_clr_sub();}
		else{__x_tcrld_normal_reverse_mono_change_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_mono_change_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_clr_sub();}
		else{__h_tcrld_sprite_reverse_mono_change_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_clr_sub();}
		else{__x_tcrld_sprite_reverse_mono_change_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SUBSET_R

void __p_tcrld_normal_reverse_mono_bright_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_bright_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_bright_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_bright_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_bright_sub();}
		else{__h_tcrld_normal_reverse_mono_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_bright_sub();}
		else{__x_tcrld_normal_reverse_mono_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_reverse_mono_bright_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_bright_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_bright_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_bright_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_bright_sub();}
		else{__h_tcrld_sprite_reverse_mono_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_bright_sub();}
		else{__x_tcrld_sprite_reverse_mono_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_reverse_mono_bright_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_bright_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_bright_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_bright_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_bright_clr_sub();}
		else{__h_tcrld_normal_reverse_mono_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_bright_clr_sub();}
		else{__x_tcrld_normal_reverse_mono_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_mono_bright_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_bright_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_bright_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_bright_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_bright_clr_sub();}
		else{__h_tcrld_sprite_reverse_mono_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_bright_clr_sub();}
		else{__x_tcrld_sprite_reverse_mono_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBSET_R

void __p_tcrld_normal_reverse_mono_change_bright_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_bright_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_bright_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_bright_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_bright_sub();}
		else{__h_tcrld_normal_reverse_mono_change_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_bright_sub();}
		else{__x_tcrld_normal_reverse_mono_change_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_sprite_reverse_mono_change_bright_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_bright_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_bright_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_bright_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_bright_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_bright_sub();}
		else{__h_tcrld_sprite_reverse_mono_change_bright_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_bright_sub();}
		else{__x_tcrld_sprite_reverse_mono_change_bright_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __p_tcrld_normal_reverse_mono_change_bright_clr_sub(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_bright_clr_sub(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_bright_clr_sub(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_bright_clr_sub(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_bright_clr_sub();}
		else{__h_tcrld_normal_reverse_mono_change_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_bright_clr_sub();}
		else{__x_tcrld_normal_reverse_mono_change_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_mono_change_bright_clr_sub(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_bright_clr_sub(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_bright_clr_sub(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_bright_clr_sub(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_bright_clr_sub(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_bright_clr_sub();}
		else{__h_tcrld_sprite_reverse_mono_change_bright_clr_sub();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_bright_clr_sub();}
		else{__x_tcrld_sprite_reverse_mono_change_bright_clr_sub();}
	}
}

