#ifndef CREATETABLEDIALOG_H
#define CREATETABLEDIALOG_H

#include <QDialog>
#include <QtSql>
#include <QtGui>
#include "addcolumndialog.h"
#include "database.h"

namespace Ui {
    class CreateTableDialog;
}

class CreateTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateTableDialog(QWidget *parent = 0);
    ~CreateTableDialog();
    QSqlDatabase db;
    void startAlterTable(QString tableName);

private:
    Ui::CreateTableDialog *ui;
    void addField(DatabaseTableField f);
    QString getSQL(void);
    QString getDefaultClause(const QString & defVal);
    bool isAlter;
    QString createTempString;
    QString insertTempString;
    QString originalColumnList;
    FieldList f_originalColumnList;
    QStringList columnList;
    QStringList gridColumns;

private slots:
    void on_buttonBox_rejected(void);
    void on_buttonBox_accepted(void);
    void on_cmdDeleteField_clicked(void);
    void on_cmdAddField_clicked(void);

};

#endif // CREATETABLEDIALOG_H
