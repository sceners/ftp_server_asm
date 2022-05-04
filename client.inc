;
; FTP Server ver 0.2
;
; written by caspar /CookieCrK/ in win32asm
;
; caspar@polishscene.dhs.org || caspar@ae.pl
; http://www.cookiecrk.org
;


.code


ClientProc PROC , Param: DWORD
	LOCAL connsock:DWORD
	LOCAL command:BYTE:100h
	LOCAL buf:BYTE:100h
	LOCAL Path:BYTE:100h
	LOCAL dsock:DWORD
	LOCAL threadID:DWORD
	LOCAL disaddr:sockaddr
	LOCAL Bla:DWORD
	LOCAL Passwd:BYTE:11h

	mov [dsock], 0

	mov edx, [Param]
	mov edx, [edx]
	mov [connsock], edx

	add [Param], 4

	push offset UserNotLogged
	push [Param]
	call lstrcpy

	add [Users], 1

	call WriteStatus

	lea edx, Path

	mov byte ptr [edx], '\'
	mov byte ptr [edx+1], 0

	push [connsock]
	push offset WelcomeMsg
	call Response

WaitForLogin:

	lea ebx, command

	push [connsock]
	push ebx
	call Receive

	cmp eax, 0
	je _ret

	lea edi, command

	push edi
	push offset User
	call lstrcmpi

	cmp eax, 0
	jz UserLoggingIn

	push edi
	push offset Pass
	call lstrcmpi

	cmp eax, 0
	jz WrongSeq

	push edi
	push offset Help
	call lstrcmpi

	cmp eax, 0
	jz HelpComm

	push edi
	push offset Quit
	call lstrcmpi

	cmp eax, 0
	jz _ret

	push [connsock]
	push offset NotLogged
	call Response

	jmp WaitForLogin

HelpComm:

	push [connsock]
	push offset HelpReply
	call Response

	jmp WaitForLogin

WrongSeq:

	push [connsock]
	push offset BadSeq
	call Response

	jmp WaitForLogin

UserLoggingIn:

	lea esi, command
	add esi, 5

	lea edi, buf

	push esi
	push edi
	call lstrcpy

	push offset AnonName
	push esi
	call lstrcmpi

	cmp eax, 0
	jz AnonymousUser

	push [connsock]
	push offset UserOK
	call Response

	lea ebx, command

	push [connsock]
	push ebx
	call Receive

	cmp eax, 0
	je _ret

	lea ebx, command

	push ebx
	push offset Pass
	call lstrcmpi

	cmp eax, 0
	jz VerifyUser

	push [connsock]
	push offset NotLogged
	call Response

	jmp WaitForLogin

AnonymousUser:

	cmp [OpAllowAnon], 1
	jne AnonymousUserBad

	push [connsock]
	push offset AnonymousOK
	call Response

	lea ebx, command
	push ebx

	push offset LogAnonymous
	push ebx
	call lstrcpy

	call lstrlen

	lea ebx, command
	add ebx, eax	

	push [connsock]
	push ebx
	call Receive

	cmp eax, 0
	je _ret

	push ebx
	push offset Pass
	call lstrcmpi

	cmp eax, 0
	jz AnonymousPassword

	push [connsock]
	push offset NotLogged
	call Response

	jmp WaitForLogin

AnonymousPassword:

	mov eax, ebx
	add eax, 5

	push eax
	push ebx
	call lstrcpy

	lea ebx, command

	push 1
	push ebx
	call WriteLog

	lea ebx, buf

	jmp UserLogged

AnonymousUserBad:

	push [connsock]
	push offset AnonymousBad
	call Response

	jmp WaitForLogin

VerifyUser:

	lea ebx, buf

	push ebx
	push offset UserName
	call lstrcmpi

	cmp eax, 0
	jz UserSCorrect

	push 0
	push offset KeyHWnd
	push 0
	push KEY_ALL_ACCESS
	push 0
	push 0
	push 0
	push offset RegKeyU
	push HKEY_CURRENT_USER
	call RegCreateKeyExA

	lea esi, buf

	lea edi, Passwd
	mov [Bla], 11h
	lea edx, Bla

	push edx
	push edi
	push 0
	push 0
	push esi
	push [KeyHWnd]
	call RegQueryValueExA

	cmp eax, 0
	je UserCorrect

	push [connsock]
	push offset PassBad
	call Response

	jmp WaitForLogin

