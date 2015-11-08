.MODEL small
.386p
.code

;* GetVer - Gets DOS version.
;*
;* Shows:   DOS Function - 30h (Get MS-DOS Version Number)
;*
;* Params:  None
;*
;* Return:  Short integer of form (M*100)+m, where M is major
;*          version number and m is minor version, or integer
;*          is 0 if DOS version earlier than 2.0

_GetVer  PROC

        mov     ah, 30h                 ; DOS Function 30h
        int     21h                     ; Get MS-DOS version number
        .IF     al == 0                 ; If version, version 1
        sub     ax, ax                  ; Set AX to 0
        .ELSE                           ; Version 2.0 or higher
        sub     ch, ch                  ; Zero CH and move minor
        mov     cl, ah                  ;   version number into CX
        mov     bl, 100
        mul     bl                      ; Multiply major by 10
        add     ax, cx                  ; Add minor to major*10
        .ENDIF
        ret                             ; Return result in AX

_GetVer  ENDP

public _RealToProto
_RealToProto    proc    near
                push bp
                mov bp, sp
                ;
                mov eax, cr0
                or eax, 1
                mov cr0, eax
                ;
                jmp flush_q1
flush_q1:
                pop bp
                ret
_RealToProto    endp

public _ProtoToReal
_ProtoToReal    proc    near
                push bp
                mov bp, sp
                ;
                mov eax, cr0
                and eax, 0fffffffeh
                mov cr0, eax
                ;
                jmp flush_q2
flush_q2:
                pop bp
                ret
_ProtoToReal    endp

        END
