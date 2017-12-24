#!/bin/bash
QTP=qt-everywhere-src-5.10.0

sed -i 's/qtCompileTest(jasper)/#qtCompileTest(jasper)/g' $QTP'/qtimageformats/qtimageformats.pro'
sed -i 's/jp2 \\/#jp2 \\/g' $QTP'/qtimageformats/src/plugins/imageformats/imageformats.pro'
rm -f $QTP'/qtimageformats/src/3rdparty/jasper.pri'
rm -Rf $QTP'/qtimageformats/src/3rdparty/jasper'
read -n 1
