#include	"pch.h"
#include	"ng_macro.h"



// 通常表示
void tcgd_normal(void){

		_asm{
			cld
			mov		edi, GRPF_d_buf
			mov		esi, GRPF_s_buf
			mov		ecx, GRPF_cnt_y
			mov		eax, GRPF_cnt_x_org
			mov		edx, GRPF_d_xl
			mov		ebx, GRPF_s_xl
		grplv_loop_y:
			push	ecx
			mov		ecx, eax

			rep		movsd

			pop		ecx
			add		edi, edx
			add		esi, ebx
			loop	grplv_loop_y
		}
}

// スプライト表示
void tcgd_sprite(void){

		_asm{
			cld
			mov		edi, GRPF_d_buf
			mov		esi, GRPF_s_buf
			mov		ecx, GRPF_cnt_y
		grplv_loop_y:
			push	ecx
			mov		ecx, GRPF_cnt_x_org
		grplv_loop_x:

			xor		eax, eax
			mov		al, [esi+3]
			and		al, al
			jz		grplv_step_nopixel
			mov		ebx, eax
			xor		eax, 0xff
			jnz		grplv_step_e
			movsd
			jmp		grplv_step_nopixel2
		grplv_step_e:
			shl		ebx, 2
			add		ebx, Gclr_table_adr_p
			mov		edx, [ebx]

			GRPMAC_GETCLRSET_B
			GRPMAC_GETCLRSET_G
			GRPMAC_GETCLRSET_R

		grplv_step_nopixel:

			add		esi, 4
			add		edi, 4

		grplv_step_nopixel2:		// 255 描画の時

			loop	grplv_loop_x
			add		edi, GRPF_d_xl
			add		esi, GRPF_s_xl
			pop		ecx
			loop	grplv_loop_y
		}
}

// 透明通常表示
void tcgd_normal_clr(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETCLRSET_B
			GRPMAC_GETCLRSET_G
			GRPMAC_GETCLRSET_R

			GRPMAC_LOOP_END
		}
}

// 透明スプライト表示
void tcgd_sprite_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR

			GRPMAC_GETCLRSET_B
			GRPMAC_GETCLRSET_G
			GRPMAC_GETCLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 変色通常表示
