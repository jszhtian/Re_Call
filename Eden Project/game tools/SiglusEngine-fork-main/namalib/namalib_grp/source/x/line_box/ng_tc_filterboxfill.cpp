#include	"pch.h"
#include	"ng_macro.h"




// ���]�ʏ�\��
void tcfbf_normal_reverse(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		esi, CTPREV
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:

	}
}



// �^�C�����]�ʏ�\��
void tcfbf_normal_reverse_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// ���]�����ʏ�\��
void tcfbf_normal_reverse_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp
		mov		esi, CTPCLR

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]�����ʏ�\��
void tcfbf_normal_reverse_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}




// ���m�ʏ�\��
void tcfbf_normal_mono(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����m�ʏ�\��
void tcfbf_normal_mono_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}




// ���m�����ʏ�\��
void tcfbf_normal_mono_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����m�����ʏ�\��
void tcfbf_normal_mono_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// ���Òʏ�\��
void tcfbf_normal_bright(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����Òʏ�\��
void tcfbf_normal_bright_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// ���Ó����ʏ�\��
void tcfbf_normal_bright_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp
		mov		esi, CTPCLR

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����Ó����ʏ�\��
void tcfbf_normal_bright_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �ϐF�ʏ�\��
void tcfbf_normal_change(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C���ϐF�ʏ�\��
void tcfbf_normal_change_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �ϐF�����ʏ�\��
void tcfbf_normal_change_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp
		mov		esi, CTPCLR

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C���ϐF�����ʏ�\��
void tcfbf_normal_change_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// ���]���m�ʏ�\��
void tcfbf_normal_reverse_mono(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]���m�ʏ�\��
void tcfbf_normal_reverse_mono_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}




// ���]���m�����ʏ�\��
void tcfbf_normal_reverse_mono_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]���m�����ʏ�\��
void tcfbf_normal_reverse_mono_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}






// ���]���Òʏ�\��
void tcfbf_normal_reverse_bright(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]���Òʏ�\��
void tcfbf_normal_reverse_bright_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// ���]���Ó����ʏ�\��
void tcfbf_normal_reverse_bright_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp
		mov		esi, CTPCLR

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]���Ó����ʏ�\��
void tcfbf_normal_reverse_bright_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}






// ���]�ϐF�ʏ�\��
void tcfbf_normal_reverse_change(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]�ϐF�ʏ�\��
void tcfbf_normal_reverse_change_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// ���]�ϐF�����ʏ�\��
void tcfbf_normal_reverse_change_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp
		mov		esi, CTPCLR

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]�ϐF�����ʏ�\��
void tcfbf_normal_reverse_change_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}







// ���m���Òʏ�\��
void tcfbf_normal_mono_bright(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����m���Òʏ�\��
void tcfbf_normal_mono_bright_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// ���m���Ó����ʏ�\��
void tcfbf_normal_mono_bright_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����m���Ó����ʏ�\��
void tcfbf_normal_mono_bright_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}






// ���m�ϐF�ʏ�\��
void tcfbf_normal_mono_change(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����m�ϐF�ʏ�\��
void tcfbf_normal_mono_change_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}




// ���m�ϐF�����ʏ�\��
void tcfbf_normal_mono_change_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����m�ϐF�����ʏ�\��
void tcfbf_normal_mono_change_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}






// �ϐF���Òʏ�\��
void tcfbf_normal_change_bright(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C���ϐF���Òʏ�\��
void tcfbf_normal_change_bright_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �ϐF���Ó����ʏ�\��
void tcfbf_normal_change_bright_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp
		mov		esi, CTPCLR

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C���ϐF���Ó����ʏ�\��
void tcfbf_normal_change_bright_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}





// ���]���m���Òʏ�\��
void tcfbf_normal_reverse_mono_bright(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]���m���Òʏ�\��
void tcfbf_normal_reverse_mono_bright_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}




// ���]���m���Ó����ʏ�\��
void tcfbf_normal_reverse_mono_bright_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]���m���Ó����ʏ�\��
void tcfbf_normal_reverse_mono_bright_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}








// ���]���m�ϐF�ʏ�\��
void tcfbf_normal_reverse_mono_change(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]���m�ϐF�ʏ�\��
void tcfbf_normal_reverse_mono_change_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}




// ���]���m�ϐF�����ʏ�\��
void tcfbf_normal_reverse_mono_change_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]���m�ϐF�����ʏ�\��
void tcfbf_normal_reverse_mono_change_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}







// ���]�ϐF���Òʏ�\��
void tcfbf_normal_reverse_change_bright(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]�ϐF���Òʏ�\��
void tcfbf_normal_reverse_change_bright_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// ���]�ϐF���Ó����ʏ�\��
void tcfbf_normal_reverse_change_bright_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp
		mov		esi, CTPCLR

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]�ϐF���Ó����ʏ�\��
void tcfbf_normal_reverse_change_bright_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}








