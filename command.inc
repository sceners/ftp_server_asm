;
; FTP Server ver 0.2
;
; written by caspar /CookieCrK/ in win32asm
;
; caspar@polishscene.dhs.org || caspar@ae.pl
; http://www.cookiecrk.org
;


.code


;********************************************************************************************
;					CDUP
;********************************************************************************************


CommCdup PROC , Path:DWORD

	push [Path]
	call lstrlen

	cmp eax, 1
	jle CdupOK

	mov edi, [Path]
	mov esi, edi
	add edi, eax
	dec edi

CdupNext:

	dec edi

	cmp byte ptr [edi], '\'
	jne CdupNext

	inc edi
	mov byte ptr [edi], 0

CdupOK:

	inc esi

	push esi
	call SetCurrentDirectoryA

	ret

CommCdup ENDP


;********************************************************************************************
;					PWD
;********************************************************************************************


CommPwd PROC uses esi, Path:DWORD, Command:DWORD

	mov esi, offset CurDir

	push esi
	push [Command]
	call lstrcpy

	push [Path]
	push [Command]
	call lstrcat

	add esi, 6

	push esi
	push [Command]
	call lstrcat

	ret

CommPwd ENDP


;********************************************************************************************
;					TYPE
;********************************************************************************************


CommType PROC uses edi esi edx, Command:DWORD

	mov esi, [Command]
	add [Command], 5

	xor eax, eax
	mov edi, [Command]
	mov al, byte ptr [edi]

	cmp al, 'A'
	je TypeNext

	cmp al, 'a'
	je TypeNextL

	cmp al, 'I'
	je TypeNext

	cmp al, 'i'
	je TypeNextL

	mov eax, offset CommNotImP

	ret

TypeNextL:

	sub al, 20h

TypeNext:

	sub [Command], 5

	push eax

	push offset TypeOK
	push [Command]
	call lstrcpy

	add [Command], 16
	mov edi, [Command]

	pop eax

	mov ah, '.'
	mov word ptr [edi], ax
	add edi, 2
	mov word ptr [edi], 0a0dh
	add edi, 2
	mov byte ptr [edi], 0

	mov eax, esi

	ret

CommType ENDP


;********************************************************************************************
;					PORT
;********************************************************************************************


CommPort PROC uses edi esi, Command:DWORD
	LOCAL dsock:DWORD
	LOCAL dsin:sockaddr_in

	add [Command], 5
	mov esi, [Command]
	mov edi, esi
	xor ecx, ecx

PortNext:

	inc edi
	cmp byte ptr [edi], ','
	jne PortNext

	cmp ecx,3
	jnl PortCol

	mov byte ptr [edi], '.'
	inc ecx

	jmp PortNext

PortCol:

	mov byte ptr [edi], 0
	inc edi

	xor eax, eax
	xor ecx, ecx
	xor edx, edx

PortRe:

	mov cl, byte ptr [edi]
	cmp cl, ','
	jz PortKo
	sub cl, 30h
	imul eax, 0ah
	add eax, ecx
	inc edi
	jmp PortRe

PortKo:

	inc edi
	shl eax,8

PortRe2:

	mov cl, byte ptr [edi]
	cmp cl, 0
	jz PortKo2
	sub cl, 30h
	imul edx, 0ah
	add edx, ecx
	inc edi
	jmp PortRe2

PortKo2:

	add eax, edx
	push eax

	push 0
	push SOCK_STREAM
	push PF_INET
	call socket

	mov [dsock], eax

	push offset iocom
	push FIONBIO
	push [dsock]
	call ioctlsocket

	mov dsin.sin_family, PF_INET

	call htons

	mov dsin.sin_port, ax

	push esi
	call inet_addr

	mov dsin.sin_addr, eax

	lea edx, dsin

	push offset len
	push edx
	push [dsock]
	call connect

	cmp eax, 0
	je PortOK

	mov eax, 0

	ret

