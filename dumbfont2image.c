#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned char unisig[32] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont16\x00";

static size_t pixelscap;
static unsigned char *pixels;
static size_t ncodepoint;
static size_t headersize;
static unsigned char header[18];
static size_t obsize;
static unsigned char *ob;
static size_t width;
static size_t height;

static void die(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

static void read_unisig(void) {
  unsigned char buf[sizeof(unisig)];

  if (1 != fread(buf, sizeof(unisig), 1, stdin)) {
    die("cannot read unisig");
  }
  if (memcmp(buf, unisig, sizeof(unisig))) {
    die("bad unisig");
  }
}

static void read_character_images(void) {
  uint16_t rows[16];
  size_t nr;

  while ((nr = fread(rows, 1, 32, stdin)) == 32) {
    ncodepoint++;
    if (!pixelscap) {
      pixelscap = 1024;
    }
    while (pixelscap && (pixelscap < ncodepoint * 32)) {
      pixelscap *= 2;
    }
    if (!(pixels = realloc(pixels, pixelscap))) {
      die("out of memory");
    }
    memcpy(pixels + 32 * (ncodepoint - 1), rows, 32);
  }
  if (ferror(stdin)) {
    die("cannot read from stdin");
  }
  if (nr) {
    die("junk data at the end of stdin");
  }
  width = 16 * ncodepoint;
  height = 16;
}

static void out_farbfeld(void) {
  unsigned char *src;
  uint16_t *dst;
  size_t codepoint, y, x;
  uint16_t rowbits, intensity;

  headersize = 16;
  memcpy(header, "farbfeld", 8);
  header[10] = width >> 8;
  header[11] = width;
  header[14] = height >> 8;
  header[15] = height;
  obsize = ncodepoint * 16 * 16 * 8;
  if (!(ob = malloc(obsize))) {
    die("out of memory");
  }
  src = pixels;
  for (codepoint = 0; codepoint < ncodepoint; codepoint++) {
    for (y = 0; y < 16; y++) {
      dst = (uint16_t *)(ob + 8 * (16 * ncodepoint * y + 16 * codepoint));
      rowbits = *src++;
      rowbits |= *src++ << 8;
      for (x = 0; x < 16; x++) {
        intensity = (rowbits & 1) ? 65535 : 0;
        *dst++ = intensity;
        *dst++ = intensity;
        *dst++ = intensity;
        *dst++ = 65535;
        rowbits >>= 1;
      }
    }
  }
}

static void out_tga(void) {
  unsigned char *src;
  unsigned char *dst;
  size_t codepoint, y, x;
  uint16_t rowbits, intensity;

  headersize = 18;
  header[2] = 2;
  header[12] = width;
  header[13] = width >> 8;
  header[14] = height;
  header[15] = height >> 8;
  header[16] = 24;
  header[17] = 32;
  obsize = ncodepoint * 16 * 16 * 3;
  if (!(ob = malloc(obsize))) {
    die("out of memory");
  }
  src = pixels;
  for (codepoint = 0; codepoint < ncodepoint; codepoint++) {
    for (y = 0; y < 16; y++) {
      dst = ob + 3 * (16 * ncodepoint * y + 16 * codepoint);
      rowbits = *src++;
      rowbits |= *src++ << 8;
      for (x = 0; x < 16; x++) {
        intensity = (rowbits & 1) ? 65535 : 0;
        *dst++ = intensity;
        *dst++ = intensity;
        *dst++ = intensity;
        rowbits >>= 1;
      }
    }
  }
}

static void (*out)(void);

static void usage(void) {
  fprintf(stderr, "usage: dumbfont2image farbfeld|tga\n");
  exit(1);
}

int main(int argc, char **argv) {
  const char *format;

  if (argc != 2) {
    usage();
  }
  format = argv[1];
  if (!strcmp(format, "farbfeld")) {
    out = out_farbfeld;
  } else if (!strcmp(format, "tga")) {
    out = out_tga;
  } else {
    usage();
  }
  read_unisig();
  read_character_images();
  out();
  if ((1 != fwrite(header, headersize, 1, stdout)) ||
      (1 != fwrite(ob, obsize, 1, stdout))) {
    die("cannot write to stdout");
  }
  return 0;
}
