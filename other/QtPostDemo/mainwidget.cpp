#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>

#include <QSettings>
#include <QMessageBox>

static const QString SETTINGS_FILE_NAME = "config.ini";

MainWidget::MainWidget( QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::MainWidget ),
    m_manager( new QNetworkAccessManager( this ) ),
    m_settings( SETTINGS_FILE_NAME, QSettings::IniFormat )
{
    ui->setupUi( this );

    ui->edURL->setText( m_settings.value( "endpoint_url", "" ).toString() );
    ui->edKey->setText( m_settings.value( "key", "" ).toString() );
    ui->edFrom->setText( m_settings.value( "from", "" ).toString() );
    ui->edTo->setText( m_settings.value( "to", "" ).toString() );

    connect( ui->bnSend, SIGNAL( clicked( bool ) ), SLOT( onSend() ) );

    connect( m_manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( onResponse( QNetworkReply* ) ) );
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::onSend()
{
    QUrl url( ui->edURL->text() );
    QNetworkRequest request( url );
    QString auth = QString( "%1:%2" ).arg( "api" ).arg( ui->edKey->text() );
    request.setRawHeader( "Authorization", "Basic " + auth.toLatin1().toBase64() );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

//    QUrl params;
//    params.addQueryItem( "from", ui->edFrom->text() );
//    params.addQueryItem( "to", ui->edTo->text() );
//    params.addQueryItem( "subject", ui->edSubject->text() );
//    params.addQueryItem( "text", ui->txtMessage->toPlainText() );

    QUrlQuery params;
    params.addQueryItem( "from", ui->edFrom->text() );
    params.addQueryItem( "to", ui->edTo->text() );
    params.addQueryItem( "subject", ui->edSubject->text() );
    params.addQueryItem( "text", ui->txtMessage->toPlainText() );

    m_settings.setValue( "endpoint_url", ui->edURL->text() );
    m_settings.setValue( "key", ui->edKey->text() );
    m_settings.setValue( "from", ui->edFrom->text() );
    m_settings.setValue( "to", ui->edTo->text() );

//    m_manager->post( request, params.encodedQuery() );
    m_manager->post( request, params.query().toLatin1() );
}

void MainWidget::onResponse( QNetworkReply* reply )
{
    QMessageBox::information(
        this,
        "Ответ сервера",
        reply->readAll(),
        QMessageBox::Ok
    );
}
