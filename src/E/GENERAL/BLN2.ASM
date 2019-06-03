.386
.model flat, C
.code

include w:\general\var.inc

public BLN2


BLN2_Data     STRUC

BLN2_Type	DD ?
BLN2_Plugins	DD 16 DUP (?)
BLN2_Picture	DD ?

	      ENDS


BLN2	      PROC
		pushad

		mov	esi, SYS_OOffs
		mov	eax, SYS_XRes
		shl	eax, 2
		mov	esi, [esi].BLN2_Picture
		add	esi, eax

		mov	edi, SYS_XRes
		shl	edi, 2

		mov	ebp, SYS_XRes
		shl	ebp, 2
		neg	ebp

		mov	edx, SYS_YRes
		sub	edx, 2
	BlobYLoop:

		add	esi, 8
		mov	ecx, SYS_XRes
		sub	ecx, 2

	BlobXLoop:

	;---- B
		mov	al, [esi]
		shr	al, 3

		mov	bl, [esi + 4]
		mov	bh, [esi - 4]
		add	bl, al
		sbb	ah, ah
		or	bl, ah
		add	bh, al
		sbb	ah, ah
		or	bh, ah
		mov	[esi + 4], bl
		mov	[esi - 4], bh

		mov	bl, [esi + edi]
		mov	bh, [esi + ebp]
		add	bl, al
		sbb	ah, ah
		or	bl, ah
		add	bh, al
		sbb	ah, ah
		or	bh, ah
		mov	[esi + edi], bl
		mov	[esi + ebp], bh

	;---- G
		inc	esi
		mov	al, [esi]
		shr	al, 3

		mov	bl, [esi + 4]
		add	bl, al
		sbb	ah, ah
		or	bl, ah
		mov	[esi + 4], bl

		mov	bl, [esi - 4]
		add	bl, al
		sbb	ah, ah
		or	bl, ah
		mov	[esi - 4], bl

		mov	bl, [esi + edi]
		add	bl, al
		sbb	ah, ah
		or	bl, ah
		mov	[esi + edi], bl

		mov	bl, [esi + ebp]
		add	bl, al
		sbb	ah, ah
		or	bl, ah
		mov	[esi + ebp], bl

	;---- R
		inc	esi
		mov	al, [esi]
		shr	al, 3

		mov	bl, [esi + 4]
		add	bl, al
		sbb	ah, ah
		or	bl, ah
		mov	[esi + 4], bl

		mov	bl, [esi - 4]
		add	bl, al
		sbb	ah, ah
		or	bl, ah
		mov	[esi - 4], bl

		mov	bl, [esi + edi]
		add	bl, al
		sbb	ah, ah
		or	bl, ah
		mov	[esi + edi], bl

		mov	bl, [esi + ebp]
		add	bl, al
		sbb	ah, ah
		or	bl, ah
		mov	[esi + ebp], bl

		add	esi, 2
		dec	ecx
		jnz	BlobXLoop

		dec	edx
		jnz	BlobYLoop


;---- Done blobbing... Updating dbuf...
		mov	ecx, SYS_XRes
		imul	ecx, SYS_YRes

		mov	esi, SYS_OOffs
		mov	esi, [esi].BLN2_Picture
		mov	edi, SYS_DBufOffs

	BLN2_Loop:
		mov	al, [esi]
		mov	dl, [edi]

		mov	ah, [esi + 1]
		mov	bh, [edi + 1]

		mov	bl, [esi + 2]
		mov	dh, [edi + 2]

		add	al, dl
		sbb	dl, dl
		or	al, dl

		add	ah, bh
		sbb	bh, bh
		or	ah, bh

		add	bl, dh
		sbb	dh, dh
		or	bl, dh

		mov	[edi + 0], al
		mov	[edi + 1], ah
		mov	[edi + 2], bl

		add	esi, 4
		add	edi, 4
		dec	ecx
		jnz	BLN2_Loop

		popad

		ret
BLN2	      ENDP


END

