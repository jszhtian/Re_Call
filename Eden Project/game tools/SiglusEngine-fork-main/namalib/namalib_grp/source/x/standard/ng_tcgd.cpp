#include	"pch.h"




// 通常表示
void tcgd_normal(void);
// スプライト表示
void tcgd_sprite(void);
// 透明通常表示
void tcgd_normal_clr(void);
// 透明スプライト表示
void tcgd_sprite_clr(void);
// 変色通常表示
void tcgd_normal_change(void);
// 変色スプライト表示
void tcgd_sprite_change(void);
// 変色透明通常表示
void tcgd_normal_change_clr(void);
// 変色透明スプライト表示
void tcgd_sprite_change_clr(void);
// 明暗通常表示
void tcgd_normal_bright(void);
// 明暗スプライト表示
void tcgd_sprite_bright(void);
// 明暗透明通常表示
void tcgd_normal_bright_clr(void);
// 明暗透明スプライト表示
void tcgd_sprite_bright_clr(void);
// 変色明暗通常表示
void tcgd_normal_change_bright(void);
// 変色明暗スプライト表示
void tcgd_sprite_change_bright(void);
// 変色明暗透明通常表示
void tcgd_normal_change_bright_clr(void);
// 変色明暗透明スプライト表示
void tcgd_sprite_change_bright_clr(void);
// モノ通常表示
void tcgd_normal_mono(void);
// モノスプライト表示
void tcgd_sprite_mono(void);
// モノ透明通常表示
void tcgd_normal_mono_clr(void);
// モノ透明スプライト表示
void tcgd_sprite_mono_clr(void);
// モノ変色通常表示
void tcgd_normal_mono_change(void);
// モノ変色スプライト表示
void tcgd_sprite_mono_change(void);
// モノ変色透明通常表示
void tcgd_normal_mono_change_clr(void);
// モノ変色透明スプライト表示
void tcgd_sprite_mono_change_clr(void);
// モノ明暗通常表示
void tcgd_normal_mono_bright(void);
// モノ明暗スプライト表示
void tcgd_sprite_mono_bright(void);
// モノ明暗透明通常表示
void tcgd_normal_mono_bright_clr(void);
// モノ明暗透明スプライト表示
void tcgd_sprite_mono_bright_clr(void);
// モノ変色明暗通常表示
void tcgd_normal_mono_change_bright(void);
// モノ変色明暗スプライト表示
void tcgd_sprite_mono_change_bright(void);
// モノ変色明暗透明通常表示
void tcgd_normal_mono_change_bright_clr(void);
// モノ変色明暗透明スプライト表示
void tcgd_sprite_mono_change_bright_clr(void);
// 反転通常表示
void tcgd_normal_reverse(void);
// 反転スプライト表示
void tcgd_sprite_reverse(void);
// 反転透明通常表示
void tcgd_normal_reverse_clr(void);
// 反転透明スプライト表示
void tcgd_sprite_reverse_clr(void);
// 反転変色通常表示
void tcgd_normal_reverse_change(void);
// 反転変色スプライト表示
void tcgd_sprite_reverse_change(void);
// 反転変色透明通常表示
void tcgd_normal_reverse_change_clr(void);
// 反転変色透明スプライト表示
void tcgd_sprite_reverse_change_clr(void);
// 反転明暗通常表示
void tcgd_normal_reverse_bright(void);
// 反転明暗スプライト表示
void tcgd_sprite_reverse_bright(void);
// 反転明暗透明通常表示
void tcgd_normal_reverse_bright_clr(void);
// 反転明暗透明スプライト表示
void tcgd_sprite_reverse_bright_clr(void);
// 反転変色明暗通常表示
void tcgd_normal_reverse_change_bright(void);
// 反転変色明暗スプライト表示
void tcgd_sprite_reverse_change_bright(void);
// 反転変色明暗透明通常表示
void tcgd_normal_reverse_change_bright_clr(void);
// 反転変色明暗透明スプライト表示
void tcgd_sprite_reverse_change_bright_clr(void);
// 反転モノ通常表示
void tcgd_normal_reverse_mono(void);
// 反転モノスプライト表示
void tcgd_sprite_reverse_mono(void);
// 反転モノ透明通常表示
void tcgd_normal_reverse_mono_clr(void);
// 反転モノ透明スプライト表示
void tcgd_sprite_reverse_mono_clr(void);
// 反転モノ変色通常表示
void tcgd_normal_reverse_mono_change(void);
// 反転モノ変色スプライト表示
void tcgd_sprite_reverse_mono_change(void);
// 反転モノ変色透明通常表示
void tcgd_normal_reverse_mono_change_clr(void);
// 反転モノ変色透明スプライト表示
void tcgd_sprite_reverse_mono_change_clr(void);
// 反転モノ明暗通常表示
void tcgd_normal_reverse_mono_bright(void);
// 反転モノ明暗スプライト表示
void tcgd_sprite_reverse_mono_bright(void);
// 反転モノ明暗透明通常表示
void tcgd_normal_reverse_mono_bright_clr(void);
// 反転モノ明暗透明スプライト表示
void tcgd_sprite_reverse_mono_bright_clr(void);
// 反転モノ変色明暗通常表示
void tcgd_normal_reverse_mono_change_bright(void);
// 反転モノ変色明暗スプライト表示
void tcgd_sprite_reverse_mono_change_bright(void);
// 反転モノ変色明暗透明通常表示
void tcgd_normal_reverse_mono_change_bright_clr(void);
// 反転モノ変色明暗透明スプライト表示
void tcgd_sprite_reverse_mono_change_bright_clr(void);

// タイル通常表示
void tcgd_normal_tile(void);
// タイルスプライト表示
void tcgd_sprite_tile(void);
// タイル透明通常表示
void tcgd_normal_clr_tile(void);
// タイル透明スプライト表示
void tcgd_sprite_clr_tile(void);
// タイル変色通常表示
void tcgd_normal_change_tile(void);
// タイル変色スプライト表示
void tcgd_sprite_change_tile(void);
// タイル変色透明通常表示
void tcgd_normal_change_clr_tile(void);
// タイル変色透明スプライト表示
void tcgd_sprite_change_clr_tile(void);
// タイル明暗通常表示
void tcgd_normal_bright_tile(void);
// タイル明暗スプライト表示
void tcgd_sprite_bright_tile(void);
// タイル明暗透明通常表示
void tcgd_normal_bright_clr_tile(void);
// タイル明暗透明スプライト表示
void tcgd_sprite_bright_clr_tile(void);
// タイル変色明暗通常表示
void tcgd_normal_change_bright_tile(void);
// タイル変色明暗スプライト表示
void tcgd_sprite_change_bright_tile(void);
// タイル変色明暗透明通常表示
void tcgd_normal_change_bright_clr_tile(void);
// タイル変色明暗透明スプライト表示
void tcgd_sprite_change_bright_clr_tile(void);
// タイルモノ通常表示
void tcgd_normal_mono_tile(void);
// タイルモノスプライト表示
void tcgd_sprite_mono_tile(void);
// タイルモノ透明通常表示
void tcgd_normal_mono_clr_tile(void);
// タイルモノ透明スプライト表示
void tcgd_sprite_mono_clr_tile(void);
// タイルモノ変色通常表示
void tcgd_normal_mono_change_tile(void);
// タイルモノ変色スプライト表示
void tcgd_sprite_mono_change_tile(void);
// タイルモノ変色透明通常表示
void tcgd_normal_mono_change_clr_tile(void);
// タイルモノ変色透明スプライト表示
void tcgd_sprite_mono_change_clr_tile(void);
// タイルモノ明暗通常表示
void tcgd_normal_mono_bright_tile(void);
// タイルモノ明暗スプライト表示
void tcgd_sprite_mono_bright_tile(void);
// タイルモノ明暗透明通常表示
void tcgd_normal_mono_bright_clr_tile(void);
// タイルモノ明暗透明スプライト表示
void tcgd_sprite_mono_bright_clr_tile(void);
// タイルモノ変色明暗通常表示
void tcgd_normal_mono_change_bright_tile(void);
// タイルモノ変色明暗スプライト表示
void tcgd_sprite_mono_change_bright_tile(void);
// タイルモノ変色明暗透明通常表示
void tcgd_normal_mono_change_bright_clr_tile(void);
// タイルモノ変色明暗透明スプライト表示
void tcgd_sprite_mono_change_bright_clr_tile(void);
// タイル反転通常表示
void tcgd_normal_reverse_tile(void);
// タイル反転スプライト表示
void tcgd_sprite_reverse_tile(void);
// タイル反転透明通常表示
void tcgd_normal_reverse_clr_tile(void);
// タイル反転透明スプライト表示
void tcgd_sprite_reverse_clr_tile(void);
// タイル反転変色通常表示
void tcgd_normal_reverse_change_tile(void);
// タイル反転変色スプライト表示
void tcgd_sprite_reverse_change_tile(void);
// タイル反転変色透明通常表示
void tcgd_normal_reverse_change_clr_tile(void);
// タイル反転変色透明スプライト表示
void tcgd_sprite_reverse_change_clr_tile(void);
// タイル反転明暗通常表示
void tcgd_normal_reverse_bright_tile(void);
// タイル反転明暗スプライト表示
void tcgd_sprite_reverse_bright_tile(void);
// タイル反転明暗透明通常表示
void tcgd_normal_reverse_bright_clr_tile(void);
// タイル反転明暗透明スプライト表示
void tcgd_sprite_reverse_bright_clr_tile(void);
// タイル反転変色明暗通常表示
void tcgd_normal_reverse_change_bright_tile(void);
// タイル反転変色明暗スプライト表示
void tcgd_sprite_reverse_change_bright_tile(void);
// タイル反転変色明暗透明通常表示
void tcgd_normal_reverse_change_bright_clr_tile(void);
// タイル反転変色明暗透明スプライト表示
void tcgd_sprite_reverse_change_bright_clr_tile(void);
// タイル反転モノ通常表示
void tcgd_normal_reverse_mono_tile(void);
// タイル反転モノスプライト表示
void tcgd_sprite_reverse_mono_tile(void);
// タイル反転モノ透明通常表示
void tcgd_normal_reverse_mono_clr_tile(void);
// タイル反転モノ透明スプライト表示
void tcgd_sprite_reverse_mono_clr_tile(void);
// タイル反転モノ変色通常表示
void tcgd_normal_reverse_mono_change_tile(void);
// タイル反転モノ変色スプライト表示
void tcgd_sprite_reverse_mono_change_tile(void);
// タイル反転モノ変色透明通常表示
void tcgd_normal_reverse_mono_change_clr_tile(void);
// タイル反転モノ変色透明スプライト表示
void tcgd_sprite_reverse_mono_change_clr_tile(void);
// タイル反転モノ明暗通常表示
void tcgd_normal_reverse_mono_bright_tile(void);
// タイル反転モノ明暗スプライト表示
void tcgd_sprite_reverse_mono_bright_tile(void);
// タイル反転モノ明暗透明通常表示
void tcgd_normal_reverse_mono_bright_clr_tile(void);
// タイル反転モノ明暗透明スプライト表示
void tcgd_sprite_reverse_mono_bright_clr_tile(void);
// タイル反転モノ変色明暗通常表示
void tcgd_normal_reverse_mono_change_bright_tile(void);
// タイル反転モノ変色明暗スプライト表示
void tcgd_sprite_reverse_mono_change_bright_tile(void);
// タイル反転モノ変色明暗透明通常表示
void tcgd_normal_reverse_mono_change_bright_clr_tile(void);
// タイル反転モノ変色明暗透明スプライト表示
void tcgd_sprite_reverse_mono_change_bright_clr_tile(void);