UserSCorrect:

	lea esi, command
	add esi, 5

	mov edi, offset UserPass

	push edi
	push esi
	call Crypt

	cmp eax, 0
	jnz UserLogged

	push [connsock]
	push offset PassBad
	call Response

	jmp WaitForLogin

UserCorrect:

	lea ebx, buf

	lea edi, Passwd
	mov esi, edi
	add esi, [Bla]
	mov byte ptr [esi], 0

	lea esi, command
	add esi, 5

	push edi
	push esi
	call Crypt

	cmp eax, 0
	jnz UserLogged

	push [connsock]
	push offset PassBad
	call Response

	jmp WaitForLogin

UserLogged:

	push ebx
	push [Param]
	call lstrcpy

	push [connsock]
	push offset PassOK
	call Response

WaitingForInput:

	lea ebx, command

	push [connsock]
	push ebx
	call Receive

	cmp eax, 0
	je _ret


	push ebx
	push offset Cdup
	call lstrcmpi

	cmp eax,0
	je Comm_Cdup

	push ebx
	push offset Pwd
	call lstrcmpi

	cmp eax,0
	je Comm_Pwd

	push ebx
	push offset Type1
	call lstrcmpi

	cmp eax,0
	je Comm_Type

	push ebx
	push offset Port
	call lstrcmpi

	cmp eax,0
	je Comm_Port

	push ebx
	push offset List
	call lstrcmpi

	cmp eax,0
	je Comm_List

	push ebx
	push offset Nlst
	call lstrcmpi

	cmp eax,0
	je Comm_Nlst

	push ebx
	push offset Cwd1
	call lstrcmpi

	cmp eax,0
	je Comm_Cwd

	push ebx
	push offset Retr
	call lstrcmpi

	cmp eax,0
	je Comm_Retr

	push ebx
	push offset Dele
	call lstrcmpi

	cmp eax,0
	je Comm_Dele

	push ebx
	push offset Rmd
	call lstrcmpi

	cmp eax,0
	je Comm_Rmd

	push ebx
	push offset Mkd
	call lstrcmpi

	cmp eax,0
	je Comm_Mkd

	push ebx
	push offset Stor
	call lstrcmpi

	cmp eax,0
	je Comm_Stor

	push ebx
	push offset Quit
	call lstrcmpi

	cmp eax,0
	je _ret

	push ebx
	push offset Help
	call lstrcmpi

	cmp eax,0
	je Comm_Help

	push ebx
	push offset Noop
	call lstrcmpi

	cmp eax,0
	je Comm_Noop


	push ebx
	push offset Acct
	call lstrcmpi

	cmp eax,0
	je Comm_NotIm

	push ebx
	push offset Smnt
	call lstrcmpi

	cmp eax,0
	je Comm_NotIm

	push ebx
	push offset Rein
	call lstrcmpi

	cmp eax,0
	je Comm_NotIm


	push [connsock]
	push offset CommNotRec
	call Response

	jmp WaitingForInput


;********************************************************************************************
;					command functions
;********************************************************************************************


Comm_Cdup:

	lea edx, Path

	push edx
	call CommCdup

	push [connsock]
	push offset CommOK
	call Response

	jmp WaitingForInput


Comm_Pwd:

	lea edx, command

	push edx

	lea edx, Path

	push edx
	call CommPwd

	push [connsock]
	push eax
	call Response

	jmp WaitingForInput


Comm_Type:

	lea edx, command

	push edx
	call CommType

	push [connsock]
	push eax
	call Response

	jmp WaitingForInput


Comm_Port:

	cmp [dsock], 0
	je Comm_PortNext

	push [dsock]
	call closesocket

Comm_PortNext:

	lea edx, command

	push edx
	call CommPort

	cmp eax, 0
	jne Comm_PortOK

	push [connsock]
	push offset SynErrPa
	call Response

	jmp WaitingForInput

Comm_PortOK:

	mov [dsock], eax

	push [connsock]
	push offset CommOK
	call Response

	jmp WaitingForInput


