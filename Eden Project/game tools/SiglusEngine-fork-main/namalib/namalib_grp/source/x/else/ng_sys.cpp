#define	expand_hensuu_np_pal_data		1
#define	expand_hensuu_ng_data			1
#define	system_liblary_nolink			1
#include	"sys.h"
#include	"pch.h"



/*=============================================================================
	エリア設定
void ng_set_eria(int xl, int yl, int *x1, int *y1, int *x2, int *y2);
		IN		int xl : ＸＬ
				int yl : ＹＬ
				int x1-y2 : エリア
		OUT		なし
-----------------------------------------------------------------------------*/
void ng_set_eria(int xl, int yl, int *x1, int *y1, int *x2, int *y2){
	int tmp;
	if(*x1==-1 && *y1==-1 && *x2==-1 && *y2==-1){
		*x1 = 0;
		*y1 = 0;
		*x2 = xl - 1;
		*y2 = yl - 1;
	}
	else{
		if(*x1 > *x2){
			tmp = *x1;
			*x1 = *x2;
			*x2 = tmp;
		}
		if(*y1 > *y2){
			tmp = *y1;
			*y1 = *y2;
			*y2 = tmp;
		}
		if(*x1 < 0){*x1 = 0;}
		if(*y1 < 0){*y1 = 0;}
		if(*x2 >= xl){*x2 = xl - 1;}
		if(*y2 >= yl){*y2 = yl - 1;}
	}
}