PortOK:

	mov eax, [dsock]

	ret

CommPort ENDP


;********************************************************************************************
;					LIST
;********************************************************************************************


CommList PROC uses edx edi esi, Param:DWORD
	LOCAL file_info:WIN32_FIND_DATA
	LOCAL Path:DWORD
	LOCAL ListData:BYTE:100h
	LOCAL FSize:BYTE:0Bh

	mov eax, [Param]
	mov [Path], eax
	add [Path], 9
	add [Param], 4

	push [Path]
	call lstrlen

	cmp eax, 1
	jle ListRoot

	mov edi, [Path]
	add edi, eax

	xor eax, eax
	mov eax, '*.*'

	mov [edi], eax
	add edi, 4
	mov byte ptr [edi], 0

	lea edx, file_info

	push edx
	push [Path]
	call FindFirstFileA

	mov esi, eax
	lea edi, ListData

CommListNext:

	lea edx, file_info
	and dword ptr [edx], 10h
	cmp dword ptr [edx], FILE_ATTRIBUTE_DIRECTORY
	jne CommListNotDir

	mov byte ptr [edi], 'd'

	jmp CommListDir

CommListNotDir:

	mov byte ptr [edi], '-'

CommListDir:

	inc edi

	push offset Rwx
	push edi
	call lstrcpy

	add edi, 40

	mov eax, [file_info.nFileSizeLow]

	lea edx, FSize

	push eax
	push offset H2D
	push edx
	call _wsprintfA

	add esp, 12

	sub edi, eax
	push eax

	lea edx, FSize

	push edx
	push edi
	call lstrcpy

	pop eax
	add edi, eax

	push offset RDate
	push edi
	call lstrcpy

	add edi, 14

	lea edx, file_info.cFileName

	push edx
	push edi
	call lstrcpy

	lea edi, ListData

	push edi
	call lstrlen

	mov word ptr [edi+eax], 0a0dh
	add eax, 2

	mov edx, [Param]
	mov edx, [edx]

	push 0
	push eax
	push edi
	push edx
	call send

	lea edx, file_info

	push edx
	push esi
	call FindNextFileA

	cmp eax, 0
	jnz CommListNext

	push esi
	call FindClose

	jmp ListEnd

ListRoot:

	push 100h
	push GMEM_ZEROINIT
	call GlobalAlloc

	mov esi, eax
	lea edi, ListData

	push eax
	push 100h
	call GetLogicalDriveStringsA

ListGetType:

	push esi
	call GetDriveTypeA	

	cmp eax, DRIVE_FIXED
	je ListSend

	push esi
	call lstrlen

	add esi, eax
	inc esi

	cmp eax, 0
	je ListEnd

	jmp ListGetType

ListSend:

	mov byte ptr [edi], 'd'

	inc edi

	push offset Rwx
	push edi
	call lstrcpy

	add edi, 39

	mov byte ptr [edi], 30h
	inc edi

	push offset RDate
	push edi
	call lstrcpy

	add edi, 14

	push esi
	push edi
	call lstrcpy

	lea edi, ListData

	push edi
	call lstrlen

	mov word ptr [edi+eax], 0a0dh
	add eax, 2

	mov edx, [Param]
	mov edx, [edx]

	push 0
	push eax
	push edi
	push edx
	call send

	push esi
	call lstrlen

	add esi, eax
	inc esi

	jmp ListGetType

ListEnd:

	mov edx, [Param]
	mov edx, [edx]

	push edx
	call closesocket

	mov edx, [Param]
	sub edx, 4
	mov edx, [edx]

	push edx
	push offset TransferOK
	call Response

	ret

CommList ENDP


;********************************************************************************************
;					NLST
;********************************************************************************************


