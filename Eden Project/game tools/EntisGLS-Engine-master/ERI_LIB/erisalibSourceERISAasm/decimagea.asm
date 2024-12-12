
; ****************************************************************************
;                        E R I S A - L i b r a r y
; ----------------------------------------------------------------------------
;         Copyright (C) 2000-2004 Leshade Entis. All rights reserved.
; ****************************************************************************


	.686
	.XMM
	.MODEL	FLAT

	INCLUDE	esl.inc
	INCLUDE	experi.inc
	INCLUDE	erisamatrix.inc

MMWORD	TYPEDEF	QWORD


IFNDEF	COMPACT_NOA_DECODER


; ----------------------------------------------------------------------------
;                            データセグメント
; ----------------------------------------------------------------------------

DataSeg	SEGMENT	PARA FLAT 'DATA'

DataSeg	ENDS


ConstSeg	SEGMENT	PARA READONLY FLAT 'CONST'

ALIGN	10H
ERI_rDiv32	REAL8	0.03125
ERI_rDiv4	REAL8	0.25

ALIGN	10H
mmxMask_ByteMSB	LABEL	MMWORD
	BYTE	8 DUP( 80H )
mmxMask_OddWord	LABEL	MMWORD
	SWORD	0, -1, 0, -1
mmxMask_Byte7F	LABEL	MMWORD
	BYTE	8 DUP( 7FH )

ALIGN	10H
mmxParam_IDCT8x8	LABEL	MMWORD
	SWORD 16385,  16069,  15137,  13623,  11585,   9103,   6271,   3197
	SWORD 16385,  13623,   6271,  -3196, -11585, -16069, -15137,  -9102
	SWORD 16385,   9103,  -6270, -16069, -11585,   3197,  15137,  13623
	SWORD 16385,   3197, -15137,  -9102,  11585,  13623,  -6270, -16069
	SWORD 16385,  -3196, -15137,   9103,  11585, -13623,  -6270,  16069
	SWORD 16385,  -9102,  -6270,  16069, -11585,  -3196,  15137, -13623
	SWORD 16385, -13623,   6271,   3197, -11585,  16069, -15137,   9103
	SWORD 16385, -16069,  15137, -13623,  11585,  -9102,   6271,  -3196
IF	0
	; 素直な IDCT 係数値
	SWORD 16384,  16069,  15137,  13623,  11585,   9102,   6270,   3196
	SWORD 16384,  13623,   6270,  -3196, -11585, -16069, -15137,  -9102
	SWORD 16384,   9102,  -6270, -16069, -11585,   3196,  15137,  13623
	SWORD 16384,   3196, -15137,  -9102,  11585,  13623,  -6270, -16069
	SWORD 16384,  -3196, -15137,   9102,  11585, -13623,  -6270,  16069
	SWORD 16384,  -9102,  -6270,  16069, -11585,  -3196,  15137, -13623
	SWORD 16384, -13623,   6270,   3196, -11585,  16069, -15137,   9102
	SWORD 16384, -16069,  15137, -13623,  11585,  -9102,   6270,  -3196
ENDIF

ALIGN	10H
mmxParam_ILOT_VERT	LABEL	MMWORD
	; { 0.678598, 0.734510 } * 8000H
	SWORD	 22237, 24069,  22237, 24069
	SWORD	-24068, 22237, -24068, 22237
	; { 0.460917, 0.887443 } * 8000H
	SWORD	 15103, 29081,  15103, 29081
	SWORD	-29080, 15103, -29080, 15103
	; { 0.242030, 0.970269 } * 8000H
	SWORD	  7931, 31795,   7931, 31795
	SWORD	-31794,  7931, -31794,  7931
IF	0
	; { 0.678598, 0.734510 } * 8000H
	SWORD	 22236, 24068,  22236, 24068
	SWORD	-24068, 22236, -24068, 22236
	; { 0.460917, 0.887443 } * 8000H
	SWORD	 15103, 29080,  15103, 29080
	SWORD	-29080, 15103, -29080, 15103
	; { 0.242030, 0.970269 } * 8000H
	SWORD	  7931, 31794,   7931, 31794
	SWORD	-31794,  7931, -31794,  7931
ENDIF

ALIGN	10H
mmxParam_ILOT_HORZ	LABEL	MMWORD
	SWORD	 22237, 24069,  22237, 24069
	SWORD	-24068, 22237, -24068, 22237
	SWORD	 15103, 29081,  15103, 2908
	SWORD	-29080, 15103, -29080, 15103
	SWORD	  7931, 31795,   7931, 31795
	SWORD	-31794,  7931, -31794,  7931
IF	0
	SWORD	 22236, 24068,  22236, 24068
	SWORD	-24068, 22236, -24068, 22236
	SWORD	 15103, 29080,  15103, 29080
	SWORD	-29080, 15103, -29080, 15103
	SWORD	  7931, 31794,   7931, 31794
	SWORD	-31794,  7931, -31794,  7931
ENDIF

nParam_ILOT	LABEL	SDWORD
	SDWORD	24068, 22236
	SDWORD	29080, 15103
	SDWORD	31794,  7931

pfnMovePBlock_SSE	LABEL	PTR NEAR32
	DWORD	OFFSET Func_MovePBlock0_SSE
	DWORD	OFFSET Func_MovePBlock1_SSE
	DWORD	OFFSET Func_MovePBlock2_SSE
	DWORD	OFFSET Func_MovePBlock3_SSE

pfnMovePBlock_MMX	LABEL	PTR NEAR32
	DWORD	OFFSET Func_MovePBlock0_MMX
	DWORD	OFFSET Func_MovePBlock1_MMX
	DWORD	OFFSET Func_MovePBlock2_MMX
	DWORD	OFFSET Func_MovePBlock2_MMX

pfnMoveBBlock_SSE	LABEL	PTR NEAR32
	DWORD	OFFSET Func_MoveBBlock0_SSE
	DWORD	OFFSET Func_MoveBBlock1_SSE
	DWORD	OFFSET Func_MoveBBlock2_SSE
	DWORD	OFFSET Func_MoveBBlock3_SSE

ConstSeg	ENDS


; ----------------------------------------------------------------------------
;                             コードセグメント
; ----------------------------------------------------------------------------

CodeSeg	SEGMENT	PARA READONLY FLAT 'CODE'

;
;	オペレーションを実行
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@PerformOperation		PROC	NEAR32 SYSCALL USES ebx esi edi,
	dwOpCode:DWORD, nAllBlockLines:DWORD, ptrNextLineBuf:PTR SBYTE

	LOCAL	_this:PERISADecoder
	LOCAL	iLoopCount:DWORD
	LOCAL	ptrLineBuf:PINT

	mov	ebx, ecx
	mov	_this, ecx
	ASSUME	ebx:PERISADecoder

	;
	;	再配列の実行
	; --------------------------------------------------------------------
	mov	eax, dwOpCode
	shr	eax, 4
	and	eax, 03H
	mov	ecx, [ebx].m_nBlockSamples
	mov	esi, [ebx].m_ptrArrangeBuf
	mov	edi, [ebx].m_ptrDecodeBuf
	mov	ebx, [ebx].m_pArrangeTable[eax * 4]
	ASSUME	ebx:NOTHING
	.REPEAT
		mov	eax, DWORD PTR [ebx]
		mov	dl, SBYTE PTR [esi]
		add	ebx, (SIZEOF DWORD)
		inc	esi
		mov	SBYTE PTR [edi + eax], dl
		dec	ecx
	.UNTIL	ZERO?
	mov	ebx, _this
	ASSUME	ebx:PERISADecoder

	;
	;	カラーオペレーションを実行
	; --------------------------------------------------------------------
	mov	eax, dwOpCode
	and	eax, 0FH
	mov	ecx, ebx
	call	ERISADecoder@@m_pfnColorOperation[eax * 4]

	;
	;	差分処理を実行（水平方向）
	; --------------------------------------------------------------------
	.IF	dwOpCode & 40H
		mov	ecx, nAllBlockLines
		mov	esi, [ebx].m_ptrDecodeBuf
		mov	edi, [ebx].m_ptrColumnBuf
		.REPEAT
			mov	iLoopCount, ecx
			;
			mov	ecx, [ebx].m_nBlockSize
			mov	al, SBYTE PTR [edi]
			mov	dl, SBYTE PTR [edi + 1]
			lea	ebx, [esi + ecx]
			ASSUME	ebx:NOTHING
			;
			.REPEAT
				add	al, SBYTE PTR [esi]
				add	dl, SBYTE PTR [ebx]
				mov	SBYTE PTR [esi], al
				inc	esi
				mov	SBYTE PTR [ebx], dl
				inc	ebx
				dec	ecx
			.UNTIL	ZERO?
			;
			mov	ecx, iLoopCount
			mov	esi, ebx
			mov	ebx, _this
			ASSUME	ebx:PERISADecoder
			mov	SBYTE PTR [edi], al
			mov	SBYTE PTR [edi + 1], dl
			add	edi, (SIZEOF SBYTE) * 2
			;
			sub	ecx, 2
		.UNTIL	ZERO?

	.ELSE
		mov	ecx, nAllBlockLines
		mov	edx, [ebx].m_nBlockSize
		mov	esi, [ebx].m_ptrDecodeBuf
		mov	edi, [ebx].m_ptrColumnBuf
		.REPEAT
			mov	al, SBYTE PTR [esi + edx - 1]
			lea	esi, [esi + edx]
			mov	SBYTE PTR [edi], al
			inc	edi
			dec	ecx
		.UNTIL	ZERO?
	.ENDIF

	;
	;	差分処理を実行（垂直方向）
	; --------------------------------------------------------------------
	mov	eax, [ebx].m_nBlockSize
	mov	edi, ptrNextLineBuf
	mov	esi, [ebx].m_ptrDecodeBuf
	mov	ecx, [ebx].m_nChannelCount
	;
	.IF	dwOpCode & 80H
	.IF	(eax >= 8) && \
			(ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM)
		.REPEAT
			mov	iLoopCount, ecx
			mov	ptrLineBuf, edi
			;
			mov	edx, [ebx].m_nBlockSize
			.REPEAT
				mov	ecx, [ebx].m_nBlockSize
				mov	eax, esi
				.REPEAT
					movq	mm0, MMWORD PTR [esi]
					movq	mm2, MMWORD PTR [edi]
					add	edi, 8
					paddb	mm0, mm2
					movq	MMWORD PTR [esi], mm0
					add	esi, 8
					sub	ecx, 8
				.UNTIL	ZERO?
				mov	edi, eax
				dec	edx
			.UNTIL	ZERO?
			;
			mov	edx, eax
			mov	edi, ptrLineBuf
			mov	ecx, [ebx].m_nBlockSize
			.REPEAT
				movq	mm0, MMWORD PTR [edx]
				add	edx, 8
				movq	MMWORD PTR [edi], mm0
				add	edi, 8
				sub	ecx, 8
			.UNTIL	ZERO?
			;
			mov	ecx, iLoopCount
			dec	ecx
		.UNTIL	ZERO?

	.ELSE
		.REPEAT
			push	ecx
			mov	ptrLineBuf, edi
			;
			mov	ecx, [ebx].m_nBlockSize
			.REPEAT
				push	ecx
				mov	ecx, [ebx].m_nBlockSize
				push	esi
				.REPEAT
					mov	al, SBYTE PTR [esi]
					mov	dl, SBYTE PTR [edi]
					inc	edi
					add	al, dl
					mov	SBYTE PTR [esi], al
					inc	esi
					dec	ecx
				.UNTIL	ZERO?
				pop	edi
				pop	ecx
				dec	ecx
			.UNTIL	ZERO?
			;
			mov	edx, edi
			mov	edi, ptrLineBuf
			mov	ecx, [ebx].m_nBlockSize
			.REPEAT
				mov	al, SBYTE PTR [edx]
				inc	edx
				mov	SBYTE PTR [edi], al
				inc	edi
				dec	ecx
			.UNTIL	ZERO?
			;
			pop	ecx
			dec	ecx
		.UNTIL	ZERO?

	.ENDIF
	.ENDIF

	.IF	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
		emms
	.ENDIF

	ASSUME	ebx:NOTHING

	ret	4 * 3

ERISADecoder@@PerformOperation		ENDP


;
;
;	チャネル間差分処理マクロ
; ----------------------------------------------------------------------------
;  Parameters ;
;	EBX register : ERISADecoder オブジェクトへのポインタ
;  Registers ;
;	EBX, EBP, ESP レジスタは保証される
; ----------------------------------------------------------------------------

@ERImage_DifferenceBetweenChannel	MACRO	op_code:REQ
	LOCAL	AddrSource, AddrDest1, AddrDest2
	LOCAL	Label_Exit

	IF	(op_code AND 1100B) EQ 0100B
		AddrSource	TEXTEQU	<edi>
		AddrDest1	TEXTEQU	<edi + edx>
		AddrDest2	TEXTEQU	<edi + edx * 2>
	ELSEIF	(op_code AND 1100B) EQ 1000B
		AddrSource	TEXTEQU	<edi + edx>
		AddrDest1	TEXTEQU	<edi>
		AddrDest2	TEXTEQU	<edi + edx * 2>
	ELSEIF	(op_code AND 1100B) EQ 1100B
		AddrSource	TEXTEQU	<edi + edx * 2>
		AddrDest1	TEXTEQU	<edi>
		AddrDest2	TEXTEQU	<edi + edx>
	ENDIF

	ASSUME	ebx:PTR ERISADecoder
	;;
	mov	ecx, [ebx].m_nBlockArea
	mov	edi, [ebx].m_ptrDecodeBuf
	mov	edx, ecx
	.IF	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
	sub	ecx, 16
	.WHILE	!SIGN?
		IF	op_code and 0001B
			movq	mm2, MMWORD PTR [AddrDest1]
			movq	mm3, MMWORD PTR [AddrDest1 + 8]
		ENDIF
		IF	op_code and 0010B
			movq	mm4, MMWORD PTR [AddrDest2]
			movq	mm5, MMWORD PTR [AddrDest2 + 8]
		ENDIF
		movq	mm0, MMWORD PTR [AddrSource]
		movq	mm1, MMWORD PTR [AddrSource + 8]
		IF	op_code and 0001B
			paddb	mm2, mm0
			paddb	mm3, mm1
		ENDIF
		IF	op_code and 0010B
			paddb	mm4, mm0
			paddb	mm5, mm1
		ENDIF
		IF	op_code and 0001B
			movq	MMWORD PTR [AddrDest1], mm2
			movq	MMWORD PTR [AddrDest1 + 8], mm3
		ENDIF
		IF	op_code and 0010B
			movq	MMWORD PTR [AddrDest2], mm4
			movq	MMWORD PTR [AddrDest2 + 8], mm5
		ENDIF
		add	edi, (SIZEOF SBYTE) * 16
		sub	ecx, 16
	.ENDW
	emms
	add	ecx, 16
	jz	Label_Exit
	.ENDIF
	.WHILE	ecx >= 2
		sub	ecx, 2
		mov	al, SBYTE PTR [AddrSource]
		mov	ah, SBYTE PTR [AddrSource + 1]
		IF	op_code AND 0001B
			add	SBYTE PTR [AddrDest1], al
			add	SBYTE PTR [AddrDest1 + 1], ah
		ENDIF
		IF	op_code AND 0010B
			add	SBYTE PTR [AddrDest2], al
			add	SBYTE PTR [AddrDest2 + 1], ah
		ENDIF
		add	edi, (SIZEOF SBYTE) * 2
	.ENDW
	.IF	ecx != 0
		mov	al, SBYTE PTR [AddrSource]
		IF	op_code AND 0001B
			add	SBYTE PTR [AddrDest1], al
		ENDIF
		IF	op_code AND 0010B
			add	SBYTE PTR [AddrDest2], al
		ENDIF
	.ENDIF
	;;
	ASSUME	ebx:NOTHING
Label_Exit:

					ENDM

ALIGN	10H
ERISADecoder@@ColorOperation0000	PROC	NEAR32 SYSCALL USES ebx esi edi
	ret
ERISADecoder@@ColorOperation0000	ENDP

ALIGN	10H
ERISADecoder@@ColorOperation0101	PROC	NEAR32 SYSCALL USES ebx esi edi
	mov	ebx, ecx
	@ERImage_DifferenceBetweenChannel	0101B
	ret
ERISADecoder@@ColorOperation0101	ENDP

ALIGN	10H
ERISADecoder@@ColorOperation0110	PROC	NEAR32 SYSCALL USES ebx esi edi
	mov	ebx, ecx
	@ERImage_DifferenceBetweenChannel	0110B
	ret
ERISADecoder@@ColorOperation0110	ENDP

ALIGN	10H
ERISADecoder@@ColorOperation0111	PROC	NEAR32 SYSCALL USES ebx esi edi
	mov	ebx, ecx
	@ERImage_DifferenceBetweenChannel	0111B
	ret
ERISADecoder@@ColorOperation0111	ENDP

ALIGN	10H
ERISADecoder@@ColorOperation1001	PROC	NEAR32 SYSCALL USES ebx esi edi
	mov	ebx, ecx
	@ERImage_DifferenceBetweenChannel	1001B
	ret
ERISADecoder@@ColorOperation1001	ENDP

ALIGN	10H
ERISADecoder@@ColorOperation1010	PROC	NEAR32 SYSCALL USES ebx esi edi
	mov	ebx, ecx
	@ERImage_DifferenceBetweenChannel	1010B
	ret
ERISADecoder@@ColorOperation1010	ENDP

ALIGN	10H
ERISADecoder@@ColorOperation1011	PROC	NEAR32 SYSCALL USES ebx esi edi
	mov	ebx, ecx
	@ERImage_DifferenceBetweenChannel	1011B
	ret
ERISADecoder@@ColorOperation1011	ENDP

ALIGN	10H
ERISADecoder@@ColorOperation1101	PROC	NEAR32 SYSCALL USES ebx esi edi
	mov	ebx, ecx
	@ERImage_DifferenceBetweenChannel	1101B
	ret
ERISADecoder@@ColorOperation1101	ENDP

ALIGN	10H
ERISADecoder@@ColorOperation1110	PROC	NEAR32 SYSCALL USES ebx esi edi
	mov	ebx, ecx
	@ERImage_DifferenceBetweenChannel	1110B
	ret
ERISADecoder@@ColorOperation1110	ENDP

ALIGN	10H
ERISADecoder@@ColorOperation1111	PROC	NEAR32 SYSCALL USES ebx esi edi
	mov	ebx, ecx
	@ERImage_DifferenceBetweenChannel	1111B
	ret
ERISADecoder@@ColorOperation1111	ENDP


