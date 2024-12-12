#pragma		once

// ****************************************************************
// 基本
// ================================================================

// 開始
#define		__START_STDGRP \
__asm				cld \
__asm				mov		edi, __DST_PTR \
__asm				mov		esi, __SRC_PTR \
__asm				mov		ecx, __DRAW_YCNT \
__asm				mov		edx, __TR_ADR \
__asm			__loop_y: \
__asm				push	ecx \
__asm				mov		ecx, __DRAW_XCNT

// 終了Ｘ
#define		__END_STDGRP_X \
__asm				add		esi, 4 \
__asm				add		edi, 4 \
__asm				dec		ecx \
__asm				jz		__loop_x_end \
__asm				jmp		__loop_x \
__asm			__loop_x_end:

// 終了Ｙ
#define		__END_STDGRP_Y \
__asm				add		esi, __SRC_NEXT \
__asm				add		edi, __DST_NEXT \
__asm				pop		ecx \
__asm				dec		ecx \
__asm				jz		__loop_y_end \
__asm				jmp		__loop_y \
__asm			__loop_y_end:

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_STDGRP_NORMAL \
					__START_STDGRP \
__asm			__loop_x:

// 終了
#define		__END_STDGRP_NORMAL \
					__END_STDGRP_X \
					__END_STDGRP_Y

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_STDGRP_SPRITE \
					__START_STDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_STDGRP_SPRITE \
__asm			__dont_draw: \
					__END_STDGRP_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_STDGRP_TR_NORMAL \
					__START_STDGRP_NORMAL

// 終了
#define		__END_STDGRP_TR_NORMAL \
					__END_STDGRP_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_STDGRP_TR_SPRITE \
					__START_STDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_STDGRP_TR_SPRITE \
					__END_STDGRP_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_STDGRP_TILE \
					__START_STDGRP \
__asm				mov		eax, __TILE_START_X \
__asm				mov		__TILE_NOW_X, eax \
__asm			__loop_x:

// 開始
#define		__START_STDGRP_TILE_NORMAL \
					__START_STDGRP_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET

// 開始ＥＸ（透明度が２５５の時は透明度処理をしないバージョン／通常タイル描画専用）
#define		__START_STDGRP_TILE_NORMAL_EX \
					__START_STDGRP_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_EX

// 終了
#define		__END_STDGRP_TILE_NORMAL \
__asm			__dont_draw: \
					__TILE_PROC_X \
					__END_STDGRP_X \
					__TILE_PROC_Y \
					__END_STDGRP_Y

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_STDGRP_TILE_SPRITE \
					__START_STDGRP_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// 終了
#define		__END_STDGRP_TILE_SPRITE \
					__END_STDGRP_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_STDGRP_TILE_TR_NORMAL \
					__START_STDGRP_TILE_NORMAL

// 終了
#define		__END_STDGRP_TILE_TR_NORMAL \
					__END_STDGRP_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_STDGRP_TILE_TR_SPRITE \
					__START_STDGRP_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// 終了
#define		__END_STDGRP_TILE_TR_SPRITE \
					__END_STDGRP_TILE_NORMAL

