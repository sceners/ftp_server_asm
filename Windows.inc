MAX_PATH			equ 260
FILE_ATTRIBUTE_DIRECTORY 	equ 10h
DRIVE_FIXED 			equ 3
FILE_ATTRIBUTE_ARCHIVE		equ 20h

HKEY_CURRENT_USER		equ 80000001h

REG_SZ equ 1
REG_DWORD equ 4
REG_BINARY equ 3

KEY_QUERY_VALUE equ 1
KEY_SET_VALUE equ 2
KEY_CREATE_SUB_KEY equ 4
KEY_ENUMERATE_SUB_KEYS equ 8
KEY_NOTIFY equ 10h
KEY_CREATE_LINK equ 20h
KEY_READ equ KEY_QUERY_VALUE OR KEY_ENUMERATE_SUB_KEYS OR KEY_NOTIFY
KEY_WRITE equ KEY_SET_VALUE OR KEY_CREATE_SUB_KEY
KEY_EXECUTE equ KEY_READ
KEY_ALL_ACCESS equ KEY_QUERY_VALUE OR KEY_SET_VALUE OR KEY_CREATE_SUB_KEY OR KEY_ENUMERATE_SUB_KEYS OR KEY_NOTIFY OR KEY_CREATE_LINK


GENERIC_WRITE			equ 40000000h
GENERIC_READ			equ 80000000h
OPEN_EXISTING			equ 3
OPEN_ALWAYS			equ 4
CREATE_ALWAYS			equ 2
FILE_SHARE_WRITE		equ 2
FILE_END			equ 2


FILETIME STRUCT
 dwLowDateTime dd ?
 dwHighDateTime dd ?
FILETIME ENDS

SYSTEMTIME STRUCT
 wYear WORD ?
 wMonth WORD ?
 wDayOfWeek WORD ?
 wDay WORD ?
 wHour WORD ?
 wMinute WORD ?
 wSecond WORD ?
 wMilliseconds WORD ?
SYSTEMTIME ENDS

WIN32_FIND_DATA STRUCT
 dwFileAttributes dd ?
 ftCreationTime FILETIME <>
 ftLastAccessTime FILETIME <>
 ftLastWriteTime FILETIME <>
 nFileSizeHigh dd ?
 nFileSizeLow dd ?
 dwReserved0 dd ?
 dwReserved1 dd ?
 cFileName BYTE MAX_PATH dup(?)
 cAlternate BYTE 14 dup(?)
WIN32_FIND_DATA ENDS

SECURITY_ATTRIBUTES STRUCT
  niLength  DWORD      ?
  lpSecurityDescriptor  DWORD      ?
  bInheritHandle  DWORD      ?
SECURITY_ATTRIBUTES ENDS

PROCESSENTRY32 STRUCT
 dwSize dd ?
 cntUsage dd ?
 th32ProcessID dd ?
 th32DefaultHeapID dd ?
 th32ModuleID dd ?
 cntThreads dd ?
 th32ParentProcessID dd ?
 pcPriClassBase dd ?
 dwFlags dd ?
 szExeFile db MAX_PATH dup(?)
PROCESSENTRY32 ENDS



