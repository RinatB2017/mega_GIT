#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickView>
#include <QQuickItem>
#include <QDebug>

#include "myclass.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

#if 0
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //engine.load(QUrl(QStringLiteral("qrc:/button.qml")));
    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }
#else
    QQmlApplicationEngine view;
    view.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *item = view.rootObjects()[0];

    MyClass *myClass = new MyClass;
    QObject::connect(item,      SIGNAL(qmlSignal(QString)),
                     myClass,   SLOT(cppSlot(QString)));
#endif

    return app.exec();
}
