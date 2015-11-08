.MODEL small
.386p
.code

_GetVer  PROC
    mov     ah, 30h
    int     21h
    .IF     al == 0
    sub     ax, ax
    .ELSE
    sub     ch, ch
    mov     cl, ah
    mov     bl, 100
    mul     bl
    add     ax, cx
    .ENDIF
    ret
_GetVer  ENDP

saveDS  dw  ?
saveES  dw  ?
saveSS  dw  ?

public _RealToProto
_RealToProto proc near
    push    bp
    mov     bp, sp
    ;
    mov     saveDS, ds
    mov     saveES, es
    mov     saveSS, ss
    ;
    cli
    ;
    mov     eax, cr0
    or      eax, 1
    mov     cr0, eax
    ;
    jmp     flush_q1
flush_q1:
    db      0eah
    dw      offset  set_cs_desc1
    dw      08h
set_cs_desc1:
    mov     ax, 10h
    mov     ds, ax
    mov     es, ax
    mov     ax, 18h
    mov     ss, ax
    ;
    pop     bp
    ret
_RealToProto endp

public _ProtoToReal
_ProtoToReal proc near
    push    bp
    mov     bp, sp
    ;
    mov     ax, 28h
    mov     ds, ax
    mov     es, ax
    mov     ss, ax
    ;
    db      0eah
    dw      offset  set_cs_desc2
    dw      20h
set_cs_desc2:
    mov     eax, cr0
    and     eax, 0fffffffeh
    mov     cr0, eax
    ;
    jmp     flush_q2
flush_q2:
    db      0eah
    dw      offset  set_cs_desc3
    dw      seg set_cs_desc3
set_cs_desc3:
    mov     ds, cs:saveDS
    mov     es, cs:saveES
    mov     ss, cs:saveSS
    ;
    pop     bp
    ret
_ProtoToReal endp

public _lgdt
_lgdt proc near
    push    bp
    mov     bp, sp
    mov     bx, [bp + 4]
    lgdt    fword ptr [bx]
    pop     bp
    ret
_lgdt endp

END