Comm_List:

	push [connsock]
	push offset OpenDataCon
	call Response

	push 108h
	push GMEM_ZEROINIT
	call GlobalAlloc

	cmp eax, 0
	je Comm_ListNOK

	add eax, 8

	lea edi, Path

	push edi
	push eax
	call lstrcpy

	sub eax, 4
	mov edi, [dsock]
	mov [eax], edi
	sub eax, 4
	mov edi, [connsock]
	mov [eax], edi

	lea edi, threadID

	push edi
	push 0
	push eax
	push offset CommList
	push 0
	push 0
	call CreateThread

	cmp eax, 0
	je Comm_ListNOK

	push eax
	call CloseHandle

	jmp WaitingForInput

Comm_ListNOK:

	push [connsock]
	push offset ReqActionAb
	call Response

	jmp WaitingForInput


Comm_Nlst:

	push [connsock]
	push offset OpenDataCon
	call Response

	push 108h
	push GMEM_ZEROINIT
	call GlobalAlloc

	cmp eax, 0
	je Comm_NlstNOK

	mov esi, eax
	add esi, 8

	lea edi, command
	add edi, 5

	cmp byte ptr [edi], '\'
	je Comm_NlstAbs

	lea edx, Path
	inc edx

	push edx
	push esi
	call lstrcpy

	cmp byte ptr [edi], 0
	je Comm_NlstSend

	push edi
	push esi
	call lstrcat

	jmp Comm_NlstSend

Comm_NlstAbs:

	inc edi

	push edi
	push esi
	call lstrcpy

Comm_NlstSend:

	sub esi, 4
	mov edi, [dsock]
	mov [esi], edi
	sub esi, 4
	mov edi, [connsock]
	mov [esi], edi

	lea edi, threadID

	push edi
	push 0
	push esi
	push offset CommNlst
	push 0
	push 0
	call CreateThread

	cmp eax, 0
	je Comm_NlstNOK

	push eax
	call CloseHandle

	jmp WaitingForInput

Comm_NlstNOK:

	push [connsock]
	push offset ReqActionAb
	call Response

	jmp WaitingForInput


Comm_Cwd:

	lea edx, command

	add edx, 4

	push edx
	push offset DirUp
	call lstrcmp

	cmp eax, 0
	je Comm_Cdup

	lea edx, command

	push edx

	lea edx, Path

	push edx
	call CommCwd

	push [connsock]
	push eax
	call Response

	jmp WaitingForInput


Comm_Retr:

	push 108h
	push GMEM_ZEROINIT
	call GlobalAlloc

	cmp eax, 0
	je Comm_RetrNOK

	mov esi, eax
	add esi, 8

	lea edi, command
	add edi, 5

	cmp byte ptr [edi], '\'
	je Comm_RetrAbs

	lea edx, Path
	inc edx

	push edx
	push esi
	call lstrcpy

	push edi
	push esi
	call lstrcat

	jmp Comm_RetrSend

Comm_RetrAbs:

	inc edi

	push edi
	push esi
	call lstrcpy

Comm_RetrSend:

	sub esi, 4
	mov edi, [dsock]
	mov [esi], edi
	sub esi, 4
	mov edi, [connsock]
	mov [esi], edi

	lea edi, threadID

	push edi
	push 0
	push esi
	push offset CommRetr
	push 0
	push 0
	call CreateThread

	cmp eax, 0
	je Comm_RetrNOK

	push eax
	call CloseHandle

	jmp WaitingForInput

Comm_RetrNOK:

	push [connsock]
	push offset ReqActionAb
	call Response

	jmp WaitingForInput


Comm_Dele:

	lea edx, command

	push edx

	lea edx, Path

	push edx
	call CommDele

	push [connsock]
	push eax
	call Response

	jmp WaitingForInput


Comm_Rmd:

	lea edx, command

	push edx

	lea edx, Path

	push edx
	call CommRmd

	push [connsock]
	push eax
	call Response

	jmp WaitingForInput


Comm_Mkd:

	lea edx, command

	push edx

	lea edx, Path

	push edx
	call CommMkd

	push [connsock]
	push eax
	call Response

	jmp WaitingForInput


Comm_Stor:

	push 108h
	push GMEM_ZEROINIT
	call GlobalAlloc

	cmp eax, 0
	je Comm_StorNOK

	mov esi, eax
	add esi, 8

	lea edi, command
	add edi, 5

	cmp byte ptr [edi], '\'
	je Comm_StorAbs

	lea edx, Path
	inc edx

	push edx
	push esi
	call lstrcpy

	push edi
	push esi
	call lstrcat

	jmp Comm_StorSend

Comm_StorAbs:

	inc edi

	push edi
	push esi
	call lstrcpy

