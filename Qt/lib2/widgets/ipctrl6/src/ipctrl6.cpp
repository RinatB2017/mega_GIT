#include <stdint.h>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QFrame>
#include <QLabel>
#include <QFont>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "ipctrl6.hpp"
//--------------------------------------------------------------------------------
IPCtrl6::IPCtrl6(QWidget *parent) : QFrame(parent)
{
    setFrameShape( QFrame::StyledPanel );
    setFrameShadow( QFrame::Sunken );

    QHBoxLayout* pLayout = new QHBoxLayout( this );
    setLayout( pLayout );
    pLayout->setContentsMargins( 0, 0, 0, 0 );
    pLayout->setSpacing( 0 );

    for ( int i = 0; i != QTUTL_IP_SIZE; ++i )
    {
        if ( i != 0 )
        {
            QLabel* pDot = new QLabel( "-", this );
            //pDot->setStyleSheet( "background: white" );
            pLayout->addWidget( pDot );
            pLayout->setStretch( pLayout->count(), 0 );
        }

        m_pLineEdit[i] = new QLineEdit( this );
        QLineEdit* pEdit = m_pLineEdit[i];
        pEdit->installEventFilter( this );

        pLayout->addWidget( pEdit );
        pLayout->setStretch( pLayout->count(), 1 );

        pEdit->setFrame( false );
        pEdit->setAlignment( Qt::AlignCenter );

        QFont font = pEdit->font();
        font.setStyleHint( QFont::Monospace );
        font.setFixedPitch( true );
        pEdit->setFont( font );

        QRegExp rx ( "^(0|[1-9]|[1-9][0-9]|1[0-9][0-9]|2([0-4][0-9]|5[0-5]))$" );
        QValidator *validator = new QRegExpValidator(rx, pEdit);
        pEdit->setValidator( validator );

    }

    setMaximumWidth( 30 * QTUTL_IP_SIZE );

    connect( this, SIGNAL(signalTextChanged(QLineEdit*)),
             this, SLOT(slotTextChanged(QLineEdit*)),
             Qt::QueuedConnection );
}
//--------------------------------------------------------------------------------
IPCtrl6::~IPCtrl6()
{

}
//--------------------------------------------------------------------------------
void IPCtrl6::slotTextChanged( QLineEdit* pEdit )
{
    for ( unsigned int i = 0; i != QTUTL_IP_SIZE; ++i )
    {
        if ( pEdit == m_pLineEdit[i] )
        {
            if ( ( pEdit->text().size() == MAX_DIGITS &&  pEdit->text().size() == pEdit->cursorPosition() ) || ( pEdit->text() == "0") )
            {
                // auto-move to next item
                if ( i+1 != QTUTL_IP_SIZE )
                {
                    m_pLineEdit[i+1]->setFocus();
                    m_pLineEdit[i+1]->selectAll();
                }
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool IPCtrl6::eventFilter(QObject *obj, QEvent *event)
{
    bool bRes = QFrame::eventFilter(obj, event);

    if ( event->type() == QEvent::KeyPress )
    {
        QKeyEvent* pEvent = dynamic_cast<QKeyEvent*>( event );
        if ( pEvent )
        {
            for ( int i = 0; i != QTUTL_IP_SIZE; ++i )
            {
                QLineEdit* pEdit = m_pLineEdit[i];
                if ( pEdit == obj )
                {
                    switch ( pEvent->key() )
                    {
                    case Qt::Key_Left:
                        if ( pEdit->cursorPosition() == 0 )
                        {
                            // user wants to move to previous item
                            MovePrevLineEdit(i);
                        }
                        break;

                    case Qt::Key_Right:
                        if ( pEdit->text().isEmpty() || (pEdit->text().size() == pEdit->cursorPosition()) )
                        {
                            // user wants to move to next item
                            MoveNextLineEdit(i);
                        }
                        break;

                    case Qt::Key_0:
                        if ( pEdit->text().isEmpty() || pEdit->text() == "0" )
                        {
                            pEdit->setText("0");
                            // user wants to move to next item
                            MoveNextLineEdit(i);
                        }
                        emit signalTextChanged( pEdit );
                        break;

                    case Qt::Key_Backspace:
                        if ( pEdit->text().isEmpty() || pEdit->cursorPosition() == 0)
                        {
                            // user wants to move to previous item
                            MovePrevLineEdit(i);
                        }
                        break;

                    case Qt::Key_Comma:
                    case Qt::Key_Period:
                        MoveNextLineEdit(i);
                        break;

                    default:
                        emit signalTextChanged( pEdit );
                        break;

                    }
                }
            }
        }
    }

    return bRes;
}
//--------------------------------------------------------------------------------
void IPCtrl6::MoveNextLineEdit(int i)
{
    if ( i+1 != QTUTL_IP_SIZE )
    {
        m_pLineEdit[i+1]->setFocus();
        m_pLineEdit[i+1]->setCursorPosition( 0 );
        m_pLineEdit[i+1]->selectAll();
    }
}
//--------------------------------------------------------------------------------
void IPCtrl6::MovePrevLineEdit(int i)
{
    if ( i != 0 )
    {
        m_pLineEdit[i-1]->setFocus();
        m_pLineEdit[i-1]->setCursorPosition( m_pLineEdit[i-1]->text().size() );
        //m_pLineEdit[i-1]->selectAll();
    }
}
//--------------------------------------------------------------------------------
#include <QDebug>
void IPCtrl6::set_url(QString url_string)
{
    QStringList sl = url_string.split('-');
    qDebug() << url_string << QString("sl count = %1").arg(sl.count());
    if(sl.count() == QTUTL_IP_SIZE)
    {
        for(int n=0; n<QTUTL_IP_SIZE; n++)
        {
            m_pLineEdit[n]->setText(QString("%1").arg(sl[n]));
        }
    }
}
//--------------------------------------------------------------------------------
QString IPCtrl6::get_url(void)
{
    return "";
}
//--------------------------------------------------------------------------------
void IPCtrl6::block_interface(bool state)
{
    QList<QLineEdit *> all_obj = findChildren<QLineEdit *>();
    foreach(QLineEdit *obj, all_obj)
    {
        obj->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
