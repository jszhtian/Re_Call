#pragma		once

// ****************************************************************
// ��{�i�摜�`��p�j
// ================================================================

// �J�n
#define		__START_CHGLINED \
					__START_LINED_FUNC \
__asm				mov		esi, edi

// �I��
#define		__END_CHGLINED				__END_LINED

// �J�n�i�^�C���Łj
#define		__START_CHGLINED_TILE \
					__START_LINED_TILE_FUNC \
__asm				mov		esi, edi

// ****************************************************************
// �ʏ�
// ================================================================

// �J�n
#define		__START_CHGLINED_NORMAL \
					__START_CHGLINED \
__asm				mov		edx, __TR_ADR

// �I��
#define		__END_CHGLINED_NORMAL \
					__END_CHGLINED

// ****************************************************************
// �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGLINED_SPRITE \
					__START_CHGLINED_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_EDX_TR_TABLE_ADR

// �I��
#define		__END_CHGLINED_SPRITE \
__asm			__dont_draw: \
					__END_CHGLINED_NORMAL

// ****************************************************************
// ���� �ʏ�
// ================================================================

// �J�n
#define		__START_CHGLINED_TR_NORMAL \
					__START_CHGLINED_NORMAL

// �I��
#define		__END_CHGLINED_TR_NORMAL \
					__END_CHGLINED_NORMAL

// ****************************************************************
// ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGLINED_TR_SPRITE \
					__START_CHGLINED_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_EDX_TR_TABLE_ADR

// �I��
#define		__END_CHGLINED_TR_SPRITE \
					__END_CHGLINED_SPRITE

// ****************************************************************
// �^�C�� �ʏ�
// ================================================================

// �J�n
#define		__START_CHGLINED_TILE_NORMAL \
					__START_CHGLINED_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_LD

// �I��
#define		__END_CHGLINED_TILE_NORMAL \
__asm			__dont_draw: \
					__END_CHGLINED

// ****************************************************************
// �^�C�� �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGLINED_TILE_SPRITE \
					__START_CHGLINED_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_LD

// �I��
#define		__END_CHGLINED_TILE_SPRITE \
					__END_CHGLINED_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �ʏ�
// ================================================================

// �J�n
#define		__START_CHGLINED_TILE_TR_NORMAL \
					__START_CHGLINED_TILE_NORMAL

// �I��
#define		__END_CHGLINED_TILE_TR_NORMAL \
					__END_CHGLINED_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGLINED_TILE_TR_SPRITE \
					__START_CHGLINED_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_LD

// �I��
#define		__END_CHGLINED_TILE_TR_SPRITE \
					__END_CHGLINED_TILE_NORMAL

