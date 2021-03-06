        .MODEL  small, pascal

        .DATA

        .CODE

;* GetVer - Gets DOS version.
;*
;* Shows:   DOS Function - 30h (Get MS-DOS Version Number)
;*
;* Params:  None
;*
;* Return:  Short integer of form (M*100)+m, where M is major
;*          version number and m is minor version, or integer
;*          is 0 if DOS version earlier than 2.0

GetVer  PROC

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

GetVer  ENDP

        END
