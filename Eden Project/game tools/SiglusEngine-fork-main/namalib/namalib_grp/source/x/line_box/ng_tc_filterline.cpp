#include	"pch.h"
#include	"ng_macro.h"



// 反転通常表示
void tcfl_normal_reverse(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			tmp1 = *FILTER_d_buf;
			tmp2 = 255 - tmp1;
			*FILTER_d_buf     = tmp2 + *(CTPREV + (tmp1 - tmp2));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = 255 - tmp1;
			*(FILTER_d_buf+1) = tmp2 + *(CTPREV + (tmp1 - tmp2));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = 255 - tmp1;
			*(FILTER_d_buf+2) = tmp2 + *(CTPREV + (tmp1 - tmp2));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		esi, CTPREV
				mov		edx, FILTER_xp

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		esi, CTPREV
				mov		edx, FILTER_yp

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
				mov		esi, CTPREV
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転通常表示
void tcfl_normal_reverse_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2, tmp3;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = 255 - tmp1;
					tmp3 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp3 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = 255 - tmp1;
					tmp3 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp3 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = 255 - tmp1;
					tmp3 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp3 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// 反転透明通常表示
void tcfl_normal_reverse_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2, tmp3;

			tmp1 = *FILTER_d_buf;
			tmp2 = 255 - tmp1;
			tmp3 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp3 - *FILTER_d_buf    ));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = 255 - tmp1;
			tmp3 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp3 - *(FILTER_d_buf+1)));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = 255 - tmp1;
			tmp3 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp3 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp
				mov		esi, CTPCLR

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp
				mov		esi, CTPCLR

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
				mov		esi, CTPCLR
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転透明通常表示
void tcfl_normal_reverse_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2, tmp3;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = 255 - tmp1;
					tmp3 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp3 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = 255 - tmp1;
					tmp3 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp3 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = 255 - tmp1;
					tmp3 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp3 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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




// モノ通常表示
void tcfl_normal_mono(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;
			*FILTER_d_buf     = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			*(FILTER_d_buf+1) = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			*(FILTER_d_buf+2) = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				mov		esi, CTPMONO

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				mov		esi, CTPMONO

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				mov		esi, CTPMONO

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイルモノ通常表示
void tcfl_normal_mono_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = *FILTER_d_buf;
					tmp2 = FILTER_pal + *(CTPMONO + (tmp1 - FILTER_pal));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = FILTER_pal + *(CTPMONO + (tmp1 - FILTER_pal));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = FILTER_pal + *(CTPMONO + (tmp1 - FILTER_pal));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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




// モノ透明通常表示
void tcfl_normal_mono_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = *FILTER_d_buf;
			tmp2 = FILTER_pal + *(CTPMONO + (tmp1 - FILTER_pal));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp2 - *FILTER_d_buf    ));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = FILTER_pal + *(CTPMONO + (tmp1 - FILTER_pal));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+1)));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = FILTER_pal + *(CTPMONO + (tmp1 - FILTER_pal));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイルモノ透明通常表示
void tcfl_normal_mono_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = *FILTER_d_buf;
					tmp2 = FILTER_pal + *(CTPMONO + (tmp1 - FILTER_pal));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = FILTER_pal + *(CTPMONO + (tmp1 - FILTER_pal));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = FILTER_pal + *(CTPMONO + (tmp1 - FILTER_pal));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// 明暗通常表示
void tcfl_normal_bright(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1;

			tmp1 = *FILTER_d_buf;
			*FILTER_d_buf     = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));

			tmp1 = *(FILTER_d_buf+1);
			*(FILTER_d_buf+1) = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));

			tmp1 = *(FILTER_d_buf+2);
			*(FILTER_d_buf+2) = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh

			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル明暗通常表示
void tcfl_normal_bright_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// 明暗透明通常表示
void tcfl_normal_bright_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			tmp1 = *FILTER_d_buf;
			tmp2 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp2 - *FILTER_d_buf    ));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+1)));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp
				mov		esi, CTPCLR

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp
				mov		esi, CTPCLR

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
				mov		esi, CTPCLR
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル明暗透明通常表示
void tcfl_normal_bright_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// 変色通常表示
void tcfl_normal_change(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1;

			tmp1 = *FILTER_d_buf;
			*FILTER_d_buf     = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));

			tmp1 = *(FILTER_d_buf+1);
			*(FILTER_d_buf+1) = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));

			tmp1 = *(FILTER_d_buf+2);
			*(FILTER_d_buf+2) = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh

			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル変色通常表示
