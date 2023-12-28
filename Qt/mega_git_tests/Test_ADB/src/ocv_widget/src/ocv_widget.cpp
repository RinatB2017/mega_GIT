/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "ocv_widget.hpp"
#include "ui_ocv_widget.h"
//--------------------------------------------------------------------------------
OCV_widget::OCV_widget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::OCV_widget)
{
    init();
}
//--------------------------------------------------------------------------------
OCV_widget::~OCV_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void OCV_widget::init(void)
{
    ui->setupUi(this);

    connect(ui->btn_load,   &QPushButton::clicked,  this,   &OCV_widget::onLoad);

    ui->lbl_screenshot->installEventFilter(this);

    foreach( QSlider* slider, findChildren< QSlider* >() )
    {
        connect(slider, &QSlider::sliderMoved, this,    &OCV_widget::refreshHSV);
        QString sliderName = slider->objectName();
        // slider->setValue( m_settings.value( sliderName, slider->value() ).toInt() );
        if( QSpinBox* sp = findChild< QSpinBox* >( "sp" + sliderName.mid( 2 ) ) )
        {
            sp->setMinimum( slider->minimum() );
            sp->setMaximum( slider->maximum() );
            sp->setValue( slider->value() );
            connect(sp,     static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    this,   &OCV_widget::refreshHSV);
        }
    }

    foreach( const QRadioButton* rb, findChildren< QRadioButton* >() )
    {
        connect(rb, &QRadioButton::clicked, this, &OCV_widget::refreshHSV);
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
}
//--------------------------------------------------------------------------------
void OCV_widget::onLoad(void)
{
    MyFileDialog *dlg = new MyFileDialog("picture_dlg");
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
void OCV_widget::refreshHSV(void)
{
    emit trace(Q_FUNC_INFO);

    //---
    cv::Mat mat(image.height(),
                image.width(),
                CV_8UC4,
                image.bits(),
                image.bytesPerLine());
    mOrigImage = mat;
    if( !mOrigImage.empty() )
    {
        cv::cvtColor( mOrigImage, mOrigImage, cv::COLOR_BGR2RGB );
    }
    //---

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
                             RETR_TREE,
                             CHAIN_APPROX_SIMPLE,
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
void OCV_widget::get_pixmap(QPixmap pixmap)
{
    ui->lbl_screenshot->setPixmap(pixmap);

    image = pixmap.toImage();
    cv::Mat mat(image.height(),
                image.width(),
                CV_8UC4,
                image.bits(),
                image.bytesPerLine());
    mOrigImage = mat;
}
//--------------------------------------------------------------------------------
bool OCV_widget::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            int pos_x = mouseEvent->pos().x();
            int pos_y = mouseEvent->pos().y();
            emit info(QString("TAP: pos %1 %2").arg(pos_x).arg(pos_y));

            emit f_tap(pos_x, pos_y);
//            bool ok = f_tap(pos_x, pos_y);
//            if(ok)
//            {
//                f_create_screenshot();
//            }
            return true;
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}
//--------------------------------------------------------------------------------
void OCV_widget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool OCV_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void OCV_widget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void OCV_widget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
#if 0
    mOrigImage = cv::imread( filename.toLocal8Bit().data() );
    if( !mOrigImage.empty() )
    {
        cv::cvtColor( mOrigImage, mOrigImage, cv::COLOR_BGR2RGB );
        refreshHSV();
    }
#endif
//--------------------------------------------------------------------------------
