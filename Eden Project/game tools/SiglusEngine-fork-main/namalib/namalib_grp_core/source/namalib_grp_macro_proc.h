#pragma		once

// ****************************************************************
// �����x�ϊ��e�[�u���̃A�h���X���d�c�w�ɋL������
// ================================================================
#define		__MOV_EDX_TR_TABLE_ADR \
__asm				shl		ebx, 2 \
__asm				add		ebx, __TABLE_TR \
__asm				mov		edx, [ebx]

// ****************************************************************
// �A���t�@�����p�̓����x���L������
// ================================================================
#define		__MOV_TR_ALPHABLEND \
__asm				mov		__ALPHABLEND_A, ebx

// ****************************************************************
// �����x�擾
// ================================================================
#define		__CHECK_SPRITE_ALPHA \
__asm				xor		ebx, ebx \
__asm				mov		bl, [esi+3] \
__asm				and		ebx, ebx \
__asm				jnz		__check_sprite_alpha_end \
__asm				jmp		__dont_draw \
__asm			__check_sprite_alpha_end:

// ****************************************************************
// �����x����
// ================================================================
#define		__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				add		ebx, __TABLE_TR \
__asm				mov		ebx, [ebx] \
__asm				add		ebx, __TR_4 \
__asm				mov		ebx, [ebx] \
__asm				and		ebx, ebx \
__asm				jnz		__step_compose_tr_end \
__asm				jmp		__dont_draw \
__asm			__step_compose_tr_end:

// ****************************************************************
// �^�C������
// ================================================================

// �^�C������
#define		__CHECK_TILE_FUNC \
__asm				xor		eax, eax \
__asm				mov		al, [ebx] \
__asm				cmp		__TILE_REVERSE, 0 \
__asm				je		__step_check_tile_normal \
__asm				not		al \
__asm			__step_check_tile_normal: \
__asm				push	ecx \
__asm				mov		ecx, __TILE_SHIFT \
__asm				shl		eax, cl \
__asm				pop		ecx \
__asm				add		eax, __TILE_ADD \
__asm				cmp		eax, 0 \
__asm				jge		__step_check_tile_great \
__asm				mov		eax, 0 \
__asm				jmp		__step_check_tile_next \
__asm			__step_check_tile_great: \
__asm				cmp		eax, 255 \
__asm				jle		__step_check_tile_next \
__asm				mov		eax, 255 \
__asm			__step_check_tile_next: \
__asm				mov		ebx, eax \
__asm				and		ebx, ebx \
__asm				jnz		__step_check_tile_end \
__asm				jmp		__dont_draw \
__asm			__step_check_tile_end:

// �^�C���̓����x����
#define		__COMPOSE_TILE_TR \
__asm				shl		ebx, 2 \
__asm				add		ebx, __TABLE_TR \
__asm				mov		ebx, [ebx] \
__asm				add		ebx, edx \
__asm				mov		ebx, [ebx] \
__asm				and		ebx, ebx \
__asm				jnz		__check_tile_sprite_end \
__asm				jmp		__dont_draw \
__asm			__check_tile_sprite_end:

// ================================================================

// �^�C������
#define		__CHECK_TILE \
__asm				mov		ebx, __TILE_NOW_ADR \
__asm				add		ebx, __TILE_NOW_X \
					__CHECK_TILE_FUNC

// �^�C������i���C���p�j
#define		__CHECK_TILE_LD \
__asm				mov		ebx, __TILE_NOW_ADR \
					__CHECK_TILE_FUNC

// ================================================================

// �^�C�������Z�b�g
#define		__TILE_PROC_SET_FUNC \
					__CHECK_TILE \
					__COMPOSE_TILE_TR

#define		__TILE_PROC_SET \
					__TILE_PROC_SET_FUNC \
					__MOV_EDX_TR_TABLE_ADR

// �^�C�������Z�b�g�i���C���p�j
#define		__TILE_PROC_SET_FUNC_LD \
					__CHECK_TILE_LD \
					__COMPOSE_TILE_TR

#define		__TILE_PROC_SET_LD \
					__TILE_PROC_SET_FUNC_LD \
					__MOV_EDX_TR_TABLE_ADR

