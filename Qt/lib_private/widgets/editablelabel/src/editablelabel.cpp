//--------------------------------------------------------------------------------
#include "editablelabel.hpp"
//--------------------------------------------------------------------------------
EditableLabel::EditableLabel( QWidget *p)
    : QLineEdit( p )
{
    if( p )
    {
        QPalette pal = palette();
        pal.setColor( backgroundRole(), p->palette().color( p->backgroundRole() ) );
        setPalette( pal );
    }
    setFrame( false );
    setFocusPolicy( Qt::ClickFocus );
    connect( this, SIGNAL(textEdited(QString)), this, SLOT(resizeByContents()) );
}
//--------------------------------------------------------------------------------
QSize EditableLabel::sizeHint(void) const
{
    QRect r = fontMetrics().boundingRect( text() );
    int lm, rm, tm, bm;
    getTextMargins( &lm, &tm, &rm, &bm );
    return QSize( r.width() + lm + rm + 5, QLineEdit::sizeHint().height() );
}
//--------------------------------------------------------------------------------
QSize EditableLabel::minimumSizeHint(void) const
{
    return sizeHint();
}
//--------------------------------------------------------------------------------
void EditableLabel::resizeByContents(void)
{
    setFixedWidth( sizeHint().width() );
    if( QWidget* p = parentWidget() )
    {
        p->adjustSize();
    }
}
//--------------------------------------------------------------------------------