// 通常表示（加算）
void tcgd_normal_add(void);
// スプライト表示（加算）
void tcgd_sprite_add(void);
// 透明通常表示（加算）
void tcgd_normal_clr_add(void);
// 透明スプライト表示（加算）
void tcgd_sprite_clr_add(void);
// 変色通常表示（加算）
void tcgd_normal_change_add(void);
// 変色スプライト表示（加算）
void tcgd_sprite_change_add(void);
// 変色透明通常表示（加算）
void tcgd_normal_change_clr_add(void);
// 変色透明スプライト表示（加算）
void tcgd_sprite_change_clr_add(void);
// 明暗通常表示（加算）
void tcgd_normal_bright_add(void);
// 明暗スプライト表示（加算）
void tcgd_sprite_bright_add(void);
// 明暗透明通常表示（加算）
void tcgd_normal_bright_clr_add(void);
// 明暗透明スプライト表示（加算）
void tcgd_sprite_bright_clr_add(void);
// 変色明暗通常表示（加算）
void tcgd_normal_change_bright_add(void);
// 変色明暗スプライト表示（加算）
void tcgd_sprite_change_bright_add(void);
// 変色明暗透明通常表示（加算）
void tcgd_normal_change_bright_clr_add(void);
// 変色明暗透明スプライト表示（加算）
void tcgd_sprite_change_bright_clr_add(void);
// モノ通常表示（加算）
void tcgd_normal_mono_add(void);
// モノスプライト表示（加算）
void tcgd_sprite_mono_add(void);
// モノ透明通常表示（加算）
void tcgd_normal_mono_clr_add(void);
// モノ透明スプライト表示（加算）
void tcgd_sprite_mono_clr_add(void);
// モノ変色通常表示（加算）
void tcgd_normal_mono_change_add(void);
// モノ変色スプライト表示（加算）
void tcgd_sprite_mono_change_add(void);
// モノ変色透明通常表示（加算）
void tcgd_normal_mono_change_clr_add(void);
// モノ変色透明スプライト表示（加算）
void tcgd_sprite_mono_change_clr_add(void);
// モノ明暗通常表示（加算）
void tcgd_normal_mono_bright_add(void);
// モノ明暗スプライト表示（加算）
void tcgd_sprite_mono_bright_add(void);
// モノ明暗透明通常表示（加算）
void tcgd_normal_mono_bright_clr_add(void);
// モノ明暗透明スプライト表示（加算）
void tcgd_sprite_mono_bright_clr_add(void);
// モノ変色明暗通常表示（加算）
void tcgd_normal_mono_change_bright_add(void);
// モノ変色明暗スプライト表示（加算）
void tcgd_sprite_mono_change_bright_add(void);
// モノ変色明暗透明通常表示（加算）
void tcgd_normal_mono_change_bright_clr_add(void);
// モノ変色明暗透明スプライト表示（加算）
void tcgd_sprite_mono_change_bright_clr_add(void);
// 反転通常表示（加算）
void tcgd_normal_reverse_add(void);
// 反転スプライト表示（加算）
void tcgd_sprite_reverse_add(void);
// 反転透明通常表示（加算）
void tcgd_normal_reverse_clr_add(void);
// 反転透明スプライト表示（加算）
void tcgd_sprite_reverse_clr_add(void);
// 反転変色通常表示（加算）
void tcgd_normal_reverse_change_add(void);
// 反転変色スプライト表示（加算）
void tcgd_sprite_reverse_change_add(void);
// 反転変色透明通常表示（加算）
void tcgd_normal_reverse_change_clr_add(void);
// 反転変色透明スプライト表示（加算）
void tcgd_sprite_reverse_change_clr_add(void);
// 反転明暗通常表示（加算）
void tcgd_normal_reverse_bright_add(void);
// 反転明暗スプライト表示（加算）
void tcgd_sprite_reverse_bright_add(void);
// 反転明暗透明通常表示（加算）
void tcgd_normal_reverse_bright_clr_add(void);
// 反転明暗透明スプライト表示（加算）
void tcgd_sprite_reverse_bright_clr_add(void);
// 反転変色明暗通常表示（加算）
void tcgd_normal_reverse_change_bright_add(void);
// 反転変色明暗スプライト表示（加算）
void tcgd_sprite_reverse_change_bright_add(void);
// 反転変色明暗透明通常表示（加算）
void tcgd_normal_reverse_change_bright_clr_add(void);
// 反転変色明暗透明スプライト表示（加算）
void tcgd_sprite_reverse_change_bright_clr_add(void);
// 反転モノ通常表示（加算）
void tcgd_normal_reverse_mono_add(void);
// 反転モノスプライト表示（加算）
void tcgd_sprite_reverse_mono_add(void);
// 反転モノ透明通常表示（加算）
void tcgd_normal_reverse_mono_clr_add(void);
// 反転モノ透明スプライト表示（加算）
void tcgd_sprite_reverse_mono_clr_add(void);
// 反転モノ変色通常表示（加算）
void tcgd_normal_reverse_mono_change_add(void);
// 反転モノ変色スプライト表示（加算）
void tcgd_sprite_reverse_mono_change_add(void);
// 反転モノ変色透明通常表示（加算）
void tcgd_normal_reverse_mono_change_clr_add(void);
// 反転モノ変色透明スプライト表示（加算）
void tcgd_sprite_reverse_mono_change_clr_add(void);
// 反転モノ明暗通常表示（加算）
void tcgd_normal_reverse_mono_bright_add(void);
// 反転モノ明暗スプライト表示（加算）
void tcgd_sprite_reverse_mono_bright_add(void);
// 反転モノ明暗透明通常表示（加算）
void tcgd_normal_reverse_mono_bright_clr_add(void);
// 反転モノ明暗透明スプライト表示（加算）
void tcgd_sprite_reverse_mono_bright_clr_add(void);
// 反転モノ変色明暗通常表示（加算）
void tcgd_normal_reverse_mono_change_bright_add(void);
// 反転モノ変色明暗スプライト表示（加算）
void tcgd_sprite_reverse_mono_change_bright_add(void);
// 反転モノ変色明暗透明通常表示（加算）
void tcgd_normal_reverse_mono_change_bright_clr_add(void);
// 反転モノ変色明暗透明スプライト表示（加算）
void tcgd_sprite_reverse_mono_change_bright_clr_add(void);