void tcfl_normal_change_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// 変色透明通常表示
void tcfl_normal_change_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			tmp1 = *FILTER_d_buf;
			tmp2 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp2 - *FILTER_d_buf    ));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+1)));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp
				mov		esi, CTPCLR

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp
				mov		esi, CTPCLR

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
				mov		esi, CTPCLR
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル変色透明通常表示
void tcfl_normal_change_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// 反転モノ通常表示
void tcfl_normal_reverse_mono(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp2 = 255 - tmp1;
			*FILTER_d_buf     = tmp2 + *(CTPREV + (tmp1 - tmp2));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp2 = 255 - tmp1;
			*(FILTER_d_buf+1) = tmp2 + *(CTPREV + (tmp1 - tmp2));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp2 = 255 - tmp1;
			*(FILTER_d_buf+2) = tmp2 + *(CTPREV + (tmp1 - tmp2));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh
			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転モノ通常表示
void tcfl_normal_reverse_mono_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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




// 反転モノ透明通常表示
void tcfl_normal_reverse_mono_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp2 - *FILTER_d_buf    ));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+1)));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転モノ透明通常表示
void tcfl_normal_reverse_mono_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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






// 反転明暗通常表示
void tcfl_normal_reverse_bright(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			tmp1 = *FILTER_d_buf;
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*FILTER_d_buf     = FILTER_p_b + *(CTPB + (tmp2 - FILTER_p_b));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+1) = FILTER_p_g + *(CTPG + (tmp2 - FILTER_p_g));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+2) = FILTER_p_r + *(CTPR + (tmp2 - FILTER_p_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh

			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転明暗通常表示
void tcfl_normal_reverse_bright_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_p_b + *(CTPB + (tmp2 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_p_g + *(CTPG + (tmp2 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_p_r + *(CTPR + (tmp2 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// 反転明暗透明通常表示
void tcfl_normal_reverse_bright_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			tmp1 = *FILTER_d_buf;
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp2 = FILTER_p_b + *(CTPB + (tmp2 - FILTER_p_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp2 - *FILTER_d_buf    ));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp2 = FILTER_p_g + *(CTPG + (tmp2 - FILTER_p_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+1)));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp2 = FILTER_p_r + *(CTPR + (tmp2 - FILTER_p_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp
				mov		esi, CTPCLR

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp
				mov		esi, CTPCLR

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
				mov		esi, CTPCLR
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転明暗透明通常表示
void tcfl_normal_reverse_bright_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_p_b + *(CTPB + (tmp2 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_p_g + *(CTPG + (tmp2 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_p_r + *(CTPR + (tmp2 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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






// 反転変色通常表示
void tcfl_normal_reverse_change(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			tmp1 = *FILTER_d_buf;
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*FILTER_d_buf     = FILTER_c_b + *(CTPBETA + (tmp2 - FILTER_c_b));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+1) = FILTER_c_g + *(CTPBETA + (tmp2 - FILTER_c_g));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+2) = FILTER_c_r + *(CTPBETA + (tmp2 - FILTER_c_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh

			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転変色通常表示
void tcfl_normal_reverse_change_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_c_b + *(CTPBETA + (tmp2 - FILTER_c_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_c_g + *(CTPBETA + (tmp2 - FILTER_c_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_c_r + *(CTPBETA + (tmp2 - FILTER_c_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// 反転変色透明通常表示
void tcfl_normal_reverse_change_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			tmp1 = *FILTER_d_buf;
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp2 = FILTER_c_b + *(CTPBETA + (tmp2 - FILTER_c_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp2 - *FILTER_d_buf    ));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp2 = FILTER_c_g + *(CTPBETA + (tmp2 - FILTER_c_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+1)));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = 255 - tmp1;
			tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp2 = FILTER_c_r + *(CTPBETA + (tmp2 - FILTER_c_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp2 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp
				mov		esi, CTPCLR

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp
				mov		esi, CTPCLR

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
				mov		esi, CTPCLR
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転変色透明通常表示
void tcfl_normal_reverse_change_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_c_b + *(CTPBETA + (tmp2 - FILTER_c_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp2 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_c_g + *(CTPBETA + (tmp2 - FILTER_c_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp2 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = 255 - tmp1;
					tmp2 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp2 = FILTER_c_r + *(CTPBETA + (tmp2 - FILTER_c_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp2 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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







// モノ明暗通常表示
void tcfl_normal_mono_bright(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			*FILTER_d_buf     = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			*(FILTER_d_buf+1) = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			*(FILTER_d_buf+2) = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh
			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイルモノ明暗通常表示
void tcfl_normal_mono_bright_tile(void){

	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// モノ明暗透明通常表示
void tcfl_normal_mono_bright_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp1 - *FILTER_d_buf    ));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+1)));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイルモノ明暗透明通常表示
void tcfl_normal_mono_bright_clr_tile(void){

	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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






// モノ変色通常表示
void tcfl_normal_mono_change(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			*FILTER_d_buf     = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			*(FILTER_d_buf+1) = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			*(FILTER_d_buf+2) = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh
			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイルモノ変色通常表示
void tcfl_normal_mono_change_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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




// モノ変色透明通常表示
void tcfl_normal_mono_change_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp1 - *FILTER_d_buf    ));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+1)));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイルモノ変色透明通常表示
void tcfl_normal_mono_change_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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






// 変色明暗通常表示
void tcfl_normal_change_bright(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1;

			tmp1 = *FILTER_d_buf;
			tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			*FILTER_d_buf     = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));

			tmp1 = *(FILTER_d_buf+1);
			tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			*(FILTER_d_buf+1) = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));

			tmp1 = *(FILTER_d_buf+2);
			tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			*(FILTER_d_buf+2) = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh

			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル変色明暗通常表示
void tcfl_normal_change_bright_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// 変色明暗透明通常表示
void tcfl_normal_change_bright_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1;

			tmp1 = *FILTER_d_buf;
			tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp1 - *FILTER_d_buf    ));

			tmp1 = *(FILTER_d_buf+1);
			tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+1)));

			tmp1 = *(FILTER_d_buf+2);
			tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp
				mov		esi, CTPCLR

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp
				mov		esi, CTPCLR

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
				mov		esi, CTPCLR
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル変色明暗透明通常表示
void tcfl_normal_change_bright_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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





// 反転モノ明暗通常表示
void tcfl_normal_reverse_mono_bright(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*FILTER_d_buf     = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+1) = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+2) = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh
			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転モノ明暗通常表示
void tcfl_normal_reverse_mono_bright_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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




// 反転モノ明暗透明通常表示
void tcfl_normal_reverse_mono_bright_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp1 - *FILTER_d_buf    ));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+1)));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転モノ明暗透明通常表示
void tcfl_normal_reverse_mono_bright_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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








// 反転モノ変色通常表示
void tcfl_normal_reverse_mono_change(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*FILTER_d_buf     = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+1) = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			*(FILTER_d_buf+2) = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh
			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転モノ変色通常表示
void tcfl_normal_reverse_mono_change_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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




// 反転モノ変色透明通常表示
void tcfl_normal_reverse_mono_change_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp1 - *FILTER_d_buf    ));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+1)));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転モノ変色透明通常表示
void tcfl_normal_reverse_mono_change_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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







// 反転変色明暗通常表示
void tcfl_normal_reverse_change_bright(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			tmp1 = *FILTER_d_buf;
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			*FILTER_d_buf     = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			*(FILTER_d_buf+1) = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			*(FILTER_d_buf+2) = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh

			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転変色明暗通常表示
void tcfl_normal_reverse_change_bright_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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



// 反転変色明暗透明通常表示
void tcfl_normal_reverse_change_bright_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			tmp1 = *FILTER_d_buf;
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp1 - *FILTER_d_buf    ));

			tmp1 = *(FILTER_d_buf+1);
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+1)));

			tmp1 = *(FILTER_d_buf+2);
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp
				mov		esi, CTPCLR

			flineh_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp
				mov		esi, CTPCLR

			flineb_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
				mov		esi, CTPCLR
			flinex_loop:

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転変色明暗透明通常表示
void tcfl_normal_reverse_change_bright_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					tmp1 = *FILTER_d_buf;
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = *(FILTER_d_buf+1);
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = *(FILTER_d_buf+2);
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				xor		ebx, ebx
				mov		bl, [edi]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		bl, [edi+1]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		bl, [edi+2]
				mov		eax, ebx
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					xor		ebx, ebx
					mov		bl, [edi]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		bl, [edi+1]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		bl, [edi+2]
					mov		eax, ebx
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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








