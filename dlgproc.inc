;
; FTP Server ver 0.2
;
; written by caspar /CookieCrK/ in win32asm
;
; caspar@polishscene.dhs.org || caspar@ae.pl
; http://www.cookiecrk.org
;


.code


DlgOptionsProc PROC uses ebx edi esi, hwnd:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD

	cmp wmsg, WM_INITDIALOG
	je opinitdialog
	cmp wmsg, WM_CLOSE
	je opclose
	cmp wmsg, WM_COMMAND
	je opcommand

opnot:

	mov eax, 0

	ret

op:

	mov eax, 1

	ret

opinitdialog:

	cmp OpAllowAnon, 1
	jne dontsetanon

	push 0
	push 1
	push BM_SETCHECK
	push IDC_ANON
	push [hwnd]
	call SendDlgItemMessageA

dontsetanon:

	cmp OpLogtoFile, 1
	jne dontsetlog

	push 0
	push 1
	push BM_SETCHECK
	push IDC_LOG
	push [hwnd]
	call SendDlgItemMessageA

dontsetlog:

	push offset OpLogPath
	push 0
	push WM_SETTEXT
	push IDC_LOGFILE
	push [hwnd]
	call SendDlgItemMessageA

	jmp op

opclose:

	push 0
	push [hwnd]
	call EndDialog

	jmp op

opcommand:

	mov eax, wparam
	cmp ax, IDC_OK
	je opok

	jmp opnot

opok:

	push 0
	push 0
	push BM_GETCHECK
	push IDC_ANON
	push [hwnd]
	call SendDlgItemMessageA

	mov [OpAllowAnon], al

	push 0
	push 0
	push BM_GETCHECK
	push IDC_LOG
	push [hwnd]
	call SendDlgItemMessageA

	mov [OpLogtoFile], al

	push offset OpLogPath
	push 100h
	push WM_GETTEXT
	push IDC_LOGFILE
	push [hwnd]
	call SendDlgItemMessageA

	jmp opclose

DlgOptionsProc ENDP


DlgAboutProc PROC uses ebx edi esi, hwnd:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD

	cmp wmsg, WM_CLOSE
	je abclose
	cmp wmsg, WM_COMMAND
	je abcommand

abnot:

	mov eax, 0

	ret

abclose:

	push 0
	push [hwnd]
	call EndDialog

	mov eax, 1

	ret

abcommand:

	mov eax, wparam
	cmp ax, IDC_OK
	je abclose

	jmp abnot

DlgAboutProc ENDP


DlgUsersProc PROC uses edi esi, hwnd:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD
	LOCAL ad:sockaddr
	LOCAL buforek:BYTE:13h

	cmp wmsg, WM_INITDIALOG
	je usinit
	cmp wmsg, WM_CLOSE
	je usclose
	cmp wmsg, WM_COMMAND
	je uscommand

usnot:

	mov eax, 0

	ret

usok:

	mov eax, 1

	ret

usinit:

	push 0
	push 0
	push LB_RESETCONTENT
	push IDC_ULIST
	push [hwnd]
	call SendDlgItemMessageA

	mov edi, offset LoggedUsers
	mov esi, 0
	sub edi, 16

usnext:

	cmp esi, 20
	je usok

	add edi, 16
	inc esi

	cmp dword ptr [edi], 0
	je usnext

	lea ebx, ad

	push offset len
	push ebx
	push dword ptr [edi]
	call getpeername

	mov eax, dword ptr [ad+4]

	push eax
	call inet_ntoa

	lea ebx, buforek

	push eax
	push ebx
	call lstrcpy

	push offset UsFrom
	push ebx
	call lstrcat

	add edi, 4

	push edi
	push ebx
	call lstrcat

	push ebx
	push 0
	push LB_ADDSTRING
	push IDC_ULIST
	push [hwnd]
	call SendDlgItemMessageA

	sub edi, 4

	jmp usnext

