#!/bin/sh
#
# Scanlines (16x16)
#
# https://www.dafont.com/bitmap.php
#
set -eux
cd "$(dirname "$0")"
[ -f scanlines.zip ]
shasum scanlines.zip | diff -u scanlines.sha1 -
mkdir -p ../dumbfont16
rm -rf work
mkdir work
d work && unzip ../scanlines.zip)
ttf2dumbfont work/Scanlines.ttf 16 >../dumbfont16/scanlines.dumbfont
cat work/readme.txt work/license.txt >../dumbfont16/scanlines.license.text