/*========================================================================================
	拡大縮小パラメータ
int magnify_param(
	int _sxlen, int _dxlen,
	int sx1, int sx2,
	int dx1, int dx2,
	int _sx1, int _sx2,
	int ex1, int ex2,
	int *ret_x_alpha, int *ret_sx_free,
	int *ret_dx_free, int *ret_x_count
);
		IN		なし
		OUT		なし
----------------------------------------------------------------------------------------*/
int magnify_param(
	int _sxlen, int _dxlen,
	int sx1, int sx2,
	int dx1, int dx2,
	int _sx1, int _sx2,
	int ex1, int ex2,
	int *ret_x_alpha, int *ret_sx_free,
	int *ret_dx_free, int *ret_x_count
){
	int tmp, cnt;
	int i;

	int x_alpha;
	int _dx1, _dx2;

	int disp_flag;

	int sx_free;
	int dx_free;
	int x_count;

// ソースデータ外
	if(_sx1 > sx2){
		return(0);
	}
	if(_sx2 < sx1){
		return(0);
	}

// 転送エリア外
	if(dx1 > ex2){
		return(0);
	}
	if(dx2 < ex1){
		return(0);
	}

// サイズが同じ
	if(_sxlen == _dxlen){

		_dx1 = dx1 + (_sx1 - sx1);

	// 転送エリア外
		if(_dx1 > ex2){
			return(0);
		}
		_dx2 = _dx1 + (_sx2 - _sx1);
		if(_dx2 < ex1){
			return(0);
		}

		if(dx1 > ex1){
			ex1 = dx1;
		}
		if(dx2 < ex2){
			ex2 = dx2;
		}

		sx_free = -1;
		cnt = _sx2 - _sx1 + 1;
		for(i=0; i<cnt; i++){

			if(_dx1>=ex1 && _dx1<=ex2){
				if(sx_free == -1){
					sx_free = i;
					dx_free = _dx1;
				}
				x_count = i;
			}

			if(_dx1 == ex2){
				break;
			}

			_dx1++;
		}

	// １ドットも描画していない
		if(sx_free == -1){
			return(0);
		}

		x_count = (x_count + 1) - sx_free;

		*ret_x_alpha = 0;
		*ret_sx_free = sx_free;
		*ret_dx_free = dx_free;
		*ret_x_count = x_count;
	}
//----------------------------------------------------------------------
// ソースデータ基準
	else if(_sxlen > _dxlen){

		if(sx1 < _sx1){
			cnt     = _sx1 - sx1;
			_dx1    = dx1;
			x_alpha = 0;
			for(i=0; i<cnt; i++){
				disp_flag = 0;
				x_alpha += _dxlen;
				if(x_alpha >= _sxlen){
					x_alpha -= _sxlen;
					disp_flag = 1;
					_dx1++;
				}
			}
		}
		else if(sx1 > _sx1){
			cnt     = sx1 - _sx1;
			_dx1    = dx1 - 1;
			x_alpha = _sxlen;
			for(i=0; i<cnt; i++){
				disp_flag = 0;
				x_alpha -= _dxlen;
				if(x_alpha <= 0){
					x_alpha += _sxlen;
					disp_flag = 1;
					_dx1--;
				}
			}
		}
		else{
			_dx1    = dx1;
			x_alpha = 0;
			disp_flag = 1;
		}

	// 転送エリア外
		if(_dx1 > ex2){
			return(0);
		}
		cnt  = _sx2 - _sx1 + 2;
		tmp  = x_alpha + (_dxlen * cnt);
		_dx2 = _dx1 + (tmp / _sxlen);
		if(_dx2 < ex1){
			return(0);
		}

		if(dx1 > ex1){
			ex1 = dx1;
		}
		if(dx2 < ex2){
			ex2 = dx2;
		}

		sx_free = -1;
		cnt = _sx2 - _sx1 + 1;
		for(i=0; i<cnt; i++){

			if(disp_flag == 1){
				if(_dx1>=ex1 && _dx1<=ex2){
					if(sx_free == -1){
						*ret_x_alpha = x_alpha;
						sx_free = i;
						dx_free = _dx1;
					}
					x_count = i;
				}
				disp_flag = 0;
			}

			if(_dx1 == ex2){
				break;
			}

			x_alpha += _dxlen;
			if(x_alpha >= _sxlen){
				x_alpha -= _sxlen;
				_dx1++;
				disp_flag = 1;
			}
		}

	// １ドットも描画していない
		if(sx_free == -1){
			return(0);
		}

		x_count = (x_count + 1) - sx_free;

		*ret_sx_free = sx_free;
		*ret_dx_free = dx_free;
		*ret_x_count = x_count;
	}
//----------------------------------------------------------------------
// 転送先基準
	else{

		if(dx1 < ex1){
			cnt     = ex1 - dx1;
			tmp     = _sxlen * cnt;
			sx1     = sx1 + (tmp / _dxlen);
			x_alpha = tmp % _dxlen;
			dx1     = ex1;
		}
		else{
			x_alpha = 0;
		}

		if(dx2 > ex2){
			dx2 = ex2;
		}

	// ソースデータ外
		if(sx1 > _sx2){
			return(0);
		}
		cnt = dx2 - dx1 + 2;
		tmp = x_alpha + (_sxlen * cnt);
		sx2 = sx1 + (tmp / _dxlen);
		if(sx2 < _sx1){
			return(0);
		}

		dx_free = -1;
		cnt = dx2 - dx1 + 1;
		for(i=0; i<cnt; i++){

			if(sx1>=_sx1 && sx1<=_sx2){
				if(dx_free == -1){
					*ret_x_alpha = x_alpha;
					dx_free = i;
					sx_free = sx1;
				}
				x_count = i;
			}

			x_alpha += _sxlen;
			if(x_alpha >= _dxlen){
				x_alpha -= _dxlen;
				sx1++;
			}

			if(sx1 > _sx2){
				break;
			}
		}

	// １ドットも描画していない
		if(dx_free == -1){
			return(0);
		}

		x_count = (x_count + 1) - dx_free;
		dx_free = dx1 + dx_free;
		sx_free = sx_free - _sx1;

		*ret_sx_free = sx_free;
		*ret_dx_free = dx_free;
		*ret_x_count = x_count;
	}

	return(1);
}



/*========================================================================================
	ラインバッファリング
int line_buffring(int *buf, int buf_max, int x1, int y1, int x2, int y2);
		IN		int buf       : バッファ（ＹＬ＊２＋１）
				int (x1 - y2) : 終始点
				int mod       : 0=x_min 1=x_max
		OUT		int : バッファカウント
----------------------------------------------------------------------------------------*/

#define		LINE_BUFFRING_ASM		1