;
;	逆量子化
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@ArrangeAndIQuantumize	PROC	NEAR32 SYSCALL USES ebx esi edi,
	ptrSrcData:PTR SBYTE, ptrCoeffient:PTR SBYTE

	LOCAL	_this:PTR ERISADecoder
	LOCAL	iLoop:DWORD
	LOCAL	nTemp[3]:SDWORD
	LOCAL	pIQParam[16]:PTR DWORD
	LOCAL	nIQScale[16]:DWORD
	LOCAL	buf[64*16]:BYTE

	mov	_this, ecx
	test	ERI_EnabledProcessorType, \
			(ERI_USE_MMX_PENTIUM OR ERI_USE_XMM_P3)
	jz	Label_ArrangeAndIQuantumize486
	mov	ebx, _this
	ASSUME	ebx:PTR ERISADecoder
	cmp	[ebx].m_nBlocksetCount, 6
	jnz	Label_ArrangeAndIQuantumize486

	;
	;	MMX 専用逆量子化
	; --------------------------------------------------------------------
	;
	; 逆量子化係数を算出
	;
	xor	ecx, ecx
	mov	edi, [ebx].m_ptrIQParamBuf
	.REPEAT
		mov	esi, ptrCoeffient
		mov	iLoop, ecx
		lea	eax, nIQScale[ecx * 4]
		;
		movsx	edx, SBYTE PTR [esi + ecx]
		shl	ecx, 6				; x64
		mov	esi, [ebx].m_ptrIQParamTable
		add	esi, ecx
		mov	ecx, edx
		and	edx, 01H
		sar	ecx, 1
		.IF	SIGN?
			neg	ecx
			add	ecx, edx
			mov	DWORD PTR [eax], ecx
			xor	ecx, ecx
		.ELSE
			mov	DWORD PTR [eax], edx
		.ENDIF
		;
		pxor	mm4, mm4
		pcmpeqw	mm5, mm5
		movd	mm6, ecx
		;
		test	edx, edx
		.IF	ZERO?
			mov	ecx, 64 / 8
			.REPEAT
				movq	mm0, MMWORD PTR [esi]
				add	esi, 8
				movq	mm1, mm0
				punpcklbw	mm0, mm4
				punpckhbw	mm1, mm4
				psubw	mm0, mm5
				psubw	mm1, mm5
				psllw	mm0, mm6
				psllw	mm1, mm6
				movq	MMWORD PTR [edi], mm0
				movq	MMWORD PTR [edi + 8], mm1
				add	edi, 16
				dec	ecx
			.UNTIL	ZERO?
		.ELSE
			mov	ecx, 64 / 8
			.REPEAT
				movq	mm0, MMWORD PTR [esi]
				add	esi, 8
				movq	mm1, mm0
				punpcklbw	mm0, mm4
				punpckhbw	mm1, mm4
				psubw	mm0, mm5
				psubw	mm1, mm5
				psllw	mm0, mm6
				psllw	mm1, mm6
				movq	mm2, mm0
				movq	mm3, mm1
				psllw	mm0, 1
				psllw	mm1, 1
				paddw	mm0, mm2
				paddw	mm1, mm3
				movq	MMWORD PTR [edi], mm0
				movq	MMWORD PTR [edi + 8], mm1
				add	edi, 16
				;
				dec	ecx
			.UNTIL	ZERO?
		.ENDIF
		;
		mov	ecx, iLoop
		inc	ecx
	.UNTIL	ecx >= 2
	;
	; 直流成分差分処理
	;
	.IF	[ebx].m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI
		mov	esi, ptrSrcData
		mov	al, SBYTE PTR [esi]
		add	SBYTE PTR [esi + 1], al
		add	SBYTE PTR [esi + 2], al
		add	SBYTE PTR [esi + 3], al
	.ENDIF
	;
	; インターリーブ
	;
	mov	esi, ptrSrcData
	lea	edi, buf[0]
	mov	edx, 64
	.REPEAT
		mov	al, SBYTE PTR [esi]
		mov	ah, SBYTE PTR [esi + 1]
		mov	cl, SBYTE PTR [esi + 2]
		mov	ch, SBYTE PTR [esi + 3]
		mov	SBYTE PTR [edi], al
		mov	al, SBYTE PTR [esi + 4]
		mov	SBYTE PTR [edi + 64], ah
		mov	ah, SBYTE PTR [esi + 5]
		mov	SBYTE PTR [edi + 64*2], cl
		add	esi, 6
		mov	SBYTE PTR [edi + 64*3], ch
		mov	SBYTE PTR [edi + 64*4], al
		mov	SBYTE PTR [edi + 64*5], ah
		inc	edi
		dec	edx
	.UNTIL	ZERO?
	lea	esi, buf
	mov	ptrSrcData, esi
	;
	; 逆量子化
	;
	mov	eax, [ebx].m_ptrIQParamBuf
	lea	edx, [eax + (64*(SIZEOF WORD))]
	mov	pIQParam[00H], eax
	mov	pIQParam[04H], eax
	mov	pIQParam[08H], eax
	mov	pIQParam[0CH], eax
	mov	pIQParam[10H], edx
	mov	pIQParam[14H], edx
	;
	mov	eax, nIQScale[00H]
	mov	edx, nIQScale[04H]
	mov	nIQScale[00H], eax
	mov	nIQScale[04H], eax
	mov	nIQScale[08H], eax
	mov	nIQScale[0CH], eax
	mov	nIQScale[10H], edx
	mov	nIQScale[14H], edx
	;
	xor	ecx, ecx
	mov	esi, ptrSrcData
	mov	edi, [ebx].m_ptrMatrixBuf
	.REPEAT
		;
		; ソースを符号あり16ビット整数へ変換する
		;
		movd	mm6, DWORD PTR nIQScale[ecx * 4]
		mov	ebx, pIQParam[ecx * 4]
		ASSUME	ebx:NOTHING
		;
		mov	edx, 64 / 8
		.REPEAT
			movq	mm7, MMWORD PTR [esi]
			add	esi, 8
			punpcklbw	mm0, mm7
				punpckhbw	mm2, mm7
			psraw	mm0, 8
				psraw	mm2, 8
			pmullw	mm0, MMWORD PTR [ebx]
			pmullw	mm2, MMWORD PTR [ebx + 8]
			add	ebx, 16
			psraw	mm0, mm6
			psraw	mm2, mm6
			movq	MMWORD PTR [edi], mm0
			movq	MMWORD PTR [edi + 8], mm2
			add	edi, 16
			dec	edx
		.UNTIL	ZERO?
		;
		inc	ecx
	.UNTIL	ecx >= 6
	;
	; ジグザグ走査
	;
	mov	ebx, _this
	ASSUME	ebx:PTR ERISADecoder
	mov	ecx, 64
	mov	esi, [ebx].m_ptrBlocksetBuf[0]
	mov	edi, [ebx].m_ptrMatrixBuf
	mov	edx, [ebx].m_pArrangeTable[0]
	push	ebp
	push	ebx
	.REPEAT
		mov	ebx, DWORD PTR [esp]
		ASSUME	ebx:PTR ERISADecoder
		mov	esi, DWORD PTR [edx]
		mov	eax, [ebx].m_ptrBlocksetBuf[0]
		add	edx, (SIZEOF DWORD)
		lea	esi, [eax + esi * (SIZEOF SWORD)]
		mov	ax, SWORD PTR [edi]
		mov	bx, SWORD PTR [edi + (SIZEOF SWORD)*64]
		mov	bp, SWORD PTR [edi + (SIZEOF SWORD)*64*2]
		mov	SWORD PTR [esi], ax
		mov	ax, SWORD PTR [edi + (SIZEOF SWORD)*64*3]
		mov	SWORD PTR [esi + (SIZEOF REAL4)*64], bx
		mov	bx, SWORD PTR [edi + (SIZEOF SWORD)*64*4]
		mov	SWORD PTR [esi + (SIZEOF REAL4)*64*2], bp
		mov	bp, SWORD PTR [edi + (SIZEOF SWORD)*64*5]
		mov	SWORD PTR [esi + (SIZEOF REAL4)*64*3], ax
		mov	SWORD PTR [esi + (SIZEOF REAL4)*64*4], bx
		mov	SWORD PTR [esi + (SIZEOF REAL4)*64*5], bp
		add	edi, (SIZEOF SWORD)
		dec	ecx
	.UNTIL	ZERO?
	pop	ebx
	pop	ebp

	ASSUME	ebx:NOTHING
	ret	2 * 4

Label_ArrangeAndIQuantumize486:
	;
	;	486 互換逆量子化
	; --------------------------------------------------------------------
	;
	; 逆量子化係数を算出
	;
	mov	ebx, _this
	ASSUME	ebx:PTR ERISADecoder
	xor	ecx, ecx
	mov	edi, [ebx].m_ptrIQParamBuf
	.REPEAT
		mov	esi, ptrCoeffient
		mov	iLoop, ecx
		;
		movsx	edx, SBYTE PTR [esi + ecx]
		shl	ecx, 6				; x64
		mov	esi, [ebx].m_ptrIQParamTable
		add	esi, ecx
		mov	ecx, edx
		and	edx, 01H
		sar	ecx, 1
		neg	edx
		add	ecx, 3
		;
		mov	nTemp[0], edx
		test	ecx, ecx
		.IF	SIGN?
			neg	ecx
			mov	edx, 64
			.REPEAT
				movzx	eax, BYTE PTR [esi]
				inc	esi
				inc	eax
				mov	ebx, eax
				sar	eax, 1
				and	eax, nTemp[0]
				add	eax, ebx
				sar	eax, cl
				mov	WORD PTR [edi], ax
				add	edi, 2
				dec	edx
			.UNTIL	ZERO?
			mov	ebx, _this
		.ELSE
			mov	edx, 64
			.REPEAT
				movzx	eax, BYTE PTR [esi]
				inc	esi
				inc	eax
				shl	eax, cl
				mov	ebx, eax
				sar	eax, 1
				and	eax, nTemp[0]
				add	eax, ebx
				mov	WORD PTR [edi], ax
				add	edi, 2
				dec	edx
			.UNTIL	ZERO?
			mov	ebx, _this
		.ENDIF
		;
		mov	ecx, iLoop
		inc	ecx
	.UNTIL	ecx >= 2
	;
	; 直流成分差分処理
	;
	.IF	[ebx].m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI
		mov	esi, ptrSrcData
		mov	al, SBYTE PTR [esi]
		add	SBYTE PTR [esi + 1], al
		add	SBYTE PTR [esi + 2], al
		add	SBYTE PTR [esi + 3], al
		;
		mov	ecx, 3
		add	esi, 6
		.IF	[ebx].m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4
			mov	ecx, 1
			sub	esi, 2
		.ENDIF
		.WHILE	ecx < [ebx].m_nChannelCount
			mov	al, SBYTE PTR [esi]
			add	SBYTE PTR [esi + 1], al
			add	SBYTE PTR [esi + 2], al
			add	SBYTE PTR [esi + 3], al
			add	esi, 4
			inc	ecx
		.ENDW
	.ENDIF
	;
	; インターリーブ
	;
	mov	esi, ptrSrcData
	lea	edi, buf[0]
	mov	edx, 64
	.REPEAT
		push	edi
		mov	ecx, [ebx].m_nBlocksetCount
		.REPEAT
			mov	al, SBYTE PTR [esi]
			inc	esi
			mov	SBYTE PTR [edi], al
			add	edi, 64
			dec	ecx
		.UNTIL	ZERO?
		pop	edi
		inc	edi
		dec	edx
	.UNTIL	ZERO?
	lea	esi, buf
	mov	ptrSrcData, esi
	;
	; 逆量子化＆ジグザグ走査
	;
	mov	eax, [ebx].m_ptrIQParamBuf
	lea	edx, [eax + (64*(SIZEOF WORD))]
	mov	pIQParam[00H], eax
	mov	pIQParam[04H], eax
	mov	pIQParam[08H], eax
	mov	pIQParam[0CH], eax
	;
	.IF	[ebx].m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4
		mov	ecx, 4
		.REPEAT
			mov	pIQParam[ecx*4], edx
			inc	ecx
		.UNTIL	ecx >= 12
	.ELSE
		mov	pIQParam[10H], edx
		mov	pIQParam[14H], edx
		mov	ecx, 6
	.ENDIF
	;
	.WHILE	ecx < [ebx].m_nBlocksetCount
		mov	pIQParam[ecx*4], eax
		inc	ecx
	.ENDW
	;
	xor	ecx, ecx
	.REPEAT
		mov	iLoop, ecx
		;
		; ソースを符号あり16ビット整数へ変換する
		; 数値は固定小数点(x8->x1)で表現する
		;
		mov	esi, ptrSrcData
		mov	edi, [ebx].m_ptrMatrixBuf
		mov	ebx, pIQParam[ecx * 4]
		ASSUME	ebx:NOTHING
		;
		.IF	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
			mov	ecx, 64 / 8
			.REPEAT
				movq	mm7, MMWORD PTR [esi]
				add	esi, 8
				punpcklbw	mm0, mm7
					punpckhbw	mm2, mm7
				psraw	mm0, 8
					psraw	mm2, 8
				pmullw	mm0, MMWORD PTR [ebx]
				pmullw	mm2, MMWORD PTR [ebx + 8]
				add	ebx, 16
				psraw	mm0, 3
				psraw	mm2, 3
				movq	MMWORD PTR [edi], mm0
				movq	MMWORD PTR [edi + 8], mm2
				add	edi, 16
				dec	ecx
			.UNTIL	ZERO?
		.ELSE
			mov	ecx, 64
			.REPEAT
				movsx	eax, SBYTE PTR [esi]
				movzx	edx, WORD PTR [ebx]
				inc	esi
				imul	eax, edx
				add	ebx, (SIZEOF WORD)
				sar	eax, 3
				add	eax, 8000H
				.IF	eax > 10000H
					sar	eax, 31
					not	eax
					and	eax, 0FFFFH
				.ENDIF
				sub	eax, 8000H
				mov	SWORD PTR [edi], ax
				add	edi, (SIZEOF SWORD)
				dec	ecx
			.UNTIL	ZERO?
		.ENDIF
		mov	ptrSrcData, esi
		;
		mov	ebx, _this
		ASSUME	ebx:PTR ERISADecoder
		mov	edx, iLoop
		mov	ecx, 64
		mov	esi, [ebx].m_ptrBlocksetBuf[edx*4]
		mov	edi, [ebx].m_ptrMatrixBuf
		mov	edx, [ebx].m_pArrangeTable[0]
		ASSUME	ebx:NOTHING
		.REPEAT
			mov	eax, DWORD PTR [edx]
			add	edx, (SIZEOF DWORD)
			mov	bx, SWORD PTR [edi]
			add	edi, (SIZEOF SWORD)
			mov	SWORD PTR [esi + eax * 2], bx
			dec	ecx
		.UNTIL	ZERO?
		;
		mov	ecx, iLoop
		mov	ebx, _this
		ASSUME	ebx:PTR ERISADecoder
		inc	ecx
	.UNTIL	ecx >= [ebx].m_nBlocksetCount

	ASSUME	ebx:NOTHING
	ret	2 * 4

ERISADecoder@@ArrangeAndIQuantumize	ENDP

;
;	逆 DCT 変換（低水準）
; ----------------------------------------------------------------------------
ALIGN	10H
eriFastIDCT8x8_SW	PROC	NEAR32 C PRIVATE USES ebx esi edi,
	ptrDst:PTR SWORD

	LOCAL	rTemp[72*2]:SWORD
	LOCAL	rDCTBuf1[64*4]:REAL4
	LOCAL	rDCTBuf2[64*4]:REAL4
	LOCAL	rWork[8*4]:REAL4

	@LINE_STEP = 8 * (SIZEOF SWORD)

	.IF	ERI_EnabledProcessorType & \
			(ERI_USE_MMX_PENTIUM OR ERI_USE_XMM_P3)
	;
	;	MMX/SSE 専用コード
	; --------------------------------------------------------------------
	test	(ERISADecoder PTR [ebx]).m_fdwDecFlags, dfQuickDecode
	jnz	Label_QuickDCT8x8
	;
	; 2次元 8x8 IDCT 変換
	;
	mov	ecx, 2
	lea	edi, rTemp[0]
	mov	esi, ptrDst
	add	edi, 0FH
	and	edi, NOT 0FH
	.REPEAT
		push	edi
		lea	ebx, mmxParam_IDCT8x8
		mov	edx, 8
		.REPEAT
			movq	mm6, MMWORD PTR [ebx]
			movq	mm7, MMWORD PTR [ebx + 08H]
			movq	mm0, MMWORD PTR [esi]
			movq	mm2, MMWORD PTR [esi + 10H]
				pmaddwd	mm0, mm6
			movq	mm1, MMWORD PTR [esi + 08H]
				pmaddwd	mm2, mm6
			movq	mm3, MMWORD PTR [esi + 18H]
				pmaddwd	mm1, mm7
			movq	mm4, MMWORD PTR [esi + 20H]
				pmaddwd	mm3, mm7
			movq	mm5, MMWORD PTR [esi + 28H]
			;
			add	ebx, 16
				pmaddwd	mm4, mm6
			paddd	mm0, mm1
				movq	mm1, MMWORD PTR [esi + 30H]
				pmaddwd	mm5, mm7
			paddd	mm2, mm3
				movq	mm3, MMWORD PTR [esi + 38H]
				pmaddwd	mm1, mm6
				paddd	mm4, mm5
				pmaddwd	mm3, mm7
			;
			movq	mm6, mm0
			punpckldq	mm0, mm2
				movq	mm7, mm4
				paddd	mm1, mm3
			punpckhdq	mm6, mm2
				punpckldq	mm4, mm1
				punpckhdq	mm7, mm1
			paddd	mm0, mm6
			paddd	mm4, mm7
			;
			psrad	mm0, 14
			psrad	mm4, 14
			packssdw	mm0, mm4
			movq	MMWORD PTR [edi], mm0
			add	edi, @LINE_STEP
			;
			dec	edx
		.UNTIL	ZERO?
		;
		pop	edi
		add	esi, 40H
		add	edi, 8
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	ecx, 2
	lea	esi, rTemp[0]
	mov	edi, ptrDst
	add	esi, 0FH
	and	esi, NOT 0FH
	.REPEAT
		push	edi
		lea	ebx, mmxParam_IDCT8x8
		mov	edx, 8
		.REPEAT
			movq	mm6, MMWORD PTR [ebx]
			movq	mm7, MMWORD PTR [ebx + 08H]
			movq	mm0, MMWORD PTR [esi]
			movq	mm2, MMWORD PTR [esi + 10H]
				pmaddwd	mm0, mm6
			movq	mm1, MMWORD PTR [esi + 08H]
				pmaddwd	mm2, mm6
			movq	mm3, MMWORD PTR [esi + 18H]
				pmaddwd	mm1, mm7
			movq	mm4, MMWORD PTR [esi + 20H]
				pmaddwd	mm3, mm7
			movq	mm5, MMWORD PTR [esi + 28H]
			;
			add	ebx, 16
				pmaddwd	mm4, mm6
			paddd	mm0, mm1
				movq	mm1, MMWORD PTR [esi + 30H]
				pmaddwd	mm5, mm7
			paddd	mm2, mm3
				movq	mm3, MMWORD PTR [esi + 38H]
				pmaddwd	mm1, mm6
				paddd	mm4, mm5
				pmaddwd	mm3, mm7
			;
			movq	mm6, mm0
			punpckldq	mm0, mm2
				movq	mm7, mm4
				paddd	mm1, mm3
			punpckhdq	mm6, mm2
				punpckldq	mm4, mm1
				punpckhdq	mm7, mm1
			paddd	mm0, mm6
			paddd	mm4, mm7
			;
			psrad	mm0, 16
			psrad	mm4, 16
			packssdw	mm0, mm4
			movq	MMWORD PTR [edi], mm0
			add	edi, @LINE_STEP
			;
			dec	edx
		.UNTIL	ZERO?
		;
		pop	edi
		add	esi, 40H
		add	edi, 8
		dec	ecx
	.UNTIL	ZERO?
	;
	ret

	ALIGN	10H
Label_QuickDCT8x8:
	;
	; 2次元 8x8 IDCT 変換（ローパス）
	;
	lea	edi, rTemp[0]
	mov	esi, ptrDst
	add	edi, 0FH
	and	edi, NOT 0FH
	lea	ebx, mmxParam_IDCT8x8
	mov	edx, 8
	.REPEAT
		movq	mm6, MMWORD PTR [ebx]
		movq	mm0, MMWORD PTR [esi]
		movq	mm2, MMWORD PTR [esi + 10H]
			pmaddwd	mm0, mm6
		movq	mm4, MMWORD PTR [esi + 20H]
			pmaddwd	mm2, mm6
		;
		movq	mm1, MMWORD PTR [esi + 30H]
			pmaddwd	mm4, mm6
		add	ebx, 16
			pmaddwd	mm1, mm6
		;
		movq	mm6, mm0
		punpckldq	mm0, mm2
			movq	mm7, mm4
		punpckhdq	mm6, mm2
			punpckldq	mm4, mm1
			punpckhdq	mm7, mm1
		paddd	mm0, mm6
		paddd	mm4, mm7
		;
		psrad	mm0, 14
		psrad	mm4, 14
		packssdw	mm0, mm4
		movq	MMWORD PTR [edi], mm0
		add	edi, @LINE_STEP
		;
		dec	edx
	.UNTIL	ZERO?
	;
	mov	ecx, 2
	lea	esi, rTemp[0]
	mov	edi, ptrDst
	add	esi, 0FH
	and	esi, NOT 0FH
	.REPEAT
		push	edi
		lea	ebx, mmxParam_IDCT8x8
		mov	edx, 8
		.REPEAT
			movq	mm6, MMWORD PTR [ebx]
			movq	mm0, MMWORD PTR [esi]
			movq	mm2, MMWORD PTR [esi + 10H]
				pmaddwd	mm0, mm6
			movq	mm4, MMWORD PTR [esi + 20H]
				pmaddwd	mm2, mm6
			;
			movq	mm1, MMWORD PTR [esi + 30H]
				pmaddwd	mm4, mm6
			add	ebx, 16
				pmaddwd	mm1, mm6
			;
			movq	mm6, mm0
			punpckldq	mm0, mm2
				movq	mm7, mm4
			punpckhdq	mm6, mm2
				punpckldq	mm4, mm1
				punpckhdq	mm7, mm1
			paddd	mm0, mm6
			paddd	mm4, mm7
			;
			psrad	mm0, 16
			psrad	mm4, 16
			packssdw	mm0, mm4
			movq	MMWORD PTR [edi], mm0
			add	edi, @LINE_STEP
			;
			dec	edx
		.UNTIL	ZERO?
		;
		pop	edi
		add	esi, 40H
		add	edi, 8
		dec	ecx
	.UNTIL	ZERO?
	;
	ret

	.ELSE
	;
	;	486 互換コード
	; --------------------------------------------------------------------
	;
	; 2次元 8x8 IDCT 変換
	;
	mov	ecx, 64
	lea	edi, rDCTBuf1[0]
	mov	esi, ptrDst
	.REPEAT
		fild	SWORD PTR [esi]
		add	esi, (SIZEOF SWORD)
		fstp	REAL4 PTR [edi]
		add	edi, (SIZEOF REAL4)
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	ecx, 8
	lea	edi, rDCTBuf2[0]
	lea	esi, rDCTBuf1[0]
	.REPEAT
		push	ecx
		INVOKE	eriFastIDCT , edi, esi, 8, ADDR rWork[0], 3
		pop	ecx
		add	esi, (SIZEOF REAL4)
		add	edi, 8 * (SIZEOF REAL4)
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	ecx, 8
	lea	edi, rDCTBuf1[0]
	lea	esi, rDCTBuf2[0]
	.REPEAT
		push	ecx
		INVOKE	eriFastIDCT , edi, esi, 8, ADDR rWork[0], 3
		pop	ecx
		add	esi, (SIZEOF REAL4)
		add	edi, 8 * (SIZEOF REAL4)
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	ecx, 64
	lea	esi, rDCTBuf1[0]
	mov	edi, ptrDst
	.REPEAT
		fld	REAL4 PTR [esi]
		fmul	ERI_rDiv4
		add	esi, (SIZEOF REAL4)
		fistp	SWORD PTR [edi]
		add	edi, (SIZEOF SWORD)
		dec	ecx
	.UNTIL	ZERO?

	.ENDIF

	ret

