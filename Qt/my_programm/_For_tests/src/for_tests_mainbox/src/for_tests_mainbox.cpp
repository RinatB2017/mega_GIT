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
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 0
    char src[4]  = { 0 };
    char dst[4] = { 0 };
    src[0] = 0;
    src[1] = 1;
    src[2] = 2;
    src[3] = 3;

    char *a_src = src;
    char *a_dst = dst;

    char a = *a_src;
    char b = *(a_src+1);
    char c = *(a_src+2);
    char d = *(a_src+3);

    *a_dst   = d;
    *(a_dst+1) = c;
    *(a_dst+2) = b;
    *(a_dst+3) = a;

    a_src+=4;
    a_dst+=4;

    QByteArray ba;
    ba.append(src, 4);
    emit info(QString("buf = [%1]")
              .arg(ba.toHex().data()));

    QByteArray ba2;
    ba2.append(dst, 4);
    emit info(QString("buf = [%1]")
              .arg(ba2.toHex().data()));
#endif

#if 0
    int w = 400;
    int h = 100;

    QPainterPath m_path;
    m_path.moveTo(0, h / 2);
    m_path.lineTo(w, h);
    m_path.lineTo(w, 0);
    m_path.lineTo(0, h / 2);

    QImage full_image = QImage("/home/boss/HDD/Изображения/bitcoin.png");
    QImage small_image = full_image.copy(0, 0, w, h);

    QImage mask_image = QImage(w, h, QImage::Format_ARGB32);
    QPainter p(&mask_image);
    p.fillRect(0, 0, w, h, QColor(Qt::color1));
    p.fillPath(m_path, QBrush(Qt::color0));

    QImage result_image = QImage(w, h, QImage::Format_ARGB32);
    result_image.fill(QColor(Qt::red));

    uchar *mask_bits  = mask_image.bits();
    uchar *small_bits = small_image.bits();
    uchar *result_bits = result_image.bits();

    int max_n = w*h*4;
    for(int n=0; n<max_n; n++)
    {
        *result_bits++ = *small_bits++ & *mask_bits++;
    }

    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(result_image));
    //label->setPixmap(QPixmap::fromImage(mask_image));
    label->show();
#endif

#if 0
    QElapsedTimer timer;

    QImage image = QImage(1000, 1000, QImage::Format_ARGB32);
    image.fill(QColor(Qt::red));

    QRgb rgb;
    QRgb *p;
    int w = image.width();
    int h = image.height();
    timer.start();
    for(int y=0; y<h; y++)
    {
        for(int x=0; x<w; x++)
        {
            rgb = image.pixel(x, y);
        }
    }
    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
    timer.start();
    for(int y=0; y<h; y++)
    {
        for(int x=0; x<w; x++)
        {
            p = ((QRgb*)image.scanLine (y)) + x;
        }
    }
    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));

    timer.start();
    for(int y=0; y<h; y++)
    {
        for(int x=0; x<w; x++)
        {
            image.setPixel(x, y, 0);
        }
    }
    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
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
#include "show_hsv_color.hpp"
bool MainBox::test2(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test2");

#if 1
    Show_HSV_color* w = new Show_HSV_color();
    w->show();
#endif

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
