#pragma		once

// ****************************************************************
// ��{
// ================================================================

// �J�n
#define		__START_BLDSTDBOX			__START_STDBOX

// �I���w
#define		__END_BLDSTDBOX_X			__END_STDBOX_X

// �I���x
#define		__END_BLDSTDBOX_Y			__END_STDBOX_Y

// ****************************************************************
// �ʏ�
// ================================================================

// �J�n
#define		__START_BLDSTDBOX_NORMAL \
					__START_BLDSTDBOX \
__asm			__loop_x:

// �I��
#define		__END_BLDSTDBOX_NORMAL \
					__END_BLDSTDBOX_X \
					__END_BLDSTDBOX_Y

// ****************************************************************
// �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDSTDBOX_SPRITE \
					__START_BLDSTDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_TR_ALPHABLEND

// �I��
#define		__END_BLDSTDBOX_SPRITE \
__asm			__dont_draw: \
					__END_BLDSTDBOX_NORMAL

// ****************************************************************
// ���� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDSTDBOX_TR_NORMAL \
					__START_BLDSTDBOX_NORMAL

// �I��
#define		__END_BLDSTDBOX_TR_NORMAL \
					__END_BLDSTDBOX_NORMAL

// ****************************************************************
// ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDSTDBOX_TR_SPRITE \
					__START_BLDSTDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_TR_ALPHABLEND

// �I��
#define		__END_BLDSTDBOX_TR_SPRITE \
					__END_BLDSTDBOX_SPRITE

// ****************************************************************
// �^�C�� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDSTDBOX_TILE \
					__START_BLDSTDBOX \
__asm				mov		eax, __TILE_START_X \
__asm				mov		__TILE_NOW_X, eax \
__asm			__loop_x:

// �J�n
#define		__START_BLDSTDBOX_TILE_NORMAL \
					__START_BLDSTDBOX_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_ALPHABLEND

// �J�n�d�w�i�����x���Q�T�T�̎��͓����x���������Ȃ��o�[�W�����^�ʏ�^�C���`���p�j
#define		__START_BLDSTDBOX_TILE_NORMAL_EX \
					__START_BLDSTDBOX_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_EX_ALPHABLEND

// �I��
#define		__END_BLDSTDBOX_TILE_NORMAL \
__asm			__dont_draw: \
					__TILE_PROC_X \
					__END_BLDSTDBOX_X \
					__TILE_PROC_Y \
					__END_BLDSTDBOX_Y

// ****************************************************************
// �^�C�� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDSTDBOX_TILE_SPRITE \
					__START_BLDSTDBOX_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// �I��
#define		__END_BLDSTDBOX_TILE_SPRITE \
					__END_BLDSTDBOX_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDSTDBOX_TILE_TR_NORMAL \
					__START_BLDSTDBOX_TILE_NORMAL

// �I��
#define		__END_BLDSTDBOX_TILE_TR_NORMAL \
					__END_BLDSTDBOX_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDSTDBOX_TILE_TR_SPRITE \
					__START_BLDSTDBOX_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// �I��
#define		__END_BLDSTDBOX_TILE_TR_SPRITE \
					__END_BLDSTDBOX_TILE_NORMAL

