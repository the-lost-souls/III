;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : SORT.ASM
;³Author       : Cyberfish/TLS
;³Last update  : 10.02.1998
;³Action       :
;³
;³Notes :
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
.386
.Model flat, C
.code

include w:\general\var.inc

public SRT_OReset
public SRT_RReset
public SRT_OSort
public SRT_RSort
public SRT_ORun
public SRT_RRun



;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : SRT_RReset
;³ID           : SRT
;³Type         : Procedure
;³Last update  : 10.02.1998
;³Action       : Resets the parttables and saves end-part
;³Optimized    : -
;³
;³Input variables : None
;³
;³Notes : MUST be called before SRT_Sort
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


SRT_OReset    DD C_SortReset                    ;Reset part-tables
              DD -1                             ;No plugins


EndPartT      DD 0000                           ;Offset to next
              DD 0000                           ;
              DD 0000                           ;PartT 0 = end!
	      DD 0000				;Priority (run last!)

SRT_RReset    PROC
                call   Reset1
                call   Reset2

;---- Setup end-part
                mov    eax, SRT_Start1
                mov    d[eax + 255*4], OFFSET EndPartT
                mov    eax, SRT_End1
                mov    d[eax + 255*4], OFFSET EndPartT

                ret
SRT_RReset    ENDP



;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : SRT_RSort
;³ID           : SRT
;³Type         : Procedure
;³Last update  : 10.02.1998
;³Action       : Sorts all parts saved in list 1
;³Optimized    : -
;³
;³Input variables : None
;³
;³Notes : Values are sorted by byte 1, 2 and 3 (bit 8-31). After finished
;³sorting, the parts can be read from list 2.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

SRT_OSort     DD C_Sort                         ;Reset part-tables
              DD -1                             ;No plugins

SRT_RSort     PROC
;---- Sorting second byte from 1 -> 2, (bit 8-15)
                mov    ebx, -1
                mov    edx, SRT_Start1
                mov    ebp, SRT_End2

Radix1:
                inc    ebx
                mov    esi, [edx + ebx*4]   ;Load start of list1
                cmp    esi, 0FFFFFFFFh
                je     Radix1

                mov    edi, SRT_End1
                mov    edi, [edi + ebx*4]       ;edi = endoffset of list

DoRadix1:
                mov    eax, [esi].F_SortValue
                shr    eax, 8
                and    eax, 255
                mov    ecx, 255
                sub    ecx, eax

                mov    eax, [ebp + ecx*4]
                mov    [eax], esi
                mov    [ebp + ecx*4], esi

                cmp    esi, edi                 ;End of list?
                mov    esi, [esi]               ;Next part
                jne    DoRadix1

                cmp    ebx, 255
                jb     Radix1

                call   Reset1

;---- Sorting second byte from 2 -> 1, (bit 8-15)
                mov    ebx, -1
                mov    edx, SRT_Start2
                mov    ebp, SRT_End1

Radix2:
                inc    ebx
                mov    esi, [edx + ebx*4]       ;Load start of list1
                cmp    esi, 0FFFFFFFFh
                je     Radix2

                mov    edi, SRT_End2
                mov    edi, [edi + ebx*4]       ;edi = endoffset of list

DoRadix2:
                mov    eax, [esi].F_SortValue
                shr    eax, 16
                and    eax, 255
                mov    ecx, 255
                sub    ecx, eax

                mov    eax, [ebp + ecx*4]
                mov    [eax], esi
                mov    [ebp + ecx*4], esi

                cmp    esi, edi                 ;End of list?
                mov    esi, [esi]               ;Next part
                jne    DoRadix2

                cmp    ebx, 255
                jb     Radix2

                call   Reset2


;---- Sorting second byte from 1->2 (bit 16-31)
                mov    ebx, -1
                mov    edx, SRT_Start1
                mov    ebp, SRT_End2