// タイル通常表示（加算）
void tcgd_normal_tile_add(void);
// タイルスプライト表示（加算）
void tcgd_sprite_tile_add(void);
// タイル透明通常表示（加算）
void tcgd_normal_clr_tile_add(void);
// タイル透明スプライト表示（加算）
void tcgd_sprite_clr_tile_add(void);
// タイル変色通常表示（加算）
void tcgd_normal_change_tile_add(void);
// タイル変色スプライト表示（加算）
void tcgd_sprite_change_tile_add(void);
// タイル変色透明通常表示（加算）
void tcgd_normal_change_clr_tile_add(void);
// タイル変色透明スプライト表示（加算）
void tcgd_sprite_change_clr_tile_add(void);
// タイル明暗通常表示（加算）
void tcgd_normal_bright_tile_add(void);
// タイル明暗スプライト表示（加算）
void tcgd_sprite_bright_tile_add(void);
// タイル明暗透明通常表示（加算）
void tcgd_normal_bright_clr_tile_add(void);
// タイル明暗透明スプライト表示（加算）
void tcgd_sprite_bright_clr_tile_add(void);
// タイル変色明暗通常表示（加算）
void tcgd_normal_change_bright_tile_add(void);
// タイル変色明暗スプライト表示（加算）
void tcgd_sprite_change_bright_tile_add(void);
// タイル変色明暗透明通常表示（加算）
void tcgd_normal_change_bright_clr_tile_add(void);
// タイル変色明暗透明スプライト表示（加算）
void tcgd_sprite_change_bright_clr_tile_add(void);
// タイルモノ通常表示（加算）
void tcgd_normal_mono_tile_add(void);
// タイルモノスプライト表示（加算）
void tcgd_sprite_mono_tile_add(void);
// タイルモノ透明通常表示（加算）
void tcgd_normal_mono_clr_tile_add(void);
// タイルモノ透明スプライト表示（加算）
void tcgd_sprite_mono_clr_tile_add(void);
// タイルモノ変色通常表示（加算）
void tcgd_normal_mono_change_tile_add(void);
// タイルモノ変色スプライト表示（加算）
void tcgd_sprite_mono_change_tile_add(void);
// タイルモノ変色透明通常表示（加算）
void tcgd_normal_mono_change_clr_tile_add(void);
// タイルモノ変色透明スプライト表示（加算）
void tcgd_sprite_mono_change_clr_tile_add(void);
// タイルモノ明暗通常表示（加算）
void tcgd_normal_mono_bright_tile_add(void);
// タイルモノ明暗スプライト表示（加算）
void tcgd_sprite_mono_bright_tile_add(void);
// タイルモノ明暗透明通常表示（加算）
void tcgd_normal_mono_bright_clr_tile_add(void);
// タイルモノ明暗透明スプライト表示（加算）
void tcgd_sprite_mono_bright_clr_tile_add(void);
// タイルモノ変色明暗通常表示（加算）
void tcgd_normal_mono_change_bright_tile_add(void);
// タイルモノ変色明暗スプライト表示（加算）
void tcgd_sprite_mono_change_bright_tile_add(void);
// タイルモノ変色明暗透明通常表示（加算）
void tcgd_normal_mono_change_bright_clr_tile_add(void);
// タイルモノ変色明暗透明スプライト表示（加算）
void tcgd_sprite_mono_change_bright_clr_tile_add(void);
// タイル反転通常表示（加算）
void tcgd_normal_reverse_tile_add(void);
// タイル反転スプライト表示（加算）
void tcgd_sprite_reverse_tile_add(void);
// タイル反転透明通常表示（加算）
void tcgd_normal_reverse_clr_tile_add(void);
// タイル反転透明スプライト表示（加算）
void tcgd_sprite_reverse_clr_tile_add(void);
// タイル反転変色通常表示（加算）
void tcgd_normal_reverse_change_tile_add(void);
// タイル反転変色スプライト表示（加算）
void tcgd_sprite_reverse_change_tile_add(void);
// タイル反転変色透明通常表示（加算）
void tcgd_normal_reverse_change_clr_tile_add(void);
// タイル反転変色透明スプライト表示（加算）
void tcgd_sprite_reverse_change_clr_tile_add(void);
// タイル反転明暗通常表示（加算）
void tcgd_normal_reverse_bright_tile_add(void);
// タイル反転明暗スプライト表示（加算）
void tcgd_sprite_reverse_bright_tile_add(void);
// タイル反転明暗透明通常表示（加算）
void tcgd_normal_reverse_bright_clr_tile_add(void);
// タイル反転明暗透明スプライト表示（加算）
void tcgd_sprite_reverse_bright_clr_tile_add(void);
// タイル反転変色明暗通常表示（加算）
void tcgd_normal_reverse_change_bright_tile_add(void);
// タイル反転変色明暗スプライト表示（加算）
void tcgd_sprite_reverse_change_bright_tile_add(void);
// タイル反転変色明暗透明通常表示（加算）
void tcgd_normal_reverse_change_bright_clr_tile_add(void);
// タイル反転変色明暗透明スプライト表示（加算）
void tcgd_sprite_reverse_change_bright_clr_tile_add(void);
// タイル反転モノ通常表示（加算）
void tcgd_normal_reverse_mono_tile_add(void);
// タイル反転モノスプライト表示（加算）
void tcgd_sprite_reverse_mono_tile_add(void);
// タイル反転モノ透明通常表示（加算）
void tcgd_normal_reverse_mono_clr_tile_add(void);
// タイル反転モノ透明スプライト表示（加算）
void tcgd_sprite_reverse_mono_clr_tile_add(void);
// タイル反転モノ変色通常表示（加算）
void tcgd_normal_reverse_mono_change_tile_add(void);
// タイル反転モノ変色スプライト表示（加算）
void tcgd_sprite_reverse_mono_change_tile_add(void);
// タイル反転モノ変色透明通常表示（加算）
void tcgd_normal_reverse_mono_change_clr_tile_add(void);
// タイル反転モノ変色透明スプライト表示（加算）
void tcgd_sprite_reverse_mono_change_clr_tile_add(void);
// タイル反転モノ明暗通常表示（加算）
void tcgd_normal_reverse_mono_bright_tile_add(void);
// タイル反転モノ明暗スプライト表示（加算）
void tcgd_sprite_reverse_mono_bright_tile_add(void);
// タイル反転モノ明暗透明通常表示（加算）
void tcgd_normal_reverse_mono_bright_clr_tile_add(void);
// タイル反転モノ明暗透明スプライト表示（加算）
void tcgd_sprite_reverse_mono_bright_clr_tile_add(void);
// タイル反転モノ変色明暗通常表示（加算）
void tcgd_normal_reverse_mono_change_bright_tile_add(void);
// タイル反転モノ変色明暗スプライト表示（加算）
void tcgd_sprite_reverse_mono_change_bright_tile_add(void);
// タイル反転モノ変色明暗透明通常表示（加算）
void tcgd_normal_reverse_mono_change_bright_clr_tile_add(void);
// タイル反転モノ変色明暗透明スプライト表示（加算）
void tcgd_sprite_reverse_mono_change_bright_clr_tile_add(void);

// 通常表示（減算）
void tcgd_normal_sub(void);
// スプライト表示（減算）
void tcgd_sprite_sub(void);
// 透明通常表示（減算）
void tcgd_normal_clr_sub(void);
// 透明スプライト表示（減算）
void tcgd_sprite_clr_sub(void);
// 変色通常表示（減算）
void tcgd_normal_change_sub(void);
// 変色スプライト表示（減算）
void tcgd_sprite_change_sub(void);
// 変色透明通常表示（減算）
void tcgd_normal_change_clr_sub(void);
// 変色透明スプライト表示（減算）
void tcgd_sprite_change_clr_sub(void);
// 明暗通常表示（減算）
void tcgd_normal_bright_sub(void);
// 明暗スプライト表示（減算）
void tcgd_sprite_bright_sub(void);
// 明暗透明通常表示（減算）
void tcgd_normal_bright_clr_sub(void);
// 明暗透明スプライト表示（減算）
void tcgd_sprite_bright_clr_sub(void);
// 変色明暗通常表示（減算）
void tcgd_normal_change_bright_sub(void);
// 変色明暗スプライト表示（減算）
void tcgd_sprite_change_bright_sub(void);
// 変色明暗透明通常表示（減算）
void tcgd_normal_change_bright_clr_sub(void);
// 変色明暗透明スプライト表示（減算）
void tcgd_sprite_change_bright_clr_sub(void);
// モノ通常表示（減算）
void tcgd_normal_mono_sub(void);
// モノスプライト表示（減算）
void tcgd_sprite_mono_sub(void);
// モノ透明通常表示（減算）
void tcgd_normal_mono_clr_sub(void);
// モノ透明スプライト表示（減算）
void tcgd_sprite_mono_clr_sub(void);
// モノ変色通常表示（減算）
void tcgd_normal_mono_change_sub(void);
// モノ変色スプライト表示（減算）
void tcgd_sprite_mono_change_sub(void);
// モノ変色透明通常表示（減算）
void tcgd_normal_mono_change_clr_sub(void);
// モノ変色透明スプライト表示（減算）
void tcgd_sprite_mono_change_clr_sub(void);
// モノ明暗通常表示（減算）
void tcgd_normal_mono_bright_sub(void);
// モノ明暗スプライト表示（減算）
void tcgd_sprite_mono_bright_sub(void);
// モノ明暗透明通常表示（減算）
void tcgd_normal_mono_bright_clr_sub(void);
// モノ明暗透明スプライト表示（減算）
void tcgd_sprite_mono_bright_clr_sub(void);
// モノ変色明暗通常表示（減算）
void tcgd_normal_mono_change_bright_sub(void);
// モノ変色明暗スプライト表示（減算）
void tcgd_sprite_mono_change_bright_sub(void);
// モノ変色明暗透明通常表示（減算）
void tcgd_normal_mono_change_bright_clr_sub(void);
// モノ変色明暗透明スプライト表示（減算）
void tcgd_sprite_mono_change_bright_clr_sub(void);
// 反転通常表示（減算）
void tcgd_normal_reverse_sub(void);
// 反転スプライト表示（減算）
void tcgd_sprite_reverse_sub(void);
// 反転透明通常表示（減算）
void tcgd_normal_reverse_clr_sub(void);
// 反転透明スプライト表示（減算）
void tcgd_sprite_reverse_clr_sub(void);
// 反転変色通常表示（減算）
void tcgd_normal_reverse_change_sub(void);
// 反転変色スプライト表示（減算）
void tcgd_sprite_reverse_change_sub(void);
// 反転変色透明通常表示（減算）
void tcgd_normal_reverse_change_clr_sub(void);
// 反転変色透明スプライト表示（減算）
void tcgd_sprite_reverse_change_clr_sub(void);
// 反転明暗通常表示（減算）
void tcgd_normal_reverse_bright_sub(void);
// 反転明暗スプライト表示（減算）
void tcgd_sprite_reverse_bright_sub(void);
// 反転明暗透明通常表示（減算）
void tcgd_normal_reverse_bright_clr_sub(void);
// 反転明暗透明スプライト表示（減算）
void tcgd_sprite_reverse_bright_clr_sub(void);
// 反転変色明暗通常表示（減算）
void tcgd_normal_reverse_change_bright_sub(void);
// 反転変色明暗スプライト表示（減算）
void tcgd_sprite_reverse_change_bright_sub(void);
// 反転変色明暗透明通常表示（減算）
void tcgd_normal_reverse_change_bright_clr_sub(void);
// 反転変色明暗透明スプライト表示（減算）
void tcgd_sprite_reverse_change_bright_clr_sub(void);
// 反転モノ通常表示（減算）
void tcgd_normal_reverse_mono_sub(void);
// 反転モノスプライト表示（減算）
void tcgd_sprite_reverse_mono_sub(void);
// 反転モノ透明通常表示（減算）
void tcgd_normal_reverse_mono_clr_sub(void);
// 反転モノ透明スプライト表示（減算）
void tcgd_sprite_reverse_mono_clr_sub(void);
// 反転モノ変色通常表示（減算）
void tcgd_normal_reverse_mono_change_sub(void);
// 反転モノ変色スプライト表示（減算）
void tcgd_sprite_reverse_mono_change_sub(void);
// 反転モノ変色透明通常表示（減算）
void tcgd_normal_reverse_mono_change_clr_sub(void);
// 反転モノ変色透明スプライト表示（減算）
void tcgd_sprite_reverse_mono_change_clr_sub(void);
// 反転モノ明暗通常表示（減算）
void tcgd_normal_reverse_mono_bright_sub(void);
// 反転モノ明暗スプライト表示（減算）
void tcgd_sprite_reverse_mono_bright_sub(void);
// 反転モノ明暗透明通常表示（減算）
void tcgd_normal_reverse_mono_bright_clr_sub(void);
// 反転モノ明暗透明スプライト表示（減算）
void tcgd_sprite_reverse_mono_bright_clr_sub(void);
// 反転モノ変色明暗通常表示（減算）
void tcgd_normal_reverse_mono_change_bright_sub(void);
// 反転モノ変色明暗スプライト表示（減算）
void tcgd_sprite_reverse_mono_change_bright_sub(void);
// 反転モノ変色明暗透明通常表示（減算）
void tcgd_normal_reverse_mono_change_bright_clr_sub(void);
// 反転モノ変色明暗透明スプライト表示（減算）
void tcgd_sprite_reverse_mono_change_bright_clr_sub(void);

