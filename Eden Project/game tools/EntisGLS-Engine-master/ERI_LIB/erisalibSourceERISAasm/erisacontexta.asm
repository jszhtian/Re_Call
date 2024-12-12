
; ****************************************************************************
;                        E R I N S - L i b r a r y
; ----------------------------------------------------------------------------
;         Copyright (C) 2000-2003 Leshade Entis. All rights reserved.
; ****************************************************************************


	.686
	.XMM
	.MODEL	FLAT

	INCLUDE	experi.inc

	MMWORD	TEXTEQU	<QWORD>


; ----------------------------------------------------------------------------
;                            �f�[�^�Z�O�����g
; ----------------------------------------------------------------------------

DataSeg	SEGMENT	PARA FLAT 'DATA'

DataSeg	ENDS

ConstSeg	SEGMENT	PARA READONLY FLAT 'CONST'

ALIGN	10H
mmxInitErisaProb	LABEL	MMWORD
	WORD	1, 0, 1, 1
mmxDeltaErisaProb	LABEL	MMWORD
	WORD	0, 2, 0, 2


ConstSeg	ENDS



; ----------------------------------------------------------------------------
;                             �R�[�h�Z�O�����g
; ----------------------------------------------------------------------------

CodeSeg	SEGMENT	PARA READONLY FLAT 'CODE'

;
;	�Í�����
; ----------------------------------------------------------------------------
ALIGN	10H
ERIBshfBuffer@@DecodeBuffer	PROC	NEAR32 SYSCALL USES ebx esi edi

	mov	ebx, ecx
	ASSUME	ebx:PTR ERIBshfBuffer
	;
	; �����̏���
	;
	mov	eax, [ebx].m_dwPassOffset
	mov	esi, eax			; esi = iPos
	inc	eax
	xor	edx, edx
	.IF	eax >= [ebx].m_strPassword.m_nLength
		xor	eax, eax
	.ENDIF
	mov	[ebx].m_dwPassOffset, eax
	;
	xor	ecx, ecx
	.REPEAT
		mov	DWORD PTR [ebx].m_bufBSHF[ecx], edx
		mov	DWORD PTR [ebx].m_maskBSHF[ecx], edx
		add	ecx, 4
	.UNTIL	ecx >= 32
	;
	; �Í��𕜍�
	;
	xor	edi, edi			; edi = iBit
	mov	edx, [ebx].m_strPassword.m_pszString
	push	ebp
	mov	ebp, 256
	.REPEAT
		movzx	eax, BYTE PTR [edx + esi]
		inc	esi
		add	edi, eax
		cmp	esi, [ebx].m_strPassword.m_nLength
		sbb	edx, edx
		and	edi, 0FFH
			mov	eax, edi
			mov	ecx, edi
			shr	eax, 3
		and	esi, edx
			mov	dl, 80H
			and	ecx, 07H
			shr	dl, cl
		;
		.WHILE	[ebx].m_maskBSHF[eax] == 0FFH
			inc	eax
			add	edi, 8
			and	eax, 1FH
			and	edi, 0FFH
		.ENDW
		;
		.WHILE	[ebx].m_maskBSHF[eax] & dl
			inc	edi
			xor	ecx, ecx
			ror	dl, 1
			setc	cl
			lea	eax, [eax + ecx * 2]
			lea	edi, [edi + ecx * 8]
			and	eax, 1FH
			and	edi, 0FFH
		.ENDW
		;
		mov	ecx, 256
		mov	dh, dl
		sub	ecx, ebp
		push	esi
		mov	esi, ecx
		and	ecx, 07H
		shr	esi, 3
		xor	ecx, 07H
		movzx	esi, [ebx].m_srcBSHF[esi]
		inc	ecx
		or	dl, [ebx].m_maskBSHF[eax]
		shr	esi, cl
		mov	[ebx].m_maskBSHF[eax], dl
		sbb	esi, esi
		movzx	edx, dh
		and	edx, esi
		pop	esi
		or	[ebx].m_bufBSHF[eax], dl
		;
		mov	edx, [ebx].m_strPassword.m_pszString
		dec	ebp
	.UNTIL	ZERO?
	pop	ebp
	;
	ASSUME	ebx:NOTHING
	ret	0

ERIBshfBuffer@@DecodeBuffer	ENDP


IFNDEF	COMPACT_NOA_DECODER

;
;	�n�t�}���؂�1�̗t�̏o���p�x�𑝉�������
; ----------------------------------------------------------------------------
Align	10H
ERINA_HUFFMAN_TREE@@IncreaseOccuredCount	\
					PROC	NEAR32 SYSCALL USES ebx esi edi,
	iEntry:SDWORD

	LOCAL	iSwap:SDWORD

	mov	ebx, ecx
	ASSUME	ebx:PERINA_HUFFMAN_TREE
	mov	eax, iEntry

	@TREE@@IncreaseOccuredCount	0

	ASSUME	ebx:NOTHING

	ret	4 * 1

ERINA_HUFFMAN_TREE@@IncreaseOccuredCount	ENDP

;
;	�w��m�[�h�̏o���p�x���炠��ׂ��n�t�}���؂̏�Ԃɐ��K������
; ----------------------------------------------------------------------------
Align	10H
ERINA_HUFFMAN_TREE@@Normalize		PROC	NEAR32 SYSCALL USES ebx esi edi,
	iEntry:SDWORD

	LOCAL	iSwap:SDWORD

	mov	ebx, ecx
	ASSUME	ebx:PERINA_HUFFMAN_TREE

	mov	eax, iEntry

	@TREE@@Normalize	0

	ASSUME	ebx:NOTHING

	ret	4 * 1

ERINA_HUFFMAN_TREE@@Normalize		ENDP

ENDIF


;
;	@PrefetchBuffer �}�N��
; ----------------------------------------------------------------------------
; @PrefetchBuffer <esc_label>, <reg_mode>
; ----------------------------------------------------------------------------
; �����G
;	EBX register	: ERISADecodeContext �I�u�W�F�N�g�ւ̃|�C���^ 
;	esc_label	: �E�o���x��
;	reg_mode	: �w�肳�ꂽ���ɂ́Aecx �� [ebx].m_nIntBufCount ���A
;			: edx �� [ebx].m_dwIntBuffer ���ǂݍ��܂�Ă�����̂�
;			: ���ď��������B�����̓��e�͕ۏ؂����B
; ���W�X�^�G
;	���ׂẴ��W�X�^�͕ۑ������
; ----------------------------------------------------------------------------