// モノ変色明暗通常表示
void tcfl_normal_mono_change_bright(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			*FILTER_d_buf     = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			*(FILTER_d_buf+1) = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			*(FILTER_d_buf+2) = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh
			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイルモノ変色明暗通常表示
void tcfl_normal_mono_change_bright_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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




// モノ変色明暗透明通常表示
void tcfl_normal_mono_change_bright_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp1 - *FILTER_d_buf    ));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+1)));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイルモノ変色明暗透明通常表示
void tcfl_normal_mono_change_bright_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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










// 反転モノ変色明暗通常表示
void tcfl_normal_reverse_mono_change_bright(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			*FILTER_d_buf     = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			*(FILTER_d_buf+1) = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			*(FILTER_d_buf+2) = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, EALPHA
				add		edx, ENa
				cmp		edx, 0
				jl		flinex_step
				add		edi, EBETA
				sub		edx, ENh
			flinex_step:

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転モノ変色明暗通常表示
void tcfl_normal_reverse_mono_change_bright_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + 255);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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




// 反転モノ変色明暗透明通常表示
void tcfl_normal_reverse_mono_change_bright_clr(void){

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			int tmp1, tmp2;

			FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

			tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
			tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
			*FILTER_d_buf     = *FILTER_d_buf     + *(CTPCLR + (tmp1 - *FILTER_d_buf    ));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
			tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
			*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+1)));

			tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
			tmp2 = 255 - tmp1;
			tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
			tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
			tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
			*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CTPCLR + (tmp1 - *(FILTER_d_buf+2)));
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

				add		edi, edx

				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, ECNT
				inc		ecx
				mov		edx, ENs
			flinex_loop:

				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi

				mov		esi, CTPCLR

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
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

				dec		ecx
				jz		flinex_loop_end
				jmp		flinex_loop
			flinex_loop_end:
			}
		}
	}
}



