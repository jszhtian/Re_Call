//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
#define		GRPMACMAG_A0_START				\
__asm		mov		esi, TCMAG_src			\
__asm		mov		edi, TCMAG_dst			\
__asm		mov		ecx, TCMAG_y_count		\
__asm	a0_loop_y:							\
											\
__asm		push	ecx						\
__asm		push	esi						\
__asm		push	edi						\
											\
__asm		mov		ecx, TCMAG_x_count		\
__asm	a0_loop_x:

#define		GRPMACMAG_A0_END				\
__asm		add		esi, 4					\
__asm		add		edi, 4					\
__asm		dec		ecx						\
__asm		jz		a0_loop_x_end			\
__asm		jmp		a0_loop_x				\
__asm	a0_loop_x_end:						\
											\
__asm		pop		edi						\
__asm		pop		esi						\
__asm		pop		ecx						\
											\
__asm		dec		ecx						\
__asm		jz		a0_loop_y_end			\
__asm		add		esi, TCMAG_src_add_adr	\
__asm		add		edi, TCMAG_dst_add_adr	\
__asm		jmp		a0_loop_y				\
__asm	a0_loop_y_end:

//-----------------------------------------------------
#define		GRPMACMAG_A1_START				\
__asm		mov		esi, TCMAG_src			\
__asm		mov		edi, TCMAG_dst			\
__asm		mov		ecx, TCMAG_y_count		\
__asm		mov		ebx, TCMAG_y_alpha		\
__asm	a1_loop_y:							\
											\
__asm		push	ecx						\
__asm		push	esi						\
__asm		push	edi						\
__asm		push	ebx						\
											\
__asm		mov		ecx, TCMAG_x_count		\
__asm	a1_loop_x:

#define		GRPMACMAG_A1_END				\
__asm		add		esi, 4					\
__asm		add		edi, 4					\
__asm		dec		ecx						\
__asm		jz		a1_loop_x_end			\
__asm		jmp		a1_loop_x				\
__asm	a1_loop_x_end:						\
											\
__asm		pop		ebx						\
__asm		pop		edi						\
__asm		pop		esi						\
__asm		pop		ecx						\
											\
__asm		add		edi, TCMAG_dst_add_adr	\
__asm		add		ebx, TCMAG_syl			\
__asm		cmp		ebx, TCMAG_dyl			\
__asm		jl		a1_step_y1				\
__asm		sub		ebx, TCMAG_dyl			\
__asm		add		esi, TCMAG_src_add_adr	\
__asm	a1_step_y1:							\
__asm		dec		ecx						\
__asm		jz		a1_loop_y_end			\
__asm		jmp		a1_loop_y				\
__asm	a1_loop_y_end:

//-----------------------------------------------------
#define		GRPMACMAG_A2_START				\
__asm		mov		esi, TCMAG_src			\
__asm		mov		edi, TCMAG_dst			\
__asm		mov		ecx, TCMAG_y_count		\
__asm		mov		ebx, TCMAG_y_alpha		\
__asm	a2_loop_y:							\
											\
__asm		push	ecx						\
__asm		push	esi						\
__asm		push	edi						\
__asm		push	ebx						\
											\
__asm		mov		ecx, TCMAG_x_count		\
__asm	a2_loop_x:

#define		GRPMACMAG_A2_END				\
__asm		add		esi, 4					\
__asm		add		edi, 4					\
__asm		dec		ecx						\
__asm		jz		a2_loop_x_end			\
__asm		jmp		a2_loop_x				\
__asm	a2_loop_x_end:						\
											\
__asm		pop		ebx						\
__asm		pop		edi						\
__asm		pop		esi						\
__asm		pop		ecx						\
											\
__asm	a2_loop_y_free:						\
__asm		add		esi, TCMAG_src_add_adr	\
__asm		add		ebx, TCMAG_dyl			\
__asm		cmp		ebx, TCMAG_syl			\
__asm		jl		a2_step_y1				\
__asm		sub		ebx, TCMAG_syl			\
__asm		add		edi, TCMAG_dst_add_adr	\
__asm		jmp		a2_step_y2				\
__asm	a2_step_y1:							\
__asm		dec		ecx						\
__asm		jz		a2_loop_y_end			\
__asm		jmp		a2_loop_y_free			\
__asm	a2_step_y2:							\
__asm		dec		ecx						\
__asm		jz		a2_loop_y_end			\
__asm		jmp		a2_loop_y				\
__asm	a2_loop_y_end:

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
#define		GRPMACMAG_B0_START				\
__asm		mov		esi, TCMAG_src			\
__asm		mov		edi, TCMAG_dst			\
__asm		mov		ecx, TCMAG_y_count		\
__asm	b0_loop_y:							\
											\
