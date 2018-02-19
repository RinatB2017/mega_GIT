#include <QFileDialog>
#include "setupdialog.h"
#include "ui_setupdialog.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
#define STANDART_PATHS_IS_PRESENT
	#include <QStandardPaths>
#endif

SetupDialog::SetupDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SetupDialog)
{
	ui->setupUi(this);
	QString appDataFolder( "."  );

#ifdef STANDART_PATHS_IS_PRESENT
	 appDataFolder = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
#undef STANDART_PATHS_IS_PRESENT
#endif

	ui->pathEdit->setText( appDataFolder + "/database.sqlite" );
}

SetupDialog::~SetupDialog()
{
	delete ui;
}

void SetupDialog::on_browseButton_clicked()
{
	const QString result = QFileDialog::getSaveFileName( this
								, "Choose DB file location"
								, ui->pathEdit->text()
								, "SQLite DB(*.sqlite)" );

	ui->pathEdit->setText( result );
}

QString SetupDialog::selectedFile() const
{
	return ui->pathEdit->text();
}

void SetupDialog::setReasonText(const QString &reason)
{
	ui->reasonLabel->setText( reason );
}

