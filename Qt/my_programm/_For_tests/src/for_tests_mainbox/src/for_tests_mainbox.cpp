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
    create_test_bar();

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
void MainBox::create_test_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
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
    }
    else
    {
        emit error("mw not found!");
    }
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
void MainBox::heavy_function(bool *result)
{
    QElapsedTimer timer;
    double res = 0;

    emit info("heavy_function started");
    timer.start();
    for(int y=0; y<100000; y++)
    {
        for(int x=0; x<10000; x++)
        {
            res += (qSin(x) * qCos(y));
        }
    }
    emit info(QString("res: %1").arg(res));
    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
    emit info("heavy_function function");

    *result = true;
}
//--------------------------------------------------------------------------------
void MainBox::calc_line(qreal center_x,
                        qreal center_y,
                        qreal angle,
                        qreal radius,
                        qreal *end_x,
                        qreal *end_y)
{
    //qDebug() << "calc_line" << center_x << center_y << angle << radius;

    qreal A = radius;
    qreal B = qCos(qDegreesToRadians(angle)) * A;
    qreal C = qSin(qDegreesToRadians(angle)) * A;

    //qDebug() << center_x + B << center_y + C;

    *end_x = center_x + B;
    *end_y = center_y + C;
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
    painter.setFont(QFont("Liberation Mono", 70));
    painter.drawText(250, 280, QString::number(num));
    painter.end();

    return image;
}
//--------------------------------------------------------------------------------
#include <QtConcurrent>
#include "connection.hpp"
#include "memories.hpp"

#include "test_class.hpp"
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 1
    emit info("Text to speech");
    qDebug() << sp.availableVoices();
    // sp.setEngine("sapi");  // Попробуйте сначала sapi, winrt глючит
    // sp.setVoice(sp.availableVoices()[0]);  // Обязательно!
    sp.setVolume(1.0);
    sp.say("Hello!");
#endif

#if 0
    char *text = { (char)0x00, (char)0x10, (char)0x20, (char)0x30, (char)0x40, (char)0x50, (char)0x60, (char)0x70 };
#endif

#if 0
    QByteArray ba;
    ba.append(":000102030405\n");

    QByteArray ba_res = ba.fromHex(ba);
    emit info(QString("len: %1").arg(ba_res.length()));
    emit info(QString("3: %1").arg(ba_res.at(3), 2, 16, QChar('0')));
#endif

#if 0
    emit info("thread started");
    bool r0 = false;
    bool r1 = false;
    bool r2 = false;
    bool r3 = false;
    bool r4 = false;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QtConcurrent::run(this, &MainBox::heavy_function, &r0);
    QtConcurrent::run(this, &MainBox::heavy_function, &r1);
    QtConcurrent::run(this, &MainBox::heavy_function, &r2);
    QtConcurrent::run(this, &MainBox::heavy_function, &r3);
    QtConcurrent::run(this, &MainBox::heavy_function, &r4);
#else
    QtConcurrent::run(std::bind(&MainBox::heavy_function, this, &r0));
    QtConcurrent::run(std::bind(&MainBox::heavy_function, this, &r1));
    QtConcurrent::run(std::bind(&MainBox::heavy_function, this, &r2));
    QtConcurrent::run(std::bind(&MainBox::heavy_function, this, &r3));
    QtConcurrent::run(std::bind(&MainBox::heavy_function, this, &r4));
#endif
    emit info("thread finished");

    QElapsedTimer timer;
    timer.start();
    while(!r0 ||
          !r1 ||
          !r2 ||
          !r3 ||
          !r4)
    {
        QCoreApplication::processEvents();
    }
    emit error(QString("Time threads elapsed %1 msec").arg(timer.elapsed()));
#endif

#if 0
    double value = 0.000001;
    double freq = 5.0;

    //value = 1.23;
    if(value < 0.01)
        emit info(QString("UA%1F%2")
                  .arg(value, 0, 'f')
                  .arg(freq));
    else
        emit info(QString("UA%1F%2")
                  .arg(value)
                  .arg(freq));
#endif

#if 0
    QString temp;
    temp = "QLineEdit{ " /
            "border-width: 1px; "/
            "border-style: solid; "/
            "border-color: red white black black ;"/
            "border-top-style:none; "/
            "}";
    //qApp->setStyleSheet( "QTextEdit{ border-width: 1px; border-style: solid; border-color:  red white black black;border-top-style:none; }" );
    qApp->setStyleSheet( temp );
#endif

#if 0
    Test_class *tc = new Test_class();
    connect_log_signals(tc, this);
    tc->show();
#endif

#if 0
    Connection::set_value(1.666);
#endif

#if 0
    QImage image(300, 300, QImage::Format_ARGB32);
    image.fill(QColor(Qt::black));

    QPointF points[360];
    qreal radius = 50;
    for(int n=0; n<360; n++)
    {
        qreal end_x;
        qreal end_y;
        calc_line(150, 150, n, radius, &end_x, &end_y);
        points[n] = QPointF(end_x, end_y);
        if((rand() % 10) > 5)
            radius += rand() % 25;
        else
            radius -= rand() % 25;
        if(radius < 15)
            radius = 15;
    }
    QPainter painter;
    painter.begin(&image);
    painter.setPen(QColor(Qt::white));
    painter.setBrush(QBrush(Qt::red));
    painter.drawPolygon(points, 360);
    painter.end();

    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(image));
    label->show();
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

#if 0
    emit info(QString("value: %1")
              .arg(Connection::get_double_value()));
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::run_cube_widget(void)
{
    emit trace(Q_FUNC_INFO);

    // CubeWidget *cw = new CubeWidget();
    // cw->show();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::run_ogl_widget(void)
{
    emit trace(Q_FUNC_INFO);

    // ORL_widget *ow = new ORL_widget();
    // ow->show();

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

    //---
    int value;
    if(load_int("index", &value))
    {
        cb_test->setCurrentIndex(value);
    }
    //---

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

    save_int("index", cb_test->currentIndex());

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
