#pragma		once

// ****************************************************************
// 基本
// ================================================================

// 開始
#define		__START_BLDSTDBOX			__START_STDBOX

// 終了Ｘ
#define		__END_BLDSTDBOX_X			__END_STDBOX_X

// 終了Ｙ
#define		__END_BLDSTDBOX_Y			__END_STDBOX_Y

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_BLDSTDBOX_NORMAL \
					__START_BLDSTDBOX \
__asm			__loop_x:

// 終了
#define		__END_BLDSTDBOX_NORMAL \
					__END_BLDSTDBOX_X \
					__END_BLDSTDBOX_Y

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_BLDSTDBOX_SPRITE \
					__START_BLDSTDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_TR_ALPHABLEND

// 終了
#define		__END_BLDSTDBOX_SPRITE \
__asm			__dont_draw: \
					__END_BLDSTDBOX_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_BLDSTDBOX_TR_NORMAL \
					__START_BLDSTDBOX_NORMAL

// 終了
#define		__END_BLDSTDBOX_TR_NORMAL \
					__END_BLDSTDBOX_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_BLDSTDBOX_TR_SPRITE \
					__START_BLDSTDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_TR_ALPHABLEND

// 終了
#define		__END_BLDSTDBOX_TR_SPRITE \
					__END_BLDSTDBOX_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_BLDSTDBOX_TILE \
					__START_BLDSTDBOX \
__asm				mov		eax, __TILE_START_X \
__asm				mov		__TILE_NOW_X, eax \
__asm			__loop_x:

// 開始
#define		__START_BLDSTDBOX_TILE_NORMAL \
					__START_BLDSTDBOX_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_ALPHABLEND

// 開始ＥＸ（透明度が２５５の時は透明度処理をしないバージョン／通常タイル描画専用）
#define		__START_BLDSTDBOX_TILE_NORMAL_EX \
					__START_BLDSTDBOX_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_EX_ALPHABLEND

// 終了
#define		__END_BLDSTDBOX_TILE_NORMAL \
__asm			__dont_draw: \
					__TILE_PROC_X \
					__END_BLDSTDBOX_X \
					__TILE_PROC_Y \
					__END_BLDSTDBOX_Y

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_BLDSTDBOX_TILE_SPRITE \
					__START_BLDSTDBOX_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// 終了
#define		__END_BLDSTDBOX_TILE_SPRITE \
					__END_BLDSTDBOX_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_BLDSTDBOX_TILE_TR_NORMAL \
					__START_BLDSTDBOX_TILE_NORMAL

// 終了
#define		__END_BLDSTDBOX_TILE_TR_NORMAL \
					__END_BLDSTDBOX_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_BLDSTDBOX_TILE_TR_SPRITE \
					__START_BLDSTDBOX_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// 終了
#define		__END_BLDSTDBOX_TILE_TR_SPRITE \
					__END_BLDSTDBOX_TILE_NORMAL

