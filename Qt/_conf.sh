#!/bin/bash

./configure -static -release -silent \
-prefix ~/Qt/5.10.0_static \
-opensource -confirm-license -opengl \
-nomake examples \
-skip wayland -skip purchasing -skip serialbus -skip qtserialport -skip script -skip scxml -skip speech \
-system-xcb -qt-libpng -no-libjpeg -qt-zlib -qt-pcre -gtk -system-freetype -system-harfbuzz \
-pulseaudio -alsa
