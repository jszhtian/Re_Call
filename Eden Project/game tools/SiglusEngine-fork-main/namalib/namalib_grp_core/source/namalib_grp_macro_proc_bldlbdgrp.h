#pragma		once

// ****************************************************************
// ��{
// ================================================================

// �J�n
#define		__START_BLDLBDGRP			__START_LBDGRP

// �I���i�O�����j
#define		__END_BLDLBDGRP_BEFOR		__END_LBDGRP_BEFOR

// �I���i�㏈���j
#define		__END_BLDLBDGRP_AFTER		__END_LBDGRP_AFTER

// ****************************************************************
// �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLBDGRP_NORMAL \
					__START_BLDLBDGRP \
__asm				mov		edx, __TR_ADR

// �I��
#define		__END_BLDLBDGRP_NORMAL \
					__END_BLDLBDGRP_BEFOR \
					__END_BLDLBDGRP_AFTER

// ****************************************************************
// �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLBDGRP_SPRITE \
					__START_BLDLBDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_TR_ALPHABLEND

// �I��
#define		__END_BLDLBDGRP_SPRITE \
__asm			__dont_draw: \
					__END_BLDLBDGRP_NORMAL

// ****************************************************************
// ���� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLBDGRP_TR_NORMAL \
					__START_BLDLBDGRP_NORMAL

// �I��
#define		__END_BLDLBDGRP_TR_NORMAL \
					__END_BLDLBDGRP_NORMAL

// ****************************************************************
// ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLBDGRP_TR_SPRITE \
					__START_BLDLBDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_TR_ALPHABLEND

// �I��
#define		__END_BLDLBDGRP_TR_SPRITE \
					__END_BLDLBDGRP_SPRITE

// ****************************************************************
// �^�C�� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLBDGRP_TILE_NORMAL \
					__START_BLDLBDGRP \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_ALPHABLEND

// �I��
#define		__END_BLDLBDGRP_TILE_NORMAL \
__asm			__dont_draw: \
					__END_BLDLBDGRP_BEFOR \
					__TILE_PROC_X \
					__END_BLDLBDGRP_AFTER

// ****************************************************************
// �^�C�� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLBDGRP_TILE_SPRITE \
					__START_BLDLBDGRP \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// �I��
#define		__END_BLDLBDGRP_TILE_SPRITE \
					__END_BLDLBDGRP_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �ʏ�
// ================================================================

// �J�n
#define		__START_BLDLBDGRP_TILE_TR_NORMAL \
					__START_BLDLBDGRP_TILE_NORMAL

// �I��
#define		__END_BLDLBDGRP_TILE_TR_NORMAL \
					__END_BLDLBDGRP_TILE_NORMAL

// ****************************************************************
// �^�C�� ���� �X�v���C�g
// ================================================================

// �J�n
#define		__START_BLDLBDGRP_TILE_TR_SPRITE \
					__START_BLDLBDGRP \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// �I��
#define		__END_BLDLBDGRP_TILE_TR_SPRITE \
					__END_BLDLBDGRP_TILE_NORMAL