usclose:

	push 0
	push [hwnd]
	call EndDialog

	jmp usok

uscommand:

	mov eax, wparam
	cmp ax, IDC_OK
	je usclose
	cmp ax, IDC_REFRESH
	je usinit

	jmp usnot

DlgUsersProc ENDP


DlgAccountsProc PROC uses ebx edi esi, hwnd:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD
	LOCAL Bla:DWORD
	LOCAL RegAcc:BYTE:0Dh

	cmp wmsg, WM_INITDIALOG
	je acinit
	cmp wmsg, WM_CLOSE
	je acclose
	cmp wmsg, WM_COMMAND
	je accommand

acnot:

	mov eax, 0

	ret

acok:

	mov eax, 1

	ret

acinit:

	push 0
	push 0
	push LB_RESETCONTENT
	push IDC_ALIST
	push [hwnd]
	call SendDlgItemMessageA

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

	mov edi, 0
	lea esi, RegAcc

acnext:

	mov [Bla], 0Dh
	lea edx, Bla

	push 0
	push 0
	push 0
	push 0
	push edx
	push esi
	push edi
	push [KeyHWnd]
	call RegEnumValueA

	cmp eax, 0
	jne acok

	push esi
	push 0
	push LB_ADDSTRING
	push IDC_ALIST
	push [hwnd]
	call SendDlgItemMessageA

	inc edi

	jmp acnext

acclose:

	push 0
	push [hwnd]
	call EndDialog

	jmp acok

acnew:

	push 0
	push offset DlgEditProc
	push [hwnd]
	push offset DlgEdit
	push [AppHWnd]
	call DialogBoxParamA

	jmp acinit

acdelete:

	push 0
	push 0
	push LB_GETCURSEL
	push IDC_ALIST
	push [hwnd]
	call SendDlgItemMessageA

	mov edi, eax
	lea esi, RegAcc

	push esi
	push eax
	push LB_GETTEXT
	push IDC_ALIST
	push [hwnd]
	call SendDlgItemMessageA

	push esi
	push [KeyHWnd]
	call RegDeleteValueA

	push 0
	push edi
	push LB_DELETESTRING
	push IDC_ALIST
	push [hwnd]
	call SendDlgItemMessageA

	jmp acok

acedit:

	push 0
	push 0
	push LB_GETCURSEL
	push IDC_ALIST
	push [hwnd]
	call SendDlgItemMessageA

	cmp eax, -1
	je edok

	lea esi, Acco

	push esi
	push eax
	push LB_GETTEXT
	push IDC_ALIST
	push [hwnd]
	call SendDlgItemMessageA

	push esi
	push offset DlgEditProc
	push [hwnd]
	push offset DlgEdit
	push [AppHWnd]
	call DialogBoxParamA

	jmp acinit

accommand:

	mov eax, wparam
	cmp ax, IDC_NEW
	je acnew
	cmp ax, IDC_DELETE
	je acdelete
	cmp ax, IDC_OK
	je acclose
	cmp ax, IDC_EDIT
	je acedit

	jmp acnot

DlgAccountsProc ENDP


DlgEditProc PROC uses ebx edi esi, hwnd:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD
	LOCAL Account:BYTE:0Dh
	LOCAL Passwd:BYTE:11h
	LOCAL RegAccount:BYTE:0Dh
	LOCAL Blah:DWORD

	cmp wmsg, WM_INITDIALOG
	je edinitdialog
	cmp wmsg, WM_CLOSE
	je edclose
	cmp wmsg, WM_COMMAND
	je edcommand

ednot:

	mov eax, 0

	ret

edok:

	mov eax, 1

	ret

edinitdialog:

	push 0
	push 12
	push EM_SETLIMITTEXT
	push IDC_ANAME
	push [hwnd]
	call SendDlgItemMessageA

	push 0
	push 16
	push EM_SETLIMITTEXT
	push IDC_APASS
	push [hwnd]
	call SendDlgItemMessageA

	cmp [lparam], 0
	jne edinitnames

	jmp edok

