;
; FTP Server ver 0.2
;
; written by caspar /CookieCrK/ in win32asm
;
; caspar@polishscene.dhs.org || caspar@ae.pl
; http://www.cookiecrk.org
;
;
; Prosty serverek FTP. Ma troche bugow, ale nie mialem czasu ich poprawic.
; Nie ma zaimplementowanego passiva i paru innych waznych funkcji. Postaram
; sie to dorzucic w nextych versjach. Nie ma tez uprawnien do plikow. Kazdy
; user (anonymous tez - jesli jest wlaczony) ma pelen dostep do wszystkich
; plikow (rwxrwxrwx). To jest dosc powazna wada, ale pracuje juz nad tym :)
; Co by tu jeszcze... liczba (l)userow ograniczona do 20... Tak w ogole
; moznaby to napisac troche lepiej, ale ciagle mi brakuje czasu :(
; W zrodelku nie ma komentarzy... tak jakos wyszlo 8-]
; No to enjoy...  ^D logout


.386
locals
jumps
.model flat,STDCALL


include windows.inc
include strings.inc
include consts.inc
include client.inc
include command.inc
include dlgproc.inc


extrn GetModuleHandleA:Proc
extrn DialogBoxParamA:Proc
extrn EndDialog:Proc
extrn ExitProcess:Proc
extrn CreateStatusWindow:Proc
extrn GetMenu:Proc
extrn CheckMenuItem:Proc
extrn SendMessageA:Proc
extrn CreateThread:Proc
extrn GlobalAlloc:Proc
extrn CloseHandle:Proc
extrn SendDlgItemMessageA:Proc
extrn _wsprintfA:Proc
extrn GetLocalTime:Proc
extrn FindFirstFileA:Proc
extrn FindNextFileA:Proc
extrn FindClose:Proc
extrn GetLogicalDriveStringsA:Proc
extrn GetDriveTypeA:Proc
extrn SetCurrentDirectoryA:Proc
extrn CreateFileA:Proc
extrn GetFileSize:Proc
extrn SetFilePointer:Proc
extrn ReadFile:Proc
extrn WriteFile:Proc
extrn DeleteFileA:Proc
extrn RemoveDirectoryA:Proc
extrn CreateDirectoryA:Proc
extrn MessageBoxA:Proc

extrn WSAStartup:Proc
extrn socket:Proc
extrn WSAAsyncSelect:Proc
extrn htons:Proc
extrn bind:Proc
extrn listen:Proc
extrn accept:Proc
extrn closesocket:Proc
extrn WSACleanup:Proc
extrn send:Proc
extrn select:Proc
extrn recv:Proc
extrn inet_ntoa:Proc
extrn ioctlsocket:Proc
extrn htons:Proc
extrn inet_addr:Proc
extrn connect:Proc
extrn getpeername:Proc
extrn WSASendDisconnect:Proc

extrn WSAGetLastError:Proc

extrn lstrcpy:Proc
extrn lstrcat:Proc
extrn lstrcmpi:Proc
extrn lstrcmp:Proc
extrn lstrlen:Proc

extrn RegOpenKeyExA:Proc
extrn RegQueryValueExA:Proc
extrn RegCloseKey:Proc
extrn RegCreateKeyExA:Proc
extrn RegSetValueExA:Proc
extrn RegEnumValueA:Proc
extrn RegDeleteValueA:Proc


.data


AppHWnd dd ?

DlgHWnd dd ?
MenuHWnd dd ?
StatusHWnd dd ?

sock dd ?
csock dd ?


lpWSADATA WSAdata <0>
sin sockaddr_in <0>
addr _sockaddr <0>
ipaddr dd ?
len dd 10h
iocom dd 1
bladdr sockaddr <0>

StatusParts dd 80, 200, 300


Com db 100h dup(0)

Thread dd ?


RegKeyHWnd dd ?
KeyHWnd dd ?
RegOptn dd ?
RegOptnSize dd 4


Usr db 11 dup(0)
Users dd 0
Online db 0

LoggedUsers db 16*20 dup(0)

Acco db 13 dup(0)


OpAllowAnon db 0
OpLogtoFile db 0
OpLogPath db 100h dup(0)
OpLogPathSize dd 100h
OpLogWrote dd ?
OpLogEnter db 13,10,0


