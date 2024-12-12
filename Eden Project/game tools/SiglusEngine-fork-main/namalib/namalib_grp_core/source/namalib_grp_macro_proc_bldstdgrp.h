#pragma		once

// ****************************************************************
// ��{
// ================================================================

// �J�n
#define		__START_BLDSTDGRP			__START_STDGRP

// �I���w
#define		__END_BLDSTDGRP_X			__END_STDGRP_X

// �I���x
#define		__END_BLDSTDGRP_Y			__END_STDGRP_Y

// ****************************************************************
// �ʏ�
// ================================================================

// �J�n
#define		__START_BLDSTDGRP_NORMAL \
					__START_BLDSTDGRP \
__asm			__loop_x:

// �I��
#define		__END_BLDSTDGRP_NORMAL \
					__END_BLDSTDGRP_X \
					__END_BLDSTDGRP_Y

// ****************************************************************
// �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDSTDGRP_SPRITE \
					__START_BLDSTDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_TR_ALPHABLEND

// �I��
#define		__END_BLDSTDGRP_SPRITE \
__asm			__dont_draw: \
					__END_BLDSTDGRP_NORMAL

// ****************************************************************
// ���� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDSTDGRP_TR_NORMAL \
					__START_BLDSTDGRP_NORMAL

// �I��
#define		__END_BLDSTDGRP_TR_NORMAL \
					__END_BLDSTDGRP_NORMAL

// ****************************************************************
// ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDSTDGRP_TR_SPRITE \
					__START_BLDSTDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_TR_ALPHABLEND

// �I��
#define		__END_BLDSTDGRP_TR_SPRITE \
					__END_BLDSTDGRP_SPRITE

// ****************************************************************
// �^�C�� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDSTDGRP_TILE \
					__START_BLDSTDGRP \
__asm				mov		eax, __TILE_START_X \
__asm				mov		__TILE_NOW_X, eax \
__asm			__loop_x:

// �J�n
#define		__START_BLDSTDGRP_TILE_NORMAL \
					__START_BLDSTDGRP_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_ALPHABLEND

// �J�n�d�w�i�����x���Q�T�T�̎��͓����x���������Ȃ��o�[�W�����^�ʏ�^�C���`���p�j
#define		__START_BLDSTDGRP_TILE_NORMAL_EX \
					__START_BLDSTDGRP_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_EX_ALPHABLEND

// �I��
#define		__END_BLDSTDGRP_TILE_NORMAL \
__asm			__dont_draw: \
					__TILE_PROC_X \
					__END_BLDSTDGRP_X \
					__TILE_PROC_Y \
					__END_BLDSTDGRP_Y

// ****************************************************************
// �^�C�� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDSTDGRP_TILE_SPRITE \
					__START_BLDSTDGRP_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// �I��
#define		__END_BLDSTDGRP_TILE_SPRITE \
					__END_BLDSTDGRP_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDSTDGRP_TILE_TR_NORMAL \
					__START_BLDSTDGRP_TILE_NORMAL

// �I��
#define		__END_BLDSTDGRP_TILE_TR_NORMAL \
					__END_BLDSTDGRP_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDSTDGRP_TILE_TR_SPRITE \
					__START_BLDSTDGRP_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// �I��
#define		__END_BLDSTDGRP_TILE_TR_SPRITE \
					__END_BLDSTDGRP_TILE_NORMAL