eriFastIDCT8x8_SW	ENDP

;
;	逆 DCT 変換
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@MatrixIDCT8x8	PROC	NEAR32 SYSCALL USES ebx esi edi,
	ptrVertBufLOT:PTR REAL4

	LOCAL	nChannelCount:DWORD
	LOCAL	nDCVal[4]:SDWORD

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder

	;
	; IDCT 変換
	;
	xor	esi, esi
	.REPEAT
		INVOKE	eriFastIDCT8x8_SW , [ebx].m_ptrBlocksetBuf[esi * 4]
		inc	esi
	.UNTIL	esi >= [ebx].m_nBlocksetCount

	ASSUME	ebx:NOTHING
	ret	4

ERISADecoder@@MatrixIDCT8x8	ENDP

;
;	逆 LOT 変換（低水準）
; ----------------------------------------------------------------------------
ALIGN	10H
eriFastILOT8x8_SW	PROC	NEAR32 C PRIVATE USES ebx esi edi,
	ptrDst:PTR SWORD, ptrHorzBuf:PTR SWORD, ptrVertBuf:PTR SWORD

	LOCAL	_this:PTR ERISADecoder
	LOCAL	rTemp[72*2]:SWORD
	LOCAL	rDCTBuf1[64*4]:REAL4
	LOCAL	rDCTBuf2[64*4]:REAL4
	LOCAL	rWork[8*4]:REAL4

	@LINE_STEP = 8 * (SIZEOF SWORD)
	mov	_this, ebx

	.IF	ERI_EnabledProcessorType & \
			(ERI_USE_MMX_PENTIUM OR ERI_USE_XMM_P3)
	;
	;	MMX/SSE 専用コード
	; --------------------------------------------------------------------
	;
	; 垂直方向重複変換
	;
	mov	edi, ptrDst
	mov	ecx, 2
	.REPEAT
		movq	mm0, MMWORD PTR [edi + @LINE_STEP * 5]
		movq	mm1, MMWORD PTR [edi + @LINE_STEP * 7]
		movq	mm6, mmxParam_ILOT_VERT[20H]
		movq	mm7, mmxParam_ILOT_VERT[28H]
		movq	mm2, mm0
		punpcklwd	mm0, mm1
		punpckhwd	mm2, mm1
		movq	mm1, mm0
		pmaddwd	mm0, mm7
		movq	mm3, mm2
		pmaddwd	mm2, mm7
		psrad	mm0, 15 - 2
		pmaddwd	mm1, mm6
		psrad	mm2, 15 - 2
		pmaddwd	mm3, mm6
		psrad	mm1, 15 - 4
		psrad	mm3, 15 - 4
		packssdw	mm0, mm2
		packssdw	mm1, mm3
		;
		movq	MMWORD PTR DWORD PTR [edi + @LINE_STEP * 7], mm0
		movq	mm0, MMWORD PTR [edi + @LINE_STEP * 3]
		movq	mm6, mmxParam_ILOT_VERT[10H]
		psllw	mm0, 4
		movq	mm7, mmxParam_ILOT_VERT[18H]
		movq	mm2, mm0
		punpcklwd	mm0, mm1
		punpckhwd	mm2, mm1
		movq	mm1, mm0
		pmaddwd	mm0, mm7
		movq	mm3, mm2
		pmaddwd	mm2, mm7
		psrad	mm0, 15 + 2
		pmaddwd	mm1, mm6
		psrad	mm2, 15 + 2
		pmaddwd	mm3, mm6
		psrad	mm1, 15
		psrad	mm3, 15
		packssdw	mm0, mm2
		packssdw	mm1, mm3
		;
		movq	MMWORD PTR DWORD PTR [edi + @LINE_STEP * 5], mm0
		movq	mm0, MMWORD PTR [edi + @LINE_STEP * 1]
		movq	mm6, mmxParam_ILOT_VERT[00H]
		psllw	mm0, 4
		movq	mm7, mmxParam_ILOT_VERT[08H]
		movq	mm2, mm0
		punpcklwd	mm0, mm1
		punpckhwd	mm2, mm1
		movq	mm1, mm0
		pmaddwd	mm0, mm7
		movq	mm3, mm2
		pmaddwd	mm2, mm7
		psrad	mm0, 15 + 2
		pmaddwd	mm1, mm6
		psrad	mm2, 15 + 2
		pmaddwd	mm3, mm6
		psrad	mm1, 15 + 2
		psrad	mm3, 15 + 2
		packssdw	mm0, mm2
		packssdw	mm1, mm3
		movq	MMWORD PTR DWORD PTR [edi + @LINE_STEP * 3], mm0
		movq	MMWORD PTR DWORD PTR [edi + @LINE_STEP * 1], mm1
		;
		add	edi, 8
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	edi, ptrDst
	mov	esi, ptrVertBuf
	mov	ecx, 4
	.REPEAT
		movq	mm0, MMWORD PTR [edi]
		movq	mm4, MMWORD PTR [edi + @LINE_STEP]
		movq	mm2, MMWORD PTR [edi + 8]
		psllw	mm0, 2
		movq	mm5, MMWORD PTR [edi + 8 + @LINE_STEP]
		psllw	mm2, 2
		movq	mm1, mm0
		paddsw	mm0, mm4
		movq	mm3, mm2
		paddsw	mm2, mm5
		psubsw	mm1, mm4
			movq	mm4, MMWORD PTR [esi]
		psubsw	mm3, mm5
		;
		movq	mm6, MMWORD PTR [esi + 8]
		movq	MMWORD PTR [esi], mm0
		;
		movq	MMWORD PTR [esi + @LINE_STEP], mm1
			movq	mm5, mm4
			paddsw	mm4, mm1
		movq	MMWORD PTR [esi + 8], mm2
			movq	mm7, mm6
			paddsw	mm6, mm3
		movq	MMWORD PTR [esi + 8 + @LINE_STEP], mm3
		;
		psubsw	mm5, mm1
			movq	MMWORD PTR [edi], mm4
		psubsw	mm7, mm3
			movq	MMWORD PTR [edi + 8], mm6
		movq	MMWORD PTR [edi + @LINE_STEP], mm5
		movq	MMWORD PTR [edi + 8 + @LINE_STEP], mm7
		add	esi, @LINE_STEP * 2
		add	edi, @LINE_STEP * 2
		;
		dec	ecx
	.UNTIL	ZERO?
	;
	; 水平方向重複変換
	;
	mov	edi, ptrDst
	mov	ecx, 4
	pcmpeqd	mm6, mm6
	pcmpeqd	mm7, mm7
	psrld	mm6, 16				; = 0000FFFF0000FFFF
	pslld	mm7, 16				; = FFFF0000FFFF0000
	.REPEAT
		movq	mm0, MMWORD PTR [edi + 8]		; mm0 = a7:a6:a5:a4
		movq	mm1, MMWORD PTR [edi + 8 + @LINE_STEP]	; mm1 = b7:b6:b5:b4
		movq	mm2, mm0
		psrad	mm0, 16
		movq	mm3, mm1
		psrad	mm1, 16
			pand	mm2, mm6			; mm2 = 0:a6:0:a4
		packssdw	mm0, mm1			; mm0 = b7:b5:a7:a5
			pand	mm3, mm6			; mm3 = 0:b6:0:b4
		movq	mm1, mm0
		pmaddwd	mm0, mmxParam_ILOT_HORZ[20H]		; mm0 = b5:xx:a5:xx
		movd	mm4, DWORD PTR [edi + 4]		; mm4 = 0:0:a3:a2
		pmaddwd	mm1, mmxParam_ILOT_HORZ[28H]		; mm1 = b7:a7
		movd	mm5, DWORD PTR [edi + 4 + @LINE_STEP]	; mm5 = 0:0:b3:b2
		pslld	mm0, 1
		psrad	mm1, 15
		punpckldq	mm4, mm0
		psrlq	mm0, 32
		psrad	mm4, 16
		punpckldq	mm5, mm0
		movq	mm0, MMWORD PTR [edi]			; mm0 = a3:a2:a1:a0
		psrad	mm5, 16
		packssdw	mm4, mm5			; mm4 = b5:b3:a5:a3
		movq	mm5, mm4
		pmaddwd	mm4, mmxParam_ILOT_HORZ[10H]		; mm4 = b3:xx:a3:xx
		pmaddwd	mm5, mmxParam_ILOT_HORZ[18H]		; mm5 = b5:a5
		pslld	mm4, 1
		psrad	mm5, 15
		packssdw	mm5, mm1			; mm5 = b7:a7:b5:a5
		movq	mm1, mm5
		pslld	mm5, 16
		pand	mm1, mm7
		por	mm5, mm2				; mm5 = a7:a6:a5:a4
		movq	mm2, MMWORD PTR [edi + @LINE_STEP]	; mm2 = b3:b2:b1:b0
		por	mm1, mm3				; mm1 = b7:b6:b5:b4
		movq	mm3, mm2
		movq	MMWORD PTR [edi + 8], mm5
		movq	MMWORD PTR [edi + 8 + @LINE_STEP], mm1
		movq	mm1, mm0
		punpckldq	mm0, mm4
		psrlq	mm4, 32
			pand	mm1, mm6			; mm1 = 0:a2:0:a0
		punpckldq	mm2, mm4
			pand	mm3, mm6			; mm3 = 0:b2:0:b0
		psrad	mm0, 16					; mm0 = a3:a1
		psrad	mm2, 16					; mm2 = b3:b1
		packssdw	mm0, mm2			; mm0 = b3:b1:a3:a1
		movq	mm2, mm0
		pmaddwd	mm0, mmxParam_ILOT_HORZ[00H]		; mm0 = b1:a1
		pmaddwd	mm2, mmxParam_ILOT_HORZ[08H]		; mm2 = b3:a3
		psrad	mm0, 15
		psrad	mm2, 15
		packssdw	mm0, mm2			; mm0 = b3:a3:b1:a1
		movq	mm2, mm0
		pslld	mm0, 16
		pand	mm2, mm7
		por	mm0, mm1				; mm0 = a3:a2:a1:a0
		por	mm2, mm3				; mm2 = b3:b2:b1:b0
		movq	MMWORD PTR [edi], mm0
		movq	MMWORD PTR [edi + @LINE_STEP], mm2
		add	edi, @LINE_STEP * 2
		;
		dec	ecx
	.UNTIL	ZERO?
	;
	test	ERI_EnabledProcessorType, ERI_USE_XMM_P3
	jz	Label_ILOT_Horz486
	mov	edi, ptrDst
	mov	esi, ptrHorzBuf
	mov	ecx, 64 / 8
	.REPEAT
		movq	mm0, MMWORD PTR [edi]
		movq	mm1, MMWORD PTR [edi + 8]
		pshufw	mm0, mm0, 11011000B
				movq	mm4, MMWORD PTR [esi]
		pshufw	mm1, mm1, 11011000B
		movq	mm2, mm0
				movq	mm5, MMWORD PTR [esi + 8]
		punpckldq	mm0, mm1
		punpckhdq	mm2, mm1
		;
		movq	mm1, mm0
				pshufw	mm4, mm4, 11011000B
		paddsw	mm0, mm2
				pshufw	mm5, mm5, 11011000B
		psubsw	mm1, mm2
		;
			punpckldq	mm4, mm5
		;
		movq	mm2, mm0
		punpcklwd	mm0, mm1
			movq	mm5, mm4
			paddsw	mm4, mm1
		punpckhwd	mm2, mm1
			psubsw	mm5, mm1
		movq	MMWORD PTR [esi], mm0
			movq	mm6, mm4
		movq	MMWORD PTR [esi + 8], mm2
		;
		punpcklwd	mm4, mm5
		punpckhwd	mm6, mm5
		psraw	mm4, 1
		psraw	mm6, 1
		add	esi, 16
		movq	MMWORD PTR [edi], mm4
		movq	MMWORD PTR [edi + 8], mm6
		add	edi, 16
		;
		dec	ecx
	.UNTIL	ZERO?

	.ELSE
	;
	;	486 互換コード
	; --------------------------------------------------------------------
	;
	; 垂直方向重複変換
	;
	mov	edi, ptrDst
	mov	ecx, 8
	.REPEAT
		movsx	eax, SWORD PTR [edi + @LINE_STEP * 5]
		movsx	edx, SWORD PTR [edi + @LINE_STEP * 7]
		mov	ebx, eax
		imul	eax, nParam_ILOT[14H]
		mov	esi, edx
		imul	edx, nParam_ILOT[10H]
		imul	ebx, nParam_ILOT[10H]
		add	eax, edx
		imul	esi, nParam_ILOT[14H]
		sar	eax, 15 - 4
		sub	esi, ebx
		sar	esi, 15 - 2
		mov	SWORD PTR [edi + @LINE_STEP * 7], si
		;
		mov	edx, eax
		movsx	eax, SWORD PTR [edi + @LINE_STEP * 3]
		shl	eax, 4
		mov	ebx, eax
		imul	eax, nParam_ILOT[0CH]
		mov	esi, edx
		imul	edx, nParam_ILOT[08H]
		imul	ebx, nParam_ILOT[08H]
		add	eax, edx
		imul	esi, nParam_ILOT[0CH]
		sar	eax, 15
		sub	esi, ebx
		sar	esi, 15 + 2
		mov	SWORD PTR [edi + @LINE_STEP * 5], si
		;
		mov	edx, eax
		movsx	eax, SWORD PTR [edi + @LINE_STEP]
		shl	eax, 4
		mov	ebx, eax
		imul	eax, nParam_ILOT[04H]
		mov	esi, edx
		imul	edx, nParam_ILOT[00H]
		imul	ebx, nParam_ILOT[00H]
		add	eax, edx
		imul	esi, nParam_ILOT[04H]
		sar	eax, 15 + 2
		sub	esi, ebx
		sar	esi, 15 + 2
		mov	SWORD PTR [edi + @LINE_STEP], ax
		mov	SWORD PTR [edi + @LINE_STEP * 3], si
		;
		add	edi, (SIZEOF SWORD)
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	edi, ptrDst
	mov	esi, ptrVertBuf
	mov	ecx, 4
	.REPEAT
		push	ecx
		;
		mov	ecx, 8
		.REPEAT
			movsx	eax, SWORD PTR [edi]
			movsx	edx, SWORD PTR [edi + @LINE_STEP]
			shl	eax, 2
			mov	ebx, eax
			add	eax, edx
			sub	ebx, edx
			;
			movsx	edx, SWORD PTR [esi]
			mov	SWORD PTR [esi], ax
			mov	eax, edx
			mov	SWORD PTR [esi + @LINE_STEP], bx
			;
			add	edx, ebx
			sub	eax, ebx
			add	esi, (SIZEOF SWORD)
			mov	SWORD PTR [edi], dx
			mov	SWORD PTR [edi + @LINE_STEP], ax
			add	edi, (SIZEOF SWORD)
			;
			dec	ecx
		.UNTIL	ZERO?
		;
		pop	ecx
		add	esi, @LINE_STEP
		add	edi, @LINE_STEP
		dec	ecx
	.UNTIL	ZERO?
	;
	; 水平方向重複変換
	;
	mov	edi, ptrDst
	mov	ecx, 8
	.REPEAT
		movsx	eax, SWORD PTR [edi + (SIZEOF SWORD) * 5]
		movsx	edx, SWORD PTR [edi + (SIZEOF SWORD) * 7]
		mov	ebx, eax
		imul	eax, nParam_ILOT[14H]
		mov	esi, edx
		imul	edx, nParam_ILOT[10H]
		imul	ebx, nParam_ILOT[10H]
		add	eax, edx
		imul	esi, nParam_ILOT[14H]
		sar	eax, 15
		sub	esi, ebx
		sar	esi, 15
		mov	SWORD PTR [edi + (SIZEOF SWORD) * 7], si
		;
		mov	edx, eax
		movsx	eax, SWORD PTR [edi + (SIZEOF SWORD) * 3]
		mov	ebx, eax
		imul	eax, nParam_ILOT[0CH]
		mov	esi, edx
		imul	edx, nParam_ILOT[08H]
		imul	ebx, nParam_ILOT[08H]
		add	eax, edx
		imul	esi, nParam_ILOT[0CH]
		sar	eax, 15
		sub	esi, ebx
		sar	esi, 15
		mov	SWORD PTR [edi + (SIZEOF SWORD) * 5], si
		;
		mov	edx, eax
		movsx	eax, SWORD PTR [edi + (SIZEOF SWORD)]
		mov	ebx, eax
		imul	eax, nParam_ILOT[04H]
		mov	esi, edx
		imul	edx, nParam_ILOT[00H]
		imul	ebx, nParam_ILOT[00H]
		add	eax, edx
		imul	esi, nParam_ILOT[04H]
		sar	eax, 15
		sub	esi, ebx
		sar	esi, 15
		mov	SWORD PTR [edi + (SIZEOF SWORD)], ax
		mov	SWORD PTR [edi + (SIZEOF SWORD) * 3], si
		;
		add	edi, @LINE_STEP
		dec	ecx
	.UNTIL	ZERO?
	;
