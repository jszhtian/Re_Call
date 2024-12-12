#include	"pch.h"
#include	"ng_macro.h"
#include	"ng_tcmagnify_macro.h"

//----------------------------------------------------------------------
// �ʏ�\���i���Z�j

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
// �X�v���C�g�\���i���Z�j

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
// �����ʏ�\���i���Z�j

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
// �����X�v���C�g�\���i���Z�j

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
// �ϐF�ʏ�\���i���Z�j

//----------------------------------------------------------------------
// �ϐF�X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// �ϐF�����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// �ϐF�����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���Òʏ�\���i���Z�j

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
// ���ÃX�v���C�g�\���i���Z�j

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
// ���Ó����ʏ�\���i���Z�j

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
// ���Ó����X�v���C�g�\���i���Z�j

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
// �ϐF���Òʏ�\���i���Z�j

//----------------------------------------------------------------------
// �ϐF���ÃX�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// �ϐF���Ó����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// �ϐF���Ó����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���m�ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���m�X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���m�����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���m�����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���m�ϐF�ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���m�ϐF�X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���m�ϐF�����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���m�ϐF�����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���m���Òʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���m���ÃX�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���m���Ó����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���m���Ó����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���m�ϐF���Òʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���m�ϐF���ÃX�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���m�ϐF���Ó����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���m�ϐF���Ó����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]�ʏ�\���i���Z�j

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
// ���]�X�v���C�g�\���i���Z�j

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
// ���]�����ʏ�\���i���Z�j

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
// ���]�����X�v���C�g�\���i���Z�j

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
// ���]�ϐF�ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]�ϐF�X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]�ϐF�����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]�ϐF�����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]���Òʏ�\���i���Z�j

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
// ���]���ÃX�v���C�g�\���i���Z�j

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
// ���]���Ó����ʏ�\���i���Z�j

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
// ���]���Ó����X�v���C�g�\���i���Z�j

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
// ���]�ϐF���Òʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]�ϐF���ÃX�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]�ϐF���Ó����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]�ϐF���Ó����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�ϐF�ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�ϐF�X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�ϐF�����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�ϐF�����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m���Òʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m���ÃX�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m���Ó����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m���Ó����X�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�ϐF���Òʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�ϐF���ÃX�v���C�g�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�ϐF���Ó����ʏ�\���i���Z�j

//----------------------------------------------------------------------
// ���]���m�ϐF���Ó����X�v���C�g�\���i���Z�j


