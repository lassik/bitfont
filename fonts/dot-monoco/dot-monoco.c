#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned char chars[] = {
#include "dot-monoco.h"
};

static void die(void) {
  fprintf(stderr, "fail\n");
  exit(1);
}

static unsigned char out[128 * 32];

static const unsigned char unisig[32] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont16\x00";

static size_t ascii2offset[128];

static unsigned int reverse_byte(unsigned int byte) {
  unsigned int etyb = 0;
  for (size_t i = 0; i < 8; i++) {
    etyb <<= 1;
    etyb |= byte & 1;
    byte >>= 1;
  }
  return etyb;
}

int main(void) {
  unsigned char *ins;
  size_t ascii, offset, i;

  offset = 0;
  while (offset < sizeof(chars)) {
    size_t ascii = chars[offset++];
    ascii2offset[ascii] = offset;
    offset += 32;
  }

  memcpy(out, unisig, sizeof(unisig));
  ins = out;
  for (ascii = 0; ascii < 128; ascii++) {
    offset = ascii2offset[ascii];
    if (offset) {
      for (i = 0; i < 32; i += 2) {
        *ins++ = reverse_byte(~chars[offset + i + 0]);
        *ins++ = reverse_byte(~chars[offset + i + 1]);
      }
    } else {
      ins += 32;
    }
  }

  if ((1 != fwrite(unisig, sizeof(unisig), 1, stdout)) ||
      (1 != fwrite(out, sizeof(out), 1, stdout))) {
    die();
  }
  return 0;
}