Label_ILOT_Horz486:
	mov	edi, ptrDst
	mov	esi, ptrHorzBuf
	mov	ecx, 64 / 2
	.REPEAT
		movsx	eax, SWORD PTR [edi]
		movsx	edx, SWORD PTR [edi + 2]
		mov	ebx, eax
		add	eax, edx
		sub	ebx, edx
		;
		movsx	edx, SWORD PTR [esi]
		mov	SWORD PTR [esi], ax
		mov	eax, edx
		mov	SWORD PTR [esi + 2], bx
		;
		add	edx, ebx
		sub	eax, ebx
		add	esi, 2 * (SIZEOF SWORD)
		sar	edx, 1
		sar	eax, 1
		mov	SWORD PTR [edi], dx
		mov	SWORD PTR [edi + 2], ax
		add	edi, 2 * (SIZEOF SWORD)
		;
		dec	ecx
	.UNTIL	ZERO?

	.ENDIF

	mov	eax, _this
	.IF	ERI_EnabledProcessorType & \
			(ERI_USE_MMX_PENTIUM OR ERI_USE_XMM_P3)
	test	(ERISADecoder PTR [eax]).m_fdwDecFlags, dfQuickDecode
	jnz	Label_QuickDCT8x8
	;
	;	MMX/SSE 専用コード
	; --------------------------------------------------------------------
	;
	; 2次元 8x8 IDCT 変換
	;
	mov	ecx, 2
	lea	edi, rTemp[0]
	mov	esi, ptrDst
	add	edi, 0FH
	and	edi, NOT 0FH
	.REPEAT
		push	edi
		lea	ebx, mmxParam_IDCT8x8
		mov	edx, 8
		.REPEAT
			movq	mm6, MMWORD PTR [ebx]
			movq	mm7, MMWORD PTR [ebx + 08H]
			movq	mm0, MMWORD PTR [esi]
			movq	mm2, MMWORD PTR [esi + 10H]
				pmaddwd	mm0, mm6
			movq	mm1, MMWORD PTR [esi + 08H]
				pmaddwd	mm2, mm6
			movq	mm3, MMWORD PTR [esi + 18H]
				pmaddwd	mm1, mm7
			movq	mm4, MMWORD PTR [esi + 20H]
				pmaddwd	mm3, mm7
			movq	mm5, MMWORD PTR [esi + 28H]
			;
			add	ebx, 16
				pmaddwd	mm4, mm6
			paddd	mm0, mm1
				movq	mm1, MMWORD PTR [esi + 30H]
				pmaddwd	mm5, mm7
			paddd	mm2, mm3
				movq	mm3, MMWORD PTR [esi + 38H]
				pmaddwd	mm1, mm6
				paddd	mm4, mm5
				pmaddwd	mm3, mm7
			;
			movq	mm6, mm0
			punpckldq	mm0, mm2
				movq	mm7, mm4
				paddd	mm1, mm3
			punpckhdq	mm6, mm2
				punpckldq	mm4, mm1
				punpckhdq	mm7, mm1
			paddd	mm0, mm6
			paddd	mm4, mm7
			;
			psrad	mm0, 14 + 1 + 1
			psrad	mm4, 14 + 1 + 1
			packssdw	mm0, mm4
			movq	MMWORD PTR [edi], mm0
			add	edi, @LINE_STEP
			;
			dec	edx
		.UNTIL	ZERO?
		;
		pop	edi
		add	esi, 40H
		add	edi, 8
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	ecx, 2
	lea	esi, rTemp[0]
	mov	edi, ptrDst
	add	esi, 0FH
	and	esi, NOT 0FH
	.REPEAT
		push	edi
		lea	ebx, mmxParam_IDCT8x8
		mov	edx, 8
		.REPEAT
			movq	mm6, MMWORD PTR [ebx]
			movq	mm7, MMWORD PTR [ebx + 08H]
			movq	mm0, MMWORD PTR [esi]
			movq	mm2, MMWORD PTR [esi + 10H]
				pmaddwd	mm0, mm6
			movq	mm1, MMWORD PTR [esi + 08H]
				pmaddwd	mm2, mm6
			movq	mm3, MMWORD PTR [esi + 18H]
				pmaddwd	mm1, mm7
			movq	mm4, MMWORD PTR [esi + 20H]
				pmaddwd	mm3, mm7
			movq	mm5, MMWORD PTR [esi + 28H]
			;
			add	ebx, 16
				pmaddwd	mm4, mm6
			paddd	mm0, mm1
				movq	mm1, MMWORD PTR [esi + 30H]
				pmaddwd	mm5, mm7
			paddd	mm2, mm3
				movq	mm3, MMWORD PTR [esi + 38H]
				pmaddwd	mm1, mm6
				paddd	mm4, mm5
				pmaddwd	mm3, mm7
			;
			movq	mm6, mm0
			punpckldq	mm0, mm2
				movq	mm7, mm4
				paddd	mm1, mm3
			punpckhdq	mm6, mm2
				punpckldq	mm4, mm1
				punpckhdq	mm7, mm1
			paddd	mm0, mm6
			paddd	mm4, mm7
			;
			psrad	mm0, 16 + 1
			psrad	mm4, 16 + 1
			packssdw	mm0, mm4
			movq	MMWORD PTR [edi], mm0
			add	edi, @LINE_STEP
			;
			dec	edx
		.UNTIL	ZERO?
		;
		pop	edi
		add	esi, 40H
		add	edi, 8
		dec	ecx
	.UNTIL	ZERO?
	;
	ret

	ALIGN	10H
Label_QuickDCT8x8:
	;
	; 2次元 8x8 IDCT 変換（ローパス）
	;
	lea	edi, rTemp[0]
	mov	esi, ptrDst
	add	edi, 0FH
	and	edi, NOT 0FH
	lea	ebx, mmxParam_IDCT8x8
	mov	edx, 8
	.REPEAT
		movq	mm6, MMWORD PTR [ebx]
		movq	mm0, MMWORD PTR [esi]
		movq	mm2, MMWORD PTR [esi + 10H]
			pmaddwd	mm0, mm6
		movq	mm4, MMWORD PTR [esi + 20H]
			pmaddwd	mm2, mm6
		;
		movq	mm1, MMWORD PTR [esi + 30H]
			pmaddwd	mm4, mm6
		add	ebx, 16
			pmaddwd	mm1, mm6
		;
		movq	mm6, mm0
		punpckldq	mm0, mm2
			movq	mm7, mm4
		punpckhdq	mm6, mm2
			punpckldq	mm4, mm1
			punpckhdq	mm7, mm1
		paddd	mm0, mm6
		paddd	mm4, mm7
		;
		psrad	mm0, 14 + 1 + 1
		psrad	mm4, 14 + 1 + 1
		packssdw	mm0, mm4
		movq	MMWORD PTR [edi], mm0
		add	edi, @LINE_STEP
		;
		dec	edx
	.UNTIL	ZERO?
	;
	mov	ecx, 2
	lea	esi, rTemp[0]
	mov	edi, ptrDst
	add	esi, 0FH
	and	esi, NOT 0FH
	.REPEAT
		push	edi
		lea	ebx, mmxParam_IDCT8x8
		mov	edx, 8
		.REPEAT
			movq	mm6, MMWORD PTR [ebx]
			movq	mm0, MMWORD PTR [esi]
			movq	mm2, MMWORD PTR [esi + 10H]
				pmaddwd	mm0, mm6
			movq	mm4, MMWORD PTR [esi + 20H]
				pmaddwd	mm2, mm6
			;
			movq	mm1, MMWORD PTR [esi + 30H]
				pmaddwd	mm4, mm6
			add	ebx, 16
				pmaddwd	mm1, mm6
			;
			movq	mm6, mm0
			punpckldq	mm0, mm2
				movq	mm7, mm4
			punpckhdq	mm6, mm2
				punpckldq	mm4, mm1
				punpckhdq	mm7, mm1
			paddd	mm0, mm6
			paddd	mm4, mm7
			;
			psrad	mm0, 16 + 1
			psrad	mm4, 16 + 1
			packssdw	mm0, mm4
			movq	MMWORD PTR [edi], mm0
			add	edi, @LINE_STEP
			;
			dec	edx
		.UNTIL	ZERO?
		;
		pop	edi
		add	esi, 40H
		add	edi, 8
		dec	ecx
	.UNTIL	ZERO?
	;
	ret

	.ELSE
	;
	;	486 互換コード
	; --------------------------------------------------------------------
	;
	; 2次元 8x8 IDCT 変換
	;
	mov	ecx, 64
	lea	edi, rDCTBuf1[0]
	mov	esi, ptrDst
	.REPEAT
		fild	SWORD PTR [esi]
		add	esi, (SIZEOF SWORD)
		fstp	REAL4 PTR [edi]
		add	edi, (SIZEOF REAL4)
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	ecx, 8
	lea	edi, rDCTBuf2[0]
	lea	esi, rDCTBuf1[0]
	.REPEAT
		push	ecx
		INVOKE	eriFastIDCT , edi, esi, 8, ADDR rWork[0], 3
		pop	ecx
		add	esi, (SIZEOF REAL4)
		add	edi, 8 * (SIZEOF REAL4)
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	ecx, 8
	lea	edi, rDCTBuf1[0]
	lea	esi, rDCTBuf2[0]
	.REPEAT
		push	ecx
		INVOKE	eriFastIDCT , edi, esi, 8, ADDR rWork[0], 3
		pop	ecx
		add	esi, (SIZEOF REAL4)
		add	edi, 8 * (SIZEOF REAL4)
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	ecx, 64
	lea	esi, rDCTBuf1[0]
	mov	edi, ptrDst
	.REPEAT
		fld	REAL4 PTR [esi]
		fmul	ERI_rDiv32
		add	esi, (SIZEOF REAL4)
		fistp	SWORD PTR [edi]
		add	edi, (SIZEOF SWORD)
		dec	ecx
	.UNTIL	ZERO?

	.ENDIF

	ret

eriFastILOT8x8_SW	ENDP

;
;	逆 LOT 変換
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@MatrixILOT8x8	PROC	NEAR32 SYSCALL USES ebx esi edi,
	ptrVertBufLOT:PTR REAL4

	LOCAL	ptrHorzBufLOT:PTR SWORD

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder

	;
	; 輝度チャネルを処理
	;
	xor	esi, esi
	mov	eax, [ebx].m_ptrHorzBufLOT
	mov	ecx, 2
	mov	ptrHorzBufLOT, eax
	.REPEAT
		push	ecx
		;
		mov	ecx, 2
		mov	eax, ptrVertBufLOT
		.REPEAT
			push	ecx
			push	eax
			INVOKE	eriFastILOT8x8_SW ,
				[ebx].m_ptrBlocksetBuf[esi*4],
				ptrHorzBufLOT, eax
			mov	edx, [ebx].m_nBlockArea
			pop	eax
			pop	ecx
			inc	esi
			lea	eax, [eax + edx * 2]
			dec	ecx
		.UNTIL	ZERO?
		;
		mov	eax, ptrHorzBufLOT
		mov	edx, [ebx].m_nBlockArea
		pop	ecx
		lea	eax, [eax + edx * 2]
		mov	ptrHorzBufLOT, eax
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	eax, ptrVertBufLOT
	mov	edx, [ebx].m_nBlockArea
	lea	eax, [eax + edx * 4]
	mov	ptrVertBufLOT, eax
	;
	; 色差チャネルを処理
	;
	cmp	[ebx].m_nChannelCount, 3
	jb	Label_Exit
	;
	.IF	[ebx].m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4
		mov	ecx, 2
		.REPEAT
			push	ecx
			;
			mov	ecx, 2
			.REPEAT
				push	ecx
				;
				mov	ecx, 2
				mov	eax, ptrVertBufLOT
				.REPEAT
					push	ecx
					push	eax
					INVOKE	eriFastILOT8x8_SW ,
						[ebx].m_ptrBlocksetBuf[esi*4],
						ptrHorzBufLOT, eax
					mov	edx, [ebx].m_nBlockArea
					pop	eax
					pop	ecx
					inc	esi
					lea	eax, [eax + edx * 2]
					dec	ecx
				.UNTIL	ZERO?
				;
				mov	eax, ptrHorzBufLOT
				mov	edx, [ebx].m_nBlockArea
				pop	ecx
				lea	eax, [eax + edx * 2]
				mov	ptrHorzBufLOT, eax
				dec	ecx
			.UNTIL	ZERO?
			;
			mov	eax, ptrVertBufLOT
			mov	edx, [ebx].m_nBlockArea
			lea	eax, [eax + edx * 4]
			mov	ptrVertBufLOT, eax
			;
			pop	ecx
			dec	ecx
		.UNTIL	ZERO?
	.ELSEIF	[ebx].m_eihInfo.dwSamplingFlags == ERISF_YUV_4_1_1
		mov	ecx, 2
		.REPEAT
			push	ecx
			INVOKE	eriFastILOT8x8_SW ,
				[ebx].m_ptrBlocksetBuf[esi*4],
				ptrHorzBufLOT, ptrVertBufLOT
			mov	edx, [ebx].m_nBlockArea
			pop	ecx
			shl	edx, 1
			inc	esi
			add	ptrHorzBufLOT, edx
			add	ptrVertBufLOT, edx
			dec	ecx
		.UNTIL	ZERO?
	.ELSE
		jmp	Label_Exit
	.ENDIF
	;
	; αチャネルを処理
	;
	cmp	[ebx].m_nChannelCount, 4
	jb	Label_Exit
	;
	mov	ecx, 2
	.REPEAT
		push	ecx
		;
		mov	ecx, 2
		mov	eax, ptrVertBufLOT
		.REPEAT
			push	ecx
			push	eax
			INVOKE	eriFastILOT8x8_SW ,
				[ebx].m_ptrBlocksetBuf[esi*4],
				ptrHorzBufLOT, eax
			mov	edx, [ebx].m_nBlockArea
			pop	eax
			pop	ecx
			inc	esi
			lea	eax, [eax + edx * 2]
			dec	ecx
		.UNTIL	ZERO?
		;
		mov	eax, ptrHorzBufLOT
		mov	edx, [ebx].m_nBlockArea
		pop	ecx
		lea	eax, [eax + edx * 2]
		mov	ptrHorzBufLOT, eax
		dec	ecx
	.UNTIL	ZERO?
	;
	mov	eax, ptrVertBufLOT
	mov	edx, [ebx].m_nBlockArea
	lea	eax, [eax + edx * 4]
	mov	ptrVertBufLOT, eax

Label_Exit:
	ASSUME	ebx:NOTHING
	ret	4

ERISADecoder@@MatrixILOT8x8	ENDP

;
;	チャネルデータ出力関数
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@StoreYUVImageChannel	\
	PROC	NEAR32 C PRIVATE USES ebx esi edi,
		xBlock:SDWORD, yBlock:SDWORD, iChannel:SDWORD

	LOCAL	nYUVLineBytes:DWORD

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder
	;
	mov	eax, [ebx].m_nYUVLineBytes
	mov	nYUVLineBytes, eax
	mov	eax, xBlock
	mov	edx, yBlock
	shl	eax, 3
	shl	edx, 3
	imul	eax, [ebx].m_nYUVPixelBytes
	imul	edx, [ebx].m_nYUVLineBytes
	mov	edi, [ebx].m_ptrYUVImage
	add	eax, edx
	add	edi, eax
	mov	esi, [ebx].m_ptrDecodeBuf
	add	edi, iChannel
	;
	mov	edx, 8
	mov	ecx, [ebx].m_nYUVPixelBytes
	ASSUME	ebx:NOTHING
	;
	.REPEAT
		push	edx
		push	edi
		lea	edx, [ecx + ecx * 2]
		;
		mov	al, SBYTE PTR [esi]
		mov	ah, SBYTE PTR [esi + 1]
		mov	bl, SBYTE PTR [esi + 2]
		mov	bh, SBYTE PTR [esi + 3]
		mov	SBYTE PTR [edi], al
		mov	SBYTE PTR [edi + ecx], ah
		mov	SBYTE PTR [edi + ecx * 2], bl
		mov	SBYTE PTR [edi + edx], bh
		lea	edi, [edi + ecx * 4]
		;
		mov	al, SBYTE PTR [esi + 4]
		mov	ah, SBYTE PTR [esi + 5]
		mov	bl, SBYTE PTR [esi + 6]
		mov	bh, SBYTE PTR [esi + 7]
		add	esi, 8
		mov	SBYTE PTR [edi], al
		mov	SBYTE PTR [edi + ecx], ah
		mov	SBYTE PTR [edi + ecx * 2], bl
		mov	SBYTE PTR [edi + edx], bh
		;
		mov	eax, nYUVLineBytes
		pop	edi
		pop	edx
		add	edi, eax
		dec	edx
	.UNTIL	ZERO?

	ret

ERISADecoder@@StoreYUVImageChannel	ENDP

@ERISADecoder@@StoreYUVImageChannel_SSE	MACRO	sse_flag:REQ

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder
	;
	mov	eax, [ebx].m_nYUVLineBytes
	mov	nYUVLineBytes, eax
	mov	eax, xBlock
	mov	edx, yBlock
	shl	eax, 3
	shl	edx, 3
	imul	eax, [ebx].m_nYUVPixelBytes
	imul	edx, [ebx].m_nYUVLineBytes
	mov	edi, [ebx].m_ptrYUVImage
	add	eax, edx
	add	edi, eax
	mov	eax, iChannel
	mov	esi, [ebx].m_ptrDecodeBuf
	lea	edi, [edi + eax * 4]
	;
	mov	edx, 8
	mov	ecx, [ebx].m_nYUVPixelBytes
	ASSUME	ebx:NOTHING
	mov	eax, nYUVLineBytes
	shl	ecx, 2
	;
	.REPEAT
		movq	mm0, MMWORD PTR [esi]
		add	esi, 8
		movd	DWORD PTR [edi], mm0
		psrlq	mm0, 32
		movd	DWORD PTR [edi + ecx], mm0
		;
		add	edi, eax
		dec	edx
	.UNTIL	ZERO?

	ret

ENDM

ALIGN	10H
ERISADecoder@@StoreYUVImageChannel_MMX	\
	PROC	NEAR32 C PRIVATE USES ebx esi edi,
		xBlock:SDWORD, yBlock:SDWORD, iChannel:SDWORD

	LOCAL	nYUVLineBytes:DWORD

	@ERISADecoder@@StoreYUVImageChannel_SSE	0

ERISADecoder@@StoreYUVImageChannel_MMX	ENDP

ALIGN	10H
ERISADecoder@@StoreYUVImageChannel_SSE	\
	PROC	NEAR32 C PRIVATE USES ebx esi edi,
		xBlock:SDWORD, yBlock:SDWORD, iChannel:SDWORD

	LOCAL	nYUVLineBytes:DWORD

	@ERISADecoder@@StoreYUVImageChannel_SSE	1

ERISADecoder@@StoreYUVImageChannel_SSE	ENDP

ALIGN	10H
ERISADecoder@@StoreYUVImageChannelX2	\
	PROC	NEAR32 C PRIVATE USES ebx esi edi,
		xBlock:SDWORD, yBlock:SDWORD, iChannel:SDWORD

	LOCAL	nYUVLineBytes:DWORD

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder
	;
	mov	eax, [ebx].m_nYUVLineBytes
	mov	nYUVLineBytes, eax
	mov	eax, xBlock
	mov	edx, yBlock
	shl	eax, 3 + 1
	shl	edx, 3 + 1
	imul	eax, [ebx].m_nYUVPixelBytes
	imul	edx, [ebx].m_nYUVLineBytes
	mov	edi, [ebx].m_ptrYUVImage
	add	eax, edx
	add	edi, eax
	mov	esi, [ebx].m_ptrDecodeBuf
	add	edi, iChannel
	;
	mov	edx, 8
	mov	ecx, [ebx].m_nYUVPixelBytes
	ASSUME	ebx:NOTHING
	;
	.REPEAT
		push	edx
		push	edi
		lea	edx, [ecx + ecx * 2]
		mov	ebx, nYUVLineBytes
		add	ebx, edi
		;
		@INDEX = 0
		REPEAT	4
			mov	al, SBYTE PTR [esi + @INDEX]
			mov	ah, SBYTE PTR [esi + @INDEX + 1]
			mov	SBYTE PTR [edi], al
			mov	SBYTE PTR [ebx], al
			mov	SBYTE PTR [edi + ecx], al
			mov	SBYTE PTR [ebx + ecx], al
			mov	SBYTE PTR [edi + ecx * 2], ah
			mov	SBYTE PTR [ebx + ecx * 2], ah
			mov	SBYTE PTR [edi + edx], ah
			mov	SBYTE PTR [ebx + edx], ah
			lea	edi, [edi + ecx * 4]
			lea	ebx, [ebx + ecx * 4]
			@INDEX = @INDEX + 2
		ENDM
		;
		mov	eax, nYUVLineBytes
		pop	edi
		pop	edx
		add	esi, 8
		lea	edi, [edi + eax * 2]
		dec	edx
	.UNTIL	ZERO?

	ret

ERISADecoder@@StoreYUVImageChannelX2	ENDP

@ERISADecoder@@StoreYUVImageChannelX2_SSE	MACRO	sse_flag:REQ

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder
	;
	mov	eax, [ebx].m_nYUVLineBytes
	mov	nYUVLineBytes, eax
	mov	eax, xBlock
	mov	edx, yBlock
	shl	eax, 3 + 1
	shl	edx, 3 + 1
	imul	eax, [ebx].m_nYUVPixelBytes
	imul	edx, [ebx].m_nYUVLineBytes
	mov	edi, [ebx].m_ptrYUVImage
	add	eax, edx
	add	edi, eax
	mov	esi, [ebx].m_ptrDecodeBuf
	mov	eax, iChannel
	lea	edi, [edi + eax * 4]
	;
	mov	edx, 8
	mov	ecx, [ebx].m_nYUVPixelBytes
	ASSUME	ebx:NOTHING
	shl	ecx, 2
	;
	.REPEAT
		push	edx
		mov	eax, nYUVLineBytes
		lea	edx, [ecx + ecx * 2]
		lea	ebx, [edi + eax]
		;
		movq	mm0, MMWORD PTR [esi]
		add	esi, 8
		movq	mm2, mm0
		IF	sse_flag
			prefetcht1	[ebx]
		ENDIF
		punpcklbw	mm0, mm0
		punpckhbw	mm2, mm2
		movq	mm1, mm0
		movq	mm3, mm2
		IF	sse_flag
			prefetcht1	[ebx + 32]
		ENDIF
		psrlq	mm1, 32
		psrlq	mm3, 32
		movd	DWORD PTR [edi], mm0
		movd	DWORD PTR [edi + ecx], mm1
		movd	DWORD PTR [edi + ecx * 2], mm2
		movd	DWORD PTR [edi + edx], mm3
		IF	sse_flag
			prefetcht1	[ebx + eax]
		ENDIF
		movd	DWORD PTR [ebx], mm0
		movd	DWORD PTR [ebx + ecx], mm1
		IF	sse_flag
			prefetcht1	[ebx + eax + 32]
		ENDIF
		movd	DWORD PTR [ebx + ecx * 2], mm2
		movd	DWORD PTR [ebx + edx], mm3
		;
		pop	edx
		lea	edi, [edi + eax * 2]
		dec	edx
	.UNTIL	ZERO?

	ret

