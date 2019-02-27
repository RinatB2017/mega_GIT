#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    sett = new QSettings(qApp->organizationName(), qApp->applicationName());

    queryEditorFontSize = sett->value("QueryEditorFontSize", 9).toInt();
    queryEditorFont = QFont(sett->value("QueryEditorFont").toString());
    ui->cboQueryEditor->setCurrentFont(queryEditorFont);
    ui->spinQueryEditor->setValue(queryEditorFontSize);

    queryBrowserFontSize = sett->value("QueryBrowserFontSize", 9).toInt();
    queryBrowserFont = QFont(sett->value("QueryBrowserFont").toString());
    ui->cboQueryExplorer->setCurrentFont(queryBrowserFont);
    ui->spinQueryBrowser->setValue(queryBrowserFontSize);

    databaseExplorerFontSize = sett->value("DatabaseExplorerFontSize", 9).toInt();
    databaseExplorerFont = QFont(sett->value("DatabaseExplorerFont").toString());
    ui->cboDatabaseBrowser->setCurrentFont(databaseExplorerFont);
    ui->spinDatabaseBrowser->setValue(databaseExplorerFontSize);
}

PreferencesDialog::~PreferencesDialog()
{    
    delete ui;
}

void PreferencesDialog::on_buttonBox_accepted(void)
{
    queryEditorFont = ui->cboQueryEditor->currentFont();
    queryEditorFontSize = ui->spinQueryEditor->value();
    queryBrowserFont = ui->cboQueryExplorer->currentFont();
    queryBrowserFontSize = ui->spinQueryBrowser->value();
    databaseExplorerFont = ui->cboDatabaseBrowser->currentFont();
    databaseExplorerFontSize = ui->spinDatabaseBrowser->value();
    sett->setValue("QueryEditorFont", queryEditorFont);
    sett->setValue("QueryEditorFontSize", queryEditorFontSize);
    sett->setValue("QueryBrowserFont", queryBrowserFont);
    sett->setValue("QueryBrowserFontSize", queryBrowserFontSize);
    sett->setValue("DatabaseExplorerFont", databaseExplorerFont);
    sett->setValue("DatabaseExplorerFontSize", databaseExplorerFontSize);
}
