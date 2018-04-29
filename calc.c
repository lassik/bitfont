#include <stdio.h>
#include <stdlib.h>

static unsigned int
div_roundup(unsigned int x, unsigned int y)
{
    return (x + y - 1) / y;
}

static void
pages_sectors(unsigned int bytes_per_image, unsigned int images)
{
    unsigned int pages = div_roundup(bytes_per_image * images, 4096);
    unsigned int sectors = div_roundup(bytes_per_image * images, 512);
    printf(
        "%u images fit in %u pages (or %u sectors)\n", images, pages, sectors);
}

static void
reso(unsigned int pixels, unsigned int screenwidth, unsigned int screenheight)
{
    printf("%ux%s%u%s characters fit on a %ux%u pixel screen\n",
        screenwidth / pixels, ((screenwidth % pixels) ? "*" : ""),
        screenheight / pixels, ((screenheight % pixels) ? "+" : ""),
        screenwidth, screenheight);
}

static void
calc(unsigned int pixels)
{
    if (pixels % 8) {
        exit(1);
    }
    unsigned int bytes_per_row = pixels / 8;
    unsigned int bytes_per_image = pixels * bytes_per_row;
    printf("\n");
    printf("dumbfont%u - %ux%u pixels per image\n", pixels, pixels, pixels);
    printf("%u pixels per image\n", pixels * pixels);
    printf("%u bytes per image\n", bytes_per_image);
    printf("%u bytes per row\n", bytes_per_row);
    printf("%u rows fit in 32 bits\n", 4 / bytes_per_row);
    printf("%u rows fit in 64 bits\n", 8 / bytes_per_row);
    pages_sectors(bytes_per_image, 128);
    pages_sectors(bytes_per_image, 256);
    reso(pixels, 640, 480);
    reso(pixels, 800, 600);
    reso(pixels, 1024, 768);
    reso(pixels, 1280, 720);
    reso(pixels, 1280, 768);
    reso(pixels, 1920, 1080);
}

int
main(void)
{
    calc(8);
    calc(16);
    calc(32);
    calc(64);
    return 0;
}
