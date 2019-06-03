;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : MAIN.ASM
;³Author       : Cyberfish/TLS
;³Last update  : 12.12.1998
;³Action       :
;³
;³Notes :
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
.386
.Model flat, C
.code

include w:\general\var.inc

public MN_MainLoop                      ;mainloop-procedure
public ZERO_Object                      ;zeroobject, routine with just a ret
public KEY_OPressed                     ;Keypressed object
public KEY_RPressed                     ;Keypressed routine

public BRK_OFrames      		;Object
public BRK_RFrames

;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : MN_MainLoop
;³ID           : MN
;³Type         : Procedure
;³Last update  : 12.12.1998
;³Action       : The main loop (including plugins)
;³Optimized    : -
;³
;³Input variables : SYS_OListOffs
;³
;³Notes : List ends with -1 (FFFFFFFFh)
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

MN_MainLoop   PROC

MN_MainL:       mov    esi, SYS_OListOffs
		inc    SYS_NrFrames

MN_OLoop:       mov    eax, DWORD PTR [ esi ]   ;Load objectoffset
                cmp    eax, -1                  ;End of list?
                je     MN_MainL                 ;Done loop
                cmp    eax, 0                   ;Fake object?
                je     MN_FakeObject            ;Dump object

                push   esi                      ;Save list-pointer
                mov    SYS_OOffs, eax           ;Save object's offset





;ÄÄÄÄ Running plugins...
                xor    ecx, ecx
                lea    ebx, [ eax + 04 ]        ;Point ebx to plugin-list

MN_PluginL:     mov    eax, [ ebx ]             ;Load pluginoffset
                cmp    eax, -1                  ;End of pluginlist?
                je     MN_DonePlugins           ;Done plugins
                cmp    eax, 0                   ;Fake plugin?
                je     MN_FakePlugin            ;Dump plugin

                push   ebx
                push   ecx

                mov    eax, [ ebx + 04 ]        ;Load plugindataoffset
                mov    SYS_POffs, eax           ;Setting up system variables
                mov    eax, SYS_OOffs           ;/
                mov    SYS_PCaller, eax         ;/

		pushad				;Save all registers in case
						;/of C++ calls...

                call   [ ebx ]                  ;Call plugin!

		popad

                pop    ecx
                pop    ebx

MN_FakePlugin:  add    ebx, 8                   ;Next plugin
                inc    ecx
                cmp    ecx, 8
                je     MN_DonePlugins
                jmp    MN_PluginL

MN_DonePlugins:

;ÄÄÄÄ Done plugins...



                mov    eax, SYS_OOffs           ;Restore objectoffset
                mov    ebx, [ eax ]             ;Load object-type
                call   SYS_ORoutines[ ebx*4 ]
                pop    esi                      ;Restore listpointer

MN_FakeObject:  add    esi, 4
                cmp    SYS_QuitFlag, 0	       ;Test quit-flag
                je     MN_OLoop                ;Quit loop!

                ;jmp    MN_OLoop
MN_Done:

MN_MainLoop   ENDP




;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : ZERO_Object
;³ID           : ZERO
;³Type         : Procedure (object 0000)
;³Last update  : 10.02.1998
;³Action       : Does nothing. Just returns.
;³Optimized    : -
;³
;³Notes :
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ZERO_Object   PROC
                ret
ZERO_Object   ENDP






;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : KEY_Routine
;³ID           : KEY
;³Type         : Procedure
;³Last update  :
;³Action       : Sets SYS_QuitFlag if keypressed.
;³Optimized    : -
;³
;³Notes :
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

KEY_OPressed  DD C_KeyPressed                   ;Keypressed
              DD -1                             ;No plugins

KEY_QuitMsg   DB 'KEY: Key pressed. Terminating...', 0Dh, 0Ah, 0, '$'

KEY_RPressed  PROC
                mov    ah, 0Bh                  ;
                int    21h                      ;
                cmp    al, 0                    ;Keypressed?
                jz     KEY_Nothing              ;Nope. Return

                mov    SYS_QuitFlag, -1         ;Set quitflag
                mov    SYS_QuitMsg, OFFSET KEY_QuitMsg

KEY_Nothing:	ret
KEY_RPressed  ENDP



;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : BRK_Frames
;³ID           : BRK
;³Type         : Procedure
;³Last update  : 10.02.1998
;³Action       : Sets SYS_QuitFlag if keypressed.
;³Optimized    : -
;³
;³Notes :
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

BRK_OFrames   DD C_BreakFrames                  ;Keypressed
              DD -1                             ;No plugins
	      DD 15 DUP (0)
	      DD 0

BRK_QuitMsg   DB 'BRK: Breaking...', 0Dh, 0Ah, 0, '$'

BRK_RFrames   PROC
		mov	esi, SYS_OOffs
		dec	d[esi + 68]
		jnz	BRK_Nothing

		mov	SYS_QuitFlag, -1
		mov	SYS_QuitMsg, OFFSET BRK_QuitMsg

BRK_Nothing:	ret
BRK_RFrames   ENDP


END
