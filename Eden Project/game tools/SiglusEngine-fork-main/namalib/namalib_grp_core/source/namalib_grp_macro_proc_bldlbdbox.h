#pragma		once

// ****************************************************************
// ��{
// ================================================================

// �J�n
#define		__START_BLDLBDBOX			__START_LBDBOX

// �I���i�O�����j
#define		__END_BLDLBDBOX_BEFOR		__END_LBDBOX_BEFOR

// �I���i�㏈���j
#define		__END_BLDLBDBOX_AFTER		__END_LBDBOX_AFTER

// ****************************************************************
// �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLBDBOX_NORMAL \
					__START_BLDLBDBOX \
__asm				mov		edx, __TR_ADR

// �I��
#define		__END_BLDLBDBOX_NORMAL \
					__END_BLDLBDBOX_BEFOR \
					__END_BLDLBDBOX_AFTER

// ****************************************************************
// �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLBDBOX_SPRITE \
					__START_BLDLBDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_TR_ALPHABLEND

// �I��
#define		__END_BLDLBDBOX_SPRITE \
__asm			__dont_draw: \
					__END_BLDLBDBOX_NORMAL

// ****************************************************************
// ���� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLBDBOX_TR_NORMAL \
					__START_BLDLBDBOX_NORMAL

// �I��
#define		__END_BLDLBDBOX_TR_NORMAL \
					__END_BLDLBDBOX_NORMAL

// ****************************************************************
// ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLBDBOX_TR_SPRITE \
					__START_BLDLBDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_TR_ALPHABLEND

// �I��
#define		__END_BLDLBDBOX_TR_SPRITE \
					__END_BLDLBDBOX_SPRITE

// ****************************************************************
// �^�C�� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLBDBOX_TILE_NORMAL \
					__START_BLDLBDBOX \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_ALPHABLEND

// �I��
#define		__END_BLDLBDBOX_TILE_NORMAL \
__asm			__dont_draw: \
					__END_BLDLBDBOX_BEFOR \
					__TILE_PROC_X \
					__END_BLDLBDBOX_AFTER

// ****************************************************************
// �^�C�� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLBDBOX_TILE_SPRITE \
					__START_BLDLBDBOX \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// �I��
#define		__END_BLDLBDBOX_TILE_SPRITE \
					__END_BLDLBDBOX_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLBDBOX_TILE_TR_NORMAL \
					__START_BLDLBDBOX_TILE_NORMAL

// �I��
#define		__END_BLDLBDBOX_TILE_TR_NORMAL \
					__END_BLDLBDBOX_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLBDBOX_TILE_TR_SPRITE \
					__START_BLDLBDBOX \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// �I��
#define		__END_BLDLBDBOX_TILE_TR_SPRITE \
					__END_BLDLBDBOX_TILE_NORMAL

