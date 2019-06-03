;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : TGA.ASM
;³Author       : Cyberfish of The Lost Souls
;³Last update  : 05.08.1997
;³Action       : Decodes a TGA-picture
;³
;³Notes: Rather simple stuff... Check out the TGA-spesification.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
.386
.Model flat
.code

include w:\general\var.inc
include w:\general\mem.inc

public TGA_Load
public TGA_Width
public TGA_Heigth
public TGA_Bits
public TGA_XPos
public TGA_YPos

TGA_Header    STRUC

IDFieldLength   DB ?
ColorMapType    DB ?
ImageType       DB ?
                                                ;Color map spesification
CMapOrigLo      DB ?
CMapOrigHi      DB ?
CMapLengthLo    DB ?
CMapLengthHi    DB ?
CMapSize        DB ?
                                                ;Image specification
XOrigLo         DB ?
XOrigHi         DB ?
YOrigLo         DB ?
YOrigHi         DB ?
WidthLo         DB ?
WidthHi         DB ?
HeigthLo        DB ?
HeigthHi        DB ?
Bits            DB ?
IDescriptor     DB ?

IDField         DB ?

TGA_Header    ENDS




;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : TGA_Load
;³ID           : TGA
;³Type         : Procedure
;³Last update  : 05.08.1997
;³Action       : Decodes a .tga and stores it in memory
;³Optimized    : -
;³
;³In:               edx = OFFSET FileName
;³                  ecx = Filesize. If zero then read to end of file
;³                  esi = Position in file to read from
;³
;³Out:              edi = offset picture
;³
;³Registers changed : all
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

TGA_XPos        DD ?
TGA_YPos        DD ?
TGA_Width       DD ?
TGA_Heigth      DD ?
TGA_Bits        DD ?

Handle          DW ?
FileBuf         DD ?
OutPut          DD ?

TGA_Load      PROC
                mov    ah, 3Dh
                mov    al, 0                    ;Read access
                int    21h
                jc     Error1
                mov    Handle, ax           ;Save filehandle

                or     ecx, ecx
                jnz    ReadFile


;---- Getting filesize...
                xor    edx, edx                 ;Don't move!
                mov    ah, 42h                  ;Move filepointer
                mov    al, 2                    ;..from end of file
                mov    bx, Handle               ;Load filehandle
                int    21h
                mov    ecx, eax                 ;FileSize
                sub    ecx, esi                 ;Subtract startoffset


;---- Reading file...
ReadFile:
                mov    eax, ecx                 ;Allocate memory for filebuf
                call   MEM_Alloc
                jc     Error2
                mov    FileBuf, edx

                mov    edx, esi                 ;edx = FileOffset
                mov    ah, 42h                  ;Move filepointer
                mov    al, 0                    ;..from start of file
                mov    bx, Handle               ;Load filehandle
                int    21h
                jc     Error3

                                                ;ecx = Bytes to read
                mov    ah, 3Fh                  ;Read from file
                mov    bx, Handle               ;Load filehandle
                mov    edx, FileBuf             ;Use tempbuffer
                int    21h
                jc     Error4

                mov    ah, 3Eh                  ;Close filehandle
                mov    bx, Handle               ;Load filehandle
                int    21h
                jc     Error5


