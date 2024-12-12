//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_LOOP_START			\
__asm		cld							\
__asm		mov		edi, GRPF_d_buf		\
__asm		mov		esi, GRPF_s_buf		\
__asm		mov		ecx, GRPF_cnt_y		\
__asm		mov		edx, CTPCLR			\
__asm	grplv_loop_y:					\
__asm		push	ecx					\
__asm		mov		ecx, GRPF_cnt_x_org	\
__asm	grplv_loop_x:

#define		GRPMAC_LOOP_END				\
__asm		add		esi, 4				\
__asm		add		edi, 4				\
__asm		dec		ecx					\
__asm		jz		grplv_loop_x_end	\
__asm		jmp		grplv_loop_x		\
__asm	grplv_loop_x_end:				\
__asm		add		edi, GRPF_d_xl		\
__asm		add		esi, GRPF_s_xl		\
__asm		pop		ecx					\
__asm		dec		ecx					\
__asm		jz		grplv_loop_y_end	\
__asm		jmp		grplv_loop_y		\
__asm	grplv_loop_y_end:

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_LOOP_START_TILE		\
__asm		cld							\
__asm		mov		edi, GRPF_d_buf		\
__asm		mov		esi, GRPF_s_buf		\
__asm		mov		ecx, GRPF_cnt_y		\
__asm		mov		edx, CTPCLR			\
__asm	grplv_loop_y:					\
__asm		push	ecx					\
__asm		mov		ecx, GRPF_cnt_x_org	\
__asm		mov		eax, GRPF_t_x_org	\
__asm		mov		GRPF_t_x, eax		\
__asm	grplv_loop_x:

#define		GRPMAC_LOOP_END_TILE		\
__asm		add		esi, 4				\
__asm		add		edi, 4				\
__asm		mov		eax, GRPF_t_x		\
__asm		inc		eax					\
__asm		cmp		eax, GRPF_t_xl		\
__asm		jl		grplv_step_tile1	\
__asm		xor		eax, eax			\
__asm	grplv_step_tile1:				\
__asm		mov		GRPF_t_x, eax		\
__asm		dec		ecx					\
__asm		jz		grplv_loop_x_end	\
__asm		jmp		grplv_loop_x		\
__asm	grplv_loop_x_end:				\
__asm		add		edi, GRPF_d_xl		\
__asm		add		esi, GRPF_s_xl		\
__asm		mov		eax, GRPF_t_adr		\
__asm		add		eax, GRPF_t_xl		\
__asm		cmp		eax, GRPF_t_adr_max	\
__asm		jl		grplv_step_tile2	\
__asm		mov		eax, GRPF_t_adr_org	\
__asm	grplv_step_tile2:				\
__asm		mov		GRPF_t_adr, eax		\
__asm		pop		ecx					\
__asm		dec		ecx					\
__asm		jz		grplv_loop_y_end	\
__asm		jmp		grplv_loop_y		\
__asm	grplv_loop_y_end:

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_SPRITE					\
__asm		xor		ebx, ebx				\
__asm		mov		bl, [esi+3]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_a			\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_a:						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]

#define		GRPMAC_SPRITECLR				\
__asm		xor		ebx, ebx				\
__asm		mov		bl, [esi+3]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_clr00		\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_clr00:					\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		ebx, [ebx]				\
__asm		add		ebx, GRPF_clr4			\
__asm		mov		ebx, [ebx]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_clr01		\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_clr01:					\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_SPRITE_TILE				\
__asm		xor		ebx, ebx				\
__asm		mov		bl, [esi+3]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_d			\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_d:

#define		GRPMAC_SPRITECLR_TILE			\
__asm		xor		ebx, ebx				\
__asm		mov		bl, [esi+3]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_clr00		\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_clr00:					\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		ebx, [ebx]				\
__asm		add		ebx, GRPF_clr4			\
__asm		mov		ebx, [ebx]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_clr01		\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_clr01:					\
__asm		shl		ebx, 2					\
__asm		mov		edx, ebx

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_TILE						\
__asm		mov		ebx, GRPF_t_adr			\
__asm		add		ebx, GRPF_t_x			\
__asm		xor		eax, eax				\
__asm		mov		al, [ebx]				\
__asm		cmp		GRPF_t_reverse, 0		\
__asm		je		grplv_tile_normal		\
__asm		not		al						\
__asm	grplv_tile_normal:					\
__asm		push	ecx						\
__asm		mov		ecx, GRPF_t_shift		\
__asm		shl		eax, cl					\
__asm		pop		ecx						\
__asm		add		eax, GRPF_t_add			\
__asm		cmp		eax, 0					\
__asm		jge		grplv_tile_great		\
__asm		mov		eax, 0					\
__asm		jmp		grplv_tile_step			\
__asm	grplv_tile_great:					\
__asm		cmp		eax, 255				\
__asm		jle		grplv_tile_step			\
__asm		mov		eax, 255				\
__asm	grplv_tile_step:					\
__asm		mov		ebx, eax				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_c			\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_c:

//--------------------------------------------------------------
#define		GRPMAC_TILECLR00				\
											\
			GRPMAC_TILE						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		ebx, [ebx]				\
__asm		add		ebx, 1020				\
__asm		mov		ebx, [ebx]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_a			\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_a:						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]

#define		GRPMAC_TILECLR00EX				\
											\
			GRPMAC_TILE						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		ebx, [ebx]				\
__asm		add		ebx, 1020				\
__asm		mov		ebx, [ebx]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_a			\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_a:						\
__asm		cmp		ebx, 255				\
__asm		jne		grplv_step_e			\
__asm		mov		eax, [esi]				\
__asm		mov		[edi], eax				\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_e:						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]

#define		GRPMAC_TILECLR01				\
			GRPMAC_SPRITE_TILE				\
			GRPMAC_TILE						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		ebx, [ebx]				\
__asm		xor		eax, eax				\
__asm		mov		al, [esi+3]				\
__asm		shl		eax, 2					\
__asm		add		ebx, eax				\
__asm		mov		ebx, [ebx]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_a			\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_a:						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]

#define		GRPMAC_TILECLR02				\
											\
			GRPMAC_TILE						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		ebx, [ebx]				\
__asm		add		ebx, GRPF_clr4			\
__asm		mov		ebx, [ebx]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_a			\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_a:						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]

#define		GRPMAC_TILECLR03				\
			GRPMAC_SPRITECLR_TILE			\
			GRPMAC_TILE						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		ebx, [ebx]				\
__asm		add		ebx, edx				\
__asm		mov		ebx, [ebx]				\
__asm		and		bl, bl					\
__asm		jnz		grplv_step_a			\
__asm		jmp		grplv_step_nopixel		\
__asm	grplv_step_a:						\
__asm		shl		ebx, 2					\
__asm		add		ebx, Gclr_table_adr_p	\
__asm		mov		edx, [ebx]

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_MONO_READY				\
__asm		push	edx						\
__asm		mov		ebx, Gmono_table_b_p	\
__asm		xor		eax, eax				\
__asm		mov		al, [esi]				\
__asm		shl		eax, 2					\
__asm		add		ebx, eax				\
__asm		mov		edx, [ebx]				\
__asm		mov		ebx, Gmono_table_g_p	\
__asm		xor		eax, eax				\
__asm		mov		al, [esi+1]				\
__asm		shl		eax, 2					\
__asm		add		ebx, eax				\
__asm		add		edx, [ebx]				\
__asm		mov		ebx, Gmono_table_r_p	\
__asm		xor		eax, eax				\
__asm		mov		al, [esi+2]				\
__asm		shl		eax, 2					\
__asm		add		ebx, eax				\
__asm		add		edx, [ebx]				\
__asm		shr		edx, 8					\
__asm		mov		GRPF_mono_pal, edx		\
__asm		mov		ebx, Gclr_table_adr_p	\
__asm		add		ebx, GRPF_mono			\
__asm		mov		eax, [ebx]				\
__asm		mov		CTPMONO, eax			\
__asm		pop		edx

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_GETSRC_B				\
__asm		xor		ebx, ebx			\
__asm		mov		bl, [esi]

