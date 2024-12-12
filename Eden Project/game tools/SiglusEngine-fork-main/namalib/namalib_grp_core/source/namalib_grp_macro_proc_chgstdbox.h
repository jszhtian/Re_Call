#pragma		once

// ****************************************************************
// 基本
// ================================================================

// 開始
#define		__START_CHGSTDBOX			__START_STDBOX

// 終了Ｘ
#define		__END_CHGSTDBOX_X			__END_STDBOX_X

// 終了Ｙ
#define		__END_CHGSTDBOX_Y			__END_STDBOX_Y

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_CHGSTDBOX_NORMAL \
					__START_CHGSTDBOX \
__asm			__loop_x: \
__asm				mov		esi, edi

// 終了
#define		__END_CHGSTDBOX_NORMAL \
					__END_CHGSTDBOX_X \
					__END_CHGSTDBOX_Y

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_CHGSTDBOX_SPRITE \
					__START_CHGSTDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_CHGSTDBOX_SPRITE \
__asm			__dont_draw: \
					__END_CHGSTDBOX_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_CHGSTDBOX_TR_NORMAL \
					__START_CHGSTDBOX_NORMAL

// 終了
#define		__END_CHGSTDBOX_TR_NORMAL \
					__END_CHGSTDBOX_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_CHGSTDBOX_TR_SPRITE \
					__START_CHGSTDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_CHGSTDBOX_TR_SPRITE \
					__END_CHGSTDBOX_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_CHGSTDBOX_TILE \
					__START_CHGSTDBOX \
__asm				mov		eax, __TILE_START_X \
__asm				mov		__TILE_NOW_X, eax \
__asm			__loop_x: \
__asm				mov		esi, edi

// 開始
#define		__START_CHGSTDBOX_TILE_NORMAL \
					__START_CHGSTDBOX_TILE \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET

// 終了
#define		__END_CHGSTDBOX_TILE_NORMAL \
__asm			__dont_draw: \
					__TILE_PROC_X \
					__END_CHGSTDBOX_X \
					__TILE_PROC_Y \
					__END_CHGSTDBOX_Y

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_CHGSTDBOX_TILE_SPRITE \
					__START_CHGSTDBOX_TILE \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// 終了
#define		__END_CHGSTDBOX_TILE_SPRITE \
					__END_CHGSTDBOX_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_CHGSTDBOX_TILE_TR_NORMAL \
					__START_CHGSTDBOX_TILE_NORMAL

// 終了
#define		__END_CHGSTDBOX_TILE_TR_NORMAL \
					__END_CHGSTDBOX_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_CHGSTDBOX_TILE_TR_SPRITE \
					__START_CHGSTDBOX_TILE \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// 終了
#define		__END_CHGSTDBOX_TILE_TR_SPRITE \
					__END_CHGSTDBOX_TILE_NORMAL

