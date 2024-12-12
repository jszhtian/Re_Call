
; ****************************************************************************
;                        E R I S A - L i b r a r y
; ----------------------------------------------------------------------------
;         Copyright (C) 2000-2004 Leshade Entis. All rights reserved.
; ****************************************************************************


	.686
	.XMM
	.MODEL	FLAT

	INCLUDE	experi.inc
	INCLUDE	erisamatrix.inc
	INCLUDE	egl.inc

MMWORD	TYPEDEF	QWORD


IFNDEF	COMPACT_NOA_DECODER


; ----------------------------------------------------------------------------
;                            データセグメント
; ----------------------------------------------------------------------------

DataSeg	SEGMENT	PARA FLAT 'DATA'

;
;	定数テーブル
; ----------------------------------------------------------------------------

ALIGN	10H
ERI_r1by8	REAL8	0.125
ERI_rQuarter	REAL8	0.25
ERI_rHalf	REAL8	0.5
ERI_r2		REAL8	2.0
ERI_r4		REAL8	4.0
ERI_rCosPI4	REAL8	?				; = cos( π/4 )
ERI_r2CosPI4	REAL8	?				; = 2 * cos( π/4 )

;
; 行列係数配列 : k(n,i) = cos( (2*i+1) / (4*n) )
;
ALIGN	10H
ERI_DCTofK2	REAL4	2 DUP( ? )	; = cos( (2*i+1) / 8 )
ALIGN	10H
ERI_DCTofK4	REAL4	4 DUP( ? )	; = cos( (2*i+1) / 16 )
ERI_DCTofK8	REAL4	8 DUP( ? )	; = cos( (2*i+1) / 32 )
ERI_DCTofK16	REAL4	16 DUP( ? )	; = cos( (2*i+1) / 64 )
ERI_DCTofK32	REAL4	32 DUP( ? )	; = cos( (2*i+1) / 128 )
ERI_DCTofK64	REAL4	64 DUP( ? )	; = cos( (2*i+1) / 256 )
ERI_DCTofK128	REAL4	128 DUP( ? )	; = cos( (2*i+1) / 512 )
ERI_DCTofK256	REAL4	256 DUP( ? )	; = cos( (2*i+1) / 1024 )
ERI_DCTofK512	REAL4	512 DUP( ? )	; = cos( (2*i+1) / 2048 )
ERI_DCTofK1024	REAL4	1024 DUP( ? )	; = cos( (2*i+1) / 4096 )
ERI_DCTofK2048	REAL4	2048 DUP( ? )	; = cos( (2*i+1) / 8192 )

;
; 行列係数配列へのテーブル
;
ALIGN	10H
ERI_pMatrixDCTofK	LABEL	DWORD
		DWORD	0
		DWORD	OFFSET ERI_DCTofK2
		DWORD	OFFSET ERI_DCTofK4
		DWORD	OFFSET ERI_DCTofK8
		DWORD	OFFSET ERI_DCTofK16
		DWORD	OFFSET ERI_DCTofK32
		DWORD	OFFSET ERI_DCTofK64
		DWORD	OFFSET ERI_DCTofK128
		DWORD	OFFSET ERI_DCTofK256
		DWORD	OFFSET ERI_DCTofK512
		DWORD	OFFSET ERI_DCTofK1024
		DWORD	OFFSET ERI_DCTofK2048

;
; XMM 用 DCT 変換係数テーブル
;
ALIGN	10H
ERI_MatrixDCT4	LABEL	REAL4
		REAL4	16 DUP( ? )
ERI_MatrixIDCT4	LABEL	REAL4
		REAL4	16 DUP( ? )

xmmHalf		REAL4	4 DUP( 0.5 )

DataSeg	ENDS

ConstSeg	SEGMENT	PARA READONLY FLAT 'CONST'

ALIGN	10H
mmxMask_ByteMSB	LABEL	MMWORD
		BYTE	8 DUP( 80H )

ConstSeg	ENDS


; ----------------------------------------------------------------------------
;                             コードセグメント
; ----------------------------------------------------------------------------

CodeSeg	SEGMENT	PARA READONLY FLAT 'CODE'

;
;	行列テーブルの初期化
; ----------------------------------------------------------------------------
ALIGN	10H
eriInitializeMatrix	PROC	NEAR32 C USES ebx edi esi

	LOCAL	n:SDWORD
	LOCAL	dr:REAL8, ir:REAL8
	LOCAL	k:REAL8

	;
	; 特殊条件の定数を準備
	;
	fldpi
	fmul	ERI_rQuarter
	fcos
	fst	ERI_rCosPI4
	;
	fadd	st, st(0)
	fstp	ERI_r2CosPI4

	;
	; 行列係数配列初期化
	;
	mov	ecx, 1
	.WHILE	ecx < MAX_DCT_DEGREE
		;
		mov	eax, 1
		shl	eax, cl
		mov	n, eax
		mov	edi, ERI_pMatrixDCTofK[ecx * 4]
		;
		fldpi
		fild	n
		fmul	ERI_r4
		fdivp	st(1), st
		fst	ir
		fadd	st, st(0)
		fstp	dr
		;
		mov	eax, n
		.REPEAT
			fld	ir
			fld	st(0)
			fcos
			fstp	REAL4 PTR [edi]
			fadd	dr
			fstp	ir
			add	edi, (SIZEOF REAL4)
			dec	eax
		.UNTIL	ZERO?
		;
		inc	ecx
		;
	.ENDW
	;
	; XMM 用 DCT 行列初期化
	;
	lea	edi, ERI_MatrixDCT4
	xor	edx, edx
	.REPEAT
		lea	eax, [edx * 2 + 1]
		mov	n, eax
		fild	n
		fldpi
		fmulp	st(1), st
		fmul	ERI_r1by8
		fstp	k		; k = (edx * 2 + 1) * PI / (2 * 4)
		;
		xor	ecx, ecx
		.REPEAT
			mov	n, ecx
			fild	n
			fmul	k
			fcos
			.IF	ecx == 0
				fmul	ERI_rHalf
			.ENDIF
			fstp	REAL4 PTR [edi]
			add	edi, (SIZEOF REAL4)
			;
			inc	ecx
		.UNTIL	ecx >= 4
		;
		inc	edx
	.UNTIL	edx >= 4
	;
	; XMM 用 IDCT 行列初期化
	;
	lea	edi, ERI_MatrixIDCT4
	xor	edx, edx
	.REPEAT
		mov	n, edx
		fild	n
		fldpi
		fmulp	st(1), st
		fmul	ERI_r1by8
		fstp	k		; k = edx * PI / (2 * 4)
		;
		xor	ecx, ecx
		.REPEAT
			lea	eax, [ecx * 2 + 1]
			mov	n, eax
			fild	n
			fmul	k
			fcos
			fstp	REAL4 PTR [edi]
			add	edi, (SIZEOF REAL4)
			;
			inc	ecx
		.UNTIL	ecx >= 4
		;
		inc	edx
	.UNTIL	edx >= 4

	ret

eriInitializeMatrix	ENDP


;
;	実数丸め関数
; ----------------------------------------------------------------------------
ALIGN	10H
eriRoundR32ToInt	PROC	NEAR32 C , r:REAL4

	LOCAL	n:SDWORD

	fld	r
	fistp	n
	mov	eax, n
	ret

eriRoundR32ToInt	ENDP

ALIGN	10H
eriRoundR64ToLInt	PROC	NEAR32 C , r:REAL8

	LOCAL	n:QWord

	fld	r
	fistp	n
	mov	eax, DWORD PTR n[0]
	mov	edx, DWORD PTR n[4]
	ret

eriRoundR64ToLInt	ENDP

ALIGN	10H
eriRoundR32ToWordArray	PROC	NEAR32 C USES ebx esi edi,
	ptrDst:PTR SWORD, nStep:DWORD, ptrSrc:PTR REAL4, nCount:DWORD

	LOCAL	nValue:SDWORD

	mov	ecx, nCount
	mov	edx, nStep
	mov	edi, ptrDst
	mov	esi, ptrSrc
	shl	edx, 1
	;
	test	ecx, ecx
	.WHILE	!ZERO?
		fld	REAL4 PTR [esi]
		add	esi, (SIZEOF REAL4)
		fistp	nValue
		mov	eax, nValue
		.IF	(SDWORD PTR eax) < -8000H
			mov	eax, -8000H
		.ELSEIF	(SDWORD PTR eax) > 7FFFH
			mov	eax, 7FFFH
		.ENDIF
		mov	SWORD PTR [edi], ax
		add	edi, edx
		dec	ecx
	.ENDW
	;
	ret

eriRoundR32ToWordArray	ENDP


;
;	8 ビット符号無し整数配列を 32 ビット浮動小数点配列に変換
; ----------------------------------------------------------------------------
ALIGN	10H
eriConvertArrayByteToR32	PROC	NEAR32 C USES ebx esi edi,
	ptrDst:PTR REAL4, ptrSrc:PTR BYTE, nCount:SDWORD

	LOCAL	n:SDWORD

	mov	edi, ptrDst
	mov	esi, ptrSrc
	mov	ecx, nCount
	;
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
		sub	ecx, 8
		pxor	mm7, mm7
		.WHILE	!SIGN?
			movq	mm0, MMWORD PTR [esi]
			add	esi, 8
			movq	mm2, mm0
			punpcklbw	mm0, mm7
			punpckhbw	mm2, mm7
			movq	mm1, mm0
			movq	mm3, mm2
			punpcklwd	mm0, mm7
			punpckhwd	mm1, mm7
				cvtpi2ps	xmm0, mm0
			punpcklwd	mm2, mm7
				cvtpi2ps	xmm1, mm1
			punpckhwd	mm3, mm7
				cvtpi2ps	xmm2, mm2
				cvtpi2ps	xmm3, mm3
			movlhps	xmm0, xmm1
			movlhps	xmm2, xmm3
			movups	[edi]     , xmm0
			movups	[edi + 16], xmm2
			add	edi, 32
			sub	ecx, 8
		.ENDW
		emms
		add	ecx, 8
		jz	Label_Exit
	.ENDIF
	;
	test	ecx, ecx
	.WHILE	!ZERO?
		movzx	eax, BYTE PTR [esi]
		inc	esi
		mov	n, eax
		fild	n
		fstp	REAL4 PTR [edi]
		add	edi, 4
		dec	ecx
	.ENDW
	;
