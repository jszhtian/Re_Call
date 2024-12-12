#pragma		once

// ****************************************************************
// 描画パラメーターをローカルにコピーする
// ================================================================
#define		__PARAM_LOCAL_COPY \
					TCGD_FUNC_PARAM	fdp; \
					memcpy(&fdp, _fdp, sizeof(TCGD_FUNC_PARAM));

// ****************************************************************
// 変換テーブル
// ================================================================

// 透明度
#define		__TABLE_TR				OFFSET Gnamalib_grp_table_tr_adr

// モノ
#define		__TABLE_MONO_R			OFFSET Gnamalib_grp_table_mono_r
#define		__TABLE_MONO_G			OFFSET Gnamalib_grp_table_mono_g
#define		__TABLE_MONO_B			OFFSET Gnamalib_grp_table_mono_b

// ＲＧＢフィルタ
#define		__TABLE_FLT				Gnamalib_grp_table_flt_adr_neutral

// アルファ合成
#define		__TABLE_ALPHABLEND_TR		OFFSET Gnamalib_grp_table_alphablend_tr_adr
#define		__TABLE_ALPHABLEND_WORK1	OFFSET Gnamalib_grp_table_alphablend_work1_adr
#define		__TABLE_ALPHABLEND_WORK2	OFFSET Gnamalib_grp_table_alphablend_work2_adr

// ****************************************************************
// 描画パラメータ
// ================================================================

#define		__DST_PTR				fdp.dst_draw_ptr
#define		__SRC_PTR				fdp.src_draw_ptr

#define		__DST_NEXT				fdp.dst_next_adr_add_byte
#define		__SRC_NEXT				fdp.src_next_adr_add_byte

#define		__DRAW_YCNT				fdp.draw_pixel_y_cnt
#define		__DRAW_XCNT				fdp.draw_pixel_x_cnt

#define		__TR_4					fdp.tr4
#define		__TR_ADR				fdp.tr_table_adr

#define		__ALPHABLEND_A			fdp.alphablend_a
#define		__ALPHABLEND_R			fdp.alphablend_r
#define		__ALPHABLEND_G			fdp.alphablend_g
#define		__ALPHABLEND_B			fdp.alphablend_b

#define		__BRIGHT_R_FLAG			fdp.bright_r_flag
#define		__BRIGHT_G_FLAG			fdp.bright_g_flag
#define		__BRIGHT_B_FLAG			fdp.bright_b_flag
#define		__BRIGHT_R_ADR			fdp.bright_r_table_adr
#define		__BRIGHT_G_ADR			fdp.bright_g_table_adr
#define		__BRIGHT_B_ADR			fdp.bright_b_table_adr

#define		__COLOR_R				fdp.color_r
#define		__COLOR_G				fdp.color_g
#define		__COLOR_B				fdp.color_b
#define		__COLOR_ADR				fdp.color_table_adr

#define		__MONO					fdp.mono
#define		__MONO_FLAG				fdp.mono_flag
#define		__MONO_ADR				fdp.mono_table_adr

#define		__REVERSE_ADR			fdp.reverse_table_adr

#define		__TILE_START_X			fdp.tile_start_pos.x
#define		__TILE_XL				fdp.tile_size.cx
#define		__TILE_YL				fdp.tile_size.cy
#define		__TILE_NOW_X			fdp.tile_now_pos.x
#define		__TILE_NOW_Y			fdp.tile_now_pos.y
#define		__TILE_ADR				fdp.tile_ptr
#define		__TILE_NOW_ADR			fdp.tile_now_ptr
#define		__TILE_END_ADR			fdp.tile_end_ptr
#define		__TILE_REVERSE			fdp.tile_reverse
#define		__TILE_SHIFT			fdp.tile_shift
#define		__TILE_ADD				fdp.tile_add
#define		__TILE_XL_FLOW			fdp.tile_xl_flow
#define		__TILE_YL_FLOW			fdp.tile_yl_flow

#define		__FLT_R					fdp.flt_r
#define		__FLT_G					fdp.flt_g
#define		__FLT_B					fdp.flt_b

// ****************************************************************
// Ｙ軸基準のライン式描画パラメータ
// ================================================================
#define		__LBDP_SRC_PTR				fdp.lbdp_src_draw_ptr

