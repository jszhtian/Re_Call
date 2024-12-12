#include	"pch.h"
#include	"ng_macro.h"



// 通常表示
void tcl_normal(void){

	if(LINEF_yl == 0){
// 点
		if(LINEF_xl == 0){
			*(UI *)LINEF_d_buf = LINEF_pal;
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, LINEF_d_buf
				mov		ecx, LINEF_xl
				mov		eax, LINEF_pal
				mov		edx, LINEF_xp
				inc		ecx
			flineh_loop:
				mov		[edi], eax
				add		edi, edx
				loop	flineh_loop
			}
		}
	}
	else{
// 垂直線
		if(LINEF_xl == 0){
			_asm{
				cld
				mov		edi, LINEF_d_buf
				mov		eax, LINEF_pal
				mov		ecx, LINEF_yl
				inc		ecx
				mov		edx, LINEF_yp
			flineb_loop:
				mov		[edi], eax
				add		edi, edx
				loop	flineb_loop
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, LINEF_d_buf
				mov		eax, LINEF_pal
				mov		ecx, ECNT
				inc		ecx
				mov		ebx, EALPHA
				mov		edx, ENs
				mov		esi, ENa
			flinex_loop:
				mov		[edi], eax
				add		edi, ebx
				add		edx, esi
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh
			flinex_step:
				loop	flinex_loop
			}
		}
	}
}