Label_Exit:
	ret

eriConvertArrayByteToR32	ENDP


;
;	8 ビット符号有り整数配列を 32 ビット浮動小数点配列に変換
; ----------------------------------------------------------------------------
ALIGN	10H
eriConvertArraySByteToR32	PROC	NEAR32 C USES ebx esi edi,
	ptrDst:PTR REAL4, ptrSrc:PTR SBYTE, nCount:SDWORD

	LOCAL	n:SDWORD

	mov	edi, ptrDst
	mov	esi, ptrSrc
	mov	ecx, nCount
	;
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
		sub	ecx, 8
		pxor	mm7, mm7
		.WHILE	!SIGN?
			movq	mm0, MMWORD PTR [esi]
			add	esi, 8
			movq	mm2, mm0
			punpcklbw	mm0, mm7
			punpckhbw	mm2, mm7
			movq	mm1, mm0
			movq	mm3, mm2
			punpcklwd	mm0, mm7
			punpckhwd	mm1, mm7
				pslld	mm0, 24
			punpcklwd	mm2, mm7
				pslld	mm1, 24
			psrad	mm0, 24
			psrad	mm1, 24
			punpckhwd	mm3, mm7
				pslld	mm2, 24
				pslld	mm3, 24
				cvtpi2ps	xmm0, mm0
				cvtpi2ps	xmm1, mm1
			psrad	mm2, 24
			psrad	mm3, 24
				cvtpi2ps	xmm2, mm2
				cvtpi2ps	xmm3, mm3
			movlhps	xmm0, xmm1
			movlhps	xmm2, xmm3
			movups	[edi]     , xmm0
			movups	[edi + 16], xmm2
			add	edi, 32
			sub	ecx, 8
		.ENDW
		emms
		add	ecx, 8
		jz	Label_Exit
	.ENDIF
	;
	test	ecx, ecx
	.WHILE	!ZERO?
		movsx	eax, SBYTE PTR [esi]
		inc	esi
		mov	n, eax
		fild	n
		fstp	REAL4 PTR [edi]
		add	edi, 4
		dec	ecx
	.ENDW
	;
Label_Exit:
	ret

eriConvertArraySByteToR32	ENDP


;
;	32 ビット浮動小数点配列を 8 ビット符号なし整数配列に変換
; ----------------------------------------------------------------------------
ALIGN	10H
eriConvertArrayR32ToByte	PROC	NEAR32 C USES ebx esi edi,
	ptrDst:PTR BYTE, ptrSrc:PTR REAL4, nCount:SDWORD

	LOCAL	n:SDWORD

	mov	esi, ptrSrc
	mov	edi, ptrDst
	mov	ecx, nCount
	;
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
		prefetchnta	[esi]
		sub	ecx, 8
		prefetchnta	[esi + 32]
		.WHILE	!SIGN?
			movups	xmm0, [esi]
			movups	xmm1, [esi + 16]
			add	esi, 32
			cvtps2pi	mm0, xmm0
			movhlps	xmm0, xmm0
			prefetchnta	[esi + 32]
			cvtps2pi	mm2, xmm1
			movhlps	xmm1, xmm1
			cvtps2pi	mm1, xmm0
			cvtps2pi	mm3, xmm1
			packssdw	mm0, mm1
			packssdw	mm2, mm3
			packuswb	mm0, mm2
			movntq	MMWORD PTR [edi], mm0
			add	edi, 8
			sub	ecx, 8
		.ENDW
		sfence
		emms
		add	ecx, 8
		jz	Label_Exit
	.ENDIF
	;
	test	ecx, ecx
	.WHILE	!ZERO?
		fld	REAL4 PTR [esi]
		fistp	n
		mov	eax, n
		add	esi, 4
		.IF	(DWORD PTR eax) > 0FFH
			not	eax
			sar	eax, 31
			and	eax, 0FFH
		.ENDIF
		mov	BYTE PTR [edi], al
		inc	edi
		dec	ecx
	.ENDW
	;
Label_Exit:
	ret

eriConvertArrayR32ToByte	ENDP


;
;	32 ビット浮動小数点配列を 8 ビット符号あり整数配列に変換
; ----------------------------------------------------------------------------
ALIGN	10H
eriConvertArrayR32ToSByte	PROC	NEAR32 C USES ebx esi edi,
	ptrDst:PTR SBYTE, ptrSrc:PTR REAL4, nCount:SDWORD

	LOCAL	n:SDWORD

	mov	esi, ptrSrc
	mov	edi, ptrDst
	mov	ecx, nCount
	;
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
		prefetchnta	[esi]
		sub	ecx, 8
		prefetchnta	[esi + 32]
		.WHILE	!SIGN?
			movups	xmm0, [esi]
			movups	xmm1, [esi + 16]
			add	esi, 32
			cvtps2pi	mm0, xmm0
			movhlps	xmm0, xmm0
			prefetchnta	[esi + 32]
			cvtps2pi	mm2, xmm1
			movhlps	xmm1, xmm1
			cvtps2pi	mm1, xmm0
			cvtps2pi	mm3, xmm1
			packssdw	mm0, mm1
			packssdw	mm2, mm3
			packsswb	mm0, mm2
			movntq	MMWORD PTR [edi], mm0
			add	edi, 8
			sub	ecx, 8
		.ENDW
		emms
		sfence
		add	ecx, 8
		jz	Label_Exit
	.ENDIF
	;
	test	ecx, ecx
	.WHILE	!ZERO?
		fld	REAL4 PTR [esi]
		fistp	n
		mov	eax, n
		add	eax, 80H
		add	esi, 4
		.IF	(DWORD PTR eax) > 0FFH
			not	eax
			sar	eax, 31
			and	eax, 0FFH
		.ENDIF
		sub	eax, 80H
		mov	BYTE PTR [edi], al
		inc	edi
		dec	ecx
	.ENDW
	;
Label_Exit:
	ret

eriConvertArrayR32ToSByte	ENDP


;
;	スカラ乗算
; ----------------------------------------------------------------------------
ALIGN	10H
eriScalarMultiply	PROC	NEAR32 C USES ebx esi edi,
		ptrDst:PTR REAL4,
		rScalar:REAL4,
		nCount:DWORD

	mov	ecx, nCount
	mov	esi, ptrDst
	;
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3

		movss	xmm7, rScalar
		shufps	xmm7, xmm7, 0
		;
		.WHILE	ecx >= 8
			movups	xmm0, [esi]
			movups	xmm1, [esi + 16]
			mulps	xmm0, xmm7
			mulps	xmm1, xmm7
			movups	[esi],      xmm0
			movups	[esi + 16], xmm1
			add	esi, 32
			sub	ecx, 8
		.ENDW

	.ENDIF
	;
	fld	REAL4 PTR rScalar
	;
	.WHILE	ecx >= 2
		fld	REAL4 PTR [esi]
		fld	REAL4 PTR [esi + 4]
		fxch	st(1)
		fmul	st, st(2)
		fxch	st(1)
		fmul	st, st(2)
		fxch
		fstp	REAL4 PTR [esi]
		fstp	REAL4 PTR [esi + 4]
		add	esi, 8
		sub	ecx, 2
	.ENDW
	;
	.IF	ecx != 0
		fld	REAL4 PTR [esi]
		fmul	st, st(1)
		fstp	REAL4 PTR [esi]
	.ENDIF
	;
	fstp	st(0)

	ret

eriScalarMultiply	ENDP


;
;	ベクトル乗算
; ----------------------------------------------------------------------------
ALIGN	10H
eriVectorMultiply	PROC	NEAR32 C USES ebx esi edi,
		ptrDst:PTR REAL4,
		ptrSrc:PTR REAL4,
		nCount:DWORD

	mov	esi, ptrSrc
	mov	edi, ptrDst
	mov	ecx, nCount
	;
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
		.IF	edi & 0FH
			sub	ecx, 8
			.WHILE	!SIGN?
				movups	xmm0, [edi]
				movups	xmm2, [esi]
				movups	xmm1, [edi + 16]
					mulps	xmm0, xmm2
				movups	xmm3, [esi + 16]
					mulps	xmm1, xmm3
				add	esi, 32
				;
				movups	[edi]     , xmm0
				movups	[edi + 16], xmm1
				add	edi, 32
				;
				sub	ecx, 8
			.ENDW
		.ELSE
			sub	ecx, 8
			.WHILE	!SIGN?
				movaps	xmm0, [edi]
				movups	xmm2, [esi]
				movaps	xmm1, [edi + 16]
					mulps	xmm0, xmm2
				movups	xmm3, [esi + 16]
					mulps	xmm1, xmm3
				add	esi, 32
				;
				movaps	[edi]     , xmm0
				movaps	[edi + 16], xmm1
				add	edi, 32
				;
				sub	ecx, 8
			.ENDW
		.ENDIF
		;
		add	ecx, 8
		.WHILE	!ZERO?
			movss	xmm0, REAL4 PTR [edi]
			movss	xmm1, REAL4 PTR [esi]
			mulss	xmm0, xmm1
			add	esi, 4
			movss	REAL4 PTR [edi], xmm0
			add	edi, 4
			dec	ecx
		.ENDW
		;
		ret

	.ELSE
		test	ecx, ecx
		.WHILE	!ZERO?
			fld	REAL4 PTR [edi]
			fmul	REAL4 PTR [esi]
			add	esi, 4
			fstp	REAL4 PTR [edi]
			add	edi, 4
			dec	ecx
		.ENDW
		;
		ret

	.ENDIF