#define		__LBDP_DST_PTR				fdp.lbdp_dst_draw_ptr
#define		__LBDP_DST_MIN				fdp.lbdp_dst_draw_min_ptr
#define		__LBDP_DST_MAX				fdp.lbdp_dst_draw_max_ptr
#define		__LBDP_DST_SIZE				fdp.lbdp_dst_draw_size
#define		__LBDP_DST_CNT				fdp.lbdp_dst_draw_cnt
#define		__LBDP_DST_ALPHA			fdp.lbdp_dst_alpha

#define		__LBDP_SW					fdp.lbdp_sw
#define		__LBDP_NS					fdp.lbdp_ns
#define		__LBDP_NA					fdp.lbdp_na
#define		__LBDP_NH					fdp.lbdp_nh
#define		__LBDP_CNT					fdp.lbdp_cnt
#define		__LBDP_ALPHA				fdp.lbdp_alpha
#define		__LBDP_BETA					fdp.lbdp_beta

// ****************************************************************
// ライン描画パラメータ
// ================================================================
#define		__LINEDP_SRC_PTR			fdp.linedp_src_draw_ptr
#define		__LINEDP_DST_PTR			fdp.linedp_dst_draw_ptr

#define		__LINEDP_LINE_PTR			fdp.linedp_line_buffer_ptr
#define		__LINEDP_LINE_CNT			fdp.linedp_line_buffer_cnt

#define		__LINEDP_X_ADD				fdp.linedp_dst_add_x
#define		__LINEDP_Y_ADD				fdp.linedp_dst_add_y

// ****************************************************************
// ピクセル
// ================================================================

// ピクセルＲを取得
#define		__GET_SRCDST_R \
__asm				xor		ebx, ebx \
__asm				xor		eax, eax \
__asm				mov		bl, [esi+2] \
__asm				mov		al, [edi+2]

// ピクセルＧを取得
#define		__GET_SRCDST_G \
__asm				xor		ebx, ebx \
__asm				xor		eax, eax \
__asm				mov		bl, [esi+1] \
__asm				mov		al, [edi+1]

// ピクセルＢを取得
#define		__GET_SRCDST_B \
__asm				xor		ebx, ebx \
__asm				xor		eax, eax \
__asm				mov		bl, [esi] \
__asm				mov		al, [edi]

// ================================================================

// 描画元ピクセルＲを取得（ＥＢＸレジスタに取得）
#define		__GET_SRC_R_EBX \
__asm				xor		ebx, ebx \
__asm				mov		bl, [esi+2]

// 描画元ピクセルＧを取得（ＥＢＸレジスタに取得）
#define		__GET_SRC_G_EBX \
__asm				xor		ebx, ebx \
__asm				mov		bl, [esi+1]

// 描画元ピクセルＢを取得（ＥＢＸレジスタに取得）
#define		__GET_SRC_B_EBX \
__asm				xor		ebx, ebx \
__asm				mov		bl, [esi]

// 描画元ピクセルＡを取得（ＥＢＸレジスタに取得）
#define		__GET_SRC_A_EBX \
__asm				xor		ebx, ebx \
__asm				mov		bl, [esi+3]

// ================================================================

// 描画元ピクセルＲを取得（ＥＡＸレジスタに取得）
#define		__GET_SRC_R_EAX \
__asm				xor		eax, eax \
__asm				mov		al, [esi+2]

// 描画元ピクセルＧを取得（ＥＡＸレジスタに取得）
#define		__GET_SRC_G_EAX \
__asm				xor		eax, eax \
__asm				mov		al, [esi+1]

// 描画元ピクセルＢを取得（ＥＡＸレジスタに取得）
#define		__GET_SRC_B_EAX \
__asm				xor		eax, eax \
__asm				mov		al, [esi]

// 描画元ピクセルＡを取得（ＥＡＸレジスタに取得）
#define		__GET_SRC_A_EAX \
__asm				xor		eax, eax \
__asm				mov		al, [esi+3]

// ================================================================

// 描画先ピクセルＲを取得（ＥＢＸレジスタに取得）
#define		__GET_DST_R_EBX \
__asm				xor		ebx, ebx \
__asm				mov		bl, [edi+2]

// 描画先ピクセルＧを取得（ＥＢＸレジスタに取得）
#define		__GET_DST_G_EBX \
__asm				xor		ebx, ebx \
__asm				mov		bl, [edi+1]

// 描画先ピクセルＢを取得（ＥＢＸレジスタに取得）
#define		__GET_DST_B_EBX \
__asm				xor		ebx, ebx \
__asm				mov		bl, [edi]

// 描画先ピクセルＡを取得（ＥＢＸレジスタに取得）
#define		__GET_DST_A_EBX \
__asm				xor		ebx, ebx \
__asm				mov		bl, [edi+3]

