#pragma		once

// ****************************************************************
// 基本（画像描画用）
// ================================================================

// 開始
#define		__START_LINED_FUNC \
__asm				cld \
__asm				mov		esi, __LINEDP_LINE_PTR \
__asm				mov		edi, __LINEDP_DST_PTR \
__asm				mov		ecx, __LINEDP_LINE_CNT \
__asm			__loop: \
__asm				lodsd \
__asm				cmp		eax, 0 \
__asm				je		__step_x_end \
__asm				jg		__step_x_great \
__asm				sub		edi, __LINEDP_X_ADD \
__asm				jmp		__step_x_end \
__asm			__step_x_great: \
__asm				add		edi, __LINEDP_X_ADD \
__asm			__step_x_end: \
__asm				lodsd \
__asm				cmp		eax, 0 \
__asm				je		__step_y_end \
__asm				jg		__step_y_great \
__asm				sub		edi, __LINEDP_Y_ADD \
__asm				jmp		__step_y_end \
__asm			__step_y_great: \
__asm				add		edi, __LINEDP_Y_ADD \
__asm			__step_y_end: \
__asm				push	esi

#define		__START_LINED \
					__START_LINED_FUNC \
__asm				mov		esi, __LINEDP_SRC_PTR

// 終了
#define		__END_LINED \
__asm				pop		esi \
__asm				dec		ecx \
__asm				jz		__loop_end \
__asm				jmp		__loop \
__asm			__loop_end:

// 開始（タイル版）
#define		__START_LINED_TILE_FUNC \
__asm				cld \
__asm				mov		esi, __LINEDP_LINE_PTR \
__asm				mov		edi, __LINEDP_DST_PTR \
__asm				mov		ecx, __LINEDP_LINE_CNT \
__asm			__loop: \
__asm				mov		ebx, __TILE_NOW_ADR \
__asm				lodsd \
__asm				cmp		eax, 0 \
__asm				je		__step_x_end \
__asm				jg		__step_x_great \
__asm				sub		edi, __LINEDP_X_ADD \
					__TILE_PROC_X_DEC \
__asm				jmp		__step_x_end \
__asm			__step_x_great: \
__asm				add		edi, __LINEDP_X_ADD \
					__TILE_PROC_X_INC \
__asm			__step_x_end: \
__asm				lodsd \
__asm				cmp		eax, 0 \
__asm				je		__step_y_end \
__asm				jg		__step_y_great \
__asm				sub		edi, __LINEDP_Y_ADD \
					__TILE_PROC_Y_DEC \
__asm				jmp		__step_y_end \
__asm			__step_y_great: \
__asm				add		edi, __LINEDP_Y_ADD \
					__TILE_PROC_Y_INC \
__asm			__step_y_end: \
__asm				mov		__TILE_NOW_ADR, ebx \
__asm				push	esi

#define		__START_LINED_TILE \
					__START_LINED_TILE_FUNC \
__asm				mov		esi, __LINEDP_SRC_PTR

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_LINED_NORMAL \
					__START_LINED \
__asm				mov		edx, __TR_ADR

// 終了
#define		__END_LINED_NORMAL \
					__END_LINED

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_LINED_SPRITE \
					__START_LINED_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_LINED_SPRITE \
__asm			__dont_draw: \
					__END_LINED_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_LINED_TR_NORMAL \
					__START_LINED_NORMAL

// 終了
#define		__END_LINED_TR_NORMAL \
					__END_LINED_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_LINED_TR_SPRITE \
					__START_LINED_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_LINED_TR_SPRITE \
					__END_LINED_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_LINED_TILE_NORMAL \
					__START_LINED_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_LD

// 終了
#define		__END_LINED_TILE_NORMAL \
__asm			__dont_draw: \
					__END_LINED

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_LINED_TILE_SPRITE \
					__START_LINED_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_LD

// 終了
#define		__END_LINED_TILE_SPRITE \
					__END_LINED_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_LINED_TILE_TR_NORMAL \
					__START_LINED_TILE_NORMAL

// 終了
#define		__END_LINED_TILE_TR_NORMAL \
					__END_LINED_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_LINED_TILE_TR_SPRITE \
					__START_LINED_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_LD

// 終了
#define		__END_LINED_TILE_TR_SPRITE \
					__END_LINED_TILE_NORMAL