void tcgd_normal_change(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 変色スプライト表示
void tcgd_sprite_change(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE

			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 変色透明通常表示
void tcgd_normal_change_clr(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 変色透明スプライト表示
void tcgd_sprite_change_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR

			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 明暗通常表示
void tcgd_normal_bright(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 明暗スプライト表示
void tcgd_sprite_bright(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE

			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 明暗透明通常表示
void tcgd_normal_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 明暗透明スプライト表示
void tcgd_sprite_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR

			GRPMAC_GETSRC_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 変色明暗通常表示
void tcgd_normal_change_bright(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 変色明暗スプライト表示
void tcgd_sprite_change_bright(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE

			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 変色明暗透明通常表示
void tcgd_normal_change_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 変色明暗透明スプライト表示
void tcgd_sprite_change_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR

			GRPMAC_GETSRC_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// モノ通常表示
void tcgd_normal_mono(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// モノスプライト表示
void tcgd_sprite_mono(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// モノ透明通常表示
void tcgd_normal_mono_clr(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// モノ透明スプライト表示
void tcgd_sprite_mono_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// モノ変色通常表示
void tcgd_normal_mono_change(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// モノ変色スプライト表示
void tcgd_sprite_mono_change(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// モノ変色透明通常表示
void tcgd_normal_mono_change_clr(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// モノ変色透明スプライト表示
void tcgd_sprite_mono_change_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// モノ明暗通常表示
void tcgd_normal_mono_bright(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// モノ明暗スプライト表示
void tcgd_sprite_mono_bright(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// モノ明暗透明通常表示
void tcgd_normal_mono_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// モノ明暗透明スプライト表示
void tcgd_sprite_mono_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// モノ変色明暗通常表示
void tcgd_normal_mono_change_bright(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// モノ変色明暗スプライト表示
void tcgd_sprite_mono_change_bright(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// モノ変色明暗透明通常表示
void tcgd_normal_mono_change_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// モノ変色明暗透明スプライト表示
void tcgd_sprite_mono_change_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転通常表示
void tcgd_normal_reverse(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 反転スプライト表示
void tcgd_sprite_reverse(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転透明通常表示
void tcgd_normal_reverse_clr(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 反転透明スプライト表示
void tcgd_sprite_reverse_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転変色通常表示
void tcgd_normal_reverse_change(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 反転変色スプライト表示
void tcgd_sprite_reverse_change(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転変色透明通常表示
void tcgd_normal_reverse_change_clr(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 反転変色透明スプライト表示
void tcgd_sprite_reverse_change_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転明暗通常表示
void tcgd_normal_reverse_bright(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 反転明暗スプライト表示
void tcgd_sprite_reverse_bright(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転明暗透明通常表示
void tcgd_normal_reverse_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 反転明暗透明スプライト表示
void tcgd_sprite_reverse_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転変色明暗通常表示
void tcgd_normal_reverse_change_bright(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 反転変色明暗スプライト表示
void tcgd_sprite_reverse_change_bright(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転変色明暗透明通常表示
void tcgd_normal_reverse_change_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 反転変色明暗透明スプライト表示
void tcgd_sprite_reverse_change_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR

			GRPMAC_GETSRC_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転モノ通常表示
void tcgd_normal_reverse_mono(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 反転モノスプライト表示
void tcgd_sprite_reverse_mono(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転モノ透明通常表示
void tcgd_normal_reverse_mono_clr(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 反転モノ透明スプライト表示
void tcgd_sprite_reverse_mono_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転モノ変色通常表示
void tcgd_normal_reverse_mono_change(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 反転モノ変色スプライト表示
void tcgd_sprite_reverse_mono_change(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転モノ変色透明通常表示
void tcgd_normal_reverse_mono_change_clr(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 反転モノ変色透明スプライト表示
void tcgd_sprite_reverse_mono_change_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転モノ明暗通常表示
void tcgd_normal_reverse_mono_bright(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 反転モノ明暗スプライト表示
void tcgd_sprite_reverse_mono_bright(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転モノ明暗透明通常表示
void tcgd_normal_reverse_mono_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 反転モノ明暗透明スプライト表示
void tcgd_sprite_reverse_mono_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転モノ変色明暗通常表示
void tcgd_normal_reverse_mono_change_bright(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_SET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_SET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_SET_R

			GRPMAC_LOOP_END
		}
}

// 反転モノ変色明暗スプライト表示
void tcgd_sprite_reverse_mono_change_bright(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITE
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}

// 反転モノ変色明暗透明通常表示
void tcgd_normal_reverse_mono_change_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R

			GRPMAC_LOOP_END
		}
}

// 反転モノ変色明暗透明スプライト表示
void tcgd_sprite_reverse_mono_change_bright_clr(void){
		_asm{
			GRPMAC_LOOP_START GRPMAC_SPRITECLR
			GRPMAC_MONO_READY
			GRPMAC_GETSRC_B GRPMAC_MONO_B GRPMAC_REVERSE GRPMAC_BETA_B GRPMAC_PAL_B GRPMAC_CLRSET_B
			GRPMAC_GETSRC_G GRPMAC_MONO_G GRPMAC_REVERSE GRPMAC_BETA_G GRPMAC_PAL_G GRPMAC_CLRSET_G
			GRPMAC_GETSRC_R GRPMAC_MONO_R GRPMAC_REVERSE GRPMAC_BETA_R GRPMAC_PAL_R GRPMAC_CLRSET_R
		grplv_step_nopixel:
			GRPMAC_LOOP_END
		}
}


