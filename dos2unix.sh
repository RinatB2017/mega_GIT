#!/bin/bash

find . -iname "*.*" -print0 | xargs -0 dos2unix

#find . -iname "*.xml" -print0 | xargs -0 dos2unix
#find . -iname "*.java" -print0 | xargs -0 dos2unix
#find . -iname "*.bat" -print0 | xargs -0 dos2unix
#find . -iname "*.js" -print0 | xargs -0 dos2unix
#find . -iname "*.css" -print0 | xargs -0 dos2unix
#find . -iname "*.html" -print0 | xargs -0 dos2unix
#find . -iname "*.gradle" -print0 | xargs -0 dos2unix
