#include <QInputDialog>

#include "addcolumndialog.h"
#include "ui_addcolumndialog.h"

AddColumnDialog::AddColumnDialog(QWidget *parent,
                                 QString fieldName,
                                 QString dataType) :
    QDialog(parent),
    ui(new Ui::AddColumnDialog)
{
    ui->setupUi(this);
    ui->txtFieldName->setText(fieldName);
    ui->cboDataType->setEditText(dataType);
}

AddColumnDialog::~AddColumnDialog()
{
    delete ui;
}

void AddColumnDialog::on_cmdSetDataType_clicked(void)
{
    QString customType = QInputDialog::getText(this, tr("Get Type"), tr("Enter the custom type"));
    if (!customType.isEmpty())
        ui->cboDataType->setEditText(customType);
}

DatabaseTableField AddColumnDialog::getFieldSpec(void)
{
    DatabaseTableField f;
    f.comment = ui->txtComment->text();
    f.defval = ui->txtDefault->text();
    f.name = ui->txtFieldName->text();
    f.notnull = ui->chkNotNull->isChecked();
    f.pk = ui->cboDataType->currentText().toLower().contains("primary");
    f.type = ui->cboDataType->currentText();
    return f;
}

void AddColumnDialog::setField(DatabaseTableField f)
{
    ui->txtComment->setText(f.comment);
    ui->txtDefault->setText(f.defval);
    ui->txtFieldName->setText(f.name);
    ui->chkNotNull->setChecked(f.notnull);
    ui->cboDataType->setEditText(f.type);
}