int line_buffring(int *buf, int buf_max, int x1, int y1, int x2, int y2){
	int xl, yl;
	int xp, yp;			// 座標の増分
	int	ENs, ENa, ENh;

	int x, y;
	int *org;
#if !LINE_BUFFRING_ASM
	int i;
#endif

// 前処理
	if((xl=x2-x1)<0){xl=-xl;xp=-1;}
	else{xp=1;}
	if((yl=y2-y1)<0){yl=-yl;yp=-1;}
	else{yp=1;}

	if(xl > yl){
		if(xl >= buf_max){		// xl >= で判定しているのは、xl = x2 - x1 だからです。
			return(0);
		}
	}
	else{
		if(yl >= buf_max){		// yl >= で判定しているのは、yl = y2 - y1 だからです。
			return(0);
		}
	}

	x = x1;
	y = y1;

	org = buf;

	if(yl == 0){



// 点
		if(xl == 0){
			*(buf++) = x;
			*(buf++) = y;
		}



// 水平線
#if LINE_BUFFRING_ASM
		else{
			if(xp < 0){
				_asm{
					mov		eax, y
					mov		ebx, x
					mov		edi, buf
					mov		ecx, xl
					inc		ecx
				xv1_loop:
					mov		[edi], ebx
					add		edi, 4
					mov		[edi], eax
					add		edi, 4
					dec		ebx
					loop	xv1_loop
					mov		buf, edi
				}
			}
			else{
				_asm{
					mov		eax, y
					mov		ebx, x
					mov		edi, buf
					mov		ecx, xl
					inc		ecx
				xv2_loop:
					mov		[edi], ebx
					add		edi, 4
					mov		[edi], eax
					add		edi, 4
					inc		ebx
					loop	xv2_loop
					mov		buf, edi
				}
			}
		}
#else
		else{
			for(i=0; i<=xl; i++){
				*(buf++) = x;
				*(buf++) = y;
				x += xp;
			}
		}
#endif
	}



// 垂直線
#if LINE_BUFFRING_ASM
	else if(xl == 0){
		if(yp < 0){
			_asm{
				mov		eax, x
				mov		ebx, y
				mov		edi, buf
				mov		ecx, yl
				inc		ecx
			yv1_loop:
				mov		[edi], eax
				add		edi, 4
				mov		[edi], ebx
				add		edi, 4
				dec		ebx
				loop	yv1_loop
				mov		buf, edi
			}
		}
		else{
			_asm{
				mov		eax, x
				mov		ebx, y
				mov		edi, buf
				mov		ecx, yl
				inc		ecx
			yv2_loop:
				mov		[edi], eax
				add		edi, 4
				mov		[edi], ebx
				add		edi, 4
				inc		ebx
				loop	yv2_loop
				mov		buf, edi
			}
		}
	}
#else
	else if(xl == 0){
		for(i=0; i<=yl; i++){
			*(buf++) = x;
			*(buf++) = y;
			y += yp;
		}
	}
#endif



// Ｘ基準線
#if LINE_BUFFRING_ASM
	else if(xl > yl){
		ENs = -xl;
		ENa = yl<<1;
		ENh = xl<<1;
		if(xp < 0){
			if(yp < 0){
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, y
					mov		edx, x
					mov		edi, buf
					mov		ecx, xl
					inc		ecx
				x1_loop:
					mov		[edi], edx
					add		edi, 4
					mov		[edi], esi
					add		edi, 4
					dec		edx
					add		eax, ebx
					cmp		eax, 0
					jl		x1_step
					dec		esi
					sub		eax, ENh
				x1_step:
					loop	x1_loop
					mov		buf, edi
				}
			}
			else{
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, y
					mov		edx, x
					mov		edi, buf
					mov		ecx, xl
					inc		ecx
				x2_loop:
					mov		[edi], edx
					add		edi, 4
					mov		[edi], esi
					add		edi, 4
					dec		edx
					add		eax, ebx
					cmp		eax, 0
					jl		x2_step
					inc		esi
					sub		eax, ENh
				x2_step:
					loop	x2_loop
					mov		buf, edi
				}
			}
		}
		else{
			if(yp < 0){
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, y
					mov		edx, x
					mov		edi, buf
					mov		ecx, xl
					inc		ecx
				x3_loop:
					mov		[edi], edx
					add		edi, 4
					mov		[edi], esi
					add		edi, 4
					inc		edx
					add		eax, ebx
					cmp		eax, 0
					jl		x3_step
					dec		esi
					sub		eax, ENh
				x3_step:
					loop	x3_loop
					mov		buf, edi
				}
			}
			else{
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, y
					mov		edx, x
					mov		edi, buf
					mov		ecx, xl
					inc		ecx
				x4_loop:
					mov		[edi], edx
					add		edi, 4
					mov		[edi], esi
					add		edi, 4
					inc		edx
					add		eax, ebx
					cmp		eax, 0
					jl		x4_step
					inc		esi
					sub		eax, ENh
				x4_step:
					loop	x4_loop
					mov		buf, edi
				}
			}
		}
	}
