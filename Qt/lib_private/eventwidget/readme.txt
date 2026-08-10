#include <QApplication>
#include <QTextEdit>

#include "eventwidget.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    QTextEdit log;
    EventWidget widget;

    QObject::connect( &widget, SIGNAL(gotEvent(QString)), &log, SLOT(append(QString)) );

    log.show();
    widget.show();

    return app.exec();
}
