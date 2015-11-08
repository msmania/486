#include <stdlib.h>
#include <stdio.h>

extern short _pascal GetVer();

void main(int argc, char **argv) {
    printf("Hello, MS-DOS%d!\n", GetVer());
    exit(0);
}