eriVectorMultiply	ENDP


;
;	DCT 演算関数生成マクロ
; ----------------------------------------------------------------------------

@FastDCTProc	Macro	xmm_flag:Req
	LOCAL	DCTProcName, IDCTProcName

	IF	xmm_flag
		DCTProcName	TEXTEQU	<eriFastDCT_XMM>
		IDCTProcName	TEXTEQU	<eriFastIDCT_XMM>
	ELSE
		DCTProcName	TEXTEQU	<eriFastDCT_FPU>
		IDCTProcName	TEXTEQU	<eriFastIDCT_FPU>
	ENDIF

ALIGN	10H
DCTProcName	PROC	NEAR32 C USES ebx esi edi,
		ptrDst:PTR REAL4,
		nDstInterval:DWORD,
		ptrSrc:PTR REAL4,
		ptrWorkBuf:PTR REAL4,
		nDegreeDCT:DWORD

	LOCAL	nDegreeNum:DWORD, nHalfDegree:DWORD

	mov	eax, nDegreeDCT
	mov	edi, ptrDst
	mov	esi, ptrSrc
	mov	ebx, ptrWorkBuf
	mov	edx, nDstInterval
	;
	.IF	eax > 5
		;
		;	汎用 DCT 変換
		; ------------------------------------------------------------
		;              | I   J |
		; 交差加減算 = |       |
		;              | I  -J |
		mov	ecx, eax
		mov	eax, 1
		shl	eax, cl
		mov	nDegreeNum, eax
		shr	eax, 1
		mov	nHalfDegree, eax
		;
		xor	ecx, ecx
		mov	eax, nDegreeNum
		mov	edx, nHalfDegree
		dec	eax
		lea	edx, [ebx + edx * 4]
		.REPEAT
			fld	REAL4 PTR [esi + eax * 4]
			fld	REAL4 PTR [esi + ecx * 4]
			fld	st(1)
			fadd	st, st(1)
			fxch	st(2)
			fsubp	st(1), st
			fxch	st(1)
			fstp	REAL4 PTR [ebx + ecx * 4]
			fstp	REAL4 PTR [edx + ecx * 4]
			dec	eax
			inc	ecx
		.UNTIL	ecx >= nHalfDegree
		;
		; 前半　DCT ： A * DCT
		;
		mov	ecx, nDegreeDCT
		mov	edx, nDstInterval
		dec	ecx
		shl	edx, 1
		INVOKE	DCTProcName , edi, edx, ebx, esi, ecx
		mov	esi, ebx
		;
		; 後半 DCT-IV ： R * 2 * A * DCT * K
		;
		mov	eax, nDegreeDCT
		mov	ecx, nHalfDegree
		mov	edx, nDstInterval
		mov	eax, ERI_pMatrixDCTofK[eax * 4 - 4]
		lea	esi, [esi + ecx * (SIZEOF REAL4)]
		add	edi, edx
		mov	edx, nHalfDegree
		;
		IF	xmm_flag
			lea	edx, [edx * (SIZEOF REAL4)]
			xor	ecx, ecx
			.REPEAT
				movups	xmm0, [esi + ecx]
				movups	xmm1, [esi + ecx + 16]
				mulps	xmm0, [eax]
				mulps	xmm1, [eax + 16]
				movups	[esi + ecx],      xmm0
				movups	[esi + ecx + 16], xmm1
				add	ecx, (SIZEOF REAL4) * 8
				add	eax, (SIZEOF REAL4) * 8
			.UNTIL	ecx >= edx
		ELSE
			xor	ecx, ecx
			.REPEAT
				fld	REAL4 PTR [esi + ecx * 4]
				fld	REAL4 PTR [esi + ecx * 4 + 4]
				fxch	st(1)
				fmul	REAL4 PTR [eax]
				fxch	st(1)
				fmul	REAL4 PTR [eax + 4]
				fxch	st(1)
				add	eax, (SIZEOF REAL4) * 2
				fstp	REAL4 PTR [esi + ecx * 4]
				fstp	REAL4 PTR [esi + ecx * 4 + 4]
				add	ecx, 2
			.UNTIL	ecx >= edx
		ENDIF
		;
		mov	ecx, nDegreeDCT
		mov	edx, nDstInterval
		dec	ecx
		shl	edx, 1
		INVOKE	DCTProcName , edi, edx, esi, ebx, ecx
		;
		mov	edx, nDstInterval
		mov	ecx, nHalfDegree
		shl	edx, 1
		dec	ecx
		;
		fld	REAL4 PTR [edi]
		fadd	st(0), st
		fst	REAL4 PTR [edi]
		add	edi, edx
		;
		.REPEAT
			fld	REAL4 PTR [edi]
			fadd	st(0), st
			fsubrp	st(1), st
			fst	REAL4 PTR [edi]
			add	edi, edx
			dec	ecx
		.UNTIL	ZERO?
		;
		fstp	st(0)
		;
		mov	eax, 1
		ret

	.ELSEIF	eax == 3
		@FastDCT	3, xmm_flag
		mov	eax, 1
		ret

	.ELSEIF	eax == 4
		@FastDCT	4, xmm_flag
		mov	eax, 1
		ret

	.ELSEIF	eax == 5
		@FastDCT	5, xmm_flag
		mov	eax, 1
		ret

	.ELSEIF	eax == 2
		@FastDCT	2, xmm_flag
		mov	eax, 1
		ret

	.ELSE
		xor	eax, eax
		ret

	.ENDIF

DCTProcName	ENDP

ALIGN	10H
IDCTProcName	PROC	NEAR32 C USES ebx esi edi,
		ptrDst:PTR REAL4,
		ptrSrc:PTR REAL4,
		nSrcInterval:DWORD,
		ptrWorkBuf:PTR REAL4,
		nDegreeDCT:DWORD

	LOCAL	nDegreeNum:DWORD, nHalfDegree:DWORD

	mov	eax, nDegreeDCT
	mov	edi, ptrDst
	mov	esi, ptrSrc
	mov	ebx, ptrWorkBuf
	mov	edx, nSrcInterval
	;
	.IF	eax > 5
		;
		;	汎用 IDCT 変換
		; ------------------------------------------------------------
		;
		; 偶数行 ： IDCT
		;
		mov	ecx, eax
		mov	eax, 1
		shl	eax, cl
		mov	nDegreeNum, eax
		shr	eax, 1
		mov	nHalfDegree, eax
		;
		dec	ecx
		shl	edx, 1
		INVOKE	IDCTProcName , edi, esi, edx, ebx, ecx
		;
		; 奇数行 ： R * 2 * A * DCT * K
		;
		mov	ecx, nDegreeDCT
		mov	edx, nSrcInterval
		mov	eax, ERI_pMatrixDCTofK[ecx * 4 - 4]
		mov	ecx, nHalfDegree
		add	esi, edx
		lea	edi, [edi + ecx * (SIZEOF REAL4)]
		;
		shl	edx, 1
		xor	ecx, ecx
		.REPEAT
			fld	REAL4 PTR [esi]
			fld	REAL4 PTR [esi + edx]
			lea	esi, [esi + edx * 2]
			fxch	st(1)
			fmul	REAL4 PTR [eax]
			fxch	st(1)
			fmul	REAL4 PTR [eax + 4]
			fxch	st(1)
			add	eax, (SIZEOF REAL4) * 2
			fstp	REAL4 PTR [ebx + ecx * 4]
			fstp	REAL4 PTR [ebx + ecx * 4 + 4]
			add	ecx, 2
		.UNTIL	ecx >= nHalfDegree
		;
		mov	edx, nDegreeDCT
		mov	ecx, nHalfDegree
		mov	esi, ebx
		dec	edx
		lea	ebx, [ebx + ecx * (SIZEOF REAL4)]
		INVOKE	DCTProcName , edi, (SIZEOF REAL4), esi, ebx, edx
		;
		fld	REAL4 PTR [edi]
		fadd	st, st(0)
		fst	REAL4 PTR [edi]
		mov	ecx, 1
		.REPEAT
			fld	REAL4 PTR [edi + ecx * 4]
			fadd	st, st(0)
			fsubrp	st(1), st
			fst	REAL4 PTR [edi + ecx * 4]
			inc	ecx
		.UNTIL	ecx >= nHalfDegree
		;
		fstp	st(0)
		;              | I   I |
		; 交差加減算 = |       |
		;              | J  -J |
		mov	eax, nHalfDegree
		shl	eax, 2
		sub	edi, eax
		mov	ebx, nHalfDegree
		mov	esi, nDegreeNum
		mov	edx, ebx
		xor	eax, eax
		mov	ecx, -1
		lea	ebx, [edi + ebx * 4]
		lea	esi, [edi + esi * 4]
		shr	edx, 1
		.REPEAT
			fld	REAL4 PTR [ebx + eax * 4]
			fld	REAL4 PTR [edi + eax * 4]
			fld	st(1)
			fadd	st, st(1)
			fxch	st(2)
			fsubp	st(1), st
			;
			fld	REAL4 PTR [esi + ecx * 4]
			fld	REAL4 PTR [ebx + ecx * 4]
			fld	st(1)
			fadd	st, st(1)
			fxch	st(2)
			fsubp	st(1), st
			;
			fstp	REAL4 PTR [ebx + eax * 4]
			fstp	REAL4 PTR [ebx + ecx * 4]
			fstp	REAL4 PTR [esi + ecx * 4]
			fstp	REAL4 PTR [edi + eax * 4]
			;
			inc	eax
			dec	ecx
		.UNTIL	eax >= edx
		;
		xor	eax, eax
		ret

	.ELSEIF	eax == 3
		@FastIDCT	3, xmm_flag
		mov	eax, 1
		ret

	.ELSEIF	eax == 4
		@FastIDCT	4, xmm_flag
		mov	eax, 1
		ret

	.ELSEIF	eax == 5
		@FastIDCT	5, xmm_flag
		mov	eax, 1
		ret

	.ELSEIF	eax == 2
		@FastIDCT	2, xmm_flag
		mov	eax, 1
		ret

	.ELSE
		xor	eax, eax
		ret

	.ENDIF