#define		GRPMAC_GETSRC_G				\
__asm		xor		ebx, ebx			\
__asm		mov		bl, [esi+1]

#define		GRPMAC_GETSRC_R				\
__asm		xor		ebx, ebx			\
__asm		mov		bl, [esi+2]

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_GETDST_B				\
__asm		xor		eax, eax			\
__asm		mov		al, [edi]

#define		GRPMAC_GETDST_G				\
__asm		xor		eax, eax			\
__asm		mov		al, [edi+1]

#define		GRPMAC_GETDST_R				\
__asm		xor		eax, eax			\
__asm		mov		al, [edi+2]

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_MONO_B				\
__asm		mov		eax, GRPF_mono_pal	\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, CTPMONO		\
__asm		add		eax, [ebx]			\
__asm		mov		ebx, eax

#define		GRPMAC_MONO_G				\
__asm		mov		eax, GRPF_mono_pal	\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, CTPMONO		\
__asm		add		eax, [ebx]			\
__asm		mov		ebx, eax

#define		GRPMAC_MONO_R				\
__asm		mov		eax, GRPF_mono_pal	\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, CTPMONO		\
__asm		add		eax, [ebx]			\
__asm		mov		ebx, eax

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_REVERSE				\
__asm		mov		eax, ebx			\
__asm		not		al					\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, CTPREV			\
__asm		add		eax, [ebx]			\
__asm		mov		ebx, eax

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_BETA_B				\
__asm		mov		eax, GRPF_c_b		\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, CTPBETA		\
__asm		add		eax, [ebx]			\
__asm		mov		ebx, eax

#define		GRPMAC_BETA_G				\
__asm		mov		eax, GRPF_c_g		\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, CTPBETA		\
__asm		add		eax, [ebx]			\
__asm		mov		ebx, eax

#define		GRPMAC_BETA_R				\
__asm		mov		eax, GRPF_c_r		\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, CTPBETA		\
__asm		add		eax, [ebx]			\
__asm		mov		ebx, eax

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_PAL_B				\
__asm		mov		eax, GRPF_p_b		\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, CTPB			\
__asm		add		eax, [ebx]			\
__asm		mov		ebx, eax

#define		GRPMAC_PAL_G				\
__asm		mov		eax, GRPF_p_g		\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, CTPG			\
__asm		add		eax, [ebx]			\
__asm		mov		ebx, eax

#define		GRPMAC_PAL_R				\
__asm		mov		eax, GRPF_p_r		\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, CTPR			\
__asm		add		eax, [ebx]			\
__asm		mov		ebx, eax

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_CLR					\
__asm		shl		ebx, 2				\
__asm		add		ebx, edx			\
__asm		mov		ebx, [ebx]

//--------------------------------------------------------------
#define		GRPMAC_CLRADD				\
__asm		sub		ebx, eax			\
__asm		shl		ebx, 2				\
__asm		add		ebx, edx			\
__asm		add		eax, [ebx]

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_SET_B				\
__asm		mov		[edi], bl

#define		GRPMAC_SET_G				\
__asm		mov		[edi+1], bl

#define		GRPMAC_SET_R				\
__asm		mov		[edi+2], bl

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_CLRSET_B				\
			GRPMAC_GETDST_B				\
			GRPMAC_CLRADD				\
__asm		mov		[edi], al

#define		GRPMAC_CLRSET_G				\
			GRPMAC_GETDST_G				\
			GRPMAC_CLRADD				\
__asm		mov		[edi+1], al

#define		GRPMAC_CLRSET_R				\
			GRPMAC_GETDST_R				\
			GRPMAC_CLRADD				\