; ===============
; window messages
; ===============
;
WM_NULL				equ	0000h
WM_CREATE			equ	0001h
WM_DESTROY			equ	0002h
WM_MOVE				equ	0003h
WM_SIZE				equ	0005h
WM_ACTIVATE			equ	0006h
WM_SETFOCUS			equ	0007h
WM_KILLFOCUS			equ	0008h
WM_ENABLE			equ	000Ah
WM_SETREDRAW			equ	000Bh
WM_SETTEXT			equ	000Ch
WM_GETTEXT			equ	000Dh
WM_GETTEXTLENGTH		equ	000Eh
WM_PAINT			equ	000Fh
WM_CLOSE			equ	0010h
WM_QUERYENDSESSION		equ	0011h
WM_QUIT				equ	0012h
WM_QUERYOPEN			equ	0013h
WM_ERASEBKGND			equ	0014h
WM_SYSCOLORCHANGE		equ	0015h
WM_ENDSESSION			equ	0016h
WM_SHOWWINDOW			equ	0018h
WM_WININICHANGE			equ	001Ah
WM_SETTINGCHANGE		equ	WM_WININICHANGE
WM_DEVMODECHANGE		equ	001Bh
WM_ACTIVATEAPP			equ	001Ch
WM_FONTCHANGE			equ	001Dh
WM_TIMECHANGE			equ	001Eh
WM_CANCELMODE			equ	001Fh
WM_SETCURSOR			equ	0020h
WM_MOUSEACTIVATE		equ	0021h
WM_CHILDACTIVATE		equ	0022h
WM_QUEUESYNC			equ	0023h
WM_GETMINMAXINFO		equ	0024h
WM_PAINTICON			equ	0026h
WM_ICONERASEBKGND		equ	0027h
WM_NEXTDLGCTL			equ	0028h
WM_SPOOLERSTATUS		equ	002Ah
WM_DRAWITEM			equ	002Bh
WM_MEASUREITEM			equ	002Ch
WM_DELETEITEM			equ	002Dh
WM_VKEYTOITEM			equ	002Eh
WM_CHARTOITEM			equ	002Fh
WM_SETFONT			equ	0030h
WM_GETFONT			equ	0031h
WM_SETHOTKEY			equ	0032h
WM_GETHOTKEY			equ	0033h
WM_QUERYDRAGICON		equ	0037h
WM_COMPAREITEM			equ	0039h
WM_COMPACTING			equ	0041h
WM_COMMNOTIFY			equ	0044h		;no longer suported
WM_WINDOWPOSCHANGING		equ	0046h
WM_WINDOWPOSCHANGED		equ	0047h
WM_POWER			equ	0048h
WM_COPYDATA			equ	004Ah
WM_CANCELJOURNAL		equ	004Bh
WM_NOTIFY			equ	004Eh
WM_INPUTLANGCHANGEREQUEST	equ	0050h
WM_INPUTLANGCHANGE		equ	0051h
WM_TCARD			equ	0052h
WM_HELP				equ	0053h
WM_USERCHANGED			equ	0054h
WM_NOTIFYFORMAT			equ	0055h
WM_CONTEXTMENU			equ	007Bh
WM_STYLECHANGING		equ	007Ch
WM_STYLECHANGED			equ	007Dh
WM_DISPLAYCHANGE		equ	007Eh
WM_GETICON			equ	007Fh
WM_SETICON			equ	0080h
WM_NCCREATE			equ	0081h
WM_NCDESTROY			equ	0082h
WM_NCCALCSIZE			equ	0083h
WM_NCHITTEST			equ	0084h
WM_NCPAINT			equ	0085h
WM_NCACTIVATE			equ	0086h
WM_GETDLGCODE			equ	0087h
;
WM_NCMOUSEMOVE			equ	00A0h
WM_NCLBUTTONDOWN		equ	00A1h
WM_NCLBUTTONUP			equ	00A2h
WM_NCLBUTTONDBLCLK		equ	00A3h
WM_NCRBUTTONDOWN		equ	00A4h
WM_NCRBUTTONUP			equ	00A5h
WM_NCRBUTTONDBLCLK		equ	00A6h
WM_NCMBUTTONDOWN		equ	00A7h
WM_NCMBUTTONUP			equ	00A8h
WM_NCMBUTTONDBLCLK		equ	00A9h
;
WM_KEYFIRST			equ	0100h
WM_KEYDOWN			equ	0100h
WM_KEYUP			equ	0101h
WM_CHAR				equ	0102h
WM_DEADCHAR			equ	0103h
WM_SYSKEYDOWN			equ	0104h
WM_SYSKEYUP			equ	0105h
WM_SYSCHAR			equ	0106h
WM_SYSDEADCHAR			equ	0107h
WM_KEYLAST			equ	0108h
;
WM_IME_STARTCOMPOSITION		equ	010Dh
WM_IME_ENDCOMPOSITION		equ	010Eh
WM_IME_COMPOSITION		equ	010Fh
WM_IME_KEYLAST			equ	010Fh
WM_INITDIALOG			equ	0110h
WM_COMMAND			equ	0111h
WM_SYSCOMMAND			equ	0112h
WM_TIMER			equ	0113h
WM_HSCROLL			equ	0114h
WM_VSCROLL			equ	0115h
WM_INITMENU			equ	0116h
WM_INITMENUPOPUP		equ	0117h
WM_MENUSELECT			equ	011Fh
WM_MENUCHAR			equ	0120h
WM_ENTERIDLE			equ	0121h
WM_CTLCOLORMSGBOX		equ	0132h
WM_CTLCOLOREDIT			equ	0133h
WM_CTLCOLORLISTBOX		equ	0134h
WM_CTLCOLORBTN			equ	0135h
WM_CTLCOLORDLG			equ	0136h
WM_CTLCOLORSCROLLBAR		equ	0137h
WM_CTLCOLORSTATIC		equ	0138h
;
WM_MOUSEFIRST			equ	0200h
WM_MOUSEMOVE			equ	0200h
WM_LBUTTONDOWN			equ	0201h
WM_LBUTTONUP			equ	0202h
WM_LBUTTONDBLCLK		equ	0203h
WM_RBUTTONDOWN			equ	0204h
WM_RBUTTONUP			equ	0205h
WM_RBUTTONDBLCLK		equ	0206h
WM_MBUTTONDOWN			equ	0207h
WM_MBUTTONUP			equ	0208h
WM_MBUTTONDBLCLK		equ	0209h
WM_MOUSELAST			equ	0209h
;
WM_PARENTNOTIFY			equ	0210h
WM_ENTERMENULOOP		equ	0211h
WM_EXITMENULOOP			equ	0212h
WM_NEXTMENU			equ	0213h
WM_SIZING			equ	0214h
WM_CAPTURECHANGED		equ	0215h
WM_MOVING			equ	0216h
WM_POWERBROADCAST		equ	0218h
WM_DEVICECHANGE			equ	0219h
WM_MDICREATE			equ	0220h
WM_MDIDESTROY			equ	0221h
WM_MDIACTIVATE			equ	0222h
WM_MDIRESTORE			equ	0223h
WM_MDINEXT			equ	0224h
WM_MDIMAXIMIZE			equ	0225h
WM_MDITILE			equ	0226h
WM_MDICASCADE			equ	0227h
WM_MDIICONARRANGE		equ	0228h
WM_MDIGETACTIVE			equ	0229h
WM_MDISETMENU			equ	0230h
WM_ENTERSIZEMOVE		equ	0231h
WM_EXITSIZEMOVE			equ	0232h
WM_DROPFILES			equ	0233h
WM_MDIREFRESHMENU		equ	0234h
;
WM_IME_SETCONTEXT		equ	0281h
WM_IME_NOTIFY			equ	0282h
WM_IME_CONTROL			equ	0283h
WM_IME_COMPOSITIONFULL		equ	0284h
WM_IME_SELECT			equ	0285h
WM_IME_CHAR			equ	0286h
WM_IME_KEYDOWN			equ	0290h
WM_IME_KEYUP			equ	0291h
;
WM_CUT				equ	0300h
WM_COPY				equ	0301h
WM_PASTE			equ	0302h
WM_CLEAR			equ	0303h
WM_UNDO				equ	0304h
WM_RENDERFORMAT			equ	0305h
WM_RENDERALLFORMATS		equ	0306h
WM_DESTROYCLIPBOARD		equ	0307h
WM_DRAWCLIPBOARD		equ	0308h
WM_PAINTCLIPBOARD		equ	0309h
WM_VSCROLLCLIPBOARD		equ	030Ah
WM_SIZECLIPBOARD		equ	030Bh
WM_ASKCBFORMATNAME		equ	030Ch
WM_CHANGECBCHAIN		equ	030Dh
WM_HSCROLLCLIPBOARD		equ	030Eh
WM_QUERYNEWPALETTE		equ	030Fh
WM_PALETTEISCHANGING		equ	0310h
WM_PALETTECHANGED		equ	0311h
WM_HOTKEY			equ	0312h
WM_PRINT			equ	0317h
WM_PRINTCLIENT			equ	0318h
;
WM_HANDHELDFIRST		equ	0358h
WM_HANDHELDLAST			equ	035Fh
;
WM_AFXFIRST			equ	0360h
WM_AFXLAST			equ	037Fh
;
WM_PENWINFIRST			equ	0380h
WM_PENWINLAST			equ	038Fh
;
WM_DDE_FIRST			equ	03E0h
WM_DDE_INITIATE			equ	WM_DDE_FIRST
WM_DDE_TERMINATE		equ	WM_DDE_FIRST+1
WM_DDE_ADVISE			equ	WM_DDE_FIRST+2
WM_DDE_UNADVISE			equ	WM_DDE_FIRST+3
WM_DDE_ACK			equ	WM_DDE_FIRST+4
WM_DDE_DATA			equ	WM_DDE_FIRST+5
WM_DDE_REQUEST			equ	WM_DDE_FIRST+6
WM_DDE_POKE			equ	WM_DDE_FIRST+7
WM_DDE_EXECUTE			equ	WM_DDE_FIRST+8
WM_DDE_LAST			equ	03E8h
;
WM_USER				equ	0400h
WM_APP				equ	8000h