__asm		push	ecx						\
__asm		push	esi						\
__asm		push	edi						\
											\
__asm		mov		edx, TCMAG_x_alpha		\
__asm		mov		ecx, TCMAG_x_count		\
__asm	b0_loop_x:							\
__asm		push	edx

#define		GRPMACMAG_B0_END				\
__asm		pop		edx						\
__asm		add		edi, 4					\
__asm		add		edx, TCMAG_sxl			\
__asm		cmp		edx, TCMAG_dxl			\
__asm		jl		b0_step_x1				\
__asm		sub		edx, TCMAG_dxl			\
__asm		add		esi, 4					\
__asm	b0_step_x1:							\
__asm		dec		ecx						\
__asm		jz		b0_loop_x_end			\
__asm		jmp		b0_loop_x				\
__asm	b0_loop_x_end:						\
											\
__asm		pop		edi						\
__asm		pop		esi						\
__asm		pop		ecx						\
											\
__asm		dec		ecx						\
__asm		jz		b0_loop_y_end			\
__asm		add		esi, TCMAG_src_add_adr	\
__asm		add		edi, TCMAG_dst_add_adr	\
__asm		jmp		b0_loop_y				\
__asm	b0_loop_y_end:

//-----------------------------------------------------
#define		GRPMACMAG_B1_START				\
__asm		mov		esi, TCMAG_src			\
__asm		mov		edi, TCMAG_dst			\
__asm		mov		ecx, TCMAG_y_count		\
__asm		mov		ebx, TCMAG_y_alpha		\
__asm	b1_loop_y:							\
											\
__asm		push	ecx						\
__asm		push	esi						\
__asm		push	edi						\
__asm		push	ebx						\
											\
__asm		mov		edx, TCMAG_x_alpha		\
__asm		mov		ecx, TCMAG_x_count		\
__asm	b1_loop_x:							\
__asm		push	edx

#define		GRPMACMAG_B1_END				\
__asm		pop		edx						\
__asm		add		edi, 4					\
__asm		add		edx, TCMAG_sxl			\
__asm		cmp		edx, TCMAG_dxl			\
__asm		jl		b1_step_x1				\
__asm		sub		edx, TCMAG_dxl			\
__asm		add		esi, 4					\
__asm	b1_step_x1:							\
__asm		dec		ecx						\
__asm		jz		b1_loop_x_end			\
__asm		jmp		b1_loop_x				\
__asm	b1_loop_x_end:						\
											\
__asm		pop		ebx						\
__asm		pop		edi						\
__asm		pop		esi						\
__asm		pop		ecx						\
											\
__asm		add		edi, TCMAG_dst_add_adr	\
__asm		add		ebx, TCMAG_syl			\
__asm		cmp		ebx, TCMAG_dyl			\
__asm		jl		b1_step_y1				\
__asm		sub		ebx, TCMAG_dyl			\
__asm		add		esi, TCMAG_src_add_adr	\
__asm	b1_step_y1:							\
__asm		dec		ecx						\
__asm		jz		b1_loop_y_end			\
__asm		jmp		b1_loop_y				\
__asm	b1_loop_y_end:

//-----------------------------------------------------
#define		GRPMACMAG_B2_START				\
__asm		mov		esi, TCMAG_src			\
__asm		mov		edi, TCMAG_dst			\
__asm		mov		ecx, TCMAG_y_count		\
__asm		mov		ebx, TCMAG_y_alpha		\
__asm	b2_loop_y:							\
											\
__asm		push	ecx						\
__asm		push	esi						\
__asm		push	edi						\
__asm		push	ebx						\
											\
__asm		mov		edx, TCMAG_x_alpha		\
__asm		mov		ecx, TCMAG_x_count		\
__asm	b2_loop_x:							\
__asm		push	edx

#define		GRPMACMAG_B2_END				\
__asm		pop		edx						\
__asm		add		edi, 4					\
__asm		add		edx, TCMAG_sxl			\
__asm		cmp		edx, TCMAG_dxl			\
__asm		jl		b2_step_x1				\
__asm		sub		edx, TCMAG_dxl			\
__asm		add		esi, 4					\
__asm	b2_step_x1:							\
__asm		dec		ecx						\
__asm		jz		b2_loop_x_end			\
__asm		jmp		b2_loop_x				\
__asm	b2_loop_x_end:						\
											\