__asm		mov		[edi+2], al

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_GETCLRSET_B			\
__asm		xor		ebx, ebx			\
__asm		xor		eax, eax			\
__asm		mov		bl, [esi]			\
__asm		mov		al, [edi]			\
			GRPMAC_CLRADD				\
__asm		mov		[edi], al

#define		GRPMAC_GETCLRSET_G			\
__asm		xor		ebx, ebx			\
__asm		xor		eax, eax			\
__asm		mov		bl, [esi+1]			\
__asm		mov		al, [edi+1]			\
			GRPMAC_CLRADD				\
__asm		mov		[edi+1], al

#define		GRPMAC_GETCLRSET_R			\
__asm		xor		ebx, ebx			\
__asm		xor		eax, eax			\
__asm		mov		bl, [esi+2]			\
__asm		mov		al, [edi+2]			\
			GRPMAC_CLRADD				\
__asm		mov		[edi+2], al

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_ADDSET_B				\
			GRPMAC_GETDST_B				\
__asm		add		eax, ebx			\
__asm		cmp		eax, 255			\
__asm		jle		grplv_step_add0		\
__asm		mov		eax, 255			\
__asm	grplv_step_add0:				\
__asm		mov		[edi], al

#define		GRPMAC_ADDSET_G				\
			GRPMAC_GETDST_G				\
__asm		add		eax, ebx			\
__asm		cmp		eax, 255			\
__asm		jle		grplv_step_add1		\
__asm		mov		eax, 255			\
__asm	grplv_step_add1:				\
__asm		mov		[edi+1], al

#define		GRPMAC_ADDSET_R				\
			GRPMAC_GETDST_R				\
__asm		add		eax, ebx			\
__asm		cmp		eax, 255			\
__asm		jle		grplv_step_add2		\
__asm		mov		eax, 255			\
__asm	grplv_step_add2:				\
__asm		mov		[edi+2], al

//--------------------------------------------------------------
#define		GRPMAC_ADDCLRSET_B			\
			GRPMAC_CLR					\
			GRPMAC_GETDST_B				\
__asm		add		eax, ebx			\
__asm		cmp		eax, 255			\
__asm		jle		grplv_step_addclr0	\
__asm		mov		eax, 255			\
__asm	grplv_step_addclr0:				\
__asm		mov		[edi], al

#define		GRPMAC_ADDCLRSET_G			\
			GRPMAC_CLR					\
			GRPMAC_GETDST_G				\
__asm		add		eax, ebx			\
__asm		cmp		eax, 255			\
__asm		jle		grplv_step_addclr1	\
__asm		mov		eax, 255			\
__asm	grplv_step_addclr1:				\
__asm		mov		[edi+1], al

#define		GRPMAC_ADDCLRSET_R			\
			GRPMAC_CLR					\
			GRPMAC_GETDST_R				\
__asm		add		eax, ebx			\
__asm		cmp		eax, 255			\
__asm		jle		grplv_step_addclr2	\
__asm		mov		eax, 255			\
__asm	grplv_step_addclr2:				\
__asm		mov		[edi+2], al

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_SUBSET_B				\
			GRPMAC_GETDST_B				\
__asm		sub		eax, ebx			\
__asm		cmp		eax, 0				\
__asm		jge		grplv_step_sub0		\
__asm		xor		eax, eax			\
__asm	grplv_step_sub0:				\
__asm		mov		[edi], al

#define		GRPMAC_SUBSET_G				\
			GRPMAC_GETDST_G				\
__asm		sub		eax, ebx			\
__asm		cmp		eax, 0				\
__asm		jge		grplv_step_sub1		\
__asm		xor		eax, eax			\
__asm	grplv_step_sub1:				\
__asm		mov		[edi+1], al

#define		GRPMAC_SUBSET_R				\
			GRPMAC_GETDST_R				\
__asm		sub		eax, ebx			\
__asm		cmp		eax, 0				\
__asm		jge		grplv_step_sub2		\
__asm		xor		eax, eax			\
__asm	grplv_step_sub2:				\
__asm		mov		[edi+2], al

