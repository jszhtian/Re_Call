#pragma		once

// ****************************************************************
// ��{�i�摜�`��p�j
// ================================================================

// �J�n
#define		__START_BLDLINED				__START_LINED

// �I��
#define		__END_BLDLINED					__END_LINED

// �J�n�i�^�C���Łj
#define		__START_BLDLINED_TILE			__START_LINED_TILE

// ****************************************************************
// �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLINED_NORMAL \
					__START_BLDLINED \
__asm				mov		edx, __TR_ADR

// �I��
#define		__END_BLDLINED_NORMAL \
					__END_BLDLINED

// ****************************************************************
// �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLINED_SPRITE \
					__START_BLDLINED_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_TR_ALPHABLEND

// �I��
#define		__END_BLDLINED_SPRITE \
__asm			__dont_draw: \
					__END_BLDLINED_NORMAL

// ****************************************************************
// ���� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLINED_TR_NORMAL \
					__START_BLDLINED_NORMAL

// �I��
#define		__END_BLDLINED_TR_NORMAL \
					__END_BLDLINED_NORMAL

// ****************************************************************
// ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLINED_TR_SPRITE \
					__START_BLDLINED_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_TR_ALPHABLEND

// �I��
#define		__END_BLDLINED_TR_SPRITE \
					__END_BLDLINED_SPRITE

// ****************************************************************
// �^�C�� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLINED_TILE_NORMAL \
					__START_BLDLINED_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_LD_ALPHABLEND

// �I��
#define		__END_BLDLINED_TILE_NORMAL \
__asm			__dont_draw: \
					__END_BLDLINED

// ****************************************************************
// �^�C�� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLINED_TILE_SPRITE \
					__START_BLDLINED_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_LD_ALPHABLEND

// �I��
#define		__END_BLDLINED_TILE_SPRITE \
					__END_BLDLINED_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLINED_TILE_TR_NORMAL \
					__START_BLDLINED_TILE_NORMAL

// �I��
#define		__END_BLDLINED_TILE_TR_NORMAL \
					__END_BLDLINED_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLINED_TILE_TR_SPRITE \
					__START_BLDLINED_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_LD_ALPHABLEND

// �I��
#define		__END_BLDLINED_TILE_TR_SPRITE \
					__END_BLDLINED_TILE_NORMAL

