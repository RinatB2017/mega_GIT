@echo off
rem ====================CONFIGURE BEFORE RUN SCRIPT!!======================
set QtSrcDir=%CD%\qt-everywhere-src-5.10.0
set QtStaticDir=C:\Qt\5.10_Static
set MingwDir=C:\Qt\Tools\mingw530_32
set LANG = en
rem =======================================================================

PATH = %MingwDir%\bin;%MingwDir%\opt\bin;%SystemRoot%\system32;%SystemRoot%

set FILE_TO_PATCH=%QtSrcDir%\qtbase\mkspecs\win32-g++\qmake.conf
echo %FILE_TO_PATCH%
if exist %FILE_TO_PATCH%.patched goto skipPatch
type %FILE_TO_PATCH%>%FILE_TO_PATCH%.patched
echo.>>%FILE_TO_PATCH%
echo QMAKE_LFLAGS += -static -static-libgcc>>%FILE_TO_PATCH%
echo QMAKE_CFLAGS_RELEASE -= -O2>>%FILE_TO_PATCH%
echo QMAKE_CFLAGS_RELEASE += -Os -momit-leaf-frame-pointer>>%FILE_TO_PATCH%
echo DEFINES += QT_STATIC_BUILD>>%FILE_TO_PATCH%
:skipPatch

set QT_INSTALL_PREFIX = %QtStaticDir%
set OldDir=%CD%
cd %QtSrcDir%
cmd /C "configure.bat -shared -release -platform win32-g++ -prefix %QtStaticDir% -opensource -confirm-license -c++std c++14 -nomake examples -skip wayland -skip purchasing -skip serialbus -skip qtserialport -skip script -skip scxml -skip speech -skip location -no-opengl -qt-zlib -qt-pcre -qt-libpng -no-libjpeg -qt-freetype -no-openssl"
if ERRORLEVEL 1 goto :error

mingw32-make -r -k -j4
if ERRORLEVEL 1 goto :error

mingw32-make -k install
if ERRORLEVEL 1 goto :error

set FILE_TO_PATCH=%QtStaticDir%\mkspecs\win32-g++\qmake.conf
echo.>>%FILE_TO_PATCH%
echo CONFIG += static>>%FILE_TO_PATCH%

echo ============BUILT!============
goto exitX
:error
echo ============ERROR!============
:exitX
pause
cd %OldDir%
