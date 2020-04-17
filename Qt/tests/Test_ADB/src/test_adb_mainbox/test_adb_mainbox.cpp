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
#include "ui_test_adb_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_adb_mainbox.hpp"
#include "myfiledialog.hpp"
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
    save_widgets();

    if(myProcess)
    {
        myProcess->disconnect();
        myProcess->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    s_width = 700;
    s_height = 1000;

    ui->sb_pos_x->setRange(0, s_width);
    ui->sb_pos_y->setRange(0, s_height);

    myProcess = new QProcess(this);
    connect(myProcess,  SIGNAL(started()),                      this,   SLOT(started()));
    connect(myProcess,  SIGNAL(finished(int)),                  this,   SLOT(finished(int)));
    connect(myProcess,  SIGNAL(error(QProcess::ProcessError)),  this,   SLOT(process_error(QProcess::ProcessError)));
    connect(myProcess,  SIGNAL(readyReadStandardOutput()),      this,   SLOT(readData()));
    connect(myProcess,  SIGNAL(readyReadStandardError()),       this,   SLOT(readData()));

    connect(ui->btn_devices,            &QPushButton::clicked,  this,   &MainBox::f_devices);
    connect(ui->btn_create_screenshot,  &QPushButton::clicked,  this,   &MainBox::f_create_screenshot);
    connect(ui->btn_create_screenshot2, &QPushButton::clicked,  this,   &MainBox::f_create_screenshot2);
    connect(ui->btn_tap,                &QPushButton::clicked,  this,   &MainBox::f_screen_tap);

    connect(ui->btn_swipe_LR,           &QPushButton::clicked,  this,   &MainBox::f_test_swipe_LR);
    connect(ui->btn_swipe_RL,           &QPushButton::clicked,  this,   &MainBox::f_test_swipe_RL);
    connect(ui->btn_swipe_UD,           &QPushButton::clicked,  this,   &MainBox::f_test_swipe_UD);
    connect(ui->btn_swipe_DU,           &QPushButton::clicked,  this,   &MainBox::f_test_swipe_DU);

    connect(ui->btn_load,               &QPushButton::clicked,  this,   &MainBox::onLoad);

    ui->lbl_screenshot->installEventFilter(this);

    foreach( QSlider* slider, findChildren< QSlider* >() )
    {
        connect( slider, SIGNAL( sliderMoved( int ) ), SLOT( refreshHSV() ) );
        QString sliderName = slider->objectName();
        // slider->setValue( m_settings.value( sliderName, slider->value() ).toInt() );
        if( QSpinBox* sp = findChild< QSpinBox* >( "sp" + sliderName.mid( 2 ) ) )
        {
            sp->setMinimum( slider->minimum() );
            sp->setMaximum( slider->maximum() );
            sp->setValue( slider->value() );
            connect( sp, SIGNAL( valueChanged( int ) ), SLOT( refreshHSV() ) );
        }
    }

    foreach( const QRadioButton* rb, findChildren< QRadioButton* >() )
    {
        connect( rb, SIGNAL( clicked( bool ) ), SLOT( refreshHSV() ) );
    }

    connect(ui->spHueFrom,          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slHueFrom,          &QSlider::setValue);
    connect(ui->spSaturationFrom,   static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slSaturationFrom,   &QSlider::setValue);
    connect(ui->spValueFrom,        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slValueFrom,        &QSlider::setValue);

    connect(ui->spHueTo,            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slHueTo,            &QSlider::setValue);
    connect(ui->spSaturationTo,     static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slSaturationTo,     &QSlider::setValue);
    connect(ui->spValueTo,          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slValueTo,          &QSlider::setValue);

    connect(ui->slHueFrom,          static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spHueFrom,          &QSpinBox::setValue);
    connect(ui->slSaturationFrom,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spSaturationFrom,   &QSpinBox::setValue);
    connect(ui->slValueFrom,        static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spValueFrom,        &QSpinBox::setValue);

    connect(ui->slHueTo,            static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spHueTo,            &QSpinBox::setValue);
    connect(ui->slSaturationTo,     static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spSaturationTo,     &QSpinBox::setValue);
    connect(ui->slValueTo,          static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spValueTo,          &QSpinBox::setValue);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

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
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::run_program(const QString program,
                          const QStringList arguments)
{
    if(program.isEmpty())
    {
        emit error("program is empty");
        return;
    }

#if 1
    QString temp;
    foreach (QString str, arguments)
    {
        temp.append(str);
        temp.append(" ");
    }
    emit error(temp);
#endif

    myProcess->start(program, arguments);
}
//--------------------------------------------------------------------------------
void MainBox::readData(void)
{
    if(binary_data == false)
    {
        QString output = myProcess->readAllStandardOutput();
        QStringList lines = output.split("\n");
        emit debug(QString("received %1 bytes").arg(output.size()));
        emit debug(QString("lines.size() %1").arg(lines.size()));
        sl_data.clear();
        for(int n=0; n<lines.size(); n++)
        {
            QString line = lines.at(n);
            if(line.contains("List of devices attached") == false)
            {
                QString temp = line.remove("\t").remove("device").trimmed();
                if(temp.isEmpty() == false)
                {
                    sl_data.append(temp);
                }
            }
            emit info(QString("%1").arg(line));
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
    block_interface(true);
    emit info("Процесс начат!");
}
//--------------------------------------------------------------------------------
void MainBox::finished(int result)
{
    block_interface(false);

    process_result = result;
    f_busy = false;

    if(process_result == 0)
    {
        emit info("Процесс завершен!");
    }
    else
    {
        emit error(QString("Процесс завершён с ошибкой (%1)").arg(result));
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
bool MainBox::f_devices(void)
{
    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "devices";
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    if(process_result == 0)
    {
        ui->cb_devices->clear();
        ui->cb_devices->addItems(sl_data);  //TODO надо умнее
    }

    return (process_result == 0);
}
//--------------------------------------------------------------------------------
void MainBox::f_create_screenshot(void)
{
    QString program = PROG_PROCESS;
    QStringList arguments;

    QElapsedTimer timer;
    timer.start();
    if(!f_get_screeshot())      return;
    if(!f_get_file_screeshot()) return;
    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
    f_show_screeshot(PICTURE_NAME);
}
//--------------------------------------------------------------------------------
void MainBox::f_create_screenshot2(void)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return;
    }

    binary_data = true;
    QString filename = PICTURE_NAME;

    QElapsedTimer timer;
    timer.start();
    f_get_screeshot2();

    QByteArray ba = myProcess->readAllStandardOutput();
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        emit error(QString("File %1 not created").arg(filename));
        binary_data = false;
        return;
    }
    file.write(ba);
    file.close();

    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
    f_show_screeshot(filename);

    emit info("OK");
    binary_data = false;
}
//--------------------------------------------------------------------------------
bool MainBox::f_get_screeshot(void)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }

    //PROG_PROCESS shell screencap -p /sdcard/screencap.png;
    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "shell";
    arguments << "screencap";
    arguments << "-p";
    arguments << "/sdcard/screencap.png";

    f_busy = true;
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
bool MainBox::f_get_screeshot2(void)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }

    //PROG_PROCESS exec-out screencap -p
    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "exec-out";
    arguments << "screencap";
    arguments << "-p";

    f_busy = true;
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
bool MainBox::f_get_screeshot3(void)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }

    //    QString program = "./adb_test.sh";
    QString program = "/bin/sh";
    QStringList arguments;

    arguments << "-c";
    //    arguments << "\"";
    //    arguments << "'";

    arguments << "'ls";
    arguments << "*.user";
    arguments << "'";

    //    arguments << PROG_PROCESS;
    //    arguments << "exec-out";
    //    arguments << "screencap";
    //    arguments << "-p";
    //    arguments << ">";
    //    arguments << PICTURE_NAME;

    //    arguments << "\"";
    //    arguments << "'";

    f_busy = true;
    //    run_program(program, arguments);
    myProcess->startDetached(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
bool MainBox::f_get_file_screeshot(void)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }

    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "pull";
    arguments << "/sdcard/screencap.png";

    f_busy = true;
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
void MainBox::f_screen_tap(void)
{
    f_tap(ui->sb_pos_x->value(),
          ui->sb_pos_y->value());
}
//--------------------------------------------------------------------------------
bool MainBox::f_tap(int pos_x, int pos_y)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }
    //PROG_PROCESS shell input tap 100 100

    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "shell";
    arguments << "input";
    arguments << "tap";
    arguments << QString("%1").arg(pos_x);
    arguments << QString("%1").arg(pos_y);

    f_busy = true;
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    if(process_result == 0)
    {
        f_create_screenshot2();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
bool MainBox::f_swipe(int x1, int y1, int x2, int y2, int delay)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }
    //PROG_PROCESS shell input swipe x1 y1 x2 y2 sss

    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "shell";
    arguments << "input";
    arguments << "swipe";
    arguments << QString("%1").arg(x1);
    arguments << QString("%1").arg(y1);
    arguments << QString("%1").arg(x2);
    arguments << QString("%1").arg(y2);
    arguments << QString("%1").arg(delay);

    f_busy = true;
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    if(process_result == 0)
    {
        f_create_screenshot2();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
bool MainBox::f_test_swipe_LR(void)
{
    return f_swipe(10,
                   s_height / 2,
                   s_width / 2,
                   s_height / 2,
                   100);
}
//--------------------------------------------------------------------------------
bool MainBox::f_test_swipe_RL(void)
{
    return f_swipe(s_width / 2,
                   s_height / 2,
                   10,
                   s_height / 2,
                   100);
}
//--------------------------------------------------------------------------------
bool MainBox::f_test_swipe_UD(void)
{
    return f_swipe(s_width / 2,
                   10,
                   s_width / 2,
                   s_height / 2,
                   100);
}
//--------------------------------------------------------------------------------
bool MainBox::f_test_swipe_DU(void)
{
    return f_swipe(s_width / 2,
                   s_height / 2,
                   s_width / 2,
                   10,
                   100);
}
//--------------------------------------------------------------------------------
void MainBox::f_show_screeshot(const QString &filename)
{
    QPixmap pix = QPixmap(filename);

    s_width = pix.width();
    s_height = pix.height();

    ui->sb_pos_x->setRange(0, s_width);
    ui->sb_pos_y->setRange(0, s_height);

    emit info(QString("size %1 %2").arg(s_width).arg(s_height));
    ui->lbl_screenshot->setPixmap(pix);
    ui->lbl_screenshot->setFixedSize(s_width, s_height);

    mOrigImage = cv::imread( filename.toLocal8Bit().data() );
    if( !mOrigImage.empty() )
    {
        cv::cvtColor( mOrigImage, mOrigImage, cv::COLOR_BGR2RGB );
        refreshHSV();
    }
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            int pos_x = mouseEvent->pos().x();
            int pos_y = mouseEvent->pos().y();
            emit info(QString("TAP: pos %1 %2").arg(pos_x).arg(pos_y));

            bool ok = f_tap(pos_x, pos_y);
            if(ok)
            {
                f_create_screenshot2();
            }
            return true;
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}
//--------------------------------------------------------------------------------
void MainBox::onLoad(void)
{
    MyFileDialog *dlg = new MyFileDialog("picture_dlg", "picture_dlg", this);
    dlg->setNameFilter("PNG files (*.png)");
    dlg->selectFile("picture");
    dlg->setDefaultSuffix("png");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        if(filename.isEmpty() == false)
        {
            mOrigImage = cv::imread( filename.toLocal8Bit().data() );
            if( !mOrigImage.empty() )
            {
                cv::cvtColor( mOrigImage, mOrigImage, cv::COLOR_BGR2RGB );
                refreshHSV();
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::refreshHSV()
{
    emit trace(Q_FUNC_INFO);
    if( mOrigImage.empty() )
    {
        emit debug("Image is empty");
        return;
    }

    QImage resultImg;

    if( ui->rbOriginal->isChecked() )
    {
        resultImg = QImage(mOrigImage.data,
                           mOrigImage.cols,
                           mOrigImage.rows,
                           static_cast<int>(mOrigImage.step),
                           QImage::Format_RGB888 ).copy();
    }
    else
    {
        int hueFrom = ui->slHueFrom->value();
        int hueTo = std::max( hueFrom, ui->slHueTo->value() );

        int saturationFrom = ui->slSaturationFrom->value();
        int saturationTo = std::max( saturationFrom, ui->slSaturationTo->value() );

        int valueFrom = ui->slValueFrom->value();
        int valueTo = std::max( valueFrom, ui->slValueTo->value() );

        cv::Mat thresholdedMat;
        cv::cvtColor( mOrigImage, thresholdedMat, cv::COLOR_RGB2HSV );
        // Отфильтровываем только то, что нужно, по диапазону цветов
        cv::inRange(thresholdedMat,
                    cv::Scalar( hueFrom, saturationFrom, valueFrom ),
                    cv::Scalar( hueTo, saturationTo, valueTo ),
                    thresholdedMat
                    );

        // Убираем шум
        cv::erode(thresholdedMat,
                  thresholdedMat,
                  cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( 5, 5 ) )
                  );
        cv::dilate(thresholdedMat,
                   thresholdedMat,
                   cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( 5, 5 ) )
                   );

        // Замыкаем оставшиеся крупные объекты
        cv::dilate(thresholdedMat,
                   thresholdedMat,
                   cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( 5, 5 ) )
                   );
        cv::erode(thresholdedMat,
                  thresholdedMat,
                  cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( 5, 5 ) )
                  );

        if( ui->rbCanny->isChecked() )
        {
            // Визуально выделяем границы
            cv::Canny( thresholdedMat, thresholdedMat, 100, 50, 5 );
        }

        if( ui->rbResult->isChecked() )
        {
            // Находим контуры
            std::vector< std::vector< cv::Point > > countours;
            std::vector< cv::Vec4i > hierarchy;
            cv::findContours(thresholdedMat,
                             countours,
                             hierarchy,
                             CV_RETR_TREE,
                             CV_CHAIN_APPROX_SIMPLE,
                             cv::Point( 0, 0 )
                             );

            std::vector< cv::Rect > rects;
            int cnt = 0;
            for( uint i = 0; i < countours.size(); ++i )
            {
                // Пропускаем внутренние контуры
                if( 0 <= hierarchy[ i ][ 3 ] )
                {
                    continue;
                }
                rects.push_back( cv::boundingRect( countours[ i ] ) );
                // emit info(QString("size %1").arg(countours[i].size()));
                cnt++;
            }
            emit info(QString("cnt %1").arg(cnt));

            resultImg = QImage(mOrigImage.data,
                               mOrigImage.cols,
                               mOrigImage.rows,
                               static_cast<int>(mOrigImage.step),
                               QImage::Format_RGB888
                               ).copy();

            QPainter p;
            p.begin( &resultImg );
            p.setPen( QPen( Qt::green, 2 ) );
            foreach( const cv::Rect& r, rects )
            {
                p.drawRect( r.x, r.y, r.width, r.height );
            }
            p.end();

        }
        else
        {
            resultImg = QImage(thresholdedMat.data,
                               thresholdedMat.cols,
                               thresholdedMat.rows,
                               static_cast<int>(thresholdedMat.step),
                               QImage::Format_Indexed8
                               ).copy();
        }
    }

    ui->lbl_screenshot->setPixmap(QPixmap::fromImage( resultImg )
                                  .scaled(ui->lbl_screenshot->size(),
                                          Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation
                                          )
                                  );
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test()");

    emit info(QString("pixmap: %1 %2")
              .arg(ui->lbl_screenshot->pixmap()->width())
              .arg(ui->lbl_screenshot->pixmap()->height()));
    emit info(QString("s_*: %1 %2")
              .arg(s_width)
              .arg(s_height));

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
