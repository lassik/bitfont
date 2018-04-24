#!/bin/sh
set -eux
cd "$(dirname "$0")"
cc -Wall -Wextra -o dot_monoco dot_monoco.c
./dot_monoco >../dumbfont16/dot_monoco.dumbfont
cp -p dot_monoco.license.text ../dumbfont16/
