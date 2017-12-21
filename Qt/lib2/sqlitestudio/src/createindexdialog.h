#ifndef CREATEINDEXDIALOG_H
#define CREATEINDEXDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QtSql>

namespace Ui {
    class CreateIndexDialog;
}

class CreateIndexDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateIndexDialog(QWidget *parent = 0);
    ~CreateIndexDialog();
    QSqlDatabase db;
    void setTableName(QString tableName);
signals:
    void emitLog(QString, QString);
private:
    Ui::CreateIndexDialog *ui;
    QString getSQL();
    QString indexTable;
private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // CREATEINDEXDIALOG_H
