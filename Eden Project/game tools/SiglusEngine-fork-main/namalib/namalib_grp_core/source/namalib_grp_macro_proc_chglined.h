#pragma		once

// ****************************************************************
// 基本（画像描画用）
// ================================================================

// 開始
#define		__START_CHGLINED \
					__START_LINED_FUNC \
__asm				mov		esi, edi

// 終了
#define		__END_CHGLINED				__END_LINED

// 開始（タイル版）
#define		__START_CHGLINED_TILE \
					__START_LINED_TILE_FUNC \
__asm				mov		esi, edi

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_CHGLINED_NORMAL \
					__START_CHGLINED \
__asm				mov		edx, __TR_ADR

// 終了
#define		__END_CHGLINED_NORMAL \
					__END_CHGLINED

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_CHGLINED_SPRITE \
					__START_CHGLINED_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_CHGLINED_SPRITE \
__asm			__dont_draw: \
					__END_CHGLINED_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_CHGLINED_TR_NORMAL \
					__START_CHGLINED_NORMAL

// 終了
#define		__END_CHGLINED_TR_NORMAL \
					__END_CHGLINED_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_CHGLINED_TR_SPRITE \
					__START_CHGLINED_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_CHGLINED_TR_SPRITE \
					__END_CHGLINED_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_CHGLINED_TILE_NORMAL \
					__START_CHGLINED_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_LD

// 終了
#define		__END_CHGLINED_TILE_NORMAL \
__asm			__dont_draw: \
					__END_CHGLINED

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_CHGLINED_TILE_SPRITE \
					__START_CHGLINED_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_LD

// 終了
#define		__END_CHGLINED_TILE_SPRITE \
					__END_CHGLINED_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_CHGLINED_TILE_TR_NORMAL \
					__START_CHGLINED_TILE_NORMAL

// 終了
#define		__END_CHGLINED_TILE_TR_NORMAL \
					__END_CHGLINED_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_CHGLINED_TILE_TR_SPRITE \
					__START_CHGLINED_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_LD

// 終了
#define		__END_CHGLINED_TILE_TR_SPRITE \
					__END_CHGLINED_TILE_NORMAL

