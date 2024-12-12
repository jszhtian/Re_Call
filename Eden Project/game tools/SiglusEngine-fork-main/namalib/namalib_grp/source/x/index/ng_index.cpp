#include	"pch.h"
#include	"ng_macro.h"





/*========================================================================================
	インデックス表示
void index_color_grp_disp(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc,
	UC *pal, int palcnt,
	int x, int y,
	int clr,
	int ex1, int ey1, int ex2, int ey2
);
		IN		なし
		OUT		なし
----------------------------------------------------------------------------------------*/

void ic_normal(void){
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
			mov		al, [esi]
			mov		ebx, IC_pal
			shl		eax, 2
			add		ebx, eax

			mov		eax, [ebx]
			mov		[edi], eax

			add		esi, 1
			add		edi, 4

//			dec		ecx
//			jz		grplv_loop_x_end
//			jmp		grplv_loop_x
//		grplv_loop_x_end:
			loop	grplv_loop_x

			add		edi, GRPF_d_xl
			add		esi, GRPF_s_xl
			pop		ecx

//			dec		ecx
//			jz		grplv_loop_y_end
//			jmp		grplv_loop_y
//		grplv_loop_y_end:
			loop	grplv_loop_y
		}
}

//-----------------------------------------------------------------------------------------------

void ic_normal_clr(void){

		_asm{
			cld
			mov		edi, GRPF_d_buf
			mov		esi, GRPF_s_buf
			mov		edx, CTPCLR
			mov		ecx, GRPF_cnt_y
		grplv_loop_y:
			push	ecx
			mov		ecx, GRPF_cnt_x_org
		grplv_loop_x:

			push	esi

			xor		eax, eax
			mov		al, [esi]
			mov		esi, IC_pal
			shl		eax, 2
			add		esi, eax

			xor		eax, eax
			xor		ebx, ebx
			mov		al, [edi]
			mov		bl, [esi]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, edx
			add		eax, [ebx]
			mov		[edi], al

			xor		eax, eax
			xor		ebx, ebx
			mov		al, [edi+1]
			mov		bl, [esi+1]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, edx
			add		eax, [ebx]
			mov		[edi+1], al

			xor		eax, eax
			xor		ebx, ebx
			mov		al, [edi+2]
			mov		bl, [esi+2]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, edx
			add		eax, [ebx]
			mov		[edi+2], al

			pop		esi

			add		esi, 1
			add		edi, 4

			dec		ecx
			jz		grplv_loop_x_end
			jmp		grplv_loop_x
		grplv_loop_x_end:
			add		edi, GRPF_d_xl
			add		esi, GRPF_s_xl
			pop		ecx
			dec		ecx
			jz		grplv_loop_y_end
			jmp		grplv_loop_y
		grplv_loop_y_end:
		}
}

//-----------------------------------------------------------------------------------------------

void index_color_grp_disp(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc,
	UC *pal, int palcnt,
	int x, int y,
	int clr,
	int ex1, int ey1, int ex2, int ey2
){
	int tmp;
	int s_free_x, d_free_x, cnt_x;
	int s_free_y, d_free_y, cnt_y;

	if(s_buf==0 || d_buf==0 || pal==0){
		return;
	}

// エリア設定
	ng_set_eria(d_xl, d_yl, &ex1, &ey1, &ex2, &ey2);
// パラメータＸＣ、ＹＣを修正
	x -= s_xc;
	y -= s_yc;
// 表示範囲外チェック１
	if(x > ex2){return;}
	if(y > ey2){return;}
// 座標調整
	if(x < 0){
		s_free_x = (-x);
		cnt_x    = s_xl - s_free_x;
		d_free_x = 0;
	}
	else{
		s_free_x = 0;
		cnt_x    = s_xl;
		d_free_x = x;
	}
	if(y < 0){
		s_free_y = (-y);
		cnt_y    = s_yl - s_free_y;
		d_free_y = 0;
	}
	else{
		s_free_y = 0;
		cnt_y    = s_yl;
		d_free_y = y;
	}
// 表示範囲外チェック２
	if(cnt_x <= 0){return;}
	if(cnt_y <= 0){return;}
// 始点リミット修正
	if(ex1 > d_free_x){
		tmp = ex1 - d_free_x;
		s_free_x += tmp;
		cnt_x    -= tmp;
		d_free_x  = ex1;
	}
	if(ey1 > d_free_y){
		tmp = ey1 - d_free_y;
		s_free_y += tmp;
		cnt_y    -= tmp;
		d_free_y  = ey1;
	}
// 表示範囲外チェック３
	if(ex2 < d_free_x){return;}
	if(ey2 < d_free_y){return;}
// 終点リミット修正
	tmp = ex2 - d_free_x + 1;
	if(tmp < cnt_x){cnt_x = tmp;}
	tmp = ey2 - d_free_y + 1;
	if(tmp < cnt_y){cnt_y = tmp;}
// 表示範囲外チェック４
	if(cnt_x <= 0){return;}
	if(cnt_y <= 0){return;}

// アドレス算出
	s_buf += (s_free_x + (s_free_y * s_xl));
	d_buf += (d_free_x + (d_free_y * d_xl)) << 2;

// 描画パラメータ
	GRPF_d_buf = d_buf;
	GRPF_d_xl  = d_xl << 2;
	GRPF_d_yl  = d_yl;

	GRPF_s_buf = s_buf;
	GRPF_s_xl  = s_xl;
	GRPF_s_yl  = s_yl;

	GRPF_cnt_x_org = cnt_x;
	GRPF_cnt_y     = cnt_y;

	GRPF_d_xl -= (cnt_x << 2);
	GRPF_s_xl -= (cnt_x);

	IC_pal = pal;

	GRPF_clr4 = (clr << 2);
	GRPF_clrR = 255 - clr;
	CTPCLR = *(Gclr_table_adr_p + clr);

//****************************************************************************************

	if(clr == 255){
		ic_normal();
	}
	else{
		ic_normal_clr();
	}
}













