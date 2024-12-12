#include	"tona3_pch.h"

// ****************************************************************
// �^�C���R�s�[
// ================================================================
//		��`�̃R�s�[���[�`���ł��B
//		�������A�}�X�N�^�C���̒l�� t_limit �ȏ�̕����̂݃R�s�[���܂��B
//		��Ft_limit == 128 �� 128�ȏ�Ȃ�R�s�[����A�����Ȃ�R�s�[���Ȃ�
//		�^�C���� t_limit �͕����Ȃ������i0�`255�j�Ŏw�肵�܂��B
// ================================================================
void tile_copy(BYTE *d_buf, BYTE *s_buf, int buf_xl, int buf_yl, BYTE *tile, int t_xl, int t_yl, int t_repx, int t_repy, int t_reverse, BYTE t_limit_byte)
{
	if (s_buf == 0 || d_buf == 0)
		return;

	// �^�C���p�����[�^
	int t_x = (t_repx <= 0) ? ((-t_repx) % t_xl) : ((t_xl - (t_repx % t_xl)) % t_xl);
	int t_y = (t_repy <= 0) ? ((-t_repy) % t_yl) : ((t_yl - (t_repy % t_yl)) % t_yl);
	BYTE *t_adr = tile + (t_y * t_xl);
	BYTE *t_adr_max = tile + (t_yl * t_xl);
	int t_limit = t_limit_byte;		// cmp �͕��������r�Ȃ̂ŁA��������ɒ����܂�

	_asm{
		cld
		mov		edi, d_buf
		mov		esi, s_buf
		mov		ecx, buf_yl

	grplv_loop_y:
		push	ecx
		mov		ecx, buf_xl
		mov		edx, t_x

	grplv_loop_x:
		mov		ebx, t_adr
		add		ebx, edx
		xor		eax, eax
		mov		al, [ebx]
		cmp		t_reverse, 0
		jne		grplv_tile_reverse
		cmp		eax, t_limit
		jl		grplv_step_next
		mov		eax, [esi]
		mov		[edi], eax
		jmp		grplv_step_next
	grplv_tile_reverse:
		cmp		eax, t_limit
		jge		grplv_step_next
		mov		eax, [esi]
		mov		[edi], eax
	grplv_step_next:
		add		esi, 4
		add		edi, 4
		inc		edx
		cmp		edx, t_xl
		jl		grplv_step_tile1
		xor		edx, edx
	grplv_step_tile1:
		loop	grplv_loop_x

		mov		eax, t_adr
		add		eax, t_xl
		cmp		eax, t_adr_max
		jl		grplv_step_tile2
		mov		eax, tile
	grplv_step_tile2:
		mov		t_adr, eax
		pop		ecx
		loop	grplv_loop_y
	}
}
