/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include "for_tests_mainbox.hpp"
#include "mymainwindow.hpp"
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
QFile *MyMainWindow::m_logFile = nullptr;
//--------------------------------------------------------------------------------
MyMainWindow::MyMainWindow(QWidget *parent) :
    MainWindow(parent)
{
    //m_logFile = new QFile(QString("%1.log").arg(qAppName()));
    //m_logFile->open(QFile::Append | QFile::Text);

#ifdef HAVE_QT5
    //qInstallMessageHandler(messageHandler);
#else
    //qInstallMsgHandler(messageHandler);
#endif

#ifndef NO_MENU
    create_local_menus();
#endif
    create_toolbars();

    setUnifiedTitleAndToolBarOnMac(true);

    //TODO не надо отсюда это убирать
    load_setting();
}
//--------------------------------------------------------------------------------
MyMainWindow::~MyMainWindow()
{
#ifdef QT_DEBUG
    qDebug() << "~NyMainWindow()";
#endif
    //TODO не надо отсюда это убирать
    save_setting();

    if(file_action)     delete file_action;
    if(options_action)  delete options_action;
    if(help_action)     delete help_action;
}
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
void MyMainWindow::messageHandler(QtMsgType type,
                                  const QMessageLogContext &context,
                                  const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile);
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение

    switch (type)
    {
    case QtInfoMsg:     out << "INF ";  break;
    case QtDebugMsg:    out << "DBG ";  break;
    case QtWarningMsg:  out << "WRN ";  break;
    case QtCriticalMsg: out << "CRT ";  break;
    case QtFatalMsg:    out << "FTL ";  break;
    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category
        << ": "
        << msg
        << Qt::endl;
    out.flush();    // Очищаем буферизированные данные
}
#else
void MyMainWindow::messageHandler(QtMsgType type,
                                  const char *msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile);
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
    case QtDebugMsg:    out << "DBG ";  break;
    case QtWarningMsg:  out << "WRN ";  break;
    case QtCriticalMsg: out << "CRT ";  break;
    case QtFatalMsg:    out << "FTL ";  break;
    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << ": "
        << msg
        << Qt::endl;
    out.flush();    // Очищаем буферизированные данные
}
#endif
//--------------------------------------------------------------------------------
void MyMainWindow::create_local_menus(void)
{
    file_action = new QAction(this);
    file_action->setProperty(P_APP_ENG_TEXT, "file_action");
    file_action->setText("file_action");
    file_action->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    options_action = new QAction(this);
    options_action->setProperty(P_APP_ENG_TEXT, "options_action");
    options_action->setText("options_action");
    options_action->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    help_action = new QAction(this);
    help_action->setProperty(P_APP_ENG_TEXT, "help_action");
    help_action->setText("help_action");
    help_action->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(file_action,    &QAction::triggered,    this,   &MyMainWindow::file_action_click);
    connect(options_action, &QAction::triggered,    this,   &MyMainWindow::options_action_click);
    connect(help_action,    &QAction::triggered,    this,   &MyMainWindow::help_action_click);

#ifndef NO_MENU
    add_filemenu_action(0,      file_action);
    add_optionsmenu_action(0,   options_action);
    add_helpmenu_action(0,      help_action);

    add_filemenu_separator(1);
    add_optionsmenu_separator(1);
    add_helpmenu_separator(1);
#endif
}
//--------------------------------------------------------------------------------
void MyMainWindow::create_toolbars(void)
{
    QToolBar *toolBar = new QToolBar("tb", this);
    Q_ASSERT(toolBar);

    toolBar->setObjectName("tb");

    addToolBar(Qt::TopToolBarArea, toolBar);

    for(int n=0; n<10; n++)
    {
        QToolButton *btn = new QToolButton;
        btn->setIcon(QIcon(QString(":/red/%1.png").arg(n)));
        btn->setObjectName(QString("red_btn_%1").arg(n));
        btn->setToolTip(QString("%1").arg(n));
        btn->setProperty("value", n);
        connect(btn,    &QToolButton::clicked,  this,   &MyMainWindow::click_btn);
        toolBar->addWidget(btn);
    }

    toolBar->setMovable(true);
//#ifndef NO_MENU
//    add_windowsmenu_action(toolBar, toolBar->toggleViewAction());
//#endif

    //---

    QToolBar *toolBar2 = new QToolBar("tb2", this);
    Q_ASSERT(toolBar2);

    toolBar2->setObjectName("tb2");

    addToolBar(Qt::TopToolBarArea, toolBar2);

    for(int n=0; n<10; n++)
    {
        QToolButton *btn = new QToolButton;
        btn->setIcon(QIcon(QString(":/white/%1.png").arg(n)));
        btn->setObjectName(QString("white_btn_%1").arg(n));
        btn->setToolTip(QString("%1").arg(n));
        btn->setProperty("value", n);
        connect(btn,    &QToolButton::clicked,  this,   &MyMainWindow::click_btn);
        toolBar2->addWidget(btn);
    }

    toolBar2->setMovable(true);
//#ifndef NO_MENU
//    add_windowsmenu_action(toolBar2, toolBar2->toggleViewAction());
//#endif
}
//--------------------------------------------------------------------------------
void MyMainWindow::click_btn(void)
{
    QToolButton *btn = dynamic_cast<QToolButton *>(sender());
    if(btn)
    {
        emit info(QString("%1").arg(btn->property("value").toInt()));
    }
}
//--------------------------------------------------------------------------------
void MyMainWindow::file_action_click(void)
{
    QString title = "Информация";
    QString text = "file_action_click";
    MyWidget::messagebox_info(title, text);
}
//--------------------------------------------------------------------------------
void MyMainWindow::options_action_click(void)
{
    QString title = "Информация";
    QString text = "options_action_click";
    MyWidget::messagebox_info(title, text);
}
//--------------------------------------------------------------------------------
void MyMainWindow::help_action_click(void)
{
    QString title = "Информация";
    QString text = "help_action_click";
    MyWidget::messagebox_info(title, text);
}
//--------------------------------------------------------------------------------
void MyMainWindow::setCentralWidget(MyWidget *widget)
{
    MyMainBox *w = new MyMainBox();

    HorizontalWidget *t_widget = new HorizontalWidget;
    t_widget->setFixedHeight(W_SIZE);
    t_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    HorizontalWidget *b_widget = new HorizontalWidget;
    b_widget->setFixedHeight(W_SIZE);
    b_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    VerticalWidget *l_widget = new VerticalWidget;
    l_widget->setFixedWidth(W_SIZE);
    l_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    VerticalWidget *r_widget = new VerticalWidget;
    r_widget->setFixedWidth(W_SIZE);
    r_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    LeftTopWidget *lt_box = new LeftTopWidget;
    lt_box->setFixedSize(W_SIZE, W_SIZE);
    lt_box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    RightTopWidget *rt_box = new RightTopWidget;
    rt_box->setFixedSize(W_SIZE, W_SIZE);
    rt_box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    LeftBottomWidget *lb_box = new LeftBottomWidget;
    lb_box->setFixedSize(W_SIZE, W_SIZE);
    lb_box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    RightBottomWidget *rb_box = new RightBottomWidget;
    rb_box->setFixedSize(W_SIZE, W_SIZE);
    rb_box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(0);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->addWidget(lt_box,   0, 0);    grid->addWidget(t_widget, 0, 1);  grid->addWidget(rt_box,   0, 2);
    grid->addWidget(l_widget, 1, 0);    grid->addWidget(widget, 1, 1);    grid->addWidget(r_widget, 1, 2);
    grid->addWidget(lb_box,   2, 0);    grid->addWidget(b_widget, 2, 1);  grid->addWidget(rb_box,   2, 2);

    QToolButton *btn = new QToolButton();
    btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    btn->setCheckable(true);
    connect(btn,    &QToolButton::toggled,  widget, &MyWidget::setHidden);
    btn->setText("#");
    grid->addWidget(btn, 1, 3);

    w->setLayout(grid);

    MainWindow::setCentralWidget(w);
    MainWindow::set_c_widget(widget);   //TODO проба
}
//--------------------------------------------------------------------------------
//void MyMainWindow::changeEvent(QEvent *event)
//{
//    if(event == nullptr)
//    {
//        return;
//    }

//    switch (event->type())
//    {
//    case QEvent::LanguageChange:
//        file_action->setText("1");
//        options_action->setText("2");
//        help_action->setText("3");
//        break;

//    default:
//        QWidget::changeEvent(event);
//        break;
//    }
//    MainWindow::changeEvent(event);
//}
//--------------------------------------------------------------------------------
void MyMainWindow::closeEvent(QCloseEvent *event)
{
    //type here
    MainWindow::closeEvent(event);
}
//--------------------------------------------------------------------------------
