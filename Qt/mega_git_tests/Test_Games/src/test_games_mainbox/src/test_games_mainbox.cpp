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
#include "ui_test_games_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_games_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include "other.hpp"
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
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    ui->le_kpat->setMinimumWidth(200);
    ui->le_kmines->setMinimumWidth(200);
    ui->le_kdiamond->setMinimumWidth(200);

    ui->le_kpat->setMaximumWidth(400);
    ui->le_kmines->setMaximumWidth(400);
    ui->le_kdiamond->setMaximumWidth(400);

    connect(ui->btn_kmines,         &QPushButton::clicked,  this,   &MainBox::run_kmines);
    connect(ui->btn_kpat,           &QPushButton::clicked,  this,   &MainBox::run_kpat);
    connect(ui->btn_kdiamond,       &QPushButton::clicked,  this,   &MainBox::run_kdiamond);
    connect(ui->btn_find_kpat,      &QPushButton::clicked,  this,   &MainBox::find_kpat);
    connect(ui->btn_find_kmines,    &QPushButton::clicked,  this,   &MainBox::find_kmines);
    connect(ui->btn_find_kdiamond,  &QPushButton::clicked,  this,   &MainBox::find_kdiamond);

    load_widgets();
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

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
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
void MainBox::run_kmines(void)
{
    QString program = "kmines";
    QString program_name = "KMines";
    QStringList arguments;

    run_program(program, program_name, arguments);
    ui->le_kmines->setText(program_name);
}
//--------------------------------------------------------------------------------
void MainBox::run_kpat(void)
{
    QString program = "kpat";
    QString program_name = "KPatience";
    QStringList arguments;

    run_program(program, program_name, arguments);
    ui->le_kpat->setText(program_name);
}
//--------------------------------------------------------------------------------
void MainBox::run_kdiamond(void)
{
    QString program = "kdiamond";
    QString program_name = "KDiamond";
    QStringList arguments;

    run_program(program, program_name, arguments);
    ui->le_kdiamond->setText(program_name);
}
//--------------------------------------------------------------------------------
void MainBox::run_program(const QString &program,
                          const QString &program_name,
                          const QStringList &arguments)
{
    if(program.isEmpty())
    {
        emit error("program is empty");
        return;
    }
    if(program_name.isEmpty())
    {
        emit error("program_name is empty");
        return;
    }

    QProcess *myProcess = new QProcess(this);
    connect(myProcess,  &QProcess::started,     this,   &MainBox::started);

    connect(myProcess,  static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this,       &MainBox::finished);
    connect(myProcess,  &QProcess::errorOccurred,
            this,       &MainBox::process_error);
    myProcess->start(program, arguments);

//    ui->le_programm->setText(program_name);
}
//--------------------------------------------------------------------------------
bool MainBox::find_window(const QString &programm_title,
                          int *x,
                          int *y,
                          int *width,
                          int *heigth)
{
#ifdef Q_OS_LINUX
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
#else
    return false;
#endif
}
//--------------------------------------------------------------------------------
void MainBox::find_kpat(void)
{
    if(ui->le_kpat->text().isEmpty())
    {
        messagebox_critical("Ошибка", "Название программы не выбрано");
        return;
    }

    QString title = ui->le_kpat->text();
    QScreen *screen = QGuiApplication::primaryScreen();

    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    bool ok = find_window(title, &x, &y, &w, &h);

    // KMimes 30 * 16
    if(ok)
    {
        QPixmap screen_shot = screen->grabWindow(0, x, y, w, h);
        screen_shot.save(QString("%1.png").arg(title));

#if 0
        int temp_x = 36;
        int temp_y = 91;

        QPainter paint;
        paint.begin(&screen_shot);

        for(int n=0; n<=16; n++)
        {
            paint.drawLine ( 0, temp_y, w, temp_y );
            temp_y+=24;
        }
        for(int n=0; n<=30; n++)
        {
            paint.drawLine ( temp_x, 0, temp_x, h );
            temp_x+=24;
        }

        paint.end();
#endif

        QLabel *lbl = new QLabel;
        lbl->setPixmap(screen_shot);
        lbl->installEventFilter(this);
        lbl->show();
    }
    else
    {
        messagebox_warning("Error", QString("%1 not found!").arg(title));
    }
}
//--------------------------------------------------------------------------------
void MainBox::find_kmines(void)
{
    if(ui->le_kpat->text().isEmpty())
    {
        messagebox_critical("Ошибка", "Название программы не выбрано");
        return;
    }

    QString title = ui->le_kmines->text();
    QScreen *screen = QGuiApplication::primaryScreen();

    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    bool ok = find_window(title, &x, &y, &w, &h);

    // KMimes 30 * 16
    if(ok)
    {
        QPixmap screen_shot = screen->grabWindow(0, x, y, w, h);
        screen_shot.save(QString("%1.png").arg(title));

#if 1
        int temp_x = 36;
        int temp_y = 91;

        QPainter paint;
        paint.begin(&screen_shot);

        for(int n=0; n<=16; n++)
        {
            paint.drawLine ( 0, temp_y, w, temp_y );
            temp_y+=24;
        }
        for(int n=0; n<=30; n++)
        {
            paint.drawLine ( temp_x, 0, temp_x, h );
            temp_x+=24;
        }

        paint.end();
#endif

        QLabel *lbl = new QLabel;
        lbl->setPixmap(screen_shot);
        lbl->installEventFilter(this);
        lbl->show();
    }
    else
    {
        messagebox_warning("Error", QString("%1 not found!").arg(title));
    }
}
//--------------------------------------------------------------------------------
void MainBox::find_kdiamond(void)
{
    if(ui->le_kpat->text().isEmpty())
    {
        messagebox_critical("Ошибка", "Название программы не выбрано");
        return;
    }

    QString title = ui->le_kdiamond->text();
    QScreen *screen = QGuiApplication::primaryScreen();

    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    bool ok = find_window(title, &x, &y, &w, &h);

    // KMimes 30 * 16
    if(ok)
    {
        QPixmap screen_shot = screen->grabWindow(0, x, y, w, h);
        screen_shot.save(QString("%1.png").arg(title));

#if 0
        int temp_x = 36;
        int temp_y = 91;

        QPainter paint;
        paint.begin(&screen_shot);

        for(int n=0; n<=16; n++)
        {
            paint.drawLine ( 0, temp_y, w, temp_y );
            temp_y+=24;
        }
        for(int n=0; n<=30; n++)
        {
            paint.drawLine ( temp_x, 0, temp_x, h );
            temp_x+=24;
        }

        paint.end();
#endif

        QLabel *lbl = new QLabel;
        lbl->setPixmap(screen_shot);
        lbl->installEventFilter(this);
        lbl->show();
    }
    else
    {
        messagebox_warning("Error", QString("%1 not found!").arg(title));
    }
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
    emit info(tr("Процесс начат!"));
}
//--------------------------------------------------------------------------------
void MainBox::finished(int result, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);

    emit info(tr("Процесс завершен!"));
    if(result)
    {
        emit error(QString(tr("code %1")).arg(result));
    }
}
//--------------------------------------------------------------------------------
void MainBox::process_error(QProcess::ProcessError p_error)
{
    switch(p_error)
    {
    case QProcess::FailedToStart:
        emit error("FailedToStart");
        break;

    case QProcess::Crashed:
        emit error("Crashed");
        break;

    case QProcess::Timedout:
        emit error("Timedout");
        break;

    case QProcess::ReadError:
        emit error("ReadError");
        break;

    case QProcess::WriteError:
        emit error("WriteError");
        break;

    case QProcess::UnknownError:
        emit error("UnknownError");
        break;
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");
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