IDCTProcName	ENDP

		EndM


;
;	DCT 変換関数生成
; ----------------------------------------------------------------------------

@FastDCTProc	0		; FPU 互換関数

@FastDCTProc	1		; XMM 専用関数


;
;	DCT 変換関数
; ----------------------------------------------------------------------------
ALIGN	10H
eriFastDCT		PROC	NEAR32 C ,
		ptrDst:PTR REAL4,
		nDstInterval:DWORD,
		ptrSrc:PTR REAL4,
		ptrWorkBuf:PTR REAL4,
		nDegreeDCT:DWORD

	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3

		mov	edx, nDstInterval
		INVOKE	eriFastDCT_XMM ,
				ptrDst, Addr [edx * (SIZEOF REAL4)],
				ptrSrc, ptrWorkBuf, nDegreeDCT
		ret

	.ELSE

		mov	edx, nDstInterval
		INVOKE	eriFastDCT_FPU ,
				ptrDst, Addr [edx * (SIZEOF REAL4)],
				ptrSrc, ptrWorkBuf, nDegreeDCT
		ret

	.ENDIF

eriFastDCT		ENDP


;
;	IDCT 変換関数
; ----------------------------------------------------------------------------
ALIGN	10H
eriFastIDCT		PROC	NEAR32 C ,
		ptrDst:PTR REAL4,
		ptrSrc:PTR REAL4,
		nSrcInterval:DWORD,
		ptrWorkBuf:PTR REAL4,
		nDegreeDCT:DWORD

	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3

		mov	edx, nSrcInterval
		INVOKE	eriFastIDCT_XMM ,
				ptrDst, ptrSrc,
				Addr [edx * (SIZEOF REAL4)],
				ptrWorkBuf, nDegreeDCT
		ret

	.ELSE

		mov	edx, nSrcInterval
		INVOKE	eriFastIDCT_FPU ,
				ptrDst, ptrSrc,
				Addr [edx * (SIZEOF REAL4)],
				ptrWorkBuf, nDegreeDCT
		ret

	.ENDIF

eriFastIDCT		ENDP

;
;	画像データ複製
; ----------------------------------------------------------------------------
ALIGN	10H
eriCopyImage	PROC	NEAR32 C USES ebx esi edi,
	eiiDst:PTR EGL_IMAGE_INFO, eiiSrc:PTR EGL_IMAGE_INFO

	LOCAL	nWidth:DWORD, nWidthBytes:DWORD
	LOCAL	nHeight:DWORD, nLeftHeight:DWORD
	LOCAL	fdwFormatType:DWORD
	LOCAL	nDstPixelBytes:DWORD, nSrcPixelBytes:DWORD
	LOCAL	nDstLineBytes:DWORD, nSrcLineBytes:DWORD
	LOCAL	pbytDstLine:PTR, pbytSrcLine:PTR

	;
	;	画像情報取得
	; --------------------------------------------------------------------
	mov	esi, eiiSrc
	mov	edi, eiiDst
	ASSUME	esi:PTR EGL_IMAGE_INFO
	ASSUME	edi:PTR EGL_IMAGE_INFO
	;
	mov	eax, [edi].dwImageWidth
	mov	edx, [edi].dwImageHeight
	.IF	(eax != [esi].dwImageWidth) || (edx != [esi].dwImageHeight)
		mov	eax, eslErrGeneral
		ret
	.ENDIF
	mov	nWidth, eax
	mov	nHeight, edx
	;
	mov	eax, [esi].dwBytesPerLine
	mov	edx, [edi].dwBytesPerLine
	mov	ecx, [edi].fdwFormatType
	mov	nSrcLineBytes, eax
	mov	nDstLineBytes, edx
	mov	fdwFormatType, ecx
	;
	mov	eax, [esi].dwBitsPerPixel
	mov	edx, [edi].dwBitsPerPixel
	.IF	((eax != 24) && (eax != 32)) || ((edx != 24) && (edx != 32))
		mov	eax, eslErrGeneral
		ret
	.ENDIF
	shr	eax, 3
	shr	edx, 3
	mov	nSrcPixelBytes, eax
	mov	nDstPixelBytes, edx
	;
	; 最適化判定
	;
	imul	eax, nWidth
	mov	esi, [esi].ptrImageArray
	mov	edi, [edi].ptrImageArray
	ASSUME	esi:NOTHING
	ASSUME	edi:NOTHING
	mov	pbytSrcLine, esi
	mov	pbytDstLine, edi
	mov	nWidthBytes, eax
	;
	.IF	esi != NULL
	;
	.IF	edx != nSrcPixelBytes
		;
		;	異ピクセル深度間での複製処理
		; ------------------------------------------------------------
		mov	ecx, nHeight
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	nLeftHeight, ecx
			mov	pbytSrcLine, esi
			mov	pbytDstLine, edi
			;
			mov	ecx, nWidth
			test	ecx, ecx
			.WHILE	!ZERO?
				mov	al, BYTE PTR [esi]
				mov	bl, BYTE PTR [esi + 1]
				mov	dl, BYTE PTR [esi + 2]
				add	esi, nSrcPixelBytes
				mov	BYTE PTR [edi], al
				mov	BYTE PTR [edi + 1], bl
				mov	BYTE PTR [edi + 2], dl
				add	edi, nDstPixelBytes
				dec	ecx
			.ENDW
			;
			mov	esi, pbytSrcLine
			mov	edi, pbytDstLine
			mov	ecx, nLeftHeight
			add	esi, nSrcLineBytes
			add	edi, nDstLineBytes
			dec	ecx
		.ENDW
		;
		xor	eax, eax
		ret
	.ENDIF
	.IF	(ERI_EnabledProcessorType & ERI_USE_XMM_P3) && !(eax & 07H)
		;
		;	SSE 専用複製ルーチン
		; ------------------------------------------------------------
		mov	ecx, nHeight
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	nLeftHeight, ecx
			mov	pbytSrcLine, esi
			mov	pbytDstLine, edi
			;
			mov	ecx, nWidthBytes
			test	ecx, ecx
			.WHILE	!ZERO?
				movq	mm0, MMWORD PTR [esi]
				add	esi, 8
				movntq	MMWORD PTR [edi], mm0
				add	edi, 8
				sub	ecx, 8
			.ENDW
			;
			mov	esi, pbytSrcLine
			mov	edi, pbytDstLine
			mov	ecx, nLeftHeight
			add	esi, nSrcLineBytes
			add	edi, nDstLineBytes
			dec	ecx
		.ENDW
		sfence
		emms
		xor	eax, eax
		ret
	.ENDIF
	;
	;	486 互換複製ルーチン
	; --------------------------------------------------------------------
	pushfd
	cld
	mov	ecx, nHeight
	test	ecx, ecx
	.WHILE	!ZERO?
		mov	nLeftHeight, ecx
		mov	pbytSrcLine, esi
		mov	pbytDstLine, edi
		;
		mov	ecx, nWidthBytes
		mov	edx, ecx
		shr	ecx, 2
		and	edx, 03H
		rep	movsd
		mov	ecx, edx
		rep	movsb
		;
		mov	esi, pbytSrcLine
		mov	edi, pbytDstLine
		mov	ecx, nLeftHeight
		add	esi, nSrcLineBytes
		add	edi, nDstLineBytes
		dec	ecx
	.ENDW
	popfd
	;
	xor	eax, eax
	ret

	.ENDIF

	;
	;	入力元がブランク画像
	; --------------------------------------------------------------------
	pushfd
	cld
	mov	ecx, nHeight
	test	ecx, ecx
	.WHILE	!ZERO?
		mov	nLeftHeight, ecx
		mov	pbytDstLine, edi
		;
		xor	eax, eax
		mov	ecx, nWidthBytes
		mov	edx, ecx
		shr	ecx, 2
		and	edx, 03H
		rep	stosd
		mov	ecx, edx
		rep	stosb
		;
		mov	edi, pbytDstLine
		mov	ecx, nLeftHeight
		add	edi, nDstLineBytes
		dec	ecx
	.ENDW
	popfd
	;
	xor	eax, eax
	ret

eriCopyImage	ENDP

