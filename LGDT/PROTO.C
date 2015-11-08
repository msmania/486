#include <stdio.h>
#include <dos.h>
#include <malloc.h>
#include <process.h>

#define TypeCode        0x9a
#define TypeData        0x92
#define TypeStack       0x96
#define TypeLDT         0x82
#define TypeTSS         0x89
#define TypeTSSBusy     0x8b
#define TypeCallGate    0x84
#define TypeIntrGate    0x8e
#define TypeTrapGate    0x8f
#define TypeTaskGate    0x85

#define BigSeg      0x80
#define SmallSeg    0x00

#define Code386 0x40
#define Data386 0x40
#define Code286 0x40

typedef struct _SegDesc {
    unsigned short limitL;
    unsigned short baseL;
    unsigned char baseM;
    unsigned char type;
    unsigned char limitH;
    unsigned char baseH;
} SegDesc;

typedef struct _DescTblPtr {
    unsigned short limit;
    unsigned long base;
} DescTblPtr;

#define GDTNUM 16

SegDesc gdt[GDTNUM];
DescTblPtr gdtptr;
unsigned int gdtfre = 0x30 / 8 + 1; // 0x30 is the size of descriptor table
struct SREGS seg;

void lgdt(DescTblPtr *gdtptr);

//void InitDisp();
//void DispStr(int x, int y, char *str);
//void EnableA20();

void MakeSegDesc(SegDesc *desc,
                 unsigned long addr,
                 unsigned long limit,
                 unsigned char segtype,
                 unsigned char seg32type,
                 unsigned char dpl) {
    desc->baseL = (unsigned int)(addr & 0xffff);
    desc->baseM = (unsigned int)((addr >> 16) & 0xff);
    desc->baseH = (unsigned int)((addr >> 24) & 0xff);
    desc->limitL = (unsigned int)(limit & 0xffff);
    desc->limitH = (unsigned int)((addr >> 16) & 0x0f) + seg32type;
    desc->type = segtype | (dpl << 5);
}

// Make this function separated from MakeSegDesc so that
// we can configure Desc per task in the future
void SetSegDesc(unsigned short sel,
                unsigned long addr,
                unsigned long limit,
                unsigned char segtype,
                unsigned char seg32type,
                unsigned char dpl) {
    MakeSegDesc(
        &gdt[sel >> 3],
        addr,
        limit,
        segtype,
        seg32type,
        dpl);
}

unsigned long SegToLinear(unsigned short seg,
                          unsigned short off) {
    return ((unsigned long)seg << 4) + off;
}

void SetUpGDT() {
    unsigned short codesize, datasize, stacksize;
    DescTblPtr gdtptr;

    segread(&seg);
    codesize = (seg.ds - seg.cs) * 16;
    datasize = 0xffff;
    stacksize = 0;

    SetSegDesc(0x08, SegToLinear(seg.cs, 0), codesize, TypeCode, SmallSeg, 0);
    SetSegDesc(0x10, SegToLinear(seg.ds, 0), datasize, TypeData, SmallSeg, 0);
    SetSegDesc(0x18, SegToLinear(seg.ss, 0), stacksize, TypeStack, SmallSeg, 0);
    SetSegDesc(0x20, SegToLinear(seg.cs, 0), 0xffff, TypeCode, SmallSeg, 0);
    SetSegDesc(0x28, SegToLinear(seg.ds, 0), 0xffff, TypeData, SmallSeg, 0);
    SetSegDesc(0x30, 0L, 0xffff, TypeData, BigSeg, 3);

    gdtptr.limit = GDTNUM * sizeof(SegDesc) - 1;
    gdtptr.base = SegToLinear(seg.ds, (unsigned short)gdt);
    lgdt(&gdtptr);
}

unsigned int AllocSel() {
    if (gdtfre >= GDTNUM) {
        fprintf(stderr, "Can't allocate selector\n");
        exit(1);
    }

    return (gdtfre++) * 8;
}

void PrintSegDesc(SegDesc *desc) {
    unsigned long addr;
    unsigned long limit;
    unsigned int g, b, avl, p, dpl, s, type, a;

    addr = ((unsigned long)desc->baseH << 24)
        + ((unsigned long)desc->baseM << 16)
        + desc->baseL;
    limit = ((unsigned long)(desc->limitH & 0xf) << 16)
        + desc->limitL;

    g    = desc->limitH & 0x80;
    b    = desc->limitH & 0x40;
    avl  = desc->limitH & 0x10;
    p    = desc->type & 0x80;
    dpl  = (desc->type & 0x60) >> 5;
    s    = desc->type & 0x10;
    type = (desc->type & 0x0e) >> 1;
    a     = desc->type & 0x01;

    printf("Base=%08lX Lim=%05lX", addr, limit);
    printf(g ? " G" : "  ");
    printf(b ? " B" : "  ");
    printf(avl ? " avl" : "    ");
    printf(p ? " P" : "  ");
    printf(" dpl:%d", dpl);
    printf(s ? " seg" : "    ");
    switch(type) {
    case 0: printf(" RO-Data "); break;
    case 1: printf(" RW-Data "); break;
    case 2: printf(" RO-Stack"); break;
    case 3: printf(" RW-Stack"); break;
    case 4: printf(" EO-Code "); break;
    case 5: printf(" ER-Code "); break;
    case 6: printf(" RO-Conf "); break;
    case 7: printf(" RW-Conf "); break;
    }
    printf(a ? " A" : "  ");
    printf("\n");
}

void PrintGDTSegDesc(unsigned short sel) {
    printf("%04X: ", sel);
    PrintSegDesc(gdt + sel / 8);
}
