//--------------------------------------------------------------------------------
#ifndef CODECS_H
#define CODECS_H
//--------------------------------------------------------------------------------
#include <QTextCodec>
//--------------------------------------------------------------------------------
void set_codecs_old(void)
{
    QTextCodec *unixcodec = QTextCodec::codecForName("System");
    QTextCodec *wincodec  = QTextCodec::codecForName("System");
#ifdef PROGRAMM_IN_UTF8
    unixcodec = QTextCodec::codecForName("UTF-8");
    wincodec  = QTextCodec::codecForName("UTF-8");
#endif
#ifdef PROGRAMM_IN_KOI8R
    inixcodec = QTextCodec::codecForName("KOI8-R");
    wincodec  = QTextCodec::codecForName("KOI8-R");
#endif
#ifdef PROGRAMM_IN_CP1251
    unixcodec = QTextCodec::codecForName("CP1251");
    wincodec  = QTextCodec::codecForName("CP1251");
#endif
#ifdef PROGRAMM_IN_CP866
    unixcodec = QTextCodec::codecForName("CP866");
    wincodec  = QTextCodec::codecForName("CP866");
#endif

#ifdef Q_OS_UNIX
    Q_UNUSED(wincodec)
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTextCodec::setCodecForTr(unixcodec);
    QTextCodec::setCodecForCStrings(unixcodec);
#endif
    QTextCodec::setCodecForLocale(unixcodec);
#endif

#ifdef Q_OS_WIN
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTextCodec::setCodecForTr(unixcodec);
    QTextCodec::setCodecForCStrings(wincodec);
#else
    Q_UNUSED(unixcodec)
#endif
    QTextCodec::setCodecForLocale(wincodec);
#endif
}
//--------------------------------------------------------------------------------
void set_codecs(void)
{
    QTextCodec *codec = QTextCodec::codecForName("System");
#ifdef PROGRAMM_IN_UTF8
    codec = QTextCodec::codecForName("UTF-8");
#endif
#ifdef PROGRAMM_IN_KOI8R
    codec = QTextCodec::codecForName("KOI8-R");
#endif
#ifdef PROGRAMM_IN_CP1251
    codec = QTextCodec::codecForName("CP1251");
#endif
#ifdef PROGRAMM_IN_CP866
    codec = QTextCodec::codecForName("CP866");
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
#else
    QTextCodec::setCodecForLocale(codec);
#endif
}
//--------------------------------------------------------------------------------
#endif
