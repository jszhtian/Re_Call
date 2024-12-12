#pragma		once

// ****************************************************************
// 基本
// ================================================================

// 開始
#define		__START_CHGLBDBOX \
					__START_LBDBOX \
__asm				mov		esi, edi

// 終了（前処理）
#define		__END_CHGLBDBOX_BEFOR			__END_LBDBOX_BEFOR

// 終了（後処理）
#define		__END_CHGLBDBOX_AFTER			__END_LBDBOX_AFTER

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_CHGLBDBOX_NORMAL \
					__START_CHGLBDBOX \
__asm				mov		edx, __TR_ADR

// 終了
#define		__END_CHGLBDBOX_NORMAL \
					__END_CHGLBDBOX_BEFOR \
					__END_CHGLBDBOX_AFTER

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_CHGLBDBOX_SPRITE \
					__START_CHGLBDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_CHGLBDBOX_SPRITE \
__asm			__dont_draw: \
					__END_CHGLBDBOX_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_CHGLBDBOX_TR_NORMAL \
					__START_CHGLBDBOX_NORMAL

// 終了
#define		__END_CHGLBDBOX_TR_NORMAL \
					__END_CHGLBDBOX_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_CHGLBDBOX_TR_SPRITE \
					__START_CHGLBDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_EDX_TR_TABLE_ADR

// 終了
#define		__END_CHGLBDBOX_TR_SPRITE \
					__END_CHGLBDBOX_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_CHGLBDBOX_TILE_NORMAL \
					__START_CHGLBDBOX \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET

// 終了
#define		__END_CHGLBDBOX_TILE_NORMAL \
__asm			__dont_draw: \
					__END_CHGLBDBOX_BEFOR \
					__TILE_PROC_X \
					__END_CHGLBDBOX_AFTER

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_CHGLBDBOX_TILE_SPRITE \
					__START_CHGLBDBOX \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// 終了
#define		__END_CHGLBDBOX_TILE_SPRITE \
					__END_CHGLBDBOX_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_CHGLBDBOX_TILE_TR_NORMAL \
					__START_CHGLBDBOX_TILE_NORMAL

// 終了
#define		__END_CHGLBDBOX_TILE_TR_NORMAL \
					__END_CHGLBDBOX_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_CHGLBDBOX_TILE_TR_SPRITE \
					__START_CHGLBDBOX \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET

// 終了
#define		__END_CHGLBDBOX_TILE_TR_SPRITE \
					__END_CHGLBDBOX_TILE_NORMAL