__asm		pop		ebx						\
__asm		pop		edi						\
__asm		pop		esi						\
__asm		pop		ecx						\
											\
__asm	b2_loop_y_free:						\
__asm		add		esi, TCMAG_src_add_adr	\
__asm		add		ebx, TCMAG_dyl			\
__asm		cmp		ebx, TCMAG_syl			\
__asm		jl		b2_step_y1				\
__asm		sub		ebx, TCMAG_syl			\
__asm		add		edi, TCMAG_dst_add_adr	\
__asm		jmp		b2_step_y2				\
__asm	b2_step_y1:							\
__asm		dec		ecx						\
__asm		jz		b2_loop_y_end			\
__asm		jmp		b2_loop_y_free			\
__asm	b2_step_y2:							\
__asm		dec		ecx						\
__asm		jz		b2_loop_y_end			\
__asm		jmp		b2_loop_y				\
__asm	b2_loop_y_end:

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
#define		GRPMACMAG_C0_START				\
__asm		mov		esi, TCMAG_src			\
__asm		mov		edi, TCMAG_dst			\
__asm		mov		ecx, TCMAG_y_count		\
__asm	c0_loop_y:							\
											\
__asm		push	ecx						\
__asm		push	esi						\
__asm		push	edi						\
											\
__asm		mov		edx, TCMAG_x_alpha		\
__asm		mov		ecx, TCMAG_x_count		\
__asm	c0_loop_x:							\
__asm		push	edx

#define		GRPMACMAG_C0_END				\
__asm		pop		edx						\
__asm	c0_loop_x_free:						\
__asm		add		esi, 4					\
__asm		add		edx, TCMAG_dxl			\
__asm		cmp		edx, TCMAG_sxl			\
__asm		jl		c0_step_x1				\
__asm		sub		edx, TCMAG_sxl			\
__asm		add		edi, 4					\
__asm		jmp		c0_step_x2				\
__asm	c0_step_x1:							\
__asm		dec		ecx						\
__asm		jz		c0_loop_x_end			\
__asm		jmp		c0_loop_x_free			\
__asm	c0_step_x2:							\
__asm		dec		ecx						\
__asm		jz		c0_loop_x_end			\
__asm		jmp		c0_loop_x				\
__asm	c0_loop_x_end:						\
											\
__asm		pop		edi						\
__asm		pop		esi						\
__asm		pop		ecx						\
											\
__asm		dec		ecx						\
__asm		jz		c0_loop_y_end			\
__asm		add		esi, TCMAG_src_add_adr	\
__asm		add		edi, TCMAG_dst_add_adr	\
__asm		jmp		c0_loop_y				\
__asm	c0_loop_y_end:

//-----------------------------------------------------
#define		GRPMACMAG_C1_START				\
__asm		mov		esi, TCMAG_src			\
__asm		mov		edi, TCMAG_dst			\
__asm		mov		ecx, TCMAG_y_count		\
__asm		mov		ebx, TCMAG_y_alpha		\
__asm	c1_loop_y:							\
											\
__asm		push	ecx						\
__asm		push	esi						\
__asm		push	edi						\
__asm		push	ebx						\
											\
__asm		mov		edx, TCMAG_x_alpha		\
__asm		mov		ecx, TCMAG_x_count		\
__asm	c1_loop_x:							\
__asm		push	edx

#define		GRPMACMAG_C1_END				\
__asm		pop		edx						\
__asm	c1_loop_x_free:						\
__asm		add		esi, 4					\
__asm		add		edx, TCMAG_dxl			\
__asm		cmp		edx, TCMAG_sxl			\
__asm		jl		c1_step_x1				\
__asm		sub		edx, TCMAG_sxl			\
__asm		add		edi, 4					\
__asm		jmp		c1_step_x2				\
__asm	c1_step_x1:							\
__asm		dec		ecx						\
__asm		jz		c1_loop_x_end			\
__asm		jmp		c1_loop_x_free			\
__asm	c1_step_x2:							\
__asm		dec		ecx						\
__asm		jz		c1_loop_x_end			\
__asm		jmp		c1_loop_x				\
__asm	c1_loop_x_end:						\
											\