#else
	else if(xl > yl){
		ENs = -xl;
		ENa = yl<<1;
		ENh = xl<<1;
		for(i=0; i<=xl; i++){
			*(buf++) = x;
			*(buf++) = y;
			x   += yp;
			ENs += ENa;
			if(ENs >= 0){
				y  += xp;
				ENs -= ENh;
			}
		}
	}
#endif



// Ｙ基準線
#if LINE_BUFFRING_ASM
	else{
		ENs = -yl;
		ENa = xl<<1;
		ENh = yl<<1;
		if(yp < 0){
			if(xp < 0){
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, yl
					inc		ecx
				y1_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					dec		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y1_step
					dec		esi
					sub		eax, ENh
				y1_step:
					loop	y1_loop
					mov		buf, edi
				}
			}
			else{
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, yl
					inc		ecx
				y2_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					dec		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y2_step
					inc		esi
					sub		eax, ENh
				y2_step:
					loop	y2_loop
					mov		buf, edi
				}
			}
		}
		else{
			if(xp < 0){
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, yl
					inc		ecx
				y3_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					inc		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y3_step
					dec		esi
					sub		eax, ENh
				y3_step:
					loop	y3_loop
					mov		buf, edi
				}
			}
			else{
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, yl
					inc		ecx
				y4_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					inc		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y4_step
					inc		esi
					sub		eax, ENh
				y4_step:
					loop	y4_loop
					mov		buf, edi
				}
			}
		}
	}
#else
	else{
		ENs = -yl;
		ENa = xl<<1;
		ENh = yl<<1;
		for(i=0; i<=yl; i++){
			*(buf++) = x;
			*(buf++) = y;
			y   += yp;
			ENs += ENa;
			if(ENs >= 0){
				x  += xp;
				ENs -= ENh;
			}
		}
	}
#endif

	return( ((UI)buf-(UI)org) >> 3 );		// ( >> 3 ) ＵＩバッファだから

}



/*========================================================================================
	Ｙ軸基準のラインバッファリング
int y_line_buffring(int *buf, int buf_max, int x1, int y1, int x2, int y2, int mod);
		IN		int buf       : バッファ（ＹＬ＊２＋１）
				int (x1 - y2) : 終始点
				int mod       : 0=x_min 1=x_max
		OUT		int : バッファカウント
----------------------------------------------------------------------------------------*/

#define		Y_LINE_BUFFRING_ASM		1

