#!/bin/sh
#
# http://www.fontspace.com/04/04b30
#
set -eux
cd "$(dirname "$0")"
[ -f 04_04b30.zip ]
shasum 04_04b30.zip | diff -u 04_04b30.sha1 -
mkdir -p ../dumbfont16
rm -rf work
mkdir work
(cd work && unzip ../04_04b30.zip)
ttf2dumbfont work/04B_30__.TTF 16 >../dumbfont16/04b_30.dumbfont
cp -p work/about.gif ../dumbfont16/04b_30.license.gif
