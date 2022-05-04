;
; FTP Server ver 0.2
;
; written by caspar /CookieCrK/ in win32asm
;
; caspar@polishscene.dhs.org || caspar@ae.pl
; http://www.cookiecrk.org
;

.const


WM_SOCKET	equ WM_USER + 100


IDC_LOGFILE	equ 1000
IDC_ANAME	equ 1001
IDC_APASS	equ 1002
IDC_ANON	equ 2000
IDC_LOG		equ 2001
IDC_OK		equ 3000
IDC_REFRESH	equ 3001
IDC_EDIT	equ 3002
IDC_NEW		equ 3003
IDC_DELETE	equ 3004
IDC_LIST	equ 4000
IDC_ULIST	equ 4001
IDC_ALIST	equ 4002

IDM_ONLINE	equ 32000
IDM_USERS	equ 32001
IDM_CLEAR	equ 32002
IDM_EXIT	equ 32003
IDM_ACCOUNTS	equ 32004
IDM_OPTIONS	equ 32005
IDM_ABOUT	equ 32006


