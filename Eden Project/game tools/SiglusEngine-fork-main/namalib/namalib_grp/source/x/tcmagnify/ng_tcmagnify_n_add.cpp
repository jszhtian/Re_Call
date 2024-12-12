#include	"pch.h"
#include	"ng_macro.h"
#include	"ng_tcmagnify_macro.h"

//----------------------------------------------------------------------
// 通常表示（加算）

#define		GRPMACMAG_XX_NORMAL	\
			GRPMAC_GETSRC_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_ADDSET_R

void __a0_tcmag_normal_add(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_XX_NORMAL GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_add(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_XX_NORMAL GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_add(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_XX_NORMAL GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_add(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_XX_NORMAL GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_add(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_XX_NORMAL GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_add(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_XX_NORMAL GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_add(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_XX_NORMAL GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_add(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_XX_NORMAL GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_add(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_XX_NORMAL GRPMACMAG_C2_END}
}

void tcmag_normal_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_add();}
		else{__a2_tcmag_normal_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_add();}
		else{__b2_tcmag_normal_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_add();}
		else{__c2_tcmag_normal_add();}
	}
}

//----------------------------------------------------------------------
// スプライト表示（加算）

#define		GRPMACMAG_XX_SPRITE \
			GRPMAC_GETSRC_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_ADDCLRSET_R

void __a0_tcmag_sprite_add(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_add(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_add(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_add(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_add(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_add(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_add(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_add(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_add(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE
	grpmaglv_step_end:     GRPMACMAG_C2_END
	}
}

void tcmag_sprite_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_add();}
		else{__a2_tcmag_sprite_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_add();}
		else{__b2_tcmag_sprite_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_add();}
		else{__c2_tcmag_sprite_add();}
	}
}

//----------------------------------------------------------------------
// 透明通常表示（加算）

#define		GRPMACMAG_XX_NORMAL_CLR	\
			GRPMAC_GETSRC_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_ADDCLRSET_R

void __a0_tcmag_normal_clr_add(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_clr_add(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_clr_add(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_clr_add(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_clr_add(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_clr_add(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_clr_add(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_clr_add(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_clr_add(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_CLR GRPMACMAG_C2_END}
}

void tcmag_normal_clr_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_clr_add();}
		else{__a2_tcmag_normal_clr_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_clr_add();}
		else{__b2_tcmag_normal_clr_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_clr_add();}
		else{__c2_tcmag_normal_clr_add();}
	}
}

//----------------------------------------------------------------------
// 透明スプライト表示（加算）

void __a0_tcmag_sprite_clr_add(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_clr_add(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_clr_add(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_clr_add(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_clr_add(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_clr_add(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_clr_add(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_clr_add(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_clr_add(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE
	grpmaglv_step_end: GRPMACMAG_C2_END
	}
}

void tcmag_sprite_clr_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_clr_add();}
		else{__a2_tcmag_sprite_clr_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_clr_add();}
		else{__b2_tcmag_sprite_clr_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_clr_add();}
		else{__c2_tcmag_sprite_clr_add();}
	}
}

//----------------------------------------------------------------------
// 変色通常表示（加算）

//----------------------------------------------------------------------
// 変色スプライト表示（加算）

//----------------------------------------------------------------------
// 変色透明通常表示（加算）

//----------------------------------------------------------------------
// 変色透明スプライト表示（加算）

//----------------------------------------------------------------------
// 明暗通常表示（加算）

#define		GRPMACMAG_XX_NORMAL_BRIGHT \
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_ADDSET_R

void __a0_tcmag_normal_bright_add(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_bright_add(void){
	_asm{GRPMACMAG_A1_START  GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_bright_add(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_bright_add(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_bright_add(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_bright_add(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_bright_add(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_bright_add(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_bright_add(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_XX_NORMAL_BRIGHT GRPMACMAG_C2_END}
}

void tcmag_normal_bright_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_bright_add();}
		else{__a2_tcmag_normal_bright_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_bright_add();}
		else{__b2_tcmag_normal_bright_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_bright_add();}
		else{__c2_tcmag_normal_bright_add();}
	}
}

//----------------------------------------------------------------------
// 明暗スプライト表示（加算）

#define		GRPMACMAG_XX_SPRITE_BRIGHT \
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __a0_tcmag_sprite_bright_add(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_bright_add(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_bright_add(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_bright_add(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_bright_add(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_bright_add(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_bright_add(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_bright_add(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_bright_add(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C2_END
	}
}

void tcmag_sprite_bright_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_bright_add();}
		else{__a2_tcmag_sprite_bright_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_bright_add();}
		else{__b2_tcmag_sprite_bright_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_bright_add();}
		else{__c2_tcmag_sprite_bright_add();}
	}
}

//----------------------------------------------------------------------
// 明暗透明通常表示（加算）

#define		GRPMACMAG_XX_NORMAL_BRIGHT_CLR \
			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __a0_tcmag_normal_bright_clr_add(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_bright_clr_add(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_bright_clr_add(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_bright_clr_add(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_bright_clr_add(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_bright_clr_add(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_bright_clr_add(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_bright_clr_add(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_bright_clr_add(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_BRIGHT_CLR GRPMACMAG_C2_END}
}

void tcmag_normal_bright_clr_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_bright_clr_add();}
		else{__a2_tcmag_normal_bright_clr_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_bright_clr_add();}
		else{__b2_tcmag_normal_bright_clr_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_bright_clr_add();}
		else{__c2_tcmag_normal_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// 明暗透明スプライト表示（加算）

void __a0_tcmag_sprite_bright_clr_add(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_bright_clr_add(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_bright_clr_add(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_bright_clr_add(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_bright_clr_add(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_bright_clr_add(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_bright_clr_add(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_bright_clr_add(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_bright_clr_add(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C2_END
	}
}

void tcmag_sprite_bright_clr_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_bright_clr_add();}
		else{__a2_tcmag_sprite_bright_clr_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_bright_clr_add();}
		else{__b2_tcmag_sprite_bright_clr_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_bright_clr_add();}
		else{__c2_tcmag_sprite_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// 変色明暗通常表示（加算）

//----------------------------------------------------------------------
// 変色明暗スプライト表示（加算）

//----------------------------------------------------------------------
// 変色明暗透明通常表示（加算）

//----------------------------------------------------------------------
// 変色明暗透明スプライト表示（加算）

//----------------------------------------------------------------------
// モノ通常表示（加算）

//----------------------------------------------------------------------
// モノスプライト表示（加算）

//----------------------------------------------------------------------
// モノ透明通常表示（加算）

//----------------------------------------------------------------------
// モノ透明スプライト表示（加算）

//----------------------------------------------------------------------
// モノ変色通常表示（加算）

//----------------------------------------------------------------------
// モノ変色スプライト表示（加算）

//----------------------------------------------------------------------
// モノ変色透明通常表示（加算）

//----------------------------------------------------------------------
// モノ変色透明スプライト表示（加算）

//----------------------------------------------------------------------
// モノ明暗通常表示（加算）

//----------------------------------------------------------------------
// モノ明暗スプライト表示（加算）

//----------------------------------------------------------------------
// モノ明暗透明通常表示（加算）

//----------------------------------------------------------------------
// モノ明暗透明スプライト表示（加算）

//----------------------------------------------------------------------
// モノ変色明暗通常表示（加算）

//----------------------------------------------------------------------
// モノ変色明暗スプライト表示（加算）

//----------------------------------------------------------------------
// モノ変色明暗透明通常表示（加算）

//----------------------------------------------------------------------
// モノ変色明暗透明スプライト表示（加算）

//----------------------------------------------------------------------
// 反転通常表示（加算）

#define		GRPMACMAG_XX_NORMAL_REVERSE \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_ADDSET_R

void __a0_tcmag_normal_reverse_add(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_reverse_add(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_reverse_add(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_reverse_add(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_reverse_add(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_reverse_add(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_reverse_add(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_reverse_add(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_reverse_add(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_XX_NORMAL_REVERSE GRPMACMAG_C2_END}
}

void tcmag_normal_reverse_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_reverse_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_reverse_add();}
		else{__a2_tcmag_normal_reverse_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_reverse_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_reverse_add();}
		else{__b2_tcmag_normal_reverse_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_reverse_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_reverse_add();}
		else{__c2_tcmag_normal_reverse_add();}
	}
}

//----------------------------------------------------------------------
// 反転スプライト表示（加算）

#define		GRPMACMAG_XX_SPRITE_REVERSE \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_ADDCLRSET_R

void __a0_tcmag_sprite_reverse_add(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_reverse_add(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_reverse_add(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_reverse_add(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_reverse_add(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_reverse_add(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_reverse_add(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_reverse_add(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_reverse_add(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end:     GRPMACMAG_C2_END
	}
}

void tcmag_sprite_reverse_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_reverse_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_reverse_add();}
		else{__a2_tcmag_sprite_reverse_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_reverse_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_reverse_add();}
		else{__b2_tcmag_sprite_reverse_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_reverse_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_reverse_add();}
		else{__c2_tcmag_sprite_reverse_add();}
	}
}

//----------------------------------------------------------------------
// 反転透明通常表示（加算）

#define		GRPMACMAG_XX_NORMAL_REVERSE_CLR \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_ADDCLRSET_R

void __a0_tcmag_normal_reverse_clr_add(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_reverse_clr_add(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_reverse_clr_add(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_reverse_clr_add(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_reverse_clr_add(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_reverse_clr_add(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_reverse_clr_add(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_reverse_clr_add(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_reverse_clr_add(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_CLR GRPMACMAG_C2_END}
}

void tcmag_normal_reverse_clr_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_reverse_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_reverse_clr_add();}
		else{__a2_tcmag_normal_reverse_clr_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_reverse_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_reverse_clr_add();}
		else{__b2_tcmag_normal_reverse_clr_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_reverse_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_reverse_clr_add();}
		else{__c2_tcmag_normal_reverse_clr_add();}
	}
}

//----------------------------------------------------------------------
// 反転透明スプライト表示（加算）

void __a0_tcmag_sprite_reverse_clr_add(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_reverse_clr_add(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_reverse_clr_add(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_reverse_clr_add(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_reverse_clr_add(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_reverse_clr_add(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_reverse_clr_add(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_reverse_clr_add(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_reverse_clr_add(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE
	grpmaglv_step_end: GRPMACMAG_C2_END
	}
}

void tcmag_sprite_reverse_clr_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_reverse_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_reverse_clr_add();}
		else{__a2_tcmag_sprite_reverse_clr_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_reverse_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_reverse_clr_add();}
		else{__b2_tcmag_sprite_reverse_clr_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_reverse_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_reverse_clr_add();}
		else{__c2_tcmag_sprite_reverse_clr_add();}
	}
}

//----------------------------------------------------------------------
// 反転変色通常表示（加算）

//----------------------------------------------------------------------
// 反転変色スプライト表示（加算）

//----------------------------------------------------------------------
// 反転変色透明通常表示（加算）

//----------------------------------------------------------------------
// 反転変色透明スプライト表示（加算）

//----------------------------------------------------------------------
// 反転明暗通常表示（加算）

#define		GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_ADDSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_ADDSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_ADDSET_R

void __a0_tcmag_normal_reverse_bright_add(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_reverse_bright_add(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_reverse_bright_add(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_reverse_bright_add(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_reverse_bright_add(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_reverse_bright_add(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_reverse_bright_add(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_reverse_bright_add(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_reverse_bright_add(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT GRPMACMAG_C2_END}
}

void tcmag_normal_reverse_bright_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_reverse_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_reverse_bright_add();}
		else{__a2_tcmag_normal_reverse_bright_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_reverse_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_reverse_bright_add();}
		else{__b2_tcmag_normal_reverse_bright_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_reverse_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_reverse_bright_add();}
		else{__c2_tcmag_normal_reverse_bright_add();}
	}
}

//----------------------------------------------------------------------
// 反転明暗スプライト表示（加算）

#define		GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __a0_tcmag_sprite_reverse_bright_add(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_reverse_bright_add(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_reverse_bright_add(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_reverse_bright_add(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_reverse_bright_add(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_reverse_bright_add(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_reverse_bright_add(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_reverse_bright_add(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_reverse_bright_add(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITE
	grpmaglv_step_normal:  GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT __asm jmp grpmaglv_step_end
	grpmaglv_step_sprite:  GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end:     GRPMACMAG_C2_END
	}
}

void tcmag_sprite_reverse_bright_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_reverse_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_reverse_bright_add();}
		else{__a2_tcmag_sprite_reverse_bright_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_reverse_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_reverse_bright_add();}
		else{__b2_tcmag_sprite_reverse_bright_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_reverse_bright_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_reverse_bright_add();}
		else{__c2_tcmag_sprite_reverse_bright_add();}
	}
}

//----------------------------------------------------------------------
// 反転明暗透明通常表示（加算）

#define		GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR \
			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_ADDCLRSET_B \
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_ADDCLRSET_G \
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_ADDCLRSET_R

void __a0_tcmag_normal_reverse_bright_clr_add(void){
	_asm{GRPMACMAG_A0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_A0_END}
}
void __a1_tcmag_normal_reverse_bright_clr_add(void){
	_asm{GRPMACMAG_A1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_A1_END}
}
void __a2_tcmag_normal_reverse_bright_clr_add(void){
	_asm{GRPMACMAG_A2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_A2_END}
}
void __b0_tcmag_normal_reverse_bright_clr_add(void){
	_asm{GRPMACMAG_B0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_B0_END}
}
void __b1_tcmag_normal_reverse_bright_clr_add(void){
	_asm{GRPMACMAG_B1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_B1_END}
}
void __b2_tcmag_normal_reverse_bright_clr_add(void){
	_asm{GRPMACMAG_B2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_B2_END}
}
void __c0_tcmag_normal_reverse_bright_clr_add(void){
	_asm{GRPMACMAG_C0_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_C0_END}
}
void __c1_tcmag_normal_reverse_bright_clr_add(void){
	_asm{GRPMACMAG_C1_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_C1_END}
}
void __c2_tcmag_normal_reverse_bright_clr_add(void){
	_asm{GRPMACMAG_C2_START GRPMACMAG_NROMALCLR GRPMACMAG_XX_NORMAL_REVERSE_BRIGHT_CLR GRPMACMAG_C2_END}
}

void tcmag_normal_reverse_bright_clr_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_normal_reverse_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_normal_reverse_bright_clr_add();}
		else{__a2_tcmag_normal_reverse_bright_clr_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_normal_reverse_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_normal_reverse_bright_clr_add();}
		else{__b2_tcmag_normal_reverse_bright_clr_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_normal_reverse_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_normal_reverse_bright_clr_add();}
		else{__c2_tcmag_normal_reverse_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// 反転明暗透明スプライト表示（加算）

void __a0_tcmag_sprite_reverse_bright_clr_add(void){
	_asm{
		GRPMACMAG_A0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A0_END
	}
}
void __a1_tcmag_sprite_reverse_bright_clr_add(void){
	_asm{
		GRPMACMAG_A1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A1_END
	}
}
void __a2_tcmag_sprite_reverse_bright_clr_add(void){
	_asm{
		GRPMACMAG_A2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_A2_END
	}
}
void __b0_tcmag_sprite_reverse_bright_clr_add(void){
	_asm{
		GRPMACMAG_B0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B0_END
	}
}
void __b1_tcmag_sprite_reverse_bright_clr_add(void){
	_asm{
		GRPMACMAG_B1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B1_END
	}
}
void __b2_tcmag_sprite_reverse_bright_clr_add(void){
	_asm{
		GRPMACMAG_B2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_B2_END
	}
}
void __c0_tcmag_sprite_reverse_bright_clr_add(void){
	_asm{
		GRPMACMAG_C0_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C0_END
	}
}
void __c1_tcmag_sprite_reverse_bright_clr_add(void){
	_asm{
		GRPMACMAG_C1_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C1_END
	}
}
void __c2_tcmag_sprite_reverse_bright_clr_add(void){
	_asm{
		GRPMACMAG_C2_START GRPMACMAG_SPRITECLR GRPMACMAG_XX_SPRITE_REVERSE_BRIGHT
	grpmaglv_step_end: GRPMACMAG_C2_END
	}
}

void tcmag_sprite_reverse_bright_clr_add(void){
	if(TCMAG_sxl == TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__a0_tcmag_sprite_reverse_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__a1_tcmag_sprite_reverse_bright_clr_add();}
		else{__a2_tcmag_sprite_reverse_bright_clr_add();}
	}
	else if(TCMAG_sxl < TCMAG_dxl){
		if(TCMAG_syl == TCMAG_dyl){__b0_tcmag_sprite_reverse_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__b1_tcmag_sprite_reverse_bright_clr_add();}
		else{__b2_tcmag_sprite_reverse_bright_clr_add();}
	}
	else{
		if(TCMAG_syl == TCMAG_dyl){__c0_tcmag_sprite_reverse_bright_clr_add();}
		else if(TCMAG_syl < TCMAG_dyl){__c1_tcmag_sprite_reverse_bright_clr_add();}
		else{__c2_tcmag_sprite_reverse_bright_clr_add();}
	}
}

//----------------------------------------------------------------------
// 反転変色明暗通常表示（加算）

//----------------------------------------------------------------------
// 反転変色明暗スプライト表示（加算）

//----------------------------------------------------------------------
// 反転変色明暗透明通常表示（加算）

//----------------------------------------------------------------------
// 反転変色明暗透明スプライト表示（加算）

//----------------------------------------------------------------------
// 反転モノ通常表示（加算）

//----------------------------------------------------------------------
// 反転モノスプライト表示（加算）

//----------------------------------------------------------------------
// 反転モノ透明通常表示（加算）

//----------------------------------------------------------------------
// 反転モノ透明スプライト表示（加算）

//----------------------------------------------------------------------
// 反転モノ変色通常表示（加算）

//----------------------------------------------------------------------
// 反転モノ変色スプライト表示（加算）

//----------------------------------------------------------------------
// 反転モノ変色透明通常表示（加算）

//----------------------------------------------------------------------
// 反転モノ変色透明スプライト表示（加算）

//----------------------------------------------------------------------
// 反転モノ明暗通常表示（加算）

//----------------------------------------------------------------------
// 反転モノ明暗スプライト表示（加算）

//----------------------------------------------------------------------
// 反転モノ明暗透明通常表示（加算）

//----------------------------------------------------------------------
// 反転モノ明暗透明スプライト表示（加算）

//----------------------------------------------------------------------
// 反転モノ変色明暗通常表示（加算）

//----------------------------------------------------------------------
// 反転モノ変色明暗スプライト表示（加算）

//----------------------------------------------------------------------
// 反転モノ変色明暗透明通常表示（加算）

//----------------------------------------------------------------------
// 反転モノ変色明暗透明スプライト表示（加算）


