;旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴� ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴�
;쿙ame         : GIF.ASM
;쿌uthor       : Cyberfish of The Lost Souls
;쿗ast update  : 05.08.1997
;쿌ction       : Decodes a GIF89a-picture
;�
;쿙otes : Look GIF.DOC
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

.386
.model flat
.code

include w:\general\var.inc
include w:\general\mem.inc

public GIF_Load
public GIF_SWidth
public GIF_SHeigth
public GIF_Width
public GIF_Heigth
public GIF_XPos
public GIF_YPos
public GIF_Colors
public GIF_Palette

;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : GIF_Load
;쿔D           : GIF
;쿟ype         : Procedure
;쿗ast update  : 03.08.1997
;쿌ction       : Decodes a .GIF and stores it in memory
;쿚ptimized    : -
;�
;쿔nput variables : edx = OFFSET FileName
;�                  edi = Where to store picture
;�                  ecx = Filesize. If zero then read to end of file
;�                  esi = Position in file to read from
;�
;쿝egisters changed : all.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

                                                ;User-variables
GIF_SWidth    DD 0                              ;Screen horisontal res
GIF_SHeigth   DD 0                              ;Screen vertical res
GIF_Width    DD 0                               ;Image width
GIF_Heigth    DD 0                              ;Image heigth
GIF_XPos      DD 0                              ;Image's horisontal pos
GIF_YPos      DD 0                              ;Image's vertical pos
GIF_Colors    DD 0                              ;Number of colors in GIF
GIF_Palette   DB 768 DUP (0)

                                                ;Loop variables
GIF_BitsPrPix DD 0                              ;Bits pr pixel
GIF_Last      DD 0                              ;Offset to last charstream
GIF_Current   DD 0                              ;Offset to current charstream
GIF_Mask      DD 0                              ;Bitmask to get code
GIF_MaxCode   DD 0                              ;Current number of charstreams
GIF_InitBits  DD 0                              ;Initial number of bitsprcode
GIF_Bits      DD 0                              ;Current number of bitsprcode
GIF_BitsPos   DD 0                              ;Position of code in DWORD
GIF_Handle    DW 0                              ;File handle
GIF_ILaced    DD 0

                                                ;Pointers...
GIF_Output    DD 0                              ;Writing data here
GIF_CodeBuf   DD 0                              ;Storing codestream here
GIF_STables   DD 0                              ;Table of string pointers
GIF_EndBuf    DD 0                              ;Pointer to end of charstreams
GIF_EndList   DD 0                              ;Pointer to end of STable
GIF_FileBuf   DD 0

GIF_Err1      DB 'GIF: Error opening file.', 0Dh, 0Ah, '$'
GIF_Err2      DB 'GIF: Error moving filepointer.', 0Dh, 0Ah, '$'
GIF_Err3      DB 'GIF: Error reading file.', 0Dh, 0Ah, '$'
GIF_Err4      DB 'GIF: Error closing file.', 0Dh, 0Ah, '$'
GIF_Err5      DB 'GIF: Error allocating memory.', 0Dh, 0Ah, '$'


GIF_Init      MACRO
LOCAL GIF_InitL
                mov    ecx, GIF_InitBits
                mov    GIF_Bits, ecx
                mov    eax, 1                   ;Set mask
                shl    eax, cl                  ;/
                dec    eax                      ;/
                mov    GIF_Mask, eax            ;/

                dec    ecx                      ;Set maxcode
                mov    eax, 1                   ;/
                shl    eax, cl                  ;/
                inc    eax                      ;/Add special codes
                mov    GIF_MaxCode, eax         ;/

                mov    ebp, GIF_STables
                xor    ecx, ecx
                mov    edx, GIF_Colors
                lea    eax, [ ebp + 16384 ]


GIF_InitL:      mov    [ ebp + ecx*4 ], eax     ;Set pointer

                mov    DWORD PTR [ eax ], 1     ;Length of string
                mov    BYTE PTR [ eax + 4 ], cl ;Character

                add    eax, 5
                inc    ecx
                cmp    ecx, edx
                jb     GIF_InitL

                lea    ebx, [ ebp + ecx*4 ]
                mov    [ ebx ], eax
                mov    DWORD PTR [ eax ], 0     ;Save length=0 (clear code)
                add    eax, 5
                mov    [ ebx + 4 ], eax
                mov    DWORD PTR [ eax ], -1    ;Save length=-1 (EOI-code)
                add    eax, 5
                mov    GIF_EndBuf, eax          ;Save current end of buffer
                add    ebx, 8
                mov    GIF_EndList, ebx         ;Save current end of stable

              ENDM


GIF_TestBits  MACRO
LOCAL GIF_Test1, GIF_Test2
                cmp    GIF_BitsPos, 32
                jb     GIF_Test1
                mov    GIF_BitsPos, 8
                add    ebx, 3