__asm		pop		ebx						\
__asm		pop		edi						\
__asm		pop		esi						\
__asm		pop		ecx						\
											\
__asm		add		edi, TCMAG_dst_add_adr	\
__asm		add		ebx, TCMAG_syl			\
__asm		cmp		ebx, TCMAG_dyl			\
__asm		jl		c1_step_y1				\
__asm		sub		ebx, TCMAG_dyl			\
__asm		add		esi, TCMAG_src_add_adr	\
__asm	c1_step_y1:							\
__asm		dec		ecx						\
__asm		jz		c1_loop_y_end			\
__asm		jmp		c1_loop_y				\
__asm	c1_loop_y_end:

//-----------------------------------------------------
#define		GRPMACMAG_C2_START				\
__asm		mov		esi, TCMAG_src			\
__asm		mov		edi, TCMAG_dst			\
__asm		mov		ecx, TCMAG_y_count		\
__asm		mov		ebx, TCMAG_y_alpha		\
__asm	c2_loop_y:							\
											\
__asm		push	ecx						\
__asm		push	esi						\
__asm		push	edi						\
__asm		push	ebx						\
											\
__asm		mov		edx, TCMAG_x_alpha		\
__asm		mov		ecx, TCMAG_x_count		\
__asm	c2_loop_x:							\
__asm		push	edx

#define		GRPMACMAG_C2_END				\
__asm		pop		edx						\
__asm	c2_loop_x_free:						\
__asm		add		esi, 4					\
__asm		add		edx, TCMAG_dxl			\
__asm		cmp		edx, TCMAG_sxl			\
__asm		jl		c2_step_x1				\
__asm		sub		edx, TCMAG_sxl			\
__asm		add		edi, 4					\
__asm		jmp		c2_step_x2				\
__asm	c2_step_x1:							\
__asm		dec		ecx						\
__asm		jz		c2_loop_x_end			\
__asm		jmp		c2_loop_x_free			\
__asm	c2_step_x2:							\
__asm		dec		ecx						\
__asm		jz		c2_loop_x_end			\
__asm		jmp		c2_loop_x				\
__asm	c2_loop_x_end:						\
											\
__asm		pop		ebx						\
__asm		pop		edi						\
__asm		pop		esi						\
__asm		pop		ecx						\
											\
__asm	c2_loop_y_free:						\
__asm		add		esi, TCMAG_src_add_adr	\
__asm		add		ebx, TCMAG_dyl			\
__asm		cmp		ebx, TCMAG_syl			\
__asm		jl		c2_step_y1				\
__asm		sub		ebx, TCMAG_syl			\
__asm		add		edi, TCMAG_dst_add_adr	\
__asm		jmp		c2_step_y2				\
__asm	c2_step_y1:							\
__asm		dec		ecx						\
__asm		jz		c2_loop_y_end			\
__asm		jmp		c2_loop_y_free			\
__asm	c2_step_y2:							\
__asm		dec		ecx						\
__asm		jz		c2_loop_y_end			\
__asm		jmp		c2_loop_y				\
__asm	c2_loop_y_end:

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
#define		GRPMACMAG_NROMALCLR				\
__asm		mov		edx, CTPCLR

//-----------------------------------------------------
#define		GRPMACMAG_SPRITE				\
__asm		xor		eax, eax				\
__asm		mov		al, [esi+3]				\
__asm		and		al, al					\
__asm		jnz		grpmaglv_step_draw		\
__asm		jmp		grpmaglv_step_end		\
__asm	grpmaglv_step_draw:					\
__asm		mov		ebx, eax				\
__asm		xor		eax, 0xff				\
__asm		jz		grpmaglv_step_normal	\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]				\
__asm		jmp		grpmaglv_step_sprite

//-----------------------------------------------------
#define		GRPMACMAG_SPRITECLR				\
__asm		xor		ebx, ebx				\
__asm		mov		bl, [esi+3]				\
__asm		and		bl, bl					\
__asm		jnz		grpmaglv_step_draw		\
__asm		jmp		grpmaglv_step_end		\
__asm	grpmaglv_step_draw:					\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		ebx, [ebx]				\
__asm		add		ebx, GRPF_clr4			\
__asm		mov		ebx, [ebx]				\
__asm		and		bl, bl					\
__asm		jnz		grpmaglv_step_spriteclr	\
__asm		jmp		grpmaglv_step_end		\
__asm	grpmaglv_step_spriteclr:			\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]
