//--------------------------------------------------------------------------------
#include <QFile>
#include <QTime>
//--------------------------------------------------------------------------------
void log_to_file(QtMsgType type, const char *msg)
{
    FILE *file;
    file = fopen("file.log", "a");

    switch (type)
    {
    case QtDebugMsg:
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        fprintf(file, "%s Debug: %s\n", QTime::currentTime().toString("hh:mm:ss").toAscii().data(), msg);
#else
        fprintf(file, "%s Debug: %s\n", QTime::currentTime().toString("hh:mm:ss").toLatin1().data(), msg);
#endif
        break;

    case QtWarningMsg:
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        fprintf(file, "%s Warning: %s\n", QTime::currentTime().toString("hh:mm:ss").toAscii().data(), msg);
#else
        fprintf(file, "%s Warning: %s\n", QTime::currentTime().toString("hh:mm:ss").toLatin1().data(), msg);
#endif
        break;

    case QtCriticalMsg:
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        fprintf(file, "%s Critical: %s\n", QTime::currentTime().toString("hh:mm:ss").toAscii().data(), msg);
#else
        fprintf(file, "%s Critical: %s\n", QTime::currentTime().toString("hh:mm:ss").toLatin1().data(), msg);
#endif
        break;

    case QtFatalMsg:
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        fprintf(file, "%s Fatal: %s\n", QTime::currentTime().toString("hh:mm:ss").toAscii().data(), msg);
#else
        fprintf(file, "%s Fatal: %s\n", QTime::currentTime().toString("hh:mm:ss").toLatin1().data(), msg);
#endif
        abort();
        break;

    default:
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        fprintf(file, "%s Trace: %s\n", QTime::currentTime().toString("hh:mm:ss").toAscii().data(), msg);
#else
        fprintf(file, "%s Trace: %s\n", QTime::currentTime().toString("hh:mm:ss").toLatin1().data(), msg);
#endif
        abort();
        break;
    }
    fclose(file);
}
//--------------------------------------------------------------------------------
