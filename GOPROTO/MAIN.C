#include <stdlib.h>
#include <stdio.h>

extern short GetVer();
extern void RealToProto();
extern void ProtoToReal();

void main(int argc, char **argv) {
    printf("Hello, MS-DOS%d!\n", GetVer());
    RealToProto();
    ProtoToReal();
    printf("Successfully returned from Protected mode.\n");
    exit(0);
}