@PrefetchBuffer	MACRO	esc_label:REQ, reg_mode:=<0>, inst_align:=<0>, p3_code:=<0>
	LOCAL	Label1

	IF	reg_mode EQ 1
	.IF	ecx == 0
	ELSEIF	reg_mode EQ 2
	.IF	ZERO?
	ELSE
	.IF	[ebx].m_nIntBufCount == 0
	ENDIF
		;;
		push	eax
		IF	reg_mode EQ 0
			push	ecx
			push	edx
		ENDIF
		;;
		.IF	[ebx].m_nBufCount == 0
			;;
			;; �o�b�t�@�ɓǂݍ���
			mov	ecx, ebx
			mov	eax, [ebx].m_ptrBuffer
			mov	edx, [ebx].pvfnTable
			mov	[ebx].m_ptrNextBuf, eax
			push	[ebx].m_nBufferingSize
			push	eax
			call	NEAR32 PTR [edx + VFI_ReadNextData]
			mov	[ebx].m_nBufCount, eax
			test	eax, eax
			.IF	ZERO?
				IF	reg_mode EQ 0
					pop	edx
					pop	ecx
				ENDIF
				pop	eax
				jmp	esc_label
			.ENDIF
			;;
			;; �S�̏�]�o�C�g��0�𖄂ߍ���
			dec	eax
			mov	edx, [ebx].m_ptrBuffer
			xor	eax, 0011B
			add	edx, [ebx].m_nBufCount
			and	eax, 0011B
			.IF	!ZERO?
				add	[ebx].m_nBufCount, eax
				.REPEAT
					mov	BYTE PTR [edx], 00H
					inc	edx
					dec	eax
				.UNTIL	ZERO?
			.ENDIF
		.ENDIF
		;;
		;; ���ԃo�b�t�@�ɓǂݍ���
		mov	eax, [ebx].m_ptrNextBuf
		mov	ecx, [ebx].m_nBufCount
		IF	p3_code
			prefetchnta	[eax]
		ENDIF
		IF	reg_mode EQ 0
			mov	[ebx].m_nIntBufCount, 32
		ENDIF
		sub	ecx, 4
		mov	edx, DWORD PTR [eax]
		add	eax, (SizeOf DWORD)
		bswap	edx
		mov	[ebx].m_nBufCount, ecx
		mov	[ebx].m_ptrNextBuf, eax
		;;
		IF	reg_mode EQ 0
			mov	[ebx].m_dwIntBuffer, edx
			pop	edx
			pop	ecx
		ELSE
			mov	ecx, 32
		ENDIF
		pop	eax
	IF	inst_align
		jmp	Label1
		ALIGN	inst_align
Label1:
	ENDIF
	.ENDIF

		ENDM


;
;	@GetABit �}�N��
; ----------------------------------------------------------------------------
; @GetABit <esc_label>
; ----------------------------------------------------------------------------
; �����G
;	EBX register	: ERISADecodeContext �I�u�W�F�N�g�ւ̃|�C���^ 
;	esc_label	: �E�o���x��
; ���W�X�^;
;	EBX�AESI�AEDI�AEBP�AESP ���W�X�^�͕ۑ������
;	EAX ���W�X�^�ɁA0�܂��́|1���Ԃ���܂��B
; ----------------------------------------------------------------------------

@GetABit	MACRO	esc_label:REQ, p3_code:=<0>

	mov	ecx, [ebx].m_nIntBufCount
	mov	edx, [ebx].m_dwIntBuffer
	;;
	@PrefetchBuffer	esc_label, 1, 0, p3_code
	;;
	dec	ecx
	add	edx, edx
	sbb	eax, eax
	mov	[ebx].m_nIntBufCount, ecx
	mov	[ebx].m_dwIntBuffer, edx

		ENDM


;
;	@GetNBits �}�N��
; ----------------------------------------------------------------------------
; @GetNBits <esc_label>
; ----------------------------------------------------------------------------
; �����G
;	EBX register	: ERISADecodeContext �\���̂ւ̃|�C���^
;	ESI register	: �ǂݍ��ރr�b�g��
;	esc_label	: �E�o���x��
; ���W�X�^�G
;	EBX, EBP, ESP ���W�X�^�͕ۑ������B
;	EAX ���W�X�^�ɓǂݍ��܂ꂽ�r�b�g�񂪕Ԃ����B
; ----------------------------------------------------------------------------

@GetNBits	MACRO	esc_label:REQ

	xor	eax, eax
	mov	ecx, [ebx].m_nIntBufCount
	mov	edx, [ebx].m_dwIntBuffer
	.WHILE	esi != 0
		;;
		@PrefetchBuffer	esc_label, 1
		;;
		mov	edi, ecx
		mov	ecx, esi
		.IF	ecx > edi
			mov	ecx, edi
		.ENDIF
		sub	edi, ecx
		.IF	ecx == 32
			mov	eax, edx
			xor	edx, edx
		.ELSE
			shld	eax, edx, cl
			shl	edx, cl
		.ENDIF
		sub	esi, ecx
		mov	ecx, edi
	.ENDW
	mov	[ebx].m_nIntBufCount, ecx
	mov	[ebx].m_dwIntBuffer, edx

	ENDM


