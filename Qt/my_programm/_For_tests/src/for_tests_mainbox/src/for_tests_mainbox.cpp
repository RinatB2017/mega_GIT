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
#include <QScrollArea>
#include <QImage>

bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 1
    QImage src_image;
    bool ok = src_image.load(":/other/буддизм.jpg");
    if(!ok)
    {
        emit error("Not load");
        return false;
    }

    int s = 20;
    QImage dest_image(src_image.width() * s, src_image.height(), QImage::Format_ARGB32);
    QPainter p(&dest_image);
    int pos_x = 0;
    for(int n=0; n<s; n++)
    {
        p.drawImage(pos_x, 0, src_image);
        pos_x += src_image.width();
    }
    ok = dest_image.save("/dev/shm/xxx.png");
    if(!ok)
    {
        emit error("Error create image");
    }

    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(dest_image));

    QWidget *w = new QWidget();
    QScrollArea *area = new QScrollArea();
    area->setWidget(label);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(area);

    w->setLayout(hbox);
    w->show();

//    label->show();
#endif

#if 0
    LineWidget *le = new LineWidget();
    le->show();
#endif

    QStringList filters;
    filters << "PNG files (*.png)"
            << "JPG files (*.jpg)"
            << "JPEG files (*.jpeg)";

#if 0
    MyFileDialog *dlg = new MyFileDialog("png", "png");
    dlg->setNameFilters(filters);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        emit info(filename);

        QImage image(filename);

        int w = image.width();
        int h=image.height();
        for(int y=0; y<h; y++)
        {
            for(int x=0; x<w; x++)
            {
                int r, g, b , temp;
                QRgb rgb = image.pixel(x, y);
                QColor color = QColor::fromRgb(rgb);
                color.getRgb(&r, &g, &b);

                temp = r;
                r = g;
                g = temp;

                color.setRgb(r, g, b);

                QRgb new_rgb = color.rgb();
                image.setPixel(x, y, new_rgb);
            }
        }

        QLabel *lbl = new QLabel();
        lbl->setPixmap(QPixmap::fromImage(image));
        lbl->setFixedSize(image.size());
        lbl->show();
    }
    delete dlg;
#endif

#if 0
    MyFileDialog *dlg = new MyFileDialog("png", "png");
    dlg->setNameFilters(filters);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        emit info(filename);

        QImage image(filename);

        int w = image.width();
        int h=image.height();
        for(int y=0; y<h; y++)
        {
            for(int x=0; x<w; x++)
            {
                int r, g, b;
                QRgb rgb = image.pixel(x, y);
                QColor color = QColor::fromRgb(rgb);
                color.getRgb(&r, &g, &b);

                r ^= 55;
                g ^= 55;
                b ^= 55;

                color.setRgb(r, g, b);

                QRgb new_rgb = color.rgb();
                image.setPixel(x, y, new_rgb);
            }
        }

        QLabel *lbl = new QLabel();
        lbl->setPixmap(QPixmap::fromImage(image));
        lbl->setFixedSize(image.size());
        lbl->show();
    }
    delete dlg;
#endif

#if 0
    MyFileDialog *dlg = new MyFileDialog("png", "png");
    dlg->setNameFilters(filters);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        emit info(filename);

        QImage image(filename);

        int w = image.width();
        int h=image.height();
        for(int y=0; y<h; y++)
        {
            for(int x=0; x<w; x++)
            {
                int h, s, v;
                QRgb rgb = image.pixel(x, y);
                QColor color = QColor::fromRgb(rgb);
                color.getHsv(&h, &s, &v);

                int new_v = 100 - v;
                if(new_v < 0) new_v = 0;

                QColor new_color = color.fromHsv(h, s, new_v);
                QRgb new_rgb = new_color.rgb();
                image.setPixel(x, y, new_rgb);
            }
        }

        QLabel *lbl = new QLabel();
        lbl->setPixmap(QPixmap::fromImage(image));
        lbl->setFixedSize(image.size());
        lbl->show();
    }
    delete dlg;
#endif

#if 0
    QWidget *w = new QWidget();
    w->setParent(this);
    w->show();
#endif

#if 0
    FractalWidget *fw = new FractalWidget();
    fw->show();
#endif

#if 0
    TestWidget *tw = new TestWidget();
    tw->show();
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

    QStringList filters;
    filters << "PNG files (*.png)"
            << "JPG files (*.jpg)"
            << "JPEG files (*.jpeg)";

#if 1
    MyFileDialog *dlg = new MyFileDialog("png", "png");
    dlg->setNameFilters(filters);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        emit info(filename);

        QImage image(filename);

        int w = image.width();
        int h=image.height();
        for(int y=0; y<h; y++)
        {
            for(int x=0; x<w; x++)
            {
                int h, s, v;
                QRgb rgb = image.pixel(x, y);
                QColor color = QColor::fromRgb(rgb);
                color.getHsv(&h, &s, &v);

                int new_s = 100 - s;
                if(new_s < 0) new_s = 0;
                QColor new_color = color.fromHsv(h, new_s, v);

                //QColor new_color = color.fromHsv(h, v, s);
                QRgb new_rgb = new_color.rgb();
                image.setPixel(x, y, new_rgb);
            }
        }

        QLabel *lbl = new QLabel();
        lbl->setPixmap(QPixmap::fromImage(image));
        lbl->setFixedSize(image.size());
        lbl->show();
    }
    delete dlg;
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
