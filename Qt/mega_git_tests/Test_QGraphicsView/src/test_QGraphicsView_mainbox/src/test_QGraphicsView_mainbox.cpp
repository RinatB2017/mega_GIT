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
#include "ui_test_QGraphicsView_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_QGraphicsView_mainbox.hpp"
#include "defines.hpp"
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

    setFocusPolicy(Qt::StrongFocus);

    create_scene_0();
    create_scene_1();
    create_scene_2();

    ui->graphicsView->setScene(scene_0);
    ui->graphicsView->setFixedSize(ui->graphicsView->sizeHint());

    pos_x = ui->graphicsView->width() / 2;
    pos_y = ui->graphicsView->height() / 2;

    min_x = 10;
    max_x = ui->graphicsView->width() - 32 - 10;
    min_y = 10;
    max_y = ui->graphicsView->height() - 32 - 10;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();
    //---

#if 1
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::create_scene_0(void)
{
    scene_0 = new QGraphicsScene();
    scene_0->setSceneRect(0, 0, 500, 500);

    rect_0 = scene_0->addRect(100, 100, 100, 100, QPen(Qt::red), QBrush(Qt::green));

    item = scene_0->addPixmap(QPixmap(ICON_PROGRAMM));
    item->moveBy(250,250);
}
//--------------------------------------------------------------------------------
void MainBox::create_scene_1(void)
{
    QFont font("Liberation Mono", 24);

    scene_1 = new QGraphicsScene();
    scene_1->setSceneRect(0, 0, 500, 500);

    QGraphicsRectItem *rect_1 = scene_1->addRect(0, 0, 100, 100, QPen(Qt::NoPen), QBrush(Qt::red));
    rect_1->setRotation(10);

    QGraphicsTextItem *item_1 = scene_1->addText("scene_1");
    item_1->setFont(font);
    item_1->setPos(250, 250);
    item_1->setRotation(-45);
}
//--------------------------------------------------------------------------------
void MainBox::create_scene_2(void)
{
    QFont font("Liberation Mono", 24);

    scene_2 = new QGraphicsScene();
    scene_2->setSceneRect(0, 0, 500, 500);
    scene_2->addRect(0, 0, 100, 100, QPen(Qt::NoPen), QBrush(Qt::green));

    QGraphicsTextItem *item_2 = scene_2->addText("scene_2");
    item_2->setFont(font);
    item_2->setPos(250, 250);
    item_2->setRotation(-15);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test 0", &MainBox::test_0 });
    commands.append({ id++, "test 1", &MainBox::test_1 });
    commands.append({ id++, "test 2", &MainBox::test_2 });
    commands.append({ id++, "test 3", &MainBox::test_3 });
    commands.append({ id++, "test 4", &MainBox::test_4 });
    commands.append({ id++, "test 5", &MainBox::test_5 });
    commands.append({ id++, "test 6", nullptr });

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QCheckBox *cb_block = new QCheckBox("block");
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

    testbar->setFixedWidth(testbar->sizeHint().width() + 10);
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;
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
void MainBox::up(void)
{
    pos_y -= delta;
}
//--------------------------------------------------------------------------------
void MainBox::down(void)
{
    pos_y += delta;
}
//--------------------------------------------------------------------------------
void MainBox::left(void)
{
    pos_x -= delta;
}
//--------------------------------------------------------------------------------
void MainBox::right(void)
{
    pos_x += delta;
}
//--------------------------------------------------------------------------------
void MainBox::update(void)
{
    qreal old_pos_x = pos_x;
    qreal old_pos_y = pos_y;

    if(flag_up)     pos_y -= delta;
    if(flag_down)   pos_y += delta;
    if(flag_left)   pos_x -= delta;
    if(flag_right)  pos_x += delta;

    if(pos_x < min_x)   pos_x = min_x;
    if(pos_x > max_x)   pos_x = max_x;
    if(pos_y < min_y)   pos_y = min_y;
    if(pos_y > max_y)   pos_y = max_y;

#if 0
    emit info(QString("new_pos_x %1 new_pos_y %2")
              .arg(new_pos_x)
              .arg(new_pos_y));
#endif

    if(item)
    {
        item->setPos(pos_x, pos_y);
        bool ok = item->collidesWithItem(rect_0);
        if(ok)
        {
            pos_x = old_pos_x;
            pos_y = old_pos_y;
            item->setPos(pos_x, pos_y);
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    lock_this_button();

    ui->graphicsView->setScene(scene_0);
    setFocus();

    QElapsedTimer time;

    time.start();
    int max_n = 100000; //миллион уже много - все тормозит
    for(int n=0; n<max_n; n++)
    {
        scene_0->addRect(rand() % 500, rand() % 500, 50, 50);
    }
    emit info(QString("created %1 items").arg(max_n));
    emit info(QString("elapsed %1 msec").arg(time.elapsed()));

    unlock_this_button();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    ui->graphicsView->setScene(scene_1);
    setFocus();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    ui->graphicsView->setScene(scene_2);
    setFocus();

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
bool MainBox::eventFilter(QObject *, QEvent *e)
{
    if (e->type() == QEvent::KeyPress)
    {

    }
    if (e->type() == QEvent::MouseButtonRelease)
    {
        if (static_cast<QMouseEvent*>(e)->button() == Qt::RightButton)
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:    flag_up    = true;  break;
    case Qt::Key_Down:  flag_down  = true;  break;
    case Qt::Key_Left:  flag_left  = true;  break;
    case Qt::Key_Right: flag_right = true;  break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:    flag_up    = false;  break;
    case Qt::Key_Down:  flag_down  = false;  break;
    case Qt::Key_Left:  flag_left  = false;  break;
    case Qt::Key_Right: flag_right = false;  break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
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
