#pragma		once

// ****************************************************************
// ��{
// ================================================================

// �J�n
#define		__START_LBDBOX \
__asm				cld \
__asm				mov		esi, __LBDP_SRC_PTR \
__asm				mov		edi, __LBDP_DST_PTR \
__asm				mov		edx, __LBDP_DST_ALPHA \
__asm				mov		ecx, __LBDP_CNT \
__asm				inc		ecx \
__asm			__loop_y: \
__asm				push	ecx \
__asm				add		edx, __LBDP_DST_SIZE \
__asm				jns		__loop_x \
__asm				jmp		__loop_x_end \
__asm			__loop_x: \
__asm				cmp		edi, __LBDP_DST_MIN \
__asm				jge		__draw_step \
__asm				jmp		__draw_end \
__asm			__draw_step: \
__asm				cmp		edi, __LBDP_DST_MAX \
__asm				jle		__draw_start \
__asm				pop		ecx \
__asm				jmp		__loop_y_end \
__asm			__draw_start: \
__asm				push	edx

// �I���i�O�����j
#define		__END_LBDBOX_BEFOR \
__asm				pop		edx \
__asm			__draw_end:

// �I���i�㏈���j
#define		__END_LBDBOX_AFTER \
__asm				add		edi, 4 \
__asm				sub		edx, __LBDP_DST_CNT \
__asm				js		__loop_x_end \
__asm				jmp		__loop_x \
__asm			__loop_x_end: \
__asm				pop		ecx \
__asm				dec		ecx \
__asm				jz		__loop_y_end \
__asm				jmp		__loop_y \
__asm			__loop_y_end:


// ****************************************************************
// �ʏ�
// ================================================================

// �J�n
#define		__START_LBDBOX_NORMAL \
					__START_LBDBOX \
__asm				mov		edx, __TR_ADR

// �I��
#define		__END_LBDBOX_NORMAL \
					__END_LBDBOX_BEFOR \
					__END_LBDBOX_AFTER

// ****************************************************************
// �X�v���C�g
// ================================================================

// �J�n
#define		__START_LBDBOX_SPRITE \
					__START_LBDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_EDX_TR_TABLE_ADR

// �I��
#define		__END_LBDBOX_SPRITE \
__asm			__dont_draw: \
					__END_LBDBOX_NORMAL

// ****************************************************************
// ���� �ʏ�
// ================================================================

// �J�n
#define		__START_LBDBOX_TR_NORMAL \
					__START_LBDBOX_NORMAL

// �I��
#define		__END_LBDBOX_TR_NORMAL \
					__END_LBDBOX_NORMAL

// ****************************************************************
// ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_LBDBOX_TR_SPRITE \
					__START_LBDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_EDX_TR_TABLE_ADR

// �I��
#define		__END_LBDBOX_TR_SPRITE \
					__END_LBDBOX_SPRITE

// ****************************************************************
// �^�C�� �ʏ�
// ================================================================

// �J�n
#define		__START_LBDBOX_TILE_NORMAL \
					__START_LBDBOX \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET

// �I��
#define		__END_LBDBOX_TILE_NORMAL \
__asm			__dont_draw: \
					__END_LBDBOX_BEFOR \
					__TILE_PROC_X \
					__END_LBDBOX_AFTER

// ****************************************************************
// �^�C�� �X�v���C�g
// ================================================================

// �J�n
#define		__START_LBDBOX_TILE_SPRITE \
					__START_LBDBOX \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// �I��
#define		__END_LBDBOX_TILE_SPRITE \
					__END_LBDBOX_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �ʏ�
// ================================================================

// �J�n
#define		__START_LBDBOX_TILE_TR_NORMAL \
					__START_LBDBOX_TILE_NORMAL

// �I��
#define		__END_LBDBOX_TILE_TR_NORMAL \
					__END_LBDBOX_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_LBDBOX_TILE_TR_SPRITE \
					__START_LBDBOX \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// �I��
#define		__END_LBDBOX_TILE_TR_SPRITE \
					__END_LBDBOX_TILE_NORMAL

