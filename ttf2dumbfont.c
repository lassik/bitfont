#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define PROGNAME "ttf2dumbfont"
#define NCODEPOINT 256
#define MAXIMAGEBYTESIZE (64 * 64 / 8)
#define UNISIGSIZE 32

static const unsigned char unisig8[UNISIGSIZE] =
    "\xffUnisig\x00\x0a\x0d\x0a\x12io.lassi.dumbfont8\x00\x00";

static const unsigned char unisig16[UNISIGSIZE] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont16\x00";

static const unsigned char unisig32[UNISIGSIZE] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont32\x00";

static const unsigned char unisig64[UNISIGSIZE] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont64\x00";

static unsigned char pixels[NCODEPOINT * MAXIMAGEBYTESIZE];

static void die(const char *msg) {
  fprintf(stderr, "%s: %s\n", PROGNAME, msg);
  exit(1);
}

static void usage(void) {
  fprintf(stderr, "usage: %s %s\n", PROGNAME,
          "<ttf-file> 8|16|32|64 <baseline>");
  exit(1);
}

int main(int argc, char **argv) {
  static unsigned char imagebytes[MAXIMAGEBYTESIZE];
  const char *ttf_file;
  const unsigned char *unisig;
  FT_Library library;
  FT_Face face;
  FT_GlyphSlot glyph;
  FT_Bitmap *bitmap;
  size_t imagebytesize;
  int side, baseline, codepoint, yphys, xphys, y, x;
  unsigned int ibyte, ibit;
  int bestbaseline, leastmissing;

  if (argc != 4) {
    usage();
  }
  ttf_file = argv[1];
  switch ((side = atoi(argv[2]))) {
  case 8:
    unisig = unisig8;
    break;
  case 16:
    unisig = unisig16;
    break;
  case 32:
    unisig = unisig32;
    break;
  case 64:
    unisig = unisig64;
    break;
  default:
    usage();
  }
#if 0
  baseline = atoi(argv[3]);
  if ((baseline < 0) || (baseline >= side)) {
      usage();
  }
#endif
  imagebytesize = side * side / 8;
  if (FT_Init_FreeType(&library)) {
    die("FT_Init_FreeType");
  }
  if (FT_New_Face(library, ttf_file, 0, &face)) {
    die("FT_New_Face");
  }
  if (FT_Set_Pixel_Sizes(face, side, side)) {
    die("FT_Set_Pixel_Sizes");
  }
  bestbaseline = 0;
  leastmissing = 65535;
  for (baseline = side - 1; baseline >= 0; baseline--) {
    int missingpixels = 0;
    for (codepoint = 0; codepoint < NCODEPOINT; codepoint++) {
      if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER)) {
        die("FT_Load_Char");
      }
      glyph = face->glyph;
      bitmap = &glyph->bitmap;
      for (yphys = 0; yphys < (int)bitmap->rows; yphys++) {
        for (xphys = 0; xphys < (int)bitmap->width; xphys++) {
          if (bitmap->buffer[yphys * bitmap->pitch + xphys]) {
            x = xphys + (int)glyph->bitmap_left;
            y = yphys + side - baseline - (int)glyph->bitmap_top;
            if (!((x >= 0) && (x < side) && (y >= 0) && (y < side))) {
              missingpixels++;
            }
          }
        }
      }
    }
    if (leastmissing > missingpixels) {
      leastmissing = missingpixels;
      bestbaseline = baseline;
    }
  }
  baseline = bestbaseline;
  fprintf(stderr, "automatically selected baseline %d\n", baseline);
  for (codepoint = 0; codepoint < NCODEPOINT; codepoint++) {
    if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER)) {
      die("FT_Load_Char");
    }
    glyph = face->glyph;
    bitmap = &glyph->bitmap;
    memset(imagebytes, 0, sizeof(imagebytes));
    for (yphys = 0; yphys < (int)bitmap->rows; yphys++) {
      for (xphys = 0; xphys < (int)bitmap->width; xphys++) {
        if (bitmap->buffer[yphys * bitmap->pitch + xphys]) {
          x = xphys + (int)glyph->bitmap_left;
          y = yphys + side - baseline - (int)glyph->bitmap_top;
          if ((x >= 0) && (x < side) && (y >= 0) && (y < side)) {
            ibyte = y * (side / 8) + (x / 8);
            ibit = x % 8;
            imagebytes[ibyte] |= 1 << ibit;
          }
        }
      }
    }
    memcpy(pixels + imagebytesize * codepoint, imagebytes, imagebytesize);
  }
  if ((1 != fwrite(unisig, UNISIGSIZE, 1, stdout)) ||
      (1 != fwrite(pixels, NCODEPOINT * imagebytesize, 1, stdout))) {
    die("cannot write to stdout");
  }
  return 0;
}
