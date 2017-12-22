#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QFile>

MainWidget::MainWidget( QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::MainWidget ) {
    ui->setupUi( this );

    connect( ui->bnRun, SIGNAL( clicked( bool ) ), SLOT( runScript() ) );
    foreach( QRadioButton* rb, findChildren< QRadioButton* >() ) {
        connect( rb, SIGNAL( toggled( bool ) ), SLOT( loadScript() ) );
    }

    QScriptValue inputVal = m_engine.newQObject( ui->edInput );
    m_engine.globalObject().setProperty( "input", inputVal );

    QScriptValue outputVal = m_engine.newQObject( ui->edOutput );
    m_engine.globalObject().setProperty( "output", outputVal );

    loadScript();
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::loadScript() {
    QString fileName = "";
    if( ui->rbUpperDemo->isChecked() ) {
        fileName = "upper_demo.js";
    } else if( ui->rbConnectDemo->isChecked() ) {
        fileName = "connect_demo.js";
    } else if( ui->rbConnectFuncDemo->isChecked() ) {
        fileName = "connect_func_demo.js";
    }

    QFile f( ":/" + fileName );
    if( f.open( QIODevice::ReadOnly ) ) {
        ui->txtScript->setText( f.readAll() );
    }
}

void MainWidget::runScript() {
    ui->edInput->disconnect();
    ui->edOutput->disconnect();
    ui->edOutput->clear();

    QScriptValue result = m_engine.evaluate( ui->txtScript->toPlainText() );
    if( result.isError() ) {
        ui->edOutput->setText( result.toString() );
    }
}