ENDM

ALIGN	10H
ERISADecoder@@StoreYUVImageChannelX2_MMX	\
	PROC	NEAR32 C PRIVATE USES ebx esi edi,
		xBlock:SDWORD, yBlock:SDWORD, iChannel:SDWORD

	LOCAL	nYUVLineBytes:DWORD

	@ERISADecoder@@StoreYUVImageChannelX2_SSE	1

ERISADecoder@@StoreYUVImageChannelX2_MMX	ENDP

ALIGN	10H
ERISADecoder@@StoreYUVImageChannelX2_SSE	\
	PROC	NEAR32 C PRIVATE USES ebx esi edi,
		xBlock:SDWORD, yBlock:SDWORD, iChannel:SDWORD

	LOCAL	nYUVLineBytes:DWORD

	@ERISADecoder@@StoreYUVImageChannelX2_SSE	1

ERISADecoder@@StoreYUVImageChannelX2_SSE	ENDP

FUNC_StoreYUVImageChannel	TYPEDEF	PROTO	NEAR32 C,
	xBlock:SDWORD, yBlock:SDWORD, iChannel:SDWORD
PFUNC_StoreYUVImageChannel	TYPEDEF	PTR FUNC_StoreYUVImageChannel


;
;	サブブロックデータ飽和処理関数
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@ConvertSWordToSByte_486	PROC	NEAR32 C PRIVAE

	mov	ecx, 64 / 2
	.REPEAT
		movsx	eax, SWORD PTR [esi]
		movsx	edx, SWORD PTR [esi + 2]
		add	esi, 4
		;
		add	eax, 80H
		add	edx, 80H
		.IF	(DWORD PTR eax) > 0FFH
			sar	eax, 31
			not	eax
		.ENDIF
		.IF	(DWORD PTR edx) > 0FFH
			sar	edx, 31
			not	edx
		.ENDIF
		;
		sub	al, 80H
		sub	dl, 80H
		mov	SBYTE PTR [edi], al
		mov	SBYTE PTR [edi + 1], dl
		add	edi, 2
		;
		dec	ecx
	.UNTIL	ZERO?
	ret

ERISADecoder@@ConvertSWordToSByte_486	ENDP

ALIGN	10H
ERISADecoder@@ConvertSWordToByte_486	PROC	NEAR32 C PRIVAE

	mov	ecx, 64 / 2
	.REPEAT
		movsx	eax, SWORD PTR [esi]
		movsx	edx, SWORD PTR [esi + 2]
		add	esi, 4
		;
		.IF	(DWORD PTR eax) > 0FFH
			sar	eax, 31
			not	eax
		.ENDIF
		.IF	(DWORD PTR edx) > 0FFH
			sar	edx, 31
			not	edx
		.ENDIF
		;
		mov	SBYTE PTR [edi], al
		mov	SBYTE PTR [edi + 1], dl
		add	edi, 2
		;
		dec	ecx
	.UNTIL	ZERO?
	ret

ERISADecoder@@ConvertSWordToByte_486	ENDP

ALIGN	10H
ERISADecoder@@ConvertSWordToSByte_MMX	PROC	NEAR32 C PRIVATE

	mov	ecx, 64 / 16
	.REPEAT
		movq	mm0, MMWORD PTR [esi]
		movq	mm1, MMWORD PTR [esi + 8]
		movq	mm2, MMWORD PTR [esi + 16]
		movq	mm3, MMWORD PTR [esi + 24]
		add	esi, 32
		packsswb	mm0, mm1
		packsswb	mm2, mm3
		movq	MMWORD PTR [edi], mm0
		movq	MMWORD PTR [edi + 8], mm2
		add	edi, 16
		dec	ecx
	.UNTIL	ZERO?
	ret

ERISADecoder@@ConvertSWordToSByte_MMX	ENDP

ALIGN	10H
ERISADecoder@@ConvertSWordToByte_MMX	PROC	NEAR32 C PRIVATE

	mov	ecx, 64 / 16
	.REPEAT
		movq	mm0, MMWORD PTR [esi]
		movq	mm1, MMWORD PTR [esi + 8]
		movq	mm2, MMWORD PTR [esi + 16]
		movq	mm3, MMWORD PTR [esi + 24]
		add	esi, 32
		packuswb	mm0, mm1
		packuswb	mm2, mm3
		movq	MMWORD PTR [edi], mm0
		movq	MMWORD PTR [edi + 8], mm2
		add	edi, 16
		dec	ecx
	.UNTIL	ZERO?
	ret

ERISADecoder@@ConvertSWordToByte_MMX	ENDP

;
;	4:4:4 スケーリング
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@BlockScaling444	PROC	NEAR32 SYSCALL USES ebx esi edi,
	x:SDWORD, y:SDWORD, fdwFlags:DWORD

	LOCAL	pfnYCvtFunc:PTR NEAR32
	LOCAL	pfnCCvtFunc:PTR NEAR32
	LOCAL	pfnStoreYUVChannel:PFUNC_StoreYUVImageChannel
	LOCAL	iLoop:SDWORD, jLoop:SDWORD, kLoop:SDWORD
	LOCAL	xPos:SDWORD, yPos:DWORD

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder

	.IF	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
		.IF	fdwFlags & dfDifferential
			mov	pfnYCvtFunc, OFFSET ERISADecoder@@ConvertSWordToSByte_MMX
		.ELSE
			mov	pfnYCvtFunc, OFFSET ERISADecoder@@ConvertSWordToByte_MMX
		.ENDIF
		mov	pfnCCvtFunc, OFFSET ERISADecoder@@ConvertSWordToSByte_MMX
		.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
			mov	pfnStoreYUVChannel, \
				OFFSET ERISADecoder@@StoreYUVImageChannel_SSE
		.ELSE
			mov	pfnStoreYUVChannel, \
				OFFSET ERISADecoder@@StoreYUVImageChannel_MMX
		.ENDIF
	.ELSE
		.IF	fdwFlags & dfDifferential
			mov	pfnYCvtFunc, OFFSET ERISADecoder@@ConvertSWordToSByte_486
		.ELSE
			mov	pfnYCvtFunc, OFFSET ERISADecoder@@ConvertSWordToByte_486
		.ENDIF
		mov	pfnCCvtFunc, OFFSET ERISADecoder@@ConvertSWordToSByte_486
		mov	pfnStoreYUVChannel, OFFSET ERISADecoder@@StoreYUVImageChannel
	.ENDIF
	;
	;	中間精度（16bit）から出力精度（8bit）へ変換
	; --------------------------------------------------------------------
	xor	ecx, ecx
	cmp	[ebx].m_eihInfo.fdwTransformation, CVTYPE_LOT_ERI
	mov	eax, x
	setz	cl
	mov	edx, y
	sub	eax, ecx
	sub	edx, ecx
	mov	ecx, eax
	or	ecx, edx
	.IF	SIGN? ; ((SDWORD PTR eax) < 0) || ((SDWORD PTR edx) < 0)
		ret	4 * 3
	.ENDIF
	mov	x, eax
	mov	y, edx
	;
	; 輝度チャネル
	;
	mov	edi, [ebx].m_ptrDecodeBuf
	mov	esi, [ebx].m_ptrBlocksetBuf[0]
	call	pfnYCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[4]
	call	pfnYCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[8]
	call	pfnYCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[12]
	call	pfnYCvtFunc
	;
	; 色差チャネル
	;
	mov	esi, [ebx].m_ptrBlocksetBuf[16]
	call	pfnCCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[20]
	call	pfnCCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[24]
	call	pfnCCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[28]
	call	pfnCCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[32]
	call	pfnCCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[36]
	call	pfnCCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[40]
	call	pfnCCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[44]
	call	pfnCCvtFunc
	;
	; αチャネル
	;
	.IF	[ebx].m_nChannelCount >= 4
		mov	esi, [ebx].m_ptrBlocksetBuf[48]
		call	pfnYCvtFunc
		mov	esi, [ebx].m_ptrBlocksetBuf[52]
		call	pfnYCvtFunc
		mov	esi, [ebx].m_ptrBlocksetBuf[56]
		call	pfnYCvtFunc
		mov	esi, [ebx].m_ptrBlocksetBuf[60]
		call	pfnYCvtFunc
	.ENDIF
	;
	;	スケーリングして出力
	; --------------------------------------------------------------------
	;
	; 入出力アドレス確定
	;
	@BLOCK_BYTES = 64
	mov	edi, [ebx].m_ptrNextBlockBuf
	mov	esi, [ebx].m_ptrDecodeBuf
	;
	.IF	ERI_EnabledProcessorType & \
			(ERI_USE_XMM_P3 OR ERI_USE_MMX_PENTIUM)
		xor	ecx, ecx
		.REPEAT
			movq	mm0, MMWORD PTR [esi]
			movq	mm1, MMWORD PTR [esi + @BLOCK_BYTES*4]
			movq	mm2, MMWORD PTR [esi + @BLOCK_BYTES*8]
			movq	mm3, MMWORD PTR [esi + @BLOCK_BYTES*12]
			movq	mm4, mm0
			punpckldq	mm0, mm1
			movq	mm5, mm2
			punpckldq	mm2, mm3
			punpckhdq	mm4, mm1
			movq	MMWORD PTR [edi], mm0
			punpckhdq	mm5, mm3
			movq	MMWORD PTR [edi + 08H], mm2
			movq	MMWORD PTR [edi + 10H], mm4
			movq	MMWORD PTR [edi + 18H], mm5
			;
			movq	mm0, MMWORD PTR [esi + @BLOCK_BYTES]
			movq	mm1, MMWORD PTR [esi + @BLOCK_BYTES*5]
			movq	mm2, MMWORD PTR [esi + @BLOCK_BYTES*9]
			movq	mm3, MMWORD PTR [esi + @BLOCK_BYTES*13]
			movq	mm4, mm0
			punpckldq	mm0, mm1
			movq	mm5, mm2
			punpckldq	mm2, mm3
			punpckhdq	mm4, mm1
			movq	MMWORD PTR [edi + 20H], mm0
			punpckhdq	mm5, mm3
			movq	MMWORD PTR [edi + 28H], mm2
			movq	MMWORD PTR [edi + 30H], mm4
			movq	MMWORD PTR [edi + 38H], mm5
			;
			inc	ecx
			add	esi, 8
			add	edi, 40H
			.IF	ecx == 8
				add	esi, @BLOCK_BYTES
			.ENDIF
		.UNTIL	ecx >= 16
	.ELSE
		xor	ecx, ecx
		.REPEAT
			push	ecx
			;
			mov	ecx, 8
			.REPEAT
				mov	al, BYTE PTR [esi]
				mov	ah, BYTE PTR [esi + @BLOCK_BYTES*4]
				mov	dl, BYTE PTR [esi + @BLOCK_BYTES*8]
				mov	dh, BYTE PTR [esi + @BLOCK_BYTES*12]
				inc	esi
				mov	BYTE PTR [edi], al
				mov	BYTE PTR [edi + 1], ah
				mov	BYTE PTR [edi + 2], dl
				mov	BYTE PTR [edi + 3], dh
				add	edi, 4
				dec	ecx
			.UNTIL	ZERO?
			;
			sub	esi, 8
			mov	ecx, 8
			.REPEAT
				mov	al, BYTE PTR [esi + @BLOCK_BYTES]
				mov	ah, BYTE PTR [esi + @BLOCK_BYTES*5]
				mov	dl, BYTE PTR [esi + @BLOCK_BYTES*9]
				mov	dh, BYTE PTR [esi + @BLOCK_BYTES*13]
				inc	esi
				mov	BYTE PTR [edi], al
				mov	BYTE PTR [edi + 1], ah
				mov	BYTE PTR [edi + 2], dl
				mov	BYTE PTR [edi + 3], dh
				add	edi, 4
				dec	ecx
			.UNTIL	ZERO?
			;
			pop	ecx
			inc	ecx
			.IF	ecx == 8
				add	esi, @BLOCK_BYTES
			.ENDIF
		.UNTIL	ecx >= 16
	.ENDIF
	;
	mov	[ebx].m_ptrNextBlockBuf, edi
	ASSUME	ebx:NOTHING
	ret	4 * 3

ERISADecoder@@BlockScaling444	ENDP

;
;	4:1:1 スケーリング
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@BlockScaling411	PROC	NEAR32 SYSCALL USES ebx esi edi,
	x:SDWORD, y:SDWORD, fdwFlags:DWORD

	LOCAL	pfnYCvtFunc:PTR NEAR32
	LOCAL	pfnCCvtFunc:PTR NEAR32
	LOCAL	nYUVLineBytes:DWORD

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder

	.IF	[ebx].m_nChannelCount != 1
		.IF	ERI_EnabledProcessorType & \
				(ERI_USE_MMX_PENTIUM OR ERI_USE_XMM_P3)
			.IF	fdwFlags & dfDifferential
				mov	pfnYCvtFunc, OFFSET ERISADecoder@@ConvertSWordToSByte_MMX
			.ELSE
				mov	pfnYCvtFunc, OFFSET ERISADecoder@@ConvertSWordToByte_MMX
			.ENDIF
			mov	pfnCCvtFunc, OFFSET ERISADecoder@@ConvertSWordToSByte_MMX
		.ELSE
			.IF	fdwFlags & dfDifferential
				mov	pfnYCvtFunc, OFFSET ERISADecoder@@ConvertSWordToSByte_486
			.ELSE
				mov	pfnYCvtFunc, OFFSET ERISADecoder@@ConvertSWordToByte_486
			.ENDIF
			mov	pfnCCvtFunc, OFFSET ERISADecoder@@ConvertSWordToSByte_486
		.ENDIF
	.ENDIF
	;
	;	中間精度（16bit）から出力精度（8bit）へ変換
	; --------------------------------------------------------------------
	xor	ecx, ecx
	cmp	[ebx].m_eihInfo.fdwTransformation, CVTYPE_LOT_ERI
	mov	eax, x
	setz	cl
	mov	edx, y
	sub	eax, ecx
	sub	edx, ecx
	mov	ecx, eax
	or	ecx, edx
	.IF	SIGN? ; ((SDWORD PTR eax) < 0) || ((SDWORD PTR edx) < 0)
		ret	4 * 3
	.ENDIF
	mov	x, eax
	mov	y, edx
	;
	; 輝度チャネル
	;
	mov	edi, [ebx].m_ptrDecodeBuf
	mov	esi, [ebx].m_ptrBlocksetBuf[0]
	call	pfnYCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[4]
	call	pfnYCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[8]
	call	pfnYCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[12]
	call	pfnYCvtFunc
	;
	; 色差チャネル
	;
	mov	esi, [ebx].m_ptrBlocksetBuf[16]
	call	pfnCCvtFunc
	mov	esi, [ebx].m_ptrBlocksetBuf[20]
	call	pfnCCvtFunc
	;
	; αチャネル
	;
	.IF	[ebx].m_nChannelCount >= 4
		mov	esi, [ebx].m_ptrBlocksetBuf[24]
		call	pfnYCvtFunc
		mov	esi, [ebx].m_ptrBlocksetBuf[28]
		call	pfnYCvtFunc
		mov	esi, [ebx].m_ptrBlocksetBuf[32]
		call	pfnYCvtFunc
		mov	esi, [ebx].m_ptrBlocksetBuf[36]
		call	pfnYCvtFunc
	.ENDIF
	;
	;	スケーリングして出力
	; --------------------------------------------------------------------
	;
	; 入出力アドレス確定
	;
	mov	edi, [ebx].m_ptrNextBlockBuf
	mov	esi, [ebx].m_ptrDecodeBuf
	;
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
	.IF	!([ebx].m_fdwDecFlags & dfQuickDecode)
		;
		;	スケーリング出力：SSE 専用コード
		; ------------------------------------------------------------
		lea	edx, [esi + 64*4]
		xor	ecx, ecx
		;
		; UV 値を符号無し整数として扱えるように（補完のため）
		; 80H のバイアスをかける
		;
		movq	mm7, mmxMask_ByteMSB
		@INDEX = 0
		REPEAT	8
			movq	mm0, MMWORD PTR [edx + @INDEX]
			movq	mm1, MMWORD PTR [edx + @INDEX + 64]
			pxor	mm0, mm7
			pxor	mm1, mm7
			movq	MMWORD PTR [edx + @INDEX], mm0
			movq	MMWORD PTR [edx + @INDEX + 64], mm1
			@INDEX = @INDEX + 8
		ENDM
		;
		.REPEAT
				mov	eax, ecx
			movq	mm0, MMWORD PTR [esi]
				and	eax, 01H
			movq	mm5, MMWORD PTR [edx]
				lea	eax, [edx + eax * 8]
				cmp	ecx, 15
			movq	mm7, MMWORD PTR [edx + 64]
				cmovz	eax, edx
			movq	mm2, MMWORD PTR [esi + 64*6]
			;
			pavgb	mm5, MMWORD PTR [eax]
				pavgb	mm7, MMWORD PTR [eax + 64]
			movq	mm1, mm5
			movq	mm4, mm5
			psllq	mm5, 64-8
				movq	mm3, mm7
			psllq	mm1, 8
				movq	mm6, mm7
			psrlq	mm5, 64-8
				psllq	mm7, 64-8
			por	mm1, mm5
				psllq	mm3, 8
			movq	mm5, mm4
				psrlq	mm7, 64-8
			pavgb	mm1, mm4
				por	mm3, mm7
				movq	mm7, mm6
			punpcklbw	mm4, mm1
				pavgb	mm3, mm6
			punpckhbw	mm5, mm1
				punpcklbw	mm6, mm3
				punpckhbw	mm7, mm3
			;
			movq	mm1, mm0
				pxor	mm4, mmxMask_ByteMSB
				pxor	mm6, mmxMask_ByteMSB
			punpckldq	mm0, mm4
			movq	mm3, mm6
			punpckldq	mm6, mm2
			;
			movq	MMWORD PTR [edi], mm0
			movq	MMWORD PTR [edi + 8], mm6
			;
			punpckhdq	mm1, mm4
				pxor	mm5, mmxMask_ByteMSB
			punpckhdq	mm3, mm2
			;
			movq	MMWORD PTR [edi + 16], mm1
			movq	MMWORD PTR [edi + 24], mm3
				pxor	mm7, mmxMask_ByteMSB
			;
			movq	mm0, MMWORD PTR [esi + 64]
			movq	mm2, MMWORD PTR [esi + 64*7]
			movq	mm1, mm0
				punpckldq	mm0, mm5
			movq	mm3, mm7
				punpckldq	mm7, mm2
			movq	MMWORD PTR [edi + 32], mm0
				punpckhdq	mm1, mm5
			movq	MMWORD PTR [edi + 40], mm7
				punpckhdq	mm3, mm2
			;
			movq	MMWORD PTR [edi + 48], mm1
			movq	MMWORD PTR [edi + 56], mm3
			;
			add	esi, 8
			mov	eax, ecx
			and	eax, 1
			lea	edx, [edx + eax * 8]
			lea	eax, [esi + 64]
			cmp	ecx, 7
			cmovz	esi, eax
			add	edi, 64
			inc	ecx
		.UNTIL	ecx >= 16
	.ELSE
		lea	edx, [esi + 64*4]
		xor	ecx, ecx
		.REPEAT
			movq	mm0, MMWORD PTR [esi]
			movq	mm4, MMWORD PTR [edx]
			movq	mm6, MMWORD PTR [edx + 64]
			movq	mm2, MMWORD PTR [esi + 64*6]
			;
			movq	mm5, mm4
				punpcklbw	mm4, mm4
			movq	mm7, mm6
				punpcklbw	mm6, mm6
			movq	mm1, mm0
				punpckhbw	mm5, mm5
			movq	mm3, mm6
				punpckhbw	mm7, mm7
			;
			punpckldq	mm0, mm4
			punpckldq	mm6, mm2
				movq	MMWORD PTR [edi], mm0
			punpckhdq	mm1, mm4
				movq	MMWORD PTR [edi + 8], mm6
			punpckhdq	mm3, mm2
			;
			movq	MMWORD PTR [edi + 16], mm1
			movq	MMWORD PTR [edi + 24], mm3
			;
			movq	mm0, MMWORD PTR [esi + 64]
			movq	mm2, MMWORD PTR [esi + 64*7]
			movq	mm1, mm0
				punpckldq	mm0, mm5
			movq	mm3, mm7
				punpckldq	mm7, mm2
			movq	MMWORD PTR [edi + 32], mm0
				punpckhdq	mm1, mm5
			movq	MMWORD PTR [edi + 40], mm7
				punpckhdq	mm3, mm2
			;
			movq	MMWORD PTR [edi + 48], mm1
			movq	MMWORD PTR [edi + 56], mm3
			;
			add	esi, 8
			mov	eax, ecx
			and	eax, 1
			.IF	ecx == 7
				add	esi, 64
			.ENDIF
			lea	edx, [edx + eax * 8]
			add	edi, 64
			inc	ecx
		.UNTIL	ecx >= 16
	.ENDIF

	.ELSEIF	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
		;
		;	スケーリング出力：MMX 専用コード
		; ------------------------------------------------------------
		lea	edx, [esi + 64*4]
		xor	ecx, ecx
		.REPEAT
			mov	al, BYTE PTR [edi]
			movq	mm0, MMWORD PTR [esi]
			movq	mm4, MMWORD PTR [edx]
			movq	mm6, MMWORD PTR [edx + 64]
			movq	mm2, MMWORD PTR [esi + 64*6]
			;
			movq	mm5, mm4
				punpcklbw	mm4, mm4
			movq	mm7, mm6
				punpcklbw	mm6, mm6
			movq	mm1, mm0
				punpckhbw	mm5, mm5
			movq	mm3, mm6
				punpckhbw	mm7, mm7
			;
			punpckldq	mm0, mm4
			punpckldq	mm6, mm2
				movq	MMWORD PTR [edi], mm0
			punpckhdq	mm1, mm4
				movq	MMWORD PTR [edi + 8], mm6
			punpckhdq	mm3, mm2
			;
			movq	MMWORD PTR [edi + 16], mm1
			movq	MMWORD PTR [edi + 24], mm3
			;
			movq	mm0, MMWORD PTR [esi + 64]
			movq	mm2, MMWORD PTR [esi + 64*7]
			movq	mm1, mm0
				punpckldq	mm0, mm5
			movq	mm3, mm7
				punpckldq	mm7, mm2
			movq	MMWORD PTR [edi + 32], mm0
				punpckhdq	mm1, mm5
			movq	MMWORD PTR [edi + 40], mm7
				punpckhdq	mm3, mm2
			;
			movq	MMWORD PTR [edi + 48], mm1
			movq	MMWORD PTR [edi + 56], mm3
			;
			add	esi, 8
			mov	eax, ecx
			and	eax, 1
			.IF	ecx == 7
				add	esi, 64
			.ENDIF
			lea	edx, [edx + eax * 8]
			add	edi, 64
			inc	ecx
		.UNTIL	ecx >= 16

	.ELSE
		;
		;	スケーリング出力：486 互換コード
		; ------------------------------------------------------------
		ASSUME	ebx:NOTHING
		push	ebx
		lea	edx, [esi + 64*4]
		xor	ecx, ecx
		.REPEAT
			push	ecx
			push	edi
			;
			@INDEX = 0
			REPEAT	4
				mov	ax, WORD PTR [esi + @INDEX*2]
				mov	bl, BYTE PTR [edx + @INDEX]
				mov	bh, BYTE PTR [edx + 64 + @INDEX]
				mov	cx, WORD PTR [esi + 64*6 + @INDEX*2]
				;
				mov	BYTE PTR [edi], al
				mov	BYTE PTR [edi + 1], bl
				mov	BYTE PTR [edi + 2], bh
				mov	BYTE PTR [edi + 3], cl
				;
				mov	BYTE PTR [edi + 4], ah
				mov	BYTE PTR [edi + 5], bl
				mov	BYTE PTR [edi + 6], bh
				mov	BYTE PTR [edi + 7], ch
				add	edi, 8
				;
				@INDEX = @INDEX + 1
			ENDM
			REPEAT	4
				mov	ax, WORD PTR [esi + 64 + @INDEX*2-8]
				mov	bl, BYTE PTR [edx + @INDEX]
				mov	bh, BYTE PTR [edx + 64 + @INDEX]
				mov	cx, WORD PTR [esi + 64*7 + @INDEX*2-8]
				;
				mov	BYTE PTR [edi], al
				mov	BYTE PTR [edi + 1], bl
				mov	BYTE PTR [edi + 2], bh
				mov	BYTE PTR [edi + 3], cl
				;
				mov	BYTE PTR [edi + 4], ah
				mov	BYTE PTR [edi + 5], bl
				mov	BYTE PTR [edi + 6], bh
				mov	BYTE PTR [edi + 7], ch
				add	edi, 8
				;
				@INDEX = @INDEX + 1
			ENDM
			;
			pop	edi
			pop	ecx
			add	esi, 8
			mov	eax, ecx
			and	eax, 1
			.IF	ecx == 7
				add	esi, 64
			.ENDIF
			lea	edx, [edx + eax * 8]
			add	edi, 64
			inc	ecx
		.UNTIL	ecx >= 16
		pop	ebx
		ASSUME	ebx:PTR ERISADecoder
	.ENDIF

	mov	[ebx].m_ptrNextBlockBuf, edi
	ASSUME	ebx:NOTHING
	ret	4 * 3

