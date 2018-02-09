#include "utils.h"

QString safe::secondsToString(int value)
{
    const int minutes = value / 60;
    const int seconds = value % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}
