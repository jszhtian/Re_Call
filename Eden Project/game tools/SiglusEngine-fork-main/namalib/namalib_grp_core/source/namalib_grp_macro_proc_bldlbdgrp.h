#pragma		once

// ****************************************************************
// 基本
// ================================================================

// 開始
#define		__START_BLDLBDGRP			__START_LBDGRP

// 終了（前処理）
#define		__END_BLDLBDGRP_BEFOR		__END_LBDGRP_BEFOR

// 終了（後処理）
#define		__END_BLDLBDGRP_AFTER		__END_LBDGRP_AFTER

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_BLDLBDGRP_NORMAL \
					__START_BLDLBDGRP \
__asm				mov		edx, __TR_ADR

// 終了
#define		__END_BLDLBDGRP_NORMAL \
					__END_BLDLBDGRP_BEFOR \
					__END_BLDLBDGRP_AFTER

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_BLDLBDGRP_SPRITE \
					__START_BLDLBDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_TR_ALPHABLEND

// 終了
#define		__END_BLDLBDGRP_SPRITE \
__asm			__dont_draw: \
					__END_BLDLBDGRP_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_BLDLBDGRP_TR_NORMAL \
					__START_BLDLBDGRP_NORMAL

// 終了
#define		__END_BLDLBDGRP_TR_NORMAL \
					__END_BLDLBDGRP_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_BLDLBDGRP_TR_SPRITE \
					__START_BLDLBDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_TR_ALPHABLEND

// 終了
#define		__END_BLDLBDGRP_TR_SPRITE \
					__END_BLDLBDGRP_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_BLDLBDGRP_TILE_NORMAL \
					__START_BLDLBDGRP \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_ALPHABLEND

// 終了
#define		__END_BLDLBDGRP_TILE_NORMAL \
__asm			__dont_draw: \
					__END_BLDLBDGRP_BEFOR \
					__TILE_PROC_X \
					__END_BLDLBDGRP_AFTER

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_BLDLBDGRP_TILE_SPRITE \
					__START_BLDLBDGRP \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// 終了
#define		__END_BLDLBDGRP_TILE_SPRITE \
					__END_BLDLBDGRP_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_BLDLBDGRP_TILE_TR_NORMAL \
					__START_BLDLBDGRP_TILE_NORMAL

// 終了
#define		__END_BLDLBDGRP_TILE_TR_NORMAL \
					__END_BLDLBDGRP_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_BLDLBDGRP_TILE_TR_SPRITE \
					__START_BLDLBDGRP \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// 終了
#define		__END_BLDLBDGRP_TILE_TR_SPRITE \
					__END_BLDLBDGRP_TILE_NORMAL

