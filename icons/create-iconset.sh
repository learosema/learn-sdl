#!/bin/sh

# Windows
mkdir -p windows
magick -background none icon.svg -define icon:auto-resize=256,128,64,48,32,16 windows/icon.ico
magick -background none icon.svg -resize 16x16 windows/icon_16x16.png
magick -background none icon.svg -resize 32x32 windows/icon_32x32.png
magick -background none icon.svg -resize 48x48 windows/icon_48x48.png
magick -background none icon.svg -resize 64x64 windows/icon_64x64.png
magick -background none icon.svg -resize 128x128 windows/icon_128x128.png
magick -background none icon.svg -resize 256x256 windows/icon_256x256.png
magick -background none icon.svg -resize 512x512 windows/icon_512x512.png
# Mac
# iconutil -c icns icon.iconset

# Linux
magick -background none icon.svg -resize 1024x1024 baseicon.png
