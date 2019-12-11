#include <stdint.h>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QKeyEvent>
#include <QFrame>
#include <QLabel>
#include <QFont>
//--------------------------------------------------------------------------------
class IPCtrl6 : public QFrame
{
    Q_OBJECT

public:
    IPCtrl6(QWidget *parent = nullptr);
    ~IPCtrl6();

    virtual bool eventFilter( QObject *obj, QEvent *event );

public slots:
    void slotTextChanged( QLineEdit* pEdit );
    void set_url(QString url_string);
    QString get_url(void);
    void block_interface(bool state);

signals:
    void signalTextChanged( QLineEdit* pEdit );

private:
    enum
    {
        QTUTL_IP_SIZE   = 6,    // число октетов IP адресе
        MAX_DIGITS      = 3     // число символов в LineEdit
    };

    QLineEdit *m_pLineEdit[QTUTL_IP_SIZE];
    void MoveNextLineEdit (int i);
    void MovePrevLineEdit (int i);
};
//--------------------------------------------------------------------------------
