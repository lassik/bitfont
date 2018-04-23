#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned char unisig[32] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont16\x00";

static unsigned char bytes[128 * 32];
static const size_t nchr = 128;
static const char *fontlabel;
static const char *endlabel;

static void die(void) {
  fprintf(stderr, "fail\n");
  exit(1);
}

static void read_unisig(void) {
  unsigned char buf[sizeof(unisig)];

  if (1 != fread(buf, sizeof(unisig), 1, stdin)) {
    die();
  }
  if (memcmp(buf, unisig, sizeof(unisig))) {
    die();
  }
}

static void hexlines(size_t chr, const char *lineprefix,
                     const char *hexprefix) {
  size_t ln, col;

  for (ln = 0; ln < 4; ln++) {
    printf("%s ", lineprefix);
    for (col = 0; col < 8; col++) {
      printf("%s%02x", hexprefix,
             (unsigned int)bytes[(32 * chr) + (8 * ln) + col]);
      if (col < 7) {
        printf(",");
      } else {
        printf("\n");
      }
    }
  }
}

static void out_gnu(void) {
  size_t chr;

  printf(".globl %s\n", fontlabel);
  printf(".section \".rodata\"\n");
  printf("%s:\n", fontlabel);
  for (chr = 0; chr < nchr; chr++) {
    printf(";\n");
    hexlines(chr, ".byte", "0x");
  }
  if (endlabel) {
    printf("%s:\n", endlabel);
  }
}

static void out_nasm(void) {
  size_t chr;

  printf("global %s\n", fontlabel);
  printf("section .rodata\n");
  printf("%s:\n", fontlabel);
  for (chr = 0; chr < nchr; chr++) {
    printf(";\n");
    hexlines(chr, "db", "0x");
  }
  if (endlabel) {
    printf("%s:\n", endlabel);
  }
}

static void (*out)(void);

static void usage(void) {
  fprintf(stderr, "usage: dumbfont2asm gnu|nasm <fontlabel> [<endlabel>]\n");
  exit(1);
}

int main(int argc, char **argv) {
  const char *syntax;

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
  if (!strcmp(syntax, "gnu")) {
    out = out_gnu;
  } else if (!strcmp(syntax, "nasm")) {
    out = out_nasm;
  } else {
    usage();
  }
  read_unisig();
  if (1 != fread(bytes, sizeof(bytes), 1, stdin)) {
    die();
  }
  out();
  return 0;
}