;
;	2つの画像を 1:1 補完する
; ----------------------------------------------------------------------------
ALIGN	10H
eriBlendHalfImage	PROC	NEAR32 C USES ebx esi edi,
	eiiDst:PCEGL_IMAGE_INFO,
	eiiSrc1:PCEGL_IMAGE_INFO, eiiSrc2:PCEGL_IMAGE_INFO

	LOCAL	nWidth:DWORD
	LOCAL	nHeight:DWORD, nLeftHeight:DWORD
	LOCAL	fdwFormatType:DWORD
	LOCAL	nDstPixelBytes:DWORD
	LOCAL	nSrc1PixelBytes:DWORD, nSrc2PixelBytes:DWORD
	LOCAL	nDstLineBytes:DWORD
	LOCAL	nSrc1LineBytes:DWORD, nSrc2LineBytes:DWORD
	LOCAL	pbytDstLine:PTR, pbytSrc1Line:PTR, pbytSrc2Line:PTR

	;
	;	画像情報取得
	; --------------------------------------------------------------------
	mov	esi, eiiSrc1
	mov	ebx, eiiSrc2
	mov	edi, eiiDst
	ASSUME	esi:PTR EGL_IMAGE_INFO
	ASSUME	ebx:PTR EGL_IMAGE_INFO
	ASSUME	edi:PTR EGL_IMAGE_INFO
	;
	mov	eax, [edi].dwImageWidth
	mov	edx, [edi].dwImageHeight
	.IF	(eax != [esi].dwImageWidth) || (edx != [esi].dwImageHeight) \
		|| (eax != [ebx].dwImageWidth) || (edx != [ebx].dwImageHeight)
		mov	eax, eslErrGeneral
		ret
	.ENDIF
	mov	nWidth, eax
	mov	nHeight, edx
	;
	mov	eax, [esi].dwBytesPerLine
	mov	edx, [ebx].dwBytesPerLine
	mov	nSrc1LineBytes, eax
	mov	nSrc2LineBytes, edx
	mov	eax, [edi].dwBytesPerLine
	mov	edx, [edi].fdwFormatType
	mov	nDstLineBytes, eax
	mov	fdwFormatType, edx
	;
	mov	eax, [esi].dwBitsPerPixel
	mov	ecx, [ebx].dwBitsPerPixel
	mov	edx, [edi].dwBitsPerPixel
	.IF	((eax != 24) && (eax != 32)) \
			|| ((ecx != 24) && (ecx != 32)) \
			|| ((edx != 24) && (edx != 32))
		mov	eax, eslErrGeneral
		ret
	.ENDIF
	shr	eax, 3
	shr	ecx, 3
	shr	edx, 3
	mov	nSrc1PixelBytes, eax
	mov	nSrc2PixelBytes, ecx
	mov	nDstPixelBytes, edx
	;
	; 最適化判定
	;
	mov	esi, [esi].ptrImageArray
	mov	ebx, [ebx].ptrImageArray
	mov	edi, [edi].ptrImageArray
	ASSUME	esi:NOTHING
	ASSUME	ebx:NOTHING
	ASSUME	edi:NOTHING
	mov	pbytSrc1Line, esi
	mov	pbytSrc2Line, ebx
	mov	pbytDstLine, edi
	;
	.IF	(eax != 4) || (ecx != 4) || (edx != 4)
		;
		; 486 互換 RGB-24
		;
		mov	ecx, nHeight
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	nLeftHeight, ecx
			mov	pbytSrc1Line, esi
			mov	pbytSrc2Line, ebx
			mov	pbytDstLine, edi
			;
			mov	ecx, nWidth
			test	ecx, ecx
			.WHILE	!ZERO?
				mov	al, BYTE PTR [esi]
				mov	ah, BYTE PTR [esi + 1]
				mov	dl, BYTE PTR [esi + 2]
				add	esi, nSrc1PixelBytes
				add	al, BYTE PTR [ebx]
				rcr	al, 1
				add	ah, BYTE PTR [ebx + 1]
				rcr	ah, 1
				add	dl, BYTE PTR [ebx + 2]
				rcr	dl, 1
				add	ebx, nSrc2PixelBytes
				mov	BYTE PTR [edi], al
				mov	BYTE PTR [edi + 1], ah
				mov	BYTE PTR [edi + 2], dl
				add	edi, nDstPixelBytes
				dec	ecx
			.ENDW
			;
			mov	esi, pbytSrc1Line
			mov	ebx, pbytSrc2Line
			mov	edi, pbytDstLine
			mov	ecx, nLeftHeight
			add	esi, nSrc1LineBytes
			add	ebx, nSrc2LineBytes
			add	edi, nDstLineBytes
			dec	ecx
		.ENDW
		;
		xor	eax, eax
		ret
	.ENDIF
	;
	;
	; 486 互換 RGBA-32
	;
	mov	ecx, nHeight
	test	ecx, ecx
	.WHILE	!ZERO?
		mov	nLeftHeight, ecx
		mov	pbytSrc1Line, esi
		mov	pbytSrc2Line, ebx
		mov	pbytDstLine, edi
		;
		mov	ecx, nWidth
		push	ebp
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	eax, DWORD PTR [esi]
			add	esi, 4
			mov	edx, DWORD PTR [ebx]
			add	ebx, 4
			mov	ebp, eax
			shr	eax, 1
			and	ebp, edx
			shr	edx, 1
			and	eax, 7F7F7F7FH
			and	edx, 7F7F7F7FH
			and	ebp, 01010101H
			add	eax, edx
			add	eax, ebp
			mov	DWORD PTR [edi], eax
			add	edi, 4
			dec	ecx
		.ENDW
		pop	ebp
		;
		mov	esi, pbytSrc1Line
		mov	ebx, pbytSrc2Line
		mov	edi, pbytDstLine
		mov	ecx, nLeftHeight
		add	esi, nSrc1LineBytes
		add	ebx, nSrc2LineBytes
		add	edi, nDstLineBytes
		dec	ecx
	.ENDW
	;
	xor	eax, eax
	ret

eriBlendHalfImage	ENDP

;
;	非可逆圧縮・差分フレーム加算
; ----------------------------------------------------------------------------
ALIGN	10H
eriLSAdditionOfFrame	PROC	NEAR32 C USES ebx esi edi,
	eiiDst:PTR EGL_IMAGE_INFO, eiiSrc:PTR EGL_IMAGE_INFO

	LOCAL	nWidth:DWORD
	LOCAL	nHeight:DWORD, nLeftHeight:DWORD
	LOCAL	fdwFormatType:DWORD
	LOCAL	nDstPixelBytes:DWORD, nSrcPixelBytes:DWORD
	LOCAL	nDstLineBytes:DWORD, nSrcLineBytes:DWORD
	LOCAL	pbytDstLine:PTR, pbytSrcLine:PTR

	;
	;	画像情報取得
	; --------------------------------------------------------------------
	mov	esi, eiiSrc
	mov	edi, eiiDst
	ASSUME	esi:PTR EGL_IMAGE_INFO
	ASSUME	edi:PTR EGL_IMAGE_INFO
	;
	mov	eax, [edi].dwImageWidth
	mov	edx, [edi].dwImageHeight
	.IF	(eax != [esi].dwImageWidth) || (edx != [esi].dwImageHeight)
		mov	eax, eslErrGeneral
		ret
	.ENDIF
	mov	nWidth, eax
	mov	nHeight, edx
	;
	mov	eax, [esi].dwBytesPerLine
	mov	edx, [edi].dwBytesPerLine
	mov	ecx, [edi].fdwFormatType
	mov	nSrcLineBytes, eax
	mov	nDstLineBytes, edx
	mov	fdwFormatType, ecx
	;
	mov	eax, [esi].dwBitsPerPixel
	mov	edx, [edi].dwBitsPerPixel
	.IF	((eax != 24) && (eax != 32)) || ((edx != 24) && (edx != 32))
		mov	eax, eslErrGeneral
		ret
	.ENDIF
	shr	eax, 3
	shr	edx, 3
	mov	nSrcPixelBytes, eax
	mov	nDstPixelBytes, edx
	;
	; 最適化判定
	;
	mov	esi, [esi].ptrImageArray
	mov	edi, [edi].ptrImageArray
	ASSUME	esi:NOTHING
	ASSUME	edi:NOTHING
	mov	pbytSrcLine, esi
	mov	pbytDstLine, edi
	;
	.IF	esi != NULL
	;
	.IF	(ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM) \
			&& (edx == 4) && (edx == eax) && !(nWidth & 01H)
		;
		;	MMX 専用加算ルーチン
		; ------------------------------------------------------------
		mov	ecx, nHeight
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	nLeftHeight, ecx
			mov	ecx, nWidth
			mov	pbytSrcLine, esi
			mov	pbytDstLine, edi
			;
			movq	mm6, mmxMask_ByteMSB
			shr	ecx, 1
			pxor	mm7, mm7
			.WHILE	!ZERO?
				movq	mm2, MMWORD PTR [edi]
				movq	mm4, MMWORD PTR [esi]
				pxor	mm0, mm0
				pxor	mm1, mm1
				pxor	mm2, mm6
				movq	mm5, mm4
				punpcklbw	mm0, mm2
				punpcklbw	mm4, mm7
				add	esi, 8
				punpckhbw	mm1, mm2
				punpckhbw	mm5, mm7
				psraw	mm0, 7
				psraw	mm1, 7
				paddw	mm0, mm4
				paddw	mm1, mm5
				packuswb	mm0, mm1
				movq	MMWORD PTR [edi], mm0
				add	edi, 8
				dec	ecx
			.ENDW
			;
			mov	esi, pbytSrcLine
			mov	edi, pbytDstLine
			mov	ecx, nLeftHeight
			add	esi, nSrcLineBytes
			add	edi, nDstLineBytes
			dec	ecx
		.ENDW
		emms
		xor	eax, eax
		ret
	.ENDIF
	;
	;	486 互換加算ルーチン
	; --------------------------------------------------------------------
	mov	ecx, nHeight
	test	ecx, ecx
	.WHILE	!ZERO?
		mov	nLeftHeight, ecx
		mov	pbytSrcLine, esi
		mov	pbytDstLine, edi
		;
		mov	ecx, nWidth
		.IF	(nDstPixelBytes == 4) && (nSrcPixelBytes == 4)
			test	ecx, ecx
			.WHILE	!ZERO?
				FOR	@INDEX, <0, 1, 2, 3>
					movsx	eax, BYTE PTR [edi + @INDEX]
					movzx	edx, BYTE PTR [esi + @INDEX]
					xor	eax, -80H
					shl	eax, 1
					add	eax, edx
					cmp	eax, 0FFH
					sbb	edx, edx
					mov	ebx, eax
					and	eax, edx
					sar	ebx, 31
					or	ebx, edx
					not	ebx
					or	eax, ebx
					mov	BYTE PTR [edi + @INDEX], al
				ENDM
				add	esi, nSrcPixelBytes
				add	edi, nDstPixelBytes
				dec	ecx
			.ENDW
		.ELSE
			test	ecx, ecx
			.WHILE	!ZERO?
				FOR	@INDEX, <0, 1, 2>
					movsx	eax, BYTE PTR [edi + @INDEX]
					movzx	edx, BYTE PTR [esi + @INDEX]
					xor	eax, -80H
					shl	eax, 1
					add	eax, edx
					cmp	eax, 0FFH
					sbb	edx, edx
					mov	ebx, eax
					and	eax, edx
					sar	ebx, 31
					or	ebx, edx
					not	ebx
					or	eax, ebx
					mov	BYTE PTR [edi + @INDEX], al
				ENDM
				add	esi, nSrcPixelBytes
				add	edi, nDstPixelBytes
				dec	ecx
			.ENDW
		.ENDIF
		;
		mov	esi, pbytSrcLine
		mov	edi, pbytDstLine
		mov	ecx, nLeftHeight
		add	esi, nSrcLineBytes
		add	edi, nDstLineBytes
		dec	ecx
	.ENDW
	;
	xor	eax, eax
	ret

	.ENDIF

	;
	;	入力元がブランク画像
	; --------------------------------------------------------------------
	mov	ecx, nHeight
	test	ecx, ecx
	.WHILE	!ZERO?
		mov	nLeftHeight, ecx
		mov	pbytDstLine, edi
		;
		mov	ecx, nWidth
		.IF	nDstPixelBytes == 4
			test	ecx, ecx
			.WHILE	!ZERO?
				mov	eax, DWORD PTR [edi]
				mov	edx, eax
				and	eax, 80808080H
				mov	ebx, eax
				shr	eax, 7
				sub	ebx, eax
				and	edx, ebx
				shl	edx, 1
				mov	DWORD PTR [edi], edx
				add	edi, 4
				dec	ecx
			.ENDW
		.ELSE
			test	ecx, ecx
			.WHILE	!ZERO?
				mov	al, BYTE PTR [edi]
				mov	bl, BYTE PTR [edi + 1]
				mov	dl, BYTE PTR [edi + 2]
				mov	ah, al
				mov	bh, bl
				mov	dh, dl
				sar	al, 8
				sar	bl, 8
				sar	dl, 8
				shl	ah, 1
				shl	bh, 1
				shl	dl, 1
				and	al, ah
				and	bl, bh
				and	dl, dh
				mov	BYTE PTR [edi], al
				mov	BYTE PTR [edi + 1], bl
				mov	BYTE PTR [edi + 2], dl
				add	edi, nDstPixelBytes
				dec	ecx
			.ENDW
		.ENDIF
		;
		mov	edi, pbytDstLine
		mov	ecx, nLeftHeight
		add	edi, nDstLineBytes
		dec	ecx
	.ENDW
	;
	xor	eax, eax
	ret