edinitnames:

	mov esi, [lparam]

	push esi
	push 0
	push WM_SETTEXT
	push IDC_ANAME
	push [hwnd]
	call SendDlgItemMessageA

	lea edi, Passwd
	mov [Blah], 11h
	lea edx, Blah

	push edx
	push edi
	push 0
	push 0
	push esi
	push [KeyHWnd]
	call RegQueryValueExA

	xor edx, edx
	lea esi, Passwd
	mov edi, esi

	add esi, [Blah]
	dec esi

	mov dl, byte ptr [edi]
	xor byte ptr [esi], dl

edXoring1:

	dec esi

	mov dl, byte ptr [esi+1]
	xor byte ptr [esi], dl

	cmp esi, edi
	je edXorEnd

	jmp edXoring1

edXorEnd:

	add esi, [Blah]
	mov byte ptr [esi], 0

	push edi
	push 0
	push WM_SETTEXT
	push IDC_APASS
	push [hwnd]
	call SendDlgItemMessageA

	call WSAGetLastError

	lea esi, Acco

	push lparam
	push esi
	call lstrcpy

	jmp edok

edclose:

	push 0
	push [hwnd]
	call EndDialog

	jmp edok

edcheck:

	lea esi, Account

	push esi
	push 0Dh
	push WM_GETTEXT
	push IDC_ANAME
	push [hwnd]
	call SendDlgItemMessageA

	cmp byte ptr [esi], 0
	jne edcheckpass

	push MB_ICONSTOP
	push offset EdNameErr
	push offset EdNameText
	push [hwnd]
	call MessageBoxA

	jmp edok

edcheckpass:

	lea esi, Passwd

	push esi
	push 11h
	push WM_GETTEXT
	push IDC_APASS
	push [hwnd]
	call SendDlgItemMessageA

	cmp byte ptr [esi], 0
	jne edpro

	push MB_ICONSTOP
	push offset EdPassErr
	push offset EdPassText
	push [hwnd]
	call MessageBoxA

	jmp edok

edpro:

	cmp eax, 1
	jg edsave

	push MB_ICONSTOP
	push offset EdPassErr2
	push offset EdPassText2
	push [hwnd]
	call MessageBoxA

	jmp edok

edsave:

	call WSAGetLastError

	mov edi, 0

	lea edx, Acco

	cmp dword ptr [edx], 0
	je edcheckuser

	push edx
	push [KeyHWnd]
	call RegDeleteValueA

edcheckuser:

	mov [Blah], 0Dh
	lea edx, Blah
	lea ebx, RegAccount

	push 0
	push 0
	push 0
	push 0
	push edx
	push ebx
	push edi
	push [KeyHWnd]
	call RegEnumValueA

	cmp eax, 0
	jne edendcheck

	lea ebx, RegAccount
	lea esi, Account

	push ebx
	push esi
	call lstrcmpi

	cmp eax, 0
	je edequal

	inc edi

	jmp edcheckuser

edendcheck:

	xor edx, edx
	lea esi, Passwd
	mov edi, esi

edXoring:

	mov dl, byte ptr [esi+1]
	cmp dl,0
	je edXorLast

	xor byte ptr [esi], dl
	inc esi

	jmp edXoring

edXorLast:

	mov dl, byte ptr [edi]
	xor byte ptr [esi], dl

	lea edx, Account

	sub esi, edi
	inc esi

	push esi
	push edi
	push REG_BINARY
	push 0
	push edx
	push [KeyHWnd]
	call RegSetValueExA

	jmp edclose

edequal:

	push MB_ICONSTOP
	push offset EdUserErr
	push offset EdUserText
	push [hwnd]
	call MessageBoxA

	jmp edok

edcommand:

	mov eax, wparam
	cmp ax, IDC_OK
	je edcheck

	jmp ednot

DlgEditProc ENDP

