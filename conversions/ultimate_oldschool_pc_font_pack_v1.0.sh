#!/bin/sh
#
# The Ultimate Oldschool PC Font Pack
#
# https://int10h.org/oldschool-pc-fonts/
#
# The zipfile contains the Creative Commons Attribution-ShareAlike 4.0
# International license. However, these fonts are abandonware from
# commercial companies (they are ripped from the video memory of old
# computers) so I presume the people who extracted the fonts and made
# the zipfile cannot claim any legal rights over the fonts. The
# zipfile itself is probably copyrightable, and hence a license
# statement is in order, but nothing of the zipfile remains in the
# dumbfont files made by this script - only the original pixels from
# video memory.
#
# It's a cool project, though (and a very cool looking website :-). We
# are thankful for their effort and wish to acknowledge their work.
#
set -eux
cd "$(dirname "$0")"
[ -f ultimate_oldschool_pc_font_pack_v1.0.zip ]
shasum ultimate_oldschool_pc_font_pack_v1.0.zip | diff -u ultimate_oldschool_pc_font_pack_v1.0.sha1 -
mkdir -p ../dumbfont16
rm -rf work
mkdir work
(cd work && unzip ../ultimate_oldschool_pc_font_pack_v1.0.zip)
for ttf in work/*/*.ttf; do
	font="$(basename "${ttf%.*}")"
	ttf2dumbfont "$ttf" 16 >"../dumbfont16/$font.dumbfont"
	cp -p ultimate_oldschool_pc_font_pack_v1.0.license.text "../dumbfont16/$font.license.text"
done
