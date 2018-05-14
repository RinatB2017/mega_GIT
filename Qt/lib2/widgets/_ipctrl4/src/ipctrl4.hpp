#include <stdint.h>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QKeyEvent>
#include <QFrame>
#include <QLabel>
#include <QFont>
//--------------------------------------------------------------------------------
class IPCtrl4 : public QFrame
{
    Q_OBJECT

public:
    IPCtrl4(QWidget *parent = 0);
    ~IPCtrl4();

    QString get_host(void);
    virtual bool eventFilter( QObject *obj, QEvent *event );

public slots:
    void slotTextChanged( QLineEdit* pEdit );
    void set_url(QUrl url);

signals:
    void signalTextChanged( QLineEdit* pEdit );

private:
    enum
    {
        QTUTL_IP_SIZE   = 4,    // число октетов IP адресе
        MAX_DIGITS      = 3     // число символов в LineEdit
    };

    QLineEdit *(m_pLineEdit[QTUTL_IP_SIZE]);
    void MoveNextLineEdit (int i);
    void MovePrevLineEdit (int i);
};
//--------------------------------------------------------------------------------