// タイル通常表示（減算）
void tcgd_normal_tile_sub(void);
// タイルスプライト表示（減算）
void tcgd_sprite_tile_sub(void);
// タイル透明通常表示（減算）
void tcgd_normal_clr_tile_sub(void);
// タイル透明スプライト表示（減算）
void tcgd_sprite_clr_tile_sub(void);
// タイル変色通常表示（減算）
void tcgd_normal_change_tile_sub(void);
// タイル変色スプライト表示（減算）
void tcgd_sprite_change_tile_sub(void);
// タイル変色透明通常表示（減算）
void tcgd_normal_change_clr_tile_sub(void);
// タイル変色透明スプライト表示（減算）
void tcgd_sprite_change_clr_tile_sub(void);
// タイル明暗通常表示（減算）
void tcgd_normal_bright_tile_sub(void);
// タイル明暗スプライト表示（減算）
void tcgd_sprite_bright_tile_sub(void);
// タイル明暗透明通常表示（減算）
void tcgd_normal_bright_clr_tile_sub(void);
// タイル明暗透明スプライト表示（減算）
void tcgd_sprite_bright_clr_tile_sub(void);
// タイル変色明暗通常表示（減算）
void tcgd_normal_change_bright_tile_sub(void);
// タイル変色明暗スプライト表示（減算）
void tcgd_sprite_change_bright_tile_sub(void);
// タイル変色明暗透明通常表示（減算）
void tcgd_normal_change_bright_clr_tile_sub(void);
// タイル変色明暗透明スプライト表示（減算）
void tcgd_sprite_change_bright_clr_tile_sub(void);
// タイルモノ通常表示（減算）
void tcgd_normal_mono_tile_sub(void);
// タイルモノスプライト表示（減算）
void tcgd_sprite_mono_tile_sub(void);
// タイルモノ透明通常表示（減算）
void tcgd_normal_mono_clr_tile_sub(void);
// タイルモノ透明スプライト表示（減算）
void tcgd_sprite_mono_clr_tile_sub(void);
// タイルモノ変色通常表示（減算）
void tcgd_normal_mono_change_tile_sub(void);
// タイルモノ変色スプライト表示（減算）
void tcgd_sprite_mono_change_tile_sub(void);
// タイルモノ変色透明通常表示（減算）
void tcgd_normal_mono_change_clr_tile_sub(void);
// タイルモノ変色透明スプライト表示（減算）
void tcgd_sprite_mono_change_clr_tile_sub(void);
// タイルモノ明暗通常表示（減算）
void tcgd_normal_mono_bright_tile_sub(void);
// タイルモノ明暗スプライト表示（減算）
void tcgd_sprite_mono_bright_tile_sub(void);
// タイルモノ明暗透明通常表示（減算）
void tcgd_normal_mono_bright_clr_tile_sub(void);
// タイルモノ明暗透明スプライト表示（減算）
void tcgd_sprite_mono_bright_clr_tile_sub(void);
// タイルモノ変色明暗通常表示（減算）
void tcgd_normal_mono_change_bright_tile_sub(void);
// タイルモノ変色明暗スプライト表示（減算）
void tcgd_sprite_mono_change_bright_tile_sub(void);
// タイルモノ変色明暗透明通常表示（減算）
void tcgd_normal_mono_change_bright_clr_tile_sub(void);
// タイルモノ変色明暗透明スプライト表示（減算）
void tcgd_sprite_mono_change_bright_clr_tile_sub(void);
// タイル反転通常表示（減算）
void tcgd_normal_reverse_tile_sub(void);
// タイル反転スプライト表示（減算）
void tcgd_sprite_reverse_tile_sub(void);
// タイル反転透明通常表示（減算）
void tcgd_normal_reverse_clr_tile_sub(void);
// タイル反転透明スプライト表示（減算）
void tcgd_sprite_reverse_clr_tile_sub(void);
// タイル反転変色通常表示（減算）
void tcgd_normal_reverse_change_tile_sub(void);
// タイル反転変色スプライト表示（減算）
void tcgd_sprite_reverse_change_tile_sub(void);
// タイル反転変色透明通常表示（減算）
void tcgd_normal_reverse_change_clr_tile_sub(void);
// タイル反転変色透明スプライト表示（減算）
void tcgd_sprite_reverse_change_clr_tile_sub(void);
// タイル反転明暗通常表示（減算）
void tcgd_normal_reverse_bright_tile_sub(void);
// タイル反転明暗スプライト表示（減算）
void tcgd_sprite_reverse_bright_tile_sub(void);
// タイル反転明暗透明通常表示（減算）
void tcgd_normal_reverse_bright_clr_tile_sub(void);
// タイル反転明暗透明スプライト表示（減算）
void tcgd_sprite_reverse_bright_clr_tile_sub(void);
// タイル反転変色明暗通常表示（減算）
void tcgd_normal_reverse_change_bright_tile_sub(void);
// タイル反転変色明暗スプライト表示（減算）
void tcgd_sprite_reverse_change_bright_tile_sub(void);
// タイル反転変色明暗透明通常表示（減算）
void tcgd_normal_reverse_change_bright_clr_tile_sub(void);
// タイル反転変色明暗透明スプライト表示（減算）
void tcgd_sprite_reverse_change_bright_clr_tile_sub(void);
// タイル反転モノ通常表示（減算）
void tcgd_normal_reverse_mono_tile_sub(void);
// タイル反転モノスプライト表示（減算）
void tcgd_sprite_reverse_mono_tile_sub(void);
// タイル反転モノ透明通常表示（減算）
void tcgd_normal_reverse_mono_clr_tile_sub(void);
// タイル反転モノ透明スプライト表示（減算）
void tcgd_sprite_reverse_mono_clr_tile_sub(void);
// タイル反転モノ変色通常表示（減算）
void tcgd_normal_reverse_mono_change_tile_sub(void);
// タイル反転モノ変色スプライト表示（減算）
void tcgd_sprite_reverse_mono_change_tile_sub(void);
// タイル反転モノ変色透明通常表示（減算）
void tcgd_normal_reverse_mono_change_clr_tile_sub(void);
// タイル反転モノ変色透明スプライト表示（減算）
void tcgd_sprite_reverse_mono_change_clr_tile_sub(void);
// タイル反転モノ明暗通常表示（減算）
void tcgd_normal_reverse_mono_bright_tile_sub(void);
// タイル反転モノ明暗スプライト表示（減算）
void tcgd_sprite_reverse_mono_bright_tile_sub(void);
// タイル反転モノ明暗透明通常表示（減算）
void tcgd_normal_reverse_mono_bright_clr_tile_sub(void);
// タイル反転モノ明暗透明スプライト表示（減算）
void tcgd_sprite_reverse_mono_bright_clr_tile_sub(void);
// タイル反転モノ変色明暗通常表示（減算）
void tcgd_normal_reverse_mono_change_bright_tile_sub(void);
// タイル反転モノ変色明暗スプライト表示（減算）
void tcgd_sprite_reverse_mono_change_bright_tile_sub(void);
// タイル反転モノ変色明暗透明通常表示（減算）
void tcgd_normal_reverse_mono_change_bright_clr_tile_sub(void);
// タイル反転モノ変色明暗透明スプライト表示（減算）
void tcgd_sprite_reverse_mono_change_bright_clr_tile_sub(void);

// 通常表示（ＲＧＢフィルター）
void tcgd_normal_rgbfilter(void);
// スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_rgbfilter(void);
// 透明通常表示（ＲＧＢフィルター）
void tcgd_normal_clr_rgbfilter(void);
// 透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_clr_rgbfilter(void);
// 変色通常表示（ＲＧＢフィルター）
void tcgd_normal_change_rgbfilter(void);
// 変色スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_change_rgbfilter(void);
// 変色透明通常表示（ＲＧＢフィルター）
void tcgd_normal_change_clr_rgbfilter(void);
// 変色透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_change_clr_rgbfilter(void);
// 明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_bright_rgbfilter(void);
// 明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_bright_rgbfilter(void);
// 明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_bright_clr_rgbfilter(void);
// 明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_bright_clr_rgbfilter(void);
// 変色明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_change_bright_rgbfilter(void);
// 変色明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_change_bright_rgbfilter(void);
// 変色明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_change_bright_clr_rgbfilter(void);
// 変色明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_change_bright_clr_rgbfilter(void);
// モノ通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_rgbfilter(void);
// モノスプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_rgbfilter(void);
// モノ透明通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_clr_rgbfilter(void);
// モノ透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_clr_rgbfilter(void);
// モノ変色通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_change_rgbfilter(void);
// モノ変色スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_change_rgbfilter(void);
// モノ変色透明通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_change_clr_rgbfilter(void);
// モノ変色透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_change_clr_rgbfilter(void);
// モノ明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_bright_rgbfilter(void);
// モノ明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_bright_rgbfilter(void);
// モノ明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_bright_clr_rgbfilter(void);
// モノ明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_bright_clr_rgbfilter(void);
// モノ変色明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_change_bright_rgbfilter(void);
// モノ変色明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_change_bright_rgbfilter(void);
// モノ変色明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_change_bright_clr_rgbfilter(void);
// モノ変色明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_change_bright_clr_rgbfilter(void);
// 反転通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_rgbfilter(void);
// 反転スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_rgbfilter(void);
// 反転透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_clr_rgbfilter(void);
// 反転透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_clr_rgbfilter(void);
// 反転変色通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_change_rgbfilter(void);
// 反転変色スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_change_rgbfilter(void);
// 反転変色透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_change_clr_rgbfilter(void);
// 反転変色透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_change_clr_rgbfilter(void);
// 反転明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_bright_rgbfilter(void);
// 反転明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_bright_rgbfilter(void);
// 反転明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_bright_clr_rgbfilter(void);
// 反転明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_bright_clr_rgbfilter(void);
// 反転変色明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_change_bright_rgbfilter(void);
// 反転変色明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_change_bright_rgbfilter(void);
// 反転変色明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_change_bright_clr_rgbfilter(void);
// 反転変色明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_change_bright_clr_rgbfilter(void);
// 反転モノ通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_rgbfilter(void);
// 反転モノスプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_rgbfilter(void);
// 反転モノ透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_clr_rgbfilter(void);
// 反転モノ透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_clr_rgbfilter(void);
// 反転モノ変色通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_change_rgbfilter(void);
// 反転モノ変色スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_change_rgbfilter(void);
// 反転モノ変色透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_change_clr_rgbfilter(void);
// 反転モノ変色透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_change_clr_rgbfilter(void);
// 反転モノ明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_bright_rgbfilter(void);
// 反転モノ明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_bright_rgbfilter(void);
// 反転モノ明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_bright_clr_rgbfilter(void);
// 反転モノ明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_bright_clr_rgbfilter(void);
// 反転モノ変色明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_change_bright_rgbfilter(void);
// 反転モノ変色明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_change_bright_rgbfilter(void);
// 反転モノ変色明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_change_bright_clr_rgbfilter(void);
// 反転モノ変色明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_change_bright_clr_rgbfilter(void);