GIF_Test1:
                mov    esi, 32
                sub    esi, GIF_Bits
                cmp    GIF_BitsPos, esi
                jbe    GIF_Test2
                sub    GIF_BitsPos, 16
                add    ebx, 2
GIF_Test2:


              ENDM







GIF_Load      PROC
                mov    GIF_BitsPos, 0

                mov    ah, 3Dh
                mov    al, 0                    ;Read access
                int    21h
                jc     GIF_OpenErr
                mov    GIF_Handle, ax           ;Save filehandle

                or     ecx, ecx
                jnz    GIF_ReadFile


;---- Getting filesize...
                xor    edx, edx                 ;Don't move!
                mov    ah, 42h                  ;Move filepointer
                mov    al, 2                    ;..from end of file
                mov    bx, GIF_Handle           ;Load filehandle
                int    21h
                mov    ecx, eax                 ;FileSize
                sub    ecx, esi                 ;Subtract startoffset


;---- Reading file...
GIF_ReadFile:
                mov    eax, ecx                 ;Allocate memory for filebuf
                call   MEM_Alloc
                jc     GIF_MEMErr
                mov    GIF_FileBuf, edx

                mov    edx, esi                 ;edx = FileOffset
                mov    ah, 42h                  ;Move filepointer
                mov    al, 0                    ;..from start of file
                mov    bx, GIF_Handle           ;Load filehandle
                int    21h

                                                ;ecx = Bytes to read
                mov    ah, 3Fh                  ;Read from file
                mov    bx, GIF_Handle           ;Load filehandle
                mov    edx, GIF_FileBuf         ;Use tempbuffer
                int    21h
                jc     GIF_ReadErr

                mov    ah, 3Eh                  ;Close filehandle
                mov    bx, GIF_Handle           ;Load filehandle
                int    21h
                jc     GIF_CloseErr


;---- Allocating memory...
                mov    eax, 128*1024            ;128k for codestream
                call   MEM_Alloc
                jc     GIF_MEMErr
                mov    GIF_CodeBuf, edx

                mov    eax, 384*1024            ;384k for charstreams
                call   MEM_Alloc
                jc     GIF_MEMErr
                mov    GIF_STables, edx


;---- Reading GIF-header
                mov    esi, GIF_FileBuf

                movzx  eax, WORD PTR [ esi + 6 ];Horisontal screenres
                mov    GIF_SWidth, eax          ;/Save!

                movzx  eax, WORD PTR [ esi + 8 ];Vertical screenres
                mov    GIF_SHeigth, eax         ;/Save!

                movzx  ebx, BYTE PTR [ esi + 10 ];Bits pr. color
                and    ebx, 00000111b           ;Mask out lower 3 bits

                lea    ecx, [ ebx + 2 ]         ;Calculate initial codesize
                mov    GIF_InitBits, ecx        ;/Save!

                lea    ecx, [ ebx + 1 ]         ;Calculate nr. of colors
                mov    eax, 1                   ;/
                shl    eax, cl                  ;/
                mov    GIF_Colors, eax          ;/Save!

                movzx  ebx, BYTE PTR [ esi + 10 ];Looking for global cmap
                add    esi, 13                  ;Dump GIF-header
                test   bl, 10000000b
                jz     GIF_NoGMap

                cld
                mov    edi, OFFSET GIF_Palette  ;Store palette

                mov    ecx, GIF_Colors          ;Size of palette
                lea    ecx, [ ecx + ecx*2 ]     ;Mul by 3

GIF_GMap:       lodsb                           ;Copy local palette
                shr    al, 2
                stosb
                dec    ecx
                jnz    GIF_GMap
GIF_NoGMap:

;---- Reading image descriptor
                movzx  eax, WORD PTR [ esi + 1 ];Image startx
                mov    GIF_XPos, eax            ;/Save!

                movzx  eax, WORD PTR [ esi + 3 ];Image starty
                mov    GIF_YPos, eax            ;/Save!

                movzx  eax, WORD PTR [ esi + 5 ];Image width
                mov    GIF_Width, eax           ;/Save!

                movzx  eax, WORD PTR [ esi + 7 ];Image heigth
                mov    GIF_Heigth , eax         ;/Save!

                movzx  ebx, BYTE PTR [ esi + 9 ];Looking for local cmap
                add    esi, 10                  ;Dump image header
                test   bl, 10000000b
                jz     GIF_NoLMap

                cld
                mov    edi, OFFSET GIF_Palette

                mov    ecx, GIF_Colors          ;Size of palette
                lea    ecx, [ ecx + ecx*2 ]     ;Mul by 3
GIF_LMap:       lodsb                           ;Copy local palette
                shr    al, 2
                stosb
                dec    ecx
                jnz    GIF_LMap

