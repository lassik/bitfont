#!/bin/sh
set -eux

cd "$(dirname "$0")"
[ -f ultimate_oldschool_pc_font_pack_v1.0.zip ]
shasum ultimate_oldschool_pc_font_pack_v1.0.zip >sha1
diff -u sha1.goal sha1
rm -rf zip ttf dumbfont16 tga16
mkdir zip ttf dumbfont16 tga16
(cd zip && unzip ../ultimate_oldschool_pc_font_pack_v1.0.zip)
mv zip/*/*.ttf ttf/

convert() {
	baseline="$1"
	font="$2"
	for pix in 16; do
		../../ttf2dumbfont "ttf/$font.ttf" "$pix" "$baseline" >"dumbfont$pix/$font.dumbfont"
		../../dumbfont2image tga <"dumbfont$pix/$font.dumbfont" >"tga$pix/$font.tga"
	done
}

convert 2 Px437_AMI_BIOS
convert 2 Px437_AMI_BIOS-2y
convert 2 Px437_AmstradPC1512
convert 2 Px437_AmstradPC1512-2y
convert 4 Px437_ATI_8x14
convert 4 Px437_ATI_8x16
convert 2 Px437_ATI_8x8
convert 2 Px437_ATI_8x8-2y
convert 4 Px437_ATI_9x14
convert 4 Px437_ATI_9x16
convert 2 Px437_ATI_SmallW_6x8
convert 5 Px437_ATT_PC6300
convert 5 Px437_ATT_PC6300-2x
convert 4 Px437_CompaqThin_8x14
convert 4 Px437_CompaqThin_8x16
convert 2 Px437_CompaqThin_8x8
convert 2 Px437_DTK_BIOS
convert 2 Px437_DTK_BIOS-2y
convert 2 Px437_IBM_3270pc
convert 2 Px437_IBM_BIOS
convert 2 Px437_IBM_BIOS-2x
convert 2 Px437_IBM_BIOS-2y
convert 2 Px437_IBM_CGA
convert 2 Px437_IBM_CGA-2y
convert 2 Px437_IBM_CGAthin
convert 2 Px437_IBM_CGAthin-2y
convert 2 Px437_IBM_Conv
convert 2 Px437_IBM_Conv-2x
convert 2 Px437_IBM_Conv-2y
convert 4 Px437_IBM_EGA8
convert 4 Px437_IBM_EGA8-2x
convert 4 Px437_IBM_EGA9
convert 4 Px437_IBM_EGA9-2x
convert 4 Px437_IBM_ISO8
convert 4 Px437_IBM_ISO9
convert 4 Px437_IBM_MDA
convert 4 Px437_IBM_PGC
convert 4 Px437_IBM_PGC-2x
convert 2 Px437_IBM_PS2thin1
convert 2 Px437_IBM_PS2thin2
convert 2 Px437_IBM_PS2thin3
convert 2 Px437_IBM_PS2thin4
convert 2 Px437_IBM_VGA8
convert 2 Px437_IBM_VGA8-2x
convert 2 Px437_IBM_VGA9
convert 2 Px437_IBM_VGA9-2x
convert 2 Px437_ITT_BIOS
convert 2 Px437_ITT_BIOS-2y
convert 2 Px437_Kaypro2K
convert 2 Px437_Kaypro2k-2y
convert 2 Px437_Phoenix_BIOS
convert 2 Px437_Phoenix_BIOS-2y
convert 4 Px437_PhoenixEGA_8x14
convert 4 Px437_PhoenixEGA_8x16
convert 2 Px437_PhoenixEGA_8x8
convert 2 Px437_PhoenixEGA_8x8-2y
convert 2 Px437_PhoenixEGA_9x14
convert 4 Px437_TandyNew_225
convert 4 Px437_TandyNew_225-2y
convert 3 Px437_TandyNew_Mono
convert 2 Px437_TandyNew_TV
convert 2 Px437_TandyNew_TV-2y
convert 4 Px437_TandyOld_225
convert 4 Px437_TandyOld_225-2y
convert 2 Px437_TandyOld_TV
convert 2 Px437_TandyOld_TV-2y
convert 4 Px437_ToshibaLCD_8x16
convert 2 Px437_ToshibaLCD_8x8
convert 2 Px437_Verite_8x14
convert 2 Px437_Verite_8x16
convert 2 Px437_Verite_8x8
convert 2 Px437_Verite_8x8-2y
convert 4 Px437_Verite_9x14
convert 4 Px437_Verite_9x16
convert 2 Px437_VGA_SquarePx
convert 2 Px437_VTech_BIOS
convert 2 Px437_VTech_BIOS-2y
convert 3 Px437_Wyse700a
convert 3 Px437_Wyse700a-2y
convert 3 Px437_Wyse700b
convert 3 Px437_Wyse700b-2y
convert 2 PxPlus_AmstradPC1512
convert 2 PxPlus_AmstradPC1512-2y
convert 2 PxPlus_IBM_BIOS
convert 2 PxPlus_IBM_BIOS-2x
convert 2 PxPlus_IBM_BIOS-2y
convert 2 PxPlus_IBM_CGA
convert 2 PxPlus_IBM_CGA-2y
convert 2 PxPlus_IBM_CGAthin
convert 2 PxPlus_IBM_CGAthin-2y
convert 2 PxPlus_IBM_EGA8
convert 2 PxPlus_IBM_EGA8-2x
convert 2 PxPlus_IBM_EGA9
convert 2 PxPlus_IBM_EGA9-2x
convert 2 PxPlus_IBM_MDA
convert 2 PxPlus_IBM_VGA8
convert 2 PxPlus_IBM_VGA8-2x
convert 2 PxPlus_IBM_VGA9
convert 2 PxPlus_IBM_VGA9-2x
convert 2 PxPlus_TandyNew_225
convert 2 PxPlus_TandyNew_225-2y
convert 2 PxPlus_TandyNew_TV
convert 2 PxPlus_TandyNew_TV-2y
convert 2 PxPlus_VGA_SquarePx
