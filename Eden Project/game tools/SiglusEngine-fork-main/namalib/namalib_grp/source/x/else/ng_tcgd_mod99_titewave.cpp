#include	"pch.h"
#include	"ng_macro.h"



// 波紋タイル表示：モード９９
void tcgd_normal_clr_tile_wave(void){
	int clr, clr2, clr3;

// 現在の透明度
	clr = GRPF_clr4 >> 2;

// 波の出現速度
	clr2 = clr << 1;
	if(clr2 > 255){clr2 = 255;}
	CTPCLR = *(Gclr_table_adr_p + clr2);

// 波の移動速度
	clr2 = clr >> 1;

// 波全体の出現速度
//	if(GetAsyncKeyState((int)VK_SHIFT)&0x8000){		// SHIFT キー
//		clr3 = 255;
//	}
//	clr3 = clr >> 4;
	clr3 = 0;

		_asm{
			cld
			mov		edi, GRPF_d_buf
			mov		esi, GRPF_s_buf
			mov		ecx, GRPF_cnt_y
		grplv_loop_y:
			push	ecx
			mov		ecx, GRPF_cnt_x_org
			mov		ebx, GRPF_t_x_org
			mov		GRPF_t_x, ebx
		grplv_loop_x:

// マスク値処理 --------------------------------------------
			mov		ebx, GRPF_t_adr
			add		ebx, GRPF_t_x
			xor		eax, eax
			mov		al, [ebx]				// マスク値取得

			cmp		GRPF_t_reverse, 0
			je		grplv_tile_normal
			not		al						// マスク反転
		grplv_tile_normal:

			mov		ebx, eax
			add		ebx, clr2				// マスク値に透明度２加算
			mov		edx, ebx				// edx に保存
			shl		ebx, 3					// シフト３回
			and		ebx, 0x000000ff			// いらないビットを落とす
			and		edx, 0x00000020
			jz		grplv_step_clr
			xor		ebx, 0x000000ff			// 反転
// 透明度処理 ----------------------------------------------
		grplv_step_clr:
		// 透明度変換１
			add		eax, clr3				// マスク値に透明度３加算
			cmp		eax, 255
			jge		grplv_step_clr2

			shl		ebx, 2
			add		ebx, Gclr_table_adr_p
			mov		ebx, [ebx]				// 透明度テーブルのポインタ取得
			shl		eax, 2
			add		ebx, eax
			mov		ebx, [ebx]

		grplv_step_clr2:
		// 透明度変換２
			shl		ebx, 2
			add		ebx, CTPCLR
			mov		ebx, [ebx]
		// 透明度加算
			add		ebx, clr
			cmp		ebx, 255
			jle		grplv_step_clr_add
			mov		ebx, 255				// 255 より大きい場合は 255 に補正
		grplv_step_clr_add:
		// 透明度０判定
			mov		eax, ebx
			and		eax, eax
			jz		grplv_step_b
// 描画 ----------------------------------------------------
			shl		ebx, 2
			add		ebx, Gclr_table_adr_p
			mov		edx, [ebx]				// 透明度テーブルのポインタ取得

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

		grplv_step_b:

			add		esi, 4
			add		edi, 4

			mov		eax, GRPF_t_x
			inc		eax
			cmp		eax, GRPF_t_xl
			jl		grplv_step_tile1
			xor		eax, eax
		grplv_step_tile1:
			mov		GRPF_t_x, eax
			dec		ecx
			jz		grplv_loop_x_end
			jmp		grplv_loop_x
		grplv_loop_x_end:
			add		edi, GRPF_d_xl
			add		esi, GRPF_s_xl
			mov		eax, GRPF_t_adr
			add		eax, GRPF_t_xl
			cmp		eax, GRPF_t_adr_max
			jl		grplv_step_tile2
			mov		eax, GRPF_t_adr_org
		grplv_step_tile2:
			mov		GRPF_t_adr, eax
			pop		ecx
			dec		ecx
			jz		grplv_loop_y_end
			jmp		grplv_loop_y
		grplv_loop_y_end:
		}
}