;---- Reading header...
                mov    ebp, FileBuf


                xor    eax, eax

                mov    al, [ebp].XOrigLo
                mov    ah, [ebp].XOrigHi
                mov    TGA_XPos, eax
                mov    al, [ebp].YOrigLo
                mov    ah, [ebp].YOrigHi
                mov    TGA_YPos, eax

                mov    al, [ebp].WidthLo
                mov    ah, [ebp].WidthHi
                mov    TGA_Width, eax
                mov    al, [ebp].HeigthLo
                mov    ah, [ebp].HeigthHi
                mov    TGA_Heigth, eax

                movzx  eax, [ebp].Bits
                mov    TGA_Bits, eax

                movzx  esi, [ebp].IDFieldLength
                lea    esi, [ebp + esi + 18]    ;[esi] = cmap

                movzx  eax, [ebp].CMapSize      ;Dump cmap
                xor    ebx, ebx
                mov    bl, [ebp].CMapLengthLo
                mov    bh, [ebp].CMapLengthHi
                imul   eax, ebx
                add    esi, eax

                mov    ecx, TGA_Width
                imul   ecx, TGA_Heigth          ;ecx = number of pixels
                mov    eax, TGA_Bits
                shr    eax, 3                   ;div by 8
                imul   eax, ecx                 ;eax = size of picture/bytes
                add    eax, 32
                call   MEM_Alloc
                jc     Error7
                and    edx, -32
                mov    OutPut, edx

                mov    edi, OutPut

                                                ; eax  = picture size/bytes
                                                ; ecx  = number of pixels
                                                ;[esi] = picture offset
                                                ;[edi] = store picture here
                                                ;[ebp] = filebuffer


;---- About to decode picture data...

                cmp    [ebp].ImageType, 2       ;Type  2 = RGB, raw
                je     Type02
                cmp    [ebp].ImageType, 10      ;Type 10 = RGB, RLE
                je     Type10
                jmp    Error6





;---- Decoding TGA, type 02 (RGB, raw image data)

Type02:

                cmp    TGA_Bits, 16             ;How many bits per pixel?
                je     Type02_16
                cmp    TGA_Bits, 24
                je     Type02_24
                jmp    Error8




Type02_16:                                      ;Type 02, 16 bits
                mov    al, [esi]                ;Load, flip and store pixel
                mov    ah, [esi + 1]            ;/
                mov    [edi], ax                ;/

                add    esi, 2
                add    edi, 2
                dec    ecx
                jnz    Type02_16
                jmp    Done

Type02_24:                                      ;Type 02, 24 bits
                mov    al, [esi + 0]            ;B
                mov    ah, [esi + 1]            ;G
                mov    bl, [esi + 2]            ;R

                mov    [edi + 0], al            ;R
                mov    [edi + 1], ah            ;G
                mov    [edi + 2], bl            ;B

                add    esi, 3
                add    edi, 3
                dec    ecx
                jnz    Type02_24
                jmp    Done







;---- Decoding TGA, type 10 (RGB, RLE image data)
Type10:
                mov    ebp, ecx                 ;ebp = number of pixels
                xor    edx, edx                 ;edx = pixelcounter

                cmp    TGA_Bits, 16             ;How many bits per pixel?
                je     Type10_16
                cmp    TGA_Bits, 24
                je     Type10_24
                jmp    Error8

Type10_16:                                      ;Type 10, 16 bits
                movzx  ecx, BYTE PTR [esi]      ;Load header
                test   cl, 10000000b            ;Raw or RLE?
                jz     T10_16Raw

                                                ;RLE packet
                and    cl, 01111111b            ;ecx = counter
                inc    ecx
                add    edx, ecx
                mov    al, [esi + 1]            ;Flip bytes
                mov    ah, [esi + 2]
                rep    stosw                    ;Store pixels

                add    esi, 3

                cmp    edx, ebp                 ;Are we done?
                jb     Type10_16
                jmp    Done


T10_16Raw:
                inc    ecx
                inc    esi
                add    edx, ecx
T10_RawLoop16:                                  ;Copy raw block
                mov    al, [esi + 0]
                mov    ah, [esi + 1]
                mov    [edi], ax

                add    edi, 2
                add    esi, 2
                dec    ecx
                jnz    T10_RawLoop16

                cmp    edx, ebp
                jb     Type10_16
                jmp    Done



Type10_24:                                      ;Type 10, 24 bits
                movzx  ecx, BYTE PTR [esi]      ;Load header
                test   cl, 10000000b            ;Raw or RLE?
                jz     T10_24Raw

                                                ;RLE packet
                and    cl, 01111111b            ;ecx = counter
                inc    ecx
                add    edx, ecx
                mov    al, [esi + 1]            ;= B
                mov    ah, [esi + 2]            ;= G
                mov    bl, [esi + 3]            ;= R

