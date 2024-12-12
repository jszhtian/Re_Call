#pragma		once

// ****************************************************************
// 基本
// ================================================================

// 開始
#define		__START_BLDSTDGRP			__START_STDGRP

// 終了Ｘ
#define		__END_BLDSTDGRP_X			__END_STDGRP_X

// 終了Ｙ
#define		__END_BLDSTDGRP_Y			__END_STDGRP_Y

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_BLDSTDGRP_NORMAL \
					__START_BLDSTDGRP \
__asm			__loop_x:

// 終了
#define		__END_BLDSTDGRP_NORMAL \
					__END_BLDSTDGRP_X \
					__END_BLDSTDGRP_Y

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_BLDSTDGRP_SPRITE \
					__START_BLDSTDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_TR_ALPHABLEND

// 終了
#define		__END_BLDSTDGRP_SPRITE \
__asm			__dont_draw: \
					__END_BLDSTDGRP_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_BLDSTDGRP_TR_NORMAL \
					__START_BLDSTDGRP_NORMAL

// 終了
#define		__END_BLDSTDGRP_TR_NORMAL \
					__END_BLDSTDGRP_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_BLDSTDGRP_TR_SPRITE \
					__START_BLDSTDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_TR_ALPHABLEND

// 終了
#define		__END_BLDSTDGRP_TR_SPRITE \
					__END_BLDSTDGRP_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_BLDSTDGRP_TILE \
					__START_BLDSTDGRP \
__asm				mov		eax, __TILE_START_X \
__asm				mov		__TILE_NOW_X, eax \
__asm			__loop_x:

// 開始
#define		__START_BLDSTDGRP_TILE_NORMAL \
					__START_BLDSTDGRP_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_ALPHABLEND

// 開始ＥＸ（透明度が２５５の時は透明度処理をしないバージョン／通常タイル描画専用）
#define		__START_BLDSTDGRP_TILE_NORMAL_EX \
					__START_BLDSTDGRP_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_EX_ALPHABLEND

// 終了
#define		__END_BLDSTDGRP_TILE_NORMAL \
__asm			__dont_draw: \
					__TILE_PROC_X \
					__END_BLDSTDGRP_X \
					__TILE_PROC_Y \
					__END_BLDSTDGRP_Y

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_BLDSTDGRP_TILE_SPRITE \
					__START_BLDSTDGRP_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// 終了
#define		__END_BLDSTDGRP_TILE_SPRITE \
					__END_BLDSTDGRP_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_BLDSTDGRP_TILE_TR_NORMAL \
					__START_BLDSTDGRP_TILE_NORMAL

// 終了
#define		__END_BLDSTDGRP_TILE_TR_NORMAL \
					__END_BLDSTDGRP_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_BLDSTDGRP_TILE_TR_SPRITE \
					__START_BLDSTDGRP_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// 終了
#define		__END_BLDSTDGRP_TILE_TR_SPRITE \
					__END_BLDSTDGRP_TILE_NORMAL

