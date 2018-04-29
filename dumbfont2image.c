#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dumbfont.h"

static size_t pixelscap;
static size_t glyph_pixels;
static size_t codepoints_per_row = 8;
static unsigned char *pixels;
static size_t ncodepoint;
static size_t headersize;
static unsigned char header[0x36];
static size_t obsize;
static unsigned char *ob;
static size_t width;
static size_t height;

static unsigned int
div_roundup(unsigned int x, unsigned int y)
{
    return (x + y - 1) / y;
}

static void
read_character_images(void)
{
    unsigned char *glyph;
    size_t glyph_size, nr;

    glyph_size = glyph_pixels * glyph_pixels / 8;
    if (!(glyph = malloc(glyph_size))) {
        die("out of memory");
    }
    while ((nr = fread(glyph, 1, glyph_size, stdin)) == glyph_size) {
        ncodepoint++;
        if (!pixelscap) {
            pixelscap = 1024;
        }
        while (pixelscap && (pixelscap < ncodepoint * glyph_size)) {
            pixelscap *= 2;
        }
        if (!(pixels = realloc(pixels, pixelscap))) {
            die("out of memory");
        }
        memcpy(pixels + (ncodepoint - 1) * glyph_size, glyph, glyph_size);
    }
    if (ferror(stdin)) {
        die("cannot read from stdin");
    }
    if (nr) {
        die("junk data at the end of stdin");
    }
    width = glyph_pixels * codepoints_per_row;
    height = glyph_pixels * div_roundup(ncodepoint, codepoints_per_row);
}

static void
copy_pixels(size_t bytes_per_pixel)
{
    unsigned char *src;
    unsigned char *dst;
    size_t codepoint, rowbits, yy, y, xx, x, byte, b;

    obsize = bytes_per_pixel * width * height;
    if (!(ob = malloc(obsize))) {
        die("out of memory");
    }
    src = pixels;
    for (codepoint = 0; codepoint < ncodepoint; codepoint++) {
        for (y = 0; y < glyph_pixels; y++) {
            yy = glyph_pixels * (codepoint / codepoints_per_row) + y;
            xx = glyph_pixels * (codepoint % codepoints_per_row);
            dst = ob + bytes_per_pixel *
                           (glyph_pixels * yy * codepoints_per_row + xx);
            rowbits = *src++;
            rowbits |= *src++ << 8;
            for (x = 0; x < glyph_pixels; x++) {
                byte = (rowbits & 1) ? 255 : 0;
                for (b = 0; b < bytes_per_pixel; b++) {
                    *dst++ = byte;
                }
                rowbits >>= 1;
            }
        }
    }
}

static void
u32l(unsigned char *out, unsigned long val)
{
    out[0] = val;
    val >>= 8;
    out[1] = val;
    val >>= 8;
    out[2] = val;
    val >>= 8;
    out[3] = val;
}

static void
out_bmp(void)
{
    headersize = 0x36;
    header[0] = 'B';
    header[1] = 'M';
    u32l(&header[2], headersize + 3 * width * height);
    u32l(&header[0xa], headersize);
    u32l(&header[0xe], headersize - 0xe);
    u32l(&header[0x12], width);
    u32l(&header[0x16], 0xffffffff - height + 1);
    header[0x1a] = 1;
    header[0x1c] = 24;
    u32l(&header[0x22], 3 * width * height);
    copy_pixels(3);
}

static void
out_farbfeld(void)
{
    headersize = 16;
    memcpy(header, "farbfeld", 8);
    header[10] = width >> 8;
    header[11] = width;
    header[14] = height >> 8;
    header[15] = height;
    copy_pixels(8);
}

static void
out_tga(void)
{
    headersize = 18;
    header[2] = 2;
    header[12] = width;
    header[13] = width >> 8;
    header[14] = height;
    header[15] = height >> 8;
    header[16] = 24;
    header[17] = 1 << 5;
    copy_pixels(3);
}

static void
usage(void)
{
    fprintf(stderr, "usage: dumbfont2image bmp|farbfeld|tga\n");
    exit(1);
}

static voidfun_t
output_for_format(const char *format)
{
    if (!strcmp(format, "bmp")) {
        return out_bmp;
    }
    if (!strcmp(format, "farbfeld")) {
        return out_farbfeld;
    }
    if (!strcmp(format, "tga")) {
        return out_tga;
    }
    usage();
    return 0;
}

int
main(int argc, char **argv)
{
    voidfun_t out;

    binary_stdin();
    binary_stdout();
    if (argc != 2) {
        usage();
    }
    out = output_for_format(argv[1]);
    glyph_pixels = read_unisig();
    read_character_images();
    out();
    if ((1 != fwrite(header, headersize, 1, stdout)) ||
        (1 != fwrite(ob, obsize, 1, stdout))) {
        die("cannot write to stdout");
    }
    return 0;
}
