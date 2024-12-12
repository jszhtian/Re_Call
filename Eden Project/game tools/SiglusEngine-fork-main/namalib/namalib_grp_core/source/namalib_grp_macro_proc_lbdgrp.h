#pragma		once

// ****************************************************************
// 基本
// ================================================================

// 開始
#define		__START_LBDGRP \
__asm				cld \
__asm				mov		esi, __LBDP_SRC_PTR \
__asm				mov		edi, __LBDP_DST_PTR \
__asm				mov		edx, __LBDP_DST_ALPHA \
__asm				mov		eax, __LBDP_SW \
__asm				mov		ebx, __LBDP_NS \
__asm				mov		ecx, __LBDP_CNT \
__asm				inc		ecx \
__asm			__loop_y: \
__asm				push	ecx \
__asm				push	ebx \
__asm				push	eax \
__asm				add		edx, __LBDP_DST_SIZE \
__asm				jns		__loop_x \
__asm				jmp		__loop_x_end \
__asm			__loop_x: \
__asm				cmp		edi, __LBDP_DST_MIN \
__asm				jge		__draw_step \
__asm				jmp		__draw_end \
__asm			__draw_step: \
__asm				cmp		edi, __LBDP_DST_MAX \
__asm				jle		__draw_start \
__asm				pop		eax \
__asm				pop		ebx \
__asm				pop		ecx \
__asm				jmp		__loop_y_end \
__asm			__draw_start: \
__asm				push	edx

// 終了（前処理）
#define		__END_LBDGRP_BEFOR \
__asm				pop		edx \
__asm			__draw_end:

// 終了（後処理）
#define		__END_LBDGRP_AFTER \
__asm				add		edi, 4 \
__asm				sub		edx, __LBDP_DST_CNT \
__asm				js		__loop_x_end \
__asm				jmp		__loop_x \
__asm			__loop_x_end: \
__asm				pop		eax \
__asm				pop		ebx \
__asm				pop		ecx \
__asm				and		eax, 0x01 \
__asm				jnz		__naname \
__asm				add		esi, ebx \
__asm				jmp		__loop_y_step \
__asm			__naname: \
__asm				add		esi, __LBDP_ALPHA \
__asm				add		ebx, __LBDP_NA \
__asm				js		__loop_y_step \
__asm				add		esi, __LBDP_BETA \
__asm				sub		ebx, __LBDP_NH \
__asm			__loop_y_step: \
__asm				dec		ecx \
__asm				jz		__loop_y_end \
__asm				jmp		__loop_y \
__asm			__loop_y_end:

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_LBDGRP_NORMAL \
					__START_LBDGRP \
__asm				mov		edx, __TR_ADR

// 終了
#define		__END_LBDGRP_NORMAL \
					__END_LBDGRP_BEFOR \
					__END_LBDGRP_AFTER

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_LBDGRP_SPRITE \
					__START_LBDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_LBDGRP_SPRITE \
__asm			__dont_draw: \
					__END_LBDGRP_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_LBDGRP_TR_NORMAL \
					__START_LBDGRP_NORMAL

// 終了
#define		__END_LBDGRP_TR_NORMAL \
					__END_LBDGRP_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_LBDGRP_TR_SPRITE \
					__START_LBDGRP_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_LBDGRP_TR_SPRITE \
					__END_LBDGRP_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_LBDGRP_TILE_NORMAL \
					__START_LBDGRP \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET

// 終了
#define		__END_LBDGRP_TILE_NORMAL \
__asm			__dont_draw: \
					__END_LBDGRP_BEFOR \
					__TILE_PROC_X \
					__END_LBDGRP_AFTER

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_LBDGRP_TILE_SPRITE \
					__START_LBDGRP \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// 終了
#define		__END_LBDGRP_TILE_SPRITE \
					__END_LBDGRP_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_LBDGRP_TILE_TR_NORMAL \
					__START_LBDGRP_TILE_NORMAL

// 終了
#define		__END_LBDGRP_TILE_TR_NORMAL \
					__END_LBDGRP_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_LBDGRP_TILE_TR_SPRITE \
					__START_LBDGRP \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// 終了
#define		__END_LBDGRP_TILE_TR_SPRITE \
					__END_LBDGRP_TILE_NORMAL

