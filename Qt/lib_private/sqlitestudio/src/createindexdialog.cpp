#include <QMessageBox>

#include "createindexdialog.h"
#include "ui_createindexdialog.h"

CreateIndexDialog::CreateIndexDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateIndexDialog)
{
    ui->setupUi(this);
}

CreateIndexDialog::~CreateIndexDialog()
{
    delete ui;
}

void CreateIndexDialog::setTableName(QString tableName) {
    ui->cboFields->clear();
    QSqlQuery qu(QString("PRAGMA TABLE_INFO (%1);").arg(tableName), db);
    emitLog("", QString("PRAGMA TABLE_INFO (%1);").arg(tableName));
    if (qu.lastError().isValid()){
        QMessageBox::critical(this, tr("Error"), qu.lastError().text());
        return;
    }
    while (qu.next()) {
        ui->cboFields->insertItem(ui->cboFields->count(), qu.record().value("name").toString());
    }
    indexTable = tableName;
}

QString CreateIndexDialog::getSQL() {
    if (ui->txtIndexName->text() == "") {
        QMessageBox::warning(this, tr("Warning"), tr("Please enter the index name."));
        return "";
    }
    QString sql = "";
    //CREATE UNIQUE INDEX testIndex ON Test2(ID ASC);
    QString ascDesc = " ASC);";
    if (ui->cboOrder->currentText() == "Descending")
        ascDesc = " DESC);";
    sql += "CREATE UNIQUE INDEX " + ui->txtIndexName->text() + " ON " + indexTable + "(" +
            ui->cboFields->currentText() + ascDesc;
    return sql;
}

void CreateIndexDialog::on_buttonBox_accepted()
{
    if (getSQL() == "") {
        return;
    }
    db.exec(getSQL());
    emitLog("", getSQL());
    if (db.lastError().isValid()){
        QMessageBox::critical(this, tr("Error"), db.lastError().text());
        return;
    }
    this->accept();
}

void CreateIndexDialog::on_buttonBox_rejected()
{
    this->reject();
}
