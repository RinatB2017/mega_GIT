#include "tetrisview.h"
#include <QApplication>

int main( int argc, char* argv[] )
{
    QApplication a( argc, argv );
    TetrisView w;
    w.show();

    return a.exec();
}
