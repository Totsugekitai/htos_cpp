        bits 64
        section .text

        global IoOut8           ; void IoOut8(uint16_t addr, uint8_t data);
IoOut8:
        mov dx,di
        mov ax,si
        out dx,al
        ret

        global IoIn8            ; uint8_t IoIn8(uint16_t addr);
IoIn8:
        mov dx,di
        in al,dx
        ret

        global IoOut32          ; void IoOut32(uint16_t addr, uint32_t data);
IoOut32:
        mov dx, di              ; dx = addr
        mov eax, esi            ; eax = data
        out dx, eax
        ret

        global IoIn32           ; uint32_t IoIn32(uint16_t addr);
IoIn32:
        mov dx, di              ; dx = addr
        in eax, dx
        ret

        global GetCS            ; uint16_t GetCS(void);
GetCS:
        xor eax, eax
        mov ax, cs
        ret

        global LoadIDT          ; void LoadIDT(uint16_t limit, uint64_t offset);
LoadIDT:
        push rbp
        mov rbp, rsp
        sub rsp, 10
        mov [rsp], di           ; limit
        mov [rsp + 2], rsi      ; offset
        lidt [rsp]
        mov rsp, rbp
        pop rbp
        ret

        global ReadIa32ApicBaseMsrLow ; uint32_t ReadIa32ApicBaseMsrLow();
ReadIa32ApicBaseMsrLow:
        mov ecx, 0x1b
        rdmsr
        ret

        global ReadIa32ApicBaseMsrHigh ; uint32_t ReadIa32ApicBaseMsrHigh();
ReadIa32ApicBaseMsrHigh:
        mov ecx, 0x1b
        rdmsr
        mov eax, edx
        ret