Radix3:
                inc    ebx
                mov    esi, [edx + ebx*4]       ;Load start of list1
                cmp    esi, 0FFFFFFFFh
                je     Radix3

                mov    edi, SRT_End1
                mov    edi, [edi + ebx*4]       ;edi = endoffset of list

DoRadix3:
                mov    eax, [esi].F_SortValue
                shr    eax, 24
                and    eax, 255
                mov    ecx, 255
                sub    ecx, eax

                mov    eax, [ebp + ecx*4]
                mov    [eax], esi
                mov    [ebp + ecx*4], esi

                cmp    esi, edi                 ;End of list?
                mov    esi, [esi]               ;Next part
                jne    DoRadix3

                cmp    ebx, 255
                jb     Radix3

		call	Reset1

;---- Sorting second byte from 2 -> 1, (bit 8-15)
                mov    ebx, -1
                mov    edx, SRT_Start2
                mov    ebp, SRT_End1

Priority:
                inc    ebx
                mov    esi, [edx + ebx*4]       ;Load start of list1
                cmp    esi, 0FFFFFFFFh
                je     Priority

                mov    edi, SRT_End2
                mov    edi, [edi + ebx*4]       ;edi = endoffset of list

DoPriority:
                mov    eax, [esi].F_Priority
		mov	ecx, 255
		sub	ecx, eax

                mov    eax, [ebp + ecx*4]
                mov    [eax], esi
                mov    [ebp + ecx*4], esi

                cmp    esi, edi                 ;End of list?
                mov    esi, [esi]               ;Next part
                jne    DoPriority

                cmp    ebx, 255
                jb     Priority

                ret
SRT_RSort     ENDP


;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : SRT_Run
;³ID           : SRT
;³Type         : Procedure
;³Last update  : 10.02.1998
;³Action       : Runs all the parts
;³Optimized    : -
;³
;³Input variables : None
;³
;³Notes : The loop stops when it reaches the EndMark-part (type=0)
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

SRT_ORun      DD C_Run                          ;Reset part-tables
              DD -1                             ;No plugins

SRT_RRun      PROC
                mov    ebx, -1
                mov    edx, SRT_Start1

SearchLists:                                    ;Searching for non-empty lists
                inc    ebx
                mov    esi, [edx + ebx*4]       ;Load start of list
                cmp    esi, 0FFFFFFFFh
                je     SearchLists

RunLoop:
                mov    eax, [esi].F_Type        ;Load Parttype
                or     eax, eax
                jz     DoneParts

                mov    eax, SYS_PRoutines[eax*4]

                push   esi
                push   ebx

                call   eax

                pop    ebx
                pop    esi

                mov    edi, SRT_End1
                mov    edi, [edi + ebx*4]
                cmp    esi, edi
                mov    esi, [esi]
                jne    RunLoop

                mov    edx, SRT_Start1

                cmp    ebx, 255
                jb     SearchLists

DoneParts:
                ret
SRT_RRun      ENDP


Reset1        PROC
                mov    eax, SRT_Start1
                add    eax, 1020
                mov    ecx, 256
                mov    ebx, SRT_Start1
                mov    edx, SRT_End1

Reset1Loop:
                dec    ecx

                mov    d[edx + ecx*4], eax      ;Set end=start
                mov    d[ebx + ecx*4], -1       ;Set "nextpoly" = -1

                sub    eax, 4

                or     ecx, ecx
                jnz    Reset1Loop

                ret
Reset1        ENDP

Reset2        PROC
                mov    eax, SRT_Start2
                add    eax, 1020
                mov    ecx, 256
                mov    ebx, SRT_Start2
                mov    edx, SRT_End2
Reset2Loop:
                dec    ecx

                mov    [edx + ecx*4], eax       ;Set end=start
                mov    d[ebx + ecx*4], -1       ;Set "nextpoly" = -1

                sub    eax, 4

                or     ecx, ecx
                jnz    Reset2Loop

                ret
Reset2        ENDP



END