;
;	@GetGammaCode �}�N��
; ----------------------------------------------------------------------------
; @GetGammaCode <esc_label>
; ----------------------------------------------------------------------------
; �����G
;	EBX register	: ERISADecodeContext �I�u�W�F�N�g�ւ̃|�C���^ 
;	esc_label	: �E�o���x��
; ���W�X�^;
;	EBX�AEDI�AEBP�AESP ���W�X�^�͕ۑ������
;	EAX ���W�X�^�ɕ������ꂽ�K���}�R�[�h���Ԃ���܂��B
; ----------------------------------------------------------------------------

@GetGammaCode	MACRO	esc_label:REQ, p3_code:=<0>
	LOCAL	Loop_Begin, Label_Exit

	;;
	;; ����
	;;
	xor	eax, eax
	mov	esi, 1
	mov	ecx, [ebx].m_nIntBufCount
	mov	edx, [ebx].m_dwIntBuffer
	;;
	;; 1 �̔���
	;;
	@PrefetchBuffer	esc_label, 1, 0, p3_code
	dec	ecx
	add	edx, edx
	jnc	Label_Exit
	;;
	;; �ėp���[�`��
	;;
	xor	eax, eax
	mov	esi, 2
	;;
Loop_Begin:
	;;
	;; 2 �r�b�g�ꊇ����
	;;
	.WHILE	ecx >= 2
		add	edx, edx
		adc	eax, eax
		sub	ecx, 2
		add	edx, edx
		jnc	Label_Exit
		shl	esi, 1
	.ENDW
	;;
	;; 1 �r�b�g���o��
	;;
	@PrefetchBuffer	esc_label, 1, 0, p3_code
	dec	ecx
	add	edx, edx
	adc	eax, eax
	;;
	;; �����I������
	;;
	@PrefetchBuffer	esc_label, 1, 0, p3_code
	dec	ecx
	add	edx, edx
	jnc	Label_Exit
	shl	esi, 1
	jmp	Loop_Begin
Label_Exit:
	;;
	;; �R�[�h����
	mov	[ebx].m_nIntBufCount, ecx
	mov	[ebx].m_dwIntBuffer, edx
	add	eax, esi

		ENDM


;
;	�o�b�t�@����̂Ƃ��A���̃f�[�^��ǂݍ���
; ----------------------------------------------------------------------------
Align	10H
ERISADecodeContext@@PrefetchBuffer	PROC	NEAR32 SYSCALL USES ebx

	LOCAL	dwInitESP:DWORD

	mov	dwInitESP, esp

	mov	ebx, ecx
	ASSUME	ebx:PERISADecodeContext
	@PrefetchBuffer	esc_label
	ASSUME	ebx:NOTHING
	xor	eax, eax
	ret	0

esc_label:
	mov	esp, dwInitESP
	mov	eax, 1
	ret	0

ERISADecodeContext@@PrefetchBuffer	ENDP

;
;	1�r�b�g�擾����
; ----------------------------------------------------------------------------
Align	10H
ERISADecodeContext@@GetABit		PROC	NEAR32 SYSCALL USES ebx

	LOCAL	dwInitESP:DWORD

	mov	dwInitESP, esp

	mov	ebx, ecx
	ASSUME	ebx:PERISADecodeContext
	@GetABit	esc_label
	ASSUME	ebx:NOTHING
	ret	0

esc_label:
	mov	esp, dwInitESP
	mov	eax, 1
	ret	0

ERISADecodeContext@@GetABit		ENDP

;
;	n �r�b�g�擾����
; ----------------------------------------------------------------------------
Align	10H
ERISADecodeContext@@GetNBits		PROC	NEAR32 SYSCALL USES ebx esi edi,
	n:SDWORD

	LOCAL	dwInitESP:DWORD

	mov	dwInitESP, esp

	mov	esi, n
	mov	ebx, ecx
	ASSUME	ebx:PERISADecodeContext

	@GetNBits	esc_label

	ASSUME	ebx:NOTHING

	ret	4

esc_label:
	mov	esp, dwInitESP
	xor	eax, eax
	ret	4

ERISADecodeContext@@GetNBits		ENDP


IFNDEF	COMPACT_NOA_DECODER

;
;	�K���}�R�[�h���擾����
; ----------------------------------------------------------------------------
Align	10H
ERISADecodeContext@@GetGammaCode	PROC	NEAR32 SYSCALL USES ebx esi

	LOCAL	dwInitESP:DWORD

	mov	dwInitESP, esp

	mov	ebx, ecx
	ASSUME	ebx:PERISADecodeContext
	@GetGammaCode	esc_label
	ASSUME	ebx:NOTHING
	ret	0

esc_label:
	mov	esp, dwInitESP
	mov	eax, -1
	ret	0

ERISADecodeContext@@GetGammaCode		ENDP

;
;	�W�J�����f�[�^���擾����i�K���}�����j
; ----------------------------------------------------------------------------
Align	10H
ERISADecodeContext@@DecodeGammaCodeBytes	\
				PROC	NEAR32 SYSCALL USES ebx esi edi,
	ptrDst:PSBYTE, nCount:DWORD

	LOCAL	nDecoded:DWORD
	LOCAL	dwInitESP:DWORD
	LOCAL	bufDecode[40H]:SBYTE
	LOCAL	ptrDecBuf:PTR SBYTE
	LOCAL	nDecBufLeft:DWORD

	mov	nDecoded, 0
	mov	edi, ptrDst
	mov	dwInitESP, esp
	;;
	mov	ebx, ecx
	ASSUME	ebx:PERISADecodeContext

	.IF	[ebx].m_nLength == 0
		@GetGammaCode	esc_label
		mov	[ebx].m_nLength, eax
	.ENDIF

	.IF	(ERI_EnabledProcessorType & ERI_USE_XMM_P3) && \
				!(edi & 0FH) && !(nCount & 1FH)
	;
	;	SSE ��p�R�[�h
	; --------------------------------------------------------------------
	lea	eax, bufDecode[0]
	add	eax, 0FH
	and	eax, NOT 0FH
	mov	ptrDecBuf, eax
	prefetchnta	[eax]
	mov	edi, eax
	mov	nDecBufLeft, 20H
	;
	.WHILE	1
		;;
		;; �o�̓V���{�������Z�o
		mov	ecx, [ebx].m_nLength
		mov	eax, nDecBufLeft
		mov	edx, ecx
		cmp	ecx, eax
		cmova	ecx, eax
		sub	edx, ecx
		sub	eax, ecx
		mov	[ebx].m_nLength, edx
		;;
		;; �V���{�����o��
		mov	edx, [ebx].m_flgZero
		mov	nDecBufLeft, eax
		test	edx, edx
		.IF	ZERO?
			add	nDecoded, ecx
			xor	eax, eax
			.REPEAT
				mov	SBYTE PTR [edi], al
				inc	edi
				dec	ecx
			.UNTIL	ZERO?
		.ELSE
			.REPEAT
				push	ecx
				;;
				@GetABit	esc_label
				push	eax
				@GetGammaCode	esc_label
				pop	edx
				mov	ecx, nDecoded
				xor	eax, edx
				inc	ecx
				sub	eax, edx
				mov	nDecoded, ecx
				mov	SBYTE PTR [edi], al
				;;
				pop	ecx
				inc	edi
				dec	ecx
			.UNTIL	ZERO?
		.ENDIF
		;;
		mov	eax, nDecBufLeft
		mov	ecx, [ebx].m_nLength
		test	eax, eax
		.IF	ZERO?
			;;
			;; �o��
			mov	edi, ptrDecBuf
			mov	esi, ptrDst
			mov	nDecBufLeft, 20H
			movaps	xmm0, [edi]
			movaps	xmm1, [edi + 16]
			movntps	[esi], xmm0
			movntps	[esi + 16], xmm1
			add	esi, 32
			;;
			;; �I���H
			mov	eax, nDecoded
			mov	ptrDst, esi
			.BREAK	.IF	eax >= nCount
		.ENDIF
		;;
		;; �����O�X�R�[�h���擾
		test	ecx, ecx
		.IF	ZERO?
			not	[ebx].m_flgZero
			@GetGammaCode	esc_label
			mov	[ebx].m_nLength, eax
		.ENDIF
	.ENDW
	;
	sfence

	.ELSE
	;
	;	486 �݊��R�[�h
	; --------------------------------------------------------------------
	.WHILE	1
		;;
		;; �o�̓V���{�������Z�o
		mov	ecx, [ebx].m_nLength
		mov	eax, nCount
		mov	edx, ecx
		.IF	ecx > eax
			mov	ecx, eax
		.ENDIF
		sub	edx, ecx
		sub	eax, ecx
		mov	[ebx].m_nLength, edx
		;;
		;; �V���{�����o��
		mov	edx, [ebx].m_flgZero
		mov	nCount, eax
		test	edx, edx
		.IF	ZERO?
			add	nDecoded, ecx
			xor	eax, eax
			.IF	ecx != 0
			.REPEAT
				mov	SBYTE PTR [edi], al
				inc	edi
				dec	ecx
			.UNTIL	ZERO?
			.ENDIF
		.ELSE
			.IF	ecx != 0
			.REPEAT
				push	ecx
				;;
				@GetABit	esc_label
				push	eax
				@GetGammaCode	esc_label
				pop	edx
				mov	ecx, nDecoded
				xor	eax, edx
				inc	ecx
				sub	eax, edx
				mov	nDecoded, ecx
				mov	SBYTE PTR [edi], al
				;;
				pop	ecx
				inc	edi
				dec	ecx
			.UNTIL	ZERO?
			.ENDIF
		.ENDIF
		;;
		;; �I���H
		.BREAK	.IF	nCount == 0
		;;
		;; �����O�X�R�[�h���擾
		not	[ebx].m_flgZero
		@GetGammaCode	esc_label
		mov	[ebx].m_nLength, eax
	.ENDW
	.ENDIF

	.IF	[ebx].m_nLength == 0
		not	[ebx].m_flgZero
	.ENDIF

	ASSUME	ebx:NOTHING

esc_label:
	mov	esp, dwInitESP
	mov	eax, nDecoded
	ret	4 * 2

ERISADecodeContext@@DecodeGammaCodeBytes	ENDP

ENDIF	;	IFNDEF	COMPACT_NOA_DECODER

;
;	�n�t�}�������擾�}�N��
; ----------------------------------------------------------------------------
; @GetHuffmanCode <esc_label>
; ----------------------------------------------------------------------------
; ���� ;
;	EBX register	: ERISADecodeContext �\���̂ւ̃|�C���^
;	ESI register	: ERINA_HUFFMAN_TREE �\���̂ւ̃|�C���^
; �s�ϕϐ��G
;	_this		: PTR ERISADecodeContext
;	tree		: PTR ERINA_HUFFMAN_TREE
; ���W�X�^�G
;	EBX, EBP, ESP ���W�X�^�͕ۑ������
;	EAX ���W�X�^�ɕ������ꂽ�������Ԃ����
; ----------------------------------------------------------------------------

@GetHuffmanCode	MACRO	esc_label:REQ
	LOCAL	Label_Exit, Label_ESC, Label_ESC2, Label_Decoded

	PUSHCONTEXT	ASSUMES
	.IF	[esi].m_hnTree[ERINA_HUFFMAN_ROOT*8].m_child_code != ERINA_HUFFMAN_NULL
		;;
		mov	ecx, [ebx].m_nIntBufCount
		mov	edx, [ebx].m_dwIntBuffer
		push	ebp
		mov	eax, [esi].m_hnTree[ERINA_HUFFMAN_ROOT * 8].m_child_code
		;;
		;; �n�t�}�������𕜍�
		;;
		@PrefetchBuffer	Label_ESC, 1, 10H
		.REPEAT
			;;
			;; 1�r�b�g���o��
			add	edx, edx
			adc	eax, 0
			dec	ecx
			@PrefetchBuffer	Label_ESC2, 2
			mov	ebp, eax
			mov	eax, [esi].m_hnTree[eax * 8].m_child_code
		.UNTIL	eax & ERINA_CODE_FLAG
Label_Decoded:	;;
		and	eax, NOT ERINA_CODE_FLAG
		pop	edi
		push	eax
		mov	eax, ebp
		mov	ebp, edi
		ASSUME	edi:NOTHING
		mov	[ebx].m_nIntBufCount, ecx
		mov	[ebx].m_dwIntBuffer, edx
		;;
		;; �����̏o���p�x�𑝉�������
		.IF	([ebx].m_dwERINAFlags != 0) || \
			([esi].m_hnTree[ERINA_HUFFMAN_ROOT*8].\
				m_weight < ERINA_HUFFMAN_MAX-1)
			mov	ebx, esi
			ASSUME	ebx:PERINA_HUFFMAN_TREE
			ASSUME	esi:NOTHING
			@TREE@@IncreaseOccuredCount	1
		.ENDIF
		;;
		;; �G�X�P�[�v�R�[�h������
		pop	eax
		cmp	eax, ERINA_HUFFMAN_ESCAPE
		jnz	Label_Exit
	.ENDIF
	;;
	;; �G�X�P�[�v�R�[�h�̎���8�r�b�g�Œ蒷
	mov	ebx, _this
	ASSUME	ebx:PERISADecodeContext
	mov	esi, 8
	@GetNBits	esc_label
	;;
	push	eax
	push	eax
	mov	ecx, tree
	call	ERINA_HUFFMAN_TREE@@AddNewEntry
	pop	eax
	POPCONTEXT	ASSUMES
	jmp	Label_Exit

Label_ESC:
	pop	ebp
	jmp	esc_label

Label_ESC2:
	mov	ebp, eax
	mov	eax, [esi].m_hnTree[eax * 8].m_child_code
	test	eax, ERINA_CODE_FLAG
	jnz	Label_Decoded
	pop	ebp
	jmp	esc_label

Label_Exit:

	ENDM

;
;	�����̃n�t�}�������擾�}�N��
; ----------------------------------------------------------------------------
; @GetLengthHuffman <esc_label>
; ----------------------------------------------------------------------------
; ���� ;
;	EBX register	: ERISADecodeContext �\���̂ւ̃|�C���^
;	ESI register	: ERINA_HUFFMAN_TREE �\���̂ւ̃|�C���^
; �s�ϕϐ��G
;	_this		: PTR ERISADecodeContext
;	tree		: PTR ERINA_HUFFMAN_TREE
; ���W�X�^�G
;	EBX, EBP, ESP ���W�X�^�͕ۑ������
;	EAX ���W�X�^�ɕ������ꂽ�������Ԃ����
; ----------------------------------------------------------------------------

@GetLengthHuffman	MACRO	esc_label:REQ
	LOCAL	Label_Exit, Label_ESC, Label_ESC2, Label_Decoded

	PUSHCONTEXT	ASSUMES
	.IF	[esi].m_hnTree[ERINA_HUFFMAN_ROOT*8].m_child_code != ERINA_HUFFMAN_NULL
		;;
		;; �r�b�g�}�b�v�e�[�u�����g���Đ擪��4�r�b�g�����
		mov	ecx, [ebx].m_nIntBufCount
		mov	edx, [ebx].m_dwIntBuffer
		push	ebp
		mov	eax, [esi].m_hnTree[ERINA_HUFFMAN_ROOT * 8].m_child_code
		;;
		;; �n�t�}�������𕜍�
		;;
		@PrefetchBuffer	Label_ESC, 1, 10H
		.REPEAT
			;;
			;; 1�r�b�g���o��
			add	edx, edx
			adc	eax, 0
			dec	ecx
			@PrefetchBuffer	Label_ESC2, 2
			mov	ebp, eax
			mov	eax, [esi].m_hnTree[eax * 8].m_child_code
		.UNTIL	eax & ERINA_CODE_FLAG
Label_Decoded:	;;
		and	eax, not ERINA_CODE_FLAG
		pop	edi
		push	eax
		mov	eax, ebp
		mov	ebp, edi
		ASSUME	edi:NOTHING
		mov	[ebx].m_nIntBufCount, ecx
		mov	[ebx].m_dwIntBuffer, edx
		;;
		;; �����̏o���p�x�𑝉�������
		.IF	([ebx].m_dwERINAFlags != 0) || \
			([esi].m_hnTree[ERINA_HUFFMAN_ROOT*8].\
				m_weight < ERINA_HUFFMAN_MAX-1)
			mov	ebx, esi
			ASSUME	ebx:PERINA_HUFFMAN_TREE
			ASSUME	esi:NOTHING
			@TREE@@IncreaseOccuredCount	1
		.ENDIF
		;;
		;; �G�X�P�[�v�R�[�h������
		pop	eax
		cmp	eax, ERINA_HUFFMAN_ESCAPE
		jnz	Label_Exit
	.ENDIF
	;;
	;; �G�X�P�[�v�R�[�h�̎��̓K���}����
	mov	ebx, _this
	ASSUME	ebx:PERISADecodeContext
	@GetGammaCode	esc_label
	;;
	push	eax
	push	eax
	mov	ecx, tree
	call	ERINA_HUFFMAN_TREE@@AddNewEntry
	pop	eax
	POPCONTEXT	ASSUMES
	jmp	Label_Exit

Label_ESC:
	pop	ebp
	jmp	esc_label

Label_ESC2:
	mov	ebp, eax
	mov	eax, [esi].m_hnTree[eax * 8].m_child_code
	test	eax, ERINA_CODE_FLAG
	jnz	Label_Decoded
	pop	ebp
	jmp	esc_label

Label_Exit:

	ENDM


IFNDEF	COMPACT_NOA_DECODER

;
;	�n�t�}��������W�J
; ----------------------------------------------------------------------------
Align	10H
ERISADecodeContext@@GetHuffmanCode	PROC	NEAR32 SYSCALL USES ebx esi edi,
	tree:PERINA_HUFFMAN_TREE

	LOCAL	_this:PERISADecodeContext
	LOCAL	dwInitESP:DWORD
	LOCAL	phnEntry:PTR ERINA_HUFFMAN_NODE

	mov	dwInitESP, esp

	mov	ebx, ecx
	mov	_this, ecx
	mov	esi, tree

	ASSUME	ebx:PTR ERISADecodeContext
	ASSUME	esi:PTR ERINA_HUFFMAN_TREE
	@GetHuffmanCode	esc_label
	ASSUME	ebx:NOTHING
	ASSUME	esi:NOTHING

	ret	4

esc_label:
	mov	esp, dwInitESP
	mov	eax, ERINA_HUFFMAN_ESCAPE
	ret	4

ERISADecodeContext@@GetHuffmanCode	ENDP

;
;	�����̃n�t�}��������W�J
; ----------------------------------------------------------------------------
Align	10H
ERISADecodeContext@@GetLengthHuffman	PROC	NEAR32 SYSCALL USES ebx esi edi,
	tree:PERINA_HUFFMAN_TREE
	LOCAL	_this:PERISADecodeContext

	LOCAL	dwInitESP:DWORD
	LOCAL	phnEntry:PTR ERINA_HUFFMAN_NODE

	mov	dwInitESP, esp

	mov	ebx, ecx
	mov	_this, ecx
	mov	esi, tree

	ASSUME	ebx:PTR ERISADecodeContext
	ASSUME	esi:PTR ERINA_HUFFMAN_TREE
	@GetLengthHuffman	esc_label
	ASSUME	ebx:NOTHING
	ASSUME	esi:NOTHING

	ret	4

esc_label:
	mov	esp, dwInitESP
	mov	eax, ERINA_HUFFMAN_ESCAPE
	ret	4

ERISADecodeContext@@GetLengthHuffman	ENDP

;
;	ERINA �����𕜍�����
; ----------------------------------------------------------------------------
Align	10H
ERISADecodeContext@@DecodeERINACodeBytes	\
				PROC	NEAR32 SYSCALL USES ebx esi edi,
	ptrDst:PSBYTE, nCount:DWORD

	LOCAL	_this:PERISADecodeContext
	LOCAL	dwInitESP:DWORD
	LOCAL	phnEntry:PTR ERINA_HUFFMAN_NODE
	LOCAL	tree:PTR ERINA_HUFFMAN_TREE
	LOCAL	nDecoded:DWORD
	LOCAL	bufDecode[40H]:SBYTE
	LOCAL	ptrDecBuf:PTR SBYTE
	LOCAL	ptrBufNext:PTR SBYTE
	LOCAL	nDecBufLeft:DWORD

	mov	dwInitESP, esp

	mov	ebx, ecx
	mov	_this, ecx
	ASSUME	ebx:PTR ERISADecodeContext

	.IF	0;(ERI_EnabledProcessorType & ERI_USE_XMM_P3) && \
		;		!(ptrDst & 0FH) && !(nCount & 1FH)
	;
	;	SSE ��p�R�[�h
	; --------------------------------------------------------------------
	mov	eax, [ebx].m_pLastHuffmanTree
	mov	tree, eax
	mov	esi, eax
	;
	lea	eax, bufDecode[0]
	add	eax, 0FH
	and	eax, NOT 0FH
	mov	ptrDecBuf, eax
	mov	ptrBufNext, eax
	prefetcht1	[eax]
	mov	nDecBufLeft, 20H
	mov	ecx, [ebx].m_nLength
	mov	nDecoded, 0
	;
	.WHILE	1
		;
		; �냉�������O�X�̎c���W�J
		;
		test	ecx, ecx
		mov	edx, nDecBufLeft
		.WHILE	!ZERO?
			cmp	ecx, edx
			cmova	ecx, edx
			xor	eax, eax
			sub	[ebx].m_nLength, ecx
			sub	edx, ecx
			mov	edi, ptrBufNext
			mov	nDecBufLeft, edx
			add	nDecoded, ecx
			.REPEAT
				mov	SBYTE PTR [edi], al
				inc	edi
				dec	ecx
			.UNTIL	ZERO?
			test	edx, edx
			mov	ptrBufNext, edi
			.BREAK	.IF	!ZERO?
			;
			mov	ecx, [ebx].m_nLength
			mov	edi, ptrDecBuf
			mov	eax, ptrDst
			mov	nDecBufLeft, 20H
			movaps	xmm0, [edi]
			movaps	xmm1, [edi + 16]
			mov	edx, nDecoded
			movntps	[eax], xmm0
			cmp	edx, nCount
			movntps	[eax + 16], xmm1
			jae	Label_ExitSSE
			add	eax, 20H
			mov	edx, 20H
			mov	ptrDst, eax
			mov	ptrBufNext, edi
			test	ecx, ecx
		.ENDW
		;
		; ���̕����𕜍�
		;
		ASSUME	esi:PTR ERINA_HUFFMAN_TREE
		@GetHuffmanCode	esc_label
		ASSUME	esi:NOTHING
		mov	ecx, nDecoded
		mov	edi, ptrBufNext
		mov	edx, nDecBufLeft
		mov	ebx, _this
		mov	SBYTE PTR [edi], al
		mov	esi, [ebx].m_ppHuffmanTree
		and	eax, 0FFH
		inc	ecx
		inc	edi
		dec	edx
		mov	esi, DWORD PTR [esi + eax * 4]
		mov	nDecoded, ecx
		;
		.IF	ZERO?
			mov	edi, ptrDecBuf
			mov	edx, ptrDst
			movaps	xmm0, [edi]
			movaps	xmm1, [edi + 16]
			movntps	[edx], xmm0
			movntps	[edx + 16], xmm1
			add	edx, 32
			cmp	ecx, nCount
			mov	ptrDst, edx
			jae	Label_ExitSSE
			mov	edx, 20H
		.ENDIF
		mov	ptrBufNext, edi
		mov	nDecBufLeft, edx
		mov	tree, esi
		;
		; �냉�������O�X
		;
		xor	ecx, ecx
		test	eax, eax
		.IF	ZERO?
			;
			; �����̕������擾
			;
			push	esi
			mov	esi, [ebx].m_ppHuffmanTree
			mov	esi, DWORD PTR [esi + 100H * 4]
			ASSUME	esi:PTR ERINA_HUFFMAN_TREE
			mov	tree, esi
			@GetLengthHuffman	esc_label
			pop	esi
			mov	ebx, _this
			ASSUME	esi:NOTHING
			lea	ecx, [eax - 1]
			mov	tree, esi
			mov	[ebx].m_nLength, ecx
		.ENDIF
	.ENDW
Label_ExitSSE:
	sfence

	.ELSE
	;
	;	486 �݊��R�[�h
	; --------------------------------------------------------------------
	;;
	;; �냉�������O�X�̎c���W�J
	mov	ecx, [ebx].m_nLength
	mov	nDecoded, 0
	.IF	ecx != 0
		.IF	ecx > nCount
			mov	ecx, nCount
		.ENDIF
		xor	eax, eax
		mov	edi, ptrDst
		sub	[ebx].m_nLength, ecx
		sub	nCount, ecx
		add	nDecoded, ecx
		.IF	ecx != 0
		.REPEAT
			mov	SBYTE PTR [edi], al
			inc	edi
			dec	ecx
		.UNTIL	ZERO?
		.ENDIF
		mov	ptrDst, edi
	.ENDIF
	;;
	mov	eax, [ebx].m_pLastHuffmanTree
	mov	ecx, nCount
	mov	tree, eax
	mov	esi, eax
	.WHILE	ecx != 0
		;;
		;; ���̕����𕜍�
		ASSUME	esi:PTR ERINA_HUFFMAN_TREE
		@GetHuffmanCode	esc_label
		ASSUME	esi:NOTHING
		mov	ecx, nDecoded
		mov	edi, ptrDst
		mov	edx, nCount
		movsx	eax, al
		mov	ebx, _this
		inc	ecx
		dec	edx
		mov	SBYTE PTR [edi], al
		mov	nDecoded, ecx
		mov	nCount, edx
		inc	edi
		;;
		;; �냉�������O�X
		.IF	 eax == 0
			;;
			;; �����̕������擾
			mov	esi, [ebx].m_ppHuffmanTree
			mov	esi, DWORD PTR [esi + 100H * 4]
			ASSUME	esi:PTR ERINA_HUFFMAN_TREE
			mov	tree, esi
			mov	ptrDst, edi
			@GetLengthHuffman	esc_label
			mov	ebx, _this
			mov	edi, ptrDst
			ASSUME	esi:NOTHING
			;;
			dec	eax
			.IF	!ZERO?
				;;
				;; �냉�������O�X��W�J
				mov	ecx, eax
				.IF	eax > nCount
					mov	ecx, nCount
				.ENDIF
				sub	eax, ecx
				sub	nCount, ecx
				add	nDecoded, ecx
				mov	[ebx].m_nLength, eax
				xor	eax, eax
				.IF	ecx != 0
				.REPEAT
					mov	SBYTE PTR [edi], al
					inc	edi
					dec	ecx
				.UNTIL	ZERO?
				.ENDIF
			.ENDIF
		.ENDIF
		and	eax, 0FFH
		mov	esi, [ebx].m_ppHuffmanTree
		mov	ptrDst, edi
		mov	ecx, nCount
		mov	esi, DWORD PTR [esi + eax * 4]
		mov	tree, esi
	.ENDW
	.ENDIF
	;;
	mov	[ebx].m_pLastHuffmanTree, esi

	ASSUME	ebx:NOTHING

esc_label:
	mov	esp, dwInitESP
	mov	eax, nDecoded
	ret	4 * 2

ERISADecodeContext@@DecodeERINACodeBytes	ENDP

ENDIF	;	IFNDEF	COMPACT_NOA_DECODER


;
;	ERISA �Z�p�����̓��v��������������
; ----------------------------------------------------------------------------
ALIGN	10H
ERISA_PROB_MODEL@@Initialize	PROC	NEAR32 SYSCALL USES ebx esi edi

	mov	ebx, ecx
	ASSUME	ebx:PERISA_PROB_MODEL

	mov	[ebx].dwTotalCount, ERISA_SYMBOL_SORTS
	mov	[ebx].dwSymbolSorts, ERISA_SYMBOL_SORTS

	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
		lea	edi, [ebx].acsSymTable[0]
		mov	ecx, 100H / 4
		movq	mm0, mmxInitErisaProb
		movq	mm2, mmxDeltaErisaProb
		movq	mm1, mm0
		paddw	mm1, mm2
		paddw	mm2, mm2
		.REPEAT
			movntq	MMWORD PTR [edi], mm0
			movntq	MMWORD PTR [edi + 8], mm1
			add	edi, 16
			paddw	mm0, mm2
			paddw	mm1, mm2
			dec	ecx
		.UNTIL	ZERO?
		;
		IF	ERISA_ESC_CODE NE (-1)
			.ERR
		ENDIF
		pcmpeqd	mm0, mm0
		pslld	mm0, 16
		por	mm0, mmxInitErisaProb
		movntq	MMWORD PTR [edi], mm0
		;
		pcmpeqd	mm0, mm0
		pslld	mm0, 16
		lea	edi, [ebx].acsSubModel[0]
		mov	ecx, ERISA_SUB_SORT_MAX / 4
		.REPEAT
			movntq	MMWORD PTR [edi], mm0
			movntq	MMWORD PTR [edi + 8], mm0
			add	edi, 16
			dec	ecx
		.UNTIL	ZERO?
		sfence
		emms
	.ELSE
		xor	ecx, ecx
		.REPEAT
			mov	[ebx].acsSymTable[ecx*4].wOccured, 1
			mov	[ebx].acsSymTable[ecx*4].wSymbol, cx
			inc	ecx
		.UNTIL	ecx >= 100H
		mov	[ebx].acsSymTable[ecx*4].wOccured, 1
		mov	[ebx].acsSymTable[ecx*4].wSymbol, ERISA_ESC_CODE
		;
		mov	ecx, ERISA_SUB_SORT_MAX
		mov	eax, 0FFFF0000H
		lea	edi, [ebx].acsSubModel[0]
		.REPEAT
			mov	DWORD PTR [edi], eax
			add	edi, 4
			dec	ecx
		.UNTIL	ZERO?
	.ENDIF

	ASSUME	ebx:NOTHING
	ret	0

ERISA_PROB_MODEL@@Initialize	ENDP

;
;	�w��̃V���{���̐��N�����C���N�������g����
; ----------------------------------------------------------------------------
ALIGN	10H
ERISA_PROB_MODEL@@IncreaseSymbol	PROC	NEAR32 SYSCALL USES ebx esi edi,
		index:SDWORD

	mov	ebx, ecx
	ASSUME	ebx:PERISA_PROB_MODEL

	mov	ecx, index
	mov	eax, DWORD PTR [ebx].acsSymTable[ecx*4]
	inc	eax

	sub	ecx, 1
	.WHILE	!SIGN?
		mov	edx, DWORD PTR [ebx].acsSymTable[ecx*4]
		.BREAK	.IF	dx >= ax
		mov	DWORD PTR [ebx].acsSymTable[ecx*4+4], edx
		sub	ecx, 1
	.ENDW
	mov	DWORD PTR [ebx].acsSymTable[ecx*4+4], eax

	mov	eax, [ebx].dwTotalCount
	inc	eax
	mov	[ebx].dwTotalCount, eax
	.IF	eax >= ERISA_TOTAL_LIMIT
		push	ecx
		mov	ecx, ebx
		INVOKE	ERISA_PROB_MODEL@@HalfOccuredCount
		pop	ecx
	.ENDIF

	ASSUME	ebx:NOTHING
	mov	eax, ecx
	ret	4

ERISA_PROB_MODEL@@IncreaseSymbol	ENDP

;
;	�w��̓��v���f�����g����1�̎Z�p�����𕜍�
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecodeContext@@DecodeERISACode	\
		PROC	NEAR32 SYSCALL USES ebx esi edi,
			pModel:PERISA_PROB_MODEL

	LOCAL	nSymbol:SDWORD

	mov	ebx, ecx
	mov	esi, pModel
	ASSUME	esi:PERISA_PROB_MODEL
	push	esi
	call	NEAR32 PTR ERISADecodeContext@@DecodeERISACodeIndex

	mov	nSymbol, ERISA_ESC_CODE
	test	eax, eax
	.IF	!SIGN?
		movsx	edx, [esi].acsSymTable[eax*4].wSymbol
		mov	nSymbol, edx
		mov	ecx, esi
		push	eax
		call	NEAR32 PTR ERISA_PROB_MODEL@@IncreaseSymbol
	.ENDIF

	ASSUME	esi:NOTHING
	mov	eax, nSymbol
	ret	4

ERISADecodeContext@@DecodeERISACode	ENDP

;
;	�w��̓��v���f�����g����1�̎Z�p�����𕜍�
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecodeContext@@DecodeERISACodeIndex	\
		PROC	NEAR32 SYSCALL USES ebx esi edi,
			pModel:PERISA_PROB_MODEL

	LOCAL	dwInitESP:DWORD
	LOCAL	iSym:SDWORD

	mov	ebx, ecx
	mov	esi, pModel
	mov	dwInitESP, esp

	ASSUME	ebx:PERISADecodeContext
	ASSUME	esi:PERISA_PROB_MODEL

	;
	; �w�W�𕜍����Č���
	;
	mov	eax, [ebx].m_dwCodeRegister
	imul	[esi].dwTotalCount
	xor	edx, edx
	div	[ebx].m_dwAugendRegister
	;
	xor	edx, edx
	xor	edi, edi
	@PrefetchBuffer	esc_label, 0
	;
	.WHILE	1
		movzx	ecx, [esi].acsSymTable[edx*4].wOccured
		.BREAK	.IF	eax < ecx
		inc	edx
		sub	eax, ecx
		add	edi, ecx
		.CONTINUE	.IF	edx < [esi].dwSymbolSorts
		jmp	esc_label
	.ENDW
	mov	iSym, edx
	;
	; �R�[�h���W�X�^�ƃI�[�W�F���h���W�X�^���X�V
	;
	mov	eax, [ebx].m_dwAugendRegister
	mov	esi, [esi].dwTotalCount
	ASSUME	esi:NOTHING
	imul	edi, eax
	xor	edx, edx
	imul	eax, ecx
	div	esi
	xor	edx, edx
	mov	ecx, eax
	mov	eax, edi
	div	esi
	mov	edi, [ebx].m_dwCodeRegister
	add	edx, 0FFFFFFFFH
	adc	eax, 0
	;
	; �I�[�W�F���h���W�X�^�𐳋K�����A�R�[�h���W�X�^�ɕ�����ǂݍ���
	;
	mov	esi, 15
	sub	edi, eax
	bsr	edx, ecx
	jz	esc_label
	sub	esi, edx
	.IF	!ZERO?
		mov	eax, ecx
		mov	ecx, [ebx].m_nIntBufCount
		mov	edx, [ebx].m_dwIntBuffer
		.WHILE	1
LoopBegin_AugReg:	.IF	esi < ecx
				mov	ecx, esi
			.ENDIF
			shl	eax, cl
			shld	edi, edx, cl
			shl	edx, cl
			sub	esi, ecx
			.BREAK	.IF	ZERO?
			xor	ecx, ecx
			@PrefetchBuffer	Label_PostBit, 1
			mov	[ebx].m_nIntBufCount, ecx
		.ENDW
		sub	[ebx].m_nIntBufCount, ecx
		mov	[ebx].m_dwIntBuffer, edx
		mov	ecx, eax
	.ENDIF
	mov	[ebx].m_dwAugendRegister, ecx
	mov	[ebx].m_dwCodeRegister, edi

	mov	esp, dwInitESP
	mov	eax, iSym
	ret	4

Label_PostBit:
	mov	[ebx].m_nIntBufCount, 32
	mov	[ebx].m_dwIntBuffer, 0
	add	[ebx].m_nPostBitCount, 32
	mov	ecx, 32
	xor	edx, edx
	cmp	[ebx].m_nPostBitCount, 32
	jb	LoopBegin_AugReg

	ASSUME	ebx:NOTHING

esc_label:
	mov	esp, dwInitESP
	mov	eax, -1
	ret	4

ERISADecodeContext@@DecodeERISACodeIndex	ENDP


CodeSeg	ENDS

	END
