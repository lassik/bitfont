#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dumbfont.h"

const unsigned char unisig8[UNISIGSIZE] =
    "\xffUnisig\x00\x0a\x0d\x0a\x12io.lassi.dumbfont8\x00\x00";

const unsigned char unisig16[UNISIGSIZE] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont16\x00";

const unsigned char unisig32[UNISIGSIZE] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont32\x00";

const unsigned char unisig64[UNISIGSIZE] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont64\x00";

void
die(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

unsigned int
read_unisig(void)
{
    unsigned char buf[UNISIGSIZE];

    if (1 != fread(buf, UNISIGSIZE, 1, stdin)) {
        die("cannot read unisig");
    }
    if (!memcmp(buf, unisig8, UNISIGSIZE)) {
        return 8;
    }
    if (!memcmp(buf, unisig16, UNISIGSIZE)) {
        return 16;
    }
    if (!memcmp(buf, unisig32, UNISIGSIZE)) {
        return 32;
    }
    if (!memcmp(buf, unisig64, UNISIGSIZE)) {
        return 64;
    }
    die("bad unisig");
    return 0;
}
