#include	"pch.h"

// ****************************************************************
// ラインバッファリングマクロ

#define		__LINE_BUFFRING_HORIZONTAL(com00, loop_lavel) \
__asm				mov		eax, y \
__asm				mov		ebx, x \
__asm				mov		edi, dp \
__asm				mov		ecx, xl \
__asm				inc		ecx \
__asm			loop_lavel: \
__asm				mov		[edi], ebx \
__asm				add		edi, 4 \
__asm				mov		[edi], eax \
__asm				add		edi, 4 \
__asm				com00	ebx \
__asm				loop	loop_lavel \
__asm				mov		dp, edi

#define		__LINE_BUFFRING_VERTICAL(com00, loop_lavel) \
__asm				mov		eax, x \
__asm				mov		ebx, y \
__asm				mov		edi, dp \
__asm				mov		ecx, yl \
__asm				inc		ecx \
__asm			loop_lavel: \
__asm				mov		[edi], eax \
__asm				add		edi, 4 \
__asm				mov		[edi], ebx \
__asm				add		edi, 4 \
__asm				com00	ebx \
__asm				loop	loop_lavel \
__asm				mov		dp, edi

#define		__LINE_BUFFRING_X(com00, com01, loop_lavel, step_lavel) \
__asm				mov		eax, ENs \
__asm				mov		ebx, ENa \
__asm				mov		esi, y \
__asm				mov		edx, x \
__asm				mov		edi, dp \
__asm				mov		ecx, xl \
__asm				inc		ecx \
__asm			loop_lavel: \
__asm				mov		[edi], edx \
__asm				add		edi, 4 \
__asm				mov		[edi], esi \
__asm				add		edi, 4 \
__asm				com00	edx \
__asm				add		eax, ebx \
__asm				cmp		eax, 0 \
__asm				jl		step_lavel \
__asm				com01	esi \
__asm				sub		eax, ENh \
__asm			step_lavel: \
__asm				loop	loop_lavel \
__asm				mov		dp, edi

#define		__LINE_BUFFRING_Y(com00, com01, loop_lavel, step_lavel) \
__asm				mov		eax, ENs \
__asm				mov		ebx, ENa \
__asm				mov		esi, x \
__asm				mov		edx, y \
__asm				mov		edi, dp \
__asm				mov		ecx, yl \
__asm				inc		ecx \
__asm			loop_lavel: \
__asm				mov		[edi], esi \
__asm				add		edi, 4 \
__asm				mov		[edi], edx \
__asm				add		edi, 4 \
__asm				com00	edx \
__asm				add		eax, ebx \
__asm				cmp		eax, 0 \
__asm				jl		step_lavel \
__asm				com01	esi \
__asm				sub		eax, ENh \
__asm			step_lavel: \
__asm				loop	loop_lavel \
__asm				mov		dp, edi

// ****************************************************************
// Ｙ軸基準のラインバッファリングマクロ
#define		__Y_LINE_BUFFRING_X1(com00, com01, loop_lavel, step_lavel) \
__asm				mov		eax, ENs \
__asm				mov		ebx, ENa \
__asm				mov		esi, x \
__asm				mov		edx, y \
__asm				mov		edi, dp \
__asm				mov		ecx, xl \
__asm			loop_lavel: \
__asm				add		eax, ebx \
__asm				cmp		eax, 0 \
__asm				jl		step_lavel \
__asm				mov		[edi], esi \
__asm				add		edi, 4 \
__asm				mov		[edi], edx \
__asm				add		edi, 4 \
__asm				com00	edx \
__asm				sub		eax, ENh \
__asm			step_lavel: \
__asm				com01	esi \
__asm				loop	loop_lavel \
__asm				mov		[edi], esi \
__asm				add		edi, 4 \
__asm				mov		[edi], edx \
__asm				add		edi, 4 \
__asm				mov		dp, edi

