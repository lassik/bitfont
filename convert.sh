#!/bin/sh
set -eux
fonts/dot-monoco/dot-monoco >fonts/dot-monoco/dot-monoco.dumbfont
./dumbfont2asm nasm font fontend \
	<fonts/dot-monoco/dot-monoco.dumbfont \
	>fonts/dot-monoco/dot-monoco.nasm
./dumbfont2image farbfeld \
	<fonts/dot-monoco/dot-monoco.dumbfont \
	>fonts/dot-monoco/dot-monoco.farbfeld
./dumbfont2image tga \
	<fonts/dot-monoco/dot-monoco.dumbfont \
	>fonts/dot-monoco/dot-monoco.tga
identify fonts/dot-monoco/dot-monoco.tga

cd 'fonts/ultimate-oldschool-pc-font-pack/Px437 (TrueType - DOS charset)'
for font in *.ttf; do
	font="${font%.*}"
	../../../ttf2dumbfont "$font.ttf" 16 4 >"$font.dumbfont"
	../../../dumbfont2image tga <"$font.dumbfont" >"$font.tga"
done
