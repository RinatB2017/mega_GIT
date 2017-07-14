SECTION .data
msg     db "Hello, world!",0xa  
len     equ $ - msg
SECTION .text
global _start           ; the program entry point
_start:
        mov eax, 4      ; 'write' syscall
        mov ebx, 1      ; file descr. 1 (stdout)
        mov ecx, msg    ; pointer to the data
        mov edx, len    ; amount of data
        int 0x80        ; call to the kernel
        mov eax, 1      ; '_exit' syscall
        mov ebx, 0      ; zero exit code (success)
        int 0x80        ; call to the kernel
