#include <QMessageBox>

#include "advancedsearchdialog.h"
#include "ui_advancedsearchdialog.h"

AdvancedSearchDialog::AdvancedSearchDialog(QWidget *parent, QSqlDatabase dbToUse, QString tableName) :
    QDialog(parent),
    ui(new Ui::AdvancedSearchDialog)
{
    ui->setupUi(this);
    db = dbToUse;
    str_tableName = tableName;
    QSqlQuery qu("PRAGMA TABLE_INFO(" + tableName + ")", db);
    if (qu.lastError().isValid()) {
        QMessageBox::critical(parent, tr("Search Error"), qu.lastError().text());
        return;
    }
    while (qu.next()) {
        ui->cboColumns->insertItem(ui->cboColumns->count() , qu.record().value("name").toString());
    }
}

AdvancedSearchDialog::~AdvancedSearchDialog()
{
    delete ui;
}

void AdvancedSearchDialog::on_cmdAddtoSearch_clicked()
{
    QTableWidgetItem *colItem = new QTableWidgetItem(ui->cboColumns->currentText(), 1);
    QTableWidgetItem *criteriaItem = new QTableWidgetItem(ui->cboCriteria->currentText(), 1);
    QTableWidgetItem *searchItem = new QTableWidgetItem(ui->txtSearch->text(), 1);
    int count = ui->tblSearchTerms->rowCount();
    ui->tblSearchTerms->setRowCount(count + 1);
    ui->tblSearchTerms->setItem(count, 0, colItem);
    ui->tblSearchTerms->setItem(count, 1, criteriaItem);
    ui->tblSearchTerms->setItem(count, 2, searchItem);
    ui->tblSearchTerms->resizeRowsToContents();
}

QString AdvancedSearchDialog::generateSQL() {
    if (ui->tblSearchTerms->rowCount() < 1)
        return "";
    QString sql = "SELECT * FROM `" + str_tableName + "` WHERE ";
    for (int i = 0; i < ui->tblSearchTerms->rowCount(); i++)     {
        QString columnDesc = "`";
        columnDesc += ui->tblSearchTerms->item(i, 0)->text() + "` ";
        if (ui->tblSearchTerms->item(i, 1)->text() == "Contains") {
            columnDesc += " LIKE '\%" + ui->tblSearchTerms->item(i, 2)->text() + "\%'";
        } else {
            columnDesc += ui->tblSearchTerms->item(i, 1)->text() + " '";
            columnDesc += ui->tblSearchTerms->item(i, 2)->text() + "'";
        }
        columnDesc.append(" AND ");
        sql.append(columnDesc);
    }
    sql = sql.remove(sql.size() - 5, 5);
    return sql;
}

void AdvancedSearchDialog::on_buttonBox_accepted()
{
    searchQuery = generateSQL();
    this->accept();
}

void AdvancedSearchDialog::on_buttonBox_rejected()
{
    this->reject();
}
