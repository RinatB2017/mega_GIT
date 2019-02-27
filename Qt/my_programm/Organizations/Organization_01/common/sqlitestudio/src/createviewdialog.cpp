#include <QMessageBox>

#include "createviewdialog.h"
#include "ui_createviewdialog.h"

CreateViewDialog::CreateViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateViewDialog)
{
    ui->setupUi(this);
    connect (ui->cmdSave, SIGNAL(clicked()), SLOT(onSave()));
    highlighter =  new Highlighter(ui->txtQuery->document());
}

CreateViewDialog::~CreateViewDialog()
{
    delete ui;
}

QString CreateViewDialog::getSQL(void)
{
    QString ret = "";
    ret = "CREATE VIEW \"" + ui->txtVewName->text() + "\" AS " + ui->txtQuery->toPlainText() + "";
    return ret;
}

void CreateViewDialog::setViewName(QString viewName)
{
    ui->txtVewName->setText(viewName);
    ui->txtVewName->setReadOnly(true);
    isModify = true;
}

void CreateViewDialog::onSave(void)
{
    if (ui->txtVewName->text() == "")
    {
        QMessageBox::warning(this, tr("Enter view name"), tr("Enter a view name first."));
        ui->txtVewName->setFocus();
        return;
    }
    else
    {
        if (isModify)
        {
            db.exec(ui->txtQuery->toPlainText());
            if (!db.lastError().isValid())
            {
                db.exec("DROP VIEW " + ui->txtVewName->text());
            }
            else
            {
                QMessageBox::critical(this, tr("Error"), db.lastError().text());
                return;
            }
        }
        db.exec(getSQL());
        if (!db.lastError().isValid())
        {
            QMessageBox::information(this, tr("Success"), tr("Operation successful"));
            this->accept();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), db.lastError().text());
            return;
        }
    }
}

void CreateViewDialog::on_cmdCancel_clicked(void)
{
    this->reject();
}
