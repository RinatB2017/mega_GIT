#include "recorderSettings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RecorderSettings w;
    w.show();

    return a.exec();
}