ERISADecoder@@BlockScaling411	ENDP

;
;	中間バッファを YUV から RGB 形式へ変換
; ----------------------------------------------------------------------------
;  パラメータ；
;	sse_flag	: SSE を使用するか？
;	differential	: 差分フレームか？
;	dst_type	: 出力タイプ（１のときは出力アドレスにECXを使用）
;  レジスタ；
;	EDI		: 入力データアドレス
;	ESI		: 処理するピクセル数
;	EDX		: 破壊される
;	ECX		: dst_type=1 のとき、出力アドレス
;	EAX		: 破壊される
;	EBX		: PTR ERISADecoder
; ----------------------------------------------------------------------------

@ConvertImageYUVtoRGB_SSE	MACRO	sse_flag:REQ, differential:REQ, dst_type:=<0>
	IF	sse_flag
;		mov	edx, [ebx].m_nYUVLineBytes
		IF	dst_type
			mov	eax, [ebx].m_nDstLineBytes
		ENDIF
	ENDIF
	sub	esi, 4
	.WHILE	!SIGN?
		movq	mm4, MMWORD PTR [edi]
		movq	mm5, MMWORD PTR [edi + 8]
		punpcklbw	mm0, mm4
		punpckhbw	mm1, mm4
;		IF	sse_flag
;			prefetchnta	[edi + edx]
;		ENDIF
		punpcklbw	mm2, mm5
		punpckhbw	mm3, mm5
		IF	(sse_flag NE 0) AND (dst_type NE 0)
			prefetcht0	[ecx + eax]
		ENDIF
		IF	differential
			psraw	mm0, 8		; mm0 = y0:y1:y2:y3
			psraw	mm1, 8		; mm1 = u0:u1:u2:u3
			psraw	mm2, 8		; mm2 = v0:v1:v2:v3
			psraw	mm3, 8		; mm3 = a0:a1:a2:a3
		ELSE
			psrlw	mm0, 8		; mm0 = y0:y1:y2:y3
			psraw	mm1, 8		; mm1 = u0:u1:u2:u3
			psraw	mm2, 8		; mm2 = v0:v1:v2:v3
			psrlw	mm3, 8		; mm3 = a0:a1:a2:a3
		ENDIF
		;
		movq	mm4, mm1	; mm4 = 3*u
		movq	mm5, mm2	; mm5 = 3*v
		psllw	mm4, 1
		psllw	mm5, 1
		paddsw	mm4, mm1
		paddsw	mm5, mm2
		;
		movq	mm6, mm4	; mm6 = 3*u + 6*v
		psllw	mm4, 1		; mm4 = 7*u
		movq	mm7, mm5
		psraw	mm5, 1		; mm5 = y + (3*v)/2
		paddsw	mm4, mm1
		psllw	mm7, 1
		paddsw	mm5, mm0
		paddsw	mm6, mm7
		;
		psraw	mm4, 2		; mm4 = y + (7*u)/4
		movq	mm1, mm0	; mm1 = y - (3*u + 6*v)/8
		psraw	mm6, 3
		paddsw	mm4, mm0
		psubsw	mm1, mm6
		;
		movq	mm2, mm5
		movq	mm0, mm4
		punpcklwd	mm4, mm1 ; = b0:g0:b1:g1
		punpckhwd	mm0, mm1 ; = b2:g2:b3:g3
		punpcklwd	mm5, mm3 ; = r0:a0:r1:a1
		punpckhwd	mm2, mm3 ; = r2:a2:r3:a3
		;
		IF	(dst_type NE 0) AND (differential NE 0)
			movq	mm6, mm4
			movq	mm1, mm0
			punpckldq	mm4, mm5	; = b0:g0:r0:a0
				movq	mm3, MMWORD PTR [ecx]
			punpckldq	mm0, mm2	; = b2:g2:r2:a2
				pxor	mm7, mm7
			punpckhdq	mm6, mm5	; = b1:g1:r1:a1
				movq	mm5, mm3
			punpckhdq	mm1, mm2	; = b3:g3:r3:a3
			;
			paddsw	mm4, mm4
				punpcklbw	mm3, mm7
			paddsw	mm6, mm6
				punpckhbw	mm5, mm7
			paddsw	mm0, mm0
				movq	mm2, MMWORD PTR [ecx + 8]
			paddsw	mm1, mm1
			;
			paddsw	mm3, mm4
				movq	mm4, mm2
				punpcklbw	mm2, mm7
			paddsw	mm5, mm6
				punpckhbw	mm4, mm7
			paddsw	mm0, mm2
			paddsw	mm1, mm4
			;
			packuswb	mm3, mm5
			packuswb	mm0, mm1
			movq	MMWORD PTR [ecx], mm3
			movq	MMWORD PTR [ecx + 8], mm0
			add	ecx, 16
		ELSE
			IF	differential
				packsswb	mm4, mm0
				packsswb	mm5, mm2
			ELSE
				packuswb	mm4, mm0
				packuswb	mm5, mm2
			ENDIF
			movq	mm0, mm4
			punpcklwd	mm4, mm5 ; rgba0:rgba1
			punpckhwd	mm0, mm5 ; rgba2:rgba3
			;
			IF	dst_type
				IF	sse_flag
					movntq	MMWORD PTR [ecx], mm4
					movntq	MMWORD PTR [ecx + 8], mm0
				ELSE
					movq	MMWORD PTR [ecx], mm4
					movq	MMWORD PTR [ecx + 8], mm0
				ENDIF
				add	ecx, 16
			ELSE
				movq	MMWORD PTR [edi], mm4
				movq	MMWORD PTR [edi + 8], mm0
			ENDIF
		ENDIF
		;
		add	edi, 16
		sub	esi, 4
	.ENDW
	add	esi, 4
	jz	Loop_Continue
	;
	movd	mm0, DWORD PTR [edi]
	movd	mm1, DWORD PTR [edi + 4]
	movd	mm2, DWORD PTR [edi + 8]
	movd	mm3, DWORD PTR [edi + 12]
	punpcklbw	mm0, mm1	; y0:u0:y1:u1:y2:u2:y3:u3
	punpcklbw	mm2, mm3	; v0:a0:v1:a1:v2:a2:v3:a3
	movq	mm1, mm0
	punpcklwd	mm0, mm2	; yuva0:yuva1
	punpckhwd	mm1, mm2	; yuva2:yuva3
	movq	MMWORD PTR [edi], mm0
	movq	MMWORD PTR [edi + 8], mm1
ENDM

@ConvertImageYUVtoRGB_486	MACRO	differential:REQ
	push	ebx
	push	ebp
	test	esi, esi
	.WHILE	!ZERO?
		movsx	ecx, SBYTE PTR [edi + 2]
		movsx	ebx, SBYTE PTR [edi + 1]
		IF	differential
			movsx	eax, SBYTE PTR [edi]
		ELSE
			movzx	eax, BYTE PTR [edi]
		ENDIF
		;
		lea	edx, [ecx + ecx * 2]	; edx = 3*v
		lea	ebp, [ebx + ebx * 2]	; ebp = 3*u
		;
		mov	ecx, edx
		sar	edx, 1
		lea	ebx, [ebx + ebp * 2]	; ebx = 7*u
		lea	ecx, [ebp + ecx * 2]	; ecx = 3*u + 6*v
		sar	ebx, 2
		sar	ecx, 3
		;
		add	edx, eax		; edx = red
		add	ebx, eax		; ebx = blue
		sub	eax, ecx		; eax = green
		;
		IF	differential
			add	ebx, 80H
			add	eax, 80H
			add	edx, 80H
		ENDIF
		;
		.IF	(DWORD PTR ebx) > 0FFH
			not	ebx
			sar	ebx, 31
		.ENDIF
		.IF	(DWORD PTR eax) > 0FFH
			not	eax
			sar	eax, 31
		.ENDIF
		.IF	(DWORD PTR edx) > 0FFH
			not	edx
			sar	edx, 31
		.ENDIF
		;
		IF	differential
			sub	ebx, 80H
			sub	eax, 80H
			sub	edx, 80H
		ENDIF
		mov	SBYTE PTR [edi]    , bl
		mov	SBYTE PTR [edi + 1], al
		mov	SBYTE PTR [edi + 2], dl
		;
		add	edi, 4
		dec	esi
	.ENDW
	pop	ebp
	pop	ebx
ENDM

@ArrangeYUVTemporaryBuffer	MACRO	mmx_flag:REQ
	IF	mmx_flag
		mov	ecx, [ebx].m_nDstWidth
		add	ecx, 0FH
		shr	ecx, 4
		.WHILE	!ZERO?
			movq	mm0, MMWORD PTR [esi]
			movq	mm1, MMWORD PTR [esi + 08H]
			movq	mm2, MMWORD PTR [esi + 10H]
			movq	mm3, MMWORD PTR [esi + 18H]
			movq	mm4, MMWORD PTR [esi + 20H]
			movq	mm5, MMWORD PTR [esi + 28H]
			movq	mm6, MMWORD PTR [esi + 30H]
			movq	mm7, MMWORD PTR [esi + 38H]
			add	esi, 16*16 * 4
			movq	MMWORD PTR [edi], mm0
			movq	MMWORD PTR [edi + 08H], mm1
			movq	MMWORD PTR [edi + 10H], mm2
			movq	MMWORD PTR [edi + 18H], mm3
			movq	MMWORD PTR [edi + 20H], mm4
			movq	MMWORD PTR [edi + 28H], mm5
			movq	MMWORD PTR [edi + 30H], mm6
			movq	MMWORD PTR [edi + 38H], mm7
			add	edi, 40H
			dec	ecx
		.ENDW
	ELSE
		pushfd
		cld
		mov	edx, [ebx].m_nDstWidth
		add	edx, 0FH
		shr	edx, 4
		.WHILE	!ZERO?
			mov	ecx, 10H
			rep	movsd
			add	esi, 16*16 * 4 - 40H
			dec	edx
		.ENDW
		popfd
	ENDIF
ENDM

ALIGN	10H
ERISADecoder@@ConvertImageYUVtoRGB	PROC	NEAR32 SYSCALL USES ebx esi edi,
	fdwFlags:DWORD

	LOCAL	ptrYUVLine:PTR SBYTE
	LOCAL	ptrSrcLine:PTR SBYTE
	LOCAL	yLoop:DWORD, xLoop:DWORD

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder

	.IF	[ebx].m_nChannelCount >= 3

	mov	edi, [ebx].m_ptrYUVImage
	mov	esi, [ebx].m_ptrBlockLineBuf
	xor	ecx, ecx
	.REPEAT
		mov	ptrYUVLine, edi
		mov	ptrSrcLine, esi
		mov	yLoop, ecx
		;
		; YUV 配列正規化
		;
		.IF	ERI_EnabledProcessorType & \
				(ERI_USE_XMM_P3 OR ERI_USE_MMX_PENTIUM)
			@ArrangeYUVTemporaryBuffer	1
		.ELSE
			@ArrangeYUVTemporaryBuffer	0
		.ENDIF
		;
		mov	edi, ptrYUVLine
		mov	esi, [ebx].m_nDstWidth
		.IF	fdwFlags & dfDifferential
		;
		; 差分フレーム用
		;
		.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
			@ConvertImageYUVtoRGB_SSE	1, 1
		.ELSEIF	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
			@ConvertImageYUVtoRGB_SSE	0, 1
		.ENDIF
		;
		@ConvertImageYUVtoRGB_486	1

		.ELSE
		;
		; 独立フレーム用
		;
		.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
			@ConvertImageYUVtoRGB_SSE	1, 0
		.ELSEIF	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
			@ConvertImageYUVtoRGB_SSE	0, 0
		.ENDIF
		;
		@ConvertImageYUVtoRGB_486	0

		.ENDIF
Loop_Continue:	;
		mov	esi, ptrSrcLine
		mov	ecx, yLoop
		mov	edi, ptrYUVLine
		add	esi, 16 * 4
		inc	ecx
		add	edi, [ebx].m_nYUVLineBytes
	.UNTIL	ecx >= [ebx].m_nDstHeight

	.ENDIF

	ASSUME	ebx:NOTHING
	ret	4

ERISADecoder@@ConvertImageYUVtoRGB	ENDP

;
;	RGB 画像の出力
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@LS_RestoreRGB24	PROC	NEAR32 SYSCALL USES ebx esi edi

	LOCAL	yLoop:DWORD, nLastPixels:DWORD
	LOCAL	ptrDstLine:PTR, ptrDstNext:PTR
	LOCAL	ptrSrcLine:PTR, ptrSrcNext:PTR

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder
	;
	.IF	([ebx].m_nDstPixelBytes == 4) && \
			([ebx].m_nYUVPixelBytes == 4) && \
			(ERI_EnabledProcessorType & \
				(ERI_USE_XMM_P3 OR ERI_USE_MMX_PENTIUM))
		;
		;	MMX/SSE 専用出力コード
		; ------------------------------------------------------------
		mov	esi, [ebx].m_ptrBlockLineBuf
		mov	edi, [ebx].m_ptrDstBlock
		mov	ecx, [ebx].m_nDstHeight
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	yLoop, ecx
			mov	ptrDstLine, edi
			mov	ptrSrcLine, esi
			;
			; 中間バッファの配列変換
			;
			mov	edi, [ebx].m_ptrImageBuf
			@ArrangeYUVTemporaryBuffer	1
			;
			; YUV->RGB 変換＆出力
			;
			mov	ecx, ptrDstLine
			mov	edi, [ebx].m_ptrImageBuf
			mov	esi, [ebx].m_nDstWidth
			;
			.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
				@ConvertImageYUVtoRGB_SSE	1, 0, 1
			.ELSE;	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
				@ConvertImageYUVtoRGB_SSE	0, 0, 1
			.ENDIF
			;
			mov	ptrDstNext, ecx
			mov	ptrSrcNext, edi
			mov	nLastPixels, esi
			@ConvertImageYUVtoRGB_486	0
			;
			; ストア
			;
			mov	esi, ptrSrcNext
			mov	edi, ptrDstNext
			mov	ecx, nLastPixels
			mov	edx, [ebx].m_nYUVLineBytes
			test	ecx, ecx
			.WHILE	!ZERO?
				mov	eax, DWORD PTR [esi]
				add	esi, (SIZEOF DWORD)
				mov	DWORD PTR [edi], eax
				add	edi, (SIZEOF DWORD)
				dec	ecx
			.ENDW
Loop_Continue:		;
			mov	esi, ptrSrcLine
			mov	edi, ptrDstLine
			mov	ecx, yLoop
			add	esi, 16*4
			add	edi, [ebx].m_nDstLineBytes
			dec	ecx
		.ENDW
		.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
			sfence
		.ENDIF
	.ELSE
		;
		;	486 専用出力コード
		; ------------------------------------------------------------
		push	0
		call	ERISADecoder@@ConvertImageYUVtoRGB
		;
		mov	ecx, [ebx].m_nDstHeight
		mov	esi, [ebx].m_ptrYUVImage
		mov	edi, [ebx].m_ptrDstBlock
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	yLoop, ecx
			mov	ptrDstLine, edi
			mov	ptrSrcLine, esi
			;
			mov	ecx, [ebx].m_nDstWidth
			test	ecx, ecx
			.WHILE	!ZERO?
				mov	al, BYTE PTR [esi]
				mov	ah, BYTE PTR [esi + 1]
				mov	dl, BYTE PTR [esi + 2]
				add	esi, [ebx].m_nYUVPixelBytes
				mov	BYTE PTR [edi], al
				mov	BYTE PTR [edi + 1], ah
				mov	BYTE PTR [edi + 2], dl
				add	edi, [ebx].m_nDstPixelBytes
				dec	ecx
			.ENDW
			;
			mov	esi, ptrSrcLine
			mov	edi, ptrDstLine
			mov	ecx, yLoop
			add	esi, [ebx].m_nYUVLineBytes
			add	edi, [ebx].m_nDstLineBytes
			dec	ecx
		.ENDW
	.ENDIF
	;
	ASSUME	ebx:NOTHING
	ret	0

