#include	"pch.h"
#include	"ng_macro.h"
#include	"ng_tcrotate_macro.h"


//----------------------------------------------------------------------
// �ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL	\
			GRPMAC_GETSRC_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_ADDSET_R

void __p_tcrld_normal_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL GRPMACRLD_P_END}
}
void __h_tcrld_normal_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL GRPMACRLD_H_END}
}
void __v_tcrld_normal_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL GRPMACRLD_V_END}
}
void __x_tcrld_normal_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_add();}
		else{__h_tcrld_normal_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_add();}
		else{__x_tcrld_normal_add();}
	}
}

//----------------------------------------------------------------------
// �X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE	\
			GRPMAC_GETSRC_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_add();}
		else{__h_tcrld_sprite_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_add();}
		else{__x_tcrld_sprite_add();}
	}
}

//----------------------------------------------------------------------
// �����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_CLR	\
			GRPMAC_GETSRC_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_clr_add();}
		else{__h_tcrld_normal_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_clr_add();}
		else{__x_tcrld_normal_clr_add();}
	}
}

//----------------------------------------------------------------------
// �����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_clr_add();}
		else{__h_tcrld_sprite_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_clr_add();}
		else{__x_tcrld_sprite_clr_add();}
	}
}

//----------------------------------------------------------------------
// �ϐF�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_ADDSET_R

void __p_tcrld_normal_change_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_add();}
		else{__h_tcrld_normal_change_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_add();}
		else{__x_tcrld_normal_change_add();}
	}
}

//----------------------------------------------------------------------
// �ϐF�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_change_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_add();}
		else{__h_tcrld_sprite_change_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_add();}
		else{__x_tcrld_sprite_change_add();}
	}
}

//----------------------------------------------------------------------
// �ϐF�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_CHANGE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_change_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_clr_add();}
		else{__h_tcrld_normal_change_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_clr_add();}
		else{__x_tcrld_normal_change_clr_add();}
	}
}

//----------------------------------------------------------------------
// �ϐF�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_change_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_clr_add();}
		else{__h_tcrld_sprite_change_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_clr_add();}
		else{__x_tcrld_sprite_change_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_ADDSET_R

void __p_tcrld_normal_bright_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_bright_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_bright_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_bright_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_bright_add();}
		else{__h_tcrld_normal_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_bright_add();}
		else{__x_tcrld_normal_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_bright_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_bright_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_bright_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_bright_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_bright_add();}
		else{__h_tcrld_sprite_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_bright_add();}
		else{__x_tcrld_sprite_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_bright_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_bright_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_bright_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_bright_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_bright_clr_add();}
		else{__h_tcrld_normal_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_bright_clr_add();}
		else{__x_tcrld_normal_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_bright_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_bright_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_bright_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_bright_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_bright_clr_add();}
		else{__h_tcrld_sprite_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_bright_clr_add();}
		else{__x_tcrld_sprite_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// �ϐF���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDSET_R

void __p_tcrld_normal_change_bright_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_bright_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_bright_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_bright_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_bright_add();}
		else{__h_tcrld_normal_change_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_bright_add();}
		else{__x_tcrld_normal_change_bright_add();}
	}
}

//----------------------------------------------------------------------
// �ϐF���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_change_bright_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_bright_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_bright_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_bright_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_bright_add();}
		else{__h_tcrld_sprite_change_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_bright_add();}
		else{__x_tcrld_sprite_change_bright_add();}
	}
}

//----------------------------------------------------------------------
// �ϐF���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_change_bright_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_change_bright_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_change_bright_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_change_bright_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_change_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_change_bright_clr_add();}
		else{__h_tcrld_normal_change_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_change_bright_clr_add();}
		else{__x_tcrld_normal_change_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// �ϐF���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_change_bright_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_change_bright_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_change_bright_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_change_bright_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_change_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_change_bright_clr_add();}
		else{__h_tcrld_sprite_change_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_change_bright_clr_add();}
		else{__x_tcrld_sprite_change_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���m�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_ADDSET_R

void __p_tcrld_normal_mono_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_add();}
		else{__h_tcrld_normal_mono_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_add();}
		else{__x_tcrld_normal_mono_add();}
	}
}