#define		__Y_LINE_BUFFRING_X2(com00, com01, loop_lavel, step_lavel) \
__asm				mov		eax, ENs \
__asm				mov		ebx, ENa \
__asm				mov		esi, x \
__asm				mov		edx, y \
__asm				mov		edi, dp \
__asm				mov		ecx, xl \
__asm				mov		[edi], esi \
__asm				add		edi, 4 \
__asm				mov		[edi], edx \
__asm				add		edi, 4 \
__asm			loop_lavel: \
__asm				com00	esi \
__asm				add		eax, ebx \
__asm				cmp		eax, 0 \
__asm				jl		step_lavel \
__asm				com01	edx \
__asm				sub		eax, ENh \
__asm				mov		[edi], esi \
__asm				add		edi, 4 \
__asm				mov		[edi], edx \
__asm				add		edi, 4 \
__asm			step_lavel: \
__asm				loop	loop_lavel \
__asm				mov		dp, edi

// ****************************************************************
// ラインバッファリング
DWORD Cnamalib_grp::line_buffering(int *line_buf_p, int line_buf_max, int x1, int y1, int x2, int y2)
{
	int x, y, xl, yl, xp, yp, ENs, ENa, ENh;
	int *dp;

	// 前処理
	x = x1;		xp = 1;		xl = x2 - x1;	if (xl < 0)	{	xl = -xl;	xp = -xp;	}
	y = y1;		yp = 1;		yl = y2 - y1;	if (yl < 0)	{	yl = -yl;	yp = -yp;	}
	if (xl > yl)	{	if (xl >= line_buf_max)	{	return(0);	}	}
	else			{	if (yl >= line_buf_max)	{	return(0);	}	}
	dp = line_buf_p;

	if (yl == 0)	{
		// 点
		if (xl == 0)	{
			*dp++ = x;		*dp++ = y;
		}
		// 水平線
		else	{
			if (xp < 0)	{	_asm	{	__LINE_BUFFRING_HORIZONTAL(dec, h1_loop)	}	}
			else		{	_asm	{	__LINE_BUFFRING_HORIZONTAL(inc, h2_loop)	}	}
		}
	}
	// 垂直線
	else if (xl == 0)	{
		if (yp < 0)	{	_asm	{	__LINE_BUFFRING_VERTICAL(dec, v1_loop)	}	}
		else		{	_asm	{	__LINE_BUFFRING_VERTICAL(inc, v2_loop)	}	}
	}
	// Ｘ基準線
	else if (xl > yl)	{
		ENs = -xl;
		ENa = yl << 1;
		ENh = xl << 1;
		if (xp < 0)	{
			if (yp < 0)	{	_asm	{	__LINE_BUFFRING_X(dec, dec, x1_loop, x1_step)	}	}
			else		{	_asm	{	__LINE_BUFFRING_X(dec, inc, x2_loop, x2_step)	}	}
		}
		else		{
			if (yp < 0)	{	_asm	{	__LINE_BUFFRING_X(inc, dec, x3_loop, x3_step)	}	}
			else		{	_asm	{	__LINE_BUFFRING_X(inc, inc, x4_loop, x4_step)	}	}
		}
	}
	// Ｙ基準線
	else{
		ENs = -yl;
		ENa = xl << 1;
		ENh = yl << 1;
		if (yp < 0)	{
			if (xp < 0)	{	_asm	{	__LINE_BUFFRING_Y(dec, dec, y1_loop, y1_step)	}	}
			else		{	_asm	{	__LINE_BUFFRING_Y(dec, inc, y2_loop, y2_step)	}	}
		}
		else		{
			if (xp < 0)	{	_asm	{	__LINE_BUFFRING_Y(inc, dec, y3_loop, y3_step)	}	}
			else		{	_asm	{	__LINE_BUFFRING_Y(inc, inc, y4_loop, y4_step)	}	}
		}
	}

	return((DWORD)(((UINT_PTR)dp - (UINT_PTR)line_buf_p) / 8 ));
}

