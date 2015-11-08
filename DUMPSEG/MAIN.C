#include <stdlib.h>
#include <stdio.h>
#include <dos.h>

void main(int argc, char **argv) {
    struct SREGS sreg;
    segread(&sreg);

    printf("cs:%04x ds:%04x es:%04x ss:%04x\n",
        sreg.cs,
        sreg.ds,
        sreg.es,
        sreg.ss);

    exit(0);
}