CommNlst PROC uses edx edi esi, Param:DWORD
	LOCAL file_info:WIN32_FIND_DATA
	LOCAL Path:DWORD
	LOCAL ListData:BYTE:100h

	mov eax, [Param]
	mov [Path], eax
	add [Path], 8
	add [Param], 4

	push [Path]
	call lstrlen

	cmp eax, 1
	jle NlstRoot

	mov edi, [Path]
	add edi, eax

	xor eax, eax
	mov eax, '*.*'

	mov [edi], eax
	add edi, 4
	mov byte ptr [edi], 0

	lea edx, file_info

	push edx
	push [Path]
	call FindFirstFileA

	mov esi, eax
	lea edi, ListData

NlstNext:

	lea edx, file_info.cFileName

	push edx
	push edi
	call lstrcpy

	push edi
	call lstrlen

	mov word ptr [edi+eax], 0a0dh
	add eax, 2

	mov edx, [Param]
	mov edx, [edx]

	push 0
	push eax
	push edi
	push edx
	call send

	lea edx, file_info

	push edx
	push esi
	call FindNextFileA

	cmp eax, 0
	jnz NlstNext

	push esi
	call FindClose

	jmp NlstEnd

NlstRoot:

	push 100h
	push GMEM_ZEROINIT
	call GlobalAlloc

	mov esi, eax
	lea edi, ListData

	push eax
	push 100h
	call GetLogicalDriveStringsA

NlstGetType:

	push esi
	call GetDriveTypeA	

	cmp eax, DRIVE_FIXED
	je NlstSend

	push esi
	call lstrlen

	add esi, eax
	inc esi

	cmp eax, 0
	je NlstEnd

	jmp NlstGetType

NlstSend:

	push esi
	push edi
	call lstrcpy

	push edi
	call lstrlen

	mov word ptr [edi+eax], 0a0dh
	add eax, 2

	mov edx, [Param]
	mov edx, [edx]

	push 0
	push eax
	push edi
	push edx
	call send

	push esi
	call lstrlen

	add esi, eax
	inc esi

	jmp NlstGetType

NlstEnd:

	mov edx, [Param]
	mov edx, [edx]

	push edx
	call closesocket

	mov edx, [Param]
	sub edx, 4
	mov edx, [edx]

	push edx
	push offset TransferOK
	call Response

	ret

CommNlst ENDP


;********************************************************************************************
;					CWD
;********************************************************************************************


CommCwd PROC uses edi, Path:DWORD, Command:DWORD
	LOCAL Check:BYTE:100h


	mov edi, [Command]
	add edi, 4
	lea esi, Check

	cmp byte ptr [edi],0
	je CwdCur

	cmp byte ptr [edi],'\'
	je CwdAbs

	mov edx, [Path]

	push edx
	push esi
	call lstrcpy

	push edi
	push esi
	call lstrcat

	push esi
	call lstrlen

	add esi, eax

	cmp byte ptr [esi-1], '\'
	je CwdAbsCheck

	mov byte ptr[esi], '\'
	mov byte ptr[esi+1], 0

	sub esi, eax

	jmp CwdCheck

CwdCur:

	mov esi, [Path]

	jmp CwdCheck

CwdAbs:

	push edi
	push esi
	call lstrcpy

	push esi
	call lstrlen

	inc esi

	cmp eax, 1
	je CwdOK

	dec esi
	add esi, eax

	cmp byte ptr [esi-1], '\'
	je CwdAbsCheck

	mov byte ptr[esi], '\'
	mov byte ptr[esi+1], 0

	sub esi, eax

	jmp CwdCheck

CwdAbsCheck:

	sub esi, eax

CwdCheck:

	inc esi

	push esi
	call SetCurrentDirectoryA

	cmp eax, 0
	jne CwdOK

	mov eax, offset ActionNot

	ret

