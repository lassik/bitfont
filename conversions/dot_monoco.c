#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned char chars[] = {
#include "dot_monoco.h"
};

static const unsigned char unisig[32] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont16\x00";

static unsigned char pixels[128 * 32];
static unsigned int ascii2offset[128];

static unsigned int reverse_byte(unsigned int byte) {
  unsigned int etyb, i;

  etyb = 0;
  for (i = 0; i < 8; i++) {
    etyb <<= 1;
    etyb |= byte & 1;
    byte >>= 1;
  }
  return etyb;
}

int main(void) {
  unsigned char *bytep;
  unsigned int ascii, offset, i;

  offset = 0;
  while (offset < sizeof(chars)) {
    ascii = chars[offset++];
    ascii2offset[ascii] = offset;
    offset += 32;
  }
  bytep = pixels;
  for (ascii = 0; ascii < 128; ascii++) {
    if ((offset = ascii2offset[ascii])) {
      for (i = 0; i < 32; i++) {
        *bytep++ = reverse_byte(~chars[offset++]);
      }
    } else {
      bytep += 32;
    }
  }
  if ((1 != fwrite(unisig, sizeof(unisig), 1, stdout)) ||
      (1 != fwrite(pixels, sizeof(pixels), 1, stdout))) {
    fprintf(stderr, "fail\n");
    exit(1);
  }
  return 0;
}
