#include <stdint.h>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QKeyEvent>
#include <QFrame>
#include <QLabel>
#include <QFont>
//--------------------------------------------------------------------------------
class IPCtrl : public QFrame
{
    typedef QFrame baseClass;

    Q_OBJECT

public:
    IPCtrl(QWidget *parent);
    ~IPCtrl();

#define QTUTL_IP_SIZE 4

    virtual bool eventFilter( QObject *obj, QEvent *event );

public slots:
    void slotTextChanged( QLineEdit* pEdit );

signals:
    void signalTextChanged( QLineEdit* pEdit );

private:
    QLineEdit *(m_pLineEdit[QTUTL_IP_SIZE]);

    static std::string getIPItemStr( unsigned char item );   
};
//--------------------------------------------------------------------------------
class IPItemValidator : public QIntValidator 
{
public:
    IPItemValidator( QObject* parent ) : QIntValidator( parent )
    {
        setRange( 0, UCHAR_MAX );
    }
    ~IPItemValidator() {}

    virtual void fixup( QString & input ) const
    {
        if ( input.isEmpty() )
            input = "0";
    }
};
//--------------------------------------------------------------------------------