int y_line_buffring(int *buf, int buf_max, int x1, int y1, int x2, int y2, int mod){
	int xl, yl;
	int xp, yp;			// 座標の増分
	int	ENs, ENa, ENh;

	int x, y;
	int *org;
#if !Y_LINE_BUFFRING_ASM
	int i;
#endif

// 前処理
	if((xl=x2-x1)<0){xl=-xl;xp=-1;}
	else{xp=1;}
	if((yl=y2-y1)<0){yl=-yl;yp=-1;}
	else{yp=1;}

	if(yl >= buf_max){		// yl >= で判定しているのは、yl = y2 - y1 だからです。
		return(0);
	}

	x = x1;
	y = y1;

	org = buf;

	if(yl == 0){



// 点
		if(xl == 0){
			*(buf++) = x;
			*(buf++) = y;
		}



// 水平線
		else{
			if(mod == 0){
				if(xp < 0){
					*(buf++) = x - xl;
					*(buf++) = y;
				}
				else{
					*(buf++) = x;
					*(buf++) = y;
				}
			}
			else{
				if(xp < 0){
					*(buf++) = x;
					*(buf++) = y;
				}
				else{
					*(buf++) = x + xl;
					*(buf++) = y;
				}
			}
		}
	}



// 垂直線
#if Y_LINE_BUFFRING_ASM
	else if(xl == 0){
		if(yp < 0){
			_asm{
				mov		eax, x
				mov		ebx, y
				mov		edi, buf
				mov		ecx, yl
				inc		ecx
			v1_loop:
				mov		[edi], eax
				add		edi, 4
				mov		[edi], ebx
				add		edi, 4
				dec		ebx
				loop	v1_loop
				mov		buf, edi
			}
		}
		else{
			_asm{
				mov		eax, x
				mov		ebx, y
				mov		edi, buf
				mov		ecx, yl
				inc		ecx
			v2_loop:
				mov		[edi], eax
				add		edi, 4
				mov		[edi], ebx
				add		edi, 4
				inc		ebx
				loop	v2_loop
				mov		buf, edi
			}
		}
	}
#else
	else if(xl == 0){
		for(i=0; i<=yl; i++){
			*(buf++) = x;
			*(buf++) = y;
			y += yp;
		}
	}
#endif



// Ｘ基準線
#if Y_LINE_BUFFRING_ASM
	else if(xl > yl){
		ENs = -xl;
		ENa = yl<<1;
		ENh = xl<<1;
		if((mod==0 && xp<0) || (mod==1 && xp>0)){
			if(xp < 0){
				if(yp < 0){
					_asm{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, xl
					x1_loop:
						add		eax, ebx
						cmp		eax, 0
						jl		x1_step
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
						dec		edx
						sub		eax, ENh
					x1_step:
						dec		esi			// ＊
						loop	x1_loop
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4	
						mov		buf, edi
					}
				}
				else{
					_asm{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, xl
					x2_loop:
						add		eax, ebx
						cmp		eax, 0
						jl		x2_step
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
						inc		edx
						sub		eax, ENh
					x2_step:
						dec		esi			// ＊
						loop	x2_loop
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4	
						mov		buf, edi
					}
				}
			}
			else{
				if(yp < 0){
					_asm{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, xl
					x3_loop:
						add		eax, ebx
						cmp		eax, 0
						jl		x3_step
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
						dec		edx
						sub		eax, ENh
					x3_step:
						inc		esi			//＊
						loop	x3_loop
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4	
						mov		buf, edi
					}
				}
				else{
					_asm{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, xl
					x4_loop:
						add		eax, ebx
						cmp		eax, 0
						jl		x4_step
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
						inc		edx
						sub		eax, ENh
					x4_step:
						inc		esi			//＊
						loop	x4_loop
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4	
						mov		buf, edi
					}
				}
			}
		}
		else{
			if(xp < 0){
				if(yp < 0){
					_asm{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, xl
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x5_loop:
						dec		esi
						add		eax, ebx
						cmp		eax, 0
						jl		x5_step
						dec		edx
						sub		eax, ENh
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x5_step:
						loop	x5_loop
						mov		buf, edi
					}
				}
				else{
					_asm{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, xl
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x6_loop:
						dec		esi
						add		eax, ebx
						cmp		eax, 0
						jl		x6_step
						inc		edx
						sub		eax, ENh
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x6_step:
						loop	x6_loop
						mov		buf, edi
					}
				}
			}
			else{
				if(yp < 0){
					_asm{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, xl
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x7_loop:
						inc		esi
						add		eax, ebx
						cmp		eax, 0
						jl		x7_step
						dec		edx
						sub		eax, ENh
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x7_step:
						loop	x7_loop
						mov		buf, edi
					}
				}
				else{
					_asm{
						mov		eax, ENs
						mov		ebx, ENa
						mov		esi, x
						mov		edx, y
						mov		edi, buf
						mov		ecx, xl
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x8_loop:
						inc		esi
						add		eax, ebx
						cmp		eax, 0
						jl		x8_step
						inc		edx
						sub		eax, ENh
						mov		[edi], esi
						add		edi, 4
						mov		[edi], edx
						add		edi, 4
					x8_step:
						loop	x8_loop
						mov		buf, edi
					}
				}
			}
		}
	}
#else
	else if(xl > yl){
		ENs = -xl;
		ENa = yl<<1;
		ENh = xl<<1;
		if((mod==0 && xp<0) || (mod==1 && xp>0)){
			for(i=0; i<xl; i++){		// [<]の符号に注意
				ENs += ENa;
				if(ENs >= 0){
					*(buf++) = x;
					*(buf++) = y;
					y   += yp;
					ENs -= ENh;
				}
				x   += xp;
			}
			*(buf++) = x;
			*(buf++) = y;
		}
		else{
			*(buf++) = x;
			*(buf++) = y;
			for(i=0; i<xl; i++){		// [<]の符号に注意
				x   += xp;
				ENs += ENa;
				if(ENs >= 0){
					y   += yp;
					ENs -= ENh;
					*(buf++) = x;
					*(buf++) = y;
				}
			}
		}
	}
#endif



// Ｙ基準線
#if Y_LINE_BUFFRING_ASM
	else{
		ENs = -yl;
		ENa = xl<<1;
		ENh = yl<<1;
		if(yp < 0){
			if(xp < 0){
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, yl
					inc		ecx
				y1_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					dec		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y1_step
					dec		esi
					sub		eax, ENh
				y1_step:
					loop	y1_loop
					mov		buf, edi
				}
			}
			else{
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, yl
					inc		ecx
				y2_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					dec		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y2_step
					inc		esi
					sub		eax, ENh
				y2_step:
					loop	y2_loop
					mov		buf, edi
				}
			}
		}
		else{
			if(xp < 0){
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, yl
					inc		ecx
				y3_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					inc		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y3_step
					dec		esi
					sub		eax, ENh
				y3_step:
					loop	y3_loop
					mov		buf, edi
				}
			}
			else{
				_asm{
					mov		eax, ENs
					mov		ebx, ENa
					mov		esi, x
					mov		edx, y
					mov		edi, buf
					mov		ecx, yl
					inc		ecx
				y4_loop:
					mov		[edi], esi
					add		edi, 4
					mov		[edi], edx
					add		edi, 4
					inc		edx
					add		eax, ebx
					cmp		eax, 0
					jl		y4_step
					inc		esi
					sub		eax, ENh
				y4_step:
					loop	y4_loop
					mov		buf, edi
				}
			}
		}
	}
