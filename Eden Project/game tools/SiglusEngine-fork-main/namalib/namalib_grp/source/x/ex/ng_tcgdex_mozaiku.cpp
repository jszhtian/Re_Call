#include	"pch.h"
#include	"ng_macro.h"




/*========================================================================================
	モザイク表示
void true_color_grp_disp_mozaiku(
	UC *dst_p, int dst_xl, int dst_yl, int dx, int dy,
	UC *src_p, int src_xl, int src_yl, int sx1, int sy1, int sx2, int sy2,
	UC rate, UC clr
);
		IN		なし
		OUT		なし
----------------------------------------------------------------------------------------*/

static int TCMOZ_sxl;
static int TCMOZ_syl;
static int TCMOZ_dxl;
static int TCMOZ_dyl;
static UC *TCMOZ_dst;
static UC *TCMOZ_src;
static UC *TCMOZ_back;
static int TCMOZ_dst_free_xl;
static int TCMOZ_src_free_xl;
static int TCMOZ_dst_need_xl;

static int TCMOZ_limit;
static int TCMOZ_add;

//-----------------------------------------------------------------------------------------------

#define		TCMOZ_NORMAL_MACRO		\
__asm	mov		eax, [esi]			\
__asm	mov		[edi],eax

void tcmoz_normal(void){
	_asm{
		mov		esi, TCMOZ_src
		mov		edi, TCMOZ_dst
		mov		ecx, TCMOZ_dyl
		xor		edx, edx
		mov		ebx, esi
	a0_loop_y:

		push	ecx
		push	edx
		push	ebx
		push	esi

		mov		ecx, TCMOZ_dxl
		xor		edx, edx
		mov		ebx, esi
	a0_loop_x:
		TCMOZ_NORMAL_MACRO
		add		edi, 4
		add		ebx, 4
		add		edx, TCMOZ_add
		cmp		edx, TCMOZ_limit
		jl		a0_step_x1
		sub		edx, TCMOZ_limit
		mov		esi, ebx
	a0_step_x1:
		dec		ecx
		jz		a0_loop_x_end
		jmp		a0_loop_x
	a0_loop_x_end:

		pop		esi
		pop		ebx
		pop		edx
		pop		ecx

		add		ebx, TCMOZ_src_free_xl
		add		edi, TCMOZ_dst_free_xl
		add		edx, TCMOZ_add
		cmp		edx, TCMOZ_limit
		jl		a0_step_y1
		sub		edx, TCMOZ_limit
		mov		esi, ebx
	a0_step_y1:
		dec		ecx
		jz		a0_loop_y_end
		jmp		a0_loop_y
	a0_loop_y_end:
	}
}

//-----------------------------------------------------------------------------------------------

#define		TCMOZ_CLR_MACRO			\
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
__asm	mov		[edi+2], al

void tcmoz_clr(void){
	_asm{
		mov		esi, TCMOZ_src
		mov		edi, TCMOZ_dst
		mov		ecx, TCMOZ_dyl
		xor		edx, edx
		mov		ebx, esi
	a0_loop_y:

		push	ecx
		push	edx
		push	ebx
		push	esi

		mov		ecx, TCMOZ_dxl
		xor		edx, edx
		mov		ebx, esi
	a0_loop_x:
		push	ebx
		TCMOZ_CLR_MACRO
		pop		ebx
		add		edi, 4
		add		ebx, 4
		add		edx, TCMOZ_add
		cmp		edx, TCMOZ_limit
		jl		a0_step_x1
		sub		edx, TCMOZ_limit
		mov		esi, ebx
	a0_step_x1:
		dec		ecx
		jz		a0_loop_x_end
		jmp		a0_loop_x
	a0_loop_x_end:

		pop		esi
		pop		ebx
		pop		edx
		pop		ecx

		add		ebx, TCMOZ_src_free_xl
		add		edi, TCMOZ_dst_free_xl
		add		edx, TCMOZ_add
		cmp		edx, TCMOZ_limit
		jl		a0_step_y1
		sub		edx, TCMOZ_limit
		mov		esi, ebx
	a0_step_y1:
		dec		ecx
		jz		a0_loop_y_end
		jmp		a0_loop_y
	a0_loop_y_end:
	}
}

//-----------------------------------------------------------------------------------------------

static int mozaiku_rate[] = {
	85344, 81375, 77531, 73810, 70210, 66729, 63365, 60116, 56980, 53955, 
	51039, 48230, 45526, 42925, 40425, 38024, 35720, 33511, 31395, 29370, 
	27434, 25585, 23821, 22140, 20540, 19019, 17575, 16206, 14910, 13685, 
	12529, 11440, 10416,  9455,  8555,  7714,  6930,  6201,  5525,  4900, 
	 4324,  3795,  3311,  2870,  2470,  2109,  1785,  1496,  1240,  1015, 
	  819,   650,   506,   385,   285,   204,   140,    91,    55,    30, 
	   14,     5,     1,     0,
};