T10_RLELoop24:
                mov    [edi + 0], al            ;= B
                mov    [edi + 1], ah            ;= G
                mov    [edi + 2], bl            ;= R

                add    edi, 3
                dec    ecx
                jnz    T10_RLELoop24

                add    esi, 4

                cmp    edx, ebp                 ;Are we done?
                jb     Type10_24
                jmp    Done


T10_24Raw:
                inc    esi
                inc    ecx
                add    edx, ecx
T10_RawLoop24:                                  ;Copy raw block
                mov    al, [esi + 0]            ;B
                mov    ah, [esi + 1]            ;G
                mov    bl, [esi + 2]            ;R
                mov    [edi + 0], al            ;= B
                mov    [edi + 1], ah            ;= G
                mov    [edi + 2], bl            ;= R

                add    edi, 3
                add    esi, 3
                dec    ecx
                jnz    T10_RawLoop24

                cmp    edx, ebp
                jb     Type10_24


Done:


;--- Check if picture is upside-down...
                mov    ebp, FileBuf
                test   [ebp].IDescriptor, 0100000b
                jnz    NoFlip

                mov    ecx, TGA_Bits
                shr    ecx, 3
                imul   ecx, TGA_Width           ;Number of bytes each scanline

                mov    eax, TGA_Bits
                shr    eax, 3
                imul   eax, TGA_Width
                imul   eax, TGA_Heigth

                mov    esi, OutPut
                add    esi, eax                 ;esi = end of picture
                sub    esi, ecx
                mov    edi, OutPut

                mov    edx, TGA_Heigth          ;Number of scanlines
                shr    edx, 1

FlipLoop:
                push   esi
                push   edi
                push   ecx
ScanLine:                                       ;Flip scanlines...
                mov    al, [esi]
                mov    bl, [edi]
                mov    [esi], bl
                mov    [edi], al
                inc    esi
                inc    edi
                dec    ecx
                jnz    ScanLine

                pop    ecx
                pop    edi
                pop    esi

                add    edi, ecx
                sub    esi, ecx
                dec    edx
                jnz    FlipLoop


NoFlip:


;--- Now we're done...
                mov    edx, FileBuf
                call   MEM_DeAlloc
                jc     Error7
                mov    edi, OutPut

                clc
                ret
TGA_Load      ENDP


ErrStr1         DB 'TGA: Error opening file', 0Dh, 0Ah, '$'
ErrStr3         DB 'TGA: Error moving filepointer', 0Dh, 0Ah, '$'
ErrStr4         DB 'TGA: Error reading file', 0Dh, 0Ah, '$'
ErrStr5         DB 'TGA: Error closing file', 0Dh, 0Ah, '$'
ErrStr6         DB 'TGA: No support for such TGA-type', 0Dh, 0Ah, '$'
ErrStr8         DB 'TGA: 32-bit TGA-pictures not supported', 0Dh, 0Ah, '$'

Error1:
                mov      SYS_QuitMsg, OFFSET ErrStr1
                stc
                ret
Error2:
                mov    ah, 3Eh                  ;Close filehandle
                mov    bx, Handle               ;Load filehandle
                int    21h
                jc     Error5
                stc
                ret
Error3:
                mov    ah, 3Eh                  ;Close filehandle
                mov    bx, Handle               ;Load filehandle
                int    21h
                jc     Error5
                mov    SYS_QuitMsg, OFFSET ErrStr3
                stc
                ret
Error4:
                mov    ah, 3Eh                  ;Close filehandle
                mov    bx, Handle               ;Load filehandle
                int    21h
                jc     Error5
                mov    SYS_QuitMsg, OFFSET ErrStr4
                stc
                ret
Error5:
                mov    SYS_QuitMsg, OFFSET ErrStr5
                stc
                ret
Error6:
                mov    SYS_QuitMsg, OFFSET ErrStr6
                stc
                ret
Error7:
                stc
                ret
Error8:
                mov    SYS_QuitMsg, OFFSET ErrStr8
                stc
                ret



END
