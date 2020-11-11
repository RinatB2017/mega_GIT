/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include "test_mouse_mainbox.hpp"
//--------------------------------------------------------------------------------
#include "ui_test_mouse_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "other.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    QGridLayout *grid = new QGridLayout(this);
    grid->setMargin(0);
    grid->setSpacing(0);

    for(int y=0; y<10; y++)
    {
        for(int x=0; x<10; x++)
        {
            QToolButton *btn = new QToolButton(this);
            btn->setProperty("pos_x", x);
            btn->setProperty("pos_y", y);
            connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(click()));
            grid->addWidget(btn, y, x);
        }
    }

    setLayout(grid);
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void MainBox::click(void)
{
    QToolButton *btn = reinterpret_cast<QToolButton *>(sender());
    if(btn == nullptr)
    {
        return;
    }
    btn->setText("*");
#if 0
    emit info(QString("%1:%2")
              .arg(btn->property("pos_x").toInt())
              .arg(btn->property("pos_y").toInt()));
#endif

#if 0
    emit info(QString("pos_x %1").arg(btn->property("pos_x").toInt()));
    emit info(QString("pos_y %1").arg(btn->property("pos_y").toInt()));
    emit info(QString("width %1").arg(btn->width()));
    emit info(QString("height %1").arg(btn->height()));
#endif
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test 0", &MainBox::test_0 });
    commands.append({ id++, "test 1", &MainBox::test_1 });
    commands.append({ id++, "test 2", &MainBox::test_2 });
    commands.append({ id++, "test 3", &MainBox::test_3 });
    commands.append({ id++, "test 4", &MainBox::test_4 });
    commands.append({ id++, "test 5", &MainBox::test_5 });
    commands.append({ id++, "test 6", nullptr });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block", this);
    testbar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::mouse_click(unsigned int button, QPoint pos)
{
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        emit error("display == nullptr");
        return;
    }

    QCursor::setPos(pos);
    XTestFakeButtonEvent( display, button, true, CurrentTime );

    XFlush( display );
    XCloseDisplay( display );
}
//--------------------------------------------------------------------------------
void MainBox::mouse_release(unsigned int button)
{
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return;
    }

    XTestFakeButtonEvent( display, button, false, CurrentTime );

    XFlush( display );
    XCloseDisplay( display );
}
//--------------------------------------------------------------------------------
void MainBox::mouse_move_to(QPoint pos)
{
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return;
    }

    XTestFakeMotionEvent( display, 0, pos.x(), pos.y(), CurrentTime );

    XFlush( display );
    XCloseDisplay( display );
}
//--------------------------------------------------------------------------------
bool MainBox::find_window(const QString &programm_title, int *x, int *y, int *width, int *heigth)
{
    Display* display = XOpenDisplay( nullptr );
    Q_CHECK_PTR(display);

    bool is_found = false;
    ulong count = 0;
    Window* wins = findWindows( display, &count );
    char* name = nullptr;
    QString temp;

    for( ulong i = 0; i < count; ++i )
    {
        Window w = wins[ i ];
        name = getWindowName( display, w );
        if( name )
        {
            temp.clear();
            temp.append(name);
            XFree( name );
        }

        XWindowAttributes attrs;
        if( XGetWindowAttributes( display, w, &attrs ) )
        {
            Window child;
            if( XTranslateCoordinates( display,
                                       w, attrs.root,
                                       0, 0,
                                       &attrs.x, &attrs.y,
                                       &child
                                       ) )
            {
                if(temp.contains(programm_title))
                {
                    *x = attrs.x;
                    *y = attrs.y;
                    *width = attrs.width;
                    *heigth = attrs.height;
                    is_found = true;
                }
            }
        }
    }

    if( wins )
    {
        XFree( wins );
    }

    XCloseDisplay( display );

    return is_found;
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

#if 1
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    int mb_x = mw->centralWidget()->x();
    int mb_y = mw->centralWidget()->y();

    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    bool ok = find_window("Test_mouse", &x, &y, &w, &h);
    if(ok)
    {
        for(int temp_y=0; temp_y<10; temp_y++)
        {
            for(int temp_x=0; temp_x<10; temp_x++)
            {
                int pos_x = x + mb_x + 16 + (32 * temp_x);
                int pos_y = y + mb_y + 16 + (32 * temp_y);

                QPoint pos = QPoint(pos_x, pos_y);
                //mouse_move_to(pos);
                mouse_click(Qt::LeftButton, pos);
                mouse_release(Qt::LeftButton);
            }
        }
    }
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
