#include	"pch.h"
#include	"ng_macro.h"





/*========================================================================================
	“ü‚ê‘Ö‚¦
void true_color_grp_disp_xchg(
	UC *dst_p, int dst_xl, int dst_yl, int x, int y,
	UC *src_p, int src_xl, int src_yl, int x1, int y1, int x2, int y2
);
		IN		‚È‚µ
		OUT		‚È‚µ
----------------------------------------------------------------------------------------*/
void true_color_grp_disp_xchg(
	UC *dst_p, int dst_xl, int dst_yl, int x, int y,
	UC *src_p, int src_xl, int src_yl, int x1, int y1, int x2, int y2
){
	int cntx, cnty;
	int dst_free_xl, src_free_xl;
	int xl, yl;
	int sx1, sy1, sx2, sy2;
	int dx1, dy1, dx2, dy2;
	int tmp;
	UC *dst, *src;

	if(dst_p==0 || src_p==0){
		return;
	}

	sx1 = x1;
	sy1 = y1;
	sx2 = x2;
	sy2 = y2;

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

	xl = sx2 - sx1 + 1;
	yl = sy2 - sy1 + 1;

	dx1 = x;
	dy1 = y;
	dx2 = x + xl - 1;
	dy2 = y + yl - 1;

	if(dx1>=dst_xl || dx2<0){
		return;
	}
	if(dy1>=dst_yl || dy2<0){
		return;
	}

	if(dx1 < 0){
		dx1 = 0;
	}
	if(dx2 >= dst_xl){
		dx2 = dst_xl - 1;
	}
	if(dy1 < 0){
		dy1 = 0;
	}
	if(dy2 >= dst_yl){
		dy2 = dst_yl - 1;
	}

	cntx = dx2 - dx1 + 1;
	cnty = dy2 - dy1 + 1;

	if(cntx > xl){
		cntx = xl;
	}
	if(cnty > yl){
		cnty = yl;
	}

	src_free_xl = (src_xl - cntx) << 2;
	dst_free_xl = (dst_xl - cntx) << 2;

	src = src_p + (((src_xl * sy1) + sx1) << 2);
	dst = dst_p + (((dst_xl * dy1) + dx1) << 2);

	_asm{
		mov		ecx, cnty
		mov		esi, src
		mov		edi, dst
		mov		ebx, src_free_xl
		mov		edx, dst_free_xl
	loop_y:
		push	ecx
		mov		ecx, cntx
	loop_x:
		mov		eax, [esi]
		xchg	eax, [edi]
		mov		[esi], eax 
		add		esi, 4
		add		edi, 4
		loop	loop_x
		add		esi, ebx
		add		edi, edx
		pop		ecx
		loop	loop_y
	}
}