ERISADecoder@@LS_RestoreRGB24	ENDP

;
;	RGBA 画像の出力
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@LS_RestoreRGBA32	PROC	NEAR32 SYSCALL USES ebx esi edi

	LOCAL	yLoop:DWORD, nLastPixels:DWORD
	LOCAL	ptrDstLine:PTR, ptrDstNext:PTR
	LOCAL	ptrSrcLine:PTR, ptrSrcNext:PTR

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder
	;
	.IF	([ebx].m_nDstPixelBytes == 4) && \
			([ebx].m_nYUVPixelBytes == 4) && \
			(ERI_EnabledProcessorType & \
				(ERI_USE_XMM_P3 OR ERI_USE_MMX_PENTIUM))
		;
		;	MMX/SSE 専用出力コード
		; ------------------------------------------------------------
		mov	esi, [ebx].m_ptrBlockLineBuf
		mov	edi, [ebx].m_ptrDstBlock
		mov	ecx, [ebx].m_nDstHeight
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	yLoop, ecx
			mov	ptrDstLine, edi
			mov	ptrSrcLine, esi
			;
			; 中間バッファの配列変換
			;
			mov	edi, [ebx].m_ptrImageBuf
			@ArrangeYUVTemporaryBuffer	1
			;
			; YUV->RGB 変換＆出力
			;
			mov	ecx, ptrDstLine
			mov	edi, [ebx].m_ptrImageBuf
			mov	esi, [ebx].m_nDstWidth
			;
			.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
				@ConvertImageYUVtoRGB_SSE	1, 0, 1
			.ELSE;	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
				@ConvertImageYUVtoRGB_SSE	0, 0, 1
			.ENDIF
			;
			mov	ptrDstNext, ecx
			mov	ptrSrcNext, edi
			mov	nLastPixels, esi
			@ConvertImageYUVtoRGB_486	0
			;
			; ストア
			;
			mov	esi, ptrSrcNext
			mov	edi, ptrDstNext
			mov	ecx, nLastPixels
			mov	edx, [ebx].m_nYUVLineBytes
			test	ecx, ecx
			.WHILE	!ZERO?
				mov	eax, DWORD PTR [esi]
				add	esi, (SIZEOF DWORD)
				mov	DWORD PTR [edi], eax
				add	edi, (SIZEOF DWORD)
				dec	ecx
			.ENDW
Loop_Continue:		;
			mov	esi, ptrSrcLine
			mov	edi, ptrDstLine
			mov	ecx, yLoop
			add	esi, 16*4
			add	edi, [ebx].m_nDstLineBytes
			dec	ecx
		.ENDW
		.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
			sfence
		.ENDIF
	.ELSE
		;
		;	486 専用出力コード
		; ------------------------------------------------------------
		push	0
		call	ERISADecoder@@ConvertImageYUVtoRGB
		;
		mov	esi, [ebx].m_ptrYUVImage
		mov	edi, [ebx].m_ptrDstBlock
		mov	ecx, [ebx].m_nDstHeight
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	yLoop, ecx
			mov	ptrDstLine, edi
			mov	ptrSrcLine, esi
			;
			mov	ecx, [ebx].m_nDstWidth
			test	ecx, ecx
			.WHILE	!ZERO?
				mov	eax, DWORD PTR [edi]
				add	edi, (SIZEOF DWORD)
				mov	DWORD PTR [esi], eax
				add	esi, (SIZEOF DWORD)
				dec	ecx
			.ENDW
			;
			mov	esi, ptrSrcLine
			mov	edi, ptrDstLine
			mov	ecx, yLoop
			add	esi, [ebx].m_nYUVLineBytes
			add	edi, [ebx].m_nDstLineBytes
			dec	ecx
		.ENDW
	.ENDIF
	;
	ASSUME	ebx:NOTHING
	ret	0

ERISADecoder@@LS_RestoreRGBA32	ENDP

;
;	1ライン分、動き補償を伴って画像を複製
; ----------------------------------------------------------------------------
;  パラメータ；
;	sse_flag	: SSE を使用するか？
;  レジスタ；
;	EDI		: 出力データアドレス
;	ESI		: 破壊される
;	EDX		: 破壊される
;	ECX		: 破壊される
;	EAX		: 破壊される
;	EBX		: PTR ERISADecoder
; ----------------------------------------------------------------------------
@ERISADecoder@@MoveLineWithVector	MACRO	sse_flag:REQ
	LOCAL	Label_RefPredictBlock, Label_RefBidirectBlock
	LOCAL	Label_IntraBlock, Label_Continue

	mov	ecx, [ebx].m_nDstWidth
	mov	esi, ptrLineBlockAddrs
	;
	push	ebp
	sub	ecx, 16
	.WHILE	!SIGN?
		mov	eax, DWORD PTR [esi]
		mov	ebp, DWORD PTR [esi + 4]
		mov	edx, [ebx].m_dwPrevLineBytes
		test	eax, eax
;		IF	sse_flag
;			prefetchnta	[edi + 80H]
;		ENDIF
		.IF	ZERO?
			mov	eax, ebp
			mov	edx, [ebx].m_dwNextLineBytes
			test	eax, eax
			jz	Label_IntraBlock
			;
			; 後フレーム参照ブロック
			;
			IF	sse_flag
				xor	ebp, ebp
				test	[ebx].m_fdwDecFlags, dfQuickDecode
				cmovz	ebp, DWORD PTR [esi + 12]
				and	ebp, 03H
				call	pfnMovePBlock_SSE[ebp * 4]
			ELSE
				xor	ebp, ebp
				.IF	!([ebx].m_fdwDecFlags & dfQuickDecode)
					mov	ebp, DWORD PTR [esi + 12]
				.ENDIF
				and	ebp, 03H
				call	pfnMovePBlock_MMX[ebp * 4]
			ENDIF
			mov	DWORD PTR [esi + 4], eax
			jmp	Label_Continue
		.ENDIF
		test	ebp, ebp
		jz	Label_RefPredictBlock
Label_RefBidirectBlock:
		;
		; 双方向フレーム参照ブロック
		;
		IF	sse_flag
			xor	ebp, ebp
			test	[ebx].m_fdwDecFlags, dfQuickDecode
			cmovz	ebp, DWORD PTR [esi + 8]
			and	ebp, 03H
			call	pfnMovePBlock_SSE[ebp * 4]
			xor	ebp, ebp
			test	[ebx].m_fdwDecFlags, dfQuickDecode
			cmovz	ebp, DWORD PTR [esi + 12]
			mov	DWORD PTR [esi], eax
			mov	eax, DWORD PTR [esi + 4]
			and	ebp, 03H
			mov	edx, [ebx].m_dwNextLineBytes
			call	pfnMoveBBlock_SSE[ebp * 4]
			mov	DWORD PTR [esi + 4], eax
		ELSE
			call	Func_MoveBBlock0_MMX
			mov	DWORD PTR [esi], eax
			mov	DWORD PTR [esi + 4], ebp
		ENDIF
		jmp	Label_Continue

Label_RefPredictBlock:
		;
		; 前フレーム参照ブロック
		;
		IF	sse_flag
			xor	ebp, ebp
			test	[ebx].m_fdwDecFlags, dfQuickDecode
			cmovz	ebp, DWORD PTR [esi + 8]
			and	ebp, 03H
			call	pfnMovePBlock_SSE[ebp * 4]
		ELSE
			xor	ebp, ebp
			.IF	!([ebx].m_fdwDecFlags & dfQuickDecode)
				mov	ebp, DWORD PTR [esi + 8]
			.ENDIF
			and	ebp, 03H
			call	pfnMovePBlock_MMX[ebp * 4]
		ENDIF
		mov	DWORD PTR [esi], eax
		jmp	Label_Continue
Label_IntraBlock:
		;
		; 独立フレーム
		;
		IF	sse_flag
			xorps	xmm0, xmm0
			prefetcht0	[edi + 60H]
			movups	[edi], xmm0
			movups	[edi + 10H], xmm0
			movups	[edi + 20H], xmm0
			movups	[edi + 30H], xmm0
		ELSE
			pxor	mm0, mm0
			movq	MMWORD PTR [edi], mm0
			movq	MMWORD PTR [edi + 08H], mm0
			movq	MMWORD PTR [edi + 10H], mm0
			movq	MMWORD PTR [edi + 18H], mm0
			movq	MMWORD PTR [edi + 20H], mm0
			movq	MMWORD PTR [edi + 28H], mm0
			movq	MMWORD PTR [edi + 30H], mm0
			movq	MMWORD PTR [edi + 38H], mm0
		ENDIF
Label_Continue:
		add	edi, 40H
		add	esi, 4 * 4
		sub	ecx, 16
	.ENDW
	;
	add	ecx, 16
	.IF	!ZERO?
		mov	eax, DWORD PTR [esi]
		mov	ebp, DWORD PTR [esi + 4]
		mov	edx, [ebx].m_dwPrevLineBytes
		test	eax, eax
		.IF	!ZERO?
			test	ebp, ebp
			.IF	!ZERO?
				movq	mm7, mmxMask_Byte7F
				.REPEAT
					movd	mm0, DWORD PTR [eax]
					add	eax, 4
					movd	mm1, DWORD PTR [ebp]
					add	ebp, 4
					psrlq	mm0, 1
					psrlq	mm1, 1
					pand	mm0, mm7
					pand	mm1, mm7
					paddb	mm0, mm1
					movd	DWORD PTR [edi], mm0
					add	edi, 4
					dec	ecx
				.UNTIL	ZERO?
				mov	eax, DWORD PTR [esi]
				mov	ebp, DWORD PTR [esi + 4]
				add	eax, [ebx].m_dwPrevLineBytes
				add	ebp, [ebx].m_dwNextLineBytes
				mov	DWORD PTR [esi], eax
				mov	DWORD PTR [esi + 4], ebp
			.ELSE
				add	DWORD PTR [esi], edx
				.REPEAT
					movd	mm0, DWORD PTR [eax]
					add	eax, 4
					movd	DWORD PTR [edi], mm0
					add	edi, 4
					dec	ecx
				.UNTIL	ZERO?
			.ENDIF
		.ELSE
			test	ebp, ebp
			mov	edx, [ebx].m_dwNextLineBytes
			.IF	!ZERO?
				add	DWORD PTR [esi + 4], edx
				.REPEAT
					movd	mm0, DWORD PTR [ebp]
					add	ebp, 4
					movd	DWORD PTR [edi], mm0
					add	edi, 4
					dec	ecx
				.UNTIL	ZERO?
			.ELSE
				xor	eax, eax
				.REPEAT
					mov	DWORD PTR [edi], eax
					add	edi, 4
					dec	ecx
				.UNTIL	ZERO?
			.ENDIF
		.ENDIF
		add	esi, 4 * 4
	.ENDIF
	pop	ebp
	;
	mov	ecx, nLeftCurBlockLines
	dec	ecx
	.IF	ZERO?
		mov	ecx, 16
		mov	ptrLineBlockAddrs, esi
	.ENDIF
	mov	nLeftCurBlockLines, ecx

ENDM

;
;	ブロック動き補償
; ----------------------------------------------------------------------------
; 整数画素
ALIGN	10H
Func_MovePBlock0_SSE:
;	prefetchnta	[edi + 0A0H]
	movq	mm0, MMWORD PTR [eax]
	movq	mm1, MMWORD PTR [eax + 08H]
	movq	mm2, MMWORD PTR [eax + 10H]
	movq	mm3, MMWORD PTR [eax + 18H]
;	prefetchnta	[eax + edx]
	movq	mm4, MMWORD PTR [eax + 20H]
	movq	mm5, MMWORD PTR [eax + 28H]
	movq	mm6, MMWORD PTR [eax + 30H]
	movq	mm7, MMWORD PTR [eax + 38H]
;	prefetchnta	[eax + edx + 20H]
	movq	MMWORD PTR [edi], mm0
	movq	MMWORD PTR [edi + 08H], mm1
	movq	MMWORD PTR [edi + 10H], mm2
	movq	MMWORD PTR [edi + 18H], mm3
	movq	MMWORD PTR [edi + 20H], mm4
	add	eax, edx
	movq	MMWORD PTR [edi + 28H], mm5
	movq	MMWORD PTR [edi + 30H], mm6
	movq	MMWORD PTR [edi + 38H], mm7
	ret

Func_MovePBlock0_MMX:
	movq	mm0, MMWORD PTR [eax]
	movq	mm1, MMWORD PTR [eax + 08H]
	movq	mm2, MMWORD PTR [eax + 10H]
	movq	mm3, MMWORD PTR [eax + 18H]
	movq	mm4, MMWORD PTR [eax + 20H]
	movq	mm5, MMWORD PTR [eax + 28H]
	movq	mm6, MMWORD PTR [eax + 30H]
	movq	mm7, MMWORD PTR [eax + 38H]
	movq	MMWORD PTR [edi], mm0
	movq	MMWORD PTR [edi + 08H], mm1
	movq	MMWORD PTR [edi + 10H], mm2
	movq	MMWORD PTR [edi + 18H], mm3
	movq	MMWORD PTR [edi + 20H], mm4
	add	eax, edx
	movq	MMWORD PTR [edi + 28H], mm5
	movq	MMWORD PTR [edi + 30H], mm6
	movq	MMWORD PTR [edi + 38H], mm7
	ret

; 水平方向半画素精度
ALIGN	10H
Func_MovePBlock1_SSE:
;	prefetchnta	[edi + 0A0H]
	movq	mm0, MMWORD PTR [eax]
	movq	mm1, MMWORD PTR [eax + 08H]
	movq	mm2, MMWORD PTR [eax + 10H]
		pavgb	mm0, MMWORD PTR [eax + 04H]
	movq	mm3, MMWORD PTR [eax + 18H]
		pavgb	mm1, MMWORD PTR [eax + 0CH]
;	prefetchnta	[eax + edx]
	movq	mm4, MMWORD PTR [eax + 20H]
		pavgb	mm2, MMWORD PTR [eax + 14H]
	movq	mm5, MMWORD PTR [eax + 28H]
		pavgb	mm3, MMWORD PTR [eax + 1CH]
	movq	mm6, MMWORD PTR [eax + 30H]
		pavgb	mm4, MMWORD PTR [eax + 24H]
	movq	mm7, MMWORD PTR [eax + 38H]
		pavgb	mm5, MMWORD PTR [eax + 2CH]
;	prefetchnta	[eax + edx + 20H]
	movq	MMWORD PTR [edi], mm0
		pavgb	mm6, MMWORD PTR [eax + 34H]
	movq	MMWORD PTR [edi + 08H], mm1
		pavgb	mm7, MMWORD PTR [eax + 3CH]
	movq	MMWORD PTR [edi + 10H], mm2
	movq	MMWORD PTR [edi + 18H], mm3
	movq	MMWORD PTR [edi + 20H], mm4
	add	eax, edx
	movq	MMWORD PTR [edi + 28H], mm5
	movq	MMWORD PTR [edi + 30H], mm6
	movq	MMWORD PTR [edi + 38H], mm7
	ret

Func_MovePBlock1_MMX:
	movq	mm7, mmxMask_Byte7F
	@INDEX = 0
	REPEAT	4
		movq	mm0, MMWORD PTR [eax + @INDEX]
			movq	mm1, MMWORD PTR [eax + @INDEX + 04H]
		movq	mm2, MMWORD PTR [eax + @INDEX + 08H]
		psrlq	mm0, 1
			movq	mm3, MMWORD PTR [eax + @INDEX + 0CH]
		psrlq	mm1, 1
		pand	mm0, mm7
		psrlq	mm2, 1
		pand	mm1, mm7
		psrlq	mm3, 1
		pand	mm2, mm7
		pand	mm3, mm7
		paddb	mm0, mm1
		paddb	mm2, mm3
		movq	MMWORD PTR [edi + @INDEX], mm0
		movq	MMWORD PTR [edi + @INDEX + 08H], mm2
		@INDEX = @INDEX + 10H
	ENDM
	add	eax, edx
	ret

; 垂直方向半整数画素
ALIGN	10H
Func_MovePBlock2_SSE:
;	prefetchnta	[edi + 0A0H]
	movq	mm0, MMWORD PTR [eax]
	movq	mm1, MMWORD PTR [eax + 08H]
	movq	mm2, MMWORD PTR [eax + 10H]
		pavgb	mm0, MMWORD PTR [eax + edx]
	movq	mm3, MMWORD PTR [eax + 18H]
		pavgb	mm1, MMWORD PTR [eax + edx + 08H]
;	prefetchnta	[eax + edx * 2]
	movq	mm4, MMWORD PTR [eax + 20H]
		pavgb	mm2, MMWORD PTR [eax + edx + 10H]
	movq	mm5, MMWORD PTR [eax + 28H]
		pavgb	mm3, MMWORD PTR [eax + edx + 18H]
	movq	mm6, MMWORD PTR [eax + 30H]
		pavgb	mm4, MMWORD PTR [eax + edx + 20H]
	movq	mm7, MMWORD PTR [eax + 38H]
		pavgb	mm5, MMWORD PTR [eax + edx + 28H]
;	prefetchnta	[eax + edx * 2 + 20H]
	movq	MMWORD PTR [edi], mm0
		pavgb	mm6, MMWORD PTR [eax + edx + 30H]
	movq	MMWORD PTR [edi + 08H], mm1
		pavgb	mm7, MMWORD PTR [eax + edx + 38H]
	movq	MMWORD PTR [edi + 10H], mm2
	movq	MMWORD PTR [edi + 18H], mm3
	movq	MMWORD PTR [edi + 20H], mm4
	add	eax, edx
	movq	MMWORD PTR [edi + 28H], mm5
	movq	MMWORD PTR [edi + 30H], mm6
	movq	MMWORD PTR [edi + 38H], mm7
	ret

Func_MovePBlock2_MMX:
	movq	mm7, mmxMask_Byte7F
	@INDEX = 0
	REPEAT	4
		movq	mm0, MMWORD PTR [eax + @INDEX]
			movq	mm1, MMWORD PTR [eax + edx + @INDEX]
		movq	mm2, MMWORD PTR [eax + @INDEX + 08H]
		psrlq	mm0, 1
			movq	mm3, MMWORD PTR [eax + edx + @INDEX + 08H]
		psrlq	mm1, 1
		pand	mm0, mm7
		psrlq	mm2, 1
		pand	mm1, mm7
		psrlq	mm3, 1
		pand	mm2, mm7
		pand	mm3, mm7
		paddb	mm0, mm1
		paddb	mm2, mm3
		movq	MMWORD PTR [edi + @INDEX], mm0
		movq	MMWORD PTR [edi + @INDEX + 08H], mm2
		@INDEX = @INDEX + 10H
	ENDM
	add	eax, edx
	ret

; 水平・垂直方向半整数画素
ALIGN	10H
Func_MovePBlock3_SSE:
;	prefetchnta	[edi + 0A0H]
	@INDEX = 0
	REPEAT	2
		movq	mm0, MMWORD PTR [eax + @INDEX]
