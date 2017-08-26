#ifndef WEBGET_H
#define WEBGET_H

#include <QDialog>

namespace Ui {
class webget;
}

class webget : public QDialog
{
    Q_OBJECT

public:
    explicit webget(QWidget *parent = 0);
    ~webget();

private:
    Ui::webget *ui;
};

#endif // WEBGET_H
