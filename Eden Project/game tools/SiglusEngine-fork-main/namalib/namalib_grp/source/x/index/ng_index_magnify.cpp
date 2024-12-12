#include	"pch.h"
#include	"ng_macro.h"





/*========================================================================================
	ägëÂèkè¨ï\é¶
void index_color_magnify(
	UC *dst_p, int dst_xl, int dst_yl,
	int dx1, int dy1, int dx2, int dy2,
	UC *src_p, int src_xl, int src_yl,
	UC *pal, int palcnt,
	int sx1, int sy1, int sx2, int sy2,
	int srx, int sry, int sxl, int syl,
	int ex1, int ey1, int ex2, int ey2,
	UC clr
);
		IN		Ç»Çµ
		OUT		Ç»Çµ
----------------------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------------------------

#define		ICMAG_NORMAL_MACRO		\
__asm	xor		eax, eax			\
__asm	mov		al, [esi]			\
__asm	mov		ebx, IC_pal			\
__asm	shl		eax, 2				\
__asm	add		ebx, eax			\
__asm	mov		eax, [ebx]			\
__asm	mov		[edi],eax

//-----------------------------------------------------------------------------------------------

void icmag_normal(void){
	if(ICMAG_sxl == ICMAG_dxl){
		if(ICMAG_syl == ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
			a0_loop_y:

				push	ecx
				push	esi
				push	edi

				mov		ecx, ICMAG_x_count
			a0_loop_x:
				ICMAG_NORMAL_MACRO
				add		esi, 1
				add		edi, 4
				dec		ecx
				jz		a0_loop_x_end
				jmp		a0_loop_x
			a0_loop_x_end:

				pop		edi
				pop		esi
				pop		ecx

				dec		ecx
				jz		a0_loop_y_end
				add		esi, ICMAG_src_add_adr
				add		edi, ICMAG_dst_add_adr
				jmp		a0_loop_y
			a0_loop_y_end:
			}
		}
		else if(ICMAG_syl < ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			a1_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		ecx, ICMAG_x_count
			a1_loop_x:
				ICMAG_NORMAL_MACRO
				add		esi, 1
				add		edi, 4
				dec		ecx
				jz		a1_loop_x_end
				jmp		a1_loop_x
			a1_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

				add		edi, ICMAG_dst_add_adr
				add		ebx, ICMAG_syl
				cmp		ebx, ICMAG_dyl
				jl		a1_step_y1
				sub		ebx, ICMAG_dyl
				add		esi, ICMAG_src_add_adr
			a1_step_y1:
				dec		ecx
				jz		a1_loop_y_end
				jmp		a1_loop_y
			a1_loop_y_end:
			}
		}
		else{
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			a2_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		ecx, ICMAG_x_count
			a2_loop_x:
				ICMAG_NORMAL_MACRO
				add		esi, 1
				add		edi, 4
				dec		ecx
				jz		a2_loop_x_end
				jmp		a2_loop_x
			a2_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

			a2_loop_y_free:
				add		esi, ICMAG_src_add_adr
				add		ebx, ICMAG_dyl
				cmp		ebx, ICMAG_syl
				jl		a2_step_y1
				sub		ebx, ICMAG_syl
				add		edi, ICMAG_dst_add_adr
				jmp		a2_step_y2
			a2_step_y1:
				dec		ecx
				jz		a2_loop_y_end
				jmp		a2_loop_y_free
			a2_step_y2:
				dec		ecx
				jz		a2_loop_y_end
				jmp		a2_loop_y
			a2_loop_y_end:
			}
		}
	}
	else if(ICMAG_sxl < ICMAG_dxl){
		if(ICMAG_syl == ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
			b0_loop_y:

				push	ecx
				push	esi
				push	edi

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			b0_loop_x:
				ICMAG_NORMAL_MACRO
				add		edi, 4
				add		edx, ICMAG_sxl
				cmp		edx, ICMAG_dxl
				jl		b0_step_x1
				sub		edx, ICMAG_dxl
				add		esi, 1
			b0_step_x1:
				dec		ecx
				jz		b0_loop_x_end
				jmp		b0_loop_x
			b0_loop_x_end:

				pop		edi
				pop		esi
				pop		ecx

				dec		ecx
				jz		b0_loop_y_end
				add		esi, ICMAG_src_add_adr
				add		edi, ICMAG_dst_add_adr
				jmp		b0_loop_y
			b0_loop_y_end:
			}
		}
		else if(ICMAG_syl < ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			b1_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			b1_loop_x:
				ICMAG_NORMAL_MACRO
				add		edi, 4
				add		edx, ICMAG_sxl
				cmp		edx, ICMAG_dxl
				jl		b1_step_x1
				sub		edx, ICMAG_dxl
				add		esi, 1
			b1_step_x1:
				dec		ecx
				jz		b1_loop_x_end
				jmp		b1_loop_x
			b1_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

				add		edi, ICMAG_dst_add_adr
				add		ebx, ICMAG_syl
				cmp		ebx, ICMAG_dyl
				jl		b1_step_y1
				sub		ebx, ICMAG_dyl
				add		esi, ICMAG_src_add_adr
			b1_step_y1:
				dec		ecx
				jz		b1_loop_y_end
				jmp		b1_loop_y
			b1_loop_y_end:
			}
		}
		else{
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			b2_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			b2_loop_x:
				ICMAG_NORMAL_MACRO
				add		edi, 4
				add		edx, ICMAG_sxl
				cmp		edx, ICMAG_dxl
				jl		b2_step_x1
				sub		edx, ICMAG_dxl
				add		esi, 1
			b2_step_x1:
				dec		ecx
				jz		b2_loop_x_end
				jmp		b2_loop_x
			b2_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

			b2_loop_y_free:
				add		esi, ICMAG_src_add_adr
				add		ebx, ICMAG_dyl
				cmp		ebx, ICMAG_syl
				jl		b2_step_y1
				sub		ebx, ICMAG_syl
				add		edi, ICMAG_dst_add_adr
				jmp		b2_step_y2
			b2_step_y1:
				dec		ecx
				jz		b2_loop_y_end
				jmp		b2_loop_y_free
			b2_step_y2:
				dec		ecx
				jz		b2_loop_y_end
				jmp		b2_loop_y
			b2_loop_y_end:
			}
		}
	}
	else{
		if(ICMAG_syl == ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
			c0_loop_y:

				push	ecx
				push	esi
				push	edi

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			c0_loop_x:
				ICMAG_NORMAL_MACRO
			c0_loop_x_free:
				add		esi, 1
				add		edx, ICMAG_dxl
				cmp		edx, ICMAG_sxl
				jl		c0_step_x1
				sub		edx, ICMAG_sxl
				add		edi, 4
				jmp		c0_step_x2
			c0_step_x1:
				dec		ecx
				jz		c0_loop_x_end
				jmp		c0_loop_x_free
			c0_step_x2:
				dec		ecx
				jz		c0_loop_x_end
				jmp		c0_loop_x
			c0_loop_x_end:

				pop		edi
				pop		esi
				pop		ecx

				dec		ecx
				jz		c0_loop_y_end
				add		esi, ICMAG_src_add_adr
				add		edi, ICMAG_dst_add_adr
				jmp		c0_loop_y
			c0_loop_y_end:
			}
		}
		else if(ICMAG_syl < ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			c1_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			c1_loop_x:
				ICMAG_NORMAL_MACRO
			c1_loop_x_free:
				add		esi, 1
				add		edx, ICMAG_dxl
				cmp		edx, ICMAG_sxl
				jl		c1_step_x1
				sub		edx, ICMAG_sxl
				add		edi, 4
				jmp		c1_step_x2
			c1_step_x1:
				dec		ecx
				jz		c1_loop_x_end
				jmp		c1_loop_x_free
			c1_step_x2:
				dec		ecx
				jz		c1_loop_x_end
				jmp		c1_loop_x
			c1_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

				add		edi, ICMAG_dst_add_adr
				add		ebx, ICMAG_syl
				cmp		ebx, ICMAG_dyl
				jl		c1_step_y1
				sub		ebx, ICMAG_dyl
				add		esi, ICMAG_src_add_adr
			c1_step_y1:
				dec		ecx
				jz		c1_loop_y_end
				jmp		c1_loop_y
			c1_loop_y_end:
			}
		}
		else{
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			c2_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			c2_loop_x:
				ICMAG_NORMAL_MACRO
			c2_loop_x_free:
				add		esi, 1
				add		edx, ICMAG_dxl
				cmp		edx, ICMAG_sxl
				jl		c2_step_x1
				sub		edx, ICMAG_sxl
				add		edi, 4
				jmp		c2_step_x2
			c2_step_x1:
				dec		ecx
				jz		c2_loop_x_end
				jmp		c2_loop_x_free
			c2_step_x2:
				dec		ecx
				jz		c2_loop_x_end
				jmp		c2_loop_x
			c2_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

			c2_loop_y_free:
				add		esi, ICMAG_src_add_adr
				add		ebx, ICMAG_dyl
				cmp		ebx, ICMAG_syl
				jl		c2_step_y1
				sub		ebx, ICMAG_syl
				add		edi, ICMAG_dst_add_adr
				jmp		c2_step_y2
			c2_step_y1:
				dec		ecx
				jz		c2_loop_y_end
				jmp		c2_loop_y_free
			c2_step_y2:
				dec		ecx
				jz		c2_loop_y_end
				jmp		c2_loop_y
			c2_loop_y_end:
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------

#define		ICMAG_CLR_MACRO			\
__asm	push	esi					\
									\
__asm	xor		eax, eax			\
__asm	mov		al, [esi]			\
__asm	mov		esi, IC_pal			\
__asm	shl		eax, 2				\
__asm	add		esi, eax			\
									\
__asm	xor		eax, eax			\
__asm	xor		ebx, ebx			\
__asm	mov		al, [edi]			\
__asm	mov		bl, [esi]			\
__asm	sub		ebx, eax			\
__asm	shl		ebx, 2				\
__asm	add		ebx, CTPCLR			\
__asm	add		eax, [ebx]			\
__asm	mov		[edi], al			\
									\
__asm	xor		eax, eax			\
__asm	xor		ebx, ebx			\
__asm	mov		al, [edi+1]			\
__asm	mov		bl, [esi+1]			\
__asm	sub		ebx, eax			\
__asm	shl		ebx, 2				\
__asm	add		ebx, CTPCLR			\
__asm	add		eax, [ebx]			\
__asm	mov		[edi+1], al			\
									\
__asm	xor		eax, eax			\
__asm	xor		ebx, ebx			\
__asm	mov		al, [edi+2]			\
__asm	mov		bl, [esi+2]			\
__asm	sub		ebx, eax			\
__asm	shl		ebx, 2				\
__asm	add		ebx, CTPCLR			\
__asm	add		eax, [ebx]			\
__asm	mov		[edi+2], al			\
									\
__asm	pop	esi

void icmag_normal_clr(void){
	if(ICMAG_sxl == ICMAG_dxl){
		if(ICMAG_syl == ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
			a0_loop_y:

				push	ecx
				push	esi
				push	edi

				mov		ecx, ICMAG_x_count
			a0_loop_x:
				ICMAG_CLR_MACRO
				add		esi, 1
				add		edi, 4
				dec		ecx
				jz		a0_loop_x_end
				jmp		a0_loop_x
			a0_loop_x_end:

				pop		edi
				pop		esi
				pop		ecx

				dec		ecx
				jz		a0_loop_y_end
				add		esi, ICMAG_src_add_adr
				add		edi, ICMAG_dst_add_adr
				jmp		a0_loop_y
			a0_loop_y_end:
			}
		}
		else if(ICMAG_syl < ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			a1_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		ecx, ICMAG_x_count
			a1_loop_x:
				ICMAG_CLR_MACRO
				add		esi, 1
				add		edi, 4
				dec		ecx
				jz		a1_loop_x_end
				jmp		a1_loop_x
			a1_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

				add		edi, ICMAG_dst_add_adr
				add		ebx, ICMAG_syl
				cmp		ebx, ICMAG_dyl
				jl		a1_step_y1
				sub		ebx, ICMAG_dyl
				add		esi, ICMAG_src_add_adr
			a1_step_y1:
				dec		ecx
				jz		a1_loop_y_end
				jmp		a1_loop_y
			a1_loop_y_end:
			}
		}
		else{
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			a2_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		ecx, ICMAG_x_count
			a2_loop_x:
				ICMAG_CLR_MACRO
				add		esi, 1
				add		edi, 4
				dec		ecx
				jz		a2_loop_x_end
				jmp		a2_loop_x
			a2_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

			a2_loop_y_free:
				add		esi, ICMAG_src_add_adr
				add		ebx, ICMAG_dyl
				cmp		ebx, ICMAG_syl
				jl		a2_step_y1
				sub		ebx, ICMAG_syl
				add		edi, ICMAG_dst_add_adr
				jmp		a2_step_y2
			a2_step_y1:
				dec		ecx
				jz		a2_loop_y_end
				jmp		a2_loop_y_free
			a2_step_y2:
				dec		ecx
				jz		a2_loop_y_end
				jmp		a2_loop_y
			a2_loop_y_end:
			}
		}
	}
	else if(ICMAG_sxl < ICMAG_dxl){
		if(ICMAG_syl == ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
			b0_loop_y:

				push	ecx
				push	esi
				push	edi

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			b0_loop_x:
				ICMAG_CLR_MACRO
				add		edi, 4
				add		edx, ICMAG_sxl
				cmp		edx, ICMAG_dxl
				jl		b0_step_x1
				sub		edx, ICMAG_dxl
				add		esi, 1
			b0_step_x1:
				dec		ecx
				jz		b0_loop_x_end
				jmp		b0_loop_x
			b0_loop_x_end:

				pop		edi
				pop		esi
				pop		ecx

				dec		ecx
				jz		b0_loop_y_end
				add		esi, ICMAG_src_add_adr
				add		edi, ICMAG_dst_add_adr
				jmp		b0_loop_y
			b0_loop_y_end:
			}
		}
		else if(ICMAG_syl < ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			b1_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			b1_loop_x:
				ICMAG_CLR_MACRO
				add		edi, 4
				add		edx, ICMAG_sxl
				cmp		edx, ICMAG_dxl
				jl		b1_step_x1
				sub		edx, ICMAG_dxl
				add		esi, 1
			b1_step_x1:
				dec		ecx
				jz		b1_loop_x_end
				jmp		b1_loop_x
			b1_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

				add		edi, ICMAG_dst_add_adr
				add		ebx, ICMAG_syl
				cmp		ebx, ICMAG_dyl
				jl		b1_step_y1
				sub		ebx, ICMAG_dyl
				add		esi, ICMAG_src_add_adr
			b1_step_y1:
				dec		ecx
				jz		b1_loop_y_end
				jmp		b1_loop_y
			b1_loop_y_end:
			}
		}
		else{
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			b2_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			b2_loop_x:
				ICMAG_CLR_MACRO
				add		edi, 4
				add		edx, ICMAG_sxl
				cmp		edx, ICMAG_dxl
				jl		b2_step_x1
				sub		edx, ICMAG_dxl
				add		esi, 1
			b2_step_x1:
				dec		ecx
				jz		b2_loop_x_end
				jmp		b2_loop_x
			b2_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

			b2_loop_y_free:
				add		esi, ICMAG_src_add_adr
				add		ebx, ICMAG_dyl
				cmp		ebx, ICMAG_syl
				jl		b2_step_y1
				sub		ebx, ICMAG_syl
				add		edi, ICMAG_dst_add_adr
				jmp		b2_step_y2
			b2_step_y1:
				dec		ecx
				jz		b2_loop_y_end
				jmp		b2_loop_y_free
			b2_step_y2:
				dec		ecx
				jz		b2_loop_y_end
				jmp		b2_loop_y
			b2_loop_y_end:
			}
		}
	}
	else{
		if(ICMAG_syl == ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
			c0_loop_y:

				push	ecx
				push	esi
				push	edi

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			c0_loop_x:
				ICMAG_CLR_MACRO
			c0_loop_x_free:
				add		esi, 1
				add		edx, ICMAG_dxl
				cmp		edx, ICMAG_sxl
				jl		c0_step_x1
				sub		edx, ICMAG_sxl
				add		edi, 4
				jmp		c0_step_x2
			c0_step_x1:
				dec		ecx
				jz		c0_loop_x_end
				jmp		c0_loop_x_free
			c0_step_x2:
				dec		ecx
				jz		c0_loop_x_end
				jmp		c0_loop_x
			c0_loop_x_end:

				pop		edi
				pop		esi
				pop		ecx

				dec		ecx
				jz		c0_loop_y_end
				add		esi, ICMAG_src_add_adr
				add		edi, ICMAG_dst_add_adr
				jmp		c0_loop_y
			c0_loop_y_end:
			}
		}
		else if(ICMAG_syl < ICMAG_dyl){
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			c1_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			c1_loop_x:
				ICMAG_CLR_MACRO
			c1_loop_x_free:
				add		esi, 1
				add		edx, ICMAG_dxl
				cmp		edx, ICMAG_sxl
				jl		c1_step_x1
				sub		edx, ICMAG_sxl
				add		edi, 4
				jmp		c1_step_x2
			c1_step_x1:
				dec		ecx
				jz		c1_loop_x_end
				jmp		c1_loop_x_free
			c1_step_x2:
				dec		ecx
				jz		c1_loop_x_end
				jmp		c1_loop_x
			c1_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

				add		edi, ICMAG_dst_add_adr
				add		ebx, ICMAG_syl
				cmp		ebx, ICMAG_dyl
				jl		c1_step_y1
				sub		ebx, ICMAG_dyl
				add		esi, ICMAG_src_add_adr
			c1_step_y1:
				dec		ecx
				jz		c1_loop_y_end
				jmp		c1_loop_y
			c1_loop_y_end:
			}
		}
		else{
			_asm{
				mov		esi, ICMAG_src
				mov		edi, ICMAG_dst
				mov		ecx, ICMAG_y_count
				mov		ebx, ICMAG_y_alpha
			c2_loop_y:

				push	ecx
				push	esi
				push	edi
				push	ebx

				mov		edx, ICMAG_x_alpha
				mov		ecx, ICMAG_x_count
			c2_loop_x:
				ICMAG_CLR_MACRO
			c2_loop_x_free:
				add		esi, 1
				add		edx, ICMAG_dxl
				cmp		edx, ICMAG_sxl
				jl		c2_step_x1
				sub		edx, ICMAG_sxl
				add		edi, 4
				jmp		c2_step_x2
			c2_step_x1:
				dec		ecx
				jz		c2_loop_x_end
				jmp		c2_loop_x_free
			c2_step_x2:
				dec		ecx
				jz		c2_loop_x_end
				jmp		c2_loop_x
			c2_loop_x_end:

				pop		ebx
				pop		edi
				pop		esi
				pop		ecx

			c2_loop_y_free:
				add		esi, ICMAG_src_add_adr
				add		ebx, ICMAG_dyl
				cmp		ebx, ICMAG_syl
				jl		c2_step_y1
				sub		ebx, ICMAG_syl
				add		edi, ICMAG_dst_add_adr
				jmp		c2_step_y2
			c2_step_y1:
				dec		ecx
				jz		c2_loop_y_end
				jmp		c2_loop_y_free
			c2_step_y2:
				dec		ecx
				jz		c2_loop_y_end
				jmp		c2_loop_y
			c2_loop_y_end:
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------

void index_color_magnify(
	UC *dst_p, int dst_xl, int dst_yl,
	int dx1, int dy1, int dx2, int dy2,
	UC *src_p, int src_xl, int src_yl,
	UC *pal, int palcnt,
	int sx1, int sy1, int sx2, int sy2,
	int srx, int sry, int sxl, int syl,
	int ex1, int ey1, int ex2, int ey2,
	UC clr
){
	int tmp;
	int _dxlen, _dylen;
	int _sxlen, _sylen;
	int x_alpha, y_alpha;
	int dx_free, dy_free;
	int sx_free, sy_free;
	int x_count, y_count;

	if(dst_p==0 || src_p==0 || clr==0){
		return;
	}

	if(dst_p==0 || src_p==0){
		return;
	}

	if(dst_xl==0 || src_xl==0 || sxl==0){
		return;
	}

	if(dst_yl==0 || src_yl==0 || syl==0){
		return;
	}

	if(sx1>sx2){tmp=sx1; sx1=sx2; sx2=tmp;}
	if(sy1>sy2){tmp=sy1; sy1=sy2; sy2=tmp;}
	if(dx1>dx2){tmp=dx1; dx1=dx2; dx2=tmp;}
	if(dy1>dy2){tmp=dy1; dy1=dy2; dy2=tmp;}
	_dxlen = dx2 - dx1 + 1;
	_dylen = dy2 - dy1 + 1;
	_sxlen = sx2 - sx1 + 1;
	_sylen = sy2 - sy1 + 1;

#if 0		// ï\é¶ÉGÉäÉA ex1,ey1,ex2,ey2 Ç sx1,sy1,sx2,sy2 Ç≈í≤êÆÇπÇ»Ç†Ç©ÇÒ
		if(_dxlen==_sxlen && _dylen==_sylen){
			if(srx==0 && sry==0){
				if(src_xl==sxl && src_yl==syl){
					index_color_grp_disp(
						dst_p, dst_xl, dst_yl,
						src_p, src_xl, src_yl, sx1, sy1,
						pal, palcnt,
						dx1, dy1,
						clr,
						ex1, ey1, ex2, ey2
					);
					return;
				}
			}
		}
#endif

	ng_set_eria(dst_xl, dst_yl, &ex1, &ey1, &ex2, &ey2);

	if(
		magnify_param(
			_sxlen, _dxlen,
			sx1, sx2,
			dx1, dx2,
			srx, (srx + sxl - 1),
			ex1, ex2,
			&x_alpha, &sx_free,
			&dx_free, &x_count
		) == 0
	){
		return;
	}
	if(
		magnify_param(
			_sylen, _dylen,
			sy1, sy2,
			dy1, dy2,
			sry, (sry + syl - 1),
			ey1, ey2,
			&y_alpha, &sy_free,
			&dy_free, &y_count
		) == 0
	){
		return;
	}

	ICMAG_x_alpha = x_alpha;
	ICMAG_y_alpha = y_alpha;

	ICMAG_x_count = x_count;
	ICMAG_y_count = y_count;

	ICMAG_src = src_p + (((sxl    * sy_free) + sx_free));
	ICMAG_dst = dst_p + (((dst_xl * dy_free) + dx_free) << 2);

	ICMAG_src_add_adr = sxl;
	ICMAG_dst_add_adr = dst_xl << 2;

	ICMAG_sxl = _sxlen;
	ICMAG_syl = _sylen;
	ICMAG_dxl = _dxlen;
	ICMAG_dyl = _dylen;

	IC_pal = pal;

	GRPF_clr4 = (clr << 2);
	GRPF_clrR = 255 - clr;
	CTPCLR = *(Gclr_table_adr_p + clr);

	if(clr == 255){
		icmag_normal();
	}
	else{
		icmag_normal_clr();
	}
}