//----------------------------------------------------------------------
// ���m�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_mono_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_add();}
		else{__h_tcrld_sprite_mono_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_add();}
		else{__x_tcrld_sprite_mono_add();}
	}
}

//----------------------------------------------------------------------
// ���m�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_mono_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_clr_add();}
		else{__h_tcrld_normal_mono_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_clr_add();}
		else{__x_tcrld_normal_mono_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���m�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_mono_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_clr_add();}
		else{__h_tcrld_sprite_mono_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_clr_add();}
		else{__x_tcrld_sprite_mono_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_ADDSET_R

void __p_tcrld_normal_mono_change_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_add();}
		else{__h_tcrld_normal_mono_change_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_add();}
		else{__x_tcrld_normal_mono_change_add();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_mono_change_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_add();}
		else{__h_tcrld_sprite_mono_change_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_add();}
		else{__x_tcrld_sprite_mono_change_add();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_mono_change_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_clr_add();}
		else{__h_tcrld_normal_mono_change_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_clr_add();}
		else{__x_tcrld_normal_mono_change_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_mono_change_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_clr_add();}
		else{__h_tcrld_sprite_mono_change_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_clr_add();}
		else{__x_tcrld_sprite_mono_change_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���m���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_ADDSET_R

void __p_tcrld_normal_mono_bright_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_bright_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_bright_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_bright_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_bright_add();}
		else{__h_tcrld_normal_mono_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_bright_add();}
		else{__x_tcrld_normal_mono_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���m���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_mono_bright_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_bright_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_bright_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_bright_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_bright_add();}
		else{__h_tcrld_sprite_mono_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_bright_add();}
		else{__x_tcrld_sprite_mono_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���m���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_mono_bright_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_bright_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_bright_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_bright_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_bright_clr_add();}
		else{__h_tcrld_normal_mono_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_bright_clr_add();}
		else{__x_tcrld_normal_mono_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���m���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_mono_bright_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_bright_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_bright_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_bright_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_bright_clr_add();}
		else{__h_tcrld_sprite_mono_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_bright_clr_add();}
		else{__x_tcrld_sprite_mono_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDSET_R

void __p_tcrld_normal_mono_change_bright_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_bright_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_bright_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_bright_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_bright_add();}
		else{__h_tcrld_normal_mono_change_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_bright_add();}
		else{__x_tcrld_normal_mono_change_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_mono_change_bright_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_bright_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_bright_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_bright_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_bright_add();}
		else{__h_tcrld_sprite_mono_change_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_bright_add();}
		else{__x_tcrld_sprite_mono_change_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_mono_change_bright_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_mono_change_bright_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_mono_change_bright_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_mono_change_bright_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_mono_change_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_mono_change_bright_clr_add();}
		else{__h_tcrld_normal_mono_change_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_mono_change_bright_clr_add();}
		else{__x_tcrld_normal_mono_change_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���m�ϐF���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_mono_change_bright_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_mono_change_bright_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_mono_change_bright_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_mono_change_bright_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_mono_change_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_mono_change_bright_clr_add();}
		else{__h_tcrld_sprite_mono_change_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_mono_change_bright_clr_add();}
		else{__x_tcrld_sprite_mono_change_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_ADDSET_R

void __p_tcrld_normal_reverse_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_add();}
		else{__h_tcrld_normal_reverse_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_add();}
		else{__x_tcrld_normal_reverse_add();}
	}
}

//----------------------------------------------------------------------
// ���]�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_reverse_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_add();}
		else{__h_tcrld_sprite_reverse_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_add();}
		else{__x_tcrld_sprite_reverse_add();}
	}
}