GIF_NoLMap:     test   bl, 1000000b
                setnz  al
                movzx  eax, al
                mov    GIF_ILaced, eax

                mov    eax, GIF_Width
                imul   eax, GIF_Heigth
                call   MEM_Alloc
                jc     GIF_MEMErr
                mov    GIF_Output, edx



;---- Setting up...
;esi = start of image data

                GIF_Init

                mov    ebp, GIF_STables         ;String-table-pointers
                mov    edi, GIF_CodeBuf

;CHECK FOR EXTENSION BLOCKS HERE!!!!!!!!!!! [esi]
                inc    esi                      ;Dump codesize

GIF_LCodes:     movzx  ecx, BYTE PTR [ esi ]    ;Load BlockSize
                or     ecx, ecx                 ;End Of Information? (EOI)
                jz     GIF_DoneLC
                inc    esi                      ;Dump block size

                rep    movsb                    ;Copy block to codebuffer
                jmp    GIF_LCodes
GIF_DoneLC:

                mov    edi, GIF_Output          ;Output buffer
                mov    ebx, GIF_CodeBuf         ;Code-buffer



;---- Decoding GIFdata...
GIF_DLoop:      mov    ecx, GIF_BitsPos         ;Code's position in bits
                mov    eax, DWORD PTR [ ebx ]   ;Load code
                shr    eax, cl                  ;Make current code lower bits

                and    eax, GIF_Mask            ;Mask out bits to be used

                mov    ecx, GIF_Bits
                add    GIF_BitsPos, ecx         ;Move bit-position

                cmp    eax, GIF_MaxCode
                jg     GIF_NoCode




;---- Charstream for current code exists...

                lea    esi, [ ebp + eax*4 ]     ;Load pointer to STable
                mov    esi, [ esi ]             ;Load pointer to charstream
                mov    ecx, [ esi ]             ;Load length of charstream

                cmp    ecx, -1                  ;Test of <EOI>
                je     GIF_Done                 ;End of data
                cmp    ecx, 0                   ;Test of <CC>
                je     GIF_Reset                ;Clear String-tables

                mov    GIF_Current, esi         ;Saving current charstreamoffs
                add    esi, 4                   ;Dump "length"

                rep    movsb                    ;Output charstream


;---- Adding new string to STable...
                push   edi                      ;Save output-offset

                mov    esi, GIF_EndList         ;Updating list-end-pointer
                mov    edi, GIF_EndBuf          ;/
                mov    [ esi ], edi             ;/
                add    GIF_EndList, 4           ;/

                mov    esi, GIF_Last            ;Load offs of last charstream
                mov    ecx, [ esi ]             ;Load length of charstream
                inc    ecx                      ;One byte longer
                mov    [ edi ], ecx             ;Save length of new charstream
                add    ecx, 4                   ;Update buffer-end-pointer
                add    GIF_EndBuf, ecx

                mov    ecx, [ esi ]             ;Length of last charstream

                add    edi, 4                   ;Dump "length"
                add    esi, 4                   ;/

                rep    movsb                    ;Copy charstream

                mov    edx, GIF_Current         ;Restore current offset
                mov    GIF_Last, edx            ;Make current -> last
                mov    cl, [ edx + 4 ]          ;Load first byte of current
                mov    [ edi ], cl              ;Add byte to charstream

                pop    edi                      ;Restore Destination pointer

                inc    GIF_MaxCode
                mov    eax, GIF_Mask
                cmp    GIF_MaxCode, eax
                jb     GIF_BitsOK1

                cmp    GIF_Bits, 12             ;Maximum bits?
                je     GIF_BitsOK1
                inc    GIF_Bits
                mov    ecx, GIF_Bits            ;Set new mask
                mov    esi, 1                   ;/
                shl    esi, cl                  ;/
                dec    esi                      ;/
                mov    GIF_Mask, esi

GIF_BitsOK1:    GIF_TestBits
                jmp    GIF_DLoop


GIF_NoCode:     mov    esi, GIF_Last            ;Load last charstream
                mov    ecx, [ esi ]

                add    esi, 4
                mov    al, [ esi ]
                rep    movsb
                stosb
                                                ;Adding new string to stable
                push   edi

                mov    esi, GIF_EndList         ;Updating STable
                mov    edi, GIF_EndBuf          ;/
                mov    [ esi ], edi             ;/
                add    GIF_EndList, 4

                mov    esi, GIF_Last            ;Load offs of last charstream
                mov    ecx, [ esi ]             ;Load length of charstream
                inc    ecx                      ;One byte longer...
                mov    [ edi ], ecx             ;Save length of new charstream
                add    ecx, 4
                add    GIF_EndBuf, ecx          ;Update buffer-end-pointer
                mov    ecx, [ esi ]             ;Load length of charstream
                mov    GIF_Last, edi            ;Set old charstream
                add    edi, 4
                add    esi, 4
                mov    al, [ esi ]
                rep    movsb
                stosb

                pop    edi                      ;Restore Destination pointer

                inc    GIF_MaxCode
                mov    ecx, GIF_Bits
                mov    eax, 1
                shl    eax, cl
                sub    eax, 1
                cmp    GIF_MaxCode, eax
                jb     GIF_BitsOK2

                cmp    GIF_Bits, 12
                je     GIF_BitsOK2
                inc    GIF_Bits
                mov    ecx, GIF_Bits            ;Mask out bits to be used
                mov    esi, 1                   ;/
                shl    esi, cl                  ;/
                dec    esi                      ;/
                mov    GIF_Mask, esi