WS_POPUP		EQU	080000000h
WS_CHILD		EQU	040000000h
WS_MINIMIZE		EQU	020000000h
WS_VISIBLE		EQU	010000000h
WS_MAXIMIZE		EQU	001000000h
WS_CAPTION		EQU	000C00000h
WS_BORDER		EQU	000800000h
WS_DLGFRAME		EQU	000400000h
WS_VSCROLL		EQU	000200000h
WS_HSCROLL		EQU	000100000h
WS_SYSMENU		EQU	000080000h
;WS_SIZEBOX		EQU	000040000h
WS_MINIMIZEBOX		EQU	000020000h
WS_MAXIMIZEBOX		EQU	000010000h
WS_OVERLAPPEDWINDOW	EQU	000CF0000h
WS_EX_NOPARENTNOTIFY	EQU	000000004h
WS_EX_WINDOWEDGE	EQU	000000100h
WS_EX_CLIENTEDGE	EQU	000000200h
WS_EX_OVERLAPPEDWINDOW	EQU	WS_EX_WINDOWEDGE + WS_EX_CLIENTEDGE


; ==========
; menu flags
; ==========
;
MF_INSERT			equ	00000000h
MF_CHANGE			equ	00000080h
MF_APPEND			equ	00000100h
MF_DELETE			equ	00000200h
MF_REMOVE			equ	00001000h
;
MF_BYCOMMAND			equ	00000000h
MF_BYPOSITION			equ	00000400h
;
MF_SEPARATOR			equ	00000800h
;
MF_ENABLED			equ	00000000h
MF_GRAYED			equ	00000001h
MF_DISABLED			equ	00000002h
;
MF_UNCHECKED			equ	00000000h
MF_CHECKED			equ	00000008h
MF_USECHECKBITMAPS		equ	00000200h
;
MF_STRING			equ	00000000h
MF_BITMAP			equ	00000004h
MF_OWNERDRAW			equ	00000100h
;
MF_POPUP			equ	00000010h
MF_MENUBARBREAK			equ	00000020h
MF_MENUBREAK			equ	00000040h
;
MF_UNHILITE			equ	00000000h
MF_HILITE			equ	00000080h
;
MF_DEFAULT			equ	00001000h
MF_SYSMENU			equ	00002000h
MF_HELP				equ	00004000h
MF_RIGHTJUSTIFY			equ	00004000h
;
MF_MOUSESELECT			equ	00008000h
MF_END				equ	00000080h	;obsolete


