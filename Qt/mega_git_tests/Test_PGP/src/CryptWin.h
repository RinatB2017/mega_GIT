#ifndef CRYPTWIN_H
#define CRYPTWIN_H

#include <QMainWindow>

namespace Ui
{
    class CryptWindow;
}

class CryptWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CryptWindow(QWidget * =0L);
    ~CryptWindow();

public slots:
    void encrypt();

private:
    Ui::CryptWindow *ui;
};

#endif // CRYPTWIN_H
