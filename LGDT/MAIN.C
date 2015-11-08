#include <stdlib.h>
#include <stdio.h>

void RealToProto();
void ProtoToReal();
void SetUpGDT();
void PrintGDTSegDesc(unsigned short sel);

void main(int argc, char **argv) {
    int i, val;

    SetUpGDT();

    printf("[Before]\n");
    for (i=8 ; i<=0x30 ; i+=8) {
        PrintGDTSegDesc(i);
    }

    val = 0;
    printf("val:%d\n", val);

    RealToProto();
    val=1;
    ProtoToReal();

    printf("[After]\n");
    printf("val:%d\n", val);
    for (i=8 ; i<=0x30 ; i+=8) {
        PrintGDTSegDesc(i);
    }

    exit(0);
}