// タイル通常表示（ＲＧＢフィルター）
void tcgd_normal_tile_rgbfilter(void);
// タイルスプライト表示（ＲＧＢフィルター）
void tcgd_sprite_tile_rgbfilter(void);
// タイル透明通常表示（ＲＧＢフィルター）
void tcgd_normal_clr_tile_rgbfilter(void);
// タイル透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_clr_tile_rgbfilter(void);
// タイル変色通常表示（ＲＧＢフィルター）
void tcgd_normal_change_tile_rgbfilter(void);
// タイル変色スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_change_tile_rgbfilter(void);
// タイル変色透明通常表示（ＲＧＢフィルター）
void tcgd_normal_change_clr_tile_rgbfilter(void);
// タイル変色透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_change_clr_tile_rgbfilter(void);
// タイル明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_bright_tile_rgbfilter(void);
// タイル明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_bright_tile_rgbfilter(void);
// タイル明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_bright_clr_tile_rgbfilter(void);
// タイル明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_bright_clr_tile_rgbfilter(void);
// タイル変色明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_change_bright_tile_rgbfilter(void);
// タイル変色明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_change_bright_tile_rgbfilter(void);
// タイル変色明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_change_bright_clr_tile_rgbfilter(void);
// タイル変色明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_change_bright_clr_tile_rgbfilter(void);
// タイルモノ通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_tile_rgbfilter(void);
// タイルモノスプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_tile_rgbfilter(void);
// タイルモノ透明通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_clr_tile_rgbfilter(void);
// タイルモノ透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_clr_tile_rgbfilter(void);
// タイルモノ変色通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_change_tile_rgbfilter(void);
// タイルモノ変色スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_change_tile_rgbfilter(void);
// タイルモノ変色透明通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_change_clr_tile_rgbfilter(void);
// タイルモノ変色透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_change_clr_tile_rgbfilter(void);
// タイルモノ明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_bright_tile_rgbfilter(void);
// タイルモノ明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_bright_tile_rgbfilter(void);
// タイルモノ明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_bright_clr_tile_rgbfilter(void);
// タイルモノ明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_bright_clr_tile_rgbfilter(void);
// タイルモノ変色明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_change_bright_tile_rgbfilter(void);
// タイルモノ変色明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_change_bright_tile_rgbfilter(void);
// タイルモノ変色明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_mono_change_bright_clr_tile_rgbfilter(void);
// タイルモノ変色明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_mono_change_bright_clr_tile_rgbfilter(void);
// タイル反転通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_tile_rgbfilter(void);
// タイル反転スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_tile_rgbfilter(void);
// タイル反転透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_clr_tile_rgbfilter(void);
// タイル反転透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_clr_tile_rgbfilter(void);
// タイル反転変色通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_change_tile_rgbfilter(void);
// タイル反転変色スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_change_tile_rgbfilter(void);
// タイル反転変色透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_change_clr_tile_rgbfilter(void);
// タイル反転変色透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_change_clr_tile_rgbfilter(void);
// タイル反転明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_bright_tile_rgbfilter(void);
// タイル反転明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_bright_tile_rgbfilter(void);
// タイル反転明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_bright_clr_tile_rgbfilter(void);
// タイル反転明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_bright_clr_tile_rgbfilter(void);
// タイル反転変色明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_change_bright_tile_rgbfilter(void);
// タイル反転変色明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_change_bright_tile_rgbfilter(void);
// タイル反転変色明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_change_bright_clr_tile_rgbfilter(void);
// タイル反転変色明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_change_bright_clr_tile_rgbfilter(void);
// タイル反転モノ通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_tile_rgbfilter(void);
// タイル反転モノスプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_tile_rgbfilter(void);
// タイル反転モノ透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_clr_tile_rgbfilter(void);
// タイル反転モノ透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_clr_tile_rgbfilter(void);
// タイル反転モノ変色通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_change_tile_rgbfilter(void);
// タイル反転モノ変色スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_change_tile_rgbfilter(void);
// タイル反転モノ変色透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_change_clr_tile_rgbfilter(void);
// タイル反転モノ変色透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_change_clr_tile_rgbfilter(void);
// タイル反転モノ明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_bright_tile_rgbfilter(void);
// タイル反転モノ明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_bright_tile_rgbfilter(void);
// タイル反転モノ明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_bright_clr_tile_rgbfilter(void);
// タイル反転モノ明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_bright_clr_tile_rgbfilter(void);
// タイル反転モノ変色明暗通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_change_bright_tile_rgbfilter(void);
// タイル反転モノ変色明暗スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_change_bright_tile_rgbfilter(void);
// タイル反転モノ変色明暗透明通常表示（ＲＧＢフィルター）
void tcgd_normal_reverse_mono_change_bright_clr_tile_rgbfilter(void);
// タイル反転モノ変色明暗透明スプライト表示（ＲＧＢフィルター）
void tcgd_sprite_reverse_mono_change_bright_clr_tile_rgbfilter(void);


// 通常表示（透明度合成：モード３）
void tcgd_normal_mod03_alphablend(void);
// 変色通常表示（透明度合成：モード３）
void tcgd_normal_change_mod03_alphablend(void);
// スプライト表示（透明度合成：モード３）
void tcgd_sprite_mod03_alphablend(void);
// 変色スプライト表示（透明度合成：モード３）
void tcgd_sprite_change_mod03_alphablend(void);


// 波紋タイル表示：モード９９
void tcgd_normal_clr_tile_wave(void);