// �^�C�������Z�b�g�i�����x���Q�T�T�̎��͓����x���������Ȃ��o�[�W�����^�ʏ�^�C���`���p�j
#define		__TILE_PROC_SET_FUNC_EX \
					__CHECK_TILE \
					__COMPOSE_TILE_TR \
__asm				cmp		ebx, 255 \
__asm				jne		__step_start_tile_normal_ex \
__asm				mov		eax, [esi] \
__asm				mov		[edi], eax \
__asm				jmp		__dont_draw \
__asm			__step_start_tile_normal_ex:

#define		__TILE_PROC_SET_EX \
					__TILE_PROC_SET_FUNC_EX \
					__MOV_EDX_TR_TABLE_ADR

// �A���t�@�����p
#define		__TILE_PROC_SET_ALPHABLEND \
					__TILE_PROC_SET_FUNC \
					__MOV_TR_ALPHABLEND

#define		__TILE_PROC_SET_LD_ALPHABLEND \
					__TILE_PROC_SET_FUNC_LD \
					__MOV_TR_ALPHABLEND

#define		__TILE_PROC_SET_EX_ALPHABLEND \
					__TILE_PROC_SET_FUNC_EX \
					__MOV_TR_ALPHABLEND

// ================================================================

// �^�C�������w
#define		__TILE_PROC_X \
__asm				mov		eax, __TILE_NOW_X \
__asm				inc		eax \
__asm				cmp		eax, __TILE_XL \
__asm				jl		__step_tile_x \
__asm				xor		eax, eax \
__asm			__step_tile_x: \
__asm				mov		__TILE_NOW_X, eax

// �^�C�������x
#define		__TILE_PROC_Y \
__asm				mov		eax, __TILE_NOW_ADR \
__asm				add		eax, __TILE_XL \
__asm				cmp		eax, __TILE_END_ADR \
__asm				jl		__step_tile_y \
__asm				mov		eax, __TILE_ADR \
__asm			__step_tile_y: \
__asm				mov		__TILE_NOW_ADR, eax

// ================================================================

// �^�C�������w���Z�i���C���p�j
#define		__TILE_PROC_X_INC \
__asm				mov		eax, __TILE_NOW_X \
__asm				inc		eax \
__asm				inc		ebx \
__asm				cmp		eax, __TILE_XL \
__asm				jl		__step_tile_x_inc \
__asm				sub		eax, __TILE_XL \
__asm				sub		ebx, __TILE_XL_FLOW \
__asm			__step_tile_x_inc: \
__asm				mov		__TILE_NOW_X, eax

// �^�C�������w���Z�i���C���p�j
#define		__TILE_PROC_X_DEC \
__asm				mov		eax, __TILE_NOW_X \
__asm				dec		eax \
__asm				dec		ebx \
__asm				cmp		eax, 0 \
__asm				jg		__step_tile_x_dec \
__asm				add		eax, __TILE_XL \
__asm				add		ebx, __TILE_XL_FLOW \
__asm			__step_tile_x_dec: \
__asm				mov		__TILE_NOW_X, eax

// �^�C�������x���Z�i���C���p�j
#define		__TILE_PROC_Y_INC \
__asm				mov		eax, __TILE_NOW_Y \
__asm				inc		eax \
__asm				add		ebx, __TILE_XL \
__asm				cmp		eax, __TILE_YL \
__asm				jl		__step_tile_y_inc \
__asm				sub		eax, __TILE_YL \
__asm				sub		ebx, __TILE_YL_FLOW \
__asm			__step_tile_y_inc: \
__asm				mov		__TILE_NOW_Y, eax

// �^�C�������x���Z�i���C���p�j
#define		__TILE_PROC_Y_DEC \
__asm				mov		eax, __TILE_NOW_Y \
__asm				dec		eax \
__asm				sub		ebx, __TILE_XL \
__asm				cmp		eax, 0 \
__asm				jg		__step_tile_y_dec \
__asm				add		eax, __TILE_YL \
__asm				add		ebx, __TILE_YL_FLOW \
__asm			__step_tile_y_dec: \
__asm				mov		__TILE_NOW_Y, eax

