/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QMessageBox>
#include <QVBoxLayout>
#include <QString>
#include <QDebug>
#include <QFrame>
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "codecs.h"

#include <QPushButton>
#include <QMdiSubWindow>
#include <QMdiArea>
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    set_codecs();
    QtSingleApplication app(argc, argv);
    if(app.isRunning())
    {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Application already running!"));
        return -1;
    }

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    QPixmap pixmap(":/logo/pinguin.png");

    MySplashScreen *splash = new MySplashScreen(pixmap, 10);
    splash->show();

    qApp->processEvents();

    MainWindow main_window;
    //main_window.setWindowFlags(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowContextHelpButtonHint);

#if 1
    MainBox *mainBox = new MainBox(main_window.getThis(), splash);
#else
    QMdiArea *mainBox = new QMdiArea(main_window.getThis());
    mainBox->setViewMode(QMdiArea::TabbedView);
    mainBox->setTabsClosable(true);
    mainBox->setTabsMovable(true);

    QMdiSubWindow *subWindow1 = new QMdiSubWindow;
    QMdiSubWindow *subWindow2 = new QMdiSubWindow;
    QMdiSubWindow *subWindow3 = new QMdiSubWindow;
    QMdiSubWindow *subWindow4 = new QMdiSubWindow;
    QMdiSubWindow *subWindow5 = new QMdiSubWindow;

    subWindow1->setWindowTitle("tab1");
    subWindow2->setWindowTitle("tab2");
    subWindow3->setWindowTitle("tab3");
    subWindow4->setWindowTitle("tab4");
    subWindow5->setWindowTitle("tab5");

    QPushButton *btn = new QPushButton("test");
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(btn);
    vbox->addStretch();

    QFrame *frame = new QFrame();
    frame->setLayout(vbox);

    subWindow1->setWidget(frame);

    mainBox->addSubWindow(subWindow1);
    mainBox->addSubWindow(subWindow2);
    mainBox->addSubWindow(subWindow3);
    mainBox->addSubWindow(subWindow4);
    mainBox->addSubWindow(subWindow5);

    mainBox->setActiveSubWindow(subWindow2);
    mainBox->setActiveSubWindow(subWindow1);
#endif

    main_window.setCentralWidget(mainBox);
    main_window.show();

    splash->finish(&main_window);

    qDebug() << QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME));
    qDebug() << QLocale().name();

    return app.exec();
}
//--------------------------------------------------------------------------------
