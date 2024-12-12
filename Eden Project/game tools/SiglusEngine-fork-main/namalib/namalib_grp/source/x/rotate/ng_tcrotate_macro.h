//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
#define		GRPMACRLD_P_START				\
__asm		cld								\
__asm		mov		esi, RLINEF_s_buf		\
__asm		mov		edi, RLINEF_dst			\
__asm		mov		edx, RLINEF_dst_alpha	\
__asm		push	ecx						\
__asm		push	ebx						\
__asm		add		edx, RLINEF_dst_size	\
__asm		jns		crldlv_p_loop2			\
__asm		jmp		crldlv_p_step2			\
__asm	crldlv_p_loop2:						\
__asm		cmp		edi, RLINEF_dst_min		\
__asm		jge		crldlv_p_draw_step		\
__asm		jmp		crldlv_p_draw_end		\
__asm	crldlv_p_draw_step:					\
__asm		cmp		edi, RLINEF_dst_max		\
__asm		jle		crldlv_p_draw_start		\
__asm		pop		ebx						\
__asm		pop		ecx						\
__asm		jmp		crldlv_p_loop_end		\
__asm	crldlv_p_draw_start:				\
__asm		push	edx

#define		GRPMACRLD_P_END					\
__asm		pop		edx						\
__asm	crldlv_p_draw_end:					\
__asm		add		edi, 4					\
__asm		sub		edx, RLINEF_d_buf_size	\
__asm		js		crldlv_p_step2			\
__asm		jmp		crldlv_p_loop2			\
__asm	crldlv_p_step2:						\
__asm		pop		ebx						\
__asm		pop		ecx						\
__asm	crldlv_p_loop_end:

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
#define		GRPMACRLD_H_START				\
__asm		cld								\
__asm		mov		esi, RLINEF_s_buf		\
__asm		mov		edi, RLINEF_dst			\
__asm		mov		ebx, RLINEF_xp			\
__asm		mov		edx, RLINEF_dst_alpha	\
__asm		mov		ecx, RLINEF_xl			\
__asm		inc		ecx						\
__asm	crldlv_h_loop:						\
__asm		push	ecx						\
__asm		push	ebx						\
__asm		add		edx, RLINEF_dst_size	\
__asm		jns		crldlv_h_loop2			\
__asm		jmp		crldlv_h_step2			\
__asm	crldlv_h_loop2:						\
__asm		cmp		edi, RLINEF_dst_min		\
__asm		jge		crldlv_h_draw_step		\
__asm		jmp		crldlv_h_draw_end		\
__asm	crldlv_h_draw_step:					\
__asm		cmp		edi, RLINEF_dst_max		\
__asm		jle		crldlv_h_draw_start		\
__asm		pop		ebx						\
__asm		pop		ecx						\
__asm		jmp		crldlv_h_loop_end		\
__asm	crldlv_h_draw_start:				\
__asm		push	edx

#define		GRPMACRLD_H_END					\
__asm		pop		edx						\
__asm	crldlv_h_draw_end:					\
__asm		add		edi, 4					\
__asm		sub		edx, RLINEF_d_buf_size	\
__asm		js		crldlv_h_step2			\
__asm		jmp		crldlv_h_loop2			\
__asm	crldlv_h_step2:						\
__asm		pop		ebx						\
__asm		pop		ecx						\
__asm		add		esi, ebx				\
__asm		dec		ecx						\
__asm		jz		crldlv_h_loop_end		\
__asm		jmp		crldlv_h_loop			\
__asm	crldlv_h_loop_end:

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
#define		GRPMACRLD_V_START				\
__asm		cld								\
__asm		mov		esi, RLINEF_s_buf		\
__asm		mov		edi, RLINEF_dst			\
__asm		mov		ebx, RLINEF_yp			\
__asm		mov		edx, RLINEF_dst_alpha	\
__asm		mov		ecx, RLINEF_yl			\
__asm		inc		ecx						\
__asm	crldlv_v_loop:						\
__asm		push	ecx						\
__asm		push	ebx						\
__asm		add		edx, RLINEF_dst_size	\
__asm		jns		crldlv_v_loop2			\
__asm		jmp		crldlv_v_step2			\
__asm	crldlv_v_loop2:						\
__asm		cmp		edi, RLINEF_dst_min		\
__asm		jge		crldlv_v_draw_step		\
__asm		jmp		crldlv_v_draw_end		\
__asm	crldlv_v_draw_step:					\
__asm		cmp		edi, RLINEF_dst_max		\
__asm		jle		crldlv_v_draw_start		\
__asm		pop		ebx						\
__asm		pop		ecx						\
__asm		jmp		crldlv_v_loop_end		\
__asm	crldlv_v_draw_start:				\
__asm		push	edx