eriLSAdditionOfFrame	ENDP

;
;	2つの画像の絶対差の合計を計算
; ----------------------------------------------------------------------------
ALIGN	10H
eriSumAbsDifferenceOfBlock	PROC	NEAR32 C USES ebx esi edi,
	eiiDst:PTR EGL_IMAGE_INFO, eiiSrc:PTR EGL_IMAGE_INFO

	LOCAL	nWidth:DWORD, nWidthBytes:DWORD, nHeight:DWORD
	LOCAL	fdwFormatType:DWORD
	LOCAL	nDstPixelBytes:DWORD, nSrcPixelBytes:DWORD
	LOCAL	nDstLineBytes:DWORD, nSrcLineBytes:DWORD
	LOCAL	pbytDstLine:PTR, pbytSrcLine:PTR
	LOCAL	nLeftHeight:DWORD, dwSumAbsDiff:DWORD

	;
	;	画像情報取得
	; --------------------------------------------------------------------
	mov	esi, eiiSrc
	mov	edi, eiiDst
	ASSUME	esi:PTR EGL_IMAGE_INFO
	ASSUME	edi:PTR EGL_IMAGE_INFO
	;
	mov	eax, [edi].dwImageWidth
	mov	edx, [edi].dwImageHeight
	.IF	(eax != [esi].dwImageWidth) || (edx != [esi].dwImageHeight)
		mov	eax, 7FFFFFFFH
		ret
	.ENDIF
	mov	nWidth, eax
	mov	nHeight, edx
	;
	mov	eax, [esi].dwBytesPerLine
	mov	edx, [edi].dwBytesPerLine
	mov	ecx, [edi].fdwFormatType
	mov	nSrcLineBytes, eax
	mov	nDstLineBytes, edx
	mov	fdwFormatType, ecx
	;
	mov	eax, [esi].dwBitsPerPixel
	mov	edx, [edi].dwBitsPerPixel
	.IF	((eax != 24) && (eax != 32)) || ((edx != 24) && (edx != 32))
		mov	eax, 7FFFFFFFH
		ret
	.ENDIF
	shr	eax, 3
	shr	edx, 3
	mov	nSrcPixelBytes, eax
	mov	nDstPixelBytes, edx
	;
	; 最適化判定
	;
	imul	eax, nWidth
	mov	esi, [esi].ptrImageArray
	mov	edi, [edi].ptrImageArray
	ASSUME	esi:NOTHING
	ASSUME	edi:NOTHING
	mov	pbytSrcLine, esi
	mov	pbytDstLine, edi
	mov	nWidthBytes, eax
	;
	.IF	(ERI_EnabledProcessorType & ERI_USE_XMM_P3) && \
			(edx == nSrcPixelBytes) && !(eax & 07H)
		;
		;	SSE 専用ルーチン
		; ------------------------------------------------------------
		mov	eax, 0FFFFFFFFH
		cmp	edx, 3
		mov	edx,  00FFFFFFH
		cmovz	edx, eax
		test	fdwFormatType, EIF_WITH_ALPHA
		cmovz	eax, edx
		;
		mov	ecx, nHeight
		movd	mm6, eax
		test	ecx, ecx
		pxor	mm7, mm7		; <- Sum of abs difference
		punpckldq	mm6, mm6
		;
		.WHILE	!ZERO?
			mov	edx, nWidthBytes
			sub	edx, 16
			.WHILE	!SIGN?
				movq	mm0, MMWORD PTR [edi]
				movq	mm2, MMWORD PTR [esi]
				movq	mm1, MMWORD PTR [edi + 8]
				pand	mm0, mm6
				pand	mm2, mm6
				movq	mm3, MMWORD PTR [esi + 8]
				pand	mm1, mm6
				pand	mm3, mm6
				psadbw	mm0, mm2
				psadbw	mm1, mm3
				add	edi, 16
				paddd	mm7, mm0
				add	esi, 16
				paddd	mm7, mm1
				sub	edx, 16
			.ENDW
			add	edx, 16
			.IF	!ZERO?
				movq	mm0, MMWORD PTR [edi]
				movq	mm2, MMWORD PTR [esi]
				pand	mm0, mm6
				pand	mm2, mm6
				psadbw	mm0, mm2
				paddd	mm7, mm0
			.ENDIF
			;
			mov	edi, pbytDstLine
			mov	esi, pbytSrcLine
			add	edi, nDstLineBytes
			add	esi, nSrcLineBytes
			mov	pbytDstLine, edi
			mov	pbytSrcLine, esi
			dec	ecx
		.ENDW
		;
		movd	eax, mm7
		emms
		ret
	.ENDIF

	;
	;	486 互換ルーチン
	; --------------------------------------------------------------------
	mov	ecx, nHeight
	mov	dwSumAbsDiff, 0
	test	ecx, ecx
	.WHILE	!ZERO?
		mov	nLeftHeight, ecx
		mov	pbytDstLine, edi
		mov	pbytSrcLine, esi
		;
		mov	ecx, nWidth
		test	ecx, ecx
		.WHILE	!ZERO?
			mov	al, BYTE PTR [edi]
			mov	bl, BYTE PTR [edi + 1]
			mov	dl, BYTE PTR [edi + 2]
			sub	al, BYTE PTR [esi]
			sbb	ah, ah
			sub	bl, BYTE PTR [esi + 1]
			sbb	bh, bh
			sub	dl, BYTE PTR [esi + 2]
			sbb	dh, dh
			xor	al, ah
			add	edi, nDstPixelBytes
			xor	bl, bh
			sub	al, ah
			add	esi, nSrcPixelBytes
			xor	dl, dh
			sub	bl, bh
			sub	dl, dh
			movzx	eax, al
			movzx	ebx, bl
			movzx	edx, dl
			add	eax, dwSumAbsDiff
			add	ebx, edx
			add	eax, ebx
			dec	ecx
			mov	dwSumAbsDiff, eax
		.ENDW
		;
		mov	edi, pbytDstLine
		mov	esi, pbytSrcLine
		mov	ecx, nLeftHeight
		add	edi, nDstLineBytes
		add	esi, nSrcLineBytes
		dec	ecx
	.ENDW

	mov	eax, dwSumAbsDiff
	ret