/*========================================================================================
	ＧＲＰ転送
void true_color_grp_disp(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc, int type,
	int x, int y,
	int ex1, int ey1, int ex2, int ey2,
	int screen_mod,
	UC *tile, UC t_shift, int t_add, int t_xl, int t_yl, int t_repx, int t_repy, int t_reverse,
	UC mono,
	UC reverse,
	UC c_r, UC c_g, UC c_b, UC c_rate,
	int p_r, int p_g, int p_b,
	int bright,
	UC clr,
	int disp_mod
);
		IN		UC *d_buf      : 転送先ポインタ
				int d_xl       : 転送先ＸＬ
				int d_yl       : 転送先ＹＬ
				UC *s_buf      : 転送元ポインタ
				int s_xl       : 転送元ＸＬ
				int s_yl       : 転送元ＹＬ
				int s_xc       : 転送元ＸＣ
				int s_yc       : 転送元ＹＣ
				int type       : 0=通常 1=スプライト
				int x          : 転送座標
				int y          : 転送座標
				int ex1        : 転送先エリアＸ１（(ex1=ey1=ex2=ey2=-1)=転送先最大のエリア）
				int ey1        : 転送先エリアＹ１
				int ex2        : 転送先エリアＸ２
				int ey2        : 転送先エリアＹ２
				int screen_mod : 0=そのまま 1=逆転
				UC *tile       : タイルチップデータへのポインタ（0=なし）
				UC t_shift     : タイルシフト値  ０～８
				int t_add      : タイル加算値  －２５５～２５５（0=なし  負数=下げる  正数=上げる）
				int t_xl       : タイルＸＬ
				int t_yl       : タイルＹＬ
				int t_repx     : タイル座標修正Ｘ
				int t_repy     : タイル座標修正Ｙ
				int t_reverse  : タイルパターンのリバース
				UC mono        : モノクロ  ０－２５５（0=なし  255=モノクロ）
				UC reverse     : リバース  ０－２５５（0=なし  255=リバース）
				UC c_r         : 赤変色値　０－２５５
				UC c_g         : 緑変色値　０－２５５
				UC c_b         : 青変色値　０－２５５
				UC c_rate      : 変色レート　０－２５５（0=なし  255=ベタ）
				int p_r        : 赤味レート　－２５５～２５５（0=なし  負数=赤味下げる  正数=赤味上げる）
				int p_g        : 緑味レート　－２５５～２５５（0=なし  負数=緑味下げる  正数=緑味上げる）
				int p_b        : 青味レート　－２５５～２５５（0=なし  負数=青味下げる  正数=青味上げる）
				int bright     : 明るさ  －２５５～２５５（0=なし  負数=暗く  正数=明るく）
				UC clr         : 透明度　０－２５５（0=表示しない  255=不透明）
				int disp_mod   : 描画モード(0=通常  1=加算  2=減算)
		OUT		なし
----------------------------------------------------------------------------------------*/
void true_color_grp_disp(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc, int type,
	int x, int y,
	int ex1, int ey1, int ex2, int ey2,
	int screen_mod,
	UC *tile, UC t_shift, int t_add, int t_xl, int t_yl, int t_repx, int t_repy, int t_reverse,
	UC mono,
	UC reverse,
	UC c_r, UC c_g, UC c_b, UC c_rate,
	int p_r, int p_g, int p_b,
	int bright,
	UC clr,
	int disp_mod
){
	int tmp;
	int s_free_x, d_free_x, cnt_x;
	int s_free_y, d_free_y, cnt_y;

	if(t_shift < 0){
		t_shift = 0;
	}
	else if(t_shift > 8){
		t_shift = 8;
	}
	if(t_add < -(255<<t_shift)){
		t_add = -(255<<t_shift);
	}
	else if(t_add > 255){
		t_add = 255;
	}

	if(s_buf==0 || d_buf==0 || clr==0 || (tile && t_add==-(255<<t_shift))){
		return;
	}

// エリア設定
	ng_set_eria(d_xl, d_yl, &ex1, &ey1, &ex2, &ey2);
// パラメータＸＣ、ＹＣを修正
	x -= s_xc;
	y -= s_yc;
// 表示範囲外チェック１
	if(x > ex2){return;}
	if(y > ey2){return;}
// 座標調整
	if(x < 0){
		s_free_x = (-x);
		cnt_x    = s_xl - s_free_x;
		d_free_x = 0;
	}
	else{
		s_free_x = 0;
		cnt_x    = s_xl;
		d_free_x = x;
	}
	if(y < 0){
		s_free_y = (-y);
		cnt_y    = s_yl - s_free_y;
		d_free_y = 0;
	}
	else{
		s_free_y = 0;
		cnt_y    = s_yl;
		d_free_y = y;
	}
// 表示範囲外チェック２
	if(cnt_x <= 0){return;}
	if(cnt_y <= 0){return;}
// 始点リミット修正
	if(ex1 > d_free_x){
		tmp = ex1 - d_free_x;
		s_free_x += tmp;
		cnt_x    -= tmp;
		d_free_x  = ex1;
	}
	if(ey1 > d_free_y){
		tmp = ey1 - d_free_y;
		s_free_y += tmp;
		cnt_y    -= tmp;
		d_free_y  = ey1;
	}
// 表示範囲外チェック３
	if(ex2 < d_free_x){return;}
	if(ey2 < d_free_y){return;}
// 終点リミット修正
	tmp = ex2 - d_free_x + 1;
	if(tmp < cnt_x){cnt_x = tmp;}
	tmp = ey2 - d_free_y + 1;
	if(tmp < cnt_y){cnt_y = tmp;}
// 表示範囲外チェック４
	if(cnt_x <= 0){return;}
	if(cnt_y <= 0){return;}

// アドレス算出
	s_buf += (s_free_x + (s_free_y * s_xl)) << 2;
	if(screen_mod == 0){
		d_buf += (d_free_x + (d_free_y * d_xl)) << 2;
	}
	else{
		d_buf += (d_free_x + ((d_yl-d_free_y-1) * d_xl)) << 2;
	}

// 描画パラメータ
	GRPF_d_buf = d_buf;
	GRPF_d_xl  = d_xl << 2;
	GRPF_d_yl  = d_yl;

	GRPF_s_buf = s_buf;
	GRPF_s_xl  = s_xl << 2;
	GRPF_s_yl  = s_yl;

	GRPF_cnt_x_org = cnt_x;
	GRPF_cnt_y     = cnt_y;

	if(screen_mod == 0){
		GRPF_d_xl -= (cnt_x << 2);
		GRPF_s_xl -= (cnt_x << 2);
	}
	else{
		GRPF_d_xl  = -(GRPF_d_xl + (cnt_x << 2));
		GRPF_s_xl -= (cnt_x << 2);
	}

// 反転パラメータ
	CTPREV = *(Gclr_table_adr_p + (255 - reverse));
	GRPF_reverse = reverse;

// モノパラメータ
	GRPF_mono    = (255 - mono) << 2;

// 変色パラメータ
	if(c_rate){
		GRPF_c_r = c_r;
		GRPF_c_g = c_g;
		GRPF_c_b = c_b;
		CTPBETA = *(Gclr_table_adr_p + (255 - c_rate));
	}

// 透明度パラメータ
	GRPF_clr4 = (clr << 2);
	GRPF_clrR = 255 - clr;
	CTPCLR = *(Gclr_table_adr_p + clr);

// 色味パラメータ
	if(bright < -255){
		bright = -255;
	}
	else if(bright > 255){
		bright = 255;
	}
	if(p_r < -255){
		p_r = -255;
	}
	else if(p_r > 255){
		p_r = 255;
	}
	if(p_g < -255){
		p_g = -255;
	}
	else if(p_g > 255){
		p_g = 255;
	}
	if(p_b < -255){
		p_b = -255;
	}
	else if(p_b > 255){
		p_b = 255;
	}
	if(bright < 0){
		p_r += (((255 + p_r) * bright) / 255);
		p_g += (((255 + p_g) * bright) / 255);
		p_b += (((255 + p_b) * bright) / 255);
	}
	else if(bright > 0){
		p_r += (((255 - p_r) * bright) / 255);
		p_g += (((255 - p_g) * bright) / 255);
		p_b += (((255 - p_b) * bright) / 255);
	}

	if(disp_mod == 2){		// 減算モード
		p_r = -(p_r);
		p_g = -(p_g);
		p_b = -(p_b);
	}

	RGBFILTER_p_r = p_r;
	RGBFILTER_p_g = p_g;
	RGBFILTER_p_b = p_b;

	if(p_r >= 0){
		CTPR = *(Gclr_table_adr_p + (255 - p_r));
		GRPF_p_r = 255;
	}
	else{
		CTPR = *(Gclr_table_adr_p + (255 + p_r));
		GRPF_p_r = 0;
	}
	if(p_g >= 0){
		CTPG = *(Gclr_table_adr_p + (255 - p_g));
		GRPF_p_g = 255;
	}
	else{
		CTPG = *(Gclr_table_adr_p + (255 + p_g));
		GRPF_p_g = 0;
	}
	if(p_b >= 0){
		CTPB = *(Gclr_table_adr_p + (255 - p_b));
		GRPF_p_b = 255;
	}
	else{
		CTPB = *(Gclr_table_adr_p + (255 + p_b));
		GRPF_p_b = 0;
	}

// タイルパラメータ
	if(tile){
		GRPF_t_add = t_add;
		GRPF_t_reverse = t_reverse;
		GRPF_t_shift = t_shift;
		GRPF_t_xl = t_xl;
		GRPF_t_yl = t_yl;

		if(d_free_x >= t_repx){
			GRPF_t_x = GRPF_t_x_org = (d_free_x - t_repx) % t_xl;
		}
		else{
			GRPF_t_x = GRPF_t_x_org = (t_xl - ((t_repx - d_free_x) % t_xl)) % t_xl;
		}
		if(d_free_y >= t_repy){
			GRPF_t_y                = (d_free_y - t_repy) % t_yl;
		}
		else{
			GRPF_t_y                = (t_yl - ((t_repy - d_free_y) % t_yl)) % t_yl;
		}

		GRPF_t_adr     = tile + (GRPF_t_y * t_xl);
		GRPF_t_adr_org = tile;
		GRPF_t_adr_max = tile + (t_xl * t_yl);
	}

//****************************************************************************************

	if(type == 0){
		if(clr == 255){
			if(c_rate == 0){
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 通常表示
								switch(disp_mod){
									case 0: tcgd_normal(); break;
									case 1: tcgd_normal_add(); break;
									case 2: tcgd_normal_sub(); break;
									case 3: tcgd_normal_mod03_alphablend(); break;
									case 4: tcgd_normal_rgbfilter(); break;
								}
							}
							else{
// タイル通常表示
								switch(disp_mod){
									case 0: tcgd_normal_tile(); break;
									case 1: tcgd_normal_tile_add(); break;
									case 2: tcgd_normal_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_tile_rgbfilter(); break;
									case 99: tcgd_normal(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse(); break;
									case 1: tcgd_normal_reverse_add(); break;
									case 2: tcgd_normal_reverse_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_rgbfilter(); break;
								}
							}
							else{
// タイル反転通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_tile(); break;
									case 1: tcgd_normal_reverse_tile_add(); break;
									case 2: tcgd_normal_reverse_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono(); break;
									case 1: tcgd_normal_mono_add(); break;
									case 2: tcgd_normal_mono_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_rgbfilter(); break;
								}
							}
							else{
// タイルモノ通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_tile(); break;
									case 1: tcgd_normal_mono_tile_add(); break;
									case 2: tcgd_normal_mono_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono(); break;
									case 1: tcgd_normal_reverse_mono_add(); break;
									case 2: tcgd_normal_reverse_mono_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_tile(); break;
									case 1: tcgd_normal_reverse_mono_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_bright(); break;
									case 1: tcgd_normal_bright_add(); break;
									case 2: tcgd_normal_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_bright_rgbfilter(); break;
								}
							}
							else{
// タイル明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_bright_tile(); break;
									case 1: tcgd_normal_bright_tile_add(); break;
									case 2: tcgd_normal_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_bright(); break;
									case 1: tcgd_normal_reverse_bright_add(); break;
									case 2: tcgd_normal_reverse_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_bright_rgbfilter(); break;
								}
							}
							else{
// タイル反転明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_bright_tile(); break;
									case 1: tcgd_normal_reverse_bright_tile_add(); break;
									case 2: tcgd_normal_reverse_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_bright(); break;
									case 1: tcgd_normal_mono_bright_add(); break;
									case 2: tcgd_normal_mono_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_bright_rgbfilter(); break;
								}
							}
							else{
// タイルモノ明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_bright_tile(); break;
									case 1: tcgd_normal_mono_bright_tile_add(); break;
									case 2: tcgd_normal_mono_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_bright(); break;
									case 1: tcgd_normal_reverse_mono_bright_add(); break;
									case 2: tcgd_normal_reverse_mono_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_bright_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_bright_tile(); break;
									case 1: tcgd_normal_reverse_mono_bright_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
			else{
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 変色通常表示
								switch(disp_mod){
									case 0: tcgd_normal_change(); break;
									case 1: tcgd_normal_change_add(); break;
									case 2: tcgd_normal_change_sub(); break;
									case 3: tcgd_normal_change_mod03_alphablend(); break;
									case 4: tcgd_normal_change_rgbfilter(); break;
								}
							}
							else{
// タイル変色通常表示
								switch(disp_mod){
									case 0: tcgd_normal_change_tile(); break;
									case 1: tcgd_normal_change_tile_add(); break;
									case 2: tcgd_normal_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転変色通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change(); break;
									case 1: tcgd_normal_reverse_change_add(); break;
									case 2: tcgd_normal_reverse_change_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_rgbfilter(); break;
								}
							}
							else{
// タイル反転変色通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_tile(); break;
									case 1: tcgd_normal_reverse_change_tile_add(); break;
									case 2: tcgd_normal_reverse_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ変色通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_change(); break;
									case 1: tcgd_normal_mono_change_add(); break;
									case 2: tcgd_normal_mono_change_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_rgbfilter(); break;
								}
							}
							else{
// タイルモノ変色通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_tile(); break;
									case 1: tcgd_normal_mono_change_tile_add(); break;
									case 2: tcgd_normal_mono_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ変色通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change(); break;
									case 1: tcgd_normal_reverse_mono_change_add(); break;
									case 2: tcgd_normal_reverse_mono_change_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ変色通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_tile(); break;
									case 1: tcgd_normal_reverse_mono_change_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 変色明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_change_bright(); break;
									case 1: tcgd_normal_change_bright_add(); break;
									case 2: tcgd_normal_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_bright_rgbfilter(); break;
								}
							}
							else{
// タイル変色明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_change_bright_tile(); break;
									case 1: tcgd_normal_change_bright_tile_add(); break;
									case 2: tcgd_normal_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転変色明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_bright(); break;
									case 1: tcgd_normal_reverse_change_bright_add(); break;
									case 2: tcgd_normal_reverse_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_bright_rgbfilter(); break;
								}
							}
							else{
// タイル反転変色明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_bright_tile(); break;
									case 1: tcgd_normal_reverse_change_bright_tile_add(); break;
									case 2: tcgd_normal_reverse_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ変色明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_bright(); break;
									case 1: tcgd_normal_mono_change_bright_add(); break;
									case 2: tcgd_normal_mono_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_bright_rgbfilter(); break;
								}
							}
							else{
// タイルモノ変色明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_bright_tile(); break;
									case 1: tcgd_normal_mono_change_bright_tile_add(); break;
									case 2: tcgd_normal_mono_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ変色明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_bright(); break;
									case 1: tcgd_normal_reverse_mono_change_bright_add(); break;
									case 2: tcgd_normal_reverse_mono_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_bright_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ変色明暗通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_bright_tile(); break;
									case 1: tcgd_normal_reverse_mono_change_bright_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
		}
		else{
			if(c_rate == 0){
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_clr(); break;
									case 1: tcgd_normal_clr_add(); break;
									case 2: tcgd_normal_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_clr_rgbfilter(); break;
								}
							}
							else{
// タイル透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_clr_tile(); break;
									case 1: tcgd_normal_clr_tile_add(); break;
									case 2: tcgd_normal_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_clr_tile_rgbfilter(); break;
									case 99: tcgd_normal_clr_tile_wave(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_clr(); break;
									case 1: tcgd_normal_reverse_clr_add(); break;
									case 2: tcgd_normal_reverse_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_clr_tile(); break;
									case 1: tcgd_normal_reverse_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_clr(); break;
									case 1: tcgd_normal_mono_clr_add(); break;
									case 2: tcgd_normal_mono_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_clr_rgbfilter(); break;
								}
							}
							else{
// タイルモノ透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_clr_tile(); break;
									case 1: tcgd_normal_mono_clr_tile_add(); break;
									case 2: tcgd_normal_mono_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_clr(); break;
									case 1: tcgd_normal_reverse_mono_clr_add(); break;
									case 2: tcgd_normal_reverse_mono_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_clr_tile(); break;
									case 1: tcgd_normal_reverse_mono_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_bright_clr(); break;
									case 1: tcgd_normal_bright_clr_add(); break;
									case 2: tcgd_normal_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_bright_clr_tile(); break;
									case 1: tcgd_normal_bright_clr_tile_add(); break;
									case 2: tcgd_normal_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_bright_clr(); break;
									case 1: tcgd_normal_reverse_bright_clr_add(); break;
									case 2: tcgd_normal_reverse_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_bright_clr_tile(); break;
									case 1: tcgd_normal_reverse_bright_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_bright_clr(); break;
									case 1: tcgd_normal_mono_bright_clr_add(); break;
									case 2: tcgd_normal_mono_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイルモノ明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_bright_clr_tile(); break;
									case 1: tcgd_normal_mono_bright_clr_tile_add(); break;
									case 2: tcgd_normal_mono_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_bright_clr(); break;
									case 1: tcgd_normal_reverse_mono_bright_clr_add(); break;
									case 2: tcgd_normal_reverse_mono_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_bright_clr_tile(); break;
									case 1: tcgd_normal_reverse_mono_bright_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
			else{
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 変色透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_change_clr(); break;
									case 1: tcgd_normal_change_clr_add(); break;
									case 2: tcgd_normal_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_clr_rgbfilter(); break;
								}
							}
							else{
// タイル変色透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_change_clr_tile(); break;
									case 1: tcgd_normal_change_clr_tile_add(); break;
									case 2: tcgd_normal_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転変色透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_clr(); break;
									case 1: tcgd_normal_reverse_change_clr_add(); break;
									case 2: tcgd_normal_reverse_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転変色透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_clr_tile(); break;
									case 1: tcgd_normal_reverse_change_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ変色透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_clr(); break;
									case 1: tcgd_normal_mono_change_clr_add(); break;
									case 2: tcgd_normal_mono_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_clr_rgbfilter(); break;
								}
							}
							else{
// タイルモノ変色透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_clr_tile(); break;
									case 1: tcgd_normal_mono_change_clr_tile_add(); break;
									case 2: tcgd_normal_mono_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ変色透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_clr(); break;
									case 1: tcgd_normal_reverse_mono_change_clr_add(); break;
									case 2: tcgd_normal_reverse_mono_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ変色透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_clr_tile(); break;
									case 1: tcgd_normal_reverse_mono_change_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 変色明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_change_bright_clr(); break;
									case 1: tcgd_normal_change_bright_clr_add(); break;
									case 2: tcgd_normal_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル変色明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_change_bright_clr_tile(); break;
									case 1: tcgd_normal_change_bright_clr_tile_add(); break;
									case 2: tcgd_normal_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転変色明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_bright_clr(); break;
									case 1: tcgd_normal_reverse_change_bright_clr_add(); break;
									case 2: tcgd_normal_reverse_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転変色明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_bright_clr_tile(); break;
									case 1: tcgd_normal_reverse_change_bright_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ変色明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_bright_clr(); break;
									case 1: tcgd_normal_mono_change_bright_clr_add(); break;
									case 2: tcgd_normal_mono_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイルモノ変色明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_bright_clr_tile(); break;
									case 1: tcgd_normal_mono_change_bright_clr_tile_add(); break;
									case 2: tcgd_normal_mono_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ変色明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_bright_clr(); break;
									case 1: tcgd_normal_reverse_mono_change_bright_clr_add(); break;
									case 2: tcgd_normal_reverse_mono_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ変色明暗透明通常表示
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_bright_clr_tile(); break;
									case 1: tcgd_normal_reverse_mono_change_bright_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
		}
	}
