#!/bin/sh
set -eux
CC=${CC:-clang}
CFLAGS=${CFLAGS:--Wall -Wextra -pedantic -std=c99 -g}
$CC $CFLAGS -o calc calc.c
$CC $CFLAGS -o dumbfont2asm dumbfont2asm.c
$CC $CFLAGS -o dumbfont2image dumbfont2image.c
$CC $CFLAGS -o ttf2dumbfont ttf2dumbfont.c $(freetype-config --cflags --libs)
