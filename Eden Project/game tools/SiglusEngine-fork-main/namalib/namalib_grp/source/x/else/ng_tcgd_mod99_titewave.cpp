#include	"pch.h"
#include	"ng_macro.h"



// �g��^�C���\���F���[�h�X�X
void tcgd_normal_clr_tile_wave(void){
	int clr, clr2, clr3;

// ���݂̓����x
	clr = GRPF_clr4 >> 2;

// �g�̏o�����x
	clr2 = clr << 1;
	if(clr2 > 255){clr2 = 255;}
	CTPCLR = *(Gclr_table_adr_p + clr2);

// �g�̈ړ����x
	clr2 = clr >> 1;

// �g�S�̂̏o�����x
//	if(GetAsyncKeyState((int)VK_SHIFT)&0x8000){		// SHIFT �L�[
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

// �}�X�N�l���� --------------------------------------------
			mov		ebx, GRPF_t_adr
			add		ebx, GRPF_t_x
			xor		eax, eax
			mov		al, [ebx]				// �}�X�N�l�擾

			cmp		GRPF_t_reverse, 0
			je		grplv_tile_normal
			not		al						// �}�X�N���]
		grplv_tile_normal:

			mov		ebx, eax
			add		ebx, clr2				// �}�X�N�l�ɓ����x�Q���Z
			mov		edx, ebx				// edx �ɕۑ�
			shl		ebx, 3					// �V�t�g�R��
			and		ebx, 0x000000ff			// ����Ȃ��r�b�g�𗎂Ƃ�
			and		edx, 0x00000020
			jz		grplv_step_clr
			xor		ebx, 0x000000ff			// ���]
// �����x���� ----------------------------------------------
		grplv_step_clr:
		// �����x�ϊ��P
			add		eax, clr3				// �}�X�N�l�ɓ����x�R���Z
			cmp		eax, 255
			jge		grplv_step_clr2

			shl		ebx, 2
			add		ebx, Gclr_table_adr_p
			mov		ebx, [ebx]				// �����x�e�[�u���̃|�C���^�擾
			shl		eax, 2
			add		ebx, eax
			mov		ebx, [ebx]

		grplv_step_clr2:
		// �����x�ϊ��Q
			shl		ebx, 2
			add		ebx, CTPCLR
			mov		ebx, [ebx]
		// �����x���Z
			add		ebx, clr
			cmp		ebx, 255
			jle		grplv_step_clr_add
			mov		ebx, 255				// 255 ���傫���ꍇ�� 255 �ɕ␳
		grplv_step_clr_add:
		// �����x�O����
			mov		eax, ebx
			and		eax, eax
			jz		grplv_step_b
// �`�� ----------------------------------------------------
			shl		ebx, 2
			add		ebx, Gclr_table_adr_p
			mov		edx, [ebx]				// �����x�e�[�u���̃|�C���^�擾

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

