
; ----------------------------------------------------------------------------
;                          Entis Standard Library
; ----------------------------------------------------------------------------
;    Copyright (C) 2002-2004 Leshade Entis, Entis-soft. All rights reserved.
; ----------------------------------------------------------------------------


	.586
	.XMM
	.MODEL	FLAT

	INCLUDE	esl.inc
	INCLUDE	experi.inc

MMWORD	TYPEDEF	QWORD


; ----------------------------------------------------------------------------
;                            �f�[�^�Z�O�����g
; ----------------------------------------------------------------------------

DataSeg	SEGMENT	PARA FLAT 'DATA'

ERI_EnabledProcessorType	DWORD	0

DataSeg	ENDS


; ----------------------------------------------------------------------------
;                             �R�[�h�Z�O�����g
; ----------------------------------------------------------------------------

CodeSeg	SEGMENT	PARA READONLY FLAT 'CODE'

;
;	CPU �� MMX �ɑΉ����Ă��邩���ׁA�t���O��ݒ肷��
; ----------------------------------------------------------------------------
ALIGN	10H
eriEnableMMX	PROC	NEAR32 C USES ebx edi esi, fForceEnable:DWORD

	.IF	fForceEnable != 0
		mov	eax, fForceEnable
		or	ERI_EnabledProcessorType, eax
		ret
	.ENDIF
	;
	; �v���Z�b�T�� 80386 �� 80486 �ȍ~���𔻒�
	;
	mov	ERI_EnabledProcessorType, 0

	IFNDEF	COMPACT_NOA_DECODER

	mov	ecx, esp
	and	esp, (NOT 03H)
	pushfd
	pushfd
	pop	eax
	mov	ebx, eax
	xor	eax, 00040000H
	push	eax
	popfd
	pushfd
	pop	eax
	popfd
	mov	esp, ecx
	cmp	eax, ebx
	jz	Label_Exit	; �� 80386
	;
	; ���[�J����O�n���h����ݒ�
	;
	PUSHCONTEXT	ASSUMES
	ASSUME	fs:FLAT
	push	ebp
	push	(OFFSET Label_ExceptionHandler)
	push	DWORD PTR fs:[TIB_pSEHList]
	mov	DWORD PTR fs:[TIB_pSEHList], esp
	;
	; �x���_�[ ID �擾 (�C���e�������v���Z�b�T����r : "GenuineIntel")
	;
	xor	eax, eax
	cpuid
	.IF	(eax >= 1) && (ebx == 756E6547H) && \
			(edx == 49656E69H) && (ecx == 6C65746EH)
		;
		; �q���[�`���[�t���O���擾
		; 
		mov	eax, 1
		cpuid
		.IF	edx & (1 SHL 23)
			;
			; MMX ���߃Z�b�g���T�|�[�g����Ă���
			;
			xor	eax, eax
			pcmpeqb	mm0, mm0
			movd	eax, mm0
			emms
			.IF	eax == 0FFFFFFFFH
				or	ERI_EnabledProcessorType, \
						ERI_USE_MMX_PENTIUM
			.ENDIF
		.ENDIF
		.IF	edx & (1 SHL 25)
			;
			; XMM ���߃Z�b�g���T�|�[�g����Ă���
			;
			xorps	xmm0, xmm0
			xorps	xmm1, xmm1
			cmpps	xmm0, xmm0, 0
			movhlps	xmm0, xmm1
			xor	eax, eax
			pcmpeqb	mm0, mm0
			pinsrw	mm0, eax, 0
			movd	eax, mm0
			emms
			.IF	eax == 0FFFF0000H
				or	ERI_EnabledProcessorType, ERI_USE_XMM_P3
				.IF	edx & (1 SHL 26)
					or	ERI_EnabledProcessorType, ERI_USE_SSE2
				.ENDIF
			.ENDIF
		.ENDIF
	;
	; �x���_�[ ID ���� (AMD ���v���Z�b�T����r : "AuthenticAMD")
	;
	.ELSEIF	(eax >= 1) && (ebx == 68747541H) && \
			(edx == 69746E65H) && (ecx == 444D4163H)
		;
		; �q���[�`���[�t���O���擾
		; 
		mov	eax, 1
		cpuid
		.IF	edx & (1 SHL 23)
			;
			; MMX ���߃Z�b�g���T�|�[�g����Ă���
			;
			xor	eax, eax
			pcmpeqb	mm0, mm0
			movd	eax, mm0
			emms
			.IF	eax == 0FFFFFFFFH
				or	ERI_EnabledProcessorType, \
						ERI_USE_MMX_PENTIUM
			.ENDIF
		.ENDIF
		.IF	edx & (1 SHL 25)
			;
			; XMM ���߃Z�b�g���T�|�[�g����Ă���
			;
			xorps	xmm0, xmm0
			xorps	xmm1, xmm1
			cmpps	xmm0, xmm0, 0
			movhlps	xmm0, xmm1
			xor	eax, eax
			pcmpeqb	mm0, mm0
			pinsrw	mm0, eax, 0
			movd	eax, mm0
			emms
			.IF	eax == 0FFFF0000H
				or	ERI_EnabledProcessorType, ERI_USE_XMM_P3
				.IF	edx & (1 SHL 26)
					or	ERI_EnabledProcessorType, ERI_USE_SSE2
				.ENDIF
			.ENDIF
		.ENDIF
	;
	; ���m�̃x���_�[ ID
	;
	.ELSEIF	eax >= 1
		;
		; �q���[�`���[�t���O���擾
		; 
		mov	eax, 1
		cpuid
		.IF	edx & (1 SHL 23)
			;
			; MMX ���߃Z�b�g���T�|�[�g����Ă���
			;
			xor	eax, eax
			pcmpeqb	mm0, mm0
			movd	eax, mm0
			emms
			.IF	eax == 0FFFFFFFFH
				or	ERI_EnabledProcessorType, \
						ERI_USE_MMX_PENTIUM
			.ENDIF
		.ENDIF
		.IF	edx & (1 SHL 25)
			;
			; XMM ���߃Z�b�g���T�|�[�g����Ă���
			;
			xorps	xmm0, xmm0
			xorps	xmm1, xmm1
			cmpps	xmm0, xmm0, 0
			movhlps	xmm0, xmm1
			xor	eax, eax
			pcmpeqb	mm0, mm0
			pinsrw	mm0, eax, 0
			movd	eax, mm0
			emms
			.IF	eax == 0FFFF0000H
				or	ERI_EnabledProcessorType, ERI_USE_XMM_P3
				.IF	edx & (1 SHL 26)
					or	ERI_EnabledProcessorType, ERI_USE_SSE2
				.ENDIF
			.ENDIF
		.ENDIF
	.ENDIF
Label_EndTry:
	;
	; ���[�J����O�n���h�������
	;
	pop	DWORD PTR fs:[TIB_pSEHList]
	add	esp, (SIZEOF DWORD) * 2
	POPCONTEXT	ASSUMES
	;
Label_Exit:
	ENDIF

	ret

	IFNDEF	COMPACT_NOA_DECODER
Label_ExceptionHandler:
	push	ebp
	mov	edx, DWORD PTR ss:[esp+08H+08H]
	mov	(CONTEXT PTR [edx]).__Eip, OFFSET Label_EndTry
	mov	eax, ExceptionContinueExecution
	pop	ebp
	BYTE	0C3H	; ret	0
	ENDIF

eriEnableMMX	ENDP

;
;	MMX, XMM �t���O�����Z�b�g����
; ----------------------------------------------------------------------------
ALIGN	10H
eriDisableMMX	PROC	NEAR32 C , fForceDisable:DWORD

	mov	eax, fForceDisable
	not	eax
	and	ERI_EnabledProcessorType, eax
	ret

eriDisableMMX	ENDP

;
;	FPU ������������
; ----------------------------------------------------------------------------
ALIGN	10H
eriInitializeFPU	PROC	NEAR32 C USES ebx edi esi

	LOCAL	dwMXCSR:DWORD

	;
	; FPU ��������
	;
	finit

	;
	; XMM ��������
	;
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
		;
		; XMM �̊ۂߕ��@���ŋߒl�ɐݒ�
		;
		stmxcsr	dwMXCSR
		and	dwMXCSR, (NOT 6000H)
		ldmxcsr	dwMXCSR
	.ENDIF

	ret

eriInitializeFPU	ENDP

;
;	������������������
; ----------------------------------------------------------------------------
ALIGN	10H
eslFillMemory	PROC	NEAR32 C USES ebx esi edi,
		ptrMem:PTR, bytData:BYTE, dwLength:DWORD

	LOCAL	dwTemp:DWORD

	movzx	eax, bytData
	mov	edi, ptrMem
	mov	ecx, dwLength
	mov	edx, eax
	shl	eax, 8
	or	eax, edx
	mov	edx, eax
	shl	eax, 16
	or	eax, edx

	IFNDEF	COMPACT_NOA_DECODER

	.IF	ecx >= 80H
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
		;
		;	SSE ��p�̃������t�B��
		; ------------------------------------------------------------
		;
		; ���[�ȃo�C�g�������o��
		;
		mov	dwTemp, eax
		.IF	edi & 01H
			mov	BYTE PTR [edi], al
			inc	edi
			dec	ecx
		.ENDIF
		.IF	edi & 02H
			mov	WORD PTR [edi], ax
			add	edi, 2
			sub	ecx, 2
		.ENDIF
		movss	xmm0, dwTemp
		.WHILE	edi & 0FH
			mov	DWORD PTR [edi], eax
			add	edi, 4
			sub	ecx, 4
		.ENDW
		;
		; 16�o�C�g�P�ʂ̃����������o��
		;
		shufps	xmm0, xmm0, 0
		sub	ecx, 10H
		.WHILE	!SIGN?
			movntps	[edi], xmm0
			add	edi, 10H
			sub	ecx, 10H
		.ENDW
		add	ecx, 10H
		sfence
	.ENDIF
	.ENDIF

	;
	; ���[�ȃo�C�g�������o��
	;
	.IF	ecx == 0
		ret
	.ENDIF
	.WHILE	edi & 03H
		mov	BYTE PTR [edi], al
		inc	edi
		dec	ecx
		.IF	ZERO?
			ret
		.ENDIF
	.ENDW
	;
	; 8 �o�C�g�P�ʂ̃������̏����o��
	;
	sub	ecx, 8
	.WHILE	!SIGN?
		mov	DWORD PTR [edi], eax
		mov	DWORD PTR [edi + 4], eax
		add	edi, 8
		sub	ecx, 8
	.ENDW
	;
	; �]�����o�C�g�̏����o��
	;
	add	ecx, 8
	.WHILE	!ZERO?
		mov	BYTE PTR [edi], al
		inc	edi
		dec	ecx
	.ENDW

	ELSE
		pushfd
		cld
		mov	edx, ecx
		shr	ecx, 2
		and	edx, 03H
		rep	stosd
		mov	ecx, edx
		rep	stosb
		popfd
	ENDIF

	ret

eslFillMemory	ENDP

;
;	�������]��
; ----------------------------------------------------------------------------
eslMoveMemory	PROC	NEAR32 C USES ebx esi edi,
		ptrDst:PTR, ptrSrc:PTR, dwLength:DWORD

	mov	esi, ptrSrc
	mov	edi, ptrDst
	mov	ecx, dwLength
	;
	IFNDEF	COMPACT_NOA_DECODER
	.IF	esi > edi
	; --------------------------------------------------------------------
	;	�������������]��
	; --------------------------------------------------------------------
	.IF	ecx >= 80H
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
		;
		;	SSE ��p�̃������]��
		; ------------------------------------------------------------
		;
		; ���[�ȃo�C�g��]������
		;
		.IF	edi & 01H
			mov	al, BYTE PTR [esi]
			inc	esi
			mov	BYTE PTR [edi], al
			inc	edi
			dec	ecx
		.ENDIF
		.IF	edi & 02H
			mov	ax, WORD PTR [esi]
			add	esi, 2
			mov	WORD PTR [edi], ax
			add	edi, 2
			sub	ecx, 2
		.ENDIF
		.WHILE	edi & 0FH
			mov	eax, DWORD PTR [esi]
			add	esi, 4
			mov	DWORD PTR [edi], eax
			add	edi, 4
			sub	ecx, 4
		.ENDW
		;
		; 16/32 �o�C�g�P�ʂ̃������]��
		;
		.IF	!(esi & 0FH)
			sub	ecx, 200H
			.WHILE	!SIGN?
				movaps	xmm0, [esi]
				prefetchnta	[esi + 200H]
				movaps	xmm1, [esi + 10H]
				add	esi, 20H
				movntps	[edi], xmm0
				movntps	[edi + 10H], xmm1
				add	edi, 20H
				sub	ecx, 20H
			.ENDW
			add	ecx, 200H - 20H
			.WHILE	!SIGN?
				movaps	xmm0, [esi]
				movaps	xmm1, [esi + 10H]
				add	esi, 20H
				movntps	[edi], xmm0
				movntps	[edi + 10H], xmm1
				add	edi, 20H
				sub	ecx, 20H
			.ENDW
			add	ecx, 20H
			sfence
		.ELSE
			sub	ecx, 10H
			.WHILE	!SIGN?
				movups	xmm0, [esi]
				add	esi, 10H
				movntps	[edi], xmm0
				add	edi, 10H
				sub	ecx, 10H
			.ENDW
			add	ecx, 10H
			sfence
		.ENDIF
	.ENDIF
	.ENDIF

	;
	; ���[�ȃo�C�g��]��
	;
	.IF	ecx == 0
		ret
	.ENDIF
	.WHILE	edi & 03H
		mov	al, BYTE PTR [esi]
		inc	esi
		mov	BYTE PTR [edi], al
		inc	edi
		dec	ecx
		.IF	ZERO?
			ret
		.ENDIF
	.ENDW
	;
	; 8 �o�C�g�P�ʂ̃������̓]��
	;
	.IF	ecx >= 8
		mov	eax, DWORD PTR [esi]
		mov	edx, DWORD PTR [esi + 4]
		add	esi, 8
		sub	ecx, 8 + 8
		.WHILE	!SIGN?
			mov	DWORD PTR [edi], eax
			mov	eax, DWORD PTR [esi]
			mov	DWORD PTR [edi + 4], edx
			add	edi, 8
			mov	edx, DWORD PTR [esi + 4]
			add	esi, 8
			sub	ecx, 8
		.ENDW
		mov	DWORD PTR [edi], eax
		mov	DWORD PTR [edi + 4], edx
		add	edi, 8
		add	ecx, 8
	.ENDIF
	;
	; �]�����o�C�g�̓]��
	;
	.WHILE	!ZERO?
		mov	al, BYTE PTR [esi]
		inc	esi
		mov	BYTE PTR [edi], al
		inc	edi
		dec	ecx
	.ENDW
	ret

	.ELSEIF	esi < edi
	; --------------------------------------------------------------------
	;	�t�����������]��
	; --------------------------------------------------------------------
	add	esi, ecx
	add	edi, ecx
	.IF	ecx >= 80H
	.IF	ERI_EnabledProcessorType & ERI_USE_XMM_P3
		;
		;	SSE ��p�̃������]��
		; ------------------------------------------------------------
		;
		; ���[�ȃo�C�g��]������
		;
		.IF	edi & 01H
			mov	al, BYTE PTR [esi - 1]
			dec	esi
			mov	BYTE PTR [edi - 1], al
			dec	edi
			dec	ecx
		.ENDIF
		.IF	edi & 02H
			mov	ax, WORD PTR [esi - 2]
			sub	esi, 2
			mov	WORD PTR [edi - 2], ax
			sub	edi, 2
			sub	ecx, 2
		.ENDIF
		.WHILE	edi & 0FH
			mov	eax, DWORD PTR [esi - 4]
			sub	esi, 4
			mov	DWORD PTR [edi - 4], eax
			sub	edi, 4
			sub	ecx, 4
		.ENDW
		;
		; 16/32 �o�C�g�P�ʂ̃������]��
		;
		.IF	!(esi & 0FH)
			sub	ecx, 200H
			.WHILE	!SIGN?
				movaps	xmm0, [esi - 10H]
				prefetchnta	[esi - 200H]
				movaps	xmm1, [esi - 20H]
				sub	esi, 20H
				movntps	[edi - 10H], xmm0
				movntps	[edi - 20H], xmm1
				sub	edi, 20H
				sub	ecx, 20H
			.ENDW
			add	ecx, 200H - 20H
			.WHILE	!SIGN?
				movaps	xmm0, [esi - 10H]
				movaps	xmm1, [esi - 20H]
				sub	esi, 20H
				movntps	[edi - 10H], xmm0
				movntps	[edi - 20H], xmm1
				sub	edi, 20H
				sub	ecx, 20H
			.ENDW
			add	ecx, 20H
			sfence
		.ELSE
			sub	ecx, 10H
			.WHILE	!SIGN?
				movups	xmm0, [esi - 10H]
				sub	esi, 10H
				movntps	[edi - 10H], xmm0
				sub	edi, 10H
				sub	ecx, 10H
			.ENDW
			add	ecx, 10H
			sfence
		.ENDIF
	.ENDIF
	.ENDIF

	;
	; ���[�ȃo�C�g��]��
	;
	.IF	ecx == 0
		ret
	.ENDIF
	.WHILE	edi & 03H
		mov	al, BYTE PTR [esi - 1]
		dec	esi
		mov	BYTE PTR [edi - 1], al
		dec	edi
		dec	ecx
		.IF	ZERO?
			ret
		.ENDIF
	.ENDW
	;
	; 8 �o�C�g�P�ʂ̃������̓]��
	;
	.IF	ecx >= 8
		mov	eax, DWORD PTR [esi - 4]
		mov	edx, DWORD PTR [esi - 8]
		sub	esi, 8
		sub	ecx, 8 + 8
		.WHILE	!SIGN?
			mov	DWORD PTR [edi - 4], eax
			mov	eax, DWORD PTR [esi - 4]
			mov	DWORD PTR [edi - 8], edx
			sub	edi, 8
			mov	edx, DWORD PTR [esi - 8]
			sub	esi, 8
			sub	ecx, 8
		.ENDW
		mov	DWORD PTR [edi - 4], eax
		mov	DWORD PTR [edi - 8], edx
		sub	edi, 8
		add	ecx, 8
	.ENDIF
	;
	; �]�����o�C�g�̓]��
	;
	.WHILE	!ZERO?
		mov	al, BYTE PTR [esi - 1]
		dec	esi
		mov	BYTE PTR [edi - 1], al
		dec	edi
		dec	ecx
	.ENDW

	.ENDIF

	ELSE
		pushfd
		.IF	esi > edi
			cld
			mov	edx, ecx
			shr	ecx, 2
			and	edx, 03H
			rep	movsd
			mov	ecx, edx
			rep	movsb
		.ELSE
			std
			.IF	!(ecx & 03H)
				lea	esi, [esi + ecx - 4]
				lea	edi, [edi + ecx - 4]
				shr	ecx, 2
				rep	movsd
			.ELSE
				lea	esi, [esi + ecx - 1]
				lea	edi, [edi + ecx - 1]
				rep	movsb
			.ENDIF
		.ENDIF
		popfd
	ENDIF
	;
	ret

eslMoveMemory	ENDP


CodeSeg	ENDS

	END
