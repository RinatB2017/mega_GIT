/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MainBox_GUI(parent),
    splash(splash)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    createTestBar();

    // add_lcd_clock();
    // add_digital_clock(false);
    add_digital_clock();

    load_widgets();
}
//--------------------------------------------------------------------------------
bool MainBox::set_theme_windows(void)
{
    QString filename = ":/themes_css/Theme (Windows).css";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error(QString("file [%1] not open").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::set_norton_commander(void)
{
    QString filename = ":/themes_qss/Norton Commander.qss";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error(QString("file [%1] not open").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::set_styles(void)
{
    QString filename = ":/themes_qss/styles.qss";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error(QString("file [%1] not open").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "run_cube_widget",      &MainBox::run_cube_widget });
    commands.append({ id++, "run_ogl_widget",       &MainBox::run_ogl_widget });
    commands.append({ id++, "test",                 &MainBox::test });
    commands.append({ id++, "test2",                &MainBox::test2 });
    commands.append({ id++, "Theme (Windows).css",  &MainBox::set_theme_windows });
    commands.append({ id++, "Norton Commander.qss", &MainBox::set_norton_commander });
    commands.append({ id++, "styles.qss",           &MainBox::set_styles });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block");
    cb_block->setObjectName("cb_block");
    testbar->addWidget(cb_block);

    sb_test = new QSpinBox(this);
    sb_test->setObjectName("sb_test");
    testbar->addWidget(sb_test);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    cb_test->setProperty(NO_SAVE, true);
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

    connect(btn_choice_test,    &QToolButton::clicked,  this,   &MainBox::choice_test);

    connect(cb_block,   &QCheckBox::clicked,    cb_test,            &QComboBox::setDisabled);
    connect(cb_block,   &QCheckBox::clicked,    btn_choice_test,    &QComboBox::setDisabled);
    connect(cb_block,   &QCheckBox::clicked,    sb_test,            &QComboBox::setDisabled);

    // testbar->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    //#ifndef NO_MENU
    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
    //#endif
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
    else
    {
        emit error("not found");
    }
}
//--------------------------------------------------------------------------------
template<typename T1, typename T2>
void MainBox::test_template(void)
{
    QList<T1 *> allle = findChildren<T1 *>();
    foreach (T1 *obj, allle)
    {
        obj->setDisabled(true);
    }
    QList<T2 *> allle2 = findChildren<T2 *>();
    foreach (T2 *obj, allle2)
    {
        obj->setDisabled(true);
    }
}
//--------------------------------------------------------------------------------
template<class T>
void MainBox::test_function(int delay)
{
    QTimer::singleShot(delay, [this]{
        T::test2();
    });
}
//--------------------------------------------------------------------------------
void MainBox::test_function2(bool (MainBox::*func)(void))
{
    typedef bool (MainBox::*function)(void);
    function f;
    f = func;

    block_interface(true);
    (this->*f)();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::heavy_function(int x)
{
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < x)
    {

    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
bool MainBox::calc_norm(qreal x,
                        qreal y,
                        qreal w,
                        qreal h,
                        qreal *norm_x,
                        qreal *norm_y)
{
    if(x<0 || y<0) return false;
    if(w<0 || h<0) return false;
    if(x>w || y>h) return false;

    *norm_x = (x * 2.0) / w - 1.0;
    *norm_y = (y * 2.0) / h - 1.0;
    return true;
}
//--------------------------------------------------------------------------------
QImage MainBox::create_bone(int num)
{
    QImage image = QImage(512, 512, QImage::Format_ARGB32);
    image.fill(QColor(Qt::white));

    QPen pen;
    pen.setColor(QColor(Qt::black));
    pen.setWidth(5);

    QPainter painter;
    painter.begin(&image);
    painter.setPen(pen);
    painter.drawRect(0, 0, 511, 511);
    painter.setFont(QFont("Libaration Mono", 70));
    painter.drawText(250, 280, QString::number(num));
    painter.end();

    return image;
}
//--------------------------------------------------------------------------------
#include <QDateTime>
#include <QDate>

#include "memories.hpp"
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 1
    Memories *mem = new Memories();
    connect_log_signals(mem, this);
    mem->elapsed_time(5887 * 1000);
    delete  mem;
#endif

#if 0
    emit info(QString("size:  %1").arg(sizeof(packet)));
    emit info(QString("size2: %1").arg(sizeof(packet2)));
#endif

#if 0
    QImage image_1 = create_bone(1);
    QImage image_2 = create_bone(2);
    QImage image_3 = create_bone(3);
    QImage image_4 = create_bone(4);
    QImage image_5 = create_bone(5);
    QImage image_6 = create_bone(6);

    QImage bone = QImage(512*4, 512*3, QImage::Format_ARGB32);
    QPainter p;
    int x=0;
    int y=0;
    p.begin(&bone);
    x=512; y=0;
    p.drawImage(x, y, image_6);
    x=0; y=512;
    p.drawImage(x, y, image_1);
    x=512; y=512;
    p.drawImage(x, y, image_2);
    x=1024; y=512;
    p.drawImage(x, y, image_5);
    x=1536; y=512;
    p.drawImage(x, y, image_4);
    x=512; y=1024;
    p.drawImage(x, y, image_3);
    p.end();

    bone.save("/dev/shm/bone2.png");

//    QLabel *label = new QLabel();
//    label->setPixmap(QPixmap::fromImage(bone));
//    label->setFixedSize(512*4, 512*3);
//    label->show();
#endif


#if 0
    QImage image_1 = create_bone(1);
    QImage image_2 = create_bone(2);
    QImage image_3 = create_bone(3);
    QImage image_4 = create_bone(4);
    QImage image_5 = create_bone(5);
    QImage image_6 = create_bone(6);

    QImage bone = QImage(512*6, 512, QImage::Format_ARGB32);
    QPainter p;
    int x=0;
    p.begin(&bone);
    p.drawImage(x, 0, image_6); x+=512;
    p.drawImage(x, 0, image_1); x+=512;
    p.drawImage(x, 0, image_2); x+=512;
    p.drawImage(x, 0, image_5); x+=512;
    p.drawImage(x, 0, image_4); x+=512;
    p.drawImage(x, 0, image_3);
    p.end();

    bone.save("/dev/shm/bone.png");

//    QLabel *label = new QLabel();
//    label->setPixmap(QPixmap::fromImage(bone));
//    label->setFixedSize(512*6, 512);
//    label->show();
#endif

#if 0
    qreal pos_x;
    qreal pos_y;

    if(calc_norm(400, 400, 600, 600, &pos_x, &pos_y))
    {
        emit info(QString("%1 %2")
                  .arg(pos_x)
                  .arg(pos_y));
    }
#endif

#if 0
    RotateWidget *rw = new RotateWidget();
    rw->show();
#endif

#if 0
    QColor color = QColor(Qt::yellow);
    int h, s, v;
    color.getHsv(&h, &s, &v);
    emit info(QString("H: %1").arg(h));
    emit info(QString("S: %1").arg(s));
    emit info(QString("V: %1").arg(v));
#endif

#if 0
    Memories *mem = new Memories();
    connect_log_signals(mem, this);

    mem->set_directory();
#endif

#if 0
    emit info("Copyright \\251 2020-2025");
#endif

#if 0
    emit info("Info");
    emit debug("Debug");
    emit error("Error");
    emit trace("Trace");
    emit colorLog("yellow on blue", Qt::yellow, Qt::blue);
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test2(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test2");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::run_cube_widget(void)
{
    emit trace(Q_FUNC_INFO);

    CubeWidget *cw = new CubeWidget();
    cw->show();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::run_ogl_widget(void)
{
    emit trace(Q_FUNC_INFO);

    ORL_widget *ow = new ORL_widget();
    ow->show();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{
#ifdef QT_DEBUG
    qDebug() << "MainBox::load_setting";
#endif

    if(sb_test)
    {
        int value = 0;
        if(load_int("sb_test", &value))
        {
            sb_test->setValue(value);
        }
    }
    if(cb_block)
    {
        int value = 0;
        load_int("cb_block", &value);
        bool block_is_checked = value;
        //cb_block->clicked(block_is_checked);
        cb_block->setChecked(block_is_checked);
    }
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
#ifdef QT_DEBUG
    qDebug() << "MainBox::save_setting";
#endif

    if(sb_test)
    {
        save_int("sb_test", sb_test->value());
    }
    if(cb_block)
    {
        save_int("cb_block", cb_block->isChecked());
    }
}
//--------------------------------------------------------------------------------