; ===================
; global memory flags
; ===================
;
GMEM_FIXED			equ	0000h
GMEM_MOVEABLE			equ	0002h
GMEM_NOCOMPACT			equ	0010h
GMEM_NODISCARD			equ	0020h
GMEM_ZEROINIT			equ	0040h
GMEM_MODIFY			equ	0080h
GMEM_DISCARDABLE		equ	0100h
GMEM_NOT_BANKED			equ	1000h
GMEM_SHARE			equ	2000h
GMEM_DDESHARE			equ	2000h
GMEM_NOTIFY			equ	4000h
GMEM_INVALID_HANDLE		equ	8000h
;
GMEM_VALID_FLAGS		equ	7F72h
GMEM_LOWER			equ	GMEM_NOT_BANKED


; ================
; listbox messages
; ================
;
LB_ADDSTRING			equ	0180h
LB_INSERTSTRING			equ	0181h
LB_DELETESTRING			equ	0182h
LB_SELITEMRANGEEX		equ	0183h
LB_RESETCONTENT			equ	0184h
LB_SETSEL			equ	0185h
LB_SETCURSEL			equ	0186h
LB_GETSEL			equ	0187h
LB_GETCURSEL			equ	0188h
LB_GETTEXT			equ	0189h
LB_GETTEXTLEN			equ	018Ah
LB_GETCOUNT			equ	018Bh
LB_SELECTSTRING			equ	018Ch
LB_DIR				equ	018Dh
LB_GETTOPINDEX			equ	018Eh
LB_FINDSTRING			equ	018Fh
LB_GETSELCOUNT			equ	0190h
LB_GETSELITEMS			equ	0191h
LB_SETTABSTOPS			equ	0192h
LB_GETHORIZONTALEXTENT		equ	0193h
LB_SETHORIZONTALEXTENT		equ	0194h
LB_SETCOLUMNWIDTH		equ	0195h
LB_ADDFILE			equ	0196h
LB_SETTOPINDEX			equ	0197h
LB_GETITEMRECT			equ	0198h
LB_GETITEMDATA			equ	0199h
LB_SETITEMDATA			equ	019Ah
LB_SELITEMRANGE			equ	019Bh
LB_SETANCHORINDEX		equ	019Ch
LB_GETANCHORINDEX		equ	019Dh
LB_SETCARETINDEX		equ	019Eh
LB_GETCARETINDEX		equ	019Fh
LB_SETITEMHEIGHT		equ	01A0h
LB_GETITEMHEIGHT		equ	01A1h
LB_FINDSTRINGEXACT		equ	01A2h
LB_SETLOCALE			equ	01A5h
LB_GETLOCALE			equ	01A6h
LB_SETCOUNT			equ	01A7h
LB_INITSTORAGE			equ	01A8h
LB_ITEMFROMPOINT		equ	01A9h
LB_MSGMAX			equ	01B0h
LBN_SELCHANGE			equ	1
LB_ERR				equ	-1