GIF_BitsOK2:    GIF_TestBits
                jmp    GIF_DLoop




;---- Resetting tables...

GIF_Reset:      push   ebx

                GIF_Init                        ;Reset string-tables

                pop    ebx

                GIF_TestBits

                mov    ecx, GIF_BitsPos
                mov    eax, DWORD PTR [ ebx ]   ;Load code
                shr    eax, cl
                and    eax, GIF_Mask
                mov    esi, GIF_Bits
                add    GIF_BitsPos, esi

                GIF_TestBits

                lea    esi, [ ebp + eax*4 ]     ;Load pointer to stable
                mov    esi, [ esi ]             ;Load pointer to charstream
                mov    ecx, [ esi ]             ;Load length of charstream
                cmp    ecx, -1
                je     GIF_Done

                mov    GIF_Last, esi
                add    esi, 4
                rep    movsb
                jmp    GIF_DLoop


GIF_Done:       cmp    GIF_ILaced, 0            ;Interlaced?
                jz     GIF_NotILaced


;--- Deinterlacing picture, using GIF_CodeBuf as tempbuffer

                mov    esi, GIF_Output
                mov    eax, GIF_Width
                mov    ebx, GIF_Heigth

                mov    edi, GIF_STables
                xor    edx, edx                 ;Linecounter

GIF_8Passa:     push   edi
                mov    ecx, eax                 ;GIF_Width
                rep    movsb
                pop    edi

                lea    edi, [ edi + eax*8 ]     ;Move outputpointer
                add    edx, 8
                cmp    edx, ebx                 ;GIF_Heigth
                jb     GIF_8Passa


                mov    edi, GIF_STables
                lea    edi, [ edi + eax*4 ]
                mov    edx, 5

GIF_8Passb:     push   edi
                mov    ecx, eax                 ;GIF_Width
                rep    movsb
                pop    edi

                lea    edi, [ edi + eax*8 ]
                add    edx, 8
                cmp    edx, ebx                 ;GIF_Heigth
                jb     GIF_8Passb

                mov    edi, GIF_STables
                lea    edi, [ edi + eax*2 ]
                mov    edx, 2

GIF_4Pass:      push   edi
                mov    ecx, eax                 ;GIF_Width
                rep    movsb
                pop    edi

                lea    edi, [ edi + eax*4 ]
                add    edx, 4
                cmp    edx, ebx                 ;GIF_Heigth
                jb     GIF_4Pass


                mov    edi, GIF_STables
                lea    edi, [ edi + eax ]
                mov    edx, 1

GIF_2Pass:      push   edi
                mov    ecx, eax                 ;GIF_Width
                rep    movsb
                pop    edi

                lea    edi, [ edi + eax*2 ]
                add    edx, 2
                cmp    edx, ebx                 ;GIF_Heigth
                jb     GIF_2Pass

                mov    esi, GIF_STables
                mov    edi, GIF_Output

                mov    ecx, GIF_Width           ;Calculate nr. of bytes
                imul   ecx, GIF_Heigth          ;/in image
                rep    movsb


GIF_NotILaced:
                mov    edx, GIF_CodeBuf
                call   MEM_DeAlloc
                jc     GIF_MEMErr
                mov    edx, GIF_STables
                call   MEM_DeAlloc
                jc     GIF_MEMErr

                mov    edi, GIF_Output

                clc
                ret

GIF_OpenErr:    mov    SYS_QuitMsg, OFFSET GIF_Err1     ;Error opening file
                stc
                ret
GIF_MoveErr:    mov    SYS_QuitMsg, OFFSET GIF_Err2     ;Error moving filepointer
                stc
                ret
GIF_ReadErr:    mov    SYS_QuitMsg, OFFSET GIF_Err3     ;Error reading file
                stc
                ret
GIF_CloseErr:   mov    SYS_QuitMsg, OFFSET GIF_Err4     ;Error moving filepointer
                stc
                ret
GIF_MEMErr:     stc
                ret
GIF_Load      ENDP



END

