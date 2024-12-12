;Initialize the SRL Engine
<0055E000>
@SRL:	
	mov edx, dword ptr ss:[esp+0x80]
	pushad
	call @StrInject
	push 0x00
	call eax
	popad
	jmp TR

;Inject An string
<0055E020>
@Inject:
	pushad
	push ebx
	call @IsStr
	test eax, eax
	je @RNS
	call @StrInject
	push ebx
	call eax
	mov [esp+0x10], EAX
@RNS:
	popad
	push ebx
	mov word ptr ss:[esp+0x38], ax
	jmp RSI1

;Inject Choices
<0055E1C0>
@Inject2:
	pushad
	push esi
	call @IsStr
	test eax, eax
	je @I2NS
	push esi
	call @StrInject
	call eax
	mov [esp+4], eax
@I2NS:
	popad
	push esi ; the esi
	mov word ptr ss:[esp+0x18], dx
	jmp Ret2


;Push Str
<0055E060>
@IsStr:
	pop eax
	xchg [esp], eax
	pushad
	cmp word [eax], 0
	jne @Cnt
	sub esp, 4
	jmp @False
@Cnt:
	mov ebx, 0
	push eax
@NStr:
	mov eax, [esp]
	push eax
	push ebx
	call @GetDB
	mov ebx, eax
	test eax, eax
	je @Abort
	push eax
	call @StartsWith
	test eax, eax
	jne @False
	jmp @NStr
@False:
	mov eax, 0
	jmp @RR
@Abort:
	add esp, 4
@True:
	mov eax, 1
	jmp @RR
@RR:
	add esp, 4
	mov [esp+0x1C], eax
	popad
	ret

	
;Push Str
;Push Match
<0055E0D0>
@StartsWith:
	pop eax
	xchg [esp], eax
	pushad
	mov ebx, [esp+0x24];Get Str
	xchg eax, ebx;EAX = Str, EBX = Match
@NC:
	mov cx, [eax]
	mov dx, [ebx]
	test cx, cx
	je @End
	test dx, dx
	je @End
	add eax, 2
	add ebx, 2
	cmp dx, cx
	je @NC
	jmp @NOK
@End:
	test dx, dx
	je @OK
	jmp @NOK
@OK:
	mov eax, 1
	jmp @R1
@NOK:
	mov eax, 0
	jmp @R1
@R1:
	mov [esp+0x1C], eax;Write EAX
	popad
	add esp, 8;Fix Stack
	push [esp-8]
	ret

;Push 0 or last entry pointer
<0055E820>
@GetDB:
	pop eax
	xchg [esp], eax
	test eax, eax
	jne @Resume
	call @nxr
@nxr:
	pop eax
	add eax, 0x4F;Diff to the @DB Pos
	ret
@Resume:
	cmp word [eax], 0
	je @null
	jmp @ncr
@null:
	mov eax, 0
	ret
@ncr:
	add eax, 2
	cmp word [eax], 0
	jne @ncr
	add eax, 2
	cmp word [eax], 0
	je @null
	ret

;Is Better keep in the end but with space to add more entries
<0055E880>
@DB:
	dw 0x73;ST
	dw 0x74
	dw 0x00
	
	dw 0x65;EV
	dw 0x76
	dw 0x00
	
	dw 0x62;BG
	dw 0x67
	dw 0x00
	
	dw 0x2A; 
	dw 0x00
	
	dw 0x40;@
	dw 0x00
	
	dw 0x74;text
	dw 0x65
	dw 0x78
	dw 0x74
	dw 0x00
	
	dw 0x57;Wait
	dw 0x61
	dw 0x69
	dw 0x74
	dw 0x00
	
	dw 0x63;char
	dw 0x68
	dw 0x61
	dw 0x72
	dw 0x00
	
	dw 0x74;timer
	dw 0x69
	dw 0x6D
	dw 0x65
	dw 0x72
	dw 0x00
	
	dw 0x65;effect
	dw 0x66
	dw 0x66
	dw 0x65
	dw 0x63
	dw 0x74
	dw 0x00
	
	dw 0x6D;movie
	dw 0x6F
	dw 0x76
	dw 0x69
	dw 0x65
	dw 0x00
	
	dw 0x00;Null = End


<0055E9E0>
@StrInject:
	call @nxt
@nxt:
	pop eax
	sub eax, 0x1836
	mov eax, [eax]
	ret