; ===================
; status bar messages
; ===================
;
SB_SETTEXTA			equ	WM_USER+01
SB_GETTEXTA			equ	WM_USER+02
SB_GETTEXTLENGTHA		equ	WM_USER+03
SB_SETPARTS			equ	WM_USER+04
SB_GETPARTS			equ	WM_USER+06
SB_GETBORDERS			equ	WM_USER+07
SB_SETMINHEIGHT			equ	WM_USER+08
SB_SIMPLE			equ	WM_USER+09
SB_GETRECT			equ	WM_USER+10
SB_SETTEXTW			equ	WM_USER+11
SB_GETTEXTLENGTHW		equ	WM_USER+12
SB_GETTEXTW			equ	WM_USER+13


BM_GETCHECK			equ	0F0h
BM_SETCHECK			equ	0F1h


EM_SETLIMITTEXT			equ	0C5h

MB_ICONSTOP			equ	10h


; -------------------------- wsock32 equates -------------------------------

WSADESCRIPTION_LEN          equ 256
WSASYS_STATUS_LEN           equ 128
IPPROTO_IP          equ             0
IPPROTO_ICMP          equ           1
IPPROTO_GGP          equ            2
IPPROTO_TCP          equ            6
IPPROTO_PUP          equ            12
IPPROTO_UDP          equ            17
IPPROTO_IDP          equ            22
IPPROTO_ND          equ             77
IPPROTO_RAW          equ            255
IPPROTO_MAX          equ            256
IOCPARM_MASK           equ 7Fh
IOC_VOID               equ 20000000h
IOC_OUT                equ 40000000h
IOC_IN equ 80000000h
IOC_INOUT              equ IOC_IN OR IOC_OUT
FIONBIO                equ 8004667Eh
FIONSYNC               equ 8004667Dh
FIONREAD               equ 4004667Fh
IPPORT_ECHO          equ            7
IPPORT_DISCARD          equ         9
IPPORT_SYSTAT          equ          11
IPPORT_DAYTIME          equ         13
IPPORT_NETSTAT          equ         15
IPPORT_FTP          equ             21
IPPORT_TELNET          equ          23
IPPORT_SMTP          equ            25
IPPORT_TIMESERVER          equ      37
IPPORT_NAMESERVER          equ      42
IPPORT_WHOIS          equ           43
IPPORT_MTP          equ             57
IPPORT_TFTP          equ            69
IPPORT_RJE          equ             77
IPPORT_FINGER          equ          79
IPPORT_TTYLINK          equ         87
IPPORT_SUPDUP          equ          95
IPPORT_EXECSERVER          equ      512
IPPORT_LOGINSERVER          equ     513
IPPORT_CMDSERVER          equ       514
IPPORT_EFSSERVER          equ       520
IPPORT_BIFFUDP          equ         512
IPPORT_WHOSERVER          equ       513
IPPORT_ROUTESERVER          equ     520
IPPORT_RESERVED          equ        1024
IMPLINK_IP          equ             155
IMPLINK_LOWEXPER          equ       156
IMPLINK_HIGHEXPER          equ      158
IN_CLASSA_NET          equ          0FF000000h
IN_CLASSA_NSHIFT          equ       24
IN_CLASSA_HOST          equ         000FFFFFFh
IN_CLASSA_MAX          equ          128
IN_CLASSB_NET          equ          0FFFF0000h
IN_CLASSB_NSHIFT          equ       16
IN_CLASSB_HOST          equ         00000FFFFh
IN_CLASSB_MAX          equ          65536
IN_CLASSC_NET          equ          0FFFFFF00h
IN_CLASSC_NSHIFT          equ       8
IN_CLASSC_HOST          equ         0000000FFh
INADDR_ANY          equ             000000000h
INADDR_LOOPBACK          equ        07F000001h
INADDR_BROADCAST          equ       0FFFFFFFFh
INADDR_NONE          equ            0FFFFFFFFh
SOCK_STREAM          equ    1
SOCK_DGRAM          equ     2
SOCK_RAW          equ       3
SOCK_RDM          equ       4
SOCK_SEQPACKET          equ 5
SO_DEBUG          equ       00001h
SO_ACCEPTCONN          equ  00002h
SO_REUSEADDR          equ   00004h
SO_KEEPALIVE          equ   00008h
SO_DONTROUTE          equ   00010h
SO_BROADCAST          equ   00020h
SO_USELOOPBACK          equ 00040h
SO_LINGER          equ      00080h
SO_OOBINLINE          equ   00100h
SOL_SOCKET            equ   0FFFFh
SO_DONTLINGER          equ  NOT SO_LINGER
SO_SNDBUF          equ      01001h
SO_RCVBUF          equ      01002h
SO_SNDLOWAT          equ    01003h
SO_RCVLOWAT          equ    01004h
SO_SNDTIMEO          equ    01005h
SO_RCVTIMEO          equ    01006h
SO_ERROR          equ       01007h
SO_TYPE          equ        01008h
TCP_NODELAY          equ    00001h
AF_UNSPEC          equ      0
AF_UNIX          equ        1
AF_INET          equ        2
AF_IMPLINK          equ     3
AF_PUP          equ         4
AF_CHAOS          equ       5
AF_NS          equ          6
AF_IPX          equ         6
AF_ISO          equ         7
AF_OSI          equ         AF_ISO
AF_ECMA          equ        8
AF_DATAKIT          equ     9
AF_CCITT          equ       10
AF_SNA          equ         11
AF_DECnet          equ      12
AF_DLI          equ         13
AF_LAT          equ         14
AF_HYLINK          equ      15
AF_APPLETALK          equ   16
AF_NETBIOS          equ     17
AF_MAX          equ         18
PF_UNSPEC          equ      AF_UNSPEC
PF_UNIX          equ        AF_UNIX
PF_INET          equ        AF_INET
PF_IMPLINK          equ     AF_IMPLINK
PF_PUP          equ         AF_PUP
PF_CHAOS          equ       AF_CHAOS
PF_NS          equ          AF_NS
PF_IPX          equ         AF_IPX
PF_ISO          equ         AF_ISO
PF_OSI          equ         AF_OSI
PF_ECMA          equ        AF_ECMA
PF_DATAKIT          equ     AF_DATAKIT
PF_CCITT          equ       AF_CCITT
PF_SNA          equ         AF_SNA
PF_DECnet          equ      AF_DECnet
PF_DLI          equ         AF_DLI
PF_LAT          equ         AF_LAT
PF_HYLINK          equ      AF_HYLINK
PF_APPLETALK          equ   AF_APPLETALK
PF_MAX          equ         AF_MAX
SOMAXCONN          equ      5
MSG_OOB          equ        01h
MSG_PEEK          equ   02h
MSG_DONTROUTE          equ  04h
MSG_MAXIOVLEN          equ  16
MAXGETHOSTSTRUCT          equ       1024
FD_READ          equ        001h
FD_WRITE          equ       002h
FD_OOB          equ         004h
FD_ACCEPT          equ      008h
FD_CONNECT          equ     010h
FD_CLOSE          equ       020h
WSABASEERR          equ             10000
WSAEINTR          equ               WSABASEERR+4
WSAEBADF          equ               WSABASEERR+9
WSAEACCES          equ              WSABASEERR+13
WSAEFAULT          equ              WSABASEERR+14
WSAEINVAL          equ              WSABASEERR+22
WSAEMFILE          equ              WSABASEERR+24
WSAEWOULDBLOCK          equ         WSABASEERR + 35
WSAEINPROGRESS          equ         WSABASEERR + 36
WSAEALREADY          equ            WSABASEERR + 37
WSAENOTSOCK          equ            WSABASEERR + 38
WSAEDESTADDRREQ          equ        WSABASEERR + 39
WSAEMSGSIZE          equ            WSABASEERR + 40
WSAEPROTOTYPE          equ          WSABASEERR + 41
WSAENOPROTOOPT          equ         WSABASEERR + 42
WSAEPROTONOSUPPORT          equ     WSABASEERR + 43
WSAESOCKTNOSUPPORT          equ     WSABASEERR + 44
WSAEOPNOTSUPP          equ          WSABASEERR + 45
WSAEPFNOSUPPORT          equ        WSABASEERR + 46
WSAEAFNOSUPPORT          equ        WSABASEERR + 47
WSAEADDRINUSE          equ          WSABASEERR + 48
WSAEADDRNOTAVAIL          equ       WSABASEERR + 49
WSAENETDOWN          equ            WSABASEERR + 50
WSAENETUNREACH          equ         WSABASEERR + 51
WSAENETRESET          equ           WSABASEERR + 52
WSAECONNABORTED          equ        WSABASEERR + 53
WSAECONNRESET          equ          WSABASEERR + 54
WSAENOBUFS          equ             WSABASEERR + 55
WSAEISCONN          equ             WSABASEERR + 56
WSAENOTCONN          equ            WSABASEERR + 57
WSAESHUTDOWN          equ           WSABASEERR + 58
WSAETOOMANYREFS          equ        WSABASEERR + 59
WSAETIMEDOUT          equ           WSABASEERR + 60
WSAECONNREFUSED          equ        WSABASEERR + 61
WSAELOOP          equ               WSABASEERR + 62
WSAENAMETOOLONG          equ        WSABASEERR + 63
WSAEHOSTDOWN          equ           WSABASEERR + 64
WSAEHOSTUNREACH          equ        WSABASEERR + 65
WSAENOTEMPTY          equ           WSABASEERR + 66
WSAEPROCLIM          equ            WSABASEERR + 67
WSAEUSERS          equ              WSABASEERR + 68
WSAEDQUOT          equ              WSABASEERR + 69
WSAESTALE          equ              WSABASEERR + 70
WSAEREMOTE          equ             WSABASEERR + 71
WSASYSNOTREADY          equ         WSABASEERR+91
WSAVERNOTSUPPORTED          equ     WSABASEERR+92
WSANOTINITIALISED          equ      WSABASEERR+93
WSAHOST_NOT_FOUND          equ      WSABASEERR + 1001
HOST_NOT_FOUND          equ         WSAHOST_NOT_FOUND
WSATRY_AGAIN          equ           WSABASEERR + 1002
TRY_AGAIN          equ              WSATRY_AGAIN
WSANO_RECOVERY          equ         WSABASEERR + 1003
NO_RECOVERY          equ            WSANO_RECOVERY
WSANO_DATA          equ             WSABASEERR + 1004
NO_DATA          equ                WSANO_DATA
WSANO_ADDRESS          equ          WSANO_DATA
NO_ADDRESS          equ             WSANO_ADDRESS
EWOULDBLOCK          equ            WSAEWOULDBLOCK
EINPROGRESS          equ            WSAEINPROGRESS
EALREADY          equ               WSAEALREADY
ENOTSOCK          equ               WSAENOTSOCK
EDESTADDRREQ          equ           WSAEDESTADDRREQ
EMSGSIZE          equ               WSAEMSGSIZE
EPROTOTYPE          equ             WSAEPROTOTYPE
ENOPROTOOPT          equ            WSAENOPROTOOPT
EPROTONOSUPPORT          equ        WSAEPROTONOSUPPORT
ESOCKTNOSUPPORT          equ        WSAESOCKTNOSUPPORT
EOPNOTSUPP          equ             WSAEOPNOTSUPP
EPFNOSUPPORT          equ           WSAEPFNOSUPPORT
EAFNOSUPPORT          equ           WSAEAFNOSUPPORT
EADDRINUSE          equ             WSAEADDRINUSE
EADDRNOTAVAIL          equ          WSAEADDRNOTAVAIL
ENETDOWN          equ               WSAENETDOWN
ENETUNREACH          equ            WSAENETUNREACH
ENETRESET          equ              WSAENETRESET
ECONNABORTED          equ           WSAECONNABORTED
ECONNRESET          equ             WSAECONNRESET
ENOBUFS          equ                WSAENOBUFS
EISCONN          equ                WSAEISCONN
ENOTCONN          equ               WSAENOTCONN
ESHUTDOWN          equ              WSAESHUTDOWN
ETOOMANYREFS          equ           WSAETOOMANYREFS
ETIMEDOUT          equ              WSAETIMEDOUT
ECONNREFUSED          equ           WSAECONNREFUSED
ELOOP          equ                  WSAELOOP
ENAMETOOLONG          equ           WSAENAMETOOLONG
EHOSTDOWN          equ              WSAEHOSTDOWN
EHOSTUNREACH          equ           WSAEHOSTUNREACH
ENOTEMPTY          equ              WSAENOTEMPTY
EPROCLIM          equ               WSAEPROCLIM
EUSERS          equ                 WSAEUSERS
EDQUOT          equ                 WSAEDQUOT
ESTALE          equ                 WSAESTALE
EREMOTE          equ                WSAEREMOTE
FD_SETSIZE  equ 64
INVALID_SOCKET              equ NOT 0
SOCKET_ERROR                equ -1
SOCKET_BUFFER_SIZE          equ 512
ICMP_ECHOREPLY          equ   0
ICMP_ECHOREQ            equ   8