.code

start:

	push 0h
	call GetModuleHandleA
	mov [AppHWnd],eax

	push offset RegKeyHWnd
	push KEY_QUERY_VALUE
	push 0
	push offset RegKey
	push HKEY_CURRENT_USER
	call RegOpenKeyExA

	cmp eax, 0
	jne dontread

	push offset RegOptnSize
	push offset RegOptn
	push 0
	push 0
	push offset RegFlags
	push [RegKeyHWnd]
	call RegQueryValueExA

	cmp eax, 0
	jne dontread

	mov eax, [RegOptn]
	mov ebx, eax

	and eax, 1

	cmp eax, 1
	jne readnext

	mov [OpAllowAnon], 1

readnext:

	and ebx, 2

	cmp ebx, 2
	jne dontread

	mov [OpLogtoFile], 1

dontread:

	push offset OpLogPathSize
	push offset OpLogPath
	push 0
	push 0
	push offset RegLogFile
	push [RegKeyHWnd]
	call RegQueryValueExA

	cmp eax, 0
	je endread

	push offset OpLogFileDef
	push offset OpLogPath
	call lstrcpy

endread:

	push [RegKeyHWnd]
	call RegCloseKey

	push 0
	push offset DlgProc
	push 0
	push offset DlgName
	push [AppHWnd]
	call DialogBoxParamA

	push eax
	call ExitProcess


DlgProc PROC uses ebx edi esi, hwnd:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD

	cmp wmsg, WM_INITDIALOG
	je wminitdialog
	cmp wmsg, WM_CLOSE
	je wmclose
	cmp wmsg, WM_COMMAND
	je wmcommand
	cmp wmsg, WM_SOCKET
	je wmsocket

NotDone:

	mov eax, 0

	ret

Done:

	mov eax, 1

	ret

wminitdialog:

	mov eax, [hwnd]
	mov [DlgHWnd], eax

	push 2500
	push [hwnd]
	push 0
	push WS_VISIBLE or WS_CHILD
	call CreateStatusWindow

	mov [StatusHWnd], eax

	push offset StatusParts
	push 3
	push SB_SETPARTS
	push eax
	call SendMessageA

	call WriteStatus

	push [hwnd]
	call GetMenu

	mov [MenuHWnd], eax

	jmp done

wmclose:

	mov al, [Online]
	cmp al, 0
	je isoff

	call Disconnect

	call WriteStatus

	push 1
	push offset LogDown
	call WriteLog

isoff:

	push 0
	push offset RegKeyHWnd
	push 0
	push KEY_ALL_ACCESS
	push 0
	push 0
	push 0
	push offset RegKey
	push HKEY_CURRENT_USER
	call RegCreateKeyExA

	xor eax, eax
	mov al, [OpAllowAnon]

	cmp [OpLogtoFile], 1
	jne setval

	or eax, 2

setval:

	mov [RegOptn], eax
	mov [RegOptnSize], 4

	push RegOptnSize
	push offset RegOptn
	push REG_DWORD
	push 0
	push offset RegFlags
	push [RegKeyHWnd]
	call RegSetValueExA

	push offset OpLogPath
	call lstrlen

	push eax
	push offset OpLogPath
	push REG_SZ
	push 0
	push offset RegLogFile
	push [RegKeyHWnd]
	call RegSetValueExA

	push [RegKeyHWnd]
	call RegCloseKey

	push 0
	push [hwnd]
	call EndDialog

	jmp Done

wmcommand:

	mov eax, wparam
	mov ebx, lparam
	cmp ebx, 0
	je cm_menu

	jmp NotDone

cm_menu:

	cmp ax, IDM_ONLINE
	je go_online
	cmp ax, IDM_USERS
	je m_users
	cmp ax, IDM_CLEAR
	je clear
	cmp ax, IDM_EXIT
	je wmclose
	cmp ax, IDM_ACCOUNTS
	je accounts
	cmp ax, IDM_OPTIONS
	je opti
	cmp ax, IDM_ABOUT
	je about

	jmp NotDone

wmsocket:

	mov eax, lparam
	cmp eax, FD_ACCEPT
	je acc

	jmp NotDone