// 透明表示
void tcl_clr(void){

	if(LINEF_yl == 0){
// 点
		if(LINEF_xl == 0){
			*LINEF_d_buf     = *LINEF_d_buf     + *(CTPCLR + (LINEF_b - *LINEF_d_buf    ));
			*(LINEF_d_buf+1) = *(LINEF_d_buf+1) + *(CTPCLR + (LINEF_g - *(LINEF_d_buf+1)));
			*(LINEF_d_buf+2) = *(LINEF_d_buf+2) + *(CTPCLR + (LINEF_r - *(LINEF_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, LINEF_d_buf
				mov		ecx, LINEF_xl
				mov		esi, CTPCLR
				mov		edx, LINEF_xp
				inc		ecx

			flineh_loop:

				xor		eax, eax
				mov		al, [edi]
				mov		ebx, LINEF_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		eax, eax
				mov		al, [edi+1]
				mov		ebx, LINEF_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		eax, eax
				mov		al, [edi+2]
				mov		ebx, LINEF_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				loop	flineh_loop

			}
		}
	}
	else{
// 垂直線
		if(LINEF_xl == 0){
			_asm{
				cld
				mov		edi, LINEF_d_buf
				mov		ecx, LINEF_yl
				inc		ecx
				mov		esi, CTPCLR

			flineb_loop:

				xor		eax, eax
				mov		al, [edi]
				mov		ebx, LINEF_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		eax, eax
				mov		al, [edi+1]
				mov		ebx, LINEF_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		eax, eax
				mov		al, [edi+2]
				mov		ebx, LINEF_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, LINEF_yp

				loop	flineb_loop
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, LINEF_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
				mov		esi, CTPCLR
			flinex_loop:

				xor		eax, eax
				mov		al, [edi]
				mov		ebx, LINEF_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		eax, eax
				mov		al, [edi+1]
				mov		ebx, LINEF_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		eax, eax
				mov		al, [edi+2]
				mov		ebx, LINEF_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh
			flinex_step:
				loop	flinex_loop
			}
		}
	}
}



// タイル通常表示
void tcl_normal_tile(void){
	int tmp;
	int *CLR;

	if(LINEF_yl == 0){
// 点
		if(LINEF_xl == 0){
			if(LINEF_t_reverse){
				tmp = (UC)(~(*(LINEF_t_adr + LINEF_t_x)));
			}
			else{
				tmp = *(LINEF_t_adr + LINEF_t_x);
			}
			tmp = (tmp << LINEF_t_shift) + LINEF_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp == 255){
					*(UI *)LINEF_d_buf = LINEF_pal;
				}
				else if(tmp){
					CLR = *(Gclr_table_adr_p + tmp);
					*LINEF_d_buf     = *LINEF_d_buf     + *(CLR + (LINEF_b - *LINEF_d_buf    ));
					*(LINEF_d_buf+1) = *(LINEF_d_buf+1) + *(CLR + (LINEF_g - *(LINEF_d_buf+1)));
					*(LINEF_d_buf+2) = *(LINEF_d_buf+2) + *(CLR + (LINEF_r - *(LINEF_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, LINEF_d_buf
				mov		ecx, LINEF_xl
				mov		edx, LINEF_xp
				inc		ecx

			flineh_loop:

				mov		ebx, LINEF_t_adr
				add		ebx, LINEF_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		LINEF_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, LINEF_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, LINEF_t_add
				cmp		eax, 0
				jge		flineh_tile_great
				mov		eax, 0
				jmp		flineh_tile
			flineh_tile_great:
				cmp		eax, 255
				jle		flineh_tile
				mov		eax, 255
			flineh_tile:
				mov		ebx, eax
				and		bl, bl
				jnz		flineh_tile_step
				jmp		flineh_step
			flineh_tile_step:

				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		ebx, [ebx]
				add		ebx, 1020
				mov		ebx, [ebx]
				and		bl, bl
				jnz		flineh_255_tile_step
				jmp		flineh_step
			flineh_255_tile_step:
				cmp		ebx, 255
				jne		flineh_xxx_tile_step
				mov		eax, LINEF_pal
				mov		[edi], eax
				jmp		flineh_step
			flineh_xxx_tile_step:
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		eax, eax
				mov		al, [edi]
				mov		ebx, LINEF_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		eax, eax
				mov		al, [edi+1]
				mov		ebx, LINEF_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		eax, eax
				mov		al, [edi+2]
				mov		ebx, LINEF_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, LINEF_t_x
				add		eax, LINEF_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, LINEF_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, LINEF_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		LINEF_t_x, eax
				add		edi, edx
				dec		ecx
				jz		flineh_loop_end
				jmp		flineh_loop
			flineh_loop_end:
			}
		}
	}
	else{
// 垂直線
		if(LINEF_xl == 0){
			_asm{
				cld
				mov		edi, LINEF_d_buf
				mov		ecx, LINEF_yl
				mov		edx, LINEF_yp
				inc		ecx

			flineb_loop:

				mov		ebx, LINEF_t_adr
				add		ebx, LINEF_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		LINEF_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, LINEF_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, LINEF_t_add
				cmp		eax, 0
				jge		flineb_tile_great
				mov		eax, 0
				jmp		flineb_tile
			flineb_tile_great:
				cmp		eax, 255
				jle		flineb_tile
				mov		eax, 255
			flineb_tile:
				mov		ebx, eax
				and		bl, bl
				jnz		flineb_tile_step
				jmp		flineb_step
			flineb_tile_step:

				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		ebx, [ebx]
				add		ebx, 1020
				mov		ebx, [ebx]
				and		bl, bl
				jnz		flineb_255_tile_step
				jmp		flineb_step
			flineb_255_tile_step:
				cmp		ebx, 255
				jne		flineb_xxx_tile_step
				mov		eax, LINEF_pal
				mov		[edi], eax
				jmp		flineb_step
			flineb_xxx_tile_step:
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		eax, eax
				mov		al, [edi]
				mov		ebx, LINEF_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		eax, eax
				mov		al, [edi+1]
				mov		ebx, LINEF_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		eax, eax
				mov		al, [edi+2]
				mov		ebx, LINEF_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, LINEF_t_adr
				add		eax, LINEF_t_y_add
				cmp		eax, LINEF_t_adr_org
				jge		flineb_jge
				mov		eax, LINEF_t_adr_max
				sub		eax, LINEF_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, LINEF_t_adr_max
				jl		flineb_end
				mov		eax, LINEF_t_adr_org
			flineb_end:

				mov		LINEF_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(LINEF_xl > LINEF_yl){
				_asm{
					cld
					mov		edi, LINEF_d_buf
					mov		ecx, LINEF_xl
					inc		ecx
					mov		edx, ENs
					mov		esi, CTPCLR

				flinex_loop:

					mov		ebx, LINEF_t_adr
					add		ebx, LINEF_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		LINEF_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, LINEF_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, LINEF_t_add
					cmp		eax, 0
					jge		flinex_tile_great
					mov		eax, 0
					jmp		flinex_tile
				flinex_tile_great:
					cmp		eax, 255
					jle		flinex_tile
					mov		eax, 255
				flinex_tile:
					mov		ebx, eax
					and		bl, bl
					jnz		flinex_tile_step
					jmp		flinex_step
				flinex_tile_step:

					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		ebx, [ebx]
					add		ebx, 1020
					mov		ebx, [ebx]
					and		bl, bl
					jnz		flinex_255_tile_step
					jmp		flinex_step
				flinex_255_tile_step:
					cmp		ebx, 255
					jne		flinex_xxx_tile_step
					mov		eax, LINEF_pal
					mov		[edi], eax
					jmp		flinex_step
				flinex_xxx_tile_step:
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		eax, eax
					mov		al, [edi]
					mov		ebx, LINEF_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		eax, eax
					mov		al, [edi+1]
					mov		ebx, LINEF_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		eax, eax
					mov		al, [edi+2]
					mov		ebx, LINEF_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, LINEF_t_x
					add		eax, LINEF_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, LINEF_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, LINEF_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		LINEF_t_x, eax

					add		edi, LINEF_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, LINEF_t_adr
					add		eax, LINEF_t_y_add
					cmp		eax, LINEF_t_adr_org
					jge		flinex_jge2
					mov		eax, LINEF_t_adr_max
					sub		eax, LINEF_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, LINEF_t_adr_max
					jl		flinex_end2
					mov		eax, LINEF_t_adr_org
				flinex_end2:
					mov		LINEF_t_adr, eax

					add		edi, LINEF_yp
					sub		edx, ENh

				flinex_step2:
					dec		ecx
					jz		flinex_loop_end
					jmp		flinex_loop
				flinex_loop_end:
				}
			}
			else{
				_asm{
					cld
					mov		edi, LINEF_d_buf
					mov		ecx, LINEF_yl
					inc		ecx
					mov		edx, ENs
					mov		esi, CTPCLR

				fliney_loop:

					mov		ebx, LINEF_t_adr
					add		ebx, LINEF_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		LINEF_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, LINEF_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, LINEF_t_add
					cmp		eax, 0
					jge		fliney_tile_great
					mov		eax, 0
					jmp		fliney_tile
				fliney_tile_great:
					cmp		eax, 255
					jle		fliney_tile
					mov		eax, 255
				fliney_tile:
					mov		ebx, eax
					and		bl, bl
					jnz		fliney_tile_step
					jmp		fliney_step
				fliney_tile_step:

					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		ebx, [ebx]
					add		ebx, 1020
					mov		ebx, [ebx]
					and		bl, bl
					jnz		fliney_255_tile_step
					jmp		fliney_step
				fliney_255_tile_step:
					cmp		ebx, 255
					jne		fliney_xxx_tile_step
					mov		eax, LINEF_pal
					mov		[edi], eax
					jmp		fliney_step
				fliney_xxx_tile_step:
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		eax, eax
					mov		al, [edi]
					mov		ebx, LINEF_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		eax, eax
					mov		al, [edi+1]
					mov		ebx, LINEF_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		eax, eax
					mov		al, [edi+2]
					mov		ebx, LINEF_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, LINEF_t_adr
					add		eax, LINEF_t_y_add
					cmp		eax, LINEF_t_adr_org
					jge		fliney_jge2
					mov		eax, LINEF_t_adr_max
					sub		eax, LINEF_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, LINEF_t_adr_max
					jl		fliney_end2
					mov		eax, LINEF_t_adr_org
				fliney_end2:
					mov		LINEF_t_adr, eax

					add		edi, LINEF_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, LINEF_t_x
					add		eax, LINEF_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, LINEF_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, LINEF_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		LINEF_t_x, eax

					add		edi, LINEF_xp
					sub		edx, ENh

				fliney_step2:
					dec		ecx
					jz		fliney_loop_end
					jmp		fliney_loop
				fliney_loop_end:
				}
			}
		}
	}
}



// タイル透明表示
void tcl_clr_tile(void){
	int tmp;
	int *CLR;

	if(LINEF_yl == 0){
// 点
		if(LINEF_xl == 0){
			if(LINEF_t_reverse){
				tmp = (UC)(~(*(LINEF_t_adr + LINEF_t_x)));
			}
			else{
				tmp = *(LINEF_t_adr + LINEF_t_x);
			}
			tmp = (tmp << LINEF_t_shift) + LINEF_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + LINEF_clr);
				if(tmp == 255){
					*(UI *)LINEF_d_buf = LINEF_pal;
				}
				else if(tmp){
					CLR = *(Gclr_table_adr_p + tmp);
					*LINEF_d_buf     = *LINEF_d_buf     + *(CLR + (LINEF_b - *LINEF_d_buf    ));
					*(LINEF_d_buf+1) = *(LINEF_d_buf+1) + *(CLR + (LINEF_g - *(LINEF_d_buf+1)));
					*(LINEF_d_buf+2) = *(LINEF_d_buf+2) + *(CLR + (LINEF_r - *(LINEF_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, LINEF_d_buf
				mov		ecx, LINEF_xl
				mov		edx, LINEF_xp
				inc		ecx

			flineh_loop:

				mov		ebx, LINEF_t_adr
				add		ebx, LINEF_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		LINEF_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, LINEF_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, LINEF_t_add
				cmp		eax, 0
				jge		flineh_tile_great
				mov		eax, 0
				jmp		flineh_tile
			flineh_tile_great:
				cmp		eax, 255
				jle		flineh_tile
				mov		eax, 255
			flineh_tile:
				mov		ebx, eax
				and		bl, bl
				jnz		flineh_tile_step
				jmp		flineh_step
			flineh_tile_step:

				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		ebx, [ebx]
				mov		eax, 255
				sub		eax, LINEF_clrR
				shl		eax, 2
				add		ebx, eax
				mov		ebx, [ebx]
				and		bl, bl
				jnz		flineh_255_tile_step
				jmp		flineh_step
			flineh_255_tile_step:
				cmp		ebx, 255
				jne		flineh_xxx_tile_step
				mov		eax, LINEF_pal
				mov		[edi], eax
				jmp		flineh_step
			flineh_xxx_tile_step:
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		eax, eax
				mov		al, [edi]
				mov		ebx, LINEF_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		eax, eax
				mov		al, [edi+1]
				mov		ebx, LINEF_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		eax, eax
				mov		al, [edi+2]
				mov		ebx, LINEF_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, LINEF_t_x
				add		eax, LINEF_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, LINEF_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, LINEF_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		LINEF_t_x, eax
				add		edi, edx
				dec		ecx
				jz		flineh_loop_end
				jmp		flineh_loop
			flineh_loop_end:
			}
		}
	}
	else{
// 垂直線
		if(LINEF_xl == 0){
			_asm{
				cld
				mov		edi, LINEF_d_buf
				mov		ecx, LINEF_yl
				mov		edx, LINEF_yp
				inc		ecx

			flineb_loop:

				mov		ebx, LINEF_t_adr
				add		ebx, LINEF_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		LINEF_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, LINEF_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, LINEF_t_add
				cmp		eax, 0
				jge		flineb_tile_great
				mov		eax, 0
				jmp		flineb_tile
			flineb_tile_great:
				cmp		eax, 255
				jle		flineb_tile
				mov		eax, 255
			flineb_tile:
				mov		ebx, eax
				and		bl, bl
				jnz		flineb_tile_step
				jmp		flineb_step
			flineb_tile_step:

				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		ebx, [ebx]
				mov		eax, 255
				sub		eax, LINEF_clrR
				shl		eax, 2
				add		ebx, eax
				mov		ebx, [ebx]
				and		bl, bl
				jnz		flineb_255_tile_step
				jmp		flineb_step
			flineb_255_tile_step:
				cmp		ebx, 255
				jne		flineb_xxx_tile_step
				mov		eax, LINEF_pal
				mov		[edi], eax
				jmp		flineb_step
			flineb_xxx_tile_step:
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		eax, eax
				mov		al, [edi]
				mov		ebx, LINEF_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		eax, eax
				mov		al, [edi+1]
				mov		ebx, LINEF_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		eax, eax
				mov		al, [edi+2]
				mov		ebx, LINEF_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, LINEF_t_adr
				add		eax, LINEF_t_y_add
				cmp		eax, LINEF_t_adr_org
				jge		flineb_jge
				mov		eax, LINEF_t_adr_max
				sub		eax, LINEF_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, LINEF_t_adr_max
				jl		flineb_end
				mov		eax, LINEF_t_adr_org
			flineb_end:

				mov		LINEF_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(LINEF_xl > LINEF_yl){
				_asm{
					cld
					mov		edi, LINEF_d_buf
					mov		ecx, LINEF_xl
					inc		ecx
					mov		edx, ENs
					mov		esi, CTPCLR

				flinex_loop:

					mov		ebx, LINEF_t_adr
					add		ebx, LINEF_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		LINEF_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, LINEF_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, LINEF_t_add
					cmp		eax, 0
					jge		flinex_tile_great
					mov		eax, 0
					jmp		flinex_tile
				flinex_tile_great:
					cmp		eax, 255
					jle		flinex_tile
					mov		eax, 255
				flinex_tile:
					mov		ebx, eax
					and		bl, bl
					jnz		flinex_tile_step
					jmp		flinex_step
				flinex_tile_step:

					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		ebx, [ebx]
					mov		eax, 255
					sub		eax, LINEF_clrR
					shl		eax, 2
					add		ebx, eax
					mov		ebx, [ebx]
					and		bl, bl
					jnz		flinex_255_tile_step
					jmp		flinex_step
				flinex_255_tile_step:
					cmp		ebx, 255
					jne		flinex_xxx_tile_step
					mov		eax, LINEF_pal
					mov		[edi], eax
					jmp		flinex_step
				flinex_xxx_tile_step:
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		eax, eax
					mov		al, [edi]
					mov		ebx, LINEF_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		eax, eax
					mov		al, [edi+1]
					mov		ebx, LINEF_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		eax, eax
					mov		al, [edi+2]
					mov		ebx, LINEF_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, LINEF_t_x
					add		eax, LINEF_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, LINEF_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, LINEF_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		LINEF_t_x, eax

					add		edi, LINEF_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, LINEF_t_adr
					add		eax, LINEF_t_y_add
					cmp		eax, LINEF_t_adr_org
					jge		flinex_jge2
					mov		eax, LINEF_t_adr_max
					sub		eax, LINEF_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, LINEF_t_adr_max
					jl		flinex_end2
					mov		eax, LINEF_t_adr_org
				flinex_end2:
					mov		LINEF_t_adr, eax

					add		edi, LINEF_yp
					sub		edx, ENh

				flinex_step2:
					dec		ecx
					jz		flinex_loop_end
					jmp		flinex_loop
				flinex_loop_end:
				}
			}
			else{
				_asm{
					cld
					mov		edi, LINEF_d_buf
					mov		ecx, LINEF_yl
					inc		ecx
					mov		edx, ENs
					mov		esi, CTPCLR

				fliney_loop:

					mov		ebx, LINEF_t_adr
					add		ebx, LINEF_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		LINEF_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, LINEF_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, LINEF_t_add
					cmp		eax, 0
					jge		fliney_tile_great
					mov		eax, 0
					jmp		fliney_tile
				fliney_tile_great:
					cmp		eax, 255
					jle		fliney_tile
					mov		eax, 255
				fliney_tile:
					mov		ebx, eax
					and		bl, bl
					jnz		fliney_tile_step
					jmp		fliney_step
				fliney_tile_step:

					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		ebx, [ebx]
					mov		eax, 255
					sub		eax, LINEF_clrR
					shl		eax, 2
					add		ebx, eax
					mov		ebx, [ebx]
					and		bl, bl
					jnz		fliney_255_tile_step
					jmp		fliney_step
				fliney_255_tile_step:
					cmp		ebx, 255
					jne		fliney_xxx_tile_step
					mov		eax, LINEF_pal
					mov		[edi], eax
					jmp		fliney_step
				fliney_xxx_tile_step:
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		eax, eax
					mov		al, [edi]
					mov		ebx, LINEF_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		eax, eax
					mov		al, [edi+1]
					mov		ebx, LINEF_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		eax, eax
					mov		al, [edi+2]
					mov		ebx, LINEF_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, LINEF_t_adr
					add		eax, LINEF_t_y_add
					cmp		eax, LINEF_t_adr_org
					jge		fliney_jge2
					mov		eax, LINEF_t_adr_max
					sub		eax, LINEF_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, LINEF_t_adr_max
					jl		fliney_end2
					mov		eax, LINEF_t_adr_org
				fliney_end2:
					mov		LINEF_t_adr, eax

					add		edi, LINEF_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, LINEF_t_x
					add		eax, LINEF_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, LINEF_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, LINEF_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		LINEF_t_x, eax

					add		edi, LINEF_xp
					sub		edx, ENh

				fliney_step2:
					dec		ecx
					jz		fliney_loop_end
					jmp		fliney_loop
				fliney_loop_end:
				}
			}
		}
	}
}



/*========================================================================================
	ライン表示
void true_color_line(
	UC *d_buf, int d_xl, int d_yl,
	int x1, int y1, int x2, int y2,
	UC r, UC g, UC b,
	int ex1, int ey1, int ex2, int ey2,
	int screen_mod,
	UC *tile, UC t_shift, int t_add, int t_xl, int t_yl, int t_repx, int t_repy, int t_reverse,
	UC mono,
	UC reverse,
	UC c_r, UC c_g, UC c_b, UC c_rate,
	int p_r, int p_g, int p_b,
	int bright,
	UC clr,
	int disp_mod
);
		IN		int (x1 - y2) : 終始点
				true_color_grp_disp参照
		OUT		なし
----------------------------------------------------------------------------------------*/

void true_color_line(
	UC *d_buf, int d_xl, int d_yl,
	int x1, int y1, int x2, int y2,
	UC r, UC g, UC b,
	int ex1, int ey1, int ex2, int ey2,
	int screen_mod,
	UC *tile, UC t_shift, int t_add, int t_xl, int t_yl, int t_repx, int t_repy, int t_reverse,
	UC mono,
	UC reverse,
	UC c_r, UC c_g, UC c_b, UC c_rate,
	int p_r, int p_g, int p_b,
	int bright,
	UC clr,
	int disp_mod
){
	int xl, yl;
	int xp, yp;			// 座標の増分

	if(t_shift < 0){
		t_shift = 0;
	}
	else if(t_shift > 8){
		t_shift = 8;
	}
	if(t_add < -(255<<t_shift)){
		t_add = -(255<<t_shift);
	}
	else if(t_add > 255){
		t_add = 255;
	}

	if(d_buf==0 || clr==0 || (tile && t_add==-(255<<t_shift))){
		return;
	}

// エリア設定
	ng_set_eria(d_xl, d_yl, &ex1, &ey1, &ex2, &ey2);

// クリップ
	if(fclip(&x1, &y1, &x2, &y2, ex1, ey1, ex2, ey2) == 0){return;}

// 前処理
	if((xl=x2-x1)<0){xl=-xl;xp=-1;}
	else{xp=1;}
	if((yl=y2-y1)<0){yl=-yl;yp=-d_xl;}
	else{yp=d_xl;}

// アドレス算出
	if(screen_mod == 0){
		yp = -yp;
		d_buf += ((x1 + (d_xl * y1)) << 2);
	}
	else{
		d_buf += ((x1 + (d_xl * (d_yl-y1-1))) << 2);
	}

// 描画パラメータ
	LINEF_d_buf = d_buf;
	LINEF_d_xl  = d_xl;
	LINEF_d_yl  = d_yl;

	LINEF_d_xl = d_xl << 2;

	LINEF_r = r;
	LINEF_g = g;
	LINEF_b = b;

	LINEF_xl = xl;
	LINEF_yl = yl;

	LINEF_xp = xp << 2;
	LINEF_yp = yp << 2;

	if(LINEF_xl > LINEF_yl){
		ENs = -LINEF_xl;
		ENa = LINEF_yl<<1;
		ENh = LINEF_xl<<1;
		ECNT = LINEF_xl;
		LINEF_yp = -LINEF_yp;
		EALPHA = LINEF_xp;
		EBETA  = LINEF_yp;
	}
	else{
		ENs = -LINEF_yl;
		ENa = LINEF_xl<<1;
		ENh = LINEF_yl<<1;
		ECNT = LINEF_yl;
		LINEF_yp = -LINEF_yp;
		EALPHA = LINEF_yp;
		EBETA  = LINEF_xp;
	}

// 反転パラメータ
	CTPREV = *(Gclr_table_adr_p + (255 - reverse));

// モノパラメータ
	CTPMONO = *(Gclr_table_adr_p + (255 - mono));

// 変色パラメータ
	if(c_rate){
		LINEF_c_r = c_r;
		LINEF_c_g = c_g;
		LINEF_c_b = c_b;
		CTPBETA = *(Gclr_table_adr_p + (255 - c_rate));
	}

// 透明度パラメータ
	LINEF_clr  = clr;
	LINEF_clrR = 255 - clr;
	CTPCLR = *(Gclr_table_adr_p + clr);

// 色味パラメータ
	if(bright < -255){
		bright = -255;
	}
	else if(bright > 255){
		bright = 255;
	}
	if(p_r < -255){
		p_r = -255;
	}
	else if(p_r > 255){
		p_r = 255;
	}
	if(p_g < -255){
		p_g = -255;
	}
	else if(p_g > 255){
		p_g = 255;
	}
	if(p_b < -255){
		p_b = -255;
	}
	else if(p_b > 255){
		p_b = 255;
	}
	if(bright < 0){
		p_r += (((255 + p_r) * bright) / 255);
		p_g += (((255 + p_g) * bright) / 255);
		p_b += (((255 + p_b) * bright) / 255);
	}
	else if(bright > 0){
		p_r += (((255 - p_r) * bright) / 255);
		p_g += (((255 - p_g) * bright) / 255);
		p_b += (((255 - p_b) * bright) / 255);
	}

	if(disp_mod == 2){		// 減算モード
		p_r = -(p_r);
		p_g = -(p_g);
		p_b = -(p_b);
	}

	LINEF_p_r = p_r;
	LINEF_p_g = p_g;
	LINEF_p_b = p_b;
	if(LINEF_p_r >= 0){
		CTPR = *(Gclr_table_adr_p + (255 - LINEF_p_r));
		LINEF_p_r = 255;
	}
	else{
		CTPR = *(Gclr_table_adr_p + (255 + LINEF_p_r));
		LINEF_p_r = 0;
	}
	if(LINEF_p_g >= 0){
		CTPG = *(Gclr_table_adr_p + (255 - LINEF_p_g));
		LINEF_p_g = 255;
	}
	else{
		CTPG = *(Gclr_table_adr_p + (255 + LINEF_p_g));
		LINEF_p_g = 0;
	}
	if(LINEF_p_b >= 0){
		CTPB = *(Gclr_table_adr_p + (255 - LINEF_p_b));
		LINEF_p_b = 255;
	}
	else{
		CTPB = *(Gclr_table_adr_p + (255 + LINEF_p_b));
		LINEF_p_b = 0;
	}

// タイルパラメータ
	if(tile){
		LINEF_t_add = t_add;
		LINEF_t_reverse = t_reverse;
		LINEF_t_shift = t_shift;
		LINEF_t_xl = t_xl;
		LINEF_t_yl = t_yl;

		if(x1 >= t_repx){
			LINEF_t_x = LINEF_t_x_org = (x1 - t_repx) % t_xl;
		}
		else{
			LINEF_t_x = LINEF_t_x_org = (t_xl - ((t_repx - x1) % t_xl)) % t_xl;
		}
		if(y1 >= t_repy){
			LINEF_t_y                 = (y1 - t_repy) % t_yl;
		}
		else{
			LINEF_t_y                 = (t_yl - ((t_repy - y1) % t_yl)) % t_yl;
		}

		LINEF_t_adr     = tile + (LINEF_t_y * t_xl);
		LINEF_t_adr_org = tile;
		LINEF_t_adr_max = tile + (t_xl * t_yl);
		if(xp < 0){
			LINEF_t_x_add = -1;
		}
		else{
			LINEF_t_x_add = 1;
		}
		if(yp < 0){
			LINEF_t_y_add = -t_xl;
		}
		else{
			LINEF_t_y_add = t_xl;
		}
	}

// ＲＧＢ
	if(mono){
		LINEF_pal = (*(Gmono_table_r_p + LINEF_r) + *(Gmono_table_g_p + LINEF_g) + *(Gmono_table_b_p + LINEF_b)) >> 8;
		LINEF_r = LINEF_pal + *(CTPMONO + (LINEF_r - LINEF_pal));
		LINEF_g = LINEF_pal + *(CTPMONO + (LINEF_g - LINEF_pal));
		LINEF_b = LINEF_pal + *(CTPMONO + (LINEF_b - LINEF_pal));
	}
	if(reverse){
		LINEF_r = (255-LINEF_r) + *(CTPREV + (LINEF_r - (255-LINEF_r)));
		LINEF_g = (255-LINEF_g) + *(CTPREV + (LINEF_g - (255-LINEF_g)));
		LINEF_b = (255-LINEF_b) + *(CTPREV + (LINEF_b - (255-LINEF_b)));
	}
	if(c_rate){
		LINEF_r = LINEF_c_r + *(CTPBETA + (LINEF_r - LINEF_c_r));
		LINEF_g = LINEF_c_g + *(CTPBETA + (LINEF_g - LINEF_c_g));
		LINEF_b = LINEF_c_b + *(CTPBETA + (LINEF_b - LINEF_c_b));
	}
	if(p_r || p_g || p_b){
		LINEF_r = LINEF_p_r + *(CTPR + (LINEF_r - LINEF_p_r));
		LINEF_g = LINEF_p_g + *(CTPG + (LINEF_g - LINEF_p_g));
		LINEF_b = LINEF_p_b + *(CTPB + (LINEF_b - LINEF_p_b));
	}

//*****************************************************************************************

	if(clr == 255){
LINEF_pal = (255 << 24) + (LINEF_r << 16) + (LINEF_g << 8) + LINEF_b;
		if(tile==0 || t_add==255){
// 通常表示
tcl_normal();
		}
		else{
// タイル通常表示
tcl_normal_tile();
		}
	}
	else{
		if(tile==0 || t_add==255){
// 透明表示
tcl_clr();
		}
		else{
// タイル透明表示
tcl_clr_tile();
		}
	}
}