; ------------------------ wsock32 structures -----------------------------

fd_setstruc STRUCT
  fd_count  DWORD      ?
  fd_array  DWORD      ?
fd_setstruc ENDS

timeval STRUCT
  tv_sec  DWORD      ?
  tv_usec  DWORD      ?
timeval ENDS

sockaddr_in STRUCT
  sin_family  WORD      ?
  sin_port  WORD      ?
  sin_addr  DWORD      ?
  sin_zero  BYTE 8 dup (?)
sockaddr_in ENDS

_sockaddr STRUCT
  _sa_family  WORD      ?
  _sa_data  WORD      ?
_sockaddr ENDS

sockaddr STRUCT
 sa_family WORD ?
 sa_data BYTE 14 dup(?)
sockaddr ENDS

WSAdata STRUCT
  wVersion  WORD      ?
  wHighVersion  WORD      ?
  szDescription  BYTE WSADESCRIPTION_LEN + 1 dup (?)
  szSystemStatus  BYTE WSASYS_STATUS_LEN + 1 dup (?)
  iMaxSockets  WORD      ?
  iMaxUdpDg  WORD      ?
  lpVendorInfo  DWORD      ?
WSAdata ENDS

sockproto STRUCT
  sp_family  WORD      ?
  sp_protocol  WORD      ?
