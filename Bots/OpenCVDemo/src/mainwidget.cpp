//--------------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------------
#include "mainwidget.h"
#include "ui_mainwidget.h"
//--------------------------------------------------------------------------------
#include </usr/include/opencv2/imgproc.hpp>
#include </usr/include/opencv2/opencv.hpp>
//--------------------------------------------------------------------------------
#include <QFileDialog>
#include <QPainter>
//--------------------------------------------------------------------------------
MainWidget::MainWidget( QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::MainWidget )
{
    ui->setupUi( this );

    m_faceClassifier.load( "xml/haarcascade_frontalface_default.xml" );

    connect( ui->bnOpen, SIGNAL( clicked( bool ) ), SLOT( onLoadImage() ) );

    setMinimumSize(800, 600);
}
//--------------------------------------------------------------------------------
MainWidget::~MainWidget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainWidget::onLoadImage()
{
    QString imgPath = QFileDialog::getOpenFileName(this,
                                                   trUtf8( "Открыть" ),
                                                   ".",
                                                   trUtf8( "Изображения (*.jpg *.png *.bmp)" )
                                                   );

    if( imgPath.isEmpty() )
    {
        return;
    }

    cv::Mat img = cv::imread( imgPath.toStdString() );
    cv::Mat gray;
    cv::cvtColor( img, gray, cv::COLOR_BGR2GRAY );
    std::vector< cv::Rect > faces;
    m_faceClassifier.detectMultiScale( gray, faces );

    QPixmap pix( imgPath );
    QPainter painter;
    painter.begin( &pix );
    painter.setPen( Qt::green );
    foreach( const cv::Rect& r, faces )
    {
        painter.drawRect( r.x, r.y, r.width, r.height );
    }
    painter.end();

    ui->lbView->setPixmap( pix.scaled( ui->lbView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
}
//--------------------------------------------------------------------------------
