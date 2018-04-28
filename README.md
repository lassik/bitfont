dumbfont
========

Overview
--------

A dumbfont file encodes a fixed-size one-color bitmap font for
graphics applications that need an extremely simple way to output
text:

* Operating system and firmware consoles
* Bare metal coding experiments
* Screensavers
* Games
* Menus
* etc.

The available font sizes are:

* 8x8 pixels ("dumbfont8")
* 16x16 pixels ("dumbfont16")
* 32x32 pixels ("dumbfont32")
* 64x64 pixels ("dumbfont64")

Each font size has its own version of the file format for maximum
simplicity.

File format specification
-------------------------

Signature
---------

A dumbfont file always starts with one of the following sequences of
32 constant hex bytes. Just use `memcmp()` to check the bytes.

**dumbfont8** (`io.lassi.dumbfont8`):

    ff 55 6e 69 73 69 67 00 0a 0d 0a 12 69 6f 2e 6c
    61 73 73 69 2e 64 75 6d 62 66 6f 6e 74 38 00 00

**dumbfont16** (`io.lassi.dumbfont16`):

    ff 55 6e 69 73 69 67 00 0a 0d 0a 13 69 6f 2e 6c
    61 73 73 69 2e 64 75 6d 62 66 6f 6e 74 31 36 00

**dumbfont32** (`io.lassi.dumbfont32`):

    ff 55 6e 69 73 69 67 00 0a 0d 0a 13 69 6f 2e 6c
    61 73 73 69 2e 64 75 6d 62 66 6f 6e 74 33 32 00

**dumbfont64** (`io.lassi.dumbfont64`):

    ff 55 6e 69 73 69 67 00 0a 0d 0a 13 69 6f 2e 6c
    61 73 73 69 2e 64 75 6d 62 66 6f 6e 74 36 34 00

Characters
----------

Next comes the character data. Characters are stored in order
according to their Unicode codepoint. The first character in the file
is codepoint zero, the second character is codepoint one,
etc. Codepoints cannot be skipped, but you can simply provide blank
glyphs for characters that the font doesn't have.

Unicode defines the first 128 characters as identical to ASCII, so the
first 128 characters in the file span the entire American 7-bit ASCII
range. Going further, the first 256 characters of Unicode correspond
to a character set called Latin-1 (formally known as
ISO-8859-1). Latin-1 is ASCII plus some European extensions. So with
256 characters you can cover most European text. Note further that PC
computers in the DOS and early Windows era used various mutually
incompatible 256-character "codepages". Those are obsolete and
dumbfont doesn't have anything to do with them. Unicode is ready for
prime time, use it :-)

The file can contain an arbitrary number of characters, from zero (no
characters at all) all the way to the Unicode maximum. The number of
characters in the file is not stated anywhere inside the
file. However, each character consumes a fixed number of bytes so you
can calculate the number of characters by dividing the file size
(remember to first subtract 32 bytes for the signature, and round down
the result of the division in case there is junk data at the end of
the file).  Another approach is simply to read the file in chunks and
stop when the number of bytes left is less than the size of a glyph.

Character glyphs
----------------

Each glyph is stored with one bit per pixel, in a fixed number of
bytes:

* dumbfont8: 8 bytes per glyph (1 byte per row)
* dumbfont16: 32 bytes per glyph (2 bytes per row)
* dumbfont32: 128 bytes per glyph (4 bytes per row)
* dumbfont64: 512 bytes per glyph (8 bytes per row)

Every bit in every byte denotes a pixel in the glyph - there is no
padding at all.

Bit value 1 means "draw a pixel here" (in whatever color you like) and
bit value 0 means "don't draw a pixel here" (i.e. use the background
color, or make the pixel transparent).

Each glyph is stored in row-major order. Rows are in top-down order
visually. Each row is stored in little-endian byte order, so:

* The least significant bit of the first byte is the leftmost pixel of
  the row.
* The least significant bit of the second byte is the ninth leftmost
  pixel of the row (for dumbfont16 and up).
* The least significant bit of the third byte is the seventeenth
  leftmost pixel of the row (for dumbfont 32 and up).
* etc.

How to store the font in memory
-------------------------------

To store in memory, just cut off the signature from the beginning and
store the rest of the file as-is. The representation used in the file
is also optimal for computer RAM.

On big-endian architectures you can byte-swap words in RAM if you want
to. Remember not to byte-swap them in the dumbfont file, otherwise
little-endian computers won't be able to read the file correctly
anymore.

Rationale
---------

The font sizes (powers of two, starting at 8) are chosen to simplify
common file and memory operations. All modern general-purpose CPUs use
8-bit bytes, so a row of pixels in a character glyph fits exactly in a
fixed number of bytes. It's also possible to load several rows of
pixels into a CPU register at once. Each glyph fits exactly into a
fixed number of bytes so it's easy to lookup glyphs and their rows.

Little-endian storage enables very fast loads on Intel, RISC-V, aswell
as bi-endian ARM, MIPS, PPC and SPARC CPUs.

Compression
-----------

The file format intentionally doesn't support any compression, not
even run-length encoding. That would detract from its simplicity and
predictability. Should the need arise, you can easily compress a
dumbfont file with a general-purpose compressor such as Gzip, LZ4 or
Snappy with excellent results. If there are gaps in the character
range, those blank character glyphs are particularly easy for such
algorithms to compress (since they are just long runs of zeros).

Alternatives
------------

If dumbfont is *too* simple for your needs, the [PF2 bitmap font file
format - a new font format for
GRUB](http://grub.gibibit.com/New_font_format) is also reasonably
simple while supporting more font sizes, kerning, and a little
metadata.