Comm_StorSend:

	sub esi, 4
	mov edi, [dsock]
	mov [esi], edi
	sub esi, 4
	mov edi, [connsock]
	mov [esi], edi

	lea edi, threadID

	push edi
	push 0
	push esi
	push offset CommStor
	push 0
	push 0
	call CreateThread

	cmp eax, 0
	je Comm_StorNOK

	push eax
	call CloseHandle

	jmp WaitingForInput

Comm_StorNOK:

	push [connsock]
	push offset ReqActionAb
	call Response

	jmp WaitingForInput


Comm_Help:

	push [connsock]
	push offset HelpReply
	call Response

	jmp WaitingForInput


Comm_Noop:

	push [connsock]
	push offset CommOK
	call Response

	jmp WaitingForInput


Comm_NotIm:

	push [connsock]
	push offset CommNotIm
	call Response

	jmp WaitingForInput


;********************************************************************************************
;					_ret
;********************************************************************************************


_ret:

	sub [Users], 1

	call WriteStatus

	sub [Param], 4
	mov edx, [Param]

	cmp dword ptr [edx], 0
	je alreadyclosed

	lea edi, disaddr

	push offset len
	push edi
	push [connsock]
	call getpeername

	mov eax, dword ptr [disaddr+4]

	push eax
	call inet_ntoa

	mov esi, eax

	lea edi, command

	push offset LogDisconnect
	push edi
	call lstrcpy

	push esi
	push edi
	call lstrcat

	push 1
	push eax
	call WriteLog

	push [connsock]
	call closesocket

	mov edx, [Param]
	mov dword ptr [edx], 0

alreadyclosed:

	push [dsock]
	call closesocket

	ret

ClientProc ENDP


;********************************************************************************************
;					PROCEDURES 
;********************************************************************************************

;********************************************************************************************
;					Receive
;********************************************************************************************


Receive PROC uses ecx edx edi esi, bufaddr:DWORD, connsock:DWORD
	LOCAL	fd_set:fd_setstruc

	mov [fd_set.fd_count],1
	mov eax, [connsock]
	mov [fd_set.fd_array], eax

	xor esi, esi
	mov edi, [bufaddr]

StillWtg:

	lea eax, fd_set

	push 0
	push 0
	push 0
	push eax
	push 0
	call select

	push 0
	push 100h
	push edi
	push [connsock]
	call recv

	cmp eax,0
	je Disconn

	cmp dword ptr [edi], 0
	je Disconn

	add edi, eax
	sub edi,2
	cmp word ptr [edi], 0a0dh
	je CNext

	add edi,2

	jmp StillWtg

CNext:

	mov edi, [bufaddr]
	mov edx, edi
	dec edi

stillsearch:

	inc edi
	cmp byte ptr [edi],0dh
	jne stillsearch

	mov word ptr [edi],0

	sub edi, edx
	mov ecx, edi
	mov edi, edx
	inc edi

	mov eax, 1

	cmp ecx, 0
	je BackFromRec

nextsearch:

	cmp byte ptr [edi],20h
	jne nstill

	mov byte ptr [edi],0

	jmp BackFromRec

nstill:

	inc edi
	loop nextsearch

	jmp BackFromRec

Disconn:

	mov eax,0

BackFromRec:

	ret

Receive ENDP


;********************************************************************************************
;					Response
;********************************************************************************************


Response PROC uses edx, msgaddr:DWORD, connsock:DWORD


	mov edx, [msgaddr]

	push edx
	call lstrlen

	push 0
	push eax
	push edx
	push [connsock]
	call send

	ret

Response ENDP


;********************************************************************************************
;					Crypt 
;********************************************************************************************


Crypt PROC uses edi esi ecx edx, passwrd:DWORD, validpass:DWORD

	xor edx, edx
	mov esi, [passwrd]
	mov edi, esi

Xoring:

	mov dl, byte ptr [esi+1]
	cmp dl,0
	je XorLast

	xor byte ptr [esi], dl
	inc esi

	jmp Xoring

XorLast:

	mov dl, byte ptr [edi]
	xor byte ptr [esi], dl

	mov ecx,10

	mov esi, [validpass]

	repe cmpsb

	cmp ecx,0
	je PasswordChecked

	mov eax,0

	ret

PasswordChecked:

	mov eax,1

	ret

Crypt ENDP