//--------------------------------------------------------------
#define		GRPMAC_SUBCLRSET_B			\
			GRPMAC_CLR					\
			GRPMAC_GETDST_B				\
__asm		sub		eax, ebx			\
__asm		cmp		eax, 0				\
__asm		jge		grplv_step_subclr0	\
__asm		xor		eax, eax			\
__asm	grplv_step_subclr0:				\
__asm		mov		[edi], al

#define		GRPMAC_SUBCLRSET_G			\
			GRPMAC_CLR					\
			GRPMAC_GETDST_G				\
__asm		sub		eax, ebx			\
__asm		cmp		eax, 0				\
__asm		jge		grplv_step_subclr1	\
__asm		xor		eax, eax			\
__asm	grplv_step_subclr1:				\
__asm		mov		[edi+1], al

#define		GRPMAC_SUBCLRSET_R			\
			GRPMAC_CLR					\
			GRPMAC_GETDST_R				\
__asm		sub		eax, ebx			\
__asm		cmp		eax, 0				\
__asm		jge		grplv_step_subclr2	\
__asm		xor		eax, eax			\
__asm	grplv_step_subclr2:				\
__asm		mov		[edi+2], al

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#define		GRPMAC_RGBFILTERSET_B						\
__asm		add		ebx, RGBFILTER_p_b					\
__asm		js		grplv_rgbfilter_b_mainasu			\
__asm		mov		GRPF_p_b, 255						\
__asm		jmp		grplv_rgbfilter_b_next				\
__asm	grplv_rgbfilter_b_mainasu:						\
__asm		mov		GRPF_p_b, 0							\
__asm	grplv_rgbfilter_b_next:							\
__asm		shl		ebx, 2								\
__asm		add		ebx, Grgbfilter_table_adr_neutral	\
__asm		mov		eax, [ebx]							\
__asm		mov		CTPB, eax							\
__asm		xor		ebx, ebx							\
__asm		mov		bl, [edi]							\
__asm		mov		eax, GRPF_p_b						\
__asm		sub		ebx, eax							\
__asm		shl		ebx, 2								\
__asm		add		ebx, CTPB							\
__asm		add		eax, [ebx]							\
__asm		mov		[edi], al

#define		GRPMAC_RGBFILTERSET_G						\
__asm		add		ebx, RGBFILTER_p_g					\
__asm		js		grplv_rgbfilter_g_mainasu			\
__asm		mov		GRPF_p_g, 255						\
__asm		jmp		grplv_rgbfilter_g_next				\
__asm	grplv_rgbfilter_g_mainasu:						\
__asm		mov		GRPF_p_g, 0							\
__asm	grplv_rgbfilter_g_next:							\
__asm		shl		ebx, 2								\
__asm		add		ebx, Grgbfilter_table_adr_neutral	\
__asm		mov		eax, [ebx]							\
__asm		mov		CTPG, eax							\
__asm		xor		ebx, ebx							\
__asm		mov		bl, [edi+1]							\
__asm		mov		eax, GRPF_p_g						\
__asm		sub		ebx, eax							\
__asm		shl		ebx, 2								\
__asm		add		ebx, CTPG							\
__asm		add		eax, [ebx]							\
__asm		mov		[edi+1], al

#define		GRPMAC_RGBFILTERSET_R						\
__asm		add		ebx, RGBFILTER_p_r					\
__asm		js		grplv_rgbfilter_r_mainasu			\
__asm		mov		GRPF_p_r, 255						\
__asm		jmp		grplv_rgbfilter_r_next				\
__asm	grplv_rgbfilter_r_mainasu:						\
__asm		mov		GRPF_p_r, 0							\
__asm	grplv_rgbfilter_r_next:							\
__asm		shl		ebx, 2								\
__asm		add		ebx, Grgbfilter_table_adr_neutral	\
__asm		mov		eax, [ebx]							\
__asm		mov		CTPR, eax							\
__asm		xor		ebx, ebx							\
__asm		mov		bl, [edi+2]							\
__asm		mov		eax, GRPF_p_r						\
__asm		sub		ebx, eax							\
__asm		shl		ebx, 2								\
__asm		add		ebx, CTPR							\
__asm		add		eax, [ebx]							\
__asm		mov		[edi+2], al