//----------------------------------------------------------------------
// ���]�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_reverse_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_clr_add();}
		else{__h_tcrld_normal_reverse_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_clr_add();}
		else{__x_tcrld_normal_reverse_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_clr_add();}
		else{__h_tcrld_sprite_reverse_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_clr_add();}
		else{__x_tcrld_sprite_reverse_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_ADDSET_R

void __p_tcrld_normal_reverse_change_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_add();}
		else{__h_tcrld_normal_reverse_change_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_add();}
		else{__x_tcrld_normal_reverse_change_add();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_CHANGE	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_reverse_change_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_add();}
		else{__h_tcrld_sprite_reverse_change_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_add();}
		else{__x_tcrld_sprite_reverse_change_add();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_reverse_change_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_clr_add();}
		else{__h_tcrld_normal_reverse_change_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_clr_add();}
		else{__x_tcrld_normal_reverse_change_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_change_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_clr_add();}
		else{__h_tcrld_sprite_reverse_change_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_clr_add();}
		else{__x_tcrld_sprite_reverse_change_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_ADDSET_R

void __p_tcrld_normal_reverse_bright_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_bright_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_bright_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_bright_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_bright_add();}
		else{__h_tcrld_normal_reverse_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_bright_add();}
		else{__x_tcrld_normal_reverse_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���]���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_reverse_bright_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_bright_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_bright_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_bright_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_bright_add();}
		else{__h_tcrld_sprite_reverse_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_bright_add();}
		else{__x_tcrld_sprite_reverse_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���]���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_reverse_bright_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_bright_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_bright_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_bright_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_bright_clr_add();}
		else{__h_tcrld_normal_reverse_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_bright_clr_add();}
		else{__x_tcrld_normal_reverse_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_bright_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_bright_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_bright_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_bright_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_bright_clr_add();}
		else{__h_tcrld_sprite_reverse_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_bright_clr_add();}
		else{__x_tcrld_sprite_reverse_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDSET_R

void __p_tcrld_normal_reverse_change_bright_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_bright_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_bright_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_bright_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_bright_add();}
		else{__h_tcrld_normal_reverse_change_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_bright_add();}
		else{__x_tcrld_normal_reverse_change_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_reverse_change_bright_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_bright_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_bright_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_bright_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_bright_add();}
		else{__h_tcrld_sprite_reverse_change_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_bright_add();}
		else{__x_tcrld_sprite_reverse_change_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR	\
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_reverse_change_bright_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_change_bright_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_change_bright_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_change_bright_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_change_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_change_bright_clr_add();}
		else{__h_tcrld_normal_reverse_change_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_change_bright_clr_add();}
		else{__x_tcrld_normal_reverse_change_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]�ϐF���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_change_bright_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_change_bright_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_change_bright_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_change_bright_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_change_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_change_bright_clr_add();}
		else{__h_tcrld_sprite_reverse_change_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_change_bright_clr_add();}
		else{__x_tcrld_sprite_reverse_change_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_ADDSET_R

void __p_tcrld_normal_reverse_mono_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_add();}
		else{__h_tcrld_normal_reverse_mono_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_add();}
		else{__x_tcrld_normal_reverse_mono_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_reverse_mono_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_add();}
		else{__h_tcrld_sprite_reverse_mono_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_add();}
		else{__x_tcrld_sprite_reverse_mono_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_reverse_mono_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_clr_add();}
		else{__h_tcrld_normal_reverse_mono_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_clr_add();}
		else{__x_tcrld_normal_reverse_mono_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_mono_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_clr_add();}
		else{__h_tcrld_sprite_reverse_mono_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_clr_add();}
		else{__x_tcrld_sprite_reverse_mono_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF�ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_ADDSET_R

void __p_tcrld_normal_reverse_mono_change_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_add();}
		else{__h_tcrld_normal_reverse_mono_change_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_add();}
		else{__x_tcrld_normal_reverse_mono_change_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF�X�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_reverse_mono_change_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_add();}
		else{__h_tcrld_sprite_reverse_mono_change_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_add();}
		else{__x_tcrld_sprite_reverse_mono_change_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF�����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_reverse_mono_change_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_clr_add();}
		else{__h_tcrld_normal_reverse_mono_change_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_clr_add();}
		else{__x_tcrld_normal_reverse_mono_change_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF�����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_mono_change_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_clr_add();}
		else{__h_tcrld_sprite_reverse_mono_change_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_clr_add();}
		else{__x_tcrld_sprite_reverse_mono_change_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_ADDSET_R

void __p_tcrld_normal_reverse_mono_bright_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_bright_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_bright_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_bright_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_bright_add();}
		else{__h_tcrld_normal_reverse_mono_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_bright_add();}
		else{__x_tcrld_normal_reverse_mono_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_reverse_mono_bright_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_bright_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_bright_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_bright_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_bright_add();}
		else{__h_tcrld_sprite_reverse_mono_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_bright_add();}
		else{__x_tcrld_sprite_reverse_mono_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_reverse_mono_bright_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_bright_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_bright_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_bright_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_bright_clr_add();}
		else{__h_tcrld_normal_reverse_mono_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_bright_clr_add();}
		else{__x_tcrld_normal_reverse_mono_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_mono_bright_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_bright_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_bright_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_bright_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_bright_clr_add();}
		else{__h_tcrld_sprite_reverse_mono_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_bright_clr_add();}
		else{__x_tcrld_sprite_reverse_mono_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF���Òʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDSET_R

void __p_tcrld_normal_reverse_mono_change_bright_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_bright_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_bright_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_bright_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_bright_add();}
		else{__h_tcrld_normal_reverse_mono_change_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_bright_add();}
		else{__x_tcrld_normal_reverse_mono_change_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF���ÃX�v���C�g�\���i���Z�j

#define		GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_sprite_reverse_mono_change_bright_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_bright_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_bright_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_bright_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITE
	grprldlv_step_normal: GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT __asm jmp grprldlv_step_end
	grprldlv_step_sprite: GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end:    GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_bright_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_bright_add();}
		else{__h_tcrld_sprite_reverse_mono_change_bright_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_bright_add();}
		else{__x_tcrld_sprite_reverse_mono_change_bright_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF���Ó����ʏ�\���i���Z�j

#define		GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR	\
			GRPMAC_MONO_READY \
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __p_tcrld_normal_reverse_mono_change_bright_clr_add(void){
	_asm{GRPMACRLD_P_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_P_END}
}
void __h_tcrld_normal_reverse_mono_change_bright_clr_add(void){
	_asm{GRPMACRLD_H_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_H_END}
}
void __v_tcrld_normal_reverse_mono_change_bright_clr_add(void){
	_asm{GRPMACRLD_V_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_V_END}
}
void __x_tcrld_normal_reverse_mono_change_bright_clr_add(void){
	_asm{GRPMACRLD_X_START GRPMACRLD_NROMALCLR GRPMACRLD_XX_NORMAL_REVERSE_MONO_CHANGE_BRIGHT_CLR GRPMACRLD_X_END}
}

void true_color_rotate_line_draw_normal_reverse_mono_change_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_normal_reverse_mono_change_bright_clr_add();}
		else{__h_tcrld_normal_reverse_mono_change_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_normal_reverse_mono_change_bright_clr_add();}
		else{__x_tcrld_normal_reverse_mono_change_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// ���]���m�ϐF���Ó����X�v���C�g�\���i���Z�j

void __p_tcrld_sprite_reverse_mono_change_bright_clr_add(void){
		GRPMACRLD_P_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_P_END
}
void __h_tcrld_sprite_reverse_mono_change_bright_clr_add(void){
		GRPMACRLD_H_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_H_END
}
void __v_tcrld_sprite_reverse_mono_change_bright_clr_add(void){
		GRPMACRLD_V_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_V_END
}
void __x_tcrld_sprite_reverse_mono_change_bright_clr_add(void){
		GRPMACRLD_X_START GRPMACRLD_SPRITECLR GRPMACRLD_XX_SPRITE_REVERSE_MONO_CHANGE_BRIGHT
	grprldlv_step_end: GRPMACRLD_X_END
}

void true_color_rotate_line_draw_sprite_reverse_mono_change_bright_clr_add(void){
	if(RLINEF_yl == 0){
		if(RLINEF_xl == 0){__p_tcrld_sprite_reverse_mono_change_bright_clr_add();}
		else{__h_tcrld_sprite_reverse_mono_change_bright_clr_add();}
	}
	else{
		if(RLINEF_xl == 0){__v_tcrld_sprite_reverse_mono_change_bright_clr_add();}
		else{__x_tcrld_sprite_reverse_mono_change_bright_clr_add();}
	}
}

