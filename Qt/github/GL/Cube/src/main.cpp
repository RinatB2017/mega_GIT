
#include <QApplication>
#include <QGLFormat>

#include "cube.h" 

int main(int argc, char *argv[]) 
{ 
    QApplication app(argc, argv);
    if (!QGLFormat::hasOpenGL())
        qFatal("This system has no OpenGL support");
    
    Cube cube;
    //cube.setCaption(QObject::tr("Cube"));
    cube.resize(300, 300);
    //app.setMainWidget(&cube);
    cube.show();
    return app.exec();
}
