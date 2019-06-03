 ;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : PCX.ASM
;³Author       : Cyberfish of The Lost Souls
;³Last update  : 30.08.1997
;³Action       : Decodes a PCX-picture
;³
;³Notes :
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
.386
.Model flat
.code

include w:\general\var.inc
include w:\general\mem.inc

public        PCX_Load
public        PCX_XPos
public        PCX_YPos
public        PCX_Width
public        PCX_Heigth
public        PCX_Palette


;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : PCX_Load
;³ID           : PCX
;³Type         : Procedure
;³Last update  : 30.08.1997
;³Action       : Decodes a .PCX and stores it in memory
;³Optimized    : -
;³
;³Input variables : edx = OFFSET FileName
;³                  ecx = Filesize. If zero then read to end of file
;³                  esi = Position in file to read from
;³
;³Registers changed : all.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

PCX_Handle    DW 0
PCX_XPos      DD 0
PCX_YPos      DD 0
PCX_Width     DD 0
PCX_Heigth    DD 0
PCX_Palette   DB 768 DUP (0)
PCX_TempBuf   DD 0
PCX_OutPut    DD 0


PCX_Load      PROC
                mov    ah, 3Dh                  ;Open file
                mov    al, 0                    ;Read access
                int    21h
                jc     PCX_OpenErr

                mov    PCX_Handle, ax           ;Save filehandle

                or     ecx, ecx                 ;ecx = 0 = read to EOF
                jnz    PCX_ReadFile


;---- Getting filesize...
                xor    edx, edx                 ;No move
                mov    ah, 42h                  ;Move filepointer
                mov    al, 2                    ;..from start of file
                mov    bx, PCX_Handle           ;Load filehandle
                int    21h
                jc     PCX_MoveErr
                mov    ecx, eax                 ;eax = filesize
                sub    ecx, esi                 ;ecx = number of bytes to read




;---- Reading file...
PCX_ReadFile:
                mov    eax, ecx                 ;eax = size of tempbuffer
                call   MEM_Alloc
                jc     PCX_MemErr1
                mov    PCX_TempBuf, edx


                mov    edx, esi                 ;edx = FileOffset
                mov    ah, 42h                  ;Move filepointer
                mov    al, 0                    ;..from start of file
                mov    bx, PCX_Handle           ;Load filehandle
                int    21h
                jc     PCX_MoveErr
                                                ;ecx = Bytes to read
                mov    ah, 3Fh                  ;Read from file
                mov    bx, PCX_Handle           ;Load filehandle
                mov    edx, PCX_TempBuf         ;Load file into tempbuffer
                int    21h
                jc     PCX_ReadErr

                mov    ah, 3Eh                  ;Close filehandle
                mov    bx, PCX_Handle           ;Load filehandle
                int    21h
                jc     PCX_CloseErr


;---- Reading header...
                                                ;edi = Where to store picture
                                                ;PCX_Temp = Loaded file
                mov    ebp, PCX_TempBuf
                movzx  eax, WORD PTR [ ebp + 04 ]        ;Load UpperX
                movzx  ebx, WORD PTR [ ebp + 06 ]        ;Load UpperY
                movzx  ecx, WORD PTR [ ebp + 08 ]        ;Load Width
                movzx  edx, WORD PTR [ ebp + 10 ]        ;Load heigth
                inc    ecx
                inc    edx
                mov    PCX_XPos, eax            ;Save values
                mov    PCX_YPos, ebx            ;/
                mov    PCX_Width, ecx           ;/
                mov    PCX_Heigth, edx          ;/
                imul   edx, ecx                 ;edx = number of pixels

                push   edx
                mov    eax, edx
                call   MEM_Alloc
                mov    PCX_OutPut, edx
                mov    edi, edx
                pop    edx

                add    ebp, 128                 ;Dump header


                xor    eax, eax
                xor    ecx, ecx

;---- Decompressing...

PCX_Loop:       mov    al, [ ebp ]              ;Load byte
                cmp    al, 192                  ;Check it...
                ja     PCX_Fill

                mov    [ edi + ecx ], al        ;Single pixel
                inc    ecx
                inc    ebp
                cmp    ecx, edx                 ;Picture done?
                jb     PCX_Loop
                jmp    PCX_Done

                                                ;Compressed pixels
PCX_Fill:       mov    ebx, eax                 ;ebx = counter
                mov    al, [ ebp + 1 ]          ;Load color
                sub    ebx, 192

PCX_FillLoop:   mov    [ edi + ecx ], al
                inc    ecx
                dec    ebx
                jnz    PCX_FillLoop

                add    ebp, 2
                cmp    ecx, edx                 ;Picture done?
                jb     PCX_Loop



PCX_Done:       lea    esi, [ ebp + 1 ]         ;Point esi to palette
                mov    edi, OFFSET PCX_Palette  ;Store palette here
                mov    ecx, 768

PCX_PalFix:     lodsb
                shr    al, 2
                stosb
                dec    ecx
                jnz    PCX_PalFix

                mov    edx, PCX_TempBuf
                call   MEM_DeAlloc
                jc     PCX_MemErr2

                mov    edi, PCX_OutPut

                clc                             ;Everything OK
                ret
PCX_Load      ENDP






PCX_Err1      DB 'PCX: Error opening file.', 0Dh, 0Ah, '$'
PCX_Err2      DB 'PCX: Error moving filepointer.', 0Dh, 0Ah, '$'
PCX_Err3      DB 'PCX: Error reading file.', 0Dh, 0Ah, '$'
PCX_Err4      DB 'PCX: Error closing file.', 0Dh, 0Ah, '$'


PCX_OpenErr:    mov    SYS_QuitMsg, OFFSET PCX_Err1     ;Error opening file
                stc
                ret

PCX_MoveErr:    mov    ah, 3Eh                          ;Close filehandle
                mov    bx, PCX_Handle                   ;Load filehandle
                int    21h
                jc     PCX_CloseErr

                mov    SYS_QuitMsg, OFFSET PCX_Err2     ;Error moving filepointer
                stc
                ret

PCX_ReadErr:    mov    ah, 3Eh                          ;Close filehandle
                mov    bx, PCX_Handle                   ;Load filehandle
                int    21h
                jc     PCX_CloseErr

                mov    SYS_QuitMsg, OFFSET PCX_Err3     ;Error reading file
                stc
                ret

PCX_CloseErr:   mov    SYS_QuitMsg, OFFSET PCX_Err4     ;Error closing file
                stc
                ret

PCX_MemErr1:    mov    ah, 3Eh                          ;Close filehandle
                mov    bx, PCX_Handle                   ;Load filehandle
                int    21h
                jc     PCX_CloseErr

                stc
                ret

PCX_MemErr2:    stc
                ret


END