// ================================================================

// 描画先ピクセルＲを取得（ＥＡＸレジスタに取得）
#define		__GET_DST_R_EAX \
__asm				xor		eax, eax \
__asm				mov		al, [edi+2]

// 描画先ピクセルＧを取得（ＥＡＸレジスタに取得）
#define		__GET_DST_G_EAX \
__asm				xor		eax, eax \
__asm				mov		al, [edi+1]

// 描画先ピクセルＢを取得（ＥＡＸレジスタに取得）
#define		__GET_DST_B_EAX \
__asm				xor		eax, eax \
__asm				mov		al, [edi]

// 描画先ピクセルＡを取得（ＥＡＸレジスタに取得）
#define		__GET_DST_A_EAX \
__asm				xor		eax, eax \
__asm				mov		al, [edi+3]

// ================================================================

// 描画先ピクセルＲを設定（ＥＡＸレジスタを設定）
#define		__SET_DST_R_EAX \
__asm				mov		[edi+2], al

// 描画先ピクセルＧを設定（ＥＡＸレジスタを設定）
#define		__SET_DST_G_EAX \
__asm				mov		[edi+1], al

// 描画先ピクセルＢを設定（ＥＡＸレジスタを設定）
#define		__SET_DST_B_EAX \
__asm				mov		[edi], al

// 描画先ピクセルＡを設定（ＥＡＸレジスタを設定）
#define		__SET_DST_A_EAX \
__asm				mov		[edi+3], al

// ================================================================

// 描画先ピクセルＲを設定（ＥＢＸレジスタを設定）
#define		__SET_DST_R_EBX \
__asm				mov		[edi+2], bl

// 描画先ピクセルＧを設定（ＥＢＸレジスタを設定）
#define		__SET_DST_G_EBX \
__asm				mov		[edi+1], bl

// 描画先ピクセルＢを設定（ＥＢＸレジスタを設定）
#define		__SET_DST_B_EBX \
__asm				mov		[edi], bl

// 描画先ピクセルＡを設定（ＥＢＸレジスタを設定）
#define		__SET_DST_A_EBX \
__asm				mov		[edi+3], bl

// ================================================================

// アルファ値を２５５に設定
#define		__SET_DST_A_255 \
__asm				mov		[edi+3], 255

// ****************************************************************
// 変換
// ================================================================

// 透明度
#define		__CNV_TR \
__asm				sub		ebx, eax \
__asm				shl		ebx, 2 \
__asm				add		ebx, edx \
__asm				add		eax, [ebx]

// 透明度ＯＷＮ
#define		__CNV_TR_OWN \
__asm				shl		ebx, 2 \
__asm				add		ebx, edx \
__asm				mov		ebx, [ebx]

// ================================================================

// 色味
#define		__CNV_BRIGHT_FUNC(COLOR) \
__asm				mov		eax, __BRIGHT_##COLOR##_FLAG \
__asm				sub		ebx, eax \
__asm				shl		ebx, 2 \
__asm				add		ebx, __BRIGHT_##COLOR##_ADR \
__asm				add		eax, [ebx] \
__asm				mov		ebx, eax

#define		__CNV_BRIGHT_R		__CNV_BRIGHT_FUNC(R)
#define		__CNV_BRIGHT_G		__CNV_BRIGHT_FUNC(G)
#define		__CNV_BRIGHT_B		__CNV_BRIGHT_FUNC(B)

// ================================================================

// 変色
#define		__CNV_COLOR_FUNC(COLOR) \
__asm				mov		eax, __COLOR_##COLOR \
__asm				sub		ebx, eax \
__asm				shl		ebx, 2 \
__asm				add		ebx, __COLOR_ADR \
__asm				add		eax, [ebx] \
__asm				mov		ebx, eax

#define		__CNV_COLOR_R		__CNV_COLOR_FUNC(R)
#define		__CNV_COLOR_G		__CNV_COLOR_FUNC(G)
#define		__CNV_COLOR_B		__CNV_COLOR_FUNC(B)

// ================================================================

// 反転
#define		__CNV_REVERSE \
__asm				mov		eax, ebx \
__asm				not		al \
__asm				sub		ebx, eax \
__asm				shl		ebx, 2 \
__asm				add		ebx, __REVERSE_ADR \
__asm				add		eax, [ebx] \
__asm				mov		ebx, eax

// ================================================================