acc:

	push offset len
	push offset addr
	push [sock]
	call accept

	mov [csock], eax

	mov eax, dword ptr [addr+4]
	mov [ipaddr], eax

	mov eax, offset LoggedUsers

	sub eax, 16
	mov ecx, 0

checknext:

	cmp ecx, 20
	je ToomanyCon

	inc ecx

	add eax, 16

	cmp dword ptr [eax], 0
	jne checknext

	mov ecx, [csock]
	mov dword ptr [eax], ecx

	push offset Thread
	push 0
	push eax
	push offset ClientProc
	push 0
	push 0
	call CreateThread

	cmp eax, 0
	jne CloseThHWnd

ToomanyCon:

	push 0
	push 30
	push offset TooManyUsr
	push [csock]
	call send

	push [csock]
	call closesocket

	jmp Done

CloseThHWnd:

	push eax
	call CloseHandle

	push [ipaddr]
	call inet_ntoa

	mov [ipaddr], eax

	mov edi, offset Com

	push offset LogConnect
	push edi
	call lstrcpy

	push [ipaddr]
	push edi
	call lstrcat

	push 1
	push eax
	call WriteLog

	jmp Done

go_online:

	mov al, [Online]
	cmp al, 0
	je is_off

	push MF_UNCHECKED
	push IDM_ONLINE
	push [MenuHWnd]
	call CheckMenuItem

	mov [Online], 0

	call Disconnect

	call WriteStatus

	push 1
	push offset LogDown
	call WriteLog

	jmp Done

is_off:

	push MF_CHECKED
	push IDM_ONLINE
	push [MenuHWnd]
	call CheckMenuItem

	mov [Online], 1

	call WriteStatus

	push 0
	push offset LogStart
	call WriteLog

	push 0
	push offset LogStartcd
	call WriteLog

	cmp [OpLogtoFile], 1
	jne logtoscr

	push offset LogBoth
	push offset Com
	call lstrcpy

	push offset OpLogPath
	push offset Com
	call lstrcat

	push 0
	push offset Com
	call WriteLog

	jmp afterlog

logtoscr:

	push 0
	push offset LogScreen
	call WriteLog

afterlog:

	push [hwnd]
	call Init

	cmp eax, 0
	je unknowner
	jl portused

	push 1
	push offset LogWait
	call WriteLog

	jmp Done

unknowner:

	push 0
	push offset LogUnknown
	call WriteLog

	jmp Done

portused:

	push 0
	push offset LogNotWait
	call WriteLog

	jmp go_online

m_users:

	push 0
	push offset DlgUsersProc
	push [hwnd]
	push offset DlgUsers
	push [AppHWnd]
	call DialogBoxParamA

	jmp Done

clear:

	push 0
	push 0
	push LB_RESETCONTENT
	push IDC_LIST
	push [hwnd]
	call SendDlgItemMessageA

	jmp Done

accounts:

	push 0
	push offset DlgAccountsProc
	push [hwnd]
	push offset DlgAccounts
	push [AppHWnd]
	call DialogBoxParamA

	jmp Done

opti:

	push 0
	push offset DlgOptionsProc
	push [hwnd]
	push offset DlgOptions
	push [AppHWnd]
	call DialogBoxParamA

	jmp Done

about:

	push 0
	push offset DlgAboutProc
	push [hwnd]
	push offset DlgAbout
	push [AppHWnd]
	call DialogBoxParamA

	jmp Done

DlgProc ENDP


;********************************************************************************************
;					PROCEDURES 
;********************************************************************************************

;********************************************************************************************
;					Init
;********************************************************************************************


Init PROC , hwnd: DWORD


	push offset lpWSADATA
	push 101h
	call WSAStartup

	push 0
	push SOCK_STREAM
	push PF_INET
	call socket

	mov [sock], eax

	push FD_ACCEPT
	push WM_SOCKET
	push [hwnd]
	push [sock]
	call WSAAsyncSelect

	mov sin.sin_family,PF_INET

	push 21
	call htons

	mov sin.sin_port,ax
	mov sin.sin_addr,INADDR_ANY

	push 16
	push offset sin
	push [sock]
	call bind

	cmp eax, -1
	je bindnok

	push 20
	push [sock]
	call listen

	mov eax, 1

	ret

bindnok:

	call WSAGetLastError

	cmp eax, 10048
	jne unkerr

	mov eax, -1

	ret