// タイル反転モノ変色明暗透明通常表示
void tcfl_normal_reverse_mono_change_bright_clr_tile(void){
	int tmp;
	int *CLR;

	if(FILTER_yl == 0){
// 点
		if(FILTER_xl == 0){
			if(FILTER_t_reverse){
				tmp = (UC)(~(*(FILTER_t_adr + FILTER_t_x)));
			}
			else{
				tmp = *(FILTER_t_adr + FILTER_t_x);
			}
			tmp = (tmp << FILTER_t_shift) + FILTER_t_add;
			if(tmp < 0){
				tmp = 0;
			}
			else if(tmp > 255){
				tmp = 255;
			}
			if(tmp){
				tmp = *((*(Gclr_table_adr_p + tmp)) + FILTER_clr);
				if(tmp){
					int tmp1, tmp2;

					CLR = *(Gclr_table_adr_p + tmp);

					FILTER_pal = (*(Gmono_table_r_p + *(FILTER_d_buf+2)) + *(Gmono_table_g_p + *(FILTER_d_buf+1)) + *(Gmono_table_b_p + *FILTER_d_buf)) >> 8;

					tmp1 = FILTER_pal + *(CTPMONO + (*FILTER_d_buf     - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_b + *(CTPBETA + (tmp1 - FILTER_c_b));
					tmp1 = FILTER_p_b + *(CTPB + (tmp1 - FILTER_p_b));
					*FILTER_d_buf     = *FILTER_d_buf     + *(CLR + (tmp1 - *FILTER_d_buf    ));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+1) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_g + *(CTPBETA + (tmp1 - FILTER_c_g));
					tmp1 = FILTER_p_g + *(CTPG + (tmp1 - FILTER_p_g));
					*(FILTER_d_buf+1) = *(FILTER_d_buf+1) + *(CLR + (tmp1 - *(FILTER_d_buf+1)));

					tmp1 = FILTER_pal + *(CTPMONO + (*(FILTER_d_buf+2) - FILTER_pal));
					tmp2 = 255 - tmp1;
					tmp1 = tmp2 + *(CTPREV + (tmp1 - tmp2));
					tmp1 = FILTER_c_r + *(CTPBETA + (tmp1 - FILTER_c_r));
					tmp1 = FILTER_p_r + *(CTPR + (tmp1 - FILTER_p_r));
					*(FILTER_d_buf+2) = *(FILTER_d_buf+2) + *(CLR + (tmp1 - *(FILTER_d_buf+2)));
				}
			}
		}
// 水平線
		else{
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_xl
				inc		ecx
				mov		edx, FILTER_xp

			flineh_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineh_tile_normal
				not		al
			flineh_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineh_step:
				mov		eax, FILTER_t_x
				add		eax, FILTER_t_x_add
				cmp		eax, 0
				jge		flineh_jge
				mov		eax, FILTER_t_xl
				dec		eax
				jmp		flineh_end
			flineh_jge:
				cmp		eax, FILTER_t_xl
				jl		flineh_end
				xor		eax, eax
			flineh_end:

				mov		FILTER_t_x, eax
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
		if(FILTER_xl == 0){
			_asm{
				cld
				mov		edi, FILTER_d_buf
				mov		ecx, FILTER_yl
				inc		ecx
				mov		edx, FILTER_yp

			flineb_loop:

				mov		ebx, FILTER_t_adr
				add		ebx, FILTER_t_x
				xor		eax, eax
				mov		al, [ebx]
				cmp		FILTER_t_reverse, 0
				je		flineb_tile_normal
				not		al
			flineb_tile_normal:
				push	ecx
				mov		ecx, FILTER_t_shift
				shl		eax, cl
				pop		ecx
				add		eax, FILTER_t_add
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
				add		ebx, FILTER_clr4
				mov		ebx, [ebx]
				and		bl, bl
				jz		flineh_step
				shl		ebx, 2
				add		ebx, Gclr_table_adr_p
				mov		esi, [ebx]

				push	esi
				xor		esi, esi
				mov		ebx, Gmono_table_b_p
				xor		eax, eax
				mov		al, [edi]
				shl		eax, 2
				add		ebx, eax
				mov		esi, [ebx]
				mov		ebx, Gmono_table_g_p
				xor		eax, eax
				mov		al, [edi+1]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				mov		ebx, Gmono_table_r_p
				xor		eax, eax
				mov		al, [edi+2]
				shl		eax, 2
				add		ebx, eax
				add		esi, [ebx]
				shr		esi, 8
				mov		FILTER_pal, esi
				pop		esi

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_b
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPB
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_g
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPG
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+1]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+1], al

				xor		ebx, ebx
				mov		eax, FILTER_pal
				mov		bl, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPMONO
				add		eax, [ebx]
				mov		ebx, eax
				not		al
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPREV
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_c_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPBETA
				add		eax, [ebx]
				mov		ebx, eax
				mov		eax, FILTER_p_r
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, CTPR
				add		eax, [ebx]
				mov		ebx, eax
				xor		eax, eax
				mov		al, [edi+2]
				sub		ebx, eax
				shl		ebx, 2
				add		ebx, esi
				add		eax, [ebx]
				mov		[edi+2], al

			flineb_step:
				mov		eax, FILTER_t_adr
				add		eax, FILTER_t_y_add
				cmp		eax, FILTER_t_adr_org
				jge		flineb_jge
				mov		eax, FILTER_t_adr_max
				sub		eax, FILTER_t_xl
				jmp		flineb_end
			flineb_jge:
				cmp		eax, FILTER_t_adr_max
				jl		flineb_end
				mov		eax, FILTER_t_adr_org
			flineb_end:

				mov		FILTER_t_adr, eax
				add		edi, edx
				dec		ecx
				jz		flineb_loop_end
				jmp		flineb_loop
			flineb_loop_end:
			}
		}
		else{
// 斜線
			if(FILTER_xl > FILTER_yl){
				_asm{
					cld
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_xl
					inc		ecx
					mov		edx, ENs

				flinex_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		flinex_tile_normal
					not		al
				flinex_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				flinex_step:

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		flinex_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		flinex_end1
				flinex_jge1:
					cmp		eax, FILTER_t_xl
					jl		flinex_end1
					xor		eax, eax
				flinex_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
					add		edx, ENa
					cmp		edx, 0
					jl		flinex_step2

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		flinex_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		flinex_end2
				flinex_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		flinex_end2
					mov		eax, FILTER_t_adr_org
				flinex_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
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
					mov		edi, FILTER_d_buf
					mov		ecx, FILTER_yl
					inc		ecx
					mov		edx, ENs

				fliney_loop:

					mov		ebx, FILTER_t_adr
					add		ebx, FILTER_t_x
					xor		eax, eax
					mov		al, [ebx]
					cmp		FILTER_t_reverse, 0
					je		fliney_tile_normal
					not		al
				fliney_tile_normal:
					push	ecx
					mov		ecx, FILTER_t_shift
					shl		eax, cl
					pop		ecx
					add		eax, FILTER_t_add
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
					add		ebx, FILTER_clr4
					mov		ebx, [ebx]
					and		bl, bl
					jz		flineh_step
					shl		ebx, 2
					add		ebx, Gclr_table_adr_p
					mov		esi, [ebx]

					push	esi
					xor		esi, esi
					mov		ebx, Gmono_table_b_p
					xor		eax, eax
					mov		al, [edi]
					shl		eax, 2
					add		ebx, eax
					mov		esi, [ebx]
					mov		ebx, Gmono_table_g_p
					xor		eax, eax
					mov		al, [edi+1]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					mov		ebx, Gmono_table_r_p
					xor		eax, eax
					mov		al, [edi+2]
					shl		eax, 2
					add		ebx, eax
					add		esi, [ebx]
					shr		esi, 8
					mov		FILTER_pal, esi
					pop		esi

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_b
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPB
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_g
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPG
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+1]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+1], al

					xor		ebx, ebx
					mov		eax, FILTER_pal
					mov		bl, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPMONO
					add		eax, [ebx]
					mov		ebx, eax
					not		al
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPREV
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_c_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPBETA
					add		eax, [ebx]
					mov		ebx, eax
					mov		eax, FILTER_p_r
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, CTPR
					add		eax, [ebx]
					mov		ebx, eax
					xor		eax, eax
					mov		al, [edi+2]
					sub		ebx, eax
					shl		ebx, 2
					add		ebx, esi
					add		eax, [ebx]
					mov		[edi+2], al

				fliney_step:

					mov		eax, FILTER_t_adr
					add		eax, FILTER_t_y_add
					cmp		eax, FILTER_t_adr_org
					jge		fliney_jge2
					mov		eax, FILTER_t_adr_max
					sub		eax, FILTER_t_xl
					jmp		fliney_end2
				fliney_jge2:
					cmp		eax, FILTER_t_adr_max
					jl		fliney_end2
					mov		eax, FILTER_t_adr_org
				fliney_end2:
					mov		FILTER_t_adr, eax

					add		edi, FILTER_yp
					add		edx, ENa
					cmp		edx, 0
					jl		fliney_step2

					mov		eax, FILTER_t_x
					add		eax, FILTER_t_x_add
					cmp		eax, 0
					jge		fliney_jge1
					mov		eax, FILTER_t_xl
					dec		eax
					jmp		fliney_end1
				fliney_jge1:
					cmp		eax, FILTER_t_xl
					jl		fliney_end1
					xor		eax, eax
				fliney_end1:
					mov		FILTER_t_x, eax

					add		edi, FILTER_xp
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
	ラインフィルタ表示
void true_color_filterline(
	UC *d_buf, int d_xl, int d_yl,
	int x1, int y1, int x2, int y2,
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
void true_color_filterline(
	UC *d_buf, int d_xl, int d_yl,
	int x1, int y1, int x2, int y2,
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

	if(d_buf==0 || (tile && t_add==-(255<<t_shift))){
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
	FILTER_d_buf = d_buf;
	FILTER_d_xl  = d_xl;
	FILTER_d_yl  = d_yl;

	FILTER_d_xl = d_xl << 2;

	FILTER_xl = xl;
	FILTER_yl = yl;

	FILTER_xp = xp << 2;
	FILTER_yp = yp << 2;

	if(FILTER_xl > FILTER_yl){
		ENs = -FILTER_xl;
		ENa = FILTER_yl<<1;
		ENh = FILTER_xl<<1;
		ECNT = FILTER_xl;
		FILTER_yp = -FILTER_yp;
		EALPHA = FILTER_xp;
		EBETA  = FILTER_yp;
	}
	else{
		ENs = -FILTER_yl;
		ENa = FILTER_xl<<1;
		ENh = FILTER_yl<<1;
		ECNT = FILTER_yl;
		FILTER_yp = -FILTER_yp;
		EALPHA = FILTER_yp;
		EBETA  = FILTER_xp;
	}

// 反転パラメータ
	CTPREV = *(Gclr_table_adr_p + (255 - reverse));
	FILTER_reverse = reverse;

// モノパラメータ
	CTPMONO = *(Gclr_table_adr_p + (255 - mono));
	FILTER_mono = mono;

// 変色パラメータ
	if(c_rate){
		FILTER_c_r = c_r;
		FILTER_c_g = c_g;
		FILTER_c_b = c_b;
		CTPBETA = *(Gclr_table_adr_p + (255 - c_rate));
	}

// 透明度パラメータ
	FILTER_clr  = clr;
	FILTER_clr4 = (clr << 2);
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

	FILTER_p_r = p_r;
	FILTER_p_g = p_g;
	FILTER_p_b = p_b;
	if(FILTER_p_r >= 0){
		CTPR = *(Gclr_table_adr_p + (255 - FILTER_p_r));
		FILTER_p_r = 255;
	}
	else{
		CTPR = *(Gclr_table_adr_p + (255 + FILTER_p_r));
		FILTER_p_r = 0;
	}
	if(FILTER_p_g >= 0){
		CTPG = *(Gclr_table_adr_p + (255 - FILTER_p_g));
		FILTER_p_g = 255;
	}
	else{
		CTPG = *(Gclr_table_adr_p + (255 + FILTER_p_g));
		FILTER_p_g = 0;
	}
	if(FILTER_p_b >= 0){
		CTPB = *(Gclr_table_adr_p + (255 - FILTER_p_b));
		FILTER_p_b = 255;
	}
	else{
		CTPB = *(Gclr_table_adr_p + (255 + FILTER_p_b));
		FILTER_p_b = 0;
	}

// タイルパラメータ
	if(tile){
		FILTER_t_add = t_add;
		FILTER_t_reverse = t_reverse;
		FILTER_t_shift = t_shift;
		FILTER_t_xl = t_xl;
		FILTER_t_yl = t_yl;

		if(x1 >= t_repx){
			FILTER_t_x = FILTER_t_x_org = (x1 - t_repx) % t_xl;
		}
		else{
			FILTER_t_x = FILTER_t_x_org = (t_xl - ((t_repx - x1) % t_xl)) % t_xl;
		}
		if(y1 >= t_repy){
			FILTER_t_y                   = (y1 - t_repy) % t_yl;
		}
		else{
			FILTER_t_y                   = (t_yl - ((t_repy - y1) % t_yl)) % t_yl;
		}

		FILTER_t_adr     = tile + (FILTER_t_y * t_xl);
		FILTER_t_adr_org = tile;
		FILTER_t_adr_max = tile + (t_xl * t_yl);
		if(xp < 0){
			FILTER_t_x_add = -1;
		}
		else{
			FILTER_t_x_add = 1;
		}
		if(yp < 0){
			FILTER_t_y_add = -t_xl;
		}
		else{
			FILTER_t_y_add = t_xl;
		}
	}

//*****************************************************************************************

	if(clr == 255){
		if(c_rate == 0){
			if(p_r==0 && p_g==0 && p_b==0){
				if(mono == 0){
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 通常表示
//tcfl_normal();		// 処理する必要なし
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル通常表示
//tcfl_normal_tile();	// 処理する必要なし
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転通常表示
tcfl_normal_reverse();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転通常表示
tcfl_normal_reverse_tile();
									break;
							}
						}
					}
				}
				else{
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// モノ通常表示
tcfl_normal_mono();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイルモノ通常表示
tcfl_normal_mono_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転モノ通常表示
tcfl_normal_reverse_mono();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転モノ通常表示
tcfl_normal_reverse_mono_tile();
									break;
							}
						}
					}
				}
			}
			else{
				if(mono == 0){
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 明暗通常表示
tcfl_normal_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル明暗通常表示
tcfl_normal_bright_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転明暗通常表示
tcfl_normal_reverse_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転明暗通常表示
tcfl_normal_reverse_bright_tile();
									break;
							}
						}
					}
				}
				else{
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// モノ明暗通常表示
tcfl_normal_mono_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイルモノ明暗通常表示
tcfl_normal_mono_bright_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転モノ明暗通常表示
tcfl_normal_reverse_mono_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転モノ明暗通常表示
tcfl_normal_reverse_mono_bright_tile();
									break;
							}
						}
					}
				}
			}
		}
		else{
			if(p_r==0 && p_g==0 && p_b==0){
				if(mono == 0){
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 変色通常表示
tcfl_normal_change();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル変色通常表示
tcfl_normal_change_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転変色通常表示
tcfl_normal_reverse_change();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転変色通常表示
tcfl_normal_reverse_change_tile();
									break;
							}
						}
					}
				}
				else{
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// モノ変色通常表示
tcfl_normal_mono_change();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイルモノ変色通常表示
tcfl_normal_mono_change_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転モノ変色通常表示
tcfl_normal_reverse_mono_change();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転モノ変色通常表示
tcfl_normal_reverse_mono_change_tile();
									break;
							}
						}
					}
				}
			}
			else{
				if(mono == 0){
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 変色明暗通常表示
tcfl_normal_change_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル変色明暗通常表示
tcfl_normal_change_bright_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転変色明暗通常表示
tcfl_normal_reverse_change_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転変色明暗通常表示
tcfl_normal_reverse_change_bright_tile();
									break;
							}
						}
					}
				}
				else{
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// モノ変色明暗通常表示
tcfl_normal_mono_change_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイルモノ変色明暗通常表示
tcfl_normal_mono_change_bright_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転モノ変色明暗通常表示
tcfl_normal_reverse_mono_change_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転モノ変色明暗通常表示
tcfl_normal_reverse_mono_change_bright_tile();
									break;
							}
						}
					}
				}
			}
		}
	}
	else{
		if(c_rate == 0){
			if(p_r==0 && p_g==0 && p_b==0){
				if(mono == 0){
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 透明通常表示
//tcfl_normal_clr();		// 処理する必要なし
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル透明通常表示
//tcfl_normal_clr_tile();	// 処理する必要なし
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転透明通常表示
tcfl_normal_reverse_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転透明通常表示
tcfl_normal_reverse_clr_tile();
									break;
							}
						}
					}
				}
				else{
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// モノ透明通常表示
tcfl_normal_mono_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイルモノ透明通常表示
tcfl_normal_mono_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転モノ透明通常表示
tcfl_normal_reverse_mono_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転モノ透明通常表示
tcfl_normal_reverse_mono_clr_tile();
									break;
							}
						}
					}
				}
			}
			else{
				if(mono == 0){
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 明暗透明通常表示
tcfl_normal_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル明暗透明通常表示
tcfl_normal_bright_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転明暗透明通常表示
tcfl_normal_reverse_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転明暗透明通常表示
tcfl_normal_reverse_bright_clr_tile();
									break;
							}
						}
					}
				}
				else{
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// モノ明暗透明通常表示
tcfl_normal_mono_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイルモノ明暗透明通常表示
tcfl_normal_mono_bright_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転モノ明暗透明通常表示
tcfl_normal_reverse_mono_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転モノ明暗透明通常表示
tcfl_normal_reverse_mono_bright_clr_tile();
									break;
							}
						}
					}
				}
			}
		}
		else{
			if(p_r==0 && p_g==0 && p_b==0){
				if(mono == 0){
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 変色透明通常表示
tcfl_normal_change_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル変色透明通常表示
tcfl_normal_change_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転変色透明通常表示
tcfl_normal_reverse_change_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転変色透明通常表示
tcfl_normal_reverse_change_clr_tile();
									break;
							}
						}
					}
				}
				else{
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// モノ変色透明通常表示
tcfl_normal_mono_change_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイルモノ変色透明通常表示
tcfl_normal_mono_change_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転モノ変色透明通常表示
tcfl_normal_reverse_mono_change_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転モノ変色透明通常表示
tcfl_normal_reverse_mono_change_clr_tile();
									break;
							}
						}
					}
				}
			}
			else{
				if(mono == 0){
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 変色明暗透明通常表示
tcfl_normal_change_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル変色明暗透明通常表示
tcfl_normal_change_bright_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転変色明暗透明通常表示
tcfl_normal_reverse_change_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転変色明暗透明通常表示
tcfl_normal_reverse_change_bright_clr_tile();
									break;
							}
						}
					}
				}
				else{
					if(reverse == 0){
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// モノ変色明暗透明通常表示
tcfl_normal_mono_change_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイルモノ変色明暗透明通常表示
tcfl_normal_mono_change_bright_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// 反転モノ変色明暗透明通常表示
tcfl_normal_reverse_mono_change_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// タイル反転モノ変色明暗透明通常表示
tcfl_normal_reverse_mono_change_bright_clr_tile();
									break;
							}
						}
					}
				}
			}
		}
	}
}