//--------------------------------------------------------------
#define		GRPMAC_RGBFILTERCLRSET_B					\
__asm		add		ebx, RGBFILTER_p_b					\
__asm		js		grplv_rgbfilterclr_b_mainasu		\
__asm		mov		GRPF_p_b, 255						\
__asm		jmp		grplv_rgbfilterclr_b_next			\
__asm	grplv_rgbfilterclr_b_mainasu:					\
__asm		mov		GRPF_p_b, 0							\
__asm	grplv_rgbfilterclr_b_next:						\
__asm		shl		ebx, 2								\
__asm		add		ebx, Grgbfilter_table_adr_neutral	\
__asm		mov		eax, [ebx]							\
__asm		mov		CTPB, eax							\
__asm		xor		ebx, ebx							\
__asm		mov		bl, [edi]							\
__asm		mov		eax, GRPF_p_b						\
__asm		sub		ebx, eax							\
__asm		shl		ebx, 2								\
__asm		add		ebx, CTPB							\
__asm		add		eax, [ebx]							\
__asm		mov		ebx, eax							\
			GRPMAC_GETDST_B								\
			GRPMAC_CLRADD								\
__asm		mov		[edi], al

#define		GRPMAC_RGBFILTERCLRSET_G					\
__asm		add		ebx, RGBFILTER_p_g					\
__asm		js		grplv_rgbfilterclr_g_mainasu		\
__asm		mov		GRPF_p_g, 255						\
__asm		jmp		grplv_rgbfilterclr_g_next			\
__asm	grplv_rgbfilterclr_g_mainasu:					\
__asm		mov		GRPF_p_g, 0							\
__asm	grplv_rgbfilterclr_g_next:						\
__asm		shl		ebx, 2								\
__asm		add		ebx, Grgbfilter_table_adr_neutral	\
__asm		mov		eax, [ebx]							\
__asm		mov		CTPG, eax							\
__asm		xor		ebx, ebx							\
__asm		mov		bl, [edi+1]							\
__asm		mov		eax, GRPF_p_g						\
__asm		sub		ebx, eax							\
__asm		shl		ebx, 2								\
__asm		add		ebx, CTPG							\
__asm		add		eax, [ebx]							\
__asm		mov		ebx, eax							\
			GRPMAC_GETDST_G								\
			GRPMAC_CLRADD								\
__asm		mov		[edi+1], al

#define		GRPMAC_RGBFILTERCLRSET_R					\
__asm		add		ebx, RGBFILTER_p_r					\
__asm		js		grplv_rgbfilterclr_r_mainasu		\
__asm		mov		GRPF_p_r, 255						\
__asm		jmp		grplv_rgbfilterclr_r_next			\
__asm	grplv_rgbfilterclr_r_mainasu:					\
__asm		mov		GRPF_p_r, 0							\
__asm	grplv_rgbfilterclr_r_next:						\
__asm		shl		ebx, 2								\
__asm		add		ebx, Grgbfilter_table_adr_neutral	\
__asm		mov		eax, [ebx]							\
__asm		mov		CTPR, eax							\
__asm		xor		ebx, ebx							\
__asm		mov		bl, [edi+2]							\
__asm		mov		eax, GRPF_p_r						\
__asm		sub		ebx, eax							\
__asm		shl		ebx, 2								\
__asm		add		ebx, CTPR							\
__asm		add		eax, [ebx]							\
__asm		mov		ebx, eax							\
			GRPMAC_GETDST_R								\
			GRPMAC_CLRADD								\
__asm		mov		[edi+2], al

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
