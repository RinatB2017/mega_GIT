#ifndef CREATEVIEWDIALOG_H
#define CREATEVIEWDIALOG_H

#include <QtGui>
#include <QDialog>
#include <QtSql>
#include "highlighter.h"

namespace Ui {
    class CreateViewDialog;
}

class CreateViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateViewDialog(QWidget *parent = 0);
    ~CreateViewDialog();
    QSqlDatabase db;
    void setViewName(QString viewName);
private:
    Ui::CreateViewDialog *ui;
    QString getSQL(void);
    Highlighter *highlighter;
    bool isModify;

private slots:
    void on_cmdCancel_clicked(void);
    void onSave(void);
};

#endif // CREATEVIEWDIALOG_H
