#include "aisimulatorview.h"
#include <QApplication>

#include "easyrandomai.h"
#include "smartrandomai.h"
#include "singlememrandomai.h"
#include "accumulatingrandomai.h"
#include "smartsinglememrandomai.h"

int main( int argc, char* argv[] ) {
    QApplication a( argc, argv );

    MainWidget w;
    w.registerAI( "EasyRandomAI", new EasyRandomAI );
    w.registerAI( "SmartRandomAI", new SmartRandomAI );
    w.registerAI( "SingleMemRandomAI", new SingleMemRandomAI );
    w.registerAI( "AccumulatingRandomAI", new AccumulatingRandomAI );
    w.registerAI( "SmartSingleMemRandomAI", new SmartSingleMemRandomAI );
    w.show();

    return a.exec();
}
