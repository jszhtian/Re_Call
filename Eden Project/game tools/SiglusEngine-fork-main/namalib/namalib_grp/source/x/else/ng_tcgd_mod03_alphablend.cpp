#include	"pch.h"
#include	"ng_macro.h"



// 通常表示（透明度合成：モード３）
void tcgd_normal_mod03_alphablend(void){

		_asm{
			cld
			mov		edi, GRPF_d_buf
			mov		esi, GRPF_s_buf
			mov		ecx, GRPF_cnt_y

		grplv_loop_y:
			push	ecx
			mov		ecx, GRPF_cnt_x_org
		grplv_loop_x:

			mov		eax, [esi]
			mov		[edi], eax

			add		esi, 4
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

// 変色通常表示（透明度合成：モード３）
void tcgd_normal_change_mod03_alphablend(void){

		_asm{
			cld
			mov		edi, GRPF_d_buf
			mov		esi, GRPF_s_buf
			mov		ecx, GRPF_cnt_y

		grplv_loop_y:
			push	ecx
			mov		ecx, GRPF_cnt_x_org
		grplv_loop_x:

			xor		ebx, ebx
			mov		eax, GRPF_c_b
			mov		bl, [esi]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, CTPBETA
			add		eax, [ebx]
			mov		[edi], al

			xor		ebx, ebx
			mov		eax, GRPF_c_g
			mov		bl, [esi+1]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, CTPBETA
			add		eax, [ebx]
			mov		[edi+1], al

			xor		ebx, ebx
			mov		eax, GRPF_c_r
			mov		bl, [esi+2]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, CTPBETA
			add		eax, [ebx]
			mov		[edi+2], al

			mov		al, [esi+3]
			mov		[edi+3], al

			add		esi, 4
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

// スプライト表示（透明度合成：モード３）
void tcgd_sprite_mod03_alphablend(void){

		_asm{
			cld
			mov		edi, GRPF_d_buf
			mov		esi, GRPF_s_buf
			mov		ecx, GRPF_cnt_y
		grplv_loop_y:
			push	ecx
			mov		ecx, GRPF_cnt_x_org
		grplv_loop_x:
			push	ecx

			xor		ebx, ebx
			mov		bl, [esi+3]
			and		bl, bl
			jz		grplv_step_b		// 転送元の透明度が０なのでジャンプ

			cmp		ebx, 255
			jne		grplv_step_e

		// 転送元の透明度が２５５の時はそのままコピー
			mov		eax, [esi]
			mov		[edi], eax
			jmp		grplv_step_b

		grplv_step_e:
			mov		dl, [edi+3]
			and		dl, dl
			jnz		grplv_step_f

		// 転送先の透明度が０の時はそのままコピー
			mov		eax, [esi]
			mov		[edi], eax
			jmp		grplv_step_b

		grplv_step_f:
			shl		ebx, 2
			add		ebx, Gclr_table_adr_p
			mov		edx, [ebx]				// 転送元の透明度テーブル

			xor		ebx, ebx
			mov		bl, [edi+3]
			shl		ebx, 2
			add		ebx, Gclr_table_adr_p
			mov		ecx, [ebx]				// 転送先の透明度テーブル

		// 透明度
			xor		eax, eax
			xor		ebx, ebx
			mov		al, [edi+3]
			mov		bl, [esi+3]
			add		eax, ebx
			cmp		eax, 255
			jle		grplv_step_g
			mov		eax, 255
		grplv_step_g:
			mov		[edi+3], al

			xor		eax, eax
			xor		ebx, ebx
			mov		bl, [edi]
			shl		ebx, 2
			add		ebx, ecx
			add		eax, [ebx]
			xor		ebx, ebx
			mov		bl, [esi]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, edx
			add		eax, [ebx]
			mov		ebx, eax
			shl		ebx, 2
			add		ebx, G256_table_adr_p
			mov		ebx, [ebx]
			mov		al, [edi+3]
			shl		eax, 2
			add		ebx, eax
			mov		eax, [ebx]
			mov		[edi], al

			xor		eax, eax
			xor		ebx, ebx
			mov		bl, [edi+1]
			shl		ebx, 2
			add		ebx, ecx
			add		eax, [ebx]
			xor		ebx, ebx
			mov		bl, [esi+1]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, edx
			add		eax, [ebx]
			mov		ebx, eax
			shl		ebx, 2
			add		ebx, G256_table_adr_p
			mov		ebx, [ebx]
			mov		al, [edi+3]
			shl		eax, 2
			add		ebx, eax
			mov		eax, [ebx]
			mov		[edi+1], al

			xor		eax, eax
			xor		ebx, ebx
			mov		bl, [edi+2]
			shl		ebx, 2
			add		ebx, ecx
			add		eax, [ebx]
			xor		ebx, ebx
			mov		bl, [esi+2]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, edx
			add		eax, [ebx]
			mov		ebx, eax
			shl		ebx, 2
			add		ebx, G256_table_adr_p
			mov		ebx, [ebx]
			mov		al, [edi+3]
			shl		eax, 2
			add		ebx, eax
			mov		eax, [ebx]
			mov		[edi+2], al

		grplv_step_b:

			add		esi, 4
			add		edi, 4

			pop		ecx
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



// 変色スプライト表示（透明度合成：モード３）
void tcgd_sprite_change_mod03_alphablend(void){

		_asm{
			cld
			mov		edi, GRPF_d_buf
			mov		esi, GRPF_s_buf
			mov		ecx, GRPF_cnt_y
		grplv_loop_y:
			push	ecx
			mov		ecx, GRPF_cnt_x_org
		grplv_loop_x:
			push	ecx

			xor		ebx, ebx
			mov		bl, [esi+3]
			and		bl, bl
			jz		grplv_step_b
			xor		bl, 0xff
			jz		grplv_step_a

			mov		dl, [edi+3]
			and		dl, dl
			jnz		grplv_step_f

		grplv_step_a:				// ソース側が２５５、またはディスティネーション側が０
			xor		ebx, ebx
			mov		eax, GRPF_c_b
			mov		bl, [esi]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, CTPBETA
			add		eax, [ebx]
			mov		[edi], al

			xor		ebx, ebx
			mov		eax, GRPF_c_g
			mov		bl, [esi+1]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, CTPBETA
			add		eax, [ebx]
			mov		[edi+1], al

			xor		ebx, ebx
			mov		eax, GRPF_c_r
			mov		bl, [esi+2]
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, CTPBETA
			add		eax, [ebx]
			mov		[edi+2], al

			mov		al, [esi+3]
			mov		[edi+3], al

			jmp		grplv_step_b

		grplv_step_f:

			mov		bl, [esi+3]

			shl		ebx, 2
			add		ebx, Gclr_table_adr_p
			mov		edx, [ebx]

			xor		ebx, ebx
			mov		bl, [edi+3]
			shl		ebx, 2
			add		ebx, Gclr_table_adr_p
			mov		ecx, [ebx]

			xor		eax, eax
			xor		ebx, ebx
			mov		al, [edi+3]
			mov		bl, [esi+3]
			add		eax, ebx
			cmp		eax, 255
			jle		grplv_step_g
			mov		eax, 255
		grplv_step_g:
			mov		[edi+3], al

			push	ecx
			xor		eax, eax
			xor		ebx, ebx
			mov		bl, [edi]
			shl		ebx, 2
			add		ebx, ecx
			add		eax, [ebx]
			xor		ebx, ebx
			mov		ecx, GRPF_c_b
			mov		bl, [esi]
			sub		ebx, ecx
			shl		ebx, 2
			add		ebx, CTPBETA
			add		ecx, [ebx]
			mov		ebx, ecx
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, edx
			add		eax, [ebx]
			mov		ebx, eax
			shl		ebx, 2
			add		ebx, G256_table_adr_p
			mov		ebx, [ebx]
			mov		al, [edi+3]
			shl		eax, 2
			add		ebx, eax
			mov		eax, [ebx]
			mov		[edi], al
			pop		ecx

			push	ecx
			xor		eax, eax
			xor		ebx, ebx
			mov		bl, [edi+1]
			shl		ebx, 2
			add		ebx, ecx
			add		eax, [ebx]
			mov		bl, [esi]
			xor		ebx, ebx
			mov		ecx, GRPF_c_g
			mov		bl, [esi+1]
			sub		ebx, ecx
			shl		ebx, 2
			add		ebx, CTPBETA
			add		ecx, [ebx]
			mov		ebx, ecx
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, edx
			add		eax, [ebx]
			mov		ebx, eax
			shl		ebx, 2
			add		ebx, G256_table_adr_p
			mov		ebx, [ebx]
			mov		al, [edi+3]
			shl		eax, 2
			add		ebx, eax
			mov		eax, [ebx]
			mov		[edi+1], al
			pop		ecx

			push	ecx
			xor		eax, eax
			xor		ebx, ebx
			mov		bl, [edi+2]
			shl		ebx, 2
			add		ebx, ecx
			add		eax, [ebx]
			xor		ebx, ebx
			mov		ecx, GRPF_c_r
			mov		bl, [esi+2]
			sub		ebx, ecx
			shl		ebx, 2
			add		ebx, CTPBETA
			add		ecx, [ebx]
			mov		ebx, ecx
			sub		ebx, eax
			shl		ebx, 2
			add		ebx, edx
			add		eax, [ebx]
			mov		ebx, eax
			shl		ebx, 2
			add		ebx, G256_table_adr_p
			mov		ebx, [ebx]
			mov		al, [edi+3]
			shl		eax, 2
			add		ebx, eax
			mov		eax, [ebx]
			mov		[edi+2], al
			pop		ecx

		grplv_step_b:

			add		esi, 4
			add		edi, 4

			pop		ecx
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