// モノ準備
#define		__READY_MONO \
__asm				push	edx \
__asm				mov		ebx, __TABLE_MONO_R \
__asm				xor		eax, eax \
__asm				mov		al, [esi+2] \
__asm				shl		eax, 2 \
__asm				add		ebx, eax \
__asm				mov		edx, [ebx] \
__asm				mov		ebx, __TABLE_MONO_G \
__asm				xor		eax, eax \
__asm				mov		al, [esi+1] \
__asm				shl		eax, 2 \
__asm				add		ebx, eax \
__asm				add		edx, [ebx] \
__asm				mov		ebx, __TABLE_MONO_B \
__asm				xor		eax, eax \
__asm				mov		al, [esi] \
__asm				shl		eax, 2 \
__asm				add		ebx, eax \
__asm				add		edx, [ebx] \
__asm				shr		edx, 8 \
__asm				mov		__MONO_FLAG, edx \
__asm				mov		ebx, __TABLE_TR \
__asm				add		ebx, __MONO \
__asm				mov		eax, [ebx] \
__asm				mov		__MONO_ADR, eax \
__asm				pop		edx

// モノ
#define		__CNV_MONO \
__asm				mov		eax, __MONO_FLAG \
__asm				sub		ebx, eax \
__asm				shl		ebx, 2 \
__asm				add		ebx, __MONO_ADR \
__asm				add		eax, [ebx] \
__asm				mov		ebx, eax

// ****************************************************************
// ピクセルの最終設定
// ================================================================

// 通常
#define		__SET_R_STD			__SET_DST_R_EAX
#define		__SET_G_STD			__SET_DST_G_EAX
#define		__SET_B_STD			__SET_DST_B_EAX

// 透明度
#define		__SET_STD_TR_FUNC(COLOR) \
					__GET_DST_##COLOR##_EAX \
					__CNV_TR \
					__SET_DST_##COLOR##_EAX

#define		__SET_R_STD_TR		__SET_STD_TR_FUNC(R)
#define		__SET_G_STD_TR		__SET_STD_TR_FUNC(G)
#define		__SET_B_STD_TR		__SET_STD_TR_FUNC(B)

// ================================================================

// 加算
#define		__SET_ADD_FUNC(COLOR, color) \
					__GET_DST_##COLOR##_EAX \
__asm				add		eax, ebx \
__asm				cmp		eax, 255 \
__asm				jle		__step_set_##color##_add \
__asm				mov		eax, 255 \
__asm			__step_set_##color##_add: \
					__SET_DST_##COLOR##_EAX

#define		__SET_R_ADD			__SET_ADD_FUNC(R, r)
#define		__SET_G_ADD			__SET_ADD_FUNC(G, g)
#define		__SET_B_ADD			__SET_ADD_FUNC(B, b)

// 透明度 加算
#define		__SET_R_ADD_TR \
					__CNV_TR_OWN \
					__SET_R_ADD

#define		__SET_G_ADD_TR \
					__CNV_TR_OWN \
					__SET_G_ADD

#define		__SET_B_ADD_TR \
					__CNV_TR_OWN \
					__SET_B_ADD

// ================================================================

// 減算
#define		__SET_SUB_FUNC(COLOR, color) \
					__GET_DST_##COLOR##_EAX \
__asm				sub		eax, ebx \
__asm				cmp		eax, 0 \
__asm				jge		__step_set_##color##_sub \
__asm				xor		eax, eax \
__asm			__step_set_##color##_sub: \
					__SET_DST_##COLOR##_EAX

#define		__SET_R_SUB			__SET_SUB_FUNC(R, r)
#define		__SET_G_SUB			__SET_SUB_FUNC(G, g)
#define		__SET_B_SUB			__SET_SUB_FUNC(B, b)

// 透明度 減算
#define		__SET_R_SUB_TR \
					__CNV_TR_OWN \
					__SET_R_SUB

#define		__SET_G_SUB_TR \
					__CNV_TR_OWN \
					__SET_G_SUB

#define		__SET_B_SUB_TR \
					__CNV_TR_OWN \
					__SET_B_SUB

// ================================================================

// リバース減算
#define		__SET_RSUB_REVERSE \
__asm				sub		ebx, 255 \
__asm				neg		ebx

#define		__SET_RSUB_FUNC(COLOR, color) \
					__GET_DST_##COLOR##_EAX \
__asm				sub		eax, ebx \
__asm				cmp		eax, 0 \
__asm				jge		__step_set_##color##_sub \
__asm				xor		eax, eax \
__asm			__step_set_##color##_sub: \
					__SET_DST_##COLOR##_EAX