//****************************************************************************************
	else{
		if(clr == 255){
			if(c_rate == 0){
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite(); break;
									case 1: tcgd_sprite_add(); break;
									case 2: tcgd_sprite_sub(); break;
									case 3: tcgd_sprite_mod03_alphablend(); break;
									case 4: tcgd_sprite_rgbfilter(); break;
								}
							}
							else{
// タイルスプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_tile(); break;
									case 1: tcgd_sprite_tile_add(); break;
									case 2: tcgd_sprite_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse(); break;
									case 1: tcgd_sprite_reverse_add(); break;
									case 2: tcgd_sprite_reverse_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_rgbfilter(); break;
								}
							}
							else{
// タイル反転スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_tile(); break;
									case 1: tcgd_sprite_reverse_tile_add(); break;
									case 2: tcgd_sprite_reverse_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノスプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono(); break;
									case 1: tcgd_sprite_mono_add(); break;
									case 2: tcgd_sprite_mono_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_rgbfilter(); break;
								}
							}
							else{
// タイルモノスプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_tile(); break;
									case 1: tcgd_sprite_mono_tile_add(); break;
									case 2: tcgd_sprite_mono_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノスプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono(); break;
									case 1: tcgd_sprite_reverse_mono_add(); break;
									case 2: tcgd_sprite_reverse_mono_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノスプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_tile(); break;
									case 1: tcgd_sprite_reverse_mono_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_bright(); break;
									case 1: tcgd_sprite_bright_add(); break;
									case 2: tcgd_sprite_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_bright_rgbfilter(); break;
								}
							}
							else{
// タイル明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_bright_tile(); break;
									case 1: tcgd_sprite_bright_tile_add(); break;
									case 2: tcgd_sprite_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_bright(); break;
									case 1: tcgd_sprite_reverse_bright_add(); break;
									case 2: tcgd_sprite_reverse_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_bright_rgbfilter(); break;
								}
							}
							else{
// タイル反転明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_bright_tile(); break;
									case 1: tcgd_sprite_reverse_bright_tile_add(); break;
									case 2: tcgd_sprite_reverse_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_bright(); break;
									case 1: tcgd_sprite_mono_bright_add(); break;
									case 2: tcgd_sprite_mono_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_bright_rgbfilter(); break;
								}
							}
							else{
// タイルモノ明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_bright_tile(); break;
									case 1: tcgd_sprite_mono_bright_tile_add(); break;
									case 2: tcgd_sprite_mono_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_bright(); break;
									case 1: tcgd_sprite_reverse_mono_bright_add(); break;
									case 2: tcgd_sprite_reverse_mono_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_bright_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_bright_tile(); break;
									case 1: tcgd_sprite_reverse_mono_bright_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
			else{
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 変色スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_change(); break;
									case 1: tcgd_sprite_change_add(); break;
									case 2: tcgd_sprite_change_sub(); break;
									case 3: tcgd_sprite_change_mod03_alphablend(); break;
									case 4: tcgd_sprite_change_rgbfilter(); break;
								}
							}
							else{
// タイル変色通常表示
								switch(disp_mod){
									case 0: tcgd_sprite_change_tile(); break;
									case 1: tcgd_sprite_change_tile_add(); break;
									case 2: tcgd_sprite_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転変色スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change(); break;
									case 1: tcgd_sprite_reverse_change_add(); break;
									case 2: tcgd_sprite_reverse_change_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_rgbfilter(); break;
								}
							}
							else{
// タイル反転変色スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_tile(); break;
									case 1: tcgd_sprite_reverse_change_tile_add(); break;
									case 2: tcgd_sprite_reverse_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ変色スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change(); break;
									case 1: tcgd_sprite_mono_change_add(); break;
									case 2: tcgd_sprite_mono_change_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_rgbfilter(); break;
								}
							}
							else{
// タイルモノ変色スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_tile(); break;
									case 1: tcgd_sprite_mono_change_tile_add(); break;
									case 2: tcgd_sprite_mono_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ変色スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change(); break;
									case 1: tcgd_sprite_reverse_mono_change_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ変色スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_tile(); break;
									case 1: tcgd_sprite_reverse_mono_change_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 変色明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_change_bright(); break;
									case 1: tcgd_sprite_change_bright_add(); break;
									case 2: tcgd_sprite_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_bright_rgbfilter(); break;
								}
							}
							else{
// タイル変色明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_change_bright_tile(); break;
									case 1: tcgd_sprite_change_bright_tile_add(); break;
									case 2: tcgd_sprite_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転変色明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_bright(); break;
									case 1: tcgd_sprite_reverse_change_bright_add(); break;
									case 2: tcgd_sprite_reverse_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_bright_rgbfilter(); break;
								}
							}
							else{
// タイル反転変色明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_bright_tile(); break;
									case 1: tcgd_sprite_reverse_change_bright_tile_add(); break;
									case 2: tcgd_sprite_reverse_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ変色明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_bright(); break;
									case 1: tcgd_sprite_mono_change_bright_add(); break;
									case 2: tcgd_sprite_mono_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_bright_rgbfilter(); break;
								}
							}
							else{
// タイルモノ変色明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_bright_tile(); break;
									case 1: tcgd_sprite_mono_change_bright_tile_add(); break;
									case 2: tcgd_sprite_mono_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ変色明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_bright(); break;
									case 1: tcgd_sprite_reverse_mono_change_bright_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_bright_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ変色明暗スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_bright_tile(); break;
									case 1: tcgd_sprite_reverse_mono_change_bright_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
		}
		else{
			if(c_rate == 0){
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_clr(); break;
									case 1: tcgd_sprite_clr_add(); break;
									case 2: tcgd_sprite_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_clr_rgbfilter(); break;
								}
							}
							else{
// タイル透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_clr_tile(); break;
									case 1: tcgd_sprite_clr_tile_add(); break;
									case 2: tcgd_sprite_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_clr(); break;
									case 1: tcgd_sprite_reverse_clr_add(); break;
									case 2: tcgd_sprite_reverse_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_clr_tile(); break;
									case 1: tcgd_sprite_reverse_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_clr(); break;
									case 1: tcgd_sprite_mono_clr_add(); break;
									case 2: tcgd_sprite_mono_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_clr_rgbfilter(); break;
								}
							}
							else{
// タイルモノ透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_clr_tile(); break;
									case 1: tcgd_sprite_mono_clr_tile_add(); break;
									case 2: tcgd_sprite_mono_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_clr(); break;
									case 1: tcgd_sprite_reverse_mono_clr_add(); break;
									case 2: tcgd_sprite_reverse_mono_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_clr_tile(); break;
									case 1: tcgd_sprite_reverse_mono_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_bright_clr(); break;
									case 1: tcgd_sprite_bright_clr_add(); break;
									case 2: tcgd_sprite_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_bright_clr_tile(); break;
									case 1: tcgd_sprite_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_bright_clr(); break;
									case 1: tcgd_sprite_reverse_bright_clr_add(); break;
									case 2: tcgd_sprite_reverse_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_bright_clr_tile(); break;
									case 1: tcgd_sprite_reverse_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_bright_clr(); break;
									case 1: tcgd_sprite_mono_bright_clr_add(); break;
									case 2: tcgd_sprite_mono_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイルモノ明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_bright_clr_tile(); break;
									case 1: tcgd_sprite_mono_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_mono_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_bright_clr(); break;
									case 1: tcgd_sprite_reverse_mono_bright_clr_add(); break;
									case 2: tcgd_sprite_reverse_mono_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_bright_clr_tile(); break;
									case 1: tcgd_sprite_reverse_mono_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
			else{
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 変色透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_change_clr(); break;
									case 1: tcgd_sprite_change_clr_add(); break;
									case 2: tcgd_sprite_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_clr_rgbfilter(); break;
								}
							}
							else{
// タイル変色透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_change_clr_tile(); break;
									case 1: tcgd_sprite_change_clr_tile_add(); break;
									case 2: tcgd_sprite_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転変色透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_clr(); break;
									case 1: tcgd_sprite_reverse_change_clr_add(); break;
									case 2: tcgd_sprite_reverse_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転変色透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_clr_tile(); break;
									case 1: tcgd_sprite_reverse_change_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ変色透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_clr(); break;
									case 1: tcgd_sprite_mono_change_clr_add(); break;
									case 2: tcgd_sprite_mono_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_clr_rgbfilter(); break;
								}
							}
							else{
// タイルモノ変色透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_clr_tile(); break;
									case 1: tcgd_sprite_mono_change_clr_tile_add(); break;
									case 2: tcgd_sprite_mono_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ変色透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_clr(); break;
									case 1: tcgd_sprite_reverse_mono_change_clr_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ変色透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_clr_tile(); break;
									case 1: tcgd_sprite_reverse_mono_change_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// 変色明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_change_bright_clr(); break;
									case 1: tcgd_sprite_change_bright_clr_add(); break;
									case 2: tcgd_sprite_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル変色明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_change_bright_clr_tile(); break;
									case 1: tcgd_sprite_change_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転変色明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_bright_clr(); break;
									case 1: tcgd_sprite_reverse_change_bright_clr_add(); break;
									case 2: tcgd_sprite_reverse_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転変色明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_bright_clr_tile(); break;
									case 1: tcgd_sprite_reverse_change_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// モノ変色明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_bright_clr(); break;
									case 1: tcgd_sprite_mono_change_bright_clr_add(); break;
									case 2: tcgd_sprite_mono_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイルモノ変色明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_bright_clr_tile(); break;
									case 1: tcgd_sprite_mono_change_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_mono_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// 反転モノ変色明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_bright_clr(); break;
									case 1: tcgd_sprite_reverse_mono_change_bright_clr_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// タイル反転モノ変色明暗透明スプライト表示
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_bright_clr_tile(); break;
									case 1: tcgd_sprite_reverse_mono_change_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
		}
	}
}



