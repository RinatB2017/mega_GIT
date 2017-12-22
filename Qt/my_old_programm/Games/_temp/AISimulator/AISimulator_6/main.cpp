#include "aisimulatorview.h"
#include <QApplication>

#include "nullbotai.h"
#include "easyrandomai.h"
#include "smartrandomai.h"
#include "singlememrandomai.h"
#include "accumulatingrandomai.h"
#include "smartsinglememrandomai.h"
#include "pathfinderai.h"
#include "smartpathfinderai.h"
#include "runawayai.h"

int main( int argc, char* argv[] ) {
    QApplication a( argc, argv );

    MainWidget w;
    w.registerAI( "NullBotAI", { 2, 3 }, new NullBotAI );
    w.registerAI( "EasyRandomAI", { 2, 3 }, new EasyRandomAI );
    w.registerAI( "SmartRandomAI", { 2, 3 }, new SmartRandomAI );
    w.registerAI( "SingleMemRandomAI", { 2, 3 }, new SingleMemRandomAI );
    w.registerAI( "AccumulatingRandomAI", { 2, 3 }, new AccumulatingRandomAI );
    w.registerAI( "SmartSingleMemRandomAI", { 2, 3 }, new SmartSingleMemRandomAI );
    w.registerAI( "PathFinderAI", { 2 }, new PathFinderAI );
    w.registerAI( "SmartPathFinderAI", { 2 }, new SmartPathFinderAI );
    w.registerAI( "RunAwayAI", { 3 }, new RunAwayAI );
    w.show();

    return a.exec();
}