;		prefetchnta	[eax + edx * 2 + @INDEX]
		movq	mm4, MMWORD PTR [eax + edx + @INDEX]
		movq	mm1, MMWORD PTR [eax + @INDEX + 08H]
			pavgb	mm0, MMWORD PTR [eax + @INDEX + 04H]
		movq	mm5, MMWORD PTR [eax + edx + @INDEX + 08H]
			pavgb	mm4, MMWORD PTR [eax + edx + @INDEX + 04H]
		movq	mm2, MMWORD PTR [eax + @INDEX + 10H]
			pavgb	mm1, MMWORD PTR [eax + @INDEX + 0CH]
		movq	mm6, MMWORD PTR [eax + edx + @INDEX + 10H]
			pavgb	mm5, MMWORD PTR [eax + edx + @INDEX + 0CH]
		movq	mm3, MMWORD PTR [eax + @INDEX + 18H]
			pavgb	mm2, MMWORD PTR [eax + @INDEX + 14H]
		movq	mm7, MMWORD PTR [eax + edx + @INDEX + 18H]
			pavgb	mm6, MMWORD PTR [eax + edx + @INDEX + 14H]
		pavgb	mm0, mm4
			pavgb	mm3, MMWORD PTR [eax + @INDEX + 1CH]
		pavgb	mm1, mm5
			pavgb	mm7, MMWORD PTR [eax + edx + @INDEX + 14H]
		movq	MMWORD PTR [edi + @INDEX], mm0
		pavgb	mm2, mm6
		movq	MMWORD PTR [edi + @INDEX + 08H], mm1
		pavgb	mm3, mm7
		movq	MMWORD PTR [edi + @INDEX + 10H], mm2
		movq	MMWORD PTR [edi + @INDEX + 18H], mm3
		@INDEX = @INDEX + 20H
	ENDM
	add	eax, edx
	ret

	ASSUME	ebx:NOTHING

;
;	双方向ブロック動き補償
; ----------------------------------------------------------------------------
; 整数画素
ALIGN	10H
Func_MoveBBlock0_SSE:
	movq	mm0, MMWORD PTR [eax]
	movq	mm1, MMWORD PTR [eax + 08H]
	movq	mm2, MMWORD PTR [eax + 10H]
		pavgb	mm0, MMWORD PTR [edi]
	movq	mm3, MMWORD PTR [eax + 18H]
		pavgb	mm1, MMWORD PTR [edi + 08H]
;	prefetchnta	[eax + edx]
	movq	mm4, MMWORD PTR [eax + 20H]
		pavgb	mm2, MMWORD PTR [edi + 10H]
	movq	mm5, MMWORD PTR [eax + 28H]
		pavgb	mm3, MMWORD PTR [edi + 18H]
	movq	mm6, MMWORD PTR [eax + 30H]
		pavgb	mm4, MMWORD PTR [edi + 20H]
	movq	mm7, MMWORD PTR [eax + 38H]
		pavgb	mm5, MMWORD PTR [edi + 28H]
;	prefetchnta	[eax + edx + 20H]
	movq	MMWORD PTR [edi], mm0
		pavgb	mm6, MMWORD PTR [edi + 30H]
	movq	MMWORD PTR [edi + 08H], mm1
		pavgb	mm7, MMWORD PTR [edi + 38H]
	movq	MMWORD PTR [edi + 10H], mm2
	movq	MMWORD PTR [edi + 18H], mm3
	movq	MMWORD PTR [edi + 20H], mm4
	add	eax, edx
	movq	MMWORD PTR [edi + 28H], mm5
	movq	MMWORD PTR [edi + 30H], mm6
	movq	MMWORD PTR [edi + 38H], mm7
	ret

Func_MoveBBlock0_MMX:
	movq	mm7, mmxMask_Byte7F
	@INDEX = 0
	REPEAT	4
		movq	mm0, MMWORD PTR [eax + @INDEX]
		movq	mm2, MMWORD PTR [ebp + @INDEX]
		movq	mm1, MMWORD PTR [eax + @INDEX + 8]
		movq	mm3, MMWORD PTR [ebp + @INDEX + 8]
		psrlq	mm0, 1
		psrlq	mm2, 1
		psrlq	mm1, 1
			pand	mm0, mm7
		psrlq	mm3, 1
			pand	mm2, mm7
		pand	mm1, mm7
		pand	mm3, mm7
		paddb	mm0, mm2
		paddb	mm1, mm3
		movq	MMWORD PTR [edi + @INDEX], mm0
		movq	MMWORD PTR [edi + @INDEX + 8], mm1
		@INDEX = @INDEX + 16
	ENDM
	add	eax, edx
	add	ebp, (ERISADecoder PTR [ebx]).m_dwNextLineBytes
	ret

; 水平方向半画素精度
ALIGN	10H
Func_MoveBBlock1_SSE:
	movq	mm0, MMWORD PTR [eax]
	movq	mm1, MMWORD PTR [eax + 08H]
	movq	mm2, MMWORD PTR [eax + 10H]
		pavgb	mm0, MMWORD PTR [eax + 04H]
	movq	mm3, MMWORD PTR [eax + 18H]
		pavgb	mm1, MMWORD PTR [eax + 0CH]
;	prefetchnta	[eax + edx]
	movq	mm4, MMWORD PTR [eax + 20H]
		pavgb	mm2, MMWORD PTR [eax + 14H]
	movq	mm5, MMWORD PTR [eax + 28H]
		pavgb	mm3, MMWORD PTR [eax + 1CH]
	movq	mm6, MMWORD PTR [eax + 30H]
		pavgb	mm4, MMWORD PTR [eax + 24H]
	movq	mm7, MMWORD PTR [eax + 38H]
		pavgb	mm5, MMWORD PTR [eax + 2CH]
;	prefetchnta	[eax + edx + 20H]
			pavgb	mm0, MMWORD PTR [edi]
		pavgb	mm6, MMWORD PTR [eax + 34H]
			pavgb	mm1, MMWORD PTR [edi + 08H]
		pavgb	mm7, MMWORD PTR [eax + 3CH]
	movq	MMWORD PTR [edi], mm0
			pavgb	mm2, MMWORD PTR [edi + 10H]
	movq	MMWORD PTR [edi + 08H], mm1
			pavgb	mm3, MMWORD PTR [edi + 18H]
	movq	MMWORD PTR [edi + 10H], mm2
			pavgb	mm4, MMWORD PTR [edi + 20H]
	movq	MMWORD PTR [edi + 18H], mm3
			pavgb	mm5, MMWORD PTR [edi + 28H]
	movq	MMWORD PTR [edi + 20H], mm4
			pavgb	mm6, MMWORD PTR [edi + 30H]
	add	eax, edx
	movq	MMWORD PTR [edi + 28H], mm5
			pavgb	mm7, MMWORD PTR [edi + 38H]
	movq	MMWORD PTR [edi + 30H], mm6
	movq	MMWORD PTR [edi + 38H], mm7
	ret

; 垂直方向半整数画素
ALIGN	10H
Func_MoveBBlock2_SSE:
	movq	mm0, MMWORD PTR [eax]
	movq	mm1, MMWORD PTR [eax + 08H]
	movq	mm2, MMWORD PTR [eax + 10H]
		pavgb	mm0, MMWORD PTR [eax + edx]
	movq	mm3, MMWORD PTR [eax + 18H]
		pavgb	mm1, MMWORD PTR [eax + edx + 08H]
;	prefetchnta	[eax + edx * 2]
	movq	mm4, MMWORD PTR [eax + 20H]
		pavgb	mm2, MMWORD PTR [eax + edx + 10H]
	movq	mm5, MMWORD PTR [eax + 28H]
		pavgb	mm3, MMWORD PTR [eax + edx + 18H]
	movq	mm6, MMWORD PTR [eax + 30H]
		pavgb	mm4, MMWORD PTR [eax + edx + 20H]
	movq	mm7, MMWORD PTR [eax + 38H]
		pavgb	mm5, MMWORD PTR [eax + edx + 28H]
;	prefetchnta	[eax + edx * 2 + 20H]
			pavgb	mm0, MMWORD PTR [edi]
		pavgb	mm6, MMWORD PTR [eax + edx + 30H]
			pavgb	mm1, MMWORD PTR [edi + 08H]
		pavgb	mm7, MMWORD PTR [eax + edx + 38H]
	movq	MMWORD PTR [edi], mm0
			pavgb	mm2, MMWORD PTR [edi + 10H]
	movq	MMWORD PTR [edi + 08H], mm1
			pavgb	mm3, MMWORD PTR [edi + 18H]
	movq	MMWORD PTR [edi + 10H], mm2
			pavgb	mm4, MMWORD PTR [edi + 20H]
	movq	MMWORD PTR [edi + 18H], mm3
			pavgb	mm5, MMWORD PTR [edi + 28H]
	movq	MMWORD PTR [edi + 20H], mm4
			pavgb	mm6, MMWORD PTR [edi + 30H]
	add	eax, edx
	movq	MMWORD PTR [edi + 28H], mm5
			pavgb	mm7, MMWORD PTR [edi + 38H]
	movq	MMWORD PTR [edi + 30H], mm6
	movq	MMWORD PTR [edi + 38H], mm7
	ret

; 水平・垂直方向半整数画素
ALIGN	10H
Func_MoveBBlock3_SSE:
	@INDEX = 0
	REPEAT	2
		movq	mm0, MMWORD PTR [eax + @INDEX]
;		prefetchnta	[eax + edx * 2 + @INDEX]
		movq	mm4, MMWORD PTR [eax + edx + @INDEX]
		movq	mm1, MMWORD PTR [eax + @INDEX + 08H]
			pavgb	mm0, MMWORD PTR [eax + @INDEX + 04H]
		movq	mm5, MMWORD PTR [eax + edx + @INDEX + 08H]
			pavgb	mm4, MMWORD PTR [eax + edx + @INDEX + 04H]
		movq	mm2, MMWORD PTR [eax + @INDEX + 10H]
			pavgb	mm1, MMWORD PTR [eax + @INDEX + 0CH]
		movq	mm6, MMWORD PTR [eax + edx + @INDEX + 10H]
			pavgb	mm5, MMWORD PTR [eax + edx + @INDEX + 0CH]
		movq	mm3, MMWORD PTR [eax + @INDEX + 18H]
			pavgb	mm2, MMWORD PTR [eax + @INDEX + 14H]
		movq	mm7, MMWORD PTR [eax + edx + @INDEX + 18H]
			pavgb	mm6, MMWORD PTR [eax + edx + @INDEX + 14H]
		pavgb	mm0, mm4
			pavgb	mm3, MMWORD PTR [eax + @INDEX + 1CH]
		pavgb	mm1, mm5
				pavgb	mm0, MMWORD PTR [edi + @INDEX]
			pavgb	mm7, MMWORD PTR [eax + edx + @INDEX + 14H]
				pavgb	mm1, MMWORD PTR [edi + @INDEX + 08H]
		pavgb	mm2, mm6
		pavgb	mm3, mm7
				pavgb	mm2, MMWORD PTR [edi + @INDEX + 10H]
		movq	MMWORD PTR [edi + @INDEX], mm0
				pavgb	mm3, MMWORD PTR [edi + @INDEX + 18H]
		movq	MMWORD PTR [edi + @INDEX + 08H], mm1
		movq	MMWORD PTR [edi + @INDEX + 10H], mm2
		movq	MMWORD PTR [edi + @INDEX + 18H], mm3
		@INDEX = @INDEX + 20H
	ENDM
	add	eax, edx
	ret

	ASSUME	ebx:NOTHING

;
;	RGB 差分画像の出力
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@LS_RestoreDeltaRGB24	\
	PROC	NEAR32 SYSCALL USES ebx esi edi

	LOCAL	ptrLineBlockAddrs:PTR DWORD
	LOCAL	nLeftCurBlockLines:DWORD
	LOCAL	yLoop:DWORD, nLastPixels:DWORD
	LOCAL	ptrDstLine:PTR, ptrDstNext:PTR
	LOCAL	ptrSrcLine:PTR, ptrSrcNext:PTR

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder
	;
	.IF	([ebx].m_nDstPixelBytes == 4) && \
			([ebx].m_nYUVPixelBytes == 4) && \
			(ERI_EnabledProcessorType & \
				(ERI_USE_XMM_P3 OR ERI_USE_MMX_PENTIUM))
		;
		;	MMX/SSE 専用出力コード
		; ------------------------------------------------------------
		mov	ecx, [ebx].m_ptrNextPrevBlocks
		mov	nLeftCurBlockLines, 16
		mov	ptrLineBlockAddrs, ecx
		;
		mov	esi, [ebx].m_ptrBlockLineBuf
		mov	edi, [ebx].m_ptrDstBlock
		mov	ecx, [ebx].m_nDstHeight
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	yLoop, ecx
			mov	ptrDstLine, edi
			mov	ptrSrcLine, esi
			;
			; 動き補償付画像転送
			;
			.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
				@ERISADecoder@@MoveLineWithVector	1
			.ELSE;	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
				@ERISADecoder@@MoveLineWithVector	0
			.ENDIF
			;
			; 中間バッファの配列変換
			;
			mov	esi, ptrSrcLine
			mov	edi, [ebx].m_ptrImageBuf
			@ArrangeYUVTemporaryBuffer	1
			;
			; YUV->RGB 変換＆出力
			;
			mov	ecx, ptrDstLine
			mov	edi, [ebx].m_ptrImageBuf
			mov	esi, [ebx].m_nDstWidth
			;
			.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
				@ConvertImageYUVtoRGB_SSE	1, 1, 1
			.ELSE;	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
				@ConvertImageYUVtoRGB_SSE	0, 1, 1
			.ENDIF
			;
			mov	ptrDstNext, ecx
			mov	ptrSrcNext, edi
			mov	nLastPixels, esi
			@ConvertImageYUVtoRGB_486	0
			;
			; ストア
			;
			mov	esi, ptrSrcNext
			mov	edi, ptrDstNext
			mov	ecx, nLastPixels
			mov	eax, [ebx].m_nDstLineBytes
			pxor	mm7, mm7
			test	ecx, ecx
			.WHILE	!ZERO?
				movd	mm4, DWORD PTR [esi]
				pxor	mm1, mm1
				movd	mm0, DWORD PTR [edi]
				add	esi, 4
				;
				punpcklbw	mm1, mm4
				punpcklbw	mm0, mm7
				psraw	mm1, 7
				paddsw	mm0, mm1
				packuswb	mm0, mm7
				;
				movd	DWORD PTR [edi], mm0
				add	edi, 4
				dec	ecx
			.ENDW
Loop_Continue:		;
			mov	esi, ptrSrcLine
			mov	edi, ptrDstLine
			mov	ecx, yLoop
			add	esi, 16*4
			add	edi, [ebx].m_nDstLineBytes
			dec	ecx
		.ENDW
	.ELSE
		;
		;	486 専用出力コード
		; ------------------------------------------------------------
		call	ERISADecoder@@MoveImageWithVector
		mov	ecx, ebx
		push	dfDifferential
		call	ERISADecoder@@ConvertImageYUVtoRGB
		;
		mov	ecx, [ebx].m_nDstHeight
		mov	esi, [ebx].m_ptrYUVImage
		mov	edi, [ebx].m_ptrDstBlock
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	yLoop, ecx
			mov	ptrDstLine, edi
			mov	ptrSrcLine, esi
			;
			mov	ecx, [ebx].m_nDstWidth
			test	ecx, ecx
			.WHILE	!ZERO?
				FOR	@INDEX, <0, 1, 2>
					movsx	eax, SBYTE PTR [esi + @INDEX]
					movzx	edx, BYTE PTR [edi + @INDEX]
					shl	eax, 1
					add	eax, edx
					.IF	(DWORD PTR eax) > 0FFH
						not	eax
						sar	eax, 31
					.ENDIF
					mov	BYTE PTR [edi + @INDEX], al
				ENDM
				;
				add	esi, [ebx].m_nYUVPixelBytes
				add	edi, [ebx].m_nDstPixelBytes
				dec	ecx
			.ENDW
			;
			mov	esi, ptrSrcLine
			mov	edi, ptrDstLine
			mov	ecx, yLoop
			add	esi, [ebx].m_nYUVLineBytes
			add	edi, [ebx].m_nDstLineBytes
			dec	ecx
		.ENDW
	.ENDIF
	;
	ASSUME	ebx:NOTHING
	ret	0

ERISADecoder@@LS_RestoreDeltaRGB24	ENDP


;
;	RGBA 差分画像の出力
; ----------------------------------------------------------------------------
ALIGN	10H
ERISADecoder@@LS_RestoreDeltaRGBA32	\
	PROC	NEAR32 SYSCALL USES ebx esi edi

	LOCAL	ptrLineBlockAddrs:PTR DWORD
	LOCAL	nLeftCurBlockLines:DWORD
	LOCAL	yLoop:DWORD, nLastPixels:DWORD
	LOCAL	ptrDstLine:PTR, ptrDstNext:PTR
	LOCAL	ptrSrcLine:PTR, ptrSrcNext:PTR

	mov	ebx, ecx
	ASSUME	ebx:PTR ERISADecoder
	;
	.IF	([ebx].m_nDstPixelBytes == 4) && \
			([ebx].m_nYUVPixelBytes == 4) && \
			(ERI_EnabledProcessorType & \
				(ERI_USE_XMM_P3 OR ERI_USE_MMX_PENTIUM))
		;
		;	MMX/SSE 専用出力コード
		; ------------------------------------------------------------
		mov	ecx, [ebx].m_ptrNextPrevBlocks
		mov	nLeftCurBlockLines, 16
		mov	ptrLineBlockAddrs, ecx
		;
		mov	esi, [ebx].m_ptrBlockLineBuf
		mov	edi, [ebx].m_ptrDstBlock
		mov	ecx, [ebx].m_nDstHeight
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	yLoop, ecx
			mov	ptrDstLine, edi
			mov	ptrSrcLine, esi
			;
			; 動き補償付画像転送
			;
			.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
				@ERISADecoder@@MoveLineWithVector	1
			.ELSE;	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
				@ERISADecoder@@MoveLineWithVector	0
			.ENDIF
			;
			; 中間バッファの配列変換
			;
			mov	esi, ptrSrcLine
			mov	edi, [ebx].m_ptrImageBuf
			@ArrangeYUVTemporaryBuffer	1
			;
			; YUV->RGB 変換＆出力
			;
			mov	ecx, ptrDstLine
			mov	edi, [ebx].m_ptrImageBuf
			mov	esi, [ebx].m_nDstWidth
			;
			.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
				@ConvertImageYUVtoRGB_SSE	1, 1, 1
			.ELSE;	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
				@ConvertImageYUVtoRGB_SSE	0, 1, 1
			.ENDIF
			;
			mov	ptrDstNext, ecx
			mov	ptrSrcNext, edi
			mov	nLastPixels, esi
			@ConvertImageYUVtoRGB_486	0
			;
			; ストア
			;
			mov	esi, ptrSrcNext
			mov	edi, ptrDstNext
			mov	ecx, nLastPixels
			mov	eax, [ebx].m_nDstLineBytes
			pxor	mm7, mm7
			test	ecx, ecx
			.WHILE	!ZERO?
				movd	mm4, DWORD PTR [esi]
				pxor	mm1, mm1
				movd	mm0, DWORD PTR [edi]
				add	esi, 4
				;
				punpcklbw	mm1, mm4
				punpcklbw	mm0, mm7
				psraw	mm1, 7
				paddsw	mm0, mm1
				packuswb	mm0, mm7
				;
				movd	DWORD PTR [edi], mm0
				add	edi, 4
				dec	ecx
			.ENDW
Loop_Continue:		;
			mov	esi, ptrSrcLine
			mov	edi, ptrDstLine
			mov	ecx, yLoop
			add	esi, 16*4
			add	edi, [ebx].m_nDstLineBytes
			dec	ecx
		.ENDW
	.ELSE
		;
		;	486 専用出力コード
		; ------------------------------------------------------------
		call	ERISADecoder@@MoveImageWithVector
		mov	ecx, ebx
		push	dfDifferential
		call	ERISADecoder@@ConvertImageYUVtoRGB
		;
		mov	esi, [ebx].m_ptrYUVImage
		mov	edi, [ebx].m_ptrDstBlock
		mov	ecx, [ebx].m_nDstHeight
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	yLoop, ecx
			mov	ptrDstLine, edi
			mov	ptrSrcLine, esi
			;
			mov	ecx, [ebx].m_nDstWidth
			test	ecx, ecx
			.WHILE	!ZERO?
				FOR	@INDEX, <0, 1, 2, 4>
					movsx	eax, SBYTE PTR [esi + @INDEX]
					movzx	edx, BYTE PTR [edi + @INDEX]
					shl	eax, 1
					add	eax, edx
					.IF	(DWORD PTR eax) > 0FFH
						not	eax
						sar	eax, 31
					.ENDIF
					mov	BYTE PTR [edi + @INDEX], al
				ENDM
				;
				add	esi, 4
				add	edi, 4
				dec	ecx
			.ENDW
			;
			mov	esi, ptrSrcLine
			mov	edi, ptrDstLine
			mov	ecx, yLoop
			add	esi, [ebx].m_nYUVLineBytes
			add	edi, [ebx].m_nDstLineBytes
			dec	ecx
		.ENDW
	.ENDIF
	;
	ASSUME	ebx:NOTHING
	ret	0

ERISADecoder@@LS_RestoreDeltaRGBA32	ENDP


CodeSeg	ENDS

ENDIF	;	IFNDEF	COMPACT_NOA_DECODER

	END
