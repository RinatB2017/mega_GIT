#ifndef DEBUGOUTPUT_H
#define DEBUGOUTPUT_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
    class DebugOutput;
}

class DebugOutput : public QDialog {
    Q_OBJECT
public:
    DebugOutput(QWidget *parent = 0);
    ~DebugOutput();

    void Output(QString str);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DebugOutput *ui;
    QPlainTextEdit *ptr_to_txtfield;
};

#endif // DEBUGOUTPUT_H
