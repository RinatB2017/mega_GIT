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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mymainwindow.hpp"
#include "mywidget.hpp"

#include "defines.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
QFile *MyMainWindow::m_logFile = 0;
//--------------------------------------------------------------------------------
MyMainWindow::MyMainWindow(MainWindow *parent) :
    MainWindow(parent)
{
    m_logFile = new QFile(QString("%1.log").arg(qAppName()));
    m_logFile->open(QFile::Append | QFile::Text);

#ifdef HAVE_QT5
    qInstallMessageHandler(messageHandler);
#else
    qInstallMsgHandler(messageHandler);
#endif

    create_local_menus();
    create_toolbars();
}
//--------------------------------------------------------------------------------
MyMainWindow::~MyMainWindow()
{
    qDebug() << "~NyMainWindow()";
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
        << endl;
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
        << endl;
    out.flush();    // Очищаем буферизированные данные
}
#endif
//--------------------------------------------------------------------------------
#if 0
void MyMainWindow::closeEvent(QCloseEvent *event)
{
    int btn =  QMessageBox::question(this,
                                     "Выход из программы",
                                     "Вы уверены, что хотите выйти из программы?",
                                     QMessageBox::Yes | QMessageBox::No);
    if(btn == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
#endif
//--------------------------------------------------------------------------------
void MyMainWindow::create_local_menus(void)
{
    QAction *file_action = new QAction(this);
    file_action->setText(QObject::tr("file_action"));
    file_action->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    QAction *options_action = new QAction(this);
    options_action->setText(QObject::tr("options_action"));
    options_action->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    QAction *help_action = new QAction(this);
    help_action->setText(QObject::tr("help_action"));
    help_action->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(file_action,    SIGNAL(triggered()),    this,   SLOT(file_action_click()));
    connect(options_action, SIGNAL(triggered()),    this,   SLOT(options_action_click()));
    connect(help_action,    SIGNAL(triggered()),    this,   SLOT(help_action_click()));

    QList<QAction *> f_temp = get_file_menu()->actions();
    get_file_menu()->addAction(file_action);
    get_file_menu()->addSeparator();
    get_file_menu()->addActions(f_temp);

    QList<QAction *> f_options = get_options_menu()->actions();
    get_options_menu()->addAction(options_action);
    get_options_menu()->addSeparator();
    get_options_menu()->addActions(f_options);

    QList<QAction *> f_help = get_help_menu()->actions();
    get_help_menu()->addAction(help_action);
    get_help_menu()->addSeparator();
    get_help_menu()->addActions(f_help);
}
//--------------------------------------------------------------------------------
void MyMainWindow::create_toolbars(void)
{
    QToolBar *toolBar = new QToolBar("tb", this);
    if(toolBar == nullptr)
    {
        return;
    }

    toolBar->setObjectName("tb");

    addToolBar(Qt::TopToolBarArea, toolBar);

    for(int n=0; n<10; n++)
    {
        QToolButton *btn = new QToolButton;
        btn->setIcon(QIcon(QString(":/red/%1.png").arg(n)));
        btn->setProperty("value", n);
        connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(click_btn()));
        toolBar->addWidget(btn);
    }

    toolBar->setMovable(true);

    //---

    QToolBar *toolBar2 = new QToolBar("tb2", this);
    if(toolBar2 == nullptr)
    {
        return;
    }

    toolBar2->setObjectName("tb2");

    addToolBar(Qt::TopToolBarArea, toolBar2);

    for(int n=0; n<10; n++)
    {
        QToolButton *btn = new QToolButton;
        btn->setIcon(QIcon(QString(":/white/%1.png").arg(n)));
        btn->setProperty("value", n);
        connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(click_btn()));
        toolBar2->addWidget(btn);
    }

    toolBar2->setMovable(true);
}
//--------------------------------------------------------------------------------
void MyMainWindow::click_btn(void)
{
    QToolButton *btn = dynamic_cast<QToolButton *>(sender());
    if(btn == nullptr)
    {
        return;
    }
    emit info(QString("%1").arg(btn->property("value").toInt()));
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
#define W_SIZE 50
void MyMainWindow::setCentralWidget(QWidget *widget)
{
    QWidget *w = new QWidget;

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
    grid->setMargin(0);
    grid->addWidget(lt_box, 0, 0);    grid->addWidget(t_widget, 0, 1);  grid->addWidget(rt_box, 0, 2);
    grid->addWidget(l_widget, 1, 0);  grid->addWidget(widget, 1, 1);    grid->addWidget(r_widget, 1, 2);
    grid->addWidget(lb_box, 2, 0);    grid->addWidget(b_widget, 2, 1);  grid->addWidget(rb_box, 2, 2);
    w->setLayout(grid);

    MainWindow::setCentralWidget(w);
}
//--------------------------------------------------------------------------------
