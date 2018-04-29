@setlocal
@if not defined BCC set BCC=c:\borland\bcc55
@set PATH=%BCC%\bin;%PATH%
cd "%~dp0"
bcc32 -I"%BCC%\include" -L"%BCC%\lib" -w-8060 dumbfont2image.c dumbfont.c
bcc32 -I"%BCC%\include" -L"%BCC%\lib" -w-8060 dumbfont2include.c dumbfont.c