unkerr:

	mov eax, 0

	ret

Init ENDP


;********************************************************************************************
;					Disconnect
;********************************************************************************************


Disconnect PROC uses edi esi ebx edx

	push [sock]
	call closesocket

	mov edi, offset LoggedUsers
	mov esi, 0
	sub edi, 16

disnext:

	cmp esi, 20
	je disok

	add edi, 16
	inc esi

	cmp dword ptr [edi], 0
	je disnext

	mov edx, [edi]

	mov dword ptr [edi], 0

	push edx

	lea ebx, bladdr

	push offset len
	push ebx
	push edx
	call getpeername

	mov eax, dword ptr [bladdr+4]

	push eax
	call inet_ntoa

	push eax

	lea ebx, Com

	push offset LogDisconnect
	push ebx
	call lstrcpy

	lea ebx, Com

	push ebx
	call lstrcat

	push 1
	push eax
	call WriteLog

	call closesocket

	jmp disnext

disok:

	call WSACleanup

	ret

Disconnect ENDP


;********************************************************************************************
;					Write Status
;********************************************************************************************


WriteStatus PROC uses edi
	LOCAL buf:BYTE:100h

	lea edi, buf

	push offset StatusServ
	push edi
	call lstrcpy

	cmp [Online], 0
	je NotOnline

	push offset StatusOnline
	push edi
	call lstrcat

	jmp EWriteStat

NotOnline:

	push offset StatusOffline
	push edi
	call lstrcat

EWriteStat:

	push edi
	push 0
	push SB_SETTEXTA
	push [StatusHWnd]
	call SendMessageA

	mov eax, [Users]

	push eax
	push offset H2D
	push offset Usr
	call _wsprintfA

	add esp, 12

	lea edi, buf

	push offset StatusUsers
	push edi
	call lstrcpy

	push offset Usr
	push edi
	call lstrcat

	push edi
	push 1
	push SB_SETTEXTA
	push [StatusHWnd]
	call SendMessageA

	ret

WriteStatus ENDP


;********************************************************************************************
;					Write Log
;********************************************************************************************


WriteLog PROC uses edi esi ecx, offlog:DWORD, date:DWORD
	LOCAL buf:BYTE:100h
	LOCAL SysTime:SYSTEMTIME

	cmp [date], 0
	je WithoutDate

	lea edi, SysTime

	push edi
	call GetLocalTime

	add edi, 6
	lea esi, buf
	xor ecx, ecx
	mov byte ptr [esi], '['
	inc esi

GetTime:

	add edi, 2

	inc ecx
	push ecx

	xor eax, eax
	mov ax, word ptr [edi]

	push eax
	push offset uH2D
	push esi
	call _wsprintfA

	add esp, 12

	pop ecx

	cmp ecx, 3
	je PrintTime

	push esi
	call lstrlen

	add esi, eax
	mov byte ptr [esi], ':'
	inc esi

	jmp GetTime

PrintTime:

	push esi
	call lstrlen

	add esi, eax
	mov word ptr [esi], ' ]'
	mov dword ptr [esi+2], 0

	lea esi, buf

	push [offlog]
	push esi
	call lstrcat

	mov [offlog], eax

	jmp AfterWithout

WithoutDate:

	lea esi, buf

	push [offlog]
	push esi
	call lstrcpy

	mov [offlog], eax

AfterWithout:

	push [offlog]
	push 0
	push LB_ADDSTRING
	push IDC_LIST
	push [DlgHWnd]
	call SendDlgItemMessageA

	cmp OpLogtoFile, 0
	je NotToFile

	push 0
	push FILE_ATTRIBUTE_ARCHIVE
	push OPEN_ALWAYS
	push 0
	push FILE_SHARE_WRITE
	push GENERIC_WRITE
	push offset OpLogPath
	call CreateFileA

	mov edi, eax

	push FILE_END
	push 0
	push 0
	push eax
	call SetFilePointer

	push offset OpLogEnter
	push [offlog]
	call lstrcat

	push [offlog]
	call lstrlen

	push 0
	push offset OpLogWrote
	push eax
	push [offlog]
	push edi
	call WriteFile

	push edi
	call CloseHandle

NotToFile:

	ret

WriteLog ENDP


end start