void true_color_grp_disp_mozaiku(
	UC *dst_p, int dst_xl, int dst_yl, int dx, int dy,
	UC *src_p, int src_xl, int src_yl, int sx1, int sy1, int sx2, int sy2,
	UC rate, UC clr
){
	int dx1, dy1, dx2, dy2;
	int tmp, ratetmp;

	if(dst_p==0 || src_p==0 || clr==0){
		return;
	}

	if(dst_p==0 || src_p==0){
		return;
	}

	if(sx1 > sx2){
		tmp = sx1;
		sx1 = sx2;
		sx2 = tmp;
	}
	if(sy1 > sy2){
		tmp = sy1;
		sy1 = sy2;
		sy2 = tmp;
	}

	if(sx1>=src_xl || sx2<0){
		return;
	}
	if(sy1>=src_yl || sy2<0){
		return;
	}

	if(sx1 < 0){
		sx1 = 0;
	}
	if(sx2 >= src_xl){
		sx2 = src_xl - 1;
	}
	if(sy1 < 0){
		sy1 = 0;
	}
	if(sy2 >= src_yl){
		sy2 = src_yl - 1;
	}

	dx1 = dx;
	dy1 = dy;
	dx2 = dx + (sx2 - sx1);
	dy2 = dy + (sy2 - sy1);

	if(dx1 > dx2){
		tmp = dx1;
		dx1 = dx2;
		dx2 = tmp;
	}
	if(dy1 > dy2){
		tmp = dy1;
		dy1 = dy2;
		dy2 = tmp;
	}

	if(dx1>=dst_xl || dx2<0){
		return;
	}
	if(dy1>=dst_yl || dy2<0){
		return;
	}

	if(dx1 < 0){
		sx1 = sx1 - dx1;
		dx1 = 0;
	}
	if(dx2 >= dst_xl){
		tmp = dst_xl - 1;
		sx1 = sx1 - (dx1 - tmp);
		dx2 = tmp;
	}
	if(dy1 < 0){
		sy1 = sy1 - dy1;
		dy1 = 0;
	}
	if(dy2 >= dst_yl){
		tmp = dst_yl - 1;
		sy1 = sy1 - (dy1 - tmp);
		dy2 = tmp;
	}

	TCMOZ_sxl = sx2 - sx1 + 1;
	TCMOZ_syl = sy2 - sy1 + 1;
	if(TCMOZ_sxl<=0 || TCMOZ_syl<=0){
		return;
	}

	TCMOZ_dxl = dx2 - dx1 + 1;
	TCMOZ_dyl = dy2 - dy1 + 1;
	if(TCMOZ_dxl<=0 || TCMOZ_dyl<=0){
		return;
	}

	if(rate == 0){
		true_color_grp_disp(
			dst_p, dst_xl, dst_yl,
			src_p, src_xl, src_yl, sx1, sy1, 0,
			dx1, dy1,
			dx1, dy1, dx2, dy2,
			0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, clr, 0
		);
	}
	else{
		if(TCMOZ_sxl >= TCMOZ_syl){
			TCMOZ_limit = TCMOZ_sxl;
		}
		else{
			TCMOZ_limit = TCMOZ_syl;
		}

		TCMOZ_src_free_xl = src_xl << 2;

		TCMOZ_dst_free_xl = (dst_xl - TCMOZ_dxl) << 2;
		TCMOZ_dst_need_xl = TCMOZ_dxl << 2;

		TCMOZ_src = src_p + (((src_xl * sy1) + sx1) << 2);
		TCMOZ_dst = dst_p + (((dst_xl * dy1) + dx1) << 2);

		GRPF_clr4 = (clr << 2);
		GRPF_clrR = 255 - clr;
		CTPCLR = *(Gclr_table_adr_p + clr);

		if(TCMOZ_limit <= 80){
			if(TCMOZ_limit <= 2){
				TCMOZ_limit <<= 13;
			}
			else if(TCMOZ_limit <= 5){
				TCMOZ_limit <<= 12;
			}
			else if(TCMOZ_limit <= 10){
				TCMOZ_limit <<= 11;
			}
			else if(TCMOZ_limit <= 20){
				TCMOZ_limit <<= 10;
			}
			else if(TCMOZ_limit <= 40){
				TCMOZ_limit <<= 9;
			}
			else if(TCMOZ_limit <= 80){
				TCMOZ_limit <<= 8;
			}
		}
		else if(TCMOZ_limit <= 640){
			if(TCMOZ_limit <= 160){
				TCMOZ_limit <<= 7;
			}
			else if(TCMOZ_limit <= 320){
				TCMOZ_limit <<= 6;
			}
			else if(TCMOZ_limit <= 640){
				TCMOZ_limit <<= 5;
			}
		}
		else{
			if(TCMOZ_limit <= 1280){
				TCMOZ_limit <<= 4;
			}
			else if(TCMOZ_limit <= 2560){
				TCMOZ_limit <<= 3;
			}
			else if(TCMOZ_limit <= 5120){
				TCMOZ_limit <<= 2;
			}
			else if(TCMOZ_limit <= 10240){
				TCMOZ_limit <<= 1;
			}
		}

		ratetmp = mozaiku_rate[rate / 4];

		TCMOZ_add  = (TCMOZ_limit * ratetmp) / mozaiku_rate[0];

		if(clr == 255){
			tcmoz_normal();
		}
		else{
			tcmoz_clr();
		}
	}
}




