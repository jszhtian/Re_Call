#pragma		once

// ****************************************************************
// ��{
// ================================================================

// �J�n
#define		__START_CHGLBDBOX \
					__START_LBDBOX \
__asm				mov		esi, edi

// �I���i�O�����j
#define		__END_CHGLBDBOX_BEFOR			__END_LBDBOX_BEFOR

// �I���i�㏈���j
#define		__END_CHGLBDBOX_AFTER			__END_LBDBOX_AFTER

// ****************************************************************
// �ʏ�
// ================================================================

// �J�n
#define		__START_CHGLBDBOX_NORMAL \
					__START_CHGLBDBOX \
__asm				mov		edx, __TR_ADR

// �I��
#define		__END_CHGLBDBOX_NORMAL \
					__END_CHGLBDBOX_BEFOR \
					__END_CHGLBDBOX_AFTER

// ****************************************************************
// �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGLBDBOX_SPRITE \
					__START_CHGLBDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_EDX_TR_TABLE_ADR

// �I��
#define		__END_CHGLBDBOX_SPRITE \
__asm			__dont_draw: \
					__END_CHGLBDBOX_NORMAL

// ****************************************************************
// ���� �ʏ�
// ================================================================

// �J�n
#define		__START_CHGLBDBOX_TR_NORMAL \
					__START_CHGLBDBOX_NORMAL

// �I��
#define		__END_CHGLBDBOX_TR_NORMAL \
					__END_CHGLBDBOX_NORMAL

// ****************************************************************
// ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGLBDBOX_TR_SPRITE \
					__START_CHGLBDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_EDX_TR_TABLE_ADR

// �I��
#define		__END_CHGLBDBOX_TR_SPRITE \
					__END_CHGLBDBOX_SPRITE

// ****************************************************************
// �^�C�� �ʏ�
// ================================================================

// �J�n
#define		__START_CHGLBDBOX_TILE_NORMAL \
					__START_CHGLBDBOX \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET

// �I��
#define		__END_CHGLBDBOX_TILE_NORMAL \
__asm			__dont_draw: \
					__END_CHGLBDBOX_BEFOR \
					__TILE_PROC_X \
					__END_CHGLBDBOX_AFTER

// ****************************************************************
// �^�C�� �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGLBDBOX_TILE_SPRITE \
					__START_CHGLBDBOX \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// �I��
#define		__END_CHGLBDBOX_TILE_SPRITE \
					__END_CHGLBDBOX_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �ʏ�
// ================================================================

// �J�n
#define		__START_CHGLBDBOX_TILE_TR_NORMAL \
					__START_CHGLBDBOX_TILE_NORMAL

// �I��
#define		__END_CHGLBDBOX_TILE_TR_NORMAL \
					__END_CHGLBDBOX_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_CHGLBDBOX_TILE_TR_SPRITE \
					__START_CHGLBDBOX \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// �I��
#define		__END_CHGLBDBOX_TILE_TR_SPRITE \
					__END_CHGLBDBOX_TILE_NORMAL

