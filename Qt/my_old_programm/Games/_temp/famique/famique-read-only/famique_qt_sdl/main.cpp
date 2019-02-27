//#include "DebugMemory.h"

#include <QApplication>

#include <SDL.h>

#include <QTimer>
#include <QObject>
//#include "/Library/Frameworks/SDL.framework/sdlmain.h"







#include "gui_mainwindow.h"
#include "sdlwidget.h"

#undef main // to avoid SDL-qt linking hell

int main(int argc, char **argv)
{

    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_NativeWindows,true);
    GUI_MainWindow w;
    DebugOutput d;
   // QSDLScreenWidget *sdlw = new QSDLScreenWidget(&w);
    //w.setCentralWidget(sdlw);

    w.setGeometry(320,320,254,240);

    w.show();
    d.show();

    Emulator *emu = new Emulator();



    emu->SetDebugOutputHandler(&d);
    emu->PrintDebugString(QString("Binding QT and SDL through ugly window-id-hack...\n"));



    w.SetEmulatorPtr(emu);
    //sdlw->SetEmulatorPtr(emu);
  //  sdlw->SetPalette();
  //  emu->SetSDLSurface(sdlw->getSDLSurface());



    emu->PrintDebugString(QString("Famique initialized!\n\nHere be dragons!\n\n"));





    w.setFocus(); //we get our input from Qt, so we need the window to be in focus...



    w.Start();

    if(argc > 1)
    {
        printf("attempting to load %s", argv[1]);
        emu->OpenROM(argv[1]);
        emu->Start();
        w.setFocus();
    }

    return a.exec();
}
