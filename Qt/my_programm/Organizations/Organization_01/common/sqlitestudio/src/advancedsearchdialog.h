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
    explicit AdvancedSearchDialog(QWidget *parent = 0, QSqlDatabase dbToUse = QSqlDatabase(), QString tableName = "");
    ~AdvancedSearchDialog();
    QSqlDatabase db;
    QString str_tableName;
    QString searchQuery;

private:
    Ui::AdvancedSearchDialog *ui;
    QString generateSQL(void);

private slots:
    void on_buttonBox_rejected(void);
    void on_buttonBox_accepted(void);
    void on_cmdAddtoSearch_clicked(void);
};

#endif // ADVANCEDSEARCHDIALOG_H
