#pragma		once

// ****************************************************************
// ��{
// ================================================================

// �J�n
#define		__START_CHGSTDBOX			__START_STDBOX

// �I���w
#define		__END_CHGSTDBOX_X			__END_STDBOX_X

// �I���x
#define		__END_CHGSTDBOX_Y			__END_STDBOX_Y

// ****************************************************************
// �ʏ�
// ================================================================

// �J�n
#define		__START_CHGSTDBOX_NORMAL \
					__START_CHGSTDBOX \
__asm			__loop_x: \
__asm				mov		esi, edi

// �I��
#define		__END_CHGSTDBOX_NORMAL \
					__END_CHGSTDBOX_X \
					__END_CHGSTDBOX_Y

// ****************************************************************
// �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGSTDBOX_SPRITE \
					__START_CHGSTDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_EDX_TR_TABLE_ADR

// �I��
#define		__END_CHGSTDBOX_SPRITE \
__asm			__dont_draw: \
					__END_CHGSTDBOX_NORMAL

// ****************************************************************
// ���� �ʏ�
// ================================================================

// �J�n
#define		__START_CHGSTDBOX_TR_NORMAL \
					__START_CHGSTDBOX_NORMAL

// �I��
#define		__END_CHGSTDBOX_TR_NORMAL \
					__END_CHGSTDBOX_NORMAL

// ****************************************************************
// ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGSTDBOX_TR_SPRITE \
					__START_CHGSTDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_EDX_TR_TABLE_ADR

// �I��
#define		__END_CHGSTDBOX_TR_SPRITE \
					__END_CHGSTDBOX_SPRITE

// ****************************************************************
// �^�C�� �ʏ�
// ================================================================

// �J�n
#define		__START_CHGSTDBOX_TILE \
					__START_CHGSTDBOX \
__asm				mov		eax, __TILE_START_X \
__asm				mov		__TILE_NOW_X, eax \
__asm			__loop_x: \
__asm				mov		esi, edi

// �J�n
#define		__START_CHGSTDBOX_TILE_NORMAL \
					__START_CHGSTDBOX_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET

// �I��
#define		__END_CHGSTDBOX_TILE_NORMAL \
__asm			__dont_draw: \
					__TILE_PROC_X \
					__END_CHGSTDBOX_X \
					__TILE_PROC_Y \
					__END_CHGSTDBOX_Y

// ****************************************************************
// �^�C�� �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGSTDBOX_TILE_SPRITE \
					__START_CHGSTDBOX_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// �I��
#define		__END_CHGSTDBOX_TILE_SPRITE \
					__END_CHGSTDBOX_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �ʏ�
// ================================================================

// �J�n
#define		__START_CHGSTDBOX_TILE_TR_NORMAL \
					__START_CHGSTDBOX_TILE_NORMAL

// �I��
#define		__END_CHGSTDBOX_TILE_TR_NORMAL \
					__END_CHGSTDBOX_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGSTDBOX_TILE_TR_SPRITE \
					__START_CHGSTDBOX_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// �I��
#define		__END_CHGSTDBOX_TILE_TR_SPRITE \
					__END_CHGSTDBOX_TILE_NORMAL