// ****************************************************************
// ラインバッファリング２（変移版）
DWORD Cnamalib_grp::line_buffering2(int *line_buf_p, int line_buf_max, int x1, int y1, int x2, int y2)
{
	int cnt = (int)line_buffering(line_buf_p, line_buf_max, x1, y1, x2, y2);
	if (cnt > 0)	{	pos_buffer_convert(line_buf_p, cnt);	}
	return(cnt);
}

// ****************************************************************
// 座標バッファの変換
void Cnamalib_grp::pos_buffer_convert(int *line_buf_p, int line_buf_cnt)
{
	int i, val_x, val_y, next_val_x, next_val_y;
	int *dp;

	dp = line_buf_p;
	val_x = *dp;	
	val_y = *(dp + 1);
	for (i = 0; i < line_buf_cnt; i++)	{
		next_val_x = *dp;
		next_val_y = *(dp + 1);
		*dp       -= val_x;
		*(dp + 1) -= val_y;
		val_x = next_val_x;
		val_y = next_val_y;
		dp += 2;
	}
}

// ****************************************************************
// Ｙ軸基準のラインバッファリング
DWORD Cnamalib_grp::y_line_buffering(int *line_buf_p, int line_buf_max, int x1, int y1, int x2, int y2, int mod)
{
	int x, y, xl, yl, xp, yp, ENs, ENa, ENh;
	int *dp;

	// 前処理
	x = x1;		xp = 1;		xl = x2 - x1;	if (xl < 0)	{	xl = -xl;	xp = -xp;	}
	y = y1;		yp = 1;		yl = y2 - y1;	if (yl < 0)	{	yl = -yl;	yp = -yp;	}
	if (yl >= line_buf_max)	{	return(0);	}
	dp = line_buf_p;

	if (yl == 0)	{
		// 点
		if (xl == 0)	{
			*dp++ = x;		*dp++ = y;
		}
		// 水平線
		else	{
			if (mod == 0)	{	if (xp < 0)	{	*dp++ = x - xl;	}	else	{	*dp++ = x;		}	}
			else			{	if (xp < 0)	{	*dp++ = x;		}	else	{	*dp++ = x + xl;	}	}
			*dp++ = y;
		}
	}
	// 垂直線
	else if (xl == 0)	{
		if (yp < 0)	{	_asm	{	__LINE_BUFFRING_VERTICAL(dec, v1_loop)	}	}
		else		{	_asm	{	__LINE_BUFFRING_VERTICAL(inc, v2_loop)	}	}
	}
	// Ｘ基準線
	else if (xl > yl)	{
		ENs = -xl;
		ENa = yl << 1;
		ENh = xl << 1;
		if ((mod == 0 && xp < 0) || (mod == 1 && xp > 0))	{
			if (xp < 0)	{
				if (yp < 0)	{	_asm	{	__Y_LINE_BUFFRING_X1(dec, dec, x11_loop, x11_step)	}	}
				else		{	_asm	{	__Y_LINE_BUFFRING_X1(inc, dec, x12_loop, x12_step)	}	}
			}
			else		{
				if (yp < 0)	{	_asm	{	__Y_LINE_BUFFRING_X1(dec, inc, x13_loop, x13_step)	}	}
				else		{	_asm	{	__Y_LINE_BUFFRING_X1(inc, inc, x14_loop, x14_step)	}	}
			}
		}
		else{
			if (xp < 0)	{
				if (yp < 0)	{	_asm	{	__Y_LINE_BUFFRING_X2(dec, dec, x21_loop, x21_step)	}	}
				else		{	_asm	{	__Y_LINE_BUFFRING_X2(dec, inc, x22_loop, x22_step)	}	}
			}
			else		{
				if (yp < 0)	{	_asm	{	__Y_LINE_BUFFRING_X2(inc, dec, x23_loop, x23_step)	}	}
				else		{	_asm	{	__Y_LINE_BUFFRING_X2(inc, inc, x24_loop, x24_step)	}	}
			}
		}
	}
	// Ｙ基準線
	else{
		ENs = -yl;
		ENa = xl << 1;
		ENh = yl << 1;
		if (yp < 0)	{
			if (xp < 0)	{	_asm	{	__LINE_BUFFRING_Y(dec, dec, y1_loop, y1_step)	}	}
			else		{	_asm	{	__LINE_BUFFRING_Y(dec, inc, y2_loop, y2_step)	}	}
		}
		else		{
			if (xp < 0)	{	_asm	{	__LINE_BUFFRING_Y(inc, dec, y3_loop, y3_step)	}	}
			else		{	_asm	{	__LINE_BUFFRING_Y(inc, inc, y4_loop, y4_step)	}	}
		}
	}

	return((DWORD)(((UINT_PTR)dp - (UINT_PTR)line_buf_p) / 8 ));
}