CwdOK:

	dec esi

	mov edi, [Path]

	push esi
	push edi
	call lstrcpy

	mov esi, [Command]

	push offset DirChanged
	push esi
	call lstrcpy

	add esi, 26

	push edi
	push esi
	call lstrcat

	push esi
	call lstrlen

	add esi, eax

	mov edi, offset DirChanged
	add edi, 27

	push edi
	push esi
	call lstrcpy

	mov eax, [Command]

	ret

CommCwd ENDP


;********************************************************************************************
;					RETR
;********************************************************************************************


CommRetr PROC uses edi ebx ecx edx, Param:DWORD
	LOCAL hfile:DWORD
	LOCAL ile:DWORD
	LOCAL rest:DWORD
	LOCAL readd:DWORD
	LOCAL buf:BYTE:1000h


	mov edi, [Param]
	add edi, 8

	push 0
	push FILE_ATTRIBUTE_ARCHIVE
	push OPEN_EXISTING
	push 0
	push 0
	push GENERIC_READ
	push edi
	call CreateFileA

	cmp eax, -1
	je RetrFailed

	mov [hfile], eax

	mov edx, [Param]
	mov edx, [edx]

	push edx
	push offset OpenDataCon
	call Response

	push 0
	push [hfile]
	call GetFileSize

	mov ebx, 1000h
	xor edx, edx
	div ebx

	mov [ile], eax
	mov [rest], edx

	lea edi, buf
	mov esi, [Param]
	mov esi, [esi+4]

	cmp eax, 0
	je CommRetrBla

	xor ecx, ecx
	dec ecx

CommRetrNext:

	inc ecx
	mov eax, 1000h
	mul ecx

	push ecx

	push 0
	push 0
	push eax
	push [hfile]
	call SetFilePointer

	lea edx, readd

	push 0
	push edx
	push 1000h
	push edi
	push [hfile]
	call ReadFile

	push 0
	push [readd]
	push edi
	push esi
	call send

	pop ecx

	cmp ecx, [ile]
	jl CommRetrNext

	mov eax, [rest]
	cmp eax, 0
	jng TrComp

	mov eax, 1000h
	inc ecx
	mul ecx

CommRetrBla:

	push 0
	push 0
	push eax
	push [hfile]
	call SetFilePointer

	lea edx, readd

	push 0
	push edx
	push [rest]
	push edi
	push [hfile]
	call ReadFile

	push 0
	push [readd]
	push edi
	push esi
	call send

TrComp:

	push [hfile]
	call CloseHandle

	push esi
	call closesocket

	mov edi, [Param]
	mov edi, [edi]

	push edi
	push offset TransferOK
	call Response

	ret

RetrFailed:

	mov esi, [Param]
	mov esi, [esi+4]

	push esi
	call closesocket

	mov edi, [Param]
	mov edi, [edi]

	push edi
	push offset ReqActionNot
	call Response

	ret

CommRetr ENDP


;********************************************************************************************
;					DELE
;********************************************************************************************


CommDele PROC uses edi, Path:DWORD, Command:DWORD
	LOCAL Check:BYTE:100h

	mov edi, [Command]
	add edi, 5
	lea esi, Check

	cmp byte ptr [edi], '\'
	je DeleAbs

	mov edx, [Path]
	inc edx

	push edx
	push esi
	call lstrcpy

	push edi
	push esi
	call lstrcat

	jmp DeleDo

DeleAbs:

	inc edi

	push edi
	push esi
	call lstrcpy

DeleDo:

	push esi
	call DeleteFileA

	cmp eax, 0
	je DeleFailed

	mov eax, offset FileActionOK

	ret

DeleFailed:

	mov eax, offset ActionNot

	ret

CommDele ENDP


;********************************************************************************************
;					RMD
;********************************************************************************************


CommRmd PROC uses edi, Path:DWORD, Command:DWORD
	LOCAL Check:BYTE:100h

	mov edi, [Command]
	add edi, 4
	lea esi, Check

	cmp byte ptr [edi], '\'
	je RmdAbs

	mov edx, [Path]
	inc edx

	push edx
	push esi
	call lstrcpy

	push edi
	push esi
	call lstrcat

	jmp RmdDo