#define		GRPMACRLD_V_END					\
__asm		pop		edx						\
__asm	crldlv_v_draw_end:					\
__asm		add		edi, 4					\
__asm		sub		edx, RLINEF_d_buf_size	\
__asm		js		crldlv_v_step2			\
__asm		jmp		crldlv_v_loop2			\
__asm	crldlv_v_step2:						\
__asm		pop		ebx						\
__asm		pop		ecx						\
__asm		add		esi, ebx				\
__asm		dec		ecx						\
__asm		jz		crldlv_v_loop_end		\
__asm		jmp		crldlv_v_loop			\
__asm	crldlv_v_loop_end:

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
#define		GRPMACRLD_X_START				\
__asm		cld								\
__asm		mov		esi, RLINEF_s_buf		\
__asm		mov		edi, RLINEF_dst			\
__asm		mov		ebx, ENs				\
__asm		mov		edx, RLINEF_dst_alpha	\
__asm		mov		ecx, ECNT				\
__asm		inc		ecx						\
__asm	crldlv_x_loop:						\
__asm		push	ecx						\
__asm		push	ebx						\
__asm		add		edx, RLINEF_dst_size	\
__asm		jns		crldlv_x_loop2			\
__asm		jmp		crldlv_x_step2			\
__asm	crldlv_x_loop2:						\
__asm		cmp		edi, RLINEF_dst_min		\
__asm		jge		crldlv_x_draw_step		\
__asm		jmp		crldlv_x_draw_end		\
__asm	crldlv_x_draw_step:					\
__asm		cmp		edi, RLINEF_dst_max		\
__asm		jle		crldlv_x_draw_start		\
__asm		pop		ebx						\
__asm		pop		ecx						\
__asm		jmp		crldlv_x_loop_end		\
__asm	crldlv_x_draw_start:				\
__asm		push	edx

#define		GRPMACRLD_X_END					\
__asm		pop		edx						\
__asm	crldlv_x_draw_end:					\
__asm		add		edi, 4					\
__asm		sub		edx, RLINEF_d_buf_size	\
__asm		js		crldlv_x_step2			\
__asm		jmp		crldlv_x_loop2			\
__asm	crldlv_x_step2:						\
__asm		pop		ebx						\
__asm		pop		ecx						\
__asm		add		esi, EALPHA				\
__asm		add		ebx, ENa				\
__asm		js		crldlv_x_step			\
__asm		add		esi, EBETA				\
__asm		sub		ebx, ENh				\
__asm	crldlv_x_step:						\
__asm		dec		ecx						\
__asm		jz		crldlv_x_loop_end		\
__asm		jmp		crldlv_x_loop			\
__asm	crldlv_x_loop_end:

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
#define		GRPMACRLD_NROMALCLR				\
__asm		mov		edx, CTPCLR

//-----------------------------------------------------
#define		GRPMACRLD_SPRITE				\
__asm		xor		eax, eax				\
__asm		mov		al, [esi+3]				\
__asm		and		al, al					\
__asm		jnz		grprldlv_step_draw		\
__asm		jmp		grprldlv_step_end		\
__asm	grprldlv_step_draw:					\
__asm		mov		ebx, eax				\
__asm		xor		eax, 0xff				\
__asm		jz		grprldlv_step_normal	\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]				\
__asm		jmp		grprldlv_step_sprite

//-----------------------------------------------------
#define		GRPMACRLD_SPRITECLR				\
__asm		xor		ebx, ebx				\
__asm		mov		bl, [esi+3]				\
__asm		and		bl, bl					\
__asm		jnz		grprldlv_step_draw		\
__asm		jmp		grprldlv_step_end		\
__asm	grprldlv_step_draw:					\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		ebx, [ebx]				\
__asm		add		ebx, GRPF_clr4			\
__asm		mov		ebx, [ebx]				\
__asm		and		bl, bl					\
__asm		jnz		grprldlv_step_spriteclr	\
__asm		jmp		grprldlv_step_end		\
__asm	grprldlv_step_spriteclr:			\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]
