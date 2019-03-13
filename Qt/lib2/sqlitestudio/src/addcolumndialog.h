#ifndef ADDCOLUMNDIALOG_H
#define ADDCOLUMNDIALOG_H

#include <QDialog>
#include <QtGui>
#include "database.h"
namespace Ui {
    class AddColumnDialog;
}

class AddColumnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddColumnDialog(QWidget *parent = nullptr, QString fieldName = "", QString dataType = "Text");
    ~AddColumnDialog();
    QString fieldName;
    QString dataType;
    DatabaseTableField getFieldSpec();
    void setField(DatabaseTableField f);
    
private:
    Ui::AddColumnDialog *ui;

private slots:
    void on_cmdSetDataType_clicked();
};

#endif // ADDCOLUMNDIALOG_H
