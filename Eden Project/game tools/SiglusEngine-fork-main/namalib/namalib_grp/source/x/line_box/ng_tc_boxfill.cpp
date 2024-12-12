#include	"pch.h"
#include	"ng_macro.h"



// 通常表示
void tcbf_normal(void){

	_asm{
		cld
		mov		edi, LINEF_d_buf
		mov		eax, LINEF_pal
		mov		edx, LINEF_xp

		mov		ecx, LINEF_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, LINEF_xl
		rep		stosd

		pop		ecx
		add		edi, edx
		loop	flineh_loop_y
	}
}



// 透明表示
void tcbf_clr(void){

	_asm{
		cld
		mov		edi, LINEF_d_buf
		mov		esi, CTPCLR
		mov		edx, LINEF_xp

		mov		ecx, LINEF_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, LINEF_xl
	flineh_loop_x:

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

		add		edi, 4
		loop	flineh_loop_x

		pop		ecx
		add		edi, edx
		loop	flineh_loop_y
	}
}



// タイル通常表示
void tcbf_normal_tile(void){

	_asm{
		cld
		mov		edi, LINEF_d_buf
		mov		edx, LINEF_xp

		mov		ecx, LINEF_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, LINEF_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, LINEF_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		LINEF_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, LINEF_t_adr
		add		eax, LINEF_t_xl
		cmp		eax, LINEF_t_adr_max
		jl		flineh_jl2
		mov		eax, LINEF_t_adr_org
	flineh_jl2:
		mov		LINEF_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// タイル透明表示
void tcbf_clr_tile(void){

	_asm{
		cld
		mov		edi, LINEF_d_buf
		mov		edx, LINEF_xp

		mov		ecx, LINEF_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, LINEF_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, LINEF_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		LINEF_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, LINEF_t_adr
		add		eax, LINEF_t_xl
		cmp		eax, LINEF_t_adr_max
		jl		flineh_jl2
		mov		eax, LINEF_t_adr_org
	flineh_jl2:
		mov		LINEF_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



/*========================================================================================
	ボックスフィル表示
void true_color_boxfill(
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
void true_color_boxfill(
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
	int tmp;

	int xl, yl;
	int xp;				// 座標の増分

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
	if(x1 > x2){
		tmp = x1;
		x1  = x2;
		x2  = tmp;
	}
	if(y1 > y2){
		tmp = y1;
		y1  = y2;
		y2  = tmp;
	}

	if(x1 > ex2){
		return;
	}
	if(x2 < ex1){
		return;
	}
	if(y1 > ey2){
		return;
	}
	if(y2 < ey1){
		return;
	}

	if(x1 < ex1){
		x1 = ex1;
	}
	if(x2 > ex2){
		x2 = ex2;
	}
	if(y1 < ey1){
		y1 = ey1;
	}
	if(y2 > ey2){
		y2 = ey2;
	}

// 前処理
	xl = (x2 - x1 + 1);
	yl = (y2 - y1 + 1);
	xp = d_xl - xl;

// アドレス算出
	if(screen_mod == 0){
		d_buf += ((x1 + (d_xl * y1)) << 2);
	}
	else{
		xp = -(xl + d_xl);
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
tcbf_normal();
		}
		else{
// タイル通常表示
tcbf_normal_tile();
		}
	}
	else{
		if(tile==0 || t_add==255){
// 透明表示
tcbf_clr();
		}
		else{
// タイル透明表示
tcbf_clr_tile();
		}
	}
}



