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
#include "ui_for_tests_mainbox.h"
//--------------------------------------------------------------------------------
#include <algorithm>    // std::find_if
#include <vector>       // std::vector

#include "for_tests_mainbox.hpp"
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "myfiledialog.hpp"
#include "defines.hpp"
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

    createTestBar();

    connect_log_signals(ui->controls_widget, this);

    // add_lcd_clock();
    // add_digital_clock(false);
    add_digital_clock();

#if 1
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif

#ifdef USE_DOCK_WIDGETS
    QTimer::singleShot(0, [this]{
        MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
        if(mw)
        {
            mw->add_mdi_sorting();
            mw->add_dock_widget("test_dock",
                                "test_dock",
                                Qt::BottomDockWidgetArea,
                                ui->test_frame);
        }
        load_widgets();
    });
#endif
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
    commands.append({ id++, "load QSS",             &MainBox::load_qss });
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

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
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
void MainBox::print_mp(QWidget *widget)
{
    emit error(QString("objectName %1").arg(widget->objectName()));

    const QMetaObject* metaObject = widget->metaObject();
    emit info(QString("propertyCount  = %1").arg(metaObject->propertyCount()));
    emit info(QString("propertyOffset = %1").arg(metaObject->propertyOffset()));
    emit error("---");
    for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
    {
        emit error(QString("%1").arg(QString::fromLatin1(metaObject->property(i).name())));
    }
    emit error("---");
    emit info(QString("methodCount    = %1").arg(metaObject->methodCount()));
    emit info(QString("methodOffset   = %1").arg(metaObject->methodOffset()));
    emit error("---");
    for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
    {
        emit error(QString("%1").arg(QString::fromLatin1(metaObject->method(i).methodSignature())));
    }
    emit error("---");
}
//--------------------------------------------------------------------------------
bool MainBox::load_qss(void)
{
    emit trace(Q_FUNC_INFO);

    MyFileDialog *dlg = new MyFileDialog("qss_file", "qss_file");
    dlg->setNameFilter("QSS files (*.qss)");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();

        QFile file(files.at(0));
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        file.close();
        qApp->setStyleSheet(styleSheet);
    }
    dlg->deleteLater();

    return true;
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
//#include "qyuvopenglwidget.h"
//#include <QUiLoader>

#include "grapherbox.hpp"

bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 1
    G000();
    M000();
#endif

#if 0
    int s = 300;
    QPixmap *pixmap = new QPixmap(s, s);
    QPainter p;
    p.begin(pixmap);
    p.fillRect(0,0, s, s, Qt::black);

    qreal xxx = 200.0;
    qreal center_x = xxx;
    qreal center_y = xxx;
    QColor color;
    qreal v = 0;
    for(qreal r=0; r<xxx; r+=0.1)
    {
        color.setHsv(287, 200, v);;
        v+=0.12;
        p.setPen(QPen(color));
        p.drawEllipse(center_x - r,
                      center_y - r,
                      r,
                      r);
    }
    p.end();

    QLabel *label = new QLabel();
    label->setPixmap(*pixmap);
    label->show();
#endif

#if 0
    int s = 32;

    QPixmap *pixmap = new QPixmap(s, s);
    QPainter p;
    p.begin(pixmap);
    p.setBrush(QBrush(Qt::red));
    p.drawEllipse(0, 0, s, s);
    p.end();

    QIcon icon(*pixmap);

    QToolButton *btn = new QToolButton();
    btn->setMinimumSize(s, s);
    btn->setIcon(icon);
    btn->setCheckable(true);
    btn->show();
#endif

