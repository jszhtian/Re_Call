#pragma		once

// ****************************************************************
// 基本
// ================================================================

// 開始
#define		__START_BLDLBDBOX			__START_LBDBOX

// 終了（前処理）
#define		__END_BLDLBDBOX_BEFOR		__END_LBDBOX_BEFOR

// 終了（後処理）
#define		__END_BLDLBDBOX_AFTER		__END_LBDBOX_AFTER

// ****************************************************************
// 通常
// ================================================================

// 開始
#define		__START_BLDLBDBOX_NORMAL \
					__START_BLDLBDBOX \
__asm				mov		edx, __TR_ADR

// 終了
#define		__END_BLDLBDBOX_NORMAL \
					__END_BLDLBDBOX_BEFOR \
					__END_BLDLBDBOX_AFTER

// ****************************************************************
// スプライト
// ================================================================

// 開始
#define		__START_BLDLBDBOX_SPRITE \
					__START_BLDLBDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__MOV_TR_ALPHABLEND

// 終了
#define		__END_BLDLBDBOX_SPRITE \
__asm			__dont_draw: \
					__END_BLDLBDBOX_NORMAL

// ****************************************************************
// 透明 通常
// ================================================================

// 開始
#define		__START_BLDLBDBOX_TR_NORMAL \
					__START_BLDLBDBOX_NORMAL

// 終了
#define		__END_BLDLBDBOX_TR_NORMAL \
					__END_BLDLBDBOX_NORMAL

// ****************************************************************
// 透明 スプライト
// ================================================================

// 開始
#define		__START_BLDLBDBOX_TR_SPRITE \
					__START_BLDLBDBOX_NORMAL \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
					__MOV_TR_ALPHABLEND

// 終了
#define		__END_BLDLBDBOX_TR_SPRITE \
					__END_BLDLBDBOX_SPRITE

// ****************************************************************
// タイル 通常
// ================================================================

// 開始
#define		__START_BLDLBDBOX_TILE_NORMAL \
					__START_BLDLBDBOX \
__asm				mov		edx, __TR_4 \
					__TILE_PROC_SET_ALPHABLEND

// 終了
#define		__END_BLDLBDBOX_TILE_NORMAL \
__asm			__dont_draw: \
					__END_BLDLBDBOX_BEFOR \
					__TILE_PROC_X \
					__END_BLDLBDBOX_AFTER

// ****************************************************************
// タイル スプライト
// ================================================================

// 開始
#define		__START_BLDLBDBOX_TILE_SPRITE \
					__START_BLDLBDBOX \
					__CHECK_SPRITE_ALPHA \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// 終了
#define		__END_BLDLBDBOX_TILE_SPRITE \
					__END_BLDLBDBOX_TILE_NORMAL

// ****************************************************************
// タイル 透明 通常
// ================================================================

// 開始
#define		__START_BLDLBDBOX_TILE_TR_NORMAL \
					__START_BLDLBDBOX_TILE_NORMAL

// 終了
#define		__END_BLDLBDBOX_TILE_TR_NORMAL \
					__END_BLDLBDBOX_TILE_NORMAL

// ****************************************************************
// タイル 透明 スプライト
// ================================================================

// 開始
#define		__START_BLDLBDBOX_TILE_TR_SPRITE \
					__START_BLDLBDBOX \
					__CHECK_SPRITE_ALPHA \
					__COMPOSE_TR \
__asm				shl		ebx, 2 \
__asm				mov		edx, ebx \
					__TILE_PROC_SET_ALPHABLEND

// 終了
#define		__END_BLDLBDBOX_TILE_TR_SPRITE \
					__END_BLDLBDBOX_TILE_NORMAL