void true_color_grp_disp_eazy(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc, int type,
	int x, int y,
	int ex1, int ey1, int ex2, int ey2,
	UC clr
){
	int tmp;
	int s_free_x, d_free_x, cnt_x;
	int s_free_y, d_free_y, cnt_y;

	if(s_buf==0 || d_buf==0 || clr==0){
		return;
	}

// エリア設定
	ng_set_eria(d_xl, d_yl, &ex1, &ey1, &ex2, &ey2);
// パラメータＸＣ、ＹＣを修正
	x -= s_xc;
	y -= s_yc;
// 表示範囲外チェック１
	if(x > ex2){return;}
	if(y > ey2){return;}
// 座標調整
	if(x < 0){
		s_free_x = (-x);
		cnt_x    = s_xl - s_free_x;
		d_free_x = 0;
	}
	else{
		s_free_x = 0;
		cnt_x    = s_xl;
		d_free_x = x;
	}
	if(y < 0){
		s_free_y = (-y);
		cnt_y    = s_yl - s_free_y;
		d_free_y = 0;
	}
	else{
		s_free_y = 0;
		cnt_y    = s_yl;
		d_free_y = y;
	}
// 表示範囲外チェック２
	if(cnt_x <= 0){return;}
	if(cnt_y <= 0){return;}
// 始点リミット修正
	if(ex1 > d_free_x){
		tmp = ex1 - d_free_x;
		s_free_x += tmp;
		cnt_x    -= tmp;
		d_free_x  = ex1;
	}
	if(ey1 > d_free_y){
		tmp = ey1 - d_free_y;
		s_free_y += tmp;
		cnt_y    -= tmp;
		d_free_y  = ey1;
	}
// 表示範囲外チェック３
	if(ex2 < d_free_x){return;}
	if(ey2 < d_free_y){return;}
// 終点リミット修正
	tmp = ex2 - d_free_x + 1;
	if(tmp < cnt_x){cnt_x = tmp;}
	tmp = ey2 - d_free_y + 1;
	if(tmp < cnt_y){cnt_y = tmp;}
// 表示範囲外チェック４
	if(cnt_x <= 0){return;}
	if(cnt_y <= 0){return;}

// アドレス算出
	s_buf += (s_free_x + (s_free_y * s_xl)) << 2;
	d_buf += (d_free_x + (d_free_y * d_xl)) << 2;

// 描画パラメータ
	GRPF_d_buf = d_buf;
	GRPF_d_xl  = d_xl << 2;
	GRPF_d_yl  = d_yl;

	GRPF_s_buf = s_buf;
	GRPF_s_xl  = s_xl << 2;
	GRPF_s_yl  = s_yl;

	GRPF_cnt_x_org = cnt_x;
	GRPF_cnt_y     = cnt_y;

	GRPF_d_xl -= (cnt_x << 2);
	GRPF_s_xl -= (cnt_x << 2);

// 透明度パラメータ
	GRPF_clr4 = (clr << 2);
	GRPF_clrR = 255 - clr;
	CTPCLR = *(Gclr_table_adr_p + clr);

//****************************************************************************************

	if(type == 0){
		if(clr == 255){
// 通常表示
tcgd_normal();
		}
		else{
// 透明通常表示
tcgd_normal_clr();
		}
	}
	else{
		if(clr == 255){
// スプライト表示
tcgd_sprite();
		}
		else{
// 透明スプライト表示
tcgd_sprite_clr();
		}
	}
}

