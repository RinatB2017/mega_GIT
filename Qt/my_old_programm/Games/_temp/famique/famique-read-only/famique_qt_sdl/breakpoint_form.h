#ifndef BREAKPOINT_FORM_H
#define BREAKPOINT_FORM_H

#include <QGroupBox>

namespace Ui {
    class Breakpoint_Form;
}

class Breakpoint_Form : public QGroupBox {
    Q_OBJECT
public:
    Breakpoint_Form(QWidget *parent = 0);
    ~Breakpoint_Form();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Breakpoint_Form *ui;
};

#endif // BREAKPOINT_FORM_H
