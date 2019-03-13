#ifndef ADVANCEDSEARCHDIALOG_H
#define ADVANCEDSEARCHDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QtSql>

namespace Ui {
    class AdvancedSearchDialog;
}

class AdvancedSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdvancedSearchDialog(QWidget *parent = nullptr, QSqlDatabase dbToUse = QSqlDatabase(), QString tableName = "");
    ~AdvancedSearchDialog();
    QSqlDatabase db;
    QString str_tableName;
    QString searchQuery;
    
private:
    Ui::AdvancedSearchDialog *ui;
    QString generateSQL();
    
private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_cmdAddtoSearch_clicked();
};

#endif // ADVANCEDSEARCHDIALOG_H