#define		__SET_R_RSUB \
					__SET_RSUB_REVERSE \
					__SET_RSUB_FUNC(R, r)

#define		__SET_G_RSUB \
					__SET_RSUB_REVERSE \
					__SET_RSUB_FUNC(G, g)

#define		__SET_B_RSUB \
					__SET_RSUB_REVERSE \
					__SET_RSUB_FUNC(B, b)

// 透明度 リバース減算
#define		__SET_R_RSUB_TR \
					__SET_RSUB_REVERSE \
					__CNV_TR_OWN \
					__SET_RSUB_FUNC(R, r)

#define		__SET_G_RSUB_TR \
					__SET_RSUB_REVERSE \
					__CNV_TR_OWN \
					__SET_RSUB_FUNC(G, g)

#define		__SET_B_RSUB_TR \
					__SET_RSUB_REVERSE \
					__CNV_TR_OWN \
					__SET_RSUB_FUNC(B, b)

// ================================================================

// フィルター
#define		__SET_FLT_FLT(dst, COLOR, color, last_reg) \
__asm				add		ebx, __FLT_##COLOR \
__asm				js		__step_set_##color##_flt_minus \
__asm				cmp		ebx, 255 \
__asm				jle		__step_set_##color##_flt_plus \
__asm				mov		ebx, 255 \
__asm			__step_set_##color##_flt_plus: \
__asm				shl		ebx, 2 \
__asm				add		ebx, __TABLE_FLT \
__asm				mov		eax, [ebx] \
__asm				xor		ebx, ebx \
__asm				mov		bl, dst \
__asm				sub		ebx, 255 \
__asm				shl		ebx, 2 \
__asm				add		ebx, eax \
__asm				mov		last_reg, [ebx] \
__asm				add		last_reg, 255 \
__asm				jmp		__step_set_##color##_flt_next \
__asm			__step_set_##color##_flt_minus: \
__asm				cmp		ebx, -255 \
__asm				jge		__step_set_##color##_flt_minus_next \
__asm				mov		ebx, -255 \
__asm			__step_set_##color##_flt_minus_next: \
__asm				shl		ebx, 2 \
__asm				add		ebx, __TABLE_FLT \
__asm				mov		eax, [ebx] \
__asm				xor		ebx, ebx \
__asm				mov		bl, dst \
__asm				shl		ebx, 2 \
__asm				add		ebx, eax \
__asm				mov		last_reg, [ebx] \
__asm			__step_set_##color##_flt_next:

#define		__SET_FLT_FUNC(dst, COLOR, color) \
					__SET_FLT_FLT(dst, COLOR, color, eax) \
					__SET_DST_##COLOR##_EAX

#define		__SET_R_FLT			__SET_FLT_FUNC([edi+2], R, r)
#define		__SET_G_FLT			__SET_FLT_FUNC([edi+1], G, g)
#define		__SET_B_FLT			__SET_FLT_FUNC([edi],   B, b)

// 透明度 フィルター

#define		__SET_FLT_TR_FUNC(dst, COLOR, color) \
					__SET_FLT_FLT(dst, COLOR, color, ebx) \
					__GET_DST_##COLOR##_EAX \
					__CNV_TR \
					__SET_DST_##COLOR##_EAX

#define		__SET_R_FLT_TR		__SET_FLT_TR_FUNC([edi+2], R, r)
#define		__SET_G_FLT_TR		__SET_FLT_TR_FUNC([edi+1], G, g)
#define		__SET_B_FLT_TR		__SET_FLT_TR_FUNC([edi],   B, b)

// ================================================================

// 乗算
#define		__SET_MUL_MULL(last_reg) \
__asm				shl		eax, 2 \
__asm				add		eax, __TABLE_TR \
__asm				mov		eax, [eax] \
__asm				shl		ebx, 2 \
__asm				add		ebx, eax \
__asm				mov		last_reg, [ebx]

#define		__SET_MUL_FUNC(COLOR) \
					__GET_DST_##COLOR##_EAX \
					__SET_MUL_MULL(eax) \
					__SET_DST_##COLOR##_EAX

#define		__SET_R_MUL			__SET_MUL_FUNC(R)
#define		__SET_G_MUL			__SET_MUL_FUNC(G)
#define		__SET_B_MUL			__SET_MUL_FUNC(B)

// 透明度 乗算

