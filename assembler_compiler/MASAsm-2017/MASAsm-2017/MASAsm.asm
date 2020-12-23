.586
	.model flat, stdcall
	includelib libucrt.lib
	includelib kernel32.lib
		includelib ../Debug/MASAsmLibrary.lib
	ExitProcess PROTO :DWORD

	outc PROTO: DWORD
	outd PROTO: DWORD
	strln PROTO: DWORD
	powr PROTO: DWORD, :DWORD

.stack 4096
.const
	L0 DWORD 7
	L1 DWORD 4
	L2 DWORD 5
	L3 BYTE "helloworld, мой контольный пример", 0
	L4 BYTE "mas-2017", 0
	L5 BYTE "01071998", 0
	L6 DWORD 0
.data
	masl DWORD ?
	masaa DWORD ?
	maina DWORD ?
	mainz DWORD ?
	mainx DWORD ?
	mainy DWORD ?
	mainp DWORD ?
	mainu DWORD ?
	maink DWORD ?

.code

mas PROC mass : SDWORD, mash : SDWORD
	push mass
	push mash
	pop eax
	pop ebx
	add eax, ebx
	push eax
	push L0
	pop eax
	pop ebx
	mul ebx
	push eax
	pop masl
	push masl
		jmp local0
local0:
	pop eax
	ret
mas ENDP

masa PROC masastr : DWORD
	push masastr
	pop edx
	push masastr
		call strln
	push eax
	pop masaa
	push masaa
		jmp local1
local1:
	pop eax
	ret
masa ENDP

main PROC
	push L1
	pop mainx
	push L2
	pop mainy
	push offset L3
	pop mainz
	push mainz
		call outc
	push offset L4
	pop maink
	push maink
	pop edx
	push maink
		call masa
	push eax
	pop mainp
	push mainp
		call outd
	push mainx
	push mainy
	pop edx
	pop edx
	push mainy
	push mainx
		call mas
	push eax
	pop maina
	push maina
		call outd
	push mainy
	push L1
	pop edx
	pop edx
	push L1
	push mainy
		call powr
	push eax
	pop mainu
	push mainu
		call outd
	push offset L5
		call outc
	push 0
		jmp theend
theend:
	call ExitProcess
main ENDP
end main