#else
	else{
		ENs = -yl;
		ENa = xl<<1;
		ENh = yl<<1;
		for(i=0; i<=yl; i++){
			*(buf++) = x;
			*(buf++) = y;
			y   += yp;
			ENs += ENa;
			if(ENs >= 0){
				x  += xp;
				ENs -= ENh;
			}
		}
	}
#endif

	return( ((UI)buf-(UI)org) >> 3 );		// ( >> 3 ) ＵＩバッファだから

}



/*========================================================================================
	ボックス拡縮
void box_magnify(
	int sx1, int sy1, int sx2, int sy2,
	int dx1, int dy1, int dx2, int dy2,
	int *x1, int *y1, int *x2, int *y2
);
		IN		int (x1 - y2) : 終始点
				true_color_grp_disp参照
		OUT		なし
----------------------------------------------------------------------------------------*/
void box_magnify(
	int sx1, int sy1, int sx2, int sy2,
	int dx1, int dy1, int dx2, int dy2,
	int *x1, int *y1, int *x2, int *y2
){
	int tmp;
	int _dxlen, _dylen;
	int _sxlen, _sylen;

	int _x1, _y1, _x2, _y2;

	if(sx1>sx2){tmp=sx1; sx1=sx2; sx2=tmp;}
	if(sy1>sy2){tmp=sy1; sy1=sy2; sy2=tmp;}
	if(dx1>dx2){tmp=dx1; dx1=dx2; dx2=tmp;}
	if(dy1>dy2){tmp=dy1; dy1=dy2; dy2=tmp;}
	_dxlen = dx2 - dx1 + 1;
	_dylen = dy2 - dy1 + 1;
	_sxlen = sx2 - sx1 + 1;
	_sylen = sy2 - sy1 + 1;

	if(_sxlen != _dxlen){
		_x1 = *x1;
		_x2 = *x2;
		_x1 -= sx1;
		_x2 -= sx1;
		_x1  = (_x1  * _dxlen) / _sxlen;
		_x2  = (_x2  * _dxlen) / _sxlen;
		*x1 = _x1 + dx1;
		*x2 = _x2 + dx1;
	}

	if(_sylen != _dylen){
		_y1 = *y1;
		_y2 = *y2;
		_y1 -= sy1;
		_y2 -= sy1;
		_y1  = (_y1  * _dylen) / _sylen;
		_y2  = (_y2  * _dylen) / _sylen;
		*y1 = _y1 + dy1;
		*y2 = _y2 + dy1;
	}
}



