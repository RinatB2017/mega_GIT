//--------------------------------------------------------------------------------
#ifndef CODECS_H
#define CODECS_H
//--------------------------------------------------------------------------------
#include <QTextCodec>
//--------------------------------------------------------------------------------
void set_codecs(void)
{
    //QTextCodec *unixcodec = QTextCodec::codecForName("System");
    QTextCodec *unixcodec = QTextCodec::codecForName("UTF-8");
#ifdef PROGRAMM_IN_UTF8
    QTextCodec *wincodec  = QTextCodec::codecForName("UTF-8");
#endif
#ifdef PROGRAMM_IN_KOI8R
    QTextCodec *wincodec  = QTextCodec::codecForName("KOI8-R");
#endif
#ifdef PROGRAMM_IN_CP1251
    QTextCodec *wincodec  = QTextCodec::codecForName("CP1251");
#endif
#ifdef PROGRAMM_IN_CP866
    QTextCodec *wincodec  = QTextCodec::codecForName("CP866");
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
#endif
