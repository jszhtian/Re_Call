#pragma		once

// ****************************************************************
// 基本（画像描画用）
// ================================================================

// 開始
#define		__START_BLDLINED				__START_LINED

// 終了
#define		__END_BLDLINED					__END_LINED

// 開始（タイル版）
#define		__START_BLDLINED_TILE			__START_LINED_TILE

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_BLDLINED_NORMAL \
					__START_BLDLINED \
__asm				mov		edx, __TR_ADR

// 終了
#define		__END_BLDLINED_NORMAL \
					__END_BLDLINED

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_BLDLINED_SPRITE \
					__START_BLDLINED_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_TR_ALPHABLEND

// 終了
#define		__END_BLDLINED_SPRITE \
__asm			__dont_draw: \
					__END_BLDLINED_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_BLDLINED_TR_NORMAL \
					__START_BLDLINED_NORMAL

// 終了
#define		__END_BLDLINED_TR_NORMAL \
					__END_BLDLINED_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_BLDLINED_TR_SPRITE \
					__START_BLDLINED_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_TR_ALPHABLEND

// 終了
#define		__END_BLDLINED_TR_SPRITE \
					__END_BLDLINED_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_BLDLINED_TILE_NORMAL \
					__START_BLDLINED_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_LD_ALPHABLEND

// 終了
#define		__END_BLDLINED_TILE_NORMAL \
__asm			__dont_draw: \
					__END_BLDLINED

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_BLDLINED_TILE_SPRITE \
					__START_BLDLINED_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_LD_ALPHABLEND

// 終了
#define		__END_BLDLINED_TILE_SPRITE \
					__END_BLDLINED_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_BLDLINED_TILE_TR_NORMAL \
					__START_BLDLINED_TILE_NORMAL

// 終了
#define		__END_BLDLINED_TILE_TR_NORMAL \
					__END_BLDLINED_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_BLDLINED_TILE_TR_SPRITE \
					__START_BLDLINED_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_LD_ALPHABLEND

// 終了
#define		__END_BLDLINED_TILE_TR_SPRITE \
					__END_BLDLINED_TILE_NORMAL