#if 0
    MyFileDialog *dlg = new MyFileDialog("MainBox", "MainBox", this);
    dlg->setNameFilter("PNG files (*.png)");
    dlg->setDefaultSuffix("png");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        QPixmap *pixmap = new QPixmap();
        bool ok = pixmap->load(filename);
        if(ok)
        {
            QLabel *label = new QLabel();
            label->setPixmap(*pixmap);
            label->show();

            int buf[360] = { 0 };

            QImage image;
            image = pixmap->toImage();
            int w = image.width();
            int h = image.height();
            for(int y=0; y<h; y++)
            {
                for(int x=0; x<w; x++)
                {
                    QColor color = image.pixelColor(x, y);
                    int h_v;
                    int s_v;
                    int v_v;
                    color.getHsv(&h_v, &s_v, &v_v);
                    buf[h_v]++;
                }
            }
            GrapherBox *gb = new GrapherBox();
            int curve = gb->add_curve("Hue");
            for(int n=0;n<360; n++)
            {
                gb->add_curve_data(curve, n, buf[n]);
            }
            gb->setMinimumSize(1024, 300);
            gb->push_btn_Horizontal(true);
            gb->push_btn_Vertical(true);
            gb->show();
        }
    }
    delete dlg;
#endif

#if 0
    lock_this_button();
    QElapsedTimer *timer = new QElapsedTimer();
    timer->start();
    while(timer->elapsed() < 1000)
    {

    }
    emit info("OK");
    unlock_this_button();
#endif

#if 0
    MyFileDialog *dlg = new MyFileDialog("MainBox", "MainBox", this);
    dlg->setNameFilter("UI files (*.ui)");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        QUiLoader loader;
        QFile file(filename);
        file.open(QFile::ReadOnly);
        QWidget *formWidget = loader.load(&file);
        formWidget->show();
        file.close();
    }
    delete dlg;
#endif

#if 0
    qInfo()    << "qInfo";
    qDebug()   << "qDebug";
    qWarning() << "qWarning";
#endif

#if 0
    TestClass tc;
    tc.test();
#endif

#if 0
    test_function<MainBox>(1000);
#endif

#if 0
    test_template<QToolButton, QDoubleSpinBox>();
#endif

#if 0
    // AVFrame *frame = new AVFrame();

    QYUVOpenGLWidget *w = new QYUVOpenGLWidget();
    // w->updateTextures(frame->data[0], frame->data[1], frame->data[2], frame->linesize[0], frame->linesize[1], frame->linesize[2]);
    w->show();
#endif

#if 0
    emit info(QString("ver. [%1]").arg(qApp->applicationVersion()));
#endif

#if 0
    emit info("Copyright \\251 2020-2025");
#endif

#if 0
    emit info("Info");
    emit debug("Debug");
    emit error("Error");
    emit trace("Trace");
    //emit colorLog("yellow on blue", Qt::yellow, Qt::blue);
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test2(void)
{
#if 1
    int size = 5;
    QPixmap *pixmap = new QPixmap(size,
                                  size);
    pixmap->fill(Qt::transparent);
    QPainter p;
    p.begin(pixmap);
    p.setPen(QPen(Qt::white));
    p.setBrush(QBrush(Qt::blue));
    p.drawEllipse(0, 0, pixmap->width() - 1, pixmap->height() - 1);
    p.drawRect(0, 0, pixmap->width() - 1, pixmap->height() - 1);
    p.end();
    QCursor cursor(*pixmap);

    MainBox *mainBox = topLevelWidget()->findChild<MainBox *>();
    if(mainBox)
    {
        mainBox->setCursor(cursor);
    }
#endif

#if 0
    const QWidgetList allWidgets = QApplication::allWidgets();
    for (QWidget *widget : allWidgets)
    {
        MyWidget *mw = reinterpret_cast<MyWidget *>(widget);
        if(mw)
        {
            if(widget->objectName().isEmpty())
            {
                emit info("objectName is empty");
            }
        }
    }
#endif
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f1(int value)
{
    return (value > 0);
}
//--------------------------------------------------------------------------------
bool MainBox::f2(int value)
{
    return (value > 0);
}
//--------------------------------------------------------------------------------
bool MainBox::f3(int value)
{
    return (value > 0);
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
#ifdef QT_DEBUG
    qDebug() << "MainBox::load_setting";
#endif

    if(sb_test)
    {
        int value = 0;
        load_int("sb_test", &value);
        sb_test->setValue(value);
    }
    if(cb_block)
    {
        int value = 0;
        load_int("cb_block", &value);
        bool block_is_checked = value;
        cb_block->clicked(block_is_checked);
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
