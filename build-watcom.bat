cd "%~dp0"
wcc386 -q -wx dumbfont.c
wcc386 -q -wx dumbfont2image.c
wcc386 -q -wx dumbfont2include.c
wlink op q f dumbfont2image f dumbfont
wlink op q f dumbfont2include f dumbfont
