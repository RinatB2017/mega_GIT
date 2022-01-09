
#include <QApplication>

#include "MyGLWidget.hpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    MyGLWidget *widget = new MyGLWidget();
    widget->setFixedSize(1920, 1000);
    widget->show();

    return app.exec();
}
