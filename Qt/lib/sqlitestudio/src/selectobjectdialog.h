#ifndef SELECTOBJECTDIALOG_H
#define SELECTOBJECTDIALOG_H

#include <QDialog>

namespace Ui {
    class SelectObjectDialog;
}

class SelectObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectObjectDialog(QWidget *parent = 0, QString title = tr("Select Object"), QString label = tr("Select Object"));
    ~SelectObjectDialog();
    void setItems(QStringList items);    
    QString returnValue();
private:
    Ui::SelectObjectDialog *ui;
};

#endif // SELECTOBJECTDIALOG_H
