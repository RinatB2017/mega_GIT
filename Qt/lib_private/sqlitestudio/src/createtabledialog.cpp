#include <QMessageBox>
#include <QCheckBox>

#include "createtabledialog.h"
#include "ui_createtabledialog.h"

CreateTableDialog::CreateTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTableDialog)
{
    ui->setupUi(this);
    isAlter = false;
}

CreateTableDialog::~CreateTableDialog()
{
    delete ui;
}

void CreateTableDialog::on_cmdAddField_clicked()
{
    AddColumnDialog newColumn;
    if (newColumn.exec()) {
        addField(newColumn.getFieldSpec());
    }
}


void CreateTableDialog::addField(DatabaseTableField f) {
    int rowCount = ui->tblFields->rowCount();
    ui->tblFields->setRowCount(rowCount + 1);
    QTableWidgetItem *nameItem = new QTableWidgetItem();
    nameItem->setText(f.name);
    ui->tblFields->setItem(rowCount, 0, nameItem);
    QTableWidgetItem *typeItem = new QTableWidgetItem();
    typeItem->setText(f.type);
    ui->tblFields->setItem(rowCount, 1, typeItem);
    QTableWidgetItem *defaultItem = new QTableWidgetItem();
    defaultItem->setText(f.defval);
    ui->tblFields->setItem(rowCount, 3, defaultItem);
    QCheckBox *nullCheck = new QCheckBox(this);
    nullCheck->setChecked(f.notnull);
    ui->tblFields->setCellWidget(rowCount, 2, nullCheck);
    ui->tblFields->resizeColumnsToContents();
    ui->tblFields->resizeRowsToContents();
}

void CreateTableDialog::on_cmdDeleteField_clicked()
{
    ui->tblFields->removeRow(ui->tblFields->currentRow());
}

QString CreateTableDialog::getSQL() {    
    if (ui->tblFields->rowCount() < 1) {
        return "";
    }
    gridColumns.clear();
    QString SQL = "CREATE TABLE \"main\".\"" + ui->txtTableName->text() + "\"(";
    for (int i = 0; i < ui->tblFields->rowCount(); ++i) {
        QString flName = ui->tblFields->item(i, 0)->text();
        QString flType = ui->tblFields->item(i, 1)->text();
        QString flDefault = getDefaultClause(ui->tblFields->item(i, 3)->text());
        bool flNotNull = qobject_cast<QCheckBox*>(ui->tblFields->cellWidget(i, 2))->checkState() == Qt::Checked;
        QString notNull(flNotNull ? " NOT NULL " : "");
        SQL += QString("     \"%1\" %2 %3 %4, ").arg(flName, flType, notNull, flDefault);
        gridColumns.insert(gridColumns.count(), flName);
    }
    SQL = SQL.remove(SQL.size() - 2, 2);
    SQL += ");";
    return SQL;
}

QString CreateTableDialog::getDefaultClause(const QString & defVal)
{
    if (defVal.isNull() || defVal.isEmpty())
        return "";
    bool ok;
    defVal.toDouble(&ok);
    if (ok)
        return QString(" DEFAULT (%1)").arg(defVal);
    else if (defVal.simplified().left(1) == "'")
        return QString(" DEFAULT (%1)").arg(defVal);
    else
        return QString(" DEFAULT ('%1')").arg(defVal);
}


void CreateTableDialog::on_buttonBox_accepted()
{
    if (isAlter) {
        db.exec(createTempString);
        if (db.lastError().isValid())
        {
            QMessageBox::critical(this, tr("Error"), tr("Table Error:\n%1").arg(db.lastError().text()));
            return;
        }
        db.exec(insertTempString);
        if (db.lastError().isValid())
        {
            QMessageBox::critical(this, tr("Error"), tr("Table Error:\n%1").arg(db.lastError().text()));
            return;
        }
        db.exec(QString("DROP TABLE `%1`;").arg(ui->txtTableName->text()));
        if (db.lastError().isValid())
        {
            QMessageBox::critical(this, tr("Error"), tr("Table Error:\n%1").arg(db.lastError().text()));
            return;
        }
    }
    QSqlQuery qu(getSQL(), db);
    if (!qu.lastError().isValid()) {
        if (isAlter) {
            QString newColumnList = "";
            for (int i = 0; i < columnList.count(); ++i) {
                QString colValue = columnList.at(i);
                if (gridColumns.contains(colValue)) {
                    newColumnList += colValue + ", ";
                }
            }
            newColumnList = newColumnList.remove(newColumnList.count() - 2, 2);
            QString newInsert = "INSERT INTO `" + ui->txtTableName->text();
            newInsert += "` (";
            newInsert += newColumnList;
            newInsert += ") SELECT ";
            newInsert += newColumnList;
            newInsert += " FROM TEMP_TABLE;";
            db.exec(newInsert);
            if (db.lastError().isValid())
            {
                QMessageBox::critical(this, tr("Error"), tr("Table Error:\n%1").arg(db.lastError().text()));
                return;
            }
            db.exec("DROP TABLE TEMP_TABLE;");
            if (db.lastError().isValid())
            {
                QMessageBox::critical(this, tr("Error"), tr("Table Error:\n%1").arg(db.lastError().text()));
                return;
            }
        }
        this->accept();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Table Error:\n%1").arg(qu.lastError().text()));
        return;
    }
}

void CreateTableDialog::on_buttonBox_rejected()
{
    this->reject();
}

void CreateTableDialog::startAlterTable(QString tableName) {
    QSqlQuery qu(QString("PRAGMA TABLE_INFO (`%1`)").arg(tableName), db);
    columnList.clear();
    ui->txtTableName->setText(tableName);
    ui->txtTableName->setReadOnly(true);
    isAlter = true;
    while (qu.next()) {
        columnList.insert(columnList.count(), qu.record().value("name").toString());
        DatabaseTableField f;
        QSqlRecord rec = qu.record();
        f.cid = rec.value("cid").toInt();
        f.name = rec.value("name").toString();
        f.type = rec.value("type").toString();
        f.pk = rec.value("pk").toBool();
        f.notnull = rec.value("notnull").toBool();
        f.defval = rec.value("dflt value").toString();
        if (f.pk)
            f.type += " PRIMARY KEY";
        f_originalColumnList.insert(f_originalColumnList.count(), f);
        addField(f);
    }
    QString tempCreate = "CREATE TEMPORARY TABLE TEMP_TABLE(";
    QString tempInsert = "INSERT INTO TEMP_TABLE SELECT ";
    originalColumnList = "";
    for (int i  = 0; i < columnList.count(); ++i) {
        tempCreate += columnList.at(i);
        tempCreate += ", ";
        tempInsert += columnList.at(i);
        tempInsert += ", ";
        originalColumnList += columnList.at(i);
        originalColumnList += ", ";
    }
    tempCreate = tempCreate.remove(tempCreate.size() - 2, 2);
    tempInsert = tempInsert.remove(tempInsert.size() - 2, 2);
    originalColumnList = originalColumnList.remove(originalColumnList.size() - 2, 2);
    tempCreate += ");";
    tempInsert += " FROM `" + tableName + "`;";

    createTempString = tempCreate;
    insertTempString = tempInsert;
}