// ****************************************************************
// �ʏ��p�`��
// ================================================================
#define		__DRAW_STDGRP_STD_NORMAL_EX \
__asm				cld \
__asm				mov		edi, __DST_PTR \
__asm				mov		esi, __SRC_PTR \
__asm				mov		ecx, __DRAW_YCNT \
__asm				mov		ebx, __DRAW_XCNT \
__asm				mov		edx, __DST_NEXT \
__asm				mov		eax, __SRC_NEXT \
__asm			__loop_y: \
__asm				push	ecx \
__asm				mov		ecx, ebx \
__asm				rep		movsd \
__asm				pop		ecx \
__asm				add		edi, edx \
__asm				add		esi, eax \
__asm				loop	__loop_y

// �{�b�N�X��
#define		__DRAW_STDBOX_STD_NORMAL_EX \
__asm				cld \
__asm				mov		edi, __DST_PTR \
__asm				mov		esi, __SRC_PTR \
__asm				mov		ecx, __DRAW_YCNT \
__asm				mov		ebx, __DRAW_XCNT \
__asm				mov		edx, __DST_NEXT \
__asm				mov		eax, [esi] \
__asm			__loop_y: \
__asm				push	ecx \
__asm				mov		ecx, ebx \
__asm				rep		stosd \
__asm				pop		ecx \
__asm				add		edi, edx \
__asm				loop	__loop_y

// �A���t�@������
#define		__DRAW_BLDSTDGRP_BLD_NORMAL_EX		__DRAW_STDGRP_STD_NORMAL_EX
#define		__DRAW_BLDSTDBOX_BLD_NORMAL_EX		__DRAW_STDBOX_STD_NORMAL_EX

// ****************************************************************
// �X�v���C�g��p�`��
// ================================================================
#define		__DRAW_STDGRP_STD_SPRITE_EX \
__asm				cld \
__asm				mov		edi, __DST_PTR \
__asm				mov		esi, __SRC_PTR \
__asm				mov		ecx, __DRAW_YCNT \
__asm			__loop_y: \
__asm				push	ecx \
__asm				mov		ecx, __DRAW_XCNT \
__asm			__loop_x: \
__asm				xor		eax, eax \
__asm				mov		al, [esi+3] \
__asm				and		eax, eax \
__asm				jz		__dont_draw \
__asm				mov		ebx, eax \
__asm				xor		eax, 0xff \
__asm				jnz		__draw \
__asm				movsd \
__asm				jmp		__next \
__asm			__draw: \
					__MOV_EDX_TR_TABLE_ADR \
					__DRAW_STD_TR \
__asm			__dont_draw: \
__asm				add		esi, 4 \
__asm				add		edi, 4 \
__asm			__next: \
__asm				loop	__loop_x \
__asm				add		esi, __SRC_NEXT \
__asm				add		edi, __DST_NEXT \
__asm				pop		ecx \
__asm				loop	__loop_y

// �{�b�N�X��
#define		__DRAW_STDBOX_STD_SPRITE_EX \
__asm				cld \
__asm				mov		edi, __DST_PTR \
__asm				mov		esi, __SRC_PTR \
__asm				mov		ecx, __DRAW_YCNT \
__asm			__loop_y: \
__asm				push	ecx \
__asm				mov		ecx, __DRAW_XCNT \
__asm			__loop_x: \
__asm				xor		eax, eax \
__asm				mov		al, [esi+3] \
__asm				and		eax, eax \
__asm				jz		__dont_draw \
__asm				mov		ebx, eax \
__asm				xor		eax, 0xff \
__asm				jnz		__draw \
__asm				mov		eax, [esi] \
__asm				stosd \
__asm				jmp		__next \
__asm			__draw: \
					__MOV_EDX_TR_TABLE_ADR \
					__DRAW_STD_TR \
__asm			__dont_draw: \
__asm				add		edi, 4 \
__asm			__next: \
__asm				loop	__loop_x \
__asm				add		edi, __DST_NEXT \
__asm				pop		ecx \
__asm				loop	__loop_y

