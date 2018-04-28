#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dumbfont.h"

static unsigned char bytes[128 * 32];
static const size_t nchr = 128;
static const char *fontlabel;
static const char *endlabel;

static void
die(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

static void
read_unisig(void)
{
    unsigned char buf[sizeof(unisig16)];

    if (1 != fread(buf, sizeof(unisig16), 1, stdin)) {
        die("cannot read signature");
    }
    if (memcmp(buf, unisig16, sizeof(unisig16))) {
        die("bad signature");
    }
}

static void
hexlines(size_t chr, const char *before, const char *between, const char *after)
{
    size_t ln, col;

    for (ln = 0; ln < 4; ln++) {
        printf("%s", before);
        for (col = 0; col < 8; col++) {
            if (col) {
                printf("%s", between);
            }
            printf("%02x", (unsigned int)bytes[(32 * chr) + (8 * ln) + col]);
        }
        printf("%s\n", after);
    }
}

static void
out_as(void)
{
    size_t chr;

    printf(".globl %s\n", fontlabel);
    printf(".section \".rodata\"\n");
    printf("%s:\n", fontlabel);
    for (chr = 0; chr < nchr; chr++) {
        printf(";\n");
        hexlines(chr, ".byte 0x", ",0x", "");
    }
    if (endlabel) {
        printf("%s:\n", endlabel);
    }
}

static void
out_c(void)
{
    size_t chr;

    if (endlabel) {
        die("c doesn't support an endlabel");
    }
    printf("unsigned char %s[%zu] = {\n", fontlabel, 32 * nchr);
    for (chr = 0; chr < nchr; chr++) {
        printf("\n");
        hexlines(chr, "0x", ",0x", ",");
    }
    printf("\n};\n");
}

static void
out_go(void)
{
    size_t chr;

    if (endlabel) {
        die("go doesn't support an endlabel");
    }
    printf("package main\n\n");
    printf("const %s = \n", fontlabel);
    for (chr = 0; chr < nchr; chr++) {
        hexlines(chr, "\t\"\\x", "\\x", "\"+");
    }
    printf("\t\"\"\n");
}

static void
out_nasm(void)
{
    size_t chr;

    printf("global %s\n", fontlabel);
    printf("section .rodata\n");
    printf("%s:\n", fontlabel);
    for (chr = 0; chr < nchr; chr++) {
        printf(";\n");
        hexlines(chr, "db 0x", ",0x", "");
    }
    if (endlabel) {
        printf("%s:\n", endlabel);
    }
}

static void
usage(void)
{
    fprintf(stderr,
        "usage: dumbfont2include as|c|go|nasm <fontlabel> [<endlabel>]\n");
    exit(1);
}

int
main(int argc, char **argv)
{
    const char *syntax;
    void (*out)(void);

    if (argc == 3) {
        syntax = argv[1];
        fontlabel = argv[2];
    } else if (argc == 4) {
        syntax = argv[1];
        fontlabel = argv[2];
        endlabel = argv[3];
    } else {
        usage();
    }
    out = 0;
    if (!strcmp(syntax, "as")) {
        out = out_as;
    } else if (!strcmp(syntax, "c")) {
        out = out_c;
    } else if (!strcmp(syntax, "go")) {
        out = out_go;
    } else if (!strcmp(syntax, "nasm")) {
        out = out_nasm;
    } else {
        usage();
    }
    read_unisig();
    if (1 != fread(bytes, sizeof(bytes), 1, stdin)) {
        die("cannot read from glyphs");
    }
    out();
    return 0;
}