sockproto ENDS

linger STRUCT
  l_onoff  WORD      ?
  l_linger  WORD      ?
linger ENDS

hostentStru STRUCT
  h_name      DWORD      ?
  h_alias     DWORD      ?
  h_addr      WORD      ?
  h_len       WORD      ?
  h_list      DWORD    ?
hostentStru ENDS

netent STRUCT
  n_name      DWORD      ?
  n_aliases   DWORD      ?
  n_addrtype  WORD      ?
  n_net       DWORD      ?
netent ENDS

servent STRUCT
  s_name     DWORD      ?
  s_aliases  DWORD      ?
  s_port     WORD      ?
  s_proto    DWORD      ?
servent ENDS

icmp_hdr STRUCT
  icmp_type   BYTE      ?
  icmp_code   BYTE      ?
  icmp_cksum  WORD      ?
  icmp_id     WORD      ?
  icmp_seq    WORD      ?
  icmp_data   BYTE      ?
icmp_hdr ENDS

ip_hdr STRUCT
  ip_hlv    BYTE      ?
  ip_tos    BYTE      ?
  ip_len    WORD      ?
  ip_id     WORD      ?
  ip_off    WORD      ?
  ip_ttl    BYTE      ?
  ip_p      BYTE      ?
  ip_cksum  WORD      ?
  ip_src    DWORD      ?
  ip_dest   DWORD      ?
ip_hdr ENDS

ICMP_OPTIONS STRUCT
  Ttl              BYTE      ?
  Tos              BYTE      ?
  Flags            BYTE      ?
  OptionsSize      BYTE      ?
  OptionsData      DWORD      ?
ICMP_OPTIONS ENDS

ICMP_ECHO_REPLY STRUCT
  Address          DWORD      ?
  Status           DWORD      ?
  RoundTripTime    DWORD      ?
  DataSize         WORD      ?
  Reserved         WORD      ?
  DataPointer      DWORD      ?
  Options          DWORD      ?
  zData            BYTE 250 dup (?)
ICMP_ECHO_REPLY ENDS

