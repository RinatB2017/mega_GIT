#ifndef IMPORTCSVDIALOG_H
#define IMPORTCSVDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QtSql>

namespace Ui {
class ImportCSVDialog;
}

class ImportCSVDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportCSVDialog(QWidget *parent = 0,
                             QSqlDatabase DbToUse = QSqlDatabase(),
                             QString fileName = "");
    ~ImportCSVDialog();

private:
    Ui::ImportCSVDialog *ui;
    QSqlDatabase db;
    QString str_fileName;

private slots:
    void on_buttonBox_rejected(void);
    void on_buttonBox_accepted(void);
    void parseFile(void);
    bool generateSQL(void);
};

#endif // IMPORTCSVDIALOG_H