eriSumAbsDifferenceOfBlock	ENDP

;
;	4:2:1 ループフィルタ
; ----------------------------------------------------------------------------
ALIGN	10H
eriImageFilterLoop421	PROC	NEAR32 C USES ebx esi edi,
		eiiDst:PEGL_IMAGE_INFO,
		eiiSrc:PEGL_IMAGE_INFO,
		pFlags:PTR SBYTE, nBlockSize:DWORD

	LOCAL	pfnLineFilter:PTR NEAR32
	LOCAL	pfnImageFilter:PTR NEAR32
	LOCAL	eiiBlock:EGL_IMAGE_INFO, ptrLineLeftAddr:PTR
	LOCAL	nWidthBlocks:DWORD, nHeightBlocks:DWORD
	LOCAL	nBlockY:DWORD, nBlockX:DWORD
	LOCAL	ptrLineAddr:DWORD
	LOCAL	ptrSrcLineAddr:PTR, dwSrcLineBytes:SDWORD
	LOCAL	ptrLineBuf[4]:PTR BYTE, dwLineBufBytes:DWORD

	mov	ebx, eiiDst
	mov	ecx, eiiSrc
	mov	esi, pFlags
	ASSUME	ebx:PEGL_IMAGE_INFO
	.IF	([ebx].dwBitsPerPixel != 32) \
		|| ([ebx].dwImageWidth < 2) || ([ebx].dwImageHeight < 2)
		.IF	ecx != NULL
			INVOKE	eriCopyImage , eiiDst, eiiSrc
		.ENDIF
		ret
	.ENDIF
	.IF	ecx == NULL
		mov	ecx, ebx
	.ENDIF
	;
	ASSUME	ecx:PTR EGL_IMAGE_INFO
	mov	eax, [ecx].ptrImageArray
	mov	edx, [ecx].dwBytesPerLine
	mov	ptrSrcLineAddr, eax
	mov	dwSrcLineBytes, edx
	ASSUME	ecx:NOTHING
	;
	mov	pfnLineFilter, OFFSET eriLineFilterLoop211_486
	mov	pfnImageFilter, OFFSET eriImageFilterLoop421_486
	;
	.IF	!([ebx].dwImageWidth & 01H)
		.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
			mov	pfnLineFilter, OFFSET eriLineFilterLoop211_SSE
			mov	pfnImageFilter, OFFSET eriImageFilterLoop421_SSE
		.ELSEIF	ERI_EnabledProcessorType & ERI_USE_MMX_PENTIUM
			mov	pfnLineFilter, OFFSET eriLineFilterLoop211_MMX
			mov	pfnImageFilter, OFFSET eriImageFilterLoop421_MMX
		.ENDIF
	.ENDIF
	;
	; 一時バッファを確保
	;
	mov	eax, [ebx].dwImageWidth
	shl	eax, 2				; x4
	add	eax, 0FH
	and	eax, NOT 0FH
	mov	dwLineBufBytes, eax
	lea	edx, [eax + eax * 2]
	INVOKE	eslHeapAllocate , NULL, edx, 0
	mov	ptrLineBuf[0], eax
	mov	ptrLineBuf[12], eax
	add	eax, dwLineBufBytes
	mov	ptrLineBuf[4], eax
	add	eax, dwLineBufBytes
	mov	ptrLineBuf[8], eax
	pushfd
	cld
	;
	test	esi, esi
	.IF	ZERO?
		mov	eax, dwSrcLineBytes
		test	[ebx].dwBytesPerLine, eax
		.IF	SIGN?
			mov	ecx, [ebx].dwImageHeight
			mov	edx, [ebx].dwBytesPerLine
			dec	ecx
			imul	eax, ecx
			imul	edx, ecx
			add	eax, ptrSrcLineAddr
			add	edx, [ebx].ptrImageArray
			mov	ptrSrcLineAddr, eax
			mov	eiiBlock.ptrImageArray, edx
			mov	eax, [ebx].dwImageWidth
			mov	ecx, [ebx].dwImageHeight
			mov	eiiBlock.dwImageWidth, eax
			mov	eiiBlock.dwImageHeight, ecx
			mov	eax, dwSrcLineBytes
			mov	ecx, [ebx].dwBytesPerLine
			neg	eax
			neg	ecx
			mov	dwSrcLineBytes, eax
			mov	eiiBlock.dwBytesPerLine, ecx
			lea	ebx, eiiBlock
		.ENDIF
		call	eriBlockFilterLoop421
	.ELSE
		;
		; ブロック情報を複製
		;
		mov	eax, [ebx].dwBytesPerLine
		mov	eiiBlock.dwBytesPerLine, eax
		;
		mov	eax, [ebx].dwImageWidth
		mov	edx, [ebx].dwImageHeight
		add	eax, 0FH
		add	edx, 0FH
		shr	eax, 4
		shr	edx, 4
		mov	nWidthBlocks, eax
		mov	nHeightBlocks, edx
		;
		mov	eax, [ebx].ptrImageArray
		mov	eiiBlock.ptrImageArray, eax
		;
		; ブロック毎のループ
		;
		xor	ecx, ecx
		.WHILE	ecx < nHeightBlocks
			mov	nBlockY, ecx
			;
			; ブロックのライン共通情報を設定
			;
			shl	ecx, 4
			mov	eax, ecx
			imul	ecx, eiiBlock.dwBytesPerLine
			neg	eax
			add	ecx, [ebx].ptrImageArray
			add	eax, [ebx].dwImageHeight
			.IF	eax > 16
				mov	eax, 16
			.ENDIF
			mov	ptrLineLeftAddr, ecx
			mov	eiiBlock.dwImageHeight, eax
			;
			; イントラブロックを走査
			;
			xor	ecx, ecx
			.WHILE	ecx < nWidthBlocks
				;
				; イントラブロックを検索
				;
				mov	eax, nWidthBlocks
				.WHILE	(BYTE PTR [esi]) != 0
					inc	esi
					inc	ecx
					cmp	ecx, eax
					jae	Loop_HorzContinue
				.ENDW
				;
				mov	nBlockX, ecx
				mov	edx, ptrLineLeftAddr
				shl	ecx, 4
				lea	edx, [edx + ecx * 4]
				neg	ecx
				mov	eiiBlock.ptrImageArray, edx
				add	ecx, [ebx].dwImageWidth
				mov	eiiBlock.dwImageWidth, ecx
				;
				mov	ecx, nBlockX
				mov	eax, 16
				mov	edx, nWidthBlocks
				inc	ecx
				inc	esi
				.WHILE	ecx < edx
					.BREAK	.IF	(BYTE PTR [esi]) != 0
					inc	ecx
					inc	esi
					add	eax, 16
				.ENDW
				;
				mov	nBlockX, ecx
				.IF	eax < eiiBlock.dwImageWidth
					mov	eiiBlock.dwImageWidth, eax
				.ENDIF
				;
				; 4-2-1 ループフィルタリング
				;
				mov	pFlags, esi
				lea	ebx, eiiBlock
				mov	eax, [ebx].ptrImageArray
				mov	ptrSrcLineAddr, eax
				mov	eax, [ebx].dwBytesPerLine
				mov	dwSrcLineBytes, eax
				call	eriBlockFilterLoop421
				mov	ebx, eiiDst
				mov	esi, pFlags
				mov	ecx, nBlockX
			.ENDW
Loop_HorzContinue:	;
			mov	ecx, nBlockY
			inc	ecx
		.ENDW
	.ENDIF
	;
	.IF	ERI_EnabledProcessorType & \
			(ERI_USE_XMM_P3 OR ERI_USE_MMX_PENTIUM)
		emms
		.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
			sfence
		.ENDIF
	.ENDIF
	;
	INVOKE	eslHeapFree , NULL, ptrLineBuf[12], 0
	popfd
	ret

eriBlockFilterLoop421:
	;
	; 先頭ラインにフィルタをかける
	;
	mov	eax, [ebx].ptrImageArray
	mov	esi, ptrSrcLineAddr
	mov	edi, ptrLineBuf[0]
	mov	ptrLineAddr, eax
	call	pfnLineFilter
	;
	; 一時バッファから複製
	;
	mov	ecx, [ebx].dwImageWidth
	mov	esi, ptrLineBuf[0]
	mov	edi, ptrLineAddr
	rep	movsd
	;
	; 2 ライン目にフィルタをかける
	;
	mov	edi, ptrLineAddr
	mov	esi, ptrSrcLineAddr
	add	edi, [ebx].dwBytesPerLine
	add	esi, dwSrcLineBytes
	mov	ptrLineAddr, edi
	mov	ptrSrcLineAddr, esi
	mov	edi, ptrLineBuf[4]
	call	pfnLineFilter
	;
	; 4-2-1 フィルタ処理
	;
	mov	ecx, [ebx].dwImageHeight
	sub	ecx, 2
	.WHILE	!ZERO?
		mov	esi, ptrSrcLineAddr
		push	ecx
		;
		add	esi, dwSrcLineBytes
		mov	ptrSrcLineAddr, esi
		mov	edi, ptrLineBuf[8]
		call	pfnLineFilter
		call	pfnImageFilter
		;
		mov	eax, ptrLineBuf[0]
		mov	edi, ptrLineAddr
		mov	ecx, ptrLineBuf[4]
		add	edi, [ebx].dwBytesPerLine
		mov	edx, ptrLineBuf[8]
		mov	ptrLineBuf[0], ecx
		pop	ecx
		mov	ptrLineBuf[4], edx
		mov	ptrLineAddr, edi
		mov	ptrLineBuf[8], eax
		dec	ecx
	.ENDW
	;
	; 一時バッファから複製
	;
	mov	ecx, [ebx].dwImageWidth
	mov	esi, ptrLineBuf[4]
	mov	edi, ptrLineAddr
	rep	movsd
	;
	BYTE	0C3H	; ret

