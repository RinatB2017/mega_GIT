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
    explicit CreateViewDialog(QWidget *parent = nullptr);
    ~CreateViewDialog();
    QSqlDatabase db;
    void setViewName(QString viewName);
    
private:
    Ui::CreateViewDialog *ui;
    QString getSQL();
    Highlighter *highlighter;
    bool isModify;

private slots:
    void on_cmdCancel_clicked();
    void onSave();
};

#endif // CREATEVIEWDIALOG_H
