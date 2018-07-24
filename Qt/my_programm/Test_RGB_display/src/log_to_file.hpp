//--------------------------------------------------------------------------------
#if QT_VERSION >= 0x050000
#include <QMessageLogContext>
#endif
#include <stdio.h>
#include <QFile>
#include <QTime>
//--------------------------------------------------------------------------------
#if QT_VERSION >= 0x050000
void log_to_file(QtMsgType type, const QMessageLogContext &context, const QString &msg)
#else
void log_to_file(QtMsgType type, const char *msg)
#endif
{
    FILE *file;
    file = fopen("file.log", "a");

#if (QT_VERSION >= 0x50000)
    QByteArray localMsg = msg.toLocal8Bit();
#endif
    switch (type)
    {
    case QtDebugMsg:
#if (QT_VERSION >= 0x50000)
        fprintf(file, "%s Debug: %s\n",
                QTime::currentTime().toString("hh:mm:ss").toLocal8Bit().data(),
                context.function);
#else
        fprintf(file, "Debug: (%s)\n", msg);
#endif
        break;

    case QtWarningMsg:
#if (QT_VERSION >= 0x50000)
        fprintf(file, "%s Warning: %s\n",
                QTime::currentTime().toString("hh:mm:ss").toLocal8Bit().data(),
                context.function);
#else
        fprintf(file, "Warning: (%s)\n", msg);
#endif
        break;

    case QtCriticalMsg:
#if (QT_VERSION >= 0x50000)
        fprintf(file, "%s Critical: %s\n",
                QTime::currentTime().toString("hh:mm:ss").toLocal8Bit().data(),
                context.function);
#else
        fprintf(file, "Critical: (%s)\n", msg);
#endif
        break;

    case QtFatalMsg:
#if (QT_VERSION >= 0x50000)
        fprintf(file, "%s Fatal: %s\n",
                QTime::currentTime().toString("hh:mm:ss").toLocal8Bit().data(),
                context.function);
#else
        fprintf(file, "Fatal: (%s)\n", msg);
#endif
        abort();
        break;

    default:
#if (QT_VERSION >= 0x50000)
        fprintf(file, "%s Trace: %s\n",
                QTime::currentTime().toString("hh:mm:ss").toLocal8Bit().data(),
                context.function);
#else
        fprintf(file, "Trace: (%s)\n", msg);
#endif
        abort();
        break;
    }
    fclose(file);
}
//--------------------------------------------------------------------------------