// ****************************************************************
// クリッピング
// IN	int *x1 - *y2 : 座標へのポインター
// IN	int ex1 - ey2 : クリップ範囲
// OUT	false=不可視 true=クリップされました
bool Cnamalib_grp::box_cliping(int *x1, int *y1, int *x2, int *y2, int ex1, int ey1, int ex2, int ey2)
{
	DWORD gex1, gey1, gex2, gey2;	// 下駄エリア
	DWORD tx1, ty1, tx2, ty2;		// テンポ座標
	DWORD gx1, gy1, gx2, gy2;		// 下駄座標
	RECT clip_coad1, clip_coad2;
	DWORD tmp;

	// クリップ範囲に下駄を履かせてマイナス値を除外する
	gex1 = ex1 + 0x80000000;
	gey1 = ey1 + 0x80000000;
	gex2 = ex2 + 0x80000000;
	gey2 = ey2 + 0x80000000;

	// クリッピングコード
	clip_coad1.left   = 0;
	clip_coad1.right  = 0;
	clip_coad1.top    = 0;
	clip_coad1.bottom = 0;
	if (*x1 < ex1)	{	clip_coad1.left = 1;	}		else if (*x1 > ex2)	{	clip_coad1.right  = 1;	}
	if (*y1 < ey1)	{	clip_coad1.top  = 1;	}		else if (*y1 > ey2)	{	clip_coad1.bottom = 1;	}
	clip_coad2.left   = 0;
	clip_coad2.right  = 0;
	clip_coad2.top    = 0;
	clip_coad2.bottom = 0;
	if (*x2 < ex1)	{	clip_coad2.left = 1;	}		else if (*x2 > ex2)	{	clip_coad2.right  = 1;	}
	if (*y2 < ey1)	{	clip_coad2.top  = 1;	}		else if (*y2 > ey2)	{	clip_coad2.bottom = 1;	}

	// 完全不可視判定（同一側に点が存在する）
	if (
		(clip_coad1.left   && clip_coad2.left  ) ||
		(clip_coad1.right  && clip_coad2.right ) ||
		(clip_coad1.top    && clip_coad2.top   ) ||
		(clip_coad1.bottom && clip_coad2.bottom)
	){
		return false;
	}

	// 下駄を履かせる
	gx1 = tx1 = *x1 + 0x80000000;
	gy1 = ty1 = *y1 + 0x80000000;
	gx2 = tx2 = *x2 + 0x80000000;
	gy2 = ty2 = *y2 + 0x80000000;


	// １点目可視判定
	if (clip_coad1.left || clip_coad1.right || clip_coad1.top || clip_coad1.bottom)	{
		while (1)	{
			if (clip_coad1.left)		{	tmp = _box_cliping_sub(gx1, gy1, gx2, gy2, gex1);	if (tmp >= gey1 && tmp <= gey2)	{	tx1 = gex1; ty1 = tmp;  break;	}	}
			else if (clip_coad1.right)	{	tmp = _box_cliping_sub(gx2, gy2, gx1, gy1, gex2);	if (tmp >= gey1 && tmp <= gey2)	{	tx1 = gex2; ty1 = tmp;  break;	}	}
			if (clip_coad1.top)			{	tmp = _box_cliping_sub(gy1, gx1, gy2, gx2, gey1);	if (tmp >= gex1 && tmp <= gex2)	{	tx1 = tmp;  ty1 = gey1; break;	}	}
			else if (clip_coad1.bottom)	{	tmp = _box_cliping_sub(gy2, gx2, gy1, gx1, gey2);	if (tmp >= gex1 && tmp <= gex2)	{	tx1 = tmp;  ty1 = gey2; break;	}	}
			return false;
		}
	}

	// ２点目可視判定
	if(clip_coad2.left || clip_coad2.right || clip_coad2.top || clip_coad2.bottom){
		while(1){
			if (clip_coad2.left)		{	tmp = _box_cliping_sub(gx2, gy2, gx1, gy1, gex1);	if (tmp >= gey1 && tmp <= gey2)	{	tx2 = gex1; ty2=tmp;  break;	}	}
			else if (clip_coad2.right)	{	tmp = _box_cliping_sub(gx1, gy1, gx2, gy2, gex2);	if (tmp >= gey1 && tmp <= gey2)	{	tx2 = gex2; ty2=tmp;  break;	}	}
			if (clip_coad2.top)			{	tmp = _box_cliping_sub(gy2, gx2, gy1, gx1, gey1);	if (tmp >= gex1 && tmp <= gex2)	{	tx2 = tmp;  ty2=gey1; break;	}	}
			else if (clip_coad2.bottom)	{	tmp = _box_cliping_sub(gy1, gx1, gy2, gx2, gey2);	if (tmp >= gex1 && tmp <= gex2)	{	tx2 = tmp;  ty2=gey2; break;	}	}
			return false;
		}
	}

	// 下駄を脱がせる
	*x1 = tx1 - 0x80000000;
	*y1 = ty1 - 0x80000000;
	*x2 = tx2 - 0x80000000;
	*y2 = ty2 - 0x80000000;

	return true;
}

