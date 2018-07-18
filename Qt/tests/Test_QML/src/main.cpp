#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include "test_qml.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<PixelGrid>("Pixel", 1, 0, "PixelGrid");
    QQuickView view(QUrl("qrc:/main.qml"));
    view.show();
    return app.exec();
}

// 
