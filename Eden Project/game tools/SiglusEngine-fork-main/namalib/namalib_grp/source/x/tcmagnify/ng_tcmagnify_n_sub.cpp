#include	"pch.h"
#include	"ng_macro.h"
#include	"ng_tcmagnify_macro.h"

//----------------------------------------------------------------------
// 通常表示（減算）

#define		GRPMACMAG_XX_NORMAL	\
			GRPMAC_GETSRC_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_SUBSET_R

void __a0_tcmag_normal_sub(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_XX_NORMAL GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_sub(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_XX_NORMAL GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_sub(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_XX_NORMAL GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_sub(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_XX_NORMAL GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_sub(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_XX_NORMAL GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_sub(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_XX_NORMAL GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_sub(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_XX_NORMAL GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_sub(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_XX_NORMAL GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_sub(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_XX_NORMAL GRPMACMAG_C2_END}
}

void tcmag_normal_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_sub();}
		else{__a2_tcmag_normal_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_sub();}
		else{__b2_tcmag_normal_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_sub();}
		else{__c2_tcmag_normal_sub();}
	}
}

//----------------------------------------------------------------------
// スプライト表示（減算）

#define		GRPMACMAG_XX_SPRITE \
			GRPMAC_GETSRC_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_SUBCLRSET_R

void __a0_tcmag_sprite_sub(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_sub(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_sub(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_sub(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_sub(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_sub(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_sub(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_sub(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_sub(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_C2_END
	}
}

void tcmag_sprite_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_sub();}
		else{__a2_tcmag_sprite_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_sub();}
		else{__b2_tcmag_sprite_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_sub();}
		else{__c2_tcmag_sprite_sub();}
	}
}

//----------------------------------------------------------------------
// 透明通常表示（減算）

#define		GRPMACMAG_XX_NORMAL_CLR	\
			GRPMAC_GETSRC_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_SUBCLRSET_R

void __a0_tcmag_normal_clr_sub(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_clr_sub(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_clr_sub(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_clr_sub(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_clr_sub(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_clr_sub(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_clr_sub(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_clr_sub(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_clr_sub(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_C2_END}
}

void tcmag_normal_clr_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_clr_sub();}
		else{__a2_tcmag_normal_clr_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_clr_sub();}
		else{__b2_tcmag_normal_clr_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_clr_sub();}
		else{__c2_tcmag_normal_clr_sub();}
	}
}

//----------------------------------------------------------------------
// 透明スプライト表示（減算）

void __a0_tcmag_sprite_clr_sub(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_clr_sub(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_clr_sub(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_clr_sub(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_clr_sub(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_clr_sub(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_clr_sub(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_clr_sub(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_clr_sub(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_C2_END
	}
}

void tcmag_sprite_clr_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_clr_sub();}
		else{__a2_tcmag_sprite_clr_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_clr_sub();}
		else{__b2_tcmag_sprite_clr_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_clr_sub();}
		else{__c2_tcmag_sprite_clr_sub();}
	}
}

//----------------------------------------------------------------------
// 変色通常表示（減算）

//----------------------------------------------------------------------
// 変色スプライト表示（減算）

//----------------------------------------------------------------------
// 変色透明通常表示（減算）

//----------------------------------------------------------------------
// 変色透明スプライト表示（減算）

//----------------------------------------------------------------------
// 明暗通常表示（減算）

#define		GRPMACMAG_XX_NORMAL_BRIGHT \
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_SUBSET_R

void __a0_tcmag_normal_bright_sub(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_bright_sub(void){
	_asm{GRPMACMAG_A1_START  GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_bright_sub(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_bright_sub(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_bright_sub(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_bright_sub(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_bright_sub(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_bright_sub(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_bright_sub(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_C2_END}
}

void tcmag_normal_bright_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_bright_sub();}
		else{__a2_tcmag_normal_bright_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_bright_sub();}
		else{__b2_tcmag_normal_bright_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_bright_sub();}
		else{__c2_tcmag_normal_bright_sub();}
	}
}

//----------------------------------------------------------------------
// 明暗スプライト表示（減算）

#define		GRPMACMAG_XX_SPRITE_BRIGHT \
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __a0_tcmag_sprite_bright_sub(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_bright_sub(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_bright_sub(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_bright_sub(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_bright_sub(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_bright_sub(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_bright_sub(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_bright_sub(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_bright_sub(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C2_END
	}
}

void tcmag_sprite_bright_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_bright_sub();}
		else{__a2_tcmag_sprite_bright_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_bright_sub();}
		else{__b2_tcmag_sprite_bright_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_bright_sub();}
		else{__c2_tcmag_sprite_bright_sub();}
	}
}

//----------------------------------------------------------------------
// 明暗透明通常表示（減算）

#define		GRPMACMAG_XX_NORMAL_BRIGHT_CLR \
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __a0_tcmag_normal_bright_clr_sub(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_bright_clr_sub(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_bright_clr_sub(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_bright_clr_sub(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_bright_clr_sub(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_bright_clr_sub(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_bright_clr_sub(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_bright_clr_sub(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_bright_clr_sub(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_C2_END}
}

void tcmag_normal_bright_clr_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_bright_clr_sub();}
		else{__a2_tcmag_normal_bright_clr_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_bright_clr_sub();}
		else{__b2_tcmag_normal_bright_clr_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_bright_clr_sub();}
		else{__c2_tcmag_normal_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// 明暗透明スプライト表示（減算）

void __a0_tcmag_sprite_bright_clr_sub(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_bright_clr_sub(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_bright_clr_sub(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_bright_clr_sub(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_bright_clr_sub(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_bright_clr_sub(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_bright_clr_sub(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_bright_clr_sub(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_bright_clr_sub(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C2_END
	}
}

void tcmag_sprite_bright_clr_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_bright_clr_sub();}
		else{__a2_tcmag_sprite_bright_clr_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_bright_clr_sub();}
		else{__b2_tcmag_sprite_bright_clr_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_bright_clr_sub();}
		else{__c2_tcmag_sprite_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// 変色明暗通常表示（減算）

//----------------------------------------------------------------------
// 変色明暗スプライト表示（減算）

//----------------------------------------------------------------------
// 変色明暗透明通常表示（減算）

//----------------------------------------------------------------------
// 変色明暗透明スプライト表示（減算）

//----------------------------------------------------------------------
// モノ通常表示（減算）

//----------------------------------------------------------------------
// モノスプライト表示（減算）

//----------------------------------------------------------------------
// モノ透明通常表示（減算）

//----------------------------------------------------------------------
// モノ透明スプライト表示（減算）

//----------------------------------------------------------------------
// モノ変色通常表示（減算）

//----------------------------------------------------------------------
// モノ変色スプライト表示（減算）

//----------------------------------------------------------------------
// モノ変色透明通常表示（減算）

//----------------------------------------------------------------------
// モノ変色透明スプライト表示（減算）

//----------------------------------------------------------------------
// モノ明暗通常表示（減算）

//----------------------------------------------------------------------
// モノ明暗スプライト表示（減算）

//----------------------------------------------------------------------
// モノ明暗透明通常表示（減算）

//----------------------------------------------------------------------
// モノ明暗透明スプライト表示（減算）

//----------------------------------------------------------------------
// モノ変色明暗通常表示（減算）

//----------------------------------------------------------------------
// モノ変色明暗スプライト表示（減算）

//----------------------------------------------------------------------
// モノ変色明暗透明通常表示（減算）

//----------------------------------------------------------------------
// モノ変色明暗透明スプライト表示（減算）

//----------------------------------------------------------------------
// 反転通常表示（減算）

#define		GRPMACMAG_XX_NORMAL_REVERSE \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_SUBSET_R

void __a0_tcmag_normal_reverse_sub(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_reverse_sub(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_reverse_sub(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_reverse_sub(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_reverse_sub(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_reverse_sub(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_reverse_sub(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_reverse_sub(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_reverse_sub(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_C2_END}
}

void tcmag_normal_reverse_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_reverse_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_reverse_sub();}
		else{__a2_tcmag_normal_reverse_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_reverse_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_reverse_sub();}
		else{__b2_tcmag_normal_reverse_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_reverse_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_reverse_sub();}
		else{__c2_tcmag_normal_reverse_sub();}
	}
}

//----------------------------------------------------------------------
// 反転スプライト表示（減算）

#define		GRPMACMAG_XX_SPRITE_REVERSE \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_SUBCLRSET_R

void __a0_tcmag_sprite_reverse_sub(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_reverse_sub(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_reverse_sub(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_reverse_sub(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_reverse_sub(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_reverse_sub(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_reverse_sub(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_reverse_sub(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_reverse_sub(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_C2_END
	}
}

void tcmag_sprite_reverse_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_reverse_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_reverse_sub();}
		else{__a2_tcmag_sprite_reverse_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_reverse_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_reverse_sub();}
		else{__b2_tcmag_sprite_reverse_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_reverse_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_reverse_sub();}
		else{__c2_tcmag_sprite_reverse_sub();}
	}
}

//----------------------------------------------------------------------
// 反転透明通常表示（減算）

#define		GRPMACMAG_XX_NORMAL_REVERSE_CLR \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_SUBCLRSET_R

void __a0_tcmag_normal_reverse_clr_sub(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_reverse_clr_sub(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_reverse_clr_sub(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_reverse_clr_sub(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_reverse_clr_sub(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_reverse_clr_sub(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_reverse_clr_sub(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_reverse_clr_sub(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_reverse_clr_sub(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_C2_END}
}

void tcmag_normal_reverse_clr_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_reverse_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_reverse_clr_sub();}
		else{__a2_tcmag_normal_reverse_clr_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_reverse_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_reverse_clr_sub();}
		else{__b2_tcmag_normal_reverse_clr_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_reverse_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_reverse_clr_sub();}
		else{__c2_tcmag_normal_reverse_clr_sub();}
	}
}

//----------------------------------------------------------------------
// 反転透明スプライト表示（減算）

void __a0_tcmag_sprite_reverse_clr_sub(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_reverse_clr_sub(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_reverse_clr_sub(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_reverse_clr_sub(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_reverse_clr_sub(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_reverse_clr_sub(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_reverse_clr_sub(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_reverse_clr_sub(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_reverse_clr_sub(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_C2_END
	}
}

void tcmag_sprite_reverse_clr_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_reverse_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_reverse_clr_sub();}
		else{__a2_tcmag_sprite_reverse_clr_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_reverse_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_reverse_clr_sub();}
		else{__b2_tcmag_sprite_reverse_clr_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_reverse_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_reverse_clr_sub();}
		else{__c2_tcmag_sprite_reverse_clr_sub();}
	}
}

//----------------------------------------------------------------------
// 反転変色通常表示（減算）

//----------------------------------------------------------------------
// 反転変色スプライト表示（減算）

//----------------------------------------------------------------------
// 反転変色透明通常表示（減算）

//----------------------------------------------------------------------
// 反転変色透明スプライト表示（減算）

//----------------------------------------------------------------------
// 反転明暗通常表示（減算）

#define		GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SUBSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SUBSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SUBSET_R

void __a0_tcmag_normal_reverse_bright_sub(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_reverse_bright_sub(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_reverse_bright_sub(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_reverse_bright_sub(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_reverse_bright_sub(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_reverse_bright_sub(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_reverse_bright_sub(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_reverse_bright_sub(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_reverse_bright_sub(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_C2_END}
}

void tcmag_normal_reverse_bright_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_reverse_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_reverse_bright_sub();}
		else{__a2_tcmag_normal_reverse_bright_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_reverse_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_reverse_bright_sub();}
		else{__b2_tcmag_normal_reverse_bright_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_reverse_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_reverse_bright_sub();}
		else{__c2_tcmag_normal_reverse_bright_sub();}
	}
}

//----------------------------------------------------------------------
// 反転明暗スプライト表示（減算）

#define		GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __a0_tcmag_sprite_reverse_bright_sub(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_reverse_bright_sub(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_reverse_bright_sub(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_reverse_bright_sub(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_reverse_bright_sub(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_reverse_bright_sub(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_reverse_bright_sub(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_reverse_bright_sub(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_reverse_bright_sub(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C2_END
	}
}

void tcmag_sprite_reverse_bright_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_reverse_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_reverse_bright_sub();}
		else{__a2_tcmag_sprite_reverse_bright_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_reverse_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_reverse_bright_sub();}
		else{__b2_tcmag_sprite_reverse_bright_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_reverse_bright_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_reverse_bright_sub();}
		else{__c2_tcmag_sprite_reverse_bright_sub();}
	}
}

//----------------------------------------------------------------------
// 反転明暗透明通常表示（減算）

#define		GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SUBCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SUBCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SUBCLRSET_R

void __a0_tcmag_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_reverse_bright_clr_sub(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_C2_END}
}

void tcmag_normal_reverse_bright_clr_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_reverse_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_reverse_bright_clr_sub();}
		else{__a2_tcmag_normal_reverse_bright_clr_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_reverse_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_reverse_bright_clr_sub();}
		else{__b2_tcmag_normal_reverse_bright_clr_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_reverse_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_reverse_bright_clr_sub();}
		else{__c2_tcmag_normal_reverse_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// 反転明暗透明スプライト表示（減算）

void __a0_tcmag_sprite_reverse_bright_clr_sub(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_reverse_bright_clr_sub(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_reverse_bright_clr_sub(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_reverse_bright_clr_sub(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_reverse_bright_clr_sub(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_reverse_bright_clr_sub(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_reverse_bright_clr_sub(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_reverse_bright_clr_sub(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_reverse_bright_clr_sub(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C2_END
	}
}

void tcmag_sprite_reverse_bright_clr_sub(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_reverse_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_reverse_bright_clr_sub();}
		else{__a2_tcmag_sprite_reverse_bright_clr_sub();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_reverse_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_reverse_bright_clr_sub();}
		else{__b2_tcmag_sprite_reverse_bright_clr_sub();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_reverse_bright_clr_sub();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_reverse_bright_clr_sub();}
		else{__c2_tcmag_sprite_reverse_bright_clr_sub();}
	}
}

//----------------------------------------------------------------------
// 反転変色明暗通常表示（減算）

//----------------------------------------------------------------------
// 反転変色明暗スプライト表示（減算）

//----------------------------------------------------------------------
// 反転変色明暗透明通常表示（減算）

//----------------------------------------------------------------------
// 反転変色明暗透明スプライト表示（減算）

//----------------------------------------------------------------------
// 反転モノ通常表示（減算）

//----------------------------------------------------------------------
// 反転モノスプライト表示（減算）

//----------------------------------------------------------------------
// 反転モノ透明通常表示（減算）

//----------------------------------------------------------------------
// 反転モノ透明スプライト表示（減算）

//----------------------------------------------------------------------
// 反転モノ変色通常表示（減算）

//----------------------------------------------------------------------
// 反転モノ変色スプライト表示（減算）

//----------------------------------------------------------------------
// 反転モノ変色透明通常表示（減算）

//----------------------------------------------------------------------
// 反転モノ変色透明スプライト表示（減算）

//----------------------------------------------------------------------
// 反転モノ明暗通常表示（減算）

//----------------------------------------------------------------------
// 反転モノ明暗スプライト表示（減算）

//----------------------------------------------------------------------
// 反転モノ明暗透明通常表示（減算）

//----------------------------------------------------------------------
// 反転モノ明暗透明スプライト表示（減算）

//----------------------------------------------------------------------
// 反転モノ変色明暗通常表示（減算）

//----------------------------------------------------------------------
// 反転モノ変色明暗スプライト表示（減算）

//----------------------------------------------------------------------
// 反転モノ変色明暗透明通常表示（減算）

//----------------------------------------------------------------------
// 反転モノ変色明暗透明スプライト表示（減算）