// ****************************************************************
// クリッピングＳＵＢ関数
// ≪呼び出し条件≫
// 第１引数　<=　第３引数
// 第２引数、第４引数は大小関係無し
// 負数は取り扱えない（このルーチンでは事前に下駄を履かせてある）
// 引数の名前はあまり意味がない（ "DWORD x1" が１点目のＸ座標とは限らない）
// IN	DWORD x1 - y2 : 
// 		DWORD clip : クリップ座標
// OUT	DWORD もう一方の座標
DWORD Cnamalib_grp::_box_cliping_sub(DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD clip)
{
	_asm{
		mov		eax, x1
		mov		ebx, x2
		mov		ecx, y1
		mov		edx, y2

		mov		esi, eax
		add		esi, ebx
		rcr		esi, 1			// キャリーを含むシフト
		mov		edi, ecx
		add		edi, edx
		rcr		edi, 1			// キャリーを含むシフト

	cliping_loop:
		cmp		esi, clip
		je		cliping_end
		ja		cliping_step

		// 中点がクリップ座標より小さい
		mov		eax, esi		// 小さい座標の方を代える
		add		esi, ebx
		rcr		esi, 1			// キャリーを含むシフト
		cmp		esi, eax
		je		cliping_end
		mov		ecx, edi
		add		edi, edx
		rcr		edi, 1			// キャリーを含むシフト
		jmp		cliping_loop

		// 中点がクリップ座標より大きい
	cliping_step:
		mov		ebx, esi		// 小さい座標の方を代える
		add		esi, eax
		rcr		esi, 1			// キャリーを含むシフト
		cmp		esi, ebx
		je		cliping_end
		mov		edx, edi
		add		edi, ecx
		rcr		edi, 1			// キャリーを含むシフト
		jmp		cliping_loop

	cliping_end:
		mov		x1, edi
	}

	return(x1);

}

