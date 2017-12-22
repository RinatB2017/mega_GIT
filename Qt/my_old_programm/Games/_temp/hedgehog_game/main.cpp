#include "screencontroller.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  ScreenController w;

  w.show();

  return a.exec();
}
