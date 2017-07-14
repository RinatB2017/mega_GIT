//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
//--------------------------------------------------------------------------------
#include "SDLWidget.h"
//--------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    QApplication app(argc,argv);
    QMainWindow  mw;

    QSDLScreenWidget *sdlWidget = new QSDLScreenWidget(&mw);
    mw.setCentralWidget(sdlWidget);

    // Query the desktop to determine the size of the desktop and
    // to center the application when it opens.
    //
    int width = QApplication::desktop()->width();
    int height = QApplication::desktop()->height();

    mw.setGeometry((width - 800)/2, (height - 600)/2, 800, 600);
    mw.show();

    return app.exec();
}
//--------------------------------------------------------------------------------