RmdAbs:

	inc edi

	push edi
	push esi
	call lstrcpy

RmdDo:

	push esi
	call RemoveDirectoryA

	cmp eax, 0
	je RmdFailed

	mov eax, offset FileActionOK

	ret

RmdFailed:

	mov eax, offset ActionNot

	ret

CommRmd ENDP


;********************************************************************************************
;					MKD
;********************************************************************************************


CommMkd PROC uses edi esi ecx, Path:DWORD, Command:DWORD
	LOCAL Check:BYTE:100h

	mov edi, [Command]
	add edi, 4
	lea esi, Check

	cmp byte ptr [edi], '\'
	je MkdAbs

	mov edx, [Path]
	inc edx

	cmp byte ptr [edx], 0
	je MkdFailed

	push edx
	push esi
	call lstrcpy

	push edi
	push esi
	call lstrcat

	jmp MkdDo

MkdAbs:

	push edi
	call lstrlen

	mov ecx, edi
	add ecx, eax

MkdNext:

	dec ecx

	cmp byte ptr [ecx], '\'
	jne MkdNext

	cmp edi, ecx
	je MkdFailed

	inc edi

	push edi
	push esi
	call lstrcpy

MkdDo:

	push 0
	push esi
	call CreateDirectoryA

	cmp eax, 0
	je MkdFailed

	dec esi
	mov edi, [Command]

	push offset DirCreated
	push edi
	call lstrcpy

	push esi
	push edi
	call lstrcat

	mov esi, offset DirCreated
	add esi, 6

	push esi
	push edi
	call lstrcat

	ret

MkdFailed:

	mov eax, offset ActionNot

	ret

CommMkd ENDP


;********************************************************************************************
;					STOR
;********************************************************************************************


CommStor PROC uses edi ebx ecx edx, Param:DWORD
	LOCAL fd_set:fd_setstruc
	LOCAL hfile:DWORD
	LOCAL ile:DWORD
	LOCAL write:DWORD
	LOCAL written:DWORD
	LOCAL buf:BYTE:1000h


	mov edi, [Param]
	add edi, 8

	push 0
	push FILE_ATTRIBUTE_ARCHIVE
	push CREATE_ALWAYS
	push 0
	push 0
	push GENERIC_WRITE
	push edi
	call CreateFileA

	cmp eax, -1
	je StorFailed

	mov [hfile], eax

	mov edx, [Param]
	mov edx, [edx]

	push edx
	push offset OpenDataCon
	call Response

	mov [fd_set.fd_count],1
	mov eax, [Param]
	add eax, 4
	mov eax, [eax]
	mov [fd_set.fd_array], eax

	mov esi, eax
	lea edi, buf
	mov [ile], 0

CommStorNext:

	lea eax, fd_set

	push 0
	push eax
	push 0
	push eax
	push 0
	call select

	push 0
	push 1000h
	push edi
	push esi
	call recv

	cmp eax,0
	je CommStorWritten

	mov [write], eax

	push 0
	push 0
	push [ile]
	push [hfile]
	call SetFilePointer

	lea edx, written

	push 0
	push edx
	push [write]
	push edi
	push [hfile]
	call WriteFile

	cmp eax, -1
	je StorFailed

	mov edx, [written]
	add [ile], edx

	jmp CommStorNext

CommStorWritten:

	push [hfile]
	call CloseHandle

	push esi
	call closesocket

	mov edi, [Param]
	mov edi, [edi]

	push edi
	push offset TransferOK
	call Response

	ret

StorFailed:

	mov esi, [Param]
	mov esi, [esi+4]

	push esi
	call closesocket

	mov edi, [Param]
	mov edi, [edi]

	push edi
	push offset ReqActionNot
	call Response

	ret

CommStor ENDP

