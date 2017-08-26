#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <QFileDialog>
#include <QPixmap>
#include <QPainter>

static const char* CONFIG_FILE_NAME = "config.ini";

MainWidget::MainWidget( QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::MainWidget ),
    m_lastLoadPath( "." ),
    m_settings( CONFIG_FILE_NAME, QSettings::IniFormat )
{
    ui->setupUi( this );

    foreach( QSlider* slider, findChildren< QSlider* >() )
    {
        connect( slider, SIGNAL( sliderMoved( int ) ), SLOT( refreshHSV() ) );
        QString sliderName = slider->objectName();
        slider->setValue( m_settings.value( sliderName, slider->value() ).toInt() );
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

    connect( ui->bnLoad, SIGNAL( clicked( bool ) ), SLOT( onLoad() ) );
}

MainWidget::~MainWidget() {
    foreach( const QSlider* slider, findChildren< QSlider* >() )
    {
        QString sliderName = slider->objectName();
        m_settings.setValue( sliderName, slider->value() );
    }

    delete ui;
}

void MainWidget::onLoad() {
    QString imgName = QFileDialog::getOpenFileName( this, "Load image", m_lastLoadPath, "Images (*.jpg *.jpeg *.png *.bmp)" );
    if( imgName.isEmpty() )
    {
        return;
    }

    m_lastLoadPath = QFileInfo( imgName ).absolutePath();

    m_mat = cv::imread( imgName.toStdString() );
    if( !m_mat.empty() )
    {
        cv::cvtColor( m_mat, m_mat, cv::COLOR_BGR2RGB );
    }

    refreshHSV();
}

void MainWidget::refreshHSV() {
    if( m_mat.empty() )
    {
        return;
    }

    QImage resultImg;

    if( ui->rbOriginal->isChecked() )
    {
        resultImg = QImage( m_mat.data, m_mat.cols, m_mat.rows, m_mat.step, QImage::Format_RGB888 ).copy();
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
        cv::cvtColor( m_mat, thresholdedMat, cv::COLOR_RGB2HSV );
        // Отфильтровываем только то, что нужно, по диапазону цветов
        cv::inRange(
                    thresholdedMat,
                    cv::Scalar( hueFrom, saturationFrom, valueFrom ),
                    cv::Scalar( hueTo, saturationTo, valueTo ),
                    thresholdedMat
                    );

        // Убираем шум
        cv::erode(
                    thresholdedMat,
                    thresholdedMat,
                    cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( 5, 5 ) )
                    );
        cv::dilate(
                    thresholdedMat,
                    thresholdedMat,
                    cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( 5, 5 ) )
                    );

        // Замыкаем оставшиеся крупные объекты
        cv::dilate(
                    thresholdedMat,
                    thresholdedMat,
                    cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( 5, 5 ) )
                    );
        cv::erode(
                    thresholdedMat,
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
            cv::findContours(
                        thresholdedMat,
                        countours,
                        hierarchy,
                        CV_RETR_TREE,
                        CV_CHAIN_APPROX_SIMPLE,
                        cv::Point( 0, 0 )
                        );

            std::vector< cv::Rect > rects;
            for( uint i = 0; i < countours.size(); ++i )
            {
                // Пропускаем внутренние контуры
                if( 0 <= hierarchy[ i ][ 3 ] )
                {
                    continue;
                }
                rects.push_back( cv::boundingRect( countours[ i ] ) );
            }

            resultImg = QImage(
                        m_mat.data,
                        m_mat.cols,
                        m_mat.rows,
                        m_mat.step,
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
            resultImg = QImage(
                        thresholdedMat.data,
                        thresholdedMat.cols,
                        thresholdedMat.rows,
                        thresholdedMat.step,
                        QImage::Format_Indexed8
                        ).copy();
        }
    }

    ui->lbView->setPixmap(
                QPixmap::fromImage( resultImg ).scaled(
                    ui->lbView->size(),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                    )
                );
}
