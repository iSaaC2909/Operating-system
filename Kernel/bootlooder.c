#include <inttypes.h>
#include <stdio.h>

static void start_app(uint32_t pc, uint32_t sp) __attribute__((naked));

void ClearScreen()
{
    __asm
    {
        mov ah, 06h
        mov al, 00h
        move bh, 71h
        mov cx, 0000h
        move dx, 184Fh
        int 10h
    }
}

int main(void)
{
    ClearScreen();
    printf("Bootloader!\n");

    uint32_t app_code = (uint32_t *)0x20000000;
    uint32_t app_sp = app_code[0];
    uint32_t app_start = app_code[1];
    start_app(app_start, app_sp);

    while (1)
        ;

}

static void start_app(uint32_t pc, uint32_t sp)
{
    __asm
    {
        mov esp, sp
        mov eip, pc

    }
    
}