// ���m�ϐF���Òʏ�\��
void tcfbf_normal_mono_change_bright(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����m�ϐF���Òʏ�\��
void tcfbf_normal_mono_change_bright_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}




// ���m�ϐF���Ó����ʏ�\��
void tcfbf_normal_mono_change_bright_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����m�ϐF���Ó����ʏ�\��
void tcfbf_normal_mono_change_bright_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}










// ���]���m�ϐF���Òʏ�\��
void tcfbf_normal_reverse_mono_change_bright(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]���m�ϐF���Òʏ�\��
void tcfbf_normal_reverse_mono_change_bright_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}




// ���]���m�ϐF���Ó����ʏ�\��
void tcfbf_normal_reverse_mono_change_bright_clr(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



// �^�C�����]���m�ϐF���Ó����ʏ�\��
void tcfbf_normal_reverse_mono_change_bright_clr_tile(void){
	_asm{
		cld
		mov		edi, FILTER_d_buf
		mov		edx, FILTER_xp

		mov		ecx, FILTER_yl
	flineh_loop_y:
		push	ecx

		mov		ecx, FILTER_xl
	flineh_loop_x:

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
		inc		eax
		cmp		eax, FILTER_t_xl
		jl		flineh_jl
		xor		eax, eax
	flineh_jl:
		mov		FILTER_t_x, eax

		add		edi, 4
		dec		ecx
		jz		flineh_loop_x_end
		jmp		flineh_loop_x
	flineh_loop_x_end:

		mov		eax, FILTER_t_adr
		add		eax, FILTER_t_xl
		cmp		eax, FILTER_t_adr_max
		jl		flineh_jl2
		mov		eax, FILTER_t_adr_org
	flineh_jl2:
		mov		FILTER_t_adr, eax

		pop		ecx
		add		edi, edx
		dec		ecx
		jz		flineh_loop_y_end
		jmp		flineh_loop_y
	flineh_loop_y_end:
	}
}



/*========================================================================================
	�t�B���^�{�b�N�X�t�B���\��
void true_color_filterboxfill(
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
		IN		int (x1 - y2) : �I�n�_
				true_color_grp_disp�Q��
		OUT		�Ȃ�
----------------------------------------------------------------------------------------*/
void true_color_filterboxfill(
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
	int tmp;

	int xl, yl;
	int xp;				// ���W�̑���

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

// �G���A�ݒ�
	ng_set_eria(d_xl, d_yl, &ex1, &ey1, &ex2, &ey2);

// �N���b�v
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

// �O����
	xl = (x2 - x1 + 1);
	yl = (y2 - y1 + 1);
	xp = d_xl - xl;

// �A�h���X�Z�o
	if(screen_mod == 0){
		d_buf += ((x1 + (d_xl * y1)) << 2);
	}
	else{
		xp = -(xl + d_xl);
		d_buf += ((x1 + (d_xl * (d_yl-y1-1))) << 2);
	}

// �`��p�����[�^
	FILTER_d_buf = d_buf;
	FILTER_d_xl  = d_xl;
	FILTER_d_yl  = d_yl;

	FILTER_d_xl = d_xl << 2;

	FILTER_xl = xl;
	FILTER_yl = yl;

	FILTER_xp = xp << 2;

// ���]�p�����[�^
	CTPREV = *(Gclr_table_adr_p + (255 - reverse));
	FILTER_reverse = reverse;

// ���m�p�����[�^
	CTPMONO = *(Gclr_table_adr_p + (255 - mono));
	FILTER_mono = mono;

// �ϐF�p�����[�^
	if(c_rate){
		FILTER_c_r = c_r;
		FILTER_c_g = c_g;
		FILTER_c_b = c_b;
		CTPBETA = *(Gclr_table_adr_p + (255 - c_rate));
	}

// �����x�p�����[�^
	FILTER_clr  = clr;
	FILTER_clr4 = (clr << 2);
	CTPCLR = *(Gclr_table_adr_p + clr);

// �F���p�����[�^
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

	if(disp_mod == 2){		// ���Z���[�h
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

// �^�C���p�����[�^
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
// �ʏ�\��
//tcfbf_normal();		// ��������K�v�Ȃ�
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C���ʏ�\��
//tcfbf_normal_tile();	// ��������K�v�Ȃ�
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]�ʏ�\��
tcfbf_normal_reverse();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]�ʏ�\��
tcfbf_normal_reverse_tile();
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
// ���m�ʏ�\��
tcfbf_normal_mono();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����m�ʏ�\��
tcfbf_normal_mono_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]���m�ʏ�\��
tcfbf_normal_reverse_mono();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]���m�ʏ�\��
tcfbf_normal_reverse_mono_tile();
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
// ���Òʏ�\��
tcfbf_normal_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����Òʏ�\��
tcfbf_normal_bright_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]���Òʏ�\��
tcfbf_normal_reverse_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]���Òʏ�\��
tcfbf_normal_reverse_bright_tile();
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
// ���m���Òʏ�\��
tcfbf_normal_mono_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����m���Òʏ�\��
tcfbf_normal_mono_bright_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]���m���Òʏ�\��
tcfbf_normal_reverse_mono_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]���m���Òʏ�\��
tcfbf_normal_reverse_mono_bright_tile();
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
// �ϐF�ʏ�\��
tcfbf_normal_change();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C���ϐF�ʏ�\��
tcfbf_normal_change_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]�ϐF�ʏ�\��
tcfbf_normal_reverse_change();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]�ϐF�ʏ�\��
tcfbf_normal_reverse_change_tile();
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
// ���m�ϐF�ʏ�\��
tcfbf_normal_mono_change();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����m�ϐF�ʏ�\��
tcfbf_normal_mono_change_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]���m�ϐF�ʏ�\��
tcfbf_normal_reverse_mono_change();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]���m�ϐF�ʏ�\��
tcfbf_normal_reverse_mono_change_tile();
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
// �ϐF���Òʏ�\��
tcfbf_normal_change_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C���ϐF���Òʏ�\��
tcfbf_normal_change_bright_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]�ϐF���Òʏ�\��
tcfbf_normal_reverse_change_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]�ϐF���Òʏ�\��
tcfbf_normal_reverse_change_bright_tile();
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
// ���m�ϐF���Òʏ�\��
tcfbf_normal_mono_change_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����m�ϐF���Òʏ�\��
tcfbf_normal_mono_change_bright_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]���m�ϐF���Òʏ�\��
tcfbf_normal_reverse_mono_change_bright();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]���m�ϐF���Òʏ�\��
tcfbf_normal_reverse_mono_change_bright_tile();
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
// �����ʏ�\��
//tcfbf_normal_clr();		// ��������K�v�Ȃ�
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�������ʏ�\��
//tcfbf_normal_clr_tile();	// ��������K�v�Ȃ�
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]�����ʏ�\��
tcfbf_normal_reverse_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]�����ʏ�\��
tcfbf_normal_reverse_clr_tile();
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
// ���m�����ʏ�\��
tcfbf_normal_mono_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����m�����ʏ�\��
tcfbf_normal_mono_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]���m�����ʏ�\��
tcfbf_normal_reverse_mono_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]���m�����ʏ�\��
tcfbf_normal_reverse_mono_clr_tile();
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
// ���Ó����ʏ�\��
tcfbf_normal_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����Ó����ʏ�\��
tcfbf_normal_bright_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]���Ó����ʏ�\��
tcfbf_normal_reverse_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]���Ó����ʏ�\��
tcfbf_normal_reverse_bright_clr_tile();
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
// ���m���Ó����ʏ�\��
tcfbf_normal_mono_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����m���Ó����ʏ�\��
tcfbf_normal_mono_bright_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]���m���Ó����ʏ�\��
tcfbf_normal_reverse_mono_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]���m���Ó����ʏ�\��
tcfbf_normal_reverse_mono_bright_clr_tile();
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
// �ϐF�����ʏ�\��
tcfbf_normal_change_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C���ϐF�����ʏ�\��
tcfbf_normal_change_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]�ϐF�����ʏ�\��
tcfbf_normal_reverse_change_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]�ϐF�����ʏ�\��
tcfbf_normal_reverse_change_clr_tile();
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
// ���m�ϐF�����ʏ�\��
tcfbf_normal_mono_change_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����m�ϐF�����ʏ�\��
tcfbf_normal_mono_change_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]���m�ϐF�����ʏ�\��
tcfbf_normal_reverse_mono_change_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]���m�ϐF�����ʏ�\��
tcfbf_normal_reverse_mono_change_clr_tile();
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
// �ϐF���Ó����ʏ�\��
tcfbf_normal_change_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C���ϐF���Ó����ʏ�\��
tcfbf_normal_change_bright_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]�ϐF���Ó����ʏ�\��
tcfbf_normal_reverse_change_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]�ϐF���Ó����ʏ�\��
tcfbf_normal_reverse_change_bright_clr_tile();
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
// ���m�ϐF���Ó����ʏ�\��
tcfbf_normal_mono_change_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����m�ϐF���Ó����ʏ�\��
tcfbf_normal_mono_change_bright_clr_tile();
									break;
							}
						}
					}
					else{
						if(tile==0 || t_add==255){
							switch(disp_mod){
								case 0:
// ���]���m�ϐF���Ó����ʏ�\��
tcfbf_normal_reverse_mono_change_bright_clr();
									break;
							}
						}
						else{
							switch(disp_mod){
								case 0:
// �^�C�����]���m�ϐF���Ó����ʏ�\��
tcfbf_normal_reverse_mono_change_bright_clr_tile();
									break;
							}
						}
					}
				}
			}
		}
	}
}