eriLineFilterLoop211_SSE:
	mov	ecx, [ebx].dwImageWidth
	movq	mm4, MMWORD PTR [esi]
	shr	ecx, 1
	mov	eax, dwSrcLineBytes
	movd	DWORD PTR [edi], mm4
	sub	ecx, 2 + 1
	.WHILE	!SIGN?
		movq	mm1, MMWORD PTR [esi + 8]
		prefetchnta	[esi + eax]
		movq	mm0, mm4
		movq	mm2, mm1
		psrlq	mm0, 32
		psllq	mm1, 32
		pavgb	mm4, mm2
		por	mm0, mm1
			movq	mm1, MMWORD PTR [esi + 16]
		pavgb	mm0, mm4
			add	esi, 16
			movq	mm3, mm2
			movq	mm4, mm1
		movd	DWORD PTR [edi + 4], mm0
			psrlq	mm3, 32
		psrlq	mm0, 32
			psllq	mm1, 32
			pavgb	mm2, mm4
			por	mm1, mm3
			pavgb	mm1, mm2
		movd	DWORD PTR [edi + 8], mm0
			movd	DWORD PTR [edi + 12], mm1
			psrlq	mm1, 32
			movd	DWORD PTR [edi + 16], mm1
		add	edi, 16
		sub	ecx, 2
	.ENDW
	add	ecx, 2
	.IF	!ZERO?
		movq	mm1, MMWORD PTR [esi + 8]
		movq	mm0, mm4
		add	esi, 8
		movq	mm2, mm1
		psrlq	mm0, 32
		psllq	mm1, 32
		pavgb	mm4, mm2
		por	mm0, mm1
		pavgb	mm0, mm4
		movd	DWORD PTR [edi + 4], mm0
		psrlq	mm0, 32
		movq	mm4, mm2
		movd	DWORD PTR [edi + 8], mm0
		add	edi, 8
	.ENDIF
	psrlq	mm4, 32
	movd	DWORD PTR [edi + 4], mm4
	BYTE	0C3H	; ret

eriImageFilterLoop421_SSE:
	mov	edi, ptrLineAddr
	mov	eax, ptrLineBuf[4]
	mov	edx, ptrLineBuf[8]
	mov	ecx, [ebx].dwImageWidth
	mov	esi, ptrLineBuf[0]
	shr	ecx, 1
	sub	eax, esi
	sub	edx, esi
	sub	ecx, 2
	.WHILE	!SIGN?
		movq	mm0, MMWORD PTR [esi]
		movq	mm1, MMWORD PTR [esi + 8]
		pavgb	mm0, MMWORD PTR [esi + edx]
			movq	mm2, MMWORD PTR [esi + eax]
		pavgb	mm1, MMWORD PTR [esi + edx + 8]
			movq	mm3, MMWORD PTR [esi + eax + 8]
		pavgb	mm0, mm2
		pavgb	mm1, mm3
			add	esi, 16
		movntq	MMWORD PTR [edi], mm0
		movntq	MMWORD PTR [edi + 8], mm1
		add	edi, 16
		sub	ecx, 2
	.ENDW
	add	ecx, 2
	.IF	!ZERO?
		movq	mm0, MMWORD PTR [esi]
			movq	mm2, MMWORD PTR [esi + eax]
		pavgb	mm0, MMWORD PTR [esi + edx]
		pavgb	mm0, mm2
		movntq	MMWORD PTR [edi], mm0
	.ENDIF
	BYTE	0C3H	; ret

eriLineFilterLoop211_MMX:
	movq	mm4, MMWORD PTR [esi]
	pxor	mm7, mm7
	mov	ecx, [ebx].dwImageWidth
	movd	DWORD PTR [edi], mm4
	movq	mm5, mm4
	punpcklbw	mm4, mm7
	shr	ecx, 1
	punpckhbw	mm5, mm7
	dec	ecx
	.WHILE	!ZERO?
		movq	mm2, MMWORD PTR [esi + 8]
		movq	mm1, mm5
		movq	mm3, mm2
		punpcklbw	mm2, mm7
		punpckhbw	mm3, mm7
		;
		paddw	mm4, mm2
		paddw	mm5, mm3
		psrlw	mm4, 1
		psrlw	mm5, 1
		;
		paddw	mm1, mm4
		paddw	mm5, mm2
		psrlw	mm1, 1
		psrlw	mm5, 1
		;
		packuswb	mm1, mm5
		movq	mm4, mm2
		movq	mm5, mm3
		;
		movd	DWORD PTR [edi + 4], mm1
		psrlq	mm1, 32
		add	esi, 8
		movd	DWORD PTR [edi + 8], mm1
		add	edi, 8
		dec	ecx
	.ENDW
	packuswb	mm5, mm5
	movd	DWORD PTR [edi + 4], mm5
	BYTE	0C3H	; ret

eriImageFilterLoop421_MMX:
	mov	edi, ptrLineAddr
	mov	eax, ptrLineBuf[4]
	mov	edx, ptrLineBuf[8]
	mov	ecx, [ebx].dwImageWidth
	mov	esi, ptrLineBuf[0]
	pxor	mm7, mm7
	sub	eax, esi
	sub	edx, esi
	shr	ecx, 1
	.WHILE	!ZERO?
		movq	mm0, MMWORD PTR [esi]
		movq	mm2, MMWORD PTR [esi + edx]
		movq	mm1, mm0
		movq	mm3, mm2
		punpcklbw	mm0, mm7
		punpcklbw	mm2, mm7
			movq	mm4, MMWORD PTR [esi + eax]
		punpckhbw	mm1, mm7
			movq	mm5, mm4
		punpckhbw	mm3, mm7
		paddw	mm0, mm2
			punpcklbw	mm4, mm7
		paddw	mm1, mm3
			punpckhbw	mm5, mm7
		psrlw	mm0, 1
		psrlw	mm1, 1
			add	esi, 8
		;
		paddw	mm0, mm4
		paddw	mm1, mm5
		psrlw	mm0, 1
		psrlw	mm1, 1
		packuswb	mm0, mm1
		movq	MMWORD PTR [edi], mm0
		add	edi, 8
		dec	ecx
	.ENDW
	BYTE	0C3H	; ret

eriLineFilterLoop211_486:
	mov	ecx, [ebx].dwImageWidth
	dec	ecx
	.IF	!ZERO?
		push	ebx
		push	ebp
		mov	ebp, DWORD PTR [esi]
		add	esi, 4
		mov	DWORD PTR [edi], ebp
		add	edi, 4
		dec	ecx
		.WHILE	!ZERO?
			mov	eax, DWORD PTR [esi + 4]
			mov	edx, eax
			and	eax, ebp
			shr	ebp, 1
			shr	edx, 1
			and	ebp, 7F7F7F7FH
			and	edx, 7F7F7F7FH
			and	eax, 01010101H
			add	ebp, edx
				mov	edx, DWORD PTR [esi]
			add	eax, ebp
				mov	ebp, edx
			;
			mov	ebx, eax
			and	eax, edx
			shr	edx, 1
			shr	ebx, 1
			and	edx, 7F7F7F7FH
			and	ebx, 7F7F7F7FH
			and	eax, 01010101H
			add	edx, ebx
			add	eax, edx
			add	esi, 4
			mov	DWORD PTR [edi], eax
			add	edi, 4
			dec	ecx
		.ENDW
		mov	DWORD PTR [edi], ebp
		pop	ebp
		pop	ebx
	.ENDIF
	BYTE	0C3H	; ret

eriImageFilterLoop421_486:
	push	ebx
	push	ebp
	mov	edi, ptrLineAddr
	mov	ecx, [ebx].dwImageWidth
	mov	esi, ptrLineBuf[8]
	mov	eax, ptrLineBuf[4]
	mov	edx, ptrLineBuf[0]
	sub	esi, edx
	sub	eax, edx
	push	esi
	push	eax
	test	ecx, ecx
	.WHILE	!ZERO?
		mov	eax, DWORD PTR [edx]
		mov	ebp, DWORD PTR [edx + esi]
		mov	ebx, eax
		and	eax, ebp
		shr	ebp, 1
		shr	ebx, 1
		and	ebp, 7F7F7F7FH
		and	ebx, 7F7F7F7FH
		mov	esi, DWORD PTR [esp]
		and	eax, 01010101H
		add	ebp, ebx
		add	ebp, eax
		;
		mov	eax, DWORD PTR [edx + esi]
		add	edx, 4
		;
		mov	ebx, eax
		and	eax, ebp
		shr	ebp, 1
		shr	ebx, 1
		and	ebp, 7F7F7F7FH
		and	ebx, 7F7F7F7FH
		and	eax, 01010101H
		add	ebp, ebx
		mov	esi, DWORD PTR [esp + 4]
		add	eax, ebp
		mov	DWORD PTR [edi], eax
		add	edi, 4
		dec	ecx
	.ENDW
	pop	eax
	pop	eax
	pop	ebp
	pop	ebx
	BYTE	0C3H	; ret

	ASSUME	ebx:NOTHING

eriImageFilterLoop421	ENDP


CodeSeg	ENDS

ENDIF	;	IFNDEF COMPACT_NOA_DECODER

	END