#define		__SET_MUL_TR_FUNC(COLOR) \
					__GET_DST_##COLOR##_EAX \
					__SET_MUL_MULL(ebx) \
					__GET_DST_##COLOR##_EAX \
					__CNV_TR \
					__SET_DST_##COLOR##_EAX

#define		__SET_R_MUL_TR		__SET_MUL_TR_FUNC(R)
#define		__SET_G_MUL_TR		__SET_MUL_TR_FUNC(G)
#define		__SET_B_MUL_TR		__SET_MUL_TR_FUNC(B)

// ================================================================

// アルファ合成
#define		__SET_R_BLD			__SET_DST_R_EAX
#define		__SET_G_BLD			__SET_DST_G_EAX
#define		__SET_B_BLD			__SET_DST_B_EAX

// （透明度）
#define		__SET_A_BLD \
					__GET_SRC_A_EAX \
					__SET_DST_A_EAX

// 透明度 アルファ合成
#define		__SET_BLD_TR_FUNC(COLOR) \
__asm				mov		__ALPHABLEND_##COLOR, ebx \

#define		__SET_R_BLD_TR		__SET_BLD_TR_FUNC(R)
#define		__SET_G_BLD_TR		__SET_BLD_TR_FUNC(G)
#define		__SET_B_BLD_TR		__SET_BLD_TR_FUNC(B)

// アルファ合成準備
#define		__READY_ALPHABLEND(COLOR) \
__asm				mov		ebx, __ALPHABLEND_##COLOR \
__asm				shl		ebx, 2 \
__asm				add		ebx, eax \
__asm				mov		ebx, [ebx] \
__asm				mov		__ALPHABLEND_##COLOR, ebx

// アルファ合成描画
#define		__DRAW_ALPHABLEND(COLOR) \
					__GET_DST_##COLOR##_EAX \
__asm				mul		ebx \
__asm				add		eax, __ALPHABLEND_##COLOR \
__asm				shr		eax, 8 \
__asm				div		ecx \
					__SET_DST_##COLOR##_EAX \

// アルファ合成なし描画
#define		__DRAW_ALPHABLENDLESS \
__asm				mov		ebx, __ALPHABLEND_R \
					__SET_DST_R_EBX \
__asm				mov		ebx, __ALPHABLEND_G \
					__SET_DST_G_EBX \
__asm				mov		ebx, __ALPHABLEND_B \
					__SET_DST_B_EBX \
					__SET_DST_A_EAX

// アルファ前処理
#define		__BEFOR_ALPHABLEND \
__asm				push	ecx \
__asm				mov		ecx, __ALPHABLEND_A \
__asm				shl		ecx, 2 \
__asm				mov		edx, ecx \
__asm				add		ecx, __TABLE_ALPHABLEND_TR \
__asm				mov		ecx, [ecx] \
					__GET_DST_A_EAX \
__asm				shl		eax, 2 \
__asm				add		ecx, eax \
__asm				mov		ecx, [ecx] \
__asm				and		ecx, ecx \
__asm				jnz		__befor_a_bld_tr_end \
__asm				jmp		__alphablend_skip \
__asm			__befor_a_bld_tr_end: \
__asm				add		edx, __TABLE_ALPHABLEND_WORK1 \
__asm				mov		edx, [edx] \
__asm				add		edx, eax \
__asm				mov		edx, [edx]

// アルファ後処理
#define		__AFTER_ALPHABLEND \
__asm				mov		eax, __ALPHABLEND_A \
__asm				mov		ebx, eax \
__asm				xor		ebx, 0xff \
__asm				jnz		__after_alphablend_next \
					__DRAW_ALPHABLENDLESS \
__asm				jmp		__alphablend_skip \
__asm			__after_alphablend_next: \
					__GET_DST_A_EBX \
__asm				and		ebx, ebx \
__asm				jnz		__after_alphablend_draw \
					__DRAW_ALPHABLENDLESS \
__asm				jmp		__alphablend_skip \
__asm			__after_alphablend_draw: \
__asm				shl		eax, 2 \
__asm				add		eax, __TABLE_ALPHABLEND_WORK2 \
__asm				mov		eax, [eax] \
					__READY_ALPHABLEND(R) \
					__READY_ALPHABLEND(G) \
					__READY_ALPHABLEND(B) \
__asm				mov		ebx, edx \
					__DRAW_ALPHABLEND(R) \
					__DRAW_ALPHABLEND(G) \
					__DRAW_ALPHABLEND(B) \
__asm				mov		[edi+3], cl \
__asm			__alphablend_skip: \
__asm				pop		ecx

