#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define NCODEPOINT 256
#define BASELINE 2

static const unsigned char unisig[32] =
    "\xffUnisig\x00\x0a\x0d\x0a\x13io.lassi.dumbfont16\x00";

static uint16_t pixels[NCODEPOINT * 16];

static void die(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

int main(int argc, char **argv) {
  static uint16_t rowbits[16];
  const char *ttf_file;
  FT_Library library;
  FT_Face face;
  FT_Bitmap *bitmap;
  int codepoint, x, y, realx, realy;

  if (argc != 2) {
    die("usage: ttf2dumbfont <ttf-file>");
  }
  ttf_file = argv[1];
  if (FT_Init_FreeType(&library)) {
    die("FT_Init_FreeType");
  }
  if (FT_New_Face(library, ttf_file, 0, &face)) {
    die("FT_New_Face");
  }
  if (FT_Set_Pixel_Sizes(face, 16, 16)) {
    die("FT_Set_Pixel_Sizes");
  }
  for (codepoint = 0; codepoint < NCODEPOINT; codepoint++) {
    memset(rowbits, 0, sizeof(rowbits));
    if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER)) {
      die("FT_Load_Char");
    }
    bitmap = &face->glyph->bitmap;
    for (y = 0; y < (int)bitmap->rows; y++) {
      for (x = 0; x < (int)bitmap->width; x++) {
        if (bitmap->buffer[y * bitmap->pitch + x]) {
          realx = x + (int)face->glyph->bitmap_left;
          realy = y + 16 - BASELINE - (int)face->glyph->bitmap_top;
          if ((realx >= 0) && (realx < 16) && (realy >= 0) && (realy < 16)) {
            rowbits[realy] |= 1 << realx;
          }
        }
      }
    }
    for (y = 0; y < 16; y++) {
      pixels[(16 * codepoint) + y] = rowbits[y];
    }
  }
  if ((1 != fwrite(unisig, sizeof(unisig), 1, stdout)) ||
      (1 != fwrite(pixels, sizeof(pixels), 1, stdout))) {
    die("cannot write to stdout");
  }
  return 0;
}
